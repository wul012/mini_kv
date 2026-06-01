# 239. mini-kv v183 Node route quality catalog integrity 窗口

## 本版目标

v183 冻结 v182 的 route catalog test migration window，并跟进 Node v463 route quality catalog integrity。Node v463 把 route quality report 从旧 v240 route-table snapshot 升级到 v459-v462 catalog semantics；mini-kv 只同步只读证据窗口。

## 代码变化

`src/shard_readiness_node_compatibility.cpp` 把 window 改为 `node-v433-v463-route-quality-catalog-integrity-window-read-only`，source plan 指向 Node v463，frozen source 指向 `fixtures/release/shard-readiness-v182.json`，source digest 为 `fnv1a64:e4a40fd2e1d1d741`。covered versions 增加 `Node v463`，因此窗口数量和计算跨度都是 31。

`src/shard_readiness_history.cpp` 把 fixture parity 追加 v182，并记录 `nodeV463AuditRouteQualityCatalogIntegrityPreserved=true`。这表示 mini-kv 知道 Node route-quality 报告已经接到新 catalog 语义，但它仍不参与 Node 测试执行或 route-quality 计算。

## 边界解释

控制面读取 v183 时，只能把它当作“mini-kv 证据窗口已知道 Node v463 维护语义”。它不意味着 active shard router、live read、service start、write routing、write/admin command 或 LOAD/RESTORE/COMPACT 已被批准。

## 验证

定向测试覆盖 `SHARDJSON` 输出和 frozen fixture 回放；全量 71 条 CTest 覆盖整个 C++ 项目的行为不回退；TCP smoke 证明服务端路径返回同一组只读字段，验证后端口释放。

一句话总结：v183 让 mini-kv 的 Node route/catalog 窗口跟进到 v463 route quality 语义，同时保持证据链只读、不可执行。
