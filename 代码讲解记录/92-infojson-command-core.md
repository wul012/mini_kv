# 第九十二次讲解：第四十五版 INFOJSON 命令核心
第四十五版严格按跨项目路线图推进 mini-kv 自身能力：

```text
INFO
 -> 给人和简单脚本看的 text identity

INFOJSON
 -> 给 Node 等外部控制面看的 schema-stable identity
```

这一版不接 Node，不改 Java，也不替代 `STATSJSON`。

涉及核心文件：

```text
CMakeLists.txt
src/command.cpp
src/line_editor.cpp
tests/command_tests.cpp
tests/line_editor_tests.cpp
```

一句话总结：

```text
v45 在 v44 的版本注入和 INFO 基础上，新增 INFOJSON，把同一批基础元信息稳定成 JSON object，方便后续 Node v54 直接消费。
```

---

# 1. 版本号从 0.44.0 到 0.45.0

`CMakeLists.txt` 只改一处：

```cmake
project(mini_kv VERSION 0.45.0 LANGUAGES CXX)
```

因为 v44 已经把版本号接进：

```text
include/minikv/version.hpp.in
 -> build/generated/include/minikv/version.hpp
 -> minikv::version
```

所以 v45 不需要在 C++ 里手写 `0.45.0`。

这就是 v44 先做版本注入的价值。

---

# 2. is_known_command 加入 INFOJSON

命令识别表新增：

```cpp
command == "INFOJSON"
```

这影响两件事：

```text
1. execute_trimmed 能分发 INFOJSON
2. metrics_command_name 会把 INFOJSON 计入独立命令桶
```

如果没有加入这里，即使命令能执行，也可能在统计里落到 `UNKNOWN`，这对后续观测是不对的。

---

# 3. format_protocol_json_array

v44 的 runtime info 里协议是字符串：

```text
inline
inline,resp
```

但是 v45 的 JSON 契约希望是数组：

```json
["inline"]
["inline","resp"]
```

所以新增：

```cpp
std::string format_protocol_json_array(std::string_view protocol)
```

它做三步：

```text
按逗号切分
trim 每段
用 json_string 转义并拼成 JSON array
```

这样 server 侧继续用简单字符串表达协议，外部控制面拿到的是更稳定的结构。

---

# 4. format_info_json

核心新增函数是：

```cpp
std::string format_info_json(std::size_t live_keys,
                             WriteAheadLog* wal,
                             const CommandRuntimeInfo& runtime_info)
```

它返回的结构是：

```json
{
  "version": "0.45.0",
  "server": {
    "protocol": ["inline", "resp"],
    "uptime_seconds": 12,
    "max_request_bytes": 4096
  },
  "store": {
    "live_keys": 1
  },
  "wal": {
    "enabled": false
  },
  "metrics": {
    "enabled": true
  }
}
```

代码里的实际输出是单行 JSON，方便 TCP 客户端一行读取。

---

# 5. 为什么不复用 STATSJSON

`STATSJSON` 是运行观测：

```text
commands
latency
connection_stats
wal maintenance
```

`INFOJSON` 是身份元信息：

```text
version
protocol
uptime
request limit
store live_keys
wal enabled
metrics enabled
```

这两个命令语义不同。

如果 Node 后续只想识别 mini-kv 版本和能力边界，不应该被迫解析庞大的 `STATSJSON`。

---

# 6. execute_trimmed 分支

执行分支很短：

```cpp
if (command == "INFOJSON") {
    if (has_extra_token(input)) {
        return usage("INFOJSON");
    }

    return {format_info_json(store_.size(), wal_, options_.runtime_info)};
}
```

规则和 `INFO` 一致：

```text
INFOJSON
 -> 返回 JSON object

INFOJSON extra
 -> ERR usage: INFOJSON
```

保持命令家族的使用体验一致。

---

# 7. HELP 和补全同步

`help_text()` 增加：

```text
INFOJSON
```

`default_client_completion_options()` 也增加：

```text
INFOJSON
```

由于 `INFO` 和 `INFOJSON` 有共同前缀，Tab 行为变成：

```text
INF
 -> INFO

INFOJ
 -> INFOJSON（唯一候选，补全后会追加空格）
```

这不是 bug，而是 line editor 的共同前缀补全规则。

---

# 8. 一句话回看

v45 的核心是把 v44 的 text identity 再落一层结构化契约：

```text
INFOJSON 是给后续 Node v54 消费的稳定 mini-kv 身份入口。
```
