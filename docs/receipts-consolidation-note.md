# Runtime receipts 合并设计说明

## 1. 目标、角色和“不是什么”

本文是 mini-kv v1625 的 receipts 合并设计闸门。它解决的不是新业务需求，而是一个已经可以量化的维护问题：`src/runtime_*_receipts.cpp` 已形成大批结构相近、字段不同、输出顺序敏感的手写 JSON formatter。继续逐文件复制会让修复边界字段、调整共同元数据或增加测试保护时需要同时理解几十个长文件，也会增加漏字段、字段顺序漂移和 digest 输入错位的概率。

本版只冻结必要性证明、共享 formatter 设计、字节一致性策略和后续切片顺序。它不是新的 runtime command，不新增 router，不修改 `SMOKEJSON`、`INFOJSON`、fixture 或 manifest 输出，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，不写 Store/WAL，不读取 credential value，不解析 raw endpoint URL，不打开 HTTP/TCP 或 managed audit connection，也不启动 Node、Java 或 mini-kv 服务。

## 2. Necessary proof：为什么需要合并

v1625 开始时的源代码库存如下：

| 范围 | 文件数 | 字节数 | 说明 |
| --- | ---: | ---: | --- |
| `src/runtime_*_receipts.cpp` | 28 | 788,811 | runtime receipt 全家族，约 770.3 KiB |
| `src/runtime_credential_resolver_receipts.cpp` | 1 | 34,959 | credential resolver 基础 receipt |
| `src/runtime_credential_resolver_*_receipts.cpp` | 25 | 671,207 | credential resolver 专题 receipt |
| credential resolver 合计 | 26 | 706,166 | 当前最密集、最适合先治理的子家族 |

25 个专题文件中，18 个已经复用 `runtime_credential_resolver_receipt_utils.hpp`，7 个仍在本文件重复 `DigestPart`、`field_string` 或 `receipt_digest` 包装。专题文件合计包含约 180 个 `format_*` helper；17 个文件有独立 `format_checks_json`，20 个文件有 `closed_boundary_flags` 或 `non_participation_flags` 一类关闭边界块。这些数字说明重复不是“名字看起来相似”，而是大量文件持续重复以下骨架：

1. 固定的 receipt version、fixture path、release version、artifact path、consumer 和 runtime role。
2. Node 来源版本、profile、digest、check count 和下一步消费方。
3. `required_fields`、`prohibited_fields`、`warnings`、`recommendations`、`checks` 等数组或对象格式化。
4. digest 输入按固定顺序拼成 `std::vector<DigestPart>`。
5. 顶层 JSON 按固定顺序串接共同元数据、来源证据、summary、fixture 指针和关闭边界。
6. `read_only=true`、`execution_allowed=false`、no credential、no raw endpoint、no network、no write、no auto-start、no authority 等共同边界。

维护成本来自“共同骨架必须一致，但每份 receipt 又有少量领域差异”。完全不抽象会继续复制；一次性把所有字段塞进万能 builder 又会制造更难读的巨型配置。正确方向是只抽共同拼装机制，保留每份 receipt 的领域数据和领域 section。

## 3. 当前可复用基础与缺口

现有 `src/runtime_credential_resolver_receipt_utils.hpp` 已经提供四个基础能力：

- `field_string`：统一 JSON string escaping。
- `receipt_digest`：统一调用 runtime evidence digest。
- `format_common_credential_boundary_json`：输出共同的关闭边界字段。
- `format_no_go_conditions_json`：输出一组历史 no-go code。

这证明项目已经开始去重，但仍有两个缺口。

第一，helper 只解决“值怎么编码”，没有解决“字段如何按原顺序组成对象”。每个文件仍手写数百行字符串相加，一旦漏逗号、改顺序或把 raw JSON 当普通字符串转义，公共输出就会改变。

第二，现有 receipt 测试很丰富，但大多使用 `assert_contains` 验证字段、digest 和关闭边界；它们能保护语义，却不能单独证明完整字符串逐字节一致。`fixtures/release/*.json` 才是已经提交、被 manifest/SMOKEJSON 和下游引用的字节基准。后续迁移不能把“现有测试很多”误写成“已经有完整 byte parity”。

## 4. Shared formatter design

### 4.1 设计原则

共享 formatter 只负责确定性拼装，不负责理解业务，不负责推导权限，也不负责自动补字段。建议新增内部模块：

```text
src/runtime_receipt_json_builder.hpp
src/runtime_receipt_json_builder.cpp
```

它不进入 public include，不新增外部依赖，也不引入通用 JSON 库。核心模型保持很小：

```cpp
struct OrderedJsonField {
    std::string_view name;
    std::string value_json;
};

std::string json_object(std::span<const OrderedJsonField> fields);
std::string json_array(std::span<const std::string> values_json);
```

`value_json` 明确表示“已经编码好的 JSON value”，而不是普通字符串。普通文本必须先走 `runtime_evidence::json_string`，布尔和数字由专门 helper 产生，嵌套对象和数组由 builder 产生。这样可以同时守住三件事：

- 字段顺序由输入 span 的顺序决定，不使用会重排 key 的 map。
- 不自动省略 false、0、空数组或空对象，避免输出缩水。
- 不自动格式化空白，不添加换行，不改变当前 compact JSON。

### 4.2 每份 receipt 保留什么

每份领域文件继续拥有：

- receipt/version/fixture/source/digest 常量。
- 领域专用 struct 或 constexpr table。
- required/prohibited/check/warning/recommendation 的领域数据。
- digest prefix 和 digest parts 的明确顺序。
- public formatter 和 digest 函数名。

共享模块只接收已经明确排序的数据并拼装 JSON。它不能根据字段名猜测 `read_only`，不能自动把所有 credential receipt 都标成同一种模式，也不能把不同 Node 版本的来源结构压成一个长布尔参数函数。

### 4.3 不采用的方案

- 不引入第三方 JSON 依赖：mini-kv 的零外部依赖仍是项目特性。
- 不使用 `std::map` 或 unordered container 生成对象：它们会破坏历史字段顺序。
- 不一次性建立包含上百个 optional 字段的 `ReceiptSpec`：这只是把 26 个长文件转成一个巨型 schema。
- 不先合并文件再补测试：parity harness 必须先于第一刀迁移。
- 不修改 fixture 来迎合新 formatter：fixture 是 oracle，不是迁移产物。

## 5. Byte-identical output contract

后续每个迁移切片都必须满足下面的字节一致性合同：

1. public formatter 名称、签名和调用点不变。
2. `fixtures/release/*.json` 不修改、不移动、不重命名。
3. 字段名称、字段顺序、数组顺序、布尔值、数字、转义、逗号和 compact whitespace 全部不变。
4. receipt digest 不变，digest parts 的内容和顺序不变。
5. `SMOKEJSON`、runtime smoke fixture、verification manifest 中嵌入的 receipt 不变。
6. 现有测试 expectation 不允许为了迁移而改写。

第一刀代码迁移之前，先新增 fixture parity harness。建议由测试 helper 从 standalone fixture 中按目标字段名定位嵌套 receipt，使用字符串状态机处理引号、反斜线和花括号深度，提取完整 JSON object，然后与 public formatter 的返回值做 `==` 比较。这里不需要把 JSON 解析成对象，因为结构化解析后重新序列化可能改变字段顺序；本测试要保护的恰恰是原始字节。

如果某份历史 fixture 的嵌套对象无法可靠提取，允许退回“迁移前 formatter 输出 SHA-256/FNV + 长度冻结”的临时 parity，但必须在同一版本说明原因。不能只依赖 `assert_contains`。

## 6. 分批迁移路线

### Slice 0：parity harness 和 builder 基础

只新增内部 ordered builder、builder 单元测试和 fixture 子对象提取 helper，不迁移 receipt。测试必须覆盖：

- string escaping；
- raw object/array value 不被二次转义；
- 字段顺序；
- 空数组、空对象、false 和 0 不被省略；
- fixture 子对象能精确提取；
- malformed fixture fail closed。

### Slice 1：同构度最高的最新合同 pair

先迁移：

- `runtime_credential_resolver_no_network_safety_fixture_receipts.cpp`
- `runtime_credential_resolver_abort_rollback_semantics_receipts.cpp`

两者都已经使用共享 utils，且都有 required/prohibited/rejection/no-go/upstream-echo/summary/boundary 结构，最适合验证 builder 是否真的减少重复。每个文件迁移前后都要对 standalone fixture 子对象逐字节比较。

### Slice 2：approval artifact 合同组

候选包括 signed human approval、credential handle approval、endpoint handle allowlist approval、approval prerequisite artifact 和 human approval artifact。先做形状 census，再选择不超过 3 个同构文件。任何结构明显不同的文件留在本地，不强行塞进 builder。

### Slice 3：runtime shell blocked 决策组

候选包括 disabled runtime shell、candidate gate、decision record、post-decision plan 和 stop/prerequisite。重点复用共同 boundary/check/summary 拼装，同时保留每份 decision 的领域 section。

### Slice 4：legacy helper 收口

最后处理尚未复用 `runtime_credential_resolver_receipt_utils.hpp` 的 7 个专题文件以及基础 `runtime_credential_resolver_receipts.cpp`。只有当前三批证明 builder 稳定后才做，避免同时迁移结构和统一旧 helper。

## 7. 每批验收与停止条件

每批必须通过：

- builder focused tests；
- 对应 receipt focused tests；
- standalone fixture 子对象 byte parity；
- `smokejson_command_receipt_tests`；
- `runtime_smoke_evidence_tests`；
- `release_verification_manifest_tests`；
- full CTest；
- sanitizer、coverage 和 clang-format CI；
- `git diff --check` 与 `git show --check`。

出现以下情况立即停止该文件迁移：

- 需要修改 fixture 或既有 expectation 才能通过。
- builder 需要知道具体 Node 版本、具体 prerequisite 或具体 authority 语义。
- 为容纳一个例外而新增大量 optional 字段或长布尔参数。
- digest、字段顺序、compact whitespace 或嵌入输出发生变化。
- 迁移后文件没有明显变短，或者共同逻辑只是从一个长文件搬到另一个长文件。
- 需要打开 network、执行写命令、启动 sibling 服务或读取 secret 才能验证。

## 8. 本版完成标准

v1625 只在以下条件全部成立时完成：

- 本说明记录 28/26/25 文件库存与字节基线。
- CTest `receipts_consolidation_note_contract` 能验证文档关键章节、源文件库存和 no-execution 边界。
- C++ runtime source、fixtures 和 public formatter 均未改动。
- full CTest 继续通过。
- 后续 v1626 从 Slice 0 开始，不跳过 parity harness 直接搬 receipt。

一句话：v1625 把“receipts 看起来重复”升级为可量化、可验证、可停止的重构合同，为后续在不改变任何 JSON 字节、digest 和只读边界的前提下逐步收敛 26 个 credential resolver receipt 文件。

## 9. v1626 Slice 0 实施记录

v1626 完成了本设计的第一层基础设施，但没有迁移任何 receipt formatter。新增的 `src/runtime_receipt_json_builder.hpp/.cpp` 是内部 ordered JSON builder，复用既有 `runtime_evidence::json_string`、`json_bool` 和 `json_object`，只补齐数字、raw JSON array、initializer-list 入口和显式 ordered field 模型。它不进入 public include，不新增第三方依赖，不读取 Node/Java，不启动服务，也不解释 receipt 业务语义。

测试侧新增 `tests/receipt_fixture_parity.hpp` 和 `tests/runtime_receipt_json_builder_tests.cpp`。前者按字段名从已提交 fixture 中提取完整 JSON object，能处理字符串转义、嵌套对象、数组和空白，并对未闭合对象、重复字段、非对象目标、坏字段名、未终止字符串等情况 fail closed。后者先验证 builder 的字节行为：字段顺序保持输入顺序，false、0、空对象、空数组不被省略，普通字符串正确转义，已经编码好的 object/array 不会被二次转义。

fixture 冻结结果如下：

- `credential_resolver_no_network_safety_fixture_contract_non_participation_receipt`：嵌套对象长度 17,738 字节，FNV digest `fnv1a64:794a730788ce0e87`。
- `credential_resolver_abort_rollback_semantics_contract_non_participation_receipt`：嵌套对象长度 19,183 字节，FNV digest `fnv1a64:0e41aded16909c6d`。

候选 formatter 状态也被显式记录：abort/rollback 这份当前已经与冻结 fixture 子对象 byte-for-byte 相等；no-network 这份当前不是 byte-for-byte 相等，因此只能作为已知 pre-migration drift 输入，不能在下一版直接声称“迁移前后全等”。这不是放宽标准，而是把风险提前暴露出来：后续 Slice 1 若要迁移 no-network，必须先定位并解释漂移来源，或者先选择 abort/rollback 作为更低风险的首个迁移样本。

v1626 的验收结果：focused receipt/build tests 通过，`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests`、`release_verification_manifest_tests` 通过，full local CTest 为 342/342。fixtures、public receipt output、runtime commands、router authority、write path、WAL behavior、network behavior、credential boundary 和 execution authority 均未扩展。
## 10. v1627 Slice 1a 实施记录：先迁移 abort/rollback，先诊断 no-network

v1627 只执行 Slice 1 的低风险半步：先迁移 `runtime_credential_resolver_abort_rollback_semantics_receipts.cpp`，暂不迁移 `runtime_credential_resolver_no_network_safety_fixture_receipts.cpp`。原因很直接：v1626 已经证明 abort/rollback 当前 public formatter 与冻结 fixture 子对象逐字节相等，而 no-network 已经被标记为 pre-migration drift 输入。按照本设计的停止条件，有漂移的 formatter 不能被直接改写；必须先查清漂移，再决定下一刀。

abort/rollback 的迁移方式是保守的。文件仍然保留本 receipt 的常量、digest parts、领域段落、source Node v326/v325 引用和所有边界字段；新的内部 ordered JSON builder 只负责把已经排好序的字段拼成 compact JSON。`required_fields`、`prohibited_fields`、`checks` 等历史 raw JSON 片段没有被重新解释，digest parts 的顺序也没有改变。顶层对象仍按原顺序输出：版本和来源字段、Node 引用、mini-kv receipt、checks、summary、warnings、recommendations、evidence endpoints、digest 字段、关闭边界旗标、`boundary` 和 `node_action`。这保证迁移是在“拼装机制”层面收敛重复，而不是在“证据语义”层面发明新 contract。

本版同时把 no-network 漂移查清楚并写进测试。诊断结论是：committed fixture 子对象和当前 formatter 在 JSON 语义上完全一致，字段数都是 118，字段顺序一致，递归解析后的值差异数为 0，`receipt_digest` 都是 `fnv1a64:3d8c483c93f8acf9`。唯一的原始字节差异出现在一段说明文字：fixture 使用 `Node v323\u0027s no-network fixture contract`，当前 formatter 使用 `Node v323's no-network fixture contract`。长度差正好是 5 个字符，因为 `\u0027` 比直接 apostrophe 多 5 个字节。`runtime_receipt_json_builder_tests` 现在用 `replace_once` 明确证明：只把 fixture 中这一处 `\u0027` 替换为直接 apostrophe，整段 no-network 子对象就与当前 formatter 完全相等。

这不是给 no-network 迁移开绿灯，而是给下一版设置更清楚的入口条件。下一步若迁移 no-network，需要先明确选择：要么让新 builder 保留 fixture 的历史 `\u0027` 字节拼写，追求 fixture 子对象 byte parity；要么承认当前 formatter 的直接 apostrophe 是已经运行中的 public surface，并补充一个专门的 fixture/manifest 兼容决策。无论选择哪条，都不能在迁移中顺手“格式化一下”或偷偷改 fixture。v1627 的价值在于把这个差异缩小到一个可命名、可测试、可审查的字节级事实。

验收结果：`runtime_receipt_json_builder_tests`、`smokejson_command_receipt_tests`、`release_verification_manifest_tests`、`runtime_smoke_evidence_tests`、`no_network_safety_fixture_contract_receipt_tests`、`abort_rollback_semantics_contract_receipt_tests` 均通过；full local CTest 为 342/342。`clang-format --dry-run --Werror`、`git diff --check` 和严格归档预算检查通过。fixtures、public command surface、SMOKEJSON、release manifest、router authority、Store/WAL 写入、credential value、raw endpoint、network access、rollback/deployment/LOAD/COMPACT/RESTORE/SETNXEX execution authority 均未扩展。
