# mini-kv v1644 代码讲解：signed-human 顶层 builder 与命名 canonical 边界

## 1. 本版解决什么，又明确不是什么

v1644 继续做内部 receipts consolidation，目标是把 `src/runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp` 中的公开 formatter 从长字符串拼接迁移到有序 JSON builder。入口函数、返回值、fixture 路径、digest 算法、`SMOKEJSON` 字段、runtime smoke evidence 和 release manifest 均保持不变。它不是签名功能，不生成或校验签名，不保存审批制品，不读取私钥，不打开 credential resolver，也不把 mini-kv 变成审批权威。

这份 receipt 的作用仍是只读非参与证明。Node 控制面可以据此确认 mini-kv 看见了 signed-human approval artifact 合同形状，同时确认 mini-kv 没有接受秘密值、没有验证 artifact、没有发送网络请求、没有写 ledger/schema、没有执行 LOAD/COMPACT/RESTORE/SETNXEX。最重要的解释字段仍是 `read_only=true`、`execution_allowed=false`、`order_authoritative=false`。代码重构不能把“结构更清楚”误写成“能力更强”。

## 2. 为什么 signed-human 是单独一版

credential-handle 和 endpoint-handle 在 v1642/v1643 已完成同类顶层迁移，但 signed-human 不能和它们机械合并。冻结 release fixture 中有一处历史拼写 `Node v314\u0027s non-secret contract`，当前运行时 formatter 则输出 `Node v314's non-secret contract`。两者解析为同一文本，却不是同一字节。v1638 已把这件事钉死成 signed-human family 自己的命名单点兼容规则。

因此 v1644 的风险不是 builder 能否拼出 JSON，而是迁移过程中会不会把单点规则泛化成“所有 apostrophe 都可忽略”。如果测试对整份字符串做通配替换，未来第二处转义漂移也会被吞掉，byte parity 门便失去意义。正确做法是继续断言 fixture 中 legacy phrase 恰好一次、runtime 中 canonical phrase 恰好一次、两侧字段数相同、fixture 只多五个转义字符，然后仅替换那个命名常量再比较整份对象。

这条规则只存在于 `tests/runtime_receipt_json_builder_tests.cpp`。生产 builder 不包含 canonicalization，不识别 Node v314，也不遍历字符串做替换。这样测试可以诚实描述冻结历史与当前 runtime surface 的差异，同时生产代码继续保持单一职责。

## 3. 从长字符串到命名对象

迁移前的 formatter 把十余个结构连续写在一个 return 表达式中。`source_node_v314_reference` 里面又嵌套 signed artifact contract、prerequisite transition、necessity proof 和 summary；后面还有 Node v312 reference、mini-kv receipt、另一份 summary、warnings、recommendations、evidence endpoints 与 action arrays。阅读者需要靠肉眼追踪花括号层级和逗号。

v1644 把它拆成以下局部值：

- `signed_artifact_contract` 保存 contract digest、artifact name/version、purpose、required/prohibited fields、拒绝理由和 no-go 边界；
- `prerequisite_transition` 说明 v314 只是把 missing prerequisite 推到 contract-intake-defined，并未关闭后续 credential/endpoint/no-network/abort-rollback 前置条件；
- `necessity_proof` 写明为什么 v312 不能复用以及遇到秘密值、网络、写入或自动启动要求时必须停止；
- `source_node_v314_reference` 聚合当前合同和只读边界，`source_node_v312_reference` 保存此前 governance stop 决策；
- `mini_kv_receipt`、顶层 `summary`、warnings、recommendations、evidence endpoints、next evidence versions 与 next actions 分别承担独立结构责任。

字符串值经过 `json_string`，整数经过 `json_integer`，布尔值经过 `json_bool`。already-encoded 的领域数组继续由原 helper 生成，作为 raw JSON value 进入 `OrderedJsonField`。最终顶层 `fields` 按历史顺序排列，再交给 `json_object(std::span<...>)` 输出 compact JSON。builder 不排序字段、不推导状态，也不改变任何领域 helper。

## 4. 临时 bridge 为什么在这一版删除

v1641 只迁移闭合边界 flag block 时，顶层仍是字符串，所以代码先把 boundary fields 生成完整 object，再用 `substr` 剥去外层花括号，把内部片段拼回旧 return 表达式。这是受测试保护的安全过渡，但它造成了不必要的中间字符串，也让字段所有权分成“顶层长字符串”和“builder fragment”两套机制。

v1644 顶层已经是 `std::vector<OrderedJsonField>`，所以直接调用 `append_closed_boundary_flag_fields(fields)`。之后只需追加 `boundary` 和 `node_action`。`format_closed_boundary_flags_json()` 被删除，闭合字段没有复制，也没有改变位置。这个删除是结构完成信号：signed-human 不再处于半迁移状态。

## 5. signed artifact 专属安全边界

本文件没有抽成一个万能 approval boundary 模板，因为 signed artifact 的风险面独特。除公共的 runtime shell、credential、raw endpoint、network、Store/WAL write、schema、restore/load/compact 和 auto-start 外，它还必须显式关闭：

- `signing_private_key_accepted/read/stored/rendered=false`；
- `signed_approval_artifact_stored/validated/authoritative=false`；
- `signed_approval_artifact_signature_verified=false`；
- approval ledger write、managed audit storage 与 production record write 均为 false。

这些字段继续由本文件的 `append_closed_boundary_flag_fields` 逐项追加。局部所有权使代码审查者不用理解一组布尔参数如何配置共享模板，便能直接确认“私钥从未进入系统、artifact 从未验证、mini-kv 不具审批权”。这比追求更少代码行更重要。

源文件从 491 行变为 545 行。增长来自把横向字符串展开为纵向结构字段，不是功能膨胀。项目已经明确：行数是 review metric，不是 stop condition。真正停止迁移的条件是 fixture/canonical 规则扩大、字段数变化、digest 漂移、边界值变化或消费表面失败。

## 6. 测试如何证明没有“借重构开能力”

`runtime_receipt_json_builder_tests` 同时保护 builder 基础行为和 signed-human parity。测试先提取 frozen fixture 子对象，断言唯一 legacy phrase，再调用公开 formatter，断言唯一 runtime canonical phrase。只做一次命名替换后，整个对象必须相等；字段数、长度差和关键 closed flags 也要匹配。这证明兼容规则没有扩散。

`signed_human_approval_artifact_contract_receipt_tests` 保护合同语义与 digest。它验证 required/prohibited 字段、来源版本、warnings/recommendations、receipt digest，以及 private key、signed artifact、credential、raw endpoint、network、write、schema 和 execution 边界。

`smokejson_command_receipt_tests` 证明命令聚合仍暴露同一 receipt；`runtime_smoke_evidence_tests` 证明运行时证据聚合没有漏项；`release_verification_manifest_tests` 证明发布清单仍引用同一 fixture 与 digest。五条 focused lane 共同覆盖 standalone、command、runtime 与 release 四个消费面。

最终完整 build 与 344 条 CTest 继续覆盖 KV、WAL、snapshot、RESP/TCP、OSFS、shard readiness 和所有历史证据门。真实 CLI 运行 `SMOKEJSON`，再用 `CHECKJSON LOAD data/prod.snap` 证明 LOAD 只被分类为 write/side effect，没有执行、没有替换 Store、没有触碰 WAL。若只跑 formatter 单测，无法给出这种跨模块不回归证据。

## 7. 需求到证据矩阵

| 要求 | 实现 | 证据 | 结论 |
| --- | --- | --- | --- |
| 完整迁移 signed-human 顶层拼接 | 命名 object/array + ordered top-level fields | 编译、focused parity、源码审查 | 完整 |
| 保留单点 canonical 规则 | 测试中的两个命名 phrase 常量 | 出现次数、长度差、字段数、整对象比较 | 完整 |
| 不在生产代码做 normalization | builder 与 formatter 无替换逻辑 | 代码搜索与测试边界 | 完整 |
| 保持 digest 与消费表面 | digest parts 不变，public formatter 不变 | receipt/SMOKEJSON/runtime/manifest tests | 完整 |
| 保持私钥、artifact、网络、写入、执行关闭 | 本地 ordered boundary fields | 领域测试、parity、CLI、full CTest | 完整 |
| 清除半迁移桥接 | 删除 strip-and-splice helper | 编译与源码差异 | 完整 |

## 8. 对抗性自审与后续方向

最强质疑是：既然测试允许 fixture 与 runtime 有一处不同，是否已经放弃 byte parity？答案是否定的。它允许的不是“任意一处不同”，而是一个具体 family、一个具体字段短语、一个具体旧值到新值的单次映射；测试同时断言任何第二处 legacy escape 都失败。规范化之后仍比较完整对象，不是只检查若干片段。这个机制比静默改 fixture 更诚实，也比通配字符串替换更严格。

v1644 完成了 Slice 2 的 credential-handle、endpoint-handle、signed-human 三份审批合同顶层迁移，但整个 28 文件 census 仍远未完成。下一步应先建立下一组同构 formatter 的冻结 parity 与 census 门，尤其要识别是否还有各自的历史转义差异；没有基线就不批量迁移。这样优化速度服从证据质量，而不是服从版本数字。

一句话总结：v1644 在不隐藏 signed-human 唯一历史拼写差、不改变任何 public 字节语义和执行边界的前提下，完成审批合同三件套的结构收敛，让 Node/Java/mini-kv 松耦合证据链更容易审查，却仍只代表 `single-project validation + cross-project contract alignment`。
