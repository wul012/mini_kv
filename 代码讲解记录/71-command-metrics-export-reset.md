# 第七十一次讲解：第三十五版指标导出与重置核心

第三十三版已经把命令指标升级成：

```text
总命令数
成功命令数
错误命令数
总延迟 / 平均延迟 / 最大延迟
按命令拆分的 breakdown
```

第三十五版继续把这套运行期观测能力往“可运维”方向推进一步：

```text
STATSJSON
 -> 把 STATS 的同一份运行快照导出成 JSON

RESETSTATS
 -> 清空命令执行指标，便于重新观察下一段运行窗口
```

涉及的核心文件是：

```text
include/minikv/command.hpp
src/command.cpp
src/line_editor.cpp
```

一句话总结：

```text
v35 让 mini-kv 不只会展示当前指标，还能把指标导出给脚本，并能在不中断服务、不清空数据的情况下重置命令指标窗口。
```

---

# 1. 为什么需要 STATSJSON

v33 的 `STATS` 是一行 key-value 文本：

```text
live_keys=1 wal_enabled=no total_commands=3 ...
```

这种格式适合人看，也适合简单 grep。

但如果要把指标交给脚本、日志采集器或之后的 benchmark 工具，JSON 更容易处理：

```text
有层级
有数组
布尔值是 true / false
数字不需要再拆字符串
```

所以 v35 新增：

```text
STATSJSON
```

它和 `STATS` 读取的是同一批运行期信息：

```text
Store live key 数
WAL maintenance 状态
Command metrics
TCP connection stats
```

只是输出格式从 key-value 变成 JSON。

---

# 2. 为什么需要 RESETSTATS

长期运行时，累计指标会越来越大。

例如服务已经跑了半小时：

```text
total_commands=120000
error_commands=37
```

如果这时想观察“接下来 5 分钟”的行为，旧指标会干扰判断。

v35 新增：

```text
RESETSTATS
```

它只清空命令执行指标：

```text
total_commands
successful_commands
error_commands
latency counters
command_breakdown
```

它不会清空：

```text
Store 数据
WAL 文件
WAL compact counters
TCP total / active / peak connection counters
```

这样它是一个轻量的观测窗口重置，而不是服务状态重置。

---

# 3. CommandMetricsTracker 新增 reset

头文件新增方法：

```cpp
void reset();
```

实现很直接：

```cpp
void CommandMetricsTracker::reset() {
    std::lock_guard lock{mutex_};
    totals_ = {};
    breakdown_.clear();
}
```

这里继续复用 v33 引入的 mutex。

原因是 metrics tracker 里现在有两层数据：

```text
totals_
 -> 总指标

breakdown_
 -> 每个命令桶
```

重置时必须保证两者一起清空。

如果只清空 totals，不清空 breakdown，就会出现：

```text
total_commands=0
command_breakdown=PING:10/10/0/...
```

这种前后矛盾的快照。

---

# 4. RESETSTATS 为什么不计入新指标

`CommandProcessor::execute` 的通用流程是：

```text
执行命令
记录 metrics
返回结果
```

如果 `RESETSTATS` 也完全走这个流程，就会发生：

```text
RESETSTATS 清空指标
然后 execute 又把 RESETSTATS 自己记进去
```

结果用户马上执行 `STATS` 会看到：

```text
total_commands=1
command_breakdown=RESETSTATS:1/1/0/...
```

这不符合“重置后从零开始观察”的直觉。

所以 v35 在 `execute` 里加了一个特判：

```cpp
const bool reset_succeeded = command == "RESETSTATS" && result.response == "OK stats reset";
if (!reset_succeeded) {
    metrics_tracker_->record(command, result, elapsed_ns);
}
```

含义是：

```text
RESETSTATS 成功
 -> 清空指标，并且不把自己记入新窗口

RESETSTATS extra 这种 usage error
 -> 仍然计入错误命令
```

这样既保持了成功重置的干净语义，也不会隐藏错误输入。

---

# 5. STATSJSON 的快照语义

`STATSJSON` 和 `STATS` 一样，返回的是“命令自身被计数之前”的快照。

例如：

```text
PING
STATSJSON
```

`STATSJSON` 响应里看到的是：

```text
"total_commands":1
```

等 `STATSJSON` 返回之后，tracker 内部才会把 `STATSJSON` 自己计入下一次快照。

这和 v32 / v33 对 `STATS` 的语义保持一致：

```text
观测命令展示观测前状态
观测命令返回后再进入下一轮统计
```

---

# 6. JSON 输出结构

v35 新增 `format_stats_json`。

顶层结构大致是：

```json
{
  "live_keys": 1,
  "wal_enabled": false,
  "wal": null,
  "commands": {},
  "connection_stats": {}
}
```

没有启用 WAL 时：

```json
"wal": null
```

启用 WAL 时，`wal` 会展开为：

```text
bytes
records
live_keys
compact_recommended
compact_min_records
compact_record_ratio
compact_min_bytes
manual_compactions
auto_compactions
repair_compactions
compacted_keys
records_removed
bytes_saved
```

连接统计也是一样：

```text
connection_stats.available=false
 -> 说明当前不是 TCP server 注入的上下文

connection_stats.available=true
 -> 继续输出 active / total / peak
```

---

# 7. commands JSON 如何复用 v33 指标

`format_command_metrics_json` 和 `format_command_metrics` 使用同一份 `CommandProcessorMetrics`。

它输出：

```text
total_commands
successful_commands
error_commands
total_latency_ns
avg_latency_ns
max_latency_ns
breakdown
```

其中 `breakdown` 是数组：

```json
[
  {
    "command": "PING",
    "total_commands": 1,
    "successful_commands": 1,
    "error_commands": 0,
    "total_latency_ns": 1000,
    "avg_latency_ns": 1000,
    "max_latency_ns": 1000
  }
]
```

这样 JSON 版本没有重新设计一套指标模型，只是把 v33 的模型换成结构化输出。

---

# 8. 为什么加 json_string

虽然当前命令桶通常是：

```text
PING
GET
UNKNOWN
STATSJSON
```

理论上都很安全。

但 JSON 字符串仍然应该按 JSON 规则转义。

所以 v35 增加了：

```cpp
std::string json_string(std::string_view value)
```

它会处理：

```text
双引号
反斜杠
\b / \f / \n / \r / \t
控制字符
```

这让 JSON 输出逻辑更稳，不依赖“当前命令名刚好都很干净”这个假设。

---

# 9. 新命令加入 known command

`is_known_command` 新增：

```text
STATSJSON
RESETSTATS
```

这很重要。

否则 metrics 会把它们归到：

```text
UNKNOWN
```

v35 之后，错误或成功的 `STATSJSON` / `RESETSTATS` 都会进入自己的命令桶。

只有成功的 `RESETSTATS` 特殊：它会重置后不计入新窗口。

---

# 10. 客户端 Tab 补全同步

v34 的默认补全候选里有：

```text
STATS
HEALTH
```

v35 同步加入：

```text
STATSJSON
RESETSTATS
```

这样交互式客户端里：

```text
R + Tab
 -> RESETSTATS 后带一个补全空格

STATSJ + Tab
 -> STATSJSON 后带一个补全空格
```

如果输入：

```text
STAT + Tab
```

会扩展成：

```text
STATS
```

再按 Tab 不会继续乱选，因为 `STATS` 和 `STATSJSON` 都匹配，最长公共前缀已经没有增长。

---

# 11. 一句话总结

第三十五版把命令指标从“只能读一行状态”推进到“可导出、可重置”的运维小闭环：

```text
STATS
 -> 人看的一行 key-value

STATSJSON
 -> 脚本看的结构化快照

RESETSTATS
 -> 清空命令指标窗口，重新观察下一段运行
```

这一步不改变数据读写语义，也不改变 TCP 协议主体，只是让已经积累起来的观测能力更好用。
