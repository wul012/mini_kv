# mini-kv v1636：OSFS 课程设计最终交付包刷新讲解

本版目标是把 v1634 和 v1635 已经落地的 OSFS 实质机制收束成可以给教师、同学或复核者阅读、运行和追问的最终交付包。它不是一个新的执行入口，不是把 mini-kv 的 KV/WAL/TCP 主线包装成课程设计，也不是用漂亮报告掩盖功能缺口。v1636 的角色更接近“交付证据整理层”：把一级间接块、只读 FSCK、USERADD/PASSWD、文件描述符位移、MFD/UFD 二级目录、权限拒绝和真实构建测试结果放到同一套可复现材料里，让读者可以从报告、矩阵、脚本、transcript、源码和 CTest 之间互相核对。

这一版为什么值得单独收口，原因在于前几版的 OSFS 已经不再是一个能跑几条命令的小 demo。v1631 把单根目录草稿升级为磁盘格式 v2，补出了持久化用户表、MFD、每用户 UFD、双位图和 uid/gid；v1632 给打开文件表补出 read/write offset，让 `READ`、`WRITE`、`SEEK`、`TELL` 不再是整文件字符串操作；v1633 首次生成正式课程包，但当时还主动披露“无间接块、无 fsck、无动态用户管理”；v1634 把 inode 提升到磁盘格式 v3，增加一级间接块；v1635 又补上只读一致性检查和 root-only 用户管理。到 v1636 时，如果还让提交目录停在 v1633，就会出现一个危险的割裂：代码已经有了新机制，但正式报告、演示步骤和需求矩阵仍然按旧状态叙述，答辩者很容易在“当前代码”和“交付材料”之间说不清。因此本版的价值不是新增命令，而是把交付口径修正到当前真实工程状态。

## 入口命令与本版输入输出

本版仍然以 `minikv_osfs` 为唯一课堂演示入口。入口代码在 `src/osfs_main.cpp`，它只解析 `--disk`、`--format`、`--blocks`、`--block-size` 和 `--script`，然后打开或格式化虚拟磁盘，再把脚本每一行交给 `minikv::osfs::CommandProcessor`。也就是说，本版所有“能不能交”的证据都围绕一个简单输入输出展开：输入是一个 `.img` 镜像路径和 `osfs-demo-script.txt`，输出是命令响应、磁盘镜像状态变化、`osfs-demo-transcript.txt`、报告 PDF/DOCX、需求矩阵和 CTest 结果。

新交付目录是 `课程设计交付/v1636-osfs-final/`。其中 `osfs-demo-script.txt` 是真正可执行的脚本，不是报告里的伪代码；`osfs-demo-transcript.txt` 是本机构建出的 `cmake-build-v1636/minikv_osfs.exe` 在新镜像上真实运行得到的 UTF-8 输出，专门修掉了 v1633 transcript 曾经以 UTF-16/空字节形式保存的问题。脚本里有一条 4291 字节的 `WRITE 5 ...`，它故意把文件写过八个 512 字节直接块的 4096 字节边界；之后执行 `SEEK 6 4088` 和 `READ 6 64`，输出 `DIRECT8|INDIRECT-BEGIN|...`。这个片段是 v1636 最重要的容量证据：读者不需要相信文字描述，只要看到 read offset 和输出，就能判断数据确实跨过 direct[7] 尾部进入了 indirect block 指向的数据块。

## 报告材料的结构

`操作系统课程设计报告-OSFS-v1636.docx` 和同名 PDF 不是从旧 v1633 文件直接改几个字生成的。它重新组织为九页：封面、摘要、课题目标、总体架构、磁盘布局、MFD/UFD、fd 位移、一级间接块、FSCK/用户管理、真实输出摘录、测试矩阵和边界说明。报告里有四张图：总体结构图、磁盘格式 v3 图、一级间接块寻址图、FSCK 与用户管理职责图。图的作用不是装饰，而是把代码里的结构显式化：`CommandProcessor` 维护会话和 fd 表，`FileSystem` 暴露格式化、认证、读写、fsck 和用户管理 API，`VirtualDisk` 只做 block 级 `.img` 读写；磁盘中从 Block 0 到数据区依次是 superblock、block bitmap、inode bitmap、user table、inode table、MFD/UFD/普通文件数据和间接块。

这一版的文档写法刻意区分“机制已完成”和“教学简化”。例如报告会明确说当前没有 EXT2 group descriptor，仍然是单教学 block group；也会说明 `FSCK` 是只读检查，不是生产级自动修复器；密码哈希只为避免明文落盘，不等同 PBKDF2/bcrypt/argon2 这类生产密码派生算法。这种披露很重要，因为课程答辩里最危险的不是承认边界，而是把没有实现的内容说成实现了。v1636 的策略是让矩阵一眼看出哪些是完整、哪些是部分/教学简化、哪些明确不做。

## 需求矩阵如何读

`需求-证据矩阵.md` 是本版的中心索引。它把课程要求拆成二进制文件模拟磁盘、超级块、block bitmap、inode bitmap、inode table、用户信息、login、MFD/UFD、dir、create/delete、open/close、read/write、fd 位移、权限保护、owner/group、时间字段、间接块、fsck、用户管理，以及 EXT2 group descriptor 和更深目录等边界项。每一行都包含三个问题：实现在哪里，证据是什么，状态算不算完整。

这里最值得看的是三行状态变化。第一是“间接块”，实现位置指向 `InodeDisk::indirect_block` 和逻辑块计数/映射 helper，证据不是“代码里多了一个字段”，而是 4291 字节文件跨越 `DIRECT8|INDIRECT-BEGIN|` 的真实 transcript，以及超上限拒绝、删除释放间接块的测试。第二是“一致性检查 fsck”，实现位置指向 `src/osfs_fsck.cpp` 和 `FSCK` 命令，证据同时包含干净镜像的 `FSCK OK` 与 v1635 测试里故意翻转 block bitmap 后得到的 `FSCK ERROR`。第三是“用户管理”，实现位置指向 `src/osfs_user_admin.cpp`、`USERADD` 和 `PASSWD`，证据包含非 root 被拒、root 创建 Carol、改密后 Carol 用新密码登录以及最终 FSCK 仍然 OK。

矩阵后面的对抗性自审也不是形式化尾巴。第一问专门回答“是不是仍然只有一个根目录，只按 owner 过滤 DIR 输出”。答案是否定的，因为用户表和 MFD 都保存 UFD inode，Alice、Bob、Carol 各自拥有不同目录 inode 和目录数据块；创建文件时先按当前 uid 定位 UFD，再把目录项写入对应 UFD。第二问回答“一级间接块有没有真实参与 I/O”。答案用 4291 字节 transcript 和 4096 字节直接块边界说明。第三问回答“FSCK 是不是只输出 OK 的壳”。答案指向 `src/osfs_fsck.cpp` 对 superblock、双位图、inode、MFD/UFD、用户表和可达块的重算，以及破坏 bitmap 的负向测试。

## 与代码模块的对应关系

虽然 v1636 主要提交交付材料，但它解释的代码模块很具体。`src/osfs_disk_layout.hpp` 定义磁盘格式 v3：`SuperBlockDisk` 固定 512 字节，`InodeDisk` 固定 96 字节，包含 `direct[max_direct_blocks]` 和 `indirect_block`，`UserRecordDisk` 固定 64 字节保存 username、uid、gid、UFD inode 和 password hash。`src/osfs_filesystem.cpp` 负责 format/open/users/authenticate；格式化时预置 root、alice、bob，为 MFD 和每个 UFD 分配 inode 与数据块。`src/osfs_directory.cpp` 负责 UFD 内的 create/delete/dir/stat/chmod；`src/osfs_file_io.cpp` 负责整体写、truncate、range read/write 和 indirect block 映射；`src/osfs_fsck.cpp` 负责只读一致性检查；`src/osfs_user_admin.cpp` 负责 root-only 新用户和 root/self 密码修改；`src/osfs_command_processor.cpp` 把这些文件系统 API 映射成课堂可见的命令。

这里的职责边界值得强调。`FSCK` 调用 `FileSystem::check_consistency()`，只读取并汇总检查项，不调用 `allocate_block`、`release_block`、`write_inodes` 或任何修复路径。`USERADD` 则是明确的写路径：它检查 actor 是否 root，检查用户名和密码，找 user table 空槽，分配 UFD inode 与 UFD 数据块，必要时扩展 MFD，更新用户表、MFD 条目、inode/block bitmap 和 superblock 计数。`PASSWD` 的写面更窄，只更新目标用户的 password hash，并且普通用户只能改自己。报告图 4 把这两个方向分开，是为了让读者明白“诊断”和“管理”不是同一个权限面。

## 真实演示链路

v1636 的演示脚本按答辩顺序组织。开头 `WHOAMI` 和未登录 `CREATE` 证明认证门禁；`LOGIN alice wrong` 和 `LOGIN alice alice123` 证明密码验证；Alice 创建 `report` 后两次写入，`TELL 3` 显示 `write_offset=13`，随后 `READ 4 6`、`READ 4 1`、`READ 4` 分段读出 `course`、`-`、`design`，证明 fd 的读位移会推进。`CHMOD report 0600` 后，Bob 的 `OPEN alice/report r` 被拒绝，说明权限不是报告文字。

中段是本版加厚点。Alice 创建 `bigfile` 并写入 4291 字节，`TELL 5` 显示 `write_offset=4291`，`SEEK 6 4088` 后 `READ 6 64` 读到 `DIRECT8|INDIRECT-BEGIN|...`。随后第一次 `FSCK` 输出 `FSCK OK checks=6`，并列出 superblock、inode bitmap、block bitmap、file sizes、user table、directories 六类 OK。后段 Bob 执行 `USERADD carol carol123` 被拒，root 执行同一命令成功，再 `PASSWD carol carol456`，Carol 用新密码登录并创建自己的 `carol-note`。最后再次 `FSCK OK`，说明用户管理后磁盘结构仍然一致。

## 测试与渲染证明保护什么

本版没有新增 C++ 源文件，但仍然需要重新构建和跑测试，因为交付包的 transcript 依赖当前二进制。构建使用 CLion 捆绑 CMake/Ninja/MinGW，先生成 `minikv_osfs` 和 `minikv_osfs_tests`，再用新脚本真实运行。后续还要跑 focused OSFS 测试和全量 CTest。focused 测试保护 OSFS 的结构语义：格式化、重开持久化、目录隔离、权限拒绝、fd 位移、range I/O、一级间接块、删除释放、FSCK 错误检测、USERADD/PASSWD 权限和持久化。全量 CTest 则证明独立 OSFS 改动没有破坏原 mini-kv 的 KV、WAL、snapshot、TCP/RESP、read-only evidence 和 archive quality gates。

文档本身也做了渲染验证。DOCX 转 PDF 时，LibreOffice 在中文路径下对 embedded PNG 报了 `libpng Write Error`，所以本版用 ASCII 临时路径转换，再把 PDF 复制回中文最终目录；之后用 Poppler 原生 `pdftoppm.exe` 渲染出九页 PNG，并查看 contact sheet 和第 8、9 页矩阵。第一个渲染版发现矩阵有一行跨页拆开，虽然没有丢字，但作为正式报告不够好，于是临时生成器给表格行加了 `w:cantSplit`，重生成后第 8 页和第 9 页都保持整行分页。这个小修正体现了本版不是“能打开就交”，而是对文档版面也做了工程式验收。

## 边界字段和跨项目意义

OSFS 本身不是 Node/Java 的运行证据路由，但 mini-kv 这个仓库长期有 read-only、no-router、no-write、no-WAL、no-network、no-execution 等边界字段治理习惯。v1636 仍然遵守同一精神：它不增加 Node 或 Java 可调用的新路由，不让课程报告成为执行授权，不启动 TCP 服务，不读取生产凭据，不触发 restore/load/compact，不把 FSCK 做成会修改磁盘的自动修复入口。交付包只是让人类读者和课堂演示能理解当前 C++ 文件系统层的输入输出。

对后续松耦合融合的价值在于：Node/Java 即使不直接运行 OSFS，也能把 v1636 当成一个清晰的 C++ 侧能力边界样例。它说明 mini-kv 可以在不污染主线 KV/WAL/TCP 的前提下承载一个独立课程设计模块；也说明每个功能点都可以通过“源码位置 + 测试 + 真实运行输出 + 报告矩阵”形成证据链。以后如果要继续做 C++ 教学子系统、read-only 审计包或跨项目理解材料，v1636 的交付方式比早期“做完一版就急着交付”的方式更稳：先补机制，再跑真实演示，再写矩阵，再渲染检查，最后才说它能交。

## 为什么这是交付工程，而不是文档修饰

如果只看 diff，v1636 很容易被误判成“文档版”。这种判断会漏掉一个关键事实：课程设计的评分并不只看源码是否存在，还看提交材料能否把机制、运行方式和限制讲明白。一个功能已经实现但报告仍写旧边界，等于在答辩时主动制造矛盾；一个 transcript 真实存在但编码是 UTF-16 空字节，等于在其他机器上打开时给复核者制造噪声；一个矩阵把“fsck”写成完成但没有负向证据，等于把最容易被追问的地方留给口头解释。v1636 做的是把这些断点补成连续链路，所以它属于交付工程。

这一版也刻意没有继续新增 C++ 命令。原因很简单：v1634 和 v1635 已经补上了最影响课设厚度的三项机制，再往里塞任意深度目录、自动修复 fsck 或崩溃日志，会把题目从“Linux 二级文件系统”拉向另一个更大系统，反而削弱 MFD/UFD 的叙事。顶级工程师式的推进不等于无限加功能，而是判断此刻最短板在哪里。此刻的最短板不是命令数量，而是交付口径、报告可信度、渲染质量和需求证据矩阵是否与代码同步。把这些做好后，教师看到的不是“又写了几百行”，而是一套能运行、能解释、能追溯、能承认边界的完整作品。

还有一个细节值得留下：文档渲染本身也被当成测试对象。第一次 PDF 页面检查发现矩阵跨页拆行，这种问题不会让 CTest 失败，却会让提交物显得粗糙。修复它需要理解 Word 表格的分页属性，而不是简单缩小字号。这里选择 `w:cantSplit` 的思路，是让表格行作为一个语义单元移动到下一页，保持读者理解连续。这个处理和 C++ 侧“不要把目录项当过滤结果”“不要把 FSCK 当口号”是同一个原则：能用结构表达的地方，就不要靠读者脑补。

一句话总结：v1636 把 OSFS 从“代码已经加厚但交付材料滞后”的状态，推进到“机制、脚本、transcript、报告、矩阵、渲染和边界自审一致”的最终课程交付闭环，同时不扩大 mini-kv 对 Node/Java 或 KV/WAL/TCP 主线的执行权限。
