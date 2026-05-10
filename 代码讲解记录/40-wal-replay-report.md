# 第四十次讲解：第十九版 WAL replay report 和恢复加固

第三十九次讲了第十八版并发 RESP-over-TCP 测试、server_main 日志加锁、README、CMake、a/18 归档和整体增删改。

这次进入第十九版持久化恢复加固核心：

```text
include/minikv/wal.hpp
src/wal.cpp
src/main.cpp
src/server_main.cpp
tests/wal_tests.cpp
tests/snapshot_tests.cpp
```

第十九版主要做两件事：

```text
第一，让 WAL replay 不只返回成功应用数量，还能报告 skipped / truncated
第二，补测试证明坏 WAL 和坏 snapshot 不会污染恢复后的 Store
```

前面第四版已经有 WAL。

第十九版不是重写 WAL，而是让 WAL 恢复更可观测：

```text
正常记录
 -> 应用到 Store
 -> applied_records +1

坏记录
 -> 跳过
 -> skipped_records +1

末尾无换行记录
 -> 视为疑似部分写入
 -> skipped_records +1
 -> truncated_records +1
```

---

# 1. 为什么要加 WAL replay report

第十八版之前，WAL replay 只有一个返回值：

```cpp
std::size_t replay(Store& store) const;
```

它表示：

```text
成功应用了多少条记录
```

但是它看不出：

```text
文件里有多少坏记录
有没有尾部半条记录
恢复时到底跳过了什么
```

例如 WAL 文件里有：

```text
SET kept value
BROKEN record
SET partial value
```

如果最后一行没有换行，那么它很可能是程序崩溃时写了一半的记录。

第十九版希望启动时能看到：

```text
records=1 skipped=2 truncated=1
```

这样运维和调试时就知道：

```text
恢复了 1 条完整记录
跳过了 2 条记录
其中 1 条是尾部疑似截断记录
```

---

# 2. WalReplayReport

`include/minikv/wal.hpp` 新增：

```cpp
struct WalReplayReport {
    std::size_t applied_records = 0;
    std::size_t skipped_records = 0;
    std::size_t truncated_records = 0;
};
```

三个字段分别表示：

```text
applied_records
 -> 成功应用到 Store 的 WAL 记录数

skipped_records
 -> 被跳过的 WAL 记录数

truncated_records
 -> 被识别为尾部半条记录的数量
```

`truncated_records` 也会计入 `skipped_records`。

也就是说：

```text
skipped_records = 坏记录 + 截断记录
```

---

# 3. 为什么保留 replay()

头文件里仍然保留：

```cpp
std::size_t replay(Store& store) const;
```

并新增：

```cpp
WalReplayReport replay_with_report(Store& store) const;
```

这样做是为了兼容旧调用方。

以前的代码只关心：

```text
恢复了多少条
```

仍然可以继续调用：

```cpp
wal.replay(store)
```

新入口需要更完整信息时，才调用：

```cpp
wal.replay_with_report(store)
```

这是一种温和演进接口的方式。

---

# 4. wal.hpp 新增 cstddef

第十九版还显式加入：

```cpp
#include <cstddef>
```

原因是 `WalReplayReport` 里直接使用：

```cpp
std::size_t
```

虽然以前可能通过其他头文件间接拿到，但头文件最好自己包含自己需要的标准库头。

这属于头文件卫生。

---

# 5. replay_record 仍是单条记录解释器

`src/wal.cpp` 里原来的核心函数还在：

```cpp
bool replay_record(Store& store, std::string_view record)
```

它负责解释一条 WAL 记录。

支持三类记录：

```text
SET key value
DEL key
EXPIREAT key epoch_millis
```

返回值含义是：

```text
true
 -> 这条记录格式正确，并且已经应用

false
 -> 这条记录格式不合法或命令未知
```

第十九版的 report 就是围绕这个返回值统计出来的。

---

# 6. SET replay

`SET` 分支：

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

它允许 value 中包含空格。

例如：

```text
SET phrase hello from wal
```

会恢复为：

```text
key = phrase
value = hello from wal
```

如果 key 或 value 为空，就认为是坏记录。

---

# 7. DEL replay

`DEL` 分支：

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

它要求：

```text
DEL 后面只能有一个 key
```

例如：

```text
DEL name
```

合法。

但是：

```text
DEL name extra
```

会被认为是坏记录。

---

# 8. EXPIREAT replay

`EXPIREAT` 分支：

```cpp
if (command == "EXPIREAT") {
    std::string key;
    long long epoch_millis = 0;
    input >> key >> epoch_millis;

    if (key.empty() || !input || has_extra_token(input)) {
        return false;
    }
    ...
}
```

它要求：

```text
key 非空
epoch_millis 能解析成整数
没有多余 token
```

如果过期时间已经过去：

```cpp
store.erase(key);
```

否则：

```cpp
store.expire_at(key, expires_at);
```

这保持了第四版以来的语义：

```text
过期数据不会在重启后回来
```

---

# 9. is_blank

第十九版新增：

```cpp
bool is_blank(std::string_view text)
```

它遍历一行：

```cpp
for (const char ch : text) {
    if (std::isspace(static_cast<unsigned char>(ch)) == 0) {
        return false;
    }
}
return true;
```

含义是：

```text
整行都是空白字符
```

空白行在 replay 时会直接跳过。

注意：

```text
空白行不计入 skipped_records
```

因为它不是坏记录，只是无内容。

---

# 10. has_final_newline

第十九版新增：

```cpp
bool has_final_newline(const std::filesystem::path& path)
```

它用二进制模式打开文件：

```cpp
std::ifstream input{path, std::ios::binary};
```

然后跳到文件末尾：

```cpp
input.seekg(0, std::ios::end);
```

如果文件为空：

```cpp
return true;
```

非空文件则读取最后一个字节：

```cpp
input.seekg(-1, std::ios::end);
char last = '\0';
input.get(last);
return last == '\n';
```

如果最后一个字节不是换行，就认为：

```text
最后一条 WAL 记录可能没有写完整
```

---

# 11. 为什么尾部无换行视为截断

`WriteAheadLog::append()` 写记录时是：

```cpp
output << record << '\n';
output.flush();
```

也就是说，正常追加完成后，每条记录末尾都应该有换行。

如果 WAL 文件最后没有换行，常见原因是：

```text
进程崩溃
磁盘写了一半
手工编辑留下半条
```

第十九版保守处理：

```text
最后一行即使看起来像合法命令，也先跳过
```

这样避免把半条写入当成完整状态恢复。

---

# 12. replay 现在委托给 replay_with_report

旧接口实现变成：

```cpp
std::size_t WriteAheadLog::replay(Store& store) const {
    return replay_with_report(store).applied_records;
}
```

这表示：

```text
旧接口的语义不变
仍然只返回成功应用数量
```

但内部逻辑统一走新实现。

这样可以避免两套 replay 逻辑分叉。

---

# 13. replay_with_report：打开文件

新接口开头：

```cpp
WalReplayReport WriteAheadLog::replay_with_report(Store& store) const {
    std::lock_guard lock(mutex_);

    std::ifstream input{path_};
    if (!input) {
        return {};
    }
    ...
}
```

它仍然加锁。

原因是：

```text
append 和 replay 不能同时操作同一个 WAL 对象
```

如果文件打不开，就返回空 report：

```text
applied=0
skipped=0
truncated=0
```

这保持了旧版“不存在 WAL 就恢复 0 条”的行为。

---

# 14. 先读入所有行

第十九版先把所有行读进 vector：

```cpp
std::vector<std::string> lines;
std::string line;
while (std::getline(input, line)) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    lines.push_back(std::move(line));
}
```

这里顺手处理 Windows CRLF：

```text
\r\n
```

`std::getline` 会去掉 `\n`，但会留下 `\r`。

所以第十九版把末尾的 `\r` 去掉。

---

# 15. 为什么先读入 vector

为了处理尾部无换行。

逻辑是：

```cpp
if (!has_final_newline(path_) && !lines.empty()) {
    lines.pop_back();
    ++report.skipped_records;
    ++report.truncated_records;
}
```

也就是：

```text
先拿到所有行
再判断最后一行是否疑似截断
如果截断，就从待 replay 列表里删除最后一行
```

这样后面遍历 `lines` 时，就不会误应用半条记录。

---

# 16. 统计 skipped 和 truncated

这段：

```cpp
lines.pop_back();
++report.skipped_records;
++report.truncated_records;
```

表示：

```text
尾部半条记录同时属于 skipped 和 truncated
```

例如：

```text
SET kept value\n
BROKEN record\n
SET partial value
```

最后一条没有换行。

第十九版会：

```text
跳过 SET partial value
skipped_records +1
truncated_records +1
```

---

# 17. replay 每一行

后面遍历：

```cpp
for (const auto& record : lines) {
    if (is_blank(record)) {
        continue;
    }

    if (replay_record(store, record)) {
        ++report.applied_records;
    } else {
        ++report.skipped_records;
    }
}
```

这里有三种情况：

```text
空白行
 -> continue
 -> 不计数

合法记录
 -> replay_record true
 -> applied_records +1

坏记录
 -> replay_record false
 -> skipped_records +1
```

---

# 18. CLI 启动输出

`src/main.cpp` 里原来是：

```cpp
const auto replayed = wal->replay(store);
std::cout << "WAL: " << wal->path().string() << " (" << replayed << " records replayed)\n";
```

第十九版改成：

```cpp
const auto replay = wal->replay_with_report(store);
std::cout << "WAL: " << wal->path().string() << " (" << replay.applied_records
          << " records replayed, " << replay.skipped_records << " skipped, "
          << replay.truncated_records << " truncated)\n";
```

现在 CLI 启动时能看到：

```text
WAL: data.wal (1 records replayed, 2 skipped, 1 truncated)
```

---

# 19. server_main 启动日志

`src/server_main.cpp` 里原来输出：

```text
event=wal_replay path="..." records=N
```

第十九版改成：

```cpp
const auto replay = wal->replay_with_report(store);
std::cout << "event=wal_replay path=" << quote_value(wal->path().string())
          << " records=" << replay.applied_records
          << " skipped=" << replay.skipped_records
          << " truncated=" << replay.truncated_records << '\n';
```

结构化日志变成：

```text
event=wal_replay path="..." records=1 skipped=2 truncated=1
```

这对真实服务端更有用。

---

# 20. wal_tests 正常路径仍保留

`tests/wal_tests.cpp` 先保留原来的正常 WAL 测试：

```text
SET name mini-kv
SET phrase hello from wal
EXPIRE name 60
DEL phrase
SET stale old
EXPIRE stale 1
```

然后等待 `stale` 过期。

恢复后断言：

```cpp
assert(applied == 6);
assert(restored.get("name") == std::optional<std::string>{"mini-kv"});
assert(restored.ttl("name").has_value());
assert(!restored.get("phrase").has_value());
assert(!restored.get("stale").has_value());
```

这保证第十九版没有破坏正常 WAL replay。

---

# 21. wal_tests 新增坏 WAL fixture

第十九版新增一个坏 WAL 文件：

```cpp
std::ofstream output{corrupt_path, std::ios::binary};
output << "SET kept value\n";
output << "GARBAGE record\n";
output << "EXPIREAT kept not-a-number\n";
output << "SET partial value";
```

四个点：

```text
SET kept value
 -> 合法完整记录

GARBAGE record
 -> 未知命令

EXPIREAT kept not-a-number
 -> EXPIREAT 参数不是整数

SET partial value
 -> 末尾无换行，视为疑似部分写入
```

---

# 22. wal_tests 新断言

恢复这个坏 WAL 后：

```cpp
const auto report = wal.replay_with_report(restored);

assert(report.applied_records == 1);
assert(report.skipped_records == 3);
assert(report.truncated_records == 1);
assert(restored.get("kept") == std::optional<std::string>{"value"});
assert(!restored.get("partial").has_value());
assert(wal.replay(restored) == 1);
```

含义是：

```text
只应用 SET kept value
两个坏记录 + 一个截断记录都跳过
partial 不会进入 Store
旧 replay() 仍返回 1
```

这就是第十九版 WAL 加固的核心测试。

---

# 23. snapshot_tests 新增坏文件保护

Snapshot 本身在第十九版没有改生产逻辑。

但补了一个重要测试：

```cpp
minikv::Store protected_store;
assert(protected_store.set("existing", "survives"));
```

先准备一个已有数据的 Store。

然后写坏 snapshot：

```cpp
std::ofstream output{corrupt_path};
output << "MINIKV_SNAPSHOT 1\n";
output << "ENTRY - \"broken\"";
```

这条 `ENTRY` 少了 value 字段。

---

# 24. 坏 snapshot 不替换 Store

测试断言：

```cpp
loaded = 123;
assert(!minikv::SnapshotFile::load(protected_store, corrupt_path, &loaded));
assert(loaded == 123);
assert(protected_store.get("existing") == std::optional<std::string>{"survives"});
assert(protected_store.size() == 1);
```

这证明：

```text
load 返回 false
loaded 输出参数没有被误改
已有 key existing 仍然存在
Store 没有被坏文件清空或替换
```

这很重要。

因为 `SnapshotFile::load` 是先把文件解析到临时 vector，最后才：

```cpp
store.restore_snapshot(std::move(items));
```

坏文件会在 restore 前返回 false。

---

# 25. 第十九版真实 smoke

真实 smoke 构造 WAL：

```text
SET kept value
BROKEN record
SET partial value
```

最后一行没有换行。

启动真实服务端：

```text
minikv_server.exe 6420 127.0.0.1 corrupt-replay.wal --accept-poll-ms 50
```

服务端日志显示：

```text
event=wal_replay ... records=1 skipped=2 truncated=1
```

然后客户端发送：

```text
GET kept
GET partial
QUIT
```

结果：

```text
value
(nil)
BYE
```

说明：

```text
完整记录恢复了
尾部半条记录没有恢复
```

---

# 26. 第十九版仍然没有做什么

这一版是恢复加固的第一步。

还没有做：

```text
WAL 文件校验和
WAL 自动截断修复
WAL compact
WAL rotation
Snapshot checksum
原子写 snapshot 临时文件再 rename
损坏文件详细错误原因返回
```

现在做的是：

```text
跳过坏记录
识别尾部半条记录
把恢复结果报告出来
保证坏 snapshot 不污染 Store
```

这已经能减少很多恢复时的“静默不清楚”。

---

# 27. 第十九版恢复链路总结

整体链路是：

```text
WriteAheadLog::replay_with_report
 -> 加锁
 -> 打开 WAL
 -> 读取所有行
 -> 去掉 CRLF 的 CR
 -> 检查文件末尾是否有 '\n'
 -> 如果最后一行无换行，pop_back 并计入 skipped/truncated
 -> 遍历剩余记录
 -> 空白行跳过不计数
 -> replay_record 成功则 applied +1
 -> replay_record 失败则 skipped +1
 -> 返回 WalReplayReport

main / server_main
 -> 调用 replay_with_report
 -> 输出 applied / skipped / truncated
```

---

# 第四十次总结

第十九版给 WAL replay 加了“恢复报告”，并补上坏记录和尾部半条记录的保护。

一句话总结：

```text
第十九版让 WAL 恢复从“只知道应用了几条”升级为“知道应用、跳过和截断各几条”，同时通过测试确认坏 WAL 不污染 Store、坏 snapshot 不替换 Store。
```
