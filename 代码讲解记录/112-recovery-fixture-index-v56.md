# 第一百一十二次讲解：第五十六版 recovery fixture index

本版目标是给 mini-kv 的恢复证据补一个稳定索引。

它不是新的服务端命令，不是新的 HTTP endpoint，也不是新的写入能力。它的角色是把 v55 已经生成的 restart recovery evidence fixture，用一个更容易被 Node 控制面发现和校验的 `index.json` 包起来：控制面先读索引，知道有哪些恢复证据、路径在哪里、digest 是什么、边界是什么，再决定是否读取具体 fixture。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v99-production-readiness-v2-roadmap.md
```

计划里 mini-kv v56 的目标是：

```text
补 recovery evidence 的说明 endpoint / fixture index
```

mini-kv 当前没有 HTTP endpoint 层，因此本版选择更符合项目边界的 fixture index，而不是新增协议命令。

## 索引入口

新增文件：

```text
fixtures/recovery/index.json
```

它是下游控制面读取恢复证据的入口。最外层字段是：

```text
index_version
read_only
execution_allowed
order_authoritative
fixtures
diagnostics
```

几个硬边界字段是：

```text
read_only=true
execution_allowed=false
order_authoritative=false
```

这三个字段的意思是：这个索引只能被当作 evidence 目录，不能被当作执行许可，也不能让 Node 把 mini-kv 视为订单权威数据源。

## fixture 条目

当前索引只有一个 fixture：

```text
restart-recovery-evidence
```

条目里记录：

```text
path=fixtures/recovery/restart-recovery-evidence.json
evidence_version=mini-kv-restart-recovery.v1
recovered=true
digests_match=true
digest=fnv1a64:0991cf4e41a8005a
consumer_hint=Node v103 production readiness summary v3
```

对 Node 来说，这比直接硬编码路径更稳：后续如果 mini-kv 增加更多恢复样本，可以继续往 `fixtures` 数组里追加，而不需要 Node 先猜目录结构。

## 边界说明

索引条目里还有 `boundaries`：

```text
read-only fixture
no server command added
no write command expanded
not order authoritative
```

这些不是装饰文案，而是给控制面消费时的安全提示：

- `read-only fixture`：只能读文件，不能触发恢复动作。
- `no server command added`：mini-kv 没有新增运行时命令，避免扩大协议面。
- `no write command expanded`：没有新增或放宽 `SET` / `DEL` / `EXPIRE`。
- `not order authoritative`：这只是 KV 恢复能力证据，不是订单一致性承诺。

## 配套 README

更新文件：

```text
fixtures/recovery/README.md
```

README 现在先解释 `index.json`，再解释具体 `restart-recovery-evidence.json`。

这样后续维护时能看清两层关系：

```text
index.json -> 证据目录和边界
restart-recovery-evidence.json -> 具体恢复样本
```

## 测试覆盖

新增测试：

```text
tests/recovery_fixture_index_tests.cpp
```

测试读取两个文件：

```text
fixtures/recovery/index.json
fixtures/recovery/restart-recovery-evidence.json
```

核心断言包括：

```cpp
assert_contains(index, "\"fixture_count\":1");
assert_contains(index, "\"path\":\"fixtures/recovery/restart-recovery-evidence.json\"");
assert_contains(index, "\"digest\":\"fnv1a64:0991cf4e41a8005a\"");
assert_contains(index, "\"consumer_hint\":\"Node v103 production readiness summary v3\"");
assert_contains(evidence, "\"evidence_version\":\"mini-kv-restart-recovery.v1\"");
assert_contains(evidence, "\"recovered\":true");
```

这些断言保护三件事：

```text
索引仍然指向真实存在的 fixture
索引里的 digest 和 fixture 保持一致
下游 Node v103 的消费提示没有丢
```

测试还检查 `restart-recovery-evidence` 只出现一次，避免索引重复登记同一个样本。

## CMake 与 README

`CMakeLists.txt` 版本提升到：

```text
0.56.0
```

新增测试目标：

```text
minikv_recovery_fixture_index_tests
```

README 当前版本提升为 Version 56，并补充：

```text
fixtures/recovery/index.json
recovery_fixture_index_tests
Node v103 后续消费方向
```

## 一句话总结

v56 没有扩大 mini-kv 的运行时能力，而是把 v55 的恢复证据整理成可发现、可校验、可被 Node v103 消费的 fixture index，让跨项目融合继续走“只读证据优先、强耦合靠后”的路线。
