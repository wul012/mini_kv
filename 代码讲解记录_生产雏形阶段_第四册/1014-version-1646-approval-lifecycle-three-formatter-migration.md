# v1646 代码讲解：approval lifecycle 三份 formatter 有序迁移与共享边界收敛

## 一、本版目标、角色定位，以及明确不是什么

v1646 的目标不是增加一条新命令，也不是让 mini-kv 参与审批、凭据解析或人工审核。它处理的是一个很具体的维护问题：三份 approval lifecycle receipt 已经拥有稳定的 public formatter、冻结 fixture 和下游消费面，但它们仍靠数百段字符串相加来组装 JSON。字符串拼接能工作，却把字段层次、标量类型和顺序隐藏在引号与逗号之间；维护者只想增加一个布尔边界时，也必须重新审查整段 JSON 是否漏逗号、重复字段或打乱嵌套关系。

本版的角色是“合同保持型重构”。它把三份已经由 v1645 证明可迁移的 formatter 改为内部 ordered JSON builder，保留 public 函数、字段名称、字段顺序、compact whitespace、digest 输入、fixture 字节和运行时边界。三份目标分别是 approval-required boundary、approval-prerequisite artifact intake、human approval artifact review。第四份 post-echo receipt 没有进入本版，因为 v1645 已经机械证明它除了命名 apostrophe 差异之外，还存在一处 `} },` 与 `}},` 的额外空白漂移；把它混进来会让结构迁移和行为修正失去边界。

因此，本版明确不是什么：不是新的 router；不是审批执行入口；不是 secret provider 或 resolver client；不读取 credential value，不解析 raw endpoint URL，不发 HTTP/TCP；不写 Store、WAL、approval ledger 或 managed-audit 状态；不执行 schema migration、LOAD、COMPACT、RESTORE 或 SETNXEX；不自动启动 Node、Java 或 mini-kv；也不让 mini-kv 成为 audit authority 或 order authority。它只改善既有只读证据的构造方式。

## 二、入口在哪里，调用者看到什么

三个 public 入口仍在 `minikv::runtime_evidence_receipts` 命名空间中：

- `format_credential_resolver_approval_required_boundary_non_participation_receipt_json`；
- `format_credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_json`；
- `format_credential_resolver_human_approval_artifact_review_non_participation_receipt_json`。

调用者继续传入真实只读命令清单，例如 `INFOJSON`、`STORAGEJSON`、`HEALTH` 和 `STATSJSON`。formatter 会把命令清单 digest、binary provenance、retention check、retention replay marker 以及 receipt 自身 digest 放进结果。上层 `SMOKEJSON`、runtime smoke evidence 和 release verification manifest 仍调用同一 public 函数，所以本版没有另开旁路，也没有要求下游切换 schema。

返回结构仍是 compact JSON object。最前面是 receipt 身份和来源，例如 `receipt_version`、`receipt_fixture_path`、`source_*`、`consumer_hint`、当前 project/release/artifact 信息；中间是 Node 来源引用、artifact 或 review packet、necessity proof、checks、summary、warnings、recommendations 和 evidence endpoints；随后是 provenance/read-command/receipt digest；最后是大组闭合边界字段，再以 `boundary` 和 `node_action` 收束。顺序本身属于冻结证据的一部分，不能因为 JSON 语义上“对象无序”就任意重排。

## 三、ordered JSON builder 的机械职责

内部入口是 `src/runtime_receipt_json_builder.hpp/.cpp`。它只提供五类很小的能力：`json_string` 负责沿用项目现有转义规则，`json_bool` 和 `json_integer` 负责显式标量编码，`json_object` 按 `OrderedJsonField` 的输入顺序渲染字段，`json_array` 按输入顺序连接已经编码的元素。`OrderedJsonField` 只有字段名和 `value_json` 两项，没有 schema 推理、默认值、字段排序或业务规则。

这种“已编码值 + 有序字段”的设计刻意保持简单。receipt 中许多领域数组，例如 required fields、prohibited fields、rejection reasons 和 checks，已经有本文件自己的稳定 helper。v1646 不同时重写这些 helper，而是把它们返回的完整 JSON 当作已经编码的 value 放入 ordered object。这样每一刀只负责消除顶层和主要嵌套对象的手工逗号风险，不会把领域数据重写、builder 扩展和 fixture 迁移塞进同一版本。

最终顶层对象不能直接使用固定 initializer list，因为闭合边界字段数量很多，并且必须插入在 receipt digest 之后、`boundary` 之前。因此三个 formatter 都先创建 `std::vector<OrderedJsonField> fields`，按历史顺序加入固定部分，再调用边界追加函数，最后加入 `boundary` 与 `node_action`，通过 `json_object(std::span<const OrderedJsonField>{...})` 一次渲染。这里的 `std::span` 只提供不拥有内存的连续视图，不复制字段，也不改变生命周期。

## 四、approval-required boundary 的结构化流程

approval-required boundary 是三份中较早、结构也不同的一份。它描述六个仍需审批的边界：credential handle、endpoint handle、operator approval、rollback boundary、schema migration policy 和 audit ledger write policy。v1646 先构造 `source_node_v274_verification`，其中保留 check count、candidate decision count、request/response/failure 数量、candidate digest 以及 Java/mini-kv 对齐标志；再构造 `approval_required_boundaries`，保持 boundary count、code array 和 detail array 的原顺序。

随后单独构造 summary、warnings、recommendations 和 evidence endpoints。这样阅读代码时可以直接回答“哪一段对应 Node v274 验证”“哪一段列出六个审批边界”“哪一段说明下游版本”，不必从一条跨一百多行的字符串里寻找花括号闭合点。其闭合字段集合比后两份更早，也没有完整的 disabled runtime-shell packet 语义，因此本版保留本地 `append_approval_boundary_non_participation_flag_fields`。这是有意的所有权边界：相似不等于相同，不能为了少写几行把不同合同强行塞进万能模板。

## 五、artifact intake 与 human review 为什么既共享又分开

approval-prerequisite artifact intake 负责回显 Node v306 的非秘密 artifact 合同。它的 `artifact_intake_plan` 包含 artifact name/version、intake mode、source span、digest、required/prohibited fields、rejection reasons、no-go boundaries 和 upstream echo requests。`necessity_proof` 解释为什么 v305 不能直接复用；`source_node_v306_summary` 保存数量口径；`source_node_v306_reference` 把这些子对象连到 Node 来源状态。另一份 `source_node_v305_reference` 仍单独存在，用来说明上一个验证阶段只证明 prerequisite gate 仍受阻，并未授权 runtime shell。

human approval review 负责回显 Node v308 的人工审核 packet。它与 artifact intake 形状相近，但多出 missing-field checks，并拥有 packet name/version/review mode 等自己的字段；来源链也是 Node v308 加 Node v307，而不是 v306 加 v305。v1646 因此复制“构造命名子对象的模式”，没有复制“领域字段表”。`review_packet`、`source_node_v308_summary`、`source_node_v308_reference` 和 `source_node_v307_reference` 都由本文件自己维护，避免未来在一个合同里加字段时误改另一个合同。

两份文件真正完全重复的是顶层 receipt digest 之后的大段 disabled runtime-shell 封闭边界：runtime shell 未实现、未启用、不可调用；resolver/provider/client 未实例化；credential value 未接受、读取、提供、加载、存储、包含或渲染；raw endpoint 未接受、解析或渲染；external request 不允许也未发送；managed audit、Store/WAL、approval ledger、schema migration、restore/load/compact、SETNXEX 和 auto-start 全部关闭；audit/order authority 均为 false。v1646 将这一完全相同、顺序也相同的后缀提取到 `credential_resolver_detail::append_closed_runtime_shell_boundary_fields`。每个 formatter 仍先追加自己的 receipt-family 字段，再调用共享 helper，因此共享发生在真实重复处，不吞掉 family identity。

## 六、关键边界字段应该怎样理解

`read_only=true` 表示这份输出是观察和证明，不是执行请求。`execution_allowed=false` 是更强的显式否定：即使某个消费者读到完整 packet，也不能据此调用 runtime shell、连接外部系统或执行恢复命令。`credential_resolver_implemented=false` 与 `credential_resolver_invoked=false` 分别否定“实现存在”和“实现被调用”，避免只用一个模糊字段掩盖状态。

`credential_value_read_allowed=false`、`credential_value_read=false`、`credential_value_stored=false` 和 `credential_value_included=false` 从许可、观察、持久化、输出四个角度关闭秘密数据链。raw endpoint、external request、HTTP/TCP 也采用同样思路：既不接受原始 URL，也不解析、渲染或发起连接。`storage_write_allowed=false`、`write_commands_executed=false`、`runtime_write_observed=false` 与 `approval_ledger_written=false` 共同证明没有把“只读 receipt 重构”偷偷变成写路径。

`restore_execution_allowed=false` 和 `load_restore_compact_executed=false` 说明 fixture、manifest 或 CHECKJSON 中出现 LOAD/RESTORE 字样不等于执行过命令。`node_auto_start_allowed=false`、`java_auto_start_allowed=false`、`mini_kv_auto_start_allowed=false` 说明跨项目证据链仍由操作者控制，不会因为读取 receipt 自动拉起服务。`audit_authoritative=false` 与 `order_authoritative=false` 最后划清职责：mini-kv 提供存储与恢复侧证据，但不裁决审计审批，也不成为订单权威。

## 七、fixture parity 与 canonical spelling 如何保护迁移

v1645 已经在生产代码修改前冻结三份候选的 fixture 子对象大小、FNV digest 和顶层字段数。approval-required boundary 是 12,949 字节、108 个顶层字段，fixture 与 runtime formatter 完全相等。approval-prerequisite artifact 是 16,483 字节、107 个字段；human approval review 是 17,116 字节、107 个字段。后两份分别有一处历史 fixture `\u0027` 与当前 runtime 直接 apostrophe 的差异，而且每个 family 都有独立 legacy/canonical 常量。

测试中的 `named_apostrophe_runtime_surface_canonical_json` 不是通配 normalizer。它先断言 legacy 短语在 fixture 中恰好一次、canonical 短语不存在，再只替换指定短语；runtime 侧必须恰好出现一次 canonical 短语且不能出现 legacy 拼写。替换后整份对象必须相等。v1646 没有修改该 helper、常量、fixture 或期望值，所以通过测试意味着 builder 恰好复现了迁移前 runtime surface，而不是测试迁就了新代码。

post-echo 的 `} },` 差异也说明为什么规则必须窄。它不是 apostrophe 的另一种写法，而是独立的 compact whitespace 漂移。v1645 将其 `migration_ready` 设为 false，v1646 没有修改该状态或源文件。下一版必须先决定 runtime 应回到 compact `}},`，并用专门证据证明这是错误修正；在此之前，它不能借用现有 apostrophe 规则。

## 八、census ratchet 与测试分别保护什么

`receipt_builder_census_contract` 运行 `cmake/check_receipt_builder_census.cmake`，扫描 28 个 `runtime_*_receipts.cpp`。它区分 27 个拥有 public formatter 的 owner 和唯一没有 formatter 的 retention waiver，并检查直接包含 builder header 的文件数。v1646 把 floor 从 5 提到 8；迁移完成后实际也是 8，pending 从 22 降到 19。这个门不是进度表自述：若未来有人退回字符串实现、移除 builder include、增加新 formatter 却不记账，CTest 会真实失败。floor 只能收紧，不能为了过 CI 调低。

`runtime_receipt_json_builder_tests` 保护 fixture 子对象字节、大小、digest、顶层字段数、canonical 规则和核心边界。三份领域测试保护各自 standalone fixture 与 formatter 合同。`smokejson_command_receipt_tests` 保护真实命令聚合面，防止内部 formatter 正确但 SMOKEJSON 漏字段。`runtime_smoke_evidence_tests` 保护运行时证据文件中的嵌入，`release_verification_manifest_tests` 保护发布清单引用。focused lane 8/8 通过后，完整 345/345 CTest 又覆盖 Store、WAL、snapshot、RESP/TCP、OSFS、shard readiness、archive 与其他 receipt family，证明共享 header 没有产生跨模块回归。

真实 CLI smoke 再补一层不同证据。输入 `SMOKEJSON` 后，输出仍含 `read_only=true` 与 `execution_allowed=false`；输入 `CHECKJSON LOAD data/prod.snap` 后，系统只报告 `command=LOAD`、`write_command=true`、side effect 分类和 `touches_wal=false`，随后 `QUIT` 返回 `BYE`。这证明测试之外的实际可执行程序仍把 CHECKJSON 当静态分析入口，没有执行 LOAD、替换 Store 或写 WAL。

## 九、维护收益与下一步

迁移后，三个目标文件不一定比旧文件更短。结构化 JSON 会把原来藏在字符串中的每个字段显式占一行，物理行数可能增加；这不应被误判为失败。真正的收益是：嵌套对象有名字，整数和布尔值有类型，字段顺序能直接审阅，逗号和转义交给同一 builder，重复边界后缀只有一个维护点，而 fixture parity 给出了逐字节的回归裁判。维护者以后可以在局部对象里定位变化，不必重新解析整条字符串。

下一版应只处理 post-echo 漂移：先冻结并解释 `} },` 的来源，决定是否把 runtime 修正为 compact `}},`，确认 canonical fixture 与 runtime 在唯一 apostrophe 规则下相等，再考虑迁移 formatter 并把 census floor 从 8 提到 9。若仍出现第二处未解释差异，应立即停版，不应增加宽泛 waiver。

## 十、为什么没有继续抽象成通用 receipt schema

看到三份 formatter 都在构造来源、summary、warnings 和边界字段，一个自然想法是再设计一套通用 receipt schema：传入版本、来源和若干计数，框架自动生成全部 JSON。v1646 没有这么做，原因不是缺少抽象能力，而是这里的重复大多只是外观相似。approval-required 描述六类审批边界，artifact intake 描述非秘密 artifact 的接收合同，human review 描述人工审核 packet；三者的来源版本、计数口径、嵌套对象、缺失字段、拒绝理由和 no-go 语义并不相同。若用一个带几十个 optional 参数的万能结构承载，字段所有权会从具体 receipt 文件转移到难以审查的中央模板，新增一个 family 字段还可能影响其他输出。

本版采用较窄的复用准则：只有名称、值、顺序和语义都完全相同的字段序列才提取。两个较新的 runtime-shell receipt 的封闭边界后缀满足这个条件，所以共享；approval-required 的边界集合不满足，所以留在本地。嵌套 object 的“构造模式”可以复制，但 object 内字段仍由各自文件拥有。这样既减少了 60 余个字段的双份维护，又保留了领域差异可见性。未来若第三份 receipt 也证明拥有完全同序的后缀，可以通过 fixture parity 后接入同一 helper；不能仅凭字段名大致相似就加入。

同样，builder 没有提供“自动排序”“忽略空白”“忽略未知字段”或“对象语义比较”。这些功能在普通业务 JSON 中可能方便，在版本化证据里反而危险。下游 fixture 会冻结原始字节和 digest，字段顺序与 compact spelling 都是审计线索。builder 若擅自排序，测试可能只看到解析值相同，却让 digest 和跨项目证据发生漂移；若自动忽略未知字段，新增危险能力也可能被吞掉。因此这里宁可让字段表显式一些，也不让便利层替维护者做不可见决定。

## 十一、共享 helper 的影响面和失败方式

`append_closed_runtime_shell_boundary_fields` 放在 `runtime_credential_resolver_receipt_utils.hpp`，它接受目标字段 vector 的引用，只追加已编码布尔值，不返回字符串、不创建第二个 object，也不持有任何生命周期状态。调用点决定它出现在哪里，因此字段顺序仍由 formatter 控制。helper 没有条件分支和 family 参数，这一点很重要：如果为了适配不同 receipt 加入大量开关，它就会重新变成隐蔽的 schema 引擎。当前接口表达的是一个简单事实：“从这里开始，追加这一组完全相同的封闭边界后缀”。

共享也扩大了影响面，所以测试不能只跑两份直接调用者。若 helper 漏掉 `external_request_sent`，两份 formatter 会一起漂移；若把 `approval_ledger_written` 误改为 true，风险也会同时扩散。v1646 用三层门应对：第一层是两份 frozen fixture 的整对象比较，会在任何字段缺失、顺序或值变化时失败；第二层是 SMOKEJSON、runtime smoke 与 release manifest，证明聚合后的消费表面仍一致；第三层是 345 条全量 CTest，证明 header 传播没有触发其他 translation unit 的行为变化。census 只证明“使用了 builder”，不能替代 parity；parity 证明输出字节，不能替代真实 CLI；这些证据各自回答不同问题。

header 中的 inline helper 还带来一个可接受的构建成本：任何包含该 utility 的 translation unit 在 helper 改动后可能重新编译。本次冷构建确实重新走了 1000 个编译/链接步骤，约五分钟。这里没有为了节省一次构建而把 helper 留成复制代码，因为长期维护风险高于局部增量编译成本；同时也没有把它拆成新的 `.cpp` 和公共 ABI，因为它只服务内部 receipt 构造，保持 header-only 可以避免增加链接层和 CMake source 清单。若未来 utility 继续增长或被大量文件包含，再根据测量结果拆出独立 translation unit，而不是现在预先复杂化。

## 十二、对抗性自审：这是不是只把字符串换了写法

最强的反对意见是：旧 formatter 已经通过测试，新代码行数更多，是否只是把能工作的字符串换成更“好看”的形式，却没有真实价值？如果本版只有语法替换而没有机械收益，这个质疑成立。v1646 的价值必须同时满足四个条件。第一，迁移前有冻结 oracle，而不是迁移后才补一个迎合新输出的测试；v1645 已先固定大小、digest、字段数和差异类型。第二，迁移消除了实际故障源：顶层与主要嵌套对象不再手写逗号、引号和花括号，整数与布尔值不再依赖 `std::to_string` 和字符串位置。第三，重复字段真正减少：两个 family 的同序封闭后缀只有一个实现，而不同的 approval-required 集合没有被错误合并。第四，进度成为只升不降的 census ratchet，后续不能静默退回旧写法。

反过来，本版没有把“测试全绿”夸大成业务能力升级。receipt 仍只描述证据，没有新增真实跨项目调用；成熟度仍是 single-project validation 加 cross-project contract alignment。也不能因为 builder-backed 数量达到 8 就宣称阶段一完成，census 明确还有 19 个 pending formatter 和 1 个结构性 waiver。post-echo 仍是公开阻塞项，而不是被文档措辞掩盖。这样的结论比“重构完成、质量提升”更克制，也更便于下一位维护者复现。

一句话总结：v1646 把三份 approval lifecycle 只读证据从脆弱的长字符串组装改造成可逐层审查、可机械计数、可由冻结 fixture 裁决的有序结构，同时保持 Node/Java/mini-kv 松耦合链中的 no router、no write、no WAL、no network、no credential、no execution 与非权威边界不变。
