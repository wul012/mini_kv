# mini-kv v1657：项目文档诚实性刷新与机械防漂移

## 一、本版为什么值得单独成为一个版本

v1657 没有新增 KV 命令，也没有扩展 OSFS 功能，但它并不是把几个版本号机械替换一下。仓库在 v1629 以后连续加入了完整的 OSFS 课程层，又在 v1637-v1656 完成了二十七个 receipt formatter owner 的 builder 收敛；与此同时，最常被新维护者读取的 `START_HERE.md` 仍把当前焦点写成 v1628，`README.md` 所谓“最近十版”实际停在 v1641，`docs/archive-retention-index.md` 的 active root 仍只统计到 `f/1620`，能力快照甚至没有一个独立 OSFS 小节。这意味着代码和测试是新的，但入口文档在描述旧世界。继续放任这种差异，会让后来的人按照错误基线做决策，也会让“项目具备什么能力”退化成凭记忆回答。

本版的角色是建立文档与 live tree 之间的可验证联系。它不是新的执行入口，不是新的 receipt schema，不是 OSFS 的第三轮功能扩容，不修改任何 frozen fixture，不移动历史 archive，不触碰 Store、WAL、Snapshot、RESP/TCP，也不打开 Node/Java router、credential、network、write、execution、auto-start 或 order authority。它只做三类工作：重新核实事实，写清事实从哪里来，再让关键事实缺失时 CTest 失败。这样“文档刷新”才是可维护性工程，而不是一次性的文字美容。

## 二、Step-0 如何发现真实漂移

执行前先读取 Node 最新的 `v2187-v2189-n5-source-size-health-roadmap.md`。该计划明确 mini-kv 可以并行做内部维护，不要求新的跨项目契约，也禁止修改 Node/Java 文件。随后读取本地 v1637 简报的阶段二要求：README、START_HERE 和 capability snapshot 必须反映 v1631-v1636 的二级目录、用户表、fd offset、间接块、FSCK 与用户管理；archive inventory 要重新测量；OSFS 简报要标记执行状态；每条能力主张要有真实文件或测试引用。

对账时没有相信进度表自述，而是查看当前 git/tag/CI、运行 `python scripts/archive_inventory.py`、读取 CMake 注册测试数，并用 CodeGraph 从 `FileSystem`、`UserInfo`、`add_user`、directory ownership 等符号进入 OSFS，再针对 file I/O、FSCK、descriptor 和测试做定点核对。结果证明功能本身与 Claude 复核结论一致，但文档入口确实落后。v1656 CI 在 v1657 开始时处于正常执行中，format 与 archive inventory 已绿，其余 job 仍在跑；所以本版没有把“已启动”写成“全绿”，而是按中间版本策略继续本地工作，并在收尾前再次检查。

## 三、README 和 changelog 为什么要一起改

README 明确承诺只保留最新十条，但原内容是 v1641 到 v1632。单独把“Current focus”改成 v1657，仍会留下一个内部矛盾：读者向上看是旧十版，向下看是新焦点。v1657 因此把 recent block 重新限定为 v1657-v1648，依次说明文档诚实性、阶段一完成、implementation/base/legacy/runtime-shell 各批迁移与冻结门。每条只保留足以理解版本价值和边界的摘要，详细数字仍指向 progress ledger、consolidation note 与 `f/<version>/证据/summary.txt`，避免 README 重新膨胀成历史仓库。

README 还声称完整历史位于 `docs/CHANGELOG.md`，但 changelog 同样停在 v1641。若不补 v1642-v1657，这句话就是过度主张。因此本版按倒序补齐缺失区间：三个审批合同顶层迁移、approval lifecycle baseline 与 post-echo 修复、runtime-shell baseline/迁移、legacy baseline/迁移、最终十三对象冻结、base/implementation/sandbox 三批收口，以及当前文档刷新。它们不是从记忆编造，而是从已经提交的 progress rows、consolidation note 和每版证据摘要压缩而来。今后 recent block 可以继续只保留十条，但 changelog 必须连续。

## 四、START_HERE 如何重新回答“这是个什么项目”

旧 START_HERE 只介绍 Redis-like KV、WAL、TCP、RESP 与 runtime JSON，没有告诉新读者同一仓库还有独立 `minikv_osfs`。v1657 新增的描述刻意使用“Independent OSFS course-design filesystem”，因为 OSFS 与 KV 是两个并列 executable，不应让人误以为 OSFS 已替代 KV 的 WAL/Snapshot 存储。能力列表现在包含磁盘镜像、MFD/UFD、持久用户、权限、descriptor offset、direct/single-indirect blocks、read-only FSCK 与 USERADD/PASSWD；技术亮点也增加 OSFS 机制与 corruption/reopen tests。

当前焦点从 v1628 改为 v1657，并写清前一阶段的机械结果：二十八个 receipt source、二十七个 formatter owner、二十七个 builder-backed、零 pending、一个 no-formatter waiver。测试数不是模糊的“full coverage”，而是 `ctest -N` 可复现的三百四十九条注册测试；新增的一条正是本版 honesty contract。Where to look next 也指向 docs matrix、OSFS 源码/头文件/测试和 v1636 final package，让读者能从总览直接进入实现、证据或交付材料，而不是在一千多个历史文件中猜路径。

## 五、能力快照怎样做到“每条主张都有落点”

`docs/CAPABILITY-SNAPSHOT.md` 原有一百多条 KV 与跨项目 evidence 说明，但没有 OSFS 专节。v1657 新增的章节不是列命令名，而是按机制拆成七条证据链。第一条把 disk-format v3 的 superblock、双 bitmap、user table、inode table、MFD/UFD 与 data blocks 映射到 `osfs_filesystem.cpp` 和 `osfs_disk_layout.*`；第二条把认证、目录隔离和 mode bits 映射到 filesystem/directory 实现与两个 focused test；第三条把 `read_offset`、`write_offset`、READ/WRITE/SEEK/TELL 映射到 command processor header/source 与 descriptor test。

第四条解释八个 direct block 加一个 single-indirect pointer 的容量公式 `8 + block_size / 4`，并指向 file I/O、layout helper 与 persist/release test；第五条把 read-only FSCK 的 geometry、bitmap reachability、MFD/user/UFD alignment、ownership 与 free-counter 交叉检查指向 `osfs_fsck.cpp` 和真实 bitmap corruption test；第六条把 root-only USERADD、root reset、self PASSWD、旧密码拒绝、跨用户拒绝与 reopen 指向 `osfs_user_admin.cpp` 和对应测试；第七条说明 v1636 final package 是当前交付，旧包只是 superseded history。

每条能力后面同时写出限制。password hash 是教学确定性 hash，不是生产 KDF；FSCK 只检测不修复；目录故事只有 MFD/UFD 两级，不是 arbitrary-depth namespace；文件只有一级间接块，不做 double/triple indirect；没有 journal/crash log 或多进程并发协议；OSFS 也不获得 Node/Java credential、managed-audit 或 order authority。把这些边界写出来不会削弱项目，反而让老师或维护者知道哪些是完整机制、哪些是有意教学范围。

## 六、archive inventory 为什么不能只看 f/1656

K5 在 v1620 建立 `scripts/archive_inventory.py` 与 retention index，当时 `f/` 只有 1597-1620、九十九个文件、约 3.20 MiB。之后 OSFS 从 v1629 到 v1636 产生截图、说明和交付证据，receipt 系列又持续到 v1656；旧 index 继续显示 v1620 会严重低估 active root。v1657 重新运行脚本，得到 `f/` 为 1597-1656、二百九十三个文件、10,501,451 bytes、10.01 MiB，最新 v1656 只有六文件、78,819 bytes，全部低于 128 MiB root ceiling 与 8 MiB per-version budget，warnings 为 none。

OSFS 区间还被单独逐版计量：`f/1629-1636` 合计四十二个文件、3,440,216 bytes、3.28 MiB。`课程设计交付/` 不属于 a-f screenshot root，脚本不应偷偷把它混入 root totals，所以本版使用单独的只读文件系统测量，记录一百零二个文件、5,445,971 bytes、5.19 MiB，并注明当前权威包是 v1636，v1630/v1633 保留为 superseded history。计量只产生事实，不授权删除、移动、重命名或重新压缩任何路径。

## 七、两份简报与“三份”要求如何诚实处理

v1637 阶段二写了“三份 OSFS 简报标注已执行完毕”，但 live `治理计划/` 实际只有 `v1631-osfs-coursework-completion-brief.md` 和 `v1634-osfs-capacity-extension-brief.md` 两份 OSFS 执行简报。第三个相关文件 v1637 是 Production Excellence 收尾简报，不是 OSFS 执行简报。若为了满足字面数量临时复制一份所谓第三简报，既会伪造历史，也没有独立任务内容。

本版选择现实优先：在两份真实简报标题下写明执行区间、完成机制与独立复核状态；治理 README 原本已有两条 executed 索引，本版再把 v1637 状态更新为阶段一 v1656 完成、阶段二 v1657 完成、阶段三待执行；`docs/project-docs-honesty-matrix.md` 单列 count mismatch deviation，明确 “Only two OSFS execution briefs exist”。这让 reviewer 能复现目录事实，也防止后续会话误把 v1637 当成第三份课程功能计划。

## 八、project_docs_honesty_contract 怎样工作

新增的 `cmake/check_project_docs_honesty.cmake` 是零依赖 CMake script，通过 `add_test` 注册为 `project_docs_honesty_contract`。它先验证八个被文档引用的 OSFS 实现/测试路径存在，再逐文件查找关键片段：README 必须有 v1657、三百四十九条测试和 27/0/1；START_HERE 必须有当前焦点与 independent OSFS；capability snapshot 必须有 OSFS 章节、fsck/test 路径与容量公式；archive index 必须有 active range、OSFS interval bytes 与 coursework bytes；matrix 必须有 two-brief deviation；changelog 必须包含 v1656；两份简报必须有 executed marker。

最后，脚本直接读取 `tests/osfs_tests.cpp`，要求六个机制测试函数仍存在：磁盘用户与二级目录、认证 shell、descriptor offsets、indirect persist/release、fsck corruption、useradd/passwd/fsck command。它不试图解析自然语言或证明所有文档句子，但把最容易再次漂移的主张变成机械门。以后若新增测试使注册数变化、移动 OSFS 文件、删掉机制测试、再次让 README 落后或误改 archive 数字，维护者必须同步文档，而不是让错误悄悄积累。

## 九、验证层次与真实输入输出

第一层是直接 `cmake -P` 运行 honesty script，确保不依赖完整构建也能快速失败。第二层是 CMake 配置后 `ctest -N`，实际输出 `Total Tests: 349`，证明文档数字和注册现实一致。第三层 focused lane 同时运行旧 orientation、new honesty、receipts note、builder census、`osfs_tests` 与 `osfs_cli_smoke`，六条全部通过；这避免新文档门覆盖旧门或只在 Windows 文本读取上偶然成功。

真实 OSFS smoke 使用新格式化镜像：root 登录后 FSCK clean，创建 docuser、重置密码；docuser 用新密码登录，创建 note，写入 `current-docs`，TELL 显示 write offset 为 12，再列目录并运行 FSCK。关闭程序后重开同一镜像，docuser 仍能用新密码登录，UFD 中仍有 note，OPEN/READ 读回原内容，证明 user table、UFD、inode/data 和认证并非截图假象。真实 KV CLI 继续运行 SMOKEJSON 与 `CHECKJSON LOAD data/prod.snap`，前者 read-only 且不允许 execution，后者不执行 LOAD、不触 WAL，最后返回 BYE。

配置过程有两项偏差也会进入证据：第一次新 build tree 没显式 generator，CMake 选择 NMake 但本机没有 nmake，0.4 秒失败；删除不完整树后改用 CLion MinGW。冷态 all-target build 超过工具十分钟窗口而被终止，进程审计确认没有残留 cmake/make/g++，同一树增量续跑约三十秒完成 100%。这些是工具和生成器事实，不应被误写成源码失败，也不能从总结中删掉。

## 十、对后续阶段和三项目协作的价值

v1657 完成后，mini-kv 的 README 不再卡在 OSFS 前、START_HERE 不再卡在 receipt consolidation 初期，capability snapshot 能区分 KV 主线、read-only evidence 与课程 OSFS，archive index 也能解释一千六百版以后增长来自哪里。更重要的是，这些状态不只靠一篇总结，而由 CTest 中会失败的路径、片段和测试符号约束。阶段三做 E1-E10 终审时，可以直接引用这份 matrix 和 honesty contract，不需要再次从旧文档猜当前树。

## 十一、最强 reviewer 质疑与本版回应

最有力的质疑不是“文档有没有更新”，而是“这些路径和测试名只能证明文件存在，不能证明整篇文档永远正确”。这个质疑成立。自然语言中任何一个数字、范围或边界都有可能在机械门覆盖之外漂移，因此 v1657 没有把 honesty contract 宣称为形式化证明。它采用分层回应：高风险、容易机械化的事实进入 CMake 门；复杂机制由源码位置加 focused test 支撑；真实用户表面再由 CLI smoke 验证；无法由脚本合理判断的教学范围和跨项目定位则在 matrix 中明确边界，并留给阶段三的外部审查。

例如，“OSFS 有用户管理”如果只检查 `USERADD` 字样，确实可能只是空命令。这里还引用 `FileSystem::add_user` 的 inode/block 分配、user table 写回和 MFD entry 更新，并用测试证明非 root 被拒、root 创建、密码重置、重开认证和 UFD 可见；手动 smoke 又在新镜像上重做一次。类似地，“FSCK 存在”不靠帮助文本，而靠 `check_consistency()`、clean image、实际 bitmap corruption 和命令输出四层证据。honesty contract 的价值是防止关键入口悄悄删掉这些证据链接，而不是替代实现审查。

另一条可能质疑是“三百四十九条测试是否等于高质量”。答案同样是否定的：数量只是当前注册清单，不代表每条深度相同，更不代表四项目真实联调。文档因此使用“349 registered tests”，不写“349 个完整集成测试”或“全功能生产覆盖”。其中既有 unit、fixture、CMake contract、CLI smoke，也有真实 TCP 和 OSFS reopen；阶段三必须按证据类型分类，不能把总数当作生产成熟度分数。

## 十二、以后改功能时应怎样同步文档

未来新增或修改 OSFS 机制时，维护者应先改实现与 focused test，再更新 capability snapshot 中对应机制行，最后判断 README/START_HERE 是否需要提升为公共入口信息。若只是内部 helper 重命名，保持 public 行为不变，可以更新路径引用而不制造新版本宣传；若改变 disk format、最大文件容量、目录层级或密码语义，则属于真实契约变化，必须同步报告、demo、需求矩阵和旧镜像兼容说明。honesty contract 中的路径或测试名也应跟随职责重命名，但不得简单删除检查来让迁移通过。

receipt 侧若新增 formatter，首先运行 census。新的 formatter owner 必须直接使用 ordered builder，或在简报中提供明确、可机械核验的 no-formatter 理由；不能把 floor 仍写成二十七却让 pending 重新出现。若 fixture 与 runtime 有历史差异，必须像现有 family 一样先冻结、命名单用途规则、限制应用次数，再迁移 production。README 只记录版本价值，字段级细节进入 consolidation note 和 per-version summary，防止公共入口再次膨胀。

archive 侧每个版本归档后都可以运行 `scripts/archive_inventory.py --strict`。当 active root 接近 ceiling 时，先更新 AGENTS 与 retention index，再创建下一 sibling root；绝不能为了让数字好看而搬动旧版本。课程交付目录不属于脚本 root，若继续增加新 final package，需要单独测量并说明 authoritative 与 superseded 关系。这样 root budget、课程材料和历史引用三件事不会混成一个模糊总量。

## 十三、三类措辞必须分开

第一类是“实现能力”：例如 direct/single-indirect、MFD/UFD、USERADD、FSCK detection。只有真实代码、测试和运行结果同时存在，才能写“已实现”或“完整覆盖当前教学范围”。第二类是“证据能力”：例如 receipt、SMOKEJSON、CHECKJSON，它们可以证明 read-only/no-execution，但不表示 mini-kv 能执行 Node/Java 操作。文档必须使用“echo、classify、report、non-participation”，不能偷换成“集成、执行、管理”。

第三类是“生产成熟度”。三系统 CI、sanitizer、coverage 与 archive gate 都很有价值，但 OSFS 仍没有 journal、repair、任意深目录、double/triple indirect、多进程锁或生产密码 KDF；三项目也仍主要是 single-project validation 加 cross-project contract alignment。除非以后出现 Node 真调用 Java、真读取 mini-kv runtime、统一命令输出联合报告，并有 no-write integration test，否则文档不能写成 real joint production integration。v1657 通过校准这些词，避免优秀的单项目证据被夸大成另一个尚未实现的系统。

一句话总结：v1657 把 mini-kv 已经完成的 OSFS 与 receipt 工程事实重新投影到公共文档，并用三百四十九条注册测试中的新机械门锁住投影，同时保持 KV/OSFS runtime、fixture、router、credential、network、write、WAL、execution、auto-start 与 authority 边界不变。
