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
7. `step07-delete-dir-write-denial`：删除文件、DIR 字段展示、写权限拒绝。已完成。
8. `step08-indirect-boundary`：跨过八个直接块进入一级间接块。已完成。
9. `step09-fsck-corruption`：手工破坏 block bitmap，证明 FSCK 输出 ERROR。已完成。

## 复核说明

先读 [`00-复核说明.md`](00-复核说明.md)。2026-07-13 复核发现，Step 04-06 在 `--format` 后又执行了 `USERADD alice` 或 `USERADD bob`，但这两个用户本来就是格式化时的预置账号；原 grep 又没有保留 `ERR user already exists`。因此这些 USERADD 行不能作为用户创建成功证据，历史输出不改写，相关结论已重新校准。Carol 等新用户名的 USERADD、后续登录、UFD 隔离、权限、fd 和持久化证据仍然有效。
