# mini-kv v104 managed audit receipt implementation split

本版目标很小，也很明确：把 `src/managed_audit_receipts.cpp` 里那组 900+ 行的 managed audit receipt 实现拆开，降低单文件压力，同时不改 public header，不改 JSON/digest 形状，不改 Node v239 还在消费的 v102 no-start/no-write evidence 链。

这不是新的执行入口，不是新的 managed audit 能力，也不是把 mini-kv 往 write/admin/restore/credential/schema/auto-start 方向推一步。它只是把已经存在的只读证据实现，按边界职责拆成更小的编译单元。

## 本版位置

- `include/minikv/managed_audit_receipts.hpp`: 对外仍然只暴露 receipt 的 digest / JSON formatter 声明。
- `src/managed_audit_core_receipts.cpp`: 放 restore boundary、non-authoritative storage、command dispatch quality、adapter shell guard 这组基础 receipts。
- `src/managed_audit_adapter_receipts.cpp`: 放 external adapter / sandbox adapter 的非参与 receipts。
- `src/managed_audit_connection_receipts.cpp`: 放 sandbox connection echo marker 和 no-start guard receipts。
- `src/managed_audit_receipt_utils.hpp`: 放 shared 的 JSON 小工具，避免四个实现文件重复。
- `src/managed_audit_receipts.cpp`: 只保留一个很薄的协调入口。

## 关键契约

核心边界字段没有动：

- `read_only` 仍然维持只读。
- `execution_allowed` 仍然是 false。
- `order_authoritative` 仍然是 false。
- `current_version_path_hint` 仍然统一来自版本常量，不再在各个 formatter 里散落硬编码。

这次拆分也没有扩大 `INFOJSON` / `SMOKEJSON` 的对外字段，Node 看到的 receipt digest、JSON 字段名、边界说明、节点动作都还是原来的意思。

## 测试与验证

这版的验证分三层：

1. `cmake` configure + build：确认新的四个实现单元都能一起编译进库。
2. `ctest`：`35/35` 全过，说明 managed audit receipts 相关测试和整套现有回归没有被拆分打坏。
3. 真实 smoke：启动 `minikv_server.exe`，用 `minikv_client.exe` 读取 `INFOJSON`、`SMOKEJSON` 和 `HEALTH`，确认输出里仍然有 `0.102.0`、`OK` 和原有的 no-start/no-write 证据内容，服务也能正常接受并关闭连接。

## 归档

- 截图：`c/104/图片/01-cmake-configure.png`、`02-cmake-build.png`、`03-ctest.png`、`04-runtime-smoke.png`
- 说明：`c/104/解释/说明.md`

## 一句话总结

这版把一坨大而单一的证据实现切成了按边界分工的小单元，减少后续维护摩擦，但没有改动 mini-kv 对 Node/Java 的任何只读契约。
