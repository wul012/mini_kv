# mini-kv v1639 credential-handle 闭合边界 flag builder 迁移讲解

## 1. 本版目标、角色与“不是什么”

v1639 的目标是把 credential-handle approval contract receipt 中最密集、最容易漂移的一段闭合边界布尔字段，从手写 JSON 字符串迁移到内部 ordered JSON builder。上一版 v1638 已经给审批合同 Slice 2 建立了迁移前基线：credential-handle 与 endpoint-handle 两份 formatter 与 frozen fixture 子对象逐字节一致；signed-human 有一个已命名的 apostrophe 历史拼写 waiver。这个前提很关键，因为 v1639 可以放心选择 credential-handle 作为第一刀，只要迁移后新增的 parity 测试继续通过，就能知道输出表面没有变化。

本版的角色是“局部机制迁移”，不是业务扩展。它不新增命令，不改 `SMOKEJSON` 字段含义，不改 fixture，不改 digest parts，不启动服务，不连接 Node/Java，不读取 credential value，不解析 raw endpoint URL，不发 HTTP/TCP，不写 Store/WAL，不写 approval ledger，不执行 schema migration，也不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`、rollback 或 deployment。

它也不是“credential-handle formatter 已经完全 builder-owned”。这句话必须说清楚，否则后续维护者会误判进度。v1639 只迁移 `format_closed_boundary_flags_json` 这段闭合边界 flag block。Node v317 reference、Node v316 reference、contract section、checks、summary、warnings、recommendations、evidence endpoints、next actions 等领域 section 仍保留原来的局部拼装方式。这个粒度不是偷懒，而是为了把风险最大的布尔尾部先从手写逗号串里拿出来，用 v1638 的 byte parity oracle 证明它不漂，再在下一版扩展到 endpoint-handle 或 top-level/nested section。

## 2. 为什么先迁移闭合边界 flag block

credential-handle approval receipt 的尾部有一大串控制面最关心的边界字段：`read_only`、`execution_allowed`、`credential_handle_approval_contract_non_participation_receipt_only`、`credential_value_read`、`raw_endpoint_url_parsed`、`external_request_sent`、`approval_ledger_written`、`schema_migration_executed`、`load_restore_compact_executed`、`setnxex_execution_allowed`、`automatic_upstream_start`、`audit_authoritative`、`order_authoritative` 等。它们的共同特点是语义机械，但数量很多，而且绝大多数是 `false`。

手写 JSON 字符串在这种区域有两个维护风险。第一是逗号和顺序风险：新增、删除或调整一行，很容易把前后字段的逗号放错，或者把字段顺序悄悄换掉。第二是语义漏项风险：这些字段不是普通展示项，而是 mini-kv 对 Node/Java/control-plane 的权限否定承诺；少一个 `false`，下游读者就可能误以为某类行为没有被明确关闭。v1625 到 v1638 一直强调 line-count shrink 不是硬指标，真正要降低的是这种“输出表面和权限边界被无声改写”的风险。

ordered JSON builder 正适合处理这类字段。它不理解 credential-handle 的业务语义，也不自动决定哪个字段应该 true 或 false；它只接收已经排好序的 `OrderedJsonField` 列表，并按输入顺序输出 compact JSON。也就是说，领域语义仍然留在本文件里，builder 只承担拼装机制。这个边界让重构不会变成一个万能 schema，也不会把 credential-handle、endpoint-handle、signed-human 的差异硬压成一套大参数构造器。

## 3. 代码入口与职责变化

本版修改的生产文件是 `src/runtime_credential_resolver_credential_handle_approval_receipts.cpp`。文件开头新增内部 builder 头文件：

```cpp
#include "runtime_receipt_json_builder.hpp"
```

并引入三个局部 using：`json_bool`、`json_object`、`OrderedJsonField`。这里没有引入第三方 JSON 库，也没有把 builder 暴露到 public include。`runtime_receipt_json_builder` 仍然是 mini-kv 内部 formatter 工具。

核心新增函数是：

```cpp
void append_closed_boundary_flag_fields(std::vector<OrderedJsonField>& fields)
```

它按历史输出顺序逐个 `push_back` 字段，例如先输出 `read_only=true`，再输出 `execution_allowed=false`，再输出 receipt-only/intake-only/source-consumption 字段，然后进入 runtime shell、resolver/client、credential handle、credential value、raw endpoint、external request、managed audit、storage/write、approval ledger、schema migration、restore/load/compact/setnxex、auto-start、authority 等几组边界字段。字段名和布尔值仍然在本文件里肉眼可见，不被藏进循环、map 或自动推导逻辑里。

原来的 `format_closed_boundary_flags_json()` 没有删除 public 调用形态，而是改成：

```cpp
std::vector<OrderedJsonField> fields;
append_closed_boundary_flag_fields(fields);
auto object_json = json_object(fields);
return object_json.substr(1, object_json.size() - 2);
```

为什么要剥掉首尾花括号？因为历史 formatter 在 top-level object 的中段用 `"," + format_closed_boundary_flags_json() + ",\"boundary\":...` 的方式嵌入这串字段。为了保持 byte parity，v1639 不移动这段插入点，也不顺手把整个 top-level object 重写成 builder。builder 先生成一个完整 compact object，然后只取内部字段串，等价替代原来的手写 fragment。这个做法看起来保守，但它让输出位置、字段顺序、逗号布局都受 v1638 新增的 byte parity oracle 约束。

## 4. 关键字段如何被控制面理解

这些字段不是为了“看起来很生产化”，而是给控制面读的否定边界。`read_only=true` 表示 receipt 只是证据；`execution_allowed=false` 表示不能把 receipt 当作执行许可；`credential_handle_approval_contract_non_participation_receipt_only=true` 表示 mini-kv 只是非参与回执；`credential_handle_stored=false`、`credential_handle_validated=false`、`credential_handle_resolved=false`、`credential_handle_authoritative=false` 表示 mini-kv 不存储、不验证、不解析、不成为 credential handle 权威；`credential_value_read=false` 表示没有秘密值被读取；`raw_endpoint_url_parsed=false` 表示没有解析真实 endpoint；`external_request_sent=false` 和 `http_tcp_dial_allowed=false` 表示没有外部请求；`approval_ledger_written=false` 与 `schema_migration_executed=false` 表示没有写审批账本或执行迁移。

后面的 `restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false` 继续维持 mini-kv 的执行边界：receipt 可以描述危险命令为什么不能被控制面执行，但它本身不触发这些命令。`node_auto_start_allowed=false`、`java_auto_start_allowed=false`、`mini_kv_auto_start_allowed=false`、`automatic_upstream_start=false` 则明确 mini-kv 不替 sibling 项目启动任何服务。最后 `audit_authoritative=false`、`order_authoritative=false` 维持四项目关系里最核心的边界：mini-kv 是存储/恢复/证据学习系统，不是订单权威，也不是审计审批权威。

把这些字段迁移到 ordered builder 的价值在于，字段顺序现在由一行一行的 `push_back` 顺序决定，而不是由一个超长字符串的逗号连接决定。后续如果需要审查“mini-kv 到底关闭了哪些权限”，读者可以在一个连续函数里看到完整列表，也可以通过测试证明渲染后的 JSON 与历史 fixture 完全一致。

## 5. 测试如何证明“机制变了，输出没变”

v1639 依赖 v1638 新增的 `runtime_receipt_json_builder_tests`。这个测试会读取 `credential-resolver-credential-handle-approval-contract-non-participation-receipt.json`，提取内部字段 `credential_resolver_credential_handle_approval_contract_non_participation_receipt`，然后与当前 `format_credential_resolver_credential_handle_approval_contract_non_participation_receipt_json(read_commands)` 的输出逐字节比较。由于 credential-handle 在 v1638 已经证明没有历史 apostrophe waiver，所以这里必须是严格 byte parity；任何字段顺序、布尔值、digest、compact whitespace 或边界片段变化都会失败。

focused gate 还跑了 `credential_handle_approval_contract_receipt_tests`，它保护独立 fixture、SMOKEJSON 运行路径、manifest 嵌入、digest 和语义断言；`smokejson_command_receipt_tests` 证明命令层嵌入仍然存在；`runtime_smoke_evidence_tests` 证明 runtime evidence 没有漂；`release_verification_manifest_tests` 证明 release manifest 的索引和 digest 仍然对齐。换句话说，本版不是只让一个 helper 单测通过，而是从 standalone fixture、formatter、command output、runtime smoke、release manifest 五个角度证明同一件事：闭合边界 flag 的拼装机制换了，但发布表面没变。

全量 CTest 还会覆盖 OSFS、WAL、Snapshot、RESP/TCP、metrics、recovery、archive/governance receipt 链条，确保这次局部重构没有影响主 KV 路径、课程设计层或历史 evidence fixtures。这一点尤其重要，因为 receipt 文件很多，最怕一个局部 builder 改动被链接到 `SMOKEJSON` 后，意外影响其他 runtime evidence。

## 6. 为什么不一次性重写 top-level formatter

credential-handle 这份 formatter 接近 30KB，里面不仅有 top-level fields，还有两个 source Node reference、contract object、prerequisite transition、necessity proof、summary、checks、warnings、recommendations、evidence endpoints、next actions 和大量字符串说明。一次性全改成 builder 虽然看起来“完成度高”，但失败时会很难定位：到底是某个 nested section 改错了，还是闭合边界 field 变了，还是 top-level 逗号变了？

v1639 选择先迁移最机械、最重复、最可验证的闭合边界 block，是顶层工程节奏上的保守，而不是工作量不足。它把一块风险高但语义稳定的区域单独拿出来，让测试证明 builder 的字段顺序和 compact 输出足够可靠。下一版可以复制这个方法到 endpoint-handle，因为 endpoint-handle 在 v1638 也已经有严格 byte parity；也可以继续拆 credential-handle 的 top-level/nested sections，但每次都应该保持一个清楚的验收面。

一句话总结：v1639 把 credential-handle receipt 中最容易因手写字符串而出错的权限否定字段群迁到 ordered builder，同时用 fixture byte parity 证明 mini-kv 仍然只是只读 evidence provider，不获得 router、write、WAL、network、credential 或 execution 权限。
