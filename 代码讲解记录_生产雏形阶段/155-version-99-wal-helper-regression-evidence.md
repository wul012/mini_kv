# mini-kv v99 execute-with-wal regression evidence

本版目标：在 v98 已经把 `SET`、`SETNXEX`、`DEL`、`EXPIRE` 的 WAL / no-WAL 重复流程收进 `CommandProcessor::execute_with_wal` 之后，v99 不继续扩大重构，而是补上更细的回归证据。它验证 helper 周围最容易被误改的行为：usage error 不写 WAL、missing / expired no-op 不写 WAL、成功写入仍保持 append-before-mutation 顺序。

这不是什么：不是新的执行入口，不是 dispatch table 大迁移，不绕过 `CHECKJSON` / `SMOKEJSON` 的只读契约，不触碰 snapshot / restore / admin 核心，也不让 mini-kv 变成 Node / Java / managed audit 的状态权威。

## 写路径回归

`tests/wal_tests.cpp` 是本版的核心测试入口。新增断言围绕同一条 WAL 场景展开：先跑成功的 `SET`、`SETNXEX`、`DEL`、`EXPIRE` 分支，再插入 usage error、missing key 和 expired key miss。测试通过 WAL 记录数量确认这些失败或 no-op 分支不会产生空 WAL 记录，同时保留成功路径的恢复记录。

关键保护点是顺序和边界：`SET only-key`、`SETNXEX token 0 invalid`、`DEL`、`EXPIRE name not-a-number` 只返回明确 usage error；`DEL stale` 和 `EXPIRE stale 60` 在 key 已过期后返回 `0`；这些分支都不应改变 WAL 记录数。

## 证据滚动

`CMakeLists.txt` 升到 `0.99.0`，当前 runtime evidence、release manifest、只读 fixture 和测试期望同步滚到 `v99` / `c/99/`。`src/runtime_evidence_receipts.cpp` 和 `src/managed_audit_receipts.cpp` 只更新当前证据路径、版本 hint 和 digest 对齐，不改变 response 字段结构。

对控制面来说，`fixtures/release/verification-manifest.json` 和 `fixtures/release/runtime-smoke-evidence.json` 仍然表达同一组边界：`read_only=true`、`execution_allowed=false`、`restore_execution_allowed=false`、`order_authoritative=false`。v99 只补强 execute-with-wal 回归覆盖，并把后续消费者从 Node v233 的 review 语境推进到 Node v234 blocked execution rehearsal。

## 测试和 smoke

本版预期验证链仍是 CMake configure、CMake build、全量 CTest、真实 TCP smoke。真实 smoke 只执行 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`，用于证明当前二进制能输出 v99 只读 evidence，且 `GET restore:real-read-token` 仍为 `(nil)`。

测试的意义不是证明写命令可以被控制面调用，而是证明写命令内部重构没有破坏 WAL 语义。Node v234 只能消费这些只读证据来确认危险动作仍被阻断，不能据此启动 mini-kv、执行 `SETNXEX`、`LOAD`、`COMPACT`、restore、credential read、schema rehearsal 或 managed audit write。

一句话总结：v99 给 v98 的 WAL helper 加了一层更硬的行为护栏，在不扩大任何跨项目权限的前提下，让 mini-kv 的写路径重构更可回看、可验证、可继续演进。
