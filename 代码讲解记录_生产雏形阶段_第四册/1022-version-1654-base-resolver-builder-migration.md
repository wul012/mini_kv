# v1654 代码讲解：base resolver 三 formatter 有序构建迁移

## 一、本版目标、角色定位与明确不做事项

v1654 是 v1653 全量冻结后的第一批 production migration，范围只覆盖 base resolver 自然链：基础 credential resolver decision receipt、disabled credential resolver precheck receipt、test-only resolver shell receipt。三份对象分别对应 Node v260 的 decision record、Node v262 的 disabled precheck、Node v264 的 test-only shell contract。它们共同描述“材料逐步变完整，但真实 resolver 始终未实现、未启用、未调用”的早期治理链，因此适合作为一个版本闭环，而不与更新的 implementation readiness 或 managed-audit sandbox family 混合。

本版不是实现 credential resolver，不新增命令或路由，不实例化 resolver client、secret provider 或 fake provider，不读取、加载、保存、渲染 credential value，不接收或解析 raw endpoint URL，不发送 HTTP/TCP，不打开 managed-audit connection，不写 Store、WAL、ledger、schema 或 production record，不执行 LOAD、COMPACT、RESTORE、SETNXEX，不自动启动 Node、Java 或 mini-kv，也不成为 managed-audit store、sandbox router、audit authority 或 order authority。public function signature、digest 输入、fixture、领域测试期望、SMOKEJSON 字段名和 manifest 路径全部保持不变。

## 二、为什么三份 formatter 属于同一批但仍保留各自所有权

基础 decision receipt 记录 endpoint handle、credential handle、resolver policy handle、approval marker 与人工审批要求，只允许“记录决策”，不允许执行解析。disabled precheck 在这之上增加六个 env handle、两个 opt-in gate、七类 failure code 和十二字段 dry-run response shape，但所有 gate 默认关闭。test-only shell 再定义 handle-only request、无副作用 response、failure mapping、guard conditions 和 fake in-memory probe；“test-only”只说明结构可测试，不代表 fake probe 在生产 receipt 生成时真的运行。

三份对象共享一组完全同名、同值、同顺序的关闭边界，但领域 nested object 不同。v1654 因而只共享 mechanical boundary appender：decision record、disabled precheck、env handles、failure taxonomy、request/response shape、fake resolver probe 仍在各自 formatter 附近声明。没有引入“模式枚举+几十个布尔参数”的通用 resolver schema，也没有让公共 helper 根据 receipt 版本猜测字段。共享范围越小，reviewer 越容易确认某个字段为什么存在以及插入在何处。

## 三、迁移前裁判如何约束本版

`runtime_receipt_remaining_owner_baseline_tests` 在 v1653 tag 中先于任何本版 production 改动存在。它分别冻结基础 decision 7944 字节/104 字段、disabled precheck 10291 字节/118 字段、test-only shell 11201 字节/133 字段，并保存 fixture/runtime 完整对象摘要。disabled precheck 与 test-only shell 必须 raw byte-for-byte 等于 fixture；基础 decision 只允许 v1653 已命名的一次 `credential_resolver_decision_only` 历史插入顺序差异，规则应用后仍必须整对象等于当前 runtime surface。

本版采用逐份迁移节奏。先改基础 decision，重新编译 core 并运行十三对象 oracle；通过后才改 disabled precheck，再运行同一 oracle；最后迁移 test-only shell并第三次运行。这样即使某个字段顺序、逗号或 nested whitespace 出错，失败候选只有刚修改的一份，而不是在三份大 diff 中反向猜测。整个过程中 v1653 的 Candidate 常量、fixture、compatibility enum 和领域测试没有修改。

## 四、ordered builder 在这里承担什么职责

三个 formatter 现在都直接 include 内部 `runtime_receipt_json_builder.hpp`，使用 `json_string`、`json_bool`、`json_integer`、`json_object`、`json_array` 和 `OrderedJsonField`。scalar encoder 负责字符串 escaping、布尔与十进制整数表示；object/array builder 按调用点给定顺序输出 compact JSON。builder 不排序、不去重、不省略 false 或空数组，也不读取 receipt 语义，所以“字段是否存在、叫什么、值是什么、放在哪里”仍由 formatter 显式决定。

顶层组装从一段长字符串改为 `std::vector<OrderedJsonField>`。每个元素把字段名与完整 JSON value 成对放在一起，逗号由 builder 统一插入。nested object 先保存在具名局部变量中，再作为 raw JSON value 放入顶层字段；这样 reviewer 可以先读领域对象，再读完整输出顺序。最终通过 `json_object(std::span<...>)` 渲染，不需要剥离首尾大括号或在调用点手工补逗号。

## 五、common boundary fragment 为什么必须退出

旧 `format_common_credential_boundary_json` 返回的是没有外层大括号的字符串片段，从 `read_only` 开始，到 `order_authoritative` 结束。三个 formatter 在它之前手工留下逗号，在它之后继续拼 family 专属字段。这个设计能工作，但 helper 的返回类型只是 `std::string`，编译器不知道它是不是完整 JSON，调用者也必须记住“前面已有逗号、后面不能多逗号”。一旦新增边界字段，最容易出现 `,,`、缺逗号或插入位置漂移。

新 `append_common_credential_boundary_fields` 接收 ordered field vector，并逐项追加原来的三十七个字段。顺序从 `read_only=true`、`execution_allowed=false`、`dry_run_only=true` 开始，继续覆盖 resolver/provider 未实现、三项目不自动启动、无 connection、无 write/admin、不是 audit store、无 credential/raw endpoint、无 schema/restore/load/compact/setnxex，最后到 `order_authoritative=false`。它没有任何参数开关，因而不可能因调用者传错布尔值而打开能力；三个调用点必须先放自己的 receipt-only marker，再调用 appender，之后再放 family 专属尾字段。

## 六、基础 decision formatter 的结构

基础 decision 先构造 `decision_record`，其中包含 record mode、decision scope/status、source span、四类 handle/marker、operator identity 与 approval correlation requirement、两项 count，以及十个明确禁止动作。`required_decision_fields` 是八个具名对象组成的数组，每项把 id 与 accepted evidence 配对；原来的 policy、identity、correlation、redaction、rotation 常量继续动态进入 value。`checks` 则保留 Node v259 对 connection、credential、raw endpoint、write、auto-start 和 mini-kv non-participation 的十九项判断。

顶层 fields 严格沿用旧顺序：身份与版本元数据、source decision 状态、source flags/counts、Node v259 alignment、三个 nested value、next echo versions、五类 provenance/digest，随后是 `credential_resolver_decision_only=true`、common boundary、boundary 文本和 node action。特别要注意 receipt-only marker 仍位于 common boundary 之前，这正是 v1653 命名字段顺序规则保护的 canonical runtime 表面；本版没有为了追求 fixture raw equality 把它移动到 `dry_run_only` 后，也没有改写 frozen fixture。

## 七、disabled precheck formatter 的结构

`disabled_precheck` 对象集中描述 implementation status、是否可实例化 client/provider、credential/raw endpoint/network 三类禁止动作，以及 env handle、gate、failure、response、no-go 五类 count。`required_env_handles` 由六个对象组成：两个未来 opt-in gate 和四个 handle/approval marker。每个对象明确 purpose、precheck 是否需要真实值、是否是 credential/raw endpoint value，以及真实 resolver 前是否必须存在。builder 让这些字段结构可见，不再埋在一个跨多行字符串中。

`opt_in_gates` 保持两个 gate 的 required future value 为字符串 `true`、current default 为字符串 `false`，并明确 enabled-at-precheck 要被阻断；这里没有把字符串改成布尔，因为原字节合同就是字符串。`failure_taxonomy_codes` 保持七项顺序；`dry_run_response_shape` 保持十二个 camelCase field name 和九个关闭结果。`checks` 继续覆盖 source alignment、handle-only、gate default、taxonomy、shape、implementation absent 与 upstream disabled。

顶层 receipt-only marker 后调用 common appender，再按历史位置追加 `resolver_client_instantiated=false`。虽然 common appender 已包含 `secret_provider_instantiated=false`，却不包含 resolver client 字段，因此这项不能被删除或随意移入共享 helper；v1653 的 118 字段 exact oracle会立即发现缺失或顺序变化。

## 八、test-only shell formatter 的结构

test-only shell 的 `resolver_shell_contract` 说明 shell 名称、mode、fake-in-memory kind、fake/test/read-only/handle-only 属性和四类 count，同时关闭 real resolver、real provider、credential load、raw endpoint parse 与 external request。`request_shape` 复用原九字段数组，逐项声明 handle-only、approval marker/correlation、dry-run 与 fake-only requirements；`response_shape` 复用十三字段数组，明确 client/provider、credential、endpoint、request、connection、schema 和 production write 均未发生。

完整 failure mapping 与 guard conditions 已是有清晰名字、领域完整的数组 helper，本版把它们作为 raw JSON value 放入 builder，没有为了“所有 nested 都必须同一种语法”而重写稳定内容。`fake_resolver_probe` 现在是具名 object，但 `fake_resolver_probe_executed=false` 仍位于顶层 common boundary 后，说明 receipt 只是回显预期 probe contract，并未在 formatter 中执行 fake resolver。`checks` 保留十九项保护，next echo versions 与五类 digest 仍动态计算。

## 九、输入、处理、输出链条怎样保持透明

输入仍是调用者传入的只读命令列表和编译时版本/历史常量。digest 函数继续把版本、release、source profile、counts、状态布尔和 `read_command_list_digest` 按旧顺序放入 length-delimited FNV 输入；本版没有把 builder object 反过来用于计算 public digest，因此 JSON 结构迁移不会隐式改变 receipt 身份。binary provenance、retention check/replay 与 read command digest 仍在调用时获取。

处理阶段先生成 domain nested value，再按历史 top-level order 构造 fields，追加 common boundary 与 family tail，最后渲染 compact object。输出仍是 `std::string`，被领域测试、SMOKEJSON、runtime-smoke evidence、verification manifest 和 standalone fixture 同时消费。没有新文件 I/O、网络 I/O、线程或进程，也没有把 fixture 作为 production 输出源；fixture 只存在于测试裁判侧。

## 十、代码体量与拆分判断

迁移前双 formatter 文件含空行 524 行，test-only 文件 329 行，utils 122 行；迁移后分别是 583、361、142 行，合计净增 111 行。增长来自显式 ordered fields、具名 nested object 和三十七字段 appender，不是新增业务 schema或重复同一对象。最大文件 583 行，距离本轮预设的 800 行风险线仍有余量，因此没有为了降低数字把第二 formatter 移到一个 census 不认识的“singular receipt”文件，也没有通过新 companion 文件制造统计盲区。

这次判断兼顾两种风险：继续长字符串会保留逗号/顺序维护债；过度拆分又会让常量、digest、formatter 分散到多个只为行数服务的文件。当前两个 public formatter共享同一组常量年代和 boundary helper，保持在一个 583 行 owner 内仍可一次读懂；test-only 的 request/response/failure/probe 也在 361 行内闭环。若未来继续新增职责，应先新建明确 family 而不是往这两个历史 formatter 追加字段。

## 十一、机械 census 为什么从 18 只升到 20

本版迁移了三个 public formatter，但它们属于两个 source owner。census 的契约单位一直是拥有目标 formatter 的 `runtime_*_receipts.cpp` 文件，而不是函数数量。因此两个文件直接 include builder 并真实使用 ordered API 后，builder-backed 从十八升到二十，pending 从九降到七；不是从十八升到二十一。十三 formatter 级别的 v1653 oracle与二十 owner 级别的 census互补：前者防止一个多 formatter owner 只迁一半，后者防止 source 文件删除 include 或新增未登记 owner。

`CMakeLists.txt` 的 `MIN_BUILDER_BACKED` 从 18 收紧到 20，只升不降。总数仍是 28 source、27 formatter owner、1 个没有目标 formatter 的 execution-denied retention waiver。没有新增 waiver，没有把 test file 或 header 计入 owner，也没有因为文件内有两个 formatter就重复计数。

## 十二、测试矩阵分别保护什么

第一层是 v1653 complete-object oracle：三份迁移后仍为 7944/10291/11201 字节、104/118/133 字段和原摘要；基础 decision 只使用自己那条 named order rule，另外两份 exact。第二层是三个领域 CTest：它们检查 source Node 版本、profile/state、counts、handles/gates/shapes、receipt digest、fixture path、SMOKEJSON 与下游 manifest。第三层是 builder census，确认 owner 真实达到 20/7/1并守住 floor。

第四层 focused downstream 会覆盖 `smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests`，证明三份对象仍从真实聚合入口出现。第五层 full CTest 覆盖 WAL、snapshot、Store、RESP/TCP、OSFS 和其他 receipt family。最后真实 CLI 输入 `SMOKEJSON`、`CHECKJSON LOAD data/prod.snap` 与 `QUIT`，结构化解析三份对象的关闭字段，并确认检查 LOAD 仍是 `read_only=true`、`execution_allowed=false`、`wal.touches_wal=false`，没有执行命令。

## 十三、对抗性自审与失败条件

最强质疑是 common appender 会不会把三个不同 family 错误同构。缓解在于它只包含逐项核对完全相同的三十七字段，且无参数；decision-only、disabled-only、test-only、resolver client 和 fake probe executed 等差异都保留在调用点。任何历史插入位置不一致都会触发 whole-object oracle。第二个质疑是 builder 迁移是否通过改 fixture或测试自证。git diff 应显示 tests 与 `fixtures/release` 零改动，v1653 tag 可独立重放，三份 Candidate 数字也不变。

第三个质疑是 583 行是否仍太大。本版测量了实际大小并设 800 行停止线；它比迁移前增加 59 行但消除了两个超长 top-level 拼接，且没有新增 family 职责。若 formatter 必须继续增长到风险线，下一次应按 base decision 与 disabled precheck 做显式 logical-owner split，并同步升级 census，而不是静默规避。当前版本最强 likely reviewer objection 已被 byte oracle、owner ratchet、领域测试和 size census 四层覆盖。

后续 implementation chain 五个 owner 当前全部 exact，允许在下一版批量迁移；但不得复制本版基础 decision 的 field-order rule，也不得修改 v1653 oracle。再后一版 sandbox chain 才能消费 manual dry-run 的三字段 frozen metadata rule。顺序这样安排，是为了让每条历史兼容规则只在自己的 family 中出现，避免“已经有两个例外”演变成通用宽松策略。

## 十四、后续维护者应怎样修改这三份对象

如果未来真的出现新的上游 contract，维护者首先要判断它属于 domain nested value、family 专属顶层字段，还是三个 receipt 完全共有的关闭边界。decision handle 或 approval evidence 应留在 `decision_record`；precheck gate、failure class 和 response shape 应留在 disabled precheck；request/response/fake probe 字段应留在 test-only shell。只有三个 formatter 同名、同值、同顺序且含义完全一致的字段，才有资格进入 common appender。不能因为两份对象“看起来都是否定布尔值”就把它们合并。

字段插入也必须服从已有顺序。新增 nested 字段时在对应 `json_object` 的语义位置插入；新增顶层字段时先查看 frozen object 的前后邻居，不能一律堆到尾部。若新字段属于真正 contract 变化，应先更新来源计划和独立领域测试，再生成新 fixture/digest 版本并让下游明确消费；v1653 baseline 应通过一个专门 contract migration 被有意识升级，而不是在内部重构 commit 中顺手改常量。内部实现重构则完全不应修改 baseline。

类型同样不能凭直觉“修正”。disabled precheck 的 `required_value_for_future_resolver` 和 `current_default` 是字符串，而不是布尔；request/response `fields` 是有序字符串数组；count 必须走 `json_integer`；already-encoded failure mapping 和 guard condition 必须作为 raw JSON value 传入，不能再走 `json_string` 二次转义。builder 只保证传入类型如何编码，不会替调用者发现业务类型选错，所以 exact oracle 与领域测试仍不可替代。

review 时建议按固定顺序阅读：先看 public digest 是否未变，再看 nested value 是否保留原语义，然后从 fields 开头走到 receipt digest，最后核对 family marker、common appender 和专属 tail 的插入顺序。这样可以把“值是否正确”“字段是否齐全”“顺序是否正确”“边界是否关闭”四个问题分开。遇到 oracle 失败时先看字节数和字段数，再看完整摘要与首个差异，不应先改测试；字段数相同通常指向顺序或值，字段数变化才优先检查增删。

## 十五、跨项目价值与一句话总结

Node 最新 N5 source-size roadmap 明确 Java 与 mini-kv 可并行，本版属于 mini-kv 内部 formatter ownership 维护，不要求新 Node/Java evidence，也不修改跨项目 contract。Node 以后读取 frozen fixture 或真实 SMOKEJSON 时仍看到相同字节、digest 和关闭边界；mini-kv 仍只是 runtime evidence provider。当前成熟度仍应表述为 single-project validation 加 cross-project contract alignment，不能把三份历史 receipt 的本地重构说成新的实时联合测试。

一句话总结：v1654 在 v1653 不可变裁判下，把 base resolver 三份 7944/10291/11201 字节的旧字符串 formatter 改为具名 nested value、ordered top-level fields 和无参数 common boundary appender，census 从 18/9/1 收紧到 20/7/1，同时保持 no credential、no router/store、no write、no execution、no auto-start 与 no authority 全部关闭。
