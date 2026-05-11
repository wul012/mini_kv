# 第九十八次讲解：第四十八版 EXPLAINJSON 命令解释核心
第四十八版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v59-post-preflight-control-roadmap.md
```

计划要求 mini-kv v48 做：

```text
EXPLAINJSON <command>
```

目标是让 Node 在真实执行 raw command 前，先拿到结构化风险解释。

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
v48 新增 EXPLAINJSON，只解析和解释目标命令，不执行目标命令，因此可以安全暴露 mutates_store、touches_wal、key、ttl_sensitive 和 warnings。
```

---

# 1. 为什么不是直接执行 dry-run

跨项目计划里明确说：

```text
不执行被 explain 的命令
不做 ACL
不做事务
不新增后台扫描
不让 mini-kv 承担订单权威存储
```

所以 v48 的边界很窄：

```text
输入一条命令文本
 -> mini-kv 只做解析
 -> 返回 JSON 风险说明
 -> 不读写 Store
 -> 不追加 WAL
```

例如：

```text
EXPLAINJSON SET orderops:1 value
```

返回 `SET` 是写命令、会修改 Store、会触碰 WAL，但不会真的写入 `orderops:1`。

---

# 2. command catalog 新增 EXPLAINJSON

`src/command.cpp` 的命令目录新增：

```cpp
{"EXPLAINJSON", "meta", false, false, true,
 "Explain a command risk profile as JSON without executing it"}
```

这里把它归为：

```text
category=meta
mutates_store=false
touches_wal=false
```

原因是 `EXPLAINJSON` 自己只是元信息查询。它解释的目标命令可能是写命令，但 `EXPLAINJSON` 这条命令本身不写数据。

---

# 3. CommandExplain 结构

v48 增加了一个内部结构：

```cpp
struct CommandExplain {
    std::string command;
    std::string category = "unknown";
    bool mutates_store = false;
    bool touches_wal = false;
    std::optional<std::string> key;
    bool requires_value = false;
    bool ttl_sensitive = false;
    bool allowed_by_parser = false;
    std::vector<std::string> warnings;
};
```

这些字段对应 Node 后续预演需要看的几个核心问题：

```text
这是什么命令
是不是写命令
会不会触碰 WAL
目标 key 是哪个
是否需要 value
是否和 TTL 有关
当前输入是否符合 mini-kv parser
有什么警告
```

---

# 4. explain_command 的核心流程

核心函数是：

```cpp
CommandExplain explain_command(std::string_view line)
```

它的流程是：

```text
trim 输入
解析第一个 token 作为 command
转成大写
查 command_catalog
按命令类型做轻量语法校验
填充 CommandExplain
```

如果命令不存在：

```json
{
  "command": "NOPE",
  "category": "unknown",
  "allowed_by_parser": false,
  "warnings": ["unknown command"]
}
```

这让 Node 能区分：

```text
未知命令
已知命令但参数错误
已知命令且 parser 允许
```

---

# 5. 写命令解释

`SET` 的解释规则：

```text
SET key value
 -> category=write
 -> mutates_store=true
 -> touches_wal=true
 -> key=key
 -> requires_value=true
 -> ttl_sensitive=false
```

如果缺 key 或 value：

```text
allowed_by_parser=false
warnings=["usage: SET key value"]
```

`DEL` 的解释规则更简单：

```text
DEL key
 -> category=write
 -> mutates_store=true
 -> touches_wal=true
 -> key=key
 -> requires_value=false
```

---

# 6. TTL 相关命令解释

`EXPIRE` 和 `TTL` 都会标记：

```text
ttl_sensitive=true
```

区别是：

```text
EXPIRE
 -> write
 -> mutates_store=true
 -> touches_wal=true

TTL
 -> read
 -> mutates_store=false
 -> touches_wal=false
```

这样 Node 后续可以知道命令是否会影响过期时间，哪怕它不是单纯的 key/value 读写。

---

# 7. format_explain_json

`format_explain_json` 把 `CommandExplain` 输出成一行 JSON：

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

输出继续复用已有的：

```cpp
json_string
format_json_bool
```

所以 key、warning 和 description 里的特殊字符仍然会被正确转义。

---

# 8. execute_trimmed 分支

真正入口是：

```cpp
if (command == "EXPLAINJSON") {
    std::string target_command;
    std::getline(input >> std::ws, target_command);
    if (target_command.empty()) {
        return usage("EXPLAINJSON command");
    }

    return {format_explain_json(explain_command(target_command))};
}
```

注意这里没有调用：

```text
execute_trimmed(target_command)
```

这就是“不执行被 explain 命令”的关键。

---

# 9. 一句话回看

v48 把 mini-kv 从“能告诉 Node 有哪些命令”推进到“能告诉 Node 某条 raw command 的风险含义”：

```text
COMMANDSJSON
 -> 命令目录

EXPLAINJSON
 -> 单条命令的结构化风险解释
```
