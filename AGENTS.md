# mini-kv Agent Notes

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
