# OSFS 课程设计说明

本文说明 mini-kv v1629 新增的 OSFS 模块如何用于“操作系统课程设计”。它的定位不是把 mini-kv 原有键值存储主线改成传统文件系统，而是在现有 C++ 工程底座上增加一个独立的、可运行、可测试、可讲解的用户态二级文件系统模拟层。这样既能贴合课程设计中“Linux 二级文件系统”或“操作系统中的存储管理软件设计”方向，又不会污染原来的 WAL、snapshot、TCP/RESP、KV command 体系。

## 推荐选题

如果老师允许选择“其他：操作系统中的软件设计”，推荐题目是：

```text
基于 Linux 的轻量级用户态二级文件系统模拟器设计与实现
```

如果老师要求必须贴合“Linux 二级文件系统”题目，可以把 OSFS 作为该题目的用户态模拟版本来讲。它已经具备课程报告中最重要的结构：二进制磁盘镜像、superblock、block bitmap、inode table、root directory、文件创建/删除、打开/关闭、读写、权限控制、目录 listing 和可执行测试。

## 和原 mini-kv 的关系

mini-kv 原核心仍然是键值存储系统：

- `minikv_cli` 继续负责 KV 命令。
- `minikv_server` 继续负责 TCP/RESP 服务。
- WAL、snapshot、restore、CHECKJSON、SMOKEJSON 等证据链不变。
- OSFS 不接管 managed audit、order authority、router、WAL 或恢复执行权限。

OSFS 是一个独立入口：

```text
minikv_osfs
```

它共享工程构建、测试、代码规范和 CTest 体系，但不把文件系统命令塞进 `CommandProcessor` 的 KV 命令主链。这一点很关键：课设功能有自己的模块边界，后续可以继续扩展 login、权限、句柄表、磁盘布局和报告材料，而不会把原项目做成一个难维护的混合体。

## 当前实现覆盖的操作系统概念

| OSFS 能力 | 对应操作系统概念 |
|---|---|
| `disk.img` 二进制文件 | 用普通文件模拟磁盘设备 |
| block size / block count | 磁盘块与块寻址 |
| superblock | 文件系统总体元信息 |
| block bitmap | 空闲块分配与回收 |
| inode table | 文件元数据管理 |
| root directory | 文件名到 inode 的映射 |
| create/delete | 文件生命周期 |
| open/close/read/write | 用户态文件操作接口 |
| mode / owner uid | 简化权限模型 |
| CTest + CLI smoke | 系统测试与演示验证 |

## 构建

Windows 本机建议继续使用 CLion 捆绑的 CMake/MinGW 工具链：

```powershell
$env:PATH='D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
$cmake='D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
& $cmake -S . -B cmake-build-osfs -G "MinGW Makefiles"
& $cmake --build cmake-build-osfs --target minikv_osfs minikv_osfs_tests
```

Linux 下可用普通 CMake：

```bash
cmake -S . -B build
cmake --build build --target minikv_osfs minikv_osfs_tests
ctest --test-dir build -R 'osfs_(tests|cli_smoke)' --output-on-failure
```

## 演示脚本

格式化一个新的模拟磁盘：

```powershell
.\cmake-build-osfs\minikv_osfs.exe --disk osfs-course.img --format --blocks 96
```

进入交互 shell 后可以执行：

```text
CREATE report
OPEN report rw
WRITE 3 course design storage layer
READ 3
CHMOD report 0600
LOGIN guest
OPEN report w
DIR
QUIT
```

这组命令展示了完整链路：

1. 创建二进制磁盘镜像。
2. 写入 superblock、bitmap、inode table 和 root directory。
3. 创建文件 `report`。
4. 打开文件得到 fd。
5. 通过 fd 写入内容。
6. 通过 fd 读回内容。
7. 修改权限为 `0600`。
8. 切换到普通用户 `guest`。
9. 验证普通用户不能写。
10. 输出目录行，包含文件名、inode、物理块号、保护码、owner、长度。

目录输出示例：

```text
name inode physical mode owner size
report 1 15 0600 0 27
```

它正好对应课设要求里的“文件名、物理地址、保护码、文件长度”。

## 测试入口

本版新增两个测试：

```text
osfs_tests
osfs_cli_smoke
```

`osfs_tests` 直接调用 C++ API，验证：

- 格式化磁盘后 superblock 几何信息正确。
- 创建文件后目录项、inode、物理块号和长度正确。
- 关闭进程后重新打开磁盘仍能读回文件内容。
- `chmod 0600` 后普通用户读写被拒绝。
- 句柄式 `OPEN/READ/WRITE/CLOSE` 命令能正确工作。

`osfs_cli_smoke` 通过真实 `minikv_osfs` 可执行程序运行脚本，并检查输出中包含：

- shell banner。
- `OK created report`。
- 写入后读回的文本。
- `ERR permission denied`。
- 目录中的 `report` 行。

## 目前还不是完整 EXT2

OSFS 是课程级二级文件系统模拟层，不是完整 EXT2。当前版本先做了最核心、最能支撑报告和演示的部分：

- 单级根目录。
- 直接块，不含一级/二级/三级间接块。
- 简化 uid 权限模型。
- 简化 fd 表，暂不保存偏移量。
- 暂无多用户密码文件。
- 暂无崩溃恢复日志。

这些限制是有意保留的。课设报告可以把它们写成“当前设计边界”和“后续改进方向”，而不是把一个学习系统硬说成完整生产文件系统。

## 后续最值得补的内容

按课程价值排序，后续建议：

1. 增加 `mkfs` 命令，让格式化也可以在 OSFS shell 内完成。
2. 增加用户表和 `login user password`。
3. 让 fd 保存读写偏移，支持追加写和分段读。
4. 增加多级目录。
5. 增加一级间接块，突破 8 个直接块限制。
6. 增加 `fsck`，检查 bitmap、inode、directory 的一致性。
7. 增加报告用结构图和演示脚本。

## 课程报告写法建议

报告不要把重点放在 mini-kv 的治理文档、跨项目证据链或 managed audit receipt 上。老师更关心操作系统机制，所以报告建议按下面组织：

1. 需求分析：说明课程要求、用户态模拟磁盘、文件操作、权限、目录输出。
2. 总体设计：画出 `minikv_osfs -> CommandProcessor -> FileSystem -> VirtualDisk -> disk.img`。
3. 磁盘布局：说明 0 号块 superblock、1 号块 bitmap、2 号块起 inode table、后续 data blocks。
4. 数据结构：说明 superblock、inode、directory entry、bitmap 的字段。
5. 核心算法：说明 block allocation、create、write、read、delete、chmod。
6. 测试与运行：展示 CTest 和 CLI smoke。
7. 总结与改进：说明当前是单级目录、直接块、简化权限，后续可加多级目录、间接块和 fsck。

一句话定位：OSFS 让 mini-kv 从“工程量很大的 KV 项目”变成“可以被操作系统课设理解和评分的存储管理实验系统”。
