# mini-kv v1638 审批合同 receipt 迁移前基线讲解

## 1. 本版目标、角色与不是什么

v1638 的目标不是再增加一个 runtime command，也不是把新的 Node、Java 或 mini-kv 服务接起来，而是在 runtime receipts consolidation 进入审批合同组之前，先把三份最接近的审批合同 receipt 做成可执行的迁移前基线。上一版 v1637 已经把 no-network 的历史拼写漂移处理成明确的 canonical runtime surface 决策，并把 no-network formatter 迁移到内部 ordered JSON builder。按 `docs/receipts-consolidation-note.md` 的路线，下一批自然会碰到 approval artifact 相关 receipt；这些文件字段多、边界字段密、字符串输出对顺序和转义敏感，如果没有迁移前 oracle，后面拆 builder 时很容易把“看起来字段都在”误当成“输出没有变”。

本版的角色就是给下一刀重构建立审计底座：从 frozen fixture 中提取嵌套 receipt object，再和当前 public formatter 输出比较。这样后续迁移 formatter 时，不需要靠人工肉眼确认几百行 JSON 字符串有没有少一个逗号、换一个转义、调一处顺序，而是让 CTest 在迁移前后直接证明原始字节或明确 canonical 后的字节是否一致。

它明确不是什么：不是新的执行入口，不打开 HTTP/TCP，不读取 credential value，不解析 raw endpoint URL，不写 Store/WAL，不写 approval ledger，不执行 LOAD/COMPACT/RESTORE/SETNXEX，不启动 Node/Java/mini-kv sibling 服务，也不把 mini-kv 变成订单、审计、审批或 endpoint authority。本版甚至没有迁移任何 formatter；这是一个护栏版本，先把审批合同组里已知的 runtime/fixture 关系钉住，再让后续版本在护栏内做 builder 迁移。

## 2. 为什么先选这三份，而不是一口气迁移所有 approval 文件

审批相关文件很多，但形状并不完全一样。v1638 选择的是三份合同链条最接近的 receipt：

- `runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp`
- `runtime_credential_resolver_credential_handle_approval_receipts.cpp`
- `runtime_credential_resolver_endpoint_handle_allowlist_approval_receipts.cpp`

这三份都围绕“上游 Node 定义一个非秘密合同，mini-kv 只做非参与 receipt echo”展开，核心字段都包括 source Node reference、contract/intake 信息、required/prohibited 字段、checks、summary、evidence endpoints、digest、read-only/no-write/no-network/no-execution 边界。它们适合进入同一个 Slice 2 审批合同基线。

相反，`approval_prerequisite_artifact` 和 `human_approval_artifact_review` 的形态更像 packet、review 或 prerequisite intake，不只是同一模板换名字。把它们硬塞进同一版 builder 迁移，会把“结构真的相似”混成“名字里都有 approval”，后续维护者很难判断哪些字段可以共用、哪些字段只是碰巧相邻。本版只基线化三份最稳的合同链条，就是为了避免把重构做成新的巨型抽象。

## 3. 入口模块与新增测试结构

本版唯一修改的 C++ 测试入口是 `tests/runtime_receipt_json_builder_tests.cpp`。这个测试文件原本已经承担三个职责：验证内部 ordered JSON builder 的基础行为、验证 fixture 子对象提取 helper 的 fail-closed 行为、记录 no-network 与 abort/rollback 的候选 formatter parity 状态。v1638 在这个文件中增加了一个通用断言函数和一个审批合同组测试函数。

新增的 `assert_formatter_matches_fixture_object` 接收四类输入：fixture 文件名、fixture 内部字段名、当前 formatter 输出，以及必须存在的边界片段列表。它先用 `read_fixture_text(release_fixture_path(...))` 读取 frozen release fixture，再用 `extract_json_object_field` 按字段名提取完整嵌套 object。这个提取不是 JSON 反序列化后重排，而是字符串状态机式提取：它理解引号、转义、嵌套对象、数组和花括号深度，最后拿到原始 compact JSON object 字节。然后测试把这个 object 与 formatter 当前输出做直接比较，并继续断言关键边界字段存在。

这里的“直接比较”很重要。很多已有 receipt 测试用 `assert_contains` 检查重要字段，这能保护语义，但不能保护完整字节。比如字段顺序换了、某个 apostrophe 从 `\u0027` 变成直接 `'`、或者某处 compact whitespace 被改动，`assert_contains` 往往看不出来。runtime receipts 的下游消费和 digest 证据链依赖的是已经发布过的 JSON 表面，所以本版增加的是更硬的迁移前证据。

新增的 `approval_contract_slice_formatters_have_fixture_parity_before_builder_migration` 具体覆盖三份 formatter：signed-human approval artifact、credential-handle approval、endpoint-handle allowlist approval。它使用同一组只读命令列表 `INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON`，因为这些 receipt 的 digest 和 runtime 输出一直以这组 read command list 作为基础输入。测试不触发任何写命令，也不启动网络服务，只在进程内调用 public formatter。

## 4. 三份 receipt 的实际基线结论

credential-handle approval 与 endpoint-handle allowlist approval 是最干净的两份：fixture 子对象与当前 formatter 输出逐字节相等。测试要求它们继续包含 `read_only=true`、`execution_allowed=false`、`credential_value_read=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`automatic_upstream_start=false`，以及各自的 `credential_handle_approval_contract_non_participation_receipt_only=true` 或 `endpoint_handle_allowlist_approval_contract_non_participation_receipt_only=true`。这说明后续迁移这两份 formatter 时，最理想的验收就是普通 byte parity：迁移后仍然和 frozen fixture 子对象完全一致。

signed-human approval artifact 暴露了一个更细的事实：它不是完全 byte parity，但差异不是语义漂移。fixture 里 `boundary` 文本使用 `Node v314\u0027s non-secret contract`，而当前 formatter 输出是 `Node v314's non-secret contract`。差异长度正好是 5，因为 JSON 字节里的 `\u0027` 比直接 apostrophe 多 5 个字符。测试因此没有撒谎说它完全相等，也没有改 fixture 或 formatter，而是新增 `signed_human_runtime_surface_canonical_json`，只允许把这一处历史 fixture 拼写规范化成当前 runtime canonical surface 后再比较。

这个 waiver 被写得很窄：fixture 中 `\u0027` 总数必须是 1，legacy 短语必须只出现 1 次，runtime 输出中 legacy 拼写必须是 0 次，runtime canonical 短语必须出现 1 次；两边 top-level field count 必须一致，canonicalized fixture 必须和当前 formatter 完全相等，所有 read-only/no-credential/no-raw-endpoint/no-network/no-write/no-execution 边界字段仍然必须存在。这样一来，signed-human 的历史拼写差异被命名、被定位、被限制，而不是变成一个模糊的“差不多相等”。

## 5. 关键边界字段怎么被控制面理解

这些 receipt 被 Node 或后续控制面读取时，最重要的不是“mini-kv 知道怎样审批”，而是“mini-kv 明确没有参与审批”。`read_only=true` 表示这份证据只是状态与合同边界说明；`execution_allowed=false` 表示控制面不能把它当成执行许可；`credential_value_read=false` 表示 mini-kv 没有读取或接收秘密值；`raw_endpoint_url_parsed=false` 表示 mini-kv 没有解析真实 endpoint；`external_request_sent=false` 表示没有 HTTP/TCP 外呼；`approval_ledger_written=false` 表示没有写审批账本；`automatic_upstream_start=false` 表示不会替 Node 或 Java 启动上游服务。

这些字段共同说明 mini-kv 在四项目关系里的位置：它可以提供 frozen evidence、runtime smoke、manifest、digest 和 no-participation receipt，但不变成 Java order authority，不变成 Node managed audit runtime，也不执行恢复、部署、回滚或真实网络连接。本版测试把这些边界片段放进审批合同三件套的 parity 断言里，是为了防止后续 builder 迁移只关注 JSON 形状，却把最关键的权限否定字段漏掉。

## 6. 为什么这版没有迁移 formatter

从工程节奏看，v1638 没有迁移 formatter 是有意为之。v1637 刚刚证明 no-network 的 historical fixture 与 runtime surface 可以通过一个很窄的 canonical 规则共存；v1638 又发现 signed-human 也存在同型的 apostrophe 漂移。如果同一版里一边发现新漂移、一边迁移三份大型 formatter，评审时就很难判断失败来自哪里：是迁移引入了变化，还是历史 fixture 本来就和 runtime 不完全一致。

把基线和迁移拆开，能让下一版更干净。credential-handle 和 endpoint-handle 可以直接用 byte parity 做首批审批合同迁移；signed-human 则可以在已经命名的单点 canonical spelling waiver 下迁移。这样后续版本的成功标准非常明确：迁移代码可以重写拼装机制，但不能改字段、不能改 digest、不能改 compact output、不能扩大 waiver、不能移动或重写 fixture。

## 7. 本版测试保护了什么

本版 focused test 是 `runtime_receipt_json_builder_tests`。它保护五层东西：第一，builder 的 string escaping、raw nested value、字段顺序、false/0/empty value 不省略仍然成立；第二，fixture 子对象提取 helper 对 malformed JSON、重复字段、非对象目标 fail closed；第三，no-network 和 abort/rollback 的既有 parity/canonical 规则没有被破坏；第四，审批合同三件套有迁移前基线；第五，signed-human 的新漂移被压缩到唯一短语、唯一字段、唯一转义差异。

后续 closeout 还会跑 focused receipt tests、full CTest、format/diff hygiene。它们一起证明本版没有改 public command output、没有改 fixtures、没有改 release manifest、没有打开 router/write/WAL/network/credential/execution 权限。对于 receipts consolidation 来说，这种测试比“文件行数有没有减少”更关键，因为真实风险不是多了几行 helper，而是输出表面被无声改写。

## 8. 对后续 Node/Java/mini-kv 松耦合融合的价值

v1638 给后续松耦合融合的价值是：mini-kv 在不启动 sibling 服务、不读取秘密、不执行网络或写入的前提下，把审批合同 receipt 的历史表面和当前运行表面都讲清楚了。Node/Java 后续如果只读消费这些证据，可以知道哪些 receipt 是严格 byte parity，哪一个 receipt 有唯一已知 apostrophe spelling waiver，以及这些 waiver 没有触及 read-only/no-execution 权限边界。这让后续三项目协作可以继续建立在版本化证据和可失败测试上，而不是建立在“看起来差不多”的人工判断上。
