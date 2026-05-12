# 第一百一十一次讲解：第五十五版 restart recovery evidence sample

本版目标是给 mini-kv 补一个面向控制面的“重启恢复证据样本”。

它不是新的服务端命令，不是新的写入口，也不是把 mini-kv 接成订单权威数据库。它的角色是把 mini-kv 已经具备的两类基础设施能力：手动 snapshot 和 WAL replay，组合成一个稳定 JSON fixture，方便后续 Node 控制面判断“这个自研 KV 至少能拿出可追溯的重启恢复证据”。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v95-production-gap-roadmap.md
```

计划里 mini-kv v55 的目标是：

```text
新增 WAL/snapshot restart recovery sample
输出前后 key count、digest、recovered=true/false
不改核心协议，不扩大写命令范围
```

所以本版只做 C++ / mini-kv，不修改 Node 或 Java。

## 证据模型

新增头文件：

```text
include/minikv/recovery_evidence.hpp
```

里面有三个核心结构：

```cpp
struct RecoveryEvidenceState {
    std::size_t key_count = 0;
    std::string digest;
};
```

它表示某个 Store 状态的摘要。控制面不需要拿到所有 key/value，只需要看到：

```text
当前有多少 live key
当前状态 digest 是什么
```

第二个结构是：

```cpp
struct RestartRecoveryEvidence {
    RecoveryEvidenceState before;
    RecoveryEvidenceState after;
    bool recovered = false;
    bool digests_match = false;
    std::size_t snapshot_saved_keys = 0;
    WalReplayReport wal_replay;
};
```

它把恢复前、恢复后、snapshot 保存数量和 WAL replay 报告放在同一个证据对象里。

几个关键字段的语义是：

```text
recovered=true
digests_match=true
wal.replay.skipped_records=0
wal.replay.truncated_records=0
wal.replay.checksum_failed_records=0
```

这代表本次样本恢复链路完整：snapshot baseline 能加载，WAL delta 能重放，恢复后 Store 摘要与恢复前一致。

## digest 规则

实现文件：

```text
src/recovery_evidence.cpp
```

`recovery_evidence_state` 会读取 Store 的 `snapshot_items()`：

```cpp
const auto items = store.snapshot_items();
```

这个接口天然会：

```text
剔除过期 key
按 key 排序
保留 value 和 expires_at
```

所以 digest 不依赖 unordered_map 的内部顺序。digest source 使用长度前缀拼接：

```text
schema
key_count
key
value
expires_at
```

然后计算 `fnv1a64:<hex>`。

本版 fixture 的固定 digest 是：

```text
fnv1a64:0991cf4e41a8005a
```

它对应两个恢复后的 live key：

```text
recovery:base  -> snapshot-value
recovery:delta -> wal-value
```

这里没有使用加密签名。它只是一个稳定证据指纹，用来让测试和 Node 后续消费时发现 fixture 漂移。

## JSON 输出

格式化函数：

```cpp
std::string format_restart_recovery_evidence_json(const RestartRecoveryEvidence& evidence);
```

返回的 fixture 核心字段是：

```text
evidence_version
read_only
execution_allowed
recovered
digests_match
before
after
snapshot
wal
diagnostics
```

边界字段固定表达为：

```text
read_only=true
execution_allowed=false
diagnostics.order_authoritative=false
diagnostics.write_commands_executed_by_evidence=false
```

这几个字段是给 Node 控制面看的：读到这份样本，只能说明 mini-kv 有可验证的恢复证据，不能因此认为 Node 可以对 mini-kv 执行写命令，也不能把 mini-kv 当成订单权威存储。

## fixture 文件

新增 fixture：

```text
fixtures/recovery/restart-recovery-evidence.json
```

它描述的恢复流程是：

```text
1. Store 写入 recovery:base
2. 保存 snapshot baseline
3. WAL 追加 recovery:delta
4. 模拟重启：新 Store 先 LOAD snapshot，再 replay WAL
5. 比较 before/after key_count 和 digest
```

注意：fixture 是最终证据样本，不是中间日志，也不是运行时临时文件。

配套说明放在：

```text
fixtures/recovery/README.md
```

这里明确说明它是只读 evidence shape，不是新的写权限模型。

## 测试覆盖

新增测试：

```text
tests/recovery_evidence_tests.cpp
```

测试不是只检查字符串字段，而是真跑一遍恢复流程：

```text
创建临时目录
保存 snapshot
追加 WAL
新 Store 加载 snapshot
重放 WAL
生成 RestartRecoveryEvidence
格式化 JSON
和 fixture 完全相等
清理临时目录
```

关键断言包括：

```cpp
assert(replay.applied_records == 1);
assert(replay.skipped_records == 0);
assert(replay.truncated_records == 0);
assert(replay.checksum_failed_records == 0);
assert(evidence.recovered);
assert(evidence.digests_match);
assert(json == fixture);
```

这些断言保护三件事：

```text
WAL replay 没有坏记录
恢复前后 Store 摘要一致
fixture 不会静默漂移
```

## CMake 与 README

`CMakeLists.txt` 版本提升到：

```text
0.55.0
```

并新增库源码和测试目标：

```text
src/recovery_evidence.cpp
minikv_recovery_evidence_tests
```

README 当前版本提升为 Version 55，并补充：

```text
fixtures/recovery/restart-recovery-evidence.json
recovery_evidence_tests
Node v99 后续消费方向
```

## 一句话总结

v55 把 mini-kv 的 WAL replay 和 snapshot baseline 组合成可被 Node 后续消费的稳定恢复证据样本，让三项目融合多了一层“底层存储可恢复性”的只读证据，但仍然保持松耦合、不新增协议、不接管订单数据。
