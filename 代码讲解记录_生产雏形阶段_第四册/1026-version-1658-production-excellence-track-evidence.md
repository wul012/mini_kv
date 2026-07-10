# mini-kv v1658：E1-E10 轨道终审证据、安全默认值与机械收口

## 一、本版目标、角色与明确不是什么

v1658 的目标不是再增加一个看起来热闹的 JSON 命令，而是回答一个更难的问题：经过 K0-K6、OSFS 课程层和 receipts builder 收敛之后，mini-kv 是否真的能把“工程质量已经达到哪一步”交给外部评审者独立复现。过去每个里程碑都有自己的测试、CI run、归档和讲解，但证据散落在 workflow、SECURITY、TESTING、progress ledger、archive index、source file 和上级 playbook 中。若最终只能靠执行者口头说“都做过”，轨道仍没有闭环。本版因此建立统一的 E1-E10 证据入口，并让关键结论在回归时直接触发 CTest 失败。

本版的角色是终审准备者，不是终审裁判。`docs/minikv-track-final-evidence.md` 中每个 gate 的 `GREEN` 表示执行方已找到机械检查并完成本地自审；文件开头同时固定 `External verdict: PENDING`，明确外部 Claude 尚未给出结论。它不是生产数据库认证，不是四项目 live integration，不新增 route，不安装 active router，不扩大 write/admin/WAL 权限，不执行 LOAD、RESTORE、COMPACT，不读取 credential，不打开外部网络，不启动 Node 或 Java，也不让 mini-kv 成为 order authority 或 managed-audit store。成熟度仍只能写成 single-project validation 加 cross-project contract alignment。

## 二、Step-0 为什么必须重新读取上级标准

执行前先读取 Node 计划目录的 README、最新 `v2187-v2189-n5-source-size-health-roadmap.md`、总验收计划和 mini-kv playbook。最新 Node 路线允许 mini-kv 并行做内部质量收口，但不授权本会话修改 Node/Java，也不把合同对齐等同于联合运行。总计划给出的 E1-E10 原文依次是：Build & CI、Static analysis、Coverage、Security & config、Observability、Error handling、Docs honesty、Release discipline、Code health、Archive retention。每个门都要求有一个会在回归时失败的 CI job 或 committed test，而不接受“文档里写了”作为唯一证据。

本地 v1637 简报的阶段三还增加三个约束：终审证据必须列出机械检查、waiver、census 和 CI 链接；coverage 实际值若比 floor 高至少两分，floor 只能上调；外部评审前不得宣称超过单项目验证加跨项目合同对齐。v1658 因此没有凭历史 K2 的 90% 印象直接改数字，而是从 GitHub Actions run 29095734744 和 v1657 run 29097881334 的原始 log 中读取 `TOTAL 2345 2122 90%`。旧 floor 为 88，差值正好达到两分，触发简报中的只升不降规则。

## 三、终审入口文件的输入、结构和输出

本版的主要入口是 `docs/minikv-track-final-evidence.md`。它的输入不是某个运行时请求，而是仓库当前事实：CI workflow、K0-K6 进度、receipts 27/0/1 census、文档诚实性门、SECURITY/TESTING 边界、源码非空行数、归档库存和远端 CI log。它的输出是一张 reviewer 可以逐行复核的矩阵，而不是给程序消费的执行 packet。

矩阵每行包含四部分：gate 名称、自审状态、机械检查、复现证据。E1 指向三系统 build/CTest 与独立 sanitizer、coverage、format、archive jobs；E2 指向 changed-file clang-format 脚本和 workflow contract；E3 固定核心 filter、90% floor 和 coverage artifact；E4 连接 secret-pattern scan、loopback 默认、warn 日志默认与威胁模型；E5 连接 UTC/thread logger、connection_id、HEALTH 和 metrics；E6 连接 SIGINT/SIGTERM、request_stop、请求上限和 socket timeout；E7 指向文档诚实性门；E8 解释 CMake 0.102.0 与 git vNNNN 双版本流；E9 连接源码尺寸 ratchet 与 receipt census；E10 连接 archive inventory、预算与冻结路径。

这张表不会输出 `execution_allowed=true`，也不会成为新命令。控制面读到它时只能理解为“有哪些证据值得审核”。对于 runtime JSON，`read_only=true` 表示报告本身不执行动作；`execution_allowed=false` 表示不能把报告当作 LOAD/RESTORE/部署批准；`order_authoritative=false` 表示订单事实仍由 Java 掌握；`mutates_store=false` 与 `touches_wal=false` 分别说明命令合同没有存储写和 WAL append；`warnings`、`blockers`、`diagnostics` 是审查信号，不是绕过门槛的开关。v1658 没有改变这些字段，只把解释和真实 CLI 结果重新纳入终审证据。

## 四、`minikv_track_final_evidence_contract` 如何把结论变成失败条件

新增的 `cmake/check_minikv_track_final_evidence.cmake` 通过 CMake `add_test` 注册为第 350 条测试 `minikv_track_final_evidence_contract`。脚本第一层检查终审文件自身：状态必须是 READY FOR EXTERNAL REVIEW，外部结论必须是 PENDING，十个 E gate 必须完整出现，成熟度限制语句不能消失，coverage 必须写出 2345/2122/90%，census 必须是 28 sources、27 owners、27 builder-backed、0 pending、1 个 no-formatter census exemption，且必须明确只有两项 program-level deviation。

第二层检查活的工程文件，而不是只相信矩阵文字。workflow 必须仍在 push 上运行，三种操作系统条目、sanitizer、coverage、changed-file format、archive inventory 都必须存在；coverage 参数必须精确保留 `negative_hits.warn_once_per_file` 和 `--fail-under-line 90`。TCP 头文件必须把 host 默认写成 `127.0.0.1`，真实 TCP 测试必须断言这个默认；logger 必须默认 warn，日志实现必须包含 thread 字段；TCP server 必须包含 connection_id 与 request rejection，server main 必须安装 SIGTERM handler；timeout test 必须同时保护 idle timeout 和 command timeout。

第三层检查文档和治理的连接。SECURITY 必须明确“不是 hardened production database”、不存 production secret、不可直接暴露在不可信网络；CHANGELOG 必须保留双版本策略；CMake 必须继续注册 honesty contract 和 27 owner census；archive index 必须标记 active root 历史版本冻结，inventory 脚本必须仍有 budget 参数。任一关键文件被删除、关键措辞漂移或机械门从 CMake 中被拿掉，测试都会失败。

## 五、E4 审查发现的真实缺口：默认绑定地址

逐门审查不是把已有材料重新排版，它应当能够发现不满足标准的现实。E4 要求 safe-by-default config，而 `TcpServer::Options` 原先的 host 默认值是 `0.0.0.0`。这意味着用户只运行 `minikv_server 6379` 时，会监听所有接口；项目又明确没有认证和 TLS，因此这个默认与 SECURITY 中“本地学习系统”的范围不一致。若 v1658 只写“建议使用 127.0.0.1”却继续保留 wildcard 默认，E4 不应被标绿。

修复位于 `include/minikv/tcp_server.hpp`：默认 host 改为 `127.0.0.1`。显式传入 `0.0.0.0`、localhost 或其他地址的接口没有删除，所以需要受控外部绑定的操作者仍可明确选择并承担边界责任。`tests/tcp_server_tests.cpp` 直接构造未修改的 `TcpServer::Options` 并断言 host 为 loopback，终审 CMake 脚本又读取同一实现和测试片段，形成运行级与治理级双门。README 和 SECURITY 同步说明：省略 host 只监听本机，wildcard 必须显式，并需要外部认证、TLS 和防火墙。

本地真实 smoke 没有传 host，随机选择空闲端口后启动 `minikv_server.exe <port>`。操作系统连接表显示 PID 28948 的监听地址是 `127.0.0.1`，客户端以 5000ms socket timeout 连接，HEALTH 返回 live_keys、WAL 状态、命令总数、错误数、延迟和 connection stats，随后 QUIT 返回 BYE；服务端日志同样打印 `event=server_start host=127.0.0.1`。验证结束后该进程被停止。这证明入口参数解析没有在运行时覆盖安全默认值。

## 六、E3 为什么从 88 收紧到 90，而不是为了好看

K2 在 v1614 记录的第一份 baseline 是 2292 行、2082 执行、90%，当时用 baseline 减两分建立 88% floor，为早期拆分保留小幅波动空间。K4 后 command 代码拆分，filter 随 translation unit 扩展；后续 receipts 与 OSFS 工作并未把 OSFS 纳入核心覆盖集合，因为该集合始终专注 Store、Command、WAL、Snapshot、RESP。v1656/v1657 的新鲜 CI 重新测得 2345 行、2122 执行、90%，说明拆分后的核心基线已经稳定。

`.github/workflows/ci.yml` 因此把 `--fail-under-line 88` 改成 90，`tests/dependabot_config_tests.cpp` 同步要求精确 90。这里修改测试期望是合法的 ratchet 收紧：旧测试允许门槛偏低，新测试阻止将来降回 88；它不是为了让实现迁移通过而放宽 fixture 或行为断言。若 v1658 Ubuntu coverage 因取整或未覆盖代码跌到 89，required job 将直接失败，本版不能收尾。coverage 的 gcov parser workaround 仍只忽略工具报告的 negative branch-hit parse 异常，不忽略 build、CTest、report 生成或 floor 失败。

## 七、两项 deviation 为什么必须精确列举

第一项是 E9 的 `src/shard_readiness_boundary_field_names.cpp`。它当前约 829 个非空物理行，是按历史字段顺序维护的静态名称注册表，不包含路由激活或写执行控制流。K4 已评审接受暂不转换，因为换成另一种表结构需要对巨大 SHARDJSON 字段链做独立 byte-for-byte 证明。v1658 的尺寸门不是“所有大文件都可解释”，而是只允许这个完整相对路径超过 800；任何第二个 source/header 超限立即失败。如果注册表以后拆到阈值内，脚本反而会报 stale waiver，要求删除不再需要的豁免。

第二项是 E3 的 `--gcov-ignore-parse-errors negative_hits.warn_once_per_file`。v1620 曾真实删除参数，run 27428515080 在 `src/command_catalog.cpp` 的负 branch hit 上失败，证明问题没有随 command.cpp 拆分消失。恢复参数后，覆盖率 job 仍必须编译、跑全量 CTest、生成报告并达到 floor。它不是任意 parse error 通配符，workflow 与两个合同都保护精确参数。

27/0/1 census 中的 “1” 不是第三项 program waiver。`runtime_credential_resolver_execution_denied_retention_receipts.cpp` 不拥有 public formatter，所以它不属于二十七个迁移 owner；census 只是要求这个例外命名且数量固定。Windows MinGW 缺少 ASan/UBSan runtime、gcov sidecar 路径过长也不是放行：本地配置会明确 fast-fail，required Ubuntu jobs 仍负责最终信号。终审文件明确写 `No third program-level waiver is claimed`，防止以后把所有困难都塞进模糊 deviation。

## 八、E9 尺寸口径与 command.cpp 的透明解释

总标准使用“约 800 行”，K4 review 明确把 `command.cpp` 记为 732-line coherent core。v1658 复现同一口径：统计非空物理行，包含代码、include 和非空注释，不把空行当复杂度。当前 `command.cpp` 是 894 个总文本行，其中非空行 732；它保留 metrics、WAL gate 调用和 runtime evidence command machinery，与 K4 评审数字一致。若突然改用总文本行，项目会在同一文件、同一内容上把历史 PASS 解释为 FAIL，造成指标语义漂移。

终审脚本使用 `file(STRINGS)` 后剔除空白行，对 `src/*.cpp`、`src/*.hpp`、`include/*.hpp` 全量计数。除 829 行 registry 外，没有第二个文件超过 800；下一高的是 732 行 command.cpp。这里没有说 command.cpp 永远无需拆分：若后续新增功能使非空行超过阈值，脚本会阻断；维护者应先拆 runtime evidence family，而不是扩大 waiver。这个口径、例外和停止条件都写入终审文档，外部 reviewer 可以用 PowerShell 独立重算。

## 九、E4 高置信 secret scan 的范围与局限

终审脚本对 `src/`、`include/`、GitHub workflow 和 CMakeLists 做高置信模式扫描，查找 AWS access-key 前缀、GitHub token、Slack token、OpenAI 风格长 token 和 private-key header。它刻意不对 frozen fixture 与边界文档做“credential”关键词扫描，因为这些文件大量记录 `credential_value_read_allowed=false`，把领域词当密钥会产生没有意义的假阳性。扫描目标是 executable/configuration surfaces 中看起来像真实秘密的字节模式。

这个扫描不是专业 secret scanner 的替代，也不证明本地 git 历史从未出现秘密。E4 的证据组合是：高置信当前树扫描、默认 loopback、默认 warn、无生产 secret storage 的明确范围、无认证/TLS 时禁止不可信网络暴露，以及大量 no-credential runtime contract。若以后引入真实部署配置，应增加专门 secret scanner、密钥轮换和生产 profile，而不是继续把本 CMake 检查描述成完整供应链安全。

## 十、测试分层分别保护什么

第一层是无构建快速门：直接 `cmake -P cmake/check_minikv_track_final_evidence.cmake` 检查文档、workflow、safe default、secret pattern、source-size 和 deviation；`check_project_docs_honesty.cmake` 再核 v1658/350、OSFS 路径、归档与简报。第二层是聚焦 CTest：final evidence、docs honesty、receipt census、dependabot/workflow contract、logger、TCP server 和 timeout/limit 全部通过。logger test 保护默认 warn 过滤 info/debug，TCP test 保护 loopback、connection id、HEALTH/metrics 和 stop，timeout test 保护 idle/partial-command 关闭。

第三层是完整回归：新 MinGW build tree 配置后 `ctest -N` 得到 350，冷构建全部目标成功，完整 CTest 为 350/350、零失败、173.95 秒。这里既有 Store/WAL/Snapshot/RESP/TCP，也有 OSFS、release fixture、receipt parity、archive 和治理检查，能发现安全默认变化对旧网络行为或证据合同的旁路影响。第四层是真实进程与 CLI smoke：默认服务器实际监听 loopback；SMOKEJSON 包含 read_only=true、execution_allowed=false、write/admin/runtime_write=false；CHECKJSON LOAD 只报告可能的 store replacement，随后 SIZE 仍为 0；SHARDJSON 保持 readOnly、no execution、no active router、no write routing。

这些测试不等价。CMake 片段检查擅长防配置退化，却不能证明 socket 可用；真实 socket 能证明入口行为，却不能枚举所有历史 fixture；全量 CTest 能覆盖大量回归，却不能证明四项目 live integration。终审矩阵把证据类型分开，避免用“350 tests”一个数字代替机制解释。

## 十一、E7、E8、E10 如何避免工程仪式遮住事实

E7 复用 v1657 的 `project_docs_honesty_contract`，但把当前指针提升到 v1658 和 350 tests。README 最近十版变为 v1658-v1649，START_HERE 说明外部 verdict pending，capability snapshot 不宣称 live integration，TESTING 把 coverage floor 更新为 90。`docs/project-docs-honesty-matrix.md` 继续保留“实际只有两份 OSFS execution brief”的纠错，没有为了满足旧简报字面数量伪造第三份历史文件。

E8 不强行把 CMake `0.102.0` 改成 v1658。0.102.0 是冻结 runtime receipt 使用的兼容版本，git `vNNNN` 是交付节奏；CHANGELOG 已明确双版本流。终审要求 commit、annotated tag、push、`git show --check` 与远端 CI 形成闭环，但 tag 和 green run 必须由外部 reviewer 再核，不能仅凭文档自述。

E10 继续运行 stdlib-only `archive_inventory.py --budget-mib 8 --strict`。归档前基线为 `f/1597-1657`、299 files、10,584,325 bytes、10.09 MiB，最新 v1657 仅 82,874 bytes，warnings none。v1658 只增加选择性截图、中文说明和摘要，不放 build binary 或完整 350-test raw log。添加归档后 index 会重测并更新到最终数字，但任何测量都不授权移动、重命名或删除 a-f 与 release fixture。

## 十二、最强 reviewer 质疑与诚实回答

最强质疑是：仓库拥有大量治理 evidence，会不会用工程仪式掩盖它仍不是生产数据库。答案是不能掩盖，而且终审文件主动写出缺口。mini-kv 没有认证、TLS、复制、多进程写协调、网络分区恢复和生产密钥管理；OSFS 是独立教学 executable，不是 KV 的生产存储替代；FNV 是漂移/损坏检测，不是密码签名；read-only FSCK 只检测不修复。E4 GREEN 只表示声明范围内的默认配置、秘密边界和网络暴露说明受机械门保护，不等于可以直接部署到不可信网络。

第二个质疑是跨项目证据是否只是互相引用文件。这个质疑同样成立：目前有 frozen fixture、digest、no-write/no-execution 字段和 sibling contract alignment，但没有本仓库启动真实 Java jar，也没有 Node 在本测试中消费当前 mini-kv 进程。系统级 C1-C4 capstone 属于上级 Node acceptance program。外部终审可以批准 mini-kv 单轨道达到标准，却不能借此把整个四项目系统写成真实联合生产运行。

第三个质疑是 coverage 90 是否足够。90% 只覆盖明确的 core filter，不包括所有治理 formatter、OSFS 或平台异常路径；它的价值是建立稳定 floor 并阻止核心回退，不是宣称全仓 90%。OSFS 由独立 focused tests 和真实课程演示支撑，receipt 由 byte parity/census 支撑，网络由真实 socket 支撑。不同机制使用适合自己的证据，而不是扩大 filter 造一个含义模糊的总百分比。

## 十三、后续维护者如何使用这一版

后续新增 source/header 时，先跑 final evidence contract。若文件超过 800 非空行，应拆职责，不得复制 registry waiver；若 registry 已拆小，应删除 stale waiver。新增 receipt formatter 时，census 必须把 owner 纳入 builder-backed，不能让 pending 重生。修改 workflow 时，coverage floor 只能保持或提高，gcov workaround 只能在重新做删除实验且 CI 成功后移除，不能扩大为通配错误忽略。

修改服务器默认、认证、TLS 或外部绑定策略时，应先更新 SECURITY 威胁模型和真实 TCP test；只改 README 不算闭环。新增系统联调时，应在上级 capstone 中让 Node 真启动/调用 Java 与 mini-kv，并证明不能触发写、LOAD/RESTORE/COMPACT、credential read 或服务自动启动；完成之前保持当前成熟度措辞。归档继续按 active root policy 写 `f/<version>/图片` 与 `解释/说明.md`，接近 ceiling 时先改 retention index 和 AGENTS，再开新 root，历史路径不搬。

一句话总结：v1658 把 mini-kv 分散的构建、格式、覆盖率、安全默认、日志、错误处理、文档、发布、源码尺寸与归档证据收束成第 350 条会失败的终审合同，真实修复 wildcard 监听默认，收紧 coverage 到 90%，同时把两项 deviation、非生产边界和外部评审 pending 状态写得可独立复核，为 Node/Java/mini-kv 松耦合协作提供可信的单轨道质量底座，而不越权宣称 live integration。
