# 第一百零五次讲解：第五十一版测试、README、归档和整体增删改
第一百零四次讲的是 v51 核心代码：

```text
CHECKJSON
execution_contract_durability
execution_contract_warnings
format_check_json
```

这一篇讲配套修改：

```text
tests/command_tests.cpp
tests/line_editor_tests.cpp
README.md
a/51/解释/说明.md
a/51/图片/
代码讲解记录/README.md
```

一句话总结：

```text
v51 把 CHECKJSON 做成完整闭环：命令目录、HELP、Tab 补全、parser/write/WAL contract、真实 TCP smoke、README 和归档都一起完成。
```

---

# 1. command catalog 测试

`COMMANDS` 数量从：

```text
25
```

变成：

```text
26
```

并检查：

```text
CHECKJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)
"name":"CHECKJSON","category":"meta"
```

这证明 `CHECKJSON` 自己是元信息命令。

---

# 2. CHECKJSON usage 测试

新增：

```text
CHECKJSON
 -> ERR usage: CHECKJSON command
```

这保证 `CHECKJSON` 后面必须带一条目标命令。

---

# 3. CHECKJSON SET 测试

测试执行：

```text
CHECKJSON SET orderops:1 value
```

重点断言：

```text
read_only=true
execution_allowed=false
write_command=true
allowed_by_parser=true
checks.parser_allowed=true
checks.write_command=true
checks.wal_append_when_enabled=true
wal.enabled=false
wal.durability=memory_only
```

并继续执行：

```text
GET orderops:1
 -> (nil)
```

证明 `CHECKJSON SET ...` 没有执行写入。

---

# 4. CHECKJSON GET 测试

测试执行：

```text
CHECKJSON GET orderops:1
```

重点断言：

```text
write_command=false
wal.durability=not_applicable
warnings=["not a write command"]
```

这样 Node 可以识别当前 contract 不是写命令 contract。

---

# 5. parser 错误测试

测试执行：

```text
CHECKJSON SET orderops:1
```

重点断言：

```text
allowed_by_parser=false
warnings 包含 usage: SET key value
```

这说明 parser 检查进入了 contract，而不是只看命令名字。

---

# 6. WAL enabled 测试

测试创建一个带 WAL 指针的 `CommandProcessor`。

然后执行：

```text
CHECKJSON SET orderops:1 value
```

重点断言：

```text
wal_enabled=true
wal.durability=wal_backed
warnings=[]
```

同时检查 WAL 文件没有出现，证明 `CHECKJSON` 没有 append WAL。

---

# 7. line_editor_tests

补全候选加入：

```text
CHECKJSON
```

并验证：

```text
CHECKJ
 -> "CHECKJSON "
```

真实交互里用户输入这条较长命令会更顺手。

---

# 8. 真实 smoke 验证什么

真实运行验证使用：

```text
minikv_server.exe
minikv_client.exe
```

流程：

```text
启动 server：port=6414
发送 CHECKJSON SET orderops:1 value
发送 GET orderops:1
发送 CHECKJSON GET orderops:1
发送 QUIT
停止 server
```

验证重点：

```text
CHECKJSON SET 返回 read_only=true、execution_allowed=false
wal.durability=memory_only
command_digest 符合 fnv1a64:<16位hex>
GET orderops:1 仍然返回 (nil)
CHECKJSON GET 返回 write_command=false、durability=not_applicable
```

---

# 9. 一句话回看

v51 的配套验证确保一个点：

```text
Node v71 后续可以接入 CHECKJSON execution-contract 证据，但 mini-kv 仍然没有开放真实写执行入口。
```
