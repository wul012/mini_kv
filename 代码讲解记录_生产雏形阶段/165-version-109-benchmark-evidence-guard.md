# mini-kv v109 benchmark evidence guard

本版目标是做一个轻量质量优化：让 `minikv_benchmark` 除了原来给人看的表格，也能用 `--evidence-json` 输出一行机器可读 evidence，方便 CI 或本地 smoke 明确确认 benchmark 没有越界。

这不是新的 runtime 命令，不是性能承诺，不是生产压测报告，不打开 TCP server，不启用 WAL，不执行 `LOAD`、`COMPACT`、`RESTORE`、snapshot，也不读取 credential 或连接 managed audit。它只是本地进程内 benchmark 的证据守卫。

## 入口

入口仍然是独立可执行文件：

```text
minikv_benchmark.exe [--evidence-json] [operations] [key_count]
```

不带 `--evidence-json` 时，原来的人工可读输出保持不变：`store.set`、`store.get`、`command.set`、`command.get`、`store.erase` 仍按表格打印耗时和吞吐。

带 `--evidence-json` 时，程序输出一行 JSON。它包含 `evidence_type=mini-kv-benchmark-evidence-guard.v1`、`release_version=v109`、`artifact_path_hint=c/109/`，以及每个 measurement 的操作数、耗时和吞吐。

## 边界字段

JSON 里最重要的不是吞吐数字，而是边界字段：

- `benchmark_mutates_ephemeral_store=true`
- `persistent_storage_write_allowed=false`
- `wal_enabled=false`
- `network_started=false`
- `snapshot_executed=false`
- `restore_executed=false`
- `load_restore_compact_executed=false`
- `credential_value_read=false`
- `managed_audit_connection_opened=false`
- `managed_audit_storage_backend=false`
- `order_authoritative=false`

控制面读到这些字段时应该理解为：benchmark 可以改本进程里的临时 `Store`，但不能被解释成 WAL 写入、网络连接、恢复演练、credential 检查、managed audit backend 或 Java order authority。

## 代码位置

`src/benchmark_main.cpp` 增加了三块小逻辑：

1. 参数解析支持可选 `--evidence-json`，位置可以在数字参数前。
2. `json_string()` 与 `print_result_json()` 负责把 benchmark result 安全格式化成 JSON 字段。
3. `print_evidence_json()` 汇总版本、边界、命中数、最终 store size 和 measurement 列表。

原有 benchmark 的核心测量流程没有拆换，仍然使用同一个 `Store` 和 `CommandProcessor` 运行 `SET` / `GET` 路径。

## 测试

`CMakeLists.txt` 注册了 `benchmark_evidence_guard`：

```text
minikv_benchmark --evidence-json 12 4
```

CTest 用 `PASS_REGULAR_EXPRESSION` 检查输出里必须同时存在：

- `evidence_type`
- `wal_enabled=false`
- `network_started=false`
- `restore_executed=false`
- `managed_audit_connection_opened=false`
- `final_store_size=0`

这类测试不是为了证明性能数值稳定，而是防止 evidence 输出丢失边界字段，或 benchmark 意外留下临时数据。

## 归档

本版运行截图和说明写入：

- `c/109/图片/`
- `c/109/解释/说明.md`

归档包含 CMake configure、build、全量 CTest、以及真实 benchmark evidence smoke。smoke 使用 `ConvertFrom-Json` 解析输出，确认 release、操作数、measurement 数量和 no-WAL/no-network/no-restore 边界。

## 一句话总结

v109 把 mini-kv 的 benchmark 从“人能看”推进到“机器能守住边界”，为 Node v248 / Java v100 / mini-kv v109 这一轮质量优化提供低风险 C++ 侧证据，同时不改变任何存储、恢复或跨项目权限语义。
