# 第七十版：Restore Drill Evidence

本版目标是按 `v169-post-production-environment-preflight-roadmap.md` 的最新指导，给 mini-kv 增加一份恢复演练证据样本，让后续 Node v173 的 release window readiness packet 能引用 mini-kv v70 的恢复演练准备状态。

它不是什么：不是新的恢复执行入口，不执行 `LOAD` / `COMPACT` / `SETNXEX`，不绕过人工审批，不读取生产密钥，不连接 Java 订单事务链，也不把 mini-kv 变成 Java 订单权威库。

## 1. CMakeLists.txt

本版把 CMake 项目版本从 `0.69.0` 提升到 `0.70.0`。这个版本号会生成到 `minikv/version.hpp`，所以 `INFO`、`INFOJSON`、`STORAGEJSON` 和相关测试都会读到同一个运行时版本。

新增测试目标：

```text
minikv_restore_drill_evidence_tests
```

它和已有 release evidence 测试一样注入 `MINIKV_SOURCE_DIR`，这样测试可以直接读取仓库里的 fixture，并确认文件路径、字段和真实命令解释结果保持一致。

## 2. restore-drill-evidence.json

新增文件：

```text
fixtures/release/restore-drill-evidence.json
```

这份 fixture 的角色是“恢复演练证据”，不是恢复执行器。核心字段包括：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
consumer_hint=Node v173 release window readiness packet
```

控制面读到这些字段时应该理解为：mini-kv 提供的是可审核、可引用的恢复演练准备材料，但任何真实恢复动作仍然必须在 mini-kv 外部经过人工确认和运维流程。

`restore_drill_target` 记录本次演练目标是 `v70`，并引用 v69 release artifact digest package、v68 artifact digest compatibility matrix 等历史证据作为比较基础。`digest_comparison_placeholder` 保留 `sha256:<operator-recorded-restore-drill-digest>`，意思是摘要值由操作员或外部流程记录，mini-kv 只提供占位契约。

`restore_drill_command_profile` 记录演练命令：

```text
INFOJSON
CHECKJSON LOAD data/restore-drill.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX restore:drill-token 30 value
STORAGEJSON
HEALTH
GET restore:drill-token
QUIT
```

关键点在 `CHECKJSON`：它只解释命令风险和 side effects，不执行命令本体。因此 `GET restore:drill-token` 必须保持 `(nil)`，证明 `SETNXEX` 没有真的写入 token。

## 3. verification-manifest 和 README

`fixtures/release/verification-manifest.json` 升级为 v70，加入：

```text
minikv_restore_drill_evidence_tests
fixtures/release/restore-drill-evidence.json
CHECKJSON LOAD data/restore-drill.snap
CHECKJSON SETNXEX restore:drill-token 30 value
GET restore:drill-token
```

同时继续保留 v69 release artifact digest package 的历史语义。也就是说，v69 的 `release:token` 样本没有被重写成 v70；v70 只是新增一层 restore drill evidence，让后续 Node v173 可以消费更靠近发布窗口的恢复演练证据。

README 更新 Current version、fixture 列表、CLion 目标列表和 Roadmap，把当前主线对齐为 Java v61 + mini-kv v70 并行，Node v173 等待两边完成后再生成 release window readiness packet。

## 4. tests/restore_drill_evidence_tests.cpp

新增测试分两层保护：

第一层是 fixture 合同断言：确认项目版本、发布版本、只读边界、恢复目标、digest placeholder、operator confirmation、输入 fixture、warning 和 pause condition 都存在。

第二层是真实 `CommandProcessor` 断言：执行 `INFOJSON`、`CHECKJSON LOAD`、`CHECKJSON COMPACT`、`CHECKJSON SETNXEX`、`STORAGEJSON`、`GET restore:drill-token`。这里的“执行”只是在测试进程里调用命令处理器；其中 `CHECKJSON` 仍然是读合同，不会真正 LOAD、COMPACT 或 SETNXEX。

这类断言保护的是恢复演练证据的边界：控制面能看到风险解释，但不能误以为 mini-kv 已经执行了恢复。

## 5. 验证和归档

本版使用 CLion bundled CMake/CTest 工具链完成：

```text
cmake -S . -B cmake-build-v70 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-v70 --parallel 2
ctest --test-dir cmake-build-v70 --output-on-failure
```

完整 CTest 结果为 `25/25 passed`，新增的 `restore_drill_evidence_tests` 通过。

真实 smoke 启动 `minikv_server 6410 127.0.0.1`，再用 bundled TCP client 发送 v70 restore drill profile。结果证明：

```text
INFOJSON version=0.70.0
CHECKJSON LOAD/COMPACT/SETNXEX 只返回 read_only contract
STORAGEJSON live_keys=0 且 order_authoritative=false
GET restore:drill-token 返回 (nil)
服务端验证后已停止
```

一句话总结：v70 把 mini-kv 从“发布产物摘要包”推进到“恢复演练证据可被发布窗口引用”的状态，但仍然守住只读证据、人工审核和非订单权威边界。
