# v1640 endpoint-handle 边界 flag builder 迁移讲解

## 1. 本版目标、角色定位，以及它不是什么

v1640 的目标很窄，但不是小修小补：把 `runtime_credential_resolver_endpoint_handle_allowlist_approval_receipts.cpp` 里最密集、最容易出错的闭合边界布尔字段群，从一段手写 JSON 字符串迁移到内部 ordered JSON builder。这个 receipt 对应的是 Node v320 endpoint-handle allowlist approval contract intake 的 mini-kv 非参与证明，给 Node v321 的上游回显验证消费。它表达的核心含义是：mini-kv 只读回显一个“非 raw URL 的 endpoint handle allowlist approval 合同”，不会存 raw endpoint URL，不会验证 endpoint authority，不会成为 endpoint handle 或 allowlist approval status 的权威，也不会读取 credential、发网络请求、写 WAL/Store/ledger/schema、执行 LOAD/COMPACT/RESTORE/SETNXEX 或自动启动任何上游。

这版不是新的执行入口，不是 endpoint router，不是 managed audit client，不是 raw endpoint URL parser，也不是 approval ledger writer。它没有新增命令，没有改 `SMOKEJSON`、`CHECKJSON`、`verification-manifest` 的字段含义，没有改 fixture 字节，也没有扩大 v1638 给 signed-human approval artifact 命名的单点 apostrophe 兼容规则。它做的是生产维护里很有价值的一种“低语义、高风险拼装层”收敛：把本来靠人眼维护的超长 `"field":false,` 字符串，变成逐字段可审查的 `OrderedJsonField` 清单，同时让 shared builder 负责逗号、true/false 和 compact JSON 结构。

## 2. 入口命令与模块位置

本版入口仍然是既有 public formatter：

```cpp
format_credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_json(...)
```

它声明在 `minikv/runtime_evidence_receipts.hpp` 暴露的 runtime evidence receipt 表面里，由 SMOKEJSON、runtime smoke evidence、release manifest 和对应 receipt 测试间接消费。调用路径没有变化：上层仍然向 formatter 传入 read command list，formatter 仍然组装同一份 compact JSON，digest helper 仍然使用原来的 `credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_digest(...)`。本版真正动到的是文件内部的局部 helper：

```cpp
format_closed_boundary_flags_json()
```

旧版本里它返回一段没有外层花括号的 JSON fragment，例如 `"read_only":true,"execution_allowed":false,...`。这个设计来自历史 formatter：大对象主体由很多字符串片段拼接而成，闭合边界字段群需要嵌在 summary/digest/boundary 之前的同一层。因此 v1640 没有强行改变调用方形状，而是新增：

```cpp
append_closed_boundary_flag_fields(std::vector<OrderedJsonField>& fields)
```

然后在 `format_closed_boundary_flags_json()` 内部调用 `json_object(fields)` 生成完整 object，再通过 `substr(1, object_json.size() - 2)` 去掉外层 `{}`，回到历史 inline fragment。这个小动作保证上层 formatter 的插入位置、逗号关系、字段顺序和 compact whitespace 都保持不变。

## 3. 响应结构与证据字段怎样读

endpoint-handle receipt 的 JSON 结构可以分成几层。第一层是 receipt 自身的身份信息：`receipt_version`、`receipt_fixture_path`、`source_contract_intake`、`source_closure_review`、`source_prerequisite_catalog`、`consumer_hint`、`current_project_version`、`current_release_version` 和 `runtime_role`。这些字段告诉控制面：这份证据来自 mini-kv，它只作为 runtime evidence provider，不替 Node 或 Java 做权威判断。

第二层是 Node v320 与 Node v319 的引用。Node v320 定义 endpoint-handle allowlist approval contract intake，Node v319 是 credential-handle prerequisite closure review，Node v313 是更早的 prerequisite catalog。mini-kv 在这里不是向 Node 发请求，而是把已经冻结/已知的上游合同形状写入本地 receipt，让 Node 后续能验证“三方都对同一个边界说了同样的话”。

第三层是 contract 本身：required fields、prohibited fields、rejection reasons、no-go boundaries、upstream echo requests、completed/remaining prerequisites、necessity proof、checks、summary、warnings、recommendations、evidence endpoints、digest fields。v1640 没有迁移这些领域 section，因为它们包含大量业务含义和嵌套数组。一次性搬完整个 top-level object 会把“机械边界字段收敛”和“领域结构重组”混在一起，不利于定位漂移。

第四层就是本版迁移的闭合边界 flags。它们是控制面最关心的硬边界：`read_only=true` 表示此 receipt 只能读证据；`execution_allowed=false` 表示不能执行任何真实动作；`endpoint_handle_stored=false`、`endpoint_handle_validated=false`、`endpoint_handle_authoritative=false` 表示 mini-kv 不接管 endpoint handle；`endpoint_allowlist_authority=false` 和 `endpoint_allowlist_approval_status_authoritative=false` 表示 allowlist 与 approval status 也不是 mini-kv 的权威；`credential_value_read=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`schema_migration_executed=false`、`load_restore_compact_executed=false`、`automatic_upstream_start=false` 则把 credential、raw endpoint、网络、ledger/schema、危险命令和服务启动全部关住。v1640 把这一层做成 ordered field list，是为了让这些“危险能力仍然关闭”的事实更可维护。

## 4. 核心迁移流

旧实现是一条长字符串。它的问题不是运行错误，而是维护风险：字段很多，语义相似，true/false 密集，逗号靠手写，任意一次后续维护都可能误删字段、调换顺序，或者把 `false` 写成 `true`。对 runtime receipt 来说，这类错误比普通 UI 文案更危险，因为 Node/Java 侧可能把这些字段当作边界证明消费。

新实现把字段清单拆成了清晰的追加步骤。`append_closed_boundary_flag_fields(...)` 只负责把历史字段按原顺序加入 `std::vector<OrderedJsonField>`，每一项都是字段名加 `json_bool(true/false)`。它不读取 Node 文件，不解析 fixture，不计算 digest，不碰 store，不碰 WAL，不发网络，也不知道 endpoint-handle 的业务规则如何审批。它只是把已经存在的字段和值结构化地表达出来。

然后 `format_closed_boundary_flags_json()` 做三件事：创建 vector，调用 append helper，调用 `json_object(fields)`。`json_object` 是 v1626 起建立的内部 builder，它的职责是保持输入顺序、输出 compact JSON、正确放置逗号。最后去掉外层花括号，是为了保持旧 formatter 的嵌入契约。这样迁移后的 public bytes 应该和迁移前完全一样，而不需要修改任何 fixture 或测试 expectation。

这里最重要的工程判断是“不扩大迁移面”。endpoint-handle 与 credential-handle 很像，v1639 已证明同类模式可行；但 endpoint-handle 多了 raw endpoint 和 allowlist authority 字段，所以仍然需要逐项抄对历史字段，而不是做一个“credential/endpoint 通用模板”抽象。现在抽象还太早：字段差异没有足够多版本证明，贸然抽通用函数可能把两个合同的不同边界揉平。v1640 选择复制结构而不是合并语义，是更稳的工程路线。

## 5. 测试保护了什么

`runtime_receipt_json_builder_tests` 是迁移前后的第一道门。v1638 已经为 signed-human、credential-handle、endpoint-handle 三份审批合同建立了 fixture/formatter parity baseline。对 endpoint-handle 来说，它要求 public formatter 输出与 frozen fixture 子对象 byte-for-byte 相等，并且继续包含 `read_only=true`、`execution_allowed=false`、`credential_value_read=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`automatic_upstream_start=false` 等关键边界。v1640 迁移后这条测试仍然通过，说明字段顺序、compact whitespace、值和结构没有漂移。

`endpoint_handle_allowlist_approval_contract_receipt_tests` 是该 receipt 的专项合同测试。它保护 standalone fixture、digest、Node v320/v319 引用、required/prohibited counts、no-go boundaries、warning/recommendation 和 endpoint/raw-url 相关禁止字段。它通过后，说明本版没有把 endpoint approval 证据改成别的合同。

`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 与 `release_verification_manifest_tests` 保护外层暴露面：SMOKEJSON 能看到同一份 receipt，runtime smoke evidence 能嵌入同一份证据，release manifest 能继续列出同一份 fixture/receipt。它们共同证明本版没有只让单元测试绿色，而是让实际运行时证据路径也保持稳定。

最后是 full local CTest：344/344。它覆盖 OSFS、store、command、WAL、snapshot、TCP/RESP、readiness、archive、retention、receipt、orientation 和一大批 governance evidence。v1640 的改动虽然只在一个 receipt 文件，但 full suite 仍然必要，因为 receipt 源文件被链接进核心库，任何编译、链接、manifest 或 smoke 嵌入问题都可能从别处暴露。

## 6. 对后续 Node/Java/mini-kv 松耦合融合的价值

v1640 的直接价值是让 endpoint-handle approval receipt 的危险边界字段更可维护；更大的价值是把 mini-kv 的证据链继续保持在“可读、可验、不可误执行”的轨道上。Node 可以继续把 mini-kv 当成只读证据提供者，而不是 endpoint authority；Java 可以只读回显同一合同，而不需要担心 mini-kv 偷偷打开 raw endpoint、credential 或 network path。换句话说，本版不是让系统更会“做事”，而是让系统更清楚地证明哪些事它绝不会做。
