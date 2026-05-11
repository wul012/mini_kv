# 第八十七次讲解：第四十二版测试、README、归档和整体增删改

第八十六次讲了 v42 的核心：

```text
--metrics-file-format text|jsonl
MetricsExportFormat
metrics_json_line
复用 format_command_metrics_json
```

这一篇讲配套修改：

```text
CMakeLists.txt
README.md
tests/tcp_server_tests.cpp
a/42/解释/说明.md
a/42/图片/
代码讲解记录/README.md
```

一句话总结：

```text
v42 把 JSONL metrics file 做成完整工程闭环：接口、实现、单测、真实进程 smoke、README、归档和代码讲解同步完成。
```

---

# 1. CMakeLists.txt 版本号

项目版本号从：

```cmake
project(mini_kv VERSION 0.41.0 LANGUAGES CXX)
```

更新为：

```cmake
project(mini_kv VERSION 0.42.0 LANGUAGES CXX)
```

本轮没有新增 target。

所有逻辑都接在已有库和已有测试 target 上。

---

# 2. tcp_server_tests 新增覆盖

`tests/tcp_server_tests.cpp` 新增一个独立的 JSONL exporter 场景。

测试流程：

```text
启动 TcpServer
metrics_log_interval = 20ms
metrics_export_format = jsonl
logger 收 stdout text
metrics_exporter 收 jsonl
发送 PING / QUIT
停止 server
检查 text logs 和 jsonl exports
```

关键断言：

```text
stdout logs 仍然包含 event=server_metrics
metrics exports 包含 "event":"server_metrics"
metrics exports 包含 "event":"tcp_stop"
metrics exports 包含 "connection_stats"
metrics exports 包含 "commands"
metrics exports 包含 "metrics_interval_ms":20
metrics exports 包含 "command":"PING"
metrics exports 不包含 event=server_metrics 这种 text 格式
```

这样证明：

```text
stdout text 没变
metrics exporter 可以切 JSONL
周期 metrics 和最终 stop 都能导出 JSONL
```

---

# 3. 真实 smoke 做了什么

真实运行验证使用：

```text
minikv_server.exe
minikv_client.exe
--metrics-file-format jsonl
```

流程：

```text
启动 server
配置 --metrics-interval-ms 100
配置 --metrics-file tmp-v42-smoke/server.metrics.jsonl
配置 --metrics-file-format jsonl
client 发送：
  PING jsonl
  SET alpha one
  STATSJSON
  QUIT
检查 metrics jsonl 文件
```

期望结果：

```text
server stdout 仍是 event=server_metrics text 行
metrics file 中每一行是 JSON object
文件包含 "event":"server_metrics"
文件包含 "commands"
文件包含 "metrics_interval_ms":100
```

这个 smoke 证明命令行入口、真实 server、真实 client 和 metrics file 写入链路是连通的。

---

# 4. README 更新点

README 当前版本更新为：

```text
Version 42 is a runnable in-memory KV service with optional JSONL metrics file export
```

功能列表新增：

```text
Optional --metrics-file-format jsonl writes metrics export records as schema-stable JSON Lines while keeping stdout lifecycle logs in text form
```

server 示例新增：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 10000 --metrics-file data\server.metrics.jsonl --metrics-file-format jsonl
```

server options 说明新增：

```text
--metrics-file-format text|jsonl
```

Roadmap 顺延到下一组较小目标：

```text
1. Add optional key-list filtering or prefix matching for larger datasets.
2. Add small client-side command aliases for frequent operational checks.
```

---

# 5. a/42 归档内容

第四十二版归档放在：

```text
a/42/图片/
a/42/解释/说明.md
```

截图计划：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-tcp-server-tests.png
05-jsonl-metrics-smoke.png
06-version-docs-check.png
07-code-explanation-check.png
08-automation-check.png
```

这些截图分别证明：

```text
CMake 配置通过
全目标构建通过
CTest 全量通过
新增 JSONL exporter 单测通过
真实 server/client JSONL metrics file smoke 通过
版本号、README、源码和测试一致
代码讲解记录补齐
自动化配置仍然存在
```

---

# 6. 整体增删改清单

修改：

```text
CMakeLists.txt
README.md
include/minikv/command.hpp
include/minikv/tcp_server.hpp
src/command.cpp
src/tcp_server.cpp
src/server_main.cpp
tests/tcp_server_tests.cpp
代码讲解记录/README.md
```

新增：

```text
代码讲解记录/86-metrics-jsonl-export.md
代码讲解记录/87-version-42-tests-docs.md
a/42/图片/*.png
a/42/解释/说明.md
```

删除：

```text
无源码删除。
本轮结束前删除 build-v42/ 和 tmp-v42-smoke/。
```

---

# 7. 本轮工作量控制

v42 没有继续做 key prefix filter，也没有增加新命令。

原因是这轮已经包含：

```text
新增 CLI 参数
新增导出格式
新增 JSON 序列化路径
新增单测
新增真实进程 smoke
文档和归档同步
```

这已经是一个完整版本的合适大小。
