# 第一百一十九次讲解：第六十三版 Release Verification Manifest

本版目标是按最新跨项目计划从“幂等演练”转入“发布验证硬化”。v61/v62 已经完成短 TTL token 和恢复证据，Node v161 也已经把幂等纵向切片收口；v63 开始补 mini-kv 的 release verification manifest，让后续 Node v162 能只读消费 mini-kv 的构建、测试、命令 smoke、fixture 和版本证据。

它不是新的 server 命令，不新增 runtime API，不改变 `SETNXEX`、WAL、Snapshot、TCP/RESP 或存储语义，不接入 Java 订单链路，也不让 Node 自动执行 mini-kv 构建。它只是把 release 前应该验证什么、用什么命令验证、哪些 fixture 和版本字段要对齐，固化成一个可测试的只读清单。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v161-post-controlled-idempotency-drill-roadmap.md
```

计划要求当前轮到：

```text
推荐并行：Java v54 + mini-kv v63
mini-kv v63 做 release verification manifest
固化 CMake configure/build、CTest、命令 smoke、fixture/version 清单
不新增 runtime 命令
不进入 Java 订单权威链路
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 现在进入“生产雏形阶段”的发布验证硬化小步。前面的版本更多是在补能力和证据字段；v63 开始回答另一个更工程化的问题：

```text
一版 mini-kv 发布前，哪些命令、哪些测试、哪些 fixture、哪些版本字段必须能复现？
```

这个问题对跨项目协作很关键。Node v162 不应该替 mini-kv 执行 CMake 或 CTest，它只应该读取 mini-kv 已经归档的 release evidence。因此 mini-kv 需要给出一份稳定 manifest，告诉下游控制面“我这一侧 release verification 应该长什么样”。

## Release Manifest Fixture

新增文件：

```text
fixtures/release/verification-manifest.json
```

核心字段：

```text
manifest_version=mini-kv-release-verification-manifest.v1
project=mini-kv
project_version=0.63.0
release_version=v63
read_only=true
execution_allowed=false
order_authoritative=false
no_runtime_command_added=true
consumer_hint=Node v162 cross-project release verification intake gate
```

这里 `read_only=true` 和 `execution_allowed=false` 说明的是 manifest 自身：它是清单证据，不是执行器。它列出的 CMake、CTest、smoke 命令需要由 mini-kv 本项目在版本开发时执行并归档；Node v162 只消费结果，不代跑。

## 构建与测试清单

manifest 中固化了三类 release verification 命令：

```text
cmake -S . -B cmake-build-v63 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-v63 --parallel 2
ctest --test-dir cmake-build-v63 --output-on-failure
```

这不是发明新流程，而是把本项目一直在做的 CMake/CTest 验证写成机器可读证据。它让下游看到：

```text
configure 必须成功
build 必须构建所有目标
CTest 必须跑完注册测试
```

本版还列出 targeted tests：

```text
minikv_command_tests
minikv_readonly_fixture_tests
minikv_recovery_fixture_index_tests
minikv_ttl_token_recovery_tests
minikv_release_verification_manifest_tests
```

这些是 release 证据中最能代表契约、fixture、恢复和 manifest 自身不漂移的测试入口。

## Read-Only Smoke

manifest 里的命令 smoke 故意选只读路径：

```text
INFOJSON
COMMANDSJSON
CHECKJSON SETNXEX release:token 30 value
STORAGEJSON
HEALTH
QUIT
```

这里最容易误解的是 `CHECKJSON SETNXEX ...`。它不会执行 `SETNXEX`，只解释这个写命令如果执行会有什么风险、是否触碰 WAL、是否会写 store。真实 smoke 最后又跑了：

```text
GET release:token -> (nil)
```

这证明 release smoke 没有真的写 token。控制面读到后应该这样理解：

```text
SETNXEX 是写命令
CHECKJSON 是只读契约检查
release smoke 可以检查写命令风险，但不能执行写命令
```

## Fixture 与版本清单

manifest 列出 mini-kv 当前 release evidence 需要关注的 fixture：

```text
fixtures/readonly/index.json
fixtures/readonly/infojson-empty-inline.json
fixtures/readonly/statsjson-empty-inline.json
fixtures/readonly/runtime-read-field-guide.json
fixtures/checkjson/set-orderops-write-contract.json
fixtures/checkjson/get-orderops-read-contract.json
fixtures/recovery/index.json
fixtures/recovery/restart-recovery-evidence.json
fixtures/ttl-token/index.json
fixtures/ttl-token/recovery-evidence.json
fixtures/release/verification-manifest.json
```

其中 `fixtures/readonly/infojson-empty-inline.json` 和 manifest 自身都同步到：

```text
0.63.0
```

这让 Node v162 可以核对“代码版本”和“只读 fixture 版本”是否一致，而不是只看 tag 名字。

## 新增测试

新增文件：

```text
tests/release_verification_manifest_tests.cpp
```

新增 CTest 目标：

```text
minikv_release_verification_manifest_tests
```

测试分四层保护 manifest：

```text
1. 检查 manifest 核心字段：read_only、execution_allowed、order_authoritative、project_version、consumer_hint
2. 检查 CMake/CTest/targeted tests/read-only smoke 命令是否写入清单
3. 检查 manifest 中列出的 fixture path 是否真实存在
4. 用 CommandProcessor 实际执行 INFOJSON、CHECKJSON SETNXEX、STORAGEJSON，确认版本和只读边界
```

最后还验证：

```text
GET release:token -> (nil)
```

这保证 `CHECKJSON SETNXEX` 没有偷跑成写命令。

## README 与版本

更新文件：

```text
CMakeLists.txt
README.md
fixtures/readonly/infojson-empty-inline.json
```

版本从：

```text
0.62.0
```

推进到：

```text
0.63.0
```

README 增加 `fixtures/release/verification-manifest.json` 的说明，把测试目标 `minikv_release_verification_manifest_tests` 加入 CLion/CMake target 列表，并把 Roadmap 推进到 Java v54 + mini-kv v63 / Node v162 release gate。

## 真实运行

本版真实 smoke 启动了 mini-kv server，并执行：

```text
INFOJSON
COMMANDSJSON
CHECKJSON SETNXEX release:token 30 value
STORAGEJSON
HEALTH
GET release:token
QUIT
```

验证结果：

```text
INFOJSON -> version=0.63.0
COMMANDSJSON -> command catalog 可读
CHECKJSON SETNXEX -> read_only=true, execution_allowed=false
STORAGEJSON -> order_authoritative=false
HEALTH -> OK live_keys=0
GET release:token -> (nil)
```

本轮 smoke 只运行 mini-kv，不启动 Java，不启动 Node，不执行 `SET` / `DEL` / `EXPIRE` / `SETNXEX`。

## 归档

从本版开始，运行调试归档按新规则写入：

```text
c/63/图片/
c/63/解释/说明.md
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
```

manifest 是 release verification 清单，不是自动化执行器。Node v162 后续应该读取 mini-kv v63 tag、manifest 和归档证据，而不是在 Node 侧代跑 mini-kv 的 CMake/CTest。

## 一句话总结

v63 让 mini-kv 的发布前验证从“人知道要跑哪些命令”推进到“清单化、可测试、可被 Node 只读消费”的 release evidence gate，为三项目从功能雏形走向可复现交付流程补上了 C++ 侧的一块关键地板。
