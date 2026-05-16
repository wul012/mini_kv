# 第八十二版 Live-Read Session Hint 代码讲解

## 本版目标

v82 的目标是按照 `D:\nodeproj\orderops-node\docs\plans\v203-post-ci-artifact-retention-roadmap.md` 的最新指导，在 mini-kv 侧给 `SMOKEJSON` 补充 live-read session hint，方便后续 Node v205 执行三项目真实只读 smoke 前核对会话、运行时间分桶和只读命令列表摘要。

这版不是什么：

- 不是 Node 自动启动 mini-kv 的许可。
- 不是新的写入入口。
- 不执行 `LOAD`、`COMPACT`、`SETNXEX`、`RESTORE`。
- 不读取生产 secret，不打开生产窗口。
- 不把 mini-kv 接入 Java 订单一致性链路。

## 1. 版本入口

`CMakeLists.txt` 把项目版本从 `0.81.0` 提升到 `0.82.0`。

这个版本号会进入生成的 `minikv/version.hpp`，随后由 `INFO`、`INFOJSON`、`SMOKEJSON` 和 release fixture 测试共同验证。也就是说，v82 的版本号既是构建产物身份，也是 Node 后续只读消费时能看到的运行时字段。

## 2. command.cpp 的 live-read session 结构

核心修改在 `src/command.cpp`。

本版新增三段小函数：

```text
read_command_list_digest(...)
uptime_bucket_for_seconds(...)
format_live_read_session_hint_json(...)
```

`read_command_list_digest(...)` 对 Node real-read 计划里的只读命令列表做稳定摘要。输入命令为：

```text
INFOJSON
STORAGEJSON
HEALTH
STATSJSON
```

输出固定为：

```text
fnv1a64:5bef33f2fbe65cc5
```

这个摘要不是认证签名，而是“命令清单有没有漂移”的轻量证据。Node v205 读到后可以先确认它准备执行的只读命令集合和 mini-kv 暴露的集合一致。

`uptime_bucket_for_seconds(...)` 把动态 `uptime_seconds` 收敛成几个稳定分桶：

```text
lt_60s
lt_5m
lt_1h
gte_1h
```

这样控制面可以判断“这是不是一个刚启动的只读窗口”，又不需要依赖精确秒数。精确秒数仍然保留在 `server.uptime_seconds` 里，但测试和跨项目契约优先依赖 bucket。

`format_live_read_session_hint_json(...)` 输出本版新增对象：

```text
live_read_session.consumer
live_read_session.session_id_echo
live_read_session.server_uptime_bucket
live_read_session.read_command_list_digest
live_read_session.read_command_count
live_read_session.read_commands
live_read_session.write_commands_allowed
live_read_session.auto_start_allowed
live_read_session.node_action
```

其中最重要的是：

- `session_id_echo="mini-kv-live-read-v82"`：给 Node v205 做会话回显核对。
- `write_commands_allowed=false`：明确本轮仍然只读。
- `auto_start_allowed=false`：明确 mini-kv 不授权 Node 自动启动服务。

## 3. SMOKEJSON 如何暴露

`format_smoke_json(...)` 在 `real_read` 后追加：

```text
"live_read_session": ...
```

它仍然保留原有的 `operator_window`、`ci_evidence`、`artifact_retention`、`failure_taxonomy` 和 diagnostics。换句话说，v82 是在 v76-v81 累积出来的 runtime smoke evidence 上加一层“真实只读会话核对提示”，不是替换既有证据链。

`diagnostics.node_consumption` 也同步更新为 Node v205 语义：Node 可以验证 live-read session echo、uptime bucket、read command digest、taxonomy digest、operator-window identity-neutral proof、CI evidence hints 和 artifact retention evidence，但前提仍然是 mini-kv 已经由操作方启动，且只读窗口已经打开。

## 4. fixture 与 manifest

本版同步更新了三类证据文件：

- `fixtures/readonly/infojson-empty-inline.json`
  - 把稳定空库 `INFOJSON` 样例升级到 `0.82.0`。
  - 保留 v81 的 `artifact_retention` 字段，说明 retention 证据没有被本版误删。

- `fixtures/release/runtime-smoke-evidence.json`
  - 升级为 `mini-kv-runtime-smoke-evidence.v7` / `v82`。
  - 把目标消费者改为 Node v205 real-read execution packet。
  - 新增 `live_read_session` 对象和 pause condition：session echo 或 read digest 不匹配时暂停。

- `fixtures/release/verification-manifest.json`
  - 将构建目录切到 `cmake-build-v82`，smoke 端口切到 `6420`。
  - 在 read-only smoke expected 中加入 `live_read_session.session_id_echo` 和 `read_command_list_digest`。
  - 继续保留 no upload、no production window、no restore、not order authoritative 等边界。

这些文件仍然是只读证据和契约样本，不是执行计划本身。

## 5. 测试保护了什么

`tests/command_tests.cpp` 直接验证真实 `SMOKEJSON` 输出包含 live-read session 的关键字段，保护运行时响应不漂移。

`tests/runtime_smoke_evidence_tests.cpp` 同时验证 fixture 和真实 `SMOKEJSON`，确保 Node v205 将要读取的字段在静态样本和运行时输出里一致。

`tests/release_verification_manifest_tests.cpp` 保护 manifest 的版本、构建目录、smoke 端口、expected 字段和边界描述，避免文档计划与可执行验证链脱节。

其他 release fixture 测试主要承担版本联动和历史证据链回归，确认 v82 没有破坏 v64-v81 积累的 rollback、restore、artifact digest、CI evidence 和 artifact retention 契约。

## 6. 真实运行证明

本版真实运行了：

```text
minikv_server 6420 127.0.0.1
minikv_client 127.0.0.1 6420 5000 --connect-retries 10 --retry-delay-ms 100
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

- `SMOKEJSON` 真实返回 `live_read_session`。
- 版本为 `0.82.0`。
- `session_id_echo` 为 `mini-kv-live-read-v82`。
- `server_uptime_bucket` 在本轮刚启动窗口中为 `lt_60s`。
- `read_command_list_digest` 为 `fnv1a64:5bef33f2fbe65cc5`。
- `write_commands_allowed=false`、`auto_start_allowed=false`。
- `GET restore:real-read-token` 仍为 `(nil)`，没有写入恢复 token。
- smoke 后 server 已停止。

## 一句话总结

v82 让 mini-kv 在不授权自动启动、不执行写命令、不进入订单链路的前提下，为 Node v205 的三项目真实只读 smoke 提供了可核对的 live-read session 证据。
