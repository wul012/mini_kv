# v1629 OSFS 课程设计文件系统层代码讲解

## 本版目标、角色和边界

v1629 的目标不是继续扩展 runtime receipt，也不是再给 managed audit 链路增加一层回显证明。本版的目标很明确：为 mini-kv 增加一个独立的 OSFS 课程设计模块，让这个项目能够更自然地承接操作系统课程设计里的“二级文件系统”或“存储管理软件设计”方向。前面的判断已经很清楚：mini-kv 原本工程量很大，包含 WAL、snapshot、TCP/RESP、CLI、命令解析、测试和证据链，但如果原样提交给操作系统课设，老师很可能会问“superblock 在哪里、inode 在哪里、bitmap 在哪里、文件打开和读写在哪里”。所以本版补的不是装饰，而是把课程要求中最关键的操作系统机制显式落到代码里。

本版新增的入口是 `minikv_osfs`，新增的公共头文件位于 `include/minikv/osfs/`，新增的实现文件位于 `src/osfs_*.cpp`。它和原来的 `minikv_cli`、`minikv_server`、`minikv_client` 并列存在，不把 OSFS 命令塞进原 KV `CommandProcessor`。这是一条很重要的工程边界：mini-kv 主线仍然是键值存储系统，WAL、snapshot、TCP server、runtime evidence 和跨项目 receipt 全部保持原样；OSFS 是一个独立的用户态文件系统模拟层，用来解释磁盘块、superblock、inode、目录、权限和文件操作。这样做既能服务课设，又不会把原项目变成一个“KV 命令和文件系统命令混在一起”的难维护系统。

本版也明确“不是什么”。它不是 Linux kernel patch，不新增系统调用；不是字符设备驱动，不加载内核模块；不是完整 EXT2，不承诺多级目录、间接块、组描述符、真实用户密码数据库；也不是生产文件系统，不承担崩溃一致性或安全隔离责任。它是一个课程级、可运行、可测试、可讲清机理的用户态二级文件系统模拟器。它用普通二进制文件模拟磁盘，把磁盘拆成固定大小的 block，再在这些 block 上放置 superblock、bitmap、inode table 和 root directory。这个边界比“硬说 mini-kv 就是文件系统”诚实，也更容易被老师接受。

## 入口命令和模块结构

本版 CMake 增加了三个核心接入点。第一，`minikv` 静态库新增 `src/osfs_virtual_disk.cpp`、`src/osfs_filesystem.cpp` 和 `src/osfs_command_processor.cpp`。第二，新增可执行程序 `minikv_osfs`，其入口是 `src/osfs_main.cpp`。第三，新增两个测试：`osfs_tests` 和 `osfs_cli_smoke`。其中 `osfs_tests` 是直接调用 C++ API 的结构性测试，`osfs_cli_smoke` 是通过真实 `minikv_osfs` 可执行程序跑脚本的演示测试。这个组合避免了“库测试过了但命令行演示不通”的常见课设风险。

公共头文件分为三层。`include/minikv/osfs/virtual_disk.hpp` 定义 `VirtualDisk`，它只关心磁盘镜像文件、block size、block count、按块读写。`include/minikv/osfs/filesystem.hpp` 定义 `FileSystem`、`SuperBlock`、`DirectoryEntry` 和 `FileStat`，它负责文件系统语义：格式化、打开已有镜像、创建文件、删除文件、读写文件、修改权限、列目录和查 stat。`include/minikv/osfs/command_processor.hpp` 定义 OSFS 自己的 `CommandProcessor`，负责把交互命令转成 `FileSystem` 调用，并维护当前用户和文件句柄表。顶层 `include/minikv/osfs.hpp` 只是聚合头，方便测试和入口程序引用。

这样的拆分是为了让每层职责清楚。`VirtualDisk` 不知道 inode，也不关心权限；`FileSystem` 不知道交互命令怎么写，也不保存 shell 当前用户；`CommandProcessor` 不直接操作二进制块，而是调用 `FileSystem`。如果后续要加 `fsck`、多级目录、间接块、追加写，应该优先扩展 `FileSystem` 或再拆子模块，而不是把所有逻辑堆进 `osfs_main.cpp`。这也是本版没有把实现直接写成一个大 main 的原因。

## 磁盘布局和响应结构

OSFS 的磁盘布局是固定而透明的。0 号块存 superblock，1 号块存 block bitmap，2 号块开始存 inode table，inode table 后面的第一个数据块作为 root directory 的初始目录块。后续普通文件的数据块都从 data area 中按 bitmap 分配。`FormatOptions` 默认 block size 是 512，block count 是 256，inode count 是 64；测试和 smoke 使用 96 个块，便于快速验证。

superblock 的磁盘结构在 `src/osfs_filesystem.cpp` 内部定义为 `SuperBlockDisk`。它包含 magic、version、block_size、block_count、inode_count、bitmap_block、inode_table_start、inode_table_blocks、data_start_block 和 root_inode。这些字段就是课程报告里最值得画图解释的部分。用户通过 `FileSystem::super_block()` 读到的是公开的 `SuperBlock`，隐藏了 reserved 字段和磁盘编码细节，但保留了报告需要的几何信息。

inode 的磁盘结构是 `InodeDisk`。每个 inode 记录 used、type、mode、owner_uid、size、created_at、modified_at 和 8 个 direct block。这里有意使用直接块模型，因为它足够解释“文件元数据如何指向物理块”，也足够支撑 `create/write/read/delete`。它还没有一级、二级、三级间接块，这是后续扩展项，不在本版一口吃下。目录项结构是 `DirectoryEntryDisk`，每项记录 used、inode 和 name。root directory 本质上就是若干目录项顺序存放在目录数据块里，列目录时根据目录项找到 inode，再把文件名、inode 编号、首个物理块号、保护码、owner 和长度组合成输出。

命令行响应也刻意贴近课程要求。`DIR` 输出表头是 `name inode physical mode owner size`，例如 `report 1 15 0600 0 27`。这里的 `physical` 是文件第一个数据块号，对应课程要求里的“物理地址”；`mode` 是类似 Unix 的保护码；`size` 是文件长度。`STAT` 输出更详细的 key-value 行，包含 created_at 和 modified_at。`OPEN` 返回 `OK fd=3`，之后 `READ 3`、`WRITE 3 text`、`CLOSE 3` 都围绕这个句柄操作。这样演示时用户能看到从文件名到句柄、从句柄到 inode 和数据块的完整路径。

## 核心流程

格式化流程从 `FileSystem::format()` 开始。它先根据 inode 数量和 block size 计算 inode table 需要多少块，再推导 data_start_block。如果磁盘几何太小，直接抛出异常，不生成半坏镜像。随后 `VirtualDisk::create()` 创建并扩展二进制文件，`write_super_block()` 写入 superblock，bitmap 标记 0 号 superblock、1 号 bitmap、inode table 区域和 root directory 初始块为已使用，然后创建 root inode。root inode 的 type 是目录，mode 是 `0755`，owner 是 root，direct[0] 指向第一个目录数据块。最后清空 root directory 数据块。这个流程对应课设报告里的“文件系统初始化”。

创建文件流程由 `FileSystem::create_file()` 处理。它先验证文件名，拒绝空名、`.`、`..`、包含路径分隔符的名字和过长名字。然后读取 superblock、inode table、bitmap 和 root directory。若目录中已有同名项，返回 `file already exists`；若 inode table 没有空位，返回 `inode table is full`。找到空 inode 后，再找空目录项。如果 root directory 当前没有空目录项，会尝试给 root directory 分配一个新的目录块。这一点让目录不是完全固定大小，后续扩展多级目录时也更自然。最后写回目录项、inode table 和 bitmap。

写文件流程由 `FileSystem::write_file()` 处理。它先根据目录项找到 inode，再用 `has_permission()` 检查当前 uid 是否有写权限。然后计算内容需要多少块，如果超过 8 个 direct block，就返回 `file exceeds direct-block limit`。这不是偷懒，而是本版明确的课程边界：直接块模型先成立，间接块留给下一版。写入时会先释放原 direct blocks 并清零，再从 bitmap 分配新块，把内容按 block size 切片写入。写完后更新 inode size 和 modified_at，最后写回 inode table 和 bitmap。读文件流程反过来，根据 inode 的 direct block 顺序读出内容，并按 inode.size 截断末尾零填充。

删除文件流程会检查 owner 或 root 权限，释放 inode direct blocks，清空数据块，清空 inode 和目录项，再写回目录与 bitmap。`chmod` 只允许 owner 或 root 修改 mode。`can_read()` 和 `can_write()` 是给命令层使用的便捷查询，底层仍然走 inode 权限位。当前权限模型只区分 owner 和 other，不做 group，这是课程级模拟的合理简化。

## 命令层和用户会话

`osfs::CommandProcessor` 是面向演示的 shell 命令层。它维护 `current_user_`、`current_uid_`、`next_fd_` 和 `handles_`。默认用户是 root，uid 为 0。`LOGIN user` 会把用户名通过 `uid_for_user()` 转成稳定 uid，并清空当前句柄表。这里没有密码文件，后续可以加 `passwd` 或用户表模块；本版先让权限边界可演示。

命令解析保持简单透明。`HELP` 输出命令列表，`WHOAMI` 输出当前用户和 uid，`DIR` 列目录，`CREATE`、`DELETE`、`STAT`、`CHMOD` 对应文件系统 API，`OPEN` 检查读写权限并返回 fd，`READ`、`WRITE`、`CLOSE` 操作句柄表。`WRITE fd text` 当前是整文件覆盖写，不是追加写，也不维护 fd offset。这个限制在讲解中必须说清楚，因为真实 OS 的 file descriptor 会维护 offset，本版只是先引入“句柄生命周期”和“权限检查”概念。

`src/osfs_main.cpp` 负责可执行程序入口。它支持 `--disk path`、`--format`、`--blocks count`、`--block-size bytes`、`--script path`。没有 `--format` 时打开已有磁盘镜像，有 `--format` 时创建新镜像。Windows 下特别处理了 `wmain` 和 `-municode`，原因是用户路径可能包含中文用户名。如果继续用普通 `main(char**)`，MinGW 对窄字符 argv 的路径转换可能在 `std::filesystem::path` 中报 `Cannot convert character sequence`。本版已经用宽字符入口修掉这个本机真实问题，所以 `C:\Users\吴林东\AppData\Local\Temp\...` 这样的路径可以通过真实 smoke。

## 测试保护了什么

`tests/osfs_tests.cpp` 是本版最重要的结构测试。第一段格式化磁盘，断言 superblock version、block_size、block_count、inode_count、data_start_block 等几何信息正确，并验证新文件系统目录为空。然后创建 `notes.txt`，重复创建应失败，写入 `hello os course` 后立即读回，目录项必须包含正确文件名、首个物理块、size 和 `0644` 保护码。这里保护的是格式化、inode 分配、目录项写入、数据块分配和读写往返。

第二段关闭并重新打开同一个磁盘镜像，证明内容不是只存在内存里，而是真的落到了二进制文件。接着执行 `chmod 0600`，普通用户读写必须被拒绝，root 删除后目录为空。这里保护的是持久化、权限和 block/inode 回收。第三段测试命令处理器，覆盖 `WHOAMI`、`CREATE`、`OPEN`、`WRITE`、`READ`、`CLOSE`、`LOGIN` 和普通用户写拒绝。它证明演示命令不是文档里的伪代码，而是走真实 C++ 命令层。

本版还额外覆盖了一个容易被课设忽略但很像真实文件系统的问题：覆盖写之前必须先确认空间足够。测试会构造一个小磁盘，用 `filler` 消耗大部分数据块，再尝试把 `keep` 覆盖成一个需要更多块的新内容。预期结果是返回 `no free data block`，并且 `keep` 的旧内容仍然可以读回。这条断言保护了失败路径的原子性：不能因为写入失败，就把旧文件内容提前清空。

`cmake/run_osfs_smoke.cmake` 和 `tests/osfs_smoke_script.txt` 组成 CLI smoke。CTest 会调用真实 `minikv_osfs`，传入临时磁盘路径和脚本，断言输出中出现 shell banner、`OK created report`、写入后读回的文本、`ERR permission denied` 和目录行 `report 1 ... 0600 0 27`。这条测试的价值在于把“课设现场演示路径”纳入自动化。以后如果有人改坏参数解析、脚本执行、权限拒绝或目录输出，CTest 会立刻红。

本版还跑了完整本地 CTest，344/344 通过。这个结果说明新增 OSFS 不只是自己能跑，还没有破坏 store、command、WAL、snapshot、RESP、TCP、runtime receipt 和既有治理测试。由于 OSFS 被接入 `minikv` 静态库，完整构建也证明新增 translation unit 不会造成链接层面冲突。

## 对原项目的价值

这版不是为了课设硬堆冗余。它给 mini-kv 增加了一个更底层的存储抽象训练场：原项目的 WAL 和 snapshot 更偏日志与状态备份，OSFS 则把 block、bitmap、inode、directory、permission 和 fd 这些操作系统概念显式建模。后续如果要做 page/block 管理、存储分层、压缩、分片持久化或一致性检查，OSFS 可以作为一个独立实验区。它不会替代 KV store，但能补足“存储系统底层结构”这块原来不够可见的能力。

对课程设计来说，OSFS 也让输入和输出更透明。输入是一串 shell 命令：格式化磁盘、创建文件、打开文件、写入文本、读取文本、修改权限、切换用户、列目录。输出是可解释的目录行、stat 行、权限拒绝和真实磁盘镜像。老师问“物理地址在哪里”，可以指向 `DIR` 的 physical 块号；问“保护码在哪里”，可以指向 `mode=0600`；问“元信息在哪里”，可以讲 superblock 和 inode table；问“是不是只在内存里”，可以说明测试关闭后重新 `FileSystem::open()` 仍能读回内容。

一句话总结：v1629 把 mini-kv 的课设适配从“靠报告解释已有 KV 项目”推进到“有独立 OS 文件系统模拟层、真实命令入口和自动化测试”，同时保持原 KV/WAL/TCP/runtime evidence 主线不被污染，为后续 Node/Java/mini-kv 松耦合融合之外的教学展示和存储底层扩展打开了一个干净入口。
