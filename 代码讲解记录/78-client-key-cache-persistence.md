# 第七十八次讲解：第三十八版客户端 Key Cache 持久化核心

第三十八版完成 Roadmap 里的这一项：

```text
Add optional persisted client key cache for cross-session key completion.
```

也就是让 v36 引入的客户端 key 补全候选可以跨客户端会话保存。

涉及的核心文件：

```text
include/minikv/client_history.hpp
src/client_history.cpp
src/client_main.cpp
```

一句话总结：

```text
v38 新增 ClientKeyCache，并给 minikv_client 增加 --key-cache-file，让成功 SET 学到的 key 可以保存到文件，下次客户端启动后继续用于 Tab 补全。
```

---

# 1. v36 的 key 补全有什么限制

v36 已经支持：

```text
SET alpha one
 -> 客户端本会话记住 alpha

GET al + Tab
 -> 可以补 alpha / alpine 的公共前缀
```

但这个候选列表只存在于当前进程内存：

```text
关闭 minikv_client
 -> known_keys 丢失
```

所以 v38 做的是：

```text
显式指定一个 key cache 文件
 -> 启动时加载
 -> 运行中更新
 -> 下次启动继续使用
```

---

# 2. 为什么是可选参数

新增参数：

```text
--key-cache-file path
```

它不是默认开启。

原因是：

```text
不默认往用户目录写文件
避免脚本 smoke test 多出隐式状态
让用户自己决定不同 server 是否共用一个 key cache
```

这样保持 mini-kv 之前的风格：

```text
--history-file
 -> 显式开启历史持久化

--key-cache-file
 -> 显式开启 key 候选持久化
```

---

# 3. ClientKeyCache 的职责

`include/minikv/client_history.hpp` 新增：

```cpp
class ClientKeyCache {
public:
    explicit ClientKeyCache(std::size_t max_entries = 1000);

    bool add(std::string key);
    bool remove(std::string_view key);
    bool clear();
    bool empty() const;
    std::size_t size() const;
    const std::vector<std::string>& entries() const;
    std::size_t load_from_file(const std::filesystem::path& path);
    void save_to_file(const std::filesystem::path& path) const;
};
```

它和 `ClientHistory` 放在同一个模块里，是因为二者都是客户端本地状态：

```text
ClientHistory
 -> 命令历史

ClientKeyCache
 -> key 补全候选
```

---

# 4. add / remove / clear 为什么返回 bool

`ClientKeyCache` 的修改函数返回是否真的改变了缓存：

```text
add 新 key
 -> true

add 重复 key
 -> false

remove 已存在 key
 -> true

remove 不存在 key
 -> false

clear 非空缓存
 -> true

clear 空缓存
 -> false
```

这样 `client_main.cpp` 可以避免不必要的文件写入：

```text
缓存有变化
 -> save_to_file

缓存没变化
 -> 不写文件
```

---

# 5. 文件格式为什么是一行一个 key

key cache 文件格式很简单：

```text
alpha
alpine
user:1
```

选择这个格式的原因：

```text
容易人工查看
容易 diff
和 history file 的一行一条记录保持一致
当前 mini-kv 的 key 本身不支持空白字符
```

加载时会：

```text
跳过空行
去掉行尾 \r
去重
按 max_entries 裁剪旧 key
```

保存时会：

```text
创建父目录
截断重写
逐行写当前 entries
```

---

# 6. client_main 如何接入

`ClientOptions` 新增：

```cpp
std::optional<std::filesystem::path> key_cache_file;
```

命令行解析新增：

```text
--key-cache-file path
```

启动后：

```cpp
minikv::ClientKeyCache key_cache;

if (options.key_cache_file.has_value()) {
    key_cache.load_from_file(*options.key_cache_file);
}
```

每次读取交互输入前，把缓存传给 line editor：

```cpp
completion_options.key_candidates = key_cache.entries();
```

也就是说：

```text
key cache 文件
 -> ClientKeyCache
 -> LineEditorCompletionOptions::key_candidates
 -> Tab 补全
```

---

# 7. 什么时候更新 key cache

v38 保留 v36 的更新规则，只是把容器从 `std::vector` 换成 `ClientKeyCache`：

```text
SET key value + OK ...
 -> add(key)

DEL key + 1 / 0
 -> remove(key)

LOAD path + OK loaded ...
 -> clear()
```

这里 `DEL key` 返回 0 也会移除：

```text
服务端明确说 key 当前不存在
 -> 客户端缓存里的该 key 已陈旧
 -> 应该删除
```

---

# 8. 什么时候保存 key cache 文件

客户端收到响应后：

```cpp
const bool key_cache_changed = update_known_keys(key_cache, resolved.command, response);

if (key_cache_changed && options.key_cache_file.has_value()) {
    key_cache.save_to_file(*options.key_cache_file);
}
```

所以保存条件是：

```text
用户启用了 --key-cache-file
并且本轮命令确实改变了 key cache
```

例如：

```text
SET alpha one
 -> 文件写入 alpha

SET alpha two
 -> alpha 已存在，文件不重复写

DEL alpha
 -> 文件移除 alpha
```

---

# 9. 一句话总结

第三十八版让客户端 key 补全从“本会话记忆”升级为“可选跨会话记忆”：

```text
不传 --key-cache-file
 -> 和 v36 一样，只在当前客户端进程内记忆

传 --key-cache-file
 -> 本次学习到的 key 会保存，下次启动继续补全
```

它仍然是客户端侧能力，不改变服务端协议，也不引入全量 key 查询命令。
