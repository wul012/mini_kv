# v1635 OSFS 只读 FSCK 与落盘用户管理讲解

v1635 的目标，是把前几版 OSFS 课设层里最容易被追问的两个缺口补成可运行、可测试、可解释的机制：第一，文件系统不只是能写入和读取，还要能自己检查超级块、位图、inode、目录和用户表是否一致；第二，用户不应该只停留在格式化时写死的 `root/alice/bob`，而应该能通过命令创建新用户并把密码修改真正落到磁盘镜像里。本版的角色仍然是课程设计中的独立 C++ 文件系统模块，不是 mini-kv 主线 KV 存储的新功能，也不是生产恢复工具。它不接入 WAL、snapshot、restore、TCP/RESP，不启动服务，不读取 Node/Java endpoint，不生成跨项目 router，不授予任何执行型修复权限。换句话说，v1635 做的是 OSFS 自身的“可诊断”和“可管理”，而不是把教学文件系统包装成生产控制面。

这版也不是通过报告或截图把功能说厚。真正新增的入口在 `FileSystem::check_consistency()`、`FileSystem::add_user()`、`FileSystem::change_password()` 和 `CommandProcessor::execute()` 的三个新命令分支里。文档、截图和讲解只是把这些机制说明白，不能替代源码和测试。用户前面已经明确提醒过，不要把低工作量版本急着包装成最终交付；所以 v1635 的验收标准是：代码里有机制，测试里有破坏性场景，真实 CLI 里能演示，归档里能看出输入输出，边界里写清楚什么没有做。这个版本先把课设核心加厚，最终 DOCX/PDF 和需求矩阵刷新留给 v1636。

## 入口与响应结构

命令入口仍然是 `src/osfs_command_processor.cpp`。原来的命令处理器先解析 `HELP`、`QUIT`、`WHOAMI`、`LOGIN`，然后检查是否已经登录；v1635 新增的 `FSCK`、`USERADD`、`PASSWD` 都放在登录检查之后。这一点很关键：`FSCK` 虽然只读，但它能暴露磁盘内部结构；`USERADD` 和 `PASSWD` 会修改用户表，更不能绕过认证。未登录时执行 `FSCK` 会得到 `ERR login required`，这说明新功能没有在命令层开一条旁路。

`FSCK` 的响应由 `format_fsck_report()` 统一格式化。第一行是总状态，例如 `FSCK OK checks=6` 或 `FSCK ERROR checks=6`；后续每一行对应一个检查项，前缀是 `OK` 或 `ERROR`，再跟检查名和细节。这样的结构比只返回一段自然语言更适合测试，也更适合课堂演示。老师看第一行就知道总状态，继续往下看能知道是 block bitmap、inode bitmap、user table 还是 directories 出了问题。这个响应结构目前不是 JSON，因为 OSFS 课程 shell 的用户是人；但它仍然是稳定、可断言、可截图的半结构化输出。

`USERADD user password` 和 `PASSWD [user] password` 的响应更短。成功时分别返回 `OK useradd <user>` 和 `OK password changed <user>`；失败时沿用原命令层风格，把底层错误包装成 `ERR ...`。`PASSWD` 支持两种输入：普通用户执行 `PASSWD newpass` 表示修改自己的密码，root 执行 `PASSWD carol newpass` 表示重置指定用户密码。这里没有引入旧密码校验，是一个教学简化：登录会话已经证明当前操作者身份，普通用户不能指定别人，root 可以管理所有用户。这个边界会在后续交付矩阵里写清楚，不把它说成生产级账号系统。

## FSCK 的核心流程

`src/osfs_fsck.cpp` 是本版新增的只读检查器。它的第一步是读取 superblock，如果 magic、版本或几何信息不合法，直接生成 `superblock` 失败项并返回。原因是后续检查都依赖 block count、inode count、data start、MFD inode 等布局字段；如果这些字段本身不可信，继续读位图和 inode 表只会产生二次噪声。superblock 通过后，检查器读取 block bitmap、inode bitmap、inode table 和 user table，然后开始构造“期望状态”。

期望状态不是复制磁盘上的位图，而是根据可达结构重新计算出来。检查器先把 `[0, data_start_block)` 的元数据块标记为已使用，因为 superblock、block bitmap、inode bitmap、user table 和 inode table 本来就属于元数据区域。随后遍历每个 used inode：目录 inode 只能使用直接块，不能使用间接块；文件 inode 则通过 `collect_file_blocks()` 收集八个直接块和一级间接块里的数据块。每遇到一个块，都会调用 `mark_expected_block()` 做三件事：检查块号是否落在 data region；检查这个块是否已经被其它 inode 或目录占用；如果合法且没有重复，就把它标进期望 block bitmap，并记录 owner 字符串。这样当两个 inode 指向同一个物理块，或者某个 inode 指向越界块，FSCK 不会只说“坏了”，而会给出哪个 inode 的哪个 direct/indirect 槽位有问题。

文件 size 检查也不是顺手做的装饰项。OSFS v1634 引入单级间接块后，文件的逻辑块数由 `ceil(size / block_size)` 决定。如果 inode 的 size 需要 9 个数据块，但实际只引用了 8 个，读到边界后就会缺块；如果 size 只需要 3 个块却还保留一个间接块，那说明释放路径可能漏掉了间接块本身。本版把这些问题放进 `file sizes` 检查项，保护 v1634 的直接块到间接块扩展不会在后续 truncate/delete 中退化。

位图检查分两层。第一层是 inode bitmap：每个 `used != 0` 的 inode 都应该在 inode bitmap 中被标记，反过来未使用 inode 不应在位图中占用。第二层是 block bitmap：重新计算出来的 expected block bitmap 要和磁盘上的 block bitmap 完全一致。测试中的坏盘面演示就是故意把文件数据块 20 的 block bitmap 位从 used 改成 free，文件 inode 仍然引用这个块，所以 expected bitmap 认为它应该使用，磁盘 bitmap 却说它空闲，于是 `FSCK` 报出 `block bitmap mismatch at block 20`。这能证明 FSCK 不是只在内存结构里绕一圈，而是真正对比了磁盘状态。

用户和目录检查是 OSFS 课设最有价值的一部分。`user table` 检查会统计 used user 数量，和 superblock 的 `user_count` 对比；还会检查 username 是否合法、uid 是否重复、UFD inode 是否是一个已使用目录 inode、UFD inode 的 owner uid 是否和用户表 uid 对齐。`directories` 检查会读取 MFD，确认 MFD 中的用户名能指向有效 UFD；再读取每个 UFD，确认目录项名称合法、指向的是已使用文件 inode，并且文件 inode 的 owner uid 与 UFD 所属用户一致。这样一来，`USERADD` 不只是写一条 user record 就算完成，它必须同时维护 user table、MFD、UFD inode、UFD data block、inode bitmap、block bitmap 和 superblock counter，否则 FSCK 会在后续检查里抓到不一致。

本版刻意不做自动修复，这不是能力不足的遮掩，而是边界选择。一个真正可靠的修复器需要先定义可恢复策略：如果 block bitmap 说空闲但 inode 引用该块，是优先相信 inode、还是优先相信 bitmap；如果两个 inode 指向同一块，是复制数据、删除其中一个引用、还是把整幅镜像标成需要人工处理；如果 MFD 有用户条目但 user table 没有对应记录，是补用户、删目录，还是保留孤儿目录。这些决策都可能写磁盘，一旦实现得粗糙，就会把“发现错误”变成“制造二次破坏”。对课程设计来说，v1635 更应该把检查机理做透明：哪些结构被扫描，哪些计数被重算，哪个块或 inode 失配，为什么它是错误。这样答辩时老师能看到的是文件系统一致性的判断过程，而不是一个黑盒 `repair` 命令。

只读定位也保护了 mini-kv 的项目边界。mini-kv 主线长期强调 no execution、no restore/load/compact execution、no hidden write expansion；OSFS 虽然是独立课设层，但仍然在同一个仓库里。如果 `FSCK` 默认修复，就很容易让一个“检查命令”偷偷变成写命令，后续文档再说 read-only 就站不住。现在 `check_consistency()` 是 `const` 方法，命令输出只描述问题，坏盘面 smoke 也只翻转测试镜像后读取诊断结果，不让 FSCK 自己改回去。这个约束让它更适合作为教学证据：读者可以把磁盘结构、位图计算和错误报告逐步对应起来，也可以放心把它放进演示脚本，不担心检查本身改变被检查对象。

## 用户管理的写入路径

`src/osfs_user_admin.cpp` 是本版第二个核心新增文件。它没有塞进 `osfs_filesystem.cpp` 或 `osfs_directory.cpp`，是为了避免课设层再次长成巨型文件。这个文件只负责用户管理相关写入：创建 UFD 目录 inode、寻找空 slot、计算目录 size、生成下一个 uid，以及执行 `add_user()` 和 `change_password()`。拆成单独 translation unit 的好处很直接：以后如果要加 `USERDEL` 或更细的权限策略，不会污染普通文件读写路径。

`add_user()` 的第一个边界是 root-only。`actor_uid != 0` 立即返回 `USERADD requires root`，不读取和分配任何数据块。随后它校验用户名合法、密码非空、用户名不重复、用户表还有空位。这里的失败顺序有意从便宜检查到昂贵检查：能在 user table 阶段失败，就不要先分配 inode 和 block。等确认可以创建用户后，函数读取 inode 表、block bitmap、inode bitmap 和 MFD entries，寻找 MFD 空槽。如果 MFD 已满但还有 direct block 槽位，则会为 MFD 增长分配一个新目录块；如果 MFD 自身已经没有 direct 槽位，就回滚已经分配的 UFD inode/block bitmap 状态并返回 `MFD directory is full`。当前课程要求是二级目录，不支持任意深度目录扩展，所以 MFD 仍然不使用间接块。

成功路径会分配一个 UFD inode 和一个 UFD 数据块。`initialize_directory_inode()` 把这个 inode 初始化为 type=directory、mode=默认目录权限、owner_uid=新 uid、owner_gid=100，并把 direct[0] 指向 UFD 数据块。然后 `clear_block()` 清空 UFD 数据块，避免旧镜像内容被解释成目录项。用户表记录写入 username、uid、gid、UFD inode 和教学密码哈希；MFD entry 写入用户名和 UFD inode；MFD size 按实际 used entry 数量更新；最后重新计算 `free_block_count`、`free_inode_count` 和 `user_count`，再按目录、用户表、inode 表、位图、superblock 的顺序写回磁盘。这个顺序不是事务系统，但它让每个落盘结构都有明确职责，并由 FSCK 做后续一致性验证。

`change_password()` 的逻辑更窄。它要求密码非空，要求 actor uid 能在 user table 中找到，要求目标用户存在；如果 actor 不是 root 且目标用户名不是自己，则返回 `permission denied`。成功时只更新目标记录的 `password_hash` 并写回 user table，不碰 inode、位图、目录或 superblock。这种最小写入面很重要：改密码不应顺手影响文件数据，也不应改变目录结构。测试会先让 root 把 Carol 密码从 `carol123` 改成 `carol456`，再验证旧密码失败、新密码成功；Carol 登录后自助改成 `carol789`，重开镜像后旧密码再次失败、新密码仍然成功，证明它不是内存态会话变量。

## 测试保护什么

`tests/osfs_tests.cpp` 本版新增两组关键测试。`test_fsck_reports_ok_and_detects_corruption()` 先格式化 192 块镜像，用 Alice 创建一个跨直接块到间接块的大文件，确认 clean FSCK 为 OK；然后通过 `set_block_bitmap_used()` 直接打开 `.img`，把文件首个物理块在 block bitmap 中置为 free。重新打开文件系统后，`check_consistency()` 必须返回 `ok=false`，且失败项名称是 `block bitmap`，detail 中包含 `block bitmap mismatch at block`。随后同一坏镜像还会通过命令处理器登录 root 并执行 `FSCK`，确保 API 和 CLI 两层都能呈现错误。

`test_useradd_passwd_and_fsck_command()` 覆盖用户管理路径。它先证明未登录执行 `FSCK` 被拒绝，Alice 执行 `USERADD` 被拒绝；root 成功创建 Carol 后，重复创建同名用户失败；root 重置 Carol 密码后，FSCK 仍然 OK。然后测试重开镜像，确认 Carol 旧密码失效、新密码有效。Carol 创建 `diary` 并写入内容后自助改密，再次重开验证新密码持久。Bob 登录后试图修改 Carol 密码、查看 Carol 目录都被拒绝；root 则可以看到 Carol 的 `diary`。最后测试还通过 API 连续加用户，直到固定 user table 满，确认失败错误是 `user table is full`，并且失败后 FSCK 仍然 OK。这组测试保护的不是单个 happy path，而是权限、持久化、容量边界和失败后不破坏结构。

原有测试也继续有用。`test_disk_users_and_two_level_directories()` 保护 MFD/UFD 的基本隔离和重开持久；`test_authenticated_command_shell()` 保护登录、创建、读写、chmod、跨用户访问；`test_descriptor_offsets_and_range_io()` 保护 fd offset、range I/O、append 和单级间接容量上限；`test_failed_rewrite_preserves_existing_data()` 保护空间不足时旧数据不被清空；`test_indirect_blocks_persist_and_release()` 保护大文件删除后释放直接块、间接块和间接数据块。v1635 把 FSCK 接到这些已有语义之上，相当于给前面几版的磁盘结构加了一个自我检查面。

## 归档与边界

本版归档在 `f/1635/图片/` 和 `f/1635/解释/说明.md`。截图优先尝试 Chrome DevTools MCP，其中构建、focused tests 和坏盘面 FSCK 可以直接截图；全量 CTest 和用户管理长会话因为首屏无法同时容纳最终断言，改用同一份 UTF-8 日志的本地文本渲染兜底。这个偏差写在说明文档里，不把工具兜底伪装成原生截图。临时脚本、日志、HTML 和 `.img` 镜像都放在 `cmake-build-v1635/_evidence-v1635/`，最终 cleanup 会删除，只保留长期需要的 PNG 和说明。

从四项目松耦合角度看，v1635 没有修改 Node、Java 或 aiproj，也没有修改 mini-kv 的 SHARDJSON/runtime receipt 证据链。它的价值在于让 mini-kv 的 OSFS 课设分支更扎实：用户、目录、位图、inode、间接块和权限不只是能跑通，还能通过 FSCK 被反向检查；用户管理不只是命令名，而是落盘结构闭环。对于后续 Node/Java/mini-kv 松耦合融合，这版提供的价值是边界清楚的本地证据：mini-kv 可以在不扩大 router/write/WAL/execution 权限的前提下，把一个独立教学模块做得更可审、可演示、可维护。
