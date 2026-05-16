# 第八十一版 Artifact Retention Evidence 代码讲解

## 本版目标

v81 的目标是按照 `D:\nodeproj\orderops-node\docs\plans\v200-post-ci-artifact-manifest-roadmap.md` 的最新指导，在 mini-kv 侧补齐只读 artifact retention / release evidence 字段，让后续 Node v203 能检查三项目 CI artifact retention gate 的一致性。

这版不是什么：

- 不是 GitHub artifact 上传实现。
- 不读取 GitHub secret，也不修改 CI 权限。
- 不打开生产窗口，不触碰 `UPSTREAM_ACTIONS_ENABLED`。
- 不执行 `LOAD`、`COMPACT`、`SETNXEX`、`RESTORE`。
- 不把 mini-kv 变成 Java 订单权威存储。

## 1. 版本入口

`CMakeLists.txt` 把项目版本从 `0.80.0` 提升到 `0.81.0`。

这个版本号会进入生成的 `minikv/version.hpp`，随后被 `INFO`、`INFOJSON`、`SMOKEJSON` 和各类测试读取。也就是说，v81 的版本变化不是文档里的文字，而是运行时真实输出的一部分。

## 2. command.cpp 的只读证据结构

核心修改在 `src/command.cpp`。

本版新增了 `RuntimeArtifactRetentionEvidence`，固定表达一组给 Node v203 读取的只读字段：

```text
consumer = Node v203 cross-project CI artifact retention gate
artifact_root = c/
artifact_path_hint = c/81/
retention_days = 30
retention_mode = dry-run-contract-only
release_evidence_ready = true
github_artifact_upload_attempted = false
production_window_allowed = false
```

这些字段的含义是：mini-kv 已经把本版运行调试证据放在 `c/81/`，后续控制面可以检查留存天数和归档路径，但不能把它理解成真实上传、真实生产窗口或真实授权。

`format_runtime_artifact_retention_evidence_json()` 负责把这个结构格式化成 JSON。它复用已有的 `json_string()` 和 `format_json_bool()`，没有引入新的 JSON 拼接风格，避免在当前文件里出现第二套输出规则。

## 3. INFOJSON 如何暴露

`format_info_json(...)` 在原有 `ci_evidence` 后追加：

```text
"artifact_retention": ...
```

因此 `INFOJSON` 继续保持 `runtime_identity` 的角色，只是多了一段给控制面读取的 retention hint。控制面读到它时应理解为：

- `read_only=true`：这条命令只是读身份和边界。
- `execution_allowed=false`：不是执行入口。
- `order_authoritative=false`：不能当订单事实来源。
- `github_artifact_upload_attempted=false`：本轮没有真实上传。
- `production_window_allowed=false`：不能据此打开生产窗口。

## 4. SMOKEJSON 如何暴露

`format_smoke_json(...)` 同样在 `ci_evidence` 后追加 `artifact_retention`，并把 `diagnostics.node_consumption` 更新为 Node v203 语义：

Node 可以验证 taxonomy digest、operator-window identity-neutral proof、CI evidence hints 和 artifact retention evidence，但前提仍然是 mini-kv 已经运行且只读窗口已经打开。

这点很重要：`SMOKEJSON` 是 read adapter target，不是 execution gate。它告诉 Node “可以读哪些证据、必须避开哪些危险动作”，但不授予执行权限。

## 5. fixture 与 manifest

本版同步更新了三类 fixture：

- `fixtures/readonly/infojson-empty-inline.json`
  - 把稳定空库 `INFOJSON` 样例升级到 `0.81.0`。
  - 增加 `artifact_retention`，用于保护空库身份响应的字段形状。

- `fixtures/release/runtime-smoke-evidence.json`
  - 升级为 `mini-kv-runtime-smoke-evidence.v6` / `v81`。
  - 把目标消费者改为 Node v203 retention gate。
  - 明确 `artifact_root="c/"`、`artifact_path_hint="c/81/"`、`retention_days=30`、未上传、未开生产窗口。

- `fixtures/release/verification-manifest.json`
  - 将构建目录和 smoke 端口切到 v81。
  - 在 read-only smoke expected 中加入 `artifact_retention` 检查项。
  - 增加 `production_window_opened=false` 和 retention dry-run 边界。

这些 fixture 的定位仍然是“证据样本”和“契约样本”，不是执行脚本，也不会触发 restore、compact 或 CI upload。

## 6. 测试保护了什么

`tests/command_tests.cpp` 直接验证真实 `INFOJSON` 和 `SMOKEJSON` 输出包含 `artifact_retention` 的关键字段，保护运行时响应不漂移。

`tests/readonly_fixture_tests.cpp` 把空库 `INFOJSON` fixture 与真实输出对齐，保护 release review 读取的稳定样例。

`tests/runtime_smoke_evidence_tests.cpp` 保护 v81 runtime smoke evidence 的 Node v203 消费语义，包括：

- retention root/path/day 是否明确。
- 是否保持 `github_artifact_upload_attempted=false`。
- 是否保持 `production_window_allowed=false`。
- Node 是否禁止 auto-start、upload、production window 和写命令。

`tests/release_verification_manifest_tests.cpp` 保护 manifest、fixture inventory、运行时 `INFOJSON/SMOKEJSON` 和 CMake 版本一致性。本版顺手保留了 `assert_contains` 的缺失文本输出，以后 fixture 断言失败时能直接看到缺的是哪一段，不必盲猜。

## 7. 真实运行证明

本版真实运行了：

```text
minikv_server 6419 127.0.0.1
minikv_client 127.0.0.1 6419 5000 --connect-retries 10 --retry-delay-ms 100
```

客户端执行：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

结果证明：

- `SMOKEJSON` 和 `INFOJSON` 都真实返回 `artifact_retention`。
- 版本为 `0.81.0`。
- `ci_evidence.artifact_path_hint` 仍保留 `c/80/`，表示 v80 的 CI evidence hint 没被误改。
- `artifact_retention.artifact_path_hint` 为 `c/81/`，表示 v81 的归档位置单独表达。
- `GET restore:real-read-token` 仍为 `(nil)`，没有写入恢复 token。
- smoke 后 server 已停止。

## 一句话总结

v81 让 mini-kv 在不执行上传、不打开生产窗口、不突破只读边界的前提下，给 Node v203 提供了可验证的 artifact retention 证据入口，继续推进三项目松耦合证据链。
