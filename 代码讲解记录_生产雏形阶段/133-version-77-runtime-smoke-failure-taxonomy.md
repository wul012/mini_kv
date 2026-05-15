# 第七十七版 Runtime Smoke Failure Taxonomy

本版目标是在 v76 `SMOKEJSON` 只读运行证据上继续补一层“失败分类说明”。Node v193 后续要把真实只读 adapter 的失败从普通 blocked/skipped 拆成可操作分类，所以 mini-kv 这边先把自己能提供的分类边界放进运行时 JSON 和 release fixture。

它不是什么要先说清楚：这不是新的执行入口，不会替 Node 自动重试或自动启动 mini-kv，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 restore，不写 WAL，不读生产密钥，也不让 mini-kv 进入 Java 订单事务链。它只是让控制面在读失败时有稳定词表可引用。

## 本版所处阶段

跨项目计划 `v191-post-real-read-adapter-roadmap.md` 指出：Node v192 已经补了人工打开真实只读窗口的 runbook，下一步推荐 Java v68 和 mini-kv v77 并行补失败分类。mini-kv 的任务很窄，只围绕 `SMOKEJSON` / runtime smoke evidence 描述连接失败、JSON 解析失败、只读命令失败和危险命令误用，不新增写命令。

## 1. `src/command.cpp`

本版新增的小结构是：

```text
SmokeFailureTaxonomyEntry
```

它只服务于 `format_smoke_failure_taxonomy_json()`，字段包括：

```text
id
source
meaning
node_action
retryable
safe_to_auto_start
write_risk
```

这里的关键不是 C++ 结构本身，而是契约含义：

```text
safe_to_auto_start=false
```

表示 Node 即使看到 `connection-refused` 或 `timeout`，也不能因此自动启动 mini-kv；真实只读窗口仍由操作者手动打开。

```text
write_risk=true
```

只出现在 `unsafe-surface` 和 `unexpected-write-signal` 这类分类上。控制面读到后应该停止 rehearsal，不应该继续生成 pass evidence。

分类列表固定为：

```text
closed-window
connection-refused
timeout
invalid-json
read-command-failed
unsafe-surface
unexpected-write-signal
```

`format_smoke_json()` 把它挂到：

```text
failure_taxonomy
```

这让真实 TCP 读取 `SMOKEJSON` 时直接拿到分类词表，而不是只靠静态文档。

## 2. `fixtures/release/runtime-smoke-evidence.json`

fixture 从 `mini-kv-runtime-smoke-evidence.v1` 升到 `v2`，项目版本和 release 版本升到：

```text
project_version=0.77.0
release_version=v77
consumer_hint=Node v193 real-read adapter failure taxonomy
```

新增的 `failure_taxonomy` 是静态验收说明：它告诉 Node v193 这些分类怎么解释，哪些可以人工窗口后重试，哪些必须停下来。例如：

```text
connection-refused -> mini-kv 未监听或人工窗口未启动
invalid-json -> SMOKEJSON 不是可解析 JSON
unsafe-surface -> adapter 计划里出现 LOAD/COMPACT/SETNXEX/RESTORE
unexpected-write-signal -> 运行证据显示写入或管理命令被执行
```

这份 fixture 仍然是证据说明，不是 retry 逻辑，也不授权 Node 改变 `UPSTREAM_ACTIONS_ENABLED`。

## 3. `fixtures/release/verification-manifest.json`

manifest 升到 v77，构建目录更新为：

```text
cmake-build-v77
```

read-only smoke 仍然保持原来的只读命令组合：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

变化点只有一个：验收期望新增 `SMOKEJSON exposes read-only failure taxonomy for Node v193`。也就是说，本版不改变运行步骤，只增强控制面能读懂的字段。

## 4. 测试保护

`command_tests.cpp` 直接执行 `SMOKEJSON`，保护运行时 JSON 包含：

```text
failure_taxonomy.schema_version=1
consumer=Node v193 real-read adapter failure taxonomy
connection-refused
invalid-json
read-command-failed
unsafe-surface
unexpected-write-signal
safe_to_auto_start=false
write_risk=true
```

`runtime_smoke_evidence_tests.cpp` 同时保护静态 fixture 和真实 `CommandProcessor` 输出，避免只更新文档却忘了运行时命令。

`release_verification_manifest_tests.cpp` 保护 v77 manifest、CMake 版本、`cmake-build-v77` 命令、`SMOKEJSON` smoke 期望和 taxonomy 消费方。

老的 release 测试里对当前运行版本的断言同步升到 `0.77.0`，但历史 fixture 自身的 source release 信息没有改。

## 5. 真实运行

本轮真实 smoke 仍启动：

```text
minikv_server 6416 127.0.0.1
minikv_client 127.0.0.1 6416 5000 --connect-retries 10 --retry-delay-ms 100
```

实际读到 `SMOKEJSON` 后可以看到 `version:"0.77.0"`、`failure_taxonomy.consumer:"Node v193 real-read adapter failure taxonomy"`，并且 `GET restore:real-read-token` 返回 `(nil)`。这证明 taxonomy 是运行时可读的只读证据，不是通过写 token 或执行 restore 得到的结果。

## 一句话总结

v77 把 mini-kv 的 real-read 证据从“能读到运行状态”推进到“读失败也有稳定分类词表”，为 Node v193 做失败分类和后续归档打好 C++ 侧边界。
