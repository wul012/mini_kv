# v1641 signed-human approval 边界 flag builder 迁移讲解

## 1. 本版目标、角色和“不是什么”

v1641 的目标很窄，也很明确：把 `runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp` 里 signed-human approval artifact contract receipt 的闭合边界布尔字段块，从手写 JSON 字符串迁移到内部 ordered JSON builder。这里的“闭合边界布尔字段块”，指的是 receipt 尾部那一长串 `read_only`、`execution_allowed`、`credential_value_read`、`raw_endpoint_url_parsed`、`external_request_sent`、`approval_ledger_written`、`schema_migration_executed`、`automatic_upstream_start`、`audit_authoritative`、`order_authoritative` 等 true/false 字段。它们不是业务数据本身，而是给 Node/Java/审计方看的边界证明：mini-kv 只提供只读证据，不成为审批、凭证、endpoint、网络连接、写入、迁移或执行的权威。

本版不是新的命令，不是新的 router，不是新的执行入口，也不是把 mini-kv 接入 managed audit。它不读取 credential value，不解析 raw endpoint URL，不发 HTTP/TCP 请求，不写 Store/WAL，不写 approval ledger，不执行 schema migration，不执行 restore/load/compact/setnxex，也不自动启动 Node、Java 或 mini-kv 服务。更重要的是，本版不宣称“signed-human approval formatter 整体已经完成迁移”，也不宣称“approval contract family 已经全部 builder-owned”。它只把一个最机械、最容易因逗号或字段顺序出错的尾部片段变成结构化 ordered field 清单。

为什么这一刀仍然有价值？因为 signed-human、credential-handle、endpoint-handle 这三份 approval contract receipt 都有大量相似的危险边界字段。v1639 已经迁移 credential-handle 的边界 flag block，v1640 已经迁移 endpoint-handle 的同类 block。v1641 补上 signed-human 后，Slice 2 的第一轮“审批合同闭合边界尾部收束”就形成了对称形态：三份 receipt 的危险能力开关都不再靠肉眼维护一条巨长字符串，而是以 `OrderedJsonField` 逐项列出，再交给同一个 `json_object` 负责 compact JSON 拼装。

## 2. 入口文件和响应结构

本版入口仍然是公开 formatter：

```cpp
format_credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_json(read_commands)
```

它的调用方没有变化。这个函数仍然返回一整个 compact JSON object，里面包含 receipt 元数据、Node v314 source reference、Node v312 source reference、mini-kv receipt 指针、checks、summary、warnings、recommendations、evidence endpoints、digest、边界 flags、`boundary` 文本和 `node_action`。因此下游看到的响应形状不应发生变化：SMOKEJSON 仍然嵌入同一份 receipt，release manifest 仍然引用同一个测试和同一个 receipt 字段名，独立 fixture 仍然保留历史 bytes。

本版只改一个内部 helper：

```cpp
format_closed_boundary_flags_json()
```

旧写法直接返回一段 `"field":value,"field":value` 片段。新写法分成两层：`append_closed_boundary_flag_fields(std::vector<OrderedJsonField>& fields)` 负责按历史顺序压入字段；`format_closed_boundary_flags_json()` 负责调用 `json_object(fields)`，生成带外层花括号的 object，然后用 `substr(1, object_json.size() - 2)` 剥掉花括号，嵌回旧 formatter 的同一个位置。这个剥花括号动作看似绕，但它是当前阶段最稳的方式。旧 formatter 的顶层 object 还没有整体迁移，如果现在顺手把整个 object 重写成 builder，就会同时触碰 Node reference、checks、summary、warnings、recommendations、evidence endpoints、digest、boundary 文本等多个区域，风险面会扩大很多。

## 3. signed-human 的特殊风险：单点 canonical spelling waiver

signed-human receipt 和 v1639/v1640 的两个样本有一个关键差异：它带着 v1638 已命名的历史拼写 waiver。冻结 fixture 里有一句 `Node v314\u0027s non-secret contract`，而当前 public formatter 输出的是 `Node v314's non-secret contract`。这不是 v1641 新制造的差异，而是 v1638 已经查清、命名、测试保护的 runtime surface 差异。

所以 v1641 的原则是：保持这个 waiver 的单点性质，不把它扩大。测试仍然要求 fixture 里 `\u0027` 只出现一次，目标短语只出现一次，runtime formatter 中 legacy 拼写出现零次；只有把这一处命名短语规范化为当前 runtime canonical surface 后，formatter 输出才允许和 fixture 子对象比较。这一点很重要，因为 receipts consolidation 的底线不是“输出看起来像 JSON”，而是“已提交 fixture、manifest、digest 和下游可消费字段不发生未解释漂移”。本版改的是边界 flag 的拼装机制，不是改变 signed-human 的 canonical surface 决策。

换句话说，v1641 不是放松 byte parity，而是在已存在的单点兼容规则之内继续收敛实现。若本次迁移导致第二处 `\u0027` 差异、字段数量变化、字段顺序变化、digest 变化、compact whitespace 变化、危险边界 true/false 变化，`runtime_receipt_json_builder_tests` 和 focused receipt tests 都应该失败。

## 4. 核心迁移流程

旧的 `format_closed_boundary_flags_json()` 是一段连续字符串，维护者必须在里面数引号、数逗号、看字段顺序。新代码把每个字段写成：

```cpp
fields.push_back({"field_name", json_bool(false)});
```

正向证明字段仍然靠前：`read_only=true`、`execution_allowed=false`、`signed_human_approval_artifact_contract_non_participation_receipt_only=true`、`signed_human_approval_artifact_contract_intake_only=true`、`read_only_artifact_contract=true`，以及消费 Node v314/v312/v313 的只读来源字段。随后是能力边界：runtime shell 没实现、没启用、没允许调用；real resolver 没实现；resolver client、secret provider、fake provider/client 都没实例化；provider client instantiation 不允许；credential value 不能 accepted/read/provided/loaded/stored/included/rendered；raw endpoint URL 不能 accepted/parsed/rendered；signing private key 不接受；signed human approval artifact 不存储、不验证、不成为权威、不校验签名。

再往后是外部副作用边界：external request payload 不接受，external request 不允许也没发送，HTTP/TCP dial 不允许；不连接 managed audit，不读取或存储 managed audit credential；不成为 managed audit store、managed audit storage backend 或 sandbox audit storage backend；storage write、write command、admin command、runtime write 都为 false。ledger/schema/restore 方向也继续关闭：approval ledger write 不允许、不发生，managed audit write 不发生，production record 不写，schema migration 不允许也不执行，schema rehearsal/migration execution 不允许，restore/load/compact/setnxex 全部不执行。最后是 auto-start 和 authority：Node/Java/mini-kv auto-start 都不允许，automatic upstream start 不发生，audit/order authority 都为 false。

这些字段的语义没有变化，但表达方式变了。旧写法更短一点时，并不天然更安全；只要有一处漏逗号、少引号、换顺序，输出就会漂。新写法的价值在于把危险能力逐项显式化。维护者以后审查 signed-human receipt 时，可以按字段清单读：这个能力是不是仍然 false？这个 true 是不是只读证明字段？字段顺序是不是还和历史输出一致？这比在超长字符串里找局部片段可靠得多。

## 5. 测试保护了什么

本版 focused lane 先跑 `runtime_receipt_json_builder_tests`。这组测试保护 builder 的基础行为：字符串 escaping、raw object/array 不二次转义、字段顺序保持输入顺序、false/0/空对象/空数组不被省略、fixture 子对象提取 fail closed。对 signed-human 来说，它还保护 v1638 的 canonical spelling waiver：fixture 与 runtime 只允许那一处命名差异，规范化后必须完全相等，并且 read-only、execution_allowed、credential、raw endpoint、external request、approval ledger、automatic upstream start 等关键边界片段必须存在。

第二个 focused test 是 `signed_human_approval_artifact_contract_receipt_tests`。它不是单纯比较字符串，而是从独立 fixture、SMOKEJSON、verification manifest、真实 `CommandProcessor` 命令响应几个角度检查同一个 receipt。它保护 Node v314 source reference、合同字段、Node v312 source decision、transition summary、signed artifact 不存储不验证不权威、credential/raw endpoint/network/write/schema/auto-start 关闭、digest 引用、manifest 暴露和命令输出暴露。也就是说，即使 builder 测试没发现某个上下游嵌入点问题，这个 receipt 专项测试也会把它卡住。

第三组是 `smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests`。它们分别保护 public command surface、runtime smoke evidence、release manifest。v1641 没有修改 fixture、manifest 或命令入口，但这些测试仍然必须跑，因为 receipt formatter 的输出会被 SMOKEJSON 嵌入，也会被 release manifest 作为交付证据引用。只跑单个 formatter 测试不够；必须证明“下游看见的证据面”也没有漂。

最后，本版跑了完整 build 和 full local CTest，结果是 344/344 通过。这一步的意义是排除隐性连锁影响：signed-human 文件属于 runtime evidence receipt 家族，和 CMake target、SMOKEJSON、manifest、receipt tests、current runtime fixture rolling guard 同处一个大证据系统。全量 CTest 通过，说明本次修改没有污染 OSFS、KV 命令、WAL、snapshot、TCP/RESP、archive retention、coverage contract 或其他 non-participation receipt。

## 6. 本版给后续维护的价值

v1641 之后，Slice 2 的三份 approval contract 样本已经形成一致的第一层结构：credential-handle、endpoint-handle、signed-human 的闭合边界 flag block 都是 builder-backed。这个状态还不是 receipts consolidation 的终点，却是一个可复用的稳定台阶。下一步如果继续推进，可以选择迁移其中一份的 top-level/nested section，也可以进入 approval prerequisite/human approval review 这类相邻 receipt，但不能把“边界 flag block 已迁移”误读成“整份 formatter 已完成”。

对 Node/Java/mini-kv 松耦合集成来说，本版的价值是一句话：mini-kv 把 signed-human approval artifact 这条证据里的危险能力边界变得更可审查，同时继续证明自己只提供只读、无网络、无写入、无执行、无 authority 的证据，不替 Node 或 Java 做审批执行。
