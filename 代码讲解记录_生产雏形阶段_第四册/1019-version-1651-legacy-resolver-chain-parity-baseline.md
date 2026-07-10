# v1651 代码讲解：legacy resolver 四回执迁移前精确基线

## 一、本版目标、角色定位和明确不做事项

v1651 不迁移生产 formatter，而是为下一次到达 Claude review 点的迁移建立独立、可失败、可复现的 exact-byte 基线。选中的四份对象按历史版本连续排列：mini-kv v117 fake-shell archive、v118 production-readiness blocked decision、v119 pre-implementation plan intake、v120 disabled implementation candidate。它们连接了“测试壳归档完成”到“生产就绪仍阻塞”，再到“只读计划已定义”，最后到“候选仍禁用”的完整旧链条。

本版只是测试和维护证据，不新增 command，不改变 SMOKEJSON，不实现 credential resolver，不实例化 provider/client，不读取 credential value，不解析 raw endpoint URL，不发送 HTTP/TCP，不连接 managed audit，不写 Store、WAL、approval ledger 或 schema，不执行 LOAD、COMPACT、RESTORE、SETNXEX，不自动启动 Node、Java、mini-kv，也不获得 audit authority 或 order authority。四份 public formatter、digest 函数、fixture、manifest 和命令路由一行未改。

## 二、为什么到 review 点前还要多做一个基线版本

v1650 结束时 builder census 为 14/13/1，距离 Claude 指定的过半检查点 18 还差四个 formatter。表面上最省版本的做法，是直接挑四个文件加入 builder，再把 floor 改成 18。但那会让同一个 commit 同时改变生产实现和定义“什么叫没有变化”，测试与实现可能互相迎合，无法证明迁移前真实输出是什么。

v1645 和 v1648 已验证过更可靠的节奏：先冻结完整 fixture 子对象，再在下一版让未修改的 oracle 裁决生产迁移。v1651 沿用这个方法。新增 baseline 后，即使 v1652 的结构改动很大，也不能修改 size、digest、field count、canonical rule 或 boundary marker 来让迁移通过。任何改变都必须被视为 production drift，而不是“builder 格式不同但语义一样”。

## 三、候选是怎样从剩余十三个 owner 中选出的

机械 census 在 v1650 后列出十三个 pending owner，其中既有 credential-resolver 专题文件，也有 `runtime_evidence_receipts.cpp`、`runtime_sandbox_receipts.cpp` 这类多 formatter 聚合文件。直接按文件大小选最小项并不可靠，因为跨 family 会混入不同 helper、不同 boundary schema 和不同下游入口。

本版先比较每个 pending 文件的 formatter 数量、行数、是否使用 shared utils、是否有独立领域测试和 standalone fixture。最终选择 archive、decision、plan、disabled candidate 四份，原因有五点。第一，它们对应 v117-v120 连续历史链；第二，每个文件恰好拥有一个 public formatter；第三，每份都有独立 CTest 与 fixture；第四，四份仍使用相同年代的本地 `field_string`、`receipt_digest` 和长字符串顶层拼接；第五，它们都位于 v1625 设计中的 legacy helper 收口阶段，前面三个主要 slice 已经用十四份 formatter 证明 builder 稳定。

没有选择 `runtime_evidence_receipts.cpp` 或 `runtime_sandbox_receipts.cpp`，因为它们包含多个 formatter，迁移 blast radius 更大；没有把 input-hardening receipt 混进来，因为它属于更新的 prerequisite chain，字段更多且与 v117-v120 不同构；没有为了凑四个数把 execution-denied retention waiver 计入 owner，因为该文件没有目标 JSON formatter，census 已明确把它作为唯一命名 waiver。

## 四、四段旧链条分别表达什么

fake-shell archive receipt 消费 Node v264 test-only shell contract 与 Node v265 upstream echo，说明 archive 证据已经存在，但 mini-kv 不读取归档文件、不重放 fake shell 行为。它的正向字段是 archive verification ready，负向字段继续关闭 resolver、credential、network、write、schema、restore 和 authority。

blocked decision receipt 消费 Node v268 production-readiness decision gate，并把 readiness decision 固定为 blocked。它列出十项 missing requirement 与十个 production blocker，表达的是“证据链对齐，但真实实现前置条件不足”。`ready_for_credential_resolver_pre_implementation_plan=false` 在这一阶段是重要语义，不能被后续计划版本的 ready 字段覆盖。

plan intake receipt 消费 Node v270 pre-implementation plan，把十项缺失要求转换成十个定义供审查的 boundary code。它的 `plan_intake_state` 可以 ready，但 execution 仍然 false。计划完整只意味着审查材料齐备，不意味着 provider/client、credential、endpoint、network、ledger 或 Store 写能力被授权。

disabled implementation candidate receipt 消费 Node v273 candidate review，把十项 boundary 分为四项 disabled-candidate-ready 与六项 approval-required。candidate-ready 只表示可以继续审查 disabled interface 和 fake wiring，不等于 runtime implementation allowed。该对象通过一组专属 false flags 明确阻止真实 resolver、secret provider、raw endpoint、HTTP/TCP、managed audit connection 和写路径。

## 五、baseline 测试的输入、处理和输出

新测试 `runtime_receipt_legacy_resolver_chain_baseline_tests.cpp` 的输入有两类。第一类是仓库已提交的四个 standalone fixture 文件；测试用 `extract_json_object_field` 从 wrapper 中取出对应完整 receipt object。提取器能识别 nested object、array 和字符串 escaping，遇到重复字段、坏结构、非 object 值或未闭合内容会 fail closed。第二类是同一组标准只读命令 `INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON`，测试把它们传给四个现有 public formatter，得到当前 runtime object。

处理过程不解析后再重排，也不通过 fixture 生成 runtime。对每份对象，测试分别检查 fixture/runtime 大小、顶层字段数、版本化 FNV 摘要、`\u0027` 次数、receipt-only marker 和八个共同关闭边界。最后直接比较两个 `std::string`。这意味着字段顺序、compact whitespace、普通 apostrophe、Unicode escaping、整数文本和每个嵌套对象的原始字节都属于合同。

测试成功时没有生产输出，也不改变任何 fixture。它只以进程退出码向 CTest 报告通过。若对象不同，诊断函数会打印首个 mismatch 索引及前后上下文，帮助下一版定位漂移，但随后仍会 assert 失败；诊断信息不能被当作 waiver。

## 六、四份冻结值及其含义

archive object 固定为 12335 字节、95 个顶层字段，版本化摘要 `fnv1a64:13e45250210526fe`。blocked decision 固定为 10623 字节、99 个字段，摘要 `fnv1a64:10793a18910b09d2`。plan intake 固定为 11114 字节、101 个字段，摘要 `fnv1a64:b5bf23a427aa5aae`。disabled candidate 固定为 15160 字节、106 个字段，摘要 `fnv1a64:a602d27c08315d04`。

这四个摘要不是 public receipt digest 的替代物。public digest 继续由各 formatter 的领域 parts 计算，用于下游识别 receipt；v1651 摘要把完整 frozen object 作为单个输入，并带 `v1651-legacy-resolver-*` 前缀，只服务测试中的版本化 byte oracle。两类 digest 同时存在，分别保护业务身份和原始输出表面。

四份 fixture/runtime 的 `\u0027` 次数都为零，且首次测量就是 exact equal。因此本 family 没有 no-network、signed-human 或 approval lifecycle 的历史 apostrophe 规则，也没有 post-echo whitespace 修正规则。v1652 不得新增 normalization helper，不得把其他 family 的 canonical waiver 借过来，更不能修改 fixture 使 builder 输出看起来一致。

## 七、为什么还要检查字段数和 boundary marker

如果只比较完整字符串，理论上已经能捕获所有字节变化，但单一 assert 不能清楚说明失败属于哪一类。固定 size 能快速发现增删或 escaping 长度变化；固定 field count 能区分顶层 schema 变化与 nested 文本变化；版本化 digest 给归档和 reviewer 一个短而稳定的指纹；exact equality 最终兜底所有未分类差异。

测试还逐份要求 receipt-only marker，并统一要求 `read_only=true`、`execution_allowed=false`、`credential_value_read_allowed=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`write_commands_executed=false`、`load_restore_compact_executed=false` 和 `order_authoritative=false`。这些断言不是用片段测试替代 whole-object equality，而是让最关键的项目边界在失败信息和代码审查中可见。将来即使 fixture 被错误地同步修改，boundary 断言仍会阻止危险 true 值静默进入。

## 八、与原有领域测试如何分工

四份原有领域测试主要使用 `assert_contains` 检查 source Node profile、counts、required fields、warning/recommendation、standalone path、receipt digest、SMOKEJSON、runtime-smoke evidence 和 verification manifest。它们擅长说明字段的业务含义和下游位置，但无法证明未列出的字段顺序与空白完全不变。

新 baseline 专门补齐 raw object 维度，不复制所有领域断言。focused lane 同时运行 census、通用 builder、baseline、四份领域测试和三个下游聚合测试，共 10/10。这样出现问题时可以区分：builder 工具坏了、完整 object 漂移了、某个领域合同缺字段了，还是 SMOKEJSON/manifest 不再消费。full CTest 再覆盖 Store、WAL、snapshot、RESP/TCP、OSFS、其他 receipt family、归档和文档合同。

## 九、census 为什么必须保持 14/13/1

v1651 新增了一个 production-linked test executable，但没有任何 pending source include `runtime_receipt_json_builder.hpp`，所以 builder-backed 仍是 14、pending 仍是 13、waiver 仍是 1。测试文件、fixture 数、讲解长度或截图数量都不能计入 formatter migration。

这个诚实状态很重要。若本版把 CMake floor 从 14 改到 18，census 脚本会实际扫描 owner 并失败；若为了通过而把测试 include builder 也计数，就会破坏 owner 口径。只有 v1652 的四个 production source 真正改用 ordered builder，并继续通过本版 oracle，floor 才能收紧到 18。

## 十、代码所有权与失败诊断细节

baseline 的 `Candidate` 保存 fixture 名、object field、digest prefix、expected digest、expected bytes、expected fields、boundary marker 与 formatter 函数指针。数据表把四份合同差异显式放在调用点，循环只实现共同验证流程。这里没有用字符串名称动态查函数，也没有通过 command router 间接调用 formatter，因此 linker 和类型系统可以保证每个函数签名正确。

fixture object 由 `std::optional<std::string>` 返回，提取失败立即 assert；runtime object 是 formatter 返回的拥有内存字符串。`count_top_level_fields` 正确跳过字符串内容和 nested depth，不会把 nested key 误算为顶层字段。首差诊断用 `std::min` 限制 context 长度，并只在 mismatch 后写 stderr，不会污染正常 CTest 输出或 production stdout。

## 十一、验证链和对抗性自审

v1651 的首轮测量确认四份 exact equal，随后观察值被固化为常量并重新编译。focused 10/10 通过，文档前全量 CTest 为 347/347，说明新测试不仅自身可运行，也没有破坏其余工程。changed-file clang-format 与 `git diff --check` 在收尾中继续执行；真实 CLI smoke 仍要证明 SMOKEJSON 暴露四份 marker，CHECKJSON 对 LOAD 只分类且不触 WAL。

最强质疑是“测试常量由同一个执行者测量并写入，是否只是自证”。缓解方式有三层：测量对象来自冻结 fixture 与未修改 production formatter；四个领域测试和下游 fixture 早已存在；v1652 将在独立 commit 中只改 production，而 v1651 tag 可供 reviewer 重新运行基线。Claude review 可以直接 checkout v1651 验证 oracle，再检查 v1652 是否改过 test/fixture。

第二个质疑是四份文件是否真的同构。它们不是字段完全相同，因此本版没有提前抽共享 schema。所谓同构只指版本链连续、单 formatter、旧 helper 年代一致、测试与 fixture 完整。v1652 只能共享逐字段确认同名同值同序的部分；如果四份 boundary 不同，就应各自保留本地 fields，而不是为了 DRY 造万能 helper。

## 十二、v1652 的允许范围和停止条件

下一版允许把四份顶层长字符串改为 ordered `json_object/json_array` 与 `std::vector<OrderedJsonField>`，允许删除只服务旧拼接的 fragment helper，允许在确认完全相同时复用无参数字段 append helper。也可以继续保留各自本地 `field_string/receipt_digest`，因为同时统一 helper 与迁移结构会扩大差异；legacy utils 的最终收口可以留给后续版本。

停止条件明确：任何 fixture diff，停止；任何 size、digest、field count 或 exact equality 失败，停止；出现新的 apostrophe/whitespace normalizer，停止；需要修改 baseline 或领域期望，停止；四份中任一文件与其他三份结构差异过大，就缩小迁移范围而不是强行到 18；任何 credential、raw endpoint、network、write、WAL、schema、restore、auto-start、execution 或 authority 边界变 true，停止。

## 十三、为什么不把四份对象塞进已有 runtime-shell baseline

v1648 的 `runtime_receipt_runtime_shell_family_baseline_tests` 已经有通用 `Candidate` 表和同样的提取、摘要、字段计数模式，看起来可以直接追加四行。但 runtime-shell 五份对象属于 v130-v134 的 disabled shell decision family，共享 readiness 与 non-participation tail；本版四份属于 v117-v120 的早期 resolver governance chain，包含 archive files、missing requirements、boundary catalog 和 candidate classification。把两个 family 塞进一个测试，会让文件名、失败定位和未来 review 都失去边界。

新建独立 baseline 的成本只有一个小型测试目标，却带来清楚所有权：runtime-shell oracle 继续只裁决五份 shell receipt，legacy-resolver oracle 只裁决四份旧链。两者复用 test-only parity support，而不复制 extractor 或 digest 算法。将来若 legacy chain 出现自己的 canonical 规则，规则也必须留在本测试并命名，不能污染 runtime-shell 的“零例外”结论。

这种拆分也让测试失败更有信息量。CTest 名称直接指出 legacy resolver chain，reviewer 不必从一个九对象大表中判断哪一组迁移触发失败。版本讲解和截图可以引用同一测试名称，CMake 注册顺序又把它放在通用 builder 与 runtime-shell baseline 之后，形成工具、已迁移 family、新候选 family 三层证据。

## 十四、跨项目输入输出与边界怎样衔接

四份 receipt 的上游文字引用 Node v266、v268、v270、v273，历史 Java/mini-kv 版本只作为已冻结来源说明。v1651 不读取 Node 或 Java 工作区，不重新验证它们的业务代码，也不声称当前发生了实时四项目联调。输入只是本仓库 committed fixture 与本地 public formatter，结论只能表述为 single-project validation 加 cross-project contract alignment。

下游共有四个消费面。standalone fixture 用于版本化归档；SMOKEJSON 把 receipt 嵌入真实 CLI runtime evidence；`runtime-smoke-evidence.json` 提供稳定离线样本；`verification-manifest.json` 记录测试与路径。baseline 直接保护 standalone object，四个领域测试与三个聚合测试保护另外三面。这样的分工避免一份 fixture 被误当成所有 runtime 行为的替身。

Node 最新全局计划虽然已经推进到 prerequisite closure 之后，但仍明确禁止 credential value、raw endpoint、真实 provider/client、HTTP/TCP、ledger/schema write 和自动启动。v1651 回头清理旧 formatter 只是内部维护，不重开历史业务链，也不改变 Node 当前实施顺序。未来 Node 读取这些历史 evidence 时，看到的字节与 digest 不变；mini-kv 仍只提供证据，不成为 managed-audit 或 order authority。

## 十五、迁移时应怎样读取四份差异

archive formatter 的 nested section 重点是 archive files、snippet groups 与 checks；blocked decision 重点是 requirements、blocker codes 和 blocked summary；plan intake 重点是 boundary codes、requirement codes 与 plan flags；disabled candidate 重点是 candidate boundary classification、interface shape 和 approval-required counts。v1652 应先逐文件命名这些对象，再组装顶层，不能先设计一个共享结构再把四份内容硬塞进去。

真正可能共享的只有机械层：identity metadata、provenance digests、read-command digest、compact JSON scalar encoding，以及经过逐项确认的部分关闭边界。即使边界字段名相同，只要在历史顺序中的插入位置不同，也不能共用一个尾部 helper。exact-byte oracle 对此会立即失败，但代码审查仍应先看清差异，不能把测试当作试错导航。

迁移完成后的理想状态不是“所有 helper 都消失”，而是顶层花括号和逗号由 builder 负责，领域数组仍由语义明确的小 helper 负责，四个 public formatter 在各自文件内可独立读懂。若某个 raw helper 已经完整返回一个数组或对象、没有重复且有清楚名字，保留它比同版重写更稳健。结构化应减少认知负担，而不是追求形式上的百分之百 builder 语法。

一句话总结：v1651 没有增加一个 builder-backed formatter，而是把 v117-v120 四段 legacy resolver 证据链冻结为 12335/10623/11114/15160 字节、95/99/101/106 字段和四个版本化摘要，为下一版在不修改 fixture、测试期望和安全边界的前提下精确到达 18/9/1 Claude review 点建立独立机械裁判。
