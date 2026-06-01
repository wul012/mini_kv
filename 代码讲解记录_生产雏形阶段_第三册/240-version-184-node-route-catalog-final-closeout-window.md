# 240. mini-kv v184 Node route catalog final closeout 窗口

## 本版目标

v184 冻结 v183 的 route quality catalog integrity window，并跟进 Node v464 route catalog final closeout。Node v464 完成 route catalog 批次的最终验证和 import-cycle 修复；mini-kv 只把这个事实纳入版本化只读证据。

## 代码变化

`src/shard_readiness_node_compatibility.cpp` 把 window 改为 `node-v433-v464-route-catalog-final-closeout-window-read-only`，source plan 指向 Node v464，frozen source 指向 `fixtures/release/shard-readiness-v183.json`，source digest 为 `fnv1a64:8b252a90cb150a99`。covered versions 增加 `Node v464`，因此窗口数量和计算跨度都是 32。

`src/shard_readiness_history.cpp` 追加 v183 frozen fixture，并记录 `nodeV464AuditRouteCatalogFinalCloseoutPreserved=true`。这保证 v464 的 closeout 只被看作历史/兼容证据，不变成运行许可。

## 验证

定向测试覆盖当前 `SHARDJSON` 与 frozen v183 回放；全量 71 条 CTest 证明既有行为不回退；TCP smoke 证明真实服务端路径也返回同样的 v184 只读证据，验证后端口释放。

一句话总结：v184 把本轮 Node route catalog 批次收口到 v464，同时保持 mini-kv 只读、无路由、无写入、无执行。
