# 第一百一十三次讲解：第五十七版 recovery index cost 与 retention boundary

本版目标是增强 mini-kv 的恢复证据索引，让它不只告诉控制面“有一个恢复样本”，还说明这个样本的 restart/replay 成本边界和 retention 边界。

它不是新的服务端命令，不是新的恢复执行入口，也不是生产备份策略。它的角色是把 v56 的 `fixtures/recovery/index.json` 继续收紧成可被 Node v107 读取的只读证据：Node 可以知道这个 fixture 只证明了一个小规模恢复样本，不能把它误读成完整生产保留、备份或轮转方案。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v103-production-auth-audit-roadmap.md
```

计划里 mini-kv v57 的目标是：

```text
增强 recovery fixture index
补 restart/replay cost 或 retention boundary 说明字段
```

所以本版只改 mini-kv / C++ 仓库内的 fixture、测试和文档，不修改 Node 或 Java。

## 索引入口

本版继续使用 v56 新增的入口：

```text
fixtures/recovery/index.json
```

v56 的索引已经记录：

```text
fixture path
evidence_version
digest
recovered
digests_match
read_only
execution_allowed
order_authoritative
```

v57 在这个基础上增加两块信息：

```text
restart_replay_cost
retention_boundary
```

这样 Node v107 后续复查 production readiness 时，不只是看到“恢复成功”，还能看到“这个恢复证据的规模和保留边界是什么”。

## restart_replay_cost

新增字段：

```json
"restart_replay_cost":{
  "cost_model":"sample_record_count",
  "snapshot_loads":1,
  "wal_records_replayed":1,
  "expected_replay_complexity":"O(snapshot_keys + wal_records)",
  "measured_in_fixture_only":true
}
```

几个字段的含义：

```text
cost_model=sample_record_count
 -> 当前只按 fixture 样本里的记录数表达成本，不是性能基准。

snapshot_loads=1
 -> 恢复样本需要加载一次 snapshot baseline。

wal_records_replayed=1
 -> 恢复样本只重放一个 WAL delta record。

expected_replay_complexity=O(snapshot_keys + wal_records)
 -> 说明恢复工作量理论上和 snapshot key 数、WAL record 数相关。

measured_in_fixture_only=true
 -> 明确这不是生产压测数据。
```

这组字段让控制面不会把一个小 fixture 误判成“任意规模 WAL 都已验证高性能恢复”。

## retention_boundary

新增字段：

```json
"retention_boundary":{
  "fixture_sample_only":true,
  "values_retained_in_index":false,
  "production_retention_policy":"not_defined",
  "backup_or_rotation_policy":"not_included",
  "operator_action_required":"define retention before production use"
}
```

它表达的是：

```text
index 本身不保留业务 value
fixture 只是样本
生产保留策略尚未定义
备份和轮转策略不在本版范围内
真正生产使用前需要单独定义 retention
```

这和 mini-kv 当前定位一致：它可以作为自研 KV、缓存、幂等/观测实验层，也可以给 Node 提供只读证据，但不能因为有 recovery fixture 就被视为生产数据库。

## consumer_hint

本版把消费提示从：

```text
Node v103 production readiness summary v3
```

更新为：

```text
Node v107 production readiness summary v4
```

原因是最新计划已经进入 v103 衍生阶段，Node v107 会复查 v104-v106 后的 auth/audit 进展，并且可并行消费 Java v48 与 mini-kv v57 的上游只读证据增强。

## 测试覆盖

更新测试：

```text
tests/recovery_fixture_index_tests.cpp
```

测试继续读取：

```text
fixtures/recovery/index.json
fixtures/recovery/restart-recovery-evidence.json
```

本版新增断言锁住：

```text
baseline_saved_keys=1
replay_records=1
restart_replay_cost
expected_replay_complexity
measured_in_fixture_only=true
retention_boundary
production_retention_policy=not_defined
backup_or_rotation_policy=not_included
operator_action_required
consumer_hint=Node v107 production readiness summary v4
retention policy not defined
```

这些断言保护三件事：

```text
恢复成本字段不会静默丢失
retention 边界不会静默丢失
Node 后续读取索引时能看到最新 v107 消费语义
```

## README 与版本号

`CMakeLists.txt` 版本提升到：

```text
0.57.0
```

README 当前版本提升为 Version 57，并补充：

```text
restart/replay sample cost
retention boundary
Node v107 consumer hint
```

`fixtures/recovery/README.md` 也补入说明：这些字段只证明小样本恢复路径，不代表生产 retention、backup 或 rotation policy 已经完成。

## 验证与归档

本版本地完成：

```text
CMake configure
CMake build
CTest 15/15
minikv_recovery_fixture_index_tests 单测直跑
fixture index smoke 字段核对
```

关键命令截图归档在：

```text
a/57/图片/
a/57/解释/说明.md
```

## 一句话总结

v57 没有扩大 mini-kv 的运行时能力，而是把恢复证据索引从“可发现”推进到“可解释边界”，让 Node v107 以后能更准确地区分恢复样本、恢复成本和生产保留策略之间的差别。
