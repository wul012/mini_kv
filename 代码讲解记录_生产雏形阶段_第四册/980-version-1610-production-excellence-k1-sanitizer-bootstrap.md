# v1610 production-excellence K1 sanitizer 诊断引导

## 目标、角色和不是什么

v1610 的目标是推进 production-excellence mini-kv playbook 的 K1：让项目开始拥有 AddressSanitizer 和 UndefinedBehaviorSanitizer 的自动化验证轨道。这个版本不是要新增一个运行时命令，也不是要继续扩展 `SHARDJSON`、`SMOKEJSON` 或其他治理 evidence 的字段，更不是把 mini-kv 带进 Node 或 Java 的执行链。它做的是一件更靠近工程地基的事情：给 CMake 增加一个显式开关 `MINIKV_SANITIZE`，让 GCC/Clang 构建可以在 Debug 模式下注入 `-fsanitize=address,undefined` 与 `-fno-omit-frame-pointer`；同时在 GitHub Actions 中新增 Ubuntu sanitizer job，用真实 Linux 工具链跑完整构建和完整 CTest。

本版的角色是“诊断轨道引导版”。它先把 sanitizer 入口接到 CI，但保持 `continue-on-error: true`。这个选择不是降低要求，而是符合 K1 的推进顺序：先让新轨道产生信号，观察是否有真实 sanitizer findings，再决定是否能升级为 required gate。mini-kv 现在有三百多个测试目标，历史 governance 代码和长字符串证据非常多，如果一上来把 sanitizer 设成强制门，任何工具链差异、运行时库缺失、平台特性问题都会把生产卓越推进卡死。v1610 先把通道铺好，并用测试证明通道存在；后续如果 Ubuntu CI 绿，就在下一版去掉 `continue-on-error`，让 K1 完整闭环。

本版明确不是什么同样重要。它不改变默认 Debug 构建，不改变 `project(mini_kv VERSION 0.102.0)`，不修改 frozen runtime fixture hint `c/102/`，不移动 `a/` 到 `f/` 的历史归档根，不改 release fixtures，不启动 TCP server，不读 raw endpoint，不解析 credential value，不打开 active router，不写 store，不触 WAL，不执行 restore/load/compact，也不让 mini-kv 对 Node 或 Java 产生任何控制面写入。sanitizer 只是编译和测试时的质量观察工具，不是运行时功能，更不是执行授权。

## 入口：CMake 的 MINIKV_SANITIZE

本版的主要入口在 `CMakeLists.txt` 顶部。K0 已经把当前 archive hint 抽成 `MINIKV_CURRENT_ARCHIVE_VERSION`，v1610 在相邻位置新增：

```cmake
option(MINIKV_SANITIZE "Enable AddressSanitizer and UndefinedBehaviorSanitizer for GCC/Clang Debug checks" OFF)
```

默认值是 `OFF`，因此普通开发者、普通 CI matrix、历史本地构建都不会因为本版自动加 sanitizer flag。只有显式传入 `-DMINIKV_SANITIZE=ON` 时，CMake 才会检查编译器并追加 sanitizer 选项。判断逻辑只接受 `Clang|GNU`，因为本版目标是 GCC/Clang 风格的 ASan/UBSan 轨道。匹配后追加：

```cmake
add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
```

这里选择目录级 `add_compile_options` 和 `add_link_options`，不是因为想扩大影响面，而是因为 mini-kv 的测试目标中有一部分不是单纯链接 `minikv` 静态库。例如 `minikv_dependabot_config_tests` 是读取 GitHub Actions 和 Dependabot 配置的独立测试目标，它也应该被 sanitizer 构建编译链接到。若只给 `minikv` library 加 sanitizer flag，独立测试目标会脱离 K1 轨道；若逐个 target 增加选项，又会在当前 CMake 文件已经很长的情况下制造重复维护点。目录级选项在开关关闭时零影响，在开关开启时覆盖所有后续目标，正好符合“诊断构建整体观察”的目标。

这个入口还有一个本机工具链保护分支：Windows + GNU 时直接 `FATAL_ERROR`，提示 CLion 捆绑 MinGW 不提供 `-lasan`/`-lubsan`。这个判断来自真实验证，不是预设猜测。v1610 先用本机 MinGW 配置了 `cmake-build-v1610-sanitize`，对象文件和 `libminikv.a` 能编译出来，但链接第一个独立测试可执行文件时失败，报 `cannot find -lasan` 和 `cannot find -lubsan`。如果不把这个情况收敛成配置期错误，下次维护者会在漫长的构建输出末尾才看到 linker failure，然后误以为项目代码本身不支持 sanitizer。现在错误在 CMake 阶段就说明：本地 Windows MinGW 缺 sanitizer runtime，请使用 Ubuntu sanitizer CI job。这是质量信息，不是功能退让。

## CI：Ubuntu sanitizer 诊断轨道

`.github/workflows/ci.yml` 新增 `sanitizer` job，名称是 `ubuntu sanitizer`，运行在 `ubuntu-latest`。它分三步：checkout、配置 sanitizer build、构建 sanitizer target set、运行 sanitizer CTest。配置命令是：

```text
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DMINIKV_SANITIZE=ON
```

构建命令是：

```text
cmake --build build-sanitize --config Debug --parallel 4 --verbose
```

测试命令是：

```text
ctest --test-dir build-sanitize -C Debug --output-on-failure --timeout 120 --progress
```

这条 CI 轨道没有放进现有 matrix，原因有三点。第一，sanitizer 的主战场是 Linux GCC/Clang，Windows runner 的 MSVC/MinGW sanitizer 行为和本地 CLion 工具链不同，直接塞进三平台 matrix 会把 K1 变成跨平台 sanitizer 兼容性工程。第二，macOS 的 sanitizer 支持、运行时行为和默认系统库也有差异，K1 当前只需要先把最可靠的 Linux ASan/UBSan 信号接起来。第三，单独 job 更便于后续把 `continue-on-error` 从 diagnostic 翻成 required，不需要干扰普通 `build-and-test` matrix。

`continue-on-error: true` 是本版最关键的状态字段。它告诉维护者：v1610 已经让 CI 跑 sanitizer，但 K1 尚未完全关闭。这个字段不是永久豁免，progress ledger 也明确写了下一步：观察 Ubuntu CI，如果绿，就在后续版本移除 `continue-on-error: true`。这比在文档里写“以后要做”更可执行，因为当前 CI 已经会实际跑 sanitizer，所有失败输出都会出现在 GitHub Actions 中；后续要做的不是设计新轨道，而是根据真实结果修复或升级门禁。

## 测试：dependabot_config_tests 保护 CI 契约

mini-kv 已经有一个 `tests/dependabot_config_tests.cpp`，虽然名字来自 Dependabot，但它实际承担了 GitHub Actions 和依赖更新配置的合同检查。v1610 没有另建一个薄薄的 CI 测试文件，而是在这个既有守卫里追加 sanitizer job 的关键断言：job 名称、`continue-on-error: true`、配置命令、构建命令和 CTest 命令都必须存在。

这些断言保护的不是“字符串好看”，而是 K1 的可执行语义。如果后续有人删除 sanitizer job，或者把 `-DMINIKV_SANITIZE=ON` 拿掉，或者把 CTest 步骤改成只跑几个 smoke test，`dependabot_config_tests` 会在本地 CTest 阶段失败。它让 CI 配置不只是 YAML 文件，而是被 C++ 测试纳入仓库内的质量契约。这个做法和 mini-kv 以往大量 fixture/evidence 测试一致：重要边界尽量变成可运行的断言，而不是只靠 README 记忆。

本版也保留了一个有意的阶段性断言：测试明确要求 `continue-on-error: true` 存在。这意味着 v1610 的测试在保护“诊断状态”，不是最终状态。后续 v1611 或其他 K1 follow-up 要把 sanitizer 变成 required 时，必须同步改这个测试：从要求存在改为要求不存在，或者改成更精确的 required-gate 断言。这个测试会逼迫维护者认真处理阶段转换，不会让 CI YAML 悄悄漂移。

## Windows MinGW 限制为什么写进 CMake

在本机验证中，默认 Debug 构建已经完整通过；`MINIKV_SANITIZE=ON` 配置也曾经通过，但构建到链接阶段失败，因为 CLion 捆绑的 MinGW toolchain 没有提供 ASan/UBSan runtime 库。这个失败点如果只写在归档说明里，下一次运行本机 sanitizer 的人仍会踩一次。v1610 因此把它写进 CMake：

```cmake
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "MINIKV_SANITIZE requires ASan/UBSan runtime libraries; ...")
endif()
```

这个分支只在显式开启 sanitizer 时生效，普通 Windows MinGW Debug 构建完全不受影响。它也没有说“Windows 永远不支持 sanitizer”，而是针对当前 GNU/MinGW 路径给出明确反馈。将来如果项目引入另一个 Windows Clang 工具链、MSVC sanitizer 配置或预装 runtime 的 MinGW 环境，可以再开单独版本扩展判断。当前 K1 的重点不是把所有平台 sanitizer 一口气做完，而是让最可靠的平台先产生持续信号。

这个设计还减少了误判。没有这个分支时，构建输出会先编译上百个源文件，再在链接 `minikv_dependabot_config_tests.exe` 时失败。失败日志很长，容易被误读成某个测试目标、某个对象文件或某个源码模块的问题。现在 configure 阶段直接告诉你缺 `-lasan`/`-lubsan`，并指向 Ubuntu CI。工程质量不仅是加工具，也是把失败变短、变准、变可解释。

## 默认构建和运行时边界没有变化

v1610 改的是构建开关和 CI，不碰运行时状态机。默认 `MINIKV_SANITIZE` 是 `OFF`，所以 `cmake -S . -B cmake-build-v1610 -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug` 仍按普通 Debug 方式生成项目。后续 `cmake --build cmake-build-v1610 -- -j 4` 完整通过，说明新增选项没有污染普通构建，也没有影响 `minikv` 静态库、CLI、server、client、benchmark 或 334 个测试目标。

全量 CTest 334/334 通过，是本版保持边界的核心证据。它覆盖 store、command、command catalog、shard readiness、route preview、WAL、snapshot、RESP、TCP、metrics、recovery evidence、runtime smoke evidence、current runtime fixture rolling guard，以及大量 non-participation receipt。它证明新增 sanitizer 开关没有改变命令输出，没有改变 fixture 语义，没有改变 `INFOJSON`/`SMOKEJSON`/`SHARDJSON` 的只读边界，也没有让 mini-kv 开始读 Node/Java 文件、启动服务、打开 router 或写 WAL。

这里尤其要注意 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers`、`diagnostics` 这类字段。本版没有新增一个需要这些字段解释的新 evidence command，但它依然必须保护这些字段的既有意义：只读 evidence 仍然是只读，执行授权仍然是 false，mini-kv 仍不是订单权威，诊断 warning/blocker 仍只是控制面解释，不是自动执行入口。sanitizer 可以帮助发现内存越界、use-after-free、未定义行为等 C++ 风险，但它不会改变这些控制字段的业务含义。

## 本地验证链路

v1610 的本地验证分成五层。第一层是 `git diff --check`，用于捕捉 trailing whitespace 和补丁级格式瑕疵。第二层是 CMake 默认配置，确认普通 Debug build 目录可生成。第三层是默认构建，确认所有目标仍能编译链接。第四层是 focused CTest，覆盖与本版最相关的 `dependabot_config_tests`、`command_tests`、`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 和 `metrics_file_tests`，快速验证 CI 配置、命令路径、运行时 smoke 和 metrics 头文件相关目标没有被牵连。第五层是全量 CTest，334 条全部通过。

sanitizer 本机验证的结果也被记录下来，但它不是“失败后忽略”。先运行 `-DMINIKV_SANITIZE=ON` 时，配置通过、构建到链接失败，暴露缺少 `-lasan`/`-lubsan`。随后本版把这个事实前移到配置期，再重新运行 sanitizer 配置，得到明确的 `FATAL_ERROR`。这个流程证明我们没有凭空写一个平台限制，而是用真实命令找到了失败边界，再把边界变成可维护反馈。

这种验证结构符合 K1 的阶段。默认构建和全量 CTest 证明本版不会破坏已有功能；sanitizer configure failure 证明本机限制被明确表达；Ubuntu CI 诊断 job 则会在 push 后给出真正的 Linux ASan/UBSan 信号。三者不是互相替代，而是各自回答不同问题：默认路径是否稳、本机工具链限制是否清楚、CI sanitizer 是否能发现真实未定义行为。

## 与后续 K2-K6 的关系

K1 是生产卓越链路里很关键的一步，因为它把“代码可能有内存风险”的判断从人工审查推进到工具验证。mini-kv 是 C++ 项目，WAL、snapshot、RESP、TCP 和命令解析都属于 sanitizer 能带来价值的区域。即使当前全量 CTest 都绿，也不代表没有越界读写、未定义行为、生命周期误用或测试里偶发未覆盖的路径。ASan/UBSan 的价值在于，当这些路径被测试触发时，它们能比普通 Debug 构建更早、更具体地指出问题。

K2 coverage 会接着解决另一个问题：哪些路径没有被测试触发。K1 与 K2 是一对，sanitizer 提升“被触发路径”的质量，coverage 显示“未触发路径”的盲区。等 K2 建好覆盖率基线后，就到了第一个适合让 Claude review 的检查点：让它看覆盖率门是否合理、是否误伤、是否遗漏关键目录。之后 K4 文件拆分也是重要 review 点，因为会动到 `command.cpp` 和 `shard_readiness.cpp` 这类高风险大文件。v1610 还没到需要 Claude review 的位置，它只是把 K1 的诊断轨道接上。

K3 logging、K4 file split、K5 archive retention 和 K6 docs/release discipline 都会受益于 v1610。后续拆大文件时，sanitizer job 会在 CI 中多跑一层内存和未定义行为检查；后续压缩 archive 或改 release discipline 时，默认构建与 sanitizer 构建都能检查是否有测试辅助路径出现隐藏问题。更重要的是，v1610 保持了“先诊断，再强制”的节奏，不会把生产卓越做成一次容易失败的大爆炸。

## 风险和回滚

本版最主要的风险是 CI 时间增加。Ubuntu sanitizer job 会重新配置、构建并运行完整 CTest，可能明显拉长 GitHub Actions 总耗时。因为它现在是单独 job 且 `continue-on-error`，即使耗时或失败也不会阻断普通 matrix 的结果；但维护者需要观察实际耗时。如果 sanitizer 运行时间过长，后续可以先缩小为关键测试集合，再逐步扩大；如果 sanitizer 发现真实问题，应优先修问题，而不是删除 job。

第二个风险是平台误解。Windows MinGW 下 sanitizer 现在会 configure-time fail，这是故意的；它只在 `MINIKV_SANITIZE=ON` 时发生。普通 Windows build 仍然通过。如果有人希望本机也跑 sanitizer，需要先换成带 ASan/UBSan runtime 的工具链，或者新增一个专门支持 Windows Clang/MSVC sanitizer 的版本。不能把当前 MinGW 限制误解成项目默认构建失败。

回滚方式很清楚：删除 `MINIKV_SANITIZE` 选项和 sanitizer CI job，移除 `dependabot_config_tests` 中相关断言，并回滚 START_HERE、README、progress 和归档说明即可。因为本版没有改变运行时代码、命令表、fixture、WAL 或 snapshot，所以回滚不会牵涉数据兼容。这个可逆性说明 v1610 是一个干净的基础设施版本，不是混杂功能版本。

一句话总结：v1610 把 mini-kv 的 K1 sanitizer 从计划变成可运行的 Ubuntu 诊断 CI 轨道，同时把本机 Windows MinGW runtime 限制前移成清晰配置错误，并保持所有运行时命令、只读证据、写入边界、WAL 边界和跨项目执行边界不变。
