# 第八十六次讲解：第四十二版 Metrics JSONL 导出核心

第四十二版接上 v39 的 metrics file 能力，只推进一个小闭环：

```text
metrics file 可以选择写 JSON Lines 格式。
```

涉及核心文件：

```text
include/minikv/command.hpp
include/minikv/tcp_server.hpp
src/command.cpp
src/tcp_server.cpp
src/server_main.cpp
```

一句话总结：

```text
v42 新增 --metrics-file-format text|jsonl，让日志采集器可以读取结构稳定的一行一个 JSON 对象，同时不改变 stdout 的文本生命周期日志。
```

---

# 1. 为什么只做 metrics file，不改 stdout

已有 server stdout 日志是人可读的 key-value 文本：

```text
event=server_metrics host=127.0.0.1 port=6379 ...
```

它适合直接看，也已经被测试和文档依赖。

v42 的目标不是替换它，而是给采集器多一个选择：

```text
stdout
 -> 仍然输出 text

metrics file
 -> 默认 text
 -> 可选 jsonl
```

这样兼容性最好，版本范围也刚好。

---

# 2. MetricsExportFormat

`include/minikv/tcp_server.hpp` 新增枚举：

```cpp
enum class MetricsExportFormat {
    text,
    jsonl,
};
```

并在 `TcpServer::Options` 中新增：

```cpp
MetricsExportFormat metrics_export_format = MetricsExportFormat::text;
```

默认值是 `text`，所以老命令不需要改：

```powershell
minikv_server.exe 6379 127.0.0.1 --metrics-file data\server.metrics.log
```

仍然写原来的文本格式。

---

# 3. export_metrics_event 的双通道

`src/tcp_server.cpp` 原来的导出逻辑是：

```text
log_event(options, message)
metrics_exporter(message)
```

v42 改成：

```text
log_event(options, text_message)

if metrics_export_format == text
 -> metrics_exporter(text_message)

if metrics_export_format == jsonl
 -> metrics_exporter(jsonl_message)
```

关键点是：

```text
stdout 始终走 text_message
metrics file 根据配置走 text 或 jsonl
```

这样不影响用户在终端里看服务端运行状态。

---

# 4. metrics_json_line 的结构

`src/tcp_server.cpp` 新增 `metrics_json_line`。

周期 metrics 样例：

```json
{"event":"server_metrics","host":"127.0.0.1","port":6379,"connection_stats":{"active_connections":0,"total_connections":0,"peak_connections":0},"commands":{"total_commands":0,"successful_commands":0,"error_commands":0,"total_latency_ns":0,"avg_latency_ns":0,"max_latency_ns":0,"breakdown":[]},"metrics_interval_ms":10000}
```

最终 stop 样例：

```json
{"event":"tcp_stop","host":"127.0.0.1","port":6379,"connection_stats":{"active_connections":0,"total_connections":1,"peak_connections":1},"commands":{"total_commands":2,"successful_commands":2,"error_commands":0,"total_latency_ns":1000,"avg_latency_ns":500,"max_latency_ns":700,"breakdown":[{"command":"PING","total_commands":1,"successful_commands":1,"error_commands":0,"total_latency_ns":300,"avg_latency_ns":300,"max_latency_ns":300}]}}
```

字段分组是刻意的：

```text
event / host / port
 -> 事件定位

connection_stats
 -> TCP 连接计数

commands
 -> 命令计数和延迟

metrics_interval_ms
 -> 只出现在周期 server_metrics 中
```

---

# 5. 复用 command metrics JSON

`src/command.cpp` 里原来已经有 `STATSJSON` 使用的命令指标 JSON 片段。

v42 把它从 cpp 内部函数提升为头文件可见函数：

```cpp
std::string format_command_metrics_json(const CommandProcessorMetrics& metrics);
```

这样 `tcp_server.cpp` 不需要重新写一套命令指标 JSON。

好处是：

```text
STATSJSON 的 commands 字段
metrics JSONL 的 commands 字段
 -> 使用同一套格式化逻辑
```

以后如果 command metrics 字段扩展，只需要维护一个地方。

---

# 6. CLI 入口

`src/server_main.cpp` 新增参数：

```text
--metrics-file-format text|jsonl
```

解析逻辑是：

```text
text
 -> MetricsExportFormat::text

jsonl
 -> MetricsExportFormat::jsonl

其他值
 -> 报错
```

这个选项必须和 `--metrics-file` 一起使用。

原因是它只控制 metrics file 的格式；没有 metrics file 时，谈不上文件格式。

---

# 7. server_start 日志

`server_start` 增加：

```text
metrics_file_format=text
```

或：

```text
metrics_file_format=jsonl
```

这样从启动日志就能看出本轮 metrics file 是按哪种格式写的。

这仍然是 text stdout 日志，不是 JSONL。
