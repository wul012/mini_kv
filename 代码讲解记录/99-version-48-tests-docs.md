# 第九十九次讲解：第四十八版测试、README、归档和整体增删改
第九十八次讲的是 v48 核心代码：

```text
EXPLAINJSON
CommandExplain
explain_command
format_explain_json
```

这一篇讲配套修改：

```text
tests/command_tests.cpp
tests/line_editor_tests.cpp
README.md
a/48/解释/说明.md
a/48/图片/
代码讲解记录/README.md
```

一句话总结：

```text
v48 把 EXPLAINJSON 做成完整闭环：命令目录暴露、语法校验、写风险解释、未知命令 warning、Tab 补全、真实 TCP smoke 和归档都一起完成。
```

---

# 1. command_tests 覆盖 EXPLAINJSON usage

新增：

```text
EXPLAINJSON
 -> ERR usage: EXPLAINJSON command
```

这保证 `EXPLAINJSON` 后面必须带目标命令。

---

# 2. command_tests 覆盖 SET 风险解释

新增：

```text
EXPLAINJSON SET orderops:1 value
```

期望：

```json
{
  "command": "SET",
  "category": "write",
  "mutates_store": true,
  "touches_wal": true,
  "key": "orderops:1",
  "requires_value": true,
  "ttl_sensitive": false,
  "allowed_by_parser": true,
  "warnings": []
}
```

测试随后执行：

```text
GET orderops:1
```

期望仍然是：

```text
(nil)
```

这证明 explain 没有执行 `SET`。

---

# 3. command_tests 覆盖读命令和 TTL 命令

新增：

```text
EXPLAINJSON GET orderops:1
EXPLAINJSON EXPIRE orderops:1 60
```

它们分别验证：

```text
GET
 -> read / 不写 / 不触碰 WAL

EXPIRE
 -> write / 写 / 触碰 WAL / ttl_sensitive=true
```

这样 Node 后续可以区分普通读、普通写和 TTL 相关写。

---

# 4. command_tests 覆盖 parser warning

新增：

```text
EXPLAINJSON GET orderops:1 extra
```

期望：

```json
"allowed_by_parser": false,
"warnings": ["usage: GET key"]
```

这不是未知命令，而是已知命令的参数不符合 mini-kv 当前 parser。

---

# 5. command_tests 覆盖未知命令

新增：

```text
EXPLAINJSON NOPE orderops:1
```

期望：

```json
{
  "command": "NOPE",
  "category": "unknown",
  "allowed_by_parser": false,
  "warnings": ["unknown command"]
}
```

这样控制面不用猜测 mini-kv 是否支持某条 raw command。

---

# 6. COMMANDS / COMMANDSJSON 同步

v48 把 command catalog 数量从：

```text
24
```

变成：

```text
25
```

并检查：

```text
EXPLAINJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)
"name":"EXPLAINJSON","category":"meta"
```

这说明 `EXPLAINJSON` 自己是安全的元信息命令。

---

# 7. line_editor_tests 补全变化

加入 `EXPLAINJSON` 后，`E` 的共同前缀仍然是：

```text
E
 -> EX
```

新增唯一候选补全：

```text
EXPLAINJ
 -> EXPLAINJSON 
```

真实交互里用户更容易输入这条较长的命令。

---

# 8. README 更新

README 的 Current version 改成：

```text
Version 48 is a runnable in-memory KV service with injected build version metadata, command catalog metadata, bounded JSON key inventory, and structured command risk explanation
```

新增说明：

```text
EXPLAINJSON command
command
category
mutates_store
touches_wal
key
requires_value
ttl_sensitive
allowed_by_parser
warnings
```

并明确：

```text
EXPLAINJSON 不执行被解释的命令
```

---

# 9. 真实 smoke 验证什么

真实运行验证使用：

```text
minikv_server.exe
minikv_client.exe
```

流程：

```text
启动 server：port=6411
发送 EXPLAINJSON SET orderops:1 value
发送 GET orderops:1
发送 EXPLAINJSON GET orderops:1 extra
发送 QUIT
停止 server
```

验证重点：

```text
SET explain 返回 mutates_store=true 和 touches_wal=true
GET orderops:1 仍然返回 (nil)
错误参数返回 allowed_by_parser=false 和 usage warning
```

---

# 10. 一句话回看

v48 的配套验证确保一个点：

```text
Node v61 未来接入 EXPLAINJSON 前，mini-kv 已经能独立证明这个 raw command 解释契约稳定、结构化、且不会执行写命令。
```
