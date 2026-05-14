# 第一百一十八次讲解：第六十二版 TTL Token Recovery Evidence

本版目标是按最新跨项目计划继续推进 mini-kv 侧的“订单幂等 / 短 TTL token / 控制面审计”纵向切片。v61 已经新增 `SETNXEX`，证明 mini-kv 可以做短 TTL token 的原子占位；v62 不继续扩张新命令，而是补齐这个 token primitive 在 WAL、Snapshot、restart replay 和过期清理之间的恢复证据。

它不是新的执行入口，不是 Java 订单幂等的权威实现，不把 mini-kv 接入 Java 交易一致性链路，不改变支付、库存或订单状态，也不让 Node 自动发起上游写操作。它只是给后续 Node v161 的 controlled idempotency drill runbook 一个 C++ 侧证据包：mini-kv 的短 TTL token 在恢复过程中哪些情况能恢复、哪些情况必须被丢弃。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v159-post-release-evidence-review-roadmap.md
```

计划当前推进到：

```text
推荐并行：Java v53 + mini-kv v62
mini-kv v62 做 ttl recovery evidence
补 TTL 与 WAL/Snapshot/重启恢复之间的证据
明确过期 token 不能恢复成有效订单幂等状态
保持 order_authoritative=false
不进入 Java 交易一致性链路
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 现在不是单纯“会不会 SET/GET”的练习项目，而是在补一条更像生产系统会关心的证据链：

```text
token 被 claim
 -> token 带 TTL
 -> WAL 能恢复未过期 token
 -> Snapshot 能保留未过期 token
 -> 过期 token 不能在重启后复活
 -> 这些边界能被 Node 控制面只读消费
```

这正好衔接 v61。v61 回答“能不能原子 claim 短 TTL token”，v62 回答“claim 之后遇到 WAL、Snapshot、restart 和 compact 时，token 还是否可信”。

## 新增测试入口

改动文件：

```text
CMakeLists.txt
tests/ttl_token_recovery_tests.cpp
```

新增测试目标：

```text
minikv_ttl_token_recovery_tests
```

这个测试不是泛泛再跑一次 `SETNXEX`，而是专门覆盖恢复链路：

```text
snapshot baseline + WAL delta restart
未过期 SETEXAT replay
已过期 SETEXAT replay
已过期 snapshot entry load
compact 后 SET + EXPIREAT replay
fixtures/ttl-token/recovery-evidence.json 契约字段
```

它让 v62 的证据不是只靠 README 说明，而是有可执行断言保护。

## WAL 恢复语义

v61 的 `SETNXEX` 成功 claim 会写入：

```text
SETEXAT key epoch_millis value
```

v62 通过测试确认这条记录的两个方向：

```text
未过期 -> replay 后 key 存在，value 恢复，TTL 仍存在
已过期 -> replay 被视为已应用但不产生 live key
```

这里的关键不是 `applied_records` 数字，而是“过期 token 不会恢复成有效 token”。控制面或后续 Java adapter 候选读到这条边界时，应该理解为：

```text
mini-kv 可以作为短期 token candidate
但是过期 token 不具备订单幂等有效性
恢复后的 live token 也仍然不是订单权威状态
```

## Snapshot 恢复语义

测试还手工构造了一个带过期时间的 snapshot entry：

```text
ENTRY <past_epoch_millis> "token:snapshot-expired" "old"
```

加载结果是：

```text
SnapshotFile::load 返回成功
loaded 计数说明文件格式被读到
Store restore 时丢弃过期 entry
最终 store.size() == 0
```

这证明 snapshot load 不是盲目把文件里的 token 放回内存，而是会按当前时间过滤过期 entry。对控制面来说，这个差别很重要：snapshot 是恢复基线，不是让旧 token 复活的豁免通道。

## Restart 场景

测试中的 restart 模型是：

```text
先 SETNXEX token:snapshot
保存 snapshot baseline
再 SETNXEX token:wal
新 Store 先 load snapshot
再 replay WAL
```

恢复后：

```text
token:snapshot 存在，并且 TTL 存在
token:wal 存在，并且 TTL 存在
WAL replay 没有 skipped / truncated / checksum failed
```

这说明 mini-kv 可以表达“snapshot baseline + WAL delta”的恢复形态。它仍然是 mini-kv 内部恢复证据，不代表 Java 订单事务会依赖它。

## Compact 场景

本版也覆盖了一个容易漏掉的边界：`SETNXEX` 原始 WAL 记录是 `SETEXAT`，但 compact 后 live token 会被重写成：

```text
SET key value
EXPIREAT key epoch_millis
```

测试确认 compact 后 replay 仍然能恢复 value 和 TTL。这说明：

```text
SETEXAT 是 claim 时的原子 WAL 记录
SET + EXPIREAT 是稳定快照 compact 后的表示
两者恢复后都必须保持 TTL 边界
```

## Fixture 证据

新增文件：

```text
fixtures/ttl-token/recovery-evidence.json
```

更新文件：

```text
fixtures/ttl-token/index.json
```

`recovery-evidence.json` 说明：

```text
capability=ttl_token_recovery
source_primitive=SETNXEX key seconds value
read_only=true
execution_allowed=false
order_authoritative=false
claim_record=SETEXAT key epoch_millis value
replay_restores_unexpired_token=true
replay_drops_expired_token=true
snapshot_load_drops_expired_token=true
restart_merges_snapshot_and_wal_tokens=true
```

这里 `read_only=true` 指的是 evidence 文件本身是只读证据，不代表 `SETNXEX` 是读命令。`SETNXEX` 仍然是写命令，控制面如果只需要检查风险，应使用 `CHECKJSON SETNXEX ...` 读取契约，而不是执行写操作。

`index.json` 追加了：

```text
recovery_evidence_path=fixtures/ttl-token/recovery-evidence.json
wal.replay_restores_unexpired_token=true
snapshot.snapshot_load_drops_expired_token=true
restart.restart_merges_snapshot_and_wal_tokens=true
```

这让 Node v161 不需要猜文件位置，也不需要解析 README 才能找到 mini-kv v62 的恢复证据。

## README 与版本

更新文件：

```text
CMakeLists.txt
README.md
fixtures/readonly/infojson-empty-inline.json
```

版本从：

```text
0.61.0
```

推进到：

```text
0.62.0
```

README 明确 v62 是“short-TTL token primitive with recovery evidence”，并把 `fixtures/ttl-token/recovery-evidence.json` 写入当前能力说明、fixture 说明、WAL/Snapshot/restart 解释和 Roadmap。

`infojson-empty-inline.json` 同步到 `0.62.0`，保证只读 fixture 与构建版本一致。

## 真实运行

本版真实 smoke 使用 server + client + WAL + snapshot：

```text
SETNXEX token:restart 10 from-wal
SAVE tmp-v62-logs\v62-token-recovery.snap
SETNXEX token:delta 10 from-wal-delta
SETNXEX token:short 1 short
WALINFO
QUIT
```

随后停止 server，等待短 token 过期，再用同一 WAL 重启：

```text
GET token:restart -> from-wal
GET token:delta -> from-wal-delta
GET token:short -> (nil)
SETNXEX token:short 10 reclaimed -> 1
LOAD tmp-v62-logs\v62-token-recovery.snap -> OK loaded 1
GET token:restart -> from-wal
GET token:delta -> (nil)
TTL token:restart -> 正数
HEALTH -> OK
```

这个 smoke 证明了：

```text
WAL 重启能恢复未过期 token
过期 token 不会在重启后复活
过期后可以重新 claim
手动 LOAD snapshot 后 snapshot 内 token 仍带 TTL
LOAD 是显式状态替换，snapshot 外的 WAL delta 不再保留在当前内存状态
```

本轮只运行 mini-kv，不启动 Java，不启动 Node。

## 测试

本版执行：

```text
cmake configure
cmake build
ctest
minikv_ttl_token_recovery_tests
minikv_wal_tests
real server ttl recovery smoke
```

CTest 结果：

```text
100% tests passed, 0 tests failed out of 17
```

专项测试保护：

```text
TTL token recovery fixture 字段不漂移
SETEXAT replay 能恢复未过期 token
SETEXAT replay 不复活过期 token
Snapshot load 不复活过期 token
Snapshot + WAL restart 能合并恢复 token
Compact 后 SET + EXPIREAT 仍能恢复 TTL
```

## 边界

本版继续保持：

```text
order_authoritative=false
execution_allowed=false 只适用于 evidence 文件
SETNXEX 本身仍是写命令
不修改 Node
不修改 Java
不启动 Node 或 Java
不进入 Java 订单交易一致性链路
不定义生产幂等归属、备份、留存或多租户策略
```

如果后续 Java v53 做 `IdempotencyStore` 抽象，mini-kv 也仍然只能作为 disabled candidate 或演练候选，而不是订单权威存储。

## 一句话总结

v62 让 mini-kv 的短 TTL token 从“能 claim”推进到“恢复边界可证明”，为后续 Node v161 把 Java 幂等抽象和 mini-kv TTL recovery evidence 合成 controlled drill runbook 打下了更可靠的 C++ 侧证据基础。
