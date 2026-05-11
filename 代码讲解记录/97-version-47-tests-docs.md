# 第九十七次讲解：第四十七版测试、README、归档和整体增删改
第九十六次讲的是 v47 核心代码：

```text
KEYSJSON
format_keys_json
key_inventory_limit
COMMANDSJSON catalog 更新
```

这一篇讲配套修改：

```text
tests/command_tests.cpp
tests/line_editor_tests.cpp
README.md
a/47/解释/说明.md
a/47/图片/
代码讲解记录/README.md
```

一句话总结：

```text
v47 把 KEYSJSON 做成完整闭环：无 prefix、带 prefix、错误用法、1000 条上限、COMMANDSJSON 暴露、Tab 补全、真实 TCP smoke 和归档都一起完成。
```

---

# 1. command_tests 覆盖普通 KEYSJSON

`tests/command_tests.cpp` 新增：

```text
KEYSJSON
```

期望：

```json
{
  "prefix": null,
  "key_count": 3,
  "keys": ["alpha", "alpine", "name"],
  "truncated": false,
  "limit": 1000
}
```

这证明无 prefix 时返回全量 live key inventory。

---

# 2. command_tests 覆盖 prefix

新增：

```text
KEYSJSON alp
```

期望：

```json
{
  "prefix": "alp",
  "key_count": 2,
  "keys": ["alpha", "alpine"],
  "truncated": false,
  "limit": 1000
}
```

这对应计划里的：

```text
KEYSJSON [prefix]
```

---

# 3. command_tests 覆盖错误用法

新增：

```text
KEYSJSON extra more
 -> ERR usage: KEYSJSON [prefix]
```

这样 `KEYSJSON` 和 `KEYS` 一样，都只允许 0 或 1 个 prefix 参数。

---

# 4. command_tests 覆盖上限

测试里创建 1002 个 `bulk:` key。

然后执行：

```text
KEYSJSON bulk:
```

断言：

```text
"key_count":1000
"truncated":true
"limit":1000
```

这证明控制面不会一次拉出无限 key。

---

# 5. COMMANDSJSON 测试同步

v47 把 command catalog 数量从：

```text
23
```

变成：

```text
24
```

并检查：

```text
KEYSJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)
"name":"KEYSJSON","category":"read"
```

这证明 Node 后续能从 `COMMANDSJSON` 看出 `KEYSJSON` 是只读命令。

---

# 6. line_editor_tests 补全变化

加入 `KEYSJSON` 后，`K` 不再能直接补成 `KEYS `。

补全变成：

```text
K
 -> KEYS

KEYSJ
 -> KEYSJSON（唯一候选，补全后会追加空格）
```

这是 line editor 的共同前缀规则。

---

# 7. README 更新

README 的 Current version 改成：

```text
Version 47 is a runnable in-memory KV service with injected build version metadata, command catalog metadata, and bounded JSON key inventory
```

新增说明：

```text
KEYSJSON [prefix]
prefix
key_count
keys
truncated
limit
```

并明确：

```text
KEYSJSON 不返回 value
默认 limit 为 1000
```

---

# 8. 真实 smoke 验证什么

真实运行验证使用：

```text
minikv_server.exe
minikv_client.exe
```

流程：

```text
启动 server：port=6410
发送 SET user:1 one
发送 SET user:2 two
发送 SET order:1 paid
发送 KEYSJSON user:
发送 QUIT
停止 server
```

验证重点：

```text
JSON 能被 ConvertFrom-Json 解析
prefix 是 user:
key_count 是 2
keys 包含 user:1 和 user:2
truncated 是 false
limit 是 1000
```

---

# 9. 一句话回看

v47 的配套验证确保一个点：

```text
Node v58 未来接入 KEYSJSON 前，mini-kv 已经能独立证明这个 key inventory 契约稳定、只读、有上限。
```
