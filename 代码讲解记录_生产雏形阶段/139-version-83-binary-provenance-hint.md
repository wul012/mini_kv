# 第八十三版 Binary Provenance Hint 代码讲解

## 本版目标

v83 的目标是按照 `D:\nodeproj\orderops-node\docs\plans\v207-post-hardening-triage-roadmap.md` 的最新指导，在 mini-kv 侧补 runtime artifact path / binary provenance hint，让后续 Node v208 做 managed audit persistence boundary candidate 时，能先核对运行二进制、fixture 和 release evidence 路径是否对齐。

这版不是什么：

- 不是生产二进制认证或签名。
- 不是新的执行入口。
- 不执行 `LOAD`、`COMPACT`、`SETNXEX`、`RESTORE`。
- 不读取生产密钥，不上传 GitHub artifact。
- 不把 mini-kv 接入 Java 订单一致性链路。

## 1. 版本入口

`CMakeLists.txt` 把项目版本从 `0.82.0` 提升到 `0.83.0`。

这个版本号会进入生成的 `minikv/version.hpp`，随后被 `INFOJSON`、`SMOKEJSON`、release manifest、runtime smoke evidence 和测试共同验证。v83 的重点不是增加新命令，而是让已有运行时 evidence 更适合被 Node v208 作为 managed audit 持久化边界的输入。

## 2. command.cpp 的 binary provenance 结构

核心修改在 `src/command.cpp`。

本版新增 `RuntimeBinaryProvenanceHint`，固定表达一组只读字段：

```text
consumer = Node v208 managed audit persistence boundary candidate
source_version = 0.83.0
artifact_path_hint = c/83/
runtime_binary_hint = cmake-build-v83/minikv_server and cmake-build-v83/minikv_client from the current CMake build
release_manifest_path = fixtures/release/verification-manifest.json
runtime_smoke_evidence_path = fixtures/release/runtime-smoke-evidence.json
provenance_digest = fnv1a64:c1c0896fc6b77fe2
read_only = true
load_restore_compact_executed = false
production_binary_claimed = false
```

这些字段的含义是：本轮运行调试证据位于 `c/83/`，当前运行二进制来自 `cmake-build-v83`，并且它与 release manifest、runtime smoke evidence 的路径形成一个可核对的只读线索。它不声称这是生产部署二进制，也不做真实 attest。

`binary_provenance_digest()` 使用已有 FNV-1a 摘要工具，把版本、归档路径、二进制提示、fixture 路径和安全边界布尔值合成稳定摘要。这个摘要不是安全签名，而是防止路径和证据链悄悄漂移的轻量 fingerprint。

`format_runtime_binary_provenance_hint_json()` 负责输出 JSON。它复用现有 `json_string()` 和 `format_json_bool()`，没有引入新的序列化方式。

## 3. INFOJSON 和 SMOKEJSON 如何暴露

`format_info_json(...)` 在 `artifact_retention` 后追加：

```text
"binary_provenance": ...
```

因此 `INFOJSON` 仍然是 runtime identity evidence，只是多了一段给控制面读取的二进制/证据路径对齐提示。

`format_smoke_json(...)` 同样追加 `binary_provenance`，并在 diagnostics notes 里加入：

```text
binary_provenance_hint
```

`diagnostics.node_consumption` 更新为 Node v208 语义：Node 可以验证 binary provenance、live-read session、taxonomy digest、operator-window proof、CI evidence 和 artifact retention，但前提仍然是 mini-kv 已经运行且只读窗口已经打开。

## 4. fixture 与 manifest

本版同步更新三类证据文件：

- `fixtures/readonly/infojson-empty-inline.json`
  - 升级到 `0.83.0`。
  - 增加 `binary_provenance`，保护空库 `INFOJSON` 样例的字段形状。

- `fixtures/release/runtime-smoke-evidence.json`
  - 升级为 `mini-kv-runtime-smoke-evidence.v8` / `v83`。
  - 目标消费者改为 Node v208 managed audit persistence boundary candidate。
  - 增加 `binary_provenance`、pause condition 和 no production binary claim 边界。

- `fixtures/release/verification-manifest.json`
  - 构建目录切到 `cmake-build-v83`，smoke 端口切到 `6421`。
  - 在 read-only smoke expected 中加入 provenance digest 和 `production_binary_claimed=false`。
  - 继续保留 no restore、no upload、no production window、not order authoritative 等边界。

这些文件仍然是证据样本和契约样本，不是执行脚本，也不会触发 restore、compact、upload 或生产切换。

## 5. 测试保护了什么

`tests/command_tests.cpp` 验证真实 `SMOKEJSON` 输出包含 `binary_provenance` 关键字段，保护运行时响应不漂移。

`tests/readonly_fixture_tests.cpp` 把 `INFOJSON` fixture 与真实输出对齐，保护控制面读取 runtime identity 时能看到同样的 binary provenance shape。

`tests/runtime_smoke_evidence_tests.cpp` 同时验证 fixture 和真实 `SMOKEJSON`，确保 Node v208 将要读取的字段在静态样本和运行时输出里一致。

`tests/release_verification_manifest_tests.cpp` 保护 manifest 的版本、构建目录、smoke 端口、expected 字段、binary provenance 边界和 CMake 版本一致性。

## 6. 真实运行证明

本版真实运行了：

```text
minikv_server 6421 127.0.0.1
minikv_client 127.0.0.1 6421 5000 --connect-retries 10 --retry-delay-ms 100
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

- `SMOKEJSON` 和 `INFOJSON` 都真实返回 `binary_provenance`。
- 版本为 `0.83.0`。
- `artifact_path_hint` 为 `c/83/`。
- `provenance_digest` 为 `fnv1a64:c1c0896fc6b77fe2`。
- `load_restore_compact_executed=false`。
- `production_binary_claimed=false`。
- `GET restore:real-read-token` 仍为 `(nil)`，没有写入恢复 token。
- smoke 后 server 已停止。

## 一句话总结

v83 让 mini-kv 在不做生产 attest、不执行恢复/压缩/写命令的前提下，为 Node v208 的 managed audit 持久化边界提供了可核对的运行二进制与证据路径对齐提示。
