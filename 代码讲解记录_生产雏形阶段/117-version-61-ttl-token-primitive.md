# 第一百一十七次讲解：第六十一版短 TTL Token Primitive

本版目标是按最新跨项目计划进入“订单幂等 / 短 TTL token / 控制面审计”这个纵向切片的 mini-kv 侧。v60 已经把只读 evidence 解释清楚，v61 开始补一个真正可运行的短 TTL token 基础能力，让后续 Node v160 能把 Java v52 的幂等边界和 mini-kv v61 的 token 能力放在同一个 readiness review 里看。

它不是 Java 订单系统的权威幂等实现，不接入 Java 交易主链路，不改变支付、库存或订单一致性语义，也不让 Node 自动触发上游写操作。它只是 mini-kv 内部的 TTL token primitive：能原子 claim 一个短期 key，能拒绝重复 claim，过期后能再次 claim，并能用 WAL 恢复保留未过期 token、丢弃已过期 token。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v159-post-release-evidence-review-roadmap.md
```

计划要求当前轮到：

```text
推荐并行：Java v52 + mini-kv v61
mini-kv v61 做 ttl token primitive
补 SETEX/TTL/过期清理测试或等价 TTL token 能力
明确 order_authoritative=false
不进入 Java 订单权威链路
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 现在从“只读证据上游”继续往“纵向能力实验位”推进。这个阶段的关键不是把 mini-kv 变成生产 Redis，也不是直接接进订单系统，而是让它拥有一个可以被控制面审查、可以被 Java 侧未来 adapter 候选引用、可以独立验证的 KV 能力。

v61 选择 `SETNXEX`，而不是只做 `SET` + `EXPIRE` 文档组合，是因为短 TTL token 最核心的边界是原子占位：

```text
第一次 claim -> 1
重复 claim -> 0
过期后 claim -> 1
```

这比“先 SET 再 EXPIRE”更接近幂等 token 语义，也避免中间状态让 token 没有 TTL。

## Store 层

改动文件：

```text
include/minikv/store.hpp
src/store.cpp
```

新增接口：

```text
set_if_absent(key, value, expires_at)
```

它做三件事：

```text
key 为空 -> false
key 存在且未过期 -> false
key 不存在或已过期 -> 写入 value 和 expires_at，返回 true
```

这个接口在 Store mutex 内完成检查和写入，所以无 WAL 模式下也有原子 absent-claim 语义。它会在发现旧 entry 已过期时先清掉旧 entry，再允许新 token 写入。

测试位置：

```text
tests/store_tests.cpp
```

新增断言保护：

```text
第一次 set_if_absent("token") 成功
重复 set_if_absent("token") 失败
过期后 get("token") 为空
过期后 set_if_absent("token") 再次成功
```

## 命令层

改动文件：

```text
src/command.cpp
```

新增命令：

```text
SETNXEX key seconds value
```

响应语义：

```text
1 -> token claim 成功
0 -> key 仍然 live，重复 claim 被拒绝
ERR usage: SETNXEX key seconds value -> 参数错误或 TTL 非正数
```

执行流程：

```text
解析 key / seconds / value
seconds 必须是正整数
计算 expires_at = now + seconds
无 WAL：调用 store.set_if_absent(...)
有 WAL：先确认 key 不存在或已过期，再追加 SETEXAT 记录，再调用 store.set_if_absent(...)
```

这里的 WAL 顺序是有意设计的：成功 claim 需要先写 WAL，再改内存，延续已有 `SET` / `DEL` / `EXPIRE` 的 durable-before-apply 风格。重复 claim 返回 `0` 时不追加 WAL，因为状态没有变化。

## WAL 层

改动文件：

```text
src/wal.cpp
```

新增内部 WAL replay 记录：

```text
SETEXAT key epoch_millis value
```

为什么不用两条记录：

```text
SET key value
EXPIREAT key epoch_millis
```

因为 `SETNXEX` 是一个原子 token claim。用一条 `SETEXAT` 可以让 WAL 中的成功 claim 不出现“value 已写但 TTL 没写”的中间证据。compact 仍然可以把当前 live store 重写成已有的 `SET` + `EXPIREAT` 形式，因为 compact 已经是在稳定快照上重建 WAL。

replay 规则：

```text
SETEXAT 未过期 -> set value，再 expire_at
SETEXAT 已过期 -> erase key
格式错误 -> skipped
```

这保证重启恢复不会把过期 token 恢复成有效 token。v62 会继续补更系统的 TTL recovery evidence，本版先把 primitive 的 WAL record 和基本 replay 链路打通。

## 命令解释与契约

`COMMANDS` / `COMMANDSJSON` 现在包含：

```text
SETNXEX(category=write, mutates_store=yes, touches_wal=yes, stable=yes)
```

`EXPLAINJSON SETNXEX orderops:token 30 value` 返回：

```text
command=SETNXEX
category=write
mutates_store=true
touches_wal=true
requires_value=true
ttl_sensitive=true
side_effects=["store_write","store_ttl_update","wal_append_when_enabled"]
side_effect_count=3
```

控制面读到后要这样理解：

```text
SETNXEX 是写命令
它会触碰 store 和 TTL
WAL 开启时会追加 WAL
它适合 token primitive，不适合当订单权威状态
```

`CHECKJSON SETNXEX ...` 仍然只读。它可以解释 SETNXEX 的写风险和 WAL durability，但不会真正执行 SETNXEX。

## TTL Token Evidence

新增文件：

```text
fixtures/ttl-token/index.json
```

它给 Node v160 一个稳定证据入口，内容包括：

```text
capability=ttl_token_primitive
command=SETNXEX key seconds value
success_response=1
duplicate_response=0
expired_token_can_be_claimed_again=true
atomic_absent_claim=true
wal.enabled_record=SETEXAT key epoch_millis value
wal.single_record_claim=true
wal.replay_drops_expired_token=true
order_authoritative=false
```

边界写得很直接：

```text
ttl token primitive only
not an order-authoritative store
not connected to Java transaction chain
does not change payment or inventory semantics
production idempotency ownership remains outside mini-kv
```

这份文件不是只读 capture response，也不是生产 pass；它是 v61 这个能力的解释性 evidence。

## 客户端体验

改动文件：

```text
src/line_editor.cpp
src/client_main.cpp
```

`line_editor` 增加 `SETNXEX` 命令补全，并让 `SETNXEX` 的第二个 token 使用 key 补全。也就是说交互式客户端里：

```text
SETN<Tab> -> SETNXEX
SETNXEX na<Tab> -> SETNXEX name
```

`client_main` 在收到 `SETNXEX ... -> 1` 时会把 key 加入本地 key cache；收到 `0` 时不更新，因为重复 claim 没有改变 key 集合。

## 测试

更新测试：

```text
tests/store_tests.cpp
tests/command_tests.cpp
tests/wal_tests.cpp
tests/line_editor_tests.cpp
```

`store_tests` 保护底层 absent-claim 和过期后 reclaim。

`command_tests` 保护：

```text
SETNXEX 第一次返回 1
重复返回 0
GET 能读到 token value
TTL 返回 0..1
过期后 GET 返回 nil
过期后再次 SETNXEX 返回 1
usage 错误被拒绝
COMMANDS / COMMANDSJSON 包含 SETNXEX
EXPLAINJSON / CHECKJSON 正确解释 side effects
fixtures/ttl-token/index.json 边界字段存在
```

`wal_tests` 保护：

```text
SETNXEX 成功 claim 会写 WAL
WAL replay 能恢复未过期 token
WAL replay 能保留 token TTL
```

`line_editor_tests` 保护命令补全和 key 补全。

## 真实运行

本版真实 smoke 运行了 mini-kv server，并发送：

```text
SETNXEX idem:token 2 claimed
SETNXEX idem:token 2 duplicate
GET idem:token
TTL idem:token
CHECKJSON SETNXEX idem:token 2 claimed
HEALTH
QUIT
```

等待 token 过期后再次发送：

```text
SETNXEX idem:token 1 reclaimed
GET idem:token
TTL idem:token
QUIT
```

验证结果：

```text
第一次 claim -> 1
重复 claim -> 0
GET -> claimed
过期后 reclaim -> 1
GET -> reclaimed
WAL 中出现 SETEXAT 记录
```

本轮 smoke 只运行 mini-kv，不启动 Java，不启动 Node，不接入订单链路。

## 归档

运行归档位置：

```text
b/61/图片/
b/61/解释/说明.md
```

## 一句话总结

v61 让 mini-kv 从只读 evidence 上游推进到拥有可验证的短 TTL token primitive，为后续 Node v160 的幂等纵向 readiness review 提供一个明确、受边界约束、非订单权威的 C++ 能力证据。
