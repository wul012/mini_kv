# 第八十次讲解：第三十九版 Metrics 文件轮转核心

第三十九版完成 Roadmap 里的这一项：

```text
Add retention or rotation controls for long-running metrics export files.
```

也就是让 `--metrics-file` 不再只能无限写一个文件，而是可以按文件大小轮转，并保留最近 N 份旧文件。

涉及的核心文件：

```text
include/minikv/metrics_file.hpp
src/metrics_file.cpp
src/server_main.cpp
```

一句话总结：

```text
v39 把 metrics 文件写入抽成 MetricsFileWriter，并给 server_main 增加 --metrics-file-max-bytes / --metrics-file-keep 两个参数。
```

---

# 1. 为什么单独做 MetricsFileWriter

v37 的 metrics file 逻辑直接写在 `server_main.cpp` 里：

```text
打开 ofstream
收到 metrics_exporter 回调
写一行
flush
```

这对简单导出够用，但轮转会多出一组文件系统规则：

```text
判断当前文件大小
关闭当前文件
移动旧文件 .1 / .2 / .3
删除超过保留数量的文件
重新打开主文件
```

如果继续塞在 `server_main.cpp`，启动入口会变得太重。

所以 v39 新增：

```cpp
class MetricsFileWriter
```

它只负责一件事：

```text
把 metrics 行安全写入文件，并在需要时完成轮转。
```

这样 `server_main.cpp` 仍然保持启动参数解析和 server 组装的职责。

---

# 2. MetricsFileOptions 表达什么

`include/minikv/metrics_file.hpp` 里新增：

```cpp
struct MetricsFileOptions {
    std::filesystem::path path;
    std::optional<std::uintmax_t> max_bytes;
    std::size_t keep_files = 3;
};
```

三个字段的含义：

```text
path
 -> 当前正在写的 metrics 主文件

max_bytes
 -> 轮转阈值；没有值时不轮转，保持 v37 行为

keep_files
 -> 保留多少个旧文件，默认 3
```

旧文件命名规则很直接：

```text
server.metrics.log
server.metrics.log.1
server.metrics.log.2
server.metrics.log.3
```

`.1` 永远是最近一次被轮转出去的文件。

---

# 3. 写入前为什么判断大小

核心写入函数是：

```cpp
void MetricsFileWriter::write_line(std::string_view line)
```

它先计算本次要写入的字节数：

```cpp
const auto byte_count = static_cast<std::uintmax_t>(line.size()) + 1;
```

这里 `+ 1` 是因为实际写入会补一个换行符：

```text
line + '\n'
```

如果配置了 `max_bytes`，并且当前文件已经有内容，同时：

```text
current_bytes_ + byte_count > max_bytes
```

就先轮转，再把这一行写入新的主文件。

这里特意加了 `current_bytes_ > 0`：

```text
如果单行 metrics 本身就超过 max_bytes，也允许它写入空文件。
```

否则会出现一种尴尬情况：

```text
一行太长
 -> 写入前轮转
 -> 新文件仍然放不下
 -> 下一次又轮转
```

保守处理是：至少保证每一条 metrics 行都能落盘。

---

# 4. rotate 的文件移动顺序

轮转时，先关闭当前输出流：

```cpp
output_.close();
```

如果 `keep_files == 0`：

```text
删除当前主文件
重新打开一个空主文件
```

这表示用户只要当前文件，不保留历史。

如果 `keep_files > 0`，移动顺序是从后往前：

```text
删除 path.keep
path.(keep - 1) -> path.keep
path.(keep - 2) -> path.(keep - 1)
...
path.1 -> path.2
path -> path.1
重新打开 path
```

必须从后往前移动，是为了避免覆盖较新的旧文件。

举例 `keep_files = 2`：

```text
删除 server.metrics.log.2
server.metrics.log.1 -> server.metrics.log.2
server.metrics.log -> server.metrics.log.1
新建 server.metrics.log
```

这样始终只保留最近两份轮转文件。

---

# 5. 文件系统错误如何处理

`src/metrics_file.cpp` 里没有默默吞掉文件系统错误。

例如：

```text
不能创建目录
不能删除旧 metrics 文件
不能 rename 轮转文件
不能写入 metrics 文件
```

都会抛出异常。

原因是 metrics 文件属于显式配置的运维输出：

```text
用户传了 --metrics-file
说明希望文件可用
如果路径或权限有问题，应该尽早失败，而不是悄悄丢指标
```

这和 WAL 的保守思路一致：和持久化、归档相关的失败要明确。

---

# 6. server_main 新增两个参数

`src/server_main.cpp` 新增：

```text
--metrics-file-max-bytes bytes
--metrics-file-keep count
```

解析后对应：

```cpp
std::optional<std::uintmax_t> metrics_file_max_bytes;
std::size_t metrics_file_keep = 3;
```

`--metrics-file-max-bytes` 必须是正整数。

`--metrics-file-keep` 可以是 0：

```text
0 表示轮转时丢弃旧文件
大于 0 表示保留 N 份旧文件
```

这两个参数都要求同时传入：

```text
--metrics-file path
```

因为没有输出文件时，轮转和保留没有意义。

---

# 7. metrics_exporter 如何接入新 writer

v39 保留 v37 的 `metrics_exporter` 回调，只是把回调内部从直接写 `ofstream` 改成：

```cpp
metrics_file->write_line(message);
```

外层仍然有 mutex：

```cpp
std::lock_guard lock{metrics_file_mutex};
```

这样当前线程模型安全，后续如果 metrics export 触发点变多，也不需要重新审查文件写入并发。

---

# 8. server_start 显示轮转配置

`event=server_start` 现在会多打印：

```text
metrics_file_max_bytes=...
metrics_file_keep=...
```

没有配置 metrics file 时：

```text
metrics_file="none"
metrics_file_max_bytes=0
metrics_file_keep=0
```

配置了轮转时，例如：

```text
metrics_file="data/server.metrics.log"
metrics_file_max_bytes=10485760
metrics_file_keep=5
```

这让运行日志一眼能看出本轮 server 的指标文件策略。

---

# 9. 一句话总结

第三十九版把 metrics 文件从“能保存”推进到“能长时间保存且不无限膨胀”：

```text
--metrics-file 负责落盘
--metrics-file-max-bytes 负责切分
--metrics-file-keep 负责保留数量
MetricsFileWriter 负责把这些规则封装成一个清晰的小模块
```
