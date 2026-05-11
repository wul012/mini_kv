# 第一百零一次讲解：第四十九版测试、README、归档和整体增删改
第一百次讲的是 v49 核心代码：

```text
side_effects
TTL / EXPIRE coverage
admin / meta coverage
```

这一篇讲配套修改：

```text
tests/command_tests.cpp
README.md
a/49/解释/说明.md
a/49/图片/
代码讲解记录/README.md
```

一句话总结：

```text
v49 把 EXPLAINJSON 从“能解释基础风险”加固到“能解释具体副作用”：测试覆盖写命令、TTL、EXPIRE、snapshot admin、WAL compact、metrics reset、metadata read 和 connection close。
```

---

# 1. command_tests 更新已有断言

v48 的断言里已经有：

```text
EXPLAINJSON SET orderops:1 value
EXPLAINJSON GET orderops:1
EXPLAINJSON EXPIRE orderops:1 60
EXPLAINJSON GET orderops:1 extra
EXPLAINJSON NOPE orderops:1
```

v49 给这些 JSON 断言统一补上：

```json
"side_effects": [...]
```

这样老能力没有丢，只是输出更完整。

---

# 2. TTL 与 DEL 新增覆盖

新增：

```text
EXPLAINJSON TTL orderops:1
EXPLAINJSON DEL orderops:1
```

重点断言：

```text
TTL
 -> ttl_sensitive=true
 -> side_effects=["store_read"]

DEL
 -> side_effects=["store_write","wal_append_when_enabled"]
```

这把 TTL 读和 key 删除写分开了。

---

# 3. admin 命令新增覆盖

新增：

```text
EXPLAINJSON SAVE data/snap.txt
EXPLAINJSON LOAD data/snap.txt
EXPLAINJSON COMPACT
EXPLAINJSON RESETSTATS
```

分别覆盖：

```text
snapshot_file_write
store_replace_from_snapshot
wal_rewrite_when_enabled
metrics_reset
```

这些都是控制面审核时比 `mutates_store` 更细的证据。

---

# 4. meta 命令新增覆盖

新增：

```text
EXPLAINJSON INFO
EXPLAINJSON QUIT
```

分别覆盖：

```text
metadata_read
connection_close
```

`QUIT` 的覆盖很关键，因为它说明连接行为也能被解释，但 explain 本身不会关闭连接。

---

# 5. README 更新

README 的 Current version 改成：

```text
Version 49 is a runnable in-memory KV service with injected build version metadata, command catalog metadata, bounded JSON key inventory, and hardened structured command risk explanation
```

新增说明字段：

```text
side_effects
```

并列出稳定标签：

```text
store_write
store_ttl_update
snapshot_file_write
store_replace_from_snapshot
wal_rewrite_when_enabled
metrics_reset
connection_close
metadata_read
```

---

# 6. 真实 smoke 验证什么

真实运行验证使用：

```text
minikv_server.exe
minikv_client.exe
```

流程：

```text
启动 server：port=6412
发送 EXPLAINJSON SET orderops:1 value
发送 GET orderops:1
发送 EXPLAINJSON EXPIRE orderops:1 60
发送 EXPLAINJSON SAVE data/snap.txt
发送 EXPLAINJSON RESETSTATS
发送 EXPLAINJSON QUIT
发送 PING still-open
发送 QUIT
停止 server
```

验证重点：

```text
SET explain 返回 store_write / wal_append_when_enabled
GET orderops:1 仍然返回 (nil)
EXPIRE explain 返回 store_ttl_update / wal_append_when_enabled
SAVE explain 返回 snapshot_file_write
RESETSTATS explain 返回 metrics_reset
QUIT explain 返回 connection_close
PING still-open 成功，说明 EXPLAINJSON QUIT 没有关闭连接
```

---

# 7. 一句话回看

v49 的配套验证确保一个点：

```text
Node 后续消费 EXPLAINJSON 时，可以拿到更细的 side_effects evidence，同时 mini-kv 仍然没有新增真实执行、ACL 或事务能力。
```
