# 第七十四次讲解：第三十六版客户端 Key 补全核心

第三十四版已经给 bundled TCP client 增加了命令补全：

```text
PI
 -> PING

STATSJ
 -> STATSJSON
```

第三十六版继续沿着客户端交互体验往前走：

```text
key-oriented command 的第二个参数也可以 Tab 补全
```

涉及的核心文件是：

```text
include/minikv/line_editor.hpp
src/line_editor.cpp
src/client_main.cpp
```

一句话总结：

```text
v36 让客户端在成功 SET 后记住 key，并在 GET / SET / DEL / EXPIRE / TTL 的 key 参数位置提供 Tab 补全。
```

---

# 1. 为什么 key 补全放在客户端本地

mini-kv 当前没有服务端 `KEYS` 或 scan 命令。

所以客户端不能随时向服务端查询全量 key。

v36 选择一个保守方案：

```text
只补全客户端本会话已经知道的 key
```

客户端通过自己的发送和响应来维护候选：

```text
SET key value 成功
 -> 记住 key

DEL key 返回 1 或 0
 -> 从候选里移除 key

LOAD path 成功
 -> 服务端数据集已被替换，清空本地候选
```

这不会改变服务端协议，也不会让补全逻辑依赖额外网络请求。

---

# 2. LineEditorCompletionOptions 的角色

头文件新增：

```cpp
struct LineEditorCompletionOptions {
    std::vector<std::string> command_candidates;
    std::vector<std::string> key_candidates;
};
```

v34 的 `LineEditorCompletion` 只有一组候选：

```text
command candidates
```

v36 拆成两组：

```text
command_candidates
 -> 第一个 token 用

key_candidates
 -> key 命令的第二个 token 用
```

这样命令补全和 key 补全可以复用同一个 Tab 入口，但候选来源分开。

---

# 3. 向后兼容旧构造函数

原来的测试和调用方式是：

```cpp
LineEditorCompletion completion{{"PING", "SET", "GET"}};
```

v36 保留这个构造函数。

内部做法是：

```text
把传入 vector 放进 options_.command_candidates
key_candidates 保持为空
```

所以旧的命令补全测试不用整体重写。

新的上下文补全用：

```cpp
LineEditorCompletion completion{LineEditorCompletionOptions{...}};
```

---

# 4. complete 如何区分命令补全和 key 补全

v36 的 `complete` 先解析第一个 token：

```text
跳过行首空白
找到第一个 token 的 start / end
```

如果光标还在第一个 token 末尾：

```text
走 command_candidates
大小写不敏感
```

如果光标已经越过第一个 token：

```text
先判断第一个 token 是不是 key-oriented command
再尝试补第二个 token
```

key-oriented command 包括：

```text
GET
SET
DEL
EXPIRE
TTL
```

这些命令的第二个参数都是 key。

---

# 5. 为什么只补第二个 token

例如：

```text
GET na
 -> 可以补 key

SET na
 -> 可以补 key

SET name value
 -> 不补
```

原因是第三个 token 开始通常是：

```text
value
seconds
path
```

这些不是 key，贸然补全容易误改用户正在输入的值。

所以 v36 的规则仍然保守：

```text
只补命令 token
只补 key 命令的第二个 token
光标在 token 中间不补
第二个 token 后还有别的文本不补
```

---

# 6. 命令补全和 key 补全的大小写策略

命令补全仍然大小写不敏感：

```text
pi
 -> PING
```

key 补全是大小写敏感：

```text
GET na
 -> name

GET NA
 -> 不匹配 name
```

原因是 mini-kv 的 key 本身是大小写敏感的。

如果 key 补全忽略大小写，就可能把用户想要的另一个 key 补错。

---

# 7. complete_token 复用公共补全逻辑

v36 把原来散在 `complete` 里的补全算法抽成：

```cpp
complete_token(...)
```

它负责：

```text
收集匹配候选
唯一匹配时补完整候选并追加空格
多匹配时只扩展最长公共前缀
没有匹配时返回 nullopt
补全结果没变化时返回 nullopt
```

命令补全和 key 补全只是参数不同：

```text
命令补全
 -> command_candidates
 -> case_insensitive=true

key 补全
 -> key_candidates
 -> case_insensitive=false
```

---

# 8. 客户端如何维护 known_keys

`client_main.cpp` 新增：

```text
std::vector<std::string> known_keys
```

每轮读取输入前，客户端构造补全选项：

```text
default_client_completion_options()
 -> 填入命令候选

completion_options.key_candidates = known_keys
 -> 填入本地已知 key
```

然后传给：

```cpp
read_client_line(prompt, history.entries(), completion_options, line)
```

交互式终端里按 Tab 时，行编辑器就能看到最新 key 候选。

---

# 9. update_known_keys 的规则

客户端发送命令并收到服务端第一行响应后，调用：

```cpp
update_known_keys(known_keys, resolved.command, response)
```

规则是：

```text
SET key value + OK ...
 -> add_known_key(key)

DEL key + 1 / 0
 -> remove_known_key(key)

LOAD path + OK loaded ...
 -> known_keys.clear()
```

为什么 `DEL key` 返回 0 也移除？

因为这说明服务端认为这个 key 当前不存在。

如果客户端本地候选里还有它，那已经是陈旧候选，应该清掉。

---

# 10. 非交互输入仍然保持旧行为

v36 没有改变 fallback 入口：

```text
stdin 或 stdout 不是终端
 -> fallback_read_line
 -> std::getline
```

所以脚本和 smoke test 仍然是逐行输入。

key 补全只影响真实交互终端里的 Tab 行为。

---

# 11. 一句话总结

第三十六版把客户端补全从“只知道命令名”推进到“知道本会话 key”：

```text
命令 token
 -> 补 PING / GET / STATSJSON / ...

key token
 -> 补本客户端会话已经 SET 成功的 key
```

它不改变服务端协议，不引入全量 key 查询，只用客户端自己观察到的成功写入维护一个轻量候选表。

