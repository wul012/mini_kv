# v1655 代码讲解：implementation chain 五个 owner 有序构建迁移

## 一、本版目标、角色定位与明确不做事项

v1655 的目标是把 v1653 已冻结的 implementation chain 五份公开 receipt formatter 从手写长字符串组装迁移到内部 ordered JSON builder。五份对象按上游状态顺序分别是：审批前实现就绪审查、实现计划草案、禁用 fake harness 合同、fake harness 执行拒绝预检、实现候选门输入加固决策。它们不是五个互不相关的小改动，而是一条连续的“材料逐渐完备，但执行权限始终关闭”的证据链，因此放在同一版本内可以同时看清状态如何推进、边界为何不推进。

本版仍只是 mini-kv 内部可维护性重构。它不是 credential resolver 实现，不新增 CLI 命令、TCP 路由或 HTTP endpoint，不实例化真实或假的 secret provider、resolver client，不读取、保存、渲染 credential value，不接受或解析 raw endpoint URL，不发送 HTTP/TCP，不连接 managed audit，不让 mini-kv 成为 router、storage backend、audit authority 或 order authority。它不写 Store、WAL、approval ledger、schema 或 production record，不执行 LOAD、COMPACT、RESTORE、SETNXEX、rollback、deployment 或 Java SQL，也不自动启动 Node、Java、mini-kv 或外部服务。公有 formatter/digest 签名、冻结 fixture、测试期望、SMOKEJSON 字段和 release 消费表面全部保持不变。

## 二、为什么这五份对象适合一次闭环

readiness receipt 回答“实现前还缺哪些审批材料”；implementation plan receipt 把缺口展开为 config handle、credential handle、endpoint handle、approval artifact、failure taxonomy、rollback guard 和 test-only fake harness 七类接口边界；disabled fake harness receipt 在计划之后定义一份默认关闭、不可调用的测试合同；execution-denied receipt 则用八次模拟尝试证明，即使有人把 route 当成执行入口，approval、credential、endpoint、provider/client、network、ledger/schema、runtime 和 auto-start 八条路径仍被拒绝；input-hardening receipt 最后要求 Java 与 mini-kv 提供稳定输入导出，但明确不因此获得 runtime shell 设计许可。

这五份对象共同的核心不是“最终能执行”，而是“每增加一层证据，仍能机械证明不能执行”。若把它们拆成五个过小版本，每一版都要重复完整构建、全量 CTest、CLI、截图、讲解和 CI，工程仪式会掩盖真正的链路关系；若再把 sandbox receipt 混进来，又会引入 manual dry-run 三字段 frozen metadata 的独立历史规则，使风险面过大。因此 v1655 只覆盖 exact-parity 的 implementation chain，下一版才处理 sandbox chain。

## 三、迁移前 oracle 怎样成为独立裁判

`tests/runtime_receipt_remaining_owner_baseline_tests.cpp` 在 v1653 已提交并打 tag，早于本版任何 production formatter 改动。它冻结五份对象的完整输出：readiness 为 16889 字节、112 个顶层字段；implementation plan 为 19163 字节、121 个字段；disabled fake harness 为 13870 字节、135 个字段；execution-denied 为 15264 字节、138 个字段；input-hardening 为 21562 字节、113 个字段。五份对象都要求 runtime 与 fixture 逐字节相等，并核对各自带版本前缀的完整摘要。

这项顺序很重要。测试不是在看到新实现后才编写一个“看起来能过”的期望，而是先冻结旧行为，再让新实现接受旧裁判。迁移期间没有修改 Candidate 常量、摘要、字段数、fixture 提取器、compatibility kind 或领域测试。若新代码只有修改 oracle 才能通过，就说明迁移改变了合同，版本应立即停止，而不是把差异登记为新 waiver。

实际执行也不是五个文件全改完才测。先迁移 readiness，重建 core 与 baseline executable，确认 16889/112；再迁移 implementation plan，确认 19163/121；随后依次验证 13870/135、15264/138、21562/113。每一步都重新链接当前 formatter 并运行全部十三个剩余 owner 输出，因此既能定位刚引入的问题，也能发现共享 helper 对先前对象的意外影响。

## 四、ordered builder 到底解决什么，不解决什么

内部 `runtime_receipt_json_builder` 只提供六类机械能力：`json_string` 负责字符串转义，`json_bool` 负责布尔字面量，`json_integer` 负责十进制整数，`json_object` 按输入顺序拼接字段，`json_array` 按输入顺序拼接元素，`OrderedJsonField` 把字段名与已经编码的 JSON value 配成一对。builder 不排序、不去重、不自动省略 false、不推断 schema，也不会因为版本较新就打开某个字段。

旧 formatter 的主要风险不是语义计算，而是几百段字符串同时承担字段名、值编码、逗号、花括号和顺序。一个字段插入点错一行，代码仍可能编译，却会改变 fixture 字节、digest 消费表面或嵌套层级。新实现先构造具名 nested object/array，再把顶层字段放入 `std::vector<OrderedJsonField>`，最后统一渲染 compact JSON。reviewer 可以分别审查“对象里有什么”和“对象在顶层放在哪里”，不再需要沿着 `+` 和相邻字符串常量手工数逗号。

builder 也不替代领域测试。已经是语义完整、长期稳定的 JSON helper，例如七类 interface boundaries、八个 simulated route attempts、input hardening requirements 和 explicit no-go conditions，本版继续把它们作为已经编码的 raw JSON value 传入。若把 raw value 错用 `json_string`，内容会被二次转义；若把字符串 `"false"` 擅自改成布尔 false，虽然语义似乎更漂亮，字节合同却已改变。exact oracle 正是用来约束这些“自认为更合理”的无意修正。

## 五、readiness owner 的职责与输出结构

`runtime_credential_resolver_implementation_readiness_receipts.cpp` 的入口是 `format_credential_resolver_approval_required_implementation_readiness_non_participation_receipt_json`。它先构造 `source_node_v275`，记录上游检查数、六类 approval-required boundary、requirement code 和每条能力关闭状态；随后构造 `boundary_readiness`，给出 ready/blocked 数量、required artifact code 与详细表；再构造 summary、warnings、recommendations 和 evidence endpoints。

readiness 中原来的 `format_readiness_non_participation_flags_json` 是一段没有外层对象的字段尾串。新 `append_readiness_non_participation_fields` 把 read-only、execution、resolver/provider、credential、raw endpoint、network、managed-audit storage、write、ledger、schema、restore/load/compact、SETNXEX、auto-start 和 authority 字段按历史顺序逐项追加。它没有“ready 模式”参数，不能根据调用者选择性开启能力。迁移后 16889 字节和 112 字段完全不变，说明 Node v275 引用、warning/recommendation 顺序和所有关闭边界均未漂移。

## 六、implementation plan owner 如何保持七类边界独立

`runtime_credential_resolver_implementation_plan_receipts.cpp` 描述七类接口边界，每类都有 owner、allowed inputs、allowed outputs、prohibited actions、required artifacts 和 mini-kv position。v1655 没有把它们改造成统一 C++ class，也没有将七类边界拆散到公共 registry；`format_interface_boundaries_json` 仍是本 owner 的语义完整数组。新 formatter 只把 `source_node_v282_reference`、`implementation_plan`、`implementation_plan_review`、summary、warnings、recommendations 和 evidence endpoints 命名后有序组合。

旧 `format_non_participation_flags_json` 被改为无参数 `append_non_participation_fields`。其中 `implementation_plan_draft_only=true`、`consumes_node_v283_plan=true` 与 `ready_for_node_v284_upstream_echo_verification=true` 是本 family 的正向证据；real resolver、fake harness、credential、raw endpoint、network、ledger/schema、restore 和 auto-start 则保持 false。不能把这段 appender 与 readiness 的 appender强行合并，因为两者字段集合和插入顺序并不完全相同。迁移后的 19163 字节、121 字段 exact parity 证明这条所有权边界正确。

## 七、disabled fake harness 为什么仍然不是 runtime

`runtime_credential_resolver_disabled_fake_harness_receipts.cpp` 的合同名字里虽然有 fake harness，但输出明确记录 runtime toggle 默认 false、invocation state 为 disabled、runtime implementation absent、runtime invocation disallowed。它允许的只是 fake credential handle、fake endpoint handle、approval artifact reference、failure simulation map 和关闭状态等非秘密材料；credential value、raw endpoint URL、provider/client instance、managed-audit HTTP client 和 ledger write request 都是 prohibited inputs。

新实现把 credential、endpoint、provider/client、network、write 和 auto-start 六类边界分别命名成 nested object，再放入 `disabled_fake_harness_contract`。`upstream_echo_requirement` 只是要求 Java v122 与 mini-kv v127 并行回显，不会启动任何上游。顶层 closed fields 继续逐项声明 fake provider/client 未实例化、HTTP/TCP dial 不允许、schema/ledger 不写。真实输出保持 13870 字节、135 字段，说明“fake”没有在结构迁移中被误解为“可运行”。

## 八、execution-denied owner 如何证明 no execution

`runtime_credential_resolver_execution_denied_receipts.cpp` 最有价值的结构是八个 `simulated_route_attempts`。每项写明 requested operation、denied_by、source_evidence 和 `denied=true`，覆盖把 route 当 approval、读取 credential、解析 raw endpoint、实例化 provider/client、HTTP/TCP dial、ledger/schema 写入、运行 fake harness、自动启动上游。它们是描述拒绝结果的静态证据，不是代码真的执行了八次危险操作。

因此 `execution_denied_route_preflight` 同时保留 `simulated_attempt_count=8`、`denied_attempt_count=8` 与 `actual_execution_attempt_count=0`。新 ordered fields 中这三个值的类型和顺序都未变化，顶层 appender也再次记录 actual count 为 0、approval gate required=true/satisfied=false、provider/client/network/write/execution 均关闭。15264 字节和 138 字段 exact parity 加上真实 CLI 的 `execution_allowed=false`，共同证明本版没有借“拒绝测试”之名触发任何执行。

## 九、input-hardening owner 的稳定导出与停止条件

`runtime_credential_resolver_input_hardening_candidate_gate_receipts.cpp` 是本批输出最大的一份。它消费 Node v328 prerequisite closure 与 Node v329 candidate-gate decision，要求 mini-kv 暴露稳定 current receipt path，供后续 Node v330 读取；但 `ready_for_node_v330_before_java_v151=false`、`ready_for_disabled_runtime_shell_design_draft=false`，所以“输入更稳定”不等于“可以设计或实现 runtime”。

本文件原先用 `format_closed_side_effect_flags_json` 同时拼进 Node v328 引用、Node v329 引用和顶层对象。v1655 把它改成 `append_closed_side_effect_fields`，三个调用点共享完全同名、同值、同顺序的七十余个关闭字段。Node v328 和 v329 引用改为各自的 ordered object，summary 仍在原位置；decision record、necessity proof、stable current receipt export 等完整 helper继续由本文件所有。输出保持 21562 字节、113 字段，说明稳定导出路径、四条 input-hardening requirement、七条 no-go condition 和停止条件均未改变。

## 十、输入、处理、输出链条怎样保持透明

五个 public formatter 的直接输入仍只有只读命令列表与编译时常量。digest 函数继续按旧顺序收集项目版本、历史 release、source profile/state、计数、关键布尔和 `read_command_list_digest`，没有改为对新 builder 输出重新 hash，也没有读取 fixture 生成 production 数据。binary provenance、retention check、retention replay marker 与 read command digest 仍在调用时取得。

处理过程现在可以概括为三步：先生成领域 nested JSON；再按历史顺序构造顶层 ordered fields并追加本 family 的关闭边界；最后由 builder 输出 compact `std::string`。输出继续进入 standalone fixture comparison、领域 receipt tests、`SMOKEJSON`、runtime smoke evidence、verification manifest 和真实 CLI。整个过程没有文件写入、网络连接、线程启动或子进程执行，fixture 只在测试侧充当裁判。

## 十一、机械 census 与文件体量为什么都要看

census 的单位是包含 public receipt formatter 的 source owner，而不是 formatter 函数数。v1655 改动五个 owner，因此 builder-backed 从 20 增至 25，pending 从 7 降至 2；总数仍为 28 个 runtime receipt source、27 个 formatter owner 和 1 个没有目标 formatter 的 execution-denied retention waiver。`CMakeLists.txt` 的 floor 同步从 20 收紧到 25，任何后续删除 direct builder include、回退实现或新增未登记 owner 都会让 CTest 失败。

含空行物理行变化是 391→460、477→551、442→479、423→461、441→501，合计净增 278 行。它不是 shrink，也不应伪装成 shrink；收益来自字段名和值并排、nested object 有名称、brace-less fragment 消失。最大文件 551 行，仍低于 800 行风险线，没有新建 singular/companion 文件绕开 census。若后续业务真的继续扩大这些历史 family，才应按领域职责拆 owner；当前为了少几十行强拆常量、digest 与 formatter，反而会增加跳转成本。

## 十二、测试矩阵分别保护什么

第一层是 v1653 whole-object oracle，五份对象逐字节、字段数和完整摘要都不变。第二层是五个领域测试，检查 Node source version/profile/state、counts、合同数组、receipt digest、standalone fixture、SMOKEJSON 与 manifest 消费。第三层是 census contract，确认 25/2/1 且 floor 不能放松。第四层是 `smokejson_command_receipt_tests`、`managed_audit_receipts_tests` 与 `readonly_fixture_tests`，证明聚合层仍能读取这些对象且不会变成执行入口。

代码聚焦矩阵 10/10 通过；写完 consolidation note、账本、讲解与归档后，最终聚焦门扩大为 22/22，继续覆盖文档和讲解质量。完整 CTest 先以冷态 195.41 秒通过，最终复验仍为 348/348、106.62 秒，覆盖 Store、WAL、snapshot、RESP/TCP、OSFS、archive retention 和其余 receipt family。真实 CLI 对五份对象逐一检查 `read_only=true`，并确认 execution、credential read、managed-audit store、storage/write、runtime write、load/restore/compact、auto-start 和 order authority 九项为 false；额外运行 `CHECKJSON LOAD data/prod.snap`，得到 `execution_allowed=false` 与 `wal.touches_wal=false`，最后 `QUIT` 返回 `BYE`。这证明检查一个写命令的边界不等于执行它。

## 十三、对抗性自审：最强质疑是什么

第一项质疑是五个大 formatter 同版迁移会不会掩盖顺序漂移。应对方式不是相信最终全绿，而是逐 owner 重建并跑同一冻结 oracle；每一份通过后才继续，失败候选始终只有刚修改的 owner。第二项质疑是共享 appender 会不会把不同 family 错误同构。v1655 只在字段名、值和顺序完全相同的调用点共享；readiness、plan、fake harness、execution-denied 各自保留本地 appender，input-hardening 只在自己文件的三个层级复用 closed-side-effect 字段，没有跨 family 万能模板。

第三项质疑是 exact parity 会不会靠改 fixture 或测试自证。最终 `git diff --name-only -- tests fixtures/release` 为空，v1653 tag 中的 oracle可独立重放，五个 frozen size/digest/field count 常量均未改变。第四项质疑是本版是否误把“稳定 current receipt export”说成实时四项目集成。没有：本版仍是 single-project validation 加 cross-project contract alignment，只证明 Node 以后读取同一 fixture/runtime surface 时不会因 mini-kv 内部组装重构而漂移。

## 十四、后续维护者应怎样修改这些 formatter

若未来上游合同真的增加字段，先判断它属于哪一个 named nested object，再在领域测试中写清来源和类型；不能为了方便把所有新字段堆到顶层末尾。只在多个调用点字段名、值、顺序、语义完全一致时才抽 appender，而且 appender应尽量无参数，避免几十个布尔开关组合出未经测试的 schema。领域数组 helper若已经语义完整，可以继续作为 raw JSON value；若需要独立增删字段，再迁移成 ordered object，而不是为了语法统一进行无收益重写。

oracle 失败时先比较字节数和字段数：字段数变化通常说明增删或嵌套层级错，字段数相同而摘要变化通常说明值、顺序、escaping 或 whitespace 漂移。不能第一反应修改 expected digest。真正的 contract 变化必须作为独立版本，明确更新来源计划、fixture 版本、消费方和 compatibility policy；内部重构版本则必须保持现有 oracle。

## 十五、下一版边界与跨项目价值

阶段一现在只剩 `runtime_evidence_receipts.cpp` 和 `runtime_sandbox_receipts.cpp` 两个 owner，共五份 sandbox-chain 输出。manual sandbox dry-run 不是 raw exact：v1653 已命名三个只存在于 frozen fixture 的历史 metadata 字段；另外四份必须 exact。下一版只能使用这条单用途规则，不能复制 base resolver 的 field-order rule，也不能新增通用“忽略 metadata”逻辑。达到 27/27 formatter owner、1 个 named waiver 后，才进入文档诚实性刷新。

Node 最新 N5 source-size roadmap 明确允许 mini-kv 并行做内部维护，本版没有写 Node/Java 文件，也没有改变跨项目 route 或 schema。它给松耦合集成带来的价值，是让五份关键 implementation-chain evidence 继续以相同字节、digest、字段顺序和关闭边界对外呈现，同时让 C++ owner 内部从脆弱拼接升级为可逐字段审查的确定性组装。

一句话总结：v1655 在 v1653 不可变 oracle 下，把 implementation chain 五份 16889/19163/13870/15264/21562 字节的 formatter 迁移为具名对象、ordered fields 与本地无参数边界 appender，census 从 20/7/1 收紧到 25/2/1，并用 348/348 CTest 与真实 CLI 证明 no credential、no router/store、no write/WAL、no network、no execution、no auto-start、no authority 全部保持关闭。
