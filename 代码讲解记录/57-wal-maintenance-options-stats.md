# 第五十七次讲解：第二十八版 WAL maintenance options 和 compact stats 核心

第五十六次讲了第二十七版 WAL auto compact 的测试、README、归档和整体增删改。
这次讲第二十八版的核心代码：

```text
include/minikv/wal.hpp
src/wal.cpp
src/command.cpp
src/main.cpp
src/server_main.cpp
```

第二十七版已经做到：

```text
--auto-compact-wal
 -> 用户显式开启后，启动期和 WAL 写命令后可以自动 compact
```

第二十八版继续补两个能力：

```text
WAL compact 阈值可配置
WAL compact 统计可查询
```

也就是从：

```text
系统能自动 compact
```

推进到：

```text
用户能调节什么时候 compact
用户能看到 compact 做过多少事
```

---

# 1. 第二十八版解决什么问题

第二十七版的阈值是写死在 `src/wal.cpp` 里的：

```text
compact_hint_min_records = 8
compact_hint_record_ratio = 4
compact_hint_min_bytes = 64 * 1024
```

这适合默认策略。
但不同使用场景可能需要不同策略：

```text
测试环境
 -> 希望很快触发 compact，方便验证

小数据场景
 -> 希望少量 WAL 历史就自动 compact

大数据场景
 -> 希望 WAL 文件达到一定字节数再 compact
```

第二十八版把这些阈值提到可配置结构里。
同时，compact 做完以后，用户还需要知道：

```text
自动 compact 做过几次
手动 compact 做过几次
repair compact 做过几次
一共写回多少 key
一共移除了多少 WAL 记录
一共节省了多少字节
```

所以本版新增了 compact 统计。

---

# 2. WalMaintenanceOptions 的角色

头文件新增：

```cpp
struct WalMaintenanceOptions {
    std::size_t compact_min_records = 8;
    std::size_t compact_record_ratio = 4;
    std::uintmax_t compact_min_bytes = 64 * 1024;
};
```

它保存 WAL maintenance 的阈值。

三个字段分别是：

```text
compact_min_records
 -> WAL 至少达到多少条物理记录，才考虑按 records/live_keys 比例推荐 compact

compact_record_ratio
 -> WAL records 相对 live_keys 的倍率阈值

compact_min_bytes
 -> WAL 文件达到多少字节时，直接推荐 compact
```

默认值仍然保持第二十六、二十七版的策略：

```text
8 records
4 倍 live_keys
64 KiB
```

所以不传新参数时，旧行为不变。

---

# 3. compact_min_records 的含义

如果 WAL 只有很少几条记录，即使 records 比 live_keys 多，也没必要太积极 compact。

比如：

```text
records=2
live_keys=1
```

虽然 records 是 live_keys 的 2 倍，但 WAL 文件很小。
为避免过早维护，先用：

```text
compact_min_records
```

设置一个下限。

默认：

```text
compact_min_records=8
```

表示至少 8 条 WAL 记录才考虑这条规则。

---

# 4. compact_record_ratio 的含义

这个字段控制：

```text
records > live_keys * compact_record_ratio
```

默认：

```text
compact_record_ratio=4
```

如果当前状态是：

```text
records=10
live_keys=1
```

那么：

```text
expected_records = 1 * 4 = 4
10 > 4
```

并且 `records >= 8`，所以推荐 compact。

如果用户配置：

```text
compact_min_records=4
compact_record_ratio=2
```

连续 4 次写同一个 key 时：

```text
records=4
live_keys=1
expected_records=2
4 > 2
```

就会触发推荐或自动 compact。

---

# 5. compact_min_bytes 的含义

字节阈值是另一条规则：

```text
report.bytes >= options.compact_min_bytes
```

只要 WAL 文件达到这个大小，就推荐 compact。

默认：

```text
compact_min_bytes=65536
```

也就是 64 KiB。

这条规则和 records/live_keys 比例是“或”的关系。

```text
字节数达到阈值
 -> 推荐 compact

或者

记录数达到下限，并且记录数显著多于 live keys
 -> 推荐 compact
```

---

# 6. WalCompactionStats 的角色

头文件新增：

```cpp
struct WalCompactionStats {
    std::size_t manual_compactions = 0;
    std::size_t auto_compactions = 0;
    std::size_t repair_compactions = 0;
    std::size_t compacted_keys = 0;
    std::size_t records_removed = 0;
    std::uintmax_t bytes_saved = 0;
};
```

它不是 WAL 文件内容的一部分。
它是当前进程内的运行期统计。

字段含义：

```text
manual_compactions
 -> 用户执行 COMPACT 的次数

auto_compactions
 -> --auto-compact-wal 自动触发 compact 的次数

repair_compactions
 -> --repair-wal 修复时重写 WAL 的次数

compacted_keys
 -> compact 总共写回过多少 live key

records_removed
 -> compact 总共移除过多少 WAL 物理记录

bytes_saved
 -> compact 总共减少过多少 WAL 字节
```

这些统计用于观察维护效果。

---

# 7. WalMaintenanceReport 扩展

第二十六版的 `WalMaintenanceReport` 只有：

```text
bytes
records
live_keys
compact_recommended
```

第二十八版扩展为：

```cpp
struct WalMaintenanceReport {
    std::uintmax_t bytes = 0;
    std::size_t records = 0;
    std::size_t live_keys = 0;
    bool compact_recommended = false;
    WalMaintenanceOptions options;
    WalCompactionStats compaction_stats;
};
```

也就是说，一次 `maintenance_report()` 现在同时告诉用户三类信息：

```text
当前 WAL 状态
 -> bytes / records / live_keys / compact_recommended

当前判断阈值
 -> compact_min_records / compact_record_ratio / compact_min_bytes

当前 compact 统计
 -> manual_compactions / auto_compactions / repair_compactions / compacted_keys / records_removed / bytes_saved
```

---

# 8. WriteAheadLog 构造函数支持 options

头文件从：

```cpp
explicit WriteAheadLog(std::filesystem::path path);
```

变成：

```cpp
explicit WriteAheadLog(std::filesystem::path path, WalMaintenanceOptions options = {});
```

默认参数是空结构。
所以旧代码仍然可以：

```cpp
WriteAheadLog wal{path};
```

如果 CLI 或 server 解析到了自定义阈值，就可以：

```cpp
WriteAheadLog wal{path, wal_options};
```

---

# 9. WriteAheadLog 保存 options 和 stats

类里新增两个成员：

```cpp
WalMaintenanceOptions options_;
WalCompactionStats compaction_stats_;
```

`options_` 表示：

```text
这个 WAL 实例使用什么 compact 判断阈值
```

`compaction_stats_` 表示：

```text
这个 WAL 实例从启动到现在累计做过哪些 compact
```

统计在进程内保存。
重启后会重新从 0 开始。

---

# 10. normalize_options 防御非法阈值

实现里新增：

```cpp
WalMaintenanceOptions normalize_options(WalMaintenanceOptions options) {
    if (options.compact_min_records == 0) {
        options.compact_min_records = 1;
    }

    if (options.compact_record_ratio == 0) {
        options.compact_record_ratio = 1;
    }

    if (options.compact_min_bytes == 0) {
        options.compact_min_bytes = 1;
    }

    return options;
}
```

虽然 CLI/server 参数解析已经要求正整数，但 C++ API 也可能被测试或未来代码直接调用。
所以底层再做一次兜底。

这样可以避免：

```text
compact_min_records=0
compact_record_ratio=0
compact_min_bytes=0
```

导致语义奇怪。

---

# 11. should_recommend_compaction 改为使用 options

之前判断使用固定常量。
第二十八版改成：

```cpp
bool should_recommend_compaction(const WalMaintenanceReport& report,
                                 const WalMaintenanceOptions& options) {
    if (report.bytes >= options.compact_min_bytes) {
        return true;
    }

    const auto expected_records = report.live_keys * options.compact_record_ratio;
    return report.records >= options.compact_min_records && report.records > expected_records;
}
```

它的判断顺序是：

```text
先看字节数
再看记录数和 live key 比例
```

注意：

```text
compact_min_bytes
```

是独立阈值。

```text
compact_min_records + compact_record_ratio
```

是组合阈值。

---

# 12. make_maintenance_report 增加 options 和 stats

第二十七版的 `make_maintenance_report` 只需要 path 和 store。
第二十八版变成：

```cpp
WalMaintenanceReport make_maintenance_report(const std::filesystem::path& path,
                                             const Store& store,
                                             const WalMaintenanceOptions& options,
                                             const WalCompactionStats& stats)
```

生成报告时会填充：

```cpp
report.options = options;
report.compaction_stats = stats;
report.compact_recommended = should_recommend_compaction(report, options);
```

所以所有调用方拿到的 report 都包含完整维护信息。

---

# 13. compact 收益计算

第二十八版新增两个 helper：

```cpp
std::uintmax_t saved_bytes(const WalMaintenanceReport& before, const WalMaintenanceReport& after)
```

和：

```cpp
std::size_t removed_records(const WalMaintenanceReport& before, const WalMaintenanceReport& after)
```

它们都是安全差值。

如果 compact 后文件没有变小，不会出现负数。

逻辑是：

```text
before.bytes > after.bytes
 -> before.bytes - after.bytes
否则
 -> 0
```

记录数同理。

---

# 14. CompactionKind 区分 compact 来源

实现里新增：

```cpp
enum class CompactionKind {
    manual,
    automatic,
    repair,
};
```

它用来区分 compact 来源：

```text
manual
 -> 用户输入 COMPACT

automatic
 -> --auto-compact-wal 自动触发

repair
 -> --repair-wal 修复时重写 WAL
```

这三个来源都会调用同一个 WAL 重写逻辑。
但统计时要分开计数。

---

# 15. record_compaction 统一更新统计

新增：

```cpp
void record_compaction(WalCompactionStats& stats,
                       CompactionKind kind,
                       const WalMaintenanceReport& before,
                       const WalMaintenanceReport& after,
                       std::size_t compacted_keys)
```

它做几件事：

```text
根据 kind 增加 manual / auto / repair 次数
累加 compacted_keys
累加 records_removed
累加 bytes_saved
```

这样手动 compact、自动 compact、repair compact 都复用同一套统计更新逻辑。

---

# 16. 手动 COMPACT 的统计

`WriteAheadLog::compact` 现在流程是：

```text
生成 before report
调用 compact_wal_file
生成 after report
record_compaction(manual)
返回 compacted_keys
```

核心代码结构是：

```cpp
const auto before = make_maintenance_report(path_, store, options_, compaction_stats_);

std::size_t compacted_keys = 0;
if (!compact_wal_file(path_, store, &compacted_keys)) {
    return false;
}

const auto after = make_maintenance_report(path_, store, options_, compaction_stats_);
record_compaction(compaction_stats_, CompactionKind::manual, before, after, compacted_keys);
```

所以用户输入：

```text
COMPACT
```

后，`WALINFO` 会看到：

```text
manual_compactions=1
```

---

# 17. 自动 compact 的统计

`compact_if_recommended` 现在仍然先判断：

```text
compact_recommended
```

不建议时：

```text
不 compact
不增加 auto_compactions
```

建议时：

```text
compact_wal_file
record_compaction(automatic)
```

并且 `WalAutoCompactReport.after` 会重新生成一次，让 after 里也带上更新后的统计。

所以自动 compact 后，`WALINFO` 会看到：

```text
auto_compactions=1
records_removed=...
bytes_saved=...
```

---

# 18. repair compact 的统计

第二十七版之前，`repair()` 内部复用 `compact()`。
但这样会把 repair 算成 manual compact。

第二十八版把 `repair()` 调整为自己调用底层：

```text
replay_with_report
 -> lock WAL
 -> before report
 -> compact_wal_file
 -> after report
 -> record_compaction(repair)
```

这样 `--repair-wal` 后统计是：

```text
repair_compactions=1
manual_compactions=0
```

来源不会混淆。

---

# 19. WALINFO 输出扩展

`src/command.cpp` 里的 `format_walinfo` 扩展后会输出：

```text
wal_bytes=...
wal_records=...
live_keys=...
compact_recommended=...
compact_min_records=...
compact_record_ratio=...
compact_min_bytes=...
manual_compactions=...
auto_compactions=...
repair_compactions=...
compacted_keys=...
records_removed=...
bytes_saved=...
```

这让用户在运行期通过一个命令看到：

```text
当前 WAL 大小
当前 compact 判断阈值
compact 历史效果
```

---

# 20. CLI 新增三个阈值参数

CLI 新增参数：

```text
--wal-compact-min-records count
--wal-compact-record-ratio ratio
--wal-compact-min-bytes bytes
```

`CliOptions` 新增：

```cpp
bool custom_wal_options = false;
minikv::WalMaintenanceOptions wal_options;
```

解析到这些参数时：

```text
填充 wal_options
custom_wal_options = true
```

然后构造 WAL：

```cpp
wal.emplace(*options->wal_path, options->wal_options);
```

---

# 21. 为什么自定义阈值必须搭配 WAL path

CLI 校验变成：

```cpp
if ((options.repair_wal || options.auto_compact_wal || options.custom_wal_options) &&
    !options.wal_path.has_value()) {
    return std::nullopt;
}
```

原因很直接：

```text
没有 WAL path
 -> 没有 WAL 文件
 -> compact 阈值没有作用对象
```

所以这些选项都必须搭配 WAL path：

```text
--repair-wal
--auto-compact-wal
--wal-compact-min-records
--wal-compact-record-ratio
--wal-compact-min-bytes
```

---

# 22. CLI 的数字解析

CLI 新增：

```cpp
parse_positive_uintmax
parse_positive_size
```

它们要求：

```text
不能为空
不能是负数
必须完全是数字
必须大于 0
不能超过目标类型范围
```

这样参数错误会走 usage。

例如：

```text
--wal-compact-min-records 0
--wal-compact-record-ratio -1
--wal-compact-min-bytes abc
```

都会被拒绝。

---

# 23. CLI stats 输出扩展

启动期 `print_wal_maintenance` 也扩展为输出：

```text
WAL stats: bytes=... records=... live_keys=...
compact_recommended=...
compact_min_records=...
compact_record_ratio=...
compact_min_bytes=...
manual_compactions=...
auto_compactions=...
repair_compactions=...
compacted_keys=...
records_removed=...
bytes_saved=...
```

所以启动时和 `WALINFO` 是同一套信息。

---

# 24. server 新增同样三个阈值参数

server usage 扩展为：

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--auto-compact-wal]
                  [--wal-compact-min-records count] [--wal-compact-record-ratio ratio]
                  [--wal-compact-min-bytes bytes] [--max-request-bytes bytes]
                  [--accept-poll-ms ms]
```

解析逻辑和 CLI 对齐：

```text
--wal-compact-min-records
--wal-compact-record-ratio
--wal-compact-min-bytes
```

都会填充：

```cpp
minikv::WalMaintenanceOptions wal_options;
```

最终构造：

```cpp
wal.emplace(*wal_path, wal_options);
```

---

# 25. server event=wal_stats 输出扩展

server 的 `log_wal_maintenance` 现在也输出：

```text
compact_min_records
compact_record_ratio
compact_min_bytes
manual_compactions
auto_compactions
repair_compactions
compacted_keys
records_removed
bytes_saved
```

这样服务端启动日志里可以直接看到：

```text
当前阈值是多少
当前 compact 统计是多少
```

TCP 客户端执行 `WALINFO` 时，也会看到同样字段。

---

# 26. 第二十八版完整链路

CLI：

```text
parse --wal-compact-min-records / --wal-compact-record-ratio / --wal-compact-min-bytes
 -> 写入 WalMaintenanceOptions
 -> WriteAheadLog(path, options)
 -> maintenance_report 使用 options
 -> WALINFO / WAL stats 输出 options 和 stats
```

server：

```text
parse WAL compact 阈值参数
 -> 写入 WalMaintenanceOptions
 -> WriteAheadLog(path, options)
 -> event=wal_stats 输出 options 和 stats
 -> TcpServer 写命令触发 auto compact 后更新 stats
 -> WALINFO 查询最新 stats
```

compact 统计：

```text
COMPACT
 -> manual_compactions + 1

--auto-compact-wal
 -> auto_compactions + 1

--repair-wal
 -> repair_compactions + 1

每次真正 compact
 -> compacted_keys 累加
 -> records_removed 累加
 -> bytes_saved 累加
```

---

# 27. 第二十八版核心总结

第二十八版新增的是：

```text
WalMaintenanceOptions
WalCompactionStats
可配置 compact_min_records
可配置 compact_record_ratio
可配置 compact_min_bytes
manual_compactions
auto_compactions
repair_compactions
compacted_keys
records_removed
bytes_saved
CLI/server WAL compact 阈值参数
扩展 WALINFO
扩展 event=wal_stats
```

一句话总结：

```text
第二十八版把 WAL 自动维护从“能自动 compact”，推进到“什么时候 compact 可配置，compact 做过什么可观察”。
```

