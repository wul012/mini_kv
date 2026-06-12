# v1617 production-excellence K4 command dispatch split 代码讲解

v1617 的目标是启动 production-excellence playbook 里的 K4 文件拆分，但只做其中风险最低、边界最清楚的一刀：把 `src/command.cpp` 里承担顶层命令分发的 `CommandProcessor::execute_trimmed` 移到新的 `src/command_dispatch.cpp`，同时把原来夹在 `command.cpp` 匿名命名空间里的 parse / usage / WAL 错误 helper 和 WAL command gate 拆成内部源文件。这个版本不是新功能版本，不新增命令，不新增 JSON evidence 字段，不改 `COMMANDSJSON`、`CHECKJSON`、`SMOKEJSON`、`SHARDJSON` 等输出结构，也不把 mini-kv 变成任何 Node/Java 的 order authority、audit authority 或 storage backend。它只是在 K1 sanitizer、K2 coverage、K3 logger 都已经落地并通过 review 之后，开始把最大风险文件从“一个文件什么都做”推进到“调度、运行证据、WAL 协调、解析 helper 分工清楚”。

这一刀选择 command dispatch，而不是先拆 `shard_readiness.cpp`，原因是 K4 的验收重点是 byte-identical public output。命令调度有现成的护栏：`command_tests` 覆盖核心命令行为，`command_catalog_refactor_fixture_tests` 固定命令目录和 contract 解释，`readonly_fixture_tests`、`runtime_smoke_evidence_tests`、`tcp_resp_tests` 等从外部响应角度守住 read-only evidence 和 RESP/TCP 路径。相比之下，`shard_readiness.cpp` 的证据链更长、历史 fixture 更多，适合作为第二刀或第三刀。v1617 因此先拆 command 的 translation unit，让后续 per-family executor 拆分有一个干净入口。

## 入口与模块职责

本版之后，`src/command.cpp` 不再承载顶层 verb switch。它保留四类职责：第一，`CommandMetricsTracker` 的统计记录、聚合、reset 和快照；第二，`CommandProcessor` 构造函数与 `execute` 外层入口，负责 trim、记录 command token、计时和 metrics 写入；第三，`execute_with_wal` 与 `auto_compact_wal_if_needed`，继续保证 WAL append 与 store mutation 的顺序；第四，`execute_runtime_evidence_command`，也就是 `STATS`、`SMOKEJSON`、`INFOJSON`、大量 `SHARDROUTE...JSON` 和 `COMMANDS/COMMANDSJSON` 这类只读 evidence 命令的分发。这个文件仍然比较大，因为 runtime evidence 命令很多，但它的职责已经从“所有 command 逻辑混在一起”收窄成“运行时 evidence 和处理器外壳”。

`src/command_dispatch.cpp` 是本版新增的主要入口文件。它定义 `CommandProcessor::execute_trimmed` 和 `CommandProcessor::help_text`。`execute_trimmed` 仍然先把输入行放进 `std::istringstream`，读取第一个 token，调用 `to_upper`，再通过 `command_catalog::lookup_dispatch_verb(command)` 进入 `CommandDispatchVerb` switch。这里没有重写 parser，没有改命令名字，没有改 command catalog，也没有把 switch 换成新表驱动机制。换句话说，v1617 不是“重新设计命令系统”，而是把既有调度器搬到单独 translation unit，为后续拆 `SET/GET/DEL`、TTL、snapshot/WAL、contract/evidence 等 family executor 留位置。

`src/command_parse_helpers.hpp` 与 `src/command_parse_helpers.cpp` 承接原来匿名命名空间里的小工具：`has_extra_token`、`usage`、`wal_error`、`wal_disabled_error`、`wal_compact_error`、`snapshot_error`、`parse_positive_seconds`、`format_expires_at` 和 `format_setex_at`。这些 helper 的特点是既被 command dispatch 使用，也被 runtime evidence 或 WAL wrapper 使用。把它们继续留在 `command.cpp` 会导致 `command_dispatch.cpp` 只能复制文案或绕回主文件；复制会让 `ERR usage: ...`、`ERR WAL not enabled`、`SETEXAT`/`EXPIREAT` 格式未来出现双份维护风险。v1617 选择把它们放进 `minikv::command_internal`，既不暴露成 public API，又避免内部重复。

`src/command_wal_gate.hpp` 与 `src/command_wal_gate.cpp` 是这版最需要小心的点。原来 `WalCommandGate` 是 `command.cpp` 匿名命名空间里的静态 gate，`execute_with_wal`、`auto_compact_wal_if_needed`、`COMPACT`、`WALINFO`、`STATS`/`SMOKEJSON` 的 WAL maintenance read 都通过同一个 gate 协调。把 `execute_trimmed` 搬到新文件后，如果简单在两个 `.cpp` 里各自复制一个匿名 `WalCommandGate`，编译会过，但语义会坏：写命令和 compact/read 可能拿到不同 gate，原来的“WAL append + mutation 与 compaction 互斥”保护就被悄悄拆成两套锁。v1617 因此把 gate 做成内部模块，并通过 `command_internal::wal_command_gate()` 返回同一个 function-local static 实例，保证 moved dispatch 和原 `execute_with_wal` 仍然共享同一个互斥协调器。

## 响应结构与证据边界

从用户或下游控制面看，v1617 的响应结构应该完全不变。`PING` 仍然回 `PONG` 或 echo message；`SET` 仍然是 `OK inserted` / `OK updated`；`GET` miss 仍然是 `(nil)`；`EXPIRE`、`TTL`、`KEYS`、`SAVE`、`LOAD`、`COMPACT`、`WALINFO`、`RESETSTATS`、`EXPLAINJSON`、`CHECKJSON`、`HELP`、`QUIT` 的返回文案都沿用原逻辑。`execute_trimmed` 被搬走，但它调用的 `Store`、`SnapshotFile`、`WriteAheadLog`、`command_response_formatters` 和 `command_contracts` 都是原对象和原函数，未引入新格式层。

`read_only` 的含义也没有扩大。`CHECKJSON GET alpha` 仍然报告 `read_only=true`、`execution_allowed=false`、`write_command=false`、`mutates_store=false` 这一类控制面解释；它只是解释命令，不执行命令。`STATSJSON`、`SMOKEJSON`、`STORAGEJSON` 仍然是读当前运行状态或维护报告，不会把 mini-kv 接成 managed audit storage backend。`execution_allowed=false` 仍然表示这些证据命令不授予 restore/load/compact 自动执行权；注意 `LOAD` 和 `COMPACT` 作为本地 CLI/admin command 仍然存在，但 Node/Java 只读消费 evidence 时不能把它们当成远程自动执行入口。`order_authoritative=false` 也没有变化：mini-kv 的数据和 evidence 不是 Java 订单事实来源。

本版还补了一处容易遗漏的质量边界：`.github/workflows/ci.yml` 的 Ubuntu coverage filter。K2 时 core filter 包含 `store|command|command_catalog|command_contracts|command_response_formatters|command_smoke_formatters|wal|snapshot|resp`。v1617 把 command 逻辑移出 `src/command.cpp` 后，如果 filter 不更新，新 `src/command_dispatch.cpp`、`src/command_parse_helpers.cpp`、`src/command_wal_gate.cpp` 就不再计入 core coverage 分母，K2 floor 会保护不到被搬走的核心逻辑。这会制造一种很危险的假象：coverage job 仍然是绿色，但它看不到真正改动的文件。v1617 因此把这三个新文件加入 filter，并同步更新 `tests/dependabot_config_tests.cpp` 的 CI 合同断言，让“拆出来的 command 文件仍属于 core coverage”成为仓库内可测试规则。

## 测试分别保护什么

`command_tests` 保护的是核心命令行为：写入、读取、删除、TTL、metrics、错误 usage、WAL 相关路径等。它能发现 `execute_trimmed` 搬家后 parser、返回文案、metrics 记录或 store mutation 语义漂移的问题。`command_catalog_refactor_fixture_tests` 保护的是命令目录、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 这类自描述 contract。如果 dispatch 文件没有正确接 `command_contracts`，或者命令分类、side effects、stable flags 发生漂移，这类测试会先红。

`readonly_fixture_tests` 和 `runtime_smoke_evidence_tests` 保护的是 read-only evidence 语义。它们关注 `SMOKEJSON`/`INFOJSON` 中那些给 Node/Java 控制面看的边界字段，例如不写 managed audit state、不读 credential value、不自动启动 Node/Java、不打开 endpoint、不成为 order authority。v1617 没有新增 evidence 字段，但移动 dispatch 后仍要证明这些命令能从同一个 `CommandProcessor` 路径正确出来。

`wal_tests` 与 `snapshot_tests` 保护的是持久化边界。前者证明 WAL append、replay、compact、maintenance report 仍然能工作；后者证明 snapshot save/load 没被 command dispatch 搬家影响。`tcp_resp_tests` 和真实 TCP smoke 则证明同一套 command dispatch 不只在 CLI 里可用，也能通过 server/RESP 路径工作。v1617 的真实 TCP smoke 发送 `PING dispatch-smoke`、`SET tcp-key tcp-value`、`GET tcp-key`、`CHECKJSON GET tcp-key`、`QUIT`，响应分别是 echo、插入成功、读回 value、只读 contract JSON 和 `BYE`，说明 TCP 上看到的仍是原 command 语义。

`dependabot_config_tests` 这次虽然名字像 Dependabot，但实际已经承担 CI workflow contract test。v1617 更新它，是为了保护 coverage filter 不退化。它断言 workflow 里必须出现扩展后的 core filter、`--gcov-ignore-parse-errors negative_hits.warn_once_per_file`、`--fail-under-line 88`、`mini-kv-core-coverage` artifact 等关键字符串。这个测试不能替代远端 Ubuntu coverage artifact，但能防止本地 review 时漏掉 YAML 质量门。

## 为什么 K4 还没有完成

v1617 是 K4 的第一刀，不是整个 K4。playbook 对 K4 的完整要求包括 command.cpp 拆成 dispatch + per-family executor 文件、`shard_readiness.cpp` 拆成 core data / JSON formatter / boundary validators，并评估 `shard_readiness_boundary_field_names.cpp` 是否能安全表驱动。v1617 只完成了 command dispatch translation-unit split 和 coverage filter 跟进。后续更合理的顺序是：先把 `command_dispatch.cpp` 内的 switch 进一步拆成 string/expiry/persistence/introspection 或类似 family executor，再处理 `shard_readiness.cpp` 的 evidence formatter 与 validator 分离，最后再让 Claude review K4。现在停在 v1617 就喊 K4 completed，会掩盖剩余风险；这版的正确结论是 K4 in progress，第一刀已验证。

## 拆分前后的调用链怎么看

维护者以后读这个路径，可以按三层去看。第一层是外层执行入口：`CommandProcessor::execute` 仍然在 `command.cpp`，它负责把原始行裁剪成 trimmed line，提取 metrics 使用的 command token，调用 `execute_trimmed`，最后把耗时和结果写进 `CommandMetricsTracker`。这层没有被搬走，是因为它横跨所有命令，不属于某个 command family；如果把 metrics 外壳和某个 family 绑在一起，后面审查 `RESETSTATS`、unknown command、空行和延迟统计时反而会更绕。

第二层是顶层调度：`execute_trimmed` 现在在 `command_dispatch.cpp`。它只回答一个问题：这一行命令应该走哪个执行分支。它仍然依赖 `command_catalog::lookup_dispatch_verb`，所以命令是否 known、属于哪个 dispatch verb、是否出现在 help/catalog/explain/check 输出里，仍然由 catalog 那套单一来源控制。这个设计很重要，因为 mini-kv 历史上曾经把命令注册、帮助文本、JSON catalog 和运行时 switch 分散在不同地方，后来已经通过 command catalog refactor 收敛过。v1617 没有把这个成果拆散，而是保留 catalog 作为事实来源，只把 switch 实现挪到专门文件。

第三层是具体执行：`SET`、`SETNXEX`、`DEL`、`EXPIRE` 这类写路径仍通过 `execute_with_wal`，先拿共享 WAL command gate，再决定 WAL record，append 成功后才 mutation；`GET`、`TTL`、`SIZE`、`KEYS` 这类读路径直接读 store 或 response formatter；`SAVE`、`LOAD`、`COMPACT`、`WALINFO` 仍然走 snapshot/WAL 的原函数；`RuntimeEvidence` 分支仍然回到 `execute_runtime_evidence_command`。这说明本版没有把具体业务行为“重新实现一遍”，只是把调用边界切清楚。后续拆 family executor 时，也应该延续这个原则：先把一组 case 原样搬到一个文件，再通过测试证明等价，而不是趁拆分重写语义。

## 为什么不直接一版拆到 family executor

从表面看，既然 K4 要求 per-family executor，v1617 好像可以直接把 string、expiry、persistence、introspection 全拆完。但这个项目的历史债不只是行数大，更是 evidence 面广。`command.cpp` 里既有真正读写 store 的命令，也有只读运行证据命令，还有 WAL/snapshot/admin 命令和 self-describing contract 命令。一次把所有 family 都拆完，diff 会同时包含接口设计、文件搬迁、helper 提取、coverage filter 更新、测试更新和文档解释，review 时很难判断某个失败来自“搬家失误”还是“新抽象失误”。

v1617 的选择是先建立一个中间稳定点。这个中间点有三个好处。第一，`command.cpp` 已经从约五万字节降到约三万八千字节，审查者能明显看到职责收敛。第二，`command_dispatch.cpp` 作为后续 family split 的唯一入口，已经独立编译并被全量测试覆盖。第三，最敏感的共享 gate 没有被复制，而是先独立成内部模块，后续 family executor 无论放在哪个文件，都可以复用同一个 gate。换句话说，这一版不是“只拆了一点点”，而是先把后面几刀最容易出错的地基打稳。

下一刀如果继续 command 侧，比较自然的拆法是把 `command_dispatch.cpp` 里的 case 分成若干私有 helper：基础 meta 命令如 `PING/HELP/QUIT` 一组，store read/write 如 `SET/GET/DEL/SIZE/KEYS` 一组，expiry/token 如 `SETNXEX/EXPIRE/TTL` 一组，persistence/WAL 如 `SAVE/LOAD/COMPACT/WALINFO` 一组，contract/evidence bridge 如 `EXPLAINJSON/CHECKJSON/RuntimeEvidence` 一组。是否把这些 helper 做成成员函数、自由函数，还是小型 executor struct，要看下一版 diff 大小和私有成员访问成本。现在不急着定死，因为 v1617 先给出了独立 dispatch 文件，下一版可以用更小 diff 试出最合适形状。

## coverage 更新怎么审

v1617 的 coverage 改动不要只看成“YAML 字符串变长”。它背后的判断是：拆分后的核心逻辑仍然必须被 K2 gate 约束。以前 `src/command.cpp` 一个文件包含外层执行、dispatch、helper、WAL gate、runtime evidence，coverage filter 只写 `command` 就能覆盖大部分 command 代码。现在 dispatch、parse helper、WAL gate 分成新文件，如果 filter 不加它们，coverage report 的分母会缩小，反而可能在代码迁移后看起来更漂亮。这种漂亮数字没有意义，因为最该被检查的新文件不在报告里。

因此，远端 CI 的 `ubuntu coverage` job 是 v1617 后续最值得看的信号。它会在 Linux/gcov 环境下重新生成 core report。维护者应关注三件事：第一，job 是否仍然 required 且没有 `continue-on-error`；第二，新 filter 是否真的列出 `command_dispatch`、`command_parse_helpers` 和 `command_wal_gate`；第三，TOTAL 行覆盖率是否仍高于 88。这个 floor 没有在 v1617 里上调，是因为 filter 分母改变后需要先看真实 artifact，再决定是否 ratchet。贸然在同一版提高 floor，会把“拆分是否等价”和“覆盖率目标是否提高”混在一起；现在保留 88，更适合作为过渡护栏。

K2 review 里提过一个后续提醒：command split 后可以重新评估 `--gcov-ignore-parse-errors negative_hits.warn_once_per_file` 是否还能删除。v1617 没有立刻删它，原因也很朴素：本版只完成 command 第一刀，远端 coverage 还没跑出新 artifact。正确时机应该是 command 侧几刀稳定后，单独开一个很小的质量版本，去掉该参数并观察 Ubuntu coverage 是否仍绿。如果绿，再删除并更新讲解；如果红，保留它并记录新的触发文件。这样不会让 K4 拆分本身背上工具链实验噪声。

## 人工 review 时该看什么

如果后续让 Claude 或人工 reviewer 看 v1617，最应该看的不是“新增了几个文件”这个表层，而是五个具体问题。第一，`WalCommandGate` 是否仍是单例共享，而不是 dispatch 和 command 主文件各拿一把锁。第二，usage/error 文案是否只被移动，没有在新旧文件之间出现双份不同实现。第三，`execute_trimmed` 是否仍只通过 command catalog 的 dispatch verb 分流，没有私自绕开 catalog。第四，coverage filter 和合同测试是否跟着新文件走，避免新核心逻辑逃出质量门。第五，README、START_HERE、progress ledger 是否如实说 K4 只是 in progress，而不是把第一刀包装成里程碑完成。

这五点比“文件数量增加了”更重要。合理的拆分有时会短期增加文件数，因为要把隐含边界显式化；不合理的拆分则会增加文件数但制造新耦合。v1617 属于前者：新增文件各自有明确职责，且没有把 public header 塞满私有 family 方法，也没有把 Node/Java 计划写进 C++ 运行时。它保持了一个好习惯：内部治理动作可以很强，但对外契约必须安静。

## 收尾价值

v1617 给后续维护带来的价值很具体。以后看 command 行为时，可以先从 `command_dispatch.cpp` 进入顶层 switch；看 runtime evidence 时，可以留在 `command.cpp`；看 usage/WAL/snapshot 文案 helper 时，可以去 `command_parse_helpers.*`；看 WAL append/mutation/compaction 的互斥语义时，可以去 `command_wal_gate.*`。这种拆分没有让代码“更炫”，但让审查路径更短，也让后续继续拆 family executor 时更不容易误碰 evidence handler。更重要的是，coverage filter 同步跟进，避免新文件逃出 K2 floor。对 Node/Java/mini-kv 松耦合融合来说，本版的价值是一句话：mini-kv 开始偿还 command 大文件债务，同时保持只读证据、无路由写入、无 credential、无外部执行这些跨项目边界完全不变。
