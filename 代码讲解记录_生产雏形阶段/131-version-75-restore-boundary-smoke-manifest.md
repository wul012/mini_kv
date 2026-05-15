# 第七十五版 Restore Boundary Smoke Manifest 代码讲解

本版目标是按 `v182-post-rehearsal-quality-roadmap.md` 的最新指导，给 mini-kv 增加一份 restore boundary read-only smoke manifest，让后续 Node v185 的 real-read rehearsal intake 能读取更接近真实运行的 mini-kv 侧证据。

这不是新的恢复执行入口，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 restore，不读取生产密钥，不启动 Node/Java，也不把 mini-kv 接进 Java 订单一致性链路。mini-kv 仍然只是只读 smoke 命令清单、runtime target、client target 和 no-write proof 的证据提供方。

## 1. 本版所处阶段

v74 已经把 restore approval boundary 做成 Node v182 可消费的审批演练证据。最新计划说明 Node v183-v184 已经先处理 Node 侧实际技术债和边界测试，下一步推荐 Java v66 与 mini-kv v75 并行，给 Node v185 提供更接近真实读取的 evidence。

所以本版不再继续横向扩张新的恢复权限字段，而是把 v74 的审批边界变成一份可按真实 server/client smoke 跑的只读清单。

## 2. CMakeLists.txt

本版把 CMake 项目版本从 `0.74.0` 提升到 `0.75.0`。运行时 `INFOJSON` 仍通过生成的 `minikv/version.hpp` 暴露版本号，所以真实运行时版本断言也同步到 `0.75.0`。

新增测试目标：

```text
minikv_restore_boundary_smoke_manifest_tests
```

这个目标只验证 smoke manifest 合同和只读命令解释，不新增 runtime 命令，也不改变 Store、WAL、Snapshot 或 TCP server 行为。

## 3. restore-boundary-smoke-manifest.json

新增文件：

```text
fixtures/release/restore-boundary-smoke-manifest.json
```

它的顶层字段先固定控制面理解方式：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
java_transaction_chain_connected=false
consumer_hint=Node v185 real-read rehearsal intake
```

这些字段告诉 Node：这份 fixture 可以作为 real-read rehearsal intake 的 mini-kv 输入，但它不是 restore executor，不打开写操作，不让 mini-kv 参与 Java transaction chain。

`smoke_target` 记录真实 smoke 的运行形状：

```text
runtime_target=minikv_server 6415 127.0.0.1
client_target=minikv_client 127.0.0.1 6415 5000 --connect-retries 10 --retry-delay-ms 100
restore_target_placeholder
artifact_digest_placeholder
```

这里的 target 是运行验证入口，不是生产发布入口。restore target 和 artifact digest 仍然保持 placeholder，真实值由外部人工流程确认。

`real_read_smoke` 使用：

```text
INFOJSON
CHECKJSON LOAD data/real-read-boundary-restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX restore:real-read-token 30 value
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

这里的 `CHECKJSON` 只解释风险，不执行目标命令。最后 `GET restore:real-read-token` 必须返回 `(nil)`，用来证明真实 smoke 没有执行 `SETNXEX` 写入。

## 4. verification-manifest.json

`fixtures/release/verification-manifest.json` 升级为 v75，新增：

```text
fixtures/release/restore-boundary-smoke-manifest.json
minikv_restore_boundary_smoke_manifest_tests
```

同时 read-only smoke 从 v74 的 approval token 切换到 v75 的 real-read token：

```text
CHECKJSON LOAD data/real-read-boundary-restore.snap
CHECKJSON SETNXEX restore:real-read-token 30 value
GET restore:real-read-token
```

这样总 manifest 代表当前版本的验证入口。历史 v64-v74 fixture 自己的 `release_version` 和 `project_version` 不被重写，避免把历史证据伪装成新版本。

## 5. 测试保护

`tests/restore_boundary_smoke_manifest_tests.cpp` 分两层保护本版。

第一层保护 fixture 合同：确认 v75 版本、consumer hint、smoke manifest id、runtime target、client target、restore target placeholder、artifact digest placeholder、required evidence fields、boundaries、warnings、pause conditions 和输入 fixture 路径都存在。

第二层保护运行时只读语义：用真实 `CommandProcessor` 执行 `INFOJSON`、`CHECKJSON LOAD`、`CHECKJSON COMPACT`、`CHECKJSON SETNXEX`、`STORAGEJSON` 和 `GET restore:real-read-token`，确认版本是 `0.75.0`，确认 `CHECKJSON` 不执行真实写入，确认 `STORAGEJSON` 仍然 `order_authoritative=false`，确认 real-read token 查询仍是 `(nil)`。

`tests/release_verification_manifest_tests.cpp` 同步保护总 manifest：它检查 v75 的 CMake/CTest 命令、targeted tests、read-only smoke、fixture inventory、CMake 版本和 no-execution boundary。

## 6. README 和归档

README 同步更新 Current version、fixture 列表、CLion target 列表、release fixture 说明和 Roadmap。当前跨项目方向对齐为：Java v66 与 mini-kv v75 推荐并行补真实运行纵深 evidence，Node v185 等待两侧完成后只读生成 real-read rehearsal intake。

运行调试归档放在：

```text
c/75/图片/
c/75/解释/说明.md
```

里面记录 CMake configure、build、CTest、真实 server + client smoke 的关键输出。真实 smoke 只发送 `CHECKJSON` 和读取命令，不执行真实恢复或写 token。

## 7. 一句话总结

v75 把 mini-kv 从“恢复审批边界可被决策演练读取”推进到“恢复边界可以按真实 server/client 形态被只读 smoke 验证”，同时继续守住只读、非订单权威、Java transaction chain disconnected 和不执行恢复的边界。
