# 第五十五次讲解：第二十七版 WAL auto compact 核心

第五十四次讲了第二十六版 WAL maintenance 的测试、README、归档和整体增删改。
这次讲第二十七版的核心代码：

```text
include/minikv/wal.hpp
src/wal.cpp
include/minikv/command.hpp
src/command.cpp
include/minikv/tcp_server.hpp
src/tcp_server.cpp
src/main.cpp
src/server_main.cpp
```

第二十六版已经能做到：

```text
WALINFO
 -> 告诉用户 wal_bytes / wal_records / live_keys / compact_recommended

CLI / server 启动
 -> 输出 WAL stats
 -> 必要时输出 compact hint
```

第二十七版继续往前走一步：

```text
--auto-compact-wal
```

也就是用户显式打开这个开关后，系统会在合适时机自动执行 WAL compact。

---

# 1. 第二十七版解决什么问题

第二十六版的逻辑是：

```text
系统发现 WAL 历史比较长
 -> 告诉用户 compact_recommended=yes
 -> 用户手动输入 COMPACT
```

这已经比完全不知道 WAL 状态好很多。
但它仍然需要用户自己做动作。

第二十七版新增的是：

```text
系统发现 WAL 历史比较长
 -> 如果用户开启 --auto-compact-wal
 -> 系统自动调用 compact
 -> WAL 文件变短
```

注意这里用了“可选”。
默认行为没有变。
只有显式传入：

```text
--auto-compact-wal
```

才会自动 compact。

这样做比较稳：

```text
默认模式
 -> 保持旧行为，只提示，不自动改写 WAL

自动模式
 -> 用户明确授权后，系统才做维护性重写
```

---

# 2. WalAutoCompactReport 的角色

头文件新增：

```cpp
struct WalAutoCompactReport {
    WalMaintenanceReport before;
    WalMaintenanceReport after;
    std::size_t compacted_keys = 0;
    bool compacted = false;
};
```

它用来描述一次自动 compact 检查的结果。

字段含义是：

```text
before
 -> 自动 compact 检查前的 WAL maintenance 状态

after
 -> 自动 compact 检查后的 WAL maintenance 状态

compacted_keys
 -> 如果真的 compact，写回了多少个 live key

compacted
 -> 这次检查是否真的执行了 compact
```

这个结构不是给普通命令直接输出用的。
它主要给 CLI / server 启动日志和测试使用。

---

# 3. 为什么 before / after 复用 WalMaintenanceReport

第二十六版已经有：

```cpp
struct WalMaintenanceReport {
    std::uintmax_t bytes = 0;
    std::size_t records = 0;
    std::size_t live_keys = 0;
    bool compact_recommended = false;
};
```

第二十七版不重新发明一套统计字段。
而是复用它：

```text
before.bytes
before.records
before.live_keys
before.compact_recommended

after.bytes
after.records
after.live_keys
after.compact_recommended
```

这样自动 compact 的效果很直观：

```text
records=10 -> 1
bytes=380 -> 38
compact_recommended=yes -> no
```

也就是说，`WalAutoCompactReport` 是一个“动作报告”，`WalMaintenanceReport` 是一个“状态快照”。

---

# 4. WriteAheadLog 新增 compact_if_recommended

头文件新增：

```cpp
bool compact_if_recommended(const Store& store, WalAutoCompactReport* report = nullptr);
```

这个函数的语义是：

```text
检查当前 WAL 是否建议 compact
如果不建议，什么都不做，返回 true
如果建议，执行 compact
如果 compact 成功，返回 true
如果 compact 失败，返回 false
```

它和手动 `compact()` 的区别是：

```text
compact()
 -> 无条件 compact

compact_if_recommended()
 -> 先看 maintenance_report
 -> 只有 compact_recommended=true 才 compact
```

---

# 5. make_maintenance_report 提取了重复逻辑

第二十六版里 `maintenance_report()` 直接组装报告。
第二十七版把这段逻辑提取成内部 helper：

```cpp
WalMaintenanceReport make_maintenance_report(const std::filesystem::path& path, const Store& store) {
    WalMaintenanceReport report;
    report.bytes = file_size_or_zero(path);
    report.records = count_wal_records(path);
    report.live_keys = store.size();
    report.compact_recommended = should_recommend_compaction(report);
    return report;
}
```

这样有两个地方可以复用同一套统计逻辑：

```text
WriteAheadLog::maintenance_report
 -> 直接返回当前状态

WriteAheadLog::compact_if_recommended
 -> compact 前后各取一次状态
```

复用这段逻辑的好处是：

```text
WALINFO
启动 WAL stats
compact hint
auto compact before / after
```

全部使用同一套判断标准。

---

# 6. compact_wal_file 提取了真实重写动作

第二十四版的 `WriteAheadLog::compact()` 里包含完整重写逻辑：

```text
Store::snapshot_items
 -> 生成当前 live keys
 -> 写入临时 WAL 文件
 -> flush / close
 -> 原子替换目标 WAL
```

第二十七版把它提取成：

```cpp
bool compact_wal_file(const std::filesystem::path& path, const Store& store, std::size_t* compacted)
```

这样：

```cpp
bool WriteAheadLog::compact(const Store& store, std::size_t* compacted) {
    std::lock_guard lock(mutex_);
    return compact_wal_file(path_, store, compacted);
}
```

手动 compact 仍然走同一条重写路径。

自动 compact 也走同一条重写路径。

这避免出现两套 WAL 重写代码。

---

# 7. compact_if_recommended 的完整流程

核心流程是：

```cpp
bool WriteAheadLog::compact_if_recommended(const Store& store, WalAutoCompactReport* report) {
    std::lock_guard lock(mutex_);

    WalAutoCompactReport local_report;
    local_report.before = make_maintenance_report(path_, store);
    local_report.after = local_report.before;

    if (!local_report.before.compact_recommended) {
        if (report != nullptr) {
            *report = local_report;
        }
        return true;
    }

    if (!compact_wal_file(path_, store, &local_report.compacted_keys)) {
        return false;
    }

    local_report.compacted = true;
    local_report.after = make_maintenance_report(path_, store);
    if (report != nullptr) {
        *report = local_report;
    }
    return true;
}
```

分成三段看。

第一段：

```text
先加 WAL mutex
再生成 before report
after 先等于 before
```

如果后面不需要 compact，那么 before 和 after 就应该一样。

第二段：

```text
before.compact_recommended=false
 -> 不 compact
 -> compacted=false
 -> 返回 true
```

这里返回 true 的意思是：

```text
检查成功，而且不需要维护动作
```

不是说一定做了 compact。
是否做了 compact 要看：

```text
report.compacted
```

第三段：

```text
before.compact_recommended=true
 -> 调用 compact_wal_file
 -> compacted=true
 -> 重新生成 after report
```

这样调用方能看到 compact 前后变化。

---

# 8. 为什么 compact_if_recommended 内部要加锁

`WriteAheadLog` 本身有：

```cpp
mutable std::mutex mutex_;
```

append、compact、replay、maintenance_report 都围绕 WAL 文件操作。
自动 compact 同样会读 WAL 文件、统计记录数、写临时 WAL、替换目标 WAL。

所以它也必须加锁：

```cpp
std::lock_guard lock(mutex_);
```

这样可以避免这些情况交错：

```text
一个线程正在 append
另一个线程正在 compact

一个线程正在 WALINFO
另一个线程正在替换 WAL 文件
```

第二十七版没有引入新的锁模型。
它继续使用 WAL 自己的 mutex。

---

# 9. CommandProcessorOptions 的作用

命令层新增：

```cpp
struct CommandProcessorOptions {
    bool auto_compact_wal = false;
};
```

然后构造函数变成：

```cpp
CommandProcessor(Store& store,
                 WriteAheadLog* wal = nullptr,
                 CommandProcessorOptions options = {});
```

默认值仍然是：

```text
auto_compact_wal=false
```

所以旧代码这样写仍然有效：

```cpp
CommandProcessor processor{store};
CommandProcessor processor{store, &wal};
```

只有 CLI / server 显式传入选项时，才开启自动 compact。

---

# 10. auto_compact_wal_if_needed

`CommandProcessor` 新增私有函数：

```cpp
void CommandProcessor::auto_compact_wal_if_needed() {
    if (wal_ == nullptr || !options_.auto_compact_wal) {
        return;
    }

    (void)wal_->compact_if_recommended(store_);
}
```

它做两层保护：

```text
wal_ == nullptr
 -> 当前是纯内存模式，不可能 compact WAL

options_.auto_compact_wal == false
 -> 用户没有打开自动 compact，不做维护动作
```

只有两个条件都满足：

```text
有 WAL
开启 auto_compact_wal
```

才调用：

```cpp
wal_->compact_if_recommended(store_);
```

这里没有把报告返回给用户。
原因是命令层仍然保持原本响应：

```text
SET -> OK inserted / OK updated
DEL -> 1 / 0
EXPIRE -> 1 / 0
```

自动 compact 在命令层属于后台维护动作。

---

# 11. 哪些命令会触发运行期自动 compact

第二十七版只在成功写入 WAL 且成功修改 Store 后触发。

`SET` 的 WAL 分支：

```cpp
if (!wal_->append(std::string{"SET "} + key + " " + value)) {
    return wal_error();
}

const bool inserted = store_.set(key, value);
auto_compact_wal_if_needed();
return {inserted ? "OK inserted" : "OK updated"};
```

`DEL` 的 WAL 分支：

```cpp
if (!wal_->append(std::string{"DEL "} + key)) {
    return wal_error();
}

const bool erased = store_.erase(key);
auto_compact_wal_if_needed();
return {erased ? "1" : "0"};
```

`EXPIRE` 的 WAL 分支：

```cpp
if (!wal_->append(format_expires_at(key, expires_at))) {
    return wal_error();
}

const bool updated = store_.expire_at(key, expires_at);
auto_compact_wal_if_needed();
return {updated ? "1" : "0"};
```

没有触发自动 compact 的命令包括：

```text
GET
TTL
SIZE
SAVE
LOAD
WALINFO
HELP
PING
```

因为它们不是普通 WAL 写命令。

---

# 12. 为什么是写入后再 compact

顺序是：

```text
append WAL
 -> 修改 Store
 -> auto compact if needed
```

原因是：

```text
compact 要根据当前 Store live state 重写 WAL
```

如果先 compact，再修改 Store，那么 compact 得到的是旧状态。
写入后 compact 才能把最新状态写进短 WAL。

例如连续执行：

```text
SET hot value0
SET hot value1
...
SET hot value7
```

第八次写入后触发 compact。
此时 Store 里 `hot` 的值已经是：

```text
value7
```

所以 compact 后 WAL 只需要一条：

```text
SET hot value7
```

---

# 13. CLI 的 --auto-compact-wal

CLI 选项结构新增：

```cpp
struct CliOptions {
    std::optional<std::string> wal_path;
    bool repair_wal = false;
    bool auto_compact_wal = false;
};
```

usage 变成：

```text
Usage: minikv_cli.exe [wal_path] [--repair-wal] [--auto-compact-wal]
```

解析到：

```cpp
if (argument == "--auto-compact-wal") {
    options.auto_compact_wal = true;
    continue;
}
```

并且要求：

```cpp
if ((options.repair_wal || options.auto_compact_wal) && !options.wal_path.has_value()) {
    return std::nullopt;
}
```

也就是说：

```text
--auto-compact-wal 必须搭配 WAL path
```

没有 WAL 文件路径，自动 compact 没有对象。

---

# 14. CLI 启动期自动 compact

CLI replay 或 repair 之后新增：

```cpp
if (options->auto_compact_wal) {
    minikv::WalAutoCompactReport compact;
    if (!wal->compact_if_recommended(store, &compact)) {
        std::cerr << "fatal: WAL auto compact failed\n";
        return 1;
    }
    print_wal_auto_compact(compact);
}
print_wal_maintenance(wal->maintenance_report(store));
```

启动期这里和命令层不同。
如果自动 compact 失败，CLI 直接 fatal。

原因是：

```text
启动期用户明确要求 auto compact
此时还没有进入交互循环
失败应该明确告诉用户
```

成功或不需要 compact 时，输出：

```text
WAL auto compact: compacted=1 records=10->1 bytes=380->38
```

或：

```text
WAL auto compact: not needed
```

---

# 15. CLI 运行期自动 compact

CLI 构造 `CommandProcessor` 时传入选项：

```cpp
minikv::CommandProcessorOptions command_options;
command_options.auto_compact_wal = options->auto_compact_wal;
minikv::CommandProcessor processor{store, wal.has_value() ? &*wal : nullptr, command_options};
```

这使得交互过程中输入：

```text
SET
DEL
EXPIRE
```

也会走命令层自动 compact 检查。

用户仍然看到原命令响应。
如果想确认 WAL 是否已经变短，可以继续执行：

```text
WALINFO
```

---

# 16. server_main 的 --auto-compact-wal

server 启动参数也新增：

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--auto-compact-wal]
                  [--max-request-bytes bytes] [--accept-poll-ms ms]
```

解析到：

```cpp
if (argument == "--auto-compact-wal") {
    auto_compact_wal = true;
    options.auto_compact_wal = true;
    continue;
}
```

同样要求必须有 WAL path：

```cpp
if ((repair_wal || auto_compact_wal) && !wal_path.has_value()) {
    print_usage(argv[0]);
    return 2;
}
```

---

# 17. server 启动期日志

server 启动期自动 compact 后会输出结构化日志。

真的 compact 时：

```text
event=wal_auto_compact path="..." compacted_keys=1
before_wal_bytes=380 after_wal_bytes=38
before_wal_records=10 after_wal_records=1
live_keys=1
```

不需要 compact 时：

```text
event=wal_auto_compact_skipped path="..." reason=not_recommended
wal_bytes=... wal_records=... live_keys=...
```

server 的 `event=server_start` 也新增：

```text
auto_compact_wal=true
```

这样看日志就知道当前服务是不是启用了自动 WAL 维护。

---

# 18. TcpServer::Options 新增 auto_compact_wal

网络层选项新增：

```cpp
bool auto_compact_wal = false;
```

这个字段不是给启动期用的。
启动期逻辑在 `server_main.cpp`。

它的作用是：

```text
把 server_main 解析到的 auto_compact_wal
传给每个 TCP 连接里的 CommandProcessor
```

---

# 19. serve_client 把选项交给命令层

`serve_client` 新增参数：

```cpp
bool auto_compact_wal
```

内部构造命令处理器时：

```cpp
CommandProcessorOptions command_options;
command_options.auto_compact_wal = auto_compact_wal;
CommandProcessor processor{store, wal, command_options};
```

这样 TCP 客户端执行：

```text
SET
DEL
EXPIRE
```

也会触发同一套运行期自动 compact。

---

# 20. TcpServer::run 传递 auto_compact_wal

接受连接后，创建线程时多传一个参数：

```cpp
std::thread{serve_client,
            std::ref(store_),
            wal_,
            client,
            options_.logger,
            endpoint,
            connection_tracker_,
            snapshot.connection_id,
            options_.max_request_bytes,
            options_.auto_compact_wal}
    .detach();
```

这表示：

```text
server 级别的配置
 -> 每个连接线程
 -> 每个连接自己的 CommandProcessor
```

最终 TCP 请求和 CLI 请求复用同一个命令层自动 compact 逻辑。

---

# 21. v27 自动 compact 的完整链路

CLI 启动期：

```text
main.cpp parse --auto-compact-wal
 -> replay / repair WAL
 -> WriteAheadLog::compact_if_recommended
 -> print_wal_auto_compact
 -> print_wal_maintenance
```

CLI 运行期：

```text
main.cpp parse --auto-compact-wal
 -> CommandProcessorOptions::auto_compact_wal=true
 -> SET / DEL / EXPIRE
 -> append WAL
 -> update Store
 -> WriteAheadLog::compact_if_recommended
```

server 启动期：

```text
server_main.cpp parse --auto-compact-wal
 -> replay / repair WAL
 -> WriteAheadLog::compact_if_recommended
 -> event=wal_auto_compact
 -> event=wal_stats
```

server 运行期：

```text
server_main.cpp 设置 TcpServer::Options::auto_compact_wal=true
 -> TcpServer::run 接受连接
 -> serve_client 构造 CommandProcessorOptions
 -> TCP SET / DEL / EXPIRE
 -> append WAL
 -> update Store
 -> WriteAheadLog::compact_if_recommended
```

---

# 22. 第二十七版没有改变什么

第二十七版没有改这些默认行为：

```text
没有 WAL path
 -> 仍然是纯内存模式

有 WAL path，但没有 --auto-compact-wal
 -> 仍然只输出 WAL stats / hint
 -> 不自动 compact

手动 COMPACT
 -> 仍然可用

WALINFO
 -> 仍然可用

--repair-wal
 -> 仍然可用
```

也就是说，v27 是一个向后兼容增强。

---

# 23. 为什么 runtime auto compact 没有额外返回文本

例如执行：

```text
SET hot value7
```

即使这条命令触发了自动 compact，返回仍然是：

```text
OK updated
```

原因是命令协议需要稳定。
如果把维护信息混进普通写命令响应，会影响客户端解析。

用户如果想看结果，可以执行：

```text
WALINFO
```

server 启动期则适合输出结构化日志，因为启动日志本来就是运维信息。

---

# 24. 第二十七版核心总结

第二十七版新增的是：

```text
WalAutoCompactReport
WriteAheadLog::compact_if_recommended
CommandProcessorOptions::auto_compact_wal
CLI --auto-compact-wal
server --auto-compact-wal
TcpServer::Options::auto_compact_wal
event=wal_auto_compact
event=wal_auto_compact_skipped
```

一句话总结：

```text
第二十七版把 WAL maintenance 从“系统能告诉你该不该 COMPACT”，推进到“用户授权后系统可以自动在启动期和写入后完成 COMPACT”。
```

