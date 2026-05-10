# 第四十九次讲解：第二十四版 WAL compact 核心

第四十八次讲了第二十三版 WAL checksum 的测试、README、归档和整体增删改。

这次讲第二十四版的核心代码：

```text
include/minikv/wal.hpp
src/wal.cpp
src/command.cpp
```

第二十四版新增一个用户命令：

```text
COMPACT
```

它解决的问题是：

```text
WAL 长时间运行后会越来越长
里面会保存很多已经失效的历史操作
启动 replay 时仍然要从头读到尾
```

比如：

```text
SET name old
SET name new
SET gone value
DEL gone
```

压缩后只需要保存当前仍然有效的状态：

```text
SET name new
```

第二十四版的目标就是：

```text
把 WAL 重写为当前 live key 的最小 WAL2 记录
删除已经被覆盖、删除、过期的历史
继续保留 checksum 和 TTL 恢复能力
```

---

# 1. 为什么要做 WAL compact

第四版加入 WAL 后，写命令会持续 append：

```text
SET
DEL
EXPIREAT
```

第十九版让 replay 能跳过坏记录、报告 applied / skipped / truncated。

第二十三版让新 WAL 记录变成：

```text
WAL2 <checksum> <record>
```

但这几个版本都没有解决一个问题：

```text
WAL 文件会无限增长
```

如果一个 key 被更新 1000 次：

```text
SET k v1
SET k v2
...
SET k v1000
```

当前内存里只需要：

```text
k = v1000
```

但 WAL replay 仍然要应用前面 999 条历史。

所以 compact 的价值是：

```text
用当前 Store 状态生成一份新的短 WAL
后续启动时只 replay live state
```

---

# 2. 头文件新增 compact API

`include/minikv/wal.hpp` 新增：

```cpp
bool compact(const Store& store, std::size_t* compacted = nullptr);
```

参数含义：

```text
store
 -> 当前内存数据库

compacted
 -> 可选输出参数，返回写入 compact WAL 的 live key 数量
```

返回值：

```text
true
 -> compact 成功

false
 -> 创建目录、写临时文件、flush、close 或替换目标 WAL 失败
```

这里没有把 compact 放在 `CommandProcessor` 里实现。

原因是：

```text
compact 属于 WAL 文件自身能力
CommandProcessor 只负责把用户命令映射到这个能力
```

---

# 3. compact 输出的不是 snapshot

mini-kv 已经有 Snapshot：

```text
SAVE path
LOAD path
```

但 WAL compact 输出的仍然是 WAL。

差别是：

```text
Snapshot
 -> 文件格式是 MINIKV_SNAPSHOT 1 + ENTRY
 -> 用 LOAD 手动替换当前 Store

WAL compact
 -> 文件格式仍然是 WAL2
 -> 程序启动时自动 replay
```

所以 compact 后的 WAL 仍然长这样：

```text
WAL2 <checksum> SET key value
WAL2 <checksum> EXPIREAT key epoch_millis
```

它不是 snapshot 文件。

它是“由当前状态重新生成的一份短 WAL”。

---

# 4. TempWalFile 的角色

`src/wal.cpp` 新增：

```cpp
class TempWalFile {
public:
    explicit TempWalFile(std::filesystem::path path) : path_(std::move(path)) {}

    ~TempWalFile() {
        if (!active_) {
            return;
        }

        std::error_code error;
        std::filesystem::remove(path_, error);
    }
```

它和第二十二版 Snapshot 的 `TempSnapshotFile` 思路一样。

作用是：

```text
compact 先写临时 WAL 文件
如果中途失败，析构时删除临时文件
只有替换目标 WAL 成功后 release
```

这避免留下很多：

```text
*.compact.tmp.*
```

临时垃圾文件。

---

# 5. active_ 标记

类里有：

```cpp
bool active_ = true;
```

默认表示：

```text
这个临时文件还需要析构清理
```

当目标 WAL 已经被替换成功：

```cpp
temp_file.release();
```

`release()` 会把：

```text
active_ = false
```

这样析构时不会误删已经被移动走的文件。

---

# 6. make_temp_wal_path

生成临时文件路径：

```cpp
std::filesystem::path make_temp_wal_path(const std::filesystem::path& path) {
    const auto parent = path.parent_path();
    const std::string filename = path.filename().empty() ? "wal" : path.filename().string();
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
```

它取目标 WAL 的：

```text
parent
filename
当前 steady_clock 时间戳
```

然后拼成：

```text
<filename>.compact.tmp.<stamp>.<attempt>
```

例如：

```text
mini-kv.wal.compact.tmp.123456789.0
```

---

# 7. 为什么有 attempt

代码会尝试 100 次：

```cpp
for (int attempt = 0; attempt < 100; ++attempt) {
    auto candidate = parent / (filename + ".compact.tmp." + std::to_string(stamp) + "." + std::to_string(attempt));
    std::error_code error;
    if (!std::filesystem::exists(candidate, error)) {
        return candidate;
    }
}
```

如果同一时刻刚好已经有同名临时文件，就换下一个后缀：

```text
.0
.1
.2
```

这比固定使用一个：

```text
mini-kv.wal.tmp
```

更稳。

---

# 8. replace_file_atomically

替换目标文件：

```cpp
bool replace_file_atomically(const std::filesystem::path& source, const std::filesystem::path& target) {
#ifdef _WIN32
    return MoveFileExW(source.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    std::error_code error;
    std::filesystem::rename(source, target, error);
    return !error;
#endif
}
```

Windows 使用：

```text
MoveFileExW
MOVEFILE_REPLACE_EXISTING
MOVEFILE_WRITE_THROUGH
```

非 Windows 使用：

```text
std::filesystem::rename
```

这和第二十二版 Snapshot 原子保存保持一致。

核心目标是：

```text
不要直接打开原 WAL 截断重写
先写完整临时文件
再替换目标文件
```

---

# 9. format_expires_at

compact 需要保留 TTL。

所以新增：

```cpp
std::string format_expires_at(std::string_view key, Store::TimePoint expires_at) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"EXPIREAT "} + std::string{key} + " " + std::to_string(epoch_millis);
}
```

它把过期时间转成：

```text
EXPIREAT key epoch_millis
```

这和第四版 WAL 里 `EXPIRE` 的持久化方式一致。

用户输入的是：

```text
EXPIRE keep 60
```

WAL 里保存的是：

```text
EXPIREAT keep 1778401549267
```

这样重启时不用重新计算相对秒数。

---

# 10. compact 的第一步：锁 WAL

实现开始：

```cpp
bool WriteAheadLog::compact(const Store& store, std::size_t* compacted) {
    std::lock_guard lock(mutex_);
```

这个 `mutex_` 原来已经用于：

```text
append
replay_with_report
```

第二十四版继续用它保护：

```text
compact 写临时文件和替换 WAL 的过程
```

它保证同一个 `WriteAheadLog` 对象内部不会同时：

```text
append
compact
replay
```

---

# 11. compact 的第二步：读取当前 live state

接着：

```cpp
const auto items = store.snapshot_items();
```

`snapshot_items()` 做了几件事：

```text
锁 Store
清理已过期 key
导出 key / value / expires_at
按 key 排序
```

所以 compact 得到的是：

```text
当前仍然有效的 key
稳定顺序
可选过期时间
```

这就是新 WAL 的数据来源。

---

# 12. 为什么不用 Store::snapshot()

`Store::snapshot()` 只返回：

```text
key
value
```

但 WAL compact 必须保留 TTL。

所以它要用：

```cpp
Store::SnapshotItem
```

其中包含：

```cpp
std::optional<TimePoint> expires_at;
```

否则 compact 后带 TTL 的 key 会变成永久 key。

---

# 13. 创建 WAL 目录

如果 WAL 路径有父目录：

```cpp
if (path_.has_parent_path()) {
    std::error_code error;
    std::filesystem::create_directories(path_.parent_path(), error);
    if (error) {
        return false;
    }
}
```

这和 `append()` 的行为一致。

例如：

```text
data/mini-kv.wal
```

如果 `data/` 不存在，会尝试创建。

---

# 14. 打开临时 WAL 文件

然后：

```cpp
TempWalFile temp_file{make_temp_wal_path(path_)};
std::ofstream output{temp_file.path(), std::ios::trunc};
if (!output) {
    return false;
}
```

这里故意打开的是：

```text
temp_file.path()
```

而不是：

```text
path_
```

也就是：

```text
写临时文件
不直接碰原 WAL
```

如果打开失败，返回 `false`，`TempWalFile` 会清理临时文件。

---

# 15. 每个 live key 写 SET

核心循环：

```cpp
for (const auto& item : items) {
    output << encode_wal_record(std::string{"SET "} + item.key + " " + item.value) << '\n';
```

每个有效 key 都先写一条：

```text
SET key value
```

再通过：

```cpp
encode_wal_record(...)
```

包装成 WAL2：

```text
WAL2 <checksum> SET key value
```

所以 compact 后的新 WAL 仍然继承第二十三版 checksum 能力。

---

# 16. 有 TTL 的 key 再写 EXPIREAT

如果有过期时间：

```cpp
if (item.expires_at.has_value()) {
    output << encode_wal_record(format_expires_at(item.key, *item.expires_at)) << '\n';
}
```

输出顺序是：

```text
SET key value
EXPIREAT key epoch_millis
```

原因是 replay 时必须先有 key，才能给它设置过期时间。

如果反过来：

```text
EXPIREAT key ...
SET key value
```

`EXPIREAT` 会失败，后面的 `SET` 还会清空 TTL。

---

# 17. 为什么 compact 不写 DEL

compact 是状态重写，不是历史重放。

所以它只写当前 live key。

已经删除的 key：

```text
SET gone value
DEL gone
```

在当前 Store 里不存在。

所以 compact 后 WAL 里不会再出现：

```text
gone
DEL gone
```

这正是压缩的目的。

---

# 18. 为什么 compact 不写过期 key

`snapshot_items()` 会先：

```text
prune_expired_locked(Clock::now())
```

所以已经过期的 key 不会出现在 `items` 里。

compact 后也不会写它。

这避免了：

```text
已经过期的数据重启后又回来
```

---

# 19. flush 和 close 检查

写完后：

```cpp
output.flush();
if (!output) {
    return false;
}

output.close();
if (!output) {
    return false;
}
```

这两步都检查。

意义是：

```text
写入缓冲失败
关闭文件失败
都不能替换原 WAL
```

只有临时文件完整写好后，才进入替换。

---

# 20. 替换目标 WAL

替换：

```cpp
if (!replace_file_atomically(temp_file.path(), path_)) {
    return false;
}
temp_file.release();
```

成功后：

```text
目标 WAL 已经变成压缩后的 WAL
临时文件不需要再清理
```

失败时：

```text
原 WAL 保留
临时文件由 TempWalFile 析构清理
```

---

# 21. compacted 返回 key 数量

最后：

```cpp
if (compacted != nullptr) {
    *compacted = items.size();
}
return true;
```

这里返回的是：

```text
live key 数量
```

不是 WAL 行数。

比如一个 key 有 TTL：

```text
SET keep value
EXPIREAT keep ...
```

这会写 2 行 WAL。

但 compacted key 数量仍然是：

```text
1
```

所以如果有：

```text
keep 带 TTL
phrase 不带 TTL
```

命令返回：

```text
OK compacted 2
```

但 WAL 文件有 3 行。

---

# 22. CommandProcessor 新增错误函数

`src/command.cpp` 新增：

```cpp
CommandResult wal_disabled_error() {
    return {"ERR WAL not enabled"};
}

CommandResult wal_compact_error() {
    return {"ERR wal compact failed"};
}
```

它们分别表示：

```text
当前没有 WAL path
 -> 无法 compact

WAL compact 执行失败
 -> 文件写入或替换失败
```

这样错误信息比复用：

```text
ERR wal append failed
```

更准确。

---

# 23. wal_command_mutex 的角色

第二十四版新增：

```cpp
std::mutex& wal_command_mutex() {
    static std::mutex mutex;
    return mutex;
}
```

这是命令层的 WAL 操作互斥锁。

它保护的是：

```text
带 WAL 的 SET / DEL / EXPIRE
COMPACT
```

为什么要加？

因为 server 模式下每个连接都有自己的 `CommandProcessor`。

如果一个连接在 compact，另一个连接同时 SET，就可能出现：

```text
compact 根据旧 Store 生成新 WAL
另一个 SET append 到旧 WAL 或新 WAL 的边界附近
```

命令层互斥后，同一进程内的 WAL 写命令和 compact 会排队执行。

---

# 24. 为什么锁放在命令层

`WriteAheadLog` 自己已经有 `mutex_`。

但 `SET` 的完整顺序是：

```text
wal append
store set
```

这跨了两个对象。

单靠 WAL 自己的锁只能保护：

```text
append 文件写入
compact 文件替换
```

不能保护：

```text
append 后还没 set
compact 读取 Store
```

所以第二十四版在命令层把相关操作包起来。

例如 SET 的 WAL 模式变成：

```cpp
std::lock_guard lock(wal_command_mutex());
if (!wal_->append(std::string{"SET "} + key + " " + value)) {
    return wal_error();
}

const bool inserted = store_.set(key, value);
return {inserted ? "OK inserted" : "OK updated"};
```

这让：

```text
append + store mutation
```

作为一个命令层临界区完成。

---

# 25. SET 的有 WAL 分支

第二十四版把 SET 分成两条路径。

有 WAL：

```cpp
if (wal_ != nullptr) {
    std::lock_guard lock(wal_command_mutex());
    if (!wal_->append(std::string{"SET "} + key + " " + value)) {
        return wal_error();
    }

    const bool inserted = store_.set(key, value);
    return {inserted ? "OK inserted" : "OK updated"};
}
```

无 WAL：

```cpp
const bool inserted = store_.set(key, value);
return {inserted ? "OK inserted" : "OK updated"};
```

也就是：

```text
只有 WAL 模式才进入 wal_command_mutex
纯内存模式不受影响
```

---

# 26. DEL 的有 WAL 分支

DEL 也分开：

```cpp
if (wal_ != nullptr) {
    std::lock_guard lock(wal_command_mutex());
    if (!store_.contains(key)) {
        return {"0"};
    }

    if (!wal_->append(std::string{"DEL "} + key)) {
        return wal_error();
    }

    return {store_.erase(key) ? "1" : "0"};
}
```

顺序仍然是：

```text
检查 key 是否存在
append DEL
erase Store
```

并且这三步在同一个命令层锁里。

---

# 27. EXPIRE 的有 WAL 分支

EXPIRE 也是：

```cpp
if (wal_ != nullptr) {
    std::lock_guard lock(wal_command_mutex());
    if (!store_.contains(key)) {
        return {"0"};
    }

    const auto expires_at = Store::Clock::now() + *seconds;
    if (!wal_->append(format_expires_at(key, expires_at))) {
        return wal_error();
    }

    return {store_.expire_at(key, expires_at) ? "1" : "0"};
}
```

这里仍然先把相对秒数转换成绝对时间：

```text
EXPIREAT key epoch_millis
```

再写入 WAL。

---

# 28. COMPACT 命令入口

新增命令：

```cpp
if (command == "COMPACT") {
    if (has_extra_token(input)) {
        return usage("COMPACT");
    }

    if (wal_ == nullptr) {
        return wal_disabled_error();
    }
```

规则是：

```text
COMPACT 不能带参数
必须启用 WAL
```

所以：

```text
COMPACT now
 -> ERR usage: COMPACT

没有 wal_path 时 COMPACT
 -> ERR WAL not enabled
```

---

# 29. COMPACT 调用 WAL compact

继续：

```cpp
std::size_t compacted = 0;
{
    std::lock_guard lock(wal_command_mutex());
    if (!wal_->compact(store_, &compacted)) {
        return wal_compact_error();
    }
}

return {std::string{"OK compacted "} + std::to_string(compacted)};
```

它和 SET / DEL / EXPIRE 使用同一个命令层锁。

这样保证：

```text
COMPACT 期间不会有另一个命令层 WAL 写入交错
```

成功后返回：

```text
OK compacted N
```

这里的 N 是当前 live key 数量。

---

# 30. HELP 文本加入 COMPACT

帮助文本新增：

```cpp
"  COMPACT\n"
```

所以 CLI 启动后会显示：

```text
Commands:
  PING [message]
  SET key value
  GET key
  DEL key
  EXPIRE key seconds
  TTL key
  SIZE
  SAVE path
  LOAD path
  COMPACT
  HELP
  EXIT
```

这让用户能直接发现新命令。

---

# 31. COMPACT 和 SAVE 的区别

两个命令都能把当前状态写到文件。

但职责不同：

```text
SAVE path
 -> 用户指定 snapshot 文件
 -> 文件格式是 snapshot
 -> 需要 LOAD path 手动加载

COMPACT
 -> 使用当前 WAL path
 -> 文件格式仍然是 WAL2
 -> 下次启动自动 replay
```

也就是说：

```text
SAVE 是手动备份/恢复
COMPACT 是维护当前 WAL
```

---

# 32. COMPACT 和 WAL checksum 的关系

compact 写每一行时仍然调用：

```cpp
encode_wal_record(...)
```

所以 compact 后的 WAL 继续是：

```text
WAL2 <checksum> <record>
```

如果 compact 后的 WAL 文件被篡改，第二十三版的 checksum 逻辑仍然生效：

```text
checksum mismatch
 -> skipped_records +1
 -> checksum_failed_records +1
```

第二十四版没有绕开第二十三版能力。

它是在第二十三版 WAL2 基础上继续叠加。

---

# 33. COMPACT 的边界

第二十四版实现的是：

```text
手动 compact
单进程内命令层 WAL 写入互斥
同一个 WriteAheadLog 对象内部 append / compact 互斥
```

它还不是：

```text
多进程 WAL 文件锁
后台自动 compact
按大小阈值自动触发
启动时自动 repair
```

这些可以作为后续版本继续做。

---

# 34. 第二十四版核心增删改

新增：

```text
WriteAheadLog::compact
TempWalFile
make_temp_wal_path
replace_file_atomically
WAL 侧 format_expires_at
wal_disabled_error
wal_compact_error
wal_command_mutex
COMPACT 命令
```

修改：

```text
SET / DEL / EXPIRE
 -> WAL 模式下增加命令层互斥

HELP
 -> 增加 COMPACT
```

保持不变：

```text
WAL2 checksum 编码格式
旧 WAL replay 兼容
SET / DEL / EXPIRE 的用户语义
无 WAL 模式的普通读写流程
```

---

# 第四十九次总结

第二十四版 WAL compact 的核心链路是：

```text
COMPACT
 -> CommandProcessor 检查无参数和 WAL 是否启用
 -> wal_command_mutex 阻止同进程 WAL 写入交错
 -> WriteAheadLog::compact
 -> Store::snapshot_items 导出 live key / value / expires_at
 -> 写临时 WAL 文件
 -> 每个 key 写 WAL2 SET
 -> 有 TTL 再写 WAL2 EXPIREAT
 -> flush / close
 -> 替换目标 WAL
 -> 返回 OK compacted N
```

一句话总结：

```text
第二十四版把 WAL 从“只会持续追加历史”推进到“可以手动重写为当前 live state 的短 WAL2 文件”，既减少长期日志膨胀，又继续保留 checksum、TTL 和启动 replay 能力。
```
