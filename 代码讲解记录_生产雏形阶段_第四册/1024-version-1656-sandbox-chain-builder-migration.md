# mini-kv v1656：sandbox chain 有序构建收口与可维护性拆分

## 一、本版目标、角色定位与明确不做什么

v1656 的目标不是增加一条更长的业务链，而是完成 `v1637-production-excellence-completion-brief.md` 第一阶段最后一块可证明的工程工作：把仍由手写长字符串拼接的 sandbox receipt formatter 迁移到项目已经建立的 ordered JSON builder，并让“所有 formatter owner 都已迁移”成为一个能够失败的机械事实。上一版结束时，仓库里共有二十八个 runtime receipt source，其中二十七个真正拥有公开 formatter，二十五个已经直接使用 builder，剩余两个 owner 正好承载五份 managed-audit sandbox 回执。若只在进度表写“完成”，以后任何人都无法判断是不是漏了一个文件；所以本版必须同时收口实现、冻结输出、census 门、真实运行和文件规模。

本版在系统中的角色仍然只是运行时证据提供者。它让 Node 或人工审查者读取“某个危险能力没有发生”的结构化事实，不让 mini-kv 获得危险能力本身。它不是新的 router，不是 credential resolver，不是 sandbox network adapter，不是 managed-audit storage，也不是 approval、deployment、rollback 或 order authority。代码没有读取 credential value，没有把 handle 解析成 raw endpoint，没有发送 DNS、TLS、HTTP 或 TCP 请求，没有启动 Node、Java 或 mini-kv 服务，没有写 Store、WAL、ledger、schema 或 production record，也没有执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`。这一点很重要：receipt 名称里出现 connection、adapter、transport、endpoint，并不表示项目实现了这些动作；恰恰相反，这些对象的职责是把动作保持关闭并给出可核验原因。

## 二、入口在哪里，五份输出从哪里进入系统

外部最容易观察的入口是 `SMOKEJSON`。命令经 command catalog 与 command processor 进入 `command_smoke_formatters.cpp`，后者调用五个公开 formatter，把各自的 compact JSON object 嵌入统一 smoke object。五个键依次是 `manual_sandbox_dry_run_command_non_participation_receipt`、`manual_sandbox_connection_precheck_non_participation_receipt`、`disabled_adapter_client_non_participation_receipt`、`fake_transport_dry_run_packet_non_participation_receipt` 和 `sandbox_endpoint_handle_non_participation_receipt`。调用关系没有在本版变化，public header 中的函数签名和 digest 函数也没有变化；改变的只是函数内部如何把已经确定的字段组装成字符串。

前两份 formatter 位于 `runtime_evidence_receipts.cpp`。manual dry-run 描述的是 Node 侧曾经提出过的一组六条人工 sandbox 命令为什么只能作为文档化、禁用的候选表面；manual connection precheck 描述连接前检查包为什么仍然只读、不可执行。后三份位于 `runtime_sandbox_receipts.cpp`：disabled adapter client 证明适配器默认关闭，fake transport 证明所谓 packet 只是测试态数据形状，endpoint handle 证明公开的是非秘密 handle 与 allowlist/TLS/redaction 要求，而不是原始 URL 或真实连接。这样的所有权划分仍然成立，所以本版没有把五份对象塞进一个“大一统 schema”。

## 三、ordered builder 实际解决了什么问题

旧实现通常先调用若干 `format_*_json` helper 得到带大括号的子对象，再用几十段 `"\"field\":" + value` 拼接顶层字符串。它能工作，却把三件事混在同一视觉层：字段名称、字段顺序和标量转义。少一个逗号、多一个空格或在错误位置闭合大括号，都可能生成仍能通过局部 `assert_contains`、却不再与冻结 fixture 一致的输出。历史上 post-echo 多出的一个空格已经证明这种风险不是假设。

新实现把每个语义完整的子对象先命名，例如 preflight packet、disabled command surface、failure mapping、request/response shape、network policy、TLS policy、redaction policy、operator instructions 与 checks。顶层再按历史顺序建立 `std::vector<OrderedJsonField>`，每个元素明确记录字段名和已经编码的 JSON value，最后由 `json_object` 统一放置逗号与大括号。字符串使用 `json_string`，布尔量使用 `json_boolean`，整数使用 `json_integer`，数组使用 `json_array`。builder 不排序字段、不猜测默认值、不读取环境、不补齐 schema，也不执行命令；它只忠实渲染调用者给出的顺序和值。因此字段的领域含义仍归 formatter 所有，而格式细节由一个经过测试的小组件负责。

这里刻意保留了一部分完整 raw JSON helper。比如多项 failure reasons、source command definitions 或较大的检查列表，本来就是一个语义整体，并且已经由旧测试和 fixture 保护。为了“看起来全部 builder 化”而把每个内部数组再次改写，会扩大 diff 和审查面，却不会增加本版的主要价值。顶层边界与容易出错的重复字段需要结构化，稳定领域对象可以作为 builder 的 value 继续复用；这是控制变更半径，而不是迁移不完整。

## 四、三段共享 helper 为什么安全

`runtime_sandbox_receipt_utils.hpp` 只提供三个无参数 appender。第一段追加共同的 read-only prefix：`read_only=true`、`execution_allowed=false`、`dry_run_only=true`，以及 Node、Java、mini-kv 都不能自动启动。第二段追加关闭写路径的字段：connection、write/admin command、runtime write、managed-audit store、storage backend、sandbox storage、storage write、managed write 与 production record 全部为 false。第三段追加关闭执行和权威的尾部：schema rehearsal、schema migration、restore、load/restore/compact、setnxex 与 order authority 全部为 false。

无参数是这项设计的重要边界。若 helper 接收十几个布尔开关，调用点会变成难以审查的真假矩阵，还可能让一个 family 意外打开另一个 family 才有的字段。现在 helper 只能输出一种历史上已经完全相同的关闭片段；family 专属字段仍由调用点显式插入。disabled adapter 额外记录 adapter state、credential required/read/loaded 和 external request；fake transport 还记录 credential included、raw endpoint included、payload secret、临时目录、临时文件和 cleanup count；endpoint handle 则记录 approval ledger、raw endpoint parsed、allowlist、TLS 与 redaction。共享的是事实完全相同的部分，不共享的是含义不同的部分。

manual 两份 receipt 没有强行使用这三个 helper，因为它们位于另一个 owner，字段集合和历史插入位置也不同。`runtime_evidence_receipts.cpp` 为 manual dry-run 与 connection precheck 各保留自己的无参数边界 appender。这样做比跨文件追求最大复用更容易审查：读者在对应 formatter 附近就能看到它究竟承诺关闭什么，也不会因为未来一份 sandbox 对象新增字段而无意改变另一条证据链。

## 五、两种 parity 判定以及为什么不能把差异抹平

迁移裁判来自 v1653 新增且本版未修改的 `runtime_receipt_remaining_owner_baseline_tests.cpp`。manual connection precheck、disabled adapter、fake transport 与 endpoint handle 必须与各自 frozen fixture 子对象逐字节相等，最终仍分别是 4176/74、5361/92、6040/92、6340/92，前者是字节数，后者是顶层字段数。逐字节相等比“解析后对象相等”更严格：字段顺序、标点、转义和空白只要漂移一处就会失败。四份输出的版本化摘要也保持 v1653 冻结值，因此不能靠碰巧相同的字段计数掩盖内容变化。

manual dry-run 是唯一例外，但不是宽松忽略。历史 fixture 有 3199 字节和 56 个字段，当前 public runtime surface 有 3060 字节和 53 个字段；fixture 独有 `runtime_project_version`、`current_runtime_fixture_release_version`、`current_runtime_fixture_artifact_path_hint` 三个冻结元数据字段。裁判只允许按固定名称、固定值各删除一次这三段，再要求剩余字节与 runtime 完全相等。若出现第四个字段、同名不同值、删除次数不是一、其他位置变化或新增转义，测试都会失败。fixture 保持历史证据，runtime 保持当前契约，两者通过单用途规则建立联系；本版没有修改 fixture，也没有把“忽略任意字段”包装成通用兼容机制。

五份 runtime object 还各自通过十项语义断言：`read_only` 必须为 true；`execution_allowed`、`credential_value_read_allowed`、`managed_audit_store`、`storage_write_allowed`、`write_commands_executed`、`runtime_write_observed`、`load_restore_compact_executed`、`node_auto_start_allowed` 与 `order_authoritative` 必须为 false。这样即使未来有人生成字节长度相同却改变布尔含义的对象，语义门也会立即失败。

## 六、为什么拆出 runtime_evidence_session.cpp

迁移前 `runtime_evidence_receipts.cpp` 有 752 个物理行，同时拥有两种职责：一类是本版处理的 manual sandbox receipts，另一类是读取命令列表摘要、live-read session hint 和 uptime bucket。后者为 INFOJSON/SMOKEJSON 的会话证据服务，并不参与任何 receipt formatter。继续把 ordered fields 展开在同一文件里虽然还没超过 800 行，却会让下一次维护迅速逼近停止线，也会让读者在 session 与 receipt 两套概念间反复跳转。

因此本版先在未改输出时运行 oracle，再把 `read_command_list_digest`、`format_live_read_session_hint_json` 和私有的 uptime bucket 计算整体移入 `runtime_evidence_session.cpp`，通过 CMake 加入同一静态库。public declaration 没移动，调用者无须修改，链接符号和行为保持不变。拆分后再次运行 oracle；若遗漏源文件、改变 namespace、重复定义或改变输出，编译或测试都会失败。格式化后 evidence receipt owner 为 700 行，新 session 单元为 52 行。这个拆分按职责降低认知负担，不移动 formatter，也不改变 census 所认定的 owner，所以不是用 companion 文件规避门槛。

`runtime_sandbox_receipts.cpp` 从旧版 665 行变为约 704 行。它没有虚报 shrink：ordered fields 把原来横向隐藏在长字符串中的结构展开后，物理行可能增加，但每个字段的位置和所有权更容易审查。新增私有 helper header 只有 46 行，且不拥有任何 public formatter。评价标准不是“行数必须下降”，而是主体低于风险线、职责更清晰、重复片段减少、差异可机械验证。这个结论也写入 consolidation note，防止未来把正常结构化增长误判成停止条件。

## 七、census 如何把完成状态变成会失败的门

`cmake/check_receipt_builder_census.cmake` 枚举 `src/runtime_*_receipts.cpp`，区分含公开 formatter 定义的 owner、直接包含 builder 的 owner、pending 文件和明确 waiver。v1656 把 CMake 的最小 builder-backed 数从 25 收紧到 27。最终复现输出是 28 个 source、27 个 formatter owner、27 个 builder-backed、0 个 pending、1 个 waiver。唯一 waiver 是 `runtime_credential_resolver_execution_denied_retention_receipts.cpp`，它只承载 retained data/helper，并不定义公开 formatter；给它虚假添加 builder include 反而会污染统计。

这个门保护的是未来，而不只是本次截图。删除某个 owner 的 builder include、增加新的手写 formatter 文件、让 waiver 集合出现第二个名字，或者把 floor 降回 25，都会使测试或审查失败。至此“阶段一完成”不依赖进度表自述，而依赖仓库当前结构。后续若确有新 receipt，开发者必须同步决定它的 owner、builder 使用和 census 语义，不能悄悄把 pending 重新引入。

## 八、测试分别保护什么，以及真实 smoke 证明什么

第一层是逐 formatter 增量 oracle。每迁移一份输出就只重建 baseline test，及时定位哪一份对象出现第一个不同字节，避免五个改动叠加后才排查。第二层是五个领域 receipt test，它们检查各自 source reference、digest、receipt-only 标志、handle、failure mapping 与关闭边界，防止为了字节相等而拼出语义错误的数据。第三层是 `runtime_receipt_json_builder_tests`，验证 builder 的字符串转义、标量编码、数组和插入顺序。第四层是 `smokejson_command_receipt_tests`、`managed_audit_receipts_tests` 与 `readonly_fixture_tests`，保护公开聚合面、冻结 fixture 消费面和只读命令契约。第五层是 `receipt_builder_census_contract`，保护迁移不会倒退。最后的 full CTest 继续覆盖 Store、WAL、snapshot、RESP/TCP、OSFS 和全部历史证据链，证明内部 JSON 组装重构没有旁路回归。

真实 CLI smoke 不是调用 formatter 的测试替身，而是启动本次构建生成的 `minikv_cli.exe`，依次输入 `SMOKEJSON`、`CHECKJSON LOAD data/prod.snap` 与 `QUIT`。脚本从实际输出中解析五份 receipt，每份逐字段检查十项共同边界，结果都是 10/10。随后检查 `CHECKJSON` 把 LOAD 分类为有副作用的写命令，但自身仍为 `read_only=true`、`execution_allowed=false` 且 `wal.touches_wal=false`；也就是说它解释风险，没有执行 LOAD、替换 Store 或触碰 WAL。最后 `BYE` 证明 CLI 正常收束。这一层把“函数输出正确”推进为“用户真实命令表面仍正确”。

验证过程中还出现了一次必须透明记录的偏差：两个被工具超时中断的 `cmake --build` 进程仍在后台编译，第三个构建进入同一 build tree 后，链接器报告某测试目标缺少 `objects.a`。进程检查证实存在重叠的 cmake/mingw32-make/g++ 树；停止本轮残留后，以单一 `-j2` 构建进程从同一代码树重跑，全部目标 100% 完成。这个现象既不能算代码通过，也不能误判成源码缺陷，所以证据同时保留初次失败的根因和干净重跑结果。

## 九、维护者以后应怎样修改这条链

若只调整某份 receipt 的领域内容，先在对应 owner 中创建或修改具名 nested value，再把字段放到明确的历史位置；不要把完整 JSON 拼回长字符串。若多个 family 出现完全相同的字段段，先比较字段名、值、顺序和插入点，四者都相同才可提取无参数 helper。若差异需要布尔开关或版本分支，优先保留本地 ownership。任何输出变化都应先判断是有意契约版本升级还是无意漂移；有意升级需要新的 fixture/consumer 协调计划，无意漂移必须修实现，不能放宽 v1653 的单用途 compatibility rule。

文件规模方面，700 行左右的两个 owner 已接近项目的关注区，但目前内部段落按常量、子对象 formatter、digest 与 public formatter 分层清楚，继续拆分会把单一 receipt 的上下文切得过碎。下一阶段也不再向这里增加功能，所以本版停在职责拆分后的稳定点是合理的。未来若新增成组 sandbox receipts，应优先新建按领域命名的 owner，并同步 census，而不是继续把两个现有文件推过 800 行。

## 十、一句话价值

v1656 让 mini-kv 向 Node/Java 提供的最后五份 sandbox 证据从脆弱的长字符串拼接变成有序、可审查、可逐字节裁决且由 27/0/1 census 防倒退的只读契约，同时没有把任何 credential、router、network、write、WAL、execution、auto-start、audit 或 order authority 带进运行时。
