# 第一百一十五次讲解：第五十九版运行时只读自描述增强

本版目标是补强 `INFOJSON` 和 `STATSJSON` 的真实运行 smoke 输出稳定性。v58 已经有只读 fixture 包，但真实 live probe 更需要从运行时响应本身直接读到边界字段，所以 v59 把运行时 JSON 输出改成自描述 evidence。

它不是新的服务端命令，不是新的写权限模型，不启动 Java 或 Node，也不让 Node 自动启动 mini-kv。它只让已经存在的只读命令在响应里明确声明：我是只读的、不会执行写命令、不是订单权威数据、哪些字段是动态字段。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v153-post-operator-runbook-roadmap.md
```

计划要求当前轮到：

```text
Java v50 + mini-kv v59
mini-kv 补强 INFOJSON / STATSJSON smoke 输出字段稳定性
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## INFOJSON

改动入口：

```text
src/command.cpp
format_info_json(...)
```

v59 给 `INFOJSON` 增加稳定 envelope：

```text
schema_version=1
read_only=true
execution_allowed=false
order_authoritative=false
evidence_type=runtime_identity
diagnostics.write_commands_executed=false
diagnostics.dynamic_fields=["server.uptime_seconds"]
```

控制面读到后可以明确判断：

```text
这个响应是 runtime identity evidence
它不执行写命令
它不是订单权威存储
uptime_seconds 是动态字段，不能当作固定 fixture 值比较
```

原有字段仍保留：

```text
version
server.protocol
server.uptime_seconds
server.max_request_bytes
store.live_keys
wal.enabled
metrics.enabled
```

## STATSJSON

改动入口：

```text
src/command.cpp
format_stats_json(...)
```

v59 给 `STATSJSON` 增加同类 envelope：

```text
schema_version=1
read_only=true
execution_allowed=false
order_authoritative=false
evidence_type=runtime_metrics
diagnostics.write_commands_executed=false
```

并明确动态字段：

```text
commands.total_latency_ns
commands.avg_latency_ns
commands.max_latency_ns
commands.breakdown[*].*_latency_ns
```

这样 Node v156 做真实只读 capture 时，可以把 latency 当运行时指标，而不是把它误判成 fixture drift。

## fixture 更新

同步更新：

```text
fixtures/readonly/infojson-empty-inline.json
fixtures/readonly/statsjson-empty-inline.json
fixtures/readonly/index.json
fixtures/readonly/README.md
```

其中 `index.json` 的 consumer hint 更新为：

```text
Node v156 real-read capture preparation
```

这表示 v59 的证据服务于下一步真实只读窗口，而不是 v58 阶段的 archive/runbook 准备。

## 测试覆盖

更新测试：

```text
tests/command_tests.cpp
tests/readonly_fixture_tests.cpp
```

`command_tests` 锁住真实运行时输出必须包含自描述字段。
`readonly_fixture_tests` 继续比较 fixture 和真实命令输出完全一致，并确认索引里的 consumer hint、schema、read-only 边界和 diagnostics 没有漂移。

## 归档

运行归档位置：

```text
b/59/图片/
b/59/解释/说明.md
```

真实 smoke 只发送：

```text
INFOJSON
STATSJSON
CHECKJSON GET orderops:1
HEALTH
QUIT
```

没有执行 `SET`、`DEL`、`EXPIRE` 等写命令。

## 一句话总结

v59 把 mini-kv 的只读运行时响应从“能被读取”推进到“响应自身声明边界”，让 Node v156 的真实只读 capture 可以更稳地识别 no-write、not-order-authoritative 和动态指标字段。
