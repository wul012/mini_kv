# 第三十二次讲解：第十五版客户端本地历史核心

第三十一次讲了第十四版 README、CMake、`a/14` 归档和整体增删改。

这次进入第十五版核心代码：

```text
include/minikv/client_history.hpp
src/client_history.cpp
src/client_main.cpp
```

第十五版主要做三件事：

```text
第一，把客户端本地历史抽成 ClientHistory 模块
第二，让 minikv_client 支持 :history / !! / !N
第三，保证本地历史命令不会误发给服务端
```

第十四版让客户端能等待服务端上线。
第十五版继续补客户端体验：

```text
用户可以查看本次会话发过什么
用户可以重复上一条命令
用户可以重复指定编号的历史命令
```

---

# 1. 为什么历史命令要做成本地能力

`:history`、`!!`、`!N` 不是 mini-kv 服务端命令。

服务端认识的是：

```text
PING
SET
GET
DEL
EXPIRE
TTL
SIZE
SAVE
LOAD
HELP
QUIT
```

如果把 `:history` 直接发给服务端，服务端只会返回：

```text
ERR unknown command
```

所以第十五版的设计是：

```text
客户端先解析用户输入
如果是本地历史命令，就在客户端处理
如果解析出真实服务端命令，再发送给 server
```

这条边界很重要：

```text
客户端体验命令
 -> 客户端处理

KV 数据命令
 -> 服务端处理
```

---

# 2. client_history.hpp 的角色

第十五版新增头文件：

```text
include/minikv/client_history.hpp
```

它定义客户端历史模块的公开接口。

这个模块不依赖 socket，也不依赖 `CommandProcessor`。
它只关心一件事：

```text
给定一行客户端输入，判断它是本地命令还是要发送给服务端的命令
```

这样做的好处是：

```text
历史逻辑可以单独测试
client_main 只负责网络交互
```

---

# 3. ClientInputAction

头文件里先定义：

```cpp
enum class ClientInputAction {
    send,
    local
};
```

它有两个值：

```text
send
 -> 这行输入最终要发送给服务端

local
 -> 这行输入只在客户端本地处理
```

比如：

```text
PING first
 -> send

:history
 -> local
```

这能让 `client_main` 不需要猜：

```text
到底该不该 send_all
```

它只看 `action` 就行。

---

# 4. ClientInputResult

第十五版新增：

```cpp
struct ClientInputResult {
    ClientInputAction action = ClientInputAction::local;
    std::string command;
    std::vector<std::string> messages;
};
```

三个字段分别表示：

```text
action
 -> 本地处理还是发送给服务端

command
 -> 真正要发送给服务端的命令

messages
 -> 客户端需要先打印的本地提示
```

例如用户输入：

```text
!!
```

如果上一条命令是：

```text
PING first
```

解析结果就是：

```text
action = send
command = PING first
messages = repeating: PING first
```

客户端先打印提示，再把 `PING first` 发给服务端。

---

# 5. ClientHistory 类

第十五版核心类是：

```cpp
class ClientHistory
```

它保存本次客户端会话里已经发送过的命令。

构造函数：

```cpp
explicit ClientHistory(std::size_t max_entries = 100);
```

默认最多保存：

```text
100 条
```

内部字段：

```cpp
std::size_t max_entries_;
std::vector<std::string> entries_;
```

这里的历史是：

```text
本次 minikv_client 进程内的内存历史
```

不是持久化历史。
程序退出后历史就没有了。

---

# 6. 为什么 max_entries 不能是 0

构造函数里有：

```cpp
if (max_entries_ == 0) {
    throw std::invalid_argument{"client history max_entries must be greater than zero"};
}
```

原因是：

```text
容量为 0 的历史没有实际意义
```

如果允许 0，后面 `add` 的裁剪逻辑会变得奇怪。

测试里也覆盖了：

```cpp
minikv::ClientHistory invalid{0};
```

它必须抛 `std::invalid_argument`。

---

# 7. add：加入历史并裁剪

`add` 的逻辑是：

```cpp
void ClientHistory::add(std::string command) {
    if (command.empty()) {
        return;
    }

    entries_.push_back(std::move(command));
    if (entries_.size() > max_entries_) {
        entries_.erase(entries_.begin());
    }
}
```

它做三件事：

```text
空命令不记录
非空命令追加到末尾
超过容量就删除最旧的一条
```

所以历史始终是：

```text
按时间顺序保存最近 max_entries 条已发送命令
```

第十五版测试里用容量 3 验证了裁剪行为。

---

# 8. last 和 get

`last` 返回上一条命令：

```cpp
std::optional<std::string> ClientHistory::last() const
```

如果历史为空：

```text
std::nullopt
```

`get` 返回指定编号的命令：

```cpp
std::optional<std::string> ClientHistory::get(std::size_t one_based_index) const
```

这里是：

```text
1-based index
```

也就是说：

```text
!1
 -> 第一条历史

!2
 -> 第二条历史
```

如果编号为 0 或超过历史长度：

```text
std::nullopt
```

---

# 9. trim_copy

`src/client_history.cpp` 里有一个辅助函数：

```cpp
std::string trim_copy(std::string_view text)
```

它用于判断本地命令时去掉首尾空白。

比如：

```text
  :history
```

也应该识别成：

```text
:history
```

但注意：

```text
普通服务端命令最终发送时保留原始 input
```

也就是说，trim 主要用于识别本地控制命令，不强行改写普通命令。

---

# 10. all_digits 和 parse_history_index

`!N` 需要解析数字编号。

第十五版先用：

```cpp
bool all_digits(std::string_view text)
```

判断：

```text
N 是否全部是数字
```

再用：

```cpp
std::optional<std::size_t> parse_history_index(std::string_view text)
```

转换成 `std::size_t`。

它还处理两个边界：

```text
数字溢出
 -> std::nullopt

结果为 0
 -> std::nullopt
```

所以：

```text
!0
超大数字
```

都不会被当成有效历史编号。

---

# 11. local_message

本地提示通过：

```cpp
ClientInputResult local_message(std::string message)
```

生成。

比如空历史执行：

```text
:history
```

会返回：

```text
action = local
messages = history is empty
```

客户端打印消息后，不会发送任何内容给服务端。

---

# 12. send_command

普通命令和重复命令最终都走：

```cpp
ClientInputResult send_command(ClientHistory& history,
                               std::string command,
                               std::vector<std::string> messages = {})
```

它先做：

```cpp
history.add(command);
```

然后返回：

```text
action = send
command = command
messages = messages
```

这说明：

```text
只要最后要发给服务端，就会进入历史
```

所以 `!!` 重复出来的命令也会再次进入历史。

---

# 13. resolve_client_input：空输入

入口函数是：

```cpp
ClientInputResult resolve_client_input(ClientHistory& history, std::string_view input)
```

第一步：

```cpp
const std::string trimmed = trim_copy(input);
if (trimmed.empty()) {
    return {};
}
```

空输入返回默认结果：

```text
action = local
command = ""
messages = []
```

在 `client_main` 中，空白输入已经提前被 `is_blank` 跳过。
这里再处理一次，是为了让这个模块本身更稳。

---

# 14. resolve_client_input：:history

如果输入是：

```text
:history
```

且历史为空：

```text
history is empty
```

如果历史不为空，就生成：

```text
1  PING first
2  SET name mini-kv
```

注意这里是：

```text
local action
```

它只打印历史，不发给服务端。

---

# 15. resolve_client_input：!!

如果输入是：

```text
!!
```

且历史为空：

```text
history is empty
```

如果历史里上一条是：

```text
PING first
```

就返回：

```text
messages = repeating: PING first
command = PING first
action = send
```

所以 `!!` 本身不会发给服务端。
真正发给服务端的是上一条命令。

---

# 16. resolve_client_input：!N

如果输入形如：

```text
!1
!2
!99
```

第十五版会把 `!` 后面的数字当成历史编号。

找到历史：

```text
repeating: <command>
然后发送 <command>
```

找不到历史：

```text
history entry not found: N
```

比如：

```text
!99
```

在历史不足 99 条时就是本地错误提示，不会发送给服务端。

---

# 17. !abc 为什么是普通命令

代码里只有这个条件会识别成历史编号：

```cpp
trimmed.size() > 1 && trimmed.front() == '!' && all_digits(...)
```

所以：

```text
!abc
```

不会被当成历史命令。
它会走最后的普通命令路径：

```cpp
return send_command(history, std::string{input});
```

这给未来扩展保留空间，也避免把所有 `!` 开头输入都拦截掉。

---

# 18. client_main 接入历史模块

第十五版在 `src/client_main.cpp` 顶部新增：

```cpp
#include "minikv/client_history.hpp"
```

连接成功后新增：

```cpp
minikv::ClientHistory history;
```

每次读入一行后：

```cpp
const auto resolved = minikv::resolve_client_input(history, line);
```

然后先打印本地消息：

```cpp
for (const auto& message : resolved.messages) {
    std::cout << message << '\n';
}
```

如果是本地命令：

```cpp
if (resolved.action == minikv::ClientInputAction::local) {
    continue;
}
```

也就是不会发给服务端。

---

# 19. client_main 发送 resolved.command

第十四版以前发送的是：

```cpp
send_all(socket.get(), line + "\n")
```

第十五版改成：

```cpp
send_all(socket.get(), resolved.command + "\n")
```

响应处理也改成：

```cpp
print_response(socket.get(), resolved.command)
```

关闭判断也改成：

```cpp
const std::string command = first_token(resolved.command);
```

这样 `!!` 或 `!N` 重复出 `QUIT` 时，也能正确关闭客户端。

---

# 20. 第十五版历史链路总结

整体链路是：

```text
用户输入一行
 -> client_main 读取 line
 -> resolve_client_input(history, line)
 -> 本地消息先打印
 -> local action 直接 continue
 -> send action 把 resolved.command 发送给 server
 -> server 返回响应
 -> first_token(resolved.command) 判断是否 QUIT / EXIT
```

本地历史命令链路：

```text
:history
 -> local
 -> 打印历史
 -> 不发送给 server

!!
 -> 找上一条历史
 -> 打印 repeating
 -> 发送上一条真实命令

!N
 -> 找第 N 条历史
 -> 打印 repeating
 -> 发送对应真实命令
```

---

# 第三十二次总结

第十五版把客户端历史从交互循环里抽成了独立模块。

一句话总结：

```text
第十五版通过 ClientHistory 和 resolve_client_input，把 :history / !! / !N 做成客户端本地会话能力，让本地控制命令不污染服务端协议，同时重复命令仍能走真实 TCP 执行链路。
```
