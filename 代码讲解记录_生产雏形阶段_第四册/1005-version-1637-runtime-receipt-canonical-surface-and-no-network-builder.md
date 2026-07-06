# v1637 runtime receipt canonical surface 与 no-network builder 迁移讲解

## 1. 本版目标、角色定位与“不是什么”

v1637 的目标很明确：把 v1627/v1628 已经查清楚的 no-network receipt 漂移变成一条可以长期维护的工程规则，然后在这条规则下面迁移 `runtime_credential_resolver_no_network_safety_fixture_receipts.cpp`。它不是一个新的 runtime command，不是新的 Node/Java 接口，不是一个会启动服务、连接网络、读取 credential value、执行 restore/load/compact/rollback 的入口，也不是对历史 fixture 的重新生成。它的角色是一次保守的证据拼装重构：把一个仍在手写字符串拼接的 receipt formatter 改到内部 ordered JSON builder 上，同时证明运行时输出、字段顺序、digest、危险边界和下游可消费语义没有漂移。

这版之所以不能直接“顺手迁移”，是因为 no-network 在 v1626 时就不是普通的 byte-for-byte fixture parity。它和冻结 fixture 之间只有一处历史拼写差异：fixture 里保存的是 `Node v323\u0027s no-network fixture contract`，当前 public formatter 输出的是 `Node v323's no-network fixture contract`。这个差异不是字段缺失，不是字段顺序变化，不是 digest 变化，也不是 no-network/no-write/no-execution 语义变化；它只是同一个 apostrophe 在历史 fixture 里被写成 JSON unicode escape，而运行时 formatter 直接输出 apostrophe。问题小，但性质敏感：如果不先写清楚 canonical surface，迁移时随手统一任意一边，就会把“冻结 fixture 是 oracle”和“运行时 public surface 已经被消费”两个原则混在一起。

所以 v1637 第一件事不是改 formatter，而是把判断落地：当前运行时 formatter 的直接 apostrophe 拼写是 canonical runtime surface；历史 fixture 不改写。这个选择有现实理由。下游真正调用的是当前命令和 public formatter 暴露出来的运行时 JSON，冻结 fixture 是已提交证据，不能为了让测试看起来更漂亮而回写历史文件。于是测试里只允许这一处从 `\u0027` 到直接 apostrophe 的规范化，而且必须失败在任何第二处类似差异上。这个规则立住以后，no-network formatter 才能进入 builder 迁移。

## 2. 入口模块与响应结构

本版最核心的实现入口是 `src/runtime_credential_resolver_no_network_safety_fixture_receipts.cpp`。这个文件仍然负责同一件事：格式化 `credential_resolver_no_network_safety_fixture_contract_non_participation_receipt`，对外函数名、receipt version、source Node 版本、digest parts、summary、warnings、recommendations、evidence endpoints、boundary 和 node action 都没有改变。变化发生在“怎么拼 JSON”这一层。

迁移前，这个 formatter 主要靠大量字符串相加、局部 `field_string` helper 和手写 `[`、`]`、`{`、`}`、逗号来拼装 JSON。迁移后，它引入内部 builder 的几个小工具：`OrderedJsonField` 表示一个已经排好序的字段，`json_object` 按输入 span 的顺序生成对象，`json_array` 按输入顺序生成数组，`json_string` 负责普通字符串转义，`json_bool` 和 `json_integer` 负责布尔和数值。这里没有引入第三方 JSON 库，也没有把字段放入 `std::map` 之类会重新排序的容器，因为 receipt 输出对字段顺序非常敏感。

响应结构仍然是原来的结构。顶层先输出 receipt identity 和 source context，再输出 no-network safety fixture contract、prerequisite transition、necessity proof、Node v323/v322 来源摘要、mini-kv receipt、summary、warnings、recommendations、evidence endpoints、digest 字段，然后输出一组关闭边界 flags，最后是 `boundary` 与 `node_action`。重要的是，builder 只做拼装，不理解业务。它不会看到字段名就猜 `read_only` 应该是什么，也不会自动省略 false、0、空数组或空对象，更不会基于 Node 版本改变语义。领域含义仍然留在 no-network receipt 文件里，builder 只是把这些已经明确排好序的 JSON value 严格串起来。

## 3. canonical surface 的规则怎么落地

canonical 决策落在 `tests/runtime_receipt_json_builder_tests.cpp` 里，而不是只写在文档里。测试新增了两个命名常量：一个代表 fixture 里的 legacy phrase，也就是 `Node v323\u0027s no-network fixture contract`；另一个代表运行时 canonical phrase，也就是 `Node v323's no-network fixture contract`。测试不再使用模糊的 replace 操作，而是通过 `no_network_runtime_surface_canonical_json` 这个 helper 做四件事。

第一，它先断言 fixture 子对象里 `\u0027` 只出现一次。这样以后如果历史 fixture、提取逻辑或 formatter 意外引入第二处 unicode apostrophe，测试会马上失败。第二，它断言 legacy phrase 也只出现一次，防止把一个局部兼容规则误用成全文替换策略。第三，它把这一处 legacy phrase 规范化为 runtime canonical phrase，然后和当前 formatter 输出做完整字符串比较。第四，它断言 formatter 输出中不再包含 legacy phrase，确保 public runtime surface 仍保持直接 apostrophe 拼写。

这条规则的价值是把“例外”关在很小的盒子里。它不是允许 no-network 随便漂移，也不是允许 fixture 被改写。相反，它明确说只有这一处已知历史拼写差可以被规范化，除此之外字段数、字段顺序、digest、compact whitespace、read-only/no-network/no-write/no-WAL/no-credential/no-execution 边界都必须像普通 byte parity 一样保持不变。测试保护的是这个工程事实，而不是保护一段漂亮的说明文字。

## 4. no-network formatter 的迁移方式

`runtime_credential_resolver_no_network_safety_fixture_receipts.cpp` 的迁移采用分层方式。第一层是普通值的编码：字符串统一走 `json_string`，布尔统一走 `json_bool`，数字统一走 `json_integer`，数组统一由 `json_array` 生成。第二层是 nested object 的拼装：例如 no-network safety fixture contract、prerequisite transition、necessity proof、source Node summary/reference、mini-kv receipt 都改成 `std::vector<OrderedJsonField>` 后传给 `json_object`。第三层是顶层 fields 的排列：文件按历史输出顺序显式 push，每一段什么时候进入顶层对象都在代码里可读。

这版还专门抽出了 `append_bool_fields` 与 `append_closed_boundary_flag_fields`。它们的作用不是抽象业务，而是减少重复的布尔字段拼装，同时保持原字段顺序。这里有一个很实际的细节：第一次迁移后 focused test 失败，因为 `execution_allowed` 的顺序和原输出不一致。修复方式不是修改 fixture，也不是放宽测试，而是调整 helper 追加顺序，让 `read_only=true` 后紧跟 `execution_allowed=false`，再继续追加其他 true/false 边界字段。这个失败很有价值，它证明测试确实在保护字段顺序，而不是只检查几个 contains。

迁移后，文件表面上不一定大幅变短。v1628 已经把 line-count shrink 降级为 review metric，而不是 stop-condition，原因就在这里。对 receipt formatter 来说，第一目标不是追求物理行数立刻下降，而是把原本隐含在长字符串里的结构显式化，把容易出错的逗号、引号、数组边界、对象边界交给统一 builder，并让测试能逐字节地证明迁移没有改变 public surface。后续多个同构 receipt 迁移后，维护收益会体现在重复模式收敛和错误面减少，而不是每一版都立刻出现漂亮的行数下降。

## 5. 关键字段与控制面如何理解

这份 receipt 仍然是“非参与证明”，不是执行许可。`read_only=true` 表示它只描述证据，不允许改变 store；`execution_allowed=false` 表示它不能被解释成可以执行 Node/Java/mini-kv 的 runtime action；`network_safety_fixture_executed=false`、`http_request_sent=false`、`tcp_connection_attempted=false` 说明 no-network fixture 没有发起网络行为；credential 相关字段继续证明没有读取 credential value、没有解析 raw endpoint、没有打开 managed audit connection；Store/WAL 字段继续证明没有写 KV、没有追加 WAL、没有执行 LOAD/COMPACT/RESTORE/SETNXEX。

`boundary` 字段是本版最敏感的位置，因为唯一历史拼写差就发生在这个文本里。v1637 的规则不是把 boundary 变成随便可改的说明字符串，而是反过来把它钉得更死：运行时 canonical phrase 必须保持直接 apostrophe，fixture 里的 unicode escape 保留为历史证据，测试只允许这一处转换。这样 Node 或 Java 读到当前 runtime receipt 时，看到的是当前公开 surface；而 reviewer 对比冻结 fixture 时，也能知道为什么这一处原始字节不同，以及为什么它不是语义漂移。

`receipt_digest` 仍然是判断语义未漂移的重要字段之一。它不是安全签名，但它把 receipt 的关键 digest parts 按固定顺序压成稳定指纹。v1637 没有调整 digest parts，也没有改变 digest 的含义。测试同时保护 digest 和完整对象比较，避免出现“digest 没变但字段输出漂了”或“字段 contains 还在但顺序变了”的情况。

## 6. 测试分别保护什么

`runtime_receipt_json_builder_tests` 是本版的主测试。它一方面继续验证 builder 的基础行为：字段顺序、字符串转义、raw object/array 不被二次转义、false/0/空数组/空对象不被省略；另一方面专门验证 no-network canonical rule，证明唯一允许的 fixture/runtime 差异就是一个 apostrophe 拼写。迁移后，它还把规范化后的 fixture 子对象和 no-network public formatter 输出做完整相等比较。

`no_network_safety_fixture_contract_receipt_tests` 保护 no-network receipt 的领域语义。它确认 no-network fixture contract 仍然是非参与证据，没有发 HTTP、没有连 TCP、没有 credential value、没有 raw endpoint、没有 managed audit connection、没有 Store/WAL 写入，也没有执行权限。这个测试关注“字段语义有没有丢”，而 builder parity test 关注“原始输出有没有漂”。两者叠在一起，才足以支撑迁移。

`abort_rollback_semantics_contract_receipt_tests` 在本版没有重新迁移 abort/rollback，但它仍然被放进 focused test 集合里，原因是 v1637 更新了共享 builder 和 migration note 的整体叙事。abort/rollback 是第一个 builder-backed formatter，它必须继续证明旧切片没有被新切片破坏。

`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 保护下游嵌入路径。即使单个 formatter 通过了，仍然要证明 SMOKEJSON、runtime smoke evidence 和 verification manifest 没有因为 builder 迁移而出现隐藏 drift。`receipts_consolidation_note_contract` 则保护文档合同，避免设计说明还停留在“只有 abort/rollback 已迁移”的旧事实。

## 7. 本版对后续融合的价值

v1637 的价值不是“新增一个能力”，而是让后续 receipts consolidation 可以继续前进而不带隐患。no-network 是最容易被误判的 formatter，因为它有一个真实存在、但范围极小的历史字节差。现在这件事被命名、被测试、被文档合同保护，后续迁移者不需要重新猜测：fixture 不改，runtime surface 优先，例外只有一处，新的 receipt 迁移不能借这个例外扩大漂移。

对 Node/Java/mini-kv 松耦合融合来说，这版继续保持 mini-kv 的定位：它提供可验证、只读、不可执行的本地证据；Node 可以读取 evidence，Java 可以参考契约，但 mini-kv 不会因为 receipt consolidation 而变成 router、writer、credential resolver、network participant 或 execution host。
