# 第九十六次讲解：第四十七版 KEYSJSON key inventory 核心
第四十七版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v56-post-dashboard-control-roadmap.md
```

计划要求 mini-kv v47 做：

```text
KEYSJSON [prefix]
```

目标是让 Node 后续不用解析 text `KEYS`，而是能只读拿到稳定的 key inventory JSON。

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
v47 在不改变 KEYS 文本语义的前提下，新增有上限的 KEYSJSON，返回 prefix、key_count、keys、truncated 和 limit。
```

---

# 1. 为什么不直接让 Node 解析 KEYS

`KEYS` 的输出是人和简单脚本看的文本：

```text
key_count=2 keys=alpha beta
```

`KEYS prefix` 又故意返回不同格式：

```text
key_count=2 prefix=user: keys=user:1 user:2
```

这是为了保护 bundled client 的 key cache，不让过滤结果误覆盖全量缓存。

Node 后续要做 Dashboard key inventory，不应该依赖这些文本细节。

所以 v47 新增：

```text
KEYSJSON [prefix]
```

---

# 2. key_inventory_limit

v47 新增固定上限：

```cpp
constexpr std::size_t key_inventory_limit = 1000;
```

原因是 key inventory 是给控制面展示和安全预演用的，不是大规模扫描接口。

本版明确不做：

```text
分页游标
glob 模糊查询
大规模 scan
value 读取
```

超过 1000 个时：

```json
"truncated": true
```

这样 Node 能知道结果被截断，但 mini-kv 不会一次吐出过大的列表。

---

# 3. format_keys_json

核心函数：

```cpp
std::string format_keys_json(std::optional<std::string_view> prefix,
                             std::vector<std::string> keys)
```

它做四件事：

```text
1. 如果 keys 超过 1000，裁剪到 1000
2. prefix 不存在时输出 null
3. keys 用 json_string 逐个转义
4. 输出 truncated 和 limit
```

返回示例：

```json
{
  "prefix": "user:",
  "key_count": 2,
  "keys": ["user:1", "user:2"],
  "truncated": false,
  "limit": 1000
}
```

实际 TCP 输出仍是一行 JSON，方便一行读取。

---

# 4. execute_trimmed 分支

命令分支和 `KEYS` 保持同一风格：

```cpp
if (command == "KEYSJSON") {
    std::string prefix;
    if (input >> prefix) {
        if (has_extra_token(input)) {
            return usage("KEYSJSON [prefix]");
        }

        return {format_keys_json(prefix, store_.keys_with_prefix(prefix))};
    }

    return {format_keys_json(std::nullopt, store_.keys())};
}
```

规则是：

```text
KEYSJSON
 -> 全量 live keys JSON

KEYSJSON user:
 -> prefix 过滤 live keys JSON

KEYSJSON user: extra
 -> ERR usage: KEYSJSON [prefix]
```

---

# 5. 过期 key 的处理

`KEYSJSON` 复用：

```cpp
store_.keys()
store_.keys_with_prefix(prefix)
```

这两个接口和 `KEYS` 使用的是同一套 Store 查询逻辑。

所以 v47 不额外写过期清理逻辑，而是沿用现有行为：

```text
输出前继续只返回 live keys
```

这样不会出现 `KEYS` 和 `KEYSJSON` 对同一批数据看法不一致。

---

# 6. COMMANDSJSON 同步

v47 也把 `KEYSJSON` 加进 command catalog：

```text
name: KEYSJSON
category: read
mutates_store: false
touches_wal: false
stable: true
description: List live keys as JSON, optionally filtered by prefix
```

这很关键。

因为 Node 后续拿到 `COMMANDSJSON` 时，也应该知道 `KEYSJSON` 是只读命令。

---

# 7. 一句话回看

v47 是 mini-kv 从“能列 key”推进到“能安全给控制面展示 key inventory”：

```text
KEYS 保持文本兼容
KEYSJSON 提供结构化只读展示入口
```
