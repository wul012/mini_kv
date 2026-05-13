# 第一百一十六次讲解：第六十版运行时只读字段说明增强

本版目标是补齐 `INFOJSON` / `STATSJSON` 被 Node release evidence review 消费时需要的字段解释。v59 已经让运行时响应自己声明 `read_only`、`execution_allowed`、`order_authoritative` 和动态字段；v60 继续往后走一步，把这些字段“怎么读、哪些稳定、哪些动态、哪些命令可以出现在 read-only smoke 里”固化成仓库内证据。

它不是新的执行入口，不是新的服务端命令，不新增 `SET`、`DEL`、`EXPIRE` 或任何写路径，也不让 mini-kv 承担订单权威数据。它只增强只读 evidence 的解释能力，方便 Node v159 把 Node capture archive、Java v51 说明和 mini-kv v60 说明合成 release evidence review。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v156-post-read-only-window-capture-roadmap.md
```

计划要求当前轮到：

```text
Java v51 + mini-kv v60
mini-kv 补 INFOJSON / STATSJSON 字段说明
mini-kv 补 read-only smoke 字段解释
不新增 SET / DEL / EXPIRE 入口
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 现在处在“生产雏形阶段”：它已经有 KV、TCP/RESP、WAL、Snapshot、metrics、CHECKJSON、STORAGEJSON、runtime read evidence 和恢复证据，但还不是生产数据库，也不是 Java 订单系统的权威存储。

v60 的成熟度变化不在于新增运行能力，而在于把“控制面应该怎样解释 mini-kv 的只读响应”写成可测试、可归档、可被下游引用的证据文件。这个动作很小，但位置关键：它让下一轮 Node v159 release review 不必靠口头约定理解字段。

## 字段说明文件

新增文件：

```text
fixtures/readonly/runtime-read-field-guide.json
```

这个文件是字段说明，不是运行时接口。它包含：

```text
guide_version
schema_version
read_only
execution_allowed
order_authoritative
consumer_hint
commands
read_only_smoke
diagnostics
```

其中 `commands` 分别解释：

```text
INFOJSON -> runtime_identity
STATSJSON -> runtime_metrics
```

每个命令下都分成稳定字段和动态字段。稳定字段可以参与 shape/contract 判断，动态字段只能检查存在性、类型和合理范围，不能和 fixture 做精确值比较。

## INFOJSON 字段

`INFOJSON` 的角色是 runtime identity evidence。它说明当前 mini-kv 进程是什么版本、启用了什么协议、WAL/metrics 是否开启、请求大小限制是多少，以及响应本身是不是只读证据。

关键字段解释：

```text
schema_version
 -> 响应结构版本，控制面先用它判断 shape 是否支持

read_only
 -> 必须为 true，表示该命令只报告运行状态

execution_allowed
 -> 必须为 false，表示这个响应不授权执行写命令

order_authoritative
 -> 必须为 false，表示 mini-kv 不是订单系统权威来源

evidence_type=runtime_identity
 -> 控制面把它归类为运行时身份信息，不是业务数据

version
 -> CMake 注入的 mini-kv 版本，用于和 tag / release review 对齐

server.protocol
 -> inline fixture 通常只有 inline，TCP server capture 通常是 inline + resp

server.max_request_bytes
 -> TCP 请求限制配置，只是运行配置证据

store.live_keys
 -> KV 当前 live key 数量，不是订单数量

wal.enabled
 -> 当前进程是否挂了 WAL

metrics.enabled
 -> 当前进程是否开启周期 metrics 输出

diagnostics.write_commands_executed
 -> 必须为 false，证明 INFOJSON 本身没有执行写命令
```

动态字段：

```text
server.uptime_seconds
```

它随进程存活时间变化。Node 做 live capture 时只能检查字段存在和类型，不能拿 fixture 里的 `0` 做精确比较。

## STATSJSON 字段

`STATSJSON` 的角色是 runtime metrics evidence。它说明命令计数、延迟、WAL 状态和连接计数，而不是业务订单状态。

关键字段解释：

```text
schema_version
read_only
execution_allowed
order_authoritative
evidence_type=runtime_metrics
live_keys
wal_enabled
wal
commands.total_commands
connection_stats.available
diagnostics.write_commands_executed
```

控制面读到后要这样理解：

```text
read_only=true
 -> 可以作为只读 capture evidence

execution_allowed=false
 -> 不允许从这个响应推导“可以执行写操作”

order_authoritative=false
 -> metrics 不让 mini-kv 变成订单权威库

wal=null
 -> memory-only 运行是合法状态，不是字段缺失

connection_stats.available=false
 -> inline fixture 常见

connection_stats.available=true
 -> TCP server 真实 capture 常见
```

动态字段：

```text
commands.total_latency_ns
commands.avg_latency_ns
commands.max_latency_ns
commands.breakdown[*].*_latency_ns
```

这些值依赖机器、命令顺序和采样时刻。它们适合证明 metrics 有形状、有数值、有运行信号，不适合当固定 digest 的精确比较对象。

## read-only smoke

字段说明里还固化了 read-only smoke 允许发送的命令：

```text
INFOJSON
STATSJSON
CHECKJSON GET orderops:1
HEALTH
QUIT
```

并明确本轮 smoke 不使用：

```text
SET
DEL
EXPIRE
LOAD
RESETSTATS
COMPACT
```

这里的边界很重要：`CHECKJSON GET orderops:1` 只是读取目标命令的执行契约，不执行 `GET` 以外的写动作；`HEALTH` 是 liveness evidence；`QUIT` 只关闭连接。即使 live probe 成功，它也只是只读 evidence，不是 production pass。

## fixture 索引

更新文件：

```text
fixtures/readonly/index.json
```

索引新增：

```text
id=runtime-read-field-guide
sample_type=field_guide
path=fixtures/readonly/runtime-read-field-guide.json
consumer_hint=Node v159 release evidence review
```

并把整体 consumer hint 推进到：

```text
Node v159 read-only capture release evidence review preparation
```

这表示 v60 不再只是为 v156 真实读取窗口做准备，而是为 Node v159 汇总 release evidence 做解释输入。

## 版本与文档

同步更新：

```text
CMakeLists.txt -> 0.60.0
fixtures/readonly/infojson-empty-inline.json
fixtures/readonly/README.md
README.md
```

`INFOJSON` fixture 的 `version` 跟着项目版本变为 `0.60.0`，避免 Node 后续拿 tag 和运行时 version 对不上。

## 测试

更新测试：

```text
tests/readonly_fixture_tests.cpp
```

新增断言保护：

```text
fixture_count=5
runtime-read-field-guide 路径存在于 index
consumer_hint 指向 Node v159
字段说明包含 INFOJSON / STATSJSON
字段说明包含 server.uptime_seconds 动态比较规则
字段说明包含 latency 动态字段
read-only smoke 包含 CHECKJSON GET orderops:1
SET / DEL / EXPIRE 明确不用于本轮 smoke
skipped/mixed 不是 production pass
field guide 不是新 server endpoint
```

同时把 `INFOJSON` 的 version 断言改为读取 `minikv::version`，这样后续升级项目版本时，测试关注的是“fixture 与构建版本一致”，而不是散落的硬编码字符串。

## 归档

运行归档位置：

```text
b/60/图片/
b/60/解释/说明.md
```

本版真实 smoke 仍只发送只读命令：

```text
INFOJSON
STATSJSON
CHECKJSON GET orderops:1
HEALTH
QUIT
```

没有执行 `SET`、`DEL`、`EXPIRE`，验证结束后停止 server 进程。

## 一句话总结

v60 把 mini-kv 的只读运行时 evidence 从“响应自己有边界字段”推进到“仓库内有可测试字段说明”，让 Node v159 能更稳地合成 Java、Node、mini-kv 的 release evidence review。
