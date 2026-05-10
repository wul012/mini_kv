# 第四十五次讲解：第二十二版 Snapshot 原子保存核心

第四十四次讲了第二十一版测试、README、归档和整体增删改。

这次讲第二十二版的核心代码：

```text
src/snapshot.cpp
```

第二十二版没有新增命令。

它改的是 `SAVE path` 背后的保存方式：

```text
旧方式：
SnapshotFile::save
 -> 直接打开目标 path
 -> std::ios::trunc 截断旧文件
 -> 写入新 snapshot

新方式：
SnapshotFile::save
 -> 写入同目录临时文件
 -> flush / close 成功
 -> 替换目标 snapshot
 -> 失败时清理临时文件
```

一句话说：

```text
第二十二版让 snapshot 保存尽量避免“新文件还没写完，旧 snapshot 已经被截断”的风险。
```

---

# 1. 为什么要做原子保存

第五版加入 Snapshot 后，`SAVE path` 已经可以把当前内存数据写成快照文件。

第十九版又补了坏 snapshot load 保护：

```text
坏 snapshot 文件
 -> load 返回 false
 -> 不替换当前 Store
```

但是保存端还有一个风险。

旧代码是：

```cpp
std::ofstream output{path, std::ios::trunc};
```

`std::ios::trunc` 会在打开时截断目标文件。

也就是说：

```text
旧 snapshot 原本是好的
 -> SAVE 开始
 -> 目标文件先被清空
 -> 写入新 snapshot
 -> 如果中途失败
 -> 目标 snapshot 可能变成半个文件
```

这就是第二十二版要修的点。

---

# 2. 新的保存策略

第二十二版改成：

```text
1. 从 Store 导出 snapshot items
2. 确保目标父目录存在
3. 在目标目录里生成临时文件路径
4. 把 snapshot 完整写入临时文件
5. flush
6. close
7. 用临时文件替换目标文件
8. 替换成功后 release 临时文件清理器
```

核心变化是：

```text
写入阶段不碰目标文件
```

只有临时文件完整写完后，才替换目标 snapshot。

---

# 3. 新增 system_error

文件头新增：

```cpp
#include <system_error>
```

它主要是为了使用：

```cpp
std::error_code
```

这里用 `error_code` 而不是让 `std::filesystem` 直接抛异常，是因为 `SnapshotFile::save()` 的接口一直是：

```cpp
static bool save(...)
```

也就是说它用：

```text
true / false
```

表达保存成功或失败。

所以内部文件系统操作也尽量转成：

```text
失败返回 false
```

而不是向外抛异常。

---

# 4. Windows 头文件保护

新增：

```cpp
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
```

这是为了在 Windows 上调用：

```cpp
MoveFileExW
```

`WIN32_LEAN_AND_MEAN` 的作用是减少 `windows.h` 引入的内容。

`NOMINMAX` 的作用是避免 Windows 头文件定义 `min` / `max` 宏，影响 C++ 标准库里的 `std::min` / `std::max`。

这些是 C++ 项目里包含 `windows.h` 时常见的保护写法。

---

# 5. TempSnapshotFile 类的角色

第二十二版新增：

```cpp
class TempSnapshotFile {
public:
    explicit TempSnapshotFile(std::filesystem::path path) : path_(std::move(path)) {}

    ~TempSnapshotFile() {
        if (!active_) {
            return;
        }

        std::error_code error;
        std::filesystem::remove(path_, error);
    }

    ...
};
```

这个类不是 snapshot 文件格式。

它是一个 RAII 清理器。

它的角色是：

```text
只要临时文件还 active
析构时就尝试删除这个临时文件
```

这样保存失败时不用在每一个 `return false` 前手动写：

```cpp
std::filesystem::remove(temp_path);
```

---

# 6. RAII 为什么适合这里

保存过程有很多失败点：

```text
临时文件打不开
写 header 失败
写 ENTRY 失败
flush 失败
close 失败
replace 失败
```

如果每个失败点都手动清理，很容易漏掉。

RAII 的好处是：

```text
只要函数返回
 -> temp_file 析构
 -> 自动删除临时文件
```

成功替换目标文件后，再调用：

```cpp
temp_file.release();
```

告诉清理器：

```text
临时文件已经不再是临时文件
不要删除
```

---

# 7. active_ 标记

类里有：

```cpp
bool active_ = true;
```

析构函数先检查：

```cpp
if (!active_) {
    return;
}
```

默认 active。

也就是：

```text
默认认为临时文件需要清理
```

成功替换后：

```cpp
void release() {
    active_ = false;
}
```

这样析构时就不会再删。

---

# 8. 为什么删除临时文件忽略 error

析构函数里：

```cpp
std::error_code error;
std::filesystem::remove(path_, error);
```

没有检查 `error`。

原因是析构函数不应该抛异常。

这里删除失败最多意味着：

```text
临时文件残留
```

这是不理想，但比在析构里抛异常导致程序终止更好。

测试里会检查成功路径下不应该残留 `.tmp.` 文件。

---

# 9. 禁止拷贝

类里写了：

```cpp
TempSnapshotFile(const TempSnapshotFile&) = delete;
TempSnapshotFile& operator=(const TempSnapshotFile&) = delete;
```

这是为了避免两个清理器同时管理同一个临时文件。

如果可以拷贝，就可能出现：

```text
对象 A 和对象 B 都持有同一个 temp path
 -> A 析构删除了
 -> B 析构又删一次
```

虽然重复删除通常不是灾难，但语义会乱。

所以这里直接禁止拷贝。

---

# 10. make_temp_snapshot_path 的角色

新增函数：

```cpp
std::filesystem::path make_temp_snapshot_path(const std::filesystem::path& path)
```

它根据目标 snapshot 路径生成临时文件路径。

比如目标是：

```text
data/state.snap
```

临时文件会类似：

```text
data/state.snap.tmp.123456789.0
```

重点是：

```text
临时文件在目标文件同一个目录
```

这是原子替换的关键前提。

跨目录 rename / replace 可能变成复制加删除，不能保证同样的原子语义。

---

# 11. 取 parent 和 filename

函数开头：

```cpp
const auto parent = path.parent_path();
const std::string filename = path.filename().empty() ? "snapshot" : path.filename().string();
```

`parent` 是目标目录。

例如：

```text
path = data/state.snap
parent = data
filename = state.snap
```

如果目标路径没有文件名，就用：

```text
snapshot
```

作为临时文件基础名。

这是一个兜底。

---

# 12. 使用 steady_clock 时间戳

接着：

```cpp
const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
```

这个 `stamp` 用来降低临时文件名冲突概率。

它不是为了表达真实时间。

它只需要提供：

```text
当前进程里足够变化的数字
```

所以 `steady_clock` 可以胜任。

---

# 13. 最多尝试 100 个文件名

生成逻辑：

```cpp
for (int attempt = 0; attempt < 100; ++attempt) {
    auto candidate = parent / (filename + ".tmp." + std::to_string(stamp) + "." + std::to_string(attempt));
    std::error_code error;
    if (!std::filesystem::exists(candidate, error)) {
        return candidate;
    }
}
```

它会尝试：

```text
state.snap.tmp.<stamp>.0
state.snap.tmp.<stamp>.1
state.snap.tmp.<stamp>.2
...
```

如果某个候选路径不存在，就返回。

这样即使前面有残留临时文件，也能换一个名字继续。

---

# 14. exists 出错时的行为

这里写的是：

```cpp
if (!std::filesystem::exists(candidate, error)) {
    return candidate;
}
```

如果 `exists` 因为某些系统错误无法判断，可能也返回 false。

这意味着函数可能返回一个后来打不开的 candidate。

但后面会立刻：

```cpp
std::ofstream output{temp_file.path(), std::ios::trunc};
if (!output) {
    return false;
}
```

所以最终仍然会失败返回 false。

这和 `SnapshotFile::save()` 的整体错误策略一致：

```text
内部失败
 -> 返回 false
```

---

# 15. fallback 路径

如果 100 次都没有找到不存在的候选路径：

```cpp
return parent / (filename + ".tmp." + std::to_string(stamp) + ".fallback");
```

这是最后兜底。

后面打开文件时如果冲突或失败，会返回 false。

正常情况下很难走到这里。

---

# 16. replace_file_atomically 的角色

新增函数：

```cpp
bool replace_file_atomically(const std::filesystem::path& source, const std::filesystem::path& target)
```

它把：

```text
source 临时文件
```

替换为：

```text
target 目标 snapshot 文件
```

这个函数把平台差异集中起来。

`SnapshotFile::save()` 只需要调用：

```cpp
replace_file_atomically(temp_file.path(), path)
```

不用关心 Windows 和非 Windows 的具体 API。

---

# 17. Windows 使用 MoveFileExW

Windows 分支：

```cpp
return MoveFileExW(source.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
```

`MOVEFILE_REPLACE_EXISTING` 表示：

```text
如果目标文件已存在，就替换它
```

`MOVEFILE_WRITE_THROUGH` 表示：

```text
尽量等移动/替换操作写入完成后再返回
```

返回值非 0 表示成功。

如果失败，函数返回 false。

---

# 18. 为什么不用 std::filesystem::rename 处理 Windows 覆盖

在一些平台上，`rename` 覆盖已有文件是常见行为。

但 Windows 的覆盖语义更容易踩坑。

第二十二版明确使用：

```text
MoveFileExW + MOVEFILE_REPLACE_EXISTING
```

这样表达更直接：

```text
我要用 source 替换已有 target
```

并且避免依赖不同标准库实现对覆盖行为的细节差异。

---

# 19. 非 Windows 使用 rename

非 Windows 分支：

```cpp
std::error_code error;
std::filesystem::rename(source, target, error);
return !error;
```

在 POSIX 文件系统上，同目录 rename 通常具备原子替换语义。

所以第二十二版的临时文件必须和目标文件在同一个目录。

失败时返回 false。

---

# 20. SnapshotFile::save 的前半段保持不变

保存函数开头还是：

```cpp
const auto items = store.snapshot_items();
```

这一步从 Store 导出当前有效数据。

它会跳过已经过期的 key。

然后仍然创建父目录：

```cpp
if (path.has_parent_path()) {
    std::error_code error;
    std::filesystem::create_directories(path.parent_path(), error);
    if (error) {
        return false;
    }
}
```

也就是说：

```text
SAVE data/state.snap
```

如果 `data/` 不存在，会先创建。

---

# 21. 新的临时文件写入

旧代码直接打开目标：

```cpp
std::ofstream output{path, std::ios::trunc};
```

新代码改成：

```cpp
TempSnapshotFile temp_file{make_temp_snapshot_path(path)};
std::ofstream output{temp_file.path(), std::ios::trunc};
if (!output) {
    return false;
}
```

也就是：

```text
创建临时文件清理器
打开临时文件
写临时文件
```

目标 snapshot 在这一步没有被截断。

---

# 22. 写入格式没有改变

写入内容仍然是：

```cpp
output << snapshot_header << '\n';
for (const auto& item : items) {
    output << "ENTRY " << format_expires_at(item.expires_at) << ' ' << std::quoted(item.key) << ' '
           << std::quoted(item.value) << '\n';
}
```

也就是说，snapshot 文件格式没有变。

仍然是：

```text
MINIKV_SNAPSHOT 1
ENTRY <expires_at> "<key>" "<value>"
```

第二十二版只改变保存过程，不改变文件格式。

旧的 snapshot 文件仍然可以被 `load()` 读取。

---

# 23. flush 检查

写完后：

```cpp
output.flush();
if (!output) {
    return false;
}
```

`flush()` 的作用是：

```text
把缓冲区里的数据推给底层文件
```

如果 flush 失败，函数返回 false。

由于 `temp_file` 还 active，返回时会自动删除临时文件。

目标文件仍然没被动过。

---

# 24. close 检查

第二十二版新增：

```cpp
output.close();
if (!output) {
    return false;
}
```

为什么要显式 close？

因为有些写入错误可能在关闭文件时才暴露。

比如：

```text
缓冲写入延迟失败
文件系统最终提交失败
```

只有 close 也成功，才进入替换目标文件阶段。

---

# 25. 替换目标文件

核心替换：

```cpp
if (!replace_file_atomically(temp_file.path(), path)) {
    return false;
}
temp_file.release();
```

如果替换失败：

```text
返回 false
TempSnapshotFile 析构
删除临时文件
目标旧 snapshot 保持原样或由平台保证不被替换成功
```

如果替换成功：

```text
临时文件已经成为目标文件
release
析构时不删除
```

---

# 26. saved 只在全部成功后设置

最后：

```cpp
if (saved != nullptr) {
    *saved = items.size();
}
return true;
```

注意它在替换成功之后才执行。

所以如果保存失败：

```text
saved 不会被更新
```

这和 `load()` 的保护风格一致：

```text
失败时尽量不改输出参数
```

---

# 27. load 函数没有改变

第二十二版没有改：

```cpp
SnapshotFile::load(...)
```

它仍然做：

```text
打开目标文件
读取 header
逐条解析 ENTRY
全部解析成功后 restore_snapshot
失败则返回 false
```

也就是说第十九版的坏 snapshot 保护继续保留。

第二十二版加固的是保存路径：

```text
save 更安全
load 保持原有保护
```

---

# 28. 原子保存的边界

第二十二版已经比直接 trunc 写目标文件安全很多。

但它不是完整数据库事务。

它保证的是：

```text
新 snapshot 完整写完前
不直接截断旧 snapshot
```

它不额外保证：

```text
跨文件系统移动
目录 fsync
断电场景下所有文件系统都完全一致
多个进程同时 SAVE 同一路径的并发协调
```

当前 mini-kv 是练习项目，这个实现已经是一个合理的可靠性进步。

---

# 29. 与 WAL 的关系

Snapshot 和 WAL 的职责不同。

Snapshot：

```text
保存当前完整状态
```

WAL：

```text
记录后续写命令
```

第二十二版只改 Snapshot 保存。

它没有改 WAL。

README Roadmap 也因此把下一步收敛为：

```text
Add WAL checksums for stronger recovery guarantees.
```

意思是：

```text
Snapshot 保存端已经做了第一轮原子写
WAL 以后可以继续加校验
```

---

# 30. 第二十二版核心增删改

新增：

```text
TempSnapshotFile
make_temp_snapshot_path
replace_file_atomically
```

修改：

```text
SnapshotFile::save
 -> 目标文件直接写
 -> 改为临时文件写完整后替换目标文件
```

保持不变：

```text
snapshot 文件格式
SnapshotFile::load
SAVE / LOAD 命令语义
Store snapshot_items / restore_snapshot
```

---

# 第四十五次总结

第二十二版 Snapshot 原子保存核心是：

```text
TempSnapshotFile
 -> 保存失败时自动清理临时文件

make_temp_snapshot_path
 -> 在目标目录生成 .tmp. 临时文件路径

replace_file_atomically
 -> Windows 用 MoveFileExW
 -> 非 Windows 用 std::filesystem::rename

SnapshotFile::save
 -> 写临时文件
 -> flush / close
 -> 替换目标 snapshot
```

一句话总结：

```text
第二十二版把 Snapshot 保存从“直接截断目标文件再写”推进到“同目录临时文件写完整后再替换目标文件”，降低 SAVE 失败时破坏旧 snapshot 的风险。
```
