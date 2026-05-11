# 第七十六次讲解：第三十七版 Server Metrics 文件导出核心

第三十七版完成 Roadmap 里的这一项：

```text
Add optional metrics file export for longer benchmark or soak-test runs.
```

也就是让 `minikv_server` 在周期性输出 `event=server_metrics` 的同时，可以把同样的指标行写入一个独立文件。

涉及的核心文件：

```text
include/minikv/tcp_server.hpp
src/tcp_server.cpp
src/server_main.cpp
```

一句话总结：

```text
v37 给 TcpServer 增加 metrics_exporter 回调，再由 server_main 的 --metrics-file 把 server_metrics / tcp_stop 指标行落到文件。
```

---

# 1. 为什么 metrics file 不做成新命令

已有命令：

```text
STATS
STATSJSON
HEALTH
```

它们适合人工或客户端主动查询。

但长时间 benchmark / soak test 更需要：

```text
服务端自己按固定间隔采样
把结果保存下来
测试结束后再统一分析
```

所以 v37 沿用 v30 开始已有的：

```text
--metrics-interval-ms
event=server_metrics
```

只新增一个输出目标：

```text
--metrics-file path
```

这样协议层不变，客户端不需要配合，老脚本也不会受影响。

---

# 2. Options 新增 metrics_exporter

`include/minikv/tcp_server.hpp` 里，`TcpServer::Options` 原来有：

```cpp
using LogHandler = std::function<void(const std::string&)>;
LogHandler logger;
```

v37 新增：

```cpp
using MetricsExportHandler = std::function<void(const std::string&)>;
MetricsExportHandler metrics_exporter;
```

两者职责不同：

```text
logger
 -> 所有结构化生命周期日志
 -> tcp_listen / tcp_client_accepted / tcp_client_closed / server_metrics / tcp_stop

metrics_exporter
 -> 只导出指标采样相关事件
 -> server_metrics / tcp_stop
```

这样 metrics file 不会被连接生命周期细节刷屏，只保留适合长期采样的指标行。

---

# 3. export_metrics_event 的作用

`src/tcp_server.cpp` 新增：

```cpp
void export_metrics_event(const TcpServer::Options& options, const std::string& message) {
    log_event(options, message);
    if (options.metrics_exporter) {
        options.metrics_exporter(message);
    }
}
```

这个函数做两件事：

```text
1. 仍然把指标行发给原来的 logger
2. 如果配置了 metrics_exporter，再额外写一份到指标文件
```

所以 v37 不改变 stdout 行为：

```text
没有 --metrics-file
 -> 行为和 v36 一样

有 --metrics-file
 -> stdout 照常输出，文件里也多一份指标采样
```

---

# 4. 哪些事件会写入 metrics file

v37 把两个地方改成调用 `export_metrics_event`：

```text
event=server_metrics
event=tcp_stop
```

`event=server_metrics` 是周期采样：

```text
active_connections
total_connections
peak_connections
total_commands
successful_commands
error_commands
total_latency_ns
avg_latency_ns
max_latency_ns
command_breakdown
metrics_interval_ms
```

`event=tcp_stop` 是服务端停止时的最终快照：

```text
连接计数最终值
命令计数最终值
延迟统计最终值
分命令 breakdown
```

这个组合对长跑测试很实用：

```text
中途看趋势
结束看最终汇总
```

---

# 5. server_main 如何打开文件

`src/server_main.cpp` 新增命令行参数：

```text
--metrics-file path
```

解析后保存到：

```cpp
std::optional<std::string> metrics_file_path;
```

如果用户传了路径，就打开：

```cpp
metrics_file.open(*metrics_file_path, std::ios::out | std::ios::trunc);
```

这里使用 `trunc`，表示每次 server 启动都会重新生成本轮 metrics 文件。
这是偏保守的选择：

```text
一轮运行
 -> 一个清晰文件

避免旧 run 和新 run 混在一起
```

如果后面要支持保留历史，就可以在 Roadmap 的 rotation / retention 里继续做。

---

# 6. 文件写入为什么每行 flush

metrics exporter 的 lambda 是：

```cpp
options.metrics_exporter = [&metrics_file, &metrics_file_mutex](const std::string& message) {
    std::lock_guard lock{metrics_file_mutex};
    metrics_file << message << '\n';
    metrics_file.flush();
};
```

这里显式 `flush` 的原因：

```text
长时间运行时，外部工具可以 tail 文件
异常停止时，尽量保留最近一次采样
smoke test 可以马上读取文件验证
```

代价是每个 metrics interval 多一次刷新。
但 metrics interval 本来通常是秒级或更慢，这个成本可以接受。

---

# 7. 为什么保留 mutex

当前 `event=server_metrics` 和 `event=tcp_stop` 都由 server run loop 线程触发。

但写文件仍然加锁：

```text
让回调具备线程安全边界
保持和 stdout logger 的写法一致
以后如果新增别的 metrics export 触发点，不必重新审计文件写入
```

这是小成本的防御式设计。

---

# 8. server_start 也显示 metrics_file

v37 的 `event=server_start` 新增字段：

```text
metrics_file="..."
```

如果没有配置：

```text
metrics_file="none"
```

这有两个好处：

```text
运行日志能直接看到本轮有没有开启文件导出
真实 smoke 可以确认 CLI 参数被 server_main 接收
```

---

# 9. 一句话总结

第三十七版没有改变 mini-kv 的数据面和协议面，它补的是运维观测面：

```text
server_metrics 原本只在 stdout 里闪过
v37 开始可以稳定落到独立文件
```

这让后续长时间压测、回归观察、指标对比都有了更稳的材料来源。
