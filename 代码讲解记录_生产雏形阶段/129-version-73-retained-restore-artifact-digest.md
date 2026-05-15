# 第七十三版 Retained Restore Artifact Digest 代码讲解

本版目标是按 `v179-post-pre-approval-roadmap.md` 的最新指导，给 mini-kv 增加一份 retained restore artifact digest 只读 fixture，让后续 Node v180 的 approval decision prerequisite gate 能读取 mini-kv 侧更稳定的恢复产物摘要证据。

这不是新的恢复执行入口，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 restore，不读取生产密钥，不启动 Node/Java，也不把 mini-kv 接进 Java 订单一致性链路。mini-kv 仍然只是恢复产物、Snapshot/WAL review 和 order-authoritative 边界的证据提供方。

## 1. 本版所处阶段

v72 已经把 restore evidence retention 做成了可被 Node v178 消费的证据保留 fixture。v73 再往审批决定前推进一小步：把“保留了哪些 artifact digest、Snapshot/WAL review digest、restore target、retention owner”明确写成单独 fixture，供 Node v180 判断是否具备进入 approval decision 版本的前置条件。

所以本版的核心价值不是扩展 KV 命令，而是把审批前缺失的 mini-kv 恢复产物摘要证据固定下来。

## 2. CMakeLists.txt

本版把 CMake 项目版本从 `0.72.0` 提升到 `0.73.0`。运行时 `INFOJSON` 仍通过生成的 `minikv/version.hpp` 暴露版本号，所以所有真实运行时版本断言也同步到 `0.73.0`。

新增测试目标：

```text
minikv_retained_restore_artifact_digest_tests
```

这个目标只验证 fixture 合同和只读命令解释，不引入新的生产代码路径。

## 3. retained-restore-artifact-digest.json

新增文件：

```text
fixtures/release/retained-restore-artifact-digest.json
```

它的顶层字段先固定控制面理解方式：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
consumer_hint=Node v180 approval decision prerequisite gate
```

这几个字段告诉 Node：这是一份审批前置证据，不是 approval decision，不是 restore executor，也不能让 mini-kv 变成 Java 订单权威存储。

`digest_target` 记录本版真正想补齐的字段：

```text
restore_artifact_digest_placeholder
snapshot_review_digest_placeholder
wal_review_digest_placeholder
restore_target_placeholder
retention_owner
```

这些字段都保持 placeholder 形态。控制面可以检查字段是否存在、是否由人工保留、是否仍然没有越过边界，但不能把它们当作真实 artifact bytes 或生产 secret value。

`retained_digest_evidence` 进一步列出 Node v180 可以检查的确认项，例如 artifact digest、Snapshot review digest、WAL review digest 是否都“保留在 mini-kv 外部”。这点很重要：mini-kv 仓库只保存证据形状和边界，真实产物和人工签核仍在外部流程里。

`checkjson_digest_evidence` 使用：

```text
INFOJSON
CHECKJSON LOAD data/retained-digest-restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX restore:digest-token 30 value
STORAGEJSON
HEALTH
GET restore:digest-token
QUIT
```

这里的 `CHECKJSON` 只解释风险，不执行目标命令。最后 `GET restore:digest-token` 必须返回 `(nil)`，用来证明 `SETNXEX` 没有被真实执行。

## 4. verification-manifest.json

`fixtures/release/verification-manifest.json` 升级为 v73，新增：

```text
fixtures/release/retained-restore-artifact-digest.json
minikv_retained_restore_artifact_digest_tests
```

同时 read-only smoke 从 v72 的 retention token 切换到 v73 的 digest token：

```text
CHECKJSON LOAD data/retained-digest-restore.snap
CHECKJSON SETNXEX restore:digest-token 30 value
GET restore:digest-token
```

这样总 manifest 代表当前版本的验证入口；历史 v64-v72 fixture 自己的 `release_version` 和 `project_version` 不被重写，避免把历史证据伪装成新版本。

## 5. 测试保护

`tests/retained_restore_artifact_digest_tests.cpp` 分两层保护本版。

第一层保护 fixture 合同：确认 v73 版本、consumer hint、retention id、restore target placeholder、artifact/Snapshot/WAL digest placeholder、retention owner、required confirmations、boundaries、warnings、pause conditions 和输入 fixture 路径都存在。

第二层保护运行时只读语义：用真实 `CommandProcessor` 执行 `INFOJSON`、`CHECKJSON LOAD`、`CHECKJSON COMPACT`、`CHECKJSON SETNXEX`、`STORAGEJSON` 和 `GET restore:digest-token`，确认版本是 `0.73.0`，确认 `CHECKJSON` 不执行真实写入，确认 `STORAGEJSON` 仍然 `order_authoritative=false`，确认 token 查询仍是 `(nil)`。

`tests/release_verification_manifest_tests.cpp` 同步保护总 manifest：它检查 v73 的 CMake/CTest 命令、targeted tests、read-only smoke、fixture inventory、CMake 版本和 no-execution boundary。

## 6. README 和归档

README 同步更新 Current version、fixture 列表、CLion target 列表、release fixture 说明和 Roadmap。当前跨项目方向对齐为：Node v179 已完成 pre-approval packet，Java v64 与 mini-kv v73 推荐并行补审批前置证据，Node v180 等待两侧新 fixture 后再判断是否可以进入 approval decision prerequisite gate。

运行调试归档放在：

```text
c/73/图片/
c/73/解释/说明.md
```

里面记录 CMake configure、build、CTest、真实 server + client smoke 的关键输出。真实 smoke 只发送 `CHECKJSON` 和读取命令，不执行真实恢复或写 token。

## 7. 一句话总结

v73 把 mini-kv 从“恢复证据已经保留”推进到“恢复产物摘要证据可以进入审批决定前置检查”，同时继续守住只读、人工确认、非订单权威和不执行恢复的边界。
