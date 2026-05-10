# 第五十一次讲解：第二十五版 WAL repair 核心

第五十次讲了第二十四版 WAL compact 的测试、README、归档和整体增删改。

这次讲第二十五版的核心代码：

```text
include/minikv/wal.hpp
src/wal.cpp
src/main.cpp
src/server_main.cpp
```

第二十五版没有新增运行中的用户命令。

它新增的是启动参数：

```text
--repair-wal
```

这个参数用于 CLI 和 server 启动阶段。

核心目标是：

```text
启动时先 replay 可恢复 WAL 记录
跳过 checksum mismatch、坏行、尾部半条记录
然后把 WAL 重写为干净的短 WAL2 文件
最后再进入正常 CLI 或 server 工作状态
```

---

# 1. 为什么 v25 要做 WAL repair

第二十三版已经能识别坏 WAL：

```text
checksum mismatch
坏格式记录
尾部半条记录
```

第二十四版已经能手动 compact：

```text
COMPACT
 -> 用当前 live state 重写短 WAL
```

但还有一个缺口：

```text
如果 WAL 已经损坏，用户每次启动都会看到 skipped / truncated / checksum failed
```

只 replay 不修复，坏记录还留在文件里。

第二十五版的 repair 就是把这两件事串起来：

```text
先 replay_with_report
再 compact
```

这样坏记录被跳过后，WAL 文件也会被重写成干净版本。

---

# 2. repair 和 compact 的关系

`COMPACT` 的前提是：

```text
当前 Store 已经是正确状态
```

它只是把当前状态写回 WAL。

`repair` 多做一步：

```text
先从可能损坏的 WAL 中恢复 Store
再 compact 这个 Store
```

所以 repair 的流程是：

```text
damaged WAL
 -> replay 可恢复记录到 Store
 -> 得到 replay report
 -> compact Store 到新 WAL
 -> 得到 clean WAL2 文件
```

它不是“修补每一行坏记录”。

它是“丢弃坏历史，保留可恢复出来的当前状态”。

---

# 3. WalRepairReport 的角色

头文件新增：

```cpp
struct WalRepairReport {
    WalReplayReport replay;
    std::size_t compacted_keys = 0;
};
```

它包含两部分。

第一部分：

```cpp
WalReplayReport replay;
```

表示 repair 前 replay 的结果：

```text
applied_records
skipped_records
truncated_records
checksum_failed_records
```

第二部分：

```cpp
std::size_t compacted_keys = 0;
```

表示 repair 后 compact 写回了多少个 live key。

---

# 4. 为什么不复用 WalReplayReport

`WalReplayReport` 只描述：

```text
读取 WAL 时发生了什么
```

它不知道 repair 最后写回了多少 key。

repair 需要同时告诉用户：

```text
原 WAL 有多少可恢复记录
原 WAL 有多少坏记录
修复后保留了多少 key
```

所以新增 `WalRepairReport` 更清楚。

---

# 5. 头文件新增 repair API

`WriteAheadLog` 新增：

```cpp
bool repair(Store& store, WalRepairReport* repair = nullptr);
```

参数含义：

```text
store
 -> repair 会把可恢复 WAL 记录 replay 到这个 Store

repair
 -> 可选输出参数，记录 replay 统计和 compacted key 数量
```

返回值：

```text
true
 -> replay 和 compact 都完成

false
 -> compact 写临时文件、flush、close 或替换 WAL 失败
```

---

# 6. repair 的实现非常短

`src/wal.cpp` 里：

```cpp
bool WriteAheadLog::repair(Store& store, WalRepairReport* repair) {
    const auto replay = replay_with_report(store);

    std::size_t compacted_keys = 0;
    if (!compact(store, &compacted_keys)) {
        return false;
    }

    if (repair != nullptr) {
        repair->replay = replay;
        repair->compacted_keys = compacted_keys;
    }
    return true;
}
```

它故意复用已有能力。

没有重新写一套 repair parser。

---

# 7. 第一步 replay_with_report

第一行：

```cpp
const auto replay = replay_with_report(store);
```

这里会执行第二十三版和第十九版积累的恢复逻辑：

```text
旧格式裸记录兼容 replay
WAL2 checksum 验证
checksum mismatch 跳过
坏命令跳过
尾部无换行半条记录跳过
applied / skipped / truncated / checksum_failed 计数
```

repair 不改变 replay 规则。

它只是把 replay 后的结果继续 compact。

---

# 8. 第二步 compact

接着：

```cpp
std::size_t compacted_keys = 0;
if (!compact(store, &compacted_keys)) {
    return false;
}
```

这一步复用第二十四版：

```text
Store::snapshot_items
 -> 导出当前 live key / value / expires_at
 -> 写临时 WAL
 -> 每个 key 写 WAL2 SET
 -> 有 TTL 再写 WAL2 EXPIREAT
 -> 替换目标 WAL
```

所以 repair 后的文件天然是：

```text
干净 WAL2
短 WAL
只包含 live state
```

---

# 9. replay 和 compact 的锁关系

`repair()` 自己没有额外加锁。

原因是它调用的两个函数都已经处理自己的锁：

```text
replay_with_report
 -> 锁 WAL mutex

compact
 -> 锁 WAL mutex
```

启动阶段没有并发命令写 WAL。

所以 v25 的 repair 用在：

```text
CLI 进入交互前
server 开始监听前
```

这时还没有客户端并发写入。

---

# 10. repair 后 Store 仍然可直接使用

repair 的 `store` 参数不是临时内部对象。

CLI / server 会把自己的主 Store 传进去。

所以流程是：

```text
Store store;
WriteAheadLog wal(path);
wal.repair(store, &report);
CommandProcessor processor{store, &wal};
```

repair 完成后，`store` 已经有恢复出的数据。

用户可以立刻：

```text
GET good
GET legacy
```

---

# 11. CLI 新增 CliOptions

`src/main.cpp` 新增：

```cpp
struct CliOptions {
    std::optional<std::string> wal_path;
    bool repair_wal = false;
};
```

原来 CLI 只有一种参数形式：

```text
minikv_cli.exe [wal_path]
```

现在多了：

```text
minikv_cli.exe [wal_path] [--repair-wal]
```

所以用结构体保存解析结果更清楚。

---

# 12. CLI usage 更新

帮助函数从：

```cpp
Usage: minikv_cli.exe [wal_path]
```

变成：

```cpp
Usage: minikv_cli.exe [wal_path] [--repair-wal]
```

这表达两个规则：

```text
wal_path 仍然可选
--repair-wal 是一个启动开关
```

但后面解析还会限制：

```text
--repair-wal 必须配合 wal_path 使用
```

---

# 13. parse_options 解析 --repair-wal

新增：

```cpp
std::optional<CliOptions> parse_options(int argc, char** argv)
```

遇到：

```cpp
if (argument == "--repair-wal") {
    options.repair_wal = true;
    continue;
}
```

就打开 repair 开关。

它不消耗后面的值。

这是典型 boolean flag。

---

# 14. parse_options 处理未知选项

如果参数以 `-` 开头，但不是 `--repair-wal`：

```cpp
if (!argument.empty() && argument.front() == '-') {
    return std::nullopt;
}
```

返回 `std::nullopt`。

主函数看到 `nullopt` 后：

```cpp
print_usage(argv[0]);
return 2;
```

这和项目已有参数错误风格一致。

---

# 15. parse_options 限制 wal_path 只能一个

普通非选项参数会作为 WAL 路径：

```cpp
if (options.wal_path.has_value()) {
    return std::nullopt;
}
options.wal_path = std::string{argument};
```

如果用户写：

```text
minikv_cli.exe one.wal two.wal
```

第二个路径会触发错误。

---

# 16. --repair-wal 必须配合 WAL 路径

最后：

```cpp
if (options.repair_wal && !options.wal_path.has_value()) {
    return std::nullopt;
}
```

也就是说：

```text
minikv_cli.exe --repair-wal
```

是错误的。

因为没有 WAL 文件就没有 repair 对象。

---

# 17. print_wal_replay

原来 CLI 启动 WAL 后直接在 main 里打印 replay 统计。

第二十五版抽成：

```cpp
void print_wal_replay(const minikv::WriteAheadLog& wal, const minikv::WalReplayReport& replay)
```

输出仍然是：

```text
WAL: path (N records replayed, X skipped, Y truncated, Z checksum failed)
```

这保持了不使用 `--repair-wal` 时的输出格式。

---

# 18. print_wal_repair

新增：

```cpp
void print_wal_repair(const minikv::WriteAheadLog& wal, const minikv::WalRepairReport& repair)
```

输出格式是：

```text
WAL: path (2 records replayed, 3 skipped, 1 truncated, 1 checksum failed; repaired to 2 keys)
```

前半段来自：

```text
repair.replay
```

后半段来自：

```text
repair.compacted_keys
```

---

# 19. CLI main 的 repair 分支

主函数里：

```cpp
if (options->wal_path.has_value()) {
    wal.emplace(*options->wal_path);
    if (options->repair_wal) {
        minikv::WalRepairReport repair;
        if (!wal->repair(store, &repair)) {
            std::cerr << "fatal: WAL repair failed\n";
            return 1;
        }
        print_wal_repair(*wal, repair);
    } else {
        print_wal_replay(*wal, wal->replay_with_report(store));
    }
}
```

拆开看：

```text
有 wal_path 且有 --repair-wal
 -> repair
 -> 打印 repaired to N keys

有 wal_path 但没有 --repair-wal
 -> 普通 replay
 -> 打印 replay 统计

没有 wal_path
 -> 纯内存模式
```

---

# 20. repair 失败为什么返回 1

如果：

```cpp
wal->repair(store, &repair)
```

返回 false，CLI 输出：

```text
fatal: WAL repair failed
```

并返回：

```text
1
```

这表示运行失败。

因为 repair 失败时，不能假装 WAL 已经修好。

---

# 21. server usage 更新

`src/server_main.cpp` usage 从：

```text
minikv_server.exe [port] [host] [wal_path] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

变成：

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

server 的 `--repair-wal` 和 CLI 一样：

```text
必须配合 wal_path 使用
```

---

# 22. server 新增 log_wal_replay

原来 server 在 main 里直接输出：

```text
event=wal_replay path="..." records=... skipped=... truncated=... checksum_failed=...
```

第二十五版抽成：

```cpp
void log_wal_replay(const minikv::WriteAheadLog& wal, const minikv::WalReplayReport& replay)
```

这样普通 replay 和 repair 分支更清楚。

---

# 23. server 新增 log_wal_repair

新增：

```cpp
void log_wal_repair(const minikv::WriteAheadLog& wal, const minikv::WalRepairReport& repair)
```

输出：

```text
event=wal_repair path="..." records=2 skipped=3 truncated=1 checksum_failed=1 compacted_keys=2
```

server 用结构化日志。

所以它不像 CLI 那样写自然语言：

```text
repaired to 2 keys
```

而是写字段：

```text
compacted_keys=2
```

---

# 24. server 参数解析新增 repair_wal

server main 里新增：

```cpp
bool repair_wal = false;
```

循环参数时：

```cpp
if (argument == "--repair-wal") {
    repair_wal = true;
    continue;
}
```

它和已有的：

```text
--max-request-bytes
--accept-poll-ms
```

并列。

---

# 25. server 限制 repair 必须有 WAL

解析完后：

```cpp
if (repair_wal && !wal_path.has_value()) {
    print_usage(argv[0]);
    return 2;
}
```

这防止：

```text
minikv_server.exe 6379 127.0.0.1 --repair-wal
```

被误解成没有 WAL 的 repair。

没有 WAL path 时，repair 没有目标文件。

---

# 26. server repair 在监听前发生

server 创建 Store 和 WAL 后：

```cpp
if (wal_path.has_value()) {
    wal.emplace(*wal_path);
    if (repair_wal) {
        minikv::WalRepairReport repair;
        if (!wal->repair(store, &repair)) {
            std::cerr << "fatal: WAL repair failed\n";
            return 1;
        }
        log_wal_repair(*wal, repair);
    } else {
        log_wal_replay(*wal, wal->replay_with_report(store));
    }
}
```

这发生在：

```cpp
TcpServer server{store, options, ...};
install_shutdown_handlers();
server.run();
```

之前。

所以 server 开始监听前，WAL 已经修复，Store 已经恢复。

---

# 27. 为什么 repair 不做成命令

第二十四版已经有运行期命令：

```text
COMPACT
```

第二十五版没有新增：

```text
REPAIR
```

原因是 repair 的目标是：

```text
启动时处理 damaged WAL
```

如果服务已经启动并接受写入，再 repair 旧 damaged WAL，容易和运行中的写入产生复杂交互。

所以 v25 把 repair 放在：

```text
启动阶段
```

这是更简单也更安全的边界。

---

# 28. repair 能保留旧格式裸记录

repair 第一步复用 `replay_with_report()`。

所以旧格式：

```text
SET legacy old
```

仍然会被 replay。

第二步 compact 后，它会被重写成：

```text
WAL2 <checksum> SET legacy old
```

这意味着 repair 还顺便把可恢复的旧格式 WAL 迁移成 WAL2。

---

# 29. repair 会丢弃坏 checksum 记录

如果文件里有：

```text
WAL2 411bcd301442e643 SET bad tampered
```

但 checksum 对应的其实是：

```text
SET bad original
```

replay 阶段会：

```text
skipped_records +1
checksum_failed_records +1
```

这条记录不会进入 Store。

compact 时自然不会写回它。

---

# 30. repair 会丢弃尾部半条记录

如果文件末尾是：

```text
SET partial value
```

并且没有最终换行，`replay_with_report()` 会把它视为疑似部分写入：

```text
skipped_records +1
truncated_records +1
```

它也不会进入 Store。

repair 后的新 WAL 不会包含它。

---

# 31. repair 后的 WAL 是按 key 排序的

compact 使用：

```cpp
Store::snapshot_items()
```

这个函数会按 key 排序。

所以 repair 后 WAL 的顺序可能和原始 WAL 顺序不同。

例如 server smoke 里 repair 后是：

```text
SET legacy srvold
SET srv value
```

因为：

```text
legacy
```

排在：

```text
srv
```

前面。

---

# 32. 第二十五版核心增删改

新增：

```text
WalRepairReport
WriteAheadLog::repair
CLI CliOptions
CLI parse_options
CLI --repair-wal
CLI print_wal_replay / print_wal_repair
server --repair-wal
server log_wal_replay / log_wal_repair
event=wal_repair
```

修改：

```text
CLI usage
server usage
CLI WAL 启动分支
server WAL 启动分支
```

保持不变：

```text
普通 WAL replay 输出
WAL2 checksum 规则
COMPACT 命令
Store / CommandProcessor 运行期命令语义
```

---

# 第五十一次总结

第二十五版 WAL repair 的核心链路是：

```text
minikv_cli.exe data.wal --repair-wal
minikv_server.exe 6379 127.0.0.1 data.wal --repair-wal
 -> 创建 WriteAheadLog
 -> repair(store, &report)
 -> replay_with_report 恢复可恢复记录并统计坏记录
 -> compact 把恢复后的 live state 写成干净 WAL2
 -> CLI 输出 repaired to N keys
 -> server 输出 event=wal_repair compacted_keys=N
 -> 后续命令在已恢复 Store 上运行
```

一句话总结：

```text
第二十五版把 WAL 从“启动时能报告损坏”推进到“启动时能主动清理损坏历史并重写为干净 WAL2”，让 checksum、坏行、尾部半条记录的处理不再只停留在观测层，而能落到文件修复上。
```
