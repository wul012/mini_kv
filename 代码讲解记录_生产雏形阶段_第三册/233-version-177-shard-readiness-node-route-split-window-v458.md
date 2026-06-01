# 233. mini-kv v177：Node v458 foundational audit route split window 只读延伸

## 本版目标

v177 的目标是把 mini-kv 的 `nodeRouteSplitCompatibilityWindow` 从 Node v457 延伸到 Node v458，并把 v176 的当前证据冻结为 `fixtures/release/shard-readiness-v176.json`。Node v458 计划只抽离 foundational audit 的 6 条 JSON/Markdown route 注册，不改变 API path、response shape，也不要求 Java 或 mini-kv 启动服务。

这版不是什么：不是 active shard router，不是 write routing，不是 runtime execution，不是 LOAD/RESTORE/COMPACT 入口，也不是让 mini-kv 参与 Node 的 route table 执行。mini-kv 只发布可供 Node/Java 控制面读取的只读证据。

## 版本化 fixture

`fixtures/release/shard-readiness-v176.json` 是 v176 的冻结基线，保留了 Node v433-v457 的 manual sandbox connection fake transport route split 兼容窗口。v177 的 rolling fixture 仍写在 `fixtures/release/shard-readiness.json`，但 `fixtureParity.historicalFixturePaths` 增加了 v176 路径，表示历史判断必须读取版本化 fixture，不能拿 rolling current 冒充旧版本。

当前 v177 的 `evidenceDigest` 是 `fnv1a64:e773708a4decc60e`。窗口审计中的 `sourceFrozenWindowDigest` 指向 v176 的 `fnv1a64:528073c4315ef89b`，这是“新窗口基于已冻结旧窗口”的证据链。

## Node compatibility formatter

`src/shard_readiness_node_compatibility.cpp` 集中维护 route split window 的元数据：

- `windowMode` 改为 `node-v433-v458-route-split-window-contract-stable-read-only`
- `sourceNodePlan` 指向 `docs/plans3/v458-post-foundational-audit-route-group-split-roadmap.md`
- `sourceFrozenReleaseVersion` 改为 `v176`
- `sourceFrozenFixturePath` 改为 `fixtures/release/shard-readiness-v176.json`
- `windowEndNodeVersion` 改为 `Node v458`
- `coveredNodeVersions` 末尾追加 `Node v458`

`format_route_split_compatibility_window_json()` 和 `format_route_split_compatibility_window_audit_json()` 都复用同一组版本列表，所以 `splitRouteGroupCount`、`expectedWindowVersionCount`、`observedWindowVersionCount` 同步变为 26，避免窗口正文和审计字段漂移。

## SHARDJSON 主证据

`src/shard_readiness.cpp` 把当前 release 推进到 `v177`，状态改为 `node-route-split-window-foundational-audit-read-only`。控制面可以从这些字段理解本版边界：

- `diagnostics.nodeConsumer`：Node v458+ 只能把 v177 当作 foundational audit route split compatibility evidence 读取。
- `archiveCompatibility.archivedNodeVersions`：归档覆盖扩展到 Node v458，但 `changesArchivedNodeEvidence=false`。
- `historicalFallback.previousConsumedReleaseVersion`：旧消费基线切换到 v176 frozen fixture。
- `nodeV458FoundationalAuditRouteSplitPreserved=true`：只记录 v458 route-registration-only refactor 已被只读窗口覆盖。

这些字段都没有打开 `routerActivationAllowed`、`writeRoutingAllowed`、`writeCommandsAllowed`、`adminCommandsAllowed`、`loadRestoreCompactAllowed` 或 `executionAllowed`。

## Boundary catalog

`src/shard_readiness_boundary_fields.cpp` 的 `boundaryCatalogIndex` 仍保持 `read-only-boundary-fields.v9`、420 fields、22 groups。v177 只把索引的 source plan 和 frozen fixture 切到 v458/v176，说明 catalog 继续把 `nodeRouteSplitCompatibilityWindow` 和 `nodeRouteSplitCompatibilityWindowAudit` 纳入只读边界字段集合，没有扩展 schema。

## 测试与 smoke

`tests/command_tests.cpp` 校验命令层 `SHARDJSON` 响应包含 v177、Node v458、26 个窗口版本、v176 frozen source、`sourceFrozenWindowDigest=fnv1a64:528073c4315ef89b`，并继续断言 no service/no router/no write/no execution。

`tests/shard_readiness_tests.cpp` 做更完整的 fixture 链校验：当前 rolling fixture 必须等于 formatter 输出，必须不同于 v176 frozen fixture，并且 v176 fixture 仍保留自己的 v457 窗口、25 个版本和 digest。这样后续 Node 消费时能区分“历史冻结证据”和“当前滚动证据”。

本版验证：

- 定向 CTest：`command_tests`、`shard_readiness_tests`，2/2 通过。
- 全量 CTest：71/71 通过。
- TCP smoke：临时端口 6450 读取 `SHARDJSON`，确认 v177、Node v458、`readOnly=true`、`executionAllowed=false`、`fnv1a64:e773708a4decc60e`，随后停止服务端并确认端口释放。

一句话总结：v177 把 Node v458 的 foundational audit route split 纳入 mini-kv 的版本化只读证据链，让 Node 可以继续消费兼容窗口，同时 mini-kv 仍不启动服务、不激活路由、不写入、不执行。
