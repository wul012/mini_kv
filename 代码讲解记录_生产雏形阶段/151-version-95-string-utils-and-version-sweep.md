# mini-kv v95 string utils shared split / version sweep

本版目标很明确：不是新功能，不是执行入口，不是审批绕过，也不是把 mini-kv 变成 Node/Java 的写入侧。v95 只做两件事：把分散在多个 cpp 里的 `trim_copy` 和 `to_upper` 收拢到共享头文件里，并把当前版本推进到 `0.95.0`，让 release fixture、runtime evidence、README 和测试都继续对齐到同一版号。

这一步很小，但很有必要。前几版已经把 command contract formatter、command response formatter、runtime evidence receipt formatter 逐层拆出去了；如果这时候还让字符串小工具散落在多个 cpp 里，后续继续拆 parser、completion、command dispatch 时，文件还是会慢慢鼓起来。v95 做的就是把这种基础重复先收住。

## 这版处在什么位置

mini-kv 现在已经不是纯粹的内存 KV 练手项目，而是带着 TCP、WAL、snapshot、只读证据和 release fixture 的生产雏形阶段。这个阶段最怕两件事：一是大文件继续膨胀，二是结构整理和行为变更混在一起。

v95 没有碰执行语义，只做公共字符串工具收口和版本同步，所以它是一个典型的“结构整洁版”。它把后续更大的拆分准备好了，但自己不引入新能力。

## 文件职责

`include/minikv/string_utils.hpp` 是这版新增的共享头文件。它只提供两个 inline 函数：

- `trim_copy(std::string_view)`：去掉首尾空白，返回新字符串
- `to_upper(std::string_view)`：返回大写副本

这两个工具现在被 `src/command.cpp`、`src/command_contracts.cpp`、`src/command_response_formatters.cpp`、`src/client_history.cpp`、`src/client_main.cpp`、`src/line_editor.cpp` 和 `src/wal.cpp` 共用。也就是说，它不是“某一个文件的私货”，而是整个 mini-kv 交互面和命令面都在用的基础工具。

`src/client_main.cpp` 继续负责 CLI 网络客户端和参数处理，`src/line_editor.cpp` 继续负责交互式编辑和补全，`src/command.cpp` 继续负责命令分发，`src/command_contracts.cpp` / `src/command_response_formatters.cpp` 继续负责契约解释和响应格式化。v95 只是把它们共同依赖的字符串整理逻辑集中了一层。

`CMakeLists.txt` 的版本号推进到 `0.95.0`，配合 README 和 fixtures 里的 `v95`、`cmake-build-v95`、`c/95/`，让版本链条在文档、测试和运行证据里保持一致。

## 控制面如何理解关键字段

这版没有改控制面语义，所以关键字段的含义仍然延续前一版：

- `read_only=true` 和 `execution_allowed=false` 仍然只代表只读契约检查
- `command_digest` 仍然是稳定的 `fnv1a64:<hex>` 指纹，不是签名
- `side_effects` / `side_effect_count` 仍然是风险解释，不是执行结果

这点很重要，因为 v95 虽然在结构上做了收口，但它没有试图把任何写入、恢复、WAL 变更偷偷塞进解释层。

## 测试和真实运行

这版同步更新了命令测试、release manifest 测试、runtime smoke 证据测试和相关 fixture。测试关注的不是“有没有多做一点事”，而是“边界有没有被改坏”。

真实 smoke 也跑过：`minikv_server` 启动后，用 `minikv_client` 发了 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`。其中 `GET restore:real-read-token` 返回 `(nil)`，说明这版没有把只读证据面变成写入面。

## 大文件现状

拆完后，`src` 目录里最大的文件已经是：

- `src/managed_audit_receipts.cpp` 619 行
- `src/tcp_server.cpp` 606 行
- `src/command.cpp` 557 行
- `src/line_editor.cpp` 545 行
- `src/command_response_formatters.cpp` 528 行
- `src/client_main.cpp` 485 行
- `src/wal.cpp` 476 行
- `src/runtime_evidence_receipts.cpp` 410 行
- `src/command_contracts.cpp` 384 行

所以“能不能拆到 1000 行左右”这个问题，答案已经是肯定的，而且是明显低于 1000。

## 一句话收尾

v95 做的是一层很朴素但很值钱的收口：让 mini-kv 的字符串基础工具和版本证据更干净，为后面继续拆 parser、completion、命令面和证据面留出更好的边界。
