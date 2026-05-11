# 第一百次讲解：第四十九版 EXPLAINJSON side effects 覆盖加固核心
第四十九版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v59-post-preflight-control-roadmap.md
```

计划要求 mini-kv v49 做：

```text
EXPLAINJSON coverage hardening
```

目标是补强 `EXPLAINJSON` 对 TTL、EXPIRE、admin 和 meta 命令的解释覆盖，让 Node 后续生成 approval evidence 时不只知道“是否写 Store / WAL”，还能知道更具体的副作用类型。

涉及核心文件：

```text
CMakeLists.txt
src/command.cpp
tests/command_tests.cpp
README.md
```

一句话总结：

```text
v49 给 EXPLAINJSON 增加 side_effects 数组，用稳定标签描述 store_write、store_ttl_update、snapshot_file_write、metrics_reset、connection_close 等风险，不新增任何执行能力。
```

---

# 1. 为什么需要 side_effects

v48 已经能输出：

```text
category
mutates_store
touches_wal
key
ttl_sensitive
allowed_by_parser
warnings
```

但这些字段还不够区分一些命令：

```text
SAVE
 -> 不修改 Store
 -> 不触碰 WAL
 -> 但会写 snapshot 文件

RESETSTATS
 -> 不修改 Store
 -> 不触碰 WAL
 -> 但会清空命令指标窗口

QUIT
 -> 不修改 Store
 -> 不触碰 WAL
 -> 但会关闭连接
```

所以 v49 增加：

```json
"side_effects": []
```

它不是 ACL，也不是权限系统，只是解释层 evidence。

---

# 2. CommandExplain 新字段

`src/command.cpp` 的内部结构新增：

```cpp
std::vector<std::string> side_effects;
```

输出位置在：

```text
allowed_by_parser
side_effects
warnings
```

这样 Node 可以先看 parser 是否允许，再看如果真的执行，可能发生哪些类型的副作用。

---

# 3. 写命令 side effects

普通写命令：

```text
SET
DEL
```

输出：

```json
"side_effects": ["store_write", "wal_append_when_enabled"]
```

含义是：

```text
会修改 Store
如果当前启用了 WAL，会追加 WAL 记录
```

这里故意写成 `wal_append_when_enabled`，因为没有启用 WAL 时不会真的写 WAL。

---

# 4. TTL / EXPIRE side effects

`TTL` 是读命令：

```json
"ttl_sensitive": true,
"side_effects": ["store_read"]
```

`EXPIRE` 是 TTL 写命令：

```json
"ttl_sensitive": true,
"side_effects": ["store_ttl_update", "wal_append_when_enabled"]
```

这能让 Node 区分：

```text
只是读取 TTL 状态
真正修改 key 的过期时间
```

---

# 5. admin 命令 side effects

v49 明确补了几类 admin 命令：

```text
SAVE
 -> snapshot_file_write

LOAD
 -> store_replace_from_snapshot

COMPACT
 -> wal_rewrite_when_enabled

RESETSTATS
 -> metrics_reset
```

这类信息是 v48 的布尔字段表达不完整的地方。

例如 `SAVE` 的：

```text
mutates_store=false
touches_wal=false
```

仍然不能说明它会写文件；`side_effects` 正好补上这个解释缺口。

---

# 6. meta 命令 side effects

大多数 meta/read 型命令归为：

```text
metadata_read
```

例如：

```text
PING
INFO
INFOJSON
COMMANDS
COMMANDSJSON
EXPLAINJSON
HELP
```

连接结束命令单独归为：

```text
EXIT / QUIT
 -> connection_close
```

真实 smoke 专门验证：

```text
EXPLAINJSON QUIT
PING still-open
```

如果 explain 真的执行了 `QUIT`，后面的 `PING` 就不会成功。现在它能返回 `still-open`，说明 explain 仍然只是解释。

---

# 7. 一句话回看

v49 没有把 mini-kv 往“执行控制面”推进，而是把解释能力补完整：

```text
EXPLAINJSON
 -> 结构化说明命令风险
 -> 不执行命令
 -> 给 Node approval evidence 提供更细证据
```
