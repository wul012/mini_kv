# v1616 production-excellence K3 zero-dependency logger 代码讲解

## 本版目标、角色和不是什么

v1616 的目标是完成 production-excellence playbook 里的 K3：在 mini-kv 内部新增一个零依赖分级日志层，并把它接到 `minikv_cli` 和 `minikv_server` 两个可执行入口上。这个目标看起来像“加日志”，但真正的难点不是打印几行字符串，而是保持 mini-kv 现有证据链的通道边界。这个项目里很多命令输出、WAL 启动信息、server lifecycle event、benchmark evidence、SMOKEJSON/INFOJSON/SHARDJSON 等内容都被测试或外部 review 当成可审计证据；如果日志直接混进 stdout，下游就可能把运行噪声误读成证据字段，或者让 fixture 测试出现非业务性变化。因此 v1616 选择的边界是：运行日志进入 stderr，证据和命令响应继续留在 stdout。

本版的角色是“可观测性基础设施”，不是业务命令版本。它不新增 Redis-like 命令，不改变 Store 的读写语义，不改变 WAL record 格式，不改变 snapshot 文件，不改变 RESP 协议，不新增 active router，不开放 write routing，不触发 restore/load/compact，也不把 mini-kv 变成 Node 或 Java 的运行时后端。它也不是引入第三方 logging 框架的版本：K3 明确要求 zero-dependency，mini-kv 当前阶段更需要一个足够小、可读、可审计的本地 logger，而不是把格式、线程模型和依赖生命周期交给外部库。

还有一个“不是什么”很重要：v1616 不是把所有 `std::cerr` 文本都机械替换成带时间戳日志。usage/help 属于命令行交互提示，不是运行事件；stdout 上的 `event=server_start`、`event=wal_replay`、`WAL stats` 等内容已经有稳定消费语义，也不是本版要迁走的对象。K3 的实际交付是为运行事件和 fatal 边界提供统一 logger，并给入口提供 `--log-level` 控制。也就是说，v1616 不是输出格式大迁移，而是把“运行日志”和“证据输出”明确分开。

## 新增 logger 模块

本版新增的入口是 `include/minikv/log.hpp` 和 `src/log.cpp`。头文件只暴露四个概念：`LogLevel`、`parse_log_level`、`log_level_name`、`Logger`，以及一个便于测试的 `format_log_line`。`LogLevel` 的级别顺序是 `error=0`、`warn=1`、`info=2`、`debug=3`，过滤规则是“事件级别小于等于当前最小级别就输出”。因此默认 `warn` 会输出 error/warn，过滤 info/debug；`debug` 会输出全部。这种整数顺序很朴素，但优点是没有隐式策略，后续维护者一眼能看出过滤关系。

`parse_log_level` 只接受 `error`、`warn`、`info`、`debug` 四个小写值，非法值返回 `std::nullopt`。这个设计让 CLI/server 入口可以沿用既有参数解析风格：缺少参数或参数非法时打印 usage 并返回 2，而不是把错误隐藏成默认级别。`log_level_name` 做反向映射，保证日志输出里的 `level=` 字段和命令行参数使用同一组名字。这样控制面读日志时，不需要猜 `warning`、`warn`、`WARN`、`Info` 等多种拼法是否等价。

`Logger` 本身保持得很小：构造时记录最小级别和输出流指针，`enabled` 判断是否输出，`log` 在通过过滤后写一行 `format_log_line`。默认构造使用 `std::cerr`，入口可以显式传入其他 `std::ostream`，测试则用 `std::ostringstream` 捕获输出。这个接口没有全局单例，没有环境变量读取，没有后台线程，没有异步缓冲，也没有文件轮转。mini-kv 现在需要的是可审计的运行事件线，而不是复杂的日志平台。

`format_log_line` 的输出形状是 `ts=<UTC> level=<level> thread=<id> <message>`。时间戳使用 UTC 秒级 ISO 风格，线程 id 通过 `std::this_thread::get_id()` 写入字符串。这里没有引入毫秒、时区、本地化或 JSON 日志，是有意的：K3 只要求 timestamps 和 thread id，后续如果要机器解析，可以另开版本设计结构化日志；现在先保持简单文本，避免一版里同时引入格式平台和解析契约。`std::gmtime` 本身返回静态缓冲，本版在内部加一个小锁复制 `std::tm`，避免多线程格式化时踩到共享状态，同时也避开 Windows/MinGW 对 `gmtime_s` 支持差异造成的本地构建风险。

## CLI 入口怎么接入

`src/main.cpp` 是 `minikv_cli` 的入口。本版在 `CliOptions` 增加 `minikv::LogLevel log_level = minikv::LogLevel::warn`，并在 usage 中增加 `[--log-level error|warn|info|debug]`。解析逻辑保持原来的顺序风格：遇到 `--log-level` 必须消耗下一个参数；如果参数缺失或 `parse_log_level` 返回空，就把整个 options 解析视为失败，外层打印 usage 并返回 2。这样非法日志级别不会被悄悄忽略，也不会被当作 WAL path。

CLI 入口创建 `minikv::Logger logger{options->log_level, std::cerr}` 后，只把 fatal 边界切到 logger：WAL repair 失败、WAL auto compact 失败，以及顶层异常。普通交互输出仍然走 stdout：启动时打印 `mini-kv CLI`、help text、prompt、命令响应。这样做的原因很直接：CLI 的 stdout 是用户和测试读取命令响应的通道，如果把 logger 的时间戳混进去，`GET`、`PING`、`SMOKEJSON` 等响应就可能被污染。v1616 因此只让错误日志进入 stderr，保留命令响应的老行为。

这里还有一个维护细节：`--log-level debug` 在 CLI 里目前不会制造额外 debug 事件，因为 K3 不打算顺手扩展 CLI 事件体系。这个参数存在的意义是统一两个入口的配置面，并让 fatal/error 日志使用同一个 logger；以后如果 CLI 需要记录更细的 WAL 维护或交互诊断，可以直接复用这个参数，而不再改命令行契约。换句话说，v1616 先把入口和过滤层打稳，不用假装已经有很多 debug 事件。

## Server 入口怎么接入

`src/server_main.cpp` 是本版更关键的改动点。原来的 `TcpServer::Options::logger` 在 server main 里被接成一个 lambda，直接把 TCP server 内部传来的消息写到 `std::cout`。这意味着像 `event=tcp_listen`、`event=tcp_client_accepted`、`event=tcp_client_closed`、`event=server_metrics` 这样的运行事件会和 `event=server_start`、`event=server_hint` 共享 stdout。K3 的要求是 logs go to stderr, evidence stays on stdout，所以 v1616 把这个 lambda 改为使用 `Logger`，并且以 `LogLevel::info` 记录 TCP server callback message。

server 参数解析同样新增 `--log-level error|warn|info|debug`，默认 warn。默认 warn 的效果是：server 启动后 stdout 仍会出现 `event=server_start` 和 `event=server_hint`，但 TCP listen/accepted/closed/metrics 这些 info 日志默认不会刷屏。运维或测试需要观察连接细节时，可以显式加 `--log-level info` 或 `--log-level debug`。这符合生产化默认：默认安静，显式打开更多运行诊断。

本版没有迁移 server 上已有的 WAL evidence 输出。`log_wal_replay`、`log_wal_repair`、`log_wal_maintenance`、`log_wal_auto_compact` 仍然写 stdout，因为这些内容在启动时描述 WAL 回放、修复、压缩建议和维护状态，已经是运行证据的一部分。如果把它们迁到 stderr，下游人工审查或既有测试可能会失去原来的证据位置。相反，真正应该进入 stderr 的是连接级、请求级和 metrics callback 这类运行日志。v1616 的真实 TCP smoke 也验证了这一点：`event=server_start` 留在 stdout，`event=tcp_listen`、`event=tcp_client_accepted`、`event=tcp_client_closed`、`event=server_metrics` 带 `ts/level/thread` 出现在 stderr。

fatal 边界同样用 logger 输出：WAL repair 失败、auto compact 失败、顶层异常会产生 error 级日志。由于 error 的优先级最高，即便用户设置 `--log-level error` 也会输出；设置 `warn/info/debug` 更不用说。这样 fatal 事件不会因为默认 warn 过滤掉，也不会继续以不同格式散落在入口逻辑里。

## CMake 和测试结构

`CMakeLists.txt` 里把 `src/log.cpp` 加进 `minikv` 静态库源列表，所以 CLI、server、测试目标都通过同一个实现链接。测试方面新增 `tests/log_tests.cpp`，并用 `minikv_add_linked_test` 注册为 `log_tests`。这个测试不依赖进程启动，直接用 `std::ostringstream` 捕获 logger 输出，断言四类行为：级别名称解析、默认 warn 过滤 info/debug、debug 级别输出全部、格式化结果包含 `ts=`、`level=`、`thread=` 和原始 message。

除了单元测试，本版还在 CTest 增加两个入口级 smoke。`cli_log_level_flag_smoke` 运行 `minikv_cli --log-level debug`，利用 CTest 的非交互 stdin EOF 让 CLI 打印 help/prompt 后正常退出，并通过 `PASS_REGULAR_EXPRESSION "mini-kv CLI"` 证明参数被接受而不是被 usage 拒绝。`server_log_level_invalid_smoke` 运行 `minikv_server --log-level trace`，并设置 `WILL_FAIL TRUE`，证明非法级别会被拒绝。这里曾经尝试把 `WILL_FAIL` 和 `PASS_REGULAR_EXPRESSION` 组合起来同时检查 usage 文本，但 CTest 对“预期失败”和 regex 的组合语义不符合这个场景，最终保留退出失败断言，把 usage 文本放到源码和归档解释里说明。这个小修正本身也是工程判断：测试要稳定表达核心边界，不要为一条输出文本制造跨平台脆弱性。

完整 CTest 从 334/337 的变化也有意义。v1615 增加 CI 维护测试后，v1616 又新增 logger 单元测试和两个入口 smoke，当前本地全量是 337 条全部通过。这不是简单数字游戏，而是证明 logger 没有破坏历史测试面：`command_tests`、`readonly_fixture_tests`、`runtime_smoke_evidence_tests`、`wal_tests`、`snapshot_tests`、`tcp_server_tests`、`tcp_resp_tests` 等都继续通过，说明 stdout evidence、RESP 响应、WAL/snapshot 行为没有被日志改动污染。

## 真实 smoke 证明了什么

本版除 CTest 外还跑了真实 TCP smoke。启动命令使用 `minikv_server 6616 127.0.0.1 --log-level info --accept-poll-ms 10 --metrics-interval-ms 1000`，然后用 TCP client 发送 `PING\n`，响应是 `PONG`。服务端随后被停止，验证 stdout 和 stderr 内容。stdout 中只有 `event=server_start` 和 `event=server_hint`；stderr 中出现带 `ts=... level=info thread=...` 的 `event=tcp_listen`、`event=tcp_client_accepted`、`event=tcp_client_closed`、`event=server_metrics`。

这个 smoke 的价值在于它穿过了真实 `TcpServer` callback，而不是只测 logger 类。`tcp_server.cpp` 本身仍然只知道 `Options::LogHandler`，并不直接依赖 `Logger`；这保持了核心 TCP 模块的可测试性和复用性。server main 决定把 callback 接到 stderr logger，是入口层的装配策略。测试里直接给 `TcpServer::Options::logger` 传 vector collector 的模式也继续有效，这就是为什么 `tcp_server_tests` 没有被迫跟着 stderr 格式变化。模块边界因此更干净：库层发事件，入口层决定事件去哪儿。

这次 smoke 还证明了默认证据流没有被改坏。`event=server_start` 仍然在 stdout，说明启动状态证据保持原通道；TCP 连接日志不在 stdout，说明运行噪声不会混入证据输出；stderr 带时间戳和 thread id，说明 K3 要求的可观测字段存在；`PING -> PONG` 证明服务端仍能处理真实请求。这个组合比单独检查某个字符串更可靠，因为它同时覆盖了启动、网络、命令执行、日志路由和进程清理。

## 边界字段怎么理解

如果把 v1616 放回 mini-kv 长期 evidence 语境里看，几个关键边界字段的含义仍然没有改变。`read_only` 对本版的含义是：logger 读取时间、线程 id 和消息字符串，但不读取业务存储，不读取 Node/Java 文件，不读取 credential value，不解析 raw endpoint。`execution_allowed` 只限于本地构建、测试和 smoke，logger 本身不触发 restore/load/compact，不启动外部服务，不打开 managed audit connection。`order_authoritative` 仍然是 false：日志不会成为订单事实来源，也不会让 mini-kv 参与 Java order authority。

`mutates_store` 仍然是 false。`Logger::log` 只写到给定 ostream，通常是 stderr；它不修改 Store，不追加 WAL，不更新 snapshot。`touches_wal` 对 logger 也为 false；CLI/server 里原本已有的 WAL repair、auto compact、maintenance 路径保持原逻辑，只有失败时的错误文本从裸 `std::cerr` 改成 error 级 logger。`warnings/blockers/diagnostics` 的解释也更清晰：warning/error/info/debug 只是运行诊断层，不是 evidence JSON 的字段扩展，不会改变 SHARDJSON 或 runtime fixture schema。

这也是为什么 v1616 没有改 `command.cpp`、`wal.cpp`、`snapshot.cpp` 的内部逻辑。playbook 提到要替换边界处裸 `std::cerr`，但实际扫描后这些核心文件并没有直接的 `std::cerr` 运行日志热点；真正有入口边界的是 `src/main.cpp` 和 `src/server_main.cpp`，真正有 TCP runtime callback 的是 server main 装配处。贸然为了“看起来覆盖更多文件”去改 command/WAL/snapshot 内部，反而会扩大风险。v1616 按实际代码形态推进：哪里是输出边界，就在哪里接 logger；哪里已经是稳定 stdout evidence，就不为了形式主义迁移。

## 后续维护价值

v1616 给 K4 file splits 的价值很实际。K4 会拆 `command.cpp`、`shard_readiness.cpp` 等大文件，拆分期间最怕两类问题：一是测试失败但缺少足够运行诊断；二是为了调试临时往 stdout 打东西，最后污染 evidence。K3 logger 提前提供了 stderr 运行日志通道，后续拆分如果需要更多诊断，可以接入 `Logger` 或入口 callback，而不是往命令响应里塞调试输出。这个基础设施不大，但会降低后续重构的噪声。

它也给长期运行体验留下一个合理默认。默认 warn 让服务端在普通启动时保持安静，避免每个连接都刷 stdout；需要诊断时 `--log-level info` 就能看到 listen、accepted、closed、metrics；未来如果需要更细的 debug 事件，命令行契约已经存在。这样既照顾学习项目的可观察性，也不牺牲生产化证据的干净程度。

最后，v1616 仍然保持跨项目松耦合。Node 和 Java 可以把 mini-kv 的日志能力理解为本地运维诊断增强，但不能因此假设 mini-kv 开放了写路由、credential 读取、managed audit 连接或真实执行入口。日志只是 stderr 上的观察信号，不是新的控制通道，不是新的证据 schema，也不是跨项目集成协议。一句话总结：v1616 把 mini-kv 的运行诊断放到正确的通道里，让后续 Node/Java/mini-kv 松耦合融合在读取证据时更干净，在排查运行问题时也更有抓手。
