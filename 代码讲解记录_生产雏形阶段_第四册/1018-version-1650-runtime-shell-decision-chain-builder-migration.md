# v1650 代码讲解：runtime-shell 决策链三 formatter 有序迁移

## 一、本版目标、角色定位和明确不做事项

v1650 延续 v1648 建立的五回执冻结基线与 v1649 完成的链首迁移，把同一条 runtime-shell 证据链剩余三份 formatter 迁入 ordered JSON builder。三份对象分别是 decision record、post-decision plan intake、stop-or-prerequisite receipt。它们不是三个互不相关的小改动，而是从“候选门已阻塞”到“阻塞后的计划选择”，再到“缺少前置条件时停止”的连续决策链。把它们放在同一版，能够让同一份 exact-byte oracle 一次裁决同构迁移，也能在一次审查中比较三个阶段哪些字段相同、哪些字段必须由各自领域拥有。

本版是合同保持型重构，不新增 runtime shell，也不让已有 receipt 获得执行能力。它不是新命令、不是 router、不是 approval controller，不实例化 secret provider 或 resolver client，不读取 credential value，不解析 raw endpoint URL，不发起 HTTP/TCP，不连接 managed audit，不写 Store、WAL、approval ledger 或 schema，不执行 LOAD、COMPACT、RESTORE、SETNXEX，不自动启动 Node、Java 或 mini-kv，也不成为 audit authority 或 order authority。公有 formatter 签名、digest parts、fixture 文件、命令路由和下游消费入口全部保持原样。

## 二、为什么本版选择这三份，而不是继续单文件推进

v1648 已经分别冻结三份对象的完整字节、顶层字段数和版本化摘要。decision record 是 14208 字节、113 个顶层字段、摘要 `fnv1a64:9926939aac05efd6`；post-decision plan intake 是 14084 字节、113 个字段、摘要 `fnv1a64:e6c73dbf0f4bc4d1`；stop-or-prerequisite 是 16306 字节、115 个字段、摘要 `fnv1a64:b6984418a3b69de7`。三份 runtime 输出在迁移前都与 frozen fixture 子对象逐字节相等，双方的 `\u0027` 出现次数都是零，没有历史 apostrophe 或 whitespace 例外。

这使它们满足同一批次的四个条件：来源属于连续 Node 决策链，顶层 schema 形状相近，安全边界尾部完全同序，且已有独立机械门。Claude 检查点允许在冻结门存在时批量迁移同构 formatter，但没有要求把全部 pending 文件一次改完。本版只做三份，既比逐文件重复搭建证据更有工程价值，又没有跨到 input-hardening、archive 或通用 runtime evidence 等不同领域。

## 三、旧实现真正难维护的地方

三份旧 formatter 都从一个很长的 `return` 表达式开始，依靠几十段字符串拼接构造完整 JSON。Node 来源对象、mini-kv receipt、summary、warnings、recommendations、endpoints、next actions、digest 和一百多个顶层边界字段混在同一个表达式中。阅读者必须同时追踪花括号层级、前导逗号、字符串转义和字段所有权，编译器却只能检查 C++ 字符串是否合法，不能判断某个 JSON 字段是否被放错对象。

每个文件还各自维护一份 `format_closed_boundary_flags_json`。其中从 `ready_for_disabled_runtime_shell_implementation` 到 `runtime_shell_implementation_allowed` 的 readiness 区块，以及从 `disabled_runtime_shell_participates` 到 `order_authoritative` 的 non-participation 尾部，与 v1649 抽出的字段完全相同。继续复制会让未来的边界修复出现“改了两份、漏了三份”的风险；做一个能按布尔参数开关字段的万能 helper 又会把 schema 差异藏起来。本版因此复用 v1649 的两个无参数 helper，只共享经过逐字段比较确认完全同名、同值、同序的部分。

## 四、ordered builder 如何保持原始字节合同

`runtime_receipt_json_builder` 不排序字段，也不根据类型推断默认值。`OrderedJsonField` 只保存字段名和已经编码好的 JSON value，`json_object` 按 vector 或 initializer list 的现有顺序输出紧凑对象。普通文本必须经过 `json_string`，整数经过 `json_integer`，布尔经过 `json_bool`；已经返回完整 `{...}` 或 `[...]` 的领域 helper 则直接作为 raw JSON value 放入，不能再套 `json_string`。

这种 API 的关键不是“更漂亮地生成 JSON”，而是把顺序变成显式数据。每个 nested object 在局部变量中有清楚名字，顶层 `std::vector<OrderedJsonField>` 又按历史 fixture 顺序列出各对象。最后通过 `std::span<const OrderedJsonField>` 同步渲染，span 不逃逸函数，vector 在渲染完成前一直存活。字段名来自字符串字面量，value 由 `std::string` 自己持有，因此不会出现临时 string_view 悬空问题。

## 五、decision record formatter 的输入与输出

入口仍是 `format_credential_resolver_runtime_shell_decision_record_non_participation_receipt_json(read_commands)`。唯一运行期输入是只读命令列表，formatter 只对列表做确定性摘要，不执行命令。其他输入来自编译期常量、项目版本、binary provenance、retention check 和 replay marker。digest 函数及其 parts 未改，因此本版不能借重构重新定义 receipt identity。

新实现先构造 `source_node_v299_reference`。这里保留 blocked decision、required evidence、no-go conditions、计数和所有禁止实现的字段。随后构造 `source_node_v298_reference`，记录 upstream echo、Java v134、mini-kv v131 与五类 gate 计数。`mini_kv_receipt` 只声明 non-participation 范围；`summary`、warning、recommendations 和 evidence endpoints 各自成为独立对象或数组。

顶层 digest 之后，decision record 自己拥有 `decision_record_state=runtime-shell-candidate-gate-decision-record-ready`、`runtime_shell_decision=blocked`、两个 source consumption 和 Node v300 readiness。随后才调用共同 readiness 与 non-participation helper。这个顺序不能按语义随意调换，因为控制面既消费字段值，也通过 frozen object 维护历史证据字节。

## 六、post-decision plan intake formatter 的职责

第二个入口接收相同类型的 `read_commands`，但领域含义不同。`source_node_v301_reference` 不是再次发布 blocked decision，而是在 blocked 前提下选择 `continue-blocked-planning`。它记录四个 continuation options、唯一 selected option、被拒绝的立即实现选项，以及为什么 Node v300 不能直接复用。`format_continuation_options_json` 仍然保留为独立 raw array helper，因为它内部是一组完整、单一职责的领域选项，没有在多个 formatter 中重复，也没有造成顶层花括号难以判断。

`source_node_v300_reference` 负责证明 blocked decision、required evidence、no-go conditions 和 side-effect boundaries 已对齐。顶层专属字段包括 plan-intake-only 标记、plan intake state、selected continuation decision、两个 source consumption，以及 Node v302 只能做 upstream echo、不能做 runtime implementation 的 readiness。共同安全尾部从这些领域字段之后开始，输出顺序与 v1648 fixture 一致。

## 七、stop-or-prerequisite formatter 为什么最需要结构化

第三份对象最长，因为它不只说“仍然阻塞”，还公开六项缺失前置条件和八项 no-go condition。六项前置条件分别覆盖 operator approval artifact、credential handle readiness、raw endpoint allowlist review、no-network safety tests、manual abort/rollback semantics，以及 Java/mini-kv prerequisite echo。它们都是 `documented-missing`，用于解释为什么 runtime shell 不能实现，不是给实现层提供凭据或连接信息。

`source_node_v304_reference` 还保留 selected path、stop condition、blocker、consumer、不能复用 v303 的理由和 existing report reuse decision。`source_node_v303_reference` 只说明上一阶段 upstream echo 已对齐且 implementation 仍阻塞。结构化以后，这两个来源不再夹在一个巨型字符串里，审查者可以直接看出 prerequisite 决策属于 v304，而 echo 对齐属于 v303。

顶层专属字段明确 `stop_runtime_shell_chain_without_prerequisites=true`，但同时 `ready_for_node_v305_stop_prerequisite_upstream_echo_verification=true`。这两个 true 不能被误读成 execution approval：紧接着的 `ready_for_node_v305_runtime_shell_implementation=false` 和共同 readiness 全部关闭实现、调用与连接。builder 只是忠实输出这组已冻结的控制面含义，不根据“ready”字样自动放开任何行为。

## 八、共同 helper 的边界与没有过度抽象的部分

本版继续使用 `runtime_credential_resolver_disabled_runtime_shell_receipt_fields.hpp`。这个名字指向整个 credential-resolver-disabled-runtime-shell family，而不只指 v1649 的第一份 receipt。三个新 formatter 的 `shell_name` 仍是同一个 disabled runtime shell，所以复用在领域上成立。为了减少同版无关改名和扩大 diff，本版没有仅为名字美观而移动 header 或改 namespace。

两个 helper 都没有参数。`append_runtime_shell_readiness_fields` 固定追加实现、调用、managed-audit adapter 和生产 readiness 的 false 值；`append_non_participation_tail_fields` 固定追加 provider/client、credential、raw endpoint、network、storage、approval、schema、restore、auto-start 和 authority 字段。三份 formatter 的 decision/plan/prerequisite 字段仍在调用点显式存在。

本版没有把 `format_required_evidence_json`、`format_no_go_conditions_json`、`format_continuation_options_json`、`format_required_prerequisites_json` 和 `format_checks_json` 强行改成 builder。这些 helper 已返回边界完整的 JSON value，职责清楚且没有跨文件重复。把它们同时重写会增加审查面，却不会改善本版的主要风险。以后只有在它们出现真实重复、需要独立类型检查或继续增长时，才应作为单独版本迁移。

## 九、关键边界字段应怎样被控制面理解

`read_only=true` 表示证据可以被读取、比较和归档；`execution_allowed=false` 表示不能由它触发动作。`runtime_shell_implemented=false`、`runtime_shell_enabled=false`、`runtime_shell_invocation_allowed=false` 分别否定实现、启用和调用。`ready_for_node_v300/v302/v305_*_upstream_echo_verification=true` 只授权下一步做证据核验，不授权 runtime implementation。

credential 组继续把 value read、provided、loaded、stored、included、rendered 全部关闭；raw endpoint 组把 parse、render、provided、included 关闭；network 组保持 external request、HTTP/TCP dial 和 managed-audit connection 为 false。write 组否定 storage write、write/admin command、runtime write、approval ledger、managed audit write 和 production record。schema/restore 组否定 migration、rehearsal、LOAD/COMPACT/RESTORE 与 SETNXEX execution。

auto-start 组否定 Node、Java、mini-kv 和 automatic upstream start。最后 `audit_authoritative=false` 与 `order_authoritative=false` 保证 mini-kv 只提供存储与恢复证据，不接管审计或订单裁决。`approval_gate_required=true` 与 `approval_gate_satisfied=false` 必须成对理解：存在审批要求，当前并未满足，不能只摘取前一个字段宣称流程已具备执行资格。

## 十、机械证据如何避免重构自证

验证顺序先改 production，再用未修改的 v1648 baseline 和领域测试裁决。第一次 focused gate 同时运行 census、family baseline 和三份领域测试，结果 5/5。family baseline 不做解析后等价比较，而是检查完整对象字节、固定大小、固定摘要、顶层字段数、核心 marker 和 fixture/runtime 原始 equality。因此少一个空格、换一个转义、移动一个字段都会失败。

第二层 focused gate加入通用 builder、SMOKEJSON、runtime smoke evidence 和 release verification manifest，共 9/9。它证明三份 public formatter 不仅单独正确，也能继续嵌入聚合命令、运行时证据和发布清单。随后全目标构建成功，文档前 full CTest 为 346/346，覆盖 Store、WAL、snapshot、RESP/TCP、OSFS、shard readiness、归档规则和其他 receipt family。本版没有改测试期望，也没有修改 `fixtures/release` 下任何文件。

这里坚持原始字节而不只比较解析后的 JSON，有明确工程原因。普通 JSON parser 通常把对象字段视为无序映射，也会把 `\u0027` 与直接 apostrophe 解析成同一个字符；如果测试只做结构等价，字段换位、历史转义变化和紧凑格式漂移都会被隐藏。但这些 receipt 还被 release fixture、摘要和跨项目归档当作可重放证据，原始表面本身就是版本合同。exact-byte 门让 builder 的结构改善不能越过这条合同线，同时又不让 production formatter 读取 fixture 来“照抄答案”。

反过来，字节相等也不能单独证明行为安全，所以领域测试和真实 CLI 仍不可省略。前者确认字段在控制面中的含义，后者确认聚合命令只输出证据，没有触发存储或网络动作。三层证据分别回答“字节是否稳定”“合同是否完整”“真实入口是否仍然只读”，不能互相替代。

## 十一、维护指标与 census 的实际变化

按物理行统计，decision record 源文件从 420 行降到 389 行，post-decision plan 从 402 行降到 382 行，stop-or-prerequisite 从 453 行降到 440 行，三份合计减少 64 行。diff 本身有 765 行新增和 829 行删除，原因是把一条巨型字符串改成逐字段结构，而不是用行数伪装小改动。真正收益是三个 nested source、summary、数组和顶层边界各有明确所有者，逗号、引号与字符串 escaping 由统一 builder 负责。

CMake census floor 从 11 收紧到 14。当前口径是 28 个 receipt sources、27 个 formatter owners、14 个 builder-backed、13 个 pending 和 1 个没有 formatter 的命名 waiver。三份迁移文件必须实际 include builder 才会计数；共享 header、测试和文档不会虚增完成量。未来若有人退回字符串拼接、删除 builder include 或增加未记账 owner，`receipt_builder_census_contract` 会失败，ratchet 不能调低来迁就回归。

## 十二、每类测试分别保护什么

`runtime_receipt_runtime_shell_family_baseline_tests` 保护三份完整历史对象的 bytes、digest、field count 和零 canonical exception；三个领域 receipt tests 保护 source references、版本提示、standalone fixture path、receipt digest 与命令输出；`runtime_receipt_json_builder_tests` 保护 escaping、raw nested JSON、ordered rendering 和 fail-closed extraction；census 保护迁移进度不回退。

`smokejson_command_receipt_tests` 保护三份 receipt 仍进入真实 SMOKEJSON 聚合；`runtime_smoke_evidence_tests` 保护运行时证据表面；`release_verification_manifest_tests` 保护发布清单消费。full CTest 把本次局部结构变化放回全仓库语境。最终真实 CLI smoke 还要直接运行本版 `minikv_cli`，确认 marker、read-only、deny-execution、no-network 和 no-write 字段存在，并确认 `CHECKJSON LOAD` 只分类、`touches_wal=false`，不会执行 LOAD。

## 十三、对抗性自审与失败条件

最强质疑是三份 schema 看起来相似，是否被错误地套进 v1649 helper。应对方式不是口头保证，而是逐字段比对与 exact parity：helper 只覆盖三份完全同序的 readiness 和 tail；每份专属决策字段留在调用点；完整 frozen object 对任何字段遗漏、顺序变化或值漂移都会失败。如果未来其中一份边界分化，应把差异移回本地或新增语义明确的 helper，不能给现有 helper 增加布尔开关形成万能 schema。

第二个质疑是本版是否只为了提高 census 数字。答案由维护变化和消费测试共同判断：三条巨型顶层拼接已消失，三个重复 boundary fragment 已删除，领域对象可单独审查，三份输出仍通过 exact bytes 和全部下游。census 只是防回退机械门，不是价值本身。

失败条件保持严格：fixture 出现任何 diff，停止；需要更新历史 size、digest 或 field count，停止；需要新增 whitespace/apostrophe waiver，停止；census 低于 14，停止；任何 credential、raw endpoint、network、write、WAL、schema、restore、auto-start、execution 或 authority 字段被放开，停止；需要修改旧测试期望才能通过，停止。版本不能用文档、截图、tag 或 CI 数量掩盖上述任一失败。

## 十四、后续路线和本版价值

v1650 结束后，五份 runtime-shell family formatter 已全部在 ordered builder 下，v1648 的 family baseline 继续作为长期防漂移门。pending 还剩 13 份，但它们不再都属于同一个同构 family。下一版应重新做结构 census，优先选择已有 exact parity、字段所有权清楚且不会与 input-hardening 或 archive receipt 混批的对象；达到计划要求的 18 个 builder-backed floor 时必须停在 Claude 检查点，不应为了连续版本目标越过评审。

一句话总结：v1650 把 runtime-shell 的 blocked decision、blocked continuation plan 与 stop/prerequisite 三段连续证据从巨型字符串迁到有序、可审查、可防回退的结构化 builder，在 fixture、digest、字段顺序和公有命令完全不变的前提下，把 census 收紧为 14/13/1，并继续证明 no router、no write、no WAL、no network、no credential、no execution 与非权威边界。
