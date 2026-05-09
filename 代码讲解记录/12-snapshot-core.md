# 第十二次讲解：第五版快照核心实现

这次讲第五版最核心的新增能力：

```text
Manual snapshots
```

也就是：

```text
手动快照持久化
```

第四版已经有了 WAL：

```text
每次写命令
 -> 追加一条日志
 -> 下次启动 replay 日志
```

第五版新增的是另一种持久化思路：

```text
把当前 Store 里的完整数据集
一次性保存到一个 snapshot 文件
```

之后可以执行：

```text
LOAD snapshot_path
```

把 snapshot 文件里的数据重新加载回 Store。

可以把第五版理解成：

```text
第四版：
  WAL 记录每一步修改
  适合增量恢复

第五版：
  Snapshot 记录某一刻完整状态
  适合保存基线数据
```

---

# 1. 第五版快照的整体链路

第五版新增的快照链路是：

```text
用户执行 SAVE path
 -> CommandProcessor 调用 SnapshotFile::save
 -> Store 导出当前未过期数据
 -> SnapshotFile 写入 snapshot 文件
 -> 返回 OK saved n
```

加载链路是：

```text
用户执行 LOAD path
 -> CommandProcessor 调用 SnapshotFile::load
 -> SnapshotFile 解析 snapshot 文件
 -> Store 替换为 snapshot 中的数据
 -> 返回 OK loaded n
```

这里要注意：

```text
SAVE / LOAD 是手动命令
不是程序启动时自动执行
```

也就是说，WAL 和 Snapshot 在第五版里是并列能力：

```text
WAL:
  通过启动参数启用
  写命令自动 append
  启动时自动 replay

Snapshot:
  通过 SAVE / LOAD 命令触发
  保存或替换当前数据集
```

---

# 2. 为什么需要 Store 导出 SnapshotItem

第五版在 `Store` 里新增：

```cpp
struct SnapshotItem {
    std::string key;
    std::string value;
    std::optional<TimePoint> expires_at;
};
```

以前 `Store::snapshot()` 只返回：

```cpp
std::vector<std::pair<std::string, std::string>>
```

也就是：

```text
key
value
```

但是第五版快照需要保留 TTL。

如果只保存 value，不保存过期时间，那么加载后所有 key 都会变成永久 key。

这就会破坏第三版 TTL 的语义。

所以第五版新增 `SnapshotItem`，它包含三部分：

```text
key        键
value      值
expires_at 可选绝对过期时间
```

`expires_at` 是：

```cpp
std::optional<TimePoint>
```

表示：

```text
没有值 -> 永不过期
有值   -> 到这个绝对时间点过期
```

---

# 3. Store 新增的快照接口

第五版在 `Store` 里新增两个接口：

```cpp
std::vector<SnapshotItem> snapshot_items() const;
void restore_snapshot(std::vector<SnapshotItem> items);
```

第一个用于保存：

```text
把当前 Store 导出成 SnapshotItem 列表
```

第二个用于加载：

```text
用 SnapshotItem 列表替换当前 Store
```

这两个接口把快照文件格式和 Store 内部结构隔开。

也就是说：

```text
Store 不关心文件怎么写
SnapshotFile 不直接碰 Store 的 unordered_map
```

职责边界更清楚。

---

# 4. `snapshot()` 继续保留

第五版没有删除原来的：

```cpp
std::vector<std::pair<std::string, std::string>> Store::snapshot() const
```

而是改成：

```cpp
std::vector<std::pair<std::string, std::string>> Store::snapshot() const {
    const auto records = snapshot_items();

    std::vector<std::pair<std::string, std::string>> items;
    items.reserve(records.size());

    for (const auto& item : records) {
        items.emplace_back(item.key, item.value);
    }

    return items;
}
```

它现在复用：

```cpp
snapshot_items()
```

然后丢掉 `expires_at`，只返回 key-value。

这样做的好处是：

```text
旧接口还能用
新接口负责完整快照
过期清理和排序逻辑只保留一份
```

---

# 5. `snapshot_items()`：导出当前有效数据

核心代码：

```cpp
std::vector<Store::SnapshotItem> Store::snapshot_items() const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());

    std::vector<SnapshotItem> items;
    items.reserve(data_.size());

    for (const auto& [key, entry] : data_) {
        items.push_back(SnapshotItem{key, entry.value, entry.expires_at});
    }

    std::ranges::sort(items, {}, &SnapshotItem::key);
    return items;
}
```

第一步：

```cpp
std::lock_guard lock(mutex_);
```

因为 Store 是线程安全的，导出快照时必须加锁。

否则 TCP server 多个客户端同时写数据时，快照可能读到一半被修改。

---

第二步：

```cpp
prune_expired_locked(Clock::now());
```

保存快照前先清理已过期 key。

这样 snapshot 文件里不会保存已经失效的数据。

这条规则很重要：

```text
过期 key 不应该因为 SAVE 而复活
```

---

第三步：

```cpp
for (const auto& [key, entry] : data_) {
    items.push_back(SnapshotItem{key, entry.value, entry.expires_at});
}
```

把每条数据复制到 `SnapshotItem`。

这里复制的是：

```text
key
value
expires_at
```

所以 TTL 信息会跟着快照一起保存。

---

最后排序：

```cpp
std::ranges::sort(items, {}, &SnapshotItem::key);
```

`data_` 是：

```cpp
std::unordered_map
```

它的遍历顺序不稳定。

如果不排序，同样的数据每次保存出来的文件顺序可能不同。

排序后，snapshot 文件更稳定，也更容易调试和比较。

---

# 6. `restore_snapshot()`：用快照替换当前数据

核心代码：

```cpp
void Store::restore_snapshot(std::vector<SnapshotItem> items) {
    std::lock_guard lock(mutex_);
    data_.clear();

    const auto now = Clock::now();
    for (auto& item : items) {
        if (item.key.empty()) {
            continue;
        }

        Entry entry{std::move(item.value), item.expires_at};
        if (is_expired(entry, now)) {
            continue;
        }

        data_.insert_or_assign(std::move(item.key), std::move(entry));
    }
}
```

第一步仍然是加锁：

```cpp
std::lock_guard lock(mutex_);
```

加载快照会替换整个 Store。

这比普通 `SET` 改动范围更大，所以更要保证线程安全。

---

然后清空当前数据：

```cpp
data_.clear();
```

这体现了 `LOAD` 的语义：

```text
LOAD 不是合并
LOAD 是替换当前内存数据集
```

如果当前 Store 里有：

```text
old = value
```

而 snapshot 文件里没有 `old`，加载后 `old` 就不存在了。

---

恢复时跳过空 key：

```cpp
if (item.key.empty()) {
    continue;
}
```

Store 本身不接受空 key。

所以快照恢复时也不恢复空 key。

---

恢复时也跳过已过期数据：

```cpp
if (is_expired(entry, now)) {
    continue;
}
```

这和保存时的规则一致：

```text
过期 key 不保存
过期记录不加载
```

这样即使 snapshot 文件里有一个已经过期的记录，`LOAD` 也不会让它复活。

---

# 7. `snapshot.hpp` 的角色

第五版新增文件：

```text
include/minikv/snapshot.hpp
```

它定义了：

```cpp
class SnapshotFile
```

这个类就是 snapshot 文件读写器。

它只有两个静态函数：

```cpp
static bool save(const Store& store, const std::filesystem::path& path, std::size_t* saved = nullptr);
static bool load(Store& store, const std::filesystem::path& path, std::size_t* loaded = nullptr);
```

为什么是静态函数？

因为当前 snapshot 模块不需要长期保存状态。

它每次只需要：

```text
拿到 Store
拿到 path
执行一次保存或加载
返回成功失败
```

所以不需要创建：

```cpp
SnapshotFile snapshot{path};
```

直接调用：

```cpp
SnapshotFile::save(store, path, &saved)
SnapshotFile::load(store, path, &loaded)
```

就够了。

---

# 8. snapshot 文件格式

第五版 snapshot 文件第一行是：

```text
MINIKV_SNAPSHOT 1
```

这行叫文件头。

代码里是：

```cpp
constexpr std::string_view snapshot_header = "MINIKV_SNAPSHOT 1";
```

它的作用是：

```text
确认这是 mini-kv snapshot 文件
确认当前文件格式版本是 1
```

后续如果第六版、第七版要改格式，可以升级成：

```text
MINIKV_SNAPSHOT 2
```

加载时就能识别版本。

---

每条数据记录格式是：

```text
ENTRY expires_at "key" "value"
```

例如：

```text
ENTRY - "phrase" "hello from snapshot"
ENTRY 1778251721471 "name" "mini-kv"
```

其中：

```text
ENTRY       记录类型
-           表示没有过期时间
数字        表示绝对过期时间 epoch millis
"key"       被 std::quoted 保护的 key
"value"     被 std::quoted 保护的 value
```

---

# 9. 为什么使用 `std::quoted`

保存时写入：

```cpp
output << "ENTRY " << format_expires_at(item.expires_at) << ' ' << std::quoted(item.key) << ' '
       << std::quoted(item.value) << '\n';
```

这里用了：

```cpp
std::quoted
```

它会把字符串写成带引号的形式。

比如 value 是：

```text
hello from snapshot
```

保存后是：

```text
"hello from snapshot"
```

这样加载时可以完整读回带空格的 value。

否则如果直接用空格分隔：

```text
ENTRY - phrase hello from snapshot
```

解析时就不知道 value 到底从哪里开始、到哪里结束。

---

# 10. `format_expires_at`

```cpp
std::string format_expires_at(std::optional<Store::TimePoint> expires_at) {
    if (!expires_at.has_value()) {
        return "-";
    }

    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at->time_since_epoch()).count();
    return std::to_string(epoch_millis);
}
```

这个函数把 Store 里的过期时间转换成 snapshot 文件里的文本。

如果没有过期时间：

```cpp
return "-";
```

文件里就写：

```text
-
```

意思是：

```text
这个 key 永不过期
```

---

如果有过期时间，就转成 epoch 毫秒：

```cpp
expires_at->time_since_epoch()
```

再转成：

```cpp
std::chrono::milliseconds
```

最后写成数字字符串。

这和第四版 WAL 的 `EXPIREAT` 思路一致：

```text
跨进程持久化 TTL
必须保存绝对时间
不能保存相对秒数
```

---

# 11. `parse_expires_at`

```cpp
bool parse_expires_at(const std::string& text, std::optional<Store::TimePoint>& expires_at) {
    if (text == "-") {
        expires_at = std::nullopt;
        return true;
    }

    try {
        std::size_t parsed = 0;
        const long long epoch_millis = std::stoll(text, &parsed);
        if (parsed != text.size()) {
            return false;
        }

        expires_at = Store::TimePoint{std::chrono::milliseconds{epoch_millis}};
        return true;
    } catch (...) {
        return false;
    }
}
```

这个函数做的是反向转换：

```text
snapshot 文件里的文本
 -> Store 能理解的 expires_at
```

如果文本是：

```text
-
```

就表示：

```cpp
std::nullopt
```

也就是没有过期时间。

---

如果不是 `-`，就尝试解析成整数：

```cpp
std::stoll(text, &parsed)
```

这里额外检查：

```cpp
parsed != text.size()
```

是为了防止半截数字被误认为合法。

比如：

```text
123abc
```

`std::stoll` 能解析出 `123`。

但 `parsed` 不等于字符串长度，所以这里会返回：

```cpp
false
```

表示 snapshot 文件格式错误。

---

# 12. `SnapshotFile::save`

核心代码：

```cpp
bool SnapshotFile::save(const Store& store, const std::filesystem::path& path, std::size_t* saved) {
    const auto items = store.snapshot_items();
```

保存前先从 Store 导出快照项。

这里拿到的是一份拷贝：

```text
vector<SnapshotItem>
```

后面写文件时不再持有 Store 的锁。

这有个好处：

```text
Store 只在导出瞬间加锁
写文件慢一点也不会一直阻塞其他 Store 操作
```

---

如果路径带父目录，就先创建目录：

```cpp
if (path.has_parent_path()) {
    std::error_code error;
    std::filesystem::create_directories(path.parent_path(), error);
    if (error) {
        return false;
    }
}
```

比如：

```text
data\mini-kv.snapshot
```

如果 `data` 不存在，就自动创建。

创建失败则返回：

```cpp
false
```

---

打开文件：

```cpp
std::ofstream output{path, std::ios::trunc};
if (!output) {
    return false;
}
```

`std::ios::trunc` 表示：

```text
覆盖写
```

这符合 `SAVE` 的语义：

```text
保存当前完整状态
不是追加历史记录
```

这和 WAL 的：

```cpp
std::ios::app
```

正好相反。

---

写文件头：

```cpp
output << snapshot_header << '\n';
```

然后逐条写 `ENTRY`：

```cpp
for (const auto& item : items) {
    output << "ENTRY " << format_expires_at(item.expires_at) << ' ' << std::quoted(item.key) << ' '
           << std::quoted(item.value) << '\n';
}
```

最后刷新并检查：

```cpp
output.flush();
if (!output) {
    return false;
}
```

如果写入失败，就返回 false。

---

保存数量：

```cpp
if (saved != nullptr) {
    *saved = items.size();
}
```

`saved` 是可选输出参数。

命令层用它返回：

```text
OK saved 1
OK saved 2
```

如果调用方不关心数量，可以传：

```cpp
nullptr
```

---

# 13. `SnapshotFile::load`

加载先打开文件：

```cpp
std::ifstream input{path};
if (!input) {
    return false;
}
```

如果文件不存在，或者打不开，返回：

```cpp
false
```

命令层会把它转换成：

```text
ERR snapshot load failed
```

---

读取并校验文件头：

```cpp
std::string header;
if (!std::getline(input, header) || header != snapshot_header) {
    return false;
}
```

如果不是：

```text
MINIKV_SNAPSHOT 1
```

就不加载。

这样可以避免把普通文本文件误当成 snapshot。

---

逐条读取记录：

```cpp
std::vector<Store::SnapshotItem> items;
std::string tag;
while (input >> tag) {
    if (tag != "ENTRY") {
        return false;
    }
```

每条记录必须以：

```text
ENTRY
```

开头。

如果出现其他 tag，说明文件格式不对。

---

读取过期时间、key、value：

```cpp
std::string expires_text;
std::string key;
std::string value;
if (!(input >> expires_text >> std::quoted(key) >> std::quoted(value))) {
    return false;
}
```

这里加载时也使用：

```cpp
std::quoted
```

所以能读回保存时带引号的字符串。

比如：

```text
"hello from snapshot"
```

会还原成：

```text
hello from snapshot
```

---

解析过期时间：

```cpp
std::optional<Store::TimePoint> expires_at;
if (!parse_expires_at(expires_text, expires_at)) {
    return false;
}
```

如果过期时间不是 `-`，也不是合法数字，就返回失败。

---

加入待恢复列表：

```cpp
items.push_back(Store::SnapshotItem{std::move(key), std::move(value), expires_at});
```

这里先不直接写入 Store。

而是先把整个文件解析完。

这样可以避免：

```text
文件前半段合法
已经改了一部分 Store
后半段突然格式错误
```

第五版的策略是：

```text
先完整解析
解析成功后再 restore_snapshot
```

---

循环结束后检查：

```cpp
if (!input.eof()) {
    return false;
}
```

如果不是正常读到文件结尾，说明读取过程中出现了异常状态。

这时也不加载。

---

最后恢复 Store：

```cpp
const auto count = items.size();
store.restore_snapshot(std::move(items));
```

这里会清空当前 Store，并恢复 snapshot 中未过期的数据。

然后写入加载数量：

```cpp
if (loaded != nullptr) {
    *loaded = count;
}
```

命令层用它返回：

```text
OK loaded n
```

---

# 14. CommandProcessor 引入快照模块

第五版在 `src/command.cpp` 新增：

```cpp
#include "minikv/snapshot.hpp"
```

这样命令处理器就可以调用：

```cpp
SnapshotFile::save
SnapshotFile::load
```

也就是说：

```text
SAVE / LOAD 不需要 CLI 或 TCP server 单独实现
```

它们只要把用户输入交给：

```text
CommandProcessor
```

就自动支持快照。

---

# 15. 快照错误统一返回

第五版新增：

```cpp
CommandResult snapshot_error(std::string_view action) {
    return {std::string{"ERR snapshot "} + std::string{action} + " failed"};
}
```

保存失败时返回：

```text
ERR snapshot save failed
```

加载失败时返回：

```text
ERR snapshot load failed
```

这和第四版 WAL 的：

```text
ERR wal append failed
```

风格保持一致。

---

# 16. `SAVE path`

第五版新增命令：

```cpp
if (command == "SAVE") {
    std::string path;
    std::getline(input >> std::ws, path);

    if (path.empty()) {
        return usage("SAVE path");
    }

    std::size_t saved = 0;
    if (!SnapshotFile::save(store_, path, &saved)) {
        return snapshot_error("save");
    }

    return {std::string{"OK saved "} + std::to_string(saved)};
}
```

先读取路径：

```cpp
std::getline(input >> std::ws, path);
```

这里用 `getline`，表示：

```text
SAVE 后面的整段文本都当作 path
```

所以路径里如果有空格，也能作为一个整体传给 `SnapshotFile::save`。

---

如果 path 为空：

```cpp
return usage("SAVE path");
```

返回：

```text
ERR usage: SAVE path
```

---

执行保存：

```cpp
std::size_t saved = 0;
if (!SnapshotFile::save(store_, path, &saved)) {
    return snapshot_error("save");
}
```

如果保存失败，返回：

```text
ERR snapshot save failed
```

成功则返回：

```text
OK saved n
```

其中 `n` 是保存的有效 key 数量。

---

# 17. `LOAD path`

第五版新增命令：

```cpp
if (command == "LOAD") {
    std::string path;
    std::getline(input >> std::ws, path);

    if (path.empty()) {
        return usage("LOAD path");
    }

    std::size_t loaded = 0;
    if (!SnapshotFile::load(store_, path, &loaded)) {
        return snapshot_error("load");
    }

    return {std::string{"OK loaded "} + std::to_string(loaded)};
}
```

整体结构和 `SAVE` 对称。

区别是调用：

```cpp
SnapshotFile::load(store_, path, &loaded)
```

成功时返回：

```text
OK loaded n
```

失败时返回：

```text
ERR snapshot load failed
```

---

`LOAD` 有一个非常重要的语义：

```text
它会替换当前 Store
```

因为底层调用的是：

```cpp
store.restore_snapshot(...)
```

而 `restore_snapshot` 里第一步是：

```cpp
data_.clear();
```

所以 `LOAD` 不是把 snapshot 数据追加到当前数据里。

它是：

```text
用 snapshot 文件作为当前内存数据的新基线
```

---

# 18. HELP 新增 SAVE / LOAD

第五版更新了 `help_text()`：

```cpp
return "Commands:\n"
       "  SET key value\n"
       "  GET key\n"
       "  DEL key\n"
       "  EXPIRE key seconds\n"
       "  TTL key\n"
       "  SIZE\n"
       "  SAVE path\n"
       "  LOAD path\n"
       "  HELP\n"
       "  EXIT";
```

这样用户在 CLI 或 TCP client 里输入：

```text
HELP
```

就能看到快照命令。

---

# 19. 快照和 WAL 的关系

第五版没有把 snapshot 自动接进 WAL。

也就是说：

```text
SAVE 本身不会写 WAL
LOAD 本身也不会写 WAL
```

这是一个刻意保持简单的设计。

当前语义是：

```text
WAL 负责记录写命令增量
Snapshot 负责手动保存完整状态
```

README 里也提醒：

```text
LOAD is a manual state replacement command;
pair it with WAL intentionally if you want both baseline snapshots and later write replay.
```

意思是：

```text
如果后面要组合 snapshot + WAL
需要明确设计二者的顺序和截断策略
```

比如未来可能会做：

```text
启动时先 LOAD snapshot
再 replay snapshot 之后的 WAL
```

但第五版先把手动 snapshot 单独跑通。

---

# 20. 第五版快照的安全边界

第五版有几个关键保护：

```text
保存前清理过期 key
加载时跳过已过期记录
加载前完整解析文件
文件头必须匹配
过期时间必须完整解析
写文件失败会返回错误
```

这些保护让 snapshot 更像一个明确格式的数据文件，而不是随便读写文本。

---

# 第十二次总结

第五版快照核心由三部分组成：

```text
Store::snapshot_items()
 -> 导出 key / value / expires_at

SnapshotFile::save()
 -> 写 MINIKV_SNAPSHOT 1 和 ENTRY 行

SnapshotFile::load()
 -> 解析文件并调用 Store::restore_snapshot()
```

`CommandProcessor` 只负责把用户命令接到快照模块：

```text
SAVE path -> SnapshotFile::save
LOAD path -> SnapshotFile::load
```

一句话总结：

```text
第五版让 mini-kv 不只会“记录写入过程”，还会“保存某一刻完整状态”，并且这个完整状态包含 TTL 信息，加载时也不会让过期 key 复活。
```
