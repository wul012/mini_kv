# 第八十一次讲解：第三十九版测试、README、归档和整体增删改

第八十次讲了 v39 的核心代码：

```text
MetricsFileWriter
--metrics-file-max-bytes
--metrics-file-keep
metrics 文件轮转和保留
```

这一篇讲配套修改：

```text
CMakeLists.txt
tests/metrics_file_tests.cpp
README.md
a/39/解释/说明.md
a/39/图片/
代码讲解记录/README.md
```

一句话总结：

```text
v39 把 metrics 文件轮转补成工程闭环：新 target、新单测、真实 server/client smoke、README、运行归档和代码讲解记录同步更新。
```

---

# 1. CMakeLists.txt 增加了什么

项目版本号从：

```cmake
project(mini_kv VERSION 0.38.0 LANGUAGES CXX)
```

更新为：

```cmake
project(mini_kv VERSION 0.39.0 LANGUAGES CXX)
```

`minikv` 静态库新增源码：

```text
src/metrics_file.cpp
```

新增测试 target：

```text
minikv_metrics_file_tests
```

并注册到 CTest：

```cmake
add_test(NAME metrics_file_tests COMMAND minikv_metrics_file_tests)
```

这样轮转逻辑不是只靠手工 smoke，而是每次 CI 都会跑。

---

# 2. metrics_file_tests 覆盖哪些边界

`tests/metrics_file_tests.cpp` 使用临时目录，不污染项目文件。

第一组测试：

```text
max_bytes = 90
keep_files = 2
连续写入 4 行 metrics
```

验证结果：

```text
主文件存在，并保存最新一行
.1 存在，并保存上一轮
.2 存在，并保存再上一轮
.3 不存在，说明 keep=2 生效
最旧的 sample=one 已经被淘汰
```

第二组测试：

```text
keep_files = 0
```

验证轮转时不保留旧文件：

```text
主文件保存最新内容
.1 不存在
旧内容不再出现
```

第三组测试：

```text
不设置 max_bytes
```

验证保持 v37 行为：

```text
所有 metrics 行都写进同一个文件
不会生成 .1
```

最后还验证两个非法配置：

```text
max_bytes = 0
空 path
```

它们都会抛出 `std::invalid_argument`。

---

# 3. README 更新点

README 的当前版本改为：

```text
Version 39 is a runnable in-memory KV service with rotated metrics file export
```

功能列表新增：

```text
Optional --metrics-file-max-bytes and --metrics-file-keep rotate and retain bounded metrics export files
```

运行示例新增：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 10000 --metrics-file data\server.metrics.log --metrics-file-max-bytes 10485760 --metrics-file-keep 5
```

TCP server options 新增两项：

```text
--metrics-file-max-bytes bytes
--metrics-file-keep count
```

Roadmap 也顺延：

```text
1. Add optional client key cache refresh from a future key-listing command.
2. Add optional structured JSONL metrics export for log collectors that prefer schema-stable fields.
```

因为 metrics 文件保留和轮转这一项已经在 v39 完成。

---

# 4. 真实 smoke 做了什么

v39 的真实运行验证不是只跑单元测试。

流程是：

```text
启动真实 minikv_server.exe
开启 --metrics-interval-ms 50
开启 --metrics-file
设置 --metrics-file-max-bytes 240
设置 --metrics-file-keep 2
用真实 minikv_client.exe 连接
发送 PING / SET / STATSJSON / QUIT
检查 metrics 主文件和 .1 / .2
确认 .3 不存在
停止 server 进程
```

验证重点：

```text
server 能正常启动
client 能正常收发命令
STATSJSON 仍然返回完整状态
metrics 文件确实发生轮转
keep=2 没有保留第三份旧文件
```

这证明新增逻辑没有破坏真实 TCP 路径。

---

# 5. a/39 归档内容

第三十九版归档放在：

```text
a/39/图片/
a/39/解释/说明.md
```

截图计划包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-metrics-file-tests.png
05-metrics-rotation-smoke.png
06-version-docs-check.png
07-code-explanation-check.png
08-automation-check.png
09-version-scope-rule-check.png
```

这些截图分别证明：

```text
CMake 配置通过
全目标构建通过
CTest 全量通过
新增 metrics_file_tests 单测通过
真实 server/client 轮转 smoke 通过
版本号、README、源码、测试一致
代码讲解记录补齐
自动化仍然是 10 分钟 heartbeat
版本工作量规则已写入 AGENTS.md
```

---

# 6. 整体增删改清单

新增：

```text
include/minikv/metrics_file.hpp
src/metrics_file.cpp
tests/metrics_file_tests.cpp
代码讲解记录/80-metrics-file-rotation.md
代码讲解记录/81-version-39-tests-docs.md
a/39/
```

修改：

```text
AGENTS.md
CMakeLists.txt
src/server_main.cpp
README.md
代码讲解记录/README.md
```

核心行为变化：

```text
没有 --metrics-file-max-bytes
 -> 仍然和 v37 一样写单个 metrics 文件

有 --metrics-file-max-bytes
 -> 写入前按大小轮转

有 --metrics-file-keep N
 -> 最多保留 N 份旧文件
```

---

# 7. 一句话总结

第三十九版是一次偏运维成熟度的推进：

```text
metrics 原来能看、能导出；
现在还能长期运行而不让导出文件无限增长。
```

这让后续压测、自动化巡检和长时间运行调试更稳。
