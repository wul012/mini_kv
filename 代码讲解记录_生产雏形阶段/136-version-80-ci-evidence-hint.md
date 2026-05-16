# 第八十版 CI Evidence Hint

本版目标是在 v79 已经提供 `operator_window.identity_neutral_proof` 的基础上，再给 `INFOJSON` 和 `SMOKEJSON` 补一层只读 CI/evidence hint。跨项目计划 `v200-post-ci-artifact-manifest-roadmap.md` 要求 mini-kv v80 为 Node v201 的 real-read window CI artifact manifest verification 提供 runtime artifact path hint、identity-neutral proof 和 no-restore proof。mini-kv 这一侧只负责把运行时证据讲清楚，不执行上传、不触发 restore，也不成为 Java 订单链路的一部分。

先说它不是什么：这不是新的执行入口，不是 CI artifact upload，不读取 GitHub secret，不改变 CI 权限，不让 Node 自动启动 mini-kv，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 `RESTORE`，也不把 mini-kv 接入 Java 事务一致性链。`ci_evidence.upload_allowed=false` 和 `ci_evidence.no_restore_proof=true` 是控制面消费前的边界提示，不是生产授权。

## 本版所处阶段

当前跨项目主线已经从“真实只读窗口证据归档”推进到“CI artifact manifest 后续验证”。Node v200 已经整理 manifest，下一步 Node v201 会验证 manifest digest、record count、文件类型和生产窗口 blocked 状态。mini-kv v80 的职责很窄：在自身运行时读接口和 release fixture 里声明 artifact path hint 与 no-restore/no-upload 边界，让 Node 能只读校验 C++ 侧证据形状。

## 1. `src/command.cpp`

新增的数据结构是：

```text
RuntimeCiEvidenceHint
```

它集中保存六个字段：

```text
consumer=Node v201 real-read window CI artifact manifest verification
artifact_path_hint=c/80/
identity_neutral_proof=true
no_restore_proof=true
upload_allowed=false
node_action=verify manifest path hints before CI artifact upload dry-run
```

`format_runtime_ci_evidence_hint_json()` 负责把这些字段格式化成 JSON。`format_info_json()` 把它挂到：

```text
INFOJSON.ci_evidence
```

`format_smoke_json()` 把同一个对象挂到：

```text
SMOKEJSON.ci_evidence
```

控制面读到后应这样理解：

- `consumer`：这组字段服务于 Node v201 的 manifest verification，不是给业务写链路使用。
- `artifact_path_hint`：mini-kv 本版运行归档默认在 `c/80/`，Node 可把它作为后续检查路径提示。
- `identity_neutral_proof`：沿用 v79 的边界，mini-kv 不接收、不推断、不持久化业务操作者身份。
- `no_restore_proof`：本版只读证据不执行 restore，也不借 `LOAD`/`COMPACT`/`SETNXEX` 伪造状态。
- `upload_allowed`：明确为 `false`，说明这里不是 GitHub artifact upload 执行开关。

这里没有改 `failure_taxonomy.taxonomy_digest`，它仍然是 `fnv1a64:f92fcba55feb26a2`。原因是本版只是在 SMOKEJSON 旁路新增 CI evidence hint，不改变失败分类词表；digest 稳定能让 Node 继续判断 v77-v79 的失败分类契约没有漂移。

## 2. `fixtures/readonly/infojson-empty-inline.json`

这个 fixture 同步到 `0.80.0`，并在空 store、无 WAL、无 metrics 的基准响应里加入：

```text
ci_evidence.artifact_path_hint=c/80/
ci_evidence.no_restore_proof=true
ci_evidence.upload_allowed=false
```

它保护的是 `INFOJSON` 的稳定样本。后续 Node 或人工回看时，可以直接看到 mini-kv 即使在最小运行状态下，也会输出同一套 CI evidence 边界。

## 3. `fixtures/release/verification-manifest.json`

release manifest 升到：

```text
project_version=0.80.0
release_version=v80
consumer_hint=Node v201 real-read window CI artifact manifest verification
cmake-build-v80
minikv_server 6418 127.0.0.1
```

新增验收点包括：

```text
INFOJSON exposes ci_evidence.artifact_path_hint=c/80/
SMOKEJSON exposes ci_evidence.no_restore_proof=true and upload_allowed=false
ci_artifact_upload_executed=false
```

这份 manifest 的角色不是执行 CI 上传，而是把后续 Node v201 要校验的 mini-kv 侧证据字段列清楚。`no CI artifact upload`、`no-restore proof only`、`does not execute LOAD/COMPACT/SETNXEX` 这些边界继续放在 manifest 里，避免把“可读证据提示”误解成“可以操作生产或恢复数据”。

## 4. `fixtures/release/runtime-smoke-evidence.json`

runtime smoke evidence 升到：

```text
runtime_smoke_evidence_version=mini-kv-runtime-smoke-evidence.v5
project_version=0.80.0
release_version=v80
target_version=Node v201
mode=real-read window CI artifact manifest verification
```

这里仍然使用真实只读命令组：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

重点新增的是 `node_must_verify`：Node 必须在验证 manifest 前确认 taxonomy digest、operator-window identity-neutral proof、`ci_evidence.artifact_path_hint=c/80/` 和 `ci_evidence.no_restore_proof=true`。同时 `node_must_not` 明确禁止 auto-start、restore/load/compact/setnxex 和 CI artifact upload。

## 5. README 和版本号

`CMakeLists.txt` 升到 `0.80.0`。README 的 Current version、命令说明、fixture 说明和 Roadmap 都同步到 v80，突出这版不是新增存储能力，而是补齐 Node v201 消费前需要的 CI/evidence runtime hint。

## 6. 测试保护

`command_tests.cpp` 直接执行 `INFOJSON` 和 `SMOKEJSON`，断言真实响应里存在：

```text
ci_evidence.consumer
ci_evidence.artifact_path_hint=c/80/
ci_evidence.no_restore_proof=true
ci_evidence.upload_allowed=false
```

`readonly_fixture_tests.cpp` 继续把 `INFOJSON` fixture 与真实命令输出做精确对比，因此能保护 JSON 拼接顺序和字段完整性。

`release_verification_manifest_tests.cpp` 和 `runtime_smoke_evidence_tests.cpp` 同时保护静态 fixture 与真实 `CommandProcessor` 输出，避免只改样本、不改运行时，或只改运行时、忘记 release evidence。其他历史 release/restore 测试只同步当前 `INFOJSON` 版本断言到 `0.80.0`，没有重写它们自己的历史来源版本。

## 7. 真实运行

本轮真实 smoke 使用：

```text
minikv_server 6418 127.0.0.1
minikv_client 127.0.0.1 6418 5000 --connect-retries 10 --retry-delay-ms 100
```

需要确认 `SMOKEJSON` 和 `INFOJSON` 都能读到 `ci_evidence`，`SMOKEJSON` 仍然包含稳定 taxonomy digest 与 operator-window proof，`GET restore:real-read-token` 仍返回 `(nil)`。这证明本版新增的是只读提示字段，不是通过写入或 restore 制造出来的状态。

## 一句话总结

v80 把 mini-kv 的真实只读窗口证据从“可证明身份中性”推进到“可给 Node v201 校验 CI artifact manifest 的路径、no-restore、no-upload 边界”，继续保持 C++ 存储核心不乱动、跨项目融合松耦合。
