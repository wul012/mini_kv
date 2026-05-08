# 第八次讲解：第三版 TTL 核心实现

这次讲解第三版最核心的代码变化：

```text
include/minikv/store.hpp
src/store.cpp
src/command.cpp
```

第三版的主题是：

```text
TTL 支持
```

也就是给 key 增加“过期时间”。

第二版之前，`Store` 只保存：

```text
key -> value
```

第三版之后，`Store` 保存的是：

```text
key -> Entry
```

而 `Entry` 里面包含：

```text
value       真正的字符串值
expires_at  可选的过期时间点
```

可以把它理解成：

```text
第一版 Store = 一个带锁的 unordered_map<string, string>
第三版 Store = 一个带锁的 unordered_map<string, Entry>
```

---

# 1. TTL 是什么

TTL 是：

```text
Time To Live
```

意思是：

```text
一个 key 还能活多久
```

第三版新增两个命令：

```text
EXPIRE key seconds
TTL key
```

例如：

```text
SET temp hello
EXPIRE temp 10
```

表示：

```text
temp 这个 key 从现在开始 10 秒后过期
```

过期后再执行：

```text
GET temp
```

应该返回：

```text
(nil)
```

也就是把它当成不存在。

---

# 2. 第三版的整体流程

TTL 的核心流程是：

```text
SET key value
 -> 保存 value
 -> 清空旧 TTL

EXPIRE key seconds
 -> 找到 key
 -> 设置 expires_at = 当前时间 + seconds

GET key
 -> 找到 key
 -> 检查是否过期
 -> 过期就删除并返回空
 -> 没过期就返回 value

TTL key
 -> key 不存在返回 -2
 -> key 存在但没有过期时间返回 -1
 -> key 存在且有过期时间返回剩余秒数
```

这里采用的是“懒清理”。

意思是：

```text
不会额外开一个后台线程定时删除过期 key
而是在 GET / CONTAINS / SIZE / SNAPSHOT / TTL 等访问路径上顺手清理
```

这对当前练习项目很合适。

因为它简单、可预测，也不需要引入后台调度线程。

---

# 3. `store.hpp` 新增 `<chrono>`

第三版在 `store.hpp` 里新增：

```cpp
#include <chrono>
```

`chrono` 是 C++ 标准库的时间工具。

这次用它表示：

```text
秒数
时间点
当前时间
```

比如：

```cpp
std::chrono::seconds
std::chrono::steady_clock::time_point
```

---

# 4. Store 新增公开方法

第三版在 `Store` 的 public 区域新增：

```cpp
bool expire(std::string_view key, std::chrono::seconds ttl);
std::optional<std::chrono::seconds> ttl(std::string_view key) const;
```

先看 `expire`：

```cpp
bool expire(std::string_view key, std::chrono::seconds ttl);
```

它负责给一个已经存在的 key 设置过期时间。

返回值含义是：

```text
true   设置成功
false  key 不存在、key 为空、ttl 不合法、或 key 已经过期
```

对应命令层的返回：

```text
1 = 成功
0 = 失败
```

---

再看 `ttl`：

```cpp
std::optional<std::chrono::seconds> ttl(std::string_view key) const;
```

它负责查询 key 的剩余存活时间。

返回值有三种情况：

```text
std::nullopt
 -> key 不存在，或者已经过期

std::chrono::seconds{-1}
 -> key 存在，但没有过期时间

非负 seconds
 -> key 存在，并且有过期时间，返回剩余秒数
```

命令层会把它翻译成文本：

```text
nullopt -> -2
-1      -> -1
其他秒数 -> 对应数字
```

这和 Redis 的 TTL 语义接近。

---

# 5. Store 内部新增 Clock

```cpp
using Clock = std::chrono::steady_clock;
```

这里给 `std::chrono::steady_clock` 起了一个短名字：

```text
Clock
```

为什么用 `steady_clock`？

因为 TTL 只关心：

```text
从现在开始过多少秒
```

不关心真实世界日期。

`steady_clock` 的特点是：

```text
单调递增
不受系统时间被手动调快或调慢影响
```

如果用系统时钟，用户把电脑时间往前或往后调，TTL 可能会变得奇怪。

所以这里用 `steady_clock` 更合适。

---

# 6. Store 内部新增 Entry

```cpp
struct Entry {
    std::string value;
    std::optional<Clock::time_point> expires_at;
};
```

这是第三版最关键的数据结构。

原来的 `data_` 直接保存字符串：

```text
key -> value
```

现在每个 key 保存一个 Entry：

```text
key -> {
  value: 字符串值
  expires_at: 可选过期时间点
}
```

`value` 是真正的数据。

比如：

```text
name -> mini-kv
```

这里的 `mini-kv` 就放在：

```cpp
Entry::value
```

---

`expires_at` 是：

```cpp
std::optional<Clock::time_point>
```

它是一个“可选时间点”。

如果没有过期时间：

```text
expires_at = std::nullopt
```

如果设置了 TTL：

```text
expires_at = 当前时间 + ttl 秒
```

所以一个永不过期的 key 类似：

```text
{
  value: "mini-kv",
  expires_at: null
}
```

一个 10 秒后过期的 key 类似：

```text
{
  value: "mini-kv",
  expires_at: now + 10s
}
```

---

# 7. Store 新增过期判断函数

```cpp
static bool is_expired(const Entry& entry, Clock::time_point now);
```

这个函数判断一个 Entry 在某个时间点是否已经过期。

为什么传入 `now`，而不是函数内部自己调用 `Clock::now()`？

因为很多地方会连续检查多个 key。

如果每次都调用 `Clock::now()`，时间点会略有不同。

传入同一个 `now` 可以让一次清理里的判断更一致。

---

实现是：

```cpp
bool Store::is_expired(const Entry& entry, Clock::time_point now) {
    return entry.expires_at.has_value() && *entry.expires_at <= now;
}
```

拆开看：

```cpp
entry.expires_at.has_value()
```

表示这个 key 有过期时间。

如果没有过期时间，就永远不算过期。

再看：

```cpp
*entry.expires_at <= now
```

表示过期时间点已经小于等于当前时间。

所以完整意思是：

```text
有过期时间
并且过期时间已经到了
```

---

# 8. Store 新增清理函数

```cpp
void prune_expired_locked(Clock::time_point now) const;
```

这个函数负责删除所有已经过期的 key。

名字里有 `locked`，意思是：

```text
调用它之前，外层必须已经持有 mutex_
```

这是一种常见命名习惯。

因为函数内部直接操作 `data_`，必须保证线程安全。

---

实现是：

```cpp
void Store::prune_expired_locked(Clock::time_point now) const {
    for (auto it = data_.begin(); it != data_.end();) {
        if (is_expired(it->second, now)) {
            it = data_.erase(it);
        } else {
            ++it;
        }
    }
}
```

这里不能写普通的：

```cpp
for (auto& item : data_) {
```

因为遍历时要删除元素。

所以用迭代器写法：

```cpp
for (auto it = data_.begin(); it != data_.end();)
```

如果当前 key 过期：

```cpp
it = data_.erase(it);
```

`erase` 会返回下一个有效迭代器。

如果没有过期：

```cpp
++it;
```

继续看下一个。

---

# 9. data_ 为什么变成 mutable

第三版里：

```cpp
mutable std::unordered_map<std::string, Entry> data_;
```

注意它是 `mutable`。

这是因为有些函数是 `const`：

```cpp
get(...) const
contains(...) const
size(...) const
ttl(...) const
snapshot(...) const
```

从语义上看，这些函数是“读取”。

但 TTL 的懒清理会在读取时删除过期 key。

例如：

```cpp
get("temp")
```

如果发现 `temp` 已经过期，就会执行：

```cpp
data_.erase(it);
```

这会修改内部存储。

所以为了允许 const 读取函数做内部清理，`data_` 被标记为 `mutable`。

可以理解成：

```text
对外仍然是读取操作
对内允许清理已经失效的数据
```

这和 `mutex_` 也是 `mutable` 的原因类似。

const 函数也需要加锁。

---

# 10. `set`：保存值并清除旧 TTL

第三版的 `set`：

```cpp
bool Store::set(std::string_view key, std::string_view value) {
    if (key.empty()) {
        return false;
    }

    std::lock_guard lock(mutex_);
    const auto [it, inserted] = data_.insert_or_assign(std::string{key}, Entry{std::string{value}, std::nullopt});
    (void)it;
    return inserted;
}
```

前面逻辑不变：

```text
空 key 不允许
加锁
插入或更新
返回是否是新插入
```

关键变化在这里：

```cpp
Entry{std::string{value}, std::nullopt}
```

以前保存的是：

```cpp
std::string{value}
```

现在保存的是：

```text
value = 新值
expires_at = null
```

也就是说：

```text
每次 SET 都会清除旧 TTL
```

例如：

```text
SET session open
EXPIRE session 10
SET session reset
TTL session
```

最后 `TTL session` 会返回：

```text
-1
```

因为第二次 `SET` 已经把过期时间清掉了。

---

# 11. `get`：读取时顺手清理过期 key

第三版的 `get`：

```cpp
std::optional<std::string> Store::get(std::string_view key) const {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return std::nullopt;
    }

    if (is_expired(it->second, Clock::now())) {
        data_.erase(it);
        return std::nullopt;
    }

    return it->second.value;
}
```

先找 key：

```cpp
const auto it = data_.find(std::string{key});
```

找不到就返回：

```cpp
std::nullopt
```

这和以前一样。

---

找到后多了一步：

```cpp
if (is_expired(it->second, Clock::now())) {
    data_.erase(it);
    return std::nullopt;
}
```

如果 key 已经过期：

```text
从 data_ 里删除
返回空
```

所以过期 key 对外看起来就像不存在。

---

没过期才返回：

```cpp
return it->second.value;
```

注意现在 value 在 Entry 里面，所以要写：

```cpp
it->second.value
```

---

# 12. `erase`：过期 key 删除时返回 false

第三版的 `erase`：

```cpp
bool Store::erase(std::string_view key) {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return false;
    }

    if (is_expired(it->second, Clock::now())) {
        data_.erase(it);
        return false;
    }

    return data_.erase(std::string{key}) > 0;
}
```

如果 key 不存在，返回：

```text
false
```

如果 key 已经过期，也先删掉，再返回：

```text
false
```

为什么过期 key 返回 false？

因为从用户角度看：

```text
过期 key 已经不存在
```

所以：

```text
DEL expired_key
```

应该像删除不存在的 key 一样返回：

```text
0
```

---

# 13. `contains`：过期时返回 false

```cpp
bool Store::contains(std::string_view key) const {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return false;
    }

    if (is_expired(it->second, Clock::now())) {
        data_.erase(it);
        return false;
    }

    return true;
}
```

逻辑和 `get` 很像。

区别是：

```text
get 返回 value
contains 只返回 true / false
```

如果 key 已经过期：

```text
删除它
返回 false
```

---

# 14. `size`：统计前先清理

```cpp
std::size_t Store::size() const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());
    return data_.size();
}
```

如果不清理，`size()` 可能会把已经过期的 key 也算进去。

所以第三版先执行：

```cpp
prune_expired_locked(Clock::now());
```

再返回：

```cpp
data_.size()
```

这样 `SIZE` 命令看到的是：

```text
当前仍然有效的 key 数量
```

---

# 15. `expire`：设置过期时间

```cpp
bool Store::expire(std::string_view key, std::chrono::seconds ttl) {
    if (key.empty() || ttl.count() <= 0) {
        return false;
    }
```

先检查：

```text
key 不能为空
ttl 必须大于 0 秒
```

所以：

```text
EXPIRE name 0
EXPIRE name -5
```

都不是合法 TTL。

---

加锁并查找 key：

```cpp
std::lock_guard lock(mutex_);
const auto it = data_.find(std::string{key});
if (it == data_.end()) {
    return false;
}
```

如果 key 不存在，返回：

```text
false
```

命令层会显示：

```text
0
```

---

检查是否已经过期：

```cpp
const auto now = Clock::now();
if (is_expired(it->second, now)) {
    data_.erase(it);
    return false;
}
```

如果 key 虽然在 map 里，但已经过期，就先删掉。

然后返回：

```text
false
```

因为过期 key 对外等于不存在。

---

真正设置过期时间：

```cpp
it->second.expires_at = now + ttl;
return true;
```

比如：

```text
ttl = 10s
now = 当前时间点
```

那么：

```text
expires_at = now + 10s
```

---

# 16. `ttl`：查询剩余时间

```cpp
std::optional<std::chrono::seconds> Store::ttl(std::string_view key) const {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return std::nullopt;
    }
```

如果 key 不存在，返回：

```cpp
std::nullopt
```

命令层会翻译成：

```text
-2
```

---

检查是否过期：

```cpp
const auto now = Clock::now();
if (is_expired(it->second, now)) {
    data_.erase(it);
    return std::nullopt;
}
```

如果已经过期，也删除并返回：

```cpp
std::nullopt
```

也就是命令层的：

```text
-2
```

---

如果 key 存在但没有设置 TTL：

```cpp
if (!it->second.expires_at.has_value()) {
    return std::chrono::seconds{-1};
}
```

返回：

```text
-1 秒
```

命令层最终显示：

```text
-1
```

表示：

```text
key 存在，但没有过期时间
```

---

如果 key 存在并且有过期时间：

```cpp
return std::chrono::ceil<std::chrono::seconds>(*it->second.expires_at - now);
```

这里先算：

```text
expires_at - now
```

得到剩余时间。

然后用：

```cpp
std::chrono::ceil<std::chrono::seconds>(...)
```

向上取整到秒。

为什么要向上取整？

因为如果剩余：

```text
0.3 秒
```

向下取整会变成：

```text
0 秒
```

但 key 实际还没过期。

向上取整后是：

```text
1 秒
```

更符合用户直觉。

---

# 17. `snapshot`：快照不包含过期 key

```cpp
std::vector<std::pair<std::string, std::string>> Store::snapshot() const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());
```

快照之前先清理过期 key。

这样返回的快照只包含当前有效数据。

---

构造返回列表：

```cpp
std::vector<std::pair<std::string, std::string>> items;
items.reserve(data_.size());

for (const auto& [key, entry] : data_) {
    items.emplace_back(key, entry.value);
}
```

因为 `data_` 的 value 从 `std::string` 变成了 `Entry`，所以这里也要从：

```cpp
value
```

改成：

```cpp
entry.value
```

---

最后仍然排序：

```cpp
std::ranges::sort(items, {}, &std::pair<std::string, std::string>::first);
```

这保持了第一版的稳定行为：

```text
snapshot 按 key 排序
```

---

# 18. `command.cpp` 新增 `<chrono>` 和 `<optional>`

第三版命令层新增：

```cpp
#include <chrono>
#include <optional>
```

`chrono` 用来创建：

```cpp
std::chrono::seconds
```

`optional` 用来表示：

```text
秒数解析成功或失败
```

---

# 19. 解析正整数秒数

第三版新增辅助函数：

```cpp
std::optional<std::chrono::seconds> parse_positive_seconds(std::string_view text) {
    std::istringstream input{std::string{text}};
    long long seconds = 0;
    char extra = '\0';

    if (!(input >> seconds) || seconds <= 0 || (input >> extra)) {
        return std::nullopt;
    }

    return std::chrono::seconds{seconds};
}
```

它负责把用户输入的秒数字符串转成：

```cpp
std::chrono::seconds
```

比如：

```text
"10" -> 10s
```

---

先把 `string_view` 包成字符串流：

```cpp
std::istringstream input{std::string{text}};
```

然后准备：

```cpp
long long seconds = 0;
char extra = '\0';
```

`seconds` 保存解析出的数字。

`extra` 用来检查后面有没有多余字符。

---

核心判断：

```cpp
if (!(input >> seconds) || seconds <= 0 || (input >> extra)) {
    return std::nullopt;
}
```

它拒绝三种情况：

```text
不是数字
小于等于 0
数字后面还有多余字符
```

比如：

```text
abc
0
-5
10abc
```

都会失败。

成功时返回：

```cpp
std::chrono::seconds{seconds}
```

---

# 20. `EXPIRE` 命令分支

第三版在 `CommandProcessor::execute` 里新增：

```cpp
if (command == "EXPIRE") {
    std::string key;
    std::string seconds_text;
    input >> key >> seconds_text;

    const auto seconds = parse_positive_seconds(seconds_text);
    if (key.empty() || !seconds.has_value() || has_extra_token(input)) {
        return usage("EXPIRE key seconds");
    }

    return {store_.expire(key, *seconds) ? "1" : "0"};
}
```

先读取参数：

```cpp
input >> key >> seconds_text;
```

对应命令：

```text
EXPIRE key seconds
```

比如：

```text
EXPIRE session 10
```

读取结果是：

```text
key = session
seconds_text = 10
```

---

解析秒数：

```cpp
const auto seconds = parse_positive_seconds(seconds_text);
```

如果秒数不是正整数，就返回空。

---

检查用法：

```cpp
if (key.empty() || !seconds.has_value() || has_extra_token(input)) {
    return usage("EXPIRE key seconds");
}
```

三种情况会报 usage：

```text
没有 key
seconds 不合法
参数太多
```

返回文本是：

```text
ERR usage: EXPIRE key seconds
```

---

真正执行：

```cpp
return {store_.expire(key, *seconds) ? "1" : "0"};
```

如果设置成功，返回：

```text
1
```

如果 key 不存在或已经过期，返回：

```text
0
```

---

# 21. `TTL` 命令分支

第三版新增：

```cpp
if (command == "TTL") {
    std::string key;
    input >> key;

    if (key.empty() || has_extra_token(input)) {
        return usage("TTL key");
    }

    const auto ttl = store_.ttl(key);
    if (!ttl.has_value()) {
        return {"-2"};
    }

    return {std::to_string(ttl->count())};
}
```

先读取 key：

```cpp
input >> key;
```

正确用法是：

```text
TTL key
```

如果没有 key 或参数太多：

```cpp
return usage("TTL key");
```

返回：

```text
ERR usage: TTL key
```

---

查询 Store：

```cpp
const auto ttl = store_.ttl(key);
```

如果 Store 返回空：

```cpp
if (!ttl.has_value()) {
    return {"-2"};
}
```

说明 key 不存在或已经过期。

所以命令返回：

```text
-2
```

---

如果有值：

```cpp
return {std::to_string(ttl->count())};
```

可能是：

```text
-1
```

表示存在但没有过期时间。

也可能是：

```text
10
```

表示还有 10 秒左右。

---

# 22. HELP 文本更新

第三版把帮助文本从：

```text
SET
GET
DEL
SIZE
HELP
EXIT
```

扩展为：

```cpp
return "Commands:\n"
       "  SET key value\n"
       "  GET key\n"
       "  DEL key\n"
       "  EXPIRE key seconds\n"
       "  TTL key\n"
       "  SIZE\n"
       "  HELP\n"
       "  EXIT";
```

这样 CLI、TCP server 和 TCP client 里的 `HELP` 都会看到新命令。

因为它们最终都调用同一个：

```cpp
CommandProcessor::help_text()
```

---

# 23. 第三版命令行为举例

正常设置 TTL：

```text
SET temp hello
 -> OK inserted

EXPIRE temp 3
 -> 1

TTL temp
 -> 3 或 2 或 1
```

等待超过 3 秒后：

```text
GET temp
 -> (nil)

TTL temp
 -> -2
```

没有 TTL 的 key：

```text
SET name mini-kv
TTL name
 -> -1
```

不存在的 key：

```text
TTL missing
 -> -2

EXPIRE missing 10
 -> 0
```

非法 TTL：

```text
EXPIRE name 0
 -> ERR usage: EXPIRE key seconds
```

---

# 第八次总结

第三版 TTL 的核心是在 `Store` 里把 value 包装成 Entry：

```text
Entry = value + optional expires_at
```

然后在读取和统计路径上做懒清理：

```text
访问 key
 -> 检查 expires_at
 -> 过期就删除并当作不存在
```

`CommandProcessor` 则把文本命令翻译成 Store 操作：

```text
EXPIRE key seconds -> store.expire(...)
TTL key            -> store.ttl(...)
```

一句话总结：

```text
第三版让 mini-kv 从“只会永久保存内存 key”前进到“可以让 key 按秒自动过期”的阶段。
```
