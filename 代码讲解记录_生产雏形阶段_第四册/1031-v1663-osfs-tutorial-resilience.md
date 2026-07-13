# v1663 代码讲解：OSFS 通俗教程与韧性证据加厚

## 本版目标、角色和明确不做什么

v1663 处理的不是 OSFS 功能缺失，而是“已经做出的机制怎样让学生真正看懂，以及现有证据能否经得住追问”。v1629-v1636 的历史讲解按版本形成，每篇需要同时交代版本目标、边界、测试、归档和跨项目关系。对维护者来说，这种写法能追踪演化；对第一次接触文件系统的学生来说，阅读顺序却被开发时间线切碎。读者要先拼起 v1631 的 MFD/UFD，再接上 v1632 的 fd，随后跳到 v1634 的间接块和 v1635 的 FSCK，才能回答一次 CREATE/WRITE 到底怎样落盘。

本版因此新增 `OSFS课程设计通俗教程/`，按磁盘布局、目录、fd、间接块、FSCK 和测试证据组织八篇专题。历史讲解不移动、不覆盖，因为它们仍是版本证据；新教程负责建立知识结构。与此同时，本版没有满足于改写文字，而是重新审计 `tests/osfs_tests.cpp` 和 FinalShell 分段记录。审计发现五类自动测试缺口，以及一处演示解释偏差，于是增加独立 `osfs_resilience_tests` 目标，并把历史输出、当前结论和待实跑步骤分开记录。

这不是磁盘格式 v4，不新增命令，不改变 `LOGIN`、`CREATE`、`WRITE`、`FSCK` 等输出，不扩展任意深度目录，也不把 OSFS 接入 KV、WAL、TCP 或 Node/Java。FinalShell Step 08/09 只有命令和观察点，远端尚未实跑，所以仍明确标记为待执行；本地 Windows 运行证据不会被改写成 Linux 证据。

## 起点复核：原有测试强在哪里，薄在哪里

原 `osfs_tests` 有七个场景，已经覆盖磁盘格式 v3、预置用户、MFD/UFD、认证、权限、命令 shell、fd 偏移、跨块 I/O、空间不足保留旧数据、一级间接块重开和释放、block bitmap 损坏、USERADD/PASSWD 与用户表满。它不是只有 happy path，也不是只按字符串判断命令存在。

不过，逐分支对照实现后仍有五个值得补的空位。第一，`login` 明确清空 `handles_`，需求矩阵也声称登录切换使旧 fd 失效，但旧测试没有直接拿 Alice 的 fd 切换到 Bob 后再访问。第二，权限测试覆盖 owner 和无权限用户，却没有真实走同 gid 的 group 分支，也没有区分组写权限和删除权限。第三，UFD 目录项每条 64 字节，一个 512 字节块只能放八条；旧测试没有创建第九个文件，因此目录扩块路径未被触发。第四，区间写覆盖块边界，却没有从 EOF 之后写入并验证中间区域清零。第五，FSCK 已有 block bitmap 故障注入，但 inode bitmap 的独立不一致没有测试。

这些缺口不会让课程设计失效，却恰好是老师追问“你怎么证明”的位置。补它们比再增加一条普通 CREATE/READ 用例更有价值。

## 测试文件如何拆分

原 `tests/osfs_tests.cpp` 已接近四百行。把五个场景继续追加进去，会让一个测试入口同时承担主流程、韧性反例和原始镜像修改工具。v1663 先抽出 `tests/osfs_test_support.hpp`，把临时镜像命名、fd 解析、预置用户查找、bitmap 位修改和 FSCK 失败项查询集中起来。数据与行为边界很窄：支撑头文件只提供测试工具，不知道任何场景的命令顺序和预期；两个测试源文件各自拥有断言。

现有 `osfs_tests.cpp` 改为引用这些 inline 工具，功能场景和断言不变。新 `osfs_resilience_tests.cpp` 只有五个主题场景，CMake 将它注册为单独 `osfs_resilience_tests`。focused CTest 因而有三个清楚的入口：原核心行为、韧性反例、真实 CLI smoke。测试失败时，维护者不必从一个六百行入口里猜测属于哪一类。

这个拆分也符合童子军规则。被触碰的原测试文件减少了内嵌工具代码，离开时比进入时更短；新文件名和标识符都在四十字符预算内，没有创建第三个相似 test-support family，也没有让生产源码增长。

## 五个新增场景分别保护什么

`test_login_switch_closes_descriptors` 先让 Alice 创建并以 `rw` 打开文件，写入后不关闭，直接登录 Bob。随后对旧 fd 执行 READ 和 TELL，必须得到 `ERR bad fd`。再登录 Alice，fd 分配必须从 3 重新开始；只读 fd 执行 WRITE 返回 `ERR fd not writable`，append fd 执行 READ 返回 `ERR fd not readable`，同一 fd 关闭两次则第二次返回 `ERR bad fd`。这一组断言把会话身份、句柄生命周期和打开模式连成一条链，防止“帮助文本写了 r/w/a，但执行时都能读写”的退化。

`test_group_permissions_and_root_override` 利用 Alice 和 Bob 同属 gid 100 的事实。Alice 把文件设为 0640 后，Bob 通过 `alice/shared` 可以读取，证明 group read 分支真实参与；Bob 写入必须被拒，因为 group write 位没有打开。Alice 改成 0660 后，Bob 可以写，但仍不能删除，因为删除策略要求 owner 或 root，而不是简单把目录删除等同于文件写权限。最后 root 把文件改为 0600 后仍可读取并删除，验证 uid 0 例外。这比只测 0600 的“别人全拒绝”更能说明九位权限码的实际含义。

`test_directory_growth_and_slot_reuse` 从空 Alice UFD 连续创建八个空文件。由于空文件只占 inode 和目录项，不占普通数据块，超级块空闲块数应保持不变。创建第九个文件时，一个目录块的八个槽已满，UFD 必须从 block bitmap 再分配一个目录数据块，空闲块数精确减少一。删除第三个文件不会回收整个目录块；创建 `replacement` 应复用空槽，不能再消耗新块。重开镜像后仍要有九个目录项、旧名字消失、新名字存在，并且 FSCK 通过。这里用容量公式和空闲计数证明扩块与复用，不只看 DIR 行数。

`test_sparse_range_write_zero_fills_gap` 在空文件 offset 600 写入 `tail`。目标大小是 604，需要两个 512 字节块。分配函数会清零新块，区间写只修改末尾四字节；测试读取整个文件，要求前六百字节逐字节为零，再从 offset 508 读取跨块区间，要求得到九十二个零和 `tail`。关闭并重开镜像后再读末尾，仍要得到相同内容。这个场景验证了块清零、跨块循环、inode.size 和持久化四件事，能抓住未初始化空间泄漏或错误填充。

`test_fsck_finds_inode_bitmap_corruption` 先创建空文件并取得真实 inode 号，确认干净 FSCK 通过，然后直接编辑镜像的 inode bitmap 块，清掉该 inode 对应位。inode 记录仍为 used，UFD 仍指向它，所以 FSCK 重建出的期望位图与磁盘位图不同。结构化报告必须在 `inode bitmap` 检查中包含 `inode bitmap mismatch at inode`，命令层也必须输出 `FSCK ERROR checks=` 和对应 ERROR 行。这与原 block bitmap 注入形成两类独立损坏，排除了 FSCK 只识别单个固定样例的质疑。

## 教程为什么按八篇而不是一个大文件

新目录的 README 给出三条阅读路径：十分钟总览、顺序备考、按代码调试。`00-十分钟看懂OSFS` 用一组真实 LOGIN/CREATE/OPEN/WRITE/TELL/DIR/FSCK 输出建立全局图，先区分 OSFS 与 OSPF，也先说明它不是 KV 存储后端。`01-CREATE到WRITE完整流程` 是全套教程的主线，把命令、会话状态、UFD 目录项、inode、数据块、位图和错误返回放进同一流程。

`02-磁盘布局与inode` 用 v1636 的 160 块、64 inode 参数实际算出 inode 表十二块、数据区从 block 16 开始，解释四个 packed 结构和 `static_assert` 为什么保护磁盘兼容。`03-用户表与二级目录` 解释用户表与 MFD 的双重指针、UFD 隔离、带用户名路径、USERADD 一致写入和教学密码摘要边界。`04-文件描述符与区间读写` 只讲 Handle、四种模式、独立读写偏移、跨块区间公式、登录清 fd 和稀疏零填充。`05-直接块与一级间接块` 从 `8 + block_size / 4` 算到 69632 字节上限，再解释第九个数据块、间接块自身的额外空间和删除释放。

`06-FSCK与用户管理` 把六项检查逐项拆开，说明期望位图如何重建、两种故障如何注入、只读诊断为什么不自动修复，同时把 USERADD/PASSWD 的写路径与 FSCK 的反向复核联系起来。`07-测试地图与答辩问答` 不重复几十篇问答，而是先区分 C++ focused test、CLI smoke、Linux 实跑和报告矩阵能证明什么，再把十二个场景映射到需求，并链接已有答辩资料。

每篇都沿用同一阅读逻辑：问题是什么，输入什么，内存和磁盘怎样变化，源码入口在哪里，用户看到什么，哪个测试会在回退时失败，教学边界是什么。共同结构让读者建立习惯，但正文没有复制同一套“本版不是什么”的治理段落。教程九个 Markdown 文件的相对链接由命令检查，`project_docs_honesty_contract` 还要求八个主题、关键公式、源码符号、测试入口和 FinalShell 状态说明持续存在。

## FinalShell 记录中发现的偏差怎样处理

分段记录复核发现 Step 04-06 在 `--format` 后又执行 `USERADD alice` 或 `USERADD bob`。格式化本来就预置这两个账号，所以命令真实结果是 `ERR user already exists`；当时 grep 没匹配这条错误，只留下命令回显。Step 04 的旧解释却把它写成建立用户入口，这会让读者误以为 USERADD 成功。

本版没有修改历史终端块来伪造一份更漂亮的输出，而是在目录增加 `00-复核说明.md`，逐项说明哪些结论仍成立、哪些不能再作证。Step 04 的正文改为：有效证据来自 Alice/Bob 后续登录和不同 DIR，而不是重复 USERADD。Step 05 对 Carol 的创建，由后续 Carol 登录提供间接证据，直接 `OK useradd carol` 应使用完整 transcript。Step 06 的重复 USERADD 与 fd/持久化结论无关，单独标注剔除。

演示覆盖还补了两个命令模板。Step 08 生成 4191 字节 payload，把 `DIRECT8|INDIRECT-BEGIN|` 放在 offset 4088，再读取 64 字节观察跨界。Step 09 从真实 STAT 输出解析物理块号，清掉 block bitmap 对应位，再运行 FSCK。两篇结果文件都写明“待 FinalShell 实跑”，只列验收信号，不填伪造日志。本地当前二进制已独立跑过同样机理：4291 字节 demo 在 offset 4088 返回标记；另一临时镜像的 block 20 位被清除后报告 `block bitmap mismatch at block 20`。这些结果进入 v1663 本地证据，但不冒充 Linux。

## 验证顺序和边界

代码改动后先用 CLion MinGW 重新配置并构建 `minikv_osfs_tests`、`minikv_osfs_resilience_tests` 和 `minikv_osfs`，再运行三条 focused CTest。第一轮结果 3/3 通过，其中原测试、韧性测试和 CLI smoke 分别承担不同证据角色。文档机械门第一次因复核说明中的断言片段写得比正文更窄而失败，修正为真实存在的句子后通过。这次红灯说明合同确实读取文档，而不是永远返回成功。

最终验证还包括 clang-format dry-run、教程链接检查、`git diff --check`、全量 CTest、真实 v1636 demo 和故障注入。新增测试没有修改旧期望，没有改 fixture，没有变更磁盘版本或命令字节。FinalShell 待执行状态是一个诚实边界，不影响本地代码和教程完成度，也不能在交付汇报中被说成“九段 Linux 演示全部完成”。

注册测试总数从 351 变为 352，只增加一条 CTest 入口而不是把五个场景伪装成五个版本。这个数字只说明调度器能看到新的可执行目标，不代表质量自动提高；真正的价值来自每个场景都对应一个原先缺失的分支，并且会在相应行为退化时终止进程。全量测试仍有必要，因为 `minikv_osfs` 链接仓库公共静态库，CMake 目标和公共头文件的变化可能影响其他测试，即使 OSFS focused 三项已经通过。

对抗性自审的最强问题是：这会不会只是把原来难懂的八篇换成另一批更长文档？回答不靠篇数，而靠入口设计和机械证据。新教程以一次命令链为主线，专题可以独立阅读；旧版本治理内容没有复制进每篇；每项机制都能回到源码和测试；新增五个反例关闭了讲解中原先缺少的可执行证据。若读者仍需按版本追溯，历史讲解保持原路径；若只为理解和答辩，新目录不要求先知道 v1631-v1636 的时间顺序。

一句话总结：v1663 没有增加 OSFS 对外功能，却把“能运行”推进为“能按机制讲清、能用反例证明、能诚实区分本地与 Linux 证据”，这也让后续 Node/Java/mini-kv 松耦合协作继续把 OSFS 视为独立课程层，而不会误把它解释成新的存储或执行入口。
