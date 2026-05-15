# 第七十九版 Operator-Window Identity-Neutral Proof

本版目标是在 v78 已经稳定的 `SMOKEJSON.failure_taxonomy.taxonomy_digest` 上，再补一层 operator-window 的身份中性证明。Node v198 已经开始要求真实只读窗口具备 operator identity binding，后续 Node v200 还要归档 CI artifact manifest；mini-kv 这一侧不能伪造、接收或持久化业务操作者身份，只能明确告诉控制面：我提供的是 runtime evidence，而且是 identity-neutral proof。

先说明它不是什么：这不是新的执行入口，不是认证系统，不是生产授权，不会让 Node 自动启动 mini-kv，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 `RESTORE`，也不把 mini-kv 接进 Java 订单事务链。`operator_window.identity_neutral_proof=true` 的意思只是“这份运行时证据不携带、不生成、不保存业务身份”，不是“操作者已经被 mini-kv 认证”。

## 本版所处阶段

最新跨项目计划是 `v197-post-readiness-checkpoint-roadmap.md`。它把下一阶段从“真实只读 adapter 证据链完整”推进到“生产窗口硬门槛落地”：Node v198 做 operator identity binding，Java v70 和 mini-kv v79 并行补只读身份/消费者提示，随后 Node v199/v200 再接 audit store handoff 和 CI archive manifest。

所以 mini-kv v79 的范围很窄：只增强 `SMOKEJSON` 的 operator-window consumer / identity-neutral proof 字段，并保持旧的 failure taxonomy digest 稳定。

## 1. `src/command.cpp`

新增的数据结构是：

```text
SmokeOperatorWindowProof
```

它只有三个字段：

```text
consumer=Node v200 real-read window CI archive artifact manifest
identity_neutral_proof=true
node_action=verify digest before importing manual window results
```

`format_smoke_operator_window_proof_json()` 负责把这三个字段格式化成 JSON。`format_smoke_json()` 把它挂到顶层：

```text
SMOKEJSON.operator_window
```

控制面读到后应该这样理解：

- `consumer`：这份字段是给 Node v200 归档真实只读窗口证据前看的。
- `identity_neutral_proof`：mini-kv 明确声明自己不接收、不伪造、不持久化操作者身份。
- `node_action`：Node 仍然要先校验 digest，再导入或归档窗口结果。

这里刻意没有改掉：

```text
failure_taxonomy.consumer=Node v196 imported window result packet
taxonomy_digest=fnv1a64:f92fcba55feb26a2
```

原因是 v79 并没有重定义失败分类词表，只是在旁边新增身份中性证明。如果 taxonomy digest 因为新增旁路字段而变化，Node 就无法判断旧的失败分类契约是否仍然稳定。

## 2. `fixtures/release/runtime-smoke-evidence.json`

fixture 升到：

```text
runtime_smoke_evidence_version=mini-kv-runtime-smoke-evidence.v4
project_version=0.79.0
release_version=v79
consumer_hint=Node v200 real-read window CI archive artifact manifest
```

新增的重点是顶层：

```text
operator_window.consumer
operator_window.identity_neutral_proof
operator_window.node_action
```

同时边界写得更明确：`operator-window proof only`、`identity-neutral proof only`、`no operator identity accepted or persisted`。这些字段保护的是 mini-kv 的职责边界，避免 Node 或人读归档时误以为 mini-kv 已经完成真实身份认证。

## 3. `fixtures/release/verification-manifest.json`

release manifest 同步到 v79，并把构建目录、只读 smoke 端口和验收文本改成：

```text
cmake-build-v79
minikv_server 6417 127.0.0.1
SMOKEJSON exposes operator_window.identity_neutral_proof=true for Node v200 archive handoff
```

这里仍然只列 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token` 这一组只读命令。`GET restore:real-read-token` 必须返回 `(nil)`，用来证明 smoke 没有顺手创建 token 或触发 restore 类写入。

## 4. 版本与 README

`CMakeLists.txt` 升到 `0.79.0`，`fixtures/readonly/infojson-empty-inline.json` 也同步到 `0.79.0`。README 的 Current version、命令说明、release manifest、runtime smoke evidence 和 Roadmap 都补了 v79 描述。

这些文档不是单纯改版本号，而是告诉后续维护者：v79 的价值是让 Node v200 能看到 mini-kv 的身份中性运行时证据，而不是让 mini-kv 参与上游业务身份体系。

## 5. 测试保护

`command_tests.cpp` 直接执行 `SMOKEJSON`，断言真实运行时响应里存在：

```text
operator_window.consumer
operator_window.identity_neutral_proof=true
operator_window.node_action
```

`runtime_smoke_evidence_tests.cpp` 同时保护静态 fixture 和真实 `CommandProcessor` 输出，避免只改 JSON 样本却忘了运行时命令。

`release_verification_manifest_tests.cpp` 保护 v79 manifest、`cmake-build-v79`、Node v200 consumer hint、operator-window proof 边界，以及真实 `SMOKEJSON` 输出。

历史 release/restore 相关测试只把当前运行版本断言同步到 `0.79.0`，没有重写它们自身的历史来源版本。

## 6. 真实运行

本轮真实 smoke 使用：

```text
minikv_server 6417 127.0.0.1
minikv_client 127.0.0.1 6417 5000 --connect-retries 10 --retry-delay-ms 100
```

实际读到 `SMOKEJSON` 后确认了 `version:"0.79.0"`、`operator_window.identity_neutral_proof:true`、`taxonomy_digest:"fnv1a64:f92fcba55feb26a2"`，并且 `GET restore:real-read-token` 返回 `(nil)`。这说明 v79 的新增证明可以通过真实 TCP 只读窗口读取，而不是靠执行写命令制造出来。

## 一句话总结

v79 把 mini-kv 的真实只读窗口证据从“可校验失败分类 digest”推进到“可声明身份中性边界”，给 Node v200 后续归档运行时证据提供了更清楚的 C++ 侧边界。
