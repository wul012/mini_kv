# v1634 OSFS 一级间接块容量扩展代码讲解

## 本版目标、角色和不做事项

v1634 的目标很明确：把 OSFS 课设层从“每个文件最多八个直接块”的教学上限，推进到“八个直接块加一个一级间接块”的文件寻址模型。它不是为了追求代码行数，也不是为了把 mini-kv 主线改造成通用文件系统，而是补上操作系统课程设计中一个很容易被老师追问的机制点：文件超过直接块之后，文件系统如何继续寻址、如何记录额外数据块、如何在删除或截断时把这些块完整释放，以及如何证明跨边界读写不是靠字符串拼接凑出来的。

本版的角色仍然只属于 `minikv_osfs` 这条独立课设支线。它不改变 KV 命令处理器，不改变 WAL、snapshot、RESP/TCP 服务端，也不新增 Node/Java 可以调用的路由或执行入口。对四项目松耦合链路来说，它只是让 mini-kv 仓库内部的课程设计文件系统更像一个真正的磁盘结构，而不是把 mini-kv 变成上游订单系统的写入 authority。换句话说，本版提升的是 OSFS 自己的机制深度，不扩大任何跨项目执行权限。

本版也有清晰边界。它没有做二级或三级间接块，没有做 EXT2 block group descriptor，没有做 fsck，也没有改用户管理命令；这些留给 v1635/v1636。这样拆分是有必要的：一级间接块本身已经触碰磁盘格式、inode 布局、空间分配、range I/O、删除释放和持久化测试，如果同时塞入 fsck 与 useradd，失败时很难定位到底是寻址错、bitmap 错、目录错，还是用户表错。

## 入口命令和可观察行为

用户能直接观察到的入口仍然是 `minikv_osfs`。CLI 命令没有增加新语法，仍然通过 `LOGIN`、`CREATE`、`OPEN`、`WRITE`、`SEEK`、`READ`、`DELETE` 这些已有命令展示文件系统行为。变化发生在这些命令背后的存储能力：过去写入超过八个 512 字节数据块时会被直接拒绝，现在文件可以跨过 4096 字节边界继续写入，因为第九个及后续数据块会通过 inode 上的 `indirect_block` 找到。

归档 smoke 的脚本用 Alice 登录，创建 `large`，写入 4204 字节，然后从 offset 4090 读取 64 字节。这个位置故意卡在第八个直接块末尾附近：4090 到 4095 仍属于直接块范围，4096 之后必须进入一级间接块。如果返回内容能连续看到 `D8|INDIRECT-BEGIN|`，就说明读取路径真的跨过了直接块到间接块的边界，没有在块边界处截断，也没有把新数据写丢。随后删除文件并看到目录为空，证明目录项清理和数据释放路径至少在用户可见层面闭合。

这类 smoke 的价值在于它能让答辩场景更透明。老师不需要理解所有 C++ 细节，只要看到“写入大于八个块、定位到边界、读回边界标记、删除后目录为空”，就能直观看到文件寻址机制已经从固定 direct block 数组推进到 indirect addressing。

## 磁盘结构变化

核心结构在 `src/osfs_disk_layout.hpp` 和 `src/osfs_disk_layout.cpp`。本版把磁盘格式版本从 2 升到 3，同时保持 `InodeDisk` 总大小仍为 96 字节。这个细节很重要：如果只是粗暴往 inode 里塞字段，inode table 的布局、后续 inode 偏移、旧测试和格式化逻辑都会受到不必要的扩大影响。本版通过把 `reserved` 从 20 字节缩到 16 字节，为 `std::uint32_t indirect_block` 腾出空间，既新增了一级间接块指针，又没有改变 inode 固定记录大小。

`indirect_block` 的含义是：当文件需要超过八个直接块时，inode 先分配一个普通数据块作为“索引块”，这个索引块不存用户文件内容，而是按 `uint32_t` 数组存放更多数据块号。默认 block size 为 512 字节，所以一个间接块能保存 128 个数据块号。文件总容量就从 8 个块变成 `8 + block_size / 4` 个块。这里的 `/ 4` 来自每个块号 4 字节，不是拍脑袋的常量。

本版新增的 helper 把这套规则集中在 disk-layout 层：`indirect_entries_per_block` 计算间接块可容纳多少项，`max_file_blocks` 给出文件最大数据块数，`inode_data_block_count` 根据文件 size 计算已需要的数据块数量，`inode_data_blocks` 收集当前 inode 引用的所有数据块，`inode_data_block_at` 负责把逻辑块序号映射成实际物理块号，`ensure_inode_data_blocks` 负责按需分配直接块、间接块和间接数据块，`release_inode_storage` 统一释放文件数据块与间接块本身。

把 helper 做集中，而不是在写入、读取、删除三个地方各自写一套逻辑，是本版维护价值最大的地方。间接块最容易出错的不是“能不能写第九个块”，而是不同路径对第九个块的理解不一致：写入认为它在间接块里，读取还只看 direct array，删除只释放 direct array，truncate 忘了释放 indirect block 自身。集中 helper 让这些路径共享同一套解释。

## 写入流程：先预检，再分配，再落盘

`src/osfs_file_io.cpp` 是本版最核心的执行路径。无论是整文件写入 `write_file`，还是 v1632 引入的区间写入 `write_file_range`，都不再自己循环 `direct_blocks`。它们先根据目标写入范围计算需要多少个逻辑数据块，再调用 `ensure_inode_data_blocks`。这个 helper 会先检查是否超过 `max_file_blocks`，再检查可用空间是否足够，最后才真正修改 bitmap、superblock free counter 和 inode 指针。

这里的顺序很关键。课程设计里最怕出现“写到一半才发现容量不够，旧内容也被破坏”的情况。本版的测试继续保护超上限写入拒绝，说明错误路径不会先分配一部分块再返回失败。尤其是进入间接块之后，除了用户数据块本身，还可能需要额外分配一个间接块，所以预检不能只看“还差几个数据块”，还要把索引块自身的成本算进去。v1634 把这个成本放在 helper 里统一处理，避免调用方忘记。

真正写数据时，代码按逻辑块序号计算 block offset，通过 `inode_data_block_at` 拿物理块号，再读出旧块内容、覆盖局部片段、写回磁盘。这个模型延续 v1632 的 range I/O 语义：写入不是把整个文件读成字符串再重写，而是按偏移定位到相关块。越过 4096 字节边界时，逻辑块 0 到 7 会落到 `direct_blocks`，逻辑块 8 开始会从 `indirect_block` 指向的块号数组里取数据块号。

## 读取、截断和删除流程

读取路径同样不能只看 direct array。`read_file_range` 根据文件 size 和 requested length 计算可读范围，然后逐块调用 `inode_data_block_at`。如果读取发生在直接块与间接块边界，调用方并不需要知道自己跨了哪类块；它只关心逻辑块序号，映射细节由 layout helper 处理。归档 smoke 中的 `SEEK 4 4090` 后 `READ 4 64` 正是验证这一点。

截断路径在本版也被收紧。`truncate_file` 不再只把八个直接块清零，而是调用 `release_inode_storage`。这个 helper 会释放当前 inode 引用的所有 direct 数据块、间接块里列出的数据块，以及 `indirect_block` 自身。然后把 inode size 归零并更新时间戳。这样 `OPEN name w` 的语义仍然成立：它不仅从用户视角清空内容，也从磁盘空间视角归还旧存储。

删除路径在 `src/osfs_directory.cpp`。删除文件时，目录项清理和 inode 释放之外，最容易漏掉的是大文件的间接数据块。如果只释放 `direct_blocks`，free block counter 会永久偏小，bitmap 也会泄漏；继续运行一段时间后，文件系统会误以为空间不足。本版把 `delete_file` 改成调用 `release_inode_storage`，测试中明确记录删除大文件后 free block count 恢复到写入前，包含“十个数据块加一个间接块”这类成本。

## 测试保护的行为

`tests/osfs_tests.cpp` 的改动不是单纯改期望值。第一类测试把旧的“八个直接块后拒绝”改成“第九块可以写，新上限之后仍拒绝”。它证明 v1634 不是取消容量限制，而是把限制升级为可解释的 indirect-block limit。这样的断言比“随便写大文件成功”更稳，因为文件系统必须知道自己的最大容量在哪里。

第二类测试是 `test_indirect_blocks_persist_and_release`。它格式化一个足够大的镜像，写入超过九个块的大文件，检查 free block count 的下降量，读回 direct-to-indirect 边界附近的标记，再关闭并重新打开镜像，继续确认内容还在。最后删除文件，检查 free block count 回到写入前。这个测试同时保护四件事：分配成本正确、跨边界读写正确、磁盘持久化正确、释放路径正确。

第三类证据是真实 CLI smoke 和全量 CTest。CLI smoke 证明用户命令层没有因为底层存储模型变化而坏掉；全量 344/344 证明 OSFS 的公共头文件和静态库链接变化没有破坏原 mini-kv 主线的命令、WAL、snapshot、TCP/RESP、只读 JSON、治理 receipt 或归档质量门。对这个仓库来说，全量回归不是形式主义，因为 OSFS 源码和主库一起构建，公共 CMake 或头文件错误会影响很多目标。

## 边界字段和跨项目解释

虽然 OSFS 本身不是 `INFOJSON` 或 `SMOKEJSON` 的证据命令，但本版仍然要按 mini-kv 的治理边界来解释。`read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal` 这些字段在主线 evidence 里代表 Node/Java 能否把 mini-kv 当执行系统使用。v1634 没有新增任何这类证据字段，也没有改变这些字段的含义。OSFS 的写入只发生在用户显式传给 `minikv_osfs --disk` 的课程设计磁盘镜像中，不写 KV store，不追加 WAL，不启动 TCP server，不把自己声明为订单权威。

从上游消费角度看，Node/Java 仍然只能把 mini-kv 主线 fixture 和 runtime JSON 当只读证据。v1634 的课程设计能力不会让 Node 获得 restore/load/compact 执行入口，也不会让 Java 通过 mini-kv 写订单状态。这个区别需要写清楚，因为本仓库长期同时承担两件事：一条是生产风格 read-only evidence 链，另一条是 OS 课程设计演示。二者共用仓库和测试环境，但权限边界不能混在一起。

## 本版价值

v1634 的价值不在于多了几百行代码，而在于把一个“报告里必须承认的简化项”升级成了真实可跑、可测、可解释的机制。文件系统课程设计最看重的不是界面命令有多少，而是磁盘结构有没有透明的因果链：inode 指到直接块，直接块不够时指到间接块，间接块列出更多数据块，读写能跨边界，删除能把所有块归还，测试能抓住泄漏和越界。v1634 正是把这条链补起来。

这一版也回答了“添加这些机制是否只是为了课设加厚”的疑问。它不是冗余，因为 v1631 的 MFD/UFD、用户表和权限让文件系统有了多用户目录结构，v1632 的 fd offset 和 range I/O 让文件读写不再是整文件替换，而 v1634 的一级间接块让这种 range I/O 能跨越直接块上限继续成立。如果没有 v1634，前两版虽然能展示登录、目录、权限和读写位置，但一旦老师要求写入超过八块的数据，系统就只能回答“这里是教学简化”。现在这条链变成了：用户登录后拿到自己的 UFD，创建文件后 inode 记录 owner 和 mode，写入过程按 fd offset 定位逻辑块，逻辑块再由 direct/indirect 映射到真实磁盘块，删除时 bitmap 和 free counter 被恢复。每一步都有输入、输出和可检查状态，不再只是命令层看起来像文件系统。

从维护角度看，本版没有把 indirect block 逻辑散落到命令层，也是为了后续 v1635 的 fsck 铺路。fsck 要检查 bitmap 与 inode 可达块是否一致，如果当前版本把块枚举逻辑写在多个函数里，v1635 就会面临“正常读写一套解释，fsck 检查另一套解释”的风险。现在 `inode_data_blocks` 和 `release_inode_storage` 等 helper 已经把“一个 inode 到底占了哪些块”表达清楚，下一版 fsck 可以复用同一套概念去构造 expected bitmap，而不是重新猜 direct 和 indirect 的规则。也就是说，v1634 的拆分不是为了漂亮，而是降低下一版一致性检查的实现风险。

一句话总结：v1634 让 mini-kv 的 OSFS 支线从“二级目录和 fd 语义已经可交付，但大文件寻址仍是教学简化”推进到“具备一级间接块的真实磁盘寻址模型”，同时保持 Node/Java/mini-kv 松耦合证据链的只读、无路由、无写扩展、无执行边界不变。
