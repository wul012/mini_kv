# v1652 代码讲解：legacy resolver 四回执有序 builder 迁移

## 一、本版目标、角色定位和明确不做事项

v1652 消费 v1651 已冻结的迁移前 oracle，把 v117 至 v120 四份 credential resolver 历史回执从巨型字符串拼接迁移到零依赖 ordered JSON builder。四份回执依次表达 fake-shell 归档、生产就绪阻塞决策、实现前计划摄入和禁用实现候选评审。它们共同描述一个控制面决策链，但每一阶段的正向状态不同，因此本版的核心不是把四份 JSON 套进同一个模板，而是让每份 formatter 的字段顺序、子对象边界和关闭能力都变成可读、可测试、可单独维护的 C++ 数据。

本版不是 credential resolver 实现，不新增命令或路由，不实例化 secret provider、resolver client、fake runtime 或 transport，不读取 credential value，不接受、解析或渲染 raw endpoint URL，不发送 HTTP/TCP，不连接 managed audit，不写 Store、WAL、approval ledger、schema 或 production record，不执行 LOAD、COMPACT、RESTORE、SETNXEX、SQL、deployment 或 rollback，不自动启动 Node、Java、mini-kv，也不取得 audit authority 或 order authority。四个 public formatter 名称、digest 函数、输入参数、release fixture、manifest 字段和下游消费入口全部保持不变。

## 二、为什么选择这四个 owner 一次迁移

v1650 结束后的机械 census 是 27 个 formatter owner 中 14 个 builder-backed、13 个 pending，另有一个没有目标 formatter 的命名 waiver。Claude review 点设在 18 个 builder-backed，因此下一批需要恰好迁移四个 owner。仅按文件最小或字符串最短来凑四个数，会把不相关 schema 混在一起，既难讲清，也会扩大回归面。

v1651 对 pending 列表做了重新分类。最终选择 `runtime_credential_resolver_archive_receipts.cpp`、`runtime_credential_resolver_decision_receipts.cpp`、`runtime_credential_resolver_plan_receipts.cpp` 和 `runtime_credential_resolver_disabled_candidate_receipts.cpp`，因为它们对应 mini-kv v117、v118、v119、v120 连续历史版本；每个文件只有一个目标 public formatter；每份对象都有独立 release fixture 和领域测试；四个文件都来自尚未引入统一 builder 的旧实现时期；它们的业务关系也能形成一条完整叙事：证据已归档，不等于生产就绪；生产仍阻塞，才需要定义计划；计划边界齐全，也只允许进入禁用候选评审，而不是直接获得执行权。

没有选择 `runtime_evidence_receipts.cpp` 或 `runtime_sandbox_receipts.cpp`，因为它们包含多个 formatter，单次 owner 迁移会触碰更宽的输出面。没有把 input-hardening 等较新 receipt 混进来，因为它们属于另一条 prerequisite 链。也没有把 execution-denied retention 文件计为迁移成果，因为 census 已证明它不拥有目标 formatter，只能继续作为唯一具名 waiver。

## 三、输入、输出与不可变化的外部合同

四个 formatter 的直接输入都是 `const std::vector<std::string>& read_commands`。当前真实调用传入 `INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON`，formatter 只计算稳定的 read-command digest，并不执行这些命令。其他输入来自编译期常量、`version`、binary provenance、retention check 和 replay marker digest。它们都是已有证据值，不是 credential 或 endpoint 数据。

输出仍是一段 compact JSON 字符串。上游含义来自 Node v264 至 v273 的历史只读合同，mini-kv 只负责回显 non-participation 证据；下游由 standalone fixture test、SMOKEJSON、runtime-smoke-evidence 和 verification manifest 消费。输出必须同时满足三层不变性：第一，完整对象字节和字段顺序不变；第二，receipt digest 输入及结果不变；第三，`read_only`、`execution_allowed`、credential、endpoint、network、write、restore 和 authority 等边界含义不变。

## 四、旧实现的问题究竟是什么

旧 formatter 不是无效代码，它们多年通过测试，输出也与 fixture 对齐。问题在于结构隐藏在字符串语法里。一个 `return` 表达式同时负责对象开闭、逗号位置、字段名、字符串转义、数字格式、嵌套对象拼接和几十个布尔边界。阅读者看到 `+ field_string(...) +` 与跨行字面量时，必须在脑内重建 JSON 树；新增字段容易放错逗号或插入位置；重复的 `format_*_boundary_json()` 返回无外层大括号的字段碎片，只有拼到顶层字符串中才是合法 JSON；编译器无法表达“这个值是完整对象”还是“这只是若干字段”。

更重要的是，旧形式把业务阶段差异藏在相似文本中。v118 的 `ready_for_credential_resolver_pre_implementation_plan=false` 与 v119 的同名 true 都是正确的，但在长字符串里很容易被复制粘贴覆盖。v120 的 `ready_for_disabled_resolver_interface_candidate=true` 也只表示允许评审接口形状，不表示允许真实 resolver。结构迁移的价值，是让这些差异出现在具名字段表和具名子对象中，而不是让维护者依赖全文搜索理解。

## 五、ordered builder 的机理

内部 `runtime_receipt_json_builder` 提供 `json_string`、`json_bool`、`json_integer`、`json_object`、`json_array` 和 `OrderedJsonField`。每个字段由名称和已经编码的 JSON value 组成，`json_object` 按 vector 或 initializer list 的原顺序输出 compact JSON。因此迁移不是先解析再重新排序，也不是使用会改变 key order 的 map；代码中字段出现的顺序就是 release fixture 要求的顺序。

字符串仍委托已有 `runtime_evidence::json_string` 转义，digest 则统一复用 `runtime_credential_resolver_receipt_utils.hpp` 中的 `DigestPart` 和 `receipt_digest`。这删除了四个文件各自复制的 `field_string` 与 `receipt_digest` 薄包装，但不改变底层算法。顶层最终都使用 `json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()})`，因此编译器能明确区分字段集合与最终对象。

对于 archive files、candidate decisions、checks 等大而语义完整的历史表，本版保留原 helper 返回的完整 JSON value。它们本身有清楚的对象或数组边界，也受完整对象 parity 保护；把它们逐项重写只会扩大 diff，并不会解决顶层所有权问题。相反，旧的“无大括号边界碎片”全部改为向 `std::vector<OrderedJsonField>` 追加具名字段，不再依靠 strip、splice 或逗号桥接。

## 六、字段所有权、生命周期与 raw value 信任边界

`OrderedJsonField::name` 是 `std::string_view`，本版传入的名称全部是编译期字符串字面量，因此在最终 `json_object` 调用完成前始终有效。`value_json` 则是拥有内存的 `std::string`：`json_string`、`json_bool`、`json_integer` 和每个具名子对象都先生成完整值，再移动或复制进 fields。最终序列化发生在所有局部值仍存活的函数作用域内，不存在指向临时字符串内容的悬空引用。这个所有权划分比旧拼接更容易检查，因为字段名和字段值的生命周期责任分开且固定。

builder 允许把已经编码的 JSON 作为 raw value 放入对象，这是一条需要明确的信任边界。v1652 只把当前文件内部、返回完整对象或完整数组的 helper 交给该入口，例如 archive files、checks、boundary codes 和 candidate decisions；不会把用户输入、credential、endpoint 或任意外部文本当作 raw JSON。所有动态字符串仍必须经过 `json_string`。如果未来 helper 开始接收外部数据，应先改为结构化 builder 或增加独立解析验证，不能沿用“历史 helper 已可信”的假设。

builder 会为各子对象和 value 产生若干短生命周期 `std::string`，相比单次长字符串拼接可能多出少量分配。但这些 receipt 只在 evidence 命令和 fixture 验证路径生成，不位于 Store GET/SET 或 RESP 请求的高频热路径；输出本身已有一万到一万五千字节，清晰的 schema 所有权比微小分配差异更重要。若将来 profiling 证明这里成为热点，可以在 builder 内部做统一容量预估，而不应退回手工逗号拼接。当前没有性能证据支持为四份历史回执增加缓存、全局状态或复杂 allocator。

如果修改的只是 C++ 变量名、子对象局部变量或 helper 拆分，v1651 baseline 就应继续通过，不需要也不允许更新 fixture。若出现差异，说明所谓内部改名实际改变了字符串值、key order、转义或对象层级。用这种方式，测试不是为实现背书的截图，而是能否继续声称“外部合同不变”的裁判。

## 七、v117 fake-shell archive formatter

archive 文件现在先构造五个具名块：`source_node_v264` 记录 test-only fake resolver shell contract；`source_node_v265` 记录 Java v107 与 mini-kv v116 upstream echo verification；`archived_evidence` 聚合 archive roots、source versions、文件表和 snippet group；`archive_verification` 保存两份 source digest、route 与 no-rerun 判定；`summary` 保存 counts。warnings、recommendations 和 next required echo versions 使用 `json_array` 明确表达。

顶层字段仍从 receipt version、fixture path、source 与版本信息开始，随后放 source readiness、digest、两个 source 对象、归档证据、checks、summary 和 provenance，最后追加 archive 专属关闭边界。`archive_files_read_by_mini_kv=false` 与 `archive_verification_reruns_fake_shell_behavior=false` 特别重要：它们说明 fixture 证明的是归档合同，而不是 mini-kv 在运行期读取 Node 归档或重放 fake shell。

## 八、v118 blocked decision formatter

blocked decision 文件把 `source_node_v267`、`production_readiness_decision`、`summary` 和 `evidence_endpoints` 分开。`pre_implementation_requirements`、blocker codes 和 checks 继续作为完整领域 helper 嵌入。生产决策对象明确保留 `decision=blocked`、十项 requirement 缺失和十个 production blocker，并把 real resolver、secret provider stub/runtime、credential value、raw endpoint、external request、managed audit connection、schema migration、ledger write、auto-start 全部设为不允许。

本文件的边界追加函数保持 `ready_for_credential_resolver_pre_implementation_plan=false`。这与下一阶段并不矛盾：v118 记录的是计划尚未形成时的历史事实。若为了复用 v119 helper 把它改成 true，完整对象可能仍是语法合法 JSON，但历史语义会被破坏。正因如此，本版没有跨文件抽取一个接受布尔参数的万能 boundary builder。

## 九、v119 pre-implementation plan intake formatter

plan intake 文件把 `source_node_v269_reference`、`pre_implementation_plan`、`plan_intake`、`summary` 和 `evidence_endpoints` 命名。十项 requirement 在 v118 是缺失项，在 v119 变成 `defined-for-review` 的边界：plan document、credential handle、endpoint handle、disabled secret provider stub、operator approval、rollback、redaction、external request simulation、schema migration policy 与 audit ledger write policy 都已定义。

这里必须区分“边界定义完整”和“运行许可打开”。所以 `ready_for_credential_resolver_pre_implementation_plan=true`，但 `real_resolver_implementation_allowed=false`、`secret_provider_runtime_allowed=false`、`credential_value_read_allowed=false`、`raw_endpoint_url_parse_allowed=false`、`external_request_allowed=false`、`schema_migration_allowed=false` 和 `approval_ledger_write_allowed=false` 仍同时存在。控制面可以读懂计划，运行面仍完全关闭。

## 十、v120 disabled implementation candidate formatter

disabled candidate 是四份中信息最厚的一份。迁移后 `source_node_v272_reference` 负责说明 v119 plan intake echo 已对齐；`interface_shape` 单独列出 handle-only request、response 字段和六类 failure class；`fake_wiring_review` 明确 fake runtime 未实例化、真实 secret provider 和 managed audit transport 不允许；`disabled_implementation_candidate` 聚合十项 disposition、四项 candidate-ready、六项 approval-required 以及前述两个子对象。

`candidate_ready` 不是 production ready。四项 ready 只表示 plan document、disabled stub 设计、redaction policy 和 external request simulation 可以继续做禁用态评审；credential handle、endpoint handle、operator approval、rollback、schema migration 和 ledger write 六项仍需审批。顶层继续保持 `credential_resolver_implemented=false`、`credential_resolver_invoked=false`、`execution_allowed=false`、`external_request_sent=false` 和全部写入/恢复/authority 字段 false。

## 十一、为什么四个边界 appender 仍分别存在

四份 receipt 的安全尾部看起来相似，但并非同序同义。archive 有 archive-files-read 与 fake-shell-rerun 字段；blocked decision 有 plan-not-ready 和 secret-provider-stub-not-defined；plan intake 有 plan-ready 但 runtime-disabled；disabled candidate 还增加 reads/stores credential、approval ledger write executed 与 audit authority。强行共用一个函数，要么需要大量布尔参数和插入位置参数，要么产生一个比原文件更难维护的 schema engine。

本版选择四个无参数、本地所有的 append 函数。每个函数只描述当前 receipt 的历史 schema，调用位置也对应 fixture 中的原始字段顺序。重复的若干 false 值是合同差异的显式成本，不应为了减少物理行数而隐藏。以后只有当两个或更多 family 出现完全同名、同值、同序、同插入点的稳定连续字段组时，才适合像 v1649/v1650 一样抽成无参数 shared helper。

## 十二、测试如何证明不是“重写后看起来一样”

第一道门是 v1651 新增的 `runtime_receipt_legacy_resolver_chain_baseline_tests`。它在生产迁移之前已经提交、打 tag、推送并通过 CI，因此 v1652 不能反向修改 oracle。四份 fixture/runtime 分别固定为 12335/10623/11114/15160 字节、95/99/101/106 个顶层字段和四个 v1651 前缀摘要；完整字符串必须相等，`\\u0027` 次数双方都为零。失败时打印首个不同字节与上下文，能快速区分 key order、缺字段、转义或逗号问题。

第二道门是四个领域 receipt test。它们保护每份 standalone fixture 的 digest、版本、source counts、阶段专属字段和 no-side-effect 语义。第三道门是 `receipt_builder_census_contract`，它扫描 formatter owner 是否真正使用 builder，并把 floor 从 14 收紧为 18。测试文件增长、include 伪装或 waiver 增长不能替代 production formatter 迁移。

第四道门是 SMOKEJSON、runtime smoke evidence 与 release verification manifest。它们证明四份对象仍通过三个既有消费表面暴露，而不是只在 formatter 单测里存在。最后是 full CTest，覆盖 Store、WAL、snapshot、RESP、OSFS、所有历史 receipt 和文档契约；真实 CLI 只运行 SMOKEJSON 与 CHECKJSON，不运行 LOAD，验证 `touches_wal=false` 后正常 QUIT。

## 十三、census、文件体量与质量判断

机械结果从 14/13/1 收紧为 18/9/1：28 个 receipt source、27 个 formatter owner、18 个 builder-backed、9 个 pending、1 个命名 waiver。floor 直接写入 CMake test 参数，未来任何一个迁移文件退回旧拼接都会使 CI 失败；pending 列表也由脚本打印，评审者可以复现，不依赖进度表自述。

按包含空行的物理行统计，四个文件从 311、298、301、408 行变为 396、371、390、490 行，合计增加 329 行。这不是 shrink，也不应包装成 shrink。增加来自将原来压在少量超长字符串行中的 95 至 106 个顶层字段、多个嵌套对象和安全边界展开为一字段一职责。最长文件仍低于 500 行，没有形成新的巨型源文件；复杂历史数组继续留在小 helper 中，顶层 formatter 则能按对象名导航。维护指标应同时看字节 parity、圈定职责、最大文件风险和重复抽象质量，而不能只奖励行数下降。

## 十四、发生漂移时应怎样定位

若 baseline 报首个差异位于对象开头，先检查顶层 fields 顺序；位于子对象附近，检查具名 object 的字段顺序和原 helper 插入位置；大小相同但 digest 不同，通常是一个字符值或 key order 改变；字段数变化则检查是否漏加、重复添加或把 nested field 错算成 top-level。任何情况下都不允许修改 fixture、expected size、expected digest 或 canonical 规则来迁就生产代码。

如果四个 domain test 通过但 census 失败，说明 formatter 输出正确但结构门未识别，需检查 production source 是否真正引入并调用 ordered builder；不能降低 floor。若下游 SMOKEJSON/manifest 失败，检查 aggregator 入口和完整对象嵌入，而不是修改 receipt 内容。若真实 CLI 出现写命令计数、WAL 触碰或网络行为，则本版应立即回退，因为结构迁移没有任何理由改变执行面。

## 十五、对抗式自审：最可能被质疑的三点

第一项质疑是“代码行增加三百多，是否反而更臃肿”。答案不能只说可读性更好，而要看具体结构：旧代码把九十多个字段压在一个表达式中，物理行少但认知复杂度高；新代码让每个 key、value 类型和 nested object 所有权可定位，最大文件仍低于五百行，且没有新增公共抽象层。若未来同一文件继续新增业务字段并越过大文件风险阈值，就应把 source reference、candidate schema 或历史表拆到独立 translation unit，而不能用本次“结构化增长”理由无限放宽。

第二项质疑是“四个 boundary appender 有重复，为什么不立即去重”。本版逐字段比较后发现，它们在 readiness、secret-provider stub、credential reads/stores、approval-ledger executed 和 authority 字段上都存在差异，且差异正是历史阶段语义。带几十个参数的通用 helper 会把差异重新藏起来，也可能把 v118 false 误传成 v119 true。当前局部重复比错误抽象更容易审查；后续若 census 剩余 owner 中出现完全相同的连续尾部，可以用独立版本提取无参数 helper，并继续由 exact-byte 门证明。

第三项质疑是“既然 formatter 独立，为何还要跑全量 CTest”。四个源文件最终链接进共享 `minikv` 静态库，SMOKEJSON、runtime evidence、manifest 和多个 command test 都通过同一聚合路径消费它们；builder 与 receipt utils 也被其他 family 共用。聚焦测试能快速定位 parity，不能证明共享链接、完整 CLI、OSFS、WAL/snapshot 或文档门没有被构建系统变化间接影响。全量测试不是每改一行都重复执行，而是在四文件批次完成、文档稳定后执行一次最终门；这与先逐文件跑 2/2、最后再跑全量的节奏相匹配。

对抗审查后的结论是：本版最强的交付证据不是“用了 builder”或“写了很多行”，而是 v1651 独立提交的旧输出 oracle 在四个生产文件重写后仍逐字节通过，census 又机械证明四个 owner 确实完成结构迁移。任何一边缺失，review 都不应判定完成。

## 十六、一句话价值

v1652 把一条历史 resolver 决策链从“只能靠长字符串和 fixture 守住”升级为“字段所有权、阶段差异和关闭边界都可直接审查，同时完整字节合同不变”，让 Node、Java、mini-kv 后续松耦合消费得到更可维护的只读证据，而没有提前打开任何 credential、network、write 或 execution 能力。
