# 第一百零四次讲解：第五十一版 CHECKJSON execution contract 核心
第五十一版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v68-post-execution-gate-roadmap.md
```

计划要求 mini-kv v51 做：

```text
只读 execution-contract / CHECKJSON 增强
```

目标是让 Node 后续接入 execution gate 证据时，能在真实写入前看到 mini-kv 对 parser、side_effects、command_digest 和 WAL 条件的检查结果。

涉及核心文件：

```text
CMakeLists.txt
src/command.cpp
src/line_editor.cpp
tests/command_tests.cpp
tests/line_editor_tests.cpp
README.md
```

一句话总结：

```text
v51 新增 CHECKJSON <command>，把 EXPLAINJSON 的解释结果包装成只读 execution contract，明确 read_only=true、execution_allowed=false，并补充 parser / write / WAL 检查。
```

---

# 1. 为什么新增 CHECKJSON

`EXPLAINJSON` 的职责是解释命令：

```text
这个命令是什么
会不会写 Store
会不会碰 WAL
side_effects 是什么
digest 是什么
```

`CHECKJSON` 的职责更靠近执行门禁：

```text
如果未来真的要执行，执行前 mini-kv 这边会怎么看 parser / write / WAL 条件
```

但 v51 仍然不执行目标命令。

---

# 2. command catalog

v51 在命令目录里新增：

```cpp
{"CHECKJSON", "meta", false, false, true,
 "Read a write command execution contract as JSON without executing it"}
```

它自己仍然是：

```text
category=meta
mutates_store=false
touches_wal=false
```

因为 `CHECKJSON` 只是读取 contract，不写 Store，也不追加 WAL。

---

# 3. CHECKJSON 输出字段

典型输出：

```json
{
  "schema_version": 1,
  "read_only": true,
  "execution_allowed": false,
  "command_digest": "fnv1a64:...",
  "command": "SET",
  "write_command": true,
  "allowed_by_parser": true,
  "side_effects": ["store_write", "wal_append_when_enabled"],
  "side_effect_count": 2,
  "checks": {
    "parser_allowed": true,
    "write_command": true,
    "wal_append_when_enabled": true,
    "wal_enabled": false
  },
  "wal": {
    "enabled": false,
    "touches_wal": true,
    "append_when_enabled": true,
    "durability": "memory_only"
  },
  "warnings": ["wal disabled; write would be in-memory only"]
}
```

这里最重要的是：

```text
read_only=true
execution_allowed=false
```

这两个字段直接说明本命令只是检查，不是执行。

---

# 4. WAL durability

v51 用 `execution_contract_durability` 生成：

```text
wal_backed
memory_only
not_wal_backed
not_applicable
```

含义分别是：

```text
wal_backed
 -> 目标命令会触碰 WAL，当前 processor 也启用了 WAL

memory_only
 -> 目标命令会触碰 WAL，但当前 processor 没启用 WAL

not_wal_backed
 -> 目标命令会修改 Store，但它本身不是 WAL-backed 命令

not_applicable
 -> 目标命令不是写命令
```

这让 Node 可以在 gate 里分辨“能执行”和“执行后是否有持久化保障”是两件事。

---

# 5. contract warnings

`execution_contract_warnings` 会复用 explain warning，再补 contract 级 warning：

```text
not a write command
wal disabled; write would be in-memory only
write command is not wal-backed
```

例如：

```text
CHECKJSON GET orderops:1
 -> not a write command

CHECKJSON SET orderops:1 value
 -> wal disabled; write would be in-memory only
```

---

# 6. 不执行目标命令

入口代码是：

```cpp
return {format_check_json(explain_command(target_command), wal_ != nullptr)};
```

它调用的是：

```text
explain_command
format_check_json
```

没有调用：

```text
execute_trimmed(target_command)
```

所以 `CHECKJSON SET orderops:1 value` 不会真的写入 `orderops:1`。

---

# 7. 一句话回看

v51 把 mini-kv 从“能解释 raw command”推进到“能给 raw write command 提供执行前只读 contract”：

```text
EXPLAINJSON
 -> 命令解释

CHECKJSON
 -> 执行前 contract 证据
```
