# 第四十六次讲解：第二十二版测试、README、归档和整体增删改

第四十五次讲了第二十二版 Snapshot 原子保存核心。

这次讲第二十二版配套内容：

```text
CMakeLists.txt
README.md
tests/snapshot_tests.cpp
a/22/解释/说明.md
a/22/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.22.0
SnapshotFile::save 不再直接截断目标 snapshot 写入
snapshot_tests 覆盖同一路径二次保存和临时文件清理
真实 minikv_cli SAVE / LOAD 能覆盖旧 snapshot 并加载新内容
README 已经记录 atomic snapshot saves
关键命令输出已经按 a/22 归档
```

---

# 1. 第二十二版的定位

第二十一版做的是客户端体验：

```text
--history-file
跨会话持久化历史
:history / !! / !N 可以使用旧会话命令
```

第二十二版回到持久化可靠性：

```text
atomic snapshot saves
```

它不是新增用户命令。

用户仍然使用：

```text
SAVE path
LOAD path
```

改变的是 `SAVE` 背后的写文件方式。

---

# 2. CMakeLists.txt 版本号更新

第二十二版把版本号改成：

```cmake
project(mini_kv VERSION 0.22.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然复用已有：

```text
minikv_snapshot_tests
```

所以构建步骤仍然是：

```text
38
```

---

# 3. snapshot_tests 原有覆盖

`tests/snapshot_tests.cpp` 原来已经覆盖两类场景。

第一类是正常 save/load：

```text
Store 里写 name / phrase
给 name 设置 TTL
SnapshotFile::save
SnapshotFile::load 到新 Store
验证 value 和 ttl
```

第二类是坏 snapshot load：

```text
准备 protected_store
写一个缺 value 的坏 snapshot 文件
SnapshotFile::load 返回 false
loaded 输出参数不变
protected_store 不被替换
```

第二十二版保留这些测试。

这表示：

```text
原子保存改动不能破坏原有 save/load 和坏文件保护行为
```

---

# 4. 新增 atomic_dir

第二十二版新增：

```cpp
const auto atomic_dir = std::filesystem::path{"minikv-snapshot-atomic-test"};
const auto atomic_path = atomic_dir / "state.snap";
std::filesystem::remove_all(atomic_dir);
std::filesystem::create_directories(atomic_dir);
```

测试专门建了一个目录：

```text
minikv-snapshot-atomic-test/
```

目标快照是：

```text
minikv-snapshot-atomic-test/state.snap
```

这样测试可以检查这个目录里有没有 `.tmp.` 临时文件残留。

---

# 5. has_temp_snapshot 辅助函数

新增辅助 lambda：

```cpp
auto has_temp_snapshot = [&]() {
    for (const auto& entry : std::filesystem::directory_iterator(atomic_dir)) {
        if (entry.path().filename().string().find(".tmp.") != std::string::npos) {
            return true;
        }
    }
    return false;
};
```

它遍历测试目录。

只要文件名包含：

```text
.tmp.
```

就认为还有临时 snapshot 文件残留。

这对应第二十二版临时文件命名：

```text
state.snap.tmp.<stamp>.<attempt>
```

---

# 6. 第一次保存旧数据

第一份 Store：

```cpp
minikv::Store first_snapshot;
assert(first_snapshot.set("name", "old"));
saved = 0;
assert(minikv::SnapshotFile::save(first_snapshot, atomic_path, &saved));
assert(saved == 1);
assert(std::filesystem::exists(atomic_path));
assert(!has_temp_snapshot());
```

这验证：

```text
第一次保存成功
保存了 1 条
目标 snapshot 文件存在
保存成功后没有 .tmp. 临时文件
```

这里的值是：

```text
name = old
```

---

# 7. 第二次保存新数据

第二份 Store：

```cpp
minikv::Store second_snapshot;
assert(second_snapshot.set("name", "new"));
assert(second_snapshot.set("extra", "value"));
saved = 0;
assert(minikv::SnapshotFile::save(second_snapshot, atomic_path, &saved));
assert(saved == 2);
assert(!has_temp_snapshot());
```

注意它保存到同一个路径：

```text
atomic_path
```

也就是覆盖第一次保存的：

```text
state.snap
```

预期是：

```text
第二次保存成功
保存 2 条
旧内容被新 snapshot 替换
没有临时文件残留
```

---

# 8. 加载替换后的 snapshot

然后测试加载：

```cpp
minikv::Store replaced;
loaded = 0;
assert(minikv::SnapshotFile::load(replaced, atomic_path, &loaded));
assert(loaded == 2);
assert(replaced.get("name") == std::optional<std::string>{"new"});
assert(replaced.get("extra") == std::optional<std::string>{"value"});
```

这里确认第二次保存真的替换了目标 snapshot。

如果替换失败或者仍然读到旧文件，结果会是：

```text
name = old
extra missing
```

测试要求：

```text
name = new
extra = value
```

---

# 9. 清理 atomic_dir

最后：

```cpp
std::filesystem::remove_all(atomic_dir);
```

这符合项目里的清理规则。

测试生成的临时目录不应留在工作区。

---

# 10. README 当前版本更新

README 当前版本从：

```text
Version 21 is a runnable in-memory KV service with persistent TCP client command history:
```

改成：

```text
Version 22 is a runnable in-memory KV service with atomic snapshot saves:
```

关键词是：

```text
atomic snapshot saves
```

这说明第二十二版重点是 Snapshot 保存可靠性。

---

# 11. README 能力列表新增项

能力列表新增：

```text
Atomic snapshot saves write a temporary file before replacing the target snapshot
```

这句话对应真实实现：

```text
TempSnapshotFile
make_temp_snapshot_path
replace_file_atomically
SnapshotFile::save
```

它告诉用户：

```text
SAVE 不再直接截断目标 snapshot 写入
```

---

# 12. README Snapshot 章节更新

Snapshot 章节新增说明：

```text
SAVE writes a temporary file in the target directory
and then replaces the target snapshot,
avoiding direct truncation of the previous snapshot
while the new one is still being written.
```

这句话拆开看：

```text
temporary file in the target directory
 -> 同目录临时文件

then replaces the target snapshot
 -> 完整写完后替换目标

avoiding direct truncation
 -> 避免直接截断旧 snapshot
```

它和第二十二版代码行为一致。

---

# 13. README 测试说明更新

测试说明里新增：

```text
snapshot_tests verifies snapshot save/load,
corrupt snapshot rejection,
and atomic overwrite behavior without leaving temporary snapshot files behind.
```

这让 README 里的测试说明跟 `tests/snapshot_tests.cpp` 同步。

也明确告诉读者：

```text
atomic overwrite
temp cleanup
```

已经有自动化测试覆盖。

---

# 14. README Roadmap 调整

第二十一版 Roadmap 里还有：

```text
Add WAL checksums or atomic snapshot writes for stronger recovery guarantees.
```

第二十二版已经完成：

```text
atomic snapshot writes
```

所以 Roadmap 改成：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL checksums for stronger recovery guarantees.
```

持久化可靠性方向剩下更自然的下一步是：

```text
WAL checksums
```

---

# 15. a/22 归档目录

第二十二版新增归档：

```text
a/22/图片/
a/22/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-snapshot-tests.png
05-cli-snapshot-smoke.png
06-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 16. 01-cmake-configure.png

第一张图对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
配置成功
构建文件写入 cmake-build-debug
```

意义：

```text
CMake 能识别 0.22.0
现有构建目标都能正常生成
```

---

# 17. 02-cmake-build.png

第二张图对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完整编译链接
38 个构建步骤成功
```

意义：

```text
TempSnapshotFile 能编译
MoveFileExW 路径能编译链接
replace_file_atomically 能编译
snapshot_tests 新增断言能编译
旧目标没有构建回归
```

---

# 18. 03-ctest.png

第三张图对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

测试列表仍是：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
resp_tests
tcp_server_tests
tcp_resp_tests
tcp_resp_compat_tests
tcp_resp_concurrency_tests
client_history_tests
```

意义：

```text
Snapshot 原子保存改动通过
前面二十一版积累的能力没有回归
```

---

# 19. 04-snapshot-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_snapshot_tests.exe
```

结果：

```text
退出码 0
Result: PASS
```

这张图专门单独复跑 Snapshot 测试。

原因是第二十二版主要改动就在：

```text
SnapshotFile::save
tests/snapshot_tests.cpp
```

---

# 20. snapshot_tests 新增验证意义

新增断言确认：

```text
第一次同路径保存
 -> 保存 1 条
 -> 目标文件存在
 -> 无 .tmp. 残留

第二次同路径保存
 -> 保存 2 条
 -> 无 .tmp. 残留

再次 load
 -> loaded = 2
 -> name = new
 -> extra = value
```

这覆盖了：

```text
覆盖旧 snapshot
加载新 snapshot
成功路径临时文件清理
```

---

# 21. 05-cli-snapshot-smoke.png

第五张图是真实 CLI smoke。

它真实运行 3 次：

```text
.\cmake-build-debug\minikv_cli.exe
```

第一轮：

```text
SET name old
SAVE tmp-v22-run/snapshot-smoke/state.snap
EXIT
```

第二轮：

```text
SET name new
SET extra value
SAVE tmp-v22-run/snapshot-smoke/state.snap
EXIT
```

第三轮：

```text
LOAD tmp-v22-run/snapshot-smoke/state.snap
GET name
GET extra
EXIT
```

---

# 22. CLI smoke 的结果

截图里结果是：

```text
first SAVE reported one item: True
second SAVE reported two items: True
snapshot file exists: True
LOAD reported two items: True
GET name returns new: True
GET extra returns value: True
snapshot content has new value: True
snapshot content has extra value: True
no temp snapshot files remain: True
Result: PASS
```

这证明：

```text
真实 CLI SAVE 可以创建 snapshot
真实 CLI SAVE 可以覆盖同一路径 snapshot
真实 CLI LOAD 可以读回覆盖后的新内容
临时文件不会留在 smoke 目录
```

---

# 23. 最终 snapshot 文件内容

smoke 里最终文件内容是：

```text
MINIKV_SNAPSHOT 1
ENTRY - "extra" "value"
ENTRY - "name" "new"
```

这说明第二次保存写入的是完整新状态：

```text
extra = value
name = new
```

旧状态：

```text
name = old
```

已经被替换。

---

# 24. 06-version-docs-check.png

第六张图对应版本和文档一致性检查。

检查项包括：

```text
CMake version is 0.22.0
README current version is Version 22
README documents atomic snapshot saves
README roadmap moved past atomic snapshot writes
Snapshot save uses temporary snapshot file
Snapshot save replaces target atomically
Windows path uses MoveFileExW replace
snapshot_tests cover atomic overwrite
snapshot_tests check temp cleanup
```

结果：

```text
Result: PASS
```

意义是：

```text
代码版本、README 描述、Snapshot 实现和测试覆盖互相对齐
```

---

# 25. 本次代码增删改总览

第二十二版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.22.0

src/snapshot.cpp
 -> 新增 TempSnapshotFile RAII 临时文件清理器
 -> 新增 make_temp_snapshot_path
 -> 新增 replace_file_atomically
 -> Windows 使用 MoveFileExW 替换目标文件
 -> 非 Windows 使用 std::filesystem::rename
 -> SnapshotFile::save 改为写临时文件、flush、close、replace

tests/snapshot_tests.cpp
 -> 新增 atomic_dir / atomic_path
 -> 新增 has_temp_snapshot
 -> 新增同一路径第一次保存验证
 -> 新增同一路径第二次覆盖保存验证
 -> 新增加载覆盖后 snapshot 内容验证
 -> 新增成功保存后无 .tmp. 临时文件残留验证

README.md
 -> 当前版本说明更新为 Version 22
 -> 能力列表新增 atomic snapshot saves
 -> Snapshot 章节补充临时文件后替换目标说明
 -> 测试说明补充 snapshot_tests atomic overwrite
 -> Roadmap 移除 atomic snapshot writes
```

第二十二版新增：

```text
a/22/图片/01-cmake-configure.png
a/22/图片/02-cmake-build.png
a/22/图片/03-ctest.png
a/22/图片/04-snapshot-tests.png
a/22/图片/05-cli-snapshot-smoke.png
a/22/图片/06-version-docs-check.png
a/22/解释/说明.md
代码讲解记录/45-snapshot-atomic-save.md
代码讲解记录/46-version-22-tests-docs.md
```

第二十二版删除：

```text
无源码文件删除
```

---

# 26. 第二十二版之后当前能力总览

到第二十二版为止，mini-kv 已经具备：

```text
内存 key-value 存储
命令解析
CLI 模式
TCP server
TCP client
TTL 过期
WAL 增量持久化
WAL replay applied / skipped / truncated 报告
WAL 坏记录跳过
WAL 尾部疑似部分写入记录跳过
Snapshot 手动完整状态保存和加载
Snapshot SAVE 临时文件写完后替换目标文件
坏 Snapshot 不替换当前 Store
Benchmark 本地吞吐观察
Stress test 并发压力验证
RESP 请求解析器
TCP RESP 请求/响应
PING 探活命令
RESP parser 参数数量限制
RESP parser bulk string 长度限制
TcpServer 可请求停止
server_main 支持 Ctrl+C / SIGTERM 停止标记
结构化生命周期日志
并发连接事件下 stdout 日志按行输出
TCP connection_id
TCP active / total / peak connection metrics
TCP request byte limit
event=tcp_request_rejected 请求拒绝日志
server_main 可配置 max_request_bytes 和 accept_poll_ms
client_main 可配置 socket timeout
localhost / hostname 网络兼容性测试
client_main 可配置连接重试和重试间隔
client_main 本地会话历史 :history / !! / !N
client_main 可选 --history-file 跨会话持久化历史
外部客户端式 RESP-over-TCP pipeline 验证
RESP-over-TCP null bulk / integer / error / protocol error 验证
并发 RESP-over-TCP 客户端验证
CTest 自动化覆盖
GitHub Actions 跨平台 CI
运行归档
代码讲解记录
```

---

# 27. 第二十二版和前几版的关系

第五版：

```text
加入 Snapshot save/load
```

第十九版：

```text
坏 snapshot load 不替换当前 Store
```

第二十二版：

```text
snapshot save 写临时文件后再替换目标文件
```

它们是一条连续演进：

```text
能保存/加载 snapshot
 -> 坏文件加载更保守
 -> 保存过程更保守
```

---

# 28. 第二十二版之后适合继续做什么

持久化可靠性方向：

```text
WAL checksums
WAL compact
Snapshot manifest 或版本信息
```

客户端体验方向：

```text
交互式行编辑
方向键历史浏览
命令补全
```

工程质量方向：

```text
CI 上传测试日志
Sanitizer job
格式检查或静态分析
```

README 当前 Roadmap 选择：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL checksums for stronger recovery guarantees.
```

---

# 29. 第二十二版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v22-run 已删除
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/22/图片/
归档说明保留在 a/22/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第四十六次总结

第二十二版配套工作证明了三件事：

```text
snapshot_tests
 -> 正常 save/load、坏 snapshot 保护、同路径二次保存、临时文件清理都有自动化测试

真实 CLI smoke
 -> SAVE / LOAD 在真实 minikv_cli 里可以覆盖旧 snapshot 并加载新内容

README 和 a/22 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十二版把 Snapshot 保存从“直接覆盖写目标文件”推进到“临时文件完整写入后替换目标文件”，并用单元测试、全量 CTest 和真实 CLI smoke 做了复核。
```
