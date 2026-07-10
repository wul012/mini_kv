# v1649 代码讲解：runtime-shell 链首双 formatter 有序迁移与边界职责拆分

## 一、本版目标、角色和“不是什么”

v1649 在 v1648 已建立的五回执 exact-byte 基线上，只迁移 runtime-shell 连续链最前面的两份生产 formatter：disabled runtime shell non-participation receipt，以及 disabled runtime shell candidate gate non-participation receipt。两份旧实现都依赖一条很长的字符串连接表达式，同时各自维护近似重复的安全边界字段。现在顶层结构、Node 来源对象、summary、warnings、recommendations、evidence endpoints 和 next actions 都改由 ordered JSON builder 组装，真正相同的边界字段放入一个本 family 专属的 testable helper。

本版属于合同保持型重构，不新增 runtime-shell 功能，也不改变任何 receipt 业务含义。它不是新的 command，不是 router，不是审批入口，不会实例化 secret provider 或 resolver client，不读取 credential value，不解析 raw endpoint URL，不发送 HTTP/TCP，不连接 managed audit，不写 Store、WAL、approval ledger 或 schema，不执行 LOAD、COMPACT、RESTORE、SETNXEX，不自动启动 Node、Java 或 mini-kv，也不取得 audit authority 或 order authority。冻结 fixture、digest 输入、公有函数签名和下游消费路径均保持不变。

## 二、执行前的外部约束怎样影响本版

本版开始时先确认 v1648 GitHub CI 六个 job 全部成功，再读取 Node `docs/plans/README.md`、最新 `production-excellence-node-playbook.md` 和当前全局 v322 路线图。Node playbook 当前工作属于 Node 自身 renderer/N5 范围；v322 路线图继续明确禁止 raw endpoint 解析、managed-audit 连接和 runtime shell 启用。因此 mini-kv 的内部 formatter 重构不引入跨项目合同依赖，可以与 Node 维护并行。

同时，Claude 在 `治理计划/v1637-production-excellence-completion-brief.md` 写入 v1643-v1648 检查点评审，结论为 PASS。评审确认 per-family 具名兼容规则、census CTest 和整份 formatter/family 粒度均符合要求，并允许在冻结门存在时按 4 到 8 个同构文件批量推进。v1649 没有把这个许可理解成“可以一次改完全部 18 个 pending”，而是只选链首两份。它们同属 disabled runtime-shell family、已被同一 v1648 测试冻结，并共享大段同序安全字段，适合作为一个可审查闭环。

## 三、旧实现的维护风险在哪里

旧的 `format_credential_resolver_disabled_runtime_shell_non_participation_receipt_json` 从 receipt identity 开始，用连续 `+` 把 Node v295 设计审查、Node v294 pre-plan、mini-kv receipt、checks、summary、warnings、recommendations、endpoints、digest 和顶层边界拼成一个字符串。candidate gate 版本用同样方式拼 Node v297 gate、Node v296 upstream echo 和近百个顶层字段。JSON 的花括号、逗号和字段归属只能靠人工追踪字符串片段。

这种代码当前能工作，但修改成本很高。一个字段如果从某个 nested object 挪到顶层，编译器无法发现；少一个逗号会让整个对象损坏；在两个 closing brace 之间多一个空格，普通语义解析仍然成功，却会破坏 frozen bytes 和 digest 证据。v1647 已真实遇到后一类问题。更麻烦的是两份 `format_closed_boundary_flags_json` 大部分字段相同，但每份都复制一遍，未来修一边漏一边的概率会随版本增加。

## 四、为什么不是做一个“万能边界 helper”

两份回执的边界并非完全相同。链首 disabled receipt 在专属字段之后直接进入共同 non-participation tail；candidate gate 在共同 readiness 字段之后，还必须保留 `runtime_prerequisite_satisfied=false`、`implementation_allowed=false`、`implementation_candidate_allowed=false` 三个 gate 决策字段，然后才能进入共同 tail。如果用一个带多个布尔参数的 helper 控制是否插入这些字段，调用点会变短，但真实 schema、顺序和组合语义会被隐藏在条件分支里。

新 header `runtime_credential_resolver_disabled_runtime_shell_receipt_fields.hpp` 因此只提供两个无参数函数。`append_runtime_shell_readiness_fields` 负责从 `ready_for_disabled_runtime_shell_implementation` 到 `runtime_shell_implementation_allowed` 的共同 readiness 区块；`append_non_participation_tail_fields` 负责从 `disabled_runtime_shell_participates` 到 `order_authoritative` 的共同尾部。candidate formatter 在两个调用之间显式 push 三个 gate 字段，disabled formatter 则直接连接前后两段。

这个拆法把“共同字段”与“插入点”同时暴露出来。header 中唯一的说明注释解释 candidate gate 为什么要在两段之间插入字段，没有引入 schema 配置对象、枚举组合或布尔开关。以后若第三份回执只与其中一段相同，可以复用那一段；若字段名、值或顺序不同，就应保持本地所有权，而不是为了表面 DRY 扩展 helper。

## 五、disabled runtime shell formatter 如何重组

public 入口仍是 `format_credential_resolver_disabled_runtime_shell_non_participation_receipt_json(read_commands)`。输入仍是标准只读命令列表，digest 函数和 digest parts 一行未改。新实现先构造 `source_node_v295_reference`，把 review identity、counts、blocked flags、review questions 和 stop conditions 放在一个命名 object 中；再构造 `source_node_v294_reference`，完整保留 pre-plan fields、next versions 和 side-effect false flags。

`mini_kv_receipt`、`summary`、`warnings`、`recommendations` 和 `evidence_endpoints` 分别成为局部对象或数组。旧 helper `format_review_questions_json`、`format_stop_conditions_json` 和 `format_checks_json` 仍返回已编码 JSON，因为它们拥有清楚的领域职责，并未造成顶层结构难以审查。builder 把这些值作为 raw object/array value 使用，不会经 `json_string` 二次转义。

最后，顶层 `std::vector<OrderedJsonField>` 按 fixture 历史顺序放入 identity、source references、mini-kv receipt、checks、summary、告警建议、endpoints、next evidence/actions 和四类 digest。随后先写本回执专属的八个模式/来源字段，再调用 readiness 与 non-participation 两段 helper，最后加入 `boundary` 和 `node_action`。`json_object(span)` 统一生成 compact JSON，public 返回值类型、生命周期和调用者均不变。

## 六、candidate gate formatter 如何保持自己的领域结构

candidate formatter 先把 necessity proof 独立为 `necessity` object，其中保留 blocker、consumer、不能复用旧报告的理由和 stop condition。它与 required gates、stop conditions、计数和 blocked flags 一起进入 `source_node_v297_reference`。Node v296 upstream echo 则独立构造，避免 planned Java v132、actual Java v133、consumed mini-kv v130 和 side-effect flags 串到 v297 对象中。

mini-kv receipt 继续说明自身只提供 candidate-gate non-participation evidence；summary 的 required/documented/review-satisfied/runtime-prerequisite/implementation-allowed 计数顺序不变。warnings、recommendations、endpoints 和 next actions 仍输出同一文本与同一 compact 字节。所有固定字符串经过 `json_string`，整数使用 `json_integer`，布尔使用 `json_bool`，领域 helper 生成的完整 arrays/objects 作为 raw JSON 嵌入。

顶层专属字段在 digest 后依次记录 read-only、candidate-gate-only、两个 source consumption 和 Node v298 readiness。共同 readiness helper 结束后，新实现显式追加三项 gate false 字段，再调用共同 non-participation tail。这个顺序是本版最关键的结构差异；若把三项移到 tail 之后，JSON 语义解析可能仍然相同，但 v1648 的 raw byte comparison 和 114-field fixture 门会失败。

## 七、从输入到输出的完整机理

两份 formatter 的直接输入都只有 `read_commands`。调用者通常传入 `INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON`。formatter 不执行这些命令，只调用 `read_command_list_digest` 对命令数量和文本做确定性摘要，并把摘要作为 receipt 的一项证据。其他输入来自编译期常量、version、binary provenance、retention check 和 retention replay marker。

内部处理分四层。第一层是领域常量和 digest，确认输出属于哪个历史 Node decision。第二层是 nested object/array 构造，明确每个字段的所有者。第三层是 ordered top-level vector，冻结原始字段顺序。第四层是 family 边界 helper，按调用顺序追加 shared fields。最终 `json_object` 只负责 JSON 语法、字符串转义和逗号处理，不推断业务字段，也不自动添加默认权限。

输出仍是单个 `std::string` receipt object。领域测试直接消费它；SMOKEJSON 把它嵌入 runtime 聚合；runtime smoke evidence 和 release verification manifest 又消费相同 public formatter。冻结 fixture 不参与生产输出生成，只在测试中由 `extract_json_object_field` 取出对应历史对象，与 runtime string 做原始字节比较。因此 production 不依赖 fixture 文件，测试也不会把 fixture 内容复制进 builder。

## 八、v1648 基线如何证明没有漂移

`runtime_receipt_runtime_shell_family_baseline_tests` 在 v1648 已冻结五份对象。本版迁移的 disabled receipt 必须继续保持 12523 字节、110 个顶层字段和摘要 `fnv1a64:08807cb6305602e8`；candidate gate 必须保持 14936 字节、114 个顶层字段和摘要 `fnv1a64:d1a2681a73de4b8e`。两份 fixture/runtime 的 `\\u0027` 计数仍为 0，不存在可借用的 canonical spelling waiver。

迁移后第一次冷编译即通过 exact baseline，没有先修改测试、没有重算 fixture 摘要、没有新增 whitespace normalizer。随后两份领域测试和 census 同时通过，说明 public fields、digest、version hints 和 command response 仍可用。扩展 focused lane 加入通用 builder、SMOKEJSON、runtime evidence 和 release manifest 后为 8/8；第一次 full CTest 为 346/346。这一执行顺序把“实现先变、旧门验证”保留下来，避免实现与期望一起修改形成自证循环。

## 九、关键边界字段怎样被控制面理解

`read_only=true` 表示 receipt 可以读取和比较，不能驱动动作；`execution_allowed=false` 是最终执行否定。`runtime_shell_implemented=false`、`runtime_shell_enabled=false`、`runtime_shell_invocation_allowed=false` 分别否定实现、启用和调用，不能因为某个 `ready_for_node_vNNN_*` 字段为 true 就绕过。ready 只表示下一步证据核验条件满足，不等于 runtime approval。

credential group 继续把 value read/provided/loaded/stored/included/rendered 全部关闭；raw endpoint group 继续把 parse/render/provided/included 关闭；network group 保持 external request、HTTP/TCP dial 和 managed-audit connection 为 false。write group 保持 storage write、write/admin commands、runtime write、approval ledger、managed audit write 和 production record 为 false。schema/restore group保持 migration、rehearsal、LOAD/COMPACT/RESTORE 和 SETNXEX 不可执行。

auto-start group继续否定 Node、Java、mini-kv 和 automatic upstream start；最后 `audit_authoritative=false` 与 `order_authoritative=false` 否定裁决权。共同 helper 只是把这些已冻结的 false/true 值按历史顺序放进 vector，不会根据环境变量或上游状态动态翻转。真实 CLI 的 CHECKJSON 仍只识别写命令风险，不会执行 LOAD 或触碰 WAL。

## 十、维护指标与 census 为什么这样计算

disabled formatter 文件从 385 行降到 358 行，candidate gate 从 423 行降到 403 行；共同边界 header 为 91 行。两个领域文件都删除了各自约百行的 `format_closed_boundary_flags_json`，同时因 nested object 显式化增加了结构行。总行数不是唯一目标，真正改善是字段归属可读、JSON 语法由 builder 负责、共同安全 schema 只有一个来源、candidate 插入点可见。

builder census 从 9 收紧到 11，pending 从 18 降到 16，waiver 仍为唯一命名文件。CMake floor 由 9 改为 11，因此未来若任何一个迁移文件删除 builder include 或退回旧字符串，CTest 会失败。新 header 本身不计入 builder-backed；只有拥有 public formatter 且真正 include builder 的两个生产 source 被计数。这避免用 helper、测试或文档数量虚增迁移进度。

## 十一、字段值所有权与 `span` 生命周期

`OrderedJsonField` 的 name 是 `std::string_view`，value 是拥有内存的 `std::string`。本版传入的字段名都是字符串字面量，生命周期覆盖整个程序；`json_string`、`json_integer`、`json_bool` 和 nested `json_object` 则各自返回完整 value string，由 vector 元素持有。因此局部的 source object、summary 和 arrays 即使在加入 vector 后不再单独使用，顶层字段仍保有自己的值，不会引用已经销毁的临时字符串。

最终调用把 `fields.data()` 与 `fields.size()` 包装为 `std::span<const OrderedJsonField>`。span 本身不拥有 vector，但 `json_object` 在同一表达式中同步遍历并生成返回字符串；函数返回前 vector 一直存活，所以没有悬空访问。这里没有把 span 保存到全局、lambda 或异步任务。若未来 builder 改成延迟渲染，当前生命周期假设就会失效，必须修改 API，而不能继续借用局部 vector。

raw nested JSON 与普通文本的处理也必须分开。`format_checks_json()` 和 `format_required_gates_json()` 已经返回 `{...}` 或 `[...]`，应直接作为 `value_json`；如果误用 `json_string`，整段对象会变成带引号的文本，fixture parity 会失败。相反，boundary、node action、failure reason 等普通文本必须经过 `json_string`，否则内部引号、反斜杠或控制字符可能破坏 JSON。整数不能手写到字符串片段中，使用 `json_integer` 才能让字段类型和十进制输出一致；布尔同理由 `json_bool` 固定为小写 `true/false`。

共享 helper 接收 vector 引用并立即 push 拥有值的字段，不返回指向局部数组的 view，也不缓存调用者容器。两个 helper 之间 candidate formatter 继续操作同一个 vector，因此三项专属字段的插入顺序由真实容器顺序决定。这样的所有权模型比返回“去掉花括号的 JSON fragment”更直接：没有 substring、没有尾逗号约定，也不需要调用者猜测 fragment 是否已含前导逗号。

## 十二、每类测试分别保护什么

family baseline 保护 frozen bytes、size、digest、field count 和核心边界；两个领域测试保护 source Node shape、standalone fixture path、release hints、receipt digest 和 command response；通用 builder 测试保护 escaping、nested raw JSON 和 extractor fail-closed；census 防止迁移进度回退。SMOKEJSON、runtime smoke evidence 和 release manifest 分别保护三个下游嵌入表面。

full CTest 再覆盖 Store、command、WAL、snapshot、RESP/TCP、OSFS、shard readiness、archive retention 和其他 receipt family。真实 CLI smoke 直接运行本版 `minikv_cli`，确认两份 marker 存在、read-only/deny-execution 字段仍大量出现，CHECKJSON 对 LOAD 只分类且 `touches_wal=false`，QUIT 返回 BYE。截图与讲解只归档这些机械结果，不替代测试。

## 十三、对抗性自审与失败条件

最强质疑是共享 helper 是否把两个并不完全相同的 schema 错误合并。应对证据有三层：helper 被拆成前后两段；candidate 三字段留在调用点；整对象 exact parity 会对任何顺序或字段差异失败。若未来两份 schema 分化，不能给 helper 增加一串布尔开关，应把差异字段移回各自 formatter，或新建语义明确的第三段。

第二个质疑是 raw helper arrays 仍为字符串，是否算迁移不完整。本版目标是消除顶层和 nested ownership 的巨大连接表达式，并保持 byte contract。`format_required_gates_json` 等 helper 已有单一领域职责、返回完整 JSON 值，而且 builder 明确支持 raw nested values。把每个小数组同时重写会扩大审查面，却不增加当前证据；可在未来有重复或独立测试价值时单独结构化。

失败条件保持严格：fixture 有任何 diff，停止；baseline size/digest/field count 或 whole-object parity 失败，停止；需要通用 whitespace/apostrophe normalizer，停止；census 低于 11，停止；任何边界从 false 变 true，停止；需要 runtime shell、credential value、raw endpoint、network、write、migration、restore 或 authority，停止。不能通过修改旧测试期望或 fixture bytes 让迁移变绿。

## 十四、后续最优路线

链首两份迁移成功后，下一组自然候选是 decision record、post-decision plan intake 和 stop-or-prerequisite 三份连续回执。v1648 已对它们冻结 14208/14084/16306 字节和 113/113/115 字段，也没有 apostrophe waiver。下一版应先比较三份边界区块是否真的同序，再决定共享 helper；不能因为名称相近就复制本版 helper，更不能跨过 Claude 设定的 floor 18 检查点。

一句话总结：v1649 在不修改 fixture、digest、public command 或安全边界的前提下，把 runtime-shell 链首两份巨型字符串 formatter 迁到可审查的 ordered builder，用两段无参数 family helper 消除真实重复，并以 exact bytes、11/16/1 census、focused/downstream/full tests 和真实 CLI 继续证明 no router、no write、no WAL、no network、no credential、no execution 与非权威边界。
