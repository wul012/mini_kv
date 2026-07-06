# FinalShell 分段演示记录

这个目录用于保存 Linux 现场演示的分段证据。

每一段单独建一个小文件夹，固定保留两个文件：

1. `01-执行命令.md`：本段在 FinalShell 中粘贴执行的命令。
2. `02-执行结果与解释.md`：用户实际执行后的终端结果，以及夹在关键位置的说明。

使用方式：

- 每次只演示一个证明点，避免终端输出太长。
- 用户在 FinalShell 执行 `01-执行命令.md` 中的命令。
- 用户把截图或复制出来的终端输出发给 Codex。
- Codex 把真实结果整理进 `02-执行结果与解释.md`，并在合适位置补充解释。

当前完成分段：

1. `step01-linux-env`：Linux 环境、项目版本、工具链。已完成。
2. `step02-build`：CMake/Ninja 构建。已完成。
3. `step03-tests`：CTest 自动测试。已完成。
4. `step04-login-mfd-ufd`：登录、MFD/UFD 二级目录隔离。已完成。
5. `step05-permission-admin`：权限拒绝、root 管理限制。已完成。
6. `step06-fd-persistence`：fd 偏移读写、退出重开持久化。已完成。
