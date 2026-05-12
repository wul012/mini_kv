# 第一百零二次讲解：第五十版 EXPLAINJSON schema / digest 核心
第五十版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v65-post-upstream-evidence-roadmap.md
```

计划要求 mini-kv v50 做：

```text
EXPLAINJSON 增加 schema_version / command_digest / side_effect_count
```

目标是让 Node 后续做 digest-aware upstream evidence verification 时，可以复核 mini-kv 解释证据的结构版本、指纹和副作用数量。

涉及核心文件：

```text
CMakeLists.txt
src/command.cpp
tests/command_tests.cpp
README.md
```

一句话总结：

```text
v50 给 EXPLAINJSON 增加 schema_version、command_digest 和 side_effect_count，让解释结果从“可读”推进到“可复核”。
```

---

# 1. schema_version

v50 新增常量：

```cpp
constexpr int explain_schema_version = 1;
```

输出字段：

```json
"schema_version": 1
```

它的意义是让 Node 知道当前解释 JSON 的结构版本。

后续如果 `EXPLAINJSON` 字段发生不兼容变化，可以提升版本，而不是让 Node 靠猜字段是否存在。

---

# 2. command_digest

v50 新增：

```json
"command_digest": "fnv1a64:..."
```

这个 digest 是对规范化 explain 字段计算的 FNV-1a 64-bit 指纹。

它覆盖的内容包括：

```text
schema_version
command
category
mutates_store
touches_wal
key
requires_value
ttl_sensitive
allowed_by_parser
side_effects
warnings
```

它不是安全签名，也不是权限凭据，只是证据链里的稳定复核指纹。

---

# 3. 为什么用长度前缀

digest 计算前，字段会进入一个内部 source 字符串。

v50 没有简单拼接：

```text
SET|write|true
```

而是使用：

```cpp
append_digest_part(source, value)
```

每段格式是：

```text
长度:内容;
```

这样即使 key 或 warning 里包含分隔符，也不会让不同字段组合碰撞成同一段文本。

---

# 4. side_effect_count

v49 已经新增：

```json
"side_effects": [...]
```

v50 再补：

```json
"side_effect_count": 2
```

它看似简单，但对 Node 很有用：

```text
快速判断解释是否有副作用
校验 side_effects 数组数量是否和摘要一致
把 evidence summary 和原始 explain JSON 对齐
```

---

# 5. 输出顺序

v50 的 `format_explain_json` 把新增字段放在前面：

```text
schema_version
command_digest
command
category
...
side_effects
side_effect_count
warnings
```

这样人看日志时，第一眼就能看到：

```text
这是哪个 schema
这条证据的 digest 是什么
它解释的是哪条 command
```

---

# 6. 一句话回看

v50 没有增加任何执行能力：

```text
不新增事务
不新增集群
不承担订单权威存储
```

它只是把 `EXPLAINJSON` 输出变成更适合 Node 证据校验的稳定结构。
