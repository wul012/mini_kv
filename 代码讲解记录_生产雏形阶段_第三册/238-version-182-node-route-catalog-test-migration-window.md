# 238. mini-kv v182 Node route catalog test migration 窗口

## 本版目标

v182 冻结 v181 的 route catalog integrity window，继续把 Node 的 route/catalog 维护窗口推进到 v462。Node v462 是 audit route catalog test migration：把大量 route-group 测试从源码字符串检查迁移到共享 catalog helper。mini-kv 只做证据跟进。

它不是什么：不是 Node 新 route，不是 Java/mini-kv 启动信号，不是 active router，不是写路由，也不是 runtime execution approval。

## 代码路径

`src/shard_readiness_node_compatibility.cpp` 把窗口改为：

- `node-v433-v462-route-catalog-test-migration-window-read-only`
- source plan 指向 `docs/plans3/v462-post-audit-route-catalog-test-migration-roadmap.md`
- frozen source 改为 `fixtures/release/shard-readiness-v181.json`
- frozen digest 改为 `fnv1a64:58e9b51084ad91e5`
- covered versions 增加 `Node v461` 和 `Node v462`

由于 audit 字段从 shared vector 计算，版本数量、窗口跨度和 range match 都自动变成 30。

`src/shard_readiness_history.cpp` 同步追加 v181 frozen fixture，并记录 `nodeV461AuditRouteCatalogTestSupportPreserved` 与 `nodeV462AuditRouteCatalogTestMigrationPreserved`，说明新 Node 维护版本仍只是归档兼容语义。

## 验证意义

测试覆盖三层：

- 命令层：`SHARDJSON` 输出 v182、v462 window、30 count/span、source digest 和 no-execution 字段。
- fixture 层：当前 fixture 不等于历史 frozen fixture，并能回放 v181 的 frozen route catalog integrity 证据。
- 真实 TCP 层：客户端从服务端拿到同样的 v182 只读证据，smoke 后端口释放。

一句话总结：v182 让 mini-kv 跟上 Node v462 的测试迁移窗口，但仍把它限制为可审计、可冻结、不可执行的证据链。
