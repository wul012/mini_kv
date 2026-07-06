# v1631 代码讲解：把 OSFS 从单根目录原型改造成真正落盘的 MFD/UFD 二级文件系统

## 1. 本版目标、角色定位与明确边界

v1629 的 OSFS 已经能够把文件内容写进二进制磁盘镜像，也具备超级块、块位图、inode 表、目录项和 `CREATE/OPEN/READ/WRITE/CLOSE` 命令。但是它把所有文件放在唯一根目录里，`LOGIN guest` 只是把用户名临时哈希为 uid，磁盘中并没有用户记录、密码或用户目录。这样的实现可以演示“文件如何映射到数据块”，却没有回答课程题目“Linux 二级文件系统”最核心的问题：主文件目录 MFD 如何找到用户，用户文件目录 UFD 又如何限定本用户可见的文件集合。

v1631 的任务是关闭这个定义性缺口。它在同一个二进制磁盘镜像中保存用户表、MFD、三个真实 UFD、块位图、inode 位图、inode 表和数据块；命令行必须经过密码认证后才能执行文件操作。这里的“真实”不是说它等同 Linux EXT2，而是说目录层次由落盘结构表达，关闭程序再打开镜像后仍能恢复，不能靠 `CommandProcessor` 内存里的 map 假装存在。

本版不是新的 KV 命令入口，不把 OSFS 接入 RESP/TCP，不复用或触发 mini-kv 的 WAL、snapshot、restore、compact，也不授予 Node/Java 任何执行能力。OSFS 仍是独立课程设计入口。密码哈希采用带固定领域前缀的 FNV-1a 教学实现，目的是避免明文落盘并解释认证链路，不宣称具备生产密码存储强度。目录层次严格做到课程题目的 MFD/UFD 两级，不继续扩展成任意深度路径树。

## 2. 入口和公开数据结构

可执行入口仍然是 `src/osfs_main.cpp` 生成的 `minikv_osfs`。启动时，`--format` 调用 `FileSystem::format` 创建磁盘镜像；没有 `--format` 时调用 `FileSystem::open` 读取既有超级块。输入行交给 `minikv::osfs::CommandProcessor::execute`，因此脚本运行和交互运行使用同一条命令路径，测试不会绕过真实解析器。

公开头文件 `include/minikv/osfs/filesystem.hpp` 把磁盘格式可观察信息扩展为 `SuperBlock`、`UserInfo`、`DirectoryEntry` 和 `FileStat`。`SuperBlock` 现在报告格式版本 2、MFD inode、空闲块数、空闲 inode 数和用户数；`UserInfo` 只暴露用户名、uid、gid 和 UFD inode，不暴露密码哈希；目录项和 stat 同时提供 owner uid/gid，stat 还区分创建、访问和修改时间。调用者由此可以解释磁盘组织，但不能越过认证接口读取敏感认证材料。

`authenticate(username, password)` 是认证入口。它读取磁盘用户表，按用户名找到记录，再用相同领域前缀计算候选哈希并比较。失败只返回空值，不区分“用户不存在”和“密码错误”，命令层统一形成 `ERR authentication failed`。`user_by_uid`、`users` 服务于目录解析和教学观察；文件创建、删除、读写、chmod、stat 都继续通过 `FileSystem`，没有让命令层直接读写磁盘块。

## 3. 为什么先拆文件再扩展格式

旧 `src/osfs_filesystem.cpp` 已有约六百行。如果把用户表、双位图、MFD/UFD 解析继续塞进去，文件会同时承担磁盘编解码、空间分配、用户认证、目录管理和文件 I/O，后续 v1632 加入读写位移时维护成本会迅速上升。因此本版先按职责拆分。

`src/osfs_disk_layout.hpp/.cpp` 是内部磁盘格式层。它定义 `SuperBlockDisk`、`InodeDisk`、`DirectoryEntryDisk` 和 `UserRecordDisk` 的定长布局，负责超级块、位图、inode、用户表和目录块的序列化，以及块/inode 分配、释放、用户名/uid 查找、密码哈希和权限判断。四个结构都有 `static_assert` 固定尺寸，避免编译器填充变化悄悄改变镜像格式。

`src/osfs_filesystem.cpp` 只处理格式化、重新打开、超级块视图和用户认证。`src/osfs_directory.cpp` 负责在指定 UFD 中创建、删除、列目录、stat 和 chmod。`src/osfs_file_io.cpp` 负责文件内容块的读取与整体替换写入。`src/osfs_command_processor.cpp` 只处理文本协议和会话状态。拆分后，每个文件都围绕一个稳定问题展开，v1632 可以在文件 I/O 层增加区间读写，而不用重新搅动格式化和用户认证代码。

## 4. 磁盘格式 v2 的实际布局

磁盘仍由固定大小 block 组成。第 0 块是 512 字节超级块；第 1 块是 block bitmap；第 2 块是 inode bitmap；第 3 块是用户表；其后是连续 inode table；再往后才是目录和文件数据块。超级块记录每一段的位置和长度，并维护 `free_block_count`、`free_inode_count`、`user_count`。格式化参数会检查一个位图块是否足以描述全部 block/inode，也会检查剩余空间至少能容纳一个 MFD 块和每个预置用户的 UFD 块。

格式化时预置 root、Alice、Bob。MFD 固定使用 inode 0，它的数据块包含三个目录项，目录项名字是用户名，inode 字段指向对应 UFD inode。每个 UFD inode 再拥有独立的数据块，其目录项才指向普通文件 inode。因此 Alice 创建 `report` 时，目录项只写入 Alice 的 UFD；Bob 的 `DIR` 读取 Bob 的 UFD，自然看不到它。这个隔离来自磁盘寻址，而不是在共享列表上做 owner 过滤。

用户表中的每条 `UserRecordDisk` 固定 64 字节，保存 used 标志、uid、gid、UFD inode、密码哈希和用户名。密码值不会进入 `UserInfo`，也不会出现在 `DIR/STAT` 输出中。格式版本提升到 2，旧 v1 镜像打开时会明确返回需要重新格式化，而不是按新结构误读旧字节。这是一项有意的格式不兼容：课设镜像是可重建学习数据，没有理由用复杂迁移逻辑掩盖结构变化。

inode 从原先只有 owner uid、ctime、mtime，扩展为 uid、gid、ctime、atime、mtime和八个直接块。块位图负责磁盘块占用，inode 位图单独负责 inode 占用，不再通过扫描 `inode.used` 冒充 inode bitmap。每次创建/删除文件或分配/释放目录块后，超级块空闲计数由位图重新计算并落盘。这里选择重新计数而不是只做加减，是因为课设规模很小，逻辑更透明，也能减少异常分支造成计数漂移的机会。

## 5. MFD/UFD 查找和权限流程

命令层登录成功后保存当前用户名和 uid。未登录时，除 `HELP`、`WHOAMI`、`LOGIN`、`QUIT` 外的文件命令都返回 `ERR login required`。登录失败不会改变当前身份；登录成功会清空旧 fd 表，避免一个用户登录后继承前一个用户已经打开的句柄。

普通文件名如 `report` 由 `resolve_file_reference` 按当前 uid 找到用户记录，再取得 UFD inode。限定引用如 `alice/report` 先按前缀找到 Alice 的用户记录，再到 Alice UFD 查目录项。限定引用不是目录遍历功能，而是为了让 root 审查其他 UFD，并让测试能够证明 Bob 即使知道 Alice 的文件名，也会在 inode 权限检查处被拒绝。

权限判断顺序是 root、owner、group、other。root uid 0 总是允许；普通用户先匹配 inode owner uid，再匹配 gid，最后使用 other 位。`0600` 文件只有 owner 可读写，因此 Bob 对 `alice/report` 的读和写都返回 `permission denied`。`DIR alice` 另有目录级规则：只有 root 或 Alice 自己能列 Alice 的 UFD。这样既展示目录隔离，也展示文件权限，两层边界不会混为一谈。

创建文件时先确认当前用户存在且名字合法，然后检查当前 UFD 是否已有同名项。系统从 inode bitmap 分配普通文件 inode，在 UFD 中寻找空目录槽；目录槽不足时才为该 UFD 增加一个直接块。若 inode 或目录块不足，函数在返回前撤销已占用位图，避免一次失败创建泄漏元数据。删除文件则释放该 inode 的所有数据块、清空 inode bitmap 位、清空目录项，并更新 UFD 大小和超级块计数。

## 6. 测试分别保护什么

`test_disk_users_and_two_level_directories` 从格式化后的超级块开始检查：版本必须为 2，用户数必须为 3，MFD 使用 inode 0，初始已占用 inode 数必须等于 MFD 加三个 UFD。它读取用户表公开视图，验证 root/Alice/Bob 拥有不同 UFD inode；随后验证错误密码失败、正确密码成功。Alice 和 Bob 分别创建文件后，两人的目录列表只能出现自己的文件，这条断言保护真正的 UFD 分离。

同一测试把 Alice 文件改为 `0600`，再由 Bob 使用限定引用尝试读写，必须得到 `permission denied`；Bob 尝试列 Alice UFD 也必须失败，而 root 可以列出。这组断言避免实现者只做“DIR 输出过滤”，因为跨 UFD 查找仍然会到达目标 inode 并接受权限判断。关闭并重新打开镜像后，测试再次认证 Alice、读取 Alice 文件并检查两个 UFD，证明用户表、密码哈希、MFD/UFD 目录项和文件数据都来自磁盘，而非进程内状态。

`test_authenticated_command_shell` 从真实文本命令表面验证未登录阻断、错误密码、成功登录、文件创建和句柄使用。切换到 Bob 后，`DIR` 必须为空，`DIR alice` 和 `OPEN alice/report` 必须被拒绝；切换到 root 后，`DIR alice` 与只读打开必须成功。这一测试保护命令解析、会话切换和文件系统 API 的组合，不允许底层正确但 shell 绕过认证。

`test_failed_rewrite_preserves_existing_data` 使用很小的磁盘制造空间不足。系统先写入旧内容和占满多个块的 filler，再尝试把旧文件扩展为无法容纳的大小。写入必须失败，旧内容必须仍可读取。这条测试沿用 v1629 的重要行为，同时适配 v2 元数据开销，确保新增 MFD/UFD 没有破坏写入前空间预检。

CTest 的 `osfs_cli_smoke` 直接启动 `minikv_osfs.exe`，执行错误密码、Alice 登录、创建写入、chmod、Bob 登录、目录隔离、跨用户拒绝、root 审查。它和 C++ 单元测试互补：前者证明真实可执行入口和脚本语法，后者能检查具体磁盘结构和重开持久性。阶段收口时再跑 344 条全量测试，目的是验证顶层 CMake 增加 translation unit 后没有破坏原 KV/WAL/TCP 主线，而不是把全量测试当作每次编辑的内循环。

## 7. 当前仍未完成什么

v1631 已经关闭二级目录和持久化用户认证缺口，但 fd 仍只有文件名和读写标志，`READ` 仍读取整文件，`WRITE` 仍替换整文件。课程 brief 要求在下一个功能版本加入读写位移、指定长度读取和连续写入。现阶段不生成新的 final 报告，因为旧报告中的单根目录图和 `LOGIN guest` 已经过时，若现在重做一次，v1632 后还要再次修改，属于以材料替代功能。

本版也明确不做超过两级的任意目录树、不做一级/二级/三级间接块、不做 fsck 和崩溃日志。单块用户表最多容纳八个用户，密码哈希不适合生产系统，这些都是教学边界而不是隐藏缺陷。最终需求-证据矩阵会把完整项、部分项和明确豁免分开，不能再用“命令存在”代替“机制实现”。

一句话总结：v1631 让 mini-kv 的独立 OSFS 从共享根目录演示器变成了磁盘上确有 MFD、UFD、用户记录和认证边界的二级文件系统，为后续 Node/Java/mini-kv 松耦合协作保留原主线不变，同时使课设答辩能够沿真实字节布局解释每一步。
