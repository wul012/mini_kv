# 第四十二次讲解：第二十版 CI、README、归档和整体增删改

第四十一次讲了第十九版测试、README、归档和整体增删改。

这次讲第二十版配套内容：

```text
.github/workflows/ci.yml
CMakeLists.txt
README.md
a/20/解释/说明.md
a/20/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.20.0
项目有了 GitHub Actions CI 入口
CI 会在 Linux / macOS / Windows 上配置、构建、运行 CTest
README 已经把 CI 纳入当前能力说明
关键命令输出已经按 a/20 归档
```

---

# 1. 第二十版的定位

第十九版做的是持久化恢复加固：

```text
WAL replay report
坏 WAL 记录跳过
尾部半条 WAL 记录识别
坏 snapshot 不替换 Store
```

第二十版没有继续扩展业务命令，也没有新增 C++ 模块。

它的重点是工程成熟度：

```text
把本地能跑通的 CMake build + CTest
变成 GitHub Actions 上可重复执行的跨平台 CI
```

也就是从：

```text
我本机验证通过
```

推进到：

```text
每次 push / PR 都可以自动验证
```

---

# 2. 新增 .github/workflows/ci.yml

第二十版新增文件：

```text
.github/workflows/ci.yml
```

它是 GitHub Actions 的工作流配置。

GitHub 仓库收到符合条件的事件后，会读取 `.github/workflows/` 目录下的 yaml 文件，然后按里面定义的 job 执行命令。

这次的 CI 核心流程是：

```text
checkout 仓库
 -> cmake configure
 -> cmake build
 -> ctest
```

这和本地开发调试时跑的主流程一致。

---

# 3. workflow 名称

文件开头是：

```yaml
name: CI
```

`name` 是 GitHub Actions 页面显示的工作流名称。

这里叫 `CI`，意思是持续集成。

它不会影响构建行为，只影响显示：

```text
Actions 页面
Pull Request 检查项
commit 状态检查名称
```

---

# 4. 触发入口 on

接着是：

```yaml
on:
  push:
    branches:
      - main
      - master
  pull_request:
  workflow_dispatch:
```

它定义什么时候运行 CI。

第一类是：

```text
push 到 main
push 到 master
```

也就是说，主分支有新提交时自动跑。

第二类是：

```text
pull_request
```

这样以后开 PR 时，GitHub 会自动验证这个 PR 的代码能不能构建、测试能不能通过。

第三类是：

```text
workflow_dispatch
```

这是手动触发入口。

在 GitHub Actions 页面里可以点按钮手动跑一次，适合：

```text
验证当前仓库状态
排查 CI 环境问题
不用提交代码也想重新跑一遍
```

---

# 5. jobs 结构

CI 的主体是：

```yaml
jobs:
  build-and-test:
```

`jobs` 下面可以有多个任务。

这次只有一个：

```text
build-and-test
```

名字表达得很直接：

```text
先构建
再测试
```

对于当前 mini-kv 来说，一个 job 足够，因为项目构建链路还比较清晰：

```text
CMake 生成构建系统
CMake 编译所有 target
CTest 跑已注册测试
```

---

# 6. matrix.os

job 里有：

```yaml
name: ${{ matrix.os }}
runs-on: ${{ matrix.os }}
```

这里的 `${{ matrix.os }}` 是 GitHub Actions 表达式。

它会从下面的矩阵里取值：

```yaml
strategy:
  fail-fast: false
  matrix:
    os:
      - ubuntu-latest
      - macos-latest
      - windows-latest
```

所以实际会展开成 3 个 job：

```text
ubuntu-latest
macos-latest
windows-latest
```

等价于：

```text
Linux 跑一遍
macOS 跑一遍
Windows 跑一遍
```

这对 C++ 项目很重要。

因为本地在 Windows / MinGW 上通过，不代表：

```text
Linux 编译器一定通过
macOS 编译器一定通过
Windows GitHub runner 一定通过
```

跨平台 CI 可以更早发现：

```text
头文件缺失
平台宏差异
socket API 链接问题
路径分隔符问题
测试对时序太敏感
```

---

# 7. fail-fast false

这里有：

```yaml
fail-fast: false
```

默认情况下，如果矩阵里一个平台失败，GitHub Actions 可能会取消其他还没跑完的平台。

`fail-fast: false` 的意思是：

```text
即使某个平台失败
其他平台也继续跑完
```

这样排查问题时能一次看到完整结果。

比如：

```text
Linux 失败
macOS 通过
Windows 失败
```

这个信息比只看到第一个失败更有价值。

它能帮助判断问题属于：

```text
单平台问题
多平台问题
通用代码问题
```

---

# 8. checkout 步骤

第一步是：

```yaml
- name: Check out repository
  uses: actions/checkout@v4
```

GitHub runner 启动后，默认只是一个干净环境。

它还没有项目代码。

`actions/checkout@v4` 的作用是：

```text
把当前仓库代码检出到 runner 工作目录
```

后面的 `cmake -S .` 才能找到：

```text
CMakeLists.txt
include/
src/
tests/
README.md
```

没有 checkout，后面的命令都会找不到源码。

---

# 9. Configure 步骤

第二步是：

```yaml
- name: Configure
  run: cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

拆开看：

```text
cmake
 -> 调用 CMake

-S .
 -> 源码目录是当前仓库根目录

-B build
 -> 构建目录是 build

-DCMAKE_BUILD_TYPE=Release
 -> 对单配置生成器使用 Release 配置
```

这一步不会真正编译源文件。

它主要做：

```text
读取 CMakeLists.txt
检查 CMake 版本
识别 project(mini_kv VERSION 0.20.0 LANGUAGES CXX)
配置 C++20
收集 src / tests 目标
生成 build 目录里的构建文件
```

如果这里失败，通常说明：

```text
CMakeLists.txt 有语法问题
目标依赖写错
平台缺少必要配置
```

---

# 10. Build 步骤

第三步是：

```yaml
- name: Build
  run: cmake --build build --config Release --parallel
```

拆开看：

```text
cmake --build build
 -> 使用 build 目录里的构建系统编译

--config Release
 -> 对 Visual Studio 等多配置生成器选择 Release

--parallel
 -> 尽量并行编译
```

这里没有直接写 `make`、`ninja` 或 `msbuild`。

原因是：

```text
GitHub 三个平台底层生成器可能不同
CMake --build 可以统一调用实际构建工具
```

这样同一条命令可以适配：

```text
Linux
macOS
Windows
```

构建成功说明：

```text
minikv 静态/对象库能编译
minikv_cli 能链接
minikv_server 能链接
minikv_client 能链接
minikv_benchmark 能链接
所有测试 exe 能链接
```

---

# 11. Test 步骤

最后一步是：

```yaml
- name: Test
  run: ctest --test-dir build -C Release --output-on-failure
```

拆开看：

```text
ctest
 -> CMake 配套测试运行器

--test-dir build
 -> 到 build 目录读取 CTestTestfile.cmake

-C Release
 -> 对多配置生成器使用 Release 测试配置

--output-on-failure
 -> 测试失败时打印失败测试输出
```

当前 CTest 注册的测试包括：

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

所以 CI 不是只检查能不能编译。

它还会验证前面版本积累下来的关键行为。

---

# 12. 为什么使用 Release

workflow 里配置的是：

```text
-DCMAKE_BUILD_TYPE=Release
--config Release
-C Release
```

这是为了让不同生成器都能理解当前要跑 Release。

需要注意：

```text
单配置生成器
 -> 主要看 CMAKE_BUILD_TYPE

多配置生成器
 -> 主要看 --config 和 ctest -C
```

Linux / macOS 常见 Ninja 或 Makefile 更偏单配置。

Windows GitHub runner 常见 Visual Studio 生成器更偏多配置。

所以三处都写 Release，是一种兼容写法。

---

# 13. CMakeLists.txt 版本号更新

第二十版把项目版本改成：

```cmake
project(mini_kv VERSION 0.20.0 LANGUAGES CXX)
```

这里的版本号表示当前项目阶段。

它不会自动改变可执行文件名字。

但是它会让 CMake 知道：

```text
PROJECT_VERSION = 0.20.0
mini_kv_VERSION = 0.20.0
```

后续如果要把版本号写入：

```text
--version 命令
安装包
生成头文件
发布产物
```

就可以从 CMake project version 取。

---

# 14. 第二十版没有新增 C++ target

这一版没有新增：

```text
src/*.cpp
include/minikv/*.hpp
tests/*.cpp
```

所以 CMake target 数量不变。

仍然是：

```text
核心库 minikv
minikv_cli
minikv_server
minikv_client
minikv_benchmark
11 个测试 executable
```

本地 clean build 仍然显示 38 个构建步骤。

这说明第二十版是：

```text
工程自动化版本
```

不是：

```text
功能行为版本
```

---

# 15. README 当前版本更新

README 当前版本从第十九版的：

```text
Version 19 is a runnable in-memory KV service with WAL and snapshot recovery hardening:
```

改成第二十版的：

```text
Version 20 is a runnable in-memory KV service with cross-platform CMake and CTest CI:
```

关键词是：

```text
cross-platform CMake and CTest CI
```

这说明本版重点是：

```text
跨平台
CMake
CTest
持续集成
```

---

# 16. README 能力列表新增项

第二十版新增能力项：

```text
GitHub Actions CI for Linux, macOS, and Windows CMake build plus CTest
```

它对应的真实文件是：

```text
.github/workflows/ci.yml
```

它对应的真实流程是：

```text
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

这让 README 的能力列表不只描述程序功能，也描述项目维护能力。

---

# 17. README 新增 CI 章节

第二十版 README 新增：

````markdown
## CI

GitHub Actions runs the same CMake build and CTest flow on Linux, macOS, and Windows:

```text
.github/workflows/ci.yml
```

Each CI job configures the project, builds every target, and runs the registered CTest suite with failure output enabled.
````

这个章节回答三个问题：

```text
CI 用什么跑
 -> GitHub Actions

CI 在哪些系统跑
 -> Linux / macOS / Windows

CI 跑什么
 -> CMake 配置、构建、CTest
```

这样新读者不用打开 workflow，也能知道项目自动化验证的范围。

---

# 18. README Roadmap 调整

第十九版 Roadmap 里还有：

```text
Add CI coverage for build and CTest.
```

第二十版已经完成这个事项。

所以 Roadmap 调整为：

```text
1. Add persistent client history or interactive line editing.
2. Add WAL checksums or atomic snapshot writes for stronger recovery guarantees.
```

意思是：

```text
CI 已经落地
下一步重新回到客户端体验和持久化安全
```

Roadmap 现在不再把已经完成的 CI 当未来事项。

---

# 19. a/20 归档目录

第二十版新增归档：

```text
a/20/图片/
a/20/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-ci-workflow-check.png
05-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

这延续了前面版本的习惯：

```text
每次关键开发调试
 -> 留命令截图
 -> 留解释说明
 -> 以后可以回看当时验证了什么
```

---

# 20. 01-cmake-configure.png

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
CMake 能识别 0.20.0
现有构建目标都能正常生成
```

---

# 21. 02-cmake-build.png

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
新增 CI 文件和 README 调整没有破坏本地构建
已有 CLI / server / client / benchmark / tests 都能编译
```

---

# 22. 03-ctest.png

第三张图对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

测试列表是：

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
第二十版没有引入功能回归
前面十九版积累的核心行为仍然通过自动化测试
```

---

# 23. 04-ci-workflow-check.png

第四张图对应 PowerShell 结构检查。

检查项包括：

```text
workflow file exists
push trigger
pull_request trigger
workflow_dispatch trigger
ubuntu-latest matrix entry
macos-latest matrix entry
windows-latest matrix entry
checkout step
cmake configure step
cmake build step
ctest step
```

结果是：

```text
Result: PASS
```

这张图的意义是：

```text
确认 ci.yml 不是空文件
确认它包含真正需要的触发入口、平台矩阵和构建测试步骤
```

---

# 24. 05-version-docs-check.png

第五张图对应版本和文档一致性检查。

检查项包括：

```text
CMake version is 0.20.0
README current version is Version 20
README has CI section
README mentions GitHub Actions three OS CI
README roadmap moved past CI
Workflow file path exists
```

结果是：

```text
Result: PASS
```

这张图的意义是：

```text
确认代码版本、README 描述和 CI 文件路径互相对齐
```

这类检查很朴素，但很有用。

因为项目持续迭代时，最容易出现的问题之一就是：

```text
代码已经到新版本
README 还停在旧版本
Roadmap 还写着已经完成的任务
归档说明和实际文件对不上
```

---

# 25. 为什么第二十版没有单独跑真实 server smoke

前几版经常会启动真实 `minikv_server.exe` 做 smoke。

第二十版没有这么做。

原因是本版改动范围是：

```text
.github/workflows/ci.yml
CMakeLists.txt 版本号
README.md
a/20 归档
```

没有改：

```text
src/server_main.cpp
src/tcp_server.cpp
src/client_main.cpp
include/minikv/*.hpp
tests/*.cpp
```

所以本版最关键的验证不是 TCP 行为，而是：

```text
构建是否仍然通过
CTest 是否仍然全绿
CI 文件结构是否正确
README / CMake / workflow 是否同步
```

---

# 26. 本次代码增删改总览

第二十版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.20.0

README.md
 -> 当前版本说明更新为 Version 20
 -> 能力列表新增 GitHub Actions CI
 -> 新增 CI 章节
 -> Roadmap 移除已完成的 CI 项
```

第二十版新增：

```text
.github/workflows/ci.yml
 -> 新增跨平台 CI 工作流
 -> push main/master 自动触发
 -> pull_request 自动触发
 -> workflow_dispatch 手动触发
 -> ubuntu-latest / macos-latest / windows-latest 矩阵
 -> checkout / configure / build / test 四步验证

a/20/图片/01-cmake-configure.png
a/20/图片/02-cmake-build.png
a/20/图片/03-ctest.png
a/20/图片/04-ci-workflow-check.png
a/20/图片/05-version-docs-check.png
a/20/解释/说明.md
代码讲解记录/42-version-20-ci-docs.md
```

第二十版删除：

```text
无源码文件删除
```

---

# 27. 第二十版之后当前能力总览

到第二十版为止，mini-kv 已经具备：

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
外部客户端式 RESP-over-TCP pipeline 验证
RESP-over-TCP null bulk / integer / error / protocol error 验证
并发 RESP-over-TCP 客户端验证
CTest 自动化覆盖
GitHub Actions 跨平台 CI
运行归档
代码讲解记录
```

---

# 28. 第二十版和前几版的关系

第十六到第十八版主要证明：

```text
RESP-over-TCP 对真实 raw socket 客户端可用
兼容边界可控
并发客户端场景可验证
```

第十九版证明：

```text
持久化恢复遇到坏 WAL / 坏 snapshot 时更保守
恢复过程可报告
```

第二十版证明：

```text
这些能力可以进入跨平台 CI 自动验证链路
```

所以第二十版不是新增一个用户命令，而是把项目从：

```text
本地练习项目
```

推进到：

```text
具备基础工程化守护的练习项目
```

---

# 29. 第二十版之后适合继续做什么

第二十版完成 CI 后，后续方向可以更聚焦：

```text
客户端体验
 -> 持久化 history 文件
 -> 交互式行编辑
 -> 更友好的连接状态提示

持久化安全
 -> WAL checksum
 -> snapshot 原子写
 -> WAL compact

工程质量
 -> CI artifact 上传测试日志
 -> 增加 sanitizer job
 -> 增加 clang-tidy 或格式检查
```

当前 README Roadmap 选择了两条最自然的下一步：

```text
1. Add persistent client history or interactive line editing.
2. Add WAL checksums or atomic snapshot writes for stronger recovery guarantees.
```

---

# 30. 第二十版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v20-run 已删除
没有 minikv_server / minikv_client 残留进程
关键截图保留在 a/20/图片/
归档说明保留在 a/20/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第四十二次总结

第二十版配套工作证明了三件事：

```text
.github/workflows/ci.yml
 -> 项目有了 Linux / macOS / Windows 三平台 CMake build + CTest 工作流

README.md
 -> 当前版本、能力列表、CI 章节和 Roadmap 已经同步到 Version 20

a/20 归档
 -> 本地 configure / build / CTest、CI 结构检查、版本文档一致性检查都有截图和说明
```

一句话总结：

```text
第二十版把 mini-kv 从“本地能构建和测试”推进到“有跨平台 CI 入口持续守护构建和测试”，项目工程成熟度向前补了一块关键地基。
```
