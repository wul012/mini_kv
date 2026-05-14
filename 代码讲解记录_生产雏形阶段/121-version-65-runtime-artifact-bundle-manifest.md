# 第一百二十一次讲解：第六十五版 Runtime Artifact Bundle Manifest

本版目标是按最新跨项目计划从 rollback readiness 继续推进到 release bundle 硬化。Java v55、mini-kv v64、Node v163 已经把回滚 readiness 收口，下一步推荐并行 Java v56 和 mini-kv v65：Java 侧整理 release bundle manifest，mini-kv 侧整理 runtime artifact bundle manifest。

它不是新的 server 命令，不新增 runtime API，不执行 `LOAD`、`COMPACT`、`SETNXEX`，不启动 Node 或 Java，不接生产数据库，也不把 mini-kv 写入 Java 订单一致性链路。它只是把 binary version、release manifest、rollback evidence、WAL/Snapshot compatibility、CTest、read-only smoke 和 fixture inventory 收成一个可被 Node v164 只读消费的 bundle manifest。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v163-post-rollback-readiness-roadmap.md
```

计划要求当前轮到：

```text
推荐并行：Java v56 + mini-kv v65
mini-kv v65 做 runtime artifact bundle manifest
引用 binary version、release manifest、runtime artifact rollback evidence
记录 CMake configure/build、CTest、read-only smoke、fixture inventory
明确 LOAD/COMPACT/SETNXEX 仍只允许 CHECKJSON 解释，不执行
不进入 Java 订单权威链路
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 仍处在“生产雏形阶段”，但本版的关注点比 v64 更偏交付整理。

v64 解决的是：

```text
回滚讨论里 binary/version、WAL、Snapshot、fixture 的边界是什么？
```

v65 解决的是：

```text
Node v164 想做跨项目 release bundle gate 时，mini-kv 应该给它一包什么证据？
```

这个 bundle 不是发布器，不是回滚器，也不是授权系统。它只是把 mini-kv 已经存在的发布验证、回滚边界、恢复/TTL/只读 fixture 和真实 smoke 验证入口打包成一份稳定清单。

## Bundle Manifest Fixture

新增文件：

```text
fixtures/release/runtime-artifact-bundle-manifest.json
```

核心字段：

```text
bundle_version=mini-kv-runtime-artifact-bundle.v1
project=mini-kv
project_version=0.65.0
release_version=v65
path=fixtures/release/runtime-artifact-bundle-manifest.json
read_only=true
execution_allowed=false
order_authoritative=false
consumer_hint=Node v164 cross-project release bundle gate
```

控制面读到这些字段时应该先知道两件事：

```text
1. 这是 bundle manifest，不是执行器
2. mini-kv 仍然不是 Java 订单权威源
```

因此它可以进入 Node 的 release bundle gate，但不能触发生产发布、生产回滚或任何上游写操作。

## Bundle 内容

bundle 里把证据拆成五类：

```text
binary-version
release-verification-manifest
runtime-artifact-rollback-evidence
wal-snapshot-compatibility
fixture-inventory
```

`binary-version` 说明当前 CMake 版本、`INFOJSON` 运行时版本和 release tag 要对齐。

`release-verification-manifest` 引用：

```text
fixtures/release/verification-manifest.json
```

它是 v65 当前 release verification 清单，记录 CMake configure/build、CTest、targeted tests 和 read-only smoke。

`runtime-artifact-rollback-evidence` 引用：

```text
fixtures/release/runtime-artifact-rollback-evidence.json
```

这里保留 `source_release_version=v64` 是有意的：v64 是回滚边界证据来源，v65 是把它纳入 bundle，而不是篡改历史回滚样本。

`wal-snapshot-compatibility` 记录 WAL2 checksum replay、`SETEXAT` token replay、`EXPIREAT` expiration replay、manual Snapshot、atomic snapshot save 等兼容性证据。但 bundle 只通过 `CHECKJSON` 查看 `LOAD` / `COMPACT` 风险，不执行它们。

`fixture-inventory` 列出 readonly、checkjson、recovery、ttl-token 这些稳定样本路径，让 Node v164 后续能做只读存在性和版本核对。

## Read-Only Smoke

v65 的 smoke 命令：

```text
INFOJSON
COMMANDSJSON
CHECKJSON LOAD data/bundle.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX bundle:token 30 value
STORAGEJSON
HEALTH
GET bundle:token
QUIT
```

这里的三个风险命令仍然只走 `CHECKJSON`：

```text
CHECKJSON LOAD ...      -> 只解释 LOAD 会 store_replace_from_snapshot
CHECKJSON COMPACT       -> 只解释 COMPACT 会 wal_rewrite_when_enabled
CHECKJSON SETNXEX ...   -> 只解释 token 写入和 WAL 风险
```

真实 smoke 最后用：

```text
GET bundle:token -> (nil)
```

证明 `SETNXEX` 没有被执行，bundle smoke 仍是只读验证。

## Release Manifest 跟随

更新文件：

```text
CMakeLists.txt
fixtures/release/verification-manifest.json
fixtures/readonly/infojson-empty-inline.json
```

版本从：

```text
0.64.0
```

推进到：

```text
0.65.0
```

`verification-manifest.json` 新增：

```text
fixtures/release/runtime-artifact-bundle-manifest.json
minikv_runtime_artifact_bundle_manifest_tests
```

这样 release manifest 自身也能指向 v65 的 bundle，而不是只停留在 v64 的 rollback evidence。

## 新增测试

新增文件：

```text
tests/runtime_artifact_bundle_manifest_tests.cpp
```

新增 CTest 目标：

```text
minikv_runtime_artifact_bundle_manifest_tests
```

测试分五层：

```text
1. 检查 bundle manifest 的版本、只读边界、consumer_hint 和 pause conditions
2. 检查五类 bundle artifacts 都存在
3. 检查 bundle 引用的 fixture path 真实存在
4. 用真实 CommandProcessor 执行 INFOJSON、COMMANDSJSON、CHECKJSON、STORAGEJSON、GET
5. 确认 LOAD、COMPACT、SETNXEX 只被解释，bundle:token 仍然不存在
```

原有：

```text
tests/release_verification_manifest_tests.cpp
tests/runtime_artifact_rollback_evidence_tests.cpp
```

也同步到当前运行版本 `0.65.0`，保证旧回滚证据仍能被当前二进制解释和验证。

## README 与 Roadmap

README 的当前版本更新为 Version 65，并新增：

```text
fixtures/release/runtime-artifact-bundle-manifest.json
minikv_runtime_artifact_bundle_manifest_tests
```

Roadmap 推进为：

```text
Java v56 + mini-kv v65 并行补 release/runtime artifact bundle manifest
Node v164 等待两边完成后生成 cross-project release bundle gate
```

这和跨项目计划保持一致。

## 真实运行

本版真实 smoke 启动 mini-kv server，并执行：

```text
INFOJSON
COMMANDSJSON
CHECKJSON LOAD data/bundle.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX bundle:token 30 value
STORAGEJSON
HEALTH
GET bundle:token
QUIT
```

验证目标：

```text
INFOJSON -> version=0.65.0
COMMANDSJSON -> command catalog 可读
CHECKJSON LOAD -> read_only=true, execution_allowed=false, side_effects=store_replace_from_snapshot
CHECKJSON COMPACT -> read_only=true, execution_allowed=false, side_effects=wal_rewrite_when_enabled
CHECKJSON SETNXEX -> read_only=true, execution_allowed=false, write_command=true
STORAGEJSON -> order_authoritative=false
GET bundle:token -> (nil)
```

本轮 smoke 只运行 mini-kv，不启动 Java，不启动 Node，不执行真实 release、restore 或 rollback 命令。

## 归档

运行调试归档继续写入：

```text
c/65/图片/
c/65/解释/说明.md
```

历史目录 `a/` 和 `b/` 保留，不搬迁。

## 边界

本版继续保持：

```text
不新增 runtime 命令
不修改 Node
不修改 Java
不启动 Node 或 Java
不打开 UPSTREAM_ACTIONS_ENABLED=true
不进入 Java 订单权威链路
不让 mini-kv 成为订单权威存储
不执行 LOAD / COMPACT / SETNXEX
不授权生产发布或回滚
```

如果未来 Node v164 发现 Java v56 和 mini-kv v65 的 release bundle 证据不一致，正确行为是暂停并让用户确认，而不是自动发布或回滚。

## 一句话总结

v65 让 mini-kv 的发布证据从“单项 manifest 和回滚边界样本”推进到“runtime artifact bundle 可只读消费”，为 Node v164 的三项目 release bundle gate 补上了 C++ 侧的交付包。
