# v1619 production-excellence K4 shard-readiness 拆分讲解

## 本版目标、角色和不是什么

v1619 是 mini-kv production-excellence K4 的第二个主闭环：上一版 v1618 已经把 `CommandProcessor` 的命令执行体拆成 string、expiry、persistence、introspection 四类 translation unit，本版则处理 K4 里另一块最容易继续膨胀的文件：`src/shard_readiness.cpp`。这个文件长期承担两类职责，一类是面向外部命令 `SHARDJSON` 的公开入口，另一类是把大量只读 evidence section、digest marker、边界字段、历史兼容段落串接成最终 JSON。前者应该小而稳定，后者天然会变长，如果继续把两者混在一个文件里，后续每次查 `fixture_path()`、查 digest、查 command catalog helper、查最终格式化入口，都要在数百行字符串拼接中定位，维护成本会越来越高。

本版的角色不是新增 shard 功能，也不是把只读预览改成真正路由。它只是一次 contract-preserving 的拆分：公开命令、fixture 路径、JSON 字段、digest 输入、测试期望、CLI/TCP 输出语义都保持不变。换句话说，v1619 不是 active shard router，不是写路由，不是 WAL 写入入口，不是 restore/load/compact 执行入口，也不是跨项目扫描器。Node 和 Java 仍只能把 mini-kv 的 shard readiness 当作冻结证据或只读回声材料来消费，不能因为本版文件结构变清晰了，就把它理解成 mini-kv 已经开始承担生产分片调度、订单权威、审计权威或执行审批职责。

为什么要在 K4 做这一刀？因为 K4 的主题不是“为了拆而拆”，而是先把两个高变动热点从巨型文件风险里拉出来。`command.cpp` 的风险在于命令执行逻辑太集中，已经影响命令族定位；`shard_readiness.cpp` 的风险在于证据聚合逻辑太集中，已经把公开入口、digest 链、边界 helper 和最终 JSON 拼装压在同一个 translation unit 里。K4 的正确终点不是继续追新功能，而是把这两块拆到可 review、可测试、可回滚的形态，然后停在 planner review checkpoint。v1619 完成后，K5 不能自动开始，因为 reviewer 需要确认拆分没有改变任何外部契约。

## 入口模块和文件职责

拆分后，`src/shard_readiness.cpp` 只保留公开入口中最稳定的一部分：`fixture_path()` 返回 `fixtures/release/shard-readiness.json`。这个文件现在是很薄的一层 shim，它的存在意义是让外部 includes 和链接关系继续通过 `minikv::shard_readiness` 命名空间访问公开函数，而不是让入口文件继续背负所有格式化细节。这个变化的一个好处是后续查看公开 contract 时不必穿过 600 多行聚合代码，入口文件本身就能表达“这里没有执行，没有运行时探测，也没有写路径”。

`src/shard_readiness_internal.hpp` 是本版新增的内部协议头。它不暴露给 include 目录，也不进入 public API；它只是让拆出去的 translation unit 共享几个内部 helper：`contract_version`、`release_version`、`json_string()`、`evidence_digest()`、`format_boundaries_json()`、`format_command_catalog_json()` 和 `format_command_catalog_quality_json()`。这里故意没有放更大的抽象，也没有把所有 evidence section 都包成对象树，因为当前项目的 SHARDJSON 仍然是历史字符串契约。过度抽象会让 byte-for-byte parity 更难判断，本版只提取必要的内部连接点。

`src/shard_readiness_core_digest.cpp` 承接原文件里 digest 证据链和 JSON 字符串转义 helper。digest 不是普通日志，它是 Node/Java/mini-kv 松耦合证据链里判断“这份 readiness 是哪个冻结来源、哪些历史 section 参与过”的核心标识。把它单独放出来之后，reviewer 可以集中检查 digest 输入是否仍然包含 `shardEnabled=false`、`slotCount=16`、`activePrototypePlanFreeze`、`liveReadGatePlanFreeze`、`runtimeExecutionApprovalGateInputPrecheck`、`boundaryCatalogMaintenance`、`slotTablePreview`、`shardRoutePreview` 以及后续大量 route preview / profile section / f-folder quality closeout marker。digest 文件仍然只调用已有的只读 formatter 和 digest marker，不读取 Node 文件，不启动服务，不接触存储目录，也不生成新审批材料。

`src/shard_readiness_boundary_validators.cpp` 承接原文件里三个小 helper：`format_boundaries_json()`、`format_command_catalog_json()`、`format_command_catalog_quality_json()`。这些 helper 的职责很集中：告诉控制面 `SHARDJSON` 是 read command，`mutatesStore=false`，`touchesWal=false`，`sideEffects=["metadata_read"]`，并继续记录 command catalog single source refactor 的质量边界。它们之所以单独成文件，是因为 boundary helper 和最终 JSON 拼装不同：前者说明命令如何被 catalog/控制面理解，后者负责把所有 section 拼成总响应。拆开后，未来如果 reviewer 只想确认“命令 catalog 是否仍然不写、不触 WAL、不突破 router”，可以只看这个文件和相关测试，而不用在完整 SHARDJSON 拼接中找几段短字符串。

`src/shard_readiness_json.cpp` 承接最终的 `format_json()`。它仍然是一个较长文件，因为 SHARDJSON 本身聚合了很多历史 evidence section，本版没有假装可以一刀把所有历史段落重塑成结构化 builder。更保守的做法是先把原有 public formatter 从入口文件中移出，把 include 依赖集中到这个“最终拼装”模块里，让 `fixture_path()`、digest helper、boundary helper 各自分离。这样既减少巨型入口文件，又避免一次性重写所有 section 的调用顺序。调用顺序对于 fixture parity 很敏感，本版保持原有顺序，靠 shard readiness 测试证明最终输出没有漂移。

这里还有一个工程判断：v1619 没有顺手把 `format_json()` 内部的大字符串拼接改造成新的 builder，也没有把所有 section 名称改成数组循环输出。原因不是这些方向没有价值，而是它们会改变 review 问题的性质。K4 的验收问题是“原来的巨大入口能不能被拆成更小职责，同时输出不变”，如果同时引入 builder、循环、字段排序工具或对象模型，reviewer 就必须重新判断 builder 是否会吞字段、改顺序、改变转义或让 diagnostics 顺序不稳定。mini-kv 当前有很多历史 fixture 和 Node 消费侧硬编码 digest/路径引用，贸然引入新的格式化策略会让“文件拆分”变成“输出系统重写”。本版只移动职责，不改变表达策略，是为了让风险边界和验收证据保持单一。

同理，本版没有为了追求表面上的小文件数量，把 `src/shard_readiness_json.cpp` 继续切成二十多个 section appender。真正可维护的拆分应该跟语义边界一致，而不是按行数平均切块。`format_json()` 现在仍然负责“总装配”，这是一种明确职责；digest 文件负责“证据链身份”，boundary validators 负责“控制面读法”，public shim 负责“稳定入口”。等 K4 review 通过以后，如果某个 section 家族继续成为高频修改点，再按 route preview、runtime gate、approval input、release catalog 等语义块逐步拆出去会更稳。现在先让 reviewer 能把一次迁移看完整，比在一个版本里制造过多小文件更重要。

## 核心流转：从命令到证据 JSON

运行时入口仍然来自 command catalog 和 command processor。用户在 CLI 或 TCP 中输入 `SHARDJSON` 时，命令分发侧会找到对应只读命令，然后调用 `minikv::shard_readiness::format_json()`。v1619 没有改变这条路径，没有增加额外参数，也没有把命令变成可执行操作。`format_json()` 的职责仍然是返回一个完整 JSON 字符串，里面包括顶层 contract、evidenceType、project、version、releaseVersion、readOnly、executionAllowed、shardEnabled、shardCount、slotCount、routingMode、evidencePath、status 等字段，以及大量历史 section。

新的模块边界把这条路径拆得更清楚：公开 shim 提供 fixture 路径；JSON formatter 决定响应整体结构；core digest 模块生成 evidence digest；boundary validator 模块生成 command catalog 和安全边界小段落；其他既有 shard preview、runtime gate、approval input、route catalog、boundary fields、history、summary section 模块继续提供各自 JSON 片段。因为本版没有修改那些既有模块，所以它不是又一轮新 evidence chain，而是把现有聚合器整理成可以继续维护的形态。

这种拆法保留了 C++ 链接层面的简单性。所有新文件都仍然进入 `minikv` 静态/目标库，`CMakeLists.txt` 只是在原有 `src/shard_readiness.cpp` 附近加入三个新的 `.cpp`。没有引入动态加载，没有添加新的第三方依赖，没有改变 include 目录，也没有让测试去依赖构建外的生成脚本。对调用者来说，`minikv::shard_readiness::format_json()` 和 `fixture_path()` 的符号仍然存在；对维护者来说，formatter、digest、boundary helper 的编辑位置终于分开。

## 证据结构和边界字段怎么理解

控制面最关心的字段不是“文件拆成了几个”，而是拆分后它读到的证据还能不能表达同样的边界。`readOnly` 和 `executionAllowed` 是第一层：前者为 true，说明这条证据命令只能读取/格式化元数据；后者为 false，说明它不能被解释成执行许可。`shardEnabled=false`、`shardCount=1`、`slotCount=16`、`routingMode=single-shard-readiness-prototype` 继续说明 mini-kv 只发布单分片预览和槽位样例，不安装 active router。

`boundaries` 小段落继续保留 `orderAuthoritative=false`、`auditAuthoritative=false`、`writeCommandsAllowed=false`、`adminCommandsAllowed=false`、`loadRestoreCompactAllowed=false`、`setnxexExecutionAllowed=false`、`multiProcessStarted=false`、`storageDirectoriesCreated=false`、`activeRouterInstalled=false`、`archivedNodeEvidenceMutated=false`。这些字段的价值在于把“不是生产执行入口”写成机器可读事实，而不是藏在文档说明里。v1619 把这段生成逻辑移到 `shard_readiness_boundary_validators.cpp`，但字段和值不变。

`commandCatalog` 继续声明 `SHARDJSON` 是 read 类命令，`mutatesStore=false`，`touchesWal=false`，`stable=true`，`extraArgsAllowed=false`，side effect 只有 `metadata_read`。这意味着无论 CLI 还是 TCP，调用者拿到的是只读描述，不会触发 WAL append、快照写入、compaction、restore/load 或后台服务启动。`commandCatalogQuality` 则继续把 v1546-v1607 command catalog single-source refactor 的质量结果带入 SHARDJSON：dispatch catalog split、contract catalog deduplicated、help/completion/key completion 从 catalog 生成，并声明 runtime command behavior、storage behavior、WAL behavior、router behavior、Node/Java 文件修改都没有变化。

本版还专门记录了 `src/shard_readiness_boundary_field_names.cpp` 的处理决定。playbook 允许在 K4 内转换 boundary field names static table，但前提是能够证明输出完全一致。这个文件是一个很大的边界字段名注册表，已有测试覆盖它的语义；如果在本版同时把它改成另一种 table representation，会把 K4 从“拆两个巨大入口”扩成“再迁移一个大字段注册系统”，review 面积会变大。v1619 因此把它作为 E9 维护迁移豁免记录下来：不是忘记处理，而是明确要求未来若要转换，必须单独做 byte-for-byte fixture proof。

## 测试保护了什么

本版最关键的测试不是只看编译过不过，而是看拆分后外部响应有没有漂移。focused CTest 里包含 `shard_readiness_tests`，这类测试保护当前 `SHARDJSON` formatter 与冻结 fixture 的一致性；只要 include 顺序、section 顺序、字段值或 digest 输入出现错误，fixture parity 会直接暴露。`shard_readiness_post_closeout_continuity_tests` 保护后续 closeout 和历史兼容段落没有因为移动文件而丢失。`readonly_fixture_tests` 和 `runtime_smoke_evidence_tests` 则继续检查只读 evidence、运行时 smoke 和 fixture 边界之间的关系没有被弱化。

同时我保留 command、TCP/RESP 相关 focused 测试，是因为 `SHARDJSON` 不只是一个内部函数，它还通过命令系统对外暴露。`command_tests` 保护命令分发和 catalog 语义；`tcp_resp_tests` 保护 TCP 入口仍然能通过真实 server/client 路径读到命令响应；全量 CTest 保护 WAL、snapshot、RESP、runtime evidence、治理证据链、历史 fixture 等旁路没有因为链接拆分而出问题。这些测试组合起来，证明本版是 translation-unit 级重构，而不是行为变更。

focused 测试负责快速盯住 K4 直接风险，全量 CTest 负责证明项目其余面没有被链接和 include 调整波及，两者缺一不可。

CLI smoke 选 `SHARDJSON`、`CHECKJSON SHARDJSON`、`SHARDROUTE sample-key` 和 `COMMANDSJSON`，用真实可执行文件确认命令目录仍能暴露 `SHARDJSON`，CHECKJSON 仍把它识别成只读元数据命令，route preview 仍是 preview-only，命令目录没有丢失。TCP smoke 则启动 `minikv_server.exe`，通过 socket 发送 `PING`、`CHECKJSON SHARDJSON`、`SHARDROUTEJSON tcp-key-v1619`、`QUIT`，确认网络入口的响应中仍能看到 `read_only=true`、`execution_allowed=false`、key routing 结果和正常断开。这比单测更接近用户使用路径，也能证明拆分没有只在库层面成立。

格式和 diff hygiene 也有意义。v1619 清理了本轮新增 shard 文件开头的 UTF-8 BOM，让新代码保持 ASCII/UTF-8 no-BOM 的项目惯例；`clang-format --dry-run --Werror` 保护新增 C++ 文件风格，`git diff --check` 保护文档和源码不引入 trailing whitespace。K4 前面已经装好的 changed-file clang-format CI gate 会在远端继续检查本轮改动，这也是 K0/K1/K2/K3 建起来的工程护栏现在开始产生复利的地方。

## 维护收益和后续边界

v1619 后，维护者查 shard readiness 时可以按问题入口分流：想看公开 contract，先看 `src/shard_readiness.cpp`；想看总响应拼装，看 `src/shard_readiness_json.cpp`；想看 digest marker 和历史 evidence 输入，看 `src/shard_readiness_core_digest.cpp`；想看只读边界和 command catalog helper，看 `src/shard_readiness_boundary_validators.cpp`；想看这些内部函数如何互相连接，看 `src/shard_readiness_internal.hpp`。这比原来一个文件内混合所有职责更容易做 review，也更容易在未来发现“这次改动到底是在改证据结构，还是只是在移动代码”。

本版也给后续 K5 以后留下更清晰的停止线。K4 到这里应该停下 review，而不是立刻继续拆所有历史 section。原因很简单：command 侧和 shard readiness 侧都是高风险入口，已经完成主要结构拆分；继续扩大会把 review 从一个可验证的 milestone 变成一串难以审完的文件搬迁。下一步应由 planner 确认 K4 是否通过：检查 v1617-v1619 的提交、归档、测试、CI、coverage gate、boundary-field-name 豁免说明，再决定 K5 的范围。

对 Node/Java/mini-kv 松耦合融合来说，本版的价值是：mini-kv 继续提供同一份只读、不可执行、不可写、不可路由的 shard readiness 证据，但内部维护面已经从巨型入口文件拆成可审查模块，后续 Node/Java 消费这份证据时不需要承担 mini-kv 文件膨胀带来的契约不确定性。
