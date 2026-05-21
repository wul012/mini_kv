# mini-kv Agent Notes

## Shared Four-Project Rules

The central backup and index is:

```text
D:\C\四项目理解统筹\AGENTS.md
```

For normal mini-kv work, use this local file first. The shared file is only an upstream reference for cross-project research, planning, quality evaluation, and long-term direction checks across:

- `D:\nodeproj\orderops-node`
- `D:\javaproj\advanced-order-platform`
- `D:\C\mini-kv`
- `D:\aiproj`

When doing cross-project evaluation, use `[MODE: RESEARCH]`: prefer read-only inspection, evidence-backed summaries, and clear separation between facts, judgments, and recommendations. Do not edit Node, Java, or aiproj unless the user explicitly asks for that project.

## Four-Project Evaluation Baseline

Use this baseline when judging overall developer/project capability unless newer evidence clearly changes it:

| Dimension | Rating |
|---|---|
| Architecture design | A- |
| Multi-language engineering | A |
| Test engineering | B+ |
| Output speed | S |
| Refactoring and debt management | B |
| Security and compliance awareness | A |
| Engineering discipline and restraint | C+ |

Core judgment: this is a high-output, strong-design, multi-stack developer with good safety awareness; the main growth area is knowing when to stop, refactor, and trade about 30% speed for restraint.

Evaluation rules:
- Praise speed, architecture, multi-language execution, and safety awareness, but do not overstate production readiness.
- Be explicit that current maturity is mostly single-project validation plus cross-project contract alignment unless real integration tests exist.
- Distinguish contract/evidence chain from live runtime integration.
- Treat large files, long names, repeated report renderers, string-based fixture checks, and delayed refactoring as real debt signals.
- Separate latest committed version from dirty working tree changes.
- When judging tests, identify whether evidence is unit, integration, smoke, fixture, screenshot, or real CI.
- Until real integration exists, describe the state as `single-project validation + cross-project contract alignment`.

Do not claim real four-project joint testing unless there is evidence such as Node actually calling Java read-only endpoints, Node actually reading mini-kv runtime output or release fixtures, one command producing a cross-project readiness report, or tests proving Node cannot trigger Java writes, mini-kv restore/load/compact execution, deployment, rollback, or production secret access.

## mini-kv Shared Direction

- mini-kv is a C++ storage and recovery learning system with production-style evidence, not merely C++ syntax practice.
- WAL, snapshot, restore, artifact digest, CHECKJSON, and retention fixtures must keep explicit no-order-authority and no-execution boundaries.
- Node may consume mini-kv evidence, but mini-kv does not become Java order authority and must not execute restore/load/compact as part of evidence review.
- Good future work strengthens evidence chain completeness, read-only/no-execution boundaries, audit/retention/release/rollback/restore/handoff contracts, focused boundary tests, and small reversible refactors that preserve public contracts.
- Risky future work combines unrelated features and refactors, lets one file grow too large without a split plan, relies only on happy-path or string-fragment tests, or advances docs/screenshots faster than executable evidence.

Refactoring rhythm:
- After 3-4 feature versions, prefer 1 version of contract-preserving refactor, deduplication, or test hardening.
- 写代码时不要形成难于维护的巨型代码文件；当文件接近大文件风险阈值时，必须做必要拆分，或先写清楚拆分计划再继续新增功能。
- Prefer structured fixture parsing and reusable test helpers over ever-growing string-fragment assertions when the risk justifies it.
- Move shared fixture loaders, report helpers, validators, renderers, and message collectors into common utilities when duplication is clear.

## Version Closure Discipline

Use these rules for mini-kv versions that include implementation, tests, fixtures, docs, screenshots, or release evidence:

- Generate final archive screenshots only after code, tests, fixtures, docs, and small formatting fixes are stable. Do not treat early screenshots as final evidence if later code changes require another build or test run.
- For rolling diagnostic fields such as `node_consumption`, tests for older versions should assert that historical meaning is still present, not that the old sentence remains the first sentence. New versions may separately assert the newest sentence is first when that ordering matters.
- After any C++ or test source change, rebuild with CMake before running CTest. Do not trust an old `.exe`, old test executable, or stale build output after a source patch.
- Preferred closure order: read the active Node plan, implement, run targeted tests, run full CTest, run real CLI/TCP smoke when relevant, update README/walkthrough/archive docs, rerun final verification for the archived screenshots, clean temporary files/processes, confirm a clean working tree, then commit/tag/push.
- Commit, supplemental tag, push, and remote verification are part of completing a mini-kv version unless the user explicitly asks not to commit.
- Run the cleanup gate before the final handoff: delete `cmake-build-v<version>`, scratch logs, render intermediates, and `d/<version>/_logs`; keep final `d/<version>/图片/`, `d/<version>/解释/说明.md`, fixtures, walkthroughs, docs, and source changes.

## C++ Ownership Rule

本线程/本项目中，Codex 的默认职责是推进 C++ 侧，也就是 `D:\C\mini-kv`。

执行要求：

- 只负责 mini-kv / C++ 代码、测试、文档、归档和 CI 修复。
- 读取 Node 计划只是为了判断 mini-kv 下一版方向，不代表可以修改 Node 项目。
- 如果最新跨项目计划的下一步不是 mini-kv / C++，则只读判断后停止，不修改 Node 或 Java。
- 如果用户要求“推进”但上下文不清，优先解释为推进 mini-kv / C++；若计划明确轮到 Node 或 Java，则说明原因并停止。
- 除非用户明确单独授权，否则不在 `D:\nodeproj` 或 `D:\javaproj` 中改文件、构建、启动服务或提交。

## Version Scope Rule

每个版本默认只推进一个大小合适、能够闭环验证的小主题。工作量不能过大，也不能过小：

- 过大：不要把多个独立功能、重构、体验改动塞进同一个版本。
- 过小：不要只改一点无法独立验证或无法体现版本价值的零碎内容。
- 每版应尽量包含实现、构建、测试、真实运行 smoke、归档、代码讲解和提交推送。
- 除非遇到阻塞修复、CI 补救、安全问题或用户明确要求，否则不要扩大或压缩版本范围。

## Cross-Project Plan Rule

每次推进 mini-kv 自己的下一个版本前，必须先只读查看 Node 项目的最新计划指导：

```text
D:\nodeproj\orderops-node\docs\plans
```

执行要求：

- 至少读取 `README.md` 和该目录下按修改时间最新的具体计划文件。
- 先确认计划中对当前 mini-kv 版本的目标、依赖关系、本版不做事项、验收顺序、完成标准和暂停条件。
- 读取后先判断计划是否合理；如果合理就按计划执行，如果不合理、过大、过小、依赖顺序不对或会破坏项目边界，先向用户说明问题并提出调整建议，不要直接执行。
- 版本主题优先服从该目录中的最新计划指导，再结合 mini-kv 自己的 `README.md` Roadmap。
- 如果跨项目计划与 mini-kv Roadmap 不一致，以跨项目计划的最新指导优先；若仍有疑惑，暂停并让用户确认。
- 最终回复中简要说明本轮读取了哪个计划文件，以及版本范围是否与计划一致。

## Command Result Archive

### Archive Root Rotation

Starting with mini-kv v122, new command screenshots and screenshot explanations must be written under the sibling archive root:

```text
d/<version>/图片/
d/<version>/解释/说明.md
```

Keep historical `a/`, `b/`, and `c/` archive roots exactly where they are. Do not move, rename, or rebalance old archive evidence just because a directory has grown large. If `d/` later becomes too large, add the next sibling archive root here first, then use that new root for future versions.

每次执行关键命令后，需要把重要执行结果归档。

v122 起的归档目录按版本组织：

```text
d/<version>/图片/
d/<version>/解释/说明.md
```

要求：

- 关键命令包括配置、构建、测试、运行验证、服务端 smoke test 等。
- 关键命令输出需要保存为 PNG 截图形式，放入 `d/<version>/图片/`。
- 截图文件名使用数字前缀，例如 `01-cmake-configure.png`。
- 在 `d/<version>/解释/` 中创建说明文档，解释每张截图对应的命令、结果和意义。
- 历史归档目录 `a/`、`b/`、`c/` 保留既有版本记录，不主动搬迁；v122 起新增运行截图和解释写入与 `a/`、`b/`、`c/` 同级的 `d/`。
- 如果启动了服务端或其他后台进程，验证结束后必须停止，并在说明文档中记录。
- 最终回复中简要说明新增的归档文件、保留文件和清理结果。

## Code Explanation Record Rule

### Walkthrough Directory Rotation

Keep `代码讲解记录_生产雏形阶段/` as the historical code walkthrough folder through mini-kv v121.

Starting with the next mini-kv version, write new code walkthrough documents to the sibling folder:

```text
代码讲解记录_生产雏形阶段_第二册/
```

Each walkthrough folder keeps its own `README.md` index. The old folder may point to the newer sibling, but old walkthrough files should not be moved unless the user explicitly asks for a historical cleanup pass.

每个版本都要补充代码讲解，但篇数按实际内容量决定：

- 如果本版实现和配套测试/文档说明都不多，优先合并成一篇讲解记录。
- 如果本版核心实现较复杂，或测试/归档/文档本身也需要较多解释，可以拆成两篇。
- 不为了固定篇数而硬拆文档；讲解应服务于后续回看和维护。
- 历史讲解保留在 `代码讲解记录/`，不主动搬迁，除非用户明确要求整理旧文件，避免产生大规模无意义改名 diff。
- v122 起，新增代码讲解写入与 `代码讲解记录/` 同级的新大文件夹；目录命名格式为 `代码讲解记录_<项目阶段名>_第二册/`，例如当前阶段使用：

```text
代码讲解记录_生产雏形阶段_第二册/
```

- 文件名继续延续编号和版本，例如：

```text
代码讲解记录_生产雏形阶段_第二册/178-example-topic-v122.md
```

- 讲解版式、编号延续、索引方式保持不变；以后新增索引优先写入当前阶段文件夹的 `README.md`，并指向该文件夹内的实际文件。
- 如果同一版本有多篇讲解，全部放在当前阶段同级文件夹中，并通过文件名区分主题。
- 如果某个讲解/文档归档目录内容已经过多，后续文档可以在同级新目录继续写入，旧目录保留不搬迁；优先通过同级目录名和 README/索引串联内容，不要把所有文档硬塞进一个文件夹。

## Code Explanation Format Rule

后续 mini-kv 代码讲解记录的版式，优先参考：

```text
D:\javaproj\advanced-order-platform\代码讲解记录\49-version-45-ops-evidence.md
```

执行要求：

- 参考它的表达结构和讲解密度，不复制 Java 项目内容。
- 开头先说明本版目标、角色定位、为什么做，以及明确“不是什么”：不是新的执行入口、不绕过审批/契约、不写不该写的数据、不突破项目边界。
- 分模块讲清楚文件职责：入口在哪里、响应/数据结构是什么、核心聚合/格式化逻辑在哪里、测试在哪里。
- 对关键字段或命令契约解释“控制面读到后怎么理解”，特别是 `read_only`、`execution_allowed`、`order_authoritative`、`warnings/blockers/diagnostics` 这类边界字段。
- 测试部分不能只列命令，要说明每类断言保护什么行为，以及真实 smoke 证明了什么。
- 结尾保留一句话总结，说明本版给后续 Node/Java/mini-kv 松耦合融合带来的价值。
- 如果本版改动很小，可以合并为一篇，但仍应保留上述表达骨架，避免只罗列字段和文件。

## Local Toolchain Environment

本机直接运行 CMake/CTest/测试程序时，优先使用 CLion 捆绑工具链，避免环境变量缺失导致误判。

常用路径：

```powershell
$cmake = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
$ctest = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\ctest.exe'
$env:PATH = 'D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
```

注意：直接跑 `ctest`、测试 exe、`minikv_cli.exe`、`minikv_server.exe`、`minikv_client.exe` 前，需要先把 `D:\CLION\CLion 2025.1.1\bin\mingw\bin` 加进当前 `PATH`。否则 Windows 可能因为找不到 MinGW 运行时 DLL 报 `0xc0000135`，这属于本机运行环境问题，不代表代码或测试失败。

## Current Version

From mini-kv v122 onward, new run screenshots and explanations should use:

```text
d/<version>/图片/
d/<version>/解释/说明.md
```

From the next mini-kv version onward, new code walkthrough documents should use:

```text
代码讲解记录_生产雏形阶段_第二册/
```

当前第一版归档位置：

```text
a/1/图片/
a/1/解释/说明.md
```

v2 到 v121 的运行截图和解释归档位置：

```text
c/<version>/图片/
c/<version>/解释/说明.md
```

Note: The `c/<version>/...` block above is historical through v121. For v122 and later, use the `d/<version>/...` archive root defined in Command Result Archive.
