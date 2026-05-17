# 148 - Version 92 Managed Audit Receipt Formatter Split

v92 的目标不是新增新的 managed audit 能力，而是把 v86-v91 累积在 `src/command.cpp` 里的 managed audit receipt JSON/digest 逻辑拆出来。它继续服务 Node v225 sandbox adapter dry-run package 的只读证据链，但本版核心价值是质量重构：让 `command.cpp` 回到运行命令聚合职责，避免继续膨胀。

本版不是什么：不是新的执行入口，不接入真实 managed audit，不读取 sandbox/production credential，不写 sandbox managed audit state，不执行 schema migration，不触碰 WAL / snapshot / restore 核心，也不改变 mini-kv 不是 Java order authority 的边界。

## 入口和模块

`CMakeLists.txt` 将工程版本推进到 `0.92.0`，并把 `src/managed_audit_receipts.cpp` 加入 `minikv` 静态库构建。这样 receipt formatter 和 digest 逻辑是正式产物的一部分，而不是测试侧辅助代码。

`include/minikv/managed_audit_receipts.hpp` 是本版新增的窄接口。它只暴露六组函数：

- restore boundary receipt 的 JSON 和 digest
- non-authoritative storage receipt 的 JSON 和 digest
- command dispatch quality receipt 的 JSON 和 digest
- adapter shell non-storage guard receipt 的 JSON 和 digest
- external adapter non-participation receipt 的 JSON 和 digest
- sandbox adapter non-participation receipt 的 JSON 和 digest

这些函数没有写路径、没有执行命令、没有访问 Store/WAL/Snapshot。它们只是把固定边界字段格式化成 runtime evidence JSON，并用 `runtime_evidence::digest()` 计算稳定 `fnv1a64` 指纹。

`src/managed_audit_receipts.cpp` 承接原本散在 `command.cpp` 里的结构体常量、字段顺序和 digest 输入顺序。v92 仍保留历史消费链：restore boundary 消费 v85，non-authoritative storage 消费 v86，command dispatch 消费 v87，adapter shell 消费 v88，external adapter 消费 v89，sandbox adapter 消费 v90。当前产物路径统一推进到 `c/92/`，所以 digest 也随版本和路径更新。

`src/command.cpp` 现在只 include `minikv/managed_audit_receipts.hpp`，并在 `format_info_json()` / `format_smoke_json()` 里调用该模块。换句话说，`command.cpp` 仍负责运行命令输出的拼装，但不再拥有 managed audit receipt 的字段常量和摘要细节。

## 字段怎么理解

控制面读到 `read_only=true`、`execution_allowed=false`、`restore_execution_allowed=false` 时，应理解为 mini-kv 只提供运行证据，不允许把这些 receipt 当作执行许可。

`sandbox_adapter_storage_backend=false`、`participates_in_sandbox_adapter=false`、`credential_value_read_allowed=false`、`production_credential_read_allowed=false`、`schema_migration_execution_allowed=false` 和 `sandbox_managed_audit_state_write_allowed=false` 是 v92 继续保护的 sandbox 边界：mini-kv 不成为 sandbox audit storage，不读凭据，不执行 migration，不写 managed audit state。

`order_authoritative=false` 仍然说明 mini-kv 不进入 Java 订单权威链路。`managed_audit_write_executed=false`、`load_restore_compact_executed=false`、`write_handler_changed=false`、`admin_handler_changed=false` 和 `wal_snapshot_restore_touched=false` 共同证明本版只是 formatter 拆分，不是写/admin/restore 行为改变。

## 测试和证据

`tests/command_tests.cpp` 与 `tests/readonly_fixture_tests.cpp` 继续从真实 `SMOKEJSON` / `INFOJSON` 输出检查 v92 digest、归档路径和边界布尔值。它们证明 formatter 拆出去后，运行输出仍保持原有字段契约。

`tests/release_verification_manifest_tests.cpp` 新增对 `include/minikv/managed_audit_receipts.hpp`、`src/managed_audit_receipts.cpp`、`src/command.cpp` 和 CMake 源文件列表的断言，确认本版拆分被 release fixture 和构建系统同时记录。

`tests/runtime_smoke_evidence_tests.cpp` 继续保护 runtime smoke fixture，并新增 “managed audit receipt formatter split only” 和 “no INFOJSON/SMOKEJSON field contract expansion from formatter split” 边界。这样后续 Node 读到 fixture 时能明确：这是代码组织证据，不是新的 managed audit 后端。

真实验证使用 CLion MinGW 工具链：CMake configure/build 通过，CTest 31/31 通过，TCP smoke 真实启动 `minikv_server` 后读取 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH` 和 `GET restore:real-read-token`。归档位于 `c/92/图片/` 与 `c/92/解释/说明.md`，smoke 结束后服务端已停止。

一句话总结：v92 把 managed audit receipt 的格式化和摘要职责从 `command.cpp` 拆成独立模块，既保留 Node/Java/mini-kv 松耦合证据链，也把下一轮继续演进的维护压力降下来。
