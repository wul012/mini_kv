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
- 共同逻辑只是从一个长文件搬到另一个长文件，且没有提升 byte parity、边界可读性、重复消除或测试可证性。
- 需要打开 network、执行写命令、启动 sibling 服务或读取 secret 才能验证。

Line-count shrink is a review metric, not a stop condition. `git diff --numstat`
里的净增或净减只能提示维护者解释迁移是否值得，不能单独决定停止。真正的停止条件是：
byte parity 无法证明、fixture/manifest 需要改写、digest/字段顺序/compact whitespace
漂移、builder 开始理解业务语义，或者共同逻辑只是搬家而没有降低后续维护风险。对于
receipts 这种字段显式、顺序敏感、边界密集的文件，首批迁移可能因为引入 ordered
field 分组、闭合边界 helper 或更硬的测试证据而短期增加 diff 行数；只要输出逐字节不变、
危险边界没有扩大、重复拼装路径确实减少，这类增长应记录为可解释的 review metric，
而不是 stop-condition。

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

## 11. v1628 Slice 1a 质量修正：false shrink metric 与 no-network 漂移落地

v1628 不迁移新的 receipt formatter，只修正两个会影响后续规模化迁移判断的质量点。

第一，v1625 的停止条件曾把“迁移后文件没有明显变短”写成硬停止条件。这个表达过粗，
在 v1627 暴露出噪声：`runtime_credential_resolver_abort_rollback_semantics_receipts.cpp`
迁移到 ordered builder 后，`git diff --numstat` 显示该文件 443 行新增、327 行删除，
净增 +116 diff lines。这个增长本身不是坏信号，因为新增行主要来自显式 ordered field
分组和 helper 调用，换来的是 fixture byte parity 继续成立、手写逗号/字段顺序风险下降、
公共 builder 行为被 focused test 锁住。正确判断应是“输出不变、边界不变、拼装机制更可证”，
而不是“物理行数必须立刻下降”。因此本说明把 line-count shrink 降级为 review metric：
它需要解释，但不能单独构成 stop-condition。

第二，本版把 no-network drift 继续收紧到测试事实，而不是停留在文档描述。新的
`runtime_receipt_json_builder_tests` 断言 no-network fixture 子对象和当前 formatter 均有
118 个顶层字段，唯一 `\u0027` 只出现在 `boundary` 文本中的
`Node v323\u0027s no-network fixture contract`，当前 formatter 对应位置是直接 apostrophe
`Node v323's no-network fixture contract`。把这一处历史转义替换后，整段 no-network
子对象与当前 formatter 完全相等；`receipt_digest`、`read_only`、`execution_allowed=false`、
`network_safety_fixture_executed=false`、`http_request_sent=false`、`tcp_connection_attempted=false`
等边界字段仍保持一致。

这意味着下一次真正迁移 no-network 时，不能再把 drift 说成“还没查清”。它已经查清：
差异是单字段、单短语、单转义拼写，不是语义漂移，也不是字段增删、顺序变更、digest 变化、
network/write/WAL/execution 边界变化。迁移前仍必须显式选择 canonical surface：要么让
builder 复现历史 fixture 的 `\u0027` 字节拼写以追求 standalone fixture byte parity；要么保留
当前 formatter 的直接 apostrophe，并用专门兼容决策说明为什么 public runtime surface 优先。
在这个选择写清之前，no-network formatter 不应被顺手迁移。

## 12. v1637 canonical surface 与 no-network builder 迁移 closeout

v1637 先把 no-network 的唯一历史漂移升级为显式决策，再在这个决策下面迁移
`runtime_credential_resolver_no_network_safety_fixture_receipts.cpp`。仓库事实现在是：Slice 0 的 ordered JSON
builder 和 parity harness 已完成，`runtime_credential_resolver_abort_rollback_semantics_receipts.cpp` 与
`runtime_credential_resolver_no_network_safety_fixture_receipts.cpp` 两个 formatter 已迁移到 builder；其余 receipt
formatter 仍按后续 slice 继续推进。

canonical 决策保持不变：**以当前运行时 formatter 的直接 apostrophe 拼写作为 canonical runtime surface**。
理由是下游运行时消费的是 public formatter/SMOKEJSON/CHECKJSON 暴露的当前输出，而历史 fixture 是已提交证据，
不能为了追求表面一致而改写。具体边界如下：

- 不改写 `fixtures/release/*.json`，历史 fixture 中的 `Node v323\u0027s no-network fixture contract` 保持冻结；
- no-network public formatter 的运行时输出保持当前 `Node v323's no-network fixture contract` 拼写；
- `runtime_receipt_json_builder_tests` 中的 parity 规则只允许这一处历史转义拼写差，把 fixture 侧
  `Node v323\u0027s no-network fixture contract` 规范化为 runtime canonical surface
  `Node v323's no-network fixture contract` 后必须与 builder 迁移后的 formatter 完全相等；
- 该规则同时断言 fixture 中 `\u0027` 总数为 1、目标短语出现 1 次、runtime 输出中 legacy 拼写出现 0 次。
  多出第二处 `\u0027`、字段顺序改变、digest 改变、边界字段改变、network/write/WAL/credential/execution 语义改变，
  都不属于允许差异。

这不是放宽 byte parity，而是把“fixture 冻结”和“runtime surface 已经公开消费”之间唯一已知历史拼写差
转成可审查、可失败的兼容规则。no-network 的迁移只发生在这一规则之后：它把 nested sections、数组、
闭合边界 flags、top-level fields 改为 ordered builder 拼装，同时保持运行时输出、receipt digest、字段顺序、
compact whitespace、release manifest 嵌入含义和所有 read-only/no-network/no-write/no-execution 边界不变。
后续 slice 不得继续扩大这个兼容例外；新的 receipt 迁移必须回到普通 fixture/runtime parity 或写出单独 waiver。

## 13. v1638 Slice 2 审批合同迁移前基线

v1638 没有继续迁移 formatter，而是先为 Slice 2 建立迁移前的可执行基线。原因是审批合同组表面上都属于
approval artifact 链条，但它们并非完全同构：`signed_human_approval_artifact` 是第一份签名人工审批 artifact
合同 intake，`credential_handle_approval` 和 `endpoint_handle_allowlist_approval` 是后续两个 handle/allowlist
审批合同；`approval_prerequisite_artifact` 与 `human_approval_artifact_review` 更接近 packet/intake 或 review
形态，不应该和前三个混在同一刀里迁移。因此本版只选择三份字段骨架最接近的合同 receipt 做迁移前锁定：

- `runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp`
- `runtime_credential_resolver_credential_handle_approval_receipts.cpp`
- `runtime_credential_resolver_endpoint_handle_allowlist_approval_receipts.cpp`

`runtime_receipt_json_builder_tests` 现在会从三份 frozen fixture 中提取对应的嵌套 receipt object，再和当前 public
formatter 输出比较。credential-handle 与 endpoint-handle 两份是普通 byte parity：fixture 子对象必须与 formatter
输出逐字节相等，并且继续包含 `read_only=true`、`execution_allowed=false`、`credential_value_read=false`、
`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`approval_ledger_written=false`、
`automatic_upstream_start=false` 等闭合边界字段。

signed-human 这份暴露出第二个历史拼写差异：frozen fixture 的 `boundary` 文本里是
`Node v314\u0027s non-secret contract`，而当前 formatter 输出是直接 apostrophe 形式
`Node v314's non-secret contract`。这与 v1637 no-network 的漂移同型：长度差正好是 5，差异只在一处
`\u0027` 与 `'` 的原始 JSON 拼写上。v1638 没有把它说成“完全 byte parity”，也没有改 fixture 或 formatter；
测试规则只允许把这一处历史 fixture 拼写规范化为当前 runtime canonical surface 后再比较，且同时断言字段数一致、
legacy 拼写只出现一次、runtime 输出中 legacy 拼写出现零次、闭合边界字段全部存在。任何第二处 `\u0027`、字段增删、
字段顺序变化、digest 漂移、network/write/WAL/credential/execution 语义变化都会失败。

因此 v1638 的状态是：builder-backed formatter 仍然只有 abort/rollback 与 no-network 两份；Slice 2 的三份审批合同
已经有迁移前 oracle。下一版如果迁移审批合同，应该先从 credential-handle 或 endpoint-handle 这种完全 byte parity
样本开刀；signed-human 可以迁移，但必须继续使用 v1638 命名的单点 canonical spelling waiver，不能把该 waiver
复制成宽泛规则。

## 14. v1639 credential-handle 边界 flag builder 迁移

v1639 选择 credential-handle approval contract 作为 Slice 2 的第一刀，但没有一次性重写整份 30KB formatter。迁移范围是
该 receipt 最密集、最容易出现逗号或顺序漂移的闭合边界 boolean field cluster：原来的 `format_closed_boundary_flags_json`
是一段超长手写 JSON fragment，现在变成 `append_closed_boundary_flag_fields(std::vector<OrderedJsonField>&)`，逐个追加
`read_only`、`execution_allowed`、credential handle、credential value、raw endpoint、external request、approval ledger、
schema migration、restore/load/compact/setnxex、auto-start、audit/order authority 等 false/true 字段，再由内部
`json_object(fields)` 渲染成 compact JSON object，最后剥掉首尾花括号嵌回历史 formatter 的同一位置。

这不是“整份 credential-handle formatter 已经完全 builder-owned”。领域 section、Node v317/v316 reference、checks、
summary、warnings、recommendations、evidence endpoints 等仍保留原来的局部拼装方式；本版只把高风险边界尾部从手写字符串
转成 ordered field 列表。选择这个粒度的原因是 v1638 已证明 credential-handle fixture 与 runtime formatter byte parity
完全成立，适合先迁移一块输出最敏感但语义最机械的区域。若这一块能在 focused tests 和 full CTest 下保持完全不漂移，
下一版再复制到 endpoint-handle 或继续拆 credential-handle 的 top-level/nested sections，风险会低得多。

验收结论：`runtime_receipt_json_builder_tests` 继续证明 credential-handle formatter 输出与 frozen fixture 子对象逐字节相等；
`credential_handle_approval_contract_receipt_tests`、`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 与
`release_verification_manifest_tests` 继续证明独立 fixture、SMOKEJSON 嵌入、runtime smoke evidence 与 release manifest
未漂移。本版没有改 fixture，没有改 digest 输入，没有改字段顺序，没有扩大 v1638 signed-human waiver，也没有打开 router、Store/WAL
write、network、credential read、raw endpoint parse、approval ledger write、schema migration、restore/load/compact/setnxex 或
automatic upstream start。

## 15. v1640 endpoint-handle 边界 flag builder 迁移

v1640 把 v1639 的做法复制到 endpoint-handle allowlist approval contract，但仍然保持同样的保守粒度：只迁移
`format_closed_boundary_flags_json` 这一组闭合边界布尔字段，不重写整份 formatter。endpoint-handle 这份 receipt
和 credential-handle 高度相似，但不是完全等价：它多了 endpoint handle、allowlist authority、raw endpoint URL stored/parsed/rendered
等字段，因此不能简单把 credential-handle 的字段表复制过来。迁移时逐字段保留历史顺序，把
`read_only`、`execution_allowed`、endpoint/allowlist authority、credential value、raw endpoint、external request、
managed audit storage、write/admin、approval ledger、schema migration、restore/load/compact/setnxex、auto-start、audit/order authority
这些字段逐项放进 `std::vector<OrderedJsonField>`，再由 `json_object(fields)` 生成 compact object，最后剥掉外层花括号嵌回原 formatter。

这个“剥掉花括号再嵌回”的方式看起来绕一点，但它是本阶段最稳的选择。旧 formatter 在更大的 JSON object 中间插入一段
`"field":value,"field":value` 片段；如果本版顺手把整份 top-level object 都改成 builder，就会同时触碰 Node reference、
mini-kv receipt、checks、summary、warnings、recommendations、evidence endpoints、digest fields 和 boundary 文本，风险面太大。
v1638 已经给 endpoint-handle 建立了 fixture/formatter byte parity oracle，因此本版只拿最机械、最密集、最容易出逗号或顺序错误的
boolean cluster 开刀。迁移后 focused test 仍然证明 endpoint-handle public formatter 与 frozen fixture subobject 逐字节相等。

本版的价值不是“行数变少”，而是把高风险字符串尾部变成结构化的、可逐字段审查的清单。旧写法里一个漏逗号、漏引号、
字段顺序调换，都会直接影响下游证据；新写法虽然 diff 行数不一定明显减少，但每个边界字段都成为独立 `OrderedJsonField`，
true/false 由 `json_bool` 生成，逗号和 compact object 拼接由 shared builder 负责。这样后续维护者新增或审查边界时，
看到的是“哪一个危险能力仍然 false”，而不是在一条超长字符串里数引号。

验收结论：`endpoint_handle_allowlist_approval_contract_receipt_tests`、`runtime_receipt_json_builder_tests`、
`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 与 `release_verification_manifest_tests` focused lane 全绿，
完整 build 后 full local CTest 344/344 全绿。fixture 未改，manifest 未改，public command surface 未改；raw endpoint parse、
network send、credential read、router、Store/WAL write、approval ledger write、schema migration、restore/load/compact/setnxex、
automatic upstream start、audit authority 和 order authority 仍全部关闭。Slice 2 当前状态是：credential-handle 与 endpoint-handle
两份 approval contract 的闭合边界 flag block 已 builder-backed；signed-human approval artifact 仍保留 v1638 命名的单点 canonical spelling
waiver，不能在没有专门决策的情况下混入同一刀。

## 16. v1641 signed-human 边界 flag builder 迁移

v1641 把 Slice 2 的第三个审批合同样本也纳入同一种边界 flag 迁移方式：`runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp` 里的 `format_closed_boundary_flags_json` 不再是一段超长手写 JSON fragment，而是先把每个布尔边界放进 `std::vector<OrderedJsonField>`，再由 `json_object(fields)` 统一生成 compact object，最后剥掉外层花括号嵌回旧 formatter 的原始位置。这与 v1639 credential-handle、v1640 endpoint-handle 的路线一致：迁移的是最机械、最容易因为逗号或顺序出错的闭合边界尾部，不是整份 signed-human formatter，也不是整条 approval family 的完成声明。

signed-human 这一刀比前两刀多一个需要反复写清楚的风险：v1638 已经命名过一处历史拼写差异，冻结 fixture 里是 `Node v314\u0027s non-secret contract`，当前 public formatter/runtime surface 是 `Node v314's non-secret contract`。v1641 没有修改 fixture，也没有把 waiver 扩大成“任意 apostrophe 差异都可以”。测试仍然要求 frozen fixture 中 `\u0027` 总数为 1、目标短语只出现 1 次、runtime 输出中 legacy 拼写出现 0 次；只有把这一处命名短语规范化为 runtime canonical surface 后，formatter 输出才允许与 fixture 子对象比较。因此这次迁移不是放松 byte parity，而是在已经存在的单点兼容规则下继续证明：字段数、字段顺序、digest、compact whitespace、read-only/no-write/no-network/no-credential/no-execution 边界都没有漂移。

字段层面的变化也保持最窄。`read_only=true`、`execution_allowed=false`、`signed_human_approval_artifact_contract_non_participation_receipt_only=true`、`signed_human_approval_artifact_contract_intake_only=true` 等正向证明字段仍在最前面；runtime shell、real resolver、provider/client、credential value、raw endpoint URL、signing private key、signed artifact store/validate/authority/signature verify、external request、managed audit store、storage write、approval ledger write、schema migration、restore/load/compact、SETNXEX、Node/Java/mini-kv auto-start、audit/order authority 等字段仍逐项为 false。旧代码把这些布尔值藏在一条很长的字符串里；新代码把它们变成可逐项审查的 ordered field 清单。维护价值不靠行数缩短证明，而靠后续修改时能清楚看见“哪个危险能力仍然关闭”。

验收结论：focused lane 中 `runtime_receipt_json_builder_tests`、`signed_human_approval_artifact_contract_receipt_tests`、`smokejson_command_receipt_tests`、`release_verification_manifest_tests` 与 `runtime_smoke_evidence_tests` 5/5 通过；完整 build 后 full local CTest 344/344 通过。v1641 没有改 release fixture，没有改 manifest，没有改 public command surface，没有打开 router、Store/WAL write、credential read、raw endpoint parse、HTTP/TCP network send、approval ledger write、schema migration、restore/load/compact/setnxex execution、automatic upstream start、audit authority 或 order authority。Slice 2 当前状态是：signed-human、credential-handle、endpoint-handle 三份 approval contract 的闭合边界 flag block 都已经 builder-backed；它们的领域 section 仍保守留在各自 formatter 中，后续若继续迁移 top-level/nested section，需要另起小版本并重新证明对应 fixture/formatter parity。

## 17. v1642 credential-handle 顶层 builder 迁移

v1642 继续 Slice 2，但只选择最干净的一份样本：`runtime_credential_resolver_credential_handle_approval_receipts.cpp`。选择它的原因是 v1638 已经证明 credential-handle fixture 子对象与当前 public formatter 逐字节相等，且它没有 signed-human 的 `Node v314\u0027s` 历史拼写 waiver，也没有 endpoint-handle 的 raw-endpoint/allowlist authority 专门字段组。因此本版可以把整份 credential-handle formatter 的顶层对象迁到 ordered JSON builder，而不需要同时扩大兼容例外或重写三份 approval formatter。

迁移范围从 v1639 的 boundary flag block 扩展到整个 top-level JSON object。原来 `format_credential_resolver_credential_handle_approval_contract_non_participation_receipt_json` 是一段从 `receipt_version` 拼到 `node_action` 的长字符串，嵌套的 `source_node_v317_reference`、`credential_handle_approval_contract`、`prerequisite_transition`、`necessity_proof`、`source_node_v316_reference`、`mini_kv_receipt`、`summary`、`warnings`、`recommendations`、`evidence_endpoints` 和 `next_actions` 都靠手写逗号连接。v1642 把这些结构先命名为局部 `json_object` / `json_array`，再把顶层字段放进 `std::vector<OrderedJsonField>`，最后调用 `json_object(std::span<...>)` 输出。闭合边界字段不再通过“剥掉 object 外壳再嵌入”的旧桥接函数，而是继续调用 `append_closed_boundary_flag_fields(fields)` 直接追加到同一个顶层 ordered field list。

这个迁移仍然不是新增合同、不是新 runtime command、也不是 release fixture 重写。领域数组如 `required_fields`、`prohibited_fields`、`checks` 仍由本文件的领域 helper 产生，并作为已经编码好的 raw JSON value 进入 builder；digest parts、`read_command_list_digest`、binary/retention provenance digest、字段名称、字段顺序、compact whitespace 和 public formatter 名称都保持不变。builder 只承担“按给定顺序拼成 JSON”的机械职责，不理解 credential approval 语义，不自动补字段，不根据 Node 版本推导状态。

验收结论：`runtime_receipt_json_builder_tests` 继续把 credential-handle public formatter 与 frozen fixture 子对象逐字节比较；`credential_handle_approval_contract_receipt_tests`、`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 与 `release_verification_manifest_tests` 共同证明 standalone fixture、SMOKEJSON 嵌入、runtime smoke evidence 和 release manifest 都没有漂移。完整本地构建后 full CTest 344/344 通过。真实 CLI smoke 中 `SMOKEJSON` 仍暴露 credential-handle receipt 链，`CHECKJSON LOAD data/prod.snap` 只报告 LOAD 是写命令/side effect，不执行 LOAD，也不触 Store/WAL。v1642 没有改 fixture，没有改 manifest，没有打开 router、Store/WAL write、credential read、raw endpoint parse、HTTP/TCP network、approval ledger write、schema migration、restore/load/compact/setnxex、automatic upstream start、audit authority 或 order authority。

Slice 2 当前状态更新为：credential-handle approval contract 的整份 top-level formatter 已经 builder-backed；endpoint-handle 与 signed-human 仍只完成 boundary flag block builder 迁移。下一刀若继续 Slice 2，优先在 endpoint-handle 上复制 v1642 的 top-level 迁移模式，因为它也有普通 byte parity；signed-human 则必须继续保留 v1638 命名的单点 canonical spelling rule，不能把该 waiver 泛化。

## 18. v1643 endpoint-handle 顶层 builder 迁移

v1643 完整迁移 `runtime_credential_resolver_endpoint_handle_allowlist_approval_receipts.cpp` 的顶层 formatter。它沿用 v1642 已验证的有序组装方式，但没有把 credential-handle 字段表复制过来：endpoint-handle 自己的 `endpoint_handle_allowlist_approval_contract`、Node v320/v319 引用、raw endpoint 禁止项、allowlist authority 与 approval-status authority 边界仍由本文件逐项拥有。原先只负责边界尾部的 `format_closed_boundary_flags_json()` 桥接函数被移除，`append_closed_boundary_flag_fields(fields)` 直接把闭合字段追加到最终顶层字段表，避免先生成对象、剥掉花括号、再拼回长字符串的中间步骤。

顶层顺序没有重排。`receipt_version` 到 `runtime_role` 仍在最前，随后依次是 `source_node_v320_reference`、`source_node_v319_reference`、`mini_kv_receipt`、`checks`、`summary`、blockers/warnings/recommendations、evidence endpoints、next evidence/actions、四个 provenance/read-command digest、receipt digest、闭合边界字段，最后才是 `boundary` 与 `node_action`。每个嵌套对象先由 `json_object` 独立构造，数组由 `json_array` 构造，再作为已经编码的 JSON value 放入 `OrderedJsonField`；builder 只负责确定性的转义、逗号和字段顺序，不推导业务状态。

迁移没有改 fixture、digest parts、公有 formatter 名称、命令输出或 release manifest。v1638 建立的 endpoint-handle fixture/formatter 逐字节 oracle 在 v1643 focused lane 中继续直接相等，证明字段序、compact whitespace、字符串转义和全部值未漂移。endpoint 专属的 `raw_endpoint_url_accepted/stored/parse_allowed/render_allowed/parsed/rendered=false`、endpoint handle store/validate/resolve/authority=false、allowlist authority=false、approval-status authority=false 仍与 credential、network、router、Store/WAL write、schema、restore/load/compact、SETNXEX、auto-start、audit/order authority 和 execution 边界一起关闭。

迁移后的源文件由 503 行变为 558 行。这个增长不是 stop condition，也不是以行数冒充收益：新增行来自把匿名长字符串拆成可命名、可逐字段审查的对象与数组；机械风险从“在数百段字符串中检查逗号和引号”变为“审查有序字段表”。评审指标仍是重复拼接是否消失、fixture parity 是否保持、边界是否更容易审查，而不是要求每次结构化迁移都缩短物理行数。

本版结束时，28 个 runtime receipt 文件中仍有大量 formatter 未迁移，不能宣称阶段一完成。当前 builder include census 为 5 个文件；其中 abort/rollback、no-network、credential-handle 与 endpoint-handle 已完成整份顶层 builder 迁移，signed-human 只完成闭合边界区块。下一刀继续处理 signed-human 的完整顶层对象，并必须保留它自己命名的 `Node v314\\u0027s` fixture spelling 兼容规则；不得把该规则扩成通配 apostrophe 归一化。

## 19. v1644 signed-human 顶层 builder 迁移

v1644 完成 Slice 2 最后一份审批合同的整份顶层迁移：`runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp` 不再用一条长字符串组装 public receipt。signed artifact contract、prerequisite transition、necessity proof、Node v314/v312 引用、两份 summary、warnings、recommendations、evidence endpoints 与 next actions 都成为命名 object/array，最后按历史字段顺序进入 `std::vector<OrderedJsonField>`。v1641 留下的 boundary strip-and-splice 桥接函数随之删除，闭合字段直接追加到最终顶层字段表。

这次迁移没有把 v1638 的 canonical 规则扩大。冻结 fixture 仍含且只含一处命名短语 `Node v314\\u0027s non-secret contract`，runtime formatter 仍输出 `Node v314's non-secret contract`。parity harness 先断言 legacy/canonical 短语各自的出现次数，再只替换这一处命名短语后比较整份对象；任何第二处 `\\u0027`、未命名替换、字段数变化或其他字节差都会失败。builder 本身没有 normalization 分支，也不知道这一历史兼容规则。

signed-human 专属边界继续由本文件逐项拥有：signing private key、signed artifact store/validate/authority/signature verify、credential value、raw endpoint、provider/client、HTTP/TCP、managed audit storage、Store/WAL write、approval ledger、schema、restore/load/compact、SETNXEX、auto-start、audit/order authority 与 execution 均保持关闭。源文件由 491 行变为 545 行；增加来自显式结构展开，不改变以 byte parity、digest 与边界为 stop condition 的规则。

v1644 focused lane 中 builder parity、signed-human receipt、SMOKEJSON、runtime smoke evidence、release manifest 5/5 通过。至此 Slice 2 选定的三份审批合同都完成整份顶层 builder 迁移，v1639-v1641 的临时 boundary block 过渡状态也已消失。28 文件总 census 尚未完成；下一版不立即批量改 formatter，而是先把下一组同构 receipts 的 fixture 子对象、digest、字段数和允许的 canonical 差异冻结成可复现机械门，再决定批量迁移范围。

## 20. v1645 approval lifecycle parity 与机械 census 门

v1645 不修改任何生产 formatter，先为下一批 approval lifecycle 候选建立迁移前基线。`runtime_receipt_json_builder_tests` 新增四份 fixture 子对象的版本化大小、digest、顶层字段数、公有 formatter 输出和闭合边界断言：approval-required boundary 为 12,949 字节/108 字段且与 runtime 逐字节相等；approval-prerequisite artifact 为 16,483 字节/107 字段，只有命名的 Node v306 apostrophe 差；human approval review 为 17,116 字节/107 字段，只有命名的 Node v308 apostrophe 差；post-echo decision 为 14,169 字节/101 字段，除 Node v310 apostrophe 外，runtime 在 summary 结束处还存在一处 `} },`，fixture 为 compact `}},`。

前三份被标为 migration-ready。post-echo 明确标为 blocked，不允许把额外空格并入 apostrophe compatibility；测试分别断言 compact/spaced transition 的出现次数，并证明移除这一处已命名空格后才与 apostrophe-canonical fixture 相等。这是漂移诊断，不是 byte-parity waiver，也不授权 v1646 迁移 post-echo。它需要后续独立版本决定修正 runtime compact surface 还是保留兼容策略。

本版同时新增 `cmake/check_receipt_builder_census.cmake` 和 `receipt_builder_census_contract`。机械口径为：`src/runtime_*_receipts.cpp` 共 28 个，其中 27 个拥有 public `format_*_receipt_json`，5 个已 builder-backed，22 个 pending；唯一 waiver 是没有 JSON formatter 的 `runtime_credential_resolver_execution_denied_retention_receipts.cpp`。CTest 固定总数、formatter owner 数、waiver 文件名和 builder floor=5；未来版本只能提高 floor，新增 receipt 或 waiver 变化必须显式修改门并接受 review。

v1645 将重复的 no-network/signed-human apostrophe 规范化前置断言收敛到一个命名 helper，但每个 fixture family 仍提供独立 legacy/canonical 常量。helper 只允许 exactly-once 替换，不接受通配 apostrophe。下一版迁移范围据此确定为三份：approval-required boundary、approval-prerequisite artifact、human approval review；post-echo 被 stop condition 排除。

## 21. v1646 approval lifecycle 三份 formatter 迁移

v1646 严格使用 v1645 的迁移门，只处理已标为 migration-ready 的三份 formatter：
`runtime_credential_resolver_approval_boundary_receipts.cpp`、
`runtime_credential_resolver_approval_prerequisite_artifact_receipts.cpp` 和
`runtime_credential_resolver_human_approval_artifact_receipts.cpp`。存在额外 `} },` 空白漂移的
`runtime_credential_resolver_human_approval_post_echo_receipts.cpp` 没有修改，仍由下一版独立决策。

三份 formatter 的顶层对象都从手工字符串拼接迁移到 `runtime_receipt_json_builder`。Node 来源引用、
artifact/review packet、necessity proof、summary、warnings、recommendations、evidence endpoints 与 next actions
先构造成命名的 `json_object` / `json_array`，随后按历史顺序放入 `std::vector<OrderedJsonField>`。
builder 只负责字符串转义、标量编码、compact 逗号和给定顺序的渲染，不推导业务状态，也不改变 digest 输入。

approval-prerequisite 与 human-approval-review 原先各自复制了一整组 disabled runtime-shell 封闭字段。
v1646 将两者完全相同的后缀收敛为
`credential_resolver_detail::append_closed_runtime_shell_boundary_fields`，两份文件只保留各自 receipt family
的前置字段，再在同一历史位置调用共享 helper。approval-required boundary 的字段集合较早且不同，仍保持本地所有权，
避免用一个“万能字段表”混淆不同合同。共享 helper 只消除真实重复，不跨越字段顺序或领域边界。

v1645 的冻结 oracle 在迁移后继续原样裁决：approval-required boundary 与 fixture 子对象逐字节相等；
approval-prerequisite artifact 只允许自己命名的 Node v306 apostrophe 历史拼写；human approval review 只允许自己命名的
Node v308 apostrophe 历史拼写。fixture、测试期望、digest、字段数和 compatibility helper 均未为本次迁移而改动。
`runtime_receipt_json_builder_tests` 与三份领域 receipt 测试全部通过，证明 builder 输出保持历史 compact surface。

机械 census floor 从 5 收紧到 8。当前口径仍为 28 个 runtime receipt source、27 个 formatter owner、
8 个 builder-backed、19 个 pending、1 个无 formatter 的命名 waiver。floor 只升不降；任何人删除 builder include、
新增未记账 formatter 或改变 waiver 集合，`receipt_builder_census_contract` 都会失败。v1646 的 full local CTest 为
345/345；真实 CLI smoke 继续证明 `SMOKEJSON` 是 `read_only=true`、`execution_allowed=false`，而
`CHECKJSON LOAD data/prod.snap` 只分类 LOAD 的写副作用且 `touches_wal=false`，没有执行 LOAD。

本版没有新增 command/router，没有修改 Store/WAL/snapshot/RESP/OSFS 行为，没有读取 credential value、解析 raw endpoint、
发送 HTTP/TCP、写 approval ledger、执行 schema migration、LOAD/COMPACT/RESTORE/SETNXEX、自动启动上游，也没有取得 audit/order
authority。下一版仍必须先解决 post-echo 的单字节 compact whitespace 漂移，不能把它作为第二个 parity waiver 顺手迁移。

## 22. v1647 post-echo 单字节修正与 builder 迁移

v1647 只处理 v1645/v1646 明确留下的 post-echo 阻塞项。根因位于
`runtime_credential_resolver_human_approval_post_echo_receipts.cpp` 的 Node v310 summary 结束处：旧 runtime formatter
手写为 `"recommendation_count":2} },"source_node_v309_reference"`，两个相邻 object closing brace 之间多出一个空格；
冻结 fixture、项目 builder 和其他 compact receipt 均使用 `...:2}},"source_node_v309_reference"`。

修正过程先只删除生产 formatter 的这一个空格，保持 v1645 测试不动。重新构建后，旧 drift 断言按预期在
`runtime_receipt_json_builder_tests.cpp:442` 失败，因为它仍要求 fixture/runtime 保持“多 4 字节”的旧关系。
随后测试删除 spaced/compact 双表面常量与专门替换分支，把 post-echo 标为 migration-ready；新门只允许既有、命名的
Node v310 apostrophe fixture spelling，规范化这一处后整份对象必须相等，大小关系必须回到 fixture 比 runtime 多 5 字节。
这是从诊断 waiver 收紧为普通 parity，不是修改 fixture 或扩大兼容规则。

在 compact parity 单独通过后，post-echo formatter 才迁移到 ordered builder。`decision_gate`、`necessity_proof`、
Node v310/v309 references、两份 summary、mini-kv receipt、warnings、recommendations、evidence endpoints 和 next actions
都成为命名 object/array；顶层字段和本 receipt 自己的闭合边界字段按历史顺序进入 `OrderedJsonField`。
post-echo 比 v1646 的共享 runtime-shell 后缀少两个 raw-endpoint 字段，因此没有误用共享 helper，而是保留本地字段所有权。

census floor 从 8 收紧到 9，当前为 28 sources、27 formatter owners、9 builder-backed、18 pending、1 named waiver。
focused lane 6/6、full CTest 345/345 通过。真实 CLI smoke 中 compact transition 恰好出现 1 次，spaced transition 为 0；
`read_only=true`、`execution_allowed=false` 继续存在，`CHECKJSON LOAD` 只分类 LOAD 且 `touches_wal=false`，随后正常 `BYE`。

本版没有改写 release fixture，没有引入通用 whitespace normalizer，没有改变 receipt digest 输入、字段数或字段顺序；
没有新增 router、Store/WAL write、network、credential read、raw endpoint parse、approval ledger write、schema migration、
LOAD/COMPACT/RESTORE/SETNXEX execution、auto-start、audit authority 或 order authority。post-echo 阻塞现已关闭，下一批候选
必须重新按同构 family 建立迁移前 parity，而不能继续沿用本版的缺陷修正理由。
