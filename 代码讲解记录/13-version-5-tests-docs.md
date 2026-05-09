# 第十三次讲解：第五版测试、构建、文档和归档

第十二次讲了第五版快照核心：

```text
Store::snapshot_items
Store::restore_snapshot
SnapshotFile::save
SnapshotFile::load
CommandProcessor 的 SAVE / LOAD
```

这次讲第五版剩下的配套改动：

```text
tests/snapshot_tests.cpp
tests/command_tests.cpp
CMakeLists.txt
README.md
AGENTS.md
a/5/解释/说明.md
a/5/图片/
```

它们负责证明第五版不是只写了模块，而是真的能：

```text
编译
测试
通过 CLI 运行
通过 TCP 运行
被文档记录
被后续开发者少踩环境坑
```

---

# 1. 第五版测试整体思路

第五版新增的是快照能力，所以测试分成两层：

```text
底层测试：
  tests/snapshot_tests.cpp
  直接调用 SnapshotFile::save / load

命令层测试：
  tests/command_tests.cpp
  通过 CommandProcessor 执行 SAVE / LOAD
```

底层测试确认：

```text
快照模块本身能保存和加载 Store
```

命令层测试确认：

```text
用户输入 SAVE / LOAD 时，CommandProcessor 能正确接到快照模块
```

这两层都要有。

否则只测 `SnapshotFile`，不能证明 CLI / TCP 命令可用。

只测命令层，又不够直接说明快照文件读写细节正确。

---

# 2. `tests/snapshot_tests.cpp` 的角色

第五版新增文件：

```text
tests/snapshot_tests.cpp
```

它是快照模块的专门测试。

测试目标是：

```text
保存普通 key
保存带 TTL 的 key
加载到新的 Store
验证 value 被恢复
验证 TTL 信息被恢复
```

---

# 3. snapshot_tests 使用临时文件

```cpp
const auto path = std::filesystem::path{"minikv-snapshot-test.snap"};
std::filesystem::remove(path);
```

这里使用一个测试快照文件：

```text
minikv-snapshot-test.snap
```

测试开始前先删除。

原因是：

```text
保证测试从干净状态开始
避免上次残留文件影响结果
```

测试结束后也会删除它。

---

# 4. 创建待保存的 Store

```cpp
minikv::Store store;
assert(store.set("name", "mini-kv"));
assert(store.set("phrase", "hello from snapshot"));
assert(store.expire("name", 60s));
```

这里写入两个 key：

```text
name   -> mini-kv
phrase -> hello from snapshot
```

然后给 `name` 设置 TTL：

```text
60 秒
```

这覆盖了两类数据：

```text
带 TTL 的 key
不带 TTL 的 key
```

这样可以验证 snapshot 是否同时保存 value 和 expires_at。

---

# 5. 保存快照

```cpp
std::size_t saved = 0;
assert(minikv::SnapshotFile::save(store, path, &saved));
assert(saved == 2);
```

这里直接调用：

```cpp
SnapshotFile::save
```

如果保存失败，测试会失败。

`saved == 2` 说明快照保存了两个有效 key：

```text
name
phrase
```

---

# 6. 加载到新的 Store

```cpp
minikv::Store restored;
std::size_t loaded = 0;
assert(minikv::SnapshotFile::load(restored, path, &loaded));
assert(loaded == 2);
```

这里没有加载回原来的 `store`，而是加载到新的：

```cpp
restored
```

这样能证明：

```text
数据确实来自 snapshot 文件
不是原 Store 里本来就有
```

`loaded == 2` 说明文件里两条记录都被解析并传给 Store 恢复。

---

# 7. 验证 value 和 TTL

```cpp
assert(restored.get("name") == std::optional<std::string>{"mini-kv"});
assert(restored.ttl("name").has_value());
assert(restored.get("phrase") == std::optional<std::string>{"hello from snapshot"});
```

第一条验证：

```text
name 的 value 被恢复
```

第二条验证：

```text
name 的 TTL 也被恢复
```

这里没有要求 TTL 一定等于 60。

因为保存和加载之间会消耗一点时间。

所以只验证：

```text
ttl(name).has_value()
```

表示：

```text
它仍然是一个会过期的 key
```

第三条验证：

```text
phrase 的带空格 value 被恢复
```

这也间接验证了 snapshot 文件里的：

```cpp
std::quoted
```

可以处理 value 中的空格。

---

# 8. snapshot_tests 清理临时文件

```cpp
std::filesystem::remove(path);
```

测试结束后删除：

```text
minikv-snapshot-test.snap
```

这样测试不会在项目目录里留下临时快照。

---

# 9. command_tests 新增 SAVE / LOAD 覆盖

第五版还修改了：

```text
tests/command_tests.cpp
```

新增一段命令层快照测试。

它的作用是：

```text
不直接调用 SnapshotFile
而是像用户一样通过 CommandProcessor 执行命令
```

---

创建命令层快照路径：

```cpp
const auto snapshot_path = std::filesystem::path{"minikv-command-snapshot-test.snap"};
std::filesystem::remove(snapshot_path);

store.clear();
```

先删旧文件。

然后：

```cpp
store.clear();
```

把前面命令测试留下的数据清空。

这样快照测试只关注自己写入的 key。

---

写入并保存：

```cpp
result = processor.execute("SET snap saved value");
assert(result.response == "OK inserted");

result = processor.execute(std::string{"SAVE "} + snapshot_path.string());
assert(result.response == "OK saved 1");
```

这里 value 是：

```text
saved value
```

带空格。

`SAVE` 成功后返回：

```text
OK saved 1
```

说明命令层不仅识别了 `SAVE`，也拿到了保存数量。

---

删除后再加载：

```cpp
result = processor.execute("DEL snap");
assert(result.response == "1");

result = processor.execute(std::string{"LOAD "} + snapshot_path.string());
assert(result.response == "OK loaded 1");
```

这一步很关键。

如果不先删除，后面的 `GET snap` 即使成功，也可能只是原内存数据还在。

先删除，再 `LOAD`，才能证明：

```text
数据确实从 snapshot 文件恢复回来
```

---

验证恢复结果：

```cpp
result = processor.execute("GET snap");
assert(result.response == "saved value");
```

这说明：

```text
CommandProcessor 的 LOAD 命令真正恢复了数据
```

最后清理：

```cpp
std::filesystem::remove(snapshot_path);
```

---

# 10. CMake 新增 snapshot.cpp

第五版把快照实现加入核心库：

```cmake
add_library(minikv
    src/command.cpp
    src/snapshot.cpp
    src/store.cpp
    src/tcp_server.cpp
    src/wal.cpp
)
```

这样所有链接 `minikv` 的目标都能使用快照模块：

```text
minikv_cli
minikv_server
minikv_client
tests
```

虽然 client 不直接调用 snapshot 代码，但它链接同一个库，整体构建保持简单。

---

# 11. CMake 新增 snapshot_tests

第五版新增测试目标：

```cmake
add_executable(minikv_snapshot_tests
    tests/snapshot_tests.cpp
)

target_link_libraries(minikv_snapshot_tests PRIVATE minikv)

add_test(NAME snapshot_tests COMMAND minikv_snapshot_tests)
```

这会生成：

```text
minikv_snapshot_tests
```

并注册到 CTest：

```text
snapshot_tests
```

第五版运行 CTest 时会看到四个测试：

```text
store_tests
command_tests
wal_tests
snapshot_tests
```

---

# 12. CMake 版本更新

第五版项目版本改成：

```cmake
project(mini_kv VERSION 0.5.0 LANGUAGES CXX)
```

版本节奏现在是：

```text
0.1.0 = 内存 KV + CLI + TCP server
0.2.0 = TCP client
0.3.0 = TTL
0.4.0 = WAL persistence
0.5.0 = Manual snapshots
```

---

# 13. README 当前版本更新

第五版 README 当前版本变成：

```text
Version 5 is a runnable in-memory KV service with snapshots:
```

功能列表新增：

```text
Manual snapshots with SAVE and LOAD
```

这说明当前项目已经支持：

```text
手动保存当前内存状态
手动加载 snapshot 替换当前内存状态
```

---

# 14. README 新增 snapshot 测试目标

CLion 目标列表新增：

```text
minikv_snapshot_tests
```

这样在 CLion 里可以直接运行第五版快照测试。

---

# 15. README 新增 CLI 命令

第五版命令列表新增：

```text
SAVE path
LOAD path
```

完整命令列表现在是：

```text
SET key value
GET key
DEL key
EXPIRE key seconds
TTL key
SIZE
SAVE path
LOAD path
HELP
EXIT
```

这说明快照功能不是隐藏 API，而是用户可直接输入的命令。

---

# 16. README 新增 TCP 协议示例

TCP protocol 示例新增：

```text
SAVE data\mini-kv.snapshot
DEL name
LOAD data\mini-kv.snapshot
```

这说明：

```text
TCP server 不需要额外接口
只要客户端发送 SAVE / LOAD 文本命令即可
```

因为 TCP server 底层也是调用：

```cpp
CommandProcessor
```

所以 CLI 和 TCP 共享同一套命令能力。

---

# 17. README 新增 Snapshots 章节

第五版新增：

```text
## Snapshots
```

核心说明是：

```text
SAVE path  Save all currently live keys to a snapshot file.
LOAD path  Replace the current in-memory dataset with a snapshot file.
```

这里有两个关键词。

第一个是：

```text
currently live keys
```

意思是：

```text
只保存当前仍然有效的 key
```

已过期 key 不会保存。

---

第二个是：

```text
Replace
```

意思是：

```text
LOAD 是替换当前内存数据
不是合并
```

这和 `Store::restore_snapshot` 里的：

```cpp
data_.clear();
```

是对应的。

---

README 还说明：

```text
Snapshot files preserve values and absolute expiration times.
```

也就是：

```text
snapshot 会保存 value
也会保存 TTL 的绝对过期时间
```

这对应第十二次讲过的：

```text
SnapshotItem::expires_at
ENTRY epoch_millis "key" "value"
```

---

# 18. Roadmap 更新

第五版完成 snapshot 后，Roadmap 变成：

```text
1. Add benchmarking and stress tests.
2. Add a Redis RESP parser.
```

也就是说：

```text
Add snapshots
```

已经从计划项变成已完成功能。

---

# 19. 第五版归档 `a/5`

第五版按项目规则新增：

```text
a/5/图片/
a/5/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-cli-snapshot-smoke.png
05-tcp-snapshot-smoke.png
```

说明文档记录：

```text
第五版做了什么
每张截图对应什么命令
构建、测试、CLI smoke test、TCP smoke test 的结果
服务端进程是否停止
临时文件是否清理
```

---

# 20. `01-cmake-configure.png`

对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
配置成功
```

意义：

```text
CMake 能识别第五版新增的 snapshot.cpp、snapshot_tests.cpp 和测试目标
```

---

# 21. `02-cmake-build.png`

对应命令：

```text
cmake --build cmake-build-debug
```

结果：

```text
编译成功
```

意义：

```text
第五版快照模块、命令层和 Store 调整能通过 C++20 编译
```

---

# 22. `03-ctest.png`

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
100% tests passed
4 tests passed
0 tests failed
```

四个测试是：

```text
store_tests
command_tests
wal_tests
snapshot_tests
```

意义：

```text
原有 Store、CommandProcessor、WAL 行为没有被快照改坏
新增 snapshot_tests 也通过
```

---

# 23. `04-cli-snapshot-smoke.png`

CLI smoke test 通过管道输入：

```text
SET v5 first snapshot value
SAVE a\5\mini-kv-v5-cli.snapshot
DEL v5
GET v5
LOAD a\5\mini-kv-v5-cli.snapshot
GET v5
QUIT
```

关键结果：

```text
OK saved 1
OK loaded 1
first snapshot value
```

这证明：

```text
CLI 可以执行 SAVE
CLI 可以执行 LOAD
LOAD 后能恢复删除过的数据
```

---

# 24. `05-tcp-snapshot-smoke.png`

TCP smoke test 先启动服务端：

```text
minikv_server.exe 6395
```

然后客户端连接：

```text
minikv_client.exe 127.0.0.1 6395
```

输入：

```text
SET tcp from network snapshot
SAVE a\5\mini-kv-v5-tcp.snapshot
DEL tcp
LOAD a\5\mini-kv-v5-tcp.snapshot
GET tcp
QUIT
```

关键结果：

```text
OK saved 1
OK loaded 1
from network snapshot
```

这证明：

```text
TCP server 复用 CommandProcessor 后，也支持 SAVE / LOAD
```

验证结束后，测试中启动的 server 进程已停止。

---

# 25. 本机工具链规则补充

第五版调试时遇到一个本机环境问题：

```text
直接运行 ctest 时，测试 exe 可能因为找不到 MinGW 运行时 DLL 报 0xc0000135
```

这个错误不是代码逻辑失败。

它表示：

```text
Windows 找不到程序运行依赖的 DLL
```

所以后来在 `AGENTS.md` 里补充了本机规则：

```powershell
$cmake = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
$ctest = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\ctest.exe'
$env:PATH = 'D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
```

也就是说，后续在本机跑：

```text
ctest
测试 exe
minikv_cli.exe
minikv_server.exe
minikv_client.exe
```

都要先把：

```text
D:\CLION\CLion 2025.1.1\bin\mingw\bin
```

加到当前 `PATH`。

这样可以少走一次弯路。

---

# 26. 第五版文件变化总表

第五版相关增删改可以整理成：

```text
新增：
  include/minikv/snapshot.hpp
  src/snapshot.cpp
  tests/snapshot_tests.cpp
  a/5/图片/
  a/5/解释/说明.md

修改：
  include/minikv/store.hpp
    新增 SnapshotItem
    新增 snapshot_items()
    新增 restore_snapshot()

  src/store.cpp
    导出带 TTL 的快照项
    加载 snapshot 时替换当前数据
    跳过已过期记录

  src/command.cpp
    新增 SAVE path
    新增 LOAD path
    HELP 中加入 SAVE / LOAD

  tests/command_tests.cpp
    新增命令层 SAVE / LOAD 覆盖

  CMakeLists.txt
    版本 0.5.0
    minikv 加入 snapshot.cpp
    新增 snapshot_tests

  README.md
    Version 5、Snapshots 章节、命令列表、测试目标、Roadmap 更新

  AGENTS.md
    新增本机 CLion 工具链和 MinGW PATH 规则

删除：
  无源码文件删除
```

---

# 27. 第五版当前能力总览

到第五版为止，mini-kv 已经具备：

```text
内存 key-value 存储
命令解析
CLI 模式
TCP server
TCP client
TTL 过期
WAL 增量持久化
Snapshot 手动完整状态保存和加载
CTest 覆盖
运行归档
```

从数据持久化角度看，现在有两条链路：

```text
WAL 链路：
  启动时传 wal_path
  写命令自动 append
  下次启动 replay

Snapshot 链路：
  运行中执行 SAVE path
  当前有效数据写入 snapshot
  运行中执行 LOAD path
  当前 Store 替换为 snapshot 数据
```

---

# 第十三次总结

第五版不只是新增了 `SnapshotFile`，还把它完整接到了项目体系里：

```text
CMake 能构建
CTest 有 snapshot_tests
CommandProcessor 测了 SAVE / LOAD
README 写了用法
a/5 有配置、构建、测试、CLI、TCP 的运行证据
AGENTS.md 记录了本机工具链坑
```

一句话总结：

```text
第五版把 snapshot 从一个文件读写模块变成了 mini-kv 的正式用户功能：CLI 能用，TCP 能用，测试能证明，文档和归档也能追溯。
```
