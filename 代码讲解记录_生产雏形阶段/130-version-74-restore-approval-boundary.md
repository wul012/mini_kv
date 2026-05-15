# 第七十四版 Restore Approval Boundary 代码讲解

本版目标是按 `v179-post-pre-approval-roadmap.md` 的最新指导，给 mini-kv 增加一份 restore approval boundary 只读 fixture，让后续 Node v182 的 release approval decision rehearsal packet 能读取 mini-kv 侧的恢复审批边界证据。

这不是新的恢复执行入口，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 restore，不读取生产密钥，不启动 Node/Java，也不把 mini-kv 接进 Java 订单一致性链路。mini-kv 仍然只是恢复审批边界、restore target、artifact digest 和 `order_authoritative=false` 的证据提供方。

## 1. 本版所处阶段

v73 已经把 retained restore artifact digest 固定成 Node v180 可以消费的审批前置证据。Node v180 和 v181 已经继续推进到 prerequisite gate 与 approval ledger dry-run envelope。v74 再补下一步 Node v182 需要的 mini-kv 侧边界：谁是 restore approver、恢复目标是什么、artifact digest 如何占位、mini-kv 是否仍然不属于 Java transaction chain。

所以本版的核心价值不是扩展 KV 能力，而是把“恢复审批只是一份边界证据，不是恢复执行授权”写成可以被测试保护的 fixture。

## 2. CMakeLists.txt

本版把 CMake 项目版本从 `0.73.0` 提升到 `0.74.0`。运行时 `INFOJSON` 仍通过生成的 `minikv/version.hpp` 暴露版本号，所以真实运行时版本断言也同步到 `0.74.0`。

新增测试目标：

```text
minikv_restore_approval_boundary_tests
```

这个目标只验证 fixture 合同和只读命令解释，不新增 runtime 命令，也不改变存储、WAL、Snapshot 的执行路径。

## 3. restore-approval-boundary.json

新增文件：

```text
fixtures/release/restore-approval-boundary.json
```

它的顶层字段先固定控制面理解方式：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
java_transaction_chain_connected=false
consumer_hint=Node v182 release approval decision rehearsal packet
```

这些字段告诉 Node：这份 fixture 可以作为 release approval decision rehearsal 的输入，但它不是 approval decision，不写 approval ledger，不执行 restore，也不能让 mini-kv 变成 Java 订单权威存储。

`approval_boundary_target` 记录本版真正补齐的审批边界字段：

```text
restore_approver_placeholder
restore_target_placeholder
artifact_digest_placeholder
java_transaction_chain=disconnected
order_authoritative=false
```

这些字段都保持 placeholder 形态。控制面可以检查字段是否存在、是否明确由人工确认、是否仍然断开 Java transaction chain，但不能把它们当作真实 secret、真实 ledger 写入或真实恢复授权。

`restore_approval_evidence` 列出 Node v182 可以理解的确认项：restore approver 不包含生产 secret、restore target 和 artifact digest 存在于 mini-kv 外部、Java transaction chain 仍然 disconnected、restore execution 仍然 disabled。

`checkjson_boundary_evidence` 使用：

```text
INFOJSON
CHECKJSON LOAD data/approval-boundary-restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX restore:approval-token 30 value
STORAGEJSON
HEALTH
GET restore:approval-token
QUIT
```

这里的 `CHECKJSON` 只解释风险，不执行目标命令。最后 `GET restore:approval-token` 必须返回 `(nil)`，用来证明 `SETNXEX` 没有被真实执行。

## 4. verification-manifest.json

`fixtures/release/verification-manifest.json` 升级为 v74，新增：

```text
fixtures/release/restore-approval-boundary.json
minikv_restore_approval_boundary_tests
```

同时 read-only smoke 从 v73 的 digest token 切换到 v74 的 approval token：

```text
CHECKJSON LOAD data/approval-boundary-restore.snap
CHECKJSON SETNXEX restore:approval-token 30 value
GET restore:approval-token
```

这样总 manifest 代表当前版本的验证入口。历史 v64-v73 fixture 自己的 `release_version` 和 `project_version` 不被重写，避免把历史证据伪装成新版本。

## 5. 测试保护

`tests/restore_approval_boundary_tests.cpp` 分两层保护本版。

第一层保护 fixture 合同：确认 v74 版本、consumer hint、approval boundary id、restore approver placeholder、restore target placeholder、artifact digest placeholder、Java transaction chain disconnected、required confirmations、boundaries、warnings、pause conditions 和输入 fixture 路径都存在。

第二层保护运行时只读语义：用真实 `CommandProcessor` 执行 `INFOJSON`、`CHECKJSON LOAD`、`CHECKJSON COMPACT`、`CHECKJSON SETNXEX`、`STORAGEJSON` 和 `GET restore:approval-token`，确认版本是 `0.74.0`，确认 `CHECKJSON` 不执行真实写入，确认 `STORAGEJSON` 仍然 `order_authoritative=false`，确认 approval token 查询仍是 `(nil)`。

`tests/release_verification_manifest_tests.cpp` 同步保护总 manifest：它检查 v74 的 CMake/CTest 命令、targeted tests、read-only smoke、fixture inventory、CMake 版本和 no-execution boundary。

## 6. README 和归档

README 同步更新 Current version、fixture 列表、CLion target 列表、release fixture 说明和 Roadmap。当前跨项目方向对齐为：Java v65 与 mini-kv v74 推荐并行补 rollback/restore approval boundary，Node v182 等待 Node v181、Java v65、mini-kv v74 完成后再生成 release approval decision rehearsal packet。

运行调试归档放在：

```text
c/74/图片/
c/74/解释/说明.md
```

里面记录 CMake configure、build、CTest、真实 server + client smoke 的关键输出。真实 smoke 只发送 `CHECKJSON` 和读取命令，不执行真实恢复或写 token。

## 7. 一句话总结

v74 把 mini-kv 从“恢复产物摘要证据可被审批前置检查读取”推进到“恢复审批边界可被 release decision rehearsal 读取”，同时继续守住只读、人工确认、非订单权威、Java transaction chain disconnected 和不执行恢复的边界。
