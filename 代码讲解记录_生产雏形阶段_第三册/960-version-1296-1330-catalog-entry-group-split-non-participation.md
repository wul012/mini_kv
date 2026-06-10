# v1296-v1330 catalog entry group split non-participation

本版把 v1295 的 release evidence review non-participation 冻结成来源证据，再把 Node v1722-v1756 的 controlled read-only shard preview type module catalog entry group split 计划，版本化成 mini-kv 的只读证据链。

这不是新的执行入口，不是 catalog 运行时，不是 route/router 实现，也不是把 Node 的 catalog 资料搬进 mini-kv。它只做一件事：证明 mini-kv 能把这段跨项目计划当成只读、版本化、可回溯的证据消费掉，同时继续保持 no fresh evidence、no catalog endpoint read、no record import、no execution。

结构上仍然沿用四分法：

- `src/shard_preview_catalog_entry_group_split_non_participation.cpp`
- `src/shard_preview_catalog_entry_group_split_non_participation_stages.cpp`
- `src/shard_preview_catalog_entry_group_split_non_participation_checks.cpp`
- `src/shard_preview_catalog_entry_group_split_non_participation_validation.cpp`

职责很清楚：core 负责最终 JSON、稳定 contract、status、digest 和 stage chain；stages 负责 35 个版本台阶；checks 负责把每个版本台阶解释成只读校验记录；validation 负责把源冻结、计数对齐、关闭条件和 no-side-effect 边界收成通过/失败判断。

控制面读到这里，应该得到的判断是：

- `read_only=true`
- `executionAllowed=false`
- `writeRoutingAllowed=false`
- `touchesWal=false`
- `miniKvStartsCatalogSplit=false`
- `miniKvConsumesCatalogRecords=false`
- `miniKvReadsCatalogEndpoint=false`

测试分三层：stages/checks/validation 单测锁住连续性、计数边界和关闭条件；命令测试锁住 `HELP`、`COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 和新命令 usage；`SHARDJSON` 与 CLI/TCP smoke 证明这条链路在真实输入路径下仍然只读，不会写 WAL，也不会启动 catalog 或 sibling service。

本版的价值是把 Node 的 catalog split 维护计划，转成 mini-kv 里一条干净的只读证据链。后续 Node/Java/mini-kv 三方继续拆分时，可以直接复用这套版本化、计数化、边界化的写法。
