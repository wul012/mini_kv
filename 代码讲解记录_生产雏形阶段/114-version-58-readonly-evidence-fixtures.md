# 第一百一十四次讲解：第五十八版只读 evidence fixture 包

本版目标是给 Node v152/v153 准备一个 mini-kv 只读 evidence 供给包。它把已有的 `CHECKJSON` 写/读命令契约样本，和新增的 `INFOJSON`、`STATSJSON` 运行态只读样本放到同一个索引里，方便 Node 后续引用。

它不是新的服务端命令，不是生产联调执行器，不启动 Java 或 Node，也不执行 mini-kv 写命令。它的角色是把“mini-kv 能提供哪些只读证据”固定成文件级样本和测试契约，让 Node v152 先能引用上游 evidence，再由 v153 写真实只读联调 runbook。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v149-post-production-pass-evidence-roadmap.md
```

计划要求当前轮到：

```text
Java v49 + mini-kv v58
mini-kv 补 CHECKJSON/INFOJSON/STATSJSON 只读样本与说明
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 索引入口

新增入口：

```text
fixtures/readonly/index.json
```

这个文件的定位不是“真实 pass evidence”，而是“只读样本索引”。关键边界字段是：

```text
read_only=true
execution_allowed=false
runtime_read_commands_allowed=true
write_execution_allowed=false
order_authoritative=false
consumer_hint=Node v152/v153 production pass evidence preparation
```

控制面读到它时应该这样理解：

```text
可以把这些样本当作响应结构和 no-write 边界参考
不能把这些样本当作生产联调已经通过
不能据此打开 UPSTREAM_ACTIONS_ENABLED=true
不能把 mini-kv 当订单权威存储
```

## CHECKJSON 样本

已有两个样本仍放在：

```text
fixtures/checkjson/set-orderops-write-contract.json
fixtures/checkjson/get-orderops-read-contract.json
```

v58 没有复制它们，而是在 `fixtures/readonly/index.json` 中引用它们：

```text
CHECKJSON SET orderops:1 value
CHECKJSON GET orderops:1
```

`SET` 这里看起来像写命令，但实际执行的是 `CHECKJSON`，也就是只读契约检查。样本里的 `execution_allowed=false` 说明目标命令不会被执行；`runtime_mutation=false` 说明本次 evidence 不会改 Store、WAL 或订单状态。

## INFOJSON 样本

新增文件：

```text
fixtures/readonly/infojson-empty-inline.json
```

它捕获的是一个空 inline processor 的身份信息：

```text
version=0.58.0
protocol=["inline"]
uptime_seconds=0
live_keys=0
wal.enabled=false
metrics.enabled=false
```

这里最容易误解的是 `uptime_seconds`。真实运行时它会增长，所以索引里把它标成动态字段：

```text
dynamic_fields=["server.uptime_seconds"]
```

测试为了让样本稳定，会构造一个 `started_at` 略晚于当前时间的 processor，让 formatter 按已有逻辑输出 `0`。这不是改运行时，只是让 fixture 可以和真实命令输出做精确比较。

## STATSJSON 样本

新增文件：

```text
fixtures/readonly/statsjson-empty-inline.json
```

它捕获的是空 processor 在 `STATSJSON` 命令记账之前看到的 metrics：

```text
live_keys=0
wal_enabled=false
wal=null
commands.total_commands=0
commands.breakdown=[]
connection_stats.available=false
```

真实服务运行后，命令数量和 latency 字段会变化，所以索引把这些字段标成动态字段：

```text
commands.total_latency_ns
commands.avg_latency_ns
commands.max_latency_ns
commands.breakdown[*].*_latency_ns
```

这样 Node 后续消费时可以验证 JSON 结构和 no-write 边界，而不会把空样本里的计数误读成真实生产运行指标。

## 测试入口

新增测试：

```text
tests/readonly_fixture_tests.cpp
```

测试做三类保护：

```text
索引保护：fixture_count、路径、consumer_hint、no-write 边界不能漂移
样本保护：CHECKJSON 两个旧样本仍然等于真实命令输出
动态字段保护：INFOJSON/STATSJSON 空样本在受控 runtime 下等于真实命令输出
```

`CMakeLists.txt` 同步注册：

```text
minikv_readonly_fixture_tests
readonly_fixture_tests
```

## 文档与归档

README 当前版本提升为 Version 58，并说明：

```text
fixtures/readonly/index.json
fixtures/readonly/infojson-empty-inline.json
fixtures/readonly/statsjson-empty-inline.json
readonly_fixture_tests
```

运行归档从本版开始写入新位置：

```text
b/58/图片/
b/58/解释/说明.md
```

## 一句话总结

v58 没有扩大 mini-kv 的执行能力，而是把 CHECKJSON、INFOJSON、STATSJSON 的只读样本统一成 Node 可引用的 evidence 包，为后续 Node v152/v153 做真实只读联调前准备，同时继续守住 no-write 和 not-order-authoritative 边界。
