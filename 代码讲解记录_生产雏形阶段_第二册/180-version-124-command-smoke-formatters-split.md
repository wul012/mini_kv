# mini-kv v124 command smoke formatters split

## 本版目标

v124 继续做生产友好的结构拆分，把 `SMOKEJSON` 的大块格式化逻辑从 `src/command_response_formatters.cpp` 单独抽到 `src/command_smoke_formatters.cpp`。

这不是新增命令，也不是重写 runtime evidence；只是把一个已经稳定、体量偏大的格式化职责从通用 formatter 文件里分出去，让普通命令响应和 runtime smoke 证据各自独立。

## 为什么这样拆

`src/command_response_formatters.cpp` 原来同时承担：

- 普通命令格式化：`KEYS`、`INFO`、`STORAGEJSON`、`STATSJSON`。
- runtime smoke 大 JSON：`SMOKEJSON` 及其一堆 embedded receipt。

这会让文件变得像“格式化杂物箱”。拆分后，普通 formatter 更容易看，SMOKEJSON 相关变化也更集中。

## 新职责边界

`src/command_response_formatters.cpp` 现在保留：

- `format_keys`
- `format_walinfo`
- `format_info`
- `format_info_json`
- `format_stats_json`
- `format_storage_evidence_json`

`src/command_smoke_formatters.cpp` 现在独立负责：

- `format_smoke_json`
- `format_runtime_no_start_no_write_follow_up_json`
- `format_operator_window_no_start_no_write_receipt_json`

这两个小 helper 也一起挪过来，因为它们只服务 `SMOKEJSON`，不该继续挂在通用 formatter 文件里。

## 依赖选择

新文件没有重写 JSON escaping，而是直接复用 `runtime_evidence` 里的基础工具：

- `runtime_evidence::json_string`
- `runtime_evidence::json_bool`
- `runtime_evidence::json_string_array`

这样既减少重复代码，也避免一处 escape 逻辑分叉出两套实现。

## 对外契约

头文件 `include/minikv/command_response_formatters.hpp` 没改接口，`CommandProcessor` 调用点也没改。

这意味着：

- `SMOKEJSON` 输出字段不变。
- `runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 继续读同一份契约。
- `smokejson_command_receipt_tests` 继续守住 v122 的 approval-required implementation readiness non-participation receipt。

## 验证

本版通过 `cmake-build-v124` 验证：

```text
cmake -S . -B cmake-build-v124 -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-v124 --parallel 2
ctest --test-dir cmake-build-v124 --output-on-failure -R 'command_tests|smokejson_command_receipt_tests|release_verification_manifest_tests|runtime_smoke_evidence_tests'
ctest --test-dir cmake-build-v124 --output-on-failure
```

目标测试和全量测试都通过，说明这次只是纯结构拆分，没有行为漂移。

## 行数变化

- `src/command_response_formatters.cpp`: 769 行降到 471 行。
- `src/command_smoke_formatters.cpp`: 新增 351 行。
- `tests/command_tests.cpp`: 保持 792 行。

## 一句话总结

v124 把 SMOKEJSON 大格式化从通用 formatter 里拆出来，让命令输出格式化和 runtime smoke 证据各自独立，后续继续拆别的 formatter 或测试时会更顺手，也更不容易把一个文件拧成工具箱。
