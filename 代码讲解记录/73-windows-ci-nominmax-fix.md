# 第七十三次讲解：Windows CI 的 NOMINMAX 修复

这次不是新增业务功能，而是修复 GitHub Actions 的 Windows 构建失败。

Windows job 在 Build 阶段报错：

```text
warning C4003: not enough arguments for function-like macro invocation 'max'
error C2589: '(': illegal token on right side of '::'
```

出错位置在：

```text
src/client_main.cpp
```

根因是 Windows 头文件默认可能定义：

```text
min
max
```

这两个是宏。

当源码里写：

```cpp
std::numeric_limits<int>::max()
std::numeric_limits<DWORD>::max()
```

MSVC 预处理阶段会把 `max` 当宏展开，导致标准库调用被破坏。

---

# 1. CMake 层修复

`CMakeLists.txt` 的 Windows 分支新增：

```cmake
target_compile_definitions(minikv PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
```

含义是：

```text
所有链接 minikv 的目标
 -> 在 Windows 编译时都带上 NOMINMAX
 -> 同时带上 WIN32_LEAN_AND_MEAN
```

`NOMINMAX` 会阻止 Windows 头文件定义 `min` / `max` 宏。

使用 `PUBLIC` 是因为：

```text
minikv_client
minikv_server
各个测试 exe
```

都会链接 `minikv`，它们自己的 `.cpp` 也可能包含 Windows 头文件。

---

# 2. 源码层双保险

除了 CMake，所有直接包含 Windows 头文件的源码也补了：

```cpp
#ifndef NOMINMAX
#define NOMINMAX
#endif
```

位置包括：

```text
src/client_main.cpp
src/line_editor.cpp
src/snapshot.cpp
src/tcp_server.cpp
src/wal.cpp
tests/tcp_resp_compat_tests.cpp
tests/tcp_resp_concurrency_tests.cpp
tests/tcp_resp_tests.cpp
tests/tcp_server_tests.cpp
```

这样即使有人不用当前 CMake 目标传播规则，直接编译某个文件，也不容易再次踩到 `min` / `max` 宏污染。

---

# 3. 为什么本地 MinGW 没暴露

本机之前用的是 CLion 捆绑 MinGW 工具链。

MinGW 对 Windows 头宏、标准库和编译诊断的表现，与 GitHub `windows-latest` 上的 MSVC 不完全一样。

所以本机 MinGW 通过，不代表 MSVC 一定通过。

这次日志里出现：

```text
MSBuild version 18.5
minikv_client.vcxproj
```

说明 GitHub Windows job 走的是 Visual Studio / MSBuild。

---

# 4. 一句话总结

这次修复把 Windows 平台的头文件宏污染挡在项目边界外：

```text
CMake PUBLIC compile definitions
 -> 保护所有 CMake 目标

源码 NOMINMAX guard
 -> 保护直接包含 Windows 头文件的翻译单元
```

目标是让 `std::numeric_limits<...>::max()`、`std::min`、`std::max` 在 MSVC 下保持标准库语义，不再被 Windows 宏改写。

