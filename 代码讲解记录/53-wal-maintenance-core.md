# 第五十三次讲解：第二十六版 WAL maintenance 核心

第五十二次讲了第二十五版 WAL repair 的测试、README、归档和整体增删改。

这次讲第二十六版的核心代码：

```text
include/minikv/wal.hpp
src/wal.cpp
src/command.cpp
src/main.cpp
src/server_main.cpp
```

第二十六版新增一个运行期用户命令：

```text
WALINFO
```

它还新增启动期 WAL 维护输出：

```text
CLI
 -> WAL stats: bytes=... records=... live_keys=... compact_recommended=...

server
 -> event=wal_stats ...
 -> event=wal_compact_hint ...
```

核心目标是：

```text
让用户知道当前 WAL 文件有多大
物理 WAL 记录有多少条
当前内存里有多少 live key
是否建议执行 COMPACT
```

---

# 1. 为什么 v26 要做 WAL maintenance

第二十四版已经有：

```text
COMPACT
```

可以手动压缩 WAL。

第二十五版已经有：

```text
--repair-wal
```

可以启动时修复损坏 WAL。

但用户还缺少一个信号：

```text
什么时候应该 COMPACT？
```

如果只靠感觉，用户不知道 WAL 现在是：

```text
很短，不需要处理
```

还是：

```text
历史很多，已经值得压缩
```

第二十六版就是补这个观测层。

---

# 2. WalMaintenanceReport 的角色

头文件新增：

```cpp
struct WalMaintenanceReport {
    std::uintmax_t bytes = 0;
    std::size_t records = 0;
    std::size_t live_keys = 0;
    bool compact_recommended = false;
};
```

它描述一份 WAL 的维护状态。

四个字段分别是：

```text
bytes
 -> WAL 文件当前字节数

records
 -> WAL 文件中非空物理记录行数

live_keys
 -> 当前 Store 里的有效 key 数量

compact_recommended
 -> 是否建议执行 COMPACT
```

---

# 3. 为什么 bytes 用 std::uintmax_t

字段：

```cpp
std::uintmax_t bytes = 0;
```

对应：

```cpp
std::filesystem::file_size()
```

`file_size()` 返回的就是：

```text
std::uintmax_t
```

所以这里直接使用同类型，避免窄化转换。

---

# 4. 为什么 records 不是 replay applied_records

`records` 统计的是：

```text
WAL 文件里的非空物理行数
```

它不是：

```text
成功 replay 的记录数
```

例如：

```text
SET hot value0
SET hot value1
...
SET hot value9
```

这 10 条都可能成功 replay。

但当前 live key 只有：

```text
hot
```

所以：

```text
records = 10
live_keys = 1
```

这正是 compact hint 要观察的差距。

---

# 5. compact hint 阈值

`src/wal.cpp` 新增三个常量：

```cpp
constexpr std::size_t compact_hint_min_records = 8;
constexpr std::size_t compact_hint_record_ratio = 4;
constexpr std::uintmax_t compact_hint_min_bytes = 64 * 1024;
```

含义是：

```text
记录数至少 8 条
并且记录数明显大于 live_keys * 4
```

或者：

```text
WAL 文件超过 64 KiB
```

就建议 compact。

---

# 6. 为什么阈值这么小

这个项目当前还是教学型 mini-kv。

测试和 smoke 需要用很小的 WAL 就能触发提示。

所以使用：

```text
8 条记录
4 倍 live key
64 KiB
```

作为轻量启发式。

它不是生产级自动调优。

它的价值是：

```text
让长期追加历史和当前 live state 的差距可见
```

---

# 7. file_size_or_zero

新增：

```cpp
std::uintmax_t file_size_or_zero(const std::filesystem::path& path) {
    std::error_code error;
    const auto size = std::filesystem::file_size(path, error);
    if (error) {
        return 0;
    }
    return size;
}
```

它读取 WAL 文件大小。

如果文件不存在或读取失败，返回：

```text
0
```

这样 WALINFO 不会因为文件暂时不存在就抛异常。

---

# 8. 为什么不用异常版本 file_size

`std::filesystem::file_size(path)` 有可能抛异常。

这里使用带 `std::error_code` 的版本：

```cpp
std::filesystem::file_size(path, error)
```

原因是 WAL maintenance 是观测能力。

它应该尽量稳定返回：

```text
bytes=0
```

而不是因为一个不存在的 WAL 文件让命令失败。

---

# 9. count_wal_records

新增：

```cpp
std::size_t count_wal_records(const std::filesystem::path& path)
```

流程是：

```text
打开 WAL 文件
逐行读取
去掉 Windows \r
跳过空白行
统计非空记录数
```

核心循环：

```cpp
while (std::getline(input, line)) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    if (!is_blank(line)) {
        ++records;
    }
}
```

---

# 10. records 和 truncated tail 的关系

`count_wal_records()` 不做 replay 语义判断。

它只数物理行。

如果文件末尾有半条无换行记录，`std::getline` 仍然会读到它。

所以 records 可能包含：

```text
尾部半条记录
坏行
checksum mismatch 行
```

这是合理的。

因为 maintenance 关心的是：

```text
文件物理历史有多少
```

不是：

```text
有多少能成功应用
```

---

# 11. should_recommend_compaction

新增：

```cpp
bool should_recommend_compaction(const WalMaintenanceReport& report) {
    if (report.bytes >= compact_hint_min_bytes) {
        return true;
    }

    const auto expected_records = report.live_keys * compact_hint_record_ratio;
    return report.records >= compact_hint_min_records && report.records > expected_records;
}
```

两个条件满足任意一个就推荐 compact：

```text
文件足够大
```

或：

```text
记录数足够多，并且明显多于当前 live key
```

---

# 12. live_keys 为 0 时会怎样

如果：

```text
live_keys = 0
records >= 8
```

那么：

```text
expected_records = 0 * 4 = 0
records > expected_records
```

会推荐 compact。

这很合理。

因为 WAL 里还有很多历史，但当前 Store 已经没有 live key。

compact 后 WAL 可以变成：

```text
空文件
```

---

# 13. maintenance_report API

`WriteAheadLog` 新增：

```cpp
WalMaintenanceReport maintenance_report(const Store& store) const;
```

实现：

```cpp
WalMaintenanceReport WriteAheadLog::maintenance_report(const Store& store) const {
    std::lock_guard lock(mutex_);

    WalMaintenanceReport report;
    report.bytes = file_size_or_zero(path_);
    report.records = count_wal_records(path_);
    report.live_keys = store.size();
    report.compact_recommended = should_recommend_compaction(report);
    return report;
}
```

它在 WAL 模块内统一生成维护报告。

---

# 14. 为什么 maintenance_report 锁 WAL

函数开头：

```cpp
std::lock_guard lock(mutex_);
```

这是为了避免同一个 `WriteAheadLog` 对象正在：

```text
append
compact
repair
```

时同时读取文件大小和记录数。

维护报告不修改 WAL，但它读取 WAL 文件状态。

所以仍然放在 WAL mutex 下。

---

# 15. 为什么 live_keys 调 Store::size()

这里：

```cpp
report.live_keys = store.size();
```

`Store::size()` 会：

```text
锁 Store
清理已过期 key
返回当前 live key 数量
```

这保证 `live_keys` 不是简单 map 大小，而是当前仍然有效的数据数量。

---

# 16. CommandProcessor 新增 format_walinfo

`src/command.cpp` 新增：

```cpp
std::string format_walinfo(const WalMaintenanceReport& report) {
    return "wal_bytes=" + std::to_string(report.bytes) + " wal_records=" + std::to_string(report.records) +
           " live_keys=" + std::to_string(report.live_keys) +
           " compact_recommended=" + format_yes_no(report.compact_recommended);
}
```

输出格式是单行 key-value：

```text
wal_bytes=380 wal_records=10 live_keys=1 compact_recommended=yes
```

这和 server 结构化日志风格接近，也方便测试字符串匹配。

---

# 17. 为什么命令层用 yes / no

辅助函数：

```cpp
std::string format_yes_no(bool value) {
    return value ? "yes" : "no";
}
```

命令返回给用户时用：

```text
yes
no
```

server 日志里用：

```text
true
false
```

因为 server 日志更偏结构化机器字段，CLI 命令更偏用户阅读。

---

# 18. WALINFO 命令入口

新增命令：

```cpp
if (command == "WALINFO") {
    if (has_extra_token(input)) {
        return usage("WALINFO");
    }

    if (wal_ == nullptr) {
        return wal_disabled_error();
    }

    std::lock_guard lock(wal_command_mutex());
    return {format_walinfo(wal_->maintenance_report(store_))};
}
```

规则是：

```text
WALINFO 不能带参数
必须启用 WAL
```

---

# 19. 为什么 WALINFO 也用 wal_command_mutex

WALINFO 不写 WAL。

但它读取：

```text
WAL 文件大小
WAL 记录数
Store live key 数量
```

如果另一个线程正在执行：

```text
SET / DEL / EXPIRE / COMPACT
```

可能得到中间状态。

所以它也进入：

```cpp
wal_command_mutex()
```

确保命令层 WAL 操作和 WALINFO 不交错。

---

# 20. HELP 文本加入 WALINFO

帮助文本新增：

```cpp
"  WALINFO\n"
```

所以 CLI 启动后显示：

```text
COMPACT
WALINFO
HELP
EXIT
```

这让用户能看到维护查询命令。

---

# 21. CLI 启动新增 WAL stats

`src/main.cpp` 新增：

```cpp
void print_wal_maintenance(const minikv::WalMaintenanceReport& report)
```

输出：

```text
WAL stats: bytes=380 records=10 live_keys=1 compact_recommended=yes
```

如果建议 compact，还会输出：

```text
WAL hint: run COMPACT to rewrite long-running log history.
```

---

# 22. CLI 在 replay / repair 之后输出 stats

主函数里：

```cpp
if (options->wal_path.has_value()) {
    wal.emplace(*options->wal_path);
    if (options->repair_wal) {
        ...
    } else {
        ...
    }
    print_wal_maintenance(wal->maintenance_report(store));
}
```

注意 stats 在：

```text
普通 replay 后
repair 后
```

都会输出。

这样用户启动时能立刻看到 WAL 状态。

---

# 23. server 启动新增 event=wal_stats

`src/server_main.cpp` 新增：

```cpp
void log_wal_maintenance(const minikv::WriteAheadLog& wal, const minikv::WalMaintenanceReport& report)
```

它先输出：

```text
event=wal_stats path="..." wal_bytes=380 wal_records=10 live_keys=1 compact_recommended=true
```

这是服务端启动的结构化维护日志。

---

# 24. server compact hint 日志

如果：

```cpp
report.compact_recommended
```

为 true，server 额外输出：

```text
event=wal_compact_hint path="..." wal_bytes=380 wal_records=10 live_keys=1
```

这个事件更醒目。

日志采集或人工查看时，可以直接搜：

```text
event=wal_compact_hint
```

来发现需要维护的 WAL。

---

# 25. server 在 replay / repair 之后输出 stats

server main 里：

```cpp
if (wal_path.has_value()) {
    wal.emplace(*wal_path);
    if (repair_wal) {
        ...
    } else {
        ...
    }
    log_wal_maintenance(*wal, wal->maintenance_report(store));
}
```

这和 CLI 一致。

区别是：

```text
CLI 用 WAL stats / WAL hint
server 用 event=wal_stats / event=wal_compact_hint
```

---

# 26. COMPACT 后 WALINFO 的变化

典型场景：

```text
SET hot value0
SET hot value1
...
SET hot value9
```

此时：

```text
wal_records=10
live_keys=1
compact_recommended=yes
```

执行：

```text
COMPACT
```

后，WAL 只剩最后状态：

```text
SET hot value9
```

所以：

```text
wal_records=1
live_keys=1
compact_recommended=no
```

---

# 27. 第二十六版核心增删改

新增：

```text
WalMaintenanceReport
compact hint 阈值
file_size_or_zero
count_wal_records
should_recommend_compaction
WriteAheadLog::maintenance_report
CommandProcessor::WALINFO
CLI WAL stats / WAL hint
server event=wal_stats / event=wal_compact_hint
```

修改：

```text
HELP
 -> 增加 WALINFO

README / 测试
 -> 增加 WAL maintenance 说明和覆盖
```

保持不变：

```text
COMPACT 行为
repair 行为
WAL2 checksum 行为
普通 SET / DEL / EXPIRE 写 WAL 行为
```

---

# 第五十三次总结

第二十六版 WAL maintenance 的核心链路是：

```text
WriteAheadLog::maintenance_report
 -> 读取 WAL 文件 bytes
 -> 统计非空 WAL records
 -> Store::size 得到 live_keys
 -> 根据 bytes 或 records/live_keys 比例生成 compact_recommended

WALINFO
 -> 返回 wal_bytes / wal_records / live_keys / compact_recommended

CLI/server 启动
 -> replay 或 repair 后输出 WAL stats
 -> compact_recommended=true 时输出 compact hint
```

一句话总结：

```text
第二十六版把 WAL 从“可以手动 compact”推进到“能告诉用户现在是否该 compact”，让 WAL 维护从靠经验变成有明确统计和提示。
```
