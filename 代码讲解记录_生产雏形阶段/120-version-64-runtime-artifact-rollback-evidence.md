# 第一百二十次讲解：第六十四版 Runtime Artifact Rollback Evidence

本版目标是按最新跨项目计划继续从 release verification 往 rollback readiness 推进。Java v54、mini-kv v63、Node v162 已经完成发布验证证据入口，下一步轮到 Java v55 和 mini-kv v64 并行补“回滚边界证据”。mini-kv 这一侧要说明二进制版本、WAL、Snapshot、fixture 版本在回滚时能证明什么、不能证明什么。

它不是新的 server 命令，不新增 runtime API，不执行 `LOAD`、`COMPACT`、`SETNXEX`，不启动 Node 或 Java，不接生产数据库，也不把 mini-kv 写入 Java 订单一致性链路。它只是一份只读 evidence sample，让后续 Node v163 能把 mini-kv 的回滚边界纳入跨项目 release rollback readiness runbook。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v161-post-controlled-idempotency-drill-roadmap.md
```

计划要求当前轮到：

```text
推荐并行：Java v55 + mini-kv v64
mini-kv v64 做 runtime artifact rollback evidence sample
说明二进制、WAL/Snapshot、fixture 版本回滚边界
不进入 Java 交易一致性链路
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 仍处在“生产雏形阶段”，但关注点从“发布前怎么验证”往“发布后如果要回滚，证据边界在哪里”推进。

v63 回答的是：

```text
发布前 CMake、CTest、只读 smoke、fixture/version 清单如何复现？
```

v64 回答的是：

```text
回滚讨论里，mini-kv 的 binary/version、WAL、Snapshot、fixture 能提供哪些只读证据？
哪些内容必须停在人工确认和外部策略层？
```

这个边界很重要。mini-kv 可以说明 WAL replay、Snapshot load、版本 fixture 的风险和证据形状，但它不能伪造 Java 订单真相源，也不能替 Node 执行回滚。

## Rollback Evidence Fixture

新增文件：

```text
fixtures/release/runtime-artifact-rollback-evidence.json
```

核心字段：

```text
evidence_version=mini-kv-runtime-artifact-rollback.v1
project=mini-kv
project_version=0.64.0
release_version=v64
read_only=true
execution_allowed=false
order_authoritative=false
consumer_hint=Node v163 release rollback readiness runbook
```

控制面读到这些字段时应该先把它当成“回滚证据样本”，而不是“回滚执行器”。`read_only=true` 和 `execution_allowed=false` 明确说明它只描述边界；真正的生产回滚、备份保留、恢复窗口、流量切换都不在这个 fixture 内。

## 四类产物边界

fixture 把 mini-kv 回滚相关产物拆成四类：

```text
binary-version
wal
snapshot
fixtures
```

`binary-version` 说明当前 CMake 项目版本、`INFOJSON` 运行时版本和 release tag 应该互相对齐。它能证明“当前跑的是哪版 mini-kv”，但不能证明“这版一定应该接回流量”。

`wal` 说明 WAL2 checksum、`SETEXAT` token 记录、`EXPIREAT` 过期记录、replay report counters 是恢复证据。它能恢复 mini-kv 自己的 key，但不能创建 Java 订单权威状态，不能修复 Java 事务状态。

`snapshot` 说明 `SAVE/LOAD` 是手工 Snapshot 能力，且 `LOAD` 会替换当前内存 store。这个能力风险高，所以本版只用 `CHECKJSON LOAD ...` 检查它的契约，不执行 `LOAD`。

`fixtures` 说明 `verification-manifest.json`、`runtime-artifact-rollback-evidence.json`、`infojson-empty-inline.json` 是版本和证据形状样本。它们是下游核对材料，不是自动回滚脚本。

## Read-Only Smoke 契约

v64 的 smoke 故意选择这些命令：

```text
INFOJSON
CHECKJSON LOAD data/rollback.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX rollback:token 30 value
STORAGEJSON
HEALTH
GET rollback:token
QUIT
```

关键点是三个 `CHECKJSON`：

```text
CHECKJSON LOAD ...      -> 只解释 LOAD 会 store_replace_from_snapshot
CHECKJSON COMPACT       -> 只解释 COMPACT 会 wal_rewrite_when_enabled
CHECKJSON SETNXEX ...   -> 只解释 token 写入和 WAL 风险
```

它们都不执行目标命令。真实 smoke 最后再跑：

```text
GET rollback:token -> (nil)
```

这证明 `CHECKJSON SETNXEX` 没有偷跑成真实 token claim。

## Release Manifest 跟随

更新文件：

```text
fixtures/release/verification-manifest.json
fixtures/readonly/infojson-empty-inline.json
CMakeLists.txt
```

版本从：

```text
0.63.0
```

推进到：

```text
0.64.0
```

`verification-manifest.json` 也把 targeted tests 扩展为包含：

```text
minikv_runtime_artifact_rollback_evidence_tests
```

这样 Node v163 后面读取 release evidence 时，能看到 v64 的 rollback sample 已经进入 release verification 清单，而不是散落在单独文件里。

## 新增测试

新增文件：

```text
tests/runtime_artifact_rollback_evidence_tests.cpp
```

新增 CTest 目标：

```text
minikv_runtime_artifact_rollback_evidence_tests
```

测试分四层：

```text
1. 检查 rollback evidence fixture 的版本、只读边界、consumer_hint、pause conditions
2. 检查 binary-version、wal、snapshot、fixtures 四类产物边界都存在
3. 用真实 CommandProcessor 执行 INFOJSON / CHECKJSON / STORAGEJSON / GET
4. 确认 LOAD、COMPACT、SETNXEX 都只是被 CHECKJSON 解释，rollback:token 仍然不存在
```

原有：

```text
tests/release_verification_manifest_tests.cpp
```

也同步到 `0.64.0`，并检查新 rollback evidence fixture 已列入 release manifest。

## README 与 Roadmap

README 的当前版本更新为 Version 64，并新增：

```text
fixtures/release/runtime-artifact-rollback-evidence.json
minikv_runtime_artifact_rollback_evidence_tests
```

Roadmap 推进为：

```text
Java v55 + mini-kv v64 并行补 rollback evidence
Node v163 等待两边完成后生成 release rollback readiness runbook
```

这和跨项目计划保持一致。

## 真实运行

本版真实 smoke 启动 mini-kv server，并执行：

```text
INFOJSON
CHECKJSON LOAD data/rollback.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX rollback:token 30 value
STORAGEJSON
HEALTH
GET rollback:token
QUIT
```

验证目标：

```text
INFOJSON -> version=0.64.0
CHECKJSON LOAD -> read_only=true, execution_allowed=false, side_effects=store_replace_from_snapshot
CHECKJSON COMPACT -> read_only=true, execution_allowed=false, side_effects=wal_rewrite_when_enabled
CHECKJSON SETNXEX -> read_only=true, execution_allowed=false, write_command=true
STORAGEJSON -> order_authoritative=false
GET rollback:token -> (nil)
```

本轮 smoke 只运行 mini-kv，不启动 Java，不启动 Node，不执行真实回滚命令。

## 归档

运行调试归档继续按新规则写入：

```text
c/64/图片/
c/64/解释/说明.md
```

历史目录 `a/` 和 `b/` 保留，不搬迁。

## 边界

本版继续保持：

```text
不新增 runtime 命令
不修改 Node
不修改 Java
不启动 Node 或 Java
不打开 UPSTREAM_ACTIONS_ENABLED=true
不进入 Java 订单权威链路
不让 mini-kv 成为订单权威存储
不执行 LOAD / COMPACT / SETNXEX 回滚敏感命令
```

如果未来 Node v163 发现 Java v55 和 mini-kv v64 的回滚证据互相矛盾，正确行为是暂停并让用户确认，而不是自动回滚。

## 一句话总结

v64 让 mini-kv 从“发布验证证据可复现”继续推进到“运行时产物回滚边界可被只读消费”，为 Node 后续生成跨项目 release rollback readiness runbook 补上了 C++ 侧的边界材料。
