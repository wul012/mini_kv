# v1642 credential-handle 顶层 builder 迁移讲解

## 1. 本版目标、角色和不是什么

v1642 继续 mini-kv 的 runtime receipts consolidation，但这一版只做一件事：把 `src/runtime_credential_resolver_credential_handle_approval_receipts.cpp` 里 credential-handle approval contract receipt 的顶层 JSON 组装，从一整段长字符串拼接迁移到内部 ordered JSON builder。它不是新的 runtime command，不是新的 managed audit router，不是新的 credential resolver，也不是把 mini-kv 接成 Java/Node 的审批状态存储。它不读 credential value，不解析 raw endpoint URL，不发 HTTP/TCP，不写 approval ledger，不执行 schema migration，也不触发 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`。

这版的角色是维护性重构。v1625 的设计说明已经把问题量化过：大量 `runtime_*_receipts.cpp` 文件共享相似的 compact JSON 骨架，但又有各自的领域字段。继续靠长字符串拼接，风险不是“现在输出错了”，而是后续维护时很容易漏逗号、调错字段顺序、把 raw JSON 二次转义，或者在几十个 receipt 文件里重复修同一个边界字段。v1639-v1641 已经把三份 approval 合同的闭合边界 flag block 改成 `OrderedJsonField`。v1642 往前推进一步：选最干净的 credential-handle，把整份 top-level object 也迁到 builder。

为什么不同时改 endpoint-handle 和 signed-human？因为版本边界要窄。credential-handle 在 v1638 已经证明 frozen fixture 子对象与当前 public formatter 完全 byte parity；endpoint-handle 也具备这个条件，但它有更多 raw-endpoint/allowlist authority 字段；signed-human 还有 v1638 命名的 `Node v314\u0027s` 与 `Node v314's` 单点历史拼写差。先把 credential-handle 做成完整样本，能让下一刀照着一个已验证模式迁移，而不是在一个版本里同时处理三种复杂度。

## 2. 入口模块和响应结构

入口函数仍然是：

```cpp
format_credential_resolver_credential_handle_approval_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands)
```

这个函数对外签名没有变化，调用方也没有变化。它仍然被 SMOKEJSON、runtime smoke evidence、release manifest 和 focused receipt tests 消费。digest 函数 `credential_resolver_credential_handle_approval_contract_non_participation_receipt_digest` 也没有改变 digest parts 的内容和顺序。也就是说，下游看到的还是同一个 receipt：它描述 mini-kv 对 Node v317 credential-handle approval contract 的只读 non-participation receipt，告诉 Node v318 前置验证可以读取这份证据，但不能把 mini-kv 当成 credential handle 存储、验证、解析、审批或权威来源。

响应结构仍按历史字段顺序输出。顶层从 `receipt_version`、`receipt_fixture_path`、`source_contract_intake`、`source_closure_review`、`source_prerequisite_catalog` 开始，接着是 `consumer_hint`、当前项目版本、release version、artifact path hint 和 runtime role。随后是两个 Node 来源对象：`source_node_v317_reference` 表示 credential-handle approval contract intake 本身，`source_node_v316_reference` 表示前一个 signed-human prerequisite closure review。再往后是 `mini_kv_receipt`、`checks`、`summary`、空 `production_blockers`、`warnings`、`recommendations`、`evidence_endpoints`、后续 evidence version、next actions、binary/retention/read command/receipt digest，最后追加闭合边界字段、`boundary` 和 `node_action`。

本版迁移前，这个顺序靠一条很长的字符串表达。迁移后，这个顺序靠 `std::vector<OrderedJsonField> fields` 表达。字段顺序没有交给 `std::map`，也没有交给会重排 key 的 JSON 库，而是由 vector 中的插入顺序决定。这个选择是本项目 receipts consolidation 的核心：我们不是要“漂亮地解析 JSON”，而是要稳定地产生历史 compact JSON surface。

## 3. 核心改动：把长字符串拆成命名对象

v1642 最重要的代码变化在 formatter 函数内部。原代码大致是：

```cpp
return "{\"receipt_version\":" + field_string(receipt_version) +
       ",\"source_node_v317_reference\":{...}" +
       ",\"source_node_v316_reference\":{...}" +
       ",\"mini_kv_receipt\":{...}" +
       ...
       "," + format_closed_boundary_flags_json() +
       ",\"boundary\":" + field_string(boundary_text) +
       ",\"node_action\":" + field_string(node_action) + "}";
```

新代码把这些隐含结构拆开命名。比如 `credential_handle_approval_contract` 是 Node v317 合同对象，里面包含 contract digest、name、version、mode、source span、target prerequisite、purpose、required/prohibited fields、rejection reasons、no-go boundaries、upstream echo requests 和各种 count。`prerequisite_transition` 只讲 prerequisite 从 still-missing 到 contract-intake-defined 的状态变化。`necessity_proof` 只讲为什么 v316 不能复用、为什么 v317 需要单独存在、什么时候必须停止。`source_node_v317_reference` 把这些对象聚合起来，同时继续明确 runtime shell、managed audit connection、credential value、raw endpoint、external request、schema migration、approval ledger 和 automatic upstream start 都是 false。

这种拆分的价值不是减少行数。事实上，严格把每个字段拆成 `OrderedJsonField` 后，diff 行数可能增加。它的价值是把 JSON 结构从“字符串里的运气”变成“可逐项审查的数据”。维护者现在能直接看到：

```cpp
{"credential_value_read", json_bool(false)}
{"raw_endpoint_url_parsed", json_bool(false)}
{"external_request_sent", json_bool(false)}
{"approval_ledger_written", json_bool(false)}
```

这些边界不是藏在一条长字符串里，而是作为字段清单存在。以后如果某个 receipt 需要新增证明字段，审查者可以看它插在什么位置、是不是改变了历史顺序、是不是误把 false 改成 true、是不是绕开了 helper。对 receipt 这种下游证据 surface 来说，这比单纯“文件短一点”更有维护价值。

## 4. builder 负责什么，领域 helper 仍负责什么

`runtime_receipt_json_builder` 在本版里承担三类机械工作。第一，`json_string`、`json_bool`、`json_integer` 统一输出 JSON value。第二，`json_object` 按 `OrderedJsonField` 的输入顺序拼 compact object。第三，`json_array` 按输入顺序拼 compact array。这些 helper 不理解 Node 版本，也不理解 credential approval 的业务含义。

领域 helper 仍保留在 credential-handle 文件里。例如 `format_required_fields_json()` 仍定义 credential handle、approval correlation id、operator/reviewer identity handle、policy version、approval status、issued/expires/revocation/audit digest 等领域字段；`format_prohibited_fields_json()` 仍定义 credential value、raw endpoint URL、secret provider config、resolver client config、external request payload、ledger mutation 和 schema SQL 的拒绝码；`format_checks_json()` 仍是这份 receipt 的检查对象。这些 helper 作为已经编码好的 raw JSON value 进入 builder，不被二次转义。

这个边界很重要。如果把所有领域内容都塞进一个万能 `ReceiptSpec`，短期看似更“抽象”，长期会形成另一个巨型配置对象：一堆 optional 字段、布尔开关和版本分支，维护者反而更难判断某份 receipt 到底在证明什么。v1642 没走这条路。它只把本来就属于 JSON 组装的职责交给 builder，保留本 receipt 自己的领域数据、领域 helper 和 digest 规则。

## 5. 关键边界字段如何被控制面理解

这份 receipt 的核心边界仍然是：`read_only=true`，`execution_allowed=false`。这两个字段告诉 Node/Java/审查者：mini-kv 在这里只是证据提供者，不能执行真实审批、credential 解析或 managed audit 连接。

`credential_handle_approval_contract_non_participation_receipt_only=true` 表示本 receipt 只证明 mini-kv 对 credential-handle approval contract 的非参与状态。它不是“mini-kv 已经有 credential approval 模块”，更不是“mini-kv 负责审批状态”。`credential_handle_stored=false`、`credential_handle_validated=false`、`credential_handle_resolved=false`、`credential_handle_authoritative=false` 进一步关掉了这四种容易误解的方向。下游即使读到 credential-handle approval contract，也只能把它当成只读证据，不得把 mini-kv 变成 credential handle authority。

`credential_value_read=false`、`credential_value_loaded=false`、`credential_value_stored=false`、`credential_value_included=false`、`credential_value_rendered=false` 表明 credential value 完全不进入 mini-kv。`raw_endpoint_url_parsed=false`、`raw_endpoint_url_rendered=false` 表明 raw endpoint 也不进入 mini-kv。`external_request_sent=false`、`http_tcp_dial_allowed=false`、`connects_managed_audit=false` 表明这条链路不打开网络连接。`storage_write_allowed=false`、`write_commands_executed=false`、`approval_ledger_written=false`、`schema_migration_executed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false` 则分别关闭 Store/WAL 写入、ledger/schema 写入和 mini-kv 自身的危险命令执行。

这些字段迁移到 builder 后仍在同一顶层顺序中追加。测试不是只找几个字符串片段，而是把完整 public formatter 与 frozen fixture 子对象比较。只要字段顺序、compact whitespace、digest 或任何边界值漂移，`runtime_receipt_json_builder_tests` 就会失败。

## 6. 测试保护了什么

本版的第一道门是 `runtime_receipt_json_builder_tests`。它已经负责两类事情：一类是 builder 自身行为，例如字段顺序、字符串 escaping、raw object/array 不被二次转义；另一类是 fixture parity。对 credential-handle 来说，测试从 `fixtures/release/credential-resolver-credential-handle-approval-contract-non-participation-receipt.json` 提取 `credential_resolver_credential_handle_approval_contract_non_participation_receipt` 子对象，然后与当前 formatter 输出做逐字节比较。v1642 能通过，说明顶层 builder 迁移没有改变 frozen fixture byte surface。

第二道门是 `credential_handle_approval_contract_receipt_tests`。它保护独立 receipt 的语义断言，包括 read-only、execution false、credential value 不读取、raw endpoint 不解析、external request 不发送、approval ledger 不写、automatic upstream 不启动等字段。它还保护 receipt digest 和独立 fixture 暴露方式。

第三道门是 `smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests`。它们分别证明 SMOKEJSON 仍嵌入这份 receipt，runtime smoke evidence 没有漂移，release manifest 仍登记该 receipt 和相关测试。也就是说，这版不只是“单元测试函数输出没变”，而是 release 证据链里的多个入口都还看得到同一份 receipt。

第四道门是完整本地 CTest：344/344 通过。这证明变更没有影响 OSFS、Store、WAL、snapshot、RESP/TCP、shard readiness、archive inventory、其他 credential resolver receipts 和历史治理测试。最后，真实 CLI smoke 通过 `SMOKEJSON` 与 `CHECKJSON LOAD data/prod.snap` 证明运行时仍能读证据，`CHECKJSON` 只报告 LOAD 是写命令和 side effect，不执行 LOAD，不替换 store，也不触发 WAL。

## 7. 本版价值和下一步

v1642 的价值是把 Slice 2 从“只把最末尾 boundary flags 结构化”推进到“至少一份 approval contract 的整份 top-level formatter 已经结构化”。credential-handle 现在是一个完整样本：它没有 historical spelling waiver，fixture parity 是普通逐字节相等，顶层字段、嵌套对象、数组、summary、warnings、recommendations、evidence endpoints 和 boundary flags 都通过 ordered builder 输出。

下一步如果继续 receipts consolidation，最自然的是把 endpoint-handle allowlist approval contract 复制到同一种 top-level builder 模式，因为它也已有普通 byte parity，但它的 endpoint/raw-endpoint 字段需要逐项审查，不能简单复制 credential-handle 字段表。signed-human 可以更晚迁移，因为它仍带有 v1638 命名的单点 canonical spelling rule，迁移时要继续证明 waiver 没有扩大。

一句话总结：v1642 没让 mini-kv 多做任何业务，却让 mini-kv 给 Node/Java 看的 credential-handle approval 证据更容易维护、更不依赖手写字符串运气，并且仍保持 read-only、no router、no write、no WAL、no network、no credential、no execution 的松耦合集成边界。
