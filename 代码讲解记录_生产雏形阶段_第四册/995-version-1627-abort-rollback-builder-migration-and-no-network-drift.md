# 第一千六百二十七版代码讲解：abort/rollback receipt 迁移与 no-network 漂移诊断

本版目标是推进 runtime receipts consolidation 的第一刀，但只碰已经被证明安全的那一半。v1626 已经建立了内部 ordered JSON builder 和 fixture 子对象 byte-parity harness，并且给两个候选 receipt 做了迁移前体检：abort/rollback 当前 formatter 和冻结 fixture 子对象完全相等，no-network 当前 formatter 与 fixture 子对象不相等。v1627 因此选择先迁移 abort/rollback，把 `runtime_credential_resolver_abort_rollback_semantics_receipts.cpp` 从大段手写字符串拼接改为更可维护的 ordered field 组装；同时先把 no-network 的漂移查清楚并写进测试，暂时不动 no-network formatter。

这版的角色是内部维护和重构，不是新功能。它不是新的命令入口，不新增 `SMOKEJSON` 字段，不修改 release fixture，不移动归档，不打开 router，不执行网络请求，不读 credential value，不解析 raw endpoint URL，不触发 Store/WAL 写入，不执行 rollback、deployment、Java SQL、`LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。它只是把已经存在的一份只读 receipt 用更稳定的内部拼装工具表达出来，并把另一份还不适合迁移的 receipt 的漂移事实压缩成可审查的、可复现的一行字节差异。

## 入口与文件职责

本版的生产代码入口仍然是已有的 public formatter：

```text
src/runtime_credential_resolver_abort_rollback_semantics_receipts.cpp
format_credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_json(...)
```

外部调用者不知道这次迁移。命令层、manifest、runtime smoke evidence 和 receipt 测试仍然调用同一个函数名，得到同一段 compact JSON。改变的是函数内部如何拼装这段 JSON：过去是把大量 `"`、逗号、布尔值和 raw JSON 片段直接串接；现在先把每个对象段落声明成 `OrderedJsonField` 列表，再交给 `runtime_receipt_json::json_object` 输出。这个 builder 放在 `src/runtime_receipt_json_builder.hpp/.cpp`，不是 public include，因此它是 mini-kv 内部实现细节，不成为 Node、Java 或其他项目的新 API。

测试入口是：

```text
tests/runtime_receipt_json_builder_tests.cpp
tests/receipt_fixture_parity.hpp
```

`receipt_fixture_parity.hpp` 负责从已经提交的 `fixtures/release/*.json` 中按字段名提取嵌套 receipt 子对象。它不把 JSON 解析成 map 后再序列化，因为那样会丢掉字段顺序、空白、转义拼写等字节级信息。本项目的证据链经常依赖 digest、fixture 路径和历史输出片段；所以这里要保护的是原始字节，而不只是“JSON 语义差不多”。`runtime_receipt_json_builder_tests.cpp` 则负责三件事：验证 builder 的基本行为，验证 abort/rollback 迁移后的 formatter 仍与 fixture 子对象逐字节相等，验证 no-network 的漂移到底是什么。

## abort/rollback 为什么可以先迁移

迁移顺序不是随手选的。v1626 已经证明 abort/rollback 的当前 formatter 与 fixture 中的 `credential_resolver_abort_rollback_semantics_contract_non_participation_receipt` 子对象完全相等。这意味着它适合作为第一份迁移样本：如果迁移后测试仍然相等，就能说明 builder 没有改变字段顺序、转义、compact whitespace、digest 输入或边界字段。

文件内部仍然保留本 receipt 的领域常量：`receipt_version`、`receipt_consumer`、fixture path、release version、Node v326/v325 来源信息、contract digest、required/prohibited/rejection/no-go/upstream echo 计数，以及 boundary 文本。这些不是 builder 的职责。builder 只负责把已经确定的 JSON value 按给定顺序拼装出来。换句话说，领域语义仍在 receipt 文件里，公共拼装机制才进入 builder。这样可以避免把 26 个 credential resolver receipt 都硬塞进一个巨大 `ReceiptSpec`，也避免让 builder 理解 Node 版本、审批语义或 rollback 权限。

本版新增了两个小 helper：

```text
append_bool_fields(...)
append_closed_boundary_flag_fields(...)
```

它们只服务于 abort/rollback 这份 receipt 的关闭边界字段。旧代码里 `format_closed_boundary_flags_json()` 返回一长串手写 JSON 片段，维护者要在一堆转义引号和逗号里确认某个字段是 true 还是 false。新代码把字段按含义分组：先放 `read_only=true`，再放 `execution_allowed=false`，再放少量“这份 receipt 已具备/消费某来源”的 true 字段，最后集中列出大量必须为 false 的边界字段。输出顺序仍由 push 顺序决定，所以字节合同不变；但阅读时不再需要在人眼里模拟 JSON tokenizer。

## 响应结构如何保持不变

abort/rollback formatter 生成的顶层 JSON 仍然按历史顺序组织。前半部分是 receipt 元信息：版本、fixture 路径、来源合同、当前 release hint、runtime role。中间是两个来源引用：`source_node_v326_reference` 描述 Node v326 abort/rollback semantics contract intake，`source_node_v325_reference` 描述 Node v325 no-network safety fixture prerequisite closure review。随后是 `mini_kv_receipt`，说明 mini-kv 只是提供 non-participation receipt。再后面是 `checks`、`summary`、`production_blockers`、`warnings`、`recommendations`、`evidence_endpoints`、`next_required_evidence_versions` 和 `next_actions`。最后是 provenance/digest 字段、关闭边界字段、`boundary` 和 `node_action`。

其中一些段落仍然通过原有 raw JSON helper 输出，例如 `format_required_fields_json()`、`format_prohibited_fields_json()`、`format_checks_json()`。这不是偷懒，而是刻意降低迁移风险：第一刀只收敛对象拼装方式，不同时重写每一个数组和检查表。只要这些 raw helper 的内容没有变化，digest 与 fixture parity 就有更清晰的归因。如果未来要继续收敛这些数组，也应该单独开一版，用同样的 byte-parity harness 证明。

关键边界字段继续表达同一件事：`read_only=true` 表示 mini-kv 只提供只读证据；`execution_allowed=false` 表示 receipt 不批准任何执行；`credential_value_read=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`http_request_sent=false`、`tcp_connection_attempted=false`、`network_socket_opened=false` 共同说明没有 credential、endpoint 或网络行为；`rollback_executed=false`、`deployment_action_executed=false`、`java_sql_executed=false`、`load_restore_compact_executed=false` 说明没有 rollback、部署、Java SQL 或存储恢复类执行；`mini_kv_write_command_allowed=false`、`storage_write_allowed=false`、`write_commands_executed=false`、`approval_ledger_written=false` 说明没有写权限扩张；`order_authoritative=false` 和 `audit_authoritative=false` 说明 mini-kv 没有变成订单或审计权威。

## no-network 漂移到底是什么

用户这次特别要求“先把 no_network 的漂移查清楚，再碰它”。本版照这个顺序执行。诊断不是用肉眼扫字符串，而是同时做了结构和原始字节两层检查。

结构层面，当前 formatter 和 committed fixture 子对象都包含 118 个顶层字段。字段顺序一致，递归解析后的值没有差异，`receipt_digest` 都是 `fnv1a64:3d8c483c93f8acf9`。这说明 no-network 不是语义漂移：没有多字段、少字段、字段顺序错乱、布尔边界翻转、digest 变化或隐藏的 network/write/execution 改动。

字节层面，唯一差异是一段来源说明中的 apostrophe 拼写。fixture 里是：

```text
Node v323\u0027s no-network fixture contract
```

当前 formatter 里是：

```text
Node v323's no-network fixture contract
```

JSON 语义上两者都是同一个字符串，但原始字节不同。`\u0027` 是六个字符，直接 apostrophe 是一个字符，所以 fixture 比当前 formatter 长 5 个字符。测试现在明确断言这个事实：fixture 长度等于 current 长度加 5；fixture 包含 `Node v323\\u0027s...`；current 包含直接 apostrophe；current 不包含 `\u0027` 写法；并且只替换这一处后，fixture 子对象就与 current formatter 完全相等。

这个测试很重要，因为它防止后续维护者误判。不能说 no-network 已经 byte-parity，也不能说 no-network 有语义风险。准确说法是：no-network 当前只有一个历史转义拼写漂移，语义和边界都一致。下一版若迁移它，必须显式选择是保留 fixture 的 `\u0027` 字节拼写，还是把当前 formatter 的直接 apostrophe 作为运行时表面并做兼容说明。最糟糕的做法是在重构时顺手把它“格式化掉”，然后让 fixture、manifest 或下游消费方悄悄承担变化。

## 核心流程

迁移后的 abort/rollback formatter 从最内层对象开始构造。`abort_rollback_contract` 描述 contract digest、contract name/version/mode、required/prohibited fields、rejection reasons、no-go boundaries 和 upstream echo requests。`prerequisite_transition` 描述 abort/rollback semantics 在 prerequisite 链上的状态变化。`necessity_proof` 解释为什么 v325 不能被复用为 final abort/rollback contract。然后这些对象被嵌入 `source_node_v326_reference`，与 `source_node_v325_reference` 一起放进顶层 receipt。

接着构造 `mini_kv_receipt`、`summary`、`warnings`、`recommendations`、`evidence_endpoints`、`next_required_evidence_versions` 和 `next_actions`。这些对象或数组都由 `json_object` / `json_array` 产生，普通字符串用 `json_string`，布尔用 `json_bool`，整数用 `json_integer`。这几个 helper 的明确分工让调用点更可读：看到 `json_string` 就知道这是普通文本，看到 raw helper 直接放入 field value 就知道它已经是一段 JSON。

最后构造顶层 `std::vector<OrderedJsonField> fields`。这里先推入历史顶层字段，再调用 `append_closed_boundary_flag_fields(fields)` 插入所有关闭边界，最后追加 `boundary` 和 `node_action`。返回时使用 `std::span<const OrderedJsonField>{fields.data(), fields.size()}`，把顺序交给 builder 输出。这个顺序就是历史字节合同的核心：builder 不排序、不省略、不美化，也不理解业务。

## 测试保护什么

`runtime_receipt_json_builder_tests` 的 builder 基础断言保护的是拼装工具本身。它验证字段顺序保持输入顺序，字符串转义仍然正确，false、0、空对象、空数组不会被省略，raw object 和 raw array 不会被二次转义。这个层面的测试不是为了 abort/rollback 一份 receipt，而是为了后续 26 个 credential resolver receipt 的迁移底座。

fixture extraction 断言保护的是 oracle 的可靠性。`receipt_fixture_parity.hpp` 必须能从 committed fixture 中抽出完整 object，并且在坏字段名、非 object 目标、重复字段、未闭合结构等情况下 fail closed。否则 byte parity 测试会变成假安全：看似比较了 fixture，实际上可能只比较了错误片段。

abort/rollback 断言保护的是本版迁移成果：迁移后的 public formatter 与 fixture 子对象仍然完全相等，并且核心边界字段仍然存在。配套的 `abort_rollback_semantics_contract_receipt_tests`、`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 进一步证明这份 receipt 仍然能被 runtime evidence、SMOKEJSON 和 release manifest 消费，没有因为内部拼装方式变化而断链。

no-network 断言保护的是下一步入口条件。它不迁移 no-network，但把 drift 从“已知不相等”升级为“唯一一处可命名的字节拼写差异”。这类测试比文档备注更硬：后续如果有人改了 no-network 其他字段、边界或 digest，测试会直接失败；如果有人想迁移 no-network，也必须面对这条断言，而不是忽略 v1626 留下的警告。

全量 CTest 342/342 保护的是系统面。它重新跑过 WAL、snapshot、RESP、TCP、command、runtime evidence、release manifest、credential resolver receipt、archive/governance 等测试。由于本版只改内部 formatter 和测试，full CTest 的价值在于证明没有侧向破坏命令执行、读写语义、TCP 行为、fixture 消费或证据链。

## 对松耦合集成的价值

Node、Java 和 mini-kv 的松耦合关系依赖一个朴素前提：证据可以被消费，但证据生产方不能趁机变成执行方。v1627 的价值就是把这个前提落实到维护工程里。abort/rollback 的 receipt 更易维护了，但它仍然只是 non-participation receipt；no-network 的漂移更透明了，但它没有被偷偷改写。控制面读到这些字段时，仍然应该理解为 mini-kv 只报告“不参与、不执行、不写入、不联网、不成为 authority”，而不是提供任何 resolver、rollback 或 network fixture 能力。

一句话总结：v1627 把第一份安全 receipt 迁移到可维护的 ordered builder，同时把 no-network 的字节漂移查成一条明确事实，为后续 Node/Java/mini-kv 的松耦合证据融合减少维护噪声，而不扩大任何运行时权力。
