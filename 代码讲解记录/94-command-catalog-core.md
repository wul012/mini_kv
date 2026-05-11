# 第九十四次讲解：第四十六版 COMMANDS / COMMANDSJSON 命令分级核心
第四十六版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v54-post-infojson-roadmap.md
```

计划要求 mini-kv v46 做：

```text
COMMANDS
COMMANDSJSON
```

目标不是做权限系统，而是让外部控制面知道 mini-kv 支持哪些命令，以及每个命令的大致风险类别。

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
v46 把命令能力整理成统一 catalog，并通过 text / JSON 两个入口暴露 category、mutates_store、touches_wal、stable 和 description。
```

---

# 1. 为什么要做命令 catalog

v44 做了：

```text
INFO
```

v45 做了：

```text
INFOJSON
```

Node 可以知道 mini-kv 是谁、版本是多少、协议是什么。

但 Node 后续要做风险观察，还需要知道：

```text
哪些命令是读
哪些命令会写 store
哪些命令会碰 WAL
哪些命令属于 admin
哪些命令只是 meta 信息
```

所以 v46 新增命令 catalog。

---

# 2. CommandCatalogEntry

`src/command.cpp` 新增结构：

```cpp
struct CommandCatalogEntry {
    std::string_view name;
    std::string_view category;
    bool mutates_store;
    bool touches_wal;
    bool stable;
    std::string_view description;
};
```

每个字段的意义：

```text
name
 -> 命令名

category
 -> meta / read / write / admin

mutates_store
 -> 是否会改变内存 KV 数据

touches_wal
 -> 是否会读取或写入 WAL 相关状态

stable
 -> 当前对外契约是否稳定

description
 -> 给外部控制面或文档展示的短说明
```

---

# 3. 为什么用统一表

以前 `is_known_command()` 是一串硬编码判断：

```cpp
command == "PING" || command == "SET" || ...
```

v46 改成：

```cpp
return find_command_catalog_entry(command) != nullptr;
```

这样有两个好处：

```text
1. 命令识别和命令 catalog 不会分裂
2. 新增命令时只要先补 catalog，就能同步影响 metrics bucket 的已知命令判断
```

这比维护多份命令列表更稳。

---

# 4. 分类规则

计划给了主要分类：

```text
meta: PING, HELP, HEALTH, INFO, INFOJSON, STATS, STATSJSON
read: GET, TTL, SIZE, KEYS, WALINFO
write: SET, DEL, EXPIRE
admin: COMPACT, RESETSTATS
```

v46 实现时补齐了计划漏列但项目已经支持的命令：

```text
SAVE
LOAD
COMMANDS
COMMANDSJSON
EXIT
QUIT
```

补齐原因是：

```text
COMMANDSJSON 的目标是“支持哪些命令”
```

如果不列出这些命令，外部控制面拿到的 catalog 就不完整。

---

# 5. touches_wal 的理解

`touches_wal` 不只表示写 WAL。

例如：

```text
SET / DEL / EXPIRE
 -> WAL 开启时会写 WAL

COMPACT
 -> 会重写 WAL

WALINFO
 -> 会读取 WAL maintenance report

STATS / STATSJSON / HEALTH
 -> WAL 开启时会读取 WAL maintenance signal
```

所以它表达的是：

```text
这个命令是否会接触 WAL 相关路径
```

而不是只表达“是否追加 WAL record”。

---

# 6. COMMANDS 文本输出

`COMMANDS` 给人和简单脚本看：

```text
command_count=23 commands=PING(category=meta,mutates_store=no,touches_wal=no,stable=yes);...
```

它不包含 description，避免一行文本过长。

真正给控制面消费的是 `COMMANDSJSON`。

---

# 7. COMMANDSJSON 结构输出

`COMMANDSJSON` 返回：

```json
{
  "commands": [
    {
      "name": "SET",
      "category": "write",
      "mutates_store": true,
      "touches_wal": true,
      "stable": true,
      "description": "Set or update a key value"
    }
  ]
}
```

这个结构正好对齐计划里的字段：

```text
commands[]
  name
  category
  mutates_store
  touches_wal
  stable
  description
```

---

# 8. 这一版明确不做什么

v46 不做：

```text
ACL
用户权限
命令拦截
Node 接入
现有命令语义变更
```

它只是能力描述层。

---

# 9. 一句话回看

v46 是 mini-kv 面向控制面的第二块稳定契约：

```text
INFOJSON 说明“我是谁”
COMMANDSJSON 说明“我能做什么，以及风险级别是什么”
```
