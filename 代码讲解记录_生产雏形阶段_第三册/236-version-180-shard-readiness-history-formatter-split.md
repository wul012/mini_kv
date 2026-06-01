# 236. mini-kv v180 shard readiness 历史 formatter 拆分

## 本版目标

v180 是一个契约保持型中大重构：把 `SHARDJSON` 里历史 fixture、Node 归档兼容和 historical fallback 相关 formatter 从 `src/shard_readiness.cpp` 拆到独立的 `src/shard_readiness_history.cpp`，同时冻结 v179 的数字跨度审计为 `fixtures/release/shard-readiness-v179.json`。

它不是什么：不是新命令、不是服务启动入口、不是 active shard router，也不是写路由或 runtime execution gate。它只是把已经膨胀的主 formatter 拆小，并用 `shardReadinessHistoryMaintenance` 证明拆分没有改变只读边界。

## 模块职责

`include/minikv/shard_readiness_history.hpp` 定义历史 formatter 的小型接口：

- `format_diagnostics_json`
- `format_fixture_parity_json`
- `format_archive_compatibility_json`
- `format_historical_fallback_json`
- `format_history_formatter_maintenance_json`

`src/shard_readiness_history.cpp` 承接这些实现。它维护历史 fixture 列表、Node v370-v458 archived evidence 兼容列表、当前 previous-consumed baseline，以及本版新增的 maintenance 证据。

`src/shard_readiness.cpp` 仍是 `SHARDJSON` 的总装入口，但不再直接拥有这些历史长列表。它通过 `history::...` 调用拼接 JSON，并把 release 推进到 v180、status 推进到 `shard-readiness-history-formatter-maintenance-read-only`、digest 推进到 `fnv1a64:3403c490a3e623e0`。

## 新增 maintenance 证据

`shardReadinessHistoryMaintenance` 的核心字段说明这次维护拆分的边界：

- `preservesFixtureParity=true`：当前 fixture 仍是 rolling 文件，历史基线仍走版本化 frozen fixture。
- `preservesArchiveCompatibility=true`：Node archived evidence 兼容清单没有被修改成可执行入口。
- `preservesHistoricalFallback=true`：historical fallback 仍明确禁止用 rolling current 做历史 baseline。
- `changesShardJsonCommand=false` 和 `changesFixturePath=false`：命令和 fixture path 不变。
- `startsServices=false`、`runtimeProbeAllowed=false`、`writeRoutingAllowed=false`、`executionAllowed=false`：维护拆分不授权运行、写入或路由。

控制面读到这段时，应理解为“mini-kv 做了一次格式化/维护拆分，历史证据链仍可读且只读”，不能把它理解成 runtime readiness 或 router readiness。

## Boundary catalog

`src/shard_readiness_boundary_fields.cpp` 把目录推进到 `read-only-boundary-fields.v11`。字段数从 422 增加到 431，组数从 22 增加到 23，新组是 `shard-readiness-history-maintenance`。

新增字段覆盖 maintenance 里最重要的边界布尔值，包括 `preservesFixtureParity`、`preservesArchiveCompatibility`、`preservesHistoricalFallback`、`changesShardJsonCommand`、`changesFixturePath`、`startsServices`、`runtimeProbeAllowed`、`writeRoutingAllowed`、`executionAllowed`。

## 测试保护

`tests/command_tests.cpp` 和 `tests/shard_readiness_tests.cpp` 同步更新到 v180。它们验证：

- 当前 `SHARDJSON` 输出 v180 status 和 digest。
- `fixtureParity` 增加 v179 frozen fixture。
- `historicalFallback` 从 v178 baseline 前进到 v179 baseline。
- `shardReadinessHistoryMaintenance` 存在并保持只读边界。
- Node route split window 仍覆盖 v433-v458，source frozen digest 改为 v179 digest。
- boundary catalog v11 的 431 字段、23 组和新增字段都被发布。
- 冻结的 v179 fixture 仍保留 v179 数字跨度审计语义。

定向测试守住 `SHARDJSON` 合约，全量 71 条 CTest 证明拆分没有影响其他模块。TCP smoke 证明真实客户端路径仍能读到 v180 只读证据，且服务进程验证后被停止。

一句话总结：v180 把 shard-readiness 主 formatter 继续拆小，让历史证据链更容易维护，同时把“不改命令、不改 fixture path、不启动、不写、不执行”的边界写进机器可读证据。
