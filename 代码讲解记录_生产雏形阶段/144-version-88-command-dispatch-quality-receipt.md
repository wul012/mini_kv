# 144. v88 command dispatch quality receipt

## 本版目标

v88 的目标是给 Node v219 managed audit adapter implementation precheck 一份 mini-kv 侧只读质量回执：Node 可以确认 mini-kv 的 runtime evidence 命令已经从主命令函数中拆出成独立读侧分发路径，同时确认这次没有改写命令、admin 命令、WAL、snapshot、restore，也没有把 mini-kv 变成 managed audit store 或 Java 订单权威。

这不是新的执行入口，不是 handler table 大迁移，不开放 `LOAD` / `COMPACT` / `SETNXEX` / `RESTORE`，不写 managed audit 状态，不进入 Java transaction chain。它的角色更像一张边界收据：告诉控制面“读证据命令已经被整理过，整理范围可核验，危险路径没有被动到”。

## 本版所处阶段

mini-kv 当前已经不是单纯的 C++ KV 练习，而是带 TCP/RESP、WAL、Snapshot、runtime evidence、release fixture、真实 smoke 归档和跨项目控制面消费契约的生产雏形。v88 的推进方式仍然保持谨慎：只在命令分发的读侧证据区域做小步整理，用测试和真实 TCP smoke 证明行为没有漂移。

## 核心代码入口

### 1. `include/minikv/command.hpp`

本版新增了私有方法：

```cpp
CommandResult execute_runtime_evidence_command(std::string_view command, std::istringstream& input);
```

它仍然只被 `CommandProcessor::execute_trimmed` 内部调用，外部 API 没有变化。参数保留 `std::istringstream&`，是为了继续使用现有 `has_extra_token` 解析规则，避免读侧分发整理引入新的 token 解析语义。

### 2. `src/command.cpp`

新增的 `CommandDispatchFamily` 和 `dispatch_family_for` 用来给命令做低风险分类：

```text
RuntimeEvidence:
  STATS, STATSJSON, SMOKEJSON, STORAGEJSON, HEALTH, INFO, INFOJSON, COMMANDS, COMMANDSJSON

Session:
  EXIT, QUIT

Write/Admin/Read/Meta:
  继续从 command_catalog 的 category 判断
```

真正的运行时证据命令处理被收进 `execute_runtime_evidence_command`。它只搬走原来已经存在的读侧分支：统计、健康、信息、命令目录、storage evidence、smoke evidence。写命令、admin 命令、WAL、snapshot、restore 仍然留在原来的路径上，没有被这次重构触碰。

## 新增回执

本版新增 `RuntimeCommandDispatchQualityReceipt`，并通过 `format_runtime_command_dispatch_quality_receipt_json` 输出到 `INFOJSON` 和 `SMOKEJSON`。

关键字段的控制面含义如下：

```text
consumer = Node v219 managed audit adapter implementation precheck packet
consumed_by = Node v217 managed audit adapter production-hardening readiness gate
consumed_release_version = v87
consumed_artifact_path_hint = c/87/
consumed_receipt_digest = fnv1a64:111f0daf1283eab6
current_artifact_path_hint = c/88/
receipt_digest = fnv1a64:4aa6d12fb067e2a6
dispatch_family = runtime_evidence_command_family
dispatch_split_applied = true
handler_table_required = false
write_handler_changed = false
admin_handler_changed = false
wal_snapshot_restore_touched = false
behavior_changed = false
fixture_contract_preserved = true
read_only = true
execution_allowed = false
order_authoritative = false
```

这些字段的重点不是“mini-kv 多了一个能力”，而是“mini-kv 明确说明自己没有越界”。Node 读到它后，只能把它当作 managed audit adapter 前置检查的只读证据，不能把它解释为允许写入、允许 restore、允许 admin 操作或允许把 mini-kv 接入 Java 订单一致性链。

## Fixture 和文档同步

`fixtures/readonly/infojson-empty-inline.json` 重新对齐了 v88 的真实 `INFOJSON` 输出，包含新的 `command_dispatch_quality_receipt`。

`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 也加入了该回执，并把 Node 目标推进到 `Node v219 managed audit adapter implementation precheck packet`。它们同时保留 v87 的 `managed_audit_adapter_non_authoritative_storage_receipt`，因为那是历史消费链的一环，不能被 v88 新目标覆盖。

`README.md` 的 Current version 和 release fixture 说明同步强调两层关系：历史 v217 回执证明 mini-kv 不是 managed audit store；v88 新回执证明这次只做 runtime evidence command dispatch cleanup，不改写/admin/restore 路径。

## 测试保护

`command_tests.cpp` 继续从真实 `SMOKEJSON` 输出检查 runtime evidence 字段，并新增对 `command_dispatch_quality_receipt` 的 digest、消费来源、分发范围和边界布尔值断言。

`readonly_fixture_tests.cpp` 保护 `INFOJSON` fixture，确保只读身份输出和真实命令输出一致。

`runtime_smoke_evidence_tests.cpp` 保护 release runtime smoke fixture，确认 Node v219 需要核验的新回执、digest、边界字段和 `node_must_verify` 内容都在 fixture 中。

`release_verification_manifest_tests.cpp` 保护总验收清单，确保 v88 的构建、测试、真实 smoke、fixture inventory 和 command dispatch quality receipt 被同一份 manifest 收口。

一批历史 release fixture 测试也同步把 runtime 版本断言更新到 `0.88.0`，这是因为它们会读取当前 `INFOJSON` / `SMOKEJSON` 输出，版本字段必须跟当前构建一致。

## 真实运行和归档

本版验证使用 CLion CMake/CTest 工具链：

```text
cmake -S . -B cmake-build-v88 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-v88 --parallel 2
ctest --test-dir cmake-build-v88 --output-on-failure
```

结果是 31/31 测试通过。

真实 TCP smoke 启动 `minikv_server 6428 127.0.0.1`，再用 `minikv_client` 发送：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

smoke 证明 `command_dispatch_quality_receipt` 能从真实 TCP 路径读到，`receipt_digest=fnv1a64:4aa6d12fb067e2a6`，`dispatch_split_applied=true`，`write_handler_changed=false`，`admin_handler_changed=false`，`wal_snapshot_restore_touched=false`，`behavior_changed=false`，`GET restore:real-read-token` 仍然是 `(nil)`。服务端验证后已停止。

归档文件在：

```text
c/88/图片/
c/88/解释/说明.md
```

## 一句话总结

v88 把 runtime evidence 命令分发从主命令处理里拆出一个可核验的小边界，并用只读回执告诉 Node/Java/mini-kv 的松耦合融合链路：这次整理提高了可维护性，但没有扩大 mini-kv 的权力。
