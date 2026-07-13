# v1664 代码讲解：README 为什么也需要可执行证据

## 一、本版目标、角色和明确不是什么

v1664 是一个维护版本，目标不是给 mini-kv 再添一条命令，而是重做仓库最先被看见的
入口。旧 README 把最近十个版本放在最上方，后面连续铺开构建、参数、命令、协议、WAL、
Snapshot、测试和路线图。维护者能从中查资料，但第一次来到 GitHub 的读者必须先理解大量
版本背景，才能回答三个最基本的问题：这个项目到底是什么、它真正做到了什么、这些说法
如何亲手验证。v1664 把这三个问题放回首页主线，详细资料则继续由专门文档承担。

本版的角色是“展示层重构和证据导航”。它不修改 `src/`、`include/`、`tests/` 或任何
fixture，不改变 `SET`、`SMOKEJSON`、`CHECKJSON` 等命令的字节输出，也不改变 WAL、
Snapshot、RESP、TCP 或 OSFS 的行为。它不是新的运行入口，不会绕过审批和跨项目契约，
不会让 Node 获得 mini-kv 的写接口，更不会把一个教学与工程验证系统包装成生产数据库。
README 写得更清楚，不等于系统边界变宽；本版最重要的纪律恰好是让“看起来更强”和
“说得比证据更强”彻底分开。

## 二、旧首页的问题不在内容少，而在阅读顺序

旧 README 有五百多行，信息量并不低。问题是信息架构从维护历史出发：顶部先列 v1663、
v1662 一直回溯到 v1654，读者要先理解 OSFS resilience、elegance census、receipt builder
等项目内部词汇，才能看到 `Build` 和 `Run`。这种顺序适合已经参与项目的人，却不适合
老师、面试官、开源访客或未来重新回来的自己。版本记录还会不断增长，每增加一个版本，
首页最有价值的前几十行就继续向下移动。

新 README 把阅读路径改成：徽章和一句话定位 -> 30 秒事实表 -> 架构图 -> Quickstart ->
运行入口 -> 证据地图 -> 边界 -> 最近版本。这里没有删除历史，完整流水仍在
`docs/CHANGELOG.md`；也没有删除技术细节，命令字段、WAL 顺序和 RESP 限制仍在
`docs/CAPABILITY-SNAPSHOT.md`。变化只是让每类文档各司其职：首页负责判断是否值得继续
了解，能力快照负责查参数，测试文档负责复现，变更记录负责追历史，治理计划负责约束未来。

## 三、首页中的每个数字如何取得

展示页最容易出现的质量问题是“看起来可信的数字”。v1664 没有从版本总结里随手摘数字，
而是逐项回到会失败的机械门。CTest 数量使用当前 CMake 注册清单的 352，而不是简报草稿
里已经过期的 351。这个差异来自 v1663 新增独立的 `osfs_resilience_tests`，所以本版没有
为了迁就简报而倒退合同，反而在 Step-0 中明确记录 351 -> 352 的现实偏差。

覆盖率徽章写的是 “core coverage floor 90%”，刻意不写成含糊的“全仓覆盖率 90%”。
`docs/minikv-track-final-evidence.md` 保存了 Ubuntu coverage lane 的原始口径：筛选
store、command、WAL、snapshot、RESP 核心文件，共 2345 行、执行 2122 行、显示 90%；
`.github/workflows/ci.yml` 使用同一 filter 和 `--fail-under-line 90`。因此首页同时写清测量
集合、基线和强制下限，访客不会把核心模块数字误解成所有生成式证据文件的覆盖率。

源码体积也没有只写“文件都很小”。`cmake/check_minikv_track_final_evidence.cmake`
按非空物理行检查 `src/` 和 `include/`，800 行以上会失败，唯一允许的文件是已评审的字段
registry。这个豁免不是模糊的“特殊情况”，而是精确文件名；第二个超限文件会直接让合同
失败。receipt 表同理：28 个 receipt source、27 个 formatter owner、27 个 builder-backed、
0 pending、1 个没有 formatter 的具名 waiver，全部由
`check_receipt_builder_census.cmake` 重算，而不是 README 自己维护一份计数。

735/883 看起来不像宣传数字，保留它正是为了诚实。它表示存量仍有 735 个超过 40 字符的
文件名 stem 和 883 个公共头文件可见长标识符。优雅计划没有假装这些债已经消失，而是把
精确集合冻结为只减不增 baseline，新代码继续执行 40 字符预算。把存量债公开放在亮点表
中，是在展示治理能力：优秀不是“没有历史问题”，而是问题有口径、可复算、不能悄悄反弹。

## 四、成熟度标签为什么必须逐字准确

mini-kv 的 Stage-1 终审文档形成于四项目 capstone 之前，所以那份历史证据仍保留
`single-project validation + cross-project contract alignment`，不能回头篡改评审时点。
之后 Node 先完成 C1-C3，又因当时所谓 C4 只是聚合报告而被外部评审判定不完整；补上真实
aiproj 登记产物读取后，C1-C4 才得到 PASS。2026-07-13 的 program close 又针对最终 Java、
mini-kv v1663 和 aiproj 状态复跑，结果为 overall pass、C1-C4 pass、`read_only=true`、
`execution_allowed=false`。

因此首页可以采用评审唯一授权的新标签：`single-project validation + verified read-only
cross-project integration (env-gated, single machine, no execution authority)`。这句话中的每个
限定词都有作用。“verified”说明不是只看冻结 fixture；“read-only”说明 Node 对 mini-kv
执行的是新 CLI 进程中的 `SMOKEJSON` 和 `CHECKJSON`；“env-gated”说明默认测试不会读取
兄弟仓库；“single machine”排除分布式部署含义；“no execution authority”排除发布、恢复、
压缩、审批和订单权威。删掉任何一个限定词，首页都会比证据走得更远。

## 五、架构图讲的是职责，不是装饰

Mermaid 图把仓库拆成两个清晰子图。KV 主线从 Inline/RESP client 进入 parser 和 command
catalog，再到 command engine；engine 连接线程安全 Store、WAL、Snapshot 和只读 evidence
formatter。这样读者能看出 WAL 是写前记录与重放机制，Snapshot 是显式保存/装载机制，
证据 JSON 是观察面，不是另一套存储。

OSFS 子图从 `minikv_osfs` shell 进入 MFD 和持久化用户表，再到每用户 UFD、inode、权限、
fd 偏移、直接块和一级间接块；只读 FSCK 以虚线交叉检查磁盘结构。两个子图共同接受
CTest/CI 合同验证，但没有画一条 OSFS -> KV Store 的数据箭头，因为那条关系根本不存在。
Node capstone 只连向 `minikv_cli`，箭头上明确写 fresh read-only CLI process，防止读者把
“跨项目验证”误读为 Node 接管 mini-kv 生命周期或把 mini-kv 当审计后端。

## 六、Quickstart 的输入、输出和状态变化

第一组命令依次执行 configure、build 和全量 CTest。输入是当前源码树与 Debug 配置；输出
是构建目录中的可执行文件、测试程序和 `CTestTestfile.cmake` 注册清单；仓库业务数据状态
不发生变化。`ctest --test-dir build -N` 可以只看注册数，实际验收使用
`--output-on-failure` 跑完 352 项。Windows 多配置生成器会把程序放在 `build/Debug`，所以
README 单独给出 PowerShell 路径，避免照抄 Linux 路径后把环境差异误判成项目失败。

第二组命令向 CLI 输入 `SMOKEJSON`、`CHECKJSON GET demo`、`HEALTH`、`QUIT`。`SMOKEJSON`
输出运行身份、存储/WAL状态和边界字段；`CHECKJSON` 解析 `GET demo` 后输出
`mutates_store`、`touches_wal`、`execution_allowed`、warnings 等合同字段，但不执行 GET；
`HEALTH` 读取健康状态；`QUIT` 正常关闭进程。整组 smoke 不写 Store、不追加 WAL，也不会
启动 TCP server。README 另外说明若要观察真实写入恢复，应在临时 WAL 上显式运行 SET、
关闭、重开再 GET，避免把只读展示命令说成已经覆盖持久化写路径。

质量门命令把四个高价值合同放在同一正则中：终审 E1-E10、项目文档诚实性、receipt
census、elegance census。它们分别保护安全默认值和尺寸豁免、首页与实际树的对应关系、
builder 迁移不回退、命名债不新增。archive inventory 是第二条独立命令，因为归档预算和
历史路径稳定性属于文件系统清点，不应和 C++ 测试混成一个含义不明的大命令。

## 七、文档合同如何防止首页以后重新漂移

旧 `project_docs_honesty_contract` 已经要求 README 保留 v1663、352 tests 和 27/0/1 receipt
片段。v1664 没有删除这些检查，而是在其上增加新的精确片段：v1664 版本入口、获准 maturity
标签、2345/2122/90% 来源、735/883 基线、loopback 加无认证/TLS 警告、非生产边界和
program-close 证据链接。将来如果有人只改徽章数字、删掉限定词、把安全警告藏起来或让
首页指向不存在的计划文件，`project_docs_honesty_contract` 会失败。

这种合同不是 Markdown 快照测试。它不钉死整页排版，所以后续仍能改善表达；它只固定那些
一旦漂移就会改变事实或边界的句子。与 receipt byte parity 的思想相同，排版属于可演进
表面，数字、成熟度和权限含义属于必须有意变更的契约。若未来测试数增长，正确做法是先由
CMake 现实产生新数，再在同一提交更新 README、合同、矩阵和证据，而不是删掉合同来过门。

## 八、测试层分别保护什么

focused 文档门首先回答“首页是否诚实”：它检查关键数字和边界片段，同时确认链接到的 OSFS
源码、测试、教程和治理简报仍存在。`minikv_track_final_evidence_contract` 回答“展示引用的
底层门是否仍然有效”：coverage floor 仍为 90、TCP 默认仍是 loopback、唯一 800 行豁免
没有扩张、高置信密钥扫描仍在。`receipt_builder_census_contract` 和
`elegance_name_census_contract` 则重新计算两个 census，不信 README 的自述。

真实 CLI smoke 保护另一层：即使 README 链接和 CMake 文本都正确，也要确认当前构建出的
程序还能接受只读命令，并返回 `read_only=true`、`execution_allowed=false` 等运行字段。
全量 CTest 最后覆盖 Store、WAL、Snapshot、RESP/TCP、全部历史 fixture、receipt parity、
OSFS 和治理合同，证明一次首页重构没有靠误删文件或放松测试换来整洁。远端七 job CI 再把
本机结论放到 Ubuntu、macOS、Windows、sanitizer、coverage、format 和 archive 环境复验。

## 九、证据地图为什么不能只列一排文件名

证据地图按读者的问题分流，而不是按目录结构罗列。想判断 E1-E10 是否闭环的人进入
`minikv-track-final-evidence.md`，那里有每个 gate、两项 program deviation 和外部 PASS；
想实际构建的人进入 `TESTING.md`，不会被数百版治理历史打断；想判断是否能暴露到公网的人
先读 `SECURITY.md`，第一屏就能看到 loopback 只是一层默认保护，协议本身仍无认证和 TLS。
这种组织方式让文档入口对应决策，而不是要求读者先知道仓库作者的文件命名习惯。

receipt consolidation 单独指向迁移说明和 census 门，因为它回答的是“生成式 JSON 是否已经
归一为共享 builder，同时保持旧字节合同”；命名债单独指向精确 baseline 和 closeout，因为
它回答的是“剩余长名是否有意保留、未来会不会反弹”。这两类治理证据都不应塞进
`CAPABILITY-SNAPSHOT.md`：能力快照描述程序能做什么，治理材料解释代码为何维持某种形状。
职责分开后，更新一份文档不需要同时改动另一份无关叙事，也降低了再次形成巨型 README 的
风险。

OSFS 入口同样分三层。通俗教程按机制回答课堂学习问题；`v1636-osfs-final` 保存报告、需求
矩阵和完整交付；FinalShell 分段记录保存真实 Linux 命令与结果，并公开预置 Alice/Bob 的
历史 grep 漏项。v1631、v1634、v1663 三份简报分别解释二级目录和持久化用户如何补齐、
间接块/FSCK/用户管理如何加厚、教程与韧性证据如何重组。首页不重复这些正文，只给出入口
及每层能证明什么，既保留机理透明度，也避免把一份 README 写成第二本课程报告。

外部 capstone 使用完整 GitHub 链接，而不是本机 `D:\nodeproj` 路径。原因很实际：GitHub
访客无法访问作者电脑上的兄弟仓库；相对链接也不能跨仓库。program-close 文档保存最终态
复跑对象、C1-C4、只读字段和授权标签，正好支撑首页唯一的跨项目主张。mini-kv 本仓库仍
保留自己的 Stage-1 历史证据，不用事后改写旧评审文档来制造“从一开始就联合运行”的假象。

## 十、评审者最强质疑与回答

最强质疑是：“README 只是包装，是否用漂亮表格掩盖了项目仍有大量生成式命名债，或者把
一次 capstone 说成生产集成？”v1664 的回答不是形容词，而是把反证放在首页：735/883
存量债原样展示，唯一源码尺寸豁免明确存在；边界节写明无认证/TLS、not a hardened
production database；capstone 标签保留 env-gated、single machine、read-only、no execution
authority 四重限定；OSFS 也明确是教学子系统，不是 KV 后端。读者无需深入版本历史就能看见
这些限制，因此展示质量提高没有以诚实性为代价。

需求-证据矩阵最终要同时闭环六项：首页阅读路径、数字来源、可运行 Quickstart、成熟度不
越界、维护历史后移和版本仪式。任何本地测试失败、链接不存在、真实 CLI 输出不符合边界、
archive 超预算、CI 红或工作树残留，都意味着本版仍未完成。讲解写在最终 verify 之前，正是
为了让这些验收项先成文，再由命令检查，而不是测试通过后补一篇解释来合理化结果。

本轮实际结果与上述门槛一致：全目标构建完成 1011 步，focused 合同 4/4 通过，真实 CLI
结构化解析确认 SMOKEJSON 只读且无订单权威、CHECKJSON GET 非写且不触 WAL；全量 CTest
为 352/352、0 失败、217.03 秒。精选归档共六个文件、127258 字节，strict inventory 没有
warning。最终文档修改完成后仍需再跑收口验证，远端七项 CI 也必须对最终提交重新证明。

## 十一、一句话总结

v1664 没有扩大 mini-kv 的功能面，而是把已有 C++ 存储、OSFS 教学机制和机械质量门组织成
一条任何访客都能复现的证据路径；它给后续 Node/Java/mini-kv 松耦合集成带来的价值，是让
能力、输入输出和权限边界在进入跨项目消费之前就能被快速、准确、可机械核对地理解。
