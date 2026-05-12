# 第一百零三次讲解：第五十版测试、README、归档和整体增删改
第一百零二次讲的是 v50 核心代码：

```text
schema_version
command_digest
side_effect_count
```

这一篇讲配套修改：

```text
tests/command_tests.cpp
README.md
a/50/解释/说明.md
a/50/图片/
代码讲解记录/README.md
```

一句话总结：

```text
v50 把 EXPLAINJSON 的证据字段补成可校验闭环：测试覆盖 schema、digest 格式、digest 稳定性、digest 差异和 side_effect_count。
```

---

# 1. command_tests 从全量字符串转成字段断言

v49 之前，`EXPLAINJSON` 测试大量使用完整字符串断言。

v50 增加 `command_digest` 后，完整字符串会变得不易维护。

所以测试新增两个小 helper：

```cpp
assert_response_contains(...)
extract_json_string_field(...)
```

前者检查 JSON 里有关键字段，后者提取 `command_digest`。

---

# 2. schema_version 覆盖

测试检查：

```text
"schema_version":1
```

这证明输出已经进入 v50 的 explain schema。

---

# 3. command_digest 覆盖

测试检查 digest 前缀：

```text
"command_digest":"fnv1a64:
```

并检查长度：

```text
fnv1a64: + 16 位 hex
```

也就是：

```text
24 个字符
```

---

# 4. digest 稳定性

测试连续执行两次：

```text
EXPLAINJSON SET orderops:1 value
EXPLAINJSON SET orderops:1 value
```

要求两次 `command_digest` 完全一致。

这证明同一条命令解释在同一 schema 下可以稳定复核。

---

# 5. digest 差异

测试再执行：

```text
EXPLAINJSON GET orderops:1
```

要求它的 digest 不等于 `SET orderops:1 value` 的 digest。

这证明 digest 不是固定占位符，而是由解释内容生成。

---

# 6. side_effect_count 覆盖

测试继续覆盖 v49 的典型命令：

```text
SET
GET
EXPIRE
NOPE
TTL
DEL
SAVE
LOAD
COMPACT
RESETSTATS
INFO
QUIT
```

并检查：

```text
side_effect_count
```

例如：

```json
"side_effects": ["store_write", "wal_append_when_enabled"],
"side_effect_count": 2
```

未知命令则是：

```json
"side_effects": [],
"side_effect_count": 0
```

---

# 7. 真实 smoke 验证什么

真实运行验证使用：

```text
minikv_server.exe
minikv_client.exe
```

流程：

```text
启动 server：port=6413
发送 EXPLAINJSON SET orderops:1 value
发送 EXPLAINJSON SET orderops:1 value
发送 GET orderops:1
发送 EXPLAINJSON RESETSTATS
发送 QUIT
停止 server
```

验证重点：

```text
schema_version=1
command_digest 符合 fnv1a64:<16位hex>
重复 SET explain 的 digest 一致
side_effect_count 与 side_effects 数组长度一致
GET orderops:1 仍然返回 (nil)，说明 explain 没有执行 SET
```

---

# 8. README 更新

README 的 Current version 改成：

```text
Version 50 is a runnable in-memory KV service with injected build version metadata, command catalog metadata, bounded JSON key inventory, and digest-aware structured command risk explanation
```

新增说明字段：

```text
schema_version
command_digest
side_effect_count
```

并明确：

```text
command_digest 是证据指纹，不是认证签名。
```

---

# 9. 一句话回看

v50 的配套验证确保一个点：

```text
Node v67 后续可以把 mini-kv 的 EXPLAINJSON schema / digest / side_effect_count 纳入 evidence verification。
```
