# mini-kv v123 SMOKEJSON command test split

## 本版目标

v123 是一个测试结构质量重构版。目标是把 `tests/command_tests.cpp` 中过长的 live `SMOKEJSON` 回执断言拆到独立测试文件，让主命令测试重新聚焦基础命令、契约命令、metrics、HELP 和错误处理。

它不是新功能，不改变 `SMOKEJSON` 字段，不改 fixture，不新增 runtime evidence，不改 resolver/credential/managed audit 边界，也不改变任何写命令或 WAL/restore 行为。

## 为什么拆

v122 结束后，最大的问题不是生产文件继续膨胀，而是 `tests/command_tests.cpp` 已经到 1029 行。这个文件同时承担基础命令行为、JSON 契约、runtime smoke 证据、credential resolver receipt、metrics 和 HELP 覆盖，阅读成本开始过高。

本版选择最小风险拆法：只移动一段已经稳定的 live `SMOKEJSON` 断言，不抽 helper、不改生产代码、不重排无关测试。

## 文件职责

`tests/command_tests.cpp` 仍保留命令层基础覆盖：

- `SMOKEJSON extra` 的 usage 错误。
- `SMOKEJSON` 的基本只读字段：`read_only`、`execution_allowed`、`restore_execution_allowed`、`order_authoritative`、`runtime_smoke`、版本号、live key 数。
- `GET restore:real-read-token` 仍为 `(nil)`，确认命令测试没有制造恢复 token。
- 后续 `CHECKJSON`、WAL contract、inventory、metrics、HELP 等原有覆盖不变。

`tests/smokejson_command_receipt_tests.cpp` 是新增的专门测试：

- 构造一个含 1 个 key 的 `CommandProcessor`。
- 执行 `SMOKEJSON extra` 确认 usage 行为。
- 执行 `SMOKEJSON` 并覆盖 runtime smoke 的长回执断言。
- 继续检查 v122 的 credential resolver approval-required implementation readiness non-participation receipt。
- 最后检查 `GET restore:real-read-token` 仍为 `(nil)`。

`CMakeLists.txt` 增加：

```text
minikv_smokejson_command_receipt_tests
smokejson_command_receipt_tests
```

这让拆出的文件进入常规 CTest，不依赖人工单独运行。

## 边界保持

这次拆分只移动测试断言，生产路径没有变化：

- 不改 `src/command.cpp`。
- 不改 `src/command_response_formatters.cpp`。
- 不改任何 `runtime_*_receipts.cpp`。
- 不改 release fixture 或 manifest。
- 不启动 TCP server。

控制面仍按 v122 的含义理解：`read_only=true`、`execution_allowed=false`、`order_authoritative=false`、`ready_for_java_v116_mini_kv_v122_echo=true` 只表示可供后续验证，不表示 mini-kv 可以执行 resolver、secret provider、endpoint 连接、schema migration 或 storage write。

## 验证

本版用 `cmake-build-v123` 验证：

```text
cmake -S . -B cmake-build-v123 -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-v123 --parallel 2
ctest --test-dir cmake-build-v123 --output-on-failure -R 'command_tests|smokejson_command_receipt_tests'
ctest --test-dir cmake-build-v123 --output-on-failure
```

目标测试 2/2 通过，全量测试 52/52 通过。

## 行数变化

- `tests/command_tests.cpp`: 1029 行降至 792 行。
- `tests/smokejson_command_receipt_tests.cpp`: 新增 284 行。
- `src/command.cpp`: 保持 800 行。
- `src/command_response_formatters.cpp`: 保持 769 行。

## 一句话总结

v123 把 live `SMOKEJSON` 回执断言从大命令测试里拆出来，让命令测试和 evidence receipt 测试各守各的边界，同时保持 mini-kv 的运行行为、只读证据契约和跨项目边界完全不变。
