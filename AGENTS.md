# mini-kv Agent Notes

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

每次执行关键命令后，需要把重要执行结果归档。

归档目录按版本组织：

```text
a/<version>/图片/
a/<version>/解释/
```

要求：

- 关键命令包括配置、构建、测试、运行验证、服务端 smoke test 等。
- 关键命令输出需要保存为 PNG 截图形式，放入 `a/<version>/图片/`。
- 截图文件名使用数字前缀，例如 `01-cmake-configure.png`。
- 在 `a/<version>/解释/` 中创建说明文档，解释每张截图对应的命令、结果和意义。
- 如果启动了服务端或其他后台进程，验证结束后必须停止，并在说明文档中记录。
- 最终回复中简要说明新增的归档文件、保留文件和清理结果。

## Code Explanation Record Rule

每个版本都要补充 `代码讲解记录/`，但篇数按实际内容量决定：

- 如果本版实现和配套测试/文档说明都不多，优先合并成一篇讲解记录。
- 如果本版核心实现较复杂，或测试/归档/文档本身也需要较多解释，可以拆成两篇。
- 不为了固定篇数而硬拆文档；讲解应服务于后续回看和维护。
- `代码讲解记录/README.md` 索引要和实际新增篇数一致。

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

当前第一版归档位置：

```text
a/1/图片/
a/1/解释/说明.md
```
