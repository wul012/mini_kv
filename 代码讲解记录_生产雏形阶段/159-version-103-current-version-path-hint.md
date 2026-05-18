# mini-kv v103 current version path hint cleanup

本版目标很小：把生产代码里反复出现的当前运行证据路径提示收敛成一个共享常量。它不是新的执行入口，不滚动 Node v239 正在消费的 v102 runtime evidence，不写 storage，不执行 LOAD / COMPACT / RESTORE / SETNXEX，不读取 credential，也不改变 Java order authority 边界。

为什么做：v102 之后，`c/102/` 同时出现在 managed audit receipts、runtime evidence receipts 和 operator-window response formatter 中。字段值本身是契约，需要继续对外输出；但生产代码不应该让同一个当前路径提示散落多处，否则下一次滚动证据时容易漏改。

## 常量归属

`include/minikv/version.hpp.in` 现在除了 CMake 注入的 `version`，还提供：

```cpp
inline constexpr std::string_view current_version_path_hint = "c/102/";
```

它放在 version 生成头旁边，是因为这个值描述当前被运行时证据链引用的版本归档路径。v103 只抽常量，不把 Node 计划中的 v102 operator-window evidence 改成新证据版本。

## 生产代码变化

`src/managed_audit_receipts.cpp` 里 8 个当前路径字段改为使用 `current_version_path_hint`：restore boundary、non-authoritative storage、command dispatch quality、adapter shell guard、external adapter non-participation、sandbox adapter non-participation、sandbox connection echo marker、sandbox connection no-start guard。

`src/runtime_evidence_receipts.cpp` 里 binary provenance、retention provenance check、retention replay marker 的当前路径提示也改为同一个常量。

`src/command_response_formatters.cpp` 里 `runtime_no_start_no_write_follow_up` 和 `operator_window_no_start_no_write_receipt` 的 `current_artifact_path_hint` 改为通过 `json_string(current_version_path_hint)` 输出。输出字段仍是 `"c/102/"`，只是来源不再硬编码在 formatter 字符串里。

## 测试与契约

测试继续断言最终 JSON 中存在 `"current_artifact_path_hint":"c/102/"`，这是有意保留的外部契约检查。也就是说，生产代码减少重复，测试仍站在 Node/Java 消费者视角确认输出没有漂移。

本版验证包括 CMake configure/build、全量 CTest `35/35` 通过，以及真实 TCP smoke。smoke 启动 `minikv_server`，通过 `minikv_client` 读取 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH` 和 `GET restore:real-read-token`；结果证明 `operator_window_no_start_no_write_receipt` 仍输出 v102 path hint，`GET restore:real-read-token` 仍为 `(nil)`，没有发生 restore/load/compact/SETNXEX 或 managed audit 写入。

归档截图放在 `c/103/图片/`，说明放在 `c/103/解释/说明.md`。验证结束后本版启动的 server 已停止，临时日志目录会在最终清理时删除。

一句话：v103 把当前证据路径提示从多处硬编码收敛到一个常量，让后续滚动证据更不容易漏改，同时保持 Node v239 需要的 v102 no-start/no-write 证据契约不变。
