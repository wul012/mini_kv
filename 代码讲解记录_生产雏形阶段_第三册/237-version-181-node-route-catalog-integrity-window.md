# 237. mini-kv v181 Node route catalog integrity 窗口

## 本版目标

v181 冻结 v180 的 history formatter maintenance 证据为 `fixtures/release/shard-readiness-v180.json`，并把 `nodeRouteSplitCompatibilityWindow` 从 Node v433-v458 扩展到 Node v460。

它不是新的 runtime gate：Node v459/v460 是 route catalog / catalog integrity 的维护工作，mini-kv 只发布只读兼容证据，不启动 Java 或 mini-kv 服务，不安装 active router，不启用 write routing，不执行 LOAD/RESTORE/COMPACT。

## 主要变化

`src/shard_readiness_node_compatibility.cpp` 更新了 route/catalog window：

- `windowMode=node-v433-v460-route-catalog-integrity-window-read-only`
- `sourceNodePlan=docs/plans3/v460-post-audit-route-catalog-integrity-roadmap.md`
- `sourceFrozenReleaseVersion=v180`
- `sourceFrozenWindowDigest=fnv1a64:3403c490a3e623e0`
- covered versions 增加 `Node v459` 和 `Node v460`

审计字段自动从版本向量计算，`splitRouteGroupCount`、`expectedWindowVersionCount`、`observedWindowVersionCount` 和 `computedWindowVersionSpan` 都变为 28。

## 历史证据链

`src/shard_readiness_history.cpp` 把 fixture parity 追加到 `shard-readiness-v180.json`，historical fallback 的 previous consumed baseline 也前进到 v180。Node archived evidence 兼容清单追加 v459/v460，并显式保留：

- `nodeV459AuditRouteGroupCatalogPreserved=true`
- `nodeV460AuditRouteCatalogIntegrityPreserved=true`

这表示 mini-kv 知道 Node 最新维护窗口已经到 v460，但它仍只是读证据，不要求 Node/Java/mini-kv 任何服务联动。

## 测试保护

`tests/command_tests.cpp` 和 `tests/shard_readiness_tests.cpp` 更新当前 v181 断言：窗口模式、source plan、冻结 v180 digest、28 版本数量、v460 终点和 no-execution 边界都必须存在。`shard_readiness_tests` 还读取冻结的 v180 fixture，确认 v180 的 history formatter maintenance 语义继续可回放。

定向测试 2/2 覆盖 `SHARDJSON` 和 fixture parity；全量 71 条 CTest 确认其他功能未被窗口扩展扰动；TCP smoke 确认真实客户端路径也读到同一组 v181 只读证据。

一句话总结：v181 把 mini-kv 的 Node 维护窗口跟进到 v460，但把它牢牢限制在“版本化只读证据”，不让 catalog integrity 变成运行、路由或写入许可。
