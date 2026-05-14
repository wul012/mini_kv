# 第一百二十二次讲解：第六十六版 Restore Compatibility Handoff

本版目标是按最新跨项目计划从 release bundle gate 继续推进到 rollback window readiness 前的 handoff 硬化。Java v56、mini-kv v65、Node v165 已经完成 release bundle 证据消费，下一步推荐并行 Java v57 和 mini-kv v66：Java 侧补 rollback approval handoff，mini-kv 侧补 restore compatibility handoff。

它不是新的 server 命令，不新增 runtime API，不执行 `LOAD`、`COMPACT`、`SETNXEX`，不启动 Node 或 Java，不接生产数据库，也不把 mini-kv 写入 Java 订单一致性链路。它只是一份只读 handoff sample，让后续 Node v166 能把 mini-kv 的 binary、WAL、Snapshot、fixture 兼容性人工确认字段纳入跨项目 rollback window readiness checklist。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v163-post-rollback-readiness-roadmap.md
```

计划要求当前轮到：

```text
推荐并行：Java v57 + mini-kv v66
mini-kv v66 做 restore compatibility handoff sample
固化 binary/WAL/Snapshot/fixture compatibility 的人工确认字段
明确 LOAD/COMPACT/SETNXEX 不由样本执行
明确 restore compatibility 不能产生 Java order authority
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 仍处在“生产雏形阶段”，但本版的关注点从“发布包证据能被消费”推进到“回滚窗口前的人工恢复兼容确认怎么表达”。

v65 解决的是：

```text
Node v165 做 cross-project release bundle gate 时，mini-kv 给出哪一包 runtime artifact evidence？
```

v66 解决的是：

```text
Node v166 准备 rollback window checklist 时，mini-kv 的 restore compatibility 需要哪些人工确认字段？
```

这个 handoff 不是 restore 脚本，也不是回滚授权。它只是把二进制版本、WAL、Snapshot、fixture 兼容性需要人工确认的字段固化下来。

## Handoff Fixture

新增文件：

```text
fixtures/release/restore-compatibility-handoff.json
```

核心字段：

```text
handoff_version=mini-kv-restore-compatibility-handoff.v1
project=mini-kv
project_version=0.66.0
release_version=v66
path=fixtures/release/restore-compatibility-handoff.json
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
consumer_hint=Node v166 rollback window readiness checklist
```

控制面读到这些字段时应该先知道：这是人工交接样本，不是自动恢复入口。`restore_execution_allowed=false` 比普通 `execution_allowed=false` 更直白地告诉 Node：即便这是 restore compatibility 材料，也不能据此执行 `LOAD`、`COMPACT` 或 token 写命令。

## 四类人工确认

handoff 把确认项拆成四类：

```text
binary-compatibility
wal-compatibility
snapshot-compatibility
fixture-compatibility
```

`binary-compatibility` 固化目标版本、运行二进制版本、release tag、artifact digest、operator 和确认时间。它只能确认“当前要比对哪一个 mini-kv runtime artifact”，不能切流，也不能批准生产回滚。

`wal-compatibility` 固化 WAL format、checksum policy、`SETEXAT`、`EXPIREAT`、replay report reviewed 等字段。它能解释 mini-kv 自己的 WAL replay 风险，但不能修复 Java transaction state。

`snapshot-compatibility` 固化 snapshot format、`load_replaces_store_acknowledged`、backup location、restore window 等字段。这里特别强调 `LOAD` 会替换内存 store，所以本版只用 `CHECKJSON LOAD ...` 做风险解释。

`fixture-compatibility` 固化 verification manifest、bundle manifest、rollback evidence、readonly fixture 版本确认。fixture 是证据形状，不是 restore script。

## Release Manifest 跟随

更新文件：

```text
CMakeLists.txt
fixtures/release/verification-manifest.json
fixtures/readonly/infojson-empty-inline.json
```

版本从：

```text
0.65.0
```

推进到：

```text
0.66.0
```

`verification-manifest.json` 新增：

```text
fixtures/release/restore-compatibility-handoff.json
minikv_restore_compatibility_handoff_tests
```

同时保留 v64 的 rollback evidence 和 v65 的 bundle manifest 作为历史来源，不把历史样本改写成 v66。

## 新增测试

新增文件：

```text
tests/restore_compatibility_handoff_tests.cpp
```

新增 CTest 目标：

```text
minikv_restore_compatibility_handoff_tests
```

测试分五层：

```text
1. 检查 handoff fixture 的版本、只读边界、restore_execution_allowed=false 和 consumer_hint
2. 检查 binary/WAL/Snapshot/fixture 四类人工确认项都存在
3. 检查 handoff 引用的 release、rollback、bundle、readonly、recovery、ttl-token fixture 路径真实存在
4. 用真实 CommandProcessor 执行 INFOJSON、CHECKJSON、STORAGEJSON、GET
5. 确认 LOAD、COMPACT、SETNXEX 只被解释，restore:token 仍然不存在
```

原有：

```text
tests/release_verification_manifest_tests.cpp
tests/runtime_artifact_rollback_evidence_tests.cpp
tests/runtime_artifact_bundle_manifest_tests.cpp
```

也同步到当前运行版本 `0.66.0`，但旧 fixture 自己的 `project_version=0.64.0` 和 `project_version=0.65.0` 保持不变，因为它们代表历史证据来源。

## README 与 Roadmap

README 的当前版本更新为 Version 66，并新增：

```text
fixtures/release/restore-compatibility-handoff.json
minikv_restore_compatibility_handoff_tests
```

Roadmap 推进为：

```text
Java v57 + mini-kv v66 并行补 rollback approval / restore compatibility handoff sample
Node v166 等待两边完成后生成 rollback window readiness checklist
```

这和跨项目计划保持一致。

## 真实运行

本版真实 smoke 启动 mini-kv server，并执行：

```text
INFOJSON
CHECKJSON LOAD data/restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX restore:token 30 value
STORAGEJSON
HEALTH
GET restore:token
QUIT
```

验证目标：

```text
INFOJSON -> version=0.66.0
CHECKJSON LOAD -> read_only=true, execution_allowed=false, side_effects=store_replace_from_snapshot
CHECKJSON COMPACT -> read_only=true, execution_allowed=false, side_effects=wal_rewrite_when_enabled
CHECKJSON SETNXEX -> read_only=true, execution_allowed=false, write_command=true
STORAGEJSON -> order_authoritative=false
GET restore:token -> (nil)
```

本轮 smoke 只运行 mini-kv，不启动 Java，不启动 Node，不执行真实 restore、rollback 或生产操作。

## 归档

运行调试归档继续写入：

```text
c/66/图片/
c/66/解释/说明.md
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
不执行 LOAD / COMPACT / SETNXEX
不执行 restore 或 rollback
```

如果未来 Node v166 发现 Java v57 和 mini-kv v66 的 handoff evidence 不一致，正确行为是暂停并让用户确认，而不是自动恢复或回滚。

## 一句话总结

v66 让 mini-kv 从“runtime artifact bundle 可消费”推进到“restore compatibility handoff 可人工核对”，为 Node v166 的跨项目 rollback window readiness checklist 补上了 C++ 侧的恢复兼容交接材料。
