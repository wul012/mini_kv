# 第九十次讲解：第四十四版 INFO 与版本注入核心

第四十四版接上三项目路线图，只推进 mini-kv 的一个小闭环：

```text
mini-kv 暴露可被外部控制面识别的基础元信息。
```

涉及核心文件：

```text
CMakeLists.txt
include/minikv/version.hpp.in
include/minikv/command.hpp
src/command.cpp
src/main.cpp
src/tcp_server.cpp
```

一句话总结：

```text
v44 从 CMake project version 生成 minikv/version.hpp，并新增 INFO 命令返回 version、protocol、uptime、live_keys、wal_enabled、metrics_enabled 和 max_request_bytes。
```

---

# 1. 为什么要先做 INFO

Node 控制面后续要识别 mini-kv，光靠：

```text
PING
HEALTH
STATSJSON
```

还不够。

这些命令更偏运行状态，而不是“我是谁、能力边界是什么”。

所以 v44 新增：

```text
INFO
```
它是给外部观察台看的轻量身份信息。

---

# 2. CMake 版本注入

`CMakeLists.txt` 从：

```cmake
project(mini_kv VERSION 0.43.0 LANGUAGES CXX)
```

更新为：

```cmake
project(mini_kv VERSION 0.44.0 LANGUAGES CXX)
```

并新增：

```cmake
configure_file(
    ${PROJECT_SOURCE_DIR}/include/minikv/version.hpp.in
    ${PROJECT_BINARY_DIR}/generated/include/minikv/version.hpp
    @ONLY
)
```

模板文件是：

```cpp
inline constexpr std::string_view version = "@PROJECT_VERSION@";
```

生成后，代码里可以直接用：

```cpp
minikv::version
```

这样版本号来自 CMake，不需要在源码里手写第二份。

---

# 3. 为什么不用硬编码字符串

如果直接在 `command.cpp` 写：

```cpp
"0.44.0"
```

短期能跑，但以后每版都容易忘记同步。

v44 改成：

```text
CMake project version
 -> generated include/minikv/version.hpp
 -> INFO 输出 version
```

以后版本号只改 CMake 一处。

---

# 4. CommandRuntimeInfo

`include/minikv/command.hpp` 新增：

```cpp
struct CommandRuntimeInfo {
    std::string protocol = "inline";
    std::chrono::steady_clock::time_point started_at = std::chrono::steady_clock::now();
    std::size_t max_request_bytes = 0;
    bool metrics_enabled = false;
};
```

它放进：

```cpp
CommandProcessorOptions
```

原因是 `CommandProcessor` 本身不知道它运行在什么环境：

```text
CLI
 -> protocol=cli

TCP server
 -> protocol=inline,resp
 -> max_request_bytes 来自 TcpServer::Options
 -> metrics_enabled 来自 metrics_log_interval
 -> started_at 来自 server.run()
```

用 options 注入，能保持命令处理器本身可测试。

---

# 5. INFO 输出格式

`src/command.cpp` 新增：

```text
INFO
```

输出样例：

```text
version=0.44.0 protocol=inline,resp uptime_seconds=0 live_keys=1 wal_enabled=no metrics_enabled=yes max_request_bytes=4096
```

字段含义：

```text
version
 -> CMake 注入版本

protocol
 -> cli 或 inline,resp

uptime_seconds
 -> 从运行时 started_at 到当前的秒数

live_keys
 -> 当前未过期 key 数量

wal_enabled
 -> 本轮 CommandProcessor 是否带 WAL

metrics_enabled
 -> server 是否启用周期 metrics

max_request_bytes
 -> server TCP pending request 上限，CLI 中为 0
```

---

# 6. server 侧如何传 runtime info

`src/tcp_server.cpp` 在 `run()` 中记录：

```cpp
const auto server_started_at = std::chrono::steady_clock::now();
```

每个客户端连接创建 `CommandProcessor` 时传入：

```text
protocol=inline,resp
started_at=server_started_at
max_request_bytes=options_.max_request_bytes
metrics_enabled=metrics_log_interval > 0
```

这样不同连接执行 `INFO` 时，看到的是同一个 server uptime，而不是单个连接的 uptime。

---

# 7. CLI 侧如何处理

`src/main.cpp` 给本地 CLI 设置：

```text
protocol=cli
```

其他字段保留默认：

```text
max_request_bytes=0
metrics_enabled=no
```

这样本地 CLI 也能执行 `INFO`，只是输出的是 CLI 运行环境。

---

# 8. Tab 补全

`src/line_editor.cpp` 的默认命令候选也加入：

```text
INFO
```

这样 bundled TCP client 里输入：

```text
INF<Tab>
```

可以补全为：

```text
INFO
```
