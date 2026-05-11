# 第八十四次讲解：第四十一版客户端 Key Cache 刷新核心

第四十一版接上 v40 的服务端 `KEYS` 命令，只做一个小闭环：

```text
客户端收到 KEYS 的有效响应后，用服务端 key 列表刷新本地 key cache。
```

涉及核心文件：

```text
include/minikv/client_history.hpp
src/client_history.cpp
src/client_main.cpp
```

一句话总结：

```text
v41 让 bundled TCP client 可以把 KEYS 返回的 key_count=N keys=... 解析成列表，并替换本地补全缓存。
```

---

# 1. 为什么不是新建一个客户端命令

v40 已经有服务端命令：

```text
KEYS
```

用户本来就可以手动运行它。

所以 v41 没有再加：

```text
:refresh-keys
--refresh-key-cache
```

而是选择更小、更自然的行为：

```text
用户运行 KEYS
 -> server 返回 key_count=N keys=...
 -> client 看懂这个响应
 -> 用它刷新本地 key cache
```

这让“刷新”保持可选：

```text
不运行 KEYS
 -> 仍然只靠 SET / DEL / LOAD 观察更新

运行 KEYS
 -> 主动同步服务端当前 live keys
```

---

# 2. parse_key_list_response 的角色

`src/client_history.cpp` 新增：

```cpp
std::optional<std::vector<std::string>> parse_key_list_response(std::string_view response);
```

实现里使用 `std::istringstream` 拆分 `keys=` 后面的 key 列表，所以 `src/client_history.cpp` 也显式包含了：

```cpp
#include <sstream>
```

这不是功能逻辑变化，但它很重要：不同平台的标准库头文件间接包含不完全一致，显式包含能避免本机通过、macOS 或 MSVC CI 编译失败。

它只接受 v40 定义的格式：

```text
key_count=N keys=...
```

例如：

```text
key_count=2 keys=alpha beta
```

会解析成：

```text
alpha
beta
```

如果响应不是这个格式，例如：

```text
ERR usage: KEYS
key_count=two keys=alpha
key_count=1 keys=alpha beta
```

就返回 `std::nullopt`，客户端不会改缓存。

---

# 3. 为什么检查 key_count

解析时不只是把 `keys=` 后面的文本按空格切开。

它还会比较：

```text
实际解析出的 key 数量 == key_count
```

这样可以避免半截响应或格式错误误刷新缓存。

例如：

```text
key_count=1 keys=alpha beta
```

实际有 2 个 key，但 count 写 1，解析会失败。

---

# 4. ClientKeyCache::replace

`ClientKeyCache` 新增：

```cpp
bool replace(std::vector<std::string> keys);
```

它的行为是：

```text
清理空白
跳过空 key
跳过重复 key
遵守 max_entries 容量
如果最终列表和旧列表相同，返回 false
如果发生变化，替换 entries_ 并返回 true
```

返回值很重要，因为 `src/client_main.cpp` 依靠它判断是否需要保存：

```text
key cache 改了
 -> 如果配置了 --key-cache-file，就写回文件
```

---

# 5. update_known_keys 新增 KEYS 分支

`src/client_main.cpp` 原本已经根据响应更新缓存：

```text
SET + OK ...
 -> add key

DEL + 1 / 0
 -> remove key

LOAD + OK loaded ...
 -> clear cache
```

v41 新增：

```text
KEYS + key_count=N keys=...
 -> replace cache
```

代码意图是：

```cpp
if (command == "KEYS") {
    const auto refreshed_keys = minikv::parse_key_list_response(response);
    if (refreshed_keys.has_value()) {
        return keys.replace(*refreshed_keys);
    }
}
```

如果用户执行 `KEYS extra`，服务端返回 usage error，解析失败，不会污染缓存。

---

# 6. 为什么放到 client_history 模块里

`ClientKeyCache` 已经在 `client_history.hpp / cpp` 中维护。

把 `replace` 和 `parse_key_list_response` 放在同一模块，有两个好处：

```text
缓存替换规则集中在 ClientKeyCache 附近
解析逻辑可以被 client_history_tests 单元测试覆盖
```

如果只把解析写在 `client_main.cpp` 的匿名 namespace 里，就只能靠真实 smoke 覆盖，测试颗粒会粗一些。

---

# 7. 一句话总结

第四十一版把 v38 的本地 key cache 和 v40 的服务端 `KEYS` 接了起来：

```text
SET / DEL / LOAD
 -> 增量观察

KEYS
 -> 全量刷新
```

这让客户端补全缓存从“只知道自己看见过什么”进了一步，变成“可以主动向服务端对齐当前 key 集合”。
