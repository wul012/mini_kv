# v1666：shard-readiness 测试架构收口与零豁免体积门

## 一、本版要解决什么，以及它不是什么

v1665 已经把 3805 非空行的 command 回归单体拆成 manifest 管理的九个职责段，但测试体积 baseline 里仍有一项：`tests/shard_readiness_tests.cpp` 为 4328 个非空行。v1666 的目标是偿还这最后一项，使仓库内每个 `tests/*.cpp` 都不超过 1000 非空行，并让 baseline 真正清空。清空不是删掉记录假装没有债，而是先把行为完整迁走，再由机械扫描器拒绝任何新的超限文件。

本版仍是测试架构维护，不是 shard 功能升级。它不安装 active router，不允许 write routing，不启动 Node、Java 或 mini-kv 服务，不读取凭据或 raw endpoint，不打开 HTTP/TCP，不执行 LOAD、RESTORE、COMPACT、SETNXEX、部署、回滚或 Java SQL，不修改 `fixtures/release/shard-readiness*.json`，也不改变 `SHARDJSON` 和其他公开命令的一个字节。测试里出现 `readOnly`、`executionAllowed`、`routerActivationAllowed`、`writeRoutingAllowed`、`touchesWal` 等字段，是在验证原有关闭边界，不是新增控制面能力。

## 二、为什么 shard 拆分比 command 更危险

旧文件表面上只有两个函数和一个 `main`，结构看似比 command 简单，实际风险更集中。`assert_shard_readiness_contract` 从第 40 行延伸到第 3130 行，包含 3710 次 `assert_contains`。更关键的是，这三千多行不是只执行一次：`main` 先读取当前冻结 fixture，要求它与 `minikv::shard_readiness::format_json()` 字节相等，再把 fixture 传入完整合同函数；历史 v144-v200 检查结束后，测试通过真实 `CommandProcessor` 执行 `SHARDJSON`，又把运行时 response 传入同一个完整合同函数。

因此，简单地把三千行均分到五个文件还不够。若新 main 只对 fixture 调五段，而真实 response 只做几个边界字段，3710 个源码断言仍然都在，静态 census 也不会下降，但“双 oracle”语义已经丢失。反过来，如果把每段各注册成独立 CTest，它们无法自然共享本轮 runtime response，也容易把历史执行顺序打散。v1666 必须同时保护三件事：断言文本没有少、五段都在同一 wrapper 下、wrapper 仍被 fixture 与 runtime 各调用一次。

文件开工前重新冻结 SHA-256 `4079DA25929A4D38CDDE722DF2A1FB07D348A04D2DEE2067B9668004A7F82572`，并重算普通 `assert=8`、`assert_contains=3710`、`assert_fixture_differs_from_each` 定义加调用共 2 次。这个起点不是交付自述，而是迁移 oracle；任何计数变化都必须先被视为漏测或意外新增，不能用“反正 CTest 是绿的”带过。

## 三、五个合同字段域为什么这样划分

第一段 `shard_contract_candidate_tests.cpp` 负责当前合同身份、候选 request/material/profile、signed approval draft 与 production live capture 前的字段。它从 contract、project、version、readOnly 等根字段开始，一直收住 candidate profile 和第一轮非参与语义。文件格式化后 737 非空行，是大段但仍有明确主题。

第二段 `shard_contract_governance_tests.cpp` 负责 type barrel、disabled precheck、upstream echo、fake harness、blocked rehearsal、production owner receipt 和讲解质量 closeout 等治理/非参与字段。它回答“mini-kv 没有参与哪些外部动作”，而不是路由算法本身。格式化后 442 非空行。

第三段 `shard_contract_route_tests.cpp` 负责 slot table、route preview、verification/report/archive 链、operator import/value draft/value supply 与 approval template。它把真正与只读路由预览相关的结构放在一起，470 非空行。这里仍反复断言 `activeRouterInstalled=false`、`writeRoutingAllowed=false`、`executionAllowed=false`，说明“route preview”始终只是预览。

第四段 `shard_contract_history_tests.cpp` 负责 current JSON 内嵌的 release catalog、fixture parity、archive compatibility、active prototype freeze、consumer handoff、live-read gate 和 operator lifecycle。它验证当前大对象如何解释历史，而不是逐个读取 v144-v200 文件；格式化后 627 非空行。

第五段 `shard_contract_catalog_tests.cpp` 负责 Node route group compatibility、route catalog evidence/cleanup 链、boundary catalog 和末尾字段路径清单，格式化后 906 非空行，是本版最大 part。它没有继续拆成更小片段，因为 906 已低于 1000 门，且 route-catalog closeout 到 boundary index 是一个连续的审计域。若为了追求平均大小再切，维护者反而需要跨文件拼接同一 catalog 语义。

五个函数都接收 `ShardFixture& fixture`，从 wrapper 当前设置的 `active_json` 取得只读引用。`assert_shard_readiness_contract(fixture, json)` 依次调用五段，然后清空临时指针。这里的指针不拥有字符串，也不跨调用保存；fixture 与 runtime response 在 wrapper 返回前都存活。选择显式 active reference，是为了让所有 manifest entry 维持统一签名，同时把“当前正在验证哪份 JSON”集中在一个入口，而不是复制两套五函数调用。

## 四、历史 fixture 和真实命令如何拆而不改顺序

旧 `main` 的后 1190 多行不是同一种测试。前半先证明 current fixture 等于 formatter，检查 fixture path，确认 current 不等于 v144-v200 任一历史版本，然后逐版断言 releaseVersion、status、source frozen path、digest 和关闭边界；最后约 250 行才创建 Store/CommandProcessor，执行真实命令目录和 route preview smoke。

新结构用 `ShardFixture` 保存 current、historical map、formatted 和 wrapper 的临时 active JSON。`load_shard_fixture` 仍一次读取 v144-v200，范围检查的普通 assert 保留。`run_shard_fixture_early_tests` 先做字节 parity、fixture path、current 完整合同、difference 检查，再按旧顺序验证 v144-v164；`run_shard_fixture_late_tests` 接着验证 v165-v200。分界选在自然的 release 边界，不重排任何版本，也没有把历史 fixture 改成只检查首尾。两段分别 297 和 716 非空行。

`run_shard_command_tests` 最后创建一份空 Store，先验证 `SHARDJSON extra` 用法错误，再执行真实 `SHARDJSON`。response 必须与 `fixture.current` 字节相等，并再次经过完整 wrapper；Store 大小仍为零。随后原顺序执行 COMMANDS、COMMANDSJSON、EXPLAINJSON、CHECKJSON、SHARDROUTE 与各层 report/archive smoke。这个文件 269 非空行，是真实命令入口的集中地图，不与历史文件读取混在一起。

新的 `shard_readiness_tests.cpp` 只有 9 个非空行：加载 fixture，调用 early、late、command 三个场景，然后返回。它不是把测试逻辑藏进不可见宏，而是把顺序变成清楚的三步目录。入口仍是一条 `shard_readiness_tests` CTest，避免错误地把有先后关系的场景包装成可任意并行的八个测试进程。

## 五、suite graph checker 如何阻止“文件存在但没有执行”

v1665 的第一版 scanner 只接受“每个 manifest entry 在 main 中调用一次”，这对 command 合适，却不足以描述 shard：五个合同 part 应由 wrapper 调用，三个场景 part 才由 main 调用。如果为了迁就旧 checker 强迫 main 直接调用所有五段，就会复制 fixture/runtime 两套调度，或者破坏 wrapper 作为唯一合同入口的职责。

v1666 将 `verify_suite` 泛化为 suite graph 检查。manifest 仍然只有路径和 entry 两列，CMake 仍从同一份数据编译所有 translation unit；scanner 先读 main 和全部 part，要求路径、入口唯一，再要求每个 entry 在自己的文件中恰好定义一次，并在 main 或任一 part 组成的调用图文本中恰好出现一次 `entry(fixture);`。这样 candidate/governance/route/history/catalog 必须由 wrapper 各调用一次，early/late/command 必须由 main 各调用一次；遗漏、重复和悬空文件都会红灯。

仅检查静态调用图仍不足以证明 wrapper 被使用，所以 shard suite 额外固定两个 required call：`assert_shard_readiness_contract(fixture, fixture.current);` 恰好一次，`assert_shard_readiness_contract(fixture, result.response);` 恰好一次。前者保护冻结 fixture oracle，后者保护真实命令 oracle。再叠加 8/3710/2 精确 token census，既不能删字段断言，也不能保留死代码来伪装数量。

## 六、baseline 清零意味着什么

`config/test-source-size-baseline.txt` 现在只剩格式说明注释，没有任何豁免项。scanner 递归测得 361 个测试 `.cpp`，上限 1000，oversized 为 0。361 表示源文件数量，不是 CTest 注册数；拆分增加 translation unit，但默认 CTest 仍为 353。文档把这两个数字分开，避免用“文件更多”冒充“测试更多”。

门的红路径仍覆盖三种退化：新文件 1001 行且无 baseline 会失败；已有超限项从 1001 增到 1002 会失败；文件已降到 999 却保留 baseline 也会失败。因为当前 baseline 为空，未来若有人想新增豁免，diff 会非常醒目；计划明确 v1666 后停止无目标 retro split，不顺手拆 800 行以下文件，也不允许为了继续制造版本而降低阈值。

## 七、测试证据和边界解释

聚焦层先运行文档 honesty、test architecture、shard readiness 和 release verification manifest 四项。架构门输出 `sources=361 limit=1000 oversized=0`；shard 测试真实运行约 21 秒，说明 3710 个字段断言不是只被编译。全量层随后覆盖 command、Store、WAL、Snapshot、RESP/TCP、OSFS、receipt parity、fixture digest、coverage/CI contract 和历史治理链。真实 CLI 还会重新结构化解析 SMOKEJSON 与 CHECKJSON，确认 `read_only=true`、`execution_allowed=false`、`order_authoritative=false`、`write_command=false`、`touches_wal=false`。

这些字段必须按证据面理解。mini-kv 仍有 SET、DEL、EXPIRE 和 WAL 等真实功能；本版证明的是 shard readiness/跨项目 evidence 不会借预览命令获得 router、write、network、credential 或 execution authority。warnings、blockers 和 diagnostics 继续公开未满足条件，Node 可以消费只读事实，但 mini-kv 不成为 Java 订单权威，也不自动启动任何上游。

本轮构建仍报告一个历史 CMake 路径长度 warning，指向另一份名称极长的测试源文件；它没有阻止当前 1030/1030 构建，也不属于 shard 单体迁移。v1666 收口后会单独做对抗性热点复评：只有存在可验证、合同安全的修复路径才继续，不把无关改名塞进本版。

## 八、六种未来退化会怎样被抓住

第一种退化是“新建了 part，但忘记登记”。因为 CMake 只从 manifest 取附加源文件，这个文件不会进入 target；如果维护者同时从旧 part 删除对应断言，3710 census 会下降，架构门失败。若只是留下一个未跟踪文件，版本收尾的工作树检查也会暴露它。正确动作是先确定字段属于五个现有域中的哪一个，通常不应为少量新字段另建文件；只有现有 part 接近 1000 行且新边界确实独立时，才增加 manifest 行和入口。

第二种退化是“manifest 已登记、文件也编译，但入口没有调用”。`verify_suite` 会在 main 与全部 part 组成的调用图文本中查找唯一的 `entry(fixture);`。零次意味着悬空，失败；两次意味着重复执行风险，同样失败。这一检查补上了普通链接器看不见的问题：链接器允许一个函数从未被调用，但架构合同不允许测试 part 只占源码数量而不产生验证行为。

第三种退化是“绕开 wrapper 直接调用五个合同段”。这样可能让 current fixture 过全量、runtime 只过部分，或者反过来。两个 required-call 断言固定 current 与 result.response 都必须经过 `assert_shard_readiness_contract`。五段调用又只允许在 suite graph 中出现一次，所以维护者无法在 main 复制一套五段调用而不触发重复计数。唯一合法调度点仍是 wrapper。

第四种退化是“为让迁移通过而删掉难维护的字段断言”。即使剩余行为全绿，8/3710/2 精确 census 会首先失败。精确等值也意味着随手新增断言会失败；这不是禁止增强，而是要求功能版本明确说明新增了哪些合同字段，并同步提高 census。迁移版本必须零变化，功能版本可以有审计过的单向增长，不能把两种意图混在一个提交里。

第五种退化是“文件又长到 1001 行，然后给 baseline 加一项”。扫描器本身接受历史 baseline 的目的是让存量只减不增，但 v1666 的计划和文档已明确当前 baseline 为零。一次普通 code review 会直接看到从空表新增豁免；同时项目的童子军规则要求被改文件离开时达标。合理做法是在跨过门以前按字段域再拆，或者先提取共享断言 helper，不能把豁免当日常扩容按钮。

第六种退化是“修改 frozen fixture 或公共输出，让新组织方式更容易通过”。本版没有触碰 `fixtures/release` 和产品源码，release manifest focused test、完整 CTest、真实 CLI 与 git fixture diff 四层共同保护。未来若合同真的升级，必须是单独的合同版本，先冻结新 oracle、解释兼容关系，再改 formatter；测试架构版本没有权力重写历史证据。

## 九、以后新增 shard 字段时应走哪条路径

维护者先判断新增字段属于哪种证据。如果它描述候选材料、审批草稿或 profile，就进入 candidate part；如果描述不参与外部执行、拆分治理或 owner receipt，就进入 governance；slot、route preview、operator value 和验证归档进入 route；current JSON 对历史 release、prototype freeze、handoff 或 lifecycle 的解释进入 history；Node route catalog、cleanup continuity 与 boundary field path 进入 catalog。这个分类表比“把断言加到最短文件”更重要，因为后者会让文件大小均衡，却让概念重新混杂。

加入字段时应先在对应产品 formatter 的聚焦测试中证明行为，再在 shard 合同 part 增加精确断言。若 public fixture 按正式合同流程更新，先验证 formatter 与新 fixture 的整对象 parity；若 fixture 不该变化，就保持 git diff 为零。随后把 `assert_contains` census 从旧值提高到新值，并在 changelog/讲解中写出增加数量和字段域。只改 census 数字而没有可定位断言，或只加断言不更新 census，都会失败。

如果某一 part 接近 1000 非空行，拆分必须选新的概念边界。例如 catalog 可以按“兼容窗口”和“cleanup/field index”拆，而不应按第 500 行截断。新文件 family 仍由现有 manifest 和 checker 承载，不复制第二套 CMake helper。入口命名需小于 40 字符，文件名也受全仓命名门约束；如果名字需要串联十个名词才能表达范围，说明应先给那组字段提取一个短而明确的概念。

真实命令 smoke 的扩展也有固定位置。新增 SHARD 只读命令先在 `shard_command_tests.cpp` 检查 usage、COMMANDS/COMMANDSJSON 元数据、EXPLAINJSON/CHECKJSON 风险解释和真实 response；若它属于 current SHARDJSON 大合同，才再经过 wrapper。任何 smoke 都必须在空 Store 上证明预期大小不变，并明确 `mutates_store`、`touches_wal` 和 execution 边界，不能因为命令 category 标为 read 就省略副作用验证。

## 十、对抗性自审：这套结构还有哪些局限

最容易质疑的是 `active_json` 使用非拥有指针。这里没有把它包装成长期状态：wrapper 接收仍然存活的 `const std::string&`，设置指针后同步调用五段，最后立即清空；没有异步、线程、保存回调或跨函数返回使用。它的收益是八个 manifest entry 采用统一签名，checker 无需为不同函数编码第三列调用语法。若未来合同检查出现异常抛出并要求恢复状态，可以把它升级为小型作用域守卫；当前断言失败会终止测试进程，不存在捕获后继续使用脏指针的路径。

第二个质疑是 token census 不是 C++ 语法树。这个判断成立，所以 census 从来不是唯一证据。它负责回答“迁移源码数量是否变化”，编译器负责语法和链接，suite graph 负责入口可达性，focused shard 测试负责真实执行，full CTest 和 fixture parity 负责跨模块行为。多层证据各自解决一个问题；若只保留 token 数而不运行测试，它确实可能被注释或字符串欺骗，但当前 part 中的断言都要通过编译和实际调用。

第三个质疑是 1000 行阈值带有工程约定而非数学真理。它的价值不在证明 999 行一定优雅，而在给增长建立提前反馈。v1666 没有把五段强行做成相同大小，也没有继续拆 906 行 catalog，因为概念完整性优先于漂亮数字。阈值只阻止重新出现四千行单体；命名、职责、调用顺序和断言可达性仍由其他门和评审判断。

第四个质疑是拆分增加了八个 translation unit，冷构建步骤从 v1665 的 1022 增到 1030。这个成本真实存在，但只增加测试 target 的编译单元，没有增加 CTest 进程或产品库对象；换来的收益是增量修改只重编对应字段域、冲突范围更小、审阅定位更直接。对于一个已经有 361 个测试源文件的仓库，八个有明确职责的文件比一个 4328 行单体更符合维护成本曲线。

最后的停止判断同样重要。当前所有测试 `.cpp` 已低于门，baseline 为空，两个已知四千行热点都完成；继续把 700 或 800 行文件批量切碎不会自动提高质量。下一步只对构建中实际出现的长路径 warning 做独立复评，若合同 pin 或收益不足就记录并停止，而不是为了满足“持续优化”制造无终点的重构流水。

一句话总结：v1666 用五个有领域含义的合同 part、两段连续历史和一个真实命令场景，保住 3710 个字段断言与 fixture/runtime 双 oracle，同时把最后一个 4000 行级测试单体和最后一个体积豁免一起清零，使未来测试增长在进入代码库时就接受结构约束。
