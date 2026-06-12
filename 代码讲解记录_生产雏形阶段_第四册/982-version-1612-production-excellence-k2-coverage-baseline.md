# v1612 production-excellence K2 coverage baseline

## 目标、角色和不是什么

v1612 的目标是启动 production-excellence K2 覆盖率里程碑，并且只做其中最应该先落地的半步：让 mini-kv 有一个可重复的 `MINIKV_COVERAGE` 构建开关，让 GitHub Ubuntu CI 能完整配置、构建、执行 CTest，然后用 `gcovr` 产出核心模块覆盖率报告 artifact。这个版本不是最终覆盖率门禁，因为 baseline 数字必须从真实 Ubuntu CI 里读出来，再按 playbook 要求设置为 baseline minus 2 points 的 floor。也就是说，v1612 负责把测量链路接起来，下一版才把测量结果固化成失败条件。

这版的角色是 K2 的基线产出版。它不是新的运行时命令，不新增 CLI/TCP 协议，不新增 SHARDJSON 或 SMOKEJSON 字段，不调整 WAL、snapshot、RESP、store 的行为，也不参与 Node/Java/aiproj 的任何写入或执行。它更不是 K4 的大文件拆分版本，`command.cpp` 和 `shard_readiness.cpp` 在本版不会被拆。v1612 只建立覆盖率测量的工程轨道，让后续拆分前有一个能被 CI 机械执行的覆盖率信号。

这里的“不是什么”必须写清楚。覆盖率很容易被误读成生产 readiness 证明，实际上它只回答“测试跑到了哪些源文件和哪些行”。它不能证明没有业务漏洞，不能证明真实四项目联调已经发生，也不能授权 mini-kv 启动服务、读取 endpoint、解析 credential、执行 restore/load/compact、写 store、触 WAL 或打开 router。覆盖率门禁的意义是防止核心模块测试保护悄悄退化，给后续重构提供一个量化底线。它是质量工具，不是执行授权工具。

## 为什么先做 baseline 而不是直接写阈值

K2 playbook 要求的顺序是先测量当前核心模块覆盖率，再把 floor 设置在 measured baseline minus 2 points。这个顺序不是形式主义。mini-kv 的代码结构很特殊，真正的存储核心、命令解析、WAL、snapshot、RESP 模块相对小而密，旁边有大量治理证据、fixture、receipt 和历史非参与证明。直接凭感觉写一个阈值，风险有两种：阈值太低会成为摆设，阈值太高会把 CI 变成反复修工具链的噪声。v1612 因此只先接线，让 Ubuntu CI 生成真实报告。

本地 Windows MinGW 环境也逼着我们做这个拆分。普通 Debug 构建和 334 条 CTest 可以在 CLion 捆绑 MinGW 下稳定运行，但 gcov instrumentation 会为每个对象旁边生成 `.gcno` sidecar。这个仓库有大量历史长文件名和长测试 target，覆盖率 sidecar 让路径进一步变长，实际构建会在深层目标处报无法打开 `.gcno` 或依赖文件。这个问题不是 `MINIKV_COVERAGE` 的语义错误，而是 Windows MinGW 侧路径和 gcov sidecar 的组合限制。与其让本地构建跑到一半失败，v1612 在 CMake 配置阶段直接给出明确错误，提示使用 Ubuntu coverage CI job。

因此，v1612 的策略是“Windows 默认路径继续可用，Windows coverage 明确拒绝，Ubuntu coverage 作为 source of truth”。这和 K1 sanitizer 的分工相似：本地默认构建和测试负责证明常规开发路径没坏，GitHub Ubuntu job 负责证明对应质量工具链能真实运行。不同的是，K1 在 v1611 已经从诊断转成 required gate；K2 在 v1612 只是 required coverage job 产出报告，还没有设置 fail-under floor。下一版读出报告后，再把 floor 加上，才算 K2 完成。

## CMake 入口和开关语义

本版的 CMake 入口在 `CMakeLists.txt`。新增的开关是：

```cmake
option(MINIKV_COVERAGE "Enable gcov-compatible coverage instrumentation for GCC/Clang Debug checks" OFF)
```

这个开关默认关闭，不影响正常开发、正常 CI matrix、正常 CLI/TCP 运行，也不影响现有测试执行。只有显式传入 `-DMINIKV_COVERAGE=ON` 时，CMake 才会进入覆盖率配置分支。这样做的原因很简单：coverage instrumentation 会降低构建和运行速度，也会产生 `.gcda`、`.gcno` 等 sidecar 文件，它应该只存在于覆盖率 lane，而不应该污染普通 Debug 构建。

本版还加了一条互斥规则：

```cmake
if(MINIKV_SANITIZE AND MINIKV_COVERAGE)
    message(FATAL_ERROR "MINIKV_SANITIZE and MINIKV_COVERAGE are separate CI lanes; enable only one at a time")
endif()
```

这条规则避免把 sanitizer 和 coverage 混在一个构建里。两者都属于质量工具，但它们的运行成本、输出语义和失败解释不同。ASan/UBSan 关注内存和未定义行为，coverage 关注测试触达范围。混在一起会让 CI 变慢，也会让失败原因变得模糊。mini-kv 现在的正确策略是单独的 Ubuntu sanitizer job 和单独的 Ubuntu coverage job，各自配置、构建、测试、报告。

覆盖率真正启用时，CMake 对 GCC/Clang 添加：

```cmake
add_compile_options(--coverage -O0 -g)
add_link_options(--coverage)
```

`--coverage` 让编译器和链接器生成 gcov 可读取的数据；`-O0` 保证行覆盖率尽量贴近源码结构；`-g` 保留调试信息，便于报告归因。这个组合不改变运行时命令协议，也不改变 store/WAL/snapshot/RESP 的逻辑，只改变测试构建的插桩方式。CI 跑完 CTest 后，`gcovr` 才会读取这些 coverage 文件并生成 summary 和 HTML。

Windows MinGW 分支则显式失败：

```cmake
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "MINIKV_COVERAGE writes gcov sidecar files whose paths exceed the bundled Windows MinGW limit in this repository. Use the Ubuntu coverage CI job.")
endif()
```

这不是退缩，而是把已知限制前置。没有这条规则，开发者会看到一长串深层对象路径和 `.gcno` 打不开的编译错误，很难判断是源码、测试、CMake、MinGW 还是覆盖率本身的问题。现在错误在配置阶段出现，语义清楚：默认 Windows 构建继续支持，coverage 请看 Ubuntu CI。

## CI coverage job 的结构

v1612 在 `.github/workflows/ci.yml` 新增 `coverage` job，名字是 `ubuntu coverage`。这个 job 不挂在 matrix 里，而是单独运行在 `ubuntu-latest`。原因是 coverage 不是三平台兼容性检查，而是核心模块测试触达率检查。把它单独列出来，可以让 job 名、artifact、日志和后续门禁语义都更清楚。

job 的步骤按五段走。第一段 checkout 仓库。第二段安装 `gcovr` 并打印版本。第三段配置 coverage build：

```yaml
cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DMINIKV_COVERAGE=ON
```

第四段构建全部 target set，并保留 45 分钟 timeout。第五段运行完整 CTest：

```yaml
ctest --test-dir build-coverage -C Debug --output-on-failure --timeout 120 --progress
```

这一步非常重要。coverage 报告不是只编译几个文件就算了，而是必须让 full CTest 真实跑过，才会产生 `.gcda` 执行数据。mini-kv 的测试里既有核心路径，也有大量治理 evidence 和 fixture 路径；本版报告只统计核心模块，但 CTest 仍完整运行，这样可以保持和普通 Debug、sanitizer lane 相同的行为覆盖面。

最后一段生成报告：

```yaml
gcovr --root . --object-directory build-coverage --filter "src/(store|command|command_catalog|command_contracts|command_response_formatters|command_smoke_formatters|wal|snapshot|resp)\\.cpp" --txt coverage/core-coverage.txt --html-details coverage/core-coverage.html
```

这里的 filter 是本版最关键的取舍。K2 playbook 点名核心模块是 store、command、wal、snapshot、resp。mini-kv 的 command 现在已经有 catalog、contracts、response formatters、smoke formatters 等拆分辅助文件，所以 filter 没有只看 `src/command.cpp`，而是把这些 command 相关核心文件一起纳入。相反，大量 runtime credential resolver、managed audit receipts、shard preview receipt、historical non-participation evidence 文件没有纳入这个核心 floor。它们不是不重要，而是语义不同：那些更像治理证据和文档化 contract，后续如果要覆盖率门禁，应该单独设一个 lower floor 或 fixture/evidence quality gate，不应该和真正的 store/command/WAL/snapshot/RESP 混成一个数字。

job 最后上传 `mini-kv-core-coverage` artifact，里面有 `core-coverage.txt` 和 HTML details。v1612 需要的远端证据就是这个 artifact 和 job 日志。下一版会从这里读取 baseline，再设置 `--fail-under-line`。如果这个 job 本身配置、构建、CTest 或 gcovr 失败，workflow 会失败；它不是 `continue-on-error` 的软失败诊断。但它暂时没有覆盖率数值 floor，因为数值还没量出来。

## 测试如何保护 CI 契约

本版继续沿用 `tests/dependabot_config_tests.cpp` 这个本地 CI 契约测试。名字里还带 dependabot，是历史原因；现在它实际保护两类配置：Dependabot 的 github-actions 更新范围，以及 CI workflow 的关键门禁。v1612 在这里新增 coverage job 断言，检查以下内容必须存在：

```cpp
assert_contains(ci_workflow, "name: ubuntu coverage");
assert_contains(ci_workflow, "sudo apt-get install -y gcovr");
assert_contains(ci_workflow, "cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DMINIKV_COVERAGE=ON");
assert_contains(ci_workflow, "cmake --build build-coverage --config Debug --parallel 4 --verbose");
assert_contains(ci_workflow,
                "ctest --test-dir build-coverage -C Debug --output-on-failure --timeout 120 --progress");
assert_contains(ci_workflow, "name: mini-kv-core-coverage");
```

这些断言的价值不是替代 GitHub Actions，而是防止 CI 结构被无声改坏。比如有人以后删掉 `gcovr` 安装、把 CTest 换成只跑一个测试、去掉 artifact 上传，或者不小心改掉 coverage build 目录，本地 CTest 就会先提醒。它也继续保留 v1611 的 `assert_not_contains(ci_workflow, "continue-on-error: true")`，所以 coverage job 和 sanitizer job 都不能被静默改成软失败。

这里还有一条 filter 断言，保护核心模块统计范围。它检查 `src/(store|command|command_catalog|command_contracts|command_response_formatters|command_smoke_formatters|wal|snapshot|resp)\\.cpp` 这段文本存在。没有这条保护，coverage job 可能仍然存在，但统计范围被改成全仓库、空范围或只看一个文件，baseline 就会失真。覆盖率门禁最危险的退化之一就是“数字还在，但数字的分母变了”。v1612 把分母的关键轮廓写进测试，后续调整 filter 必须显式改测试和解释。

当然，这类字符串测试不是 YAML parser，也不是 coverage policy engine。它保护的是当前阶段最容易出错的结构性事实。更重的解析测试可以以后再做，但当前 repo 已有大量测试目标，K2 的第一步不应该为了完美解析而扩大范围。现在的断言足够回答一个实用问题：coverage job 是否还在用正确目录、正确开关、完整 CTest、核心 filter 和 artifact 上传。

## 与边界字段和只读契约的关系

v1612 没有新增任何 runtime evidence 字段，所以不会改变 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers`、`diagnostics` 的业务含义。控制面仍然应该这样理解 mini-kv：它可以提供只读证据、命令自描述、fixture、runtime smoke 证明和 release/restore 边界说明，但它不是 Java 订单权威，不执行 Node 的计划，不读取生产 credential，不打开 managed audit connection，不自动启动服务，不写入外部系统。

coverage 影响的是这些字段背后的工程可信度。举例来说，`CHECKJSON` 或 `SMOKEJSON` 里的 `read_only` 字段如果有测试覆盖，coverage 报告可以帮助我们看到相关 command formatter 是否被触达；WAL 和 snapshot 的覆盖率可以帮助判断重构前后是否仍有测试保护；RESP 的覆盖率可以帮助判断 TCP/RESP 兼容路径是否被测试触达。但是 coverage 只说明测试跑过，不说明字段值本身正确。字段正确性仍由专门的 C++ 测试、fixture 对比和 smoke 测试保护。

这也是为什么本版没有把 evidence/receipt 模块纳入同一个 core floor。许多 receipt 文件的价值在于冻结边界文字和非参与事实，而不是提供热路径运行逻辑。把它们混进 core coverage，可能会让大量文档化证据拖低数字，进而诱导维护者写没有价值的覆盖率填充测试。K2 的第一阶段应该保护真正会支撑 K4 重构的核心模块，也就是 store、command、WAL、snapshot、RESP。治理证据以后可以用更适合的质量门，比如 fixture diff、schema assertion、walkthrough quality gate、archive completeness check，而不是强行塞进同一个 line coverage 百分比。

## 本地验证和远端验证的分工

本地验证分三层。第一层是 coverage fast-fail：在 Windows/MinGW 下执行 `cmake -S . -B cmake-build-v1612-coverage -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DMINIKV_COVERAGE=ON`，预期配置失败，并显示明确的 `MINIKV_COVERAGE writes gcov sidecar files... Use the Ubuntu coverage CI job.` 这证明本地已知限制被前置，而不是让用户在构建中途读路径错误。

第二层是默认 Debug 构建：执行 `cmake -S . -B cmake-build-v1612 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug`，然后 `cmake --build cmake-build-v1612 -- -j 4`。这证明新增 coverage option 默认关闭，不会污染常规构建。构建过程中仍可能出现既有长 target 的 CMake path warning，但普通对象仍能成功生成，这和 v1611 前后的状态一致。

第三层是 CTest。focused test 的重点是 `dependabot_config_tests`，因为它直接保护 CI coverage job 的结构。全量 CTest 则证明 334 条测试继续通过，包含 store、command、WAL、snapshot、RESP、TCP、fixture 和治理 evidence 测试。v1612 的本地结果是 334/334 通过，这说明 coverage 基线接线没有影响默认 runtime 行为。

远端验证则由 GitHub Actions 完成。推送 v1612 后，`ubuntu coverage` job 应该安装 gcovr、配置 coverage build、构建全部 target、跑完整 CTest、生成 report、上传 artifact。只有远端 job 绿并产出 `mini-kv-core-coverage`，下一版才有真实 baseline 可以使用。如果远端 job 红，不能直接进入阈值版本；应该先读日志，判断是工具安装、CMake、构建、测试还是 gcovr filter 的问题。

## 下一版如何完成 K2

v1612 完成后，下一版要做三件事。第一，读取 GitHub Actions 的 `ubuntu coverage` job 日志或 artifact，记录 core coverage 的 line percentage。第二，在 CI 的 `gcovr` 命令里加入 `--fail-under-line <floor>`，其中 `<floor>` 应该是 measured baseline minus 2 points，通常向下取整或按 gcovr 支持的数值格式写清楚。第三，把 `docs/production-excellence-progress.md` 的 K2 从 measuring baseline 改成 completed，记录 baseline 和 floor，并再次跑本地默认构建、CTest、远端 CI。

只有这三件事完成，才应该停下来让 Claude review。production-excellence program 的 review checkpoint 是 after each project's coverage milestone，不是 coverage job 刚出现时，也不是 sanitizer gate 刚完成时。Claude 应该看的不是“有没有 YAML job”，而是“sanitizer required gate 加 coverage required floor 是否足够支撑后续 K4 文件拆分”。如果现在就 review，缺少 baseline 和 floor，只能得到半个答案。

因此，v1612 的价值是把不可见的下一步变成可见的报告。它让团队知道覆盖率数据从哪里来、统计哪些核心文件、Windows 本地为什么不跑 coverage、CI artifact 放在哪里、测试如何防止 job 退化。下一版再把数字变成门槛，K2 就闭环了。

## 风险和回滚

本版主要风险有三个。第一，Ubuntu `gcovr` 版本可能和命令参数不完全兼容。如果远端 CI 在 `--html-details` 或 filter 上失败，应优先调整 CI 命令，而不是删除 coverage job。第二，coverage build 可能比普通 Debug 慢，尤其项目 target 数量很多。本版给 build 和 test 保留了和 sanitizer 相同的 timeout，如果 CI 超时，需要看是编译太慢、测试太慢，还是 coverage artifact 生成太慢。第三，filter 可能需要后续细化。比如 command 相关文件如果继续拆分，K2 filter 也应该随之更新，并在测试和讲解中同步说明。

回滚路径也很清楚。若 v1612 的 coverage job 在 Ubuntu 上证明不可运行，可以回滚本版，回到 v1611 的 sanitizer required 状态；但如果失败原因只是 baseline 不理想，不应该回滚。baseline 不理想正是 coverage 的价值，它告诉我们哪里需要补测试或拆分前先加保护。K2 的目标不是制造漂亮数字，而是建立不会倒退的真实底线。

还有一个维护提醒：本版通过 `python -m pip install --user gcovr` 在本机装了 gcovr 用于环境确认，但仓库没有把它写成项目依赖，也不要求 Windows 本地 coverage。真正的工具来源是 GitHub Ubuntu job 的 `sudo apt-get install -y gcovr`。如果以后希望本地 Linux 或 WSL 也能跑 coverage，可以补一份开发者说明，但不要把 Windows MinGW 的 path limitation 当成源码错误。

一句话总结：v1612 把 mini-kv 的 K2 覆盖率链路从无到有接入到 CMake 和 Ubuntu CI，明确 Windows MinGW coverage 的本地限制，用 C++ 测试守住 CI job 结构和核心统计范围，并为下一版按真实 baseline 固化 coverage floor 做好证据准备，同时保持运行时命令、只读证据、归档路径、写入、WAL 和执行边界全部不变。
