# v1614 production-excellence K2 coverage floor closure 代码讲解

## 本版目标、角色和不是什么

v1614 的目标很明确：把 v1613 已经跑绿的 Ubuntu coverage artifact 从“可查看的报告”升级成“每次 CI 都必须守住的覆盖率门槛”。v1612 先接入了 `MINIKV_COVERAGE` 和 Ubuntu coverage job，证明这套工程路径能配置、构建、执行完整 CTest，并能产出 core module 的 `gcovr` 报告；v1613 修复了 `gcovr 7.0` 在 GCC/gcov negative branch-hit 输出上的解析问题，让报告生成从失败恢复为绿色；v1614 则读取那次绿色 artifact，冻结基线为 2,292 条 core line、2,082 条已执行 line、90% total line coverage，并在 CI 中加入 `--fail-under-line 88`。这个 88 不是拍脑袋的数字，而是 K2 playbook 要求的“已测 baseline 减两点”。

本版的角色是生产化质量门禁，不是运行时功能版本。它不会新增 KV 命令，不会改 `SET`、`GET`、`DEL`、`EXPIRE`、`SNAPSHOT`、`RESTORE`、`SMOKEJSON`、`INFOJSON` 等命令输出，不会触碰 WAL 写入、snapshot 文件格式、RESP/TCP 协议、fixture schema，也不会把 mini-kv 变成 Node 或 Java 的 order authority / audit authority / storage backend。它也不是“为了 coverage 数字补测试”的版本：这次没有为了提高百分比去写低价值测试，而是先把真实现状固定下来，使后续 K3 logging、K4 file split 等改动不能在覆盖率层面悄悄倒退。

从治理角度看，v1614 是 K2 的收口点。K0 已经完成 START_HERE 和基础工程文件的快速修正，K1 已经把 sanitizer 从诊断轨道推进到 required gate，K2 到本版才算闭环：有 CMake option，有远端 CI，有 report artifact，有 parser hardening，有 baseline，有 floor，还有本地 CTest 守住 workflow 里的关键参数。按照 `production-excellence-program.md` 的 review protocol，coverage milestone 之后要请 Claude review，所以本版不会擅自进入 K3 logging。

## 入口命令和模块职责

本版真正改变的执行入口在 `.github/workflows/ci.yml` 的 `ubuntu coverage` job。这个 job 的流程保持 v1612-v1613 的结构：先安装 `gcovr`，再用 `cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DMINIKV_COVERAGE=ON` 配置 coverage build，然后构建、运行 full CTest，最后调用 `gcovr` 生成 `coverage/core-coverage.txt` 和 `coverage/core-coverage.html`。v1614 只在最后一步新增 `--fail-under-line 88`，意思是如果选定 core module 的总行覆盖率低于 88%，该 job 直接失败。

这里的“core module”不是整仓所有 C++ 文件，而是 K2 阶段刻意收窄的一组运行时核心文件：`store`、`command`、`command_catalog`、`command_contracts`、`command_response_formatters`、`command_smoke_formatters`、`wal`、`snapshot`、`resp`。选择这组文件，是因为它们承载 mini-kv 的核心状态机、命令调度/响应、WAL 持久化、快照读写、RESP 解析和自描述输出。大量治理 receipt、历史 evidence、fixture archive 文档暂时不纳入同一个 floor，是为了避免第一版 coverage gate 被历史治理文件稀释，也避免让后续 K4 拆分在尚未稳定前背上不合理的总仓覆盖率包袱。

对应的本地守护入口在 `tests/dependabot_config_tests.cpp`。这个测试名字来自 v110 的 Dependabot 配置守护，但后续已经自然扩展成“CI 工作流合同测试”：它不执行 GitHub Actions，而是读取 `.github/workflows/ci.yml` 文本，断言 required sanitizer job、coverage job、filter、artifact name、negative-hit parser policy 等关键字符串仍然存在。v1614 在这里新增对 `--fail-under-line 88` 的断言。这样以后如果有人为了让 CI 暂时通过而删掉 coverage floor，本地 CTest 会先失败，问题不会等到远端审查才暴露。

## 证据结构怎么理解

v1614 的证据分三层。第一层是远端 baseline artifact，也就是 GitHub Actions run `27408621850` 的 `mini-kv-core-coverage` artifact。它的 `core-coverage.txt` 记录了每个核心文件的 Lines、Exec、Cover 和 Missing line 列表，最后的 TOTAL 行是 `2292 2082 90%`。这一行是本版 floor 的唯一来源；不是 README 里的自述数字，也不是本机 Windows 工具链跑出来的估计值。

第二层是 workflow gate。`gcovr --fail-under-line 88` 把上述 baseline 固化成机器门禁。因为 `gcovr` 在 coverage 低于 floor 时会返回非零退出码，而该步骤在 required `ubuntu coverage` job 里运行，所以它天然和 build/test 一样参与 CI 结论。只要未来某个改动降低了 core module 覆盖率，比如拆 `command.cpp` 时丢掉一类命令 fixture、改 WAL 分支时未覆盖错误路径、改 RESP parser 时误删边界测试，coverage job 就会变红。

第三层是项目内 ledger 和 archive。`docs/production-excellence-progress.md` 把 K2 从 `measuring baseline` 更新为 `completed; awaiting Claude review`，并写入 baseline 与 floor；`START_HERE.md` 告诉下一位维护者现在不能直接开 K3，而是要先做 Claude review checkpoint；`README.md` 用一条 version summary 记录这次只是质量门槛，不是运行时行为改变；`f/1614/图片` 和 `f/1614/解释/说明.md` 则保存本版关键命令证据。这个结构让后续读者能从“计划要求”“CI 入口”“测试守护”“实际 artifact”“归档截图”五个方向交叉验证 v1614，而不是只相信一段叙述。

## 核心流程和边界字段

这次没有新增 JSON response 字段，但仍然需要解释边界。对 Node/Java/mini-kv 松耦合治理来说，`read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers`、`diagnostics` 这些字段的含义没有变化：v1614 不产生新的 runtime evidence command，也不改变旧 command 的字段解释。coverage job 只在 CI runner 上编译和测试代码，它不会启动长期服务，不会连接 Node 或 Java，不会读取 credential value，不会解析 raw endpoint，不会打开 managed audit connection，不会执行 restore/load/compact，也不会写入业务状态。

更具体地说，`--fail-under-line 88` 管的是源代码测试覆盖率，不是业务执行授权。它不会让任何只读证据变成可执行命令，也不会让 mini-kv 拥有 order authority。它只是提高维护约束：如果代码未来发生改动，必须在核心运行时路径上保留足够测试信号。这个边界很重要，因为 mini-kv 历史上有大量治理 receipt 和非参与证明，容易在文档叙述里把“证据更完整”误读成“权限更多”。v1614 的答案相反：证据更完整，是为了证明权限仍然没有被扩大。

`warnings` 层面，本版继续承认 v1613 记录过的 `gcovr` negative branch-hit parser deviation。`--gcov-ignore-parse-errors negative_hits.warn_once_per_file` 仍保留在命令中，因为它处理的是 GCC/gcov 的已知负数 hit 输出，并不降低 build/test/report 的硬要求。`blockers` 层面，K2 已完成，但按照 program review protocol，下一步不是随手开 K3，而是把 coverage milestone 交给 Claude review。`diagnostics` 层面，artifact 的 Missing 列表有实际价值：它指出 `command_response_formatters.cpp`、`snapshot.cpp`、`resp.cpp`、`wal.cpp` 等文件仍有未覆盖分支，后续 K3/K4 做 logging 和 file split 时，可以把这些缺口当作补测试的候选地图，而不是盲目追逐百分比。

## 测试保护的行为

本地 focused 测试使用 `dependabot_config_tests`，它保护的是 CI 合同而不是业务逻辑。它断言 workflow 中必须存在 `name: ubuntu coverage`、必须安装 `gcovr`、必须用 `MINIKV_COVERAGE=ON` 配置、必须构建 coverage target、必须跑 coverage build 的 CTest、必须使用限定 core module 的 filter、必须保留 negative-hit parser policy、必须保留 `--fail-under-line 88`、必须上传 `mini-kv-core-coverage` artifact。这个测试的价值在于把“CI 质量门槛”变成仓库内可审查的 C++ 断言，避免 YAML 被轻易削弱。

full CTest 仍然保护运行时行为。虽然 v1614 没改命令执行代码，但每次 workflow 或 CMake 相关改动都可能间接影响构建、测试发现、路径解析或 Windows 本地工具链行为，所以本版仍按 mini-kv closure discipline 重新配置、重新构建、跑 focused test，再跑完整 CTest。完整测试覆盖 Store、Command、WAL、Snapshot、RESP、TCP server/client、metrics file、fixtures、recovery evidence 等历史回归面。它证明本版新增 coverage floor 没有破坏默认 Debug 构建，也没有改变普通测试可执行文件的运行状态。

远端 CI 则是最终门禁。Windows 和 macOS 继续跑默认 build/test，Ubuntu 默认 job 继续跑默认 build/test，Ubuntu sanitizer job 继续 required，clang-format changed files job 继续守住被触碰文件格式，Ubuntu coverage job 现在多守一条 88% floor。这个组合的意义是：v1614 不是单独依赖“我本机通过了”，而是把生产化质量门槛放进跨平台 CI，由远端 runner 以可追溯 run id 给出结论。

## 后续版本怎样使用这个门槛

后续维护者读 v1614 时，不应该把 `--fail-under-line 88` 理解成“以后只要有 88 就够了”。更合适的理解是：从 v1614 开始，core runtime 覆盖率的下界被放进 CI，任何低于它的改动都需要先解释为什么测试信号下降，再决定是补测试、拆分提交，还是在非常明确的版本里调整 floor。也就是说，v1614 建立的是一个可以被讨论、可以被审计、可以被逐步提高的质量制度，而不是一次性数字游戏。

对 K3 logging 来说，这条门槛能约束最容易出错的地方。K3 会在 `tcp_server.cpp`、`wal.cpp`、`snapshot.cpp`、`command.cpp` 等边界附近替换裸 `std::cerr`，引入最小自研 leveled logger，并保持 stdout evidence 不变。日志改造很容易产生两类风险：一类是把应该留在 stdout 的证据混进 stderr 或反过来，另一类是为了接入 logger 改动了错误路径、flush 时机、异常处理或命令响应。coverage floor 不能替代精确断言，但它可以防止核心路径覆盖率在改日志时大面积滑落；如果滑落，维护者至少要停下来检查是不是某个分支从测试视野里掉出去了。

对 K4 file split 来说，这条门槛更关键。`command.cpp` 和 `shard_readiness.cpp` 的拆分目标是降低大文件维护成本，但拆分不是“把代码剪到不同文件里就算完成”。真正的合格拆分需要做到输出 byte-identical、错误路径不丢、历史 fixture 不重写、测试仍能从外部命令和 evidence 角度证明行为相同。v1614 的 filter 覆盖了 command 相关的多个文件、WAL、snapshot、RESP，这意味着 K4 在拆 `command.cpp` 时不能只看编译通过；如果拆分把某些命令 family 的边界分支弄丢，coverage job 很可能会先发出信号。后续还可以在 K4 中把拆出的新 translation unit 加进 filter，或者用一次专门版本把 filter 从旧文件名迁移到新文件名集合。

对日常小修来说，这条门槛也能提供一个“别把质量债藏进文档”的提醒。mini-kv 历史上有大量 versioned evidence、截图、讲解和非参与证明，这些材料很有价值，但它们不能替代可执行测试。v1614 把 K2 记在 `docs/production-excellence-progress.md`，同时又把真正会失败的逻辑放在 GitHub Actions 和 CTest 中，就是为了避免出现“文档说覆盖率很重要，但 CI 实际不管”的空心治理。以后如果某个版本只更新讲解、索引或 archive 说明，coverage job 不会制造负担；但只要动到 core runtime，CI 就会自动给出下限检查。

还要注意，coverage artifact 的 Missing 列表不是责备清单，而是后续补强地图。比如 `command_response_formatters.cpp` 的缺口更多集中在响应格式化边界，`snapshot.cpp` 的缺口涉及异常和文件状态，`resp.cpp` 的缺口通常靠畸形协议输入触发，`wal.cpp` 的缺口会牵涉校验、截断、修复和 compact 边界。后续要提高 floor，最健康的做法不是写“为了执行某一行”的脆弱测试，而是围绕真实故障模型补测试：坏 snapshot、坏 WAL tail、RESP 半包、未知命令、过期 token、metrics rotation、日志级别过滤等。这样 floor 上升才会带来真实维护价值。

## 后续维护价值

K2 做到 v1614 后，后续 K3 logging 和 K4 command/shard readiness file split 才有更可靠的护栏。尤其 K4 会动到 oversized files，而 `command.cpp`、`command_response_formatters.cpp`、`wal.cpp`、`snapshot.cpp`、`resp.cpp` 正是本次 coverage filter 覆盖的核心文件。如果拆分时遗漏分支、误删测试路径、或让某些行为不再被测试触达，CI 会给出直接反馈。这个反馈比“拆完以后人工读一遍”更稳定，也比继续堆文档证据更接近生产工程约束。

本版的一个小取舍是 floor 设为 88 而不是 90。原因不是降低标准，而是遵守 playbook 的 baseline-minus-two 规则：第一版 coverage gate 要吸收不同 runner、gcovr/gcov 输出细节和后续小规模重构带来的微小波动，先保证门槛真的能长期执行。等 K3/K4 稳定后，可以再通过专门版本把 floor ratchet 到 90 或更高。门槛的方向应该是可解释、可复现、可逐步提高，而不是一次写一个看似漂亮但容易被删掉的数字。

一句话总结：v1614 把 mini-kv 的 core coverage 从“绿色报告”收束成“required CI floor”，让后续 Node/Java/mini-kv 松耦合融合中的 C++ 侧改动拥有可审计的测试下限，同时继续保持 no router、no write、no WAL side effect、no credential、no execution 的边界不变。
