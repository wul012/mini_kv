# 第八十二次讲解：第四十版 KEYS 列表命令核心

第四十版从 Roadmap 第一项拆出一个刚好一版大小的前置能力：

```text
先让服务端提供稳定的 key-listing command。
```

这一版只做 `KEYS` 命令本身，不提前把客户端缓存刷新也塞进来。这样功能边界清楚，验证也完整。

涉及核心文件：

```text
include/minikv/store.hpp
src/store.cpp
src/command.cpp
src/line_editor.cpp
```

一句话总结：

```text
v40 新增 Store::keys() 和 KEYS 命令，返回当前仍然有效的 key，并保持字典序排序。
```

---

# 1. 为什么先做服务端 KEYS

v38 已经有客户端本地 key cache：

```text
SET 成功 -> 记住 key
DEL 成功 -> 移除 key
LOAD 成功 -> 清空 key cache
```

但这个 cache 只知道当前客户端观察过的变化。

如果将来要做“主动刷新 key cache”，客户端需要一个服务端入口：

```text
向 server 请求当前 live keys
用返回结果替换或修正本地 key cache
```

所以 v40 先补服务端命令：

```text
KEYS
```

下一版再让客户端用它刷新缓存，会更稳。

---

# 2. Store::keys() 的职责

`Store` 新增：

```cpp
std::vector<std::string> keys() const;
```

它做三件事：

```text
加锁
清理过期 key
返回排序后的 key 列表
```

实现上复用 Store 已有的过期清理规则：

```cpp
prune_expired_locked(Clock::now());
```

这保证 `KEYS` 不会把已经过期但还没被访问清理掉的 key 返回给用户。

---

# 3. 为什么返回排序结果

Store 内部使用：

```cpp
std::unordered_map
```

它的遍历顺序不稳定。

如果直接把 unordered_map 的顺序返回，测试和脚本会看到不确定输出：

```text
alpha beta
beta alpha
```

所以 `Store::keys()` 最后调用：

```cpp
std::ranges::sort(result);
```

这样输出稳定，后续客户端刷新缓存也更容易验证。

---

# 4. KEYS 命令输出格式

`src/command.cpp` 新增 `format_keys`：

```text
key_count=N keys=...
```

例如：

```text
key_count=2 keys=alpha beta
```

空库时：

```text
key_count=0 keys=
```

这个格式的考虑是：

```text
人能直接看懂
脚本能按 key_count 和 keys= 解析
保持单行响应，不破坏当前 client 的 read_line 行为
```

当前 mini-kv 的 key 是命令中的单个 token，不包含空白字符，所以空格分隔足够支撑现有命令模型。

---

# 5. KEYS 的命令解析

命令入口保持和其他只读命令一致：

```cpp
if (command == "KEYS") {
    if (has_extra_token(input)) {
        return usage("KEYS");
    }

    return {format_keys(store_.keys())};
}
```

规则：

```text
KEYS
 -> 返回当前 live key 列表

KEYS extra
 -> ERR usage: KEYS
```

它不会写 WAL，因为只是只读查询。

---

# 6. metrics 为什么也认识 KEYS

`is_known_command` 新增 `KEYS`。

这影响 command metrics：

```text
KEYS 成功或失败
 -> 计入 KEYS 桶

拼错的未知命令
 -> 仍然计入 UNKNOWN 桶
```

这样 `STATS` / `STATSJSON` / `server_metrics` 里的命令分布不会把 `KEYS` 误归类为 UNKNOWN。

---

# 7. 客户端补全只加命令名

`src/line_editor.cpp` 的默认命令候选新增：

```text
KEYS
```

因此交互终端里：

```text
K + Tab
 -> KEYS
```

但这一版没有做：

```text
客户端自动调用 KEYS
客户端解析 KEYS 返回值
客户端替换本地 key cache
```

这些属于下一版的自然工作，不混进 v40。

---

# 8. 一句话总结

第四十版补的是“客户端刷新 key cache”之前必须存在的服务端能力：

```text
Store 能稳定列出 live keys
CommandProcessor 暴露 KEYS
line editor 认识 KEYS
测试和真实 TCP smoke 验证 KEYS 可用
```
