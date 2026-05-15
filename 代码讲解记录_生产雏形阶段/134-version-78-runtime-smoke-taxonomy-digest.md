# 第七十八版 Runtime Smoke Taxonomy Digest

本版目标是在 v77 `SMOKEJSON.failure_taxonomy` 上补一层稳定摘要和 verification sample。Node v196 后续要导入人工真实只读窗口结果，导入前需要知道这份结果是不是来自预期版本的 mini-kv taxonomy；因此 mini-kv 这边给运行时 `SMOKEJSON` 和静态 fixture 都加上同一个 `taxonomy_digest`。

它不是什么要先说清楚：这不是新的执行入口，不会让 Node 自动启动 mini-kv，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 restore，不写 WAL，不读生产密钥，也不让 mini-kv 进入 Java 订单事务链。`taxonomy_digest` 是证据指纹，不是认证签名，也不是生产授权。

## 本版所处阶段

跨项目计划 `v194-post-real-read-archive-roadmap.md` 指出：Node v195 已完成 real-read adapter archive verification，下一步推荐 Java v69 和 mini-kv v78 并行补只读 verification hint。mini-kv 的任务很窄：增强 `SMOKEJSON / runtime-smoke taxonomy` 的 digest 或 verification sample，让 Node v196 导入窗口结果时先做来源版本匹配。

## 1. `src/command.cpp`

v77 的 taxonomy 条目原本直接写在 `format_smoke_failure_taxonomy_json()` 里。本版把条目提升成固定数据源：

```text
smoke_failure_taxonomy_entries
```

这样同一份数据同时服务两个输出：

```text
format_smoke_failure_taxonomy_json()
smoke_failure_taxonomy_digest()
```

`smoke_failure_taxonomy_digest()` 使用现有 `fnv1a64` 和 `append_digest_part()`。它把 taxonomy schema、消费目标、条目数量、每个条目的 `id/source/meaning/node_action/retryable/safe_to_auto_start/write_risk` 串起来后生成：

```text
fnv1a64:f92fcba55feb26a2
```

这个摘要刻意不包含 `uptime_seconds`、命令计数、连接数或 live key 数量，因为这些是运行时动态字段。如果 digest 被动态字段影响，Node 每次导入窗口结果都会读到不同值，就失去“版本匹配”的意义。

`format_smoke_failure_taxonomy_json()` 现在输出：

```text
consumer=Node v196 imported window result packet
taxonomy_digest=fnv1a64:f92fcba55feb26a2
verification_sample.sample_version=mini-kv-smoke-taxonomy-verification.v1
verification_sample.source_command=SMOKEJSON
verification_sample.source_version=0.78.0
verification_sample.expected_taxonomy_digest=fnv1a64:f92fcba55feb26a2
```

控制面读到后的理解是：可以用 `expected_taxonomy_digest` 对比真实 `failure_taxonomy.taxonomy_digest`，确认 taxonomy 合约没有漂移；但仍不能据此执行写命令或启动服务。

## 2. `fixtures/release/runtime-smoke-evidence.json`

fixture 升到：

```text
runtime_smoke_evidence_version=mini-kv-runtime-smoke-evidence.v3
project_version=0.78.0
release_version=v78
consumer_hint=Node v196 imported window result packet
```

新增的关键字段是：

```text
node_must_verify=failure_taxonomy.taxonomy_digest equals fnv1a64:f92fcba55feb26a2 before importing manual window results
failure_taxonomy.taxonomy_digest=fnv1a64:f92fcba55feb26a2
failure_taxonomy.verification_sample
```

这里的边界也写得更明确：`taxonomy digest sample only`，`does not execute LOAD/COMPACT/SETNXEX`，`does not perform restore`。如果 Node 侧发现 digest 缺失或不匹配，就应该暂停导入，而不是把结果当作生产窗口通过。

## 3. `fixtures/release/verification-manifest.json`

manifest 升到 v78，构建目录更新为：

```text
cmake-build-v78
```

read-only smoke 仍然保持原来的命令组合：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

变化点是验收期望从“有失败分类”推进到“有可校验摘要”：

```text
SMOKEJSON exposes taxonomy_digest fnv1a64:f92fcba55feb26a2 for Node v196 imported-window verification
```

这说明本版没有扩大运行面，只是让同一份只读运行证据更容易被下游校验。

## 4. 测试保护

`command_tests.cpp` 直接执行 `SMOKEJSON`，保护运行时 JSON 包含：

```text
consumer=Node v196 imported window result packet
taxonomy_digest=fnv1a64:f92fcba55feb26a2
verification_sample.sample_version=mini-kv-smoke-taxonomy-verification.v1
source_command=SMOKEJSON
source_version=0.78.0
expected_taxonomy_digest=fnv1a64:f92fcba55feb26a2
```

`runtime_smoke_evidence_tests.cpp` 同时保护静态 fixture 和真实 `CommandProcessor` 输出，避免只更新文档却忘了运行时命令。

`release_verification_manifest_tests.cpp` 保护 v78 manifest、`cmake-build-v78` 命令、`taxonomy digest sample only` 边界和真实 `SMOKEJSON` digest 字段。

老的 release 测试里对当前运行版本的断言同步升到 `0.78.0`，但历史 fixture 自己的 source release 信息没有改。

## 5. 真实运行

本轮真实 smoke 启动：

```text
minikv_server 6416 127.0.0.1
minikv_client 127.0.0.1 6416 5000 --connect-retries 10 --retry-delay-ms 100
```

实际读到 `SMOKEJSON` 后可以看到 `version:"0.78.0"`、`failure_taxonomy.consumer:"Node v196 imported window result packet"`、`taxonomy_digest:"fnv1a64:f92fcba55feb26a2"`，并且 `GET restore:real-read-token` 返回 `(nil)`。这证明 digest 是运行时可读的只读证据，不是通过写 token 或执行 restore 得到的结果。

## 一句话总结

v78 把 mini-kv 的 real-read failure taxonomy 从“有稳定分类词表”推进到“有可校验指纹和样例”，为 Node v196 导入人工真实只读窗口结果前做来源版本匹配打好 C++ 侧证据边界。
