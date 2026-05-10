# 第四十三次讲解：第二十一版客户端持久化历史核心

第四十二次讲了第二十版 CI、README、归档和整体增删改。

这次讲第二十一版的核心代码：

```text
include/minikv/client_history.hpp
src/client_history.cpp
src/client_main.cpp
```

它们负责把第十五版已有的“单次会话历史”扩展成：

```text
可选历史文件
 -> 客户端启动时读取
 -> :history / !! / !N 可以使用旧会话命令
 -> 命令发送并收到响应后保存
 -> 下一次客户端启动继续使用
```

---

# 1. 第二十一版要解决什么问题

第十五版已经有客户端本地历史：

```text
:history
 -> 打印当前客户端进程里发送过的命令

!!
 -> 重复上一条发送过的命令

!N
 -> 重复第 N 条历史命令
```

但是这个历史只存在内存里。

也就是说：

```text
打开 minikv_client
 -> 输入几条命令
 -> :history 可以看到
 -> 退出客户端
 -> 再打开客户端
 -> 历史为空
```

第二十一版补的是跨会话能力：

```text
minikv_client --history-file data/client.history
```

有了这个参数后：

```text
第一次会话发送命令
 -> 写入 data/client.history

第二次会话启动
 -> 读取 data/client.history
 -> :history 能看到旧命令
 -> !! / !N 能重复旧命令
```

---

# 2. 本版核心链路

第二十一版核心链路是：

```text
parse_options
 -> 解析 --history-file path
 -> 保存到 ClientOptions::history_file

main
 -> 创建 ClientHistory
 -> 如果设置 history_file，调用 load_from_file
 -> 进入交互循环

用户输入
 -> resolve_client_input
 -> local action 只本地打印
 -> send action 发送到 server
 -> 收到 server 响应后 save_to_file
```

注意保存时机：

```text
发送成功
并且收到服务端响应
之后保存历史文件
```

这比“输入后立刻保存”更保守。

如果网络断开、发送失败、服务端没响应，这条命令不会被当成已经成功发送的历史写入文件。

---

# 3. client_history.hpp 新增 filesystem

头文件新增：

```cpp
#include <filesystem>
```

原因是新接口接收路径：

```cpp
std::filesystem::path
```

相比直接用 `std::string`，`std::filesystem::path` 更适合表达文件路径。

它可以承载：

```text
Windows 路径
Linux / macOS 路径
相对路径
绝对路径
带父目录的路径
```

---

# 4. ClientHistory 新增两个接口

类里新增：

```cpp
std::size_t load_from_file(const std::filesystem::path& path);
void save_to_file(const std::filesystem::path& path) const;
```

两个接口角色很清楚。

`load_from_file`：

```text
从历史文件读取命令
把命令加入 entries_
返回读取到的命令数量
```

`save_to_file`：

```text
把当前 entries_ 写入历史文件
每条命令一行
```

这两个接口放在 `ClientHistory` 里，而不是直接写在 `client_main.cpp` 里，是因为：

```text
历史容量裁剪规则属于 ClientHistory
文件加载后也应该走同一套 add() 规则
测试可以直接测历史模块，不依赖真实 socket
```

---

# 5. 为什么 load_from_file 返回 size_t

`load_from_file` 返回：

```cpp
std::size_t
```

表示这次从文件中读取到了多少条非空历史命令。

客户端启动时用它打印：

```text
history file: path (3 entries available, 3 loaded)
```

这里有两个数字：

```text
loaded
 -> 文件里读取到多少条非空命令

history.size()
 -> 当前可用历史有多少条
```

这两个数字可能不同。

比如历史容量是 100，文件里 150 条：

```text
loaded = 150
history.size() = 100
```

因为 `ClientHistory::add()` 会裁剪，只保留最近的 bounded history。

---

# 6. client_history.cpp 新增头文件

实现文件新增：

```cpp
#include <filesystem>
#include <fstream>
#include <system_error>
```

逐个看。

`<filesystem>`：

```text
检查文件是否存在
创建父目录
处理 path
```

`<fstream>`：

```text
std::ifstream 读历史文件
std::ofstream 写历史文件
```

`<system_error>`：

```text
std::error_code
```

这里使用 `std::error_code` 是为了让文件存在性检查和目录创建更可控。

---

# 7. load_from_file 的第一步：检查文件是否存在

代码是：

```cpp
std::error_code error;
if (!std::filesystem::exists(path, error)) {
    if (error) {
        throw std::runtime_error{"failed to check history file: " + error.message()};
    }
    return 0;
}
```

意思是：

```text
先看历史文件是否存在
```

如果不存在，而且没有系统错误：

```text
返回 0
```

这很重要。

第一次使用：

```text
--history-file data/client.history
```

文件大概率还没有创建。

这不应该算失败。

所以缺失历史文件的语义是：

```text
历史为空
可以继续运行
```

---

# 8. 为什么不用直接 ifstream 打开判断

也可以直接：

```cpp
std::ifstream input{path};
if (!input) ...
```

但这样会把两种情况混在一起：

```text
文件不存在
文件存在但没有权限
路径有问题
```

第二十一版选择先 `exists`：

```text
不存在
 -> 正常返回 0

检查存在性本身出错
 -> 抛异常

存在但打不开
 -> 后面抛异常
```

这样错误语义更清楚。

---

# 9. 打开历史文件

文件存在后：

```cpp
std::ifstream input{path};
if (!input) {
    throw std::runtime_error{"failed to open history file for reading: " + path.string()};
}
```

如果文件存在但打不开，说明不是正常的“第一次使用”。

可能是：

```text
权限不够
路径指向目录
文件被占用
```

这时客户端应该报错，让用户知道历史文件不可用。

---

# 10. 逐行读取

读取逻辑：

```cpp
std::size_t loaded = 0;
std::string line;
while (std::getline(input, line)) {
    ...
}
```

历史文件格式很简单：

```text
一行一条命令
```

比如：

```text
PING first
SET name mini-kv
GET name
QUIT
```

不引入 JSON、CSV 或转义格式，是因为当前命令输入本来就是一行文本协议。

客户端每次发送给服务端的也是：

```text
resolved.command + "\n"
```

所以历史文件直接保存“发送过的一行命令”就够用。

---

# 11. 兼容 CRLF 行尾

读取后有：

```cpp
if (!line.empty() && line.back() == '\r') {
    line.pop_back();
}
```

这是为了兼容 Windows 文本行尾。

不同系统行尾可能是：

```text
Linux / macOS: \n
Windows: \r\n
```

`std::getline(input, line)` 默认以 `\n` 分隔。

如果文件里是 CRLF，有时读出来的 `line` 末尾可能带着：

```text
\r
```

如果不去掉，历史命令会变成：

```text
GET name\r
```

后面发送到服务端就会出现隐藏字符。

所以这里主动去掉末尾 `\r`。

---

# 12. 跳过空行

接着：

```cpp
if (line.empty()) {
    continue;
}
```

空行不加入历史。

这样历史文件里偶尔有空行，不会变成一条空命令。

这和交互输入里的逻辑一致：

```cpp
if (is_blank(line)) {
    continue;
}
```

空输入不是有效命令。

---

# 13. 使用 add 加入历史

关键是：

```cpp
add(std::move(line));
++loaded;
line.clear();
```

这里没有直接：

```cpp
entries_.push_back(...)
```

而是调用 `add()`。

原因是 `add()` 里已经有容量控制：

```cpp
entries_.push_back(std::move(command));
if (entries_.size() > max_entries_) {
    entries_.erase(entries_.begin());
}
```

所以从文件加载历史时，也自动遵守：

```text
最多保留 max_entries_ 条
超过就丢掉最旧的
```

这让内存历史和文件历史使用同一套规则。

---

# 14. 为什么 move 后还要 clear

读取循环里：

```cpp
add(std::move(line));
++loaded;
line.clear();
```

`std::move(line)` 后，`line` 仍然是一个有效字符串对象，但内容处于“被移动后”的状态。

下一轮 `std::getline(input, line)` 通常也能继续使用。

这里 `line.clear()` 是一个保守写法：

```text
确保下一轮读取前 line 是空字符串状态
```

这不影响行为，只让循环状态更明确。

---

# 15. 检查读取错误

循环结束后：

```cpp
if (input.bad()) {
    throw std::runtime_error{"failed while reading history file: " + path.string()};
}
```

`std::getline` 正常读到文件末尾时，流会进入 EOF 状态。

EOF 不是错误。

真正需要报错的是：

```text
底层读文件失败
磁盘/系统 I/O 错误
```

`input.bad()` 用来捕获这种更严重的读取错误。

---

# 16. save_to_file 的第一步：创建父目录

保存时先做：

```cpp
if (const auto parent = path.parent_path(); !parent.empty()) {
    std::error_code error;
    std::filesystem::create_directories(parent, error);
    if (error) {
        throw std::runtime_error{"failed to create history directory: " + error.message()};
    }
}
```

如果用户传：

```text
--history-file data/client.history
```

而 `data/` 目录还不存在，客户端会自动创建。

使用 `create_directories` 而不是 `create_directory` 的好处是：

```text
可以创建多级目录
父目录已存在也可以接受
```

比如：

```text
data/client/history.txt
```

可以一次性创建 `data/client`。

---

# 17. 为什么 parent 为空时跳过

如果用户传的是：

```text
--history-file client.history
```

这个 path 没有父目录。

`path.parent_path()` 为空。

这种情况下直接在当前工作目录写文件就可以，不需要创建目录。

所以代码写成：

```cpp
if (const auto parent = path.parent_path(); !parent.empty()) {
    ...
}
```

---

# 18. 打开输出文件

然后：

```cpp
std::ofstream output{path, std::ios::trunc};
if (!output) {
    throw std::runtime_error{"failed to open history file for writing: " + path.string()};
}
```

`std::ios::trunc` 表示：

```text
覆盖写入
```

每次保存都把当前 bounded history 写成完整文件。

这样文件里不会无限增长。

它始终反映：

```text
当前 ClientHistory entries_
```

而不是把每条命令一直追加到文件末尾。

---

# 19. 写入每条历史

写入逻辑：

```cpp
for (const auto& entry : entries_) {
    output << entry << '\n';
}
```

格式仍然是：

```text
一行一条命令
```

比如：

```text
PING first
SET smoke history
QUIT
```

这个格式简单、可人工查看、可手动编辑。

---

# 20. 检查写入错误

写完后：

```cpp
if (!output) {
    throw std::runtime_error{"failed while writing history file: " + path.string()};
}
```

打开成功不代表写入一定成功。

写入过程中也可能失败：

```text
磁盘满
权限变化
网络盘断开
```

所以最后再检查一次输出流状态。

---

# 21. client_main.cpp 新增 filesystem

客户端入口新增：

```cpp
#include <filesystem>
```

因为 `ClientOptions` 里新增：

```cpp
std::optional<std::filesystem::path> history_file;
```

这里用 `optional` 表示：

```text
用户可以不启用历史文件
```

默认不写任何文件。

这是一种保守选择。

用户明确传 `--history-file` 时，客户端才读写磁盘。

---

# 22. ClientOptions 新增 history_file

结构体变成：

```cpp
struct ClientOptions {
    std::string host = "127.0.0.1";
    std::uint16_t port = 6379;
    std::optional<std::chrono::milliseconds> timeout;
    int connect_retries = 0;
    std::chrono::milliseconds retry_delay{250};
    std::optional<std::filesystem::path> history_file;
};
```

前面几个字段是第十二版、第十四版加的：

```text
timeout
connect_retries
retry_delay
```

第二十一版在同一个 options 结构里加入：

```text
history_file
```

这样 `main()` 不需要到处传散乱参数，只要拿着 `ClientOptions` 就知道客户端启动配置。

---

# 23. print_usage 更新

用法提示从：

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
```

扩展为：

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms] [--history-file path]
```

这样用户写错参数时，能直接看到新选项。

---

# 24. parse_options 解析 --history-file

新增分支：

```cpp
if (argument == "--history-file") {
    if (++index >= argc) {
        throw UsageError{"missing value for --history-file"};
    }
    if (argv[index][0] == '\0') {
        throw UsageError{"history-file must not be empty"};
    }
    options.history_file = std::filesystem::path{argv[index]};
    continue;
}
```

这里分三步。

第一步：

```text
确认 --history-file 后面还有一个值
```

否则：

```text
missing value for --history-file
```

第二步：

```text
确认路径不是空字符串
```

否则：

```text
history-file must not be empty
```

第三步：

```text
保存到 options.history_file
```

后面 `main()` 会根据这个 optional 是否有值决定是否启用历史持久化。

---

# 25. 参数解析和位置参数的关系

`parse_options` 里已有逻辑是：

```text
非 - 开头
 -> 当作位置参数 host / port / timeout_ms

--connect-retries
 -> 选项参数

--retry-delay-ms
 -> 选项参数
```

第二十一版把 `--history-file` 加到同一套选项解析里。

所以这些写法都可以：

```text
minikv_client.exe 127.0.0.1 6379 --history-file data/client.history
minikv_client.exe --history-file data/client.history 127.0.0.1 6379
minikv_client.exe 127.0.0.1 6379 5000 --connect-retries 10 --history-file data/client.history
```

因为解析循环会依次识别选项和位置参数。

---

# 26. main 中加载历史

连接成功后：

```cpp
minikv::ClientHistory history;
if (options.history_file.has_value()) {
    const std::size_t loaded = history.load_from_file(*options.history_file);
    std::cout << "history file: " << options.history_file->string() << " (" << history.size()
              << " entries available, " << loaded << " loaded)\n";
}
```

注意顺序：

```text
先连接服务端
再加载历史文件
```

这样客户端仍然保持原来的启动节奏：

```text
连接失败
 -> 直接报连接错误

连接成功
 -> 显示 connected
 -> 再显示 history file 状态
```

这也避免在服务端不可用时还去处理历史文件。

---

# 27. 启动提示里的两个数字

提示是：

```text
history file: path (3 entries available, 3 loaded)
```

它来自：

```cpp
history.size()
loaded
```

如果历史文件不存在：

```text
(0 entries available, 0 loaded)
```

如果历史文件有 3 条并且容量够：

```text
(3 entries available, 3 loaded)
```

如果历史文件有 200 条，但当前 `ClientHistory` 容量是 100：

```text
(100 entries available, 200 loaded)
```

这能让用户知道：

```text
文件确实读了多少
当前实际可用多少
```

---

# 28. 交互循环保持原有结构

主循环仍然是：

```text
打印 prompt
读取 stdin
空行跳过
resolve_client_input
打印本地消息
local action 继续下一轮
send action 发送到 server
读取并打印 server 响应
必要时保存历史文件
QUIT / EXIT 退出
```

第二十一版只在其中插入了保存历史文件：

```cpp
if (options.history_file.has_value()) {
    history.save_to_file(*options.history_file);
}
```

原有命令解析、发送、响应读取逻辑不变。

---

# 29. 为什么 local action 不保存

本地命令包括：

```text
:history
```

以及错误的历史引用：

```text
!! 但历史为空
!99 但找不到第 99 条
```

这些 `ClientInputAction::local` 不会发送到服务端。

第二十一版保存点在：

```text
send action
收到 server 响应之后
```

所以 local action 不会进入历史文件。

这和第十五版设计一致：

```text
历史记录的是发给服务端的命令
不是客户端本地操作
```

---

# 30. QUIT 为什么会保存

`QUIT` / `EXIT` 是 send action。

执行顺序是：

```text
resolve_client_input
 -> history.add("QUIT")
send QUIT
print BYE
save_to_file
检测 first_token == QUIT
break
```

所以历史文件会包含：

```text
QUIT
```

这不是 bug。

因为 `QUIT` 确实是发送给服务端的命令。

真实 smoke 里第一次会话后，第二次启动显示：

```text
3 entries available, 3 loaded
```

三条就是：

```text
PING first
SET smoke history
QUIT
```

---

# 31. 和第十五版 resolve_client_input 的关系

第二十一版没有改 `resolve_client_input` 的核心语义。

原来的行为仍然成立：

```text
普通命令
 -> history.add(command)
 -> send

:history
 -> local
 -> 不 add

!!
 -> 找 last
 -> add 被重复的命令
 -> send

!N
 -> 找第 N 条
 -> add 被重复的命令
 -> send
```

新增文件能力只是让 `history` 对象在创建后先拥有旧记录。

所以：

```text
!2
```

可以引用上一轮客户端会话保存下来的第 2 条命令。

---

# 32. 历史容量仍然 bounded

`ClientHistory` 默认容量还是：

```cpp
ClientHistory history;
```

构造函数默认：

```cpp
explicit ClientHistory(std::size_t max_entries = 100);
```

这意味着持久化历史文件也不是无限保留。

每次保存会写当前 `entries_`。

而 `entries_` 最大 100 条。

所以长期使用时：

```text
历史文件最多保存最近 100 条发送命令
```

这避免历史文件无限增长。

---

# 33. 为什么没有默认 history 文件

第二十一版选择：

```text
只有显式 --history-file 才启用持久化
```

没有默认写：

```text
~/.minikv_history
```

原因是当前项目仍然是练习项目，运行环境可能是：

```text
Windows PowerShell
CLion
GitHub Actions
临时测试目录
课堂/练习目录
```

默认写用户目录会引入额外问题：

```text
跨平台 home 目录规则
测试污染用户机器
用户没意识到程序写文件
```

显式参数更清楚：

```text
你指定哪个文件
客户端就读写哪个文件
```

---

# 34. 文件格式的优点和限制

优点：

```text
简单
可读
可手动编辑
不需要第三方库
和一行命令模型一致
```

限制：

```text
不能保存包含真实换行符的命令
没有时间戳
没有命令执行结果
没有去重
没有加锁
```

这些限制在当前 mini-kv 客户端里可以接受。

因为客户端发送协议本身就是：

```text
一行一条命令
```

---

# 35. 当前错误处理策略

读历史文件时：

```text
文件不存在
 -> 返回 0，继续运行

检查文件状态失败
 -> 抛 runtime_error

文件存在但打不开
 -> 抛 runtime_error

读取过程 bad
 -> 抛 runtime_error
```

写历史文件时：

```text
父目录创建失败
 -> 抛 runtime_error

文件打不开
 -> 抛 runtime_error

写入失败
 -> 抛 runtime_error
```

这些异常会被 `main()` 最外层捕获：

```cpp
} catch (const std::exception& error) {
    std::cerr << "fatal: " << error.what() << '\n';
    return 1;
}
```

所以用户会看到：

```text
fatal: failed to open history file for writing: ...
```

---

# 36. 真实使用示例

启动服务端：

```powershell
.\cmake-build-debug\minikv_server.exe 6379
```

第一次客户端：

```powershell
.\cmake-build-debug\minikv_client.exe 127.0.0.1 6379 --history-file data\client.history
```

输入：

```text
PING first
SET smoke history
QUIT
```

历史文件会保存：

```text
PING first
SET smoke history
QUIT
```

第二次客户端还是用同一个文件：

```powershell
.\cmake-build-debug\minikv_client.exe 127.0.0.1 6379 --history-file data\client.history
```

输入：

```text
:history
!2
GET smoke
QUIT
```

其中：

```text
:history
 -> 显示旧会话保存的命令

!2
 -> 重复 SET smoke history

GET smoke
 -> 读到 history
```

---

# 37. 第二十一版没有改变服务端

这版没有修改：

```text
src/server_main.cpp
src/tcp_server.cpp
include/minikv/tcp_server.hpp
```

原因是持久化历史属于：

```text
客户端本地体验
```

服务端并不关心命令来自：

```text
用户新输入
!! 重复上一条
!N 重复历史
历史文件加载出来的旧命令
```

最终发给服务端的都是一行命令文本。

所以服务端协议和响应不需要变。

---

# 38. 第二十一版核心增删改

新增接口：

```text
ClientHistory::load_from_file
ClientHistory::save_to_file
```

新增客户端参数：

```text
--history-file path
```

新增行为：

```text
启动时加载历史文件
发送命令成功后保存历史文件
历史文件按 bounded history 覆盖写入
```

保持不变：

```text
:history / !! / !N 的语义
TCP 协议
服务端行为
默认无 history 文件时的客户端行为
```

---

# 39. 第二十一版的价值

这版提升的是日常使用体验。

以前每次重新打开客户端都要重新输入：

```text
SET name mini-kv
GET name
TTL name
```

现在可以：

```text
:history
!3
!!
```

复用旧会话里常用的命令。

同时它还是显式开启的：

```text
--history-file path
```

不会在用户不知道的地方写文件。

---

# 第四十三次总结

第二十一版客户端持久化历史的核心是：

```text
ClientHistory
 -> 新增 load_from_file / save_to_file
 -> 复用 add() 做容量裁剪
 -> 一行一条命令保存 bounded history

client_main
 -> 新增 --history-file 参数
 -> 启动时加载
 -> 每次成功发送并收到响应后保存
```

一句话总结：

```text
第二十一版把客户端历史从“当前进程内可用”推进到“可选文件持久化、跨会话可复用”，同时保持原有 :history / !! / !N 语义和服务端协议不变。
```
