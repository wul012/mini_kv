# OSFS 课程设计说明（v1633）

OSFS 是 mini-kv 仓库中的独立 C++ 课程设计模块，对应《课程设计（操作系统）要求 2026》中的“实验二 Linux 二级文件系统”。它使用二进制文件模拟磁盘，实现 MFD/UFD 二级目录、持久化用户认证、inode/块管理、文件描述符位移和权限控制。OSFS 复用仓库的 CMake、CTest 和 CI，不进入 KV、WAL、snapshot、restore 或 TCP/RESP 运行链路。

## 选题名称

```text
基于虚拟磁盘的 MFD/UFD 二级文件系统设计与实现
```

## 与课程要求的关系

| 课程要求 | OSFS v1633 |
|---|---|
| 二进制文件模拟磁盘 | `VirtualDisk` 按固定 block 读写 `.img` |
| 用户、inode、超级块、文件信息落盘 | user table、inode table、superblock、data blocks |
| 二级目录 | MFD 中保存 user -> UFD inode，每用户一个真实 UFD |
| login | `LOGIN user password`，密码错误失败 |
| dir/create/delete/open/close/read/write | 全部由 `minikv_osfs` 命令行提供 |
| DIR 字段 | name、inode、physical、mode、owner、size |
| 读写保护 | uid/gid 与 owner/group/other 权限位 |
| 打开文件表 | fd 保存独立 read/write offset，支持 SEEK/TELL |

完整证据见 `课程设计交付/v1633-osfs-final/需求-证据矩阵.md`。

## 模块结构

- `src/osfs_main.cpp`：命令行入口、参数和脚本执行。
- `src/osfs_command_processor.cpp`：认证会话、fd 表和命令解析。
- `src/osfs_filesystem.cpp`：格式化、打开、用户表和认证。
- `src/osfs_disk_layout.*`：落盘结构、双位图、inode/用户/目录编解码。
- `src/osfs_directory.cpp`：UFD 中的 create/delete/dir/stat/chmod。
- `src/osfs_file_io.cpp`：整体替换、truncate、range read/write。
- `src/osfs_virtual_disk.cpp`：块设备模拟。
- `tests/osfs_tests.cpp`：结构、认证、目录、权限、位移、跨块和持久化测试。

## 磁盘格式 v2

默认块大小为 512 字节：

```text
Block 0      Superblock
Block 1      Block bitmap
Block 2      Inode bitmap
Block 3      Persistent user table
Block 4..N   Inode table
Data area    MFD block + UFD blocks + file blocks + free blocks
```

超级块记录 magic、版本、几何信息、各表位置、MFD inode、空闲 block/inode 数和用户数。inode 固定 96 字节，包含 type、mode、uid/gid、size、ctime/atime/mtime、八个直接块号和一个一级间接块号。用户记录固定 64 字节，包含 username、uid、gid、UFD inode 和教学密码哈希。

格式化预置：

```text
root  / root      uid=0    gid=0
alice / alice123  uid=1000 gid=100
bob   / bob123    uid=1001 gid=100
```

这些口令只用于课堂演示。FNV-1a 加领域前缀可以避免明文落盘，但不具备生产密码派生算法的抗暴力能力。

## MFD/UFD 查找

格式化时，MFD 使用 inode 0。MFD 的目录项以用户名为 name，以该用户 UFD inode 为 inode。用户表也保存 UFD inode，登录后文件系统按当前 uid 定位 UFD。普通文件名只在当前 UFD 中查找；`alice/report` 这样的限定引用用于 root 审查和跨用户权限测试。

`DIR` 默认列当前 UFD。普通用户不能执行 `DIR alice` 查看他人目录，root 可以。目录隔离来自不同 inode 和不同目录数据块，不是从共享列表按 owner 过滤。

## fd 位移语义

| 模式 | 初始状态 |
|---|---|
| `r` | read offset = 0，只读 |
| `w` | 先截断，write offset = 0，只写 |
| `rw` | 保留内容，read/write offset = 0 |
| `a` | write offset = EOF，只写 |

`READ fd [length]` 从 read offset 开始，按实际字节数推进。`WRITE fd text` 从 write offset 覆盖或扩展，成功后推进。`SEEK fd offset` 修改允许方向的位置，`TELL fd` 显示状态。底层通过 block index 和 block inner offset 处理跨块 I/O。

## 构建和测试

Windows：

```powershell
$env:PATH='D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
$cmake='D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
$ctest='D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\ctest.exe'
& $cmake -S . -B cmake-build-osfs -G "MinGW Makefiles"
& $cmake --build cmake-build-osfs --target minikv_osfs minikv_osfs_tests
& $ctest --test-dir cmake-build-osfs -R '^osfs_tests$|^osfs_cli_smoke$' --output-on-failure
```

最终演示：

```powershell
.\cmake-build-osfs\minikv_osfs.exe --disk osfs-course.img --format --blocks 96 --script .\课程设计交付\v1633-osfs-final\osfs-demo-script.txt
```

关闭后去掉 `--format` 再次打开相同镜像，可以验证用户表、UFD 和文件内容持久化。

## 测试保护点

- 未登录命令被拒绝，错误密码不切换身份。
- root、Alice、Bob 的 UFD inode 不同。
- Alice/Bob 的 DIR 互相隔离。
- Bob 对 Alice 0600 文件的读写被拒绝。
- root 可以列出 Alice UFD。
- 用户表、目录和文件重开后仍存在。
- 连续写、分段读、原位覆盖、append 和 EOF 位移正确。
- range I/O 跨 512 字节块边界。
- 空间不足与一级间接块容量超限不会破坏旧内容。
- 全量 CTest 证明 OSFS 修改未破坏原 mini-kv 主线。

## 设计边界

OSFS 完成的是课程要求中的 MFD/UFD 两级目录，不支持任意深度子目录。每个普通文件 inode 有八个直接块和一个一级间接块，容量上限是 `8 + block_size / 4` 个数据块；当前仍是单一教学 block group，没有 group descriptor、二级/三级间接块、fsck、崩溃日志或多进程并发。这些内容会在报告和需求矩阵中区分为已完成机制、教学简化或后续改进，不把尚未实现的能力伪装成已完成。

最终交付入口为 `课程设计交付/v1633-osfs-final/`。`v1630-osfs-final` 只保留历史草稿，不再用于提交。
