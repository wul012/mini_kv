# v1613 production-excellence K2 coverage report hardening

## 目标、角色和不是什么

v1613 的目标是修复 v1612 推送后 GitHub Actions 暴露出来的 coverage report 生成失败。v1612 已经证明三件关键事实：Ubuntu coverage build 能配置，coverage target set 能构建，coverage build 下的 334 条 CTest 能全部通过。失败发生在最后一步 `gcovr` 读取 gcov 输出并生成 report 时，原因是 GCC/gcov 在 `src/command.cpp` 的分支覆盖数据里输出了 negative branch hit，例如 `branch 7 taken -2 (fallthrough)`，`gcovr 7.0` 默认把它当成 parse error 并以 exit code 64 退出。

这版的角色是 K2 coverage baseline 的 report hardening。它不是新运行时功能，不新增命令，不新增 evidence 字段，不改变 store、WAL、snapshot、RESP、TCP，也不改变任何 fixture 的业务语义。它也不是覆盖率 floor 版本，因为 v1612/v1613 的任务仍然是让 baseline report 真实产出；只有 report 产出后，下一版才能按 measured baseline minus 2 points 设置 `--fail-under-line`。v1613 只处理一个现实矛盾：coverage 运行和测试都过了，但工具在已知 gcov negative-hit bug 上过早失败。

这个“不是什么”要写清楚，因为 `--gcov-ignore-parse-errors` 这个名字看起来像“忽略错误”。本版不是忽略测试失败，也不是允许 CMake、build、CTest 或 artifact 上传失败。它只把 GCC/gcov 已知的 negative hit 解析异常降级成 warning，而且使用的是 `gcovr` 日志里明确建议的最窄值：`negative_hits.warn_once_per_file`。换句话说，coverage job 仍然是 required，full CTest 仍然必须绿，report 仍然必须生成，artifact 仍然必须上传；只是 `gcovr` 不再因为一个工具链级别的负分支计数中断整个报告。

## 远端失败说明了什么

v1612 的远端 run `27407287210` 很有价值。它不是一条“全失败”的 CI，而是一个精确反馈：普通三平台 matrix 通过，Ubuntu sanitizer required job 通过，clang-format changed files 通过，Windows / macOS / Ubuntu 默认构建测试通过，coverage job 也完成了安装、配置、构建和 334 CTest。失败点只在 `Generate core coverage report`。

失败日志的核心片段是：

```text
(WARNING) Unrecognized GCOV output for /home/runner/work/mini_kv/mini_kv/src/command.cpp
  branch  7 taken -2 (fallthrough)
(ERROR) Exiting because of parse errors.
You can run gcovr with --gcov-ignore-parse-errors to continue anyway.
NegativeHits: Got negative hit value ... caused by a bug in gcov tool ...
Use option --gcov-ignore-parse-errors with a value of negative_hits.warn, or negative_hits.warn_once_per_file.
```

这个日志给了两个判断依据。第一，触发点是 `src/command.cpp.gcda` 的 gcov 解析，不是 mini-kv 某个测试失败。第二，工具明确说这是 gcov tool 的 bug，并给出具体处理方式。v1613 因此不去改 C++ 源码，也不去删 coverage filter，更不把 coverage job 改成 `continue-on-error`。正确修复是把已知 negative-hit parse issue 转为 warning，让 report 生成继续完成。

如果 v1613 选择删除 coverage job，那会丢掉 K2 的核心价值。如果选择 `continue-on-error: true`，那会绕开 v1611 刚建立的“禁止软失败”质量门逻辑。如果选择大幅缩小 filter，可能让 baseline 失真。当前选择是最小且语义准确的：保留 job required，保留 full CTest，保留 core filter，只调整 gcovr 对已知 negative-hit 解析异常的处理策略。

## CI 改动：只加一个有边界的 gcovr 参数

v1613 的 CI 改动在 `.github/workflows/ci.yml` 的 coverage report 步骤。v1612 的命令是：

```yaml
gcovr --root . --object-directory build-coverage --filter "src/(store|command|command_catalog|command_contracts|command_response_formatters|command_smoke_formatters|wal|snapshot|resp)\\.cpp" --txt coverage/core-coverage.txt --html-details coverage/core-coverage.html
```

v1613 改成：

```yaml
gcovr --root . --object-directory build-coverage --filter "src/(store|command|command_catalog|command_contracts|command_response_formatters|command_smoke_formatters|wal|snapshot|resp)\\.cpp" --gcov-ignore-parse-errors negative_hits.warn_once_per_file --txt coverage/core-coverage.txt --html-details coverage/core-coverage.html
```

这个参数的粒度很重要。它不是裸写 `--gcov-ignore-parse-errors`，而是指定 `negative_hits.warn_once_per_file`。这样 CI 只对 negative hit 类 parse issue 放行，并且每个文件警告一次，避免日志被同一类问题刷屏。其他类型的 gcovr 读取问题仍然会暴露出来。这个选择符合 production-excellence 的精神：遇到现实工具链噪声时，不是把门禁拆掉，而是把例外收束到最窄的、可解释的范围。

本版也没有更换 `gcovr` 安装方式。远端 Ubuntu 使用 apt 安装的 `gcovr 7.0`，这是 GitHub runner 上稳定可复现的来源。虽然本机通过 pip 安装过 `gcovr 8.6` 用于环境确认，但仓库不依赖本机 pip 工具。v1613 修的是 CI 命令，确保 runner 上的实际版本能工作。后续如果要固定更高版本，可以另做版本说明，但当前最小修复是让 Ubuntu noble apt 的 gcovr 7.0 在已知 negative-hit bug 上继续产出 report。

## 测试改动：把 parser policy 写成契约

`tests/dependabot_config_tests.cpp` 在 v1612 已经保护了 coverage job 的存在、`gcovr` 安装、coverage configure/build/full CTest、core filter 和 artifact 名称。v1613 增加一条断言：

```cpp
assert_contains(ci_workflow, "--gcov-ignore-parse-errors negative_hits.warn_once_per_file");
```

这条断言的作用是防止后续有人“清理 YAML”时把这个看似奇怪的参数删掉。没有它，coverage job 会在同一个 gcov negative-hit 输出上再次失败，或者某个维护者为了赶时间把 job 改成软失败。把参数写进 CTest 后，CI parser policy 就不再只是 YAML 的临时修补，而是被仓库测试保护的 K2 deviation。

这也延续了 v1611 的门禁思路。v1611 用 `assert_not_contains(ci_workflow, "continue-on-error: true")` 防止 sanitizer/coverage job 被静默软化；v1613 用 `assert_contains` 防止 gcov negative-hit 处理策略丢失。一个是禁止放松 job 结果，一个是固定已知工具链例外。两者配合起来，让 coverage lane 既严格又能处理现实工具输出。

需要注意的是，这个测试仍然是字符串级契约，不是 YAML AST parser。当前项目已经有大量 CTest target，本版不为了一个 CI 参数引入 YAML parser 或额外依赖。字符串断言足够覆盖本次风险：关键命令行参数是否还在。以后如果 CI workflow 继续复杂化，可以再把配置读取拆成小 helper 或写更结构化的测试，但 v1613 不扩大范围。

## 为什么这不是降低覆盖率门槛

覆盖率门槛有两层：第一层是 job 能不能真实运行，第二层是 line coverage floor 是否达标。v1613 处理的是第一层的 report generation。它没有设置 floor，也没有降低 floor，因为 baseline 还没有成功产出。它也没有把某些低覆盖文件从 filter 中拿掉。core filter 仍然覆盖 store、command、command catalog、command contracts、command response/smoke formatters、WAL、snapshot 和 RESP。

更准确地说，v1613 是让 baseline 能被看见。没有这一步，我们只能从 gcov stdout 片段里看到 `command.cpp` 曾经报过 `Lines executed:93.30% of 687`，但无法得到完整 core report，也无法上传 HTML artifact，更无法为下一版设置机械 floor。一个稳定的 coverage gate 不应该靠人工从失败日志里抄数字，它应该靠 committed CI 命令生成 artifact，再由下一版把数字固化为 `--fail-under-line`。

这个修复也没有掩盖 C++ 代码问题。negative branch hit 是 gcov 输出和 gcovr parser 的交界问题，日志指向 GCC bug 68080；它不是 mini-kv 的 `command.cpp` 逻辑错误。真正的 C++ 行为仍然由 334 CTest、Ubuntu sanitizer required job、三平台默认 CI 和后续 coverage floor 共同保护。如果 coverage report 生成后显示某些核心文件低于预期，下一版应该记录 baseline 并设置合理 floor，之后再通过测试补强逐步提高，而不是在 v1613 把 report 生成问题和覆盖率质量问题混在一起。

还有一个维护判断：v1613 没有选择 `continue-on-error`，也没有选择把 `src/command.cpp` 从 filter 中临时排除。前者会把 K2 变成装饰性 job，后者会让核心命令路径从 baseline 中消失，两种做法都会让后续 Claude review 看到不真实的安全网。`command.cpp` 正是 K4 未来要拆分的重点文件之一，如果 coverage baseline 里没有它，那么后续拆 command family 时，coverage gate 就保护不到最需要保护的地方。现在保留 `command.cpp`，只处理 gcov negative-hit parser noise，才符合“先建立真实底线，再做拆分”的顺序。

同样，v1613 也没有把 coverage 统计范围扩成全仓库。全仓库数字看起来更完整，但 mini-kv 里有大量 receipt、evidence、archive、fixture 辅助模块，它们的测试策略更偏向字符串契约、fixture 对比和归档完整性。把这些模块和 store/command/WAL/snapshot/RESP 混在一个 line coverage floor 里，容易把 K2 从核心安全网变成治理文档覆盖率游戏。v1613 保持 v1612 的核心 filter，是为了让下一版的 baseline 真正服务于后续重构，而不是服务于表面百分比。

下一版读取 baseline 时，也应该保留这种诚实。需要看 `core-coverage.txt` 的 Lines 总百分比，而不是挑某个单文件最高值；如果 HTML artifact 显示某个核心文件覆盖率明显偏低，要在讲解里记录，但不要因此改分母。K2 的第一道 floor 是防退化，不是一次性完成覆盖率理想化。等 floor 绿了，再根据报告选择具体测试补强点，逐步提高 floor，这才是可维护的 ratchet。

## 与只读边界的关系

v1613 没有触碰任何运行时 evidence 字段，所以 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers`、`diagnostics` 的含义完全不变。mini-kv 仍然不是订单权威，不执行 Node 计划，不读取 Java 或 Node runtime 数据，不打开 managed audit connection，不读 credential value，不执行 restore/load/compact，不启动服务，不路由写入。

本版增强的是质量证据链，而不是功能权限。coverage report 让控制面和维护者看到核心模块测试触达范围，但它不改变任何控制面字段。即使下一版 floor 绿了，也只说明“当前测试触达率没有低于基线”，不代表 mini-kv 可以进入生产执行模式。所有 no router、no write、no WAL、no execution 边界仍由既有命令契约、fixture tests、runtime smoke evidence 和 AGENTS.md 规则约束。

这个边界在当前阶段尤其重要，因为 production-excellence program 是内部质量工作。它明确说不改 evidence schema、不改 route contract、不改 JSON 字段。v1613 完全遵守这一点：只改 CI report 参数和本地 CI 契约测试，外加文档记录。用户后续让 Claude review 时，Claude 应该重点看的是 K2 coverage gate 是否形成机械质量约束，而不是把它误解为业务执行能力扩展。

## 本地验证与远端验证

本地验证依然要跑，因为 `tests/dependabot_config_tests.cpp` 改了。先用 CLion 捆绑 clang-format 格式化该测试文件，避免 changed-file clang-format 在远端失败。然后在 `cmake-build-v1612` 或新的 build 目录上增量构建，让测试可执行文件重新链接。focused CTest 跑 `dependabot_config_tests`，确认新增参数断言生效。最后跑全量 CTest，确认 334 条默认 Debug 测试继续通过。

远端验证才是 v1613 的关键。推送后应该观察新 run 中的 `ubuntu coverage` job。预期它会在 report 步骤打印 negative-hit warning，但不会退出 64；随后生成 `coverage/core-coverage.txt`、`coverage/core-coverage.html`，并上传 `mini-kv-core-coverage` artifact。只有这个 job 绿，v1613 才算修完；如果它又在别的 parse issue 上失败，需要继续读日志并做更窄的修复，而不是直接跳到 floor。

这里还要记录一个来自 v1612 的正信号：coverage build 下的 full CTest 在 Ubuntu 只用了约 67 秒并全部通过。也就是说，K2 的构建和测试成本在 CI 预算内。真正卡住的是 gcovr parser，不是测试体量。这个信号对下一版很有帮助，因为设置 `--fail-under-line` 后，CI 成本不会突然不可控。

## 下一版如何继续

v1613 远端绿后，下一版要读取 `mini-kv-core-coverage` artifact 或 job 日志中的 `core-coverage.txt`。需要记录 line coverage baseline，然后设置 floor。比如如果 line coverage 是 74.5%，floor 可以按 playbook 写成约 72% 或按 gcovr 支持的精度写成 baseline minus 2 points。这个决定要写进 `docs/production-excellence-progress.md`，并在 README/START_HERE/讲解里说明。

下一版还要把 K2 状态从 `measuring baseline` 改成 `completed`。这一步之后，才到用户要求的 Claude review 暂停点。原因还是同一个：review checkpoint 是 coverage milestone 完成，不是 report 参数修完。Claude review 应该看到 K1 required sanitizer、K2 required coverage floor、远端 CI green、progress table 和归档证据都齐备。

如果 v1613 远端 coverage report 产出了很低的 baseline，也不要临时拔掉 filter 或人为美化数字。低 baseline 是真实项目状态的一部分，正确做法是设置 baseline minus 2 的初始 floor，再安排后续测试补强逐步 ratchet。K2 的第一目标是防退化，不是一次性制造漂亮覆盖率。

## 风险和回滚

v1613 的主要风险是 `--gcov-ignore-parse-errors negative_hits.warn_once_per_file` 被误解成过度宽松。应对方式就是文档和测试双重记录：它只处理 GCC/gcov negative hit parser issue，full CTest 和 report 生成仍然 required。如果后续出现其他 parse error，CI 仍可能失败；届时应读具体错误，而不是扩大忽略范围。

第二个风险是 gcovr 版本升级后参数语义可能变化。当前 Ubuntu 24.04 apt 安装的是 gcovr 7.0，日志明确支持这个参数值。若未来 runner 或 apt 包升级导致参数弃用，CI 会失败，维护者需要按新版本文档更新命令和测试。这个风险可接受，因为它会显式红，不会静默退化。

第三个风险是 baseline 产出后数字和预期不一致。v1613 不处理这个风险，因为它还没有看到完整 report。下一版要客观看数字，把 floor 写在真实 baseline 下方两点，而不是临时重写 filter。filter 如需调整，必须有明确理由，例如核心 command 文件继续拆分或某个文件被证明属于 receipt/evidence 而非 core runtime。

一句话总结：v1613 把 v1612 暴露的 `gcovr` negative branch-hit 解析失败收敛成有边界的 warning，保留 Ubuntu coverage job 的 required 性质和 full CTest 约束，让 mini-kv 能继续产出真实 K2 coverage baseline，同时不改变任何运行时命令、只读证据、写入、WAL、router 或执行边界。
