# 第一千六百二十八版代码讲解：false shrink metric 修正与 no-network 漂移落地

本版目标不是继续迁移 receipt formatter，而是先把上一刀暴露出的质量判断修正干净。v1627 把 abort/rollback non-participation receipt 从手写字符串拼接迁到内部 ordered JSON builder，并保持了 frozen fixture 子对象 byte-for-byte parity；同时，它把 no-network 的 pre-migration drift 查成了一处 `\u0027` 与直接 apostrophe 的历史转义差异。这个方向是对的，但文档里还残留一个容易误导后续批量迁移的判断：v1625 的 receipts consolidation 设计说明把“迁移后文件没有明显变短”写进了停止条件。用户指出 v1627 的 abort/rollback 迁移在 diff 口径下净增 +116 行，这个增长本身是良性的，不能因为设计说明一句话就被误判为 stop-condition。v1628 因此先修规则，再加硬测试，把 no-network 漂移彻底落到可审查事实上，然后停在迁移之前。

这版的角色定位是“质量校准版”。它不是新的 runtime command，不新增 router，不修改 `SMOKEJSON`、`INFOJSON`、release manifest 或任何 fixture，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，不写 Store/WAL，不打开 HTTP/TCP，不读取 credential value，不解析 raw endpoint URL，也不启动 Node、Java 或 mini-kv 服务。控制面读到本版只能理解为：mini-kv 正在修正内部迁移治理规则，并把 no-network 迁移前证据查清；它没有获得 network safety authority、managed audit storage authority、rollback authority 或 execution authority。

## 为什么 +116 行不是硬失败

看一个重构是否有价值，行数当然有参考意义。一个所谓的“合并”如果只是把一段长字符串从 A 文件搬到 B 文件，输出没有更可证、测试没有更硬、边界没有更清楚，那它就是噪声。但 receipts 族不是普通业务函数。它们的输出是 compact JSON，字段顺序、转义拼写、digest 输入、read-only/no-execution 边界都很敏感；它们经常宁愿显式一点，也不能让 builder 自作聪明。因此第一批迁移可能出现短期行数增长：旧代码是大段字符串相加，物理行少但不容易审；新代码把字段拆成 ordered field、raw JSON 子对象、关闭边界 helper 和明确的测试断言，物理行可能增加，但可维护风险下降。

v1627 的 abort/rollback 迁移正是这种情况。`git diff --numstat` 显示 `src/runtime_credential_resolver_abort_rollback_semantics_receipts.cpp` 为 443 行新增、327 行删除，净增 +116 diff lines。如果只按“文件必须明显变短”判断，它会被误杀；但真实工程判断应该看四件事。第一，public formatter 的输出仍与 frozen fixture 子对象逐字节相等。第二，receipt digest、字段顺序、compact whitespace、boundary flags 没变。第三，builder 不理解 Node 版本、prerequisite 或 authority 语义，只负责 deterministic JSON assembly。第四，测试从 `assert_contains` 一类片段保护，推进到 fixture subobject parity 和 builder behavior focused tests。满足这些条件时，+116 行是需要解释的 review metric，不是停止迁移的硬门槛。

所以本版修改 `docs/receipts-consolidation-note.md`。旧 stop-condition 是“迁移后文件没有明显变短，或者共同逻辑只是从一个长文件搬到另一个长文件”。新表达把硬条件改成“共同逻辑只是从一个长文件搬到另一个长文件，且没有提升 byte parity、边界可读性、重复消除或测试可证性”。这句变化很关键：它把判断从单一行数转回维护价值。文档还新增一段英文锚点 `Line-count shrink is a review metric, not a stop condition`，并写明 `git diff --numstat` 的净增/净减只能提示维护者解释迁移是否值得，不能单独决定停止。

## 文档合同如何防止规则再漂

mini-kv 很容易出现“文档写了，但后续没人守”的问题，所以这版没有只改 Markdown。`cmake/check_receipts_consolidation_note.cmake` 的 `required_fragments` 增加了四个片段：`Line-count shrink is a review metric, not a stop condition`、`+116 diff lines`、`false shrink metric`、`Node v323\u0027s no-network fixture contract`。这个 CMake 脚本由 CTest `receipts_consolidation_note_contract` 执行。也就是说，如果以后有人把这段质量修正删掉，或者又把 no-network 漂移说明写模糊，本地 focused CTest 会失败。

这类文档合同的意义不是把 Markdown 变成形式主义，而是让迁移规则和代码迁移处在同一条验证链上。runtime receipts consolidation 本来就是一个容易规模化的重构，如果第一批规则错了，后面二十多个 credential resolver receipt 文件都会把错规则放大。v1628 先把错规则钉住，正是为了后续能更自由、更有节制地推进，而不是每一版都被“行数到底多了还是少了”牵着走。

## no-network 漂移到底落在哪里

v1627 已经证明 no-network 的 drift 是一处 apostrophe 转义差异，但用户要求“run the no_network drift to ground”，这意味着不能只写在讲解里，也不能停留在“替换后相等”一句话。v1628 因此加强 `tests/runtime_receipt_json_builder_tests.cpp`。

第一层新增 `count_top_level_fields`。这个函数不把 JSON 解析成 map，也不重新序列化；它只按原始字节扫描顶层对象，识别 depth=1 的 field key。这样既不会打乱字段顺序，也不会把 `\u0027` 和 `'` 归一化。测试断言 committed fixture 子对象和当前 formatter 都有 118 个顶层字段。这个断言排除了“字段多了一个”“字段少了一个”“某个嵌套对象漏出来变成顶层”的风险。

第二层新增 `count_occurrences`。测试断言 fixture 子对象里 `\u0027` 只出现一次，当前 formatter 里 `\u0027` 出现零次；fixture 里的 `Node v323\u0027s no-network fixture contract` 出现一次，当前 formatter 里的 `Node v323's no-network fixture contract` 也只出现一次。这样就不是模糊地说“有个地方不同”，而是确认差异只有一个短语、一个拼写、一个位置。

第三层把位置压到字段。测试现在断言差异短语出现在顶层 `boundary` 字段的开头附近：fixture 是 `"boundary":"credential resolver ... mini-kv echoes Node v323\u0027s no-network fixture contract`，当前 formatter 是同一段边界文字但使用直接 apostrophe。也就是说，drift 不在 `receipt_digest`、`source_node_v323_reference`、`checks`、`summary`、`warnings`、`evidence_endpoints` 或关闭布尔字段里，而是在面向审查者的边界说明文字里。

第四层保留 v1627 的替换后全等证明：只把 fixture 中这一处 `Node v323\u0027s no-network fixture contract` 替换成 `Node v323's no-network fixture contract`，整段 no-network 子对象就等于当前 formatter 输出。这个断言非常硬，因为它比较的是完整 compact JSON 字符串，不是解析后的对象，也不是几个片段。配合 118 顶层字段、唯一 occurrence 和 boundary 定位，no-network drift 已经被查到底：它不是语义漂移，不是 digest 漂移，不是字段顺序漂移，不是 no-network/no-write/WAL/credential/execution 边界漂移。

## 下一次迁移 no-network 前必须做什么

本版故意不迁移 `runtime_credential_resolver_no_network_safety_fixture_receipts.cpp`。原因不是还不知道 drift，而是 canonical surface 还没有显式选择。下一次真正迁移 no-network 时有两条路线。

第一条路线是追求 standalone fixture byte parity。builder 可以保留历史 fixture 的 `\u0027` 字节拼写，让新 formatter 输出与 committed fixture 子对象逐字节相等。这条路线的好处是最符合“fixture 是 oracle”的迁移合同；坏处是会把一个历史转义拼写带入当前 public formatter。第二条路线是保留当前 formatter 的直接 apostrophe，把它视为已经运行中的 public runtime surface，同时写一个专门兼容决策说明：为什么这里不回退到 fixture 的 `\u0027` 拼写、下游如何消费、manifest 和 smoke evidence 如何解释。两条路都可以讨论，但不能在迁移中顺手决定，更不能偷偷改 fixture。

## 测试保护了什么

`receipts_consolidation_note_contract` 保护的是迁移治理规则。它确认设计说明仍包含 necessity proof、shared formatter design、byte-identical output contract、Slice 0、Slice 1、no credential/no network/no write/no execution，以及本版新增的 false shrink metric 修正和 no-network drift 锚点。它不执行 runtime receipt，也不打开网络；它只是防止文档规则退化。

`runtime_receipt_json_builder_tests` 保护的是 builder 与候选 receipt 的迁移入口条件。builder 基础测试继续证明字段顺序、字符串 escaping、raw object/array 不二次转义、false/0/空对象/空数组不被省略。fixture extractor 测试继续证明 malformed input fail closed。frozen fixture 测试继续冻结 v141 no-network 和 v142 abort/rollback 子对象长度与 FNV digest。候选 formatter 测试现在同时证明 abort/rollback 已保持 byte parity，no-network 还未迁移但 drift 已经被压到唯一 `boundary` 转义差异。

focused CTest 先跑 `receipts_consolidation_note_contract` 和 `runtime_receipt_json_builder_tests`。完整版本收尾还会补跑 `smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests`、`release_verification_manifest_tests`、`no_network_safety_fixture_contract_receipt_tests`、`abort_rollback_semantics_contract_receipt_tests` 和 full CTest。这套组合的意义是：文档规则、builder 行为、fixture oracle、SMOKEJSON 暴露、manifest 暴露和两份 receipt 自身边界都一起被检查。只要其中任何一层出现真正漂移，版本不能收口。

## 给后续 Node/Java/mini-kv 融合的价值

Node 和 Java 不需要 mini-kv 变成执行方，它们需要的是可信、稳定、可解释的 evidence。v1628 的价值在于把“证据生产”里的维护判断校准清楚：行数增长不自动等于失败，行数下降也不自动等于好；真正重要的是 byte parity、边界不扩权、测试能证明、文档能解释。no-network drift 被查到底后，下一次迁移就不再是凭感觉修字符串，而是在两条 canonical surface 路线之间做明确工程决策。

一句话总结：v1628 没有推进新的 receipt 迁移，而是修正了会放大噪声的 false shrink metric，并把 no-network 漂移落到唯一 `boundary` 转义差异，为后续 Node/Java/mini-kv 松耦合证据融合保留稳定、透明、可审查的只读边界。
