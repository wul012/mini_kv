# v1609 production-excellence K0 快速胜利

## 目标、角色和不是什么

v1609 的目标是执行 Claude 在 Node 侧新写的 `production-excellence-minikv-playbook.md` 中第一个未完成里程碑 K0。这个里程碑不是让 mini-kv 继续发新的运行时 evidence 命令，也不是继续把 Node/Java 的计划回显成 C++ JSON，而是把项目本身的生产化基础设施先擦亮一层：入口文档不能继续停在 v106，版本头里的归档提示不能继续写死在模板里，唯一没有使用 `#pragma once` 的头文件需要和其余头文件统一，CI 需要有一个只检查变更文件的 clang-format 门，mini-kv 自己也要有本地进度账本，避免去 Node 仓库里写状态。

本版的角色是“生产卓越计划的本地落点”。它承接 Node 计划中的全局标准，但所有写入都发生在 `D:\C\mini-kv`。Node playbook 对 mini-kv 来说只是只读输入，不能因为 playbook 文件在 Node 仓库里，就让 mini-kv 反向去改 Node 的 progress table。这个边界本身就是 K0 的一部分：跨项目可以并行，但每个会话只写自己的仓库。v1609 因此新增 `docs/production-excellence-progress.md`，把 K0 到 K6 的状态表复制到 mini-kv 本地，并在 deviation 中记录为什么 `c/102/` 被保留为默认输出。

本版明确不是什么也很重要。它不是 evidence schema 变更，不新增命令，不改变 `INFOJSON`、`SMOKEJSON`、`SHARDJSON` 的字段结构，不移动 `a/` 到 `f/` 的历史归档，不改 `fixtures/release/*.json`，不打开 router，不写 store，不触 WAL，不执行 `LOAD` / `COMPACT` / restore，也不启动 Node、Java 或 mini-kv 服务。它只做工程质量的基础改造：把可维护性问题从“以后记得”变成文件、变量、CI 步骤和进度表。

## 为什么 K0 先做这些

production-excellence 计划把 mini-kv 的问题分成几类：CI 已经不错，但缺 sanitizer、coverage、logging、文件拆分、归档预算和 release discipline。K0 被放在最前面，是因为它的风险最低，却能减少后续里程碑的摩擦。先修入口文档，可以让下一个 Codex 或人类维护者进仓库后不再被 v106 的陈旧说明误导。先把 archive hint 从 `version.hpp.in` 挪到 CMake，可以让后续版本讨论“当前 runtime fixture hint 是否应变更”时有一个明确配置点，而不是每次直接改生成模板。先统一 `metrics_file.hpp` 的 include guard，可以把头文件风格的小刺拔掉。先接入 changed-file clang-format，可以让 K1/K2/K3/K4 的后续改动逐步进入格式约束，而不是等到几百个文件一起被格式化。

这里最容易犯的错，是把 K0 理解成一次“顺手更新当前版本号”。mini-kv 的 CMake project version 仍然是 `0.102.0`，这不是本版忘了改，而是历史 runtime receipt 里已经把 `0.102.0` 和 `c/102/` 当作一段 frozen evidence 来消费。`INFOJSON` 里的 `binary_provenance.artifact_path_hint`、`retention_provenance_check.artifact_path_hint`、managed-audit adapter/sandbox 相关 receipt，以及大量测试，都把这个值当作历史锚点。Node 侧还有大量绝对路径和 digest 引用，证明不能把历史路径当普通文本随便改。K0 的正确动作是把硬编码从头文件模板挪到 CMake 变量，同时保持默认输出不变。这样后续如果真要改 hint，必须显式改 `MINIKV_CURRENT_ARCHIVE_VERSION`，并承担测试和下游契约审查，而不是无意中改掉。

## CMake 配置点和 version.hpp.in

本版在 `CMakeLists.txt` 顶部新增：

```cmake
set(MINIKV_CURRENT_ARCHIVE_VERSION
    "c/102/"
    CACHE STRING "Archive path hint for the frozen current runtime fixture exposed in generated version.hpp"
)
```

这个变量通过原有的 `configure_file` 流程写入 `include/minikv/version.hpp.in`，生成到 build 目录里的 `generated/include/minikv/version.hpp`。模板里原来是：

```cpp
inline constexpr std::string_view current_version_path_hint = "c/102/";
```

现在变成：

```cpp
inline constexpr std::string_view current_version_path_hint = "@MINIKV_CURRENT_ARCHIVE_VERSION@";
```

这是一处很小的改动，但维护意义很明确：`version.hpp.in` 不再承担“记住当前归档路径”的职责，它只负责声明 C++ 常量的形状；CMake 才是配置来源。配置后的生成头仍然输出 `version = "0.102.0"` 和 `current_version_path_hint = "c/102/"`，所以运行时 JSON 不变。focused CTest 中的 `smokejson_command_receipt_tests`、`managed_audit_receipts_tests`、`runtime_smoke_evidence_tests`、`current_runtime_fixture_rolling_guard_tests` 和 `artifact_digest_compatibility_matrix_tests` 都在保护这个判断：如果这个变量不小心改变了输出，它们会直接失败。

## START_HERE 和本地进度账本

`START_HERE.md` 以前仍写着“Current focus: v106 table-drive command dispatch”，还说 README 解释 v106 dispatch table。这个信息在 v1609 已经明显不对。入口文件的作用不是完整替代 README，而是让维护者用十几秒知道项目当前状态。v1609 把它改成当前 production-excellence K0：刷新入口、CMake 配置 archive hint、统一头文件风格、加入 changed-file clang-format，并说明 CMake `0.102.0` 与 git tag `v1609` 是双轨版本。这个说明能防止后续维护者看到 `0.102.0` 就以为构建版本忘记更新。

新增的 `docs/production-excellence-progress.md` 则承担进度账本角色。它复制了 K0 到 K6 的进度表，但只放在 mini-kv 自己的仓库里。这样 Java、Node、mini-kv 三个 Codex session 可以并行执行，而不会抢写同一个 Node repo 文件。账本里的 deviation 记录了本版最关键的现实偏差：`MINIKV_CURRENT_ARCHIVE_VERSION` 默认仍是 `c/102/`，因为它是 frozen current-runtime fixture hint，不等于最新 tag。这个 deviation 不是借口，而是证据：我们看到了 playbook 的“修 stale hint”要求，也看到了现有测试和下游路径引用的约束，于是选择了“配置化但不改输出”的保守实现。

## metrics_file.hpp 的头文件风格统一

`include/minikv/metrics_file.hpp` 是 playbook 指出的唯一 include guard outlier。项目里其余公开头文件基本都使用 `#pragma once`，只有它还保留：

```cpp
#ifndef MINIKV_METRICS_FILE_HPP
#define MINIKV_METRICS_FILE_HPP
...
#endif
```

v1609 把它改成 `#pragma once`。这个改动不改变 `MetricsFileOptions`、`MetricsFileWriter` 的 ABI 语义，不改成员函数，不改 include，不改 `src/metrics_file.cpp`。它只是把头文件保护方式和项目内其他头文件对齐。`metrics_file_tests` 通过说明行为仍然正常；全量 CTest 通过说明这个头文件被 server metrics 导出、测试目标和其他 include 路径间接使用时没有出现重复定义或 include 顺序问题。

这里不需要把所有头文件重新格式化，也不需要跑一次 repo-wide clang-format。production-excellence 总纲明确禁止机械性全仓格式化扫荡，因为那会制造巨大 diff，并且把真正的语义改动埋在格式噪声里。K0 只改一个 outlier，正是为了用最小 diff 先建立规则。

## clang-format changed-file CI

本版新增 `.clang-format`，以 LLVM 为基底，`ColumnLimit: 120`，`IndentWidth: 4`，不排序 include。选择 120 列不是为了追求极长行，而是贴近 mini-kv 当前代码风格，避免一加 CI 就要求把大量历史长字符串 JSON 和 receipt 表达式重排。`SortIncludes: Never` 同样是保守选择：这个项目里不少源码文件的 include 顺序已经隐含了平台宏、生成头、测试辅助头的先后关系；K0 的目的不是重排历史，而是让后续 touched files 有一个温和边界。

CI 里新增 `clang-format changed files` job，只跑 Ubuntu。它 checkout 时使用 `fetch-depth: 0`，这样 push 和 pull_request 都能找到 base revision。步骤里如果 runner 没有 `clang-format`，就用 apt 安装；然后调用 `scripts/check_clang_format_changed_files.py`。这个脚本只看 `git diff --name-only --diff-filter=ACMRT base head` 中的 C/C++ 后缀文件，然后执行：

```text
clang-format --dry-run --Werror ...
```

这条设计有两个边界。第一，它只检查 changed files，避免 K0 变成全仓格式化；第二，它失败时是真失败，不是 warning，因为格式门如果不能在小范围内执行，就没有资格进入后续 K1/K2/K3/K4。脚本本身只使用 Python stdlib，不给 mini-kv 引入 npm、Maven、pip、cargo 或 Docker 生态。它也没有在本地写缓存文件，不会污染归档。

本机 PATH 里没有 `clang-format`，但 CLion 自带 `D:\CLION\CLion 2025.1.1\bin\clang\win\x64\bin\clang-format.exe`。本地用这个可执行文件对 `include/minikv/metrics_file.hpp` 做了 dry-run，验证新增风格文件对当前 touched C++ header 可用。CI 会在 Ubuntu 上安装或使用系统 clang-format，再对 push 中真正变更的 C++ 文件做同样检查。

这条 CI 门还有一个细节：脚本要知道“什么叫 changed”。pull request 场景下它用 `origin/${{ github.base_ref }}` 到 `HEAD`；push 场景下它用 `github.event.before` 到 `HEAD`。如果遇到初始提交或 base 不可解析，脚本会退回到仓库第一个 parent 或 `HEAD~1`。这个回退不是为了放松检查，而是为了避免 CI 因为 GitHub 事件上下文特殊而误报。真正被检查的文件仍然来自 `git diff --name-only --diff-filter=ACMRT`，只包含新增、复制、修改、重命名、类型变化的文件；删除文件不会送进 clang-format。这样 K0 的格式门是可重复的：本地可以手工传 CLion 的 clang-format，CI 可以用系统 clang-format，输入集合都来自 Git 的变更集。

changed-file 策略也给后续重构留下了正确压力。比如 K4 拆 `command.cpp` 时，新建的 command-family 文件会被 clang-format 检查；但历史上没有碰的 50KB 文件不会在同一批被重新排版。这样审查 diff 时，维护者能专注看拆分本身，而不是在几千行格式噪声里找行为变化。反过来，如果后续某版大量修改 `shard_readiness.cpp`，它就会进入 changed-file 集合，被格式门检查。这就是渐进式 ratchet：不是一次性洗仓库，而是谁改谁负责。

## 风险判断和回滚方式

K0 的风险看起来小，但实际有两个容易踩的点。第一个是把 `MINIKV_CURRENT_ARCHIVE_VERSION` 改成最新版本目录。如果这么做，`INFOJSON`、`SMOKEJSON`、managed audit receipt、runtime smoke evidence、release verification manifest 里很多断言都会变化；更糟的是，Node 可能已经把旧路径和 digest 写进自己的证据里。v1609 没有这样做，而是把变量默认值保留为 `c/102/`，并在 progress deviation 里说明它是 frozen runtime fixture hint。以后如果确实要迁移这条 hint，应当单独开契约版本，先检查 Node/Java 引用，再同步 fixture 和测试，而不是在快速胜利里顺手改。

第二个风险是把 clang-format 误用成全仓清扫。mini-kv 现在有大量长字符串 JSON、历史 receipt、归档说明和测试断言，仓库整体还没有准备好一次性格式化。如果 K0 直接跑全仓格式化，diff 会非常大，而且可能掩盖真正的 CMake/CI 改动。v1609 只新增配置和 changed-file 检查，相当于从今天开始给新改动上护栏，历史债留给后续专门批次处理。这个策略更符合生产化工程：先让规则可执行，再让规则逐步覆盖。

回滚方式也很清楚。如果 CI 的 clang-format job 在某个平台上失败，优先看失败文件列表和 clang-format 版本；如果是 `.clang-format` 规则太激进，应调窄规则，不应删除 job。如果 CMake 生成头出现错误，只需要回滚 `MINIKV_CURRENT_ARCHIVE_VERSION` 变量和 `version.hpp.in` 模板替换；这不会影响 store/WAL/snapshot，因为改动不进入运行时状态机。如果 START_HERE 文案有问题，可以独立修文档，不需要碰 C++。这种可逆性是 K0 能放在最前面的原因。

## 入口文档和 release discipline 的关系

K0 没有直接解决 README 597KB、CHANGELOG 未拆分、CMake version 与 git tag 不一致这些大问题；那些属于 K6。但 START_HERE 的修正已经把误导性最大的入口先修掉了。它现在明确写出：CMake `0.102.0` 是历史 runtime fixture 相关版本，git tag `v1609` 是交付节奏；`a/` 到 `f/` 是冻结归档根，不可移动；生产卓越进度写在 `docs/production-excellence-progress.md`。这三句话能让后续维护者避免三个错误判断：以为当前项目还停在 v106、以为 `0.102.0` 必须马上改成 `1.609.0`、以为归档目录可以为了清理体积直接挪走。

这个入口更新也给 K6 留下接口。等后续真的拆 README/CHANGELOG 时，START_HERE 不需要再承载巨大历史，只需要指向新的 CHANGELOG、SECURITY、TESTING 和 progress 文件。也就是说，K0 没有提前做 K6 的工作，但先把导航形状调对了。好的维护往往不是一次性把所有问题都解决，而是先让下一个改动更容易下手。

## 测试和 smoke 覆盖

本版的测试不是只跑 `git diff --check`。因为 CMake、生成头、公开头文件和 CI 都被修改，必须重新配置和构建。配置命令使用 CLion 捆绑 CMake 和 MinGW，生成 `cmake-build-v1609`。配置阶段通过，只出现已有的长路径 warning。构建阶段完整通过，说明 CMake 新变量没有破坏生成头 include 路径，也没有影响库和 334 个测试目标的编译链接。

focused CTest 跑了 7 个目标：`dependabot_config_tests`、`smokejson_command_receipt_tests`、`managed_audit_receipts_tests`、`metrics_file_tests`、`artifact_digest_compatibility_matrix_tests`、`runtime_smoke_evidence_tests`、`current_runtime_fixture_rolling_guard_tests`。这些测试各守一块边界。`dependabot_config_tests` 间接确认 CI 文件仍可被已有质量守卫接受。`metrics_file_tests` 保护头文件风格改动没有影响 metrics writer。`smokejson`、`managed_audit`、`runtime_smoke` 和 rolling guard 保护 `c/102/` 这条 frozen runtime fixture 语义仍然存在。`artifact_digest_compatibility_matrix_tests` 保护 project version 仍是 `0.102.0`，没有把 git tag 和 CMake version 混成一套。

全量 CTest 334/334 通过，是本版真正的安全网。它说明 `INFOJSON`、`SMOKEJSON`、`SHARDJSON`、命令目录、fixture、WAL、snapshot、RESP、TCP、恢复证据和最近几百个治理 receipt 都没有被 K0 破坏。CLI smoke 使用 UTF-8 无 BOM 输入文件重定向运行 `INFOJSON`，输出中仍能看到 `version:"0.102.0"`、`artifact_path_hint:"c/102/"`、`read_only:true`、`execution_allowed:false`、`order_authoritative:false` 等字段。这证明本版虽然改了构建配置和入口文档，但二进制可见的证据边界没有漂移。

## 对后续 K1-K6 的价值

K0 做完后，后续 K1 sanitizer、K2 coverage、K3 logging、K4 file splits、K5 archive retention、K6 docs/release discipline 都有了更好的起点。sanitizer 和 coverage 会改 CMake；现在 CMake 顶部已经有一个清晰的配置变量示范。logging 会碰多个 C++ 文件；现在 changed-file clang-format 会在增量范围内给风格兜底。K4 会拆 `command.cpp`、`shard_readiness.cpp` 和可能的 boundary table；现在 `START_HERE` 和 progress ledger 都明确了先保契约、再拆结构。K5 会处理 1.1GB 的 `e/` archive；现在 progress 和 governance 文档都已经把“冻结路径，不移动历史”写清楚。

最关键的是节奏。这个版本没有为了“多版本推进”去拆成四五个薄版本：入口文档、CMake 配置、头文件风格、CI 格式门、本地进度账本、README 摘要、讲解和归档都放在一个 K0 版本里闭环。它也没有为了做厚而硬加运行时功能；K0 的主题仍然只有一个：生产卓越计划的快速胜利。这样的版本粒度更适合后续维护：每一版都有足够解释价值，但不会把无关功能混在一起。

一句话总结：v1609 把 mini-kv 的 production-excellence K0 从外部 playbook 落成本仓库可维护的配置、文档、CI 和验证闭环，同时保持历史 runtime fixture、归档路径、命令输出和执行边界全部不变。
