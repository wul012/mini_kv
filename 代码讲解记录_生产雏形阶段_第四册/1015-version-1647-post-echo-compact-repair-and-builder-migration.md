# v1647 代码讲解：post-echo 单字节 compact 修正与有序 builder 迁移

## 一、本版目标、角色和“不是什么”

v1647 处理的是 v1645 主动停下来的一个具体缺陷：human approval artifact review post-echo decision receipt 的 runtime formatter 在两个相邻 object closing brace 之间多输出了一个空格。这个空格不改变 JSON 解析值，但会改变原始字节、长度、fixture parity 和可复现 digest 证据。v1646 已经明确把它排除在三份批量迁移之外，因此本版必须先把漂移查明并修正，再决定能否进入 ordered builder。

本版角色仍是合同保持型维护，但与纯重构略有不同：它承认 public runtime surface 存在一个一字节格式错误，并把 runtime 修正回冻结 fixture、项目 compact JSON builder 和其他 receipt 共同采用的紧凑表面。随后才迁移 formatter。它不是新增 post-echo 业务，不批准 runtime shell，不实例化 resolver/client/provider，不读取 credential value，不解析 raw endpoint，不发 HTTP/TCP，不写 Store、WAL、approval ledger 或 schema，不执行 LOAD、COMPACT、RESTORE、SETNXEX，不自动启动任何项目，也不让 mini-kv 取得 audit/order authority。

## 二、一字节漂移究竟发生在哪里

旧代码在 `source_node_v310_reference` 中依次拼接 `decision_gate`、`necessity_proof` 和 `summary`。summary 的最后字段是 `recommendation_count`，结束时需要关闭 summary object，再关闭 source Node v310 reference object，之后紧接顶层 `source_node_v309_reference`。正确 compact 字节应为：

```text
"recommendation_count":2}},"source_node_v309_reference"
```

旧 runtime formatter 却写成：

```text
"recommendation_count":2} },"source_node_v309_reference"
```

差异只有两个 `}` 之间的一个 ASCII space。它不会改变字段数量，解析器也会忽略 object 间空白，所以普通“解析后比较”发现不了；但 frozen fixture 子对象是 compact 形式，raw byte comparison 会发现。v1645 的基线因此得到一个看似反直觉的长度关系：fixture 自身含 `Node v310\u0027s`，比 runtime 的直接 apostrophe 多 5 字节；同时 runtime 又多了一个空格，于是总差只剩 4 字节。这个 `+5-1=+4` 正是 drift 的机械指纹。

## 三、为什么修 runtime，而不是修改 fixture 或允许空白

有三条独立证据指向 runtime 这一侧。第一，fixture 的 `}},` 是标准 compact JSON，与 `runtime_receipt_json_builder` 的确定性输出一致。第二，同一仓库其他 receipt 在相邻 object 结束处也没有插入任意空格，旧写法只是手工字符串中的偶然字符。第三，空格没有表达业务语义、可读性或下游兼容要求；保留它只会迫使 builder 增加“在某个特定字段后插入一个空格”的反常分支。

因此不能改 fixture。fixture 是历史证据，Node/Java/mini-kv 的路径和 digest 可能引用它；为迁就 runtime 错误而重写 fixture 会破坏证据链。也不能增加通用 whitespace normalizer，因为那会让未来任何空格、换行或 compact 漂移都被悄悄接受，失去逐字节迁移门。正确做法是修正生产 formatter 的唯一错误字符，让 runtime 回到既有 fixture 的 compact surface；唯一保留的兼容仍是该 family 已命名的 `Node v310\u0027s` 与 `Node v310's` 历史拼写。

## 四、先红后绿怎样证明执行顺序真实

v1647 没有同时修改生产代码和测试。第一步只把旧字符串中的 `"} }"` 改成 `"}}"`，测试源保持 v1645 状态。重新构建 `minikv_runtime_receipt_json_builder_tests` 后，CTest 按预期失败，断言位置是 `runtime_receipt_json_builder_tests.cpp:442`，内容仍要求 `fixture_object->size() == current.size() + 4`。退出码为 8。这个失败很重要：它证明生产输出先发生了真实的一字节变化，旧测试确实捕获了状态改变，而不是测试和实现一起改后自我证明。

第二步才收紧测试。post-echo 的 `migration_ready` 从 false 变为 true；专属的 compact transition、spaced transition 常量以及“再替换一次空格后相等”的特殊分支全部删除。新逻辑与 approval-prerequisite、human review 等普通 canonical family 一样：fixture 中 legacy apostrophe 必须恰好一次，runtime 中 canonical apostrophe 必须恰好一次；规范化这个已命名短语后，fixture 必须比 runtime 多 5 字节且整对象完全相等。测试从“描述并容纳一处已知漂移”变成“漂移再次出现就失败”，门槛是收紧，不是放宽。

## 五、public 入口与返回结构没有换壳

public 函数仍是 `format_credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_json`，参数仍为真实只读命令列表。digest 函数、prefix、parts 顺序、公有命名空间和调用链均不变。SMOKEJSON、runtime smoke evidence、release verification manifest 继续从同一入口获取 receipt，不需要新增 route 或 schema adapter。

顶层返回顺序也保持：receipt identity/source/version/runtime role 在前；随后是 Node v310 decision reference、Node v309 verification reference、mini-kv receipt、checks、summary、production blockers、warnings、recommendations、evidence endpoints、next evidence/actions；再放 binary/retention/read-command/receipt digest；最后追加大组闭合边界以及 `boundary`、`node_action`。JSON object 在语义上不依赖顺序，但这里的原始字节和 fixture digest 依赖，所以 ordered builder 必须按历史顺序输出。

## 六、builder 迁移怎样拆开嵌套结构

`decision_gate` 现在是独立的 `json_object`：decision digest、mode、source span、decision、selected path、reason，以及所有 `allows_*` 禁止能力都逐项可见；required prerequisites 和 explicit no-go conditions 仍由原领域 helper 提供已编码 JSON，计数使用 `json_integer`。`necessity_proof` 把 blocker、consumer、不能复用 v309 的理由、报告复用决策和 stop condition 聚合到一个局部对象。

`source_node_v310_summary` 单独保存 check/prerequisite/blocker/warning/recommendation 数量，然后与 decision gate、necessity proof 一起进入 `source_node_v310_reference`。Node v309 reference 仍拥有自己的 verification digest、Node v308 packet digest、字段数量、echo readiness 和 side-effect flags。这样评审者可以按来源版本阅读，不再靠追踪数十段 `+` 与引号判断某个字段属于哪层 object。

mini-kv receipt、顶层 summary、warnings、recommendations、evidence endpoints、next required versions 和 next actions 也分别命名。最后创建 `std::vector<OrderedJsonField>`，按历史顺序放入这些已经编码的 value；`json_object(std::span<...>)` 统一负责逗号、引号、转义和 compact 输出。builder 不知道 Node v310 业务，也不自动补字段，只执行给定顺序。

## 七、为什么 post-echo 没有复用 v1646 的共享边界 helper

post-echo 的闭合边界与 approval-prerequisite/human-review 很相似，但并非逐字段相同。v1646 的共享后缀包含 `raw_endpoint_url_provided=false` 和 `raw_endpoint_url_included=false`；post-echo 历史输出只有 accepted、parse allowed、render allowed、parsed、rendered，没有 provided/included。若为了复用 helper 把两项补进 post-echo，会改变 101 个顶层字段、fixture 字节和下游合同；若给共享 helper 增加多个布尔开关，又会把清楚的字段表变成隐藏 schema。

因此本版把旧 `format_closed_boundary_flags_json` 改成本文件的 `append_closed_boundary_flag_fields`，逐项保留 101 字段合同中的原始顺序。重复代码不是永远应该抽掉；只有名称、值、顺序和语义完全相同时才共享。这里保留本地所有权比追求表面 DRY 更可靠，也与 v1646 讲解中设定的复用准则一致。

## 八、关键边界字段如何约束控制面

`read_only=true` 和 `execution_allowed=false` 表明 receipt 是证据，不是动作。`human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_only=true` 把范围限定在 post-echo decision gate；它不会把 blocked decision 解释为执行批准。`ready_for_node_v311_before_upstream_echo=false` 继续要求 Java v144 与 mini-kv v137 只读 echo 先完成。

credential、raw endpoint、external request、managed audit connection、storage write、approval ledger、schema migration、restore/load/compact、SETNXEX 和 auto-start 分别有明确 false 字段。`runtime_shell_implemented=false` 与 `runtime_shell_invocation_allowed=false` 同时否定实现和调用；`audit_authoritative=false`、`order_authoritative=false` 否定最终裁决权。单字节格式修正和 builder 迁移都不能改变这些值，focused parity 和领域测试会逐层保护。

## 九、census、focused、全量与真实 CLI 各证明什么

`receipt_builder_census_contract` 的 floor 从 8 提到 9，实际口径为 28 sources、27 formatter owners、9 builder-backed、18 pending、1 named waiver。它证明进度不会静默倒退，但不证明输出正确。`runtime_receipt_json_builder_tests` 证明 fixture 大小、digest、101 个顶层字段、唯一 canonical spelling 和整对象 parity。`human_approval_post_echo_decision_receipt_tests` 保护 standalone receipt 合同。

`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests`、`release_verification_manifest_tests` 证明三条下游消费表面同步接受 compact 修正。focused 6/6 后，full CTest 345/345 再覆盖 Store、command、WAL、snapshot、RESP/TCP、OSFS、shard readiness、archive 与其他 receipt family，确认一字节 public 修正没有意外外溢。

真实 CLI smoke 直接运行本次构建的 `minikv_cli`。在 SMOKEJSON 全文中，精确 transition `"recommendation_count":2}},"source_node_v309_reference"` 出现 1 次，旧 spaced transition 出现 0 次；同时 `read_only=true` 与 `execution_allowed=false` 存在。`CHECKJSON LOAD data/prod.snap` 仍只识别 LOAD 是写命令并报告 `touches_wal=false`，随后 `QUIT` 返回 `BYE`。这证明真实可执行程序输出已经收敛，且没有借格式修正打开执行路径。

## 十、对抗性自审与后续边界

最强质疑是：JSON parser 会忽略空格，为什么值得独立一个版本？答案取决于项目是否把原始字节当证据。mini-kv 的 release fixture、跨项目 digest 和迁移 parity 都要求可复现 bytes；如果一个已知一字节差被允许留存，后续每次 builder 迁移都要增加特殊规则，最终“byte parity”会退化为宽松语义比较。本版用先红后绿证明、删除 waiver 分支和 census ratchet，把这笔小债连根拔掉，避免它扩散成规则噪声。

另一个质疑是新 formatter 从约 397 行增长到约 488 行。增长来自把隐含结构展开为命名对象和 ordered fields，不是增加业务字段或复制 fixture；评判标准仍是特殊拼接是否消失、parity 是否更严格、领域边界是否更清楚。若未来继续迁移，应先按同构 family 冻结候选，不得把“v1647 修过一个空格”当作迁移其他漂移的通行证。

## 十一、从输入到输出的完整数据流

这份 formatter 的直接输入只有 `read_commands`，通常是 `INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON` 四条只读命令。第一步，receipt digest 函数把当前项目版本、历史 release/artifact hint、Node v310/v309 digest、三类计数、三个 false 权限位和 read-command-list digest 依次送入既有 FNV digest helper。输入顺序没有因 builder 改变，输出仍是相同 receipt digest。这里没有执行命令；read command list 只是已发生只读观察的证据摘要。

第二步，领域 helper 生成 required prerequisites、no-go conditions 和 checks。这些 helper 的输出已经是完整 JSON array/object，因此作为 `value_json` 进入 builder，不会被 `json_string` 二次转义。若误把它们当普通字符串，输出会出现被引号包住的 JSON 文本，fixture parity 会立刻失败。整数通过 `json_integer` 转为十进制，布尔值通过 `json_bool` 转为固定 `true/false`，普通文本才经过 `json_string` 使用项目统一转义。

第三步，由内向外构造对象。decision gate 接收 Node v310 的 decision fields、required prerequisites 和 no-go conditions，输出一个 compact object；necessity proof 接收五段解释文本，输出另一个 object；两者和 source summary 一起成为 Node v310 reference。Node v309 reference 独立构造，避免来源字段串层。顶层拿到这些已编码对象、mini-kv receipt、checks、summary、warning/recommendation arrays 和 evidence endpoint object，按冻结顺序组成 vector。

第四步，本地 boundary helper 继续向同一 vector 追加一组布尔字段。它的输入是可变字段容器，输出不是字符串，而是容器尾部新增的有序元素；因此不存在剥花括号、拼逗号或临时 fragment。最后加入 boundary 和 node action，`json_object(span)` 一次生成 public formatter 字符串。`span` 只借用 vector 内存，调用期间 vector 仍存活，不发生悬空引用。

第五步，public 字符串向外传播。post-echo 领域测试直接调用 formatter；SMOKEJSON 把它嵌入 runtime aggregate；runtime smoke evidence 和 release manifest 又从同一聚合链消费。fixture parity 从已提交 release fixture 提取对应 object 后比较原始字节。由此形成一条清楚链路：只读命令清单和常量输入，经过 digest 与有序子对象，形成 public receipt，再被三个下游证据表面消费；链路中没有 router、网络连接或写命令执行。

## 十二、不同失败模式由哪一道门阻止

如果维护者再次在两个 closing brace 之间插入空格，整对象 canonical parity 会失败，因为专属 spaced transition 分支已经删除。如果他改变字段顺序，即使 JSON parser 仍能解析，raw object comparison 和 fixture digest 也会失败。如果他漏掉一个顶层字段，101-field count 会先失败；如果新增字段，计数同样失败，不能伪装成“向后兼容扩展”。

如果有人把 Node v310 apostrophe 规则扩大成任意 `\u0027` 替换，测试中的命名 legacy/canonical 常量和 exactly-once 断言会阻止第二处差异。若他直接改 fixture 来配合 runtime，git diff 会暴露 frozen release 文件变化，版本失败条件要求回退；fixture digest 常量也会变化。若他删除 builder include 或退回字符串 formatter，census 实际数会低于 floor 9，CTest 失败。

如果输出仍正确，但 SMOKEJSON 忘记嵌入 receipt，`smokejson_command_receipt_tests` 会失败；若 runtime evidence 或 release manifest 丢失，另外两条 focused test 分别失败。若边界字段从 false 变 true，领域 test、builder parity 与多个下游 test 会同时报警。若内部 header 改动影响 Store/WAL/OSFS 等无关模块，完整 345 条 CTest 才是最后防线。真实 CLI 则补足“测试 binary 与实际交互程序是否一致”的问题，它直接统计 compact/spaced transition 并检查 CHECKJSON 没有执行 LOAD。

这些门不能互相替代。census 只计迁移进度，不懂字节；parity 只对已选输入证明输出，不证明 CLI 交互；领域 test 不覆盖所有下游嵌入；full CTest 能发现广泛回归，却不一定解释单个字节的来历；截图和文档只能记录结果，不能制造结果。v1647 把它们组合，是为了让每种主张都有最接近的机械证据。

## 十三、本地边界表的长期维护策略

保留 post-echo 本地 boundary helper 意味着几十行字段仍显式存在，看起来不像“极致去重”。但这里的维护对象不是代码字符数，而是版本化合同。共享 helper 一旦带上 `include_raw_endpoint_provided`、`include_signed_artifact_flags`、`include_endpoint_authority` 等开关，调用点会变短，真实输出却取决于中央函数中的条件组合；评审者不能只看当前文件判断字段。随着 family 增多，布尔参数还会形成难以验证的组合爆炸。

本地 helper 则让 post-echo 的 101-field 表面完整可见。未来要增加字段，维护者必须在本文件明确选择位置，fixture parity 会要求同步的合同决策；要删除字段，也会立即触发 count 和 byte failure。只有当另一个 receipt 与它在字段名称、值、顺序、语义和演进节奏上都完全一致，并经过迁移前 parity 证明，才值得提取共享后缀。这样的抽象门槛较高，但能避免“为了 DRY 而共享错误语义”。

性能方面，这些 receipt 只在证据命令或测试中构造，局部 object string 和 field vector 的分配不是数据面热路径。本版优先选择可审查性，没有引入缓存或全局 mutable builder。若未来测量证明构造开销显著，应先优化 builder 的 reserve/移动语义并保持同一 parity，而不是退回手写字符串或牺牲边界可见性。没有性能证据时，提前缓存还会引入生命周期、线程安全和陈旧版本输出风险。

一句话总结：v1647 先用可观察的红灯关闭 post-echo 单字节 runtime 漂移，再把 formatter 迁到可审查的有序 builder，并以更严格的整对象 parity、9/18/1 census、全量测试和真实 CLI 证明 no router、no write、no WAL、no network、no credential、no execution 与非权威边界全部保持。
