# v1615 post-K2 actions runtime readiness 代码讲解

## 本版目标、角色和不是什么

v1615 的目标是收掉 K2 review 给出的时间敏感 CI 风险：GitHub Actions 日志提示 `actions/checkout@v4` 和 `actions/upload-artifact@v4` 仍运行在即将废弃的 Node 20 action runtime 上，而 GitHub 会在 2026 年 6 月 16 日开始强制转向 Node 24。当前日期是 2026 年 6 月 12 日，这个提醒距离强制切换只有四天。如果继续直接进入 K3 logging，理论上 C++ 代码可以写完，但 CI 基础设施可能在几天后因为 action runtime 兼容性变成噪声甚至阻塞。v1615 因此先做一版很窄的生产化维护：把 checkout action 升到官方最新 release `v6.0.3`，把 coverage artifact 上传 action 升到官方最新 release `v7.0.1`，并用 CTest 合同测试守住新 tag 和旧 tag 禁止回归。

本版的角色是“review advisory closure”，不是 K3 logger 的实现版本。它不新增 `include/minikv/log.hpp`，不引入日志级别，不改 `minikv_server` 或 `minikv_cli` 参数，不替换 `std::cerr` 调用，也不改变任何 stdout evidence。它更不是运行时功能版本：KV 命令、WAL、snapshot、RESP/TCP、fixture schema、coverage floor、sanitizer gate、历史 archive 根目录都保持不变。它只是把 CI 使用的 GitHub 官方 action 版本从已触发 Node 20 deprecation annotation 的旧 tag 移到已发布的新 tag，让 K3 可以在干净的 CI 基础上推进。

这版为什么不能塞进 K3？原因是版本边界。K3 本身会动 C++ 运行时日志、stderr 输出、CLI/server 参数解析和测试断言，风险点已经足够集中；如果同时混入 action major tag 升级，远端 CI 一旦失败，排查范围会被拉大：到底是 logger 改错了，还是 checkout/upload-artifact 行为变化，还是 coverage artifact 上传接口变化？v1615 把 CI runtime readiness 单独闭环，正是为了给 K3 降噪。等 v1615 远端全绿后，K3 的 CI 失败就更可能是 logger 自己的问题，而不是基础 action runtime 的变量。

## 入口文件和模块职责

本版的第一入口是 `.github/workflows/ci.yml`。这个 workflow 有四类 checkout 使用场景：changed-file clang-format job 需要 `fetch-depth: 0` 才能比较 base/head；三平台 build-and-test matrix 需要正常拉取源码；Ubuntu sanitizer job 需要拉取源码后配置 `MINIKV_SANITIZE=ON`；Ubuntu coverage job 需要拉取源码后配置 `MINIKV_COVERAGE=ON`、跑 full CTest、生成并上传 coverage report。v1615 把这些 checkout step 全部从 `actions/checkout@v4` 改为 `actions/checkout@v6.0.3`，保持 step 名称、fetch-depth、build/test 命令不变。

第二入口是 coverage job 的 artifact 上传 step。v1614 刚刚把 `mini-kv-core-coverage` artifact 变成 K2 的核心证据链，里面包含 `coverage/core-coverage.txt` 和 HTML details。这个上传动作以前使用 `actions/upload-artifact@v4`，在 v1614 远端 CI annotation 里同样被标记为 Node 20 runtime。v1615 把它改为 `actions/upload-artifact@v7.0.1`，但保留 artifact name 和 `path: coverage/`。这意味着下游 review、archive、manual audit 都仍然找同一个 artifact 名称，只是底层 action runtime 不再停留在旧大版本。

第三入口是 `tests/dependabot_config_tests.cpp`。这个测试原本已经守住 Dependabot 只覆盖 GitHub Actions、禁止 npm/Maven/pip/cargo/docker 等生态误入，也守住 CI workflow 中的 build/test/sanitizer/coverage 关键命令。v1615 把它扩展为 action runtime 合同测试：它现在断言 workflow 必须包含 `uses: actions/checkout@v6.0.3` 和 `uses: actions/upload-artifact@v7.0.1`，同时断言不再包含 `uses: actions/checkout@v4` 与 `uses: actions/upload-artifact@v4`。这比只改 YAML 更稳，因为后续维护者如果回退 tag，本地 focused CTest 会失败。

## 证据结构怎么解释

v1615 的证据不是 JSON runtime evidence，而是 CI configuration evidence。它分三层。第一层是官方 release 查询：本轮用 `gh api repos/actions/checkout/releases/latest` 查到 `tag_name=v6.0.3`，用 `gh api repos/actions/upload-artifact/releases/latest` 查到 `tag_name=v7.0.1`。这两个查询来自 GitHub API，不是凭记忆猜测 action 大版本。第二层是仓库内配置：`.github/workflows/ci.yml` 实际引用这两个 tag。第三层是 CTest 合同：`dependabot_config_tests` 把新 tag 和旧 tag 禁止回归写成断言。

从控制面视角看，v1615 对关键边界字段的含义是“不扩大权限”。如果把 workflow 也看成一种 evidence pipeline，那么 `read_only` 的等价含义是：checkout 只取仓库内容，artifact upload 只上传 coverage 目录，不读取 credential value，不连接 Node 或 Java，不解析 raw endpoint，不执行 managed audit connection。`execution_allowed` 的等价含义也没有变化：CI 仍只执行配置、构建、CTest、coverage report 和 artifact upload，不执行 mini-kv restore/load/compact，不启动长期服务，不写业务状态。`order_authoritative`、`mutates_store`、`touches_wal` 对本版都保持 false 的语义，因为 action tag 更新不会进入 mini-kv 运行时命令路径。

这也解释了为什么本版不改 coverage floor。K2 review 已经确认 90% baseline 和 88% floor 正确，v1615 不应该借着 CI 维护去重新讨论覆盖率数字、filter 范围或 negative-hit parser workaround。那些属于 K2 或未来 K4 的议题：例如 K4 拆 `command.cpp` 后可以重新评估 `--gcov-ignore-parse-errors negative_hits.warn_once_per_file` 是否还能删除。v1615 只做 action runtime readiness，不顺手重写其他质量门槛。

还有一个细节：本版选择固定到完整 release tag，而不是写成浮动的大版本标签。浮动标签的优点是未来小版本自动跟进，缺点是证据归档不够可复现；完整 tag 的优点是每次 CI 证据都能精确说明用了哪个 action 实现。mini-kv 现在强调的是生产化证据链和可审计性，所以 v1615 选择 `v6.0.3` / `v7.0.1`。如果未来 Dependabot 或人工维护要继续升级 action，小版本变化也应该形成新的可审查 diff，而不是让同一个历史 commit 的远端行为随上游浮动。

这次也没有修改 `.github/dependabot.yml` 的 semver-major 策略。v110 以来 mini-kv 明确只让 Dependabot 自动维护 GitHub Actions 的 minor/patch 类更新，并忽略 semver-major 自动升级。v1615 是手动、有证据的 major 升级，原因是平台 runtime 强制迁移已经进入倒计时。这样做并不推翻原策略，反而说明原策略是有边界的：日常不要自动吃 major；遇到平台强制切换、review 明确提醒、并且已经用官方 release 查询确认时，可以单独开版本手动升级，再用 CI 和 CTest 合同守住结果。

## 测试保护的行为

focused 测试仍然是 `dependabot_config_tests`。这次它保护四件事：第一，checkout action 必须是 `v6.0.3`；第二，upload-artifact action 必须是 `v7.0.1`；第三，旧 checkout `v4` 不能回归；第四，旧 upload-artifact `v4` 不能回归。测试里继续保留 sanitizer、coverage、`--fail-under-line 88`、coverage filter、artifact name 等既有断言，所以 v1615 不会因为修 action runtime advisory 而削弱 K1/K2 的门槛。

完整 CTest 仍然要跑。虽然本版没有改 Store、Command、WAL 或 Snapshot 的实现，但 `tests/dependabot_config_tests.cpp` 是 C++ 测试源，mini-kv closure discipline 要求改测试源后重建再跑测试。完整 CTest 的意义是确认新的断言不会破坏测试二进制，也确认默认 Debug 构建和历史 334 个测试仍然稳定。远端 CI 则承担最终证据：如果新 action tag 的 checkout、artifact upload、coverage artifact path 或跨平台行为有变化，GitHub Actions 会在对应 job 暴露问题。

特别要注意，v1615 的远端 CI 成功应当同时观察 annotation 是否消失。v1614 的 run 虽然全绿，但有 Node 20 action runtime deprecation annotation；v1615 升级后，理想状态是这些 annotation 不再出现。如果 GitHub 仍提示某个 action 运行在 Node 20，那就说明还有遗漏 action tag 或上游 action 自身未迁移，需要在同一类 CI maintenance 版本里继续处理。这个观察点比“build/test 成功”更细，因为本版的目标本来就是清掉 runtime advisory。

从失败处理角度看，v1615 的回退策略也很清楚。如果 `checkout@v6.0.3` 在某个平台上出现拉取源码、fetch-depth 或 submodule 行为差异，失败应该集中在 checkout step、changed-file clang-format 的 base/head 解析、或后续构建找不到文件；如果 `upload-artifact@v7.0.1` 有行为差异，失败应该集中在 `ubuntu coverage` job 的 artifact upload step，或者 review 时找不到 `mini-kv-core-coverage`。这两个风险都不会表现为 KV 命令语义变化，因此不应该通过改 C++ 运行时代码来“修”。正确做法是阅读 action release notes、调整 workflow 参数，或者必要时选择一个仍支持 Node 24 的更合适 tag。

这个回退策略也解释了为什么本版仍然要保留 artifact 名称不变。`mini-kv-core-coverage` 已经被 K2 review 使用，后续 Claude 或人工维护者会按这个名字找 coverage report。如果 action 升级同时改 artifact name，就会把“runtime readiness”问题和“证据路径迁移”问题混在一起。v1615 的设计是只换 transport，不换证据坐标：checkout 还是 checkout，coverage artifact 还是同名 artifact，build/test/coverage 命令不变，只有底层 action implementation 从旧 runtime 迁到新 release。

## 后续维护价值

v1615 给 K3 的价值是把地面先擦干。K3 要实现零依赖 leveled logger，涉及 stderr 格式、时间戳、thread id、`--log-level`、server/client 参数、日志过滤测试，以及证明 stdout evidence 不变。这样的版本如果遇到 CI 注解和 action runtime 强制迁移，很容易让维护者把注意力分散到工具链问题上。v1615 提前把 action runtime 更新为官方最新 release，让 K3 可以专注处理 C++ 可观测性，而不是同时承担 GitHub Actions 生命周期迁移。

它给长期维护的价值也很直接：CI action tag 本身也是生产化依赖。mini-kv 一直强调零外部运行时依赖，这不代表 CI 可以忽视上游 action 生命周期。`actions/checkout` 决定源码如何进入 runner，`actions/upload-artifact` 决定 K2 coverage evidence 如何被保存。如果这两个动作处于即将被平台强制迁移的旧 runtime 上，即使项目 C++ 代码没有 bug，证据链也可能变得不稳定。v1615 把它们纳入测试合同，是在把“CI 依赖可维护性”也变成项目工程纪律的一部分。

长期看，这版还给后续 release discipline 一个样板：外部托管工具不是越新越好，也不是永远不动最好。正确节奏是先由 CI、review 或平台公告暴露风险，再用官方来源确认可升级目标，然后单独成版、保留原行为坐标、加本地合同测试、跑完整本地和远端验证，最后归档截图和讲解。这个节奏和 mini-kv 的 C++ 运行时边界是一致的：少做、做准、留下证据、不要顺手打开无关权限。

## 跨项目边界和验收清单

Claude review 同时提醒 Java 和 Node workflow 也值得做同类检查，但 v1615 只处理 mini-kv。原因不是忽略上游，而是遵守本仓库 AGENTS 的 C++ ownership rule：mini-kv session 可以读取 Node plans、读取 review 结论、理解跨项目风险，但不能顺手修改 `D:\nodeproj` 或 `D:\javaproj`。如果 Java/Node 也需要升级 action tag，应由各自仓库的 session 单独读取自己的 playbook、跑自己的 CI、提交自己的版本。mini-kv 在这里做的只是把本仓库可控的 CI runtime 风险关闭。

本版验收清单因此很小但很硬。第一，workflow 中不能再出现 `actions/checkout@v4` 或 `actions/upload-artifact@v4`。第二，所有 checkout step 都必须统一为 `actions/checkout@v6.0.3`，不要只改 coverage job 而漏掉 clang-format、matrix build 或 sanitizer。第三，coverage artifact 上传必须仍叫 `mini-kv-core-coverage`，因为 K2 review 已经围绕这个名称验证证据。第四，本地 `dependabot_config_tests` 必须通过，证明这些约束不是口头规则。第五，远端 CI 必须全绿，并观察 Node 20 deprecation annotation 是否消失。只有这些条件都满足，v1615 才算真正给 K3 清掉了 CI runtime 噪声。

这也让后续维护者更容易判断责任边界：如果 K3 logger 版本之后出现日志格式或 stdout/stderr 混杂问题，那是 K3 的实现问题；如果出现 coverage floor 低于 88%，那是 K2 门槛触发；如果出现 action runtime annotation，优先检查是否有新 action 或旧 tag 被引入。v1615 把这些边界提前写清楚，是为了后面的版本少一点猜谜，多一点可定位的工程信号。

归档层面，本版截图不需要展示运行时命令输出，而应该展示四类证据：官方 release 查询结果、workflow 中新旧 action tag 的扫描结果、focused 合同测试通过、完整 CTest 通过。远端 CI 归档还要补充最终 run 的结论，尤其是 `clang-format changed files`、三平台 build-and-test、Ubuntu sanitizer、Ubuntu coverage 都通过，并确认不再出现 Node 20 action runtime deprecation annotation。如果远端仍有 annotation，v1615 就不能算真正收口，因为本版目标不是“能跑”，而是“把即将强制迁移的 runtime 风险提前消掉”。这种验收标准也会保护后续维护者：看到 v1615，就知道 K3 之前的 CI runtime 基线已经重新确认，不必在 logger 版本里再次解释同一个平台提醒。

一句话总结：v1615 不推进 mini-kv 运行时功能，但它把 K2 review 后暴露出的 CI runtime 风险先收口，让后续 K3 zero-dep logger 可以建立在干净、可审计、不会被 Node 20 deprecation 噪声干扰的 GitHub Actions 基础上。边界清楚，证据闭环。
