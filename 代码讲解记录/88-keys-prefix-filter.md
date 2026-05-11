# 第八十八次讲解：第四十三版 KEYS 前缀过滤核心

第四十三版接上 v40 / v41 的 key-list 链路，只推进一个小闭环：

```text
KEYS 支持可选 prefix 参数，用来列出指定前缀的 key。
```

涉及核心文件：

```text
include/minikv/store.hpp
src/store.cpp
src/command.cpp
tests/store_tests.cpp
tests/command_tests.cpp
tests/client_history_tests.cpp
```

一句话总结：

```text
v43 新增 KEYS prefix，同时保持无参数 KEYS 的旧响应格式不变，避免破坏 v41 的客户端 key cache 刷新逻辑。
```

---

# 1. 为什么要小心客户端缓存

v41 做过一个行为：

```text
客户端收到无参数 KEYS 的合法响应
 -> parse_key_list_response
 -> ClientKeyCache::replace
 -> 保存全量 key cache
```

如果 v43 直接让：

```text
KEYS alp
 -> key_count=2 keys=alpha alpine
```

客户端就会把本地 key cache 错误替换成过滤子集。

所以 v43 刻意让过滤响应多一个字段：

```text
key_count=2 prefix=alp keys=alpha alpine
```

这样人能看懂它是过滤结果，老的 `parse_key_list_response` 也会拒绝它，不会误刷新全量缓存。

---

# 2. Store::keys_with_prefix

`include/minikv/store.hpp` 新增：

```cpp
std::vector<std::string> keys_with_prefix(std::string_view prefix) const;
```

实现位置在 `src/store.cpp`。

核心逻辑：

```text
加锁
清理过期 key
遍历 data_
只收集以 prefix 开头的 key
排序后返回
```

前缀判断用：

```cpp
item.first.rfind(prefix, 0) == 0
```

这表示：

```text
prefix 出现在 key 的第 0 个位置
```

也就是标准的 starts-with 语义。

---

# 3. 为什么过滤放在 Store 层

也可以在 `CommandProcessor` 中这样做：

```text
store_.keys()
 -> 拿到全量列表
 -> command 层过滤
```

但 v43 选择放在 Store 层：

```text
store_.keys_with_prefix(prefix)
```

原因是：

```text
Store 更了解过期 key 清理和锁保护
过滤时不用先把所有 key 都返回给 command 层
后续如果做 pagination / limit，也更容易接在 Store 附近
```

这和 Roadmap 的“大量 key 列表”方向更贴合。

---

# 4. CommandProcessor 的 KEYS 分支

`src/command.cpp` 中 `KEYS` 从“不能有参数”改成“可选一个参数”：

```text
KEYS
 -> 返回全量 key_count=N keys=...

KEYS prefix
 -> 返回过滤 key_count=N prefix=prefix keys=...

KEYS prefix extra
 -> ERR usage: KEYS [prefix]
```

旧响应保持不变：

```text
KEYS
 -> key_count=3 keys=alpha alpine beta
```

新响应带 prefix：

```text
KEYS alp
 -> key_count=2 prefix=alp keys=alpha alpine
```

这样脚本可以区分全量列表和过滤列表。

---

# 5. format_prefixed_keys

`src/command.cpp` 新增：

```cpp
std::string format_prefixed_keys(std::string_view prefix, const std::vector<std::string>& keys);
```

它负责拼出：

```text
key_count=N prefix=<prefix> keys=<space separated keys>
```

和 `format_keys` 分开写，是为了避免以后有人改全量 `KEYS` 响应时，不小心把 filtered 响应也改回相同格式。

这里的分离其实是在保护客户端缓存语义。

---

# 6. 测试覆盖

`tests/store_tests.cpp` 覆盖：

```text
keys_with_prefix("alp")
 -> alpha / alpine

过期 alpha 后：
keys_with_prefix("alp")
 -> alpine
```

`tests/command_tests.cpp` 覆盖：

```text
KEYS
 -> key_count=2 keys=alpha name

KEYS alp
 -> key_count=2 prefix=alp keys=alpha alpine

KEYS z
 -> key_count=0 prefix=z keys=

KEYS alp extra
 -> ERR usage: KEYS [prefix]
```

`tests/client_history_tests.cpp` 覆盖：

```text
parse_key_list_response("key_count=2 prefix=alp keys=alpha alpine")
 -> nullopt
```

这条测试很关键，它证明 prefixed KEYS 不会触发全量 key cache 替换。
