# 第十次讲解：第四版 WAL 核心实现

这次讲解第四版最核心的新增能力：

```text
WAL persistence
```

WAL 是：

```text
Write-Ahead Log
```

也就是：

```text
写前日志
```

它的核心思想是：

```text
每次修改内存数据之前
先把这次修改写进日志文件
```

这样程序退出后，只要日志文件还在，下次启动就可以：

```text
读取 WAL 文件
 -> 按顺序重放每条记录
 -> 把内存 Store 恢复到之前的状态
```

可以把第四版理解成：

```text
前三版：
  数据只在内存里
  程序退出后数据丢失

第四版：
  可选启用 WAL 文件
  SET / DEL / EXPIRE 会写日志
  下次启动时 replay 日志恢复数据
```

---

# 1. 第四版 WAL 的整体链路

第四版新增的持久化链路是：

```text
程序启动
 -> 如果传入 wal_path
 -> 创建 WriteAheadLog
 -> replay WAL 到 Store
 -> 创建 CommandProcessor，并把 WAL 指针传进去

用户执行写命令
 -> CommandProcessor 校验命令
 -> 先 wal.append(...)
 -> WAL 写入成功
 -> 再修改 Store
 -> 返回 OK / 1
```

如果没有传入 WAL 路径：

```text
wal_ = nullptr
```

那么项目仍然保持纯内存模式。

也就是说：

```text
WAL 是可选能力
不影响原来的内存运行方式
```

---

# 2. WAL 记录哪些命令

第四版 WAL 记录的是会改变数据的命令：

```text
SET
DEL
EXPIRE
```

不记录只读命令：

```text
GET
TTL
SIZE
HELP
EXIT
QUIT
```

原因很简单：

```text
只读命令不会改变 Store
重启恢复时不需要重放
```

写命令需要记录，因为它们会改变数据：

```text
SET    改 value
DEL    删除 key
EXPIRE 改 key 的过期时间
```

---

# 3. EXPIRE 为什么写成 EXPIREAT

用户输入的是：

```text
EXPIRE key seconds
```

比如：

```text
EXPIRE session 60
```

意思是：

```text
从现在开始 60 秒后过期
```

但是 WAL 里不能简单保存：

```text
EXPIRE session 60
```

为什么？

因为如果程序 30 秒后重启，重放：

```text
EXPIRE session 60
```

就会让这个 key 又多活 60 秒。

这就错了。

所以第四版把用户的相对时间转换成内部绝对时间：

```text
EXPIREAT key epoch_millis
```

例如：

```text
EXPIREAT session 1778251721471
```

意思是：

```text
session 在这个绝对时间点过期
```

这样重启时就能判断：

```text
如果现在已经超过 expires_at
 -> 不恢复这个 key

如果现在还没超过 expires_at
 -> 恢复 key 并保留剩余 TTL
```

---

# 4. `wal.hpp` 的角色

第四版新增文件：

```text
include/minikv/wal.hpp
```

它定义了：

```cpp
class WriteAheadLog
```

这个类就是 mini-kv 的 WAL 管理器。

它负责三件事：

```text
保存 WAL 文件路径
追加一条日志记录
启动时 replay 日志到 Store
```

可以把它理解成：

```text
WriteAheadLog = Store 的持久化记录本
```

---

# 5. `wal.hpp` 引入的头文件

```cpp
#include "minikv/store.hpp"

#include <filesystem>
#include <mutex>
#include <string_view>
```

先引入：

```cpp
#include "minikv/store.hpp"
```

因为 WAL replay 时需要把记录重新应用到：

```cpp
Store
```

比如 replay 一行：

```text
SET name mini-kv
```

就要调用：

```cpp
store.set("name", "mini-kv")
```

---

`filesystem` 用来保存路径：

```cpp
std::filesystem::path
```

`mutex` 用来保护追加和 replay：

```cpp
std::mutex
```

`string_view` 用来轻量传递一条日志记录：

```cpp
std::string_view record
```

---

# 6. `WriteAheadLog` 类定义

```cpp
class WriteAheadLog {
public:
    explicit WriteAheadLog(std::filesystem::path path);

    bool append(std::string_view record);
    std::size_t replay(Store& store) const;

    const std::filesystem::path& path() const;

private:
    std::filesystem::path path_;
    mutable std::mutex mutex_;
};
```

构造函数：

```cpp
explicit WriteAheadLog(std::filesystem::path path);
```

表示创建 WAL 对象时，需要传入日志文件路径。

比如：

```text
a\4\mini-kv-v4-cli-smoke.wal
```

---

追加记录：

```cpp
bool append(std::string_view record);
```

它负责把一行记录写到 WAL 文件末尾。

例如：

```text
SET name mini-kv
DEL name
EXPIREAT temp 1778251721471
```

返回值：

```text
true   写入成功
false  写入失败
```

---

重放日志：

```cpp
std::size_t replay(Store& store) const;
```

它读取 WAL 文件，每读到一条合法记录，就应用到传入的 `store`。

返回值是：

```text
成功应用的记录数
```

比如启动时打印：

```text
WAL: a\4\mini-kv-v4-cli-smoke.wal (3 records replayed)
```

这个 `3` 就来自 replay 返回值。

---

路径访问：

```cpp
const std::filesystem::path& path() const;
```

它返回 WAL 文件路径。

CLI 和 server 启动时用它打印：

```text
WAL: 路径 (n records replayed)
```

---

私有成员：

```cpp
std::filesystem::path path_;
mutable std::mutex mutex_;
```

`path_` 保存 WAL 文件位置。

`mutex_` 保护 WAL 操作。

因为 TCP server 里多个客户端线程可能同时执行写命令，所以多个线程可能同时追加 WAL。

这里必须加锁，避免多条日志交错写入。

---

# 7. `wal.cpp` 的角色

文件：

```text
src/wal.cpp
```

是 WAL 的具体实现。

它负责：

```text
把记录追加到文件
从文件中逐行读取记录
把每条记录 replay 到 Store
```

---

# 8. `wal.cpp` 的辅助函数 `to_upper`

```cpp
std::string to_upper(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}
```

这个函数和命令层里的 `to_upper` 作用一样：

```text
把命令名转成大写
```

WAL 文件里正常会写：

```text
SET
DEL
EXPIREAT
```

但 replay 时仍然做大写转换，可以让解析更宽容。

比如 WAL 里出现：

```text
set name mini-kv
```

也可以识别成：

```text
SET
```

---

# 9. `has_extra_token`

```cpp
bool has_extra_token(std::istringstream& input) {
    std::string extra;
    return static_cast<bool>(input >> extra);
}
```

这个函数检查一条记录后面还有没有多余字段。

比如：

```text
DEL name extra
```

`DEL` 只应该有一个 key。

多出来的 `extra` 就说明这条 WAL 记录格式不对。

---

# 10. `replay_record`：重放单条 WAL 记录

```cpp
bool replay_record(Store& store, std::string_view record) {
```

这个函数负责重放一行 WAL。

比如传入：

```text
SET name mini-kv
```

它就调用：

```cpp
store.set("name", "mini-kv");
```

如果传入：

```text
DEL name
```

它就调用：

```cpp
store.erase("name");
```

返回值表示：

```text
true   这条记录格式正确并已处理
false  这条记录无法识别或格式错误
```

---

先把一行记录变成字符串流：

```cpp
std::istringstream input{std::string{record}};
```

然后读取命令名：

```cpp
std::string command;
input >> command;
command = to_upper(command);
```

后面根据命令名分别处理。

---

# 11. replay `SET`

```cpp
if (command == "SET") {
    std::string key;
    input >> key;

    std::string value;
    std::getline(input >> std::ws, value);

    if (key.empty() || value.empty()) {
        return false;
    }

    store.set(key, value);
    return true;
}
```

`SET` 的格式是：

```text
SET key value
```

先读 key：

```cpp
input >> key;
```

再读 value：

```cpp
std::getline(input >> std::ws, value);
```

这里用 `getline`，是为了支持 value 里包含空格。

例如：

```text
SET phrase hello from wal
```

如果只用 `>>`，只能读到：

```text
hello
```

用 `getline` 可以读到：

```text
hello from wal
```

---

如果 key 或 value 为空：

```cpp
return false;
```

说明这条 WAL 记录格式不合法。

否则执行：

```cpp
store.set(key, value);
```

并返回：

```cpp
true;
```

---

# 12. replay `DEL`

```cpp
if (command == "DEL") {
    std::string key;
    input >> key;

    if (key.empty() || has_extra_token(input)) {
        return false;
    }

    store.erase(key);
    return true;
}
```

`DEL` 的格式是：

```text
DEL key
```

如果 key 为空，或者后面还有多余参数，说明格式错误。

合法时调用：

```cpp
store.erase(key);
```

即使这个 key 当前不存在，`erase` 也只是返回 false。

WAL replay 这里不关心返回值。

因为 replay 的目标是：

```text
按顺序重建最终状态
```

---

# 13. replay `EXPIREAT`

```cpp
if (command == "EXPIREAT") {
    std::string key;
    long long epoch_millis = 0;
    input >> key >> epoch_millis;

    if (key.empty() || !input || has_extra_token(input)) {
        return false;
    }
```

`EXPIREAT` 是 WAL 内部记录。

格式是：

```text
EXPIREAT key epoch_millis
```

其中 `epoch_millis` 是从 Unix epoch 到过期时间点的毫秒数。

比如：

```text
EXPIREAT stale 1778251704116
```

---

这里检查三件事：

```text
key 不能为空
epoch_millis 必须成功解析
后面不能有多余参数
```

`!input` 很关键。

它能发现：

```text
EXPIREAT key abc
```

这种时间戳不是数字的坏记录。

---

把毫秒数还原成时间点：

```cpp
const Store::TimePoint expires_at{std::chrono::milliseconds{epoch_millis}};
```

`Store::TimePoint` 在第四版里是：

```cpp
std::chrono::system_clock::time_point
```

因为 WAL 需要跨进程保存绝对时间。

---

判断是否已经过期：

```cpp
if (expires_at <= Store::Clock::now()) {
    store.erase(key);
} else {
    store.expire_at(key, expires_at);
}
```

如果现在已经超过这个时间点：

```text
说明 key 已过期
replay 时要删除它
```

如果还没过期：

```text
说明 key 仍然有效
给它恢复绝对过期时间
```

这就是为什么重启后：

```text
已过期 key 不会复活
```

---

# 14. 未知 WAL 记录

最后：

```cpp
return false;
```

如果命令不是：

```text
SET
DEL
EXPIREAT
```

就返回 false。

这样 replay 会跳过无法识别的记录。

---

# 15. `WriteAheadLog` 构造函数

```cpp
WriteAheadLog::WriteAheadLog(std::filesystem::path path) : path_(std::move(path)) {}
```

构造函数只做一件事：

```text
保存 WAL 文件路径
```

这里用了：

```cpp
std::move(path)
```

表示把传入的 path 移动到成员变量 `path_` 里。

---

# 16. `append`：追加一条 WAL 记录

```cpp
bool WriteAheadLog::append(std::string_view record) {
    std::lock_guard lock(mutex_);
```

先加锁。

因为 TCP server 里多个客户端线程可能同时写 WAL。

加锁后可以保证：

```text
一条记录完整写完
另一条记录再开始写
```

---

如果路径有父目录：

```cpp
if (path_.has_parent_path()) {
    std::error_code error;
    std::filesystem::create_directories(path_.parent_path(), error);
    if (error) {
        return false;
    }
}
```

比如路径是：

```text
data\mini-kv.wal
```

父目录是：

```text
data
```

如果 `data` 不存在，就自动创建。

这里使用 `std::error_code`，避免创建目录失败时直接抛异常。

如果失败，返回：

```text
false
```

---

打开文件：

```cpp
std::ofstream output{path_, std::ios::app};
if (!output) {
    return false;
}
```

`std::ios::app` 表示：

```text
追加模式
```

不会覆盖原来的 WAL，而是在末尾继续写。

---

写入记录：

```cpp
output << record << '\n';
output.flush();
return static_cast<bool>(output);
```

每条 WAL 记录一行。

写完后调用：

```cpp
flush()
```

把缓冲区尽快刷到文件层。

最后检查输出流状态。

如果写入过程出错，返回 false。

---

# 17. `replay`：重放 WAL 文件

```cpp
std::size_t WriteAheadLog::replay(Store& store) const {
    std::lock_guard lock(mutex_);
```

replay 也加锁。

这样可以避免同一个 `WriteAheadLog` 对象同时 append 和 replay。

当前启动流程里 replay 发生在运行前，但加锁仍然让类本身更稳。

---

打开文件：

```cpp
std::ifstream input{path_};
if (!input) {
    return 0;
}
```

如果 WAL 文件不存在，说明这是第一次启动。

直接返回：

```text
0
```

也就是没有 replay 任何记录。

---

逐行读取：

```cpp
std::size_t applied = 0;
std::string line;
while (std::getline(input, line)) {
    if (line.empty()) {
        continue;
    }

    if (replay_record(store, line)) {
        ++applied;
    }
}
```

每读一行，就调用：

```cpp
replay_record(store, line)
```

如果这条记录被成功应用，就：

```cpp
++applied;
```

最后返回成功应用的数量。

---

# 18. `path`

```cpp
const std::filesystem::path& WriteAheadLog::path() const {
    return path_;
}
```

这个函数只是返回 WAL 路径。

CLI 和 server 启动时用它打印：

```text
WAL: a\4\mini-kv-v4-cli-smoke.wal (3 records replayed)
```

---

# 19. `CommandProcessor` 接入 WAL

第四版在 `command.hpp` 里新增前置声明：

```cpp
class WriteAheadLog;
```

这样头文件不需要直接包含 `wal.hpp`。

然后构造函数从：

```cpp
explicit CommandProcessor(Store& store);
```

变成：

```cpp
explicit CommandProcessor(Store& store, WriteAheadLog* wal = nullptr);
```

也就是说：

```text
可以传 WAL
也可以不传 WAL
```

默认值是：

```cpp
nullptr
```

所以原来的测试和无 WAL 模式仍然能正常工作。

---

成员变量新增：

```cpp
WriteAheadLog* wal_;
```

它是裸指针，但不负责拥有 WAL。

真正拥有 WAL 的是：

```text
main.cpp 里的 std::optional<WriteAheadLog>
server_main.cpp 里的 std::optional<WriteAheadLog>
```

`CommandProcessor` 只是借用它。

---

# 20. 构造函数保存 WAL 指针

```cpp
CommandProcessor::CommandProcessor(Store& store, WriteAheadLog* wal) : store_(store), wal_(wal) {}
```

这里同时保存：

```text
Store 引用
WAL 指针
```

如果 `wal_ == nullptr`：

```text
纯内存模式
```

如果 `wal_ != nullptr`：

```text
持久化模式
```

---

# 21. WAL 写失败的统一错误

```cpp
CommandResult wal_error() {
    return {"ERR wal append failed"};
}
```

如果 WAL 写入失败，命令返回：

```text
ERR wal append failed
```

并且不会继续修改内存 Store。

这符合写前日志的基本原则：

```text
先写日志
日志成功后再改内存
```

---

# 22. `SET` 的 WAL 接入

第四版 `SET` 在修改 Store 前新增：

```cpp
if (wal_ != nullptr && !wal_->append(std::string{"SET "} + key + " " + value)) {
    return wal_error();
}
```

意思是：

```text
如果启用了 WAL
先追加 SET key value
如果追加失败，直接返回错误
```

只有 WAL 写入成功后，才执行：

```cpp
const bool inserted = store_.set(key, value);
```

所以顺序是：

```text
append WAL
 -> set Store
 -> 返回 OK inserted / OK updated
```

---

# 23. `DEL` 的 WAL 接入

第四版 `DEL` 先检查 key 是否存在：

```cpp
if (!store_.contains(key)) {
    return {"0"};
}
```

如果 key 不存在，就不写 WAL。

因为：

```text
DEL missing
```

没有改变数据。

不需要记录。

---

如果 key 存在，再追加 WAL：

```cpp
if (wal_ != nullptr && !wal_->append(std::string{"DEL "} + key)) {
    return wal_error();
}
```

最后删除：

```cpp
return {store_.erase(key) ? "1" : "0"};
```

顺序是：

```text
确认 key 存在
 -> append DEL key
 -> erase Store
```

---

# 24. `EXPIRE` 的 WAL 接入

`EXPIRE` 比 `SET` 和 `DEL` 多一步：

```cpp
const auto expires_at = Store::Clock::now() + *seconds;
```

这里把用户输入的相对 TTL：

```text
seconds
```

转换成绝对过期时间：

```text
expires_at
```

---

然后写 WAL：

```cpp
if (wal_ != nullptr && !wal_->append(format_expires_at(key, expires_at))) {
    return wal_error();
}
```

`format_expires_at` 会生成：

```text
EXPIREAT key epoch_millis
```

最后设置 Store：

```cpp
return {store_.expire_at(key, expires_at) ? "1" : "0"};
```

所以 `EXPIRE` 的顺序是：

```text
确认 key 存在
 -> 计算绝对过期时间
 -> append EXPIREAT
 -> store.expire_at
```

---

# 25. `format_expires_at`

```cpp
std::string format_expires_at(std::string_view key, Store::TimePoint expires_at) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"EXPIREAT "} + std::string{key} + " " + std::to_string(epoch_millis);
}
```

这个函数把时间点格式化成 WAL 记录。

先把时间点转成毫秒：

```cpp
expires_at.time_since_epoch()
```

再：

```cpp
duration_cast<std::chrono::milliseconds>(...)
```

最后取数字：

```cpp
count()
```

生成的记录类似：

```text
EXPIREAT stale 1778251704116
```

---

# 26. Store 为 WAL 做的小调整

第四版把 `Store::Clock` 改成了：

```cpp
using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point;
```

第三版用的是 `steady_clock`。

`steady_clock` 适合进程内 TTL，因为它单调递增。

但它不能跨进程持久化。

因为重启之后，`steady_clock` 的时间点没有稳定的现实意义。

WAL 需要把过期时间写进文件，并在下次启动时恢复。

所以第四版改成：

```text
system_clock
```

这样 `EXPIREAT` 可以保存成：

```text
Unix epoch 毫秒数
```

跨进程仍然能比较。

---

第四版还新增：

```cpp
bool expire_at(std::string_view key, TimePoint expires_at);
```

它和 `expire` 的区别是：

```text
expire    接收相对秒数
expire_at 接收绝对过期时间点
```

命令层 `EXPIRE` 会先算出绝对时间，然后调用：

```cpp
store_.expire_at(key, expires_at)
```

WAL replay 也会调用：

```cpp
store.expire_at(key, expires_at)
```

这样运行时和恢复时都走同一套绝对过期逻辑。

---

# 第十次总结

第四版 WAL 的核心是新增 `WriteAheadLog`：

```text
append(record)
 -> 把写命令追加到文件

replay(store)
 -> 启动时逐行读取文件
 -> 重放 SET / DEL / EXPIREAT
```

`CommandProcessor` 负责把用户命令转成 WAL 记录：

```text
SET key value        -> SET key value
DEL key              -> DEL key
EXPIRE key seconds   -> EXPIREAT key epoch_millis
```

一句话总结：

```text
第四版让 mini-kv 从“只在内存里修改数据”前进到“先把写操作记进 WAL，再修改内存，并能在下次启动时 replay 恢复”的阶段。
```
