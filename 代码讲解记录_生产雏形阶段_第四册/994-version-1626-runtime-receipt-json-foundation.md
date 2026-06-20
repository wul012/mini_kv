# 第一千六百二十六版代码讲解：runtime receipt JSON 基础设施与 fixture 子对象冻结

本版目标是完成 runtime receipts 合并计划的 Slice 0：先建立一个很小的内部 ordered JSON builder，再建立一个能从历史 fixture 中提取嵌套 receipt 对象的 fail-closed 测试 helper，并用测试冻结两份候选 receipt 的历史字节。它的价值不在于立刻减少多少行代码，而在于把后续重构的第一条安全绳拉起来：以后每次把手写 JSON formatter 改成共享 builder，都必须先证明历史 fixture 子对象、当前 public formatter、SMOKEJSON、runtime smoke evidence 和 release manifest 没有被悄悄改动。

它不是什么：不是新的命令入口，不是新的路由，不是新的 evidence profile，不会把 mini-kv 变成 Node 或 Java 的执行端，不会读取 credential value，不会解析 raw endpoint URL，不会打开 HTTP/TCP 网络连接，不会启动 sibling 服务，不会执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不会扩展 Store/WAL 写入权限。它只是一个内部代码维护地基，服务于未来拆分和收敛 26 个 credential-resolver receipt formatter。

## 入口

本版没有用户可调用入口。新增入口都是编译期和测试期入口：

```text
src/runtime_receipt_json_builder.hpp
src/runtime_receipt_json_builder.cpp
tests/receipt_fixture_parity.hpp
tests/runtime_receipt_json_builder_tests.cpp
CMakeLists.txt -> runtime_receipt_json_builder_tests
```

`runtime_receipt_json_builder.hpp` 放在 `src/` 下，而不是 `include/minikv/` 下，这一点很重要。它表示 builder 是 mini-kv 内部实现细节，不承诺给外部项目调用，也不成为新的 public API。后续 Node 或 Java 消费的仍然是已经存在的 JSON evidence 和 release fixture；它们不会知道这个 builder 存在。这样可以避免一个维护工具被误解成新契约。

CMake 侧只做两件事：把 `src/runtime_receipt_json_builder.cpp` 加进 `minikv` library；注册一个新的 `runtime_receipt_json_builder_tests`。测试目标额外把 `src/` 作为 private include 目录，以便测试内部 header。这种接法也在边界上很克制：生产库可以复用 builder，测试可以验证 builder，但 public include 面没有扩大。

## 响应或证据结构

本版没有新增 runtime 响应字段，但测试明确保护以下边界语义：

```text
read_only=true
execution_allowed=false
order_authoritative=false
network_safety_fixture_executed=false
http_request_sent=false
tcp_connection_attempted=false
rollback_executed=false
mini_kv_write_command_allowed=false
automatic_upstream_start=false
```

这些字段来自被冻结的历史 receipt 子对象和当前 formatter 输出。控制面读到它们时，应该理解为：mini-kv 只是提供只读证据，证明自己没有参与 credential resolver 的运行时执行，没有成为网络安全 fixture authority，没有进行 HTTP/TCP 请求，没有执行 rollback，也没有授权 Java SQL、ledger/schema write、mini-kv admin/write command 或自动启动上游服务。

本版没有新增这些字段，也没有重排它们。它做的是把字段所在的历史 JSON object 用长度和 digest 冻结下来。这样后续迁移时，如果一个逗号、转义、字段顺序、布尔值、compact whitespace 或嵌套对象位置发生变化，测试会在本地直接暴露，而不是等 Node 或 Java 消费端发现问题。

## 上游和下游关系

本版只读了当前 Node 计划和 v1625 设计门：

```text
source plan: D:\nodeproj\orderops-node\docs\plans3\v961-controlled-read-only-shard-preview-operator-evidence-value-supply-envelope-closeout-roadmap.md
local design: docs/receipts-consolidation-note.md
source fixtures:
  fixtures/release/credential-resolver-no-network-safety-fixture-contract-non-participation-receipt.json
  fixtures/release/credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json
consumer:
  future mini-kv receipt formatter migrations
  Node/Java loose evidence consumption through existing fixtures and SMOKEJSON surfaces
```

这里的上游关系是只读关系。mini-kv 没有打开 Node 项目，没有修改 Java 项目，没有启动任何服务，也没有让 Node 计划变成 C++ runtime dependency。Node 计划只回答一个方向问题：当前是否允许 mini-kv 做内部维护。答案是允许，因为这是非 contract 的内部质量工作，不会改变证据 schema、路由、执行权限或跨项目依赖。

下游关系也不是“新功能可以用了”。更准确地说，v1626 给未来迁移提供一个关口：要迁移哪份 receipt，先看 fixture 子对象能否提取、历史字节是否冻结、当前 formatter 是否已经全等。如果全等，可以作为低风险样本；如果不全等，先解释漂移来源，不能盲目迁移。

## 核心流程

第一层是 builder。`OrderedJsonField` 只有两个字段：

```cpp
struct OrderedJsonField {
    std::string_view name;
    std::string value_json;
};
```

`name` 是字段名，`value_json` 是已经编码好的 JSON value。这个命名故意写成 `value_json`，而不是 `value`，就是提醒调用者不要把普通文本直接塞进去。普通文本必须先走 `json_string`，布尔值走 `json_bool`，数字走 `json_integer`，嵌套对象和数组由 builder 或既有 formatter 产生。这样可以避免两类常见错误：把 raw object 二次转义成字符串，或者把普通文本当 raw JSON 注入。

`json_object(std::span<const OrderedJsonField>)` 不自己排序，不使用 `std::map`，也不自动删字段。它只是把输入顺序转给既有 `runtime_evidence::json_object`。这等于复用老的 escaping 和 compact JSON 习惯，同时补上“ordered field list”这个缺口。`json_array(std::span<const std::string>)` 同样只拼接已经编码好的 JSON value，保持紧凑格式，不加多余空白。

第二层是 fixture 子对象提取器。`tests/receipt_fixture_parity.hpp` 不是通用 JSON parser，它的职责更窄：在一个已经提交的 fixture 文本里，按字段名找到某个 object value，并原样返回这段 object 的字节。为了保证失败时不会给出假阳性，它先做结构平衡检查，再处理字符串内的反斜线和引号，最后用花括号深度定位对象结束。它会拒绝空字段名、带引号或反斜线的字段名、目标不是 object、重复字段、未闭合对象和未终止字符串。

这个 helper 选择“提取原始字节”，而不是 `ConvertFrom-Json` 或第三方 JSON 库重新序列化，是因为本轮要保护的是字节合同。结构化解析再重新输出，可能改变字段顺序、空白、转义大小写或数字表现；那样测试通过也不能证明历史 fixture 没变。mini-kv 这种证据型项目最怕的不是 JSON 语义大体相同，而是下游硬编码 digest、路径或片段时，某个看似无害的格式变化让证据链断开。

第三层是测试中的候选状态记录。`runtime_receipt_json_builder_tests` 对两份候选 receipt 做了不同判断：abort/rollback 子对象当前已经与 public formatter byte-for-byte 相等；no-network 子对象当前还不等于 public formatter。这个结论看起来像一个小细节，其实是本版最重要的安全发现之一。它告诉后续 Slice 1：不能把 no-network 直接当作“迁移前就是全等”的样本；要么先定位漂移原因，要么先迁移 abort/rollback 这份更干净的样本。

## 边界字段

`read_only` 在本版里的意义是：新增 builder 和测试只能读取历史 fixture、拼装内存字符串、比较字符串，不能把只读证据变成运行时动作。测试同时检查 fixture 子对象和当前 formatter 输出里仍有 `read_only=true`。

`execution_allowed` 的意义是：receipt 仍然只是说明“不允许执行”。本版没有新增任何执行入口，也没有让 `CHECKJSON` 或 SMOKEJSON 执行危险命令。full CTest 中 `runtime_smoke_evidence_tests`、`release_verification_manifest_tests` 和大量 credential resolver receipt tests 继续通过，说明 execution boundary 没有被打开。

`order_authoritative` 在本版没有新增字段，但历史 receipt 的语义仍然是不让 mini-kv 成为订单权威。mini-kv 只是存储和证据系统，不负责 Java order state，不对 Node 的 managed audit chain 做执行裁决。

`mutates_store` 和 `touches_wal` 没有作为新字段出现，但测试路径保护了它们的反面：builder 测试只处理字符串；fixture parity 只读 committed fixture；full CTest 重新跑过 WAL、snapshot、command、TCP、SMOKEJSON、runtime smoke 和 release manifest。没有新增 Store mutation，也没有新增 WAL append。

`warnings / blockers / diagnostics` 在本版最重要的体现是“显式记录 no-network pre-migration drift”。这不是 runtime warning，而是维护诊断。它让未来维护者知道：v1626 已经发现一份候选不是全等；后续若迁移它，必须先处理这个事实。把这种差异写进测试，比在文档里一句话带过更可靠。

## 测试覆盖

新增测试文件是：

```text
tests/runtime_receipt_json_builder_tests.cpp
tests/receipt_fixture_parity.hpp
```

第一类断言保护 builder 行为。`ordered_object_preserves_scalars_and_escaping` 检查字段顺序、引号、反斜线、换行、制表符、控制字符、false、0、空对象、空数组。它防止后续 builder 为了方便而改成排序 map、跳过 false/0、增加多余空白或改变 escaping。

第二类断言保护 raw JSON 嵌套。`nested_raw_values_are_not_double_escaped` 把嵌套 object 和 array 放进外层 object，验证它们仍然是 JSON object/array，而不是被包成字符串。这个测试对 receipt 重构很关键，因为 runtime receipts 里大量字段本身就是嵌套 section，如果 builder 把它们二次转义，整个 public output 会立刻变形。

第三类断言保护 fixture 提取器。`extractor_handles_nested_objects_and_escaped_strings` 确认字符串里的 `{`、`}` 和转义引号不会干扰对象深度计算；`extractor_fails_closed_on_ambiguous_or_malformed_input` 确认缺字段、目标是数组、外层未闭合、重复字段、未终止字符串、空字段名、坏字段名都会失败。这些测试保证 parity harness 不会在坏输入上给出一个看似正确的片段。

第四类断言冻结历史 fixture 字节。`frozen_fixture_subobjects_have_versioned_bytes` 读取 v141 no-network 和 v142 abort/rollback standalone fixture，提取嵌套 receipt object，然后断言长度和 FNV digest：

```text
no-network: 17738 bytes, fnv1a64:794a730788ce0e87
abort/rollback: 19183 bytes, fnv1a64:0e41aded16909c6d
```

这比只用 `assert_contains` 更硬。`assert_contains` 能证明某个字段在，但不能证明字段顺序、逗号、转义、嵌套层级和 compact whitespace 没变。长度加 digest 则把整个子对象冻结为版本化 oracle。

第五类断言记录候选 formatter 状态。`candidate_formatters_have_explicit_fixture_parity_state` 证明 abort/rollback 现在已经与 public formatter 全等，同时证明 no-network 当前还不全等，但两者当前 formatter 仍保留 read-only、execution_allowed=false 和关键 no-network/no-rollback/no-write/no-auto-start 边界字段。这类断言不是降低标准，而是让迁移计划拥有真实入口条件。

本版还跑了 focused 和 full CTest。focused 覆盖 `runtime_receipt_json_builder_tests`、`receipts_consolidation_note_contract`、`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests`、`release_verification_manifest_tests`，并补跑 `no_network_safety_fixture_contract_receipt_tests` 和 `abort_rollback_semantics_contract_receipt_tests`。full local CTest 最终是 342/342。这个组合证明新 builder 没改变 SMOKEJSON 暴露、release manifest、runtime smoke、历史 receipt focused tests、WAL、snapshot、TCP/RESP 或 command 行为。

## 归档和清理

本版归档放在：

```text
f/1626/图片/
f/1626/解释/说明.md
```

截图会记录设计范围、定向测试、full CTest、质量门和清理门。归档的意义不是展示一个新 UI，而是给后续回看提供可视化证据：v1626 做的是内部维护地基，测试已经证明没有打开 router/write/WAL/network/execution 权限。

清理规则仍然按 AGENTS 执行：本版使用的 `cmake-build-v1626` 是临时构建目录，最终提交前删除；用于截图的临时 HTML/server 也要停止并删除；历史 `a/` 到 `f/` 归档根、`fixtures/release/*.json` 和已有讲解文件不移动、不重命名、不压缩替换。

## 后续迁移的维护判断

v1626 最容易被误读成“只是写了一个 JSON 小工具”。如果只从代码行数看，builder 的确很小；但从维护风险看，它承担的是未来一批大文件重构的验收框架。receipt formatter 的危险点不在算法复杂，而在它们是证据链的一部分：字段名、顺序、嵌套层级、digest 输入、fixture 路径和下游说明文字，都可能被 Node 或人工 review 当作稳定证据引用。这样的代码不能用“语义差不多”来验收，必须用“输出字节没有变”来验收。

本版发现 no-network 候选和当前 public formatter 不全等，恰好说明这个门禁有必要。如果没有这个测试，后续维护者可能会直接把 no-network 和 abort/rollback 一起搬进 builder，迁移后测试仍然可能因为大量 `assert_contains` 通过而显得绿色，但实际上某份历史 fixture 和 formatter 的关系已经不是想象中的全等。v1626 把这种差异前置成可见状态：它没有要求立刻修复，也没有为了通过而改 fixture，而是把“这份候选需要先解释漂移”写成测试事实。对证据型项目来说，这种诚实比短期绿灯更重要。

后续 Slice 1 可以有两条稳妥路线。第一条路线是先迁移 abort/rollback，因为它已经证明当前 formatter 与冻结 fixture 子对象 byte-for-byte 相等；迁移前后只要这个等式保持，builder 的引入就是低风险的。第二条路线是先调查 no-network 漂移：比较历史 fixture 子对象和 formatter 输出，判断差异来自字段顺序、后续追加字段、digest 输入、read command list、还是历史 fixture 生成时的上下文。调查结果应写进测试或文档，而不是藏在 commit message 里。只有当漂移来源被明确记录后，才适合迁移 no-network。

review 本版时也应按这个思路看，而不是只看有没有新功能。第一，看 `runtime_receipt_json_builder.hpp` 是否保持足够小，是否没有引入业务语义、Node 版本判断或 authority 推断。第二，看 `receipt_fixture_parity.hpp` 是否 fail closed，而不是在坏输入上尽量猜一个片段。第三，看测试是否冻结了完整子对象，而不是只检查几个字段片段。第四，看 CMake 是否只把内部 builder 加入库和测试，没有扩大 public include 面。第五，看文档是否诚实记录 no-network drift，没有把 Slice 0 写成 Slice 1 已完成。

这个维护判断还能防止另一种反向风险：过度抽象。receipt 家族有大量相似字段，但并不是所有字段都应该进入一个万能 `ReceiptSpec`。如果为了省几行，把每份 receipt 的领域段、来源段、检查段、recommendation 段都压成一个巨型配置对象，那么只是把 26 个长 formatter 变成一个更难审查的长 schema。v1626 的 builder 刻意只做 JSON 拼装，不做业务推理，就是为了让后续每一刀仍然能被普通 reviewer 看懂：领域数据留在领域文件里，共同拼装交给小工具，历史字节由 fixture parity 证明。

还要注意，本版的 no-write/no-execution 证明不是靠口头承诺，而是靠路径选择。新增代码没有包含 `Store`，没有调用 command dispatch，没有打开 TCP server，没有创建 WAL writer，也没有碰 snapshot/restore 逻辑。它的输入来自常量、测试 fixture 文本和 public formatter 返回字符串；它的输出只进入测试断言。full CTest 重新跑过 WAL、snapshot、TCP/RESP 和 runtime smoke，是为了证明“没有直接调用”之外，构建链接和现有运行表面也没有被间接改变。

因此，v1626 给后续版本留下的最重要资产不是某个 helper 函数，而是一套迁移节奏：先测 builder 的最小字节行为，再冻结历史 fixture 子对象，再记录候选 formatter 是否全等，再选择低风险样本迁移，再让 full CTest 和远端 CI 兜底。只要后续版本继续遵守这个节奏，runtime receipts 的治理就会从“文档很厚、代码很长、大家凭经验不敢动”逐步变成“每一刀都有明确 oracle、明确停止条件、明确回滚点”的工程化维护。

## 一句话总结

v1626 给后续 Node/Java/mini-kv 松耦合融合带来的价值是：它把 credential resolver receipt 的后续重构从“看起来可以抽象”推进到“先有 ordered builder、先冻结 fixture 字节、先识别候选漂移、再逐刀迁移”的可验证工程路线。
