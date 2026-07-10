# mini-kv v1643 代码讲解：endpoint-handle 顶层有序 JSON builder 收敛

## 1. 本版目标、角色定位与明确不做

v1643 是一次纯粹的内部结构优化。它处理的对象是 `src/runtime_credential_resolver_endpoint_handle_allowlist_approval_receipts.cpp` 中公开 formatter：`format_credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_json()`。这个函数负责把既有的 endpoint-handle allowlist approval 非参与回执格式化为 compact JSON，供 standalone fixture 对照、`SMOKEJSON`、runtime smoke evidence 和 release verification manifest 消费。本版不新增 endpoint 解析器，不创建网络 client，不读取凭据，不开放路由，不执行写命令，也不改变 mini-kv 对 Node/Java 合同链的角色。

更具体地说，这个回执仍然只是“控制面证据”。它告诉下游：mini-kv 看见过哪些合同字段、哪些检查通过、哪些危险能力明确没有发生。它不是 endpoint handle 存储库，不是 allowlist 审批引擎，不是 managed-audit connector，更不是 credential resolver。`read_only=true` 与 `execution_allowed=false` 仍是理解整份回执的第一原则。若有人将这份 JSON 当成执行许可，或者依据里面的 handle 名称去解析 raw URL、发 HTTP/TCP、连接外部审计系统，那已经越过了本文件和本项目的边界。

本版也没有把冻结 fixture 改成“适应新实现”的样子。fixture 是迁移前的观察基线，测试期望是裁判，不是装修材料。实现必须迁移到 builder 后继续生成同样字节；如果字段顺序、转义、空白、digest 或任何边界值变化，正确动作是修复实现或停止迁移，而不是修改 fixture 让测试变绿。

## 2. 为什么原实现值得深度优化

迁移前，formatter 从 `receipt_version` 一直拼到 `node_action`，主体接近二百行连续字符串加法。它同时嵌入 `source_node_v320_reference`、`endpoint_handle_allowlist_approval_contract`、`prerequisite_transition`、`necessity_proof`、`source_node_v319_reference`、`mini_kv_receipt`、两份 summary、warnings、recommendations、evidence endpoints 和 next actions。JSON 本身是结构化数据，但旧实现把结构压扁成“引号、逗号、花括号和字符串片段”的序列。

这种代码并非不能运行，真正的问题是维护时缺少局部边界。例如新增一个 summary 字段时，维护者必须同时确认前一个片段末尾是否已有逗号、当前花括号属于哪一层、字符串是否经过正确转义、后续 fixture 的字段顺序是否仍一致。更危险的是闭合能力字段：endpoint stored/validated/resolved、raw URL accepted/parsed/rendered、network request、credential value、Store/WAL write、schema migration、restore/load/compact、auto-start 等几十个布尔值挤在一条长 fragment 中，代码审查很难一眼确认某个值是否被误写为 true。

v1640 已把这组闭合字段转成 `std::vector<OrderedJsonField>`，但当时为了保持旧 formatter 的插入位置，仍需先 `json_object(fields)`，再剥去最外层 `{}`，最后把内部 fragment 拼回长字符串。这个桥接方式适合小步迁移，却不应成为长期结构。v1643 完成顶层迁移后，闭合字段可以直接追加到最终字段表，旧的 `format_closed_boundary_flags_json()` 自然失去存在理由。

## 3. 入口、数据结构与 builder 的职责

入口函数仍接收 `const std::vector<std::string>& read_commands`。这组只读命令参与 `read_command_list_digest` 和最终 receipt digest，签名、调用者和返回类型都没有变化。函数开头依次构造多个局部 JSON 值，例如：

- `endpoint_handle_allowlist_approval_contract`：合同名称、版本、用途、required/prohibited 字段、拒绝理由、no-go 边界和计数；
- `prerequisite_transition`：Node v320 前后的 prerequisite 状态，不把“合同已定义”误说成“审批已完成”；
- `necessity_proof`：为什么不能复用 v319、谁消费这份证据、遇到哪些条件必须停止；
- `source_node_v320_reference` 与 `source_node_v319_reference`：分别保存当前合同 intake 和前置 closure review 的只读引用；
- `mini_kv_receipt`、`summary`、`warnings`、`recommendations`、`evidence_endpoints` 与两个 actions 数组。

这些局部值通过 `json_object` 或 `json_array` 生成。字段值分成三类：字符串交给 `json_string` 做确定性转义，整数交给 `json_integer`，布尔值交给 `json_bool`；已经由领域 helper 生成的 `required_fields`、`checks` 等嵌套 JSON 则作为 raw JSON value 放入 `OrderedJsonField`。这里有一条重要分工：builder 只知道如何按给定顺序渲染合法 compact JSON，它不知道“endpoint approval 应当是什么”，不会自动补字段、排序字段、推导状态或修改 digest。业务语义继续由当前 receipts 文件拥有。

顶层最后创建 `std::vector<OrderedJsonField> fields`。字段顺序严格复制历史 public surface：版本与路径元数据在前，两个 Node 引用随后，接着是 mini-kv receipt、checks、summary、blockers/warnings/recommendations、evidence endpoints、下一步数组和 provenance/digest。之后调用 `append_closed_boundary_flag_fields(fields)`，把几十个闭合能力字段原位追加；最后追加 `boundary` 与 `node_action`。`json_object(std::span<const OrderedJsonField>{...})` 只遍历一次，统一处理逗号与字段名转义。

## 4. 为什么 endpoint-handle 不能复制 credential-handle 字段表

v1642 的 credential-handle formatter 提供了成熟模式，但 v1643 只复制“组装方法”，没有复制“字段内容”。两份回执面对的风险不同。credential-handle 重点证明 mini-kv 不读取、存储、解析、渲染或成为 credential authority；endpoint-handle 还必须证明 raw endpoint URL 没有被接受、落盘、允许解析、允许渲染、实际解析或实际渲染，也没有成为 endpoint handle、allowlist 或 approval status 的权威。

因此本文件保留自己的 `append_closed_boundary_flag_fields`。其中 `endpoint_handle_stored`、`endpoint_handle_validated`、`endpoint_handle_resolved`、`endpoint_handle_authoritative`、`endpoint_allowlist_authority`、`endpoint_allowlist_approval_status_authoritative` 都是 false；`raw_endpoint_url_accepted`、`raw_endpoint_url_stored`、`raw_endpoint_url_parse_allowed`、`raw_endpoint_url_render_allowed`、`raw_endpoint_url_parsed`、`raw_endpoint_url_rendered` 也全部为 false。随后才是 external request、HTTP/TCP dial、managed audit、Store/WAL write、approval ledger、schema、restore/load/compact、SETNXEX、auto-start、audit/order authority 等公共闭合边界。

这种局部拥有关系看似比抽出一个“万能 boundary 模板”多几行，却更适合安全审查。万能模板会把不同合同的语义差异藏在参数或条件分支中；本地有序字段表让评审者直接看见 endpoint 这一族到底关闭了什么。后续如果合同确实新增一个 endpoint 专属边界，修改位置和测试影响都清晰，不会误改 credential 或 signed-human family。

## 5. 字节一致性、digest 与 canonical surface

v1643 的核心验收不是“JSON 解析后语义相等”，而是 endpoint-handle formatter 与冻结 fixture 子对象逐字节相等。原因是公共证据链中字段顺序、compact whitespace 和字符串转义都可能被下游 digest 或快照消费。`runtime_receipt_json_builder_tests` 通过 `extract_json_object_field` 从 release fixture 中提取目标对象，再直接与当前 formatter 返回值比较；endpoint-handle 没有 signed-human 那种已命名单点 apostrophe 兼容规则，所以这里不允许任何 normalization。

最终 receipt digest 的输入函数完全未改。`binary_provenance_digest`、retention check/replay digest、`read_command_list_digest` 和 receipt digest 仍在历史位置；builder 只渲染这些已计算值，不参与计算。focused parity 通过意味着：局部对象拆分没有改变字符串转义，字段表没有漏项或重排，数值与布尔字面量一致，digest 输出也一致。若只做“解析后对象相等”，重复字段或顺序漂移可能被掩盖；逐字节 oracle 把这类风险留在机械门内。

本次物理行数从 503 行上升到 558 行。这不是回归。旧代码把大量结构压缩进横向字符串，行数较少但审查成本高；新代码将每个字段变为独立的结构元素。项目在 v1628 已修正过“行数增长即停止”的错误指标：真正的 stop condition 是 byte parity、digest、字段计数、边界或测试表面漂移。行数只用于解释维护成本变化，不能替代这些硬证据。

## 6. 测试矩阵分别保护什么

`runtime_receipt_json_builder_tests` 是最直接的迁移裁判。它保护有序 builder 自身的 object/array/escaping 行为，也把 endpoint-handle public formatter 与 frozen fixture 子对象逐字节比较。它失败通常意味着字段顺序、转义、值或 compact 形态发生了变化。

`endpoint_handle_allowlist_approval_contract_receipt_tests` 保护领域合同：required/prohibited 字段、来源版本、检查计数、warnings/recommendations、receipt digest 和 endpoint/raw-endpoint/no-network/no-write/no-execution 边界必须仍在。它比纯 parity 更能说明“这些字节为什么重要”。

`smokejson_command_receipt_tests` 保护命令聚合表面，证明 `SMOKEJSON` 仍嵌入同一份 receipt，没有因为内部函数重构而漏出证据。`runtime_smoke_evidence_tests` 保护运行时证据聚合，`release_verification_manifest_tests` 保护发布清单路径和 digest 引用。五条 focused lane 同时为绿，才说明 standalone formatter、命令输出、runtime evidence 与 release manifest 四个消费面没有分叉。

最终还要完整 build 和全量 CTest。focused tests 无法覆盖 command、WAL、snapshot、RESP/TCP、OSFS、shard readiness 与数百条历史证据约束；全量套件证明这次内部结构调整没有通过链接、公共 helper 或输出聚合意外影响其他模块。真实 CLI smoke 再执行 `SMOKEJSON` 与 `CHECKJSON LOAD data/prod.snap`：前者证明真实二进制可输出证据，后者证明 LOAD 仍只被识别为 write/side-effect 并拒绝执行，没有借“证据检查”触发 Store/WAL 修改。

## 7. 需求到证据的闭环

| 本版要求 | 实现位置 | 机械证据 | 状态 |
| --- | --- | --- | --- |
| 消除 endpoint 顶层长字符串拼接 | endpoint receipt formatter 的局部 object/array 与顶层 ordered fields | 编译、format dry-run、focused parity | 完整 |
| 保持 public JSON 字节 | 不改 fixture，保持字段表历史顺序 | builder test 对 fixture 子对象逐字节相等 | 完整 |
| 保持 digest | digest 函数与 parts 不变，顶层仅渲染结果 | receipt test、runtime smoke、release manifest | 完整 |
| 保持 endpoint 专属闭合边界 | 本地 `append_closed_boundary_flag_fields` | endpoint receipt test + builder parity | 完整 |
| 不扩大命令/网络/写入/执行能力 | 无新增入口，无 connector/client，无 Store/WAL 调用 | focused surfaces、CLI CHECKJSON、full CTest | 完整 |
| 清除临时迁移桥 | 删除 `format_closed_boundary_flags_json` | 编译和源码审查 | 完整 |

## 8. 对抗性自审与后续价值

最强的质疑是：代码行数增加、局部对象变多，是否只是把字符串拼接“改写得更漂亮”，并没有真正降低风险？回应不能只靠主观可读性。首先，旧的逗号与花括号责任被收敛到一个经过独立测试的 builder；其次，每个嵌套对象有明确名字和边界，字段 review 可以按模块进行；再次，闭合能力字段进入类型化、有序列表，不再需要生成后剥壳再拼回；最后，冻结 fixture 的逐字节 oracle 证明这种结构改善没有拿公共兼容性做交换。收益是可复现的维护风险下降，而不是格式偏好。

本版仍不能宣称 receipts consolidation 完成。28 个 runtime receipt 文件中，当前只有 5 个接入 builder，且 signed-human 尚未完成整份顶层迁移。v1643 的价值在于把第二份无 canonical waiver 的审批合同完整跑通，为下一版处理 signed-human 的命名单点差异提供清晰参照，同时证明 endpoint 专属字段可以在不复制 credential 模板、不扩大任何权限的前提下结构化。

一句话总结：v1643 把 endpoint-handle 回执从难以局部审查的长字符串变成确定性、有序、逐字段可核验的证据结构，同时用冻结 fixture、digest、五条消费表面和 no router/no write/no WAL/no network/no credential/no execution 边界证明松耦合 Node/Java/mini-kv 集成看到的仍是同一份只读事实。
