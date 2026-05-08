# 第二次讲解：`src/main.cpp`

这个文件是项目的 **CLI 命令行入口**。

它的作用不是实现数据库，也不是实现命令解析，而是负责：

```text
启动一个交互式终端
 -> 等用户输入命令
 -> 把命令交给 CommandProcessor
 -> 把结果打印到屏幕
 -> 如果用户输入 EXIT / QUIT，就退出程序
```

可以把它理解成：

```text
main.cpp = 命令行外壳
```

第一次讲过：

```text
Store 是仓库
CommandProcessor 是命令翻译器
```

那么 `main.cpp` 就是：

```text
用户和 CommandProcessor 之间的交互窗口
```

---

# 1. 引入命令处理器

文件开头：

```cpp
#include "minikv/command.hpp"
```

这里引入 `command.hpp`。

因为 `main.cpp` 需要用到两个东西：

```cpp
minikv::Store
minikv::CommandProcessor
```

虽然 `Store` 定义在 `store.hpp`，但是 `command.hpp` 里面已经包含了：

```cpp
#include "minikv/store.hpp"
```

所以这里只需要包含 `command.hpp` 就够了。

---

再看标准库头文件：

```cpp
#include <iostream>
#include <string>
```

`iostream` 用来做输入输出。

比如：

```cpp
std::cout
std::cin
```

`string` 用来保存用户输入的一整行命令。

---

# 2. 程序入口

```cpp
int main() {
```

这是 C++ 程序的入口。

当你运行：

```powershell
.\build\Debug\minikv_cli.exe
```

或者 CLion 运行 `minikv_cli` 目标时，程序就从这里开始。

---

# 3. 创建 Store

```cpp
minikv::Store store;
```

这里创建了一个内存数据库对象。

也就是说，CLI 启动后，内存里会有一个空的 key-value 仓库。

初始状态类似：

```text
{}
```

比如后面输入：

```text
SET name mini-kv
```

数据就会变成：

```text
{
  name: mini-kv
}
```

注意，这个 `store` 是局部变量。

所以它的生命周期是：

```text
main 函数开始
 -> store 创建
 -> 用户不断操作
 -> main 函数结束
 -> store 销毁
```

也就是说，CLI 退出后，所有数据都会消失。

这个项目当前版本是内存数据库，没有持久化文件。

---

# 4. 创建 CommandProcessor

```cpp
minikv::CommandProcessor processor{store};
```

这里创建命令处理器，并把刚才的 `store` 传进去。

这个关系很重要：

```text
CommandProcessor 不自己保存数据
它只是持有 Store 的引用
```

也就是说：

```text
用户输入命令
 -> processor.execute(...)
 -> processor 调用 store.set/get/erase/size
```

可以理解成：

```text
store 是仓库
processor 是仓库管理员
main.cpp 是柜台
```

---

# 5. 打印欢迎信息

```cpp
std::cout << "mini-kv CLI\n";
```

启动时先打印：

```text
mini-kv CLI
```

告诉用户现在进入了 mini-kv 命令行。

然后打印帮助文本：

```cpp
std::cout << minikv::CommandProcessor::help_text() << '\n';
```

`help_text()` 是 `CommandProcessor` 的静态函数。

它返回：

```text
Commands:
  SET key value
  GET key
  DEL key
  SIZE
  HELP
  EXIT
```

这里不需要创建额外对象，因为它是 `static` 函数，可以直接通过类名调用：

```cpp
CommandProcessor::help_text()
```

所以 CLI 一启动，用户马上知道可以输入哪些命令。

---

# 6. 准备保存用户输入

```cpp
std::string line;
```

这里定义一个字符串变量 `line`。

它用来保存用户每次输入的一整行内容。

比如用户输入：

```text
SET name mini-kv
```

那么 `line` 里就是：

```text
"SET name mini-kv"
```

---

# 7. 进入无限循环

```cpp
while (true) {
```

CLI 程序需要一直等待用户输入，所以这里使用无限循环。

整体结构是：

```text
打印提示符
 -> 读取一行命令
 -> 执行命令
 -> 打印结果
 -> 判断是否退出
 -> 回到开头继续
```

---

# 8. 打印提示符

```cpp
std::cout << "mini-kv> ";
```

每次循环先打印：

```text
mini-kv>
```

这表示程序正在等待输入。

---

# 9. 读取用户输入

```cpp
if (!std::getline(std::cin, line)) {
```

这里使用 `std::getline` 从标准输入读取一整行。

为什么不用：

```cpp
std::cin >> line
```

因为 `std::cin >> line` 只能读取一个单词。

比如：

```text
SET name mini-kv
```

如果用 `>>`，只能读到：

```text
SET
```

后面的 `name mini-kv` 会被留在输入流里。

但是 `std::getline` 会读取完整一行：

```text
SET name mini-kv
```

这正好适合命令行程序。

这里的判断：

```cpp
if (!std::getline(std::cin, line)) {
```

意思是：

```text
如果读取失败，就退出循环
```

读取失败可能有几种情况：

```text
用户按 Ctrl+Z / Ctrl+D
输入流关闭
程序被外部管道结束
```

在 Windows 终端里，常见是 `Ctrl+Z` 后回车表示输入结束。

如果读取失败：

```cpp
std::cout << '\n';
break;
```

先输出一个换行，然后跳出循环。

---

# 10. 执行命令

```cpp
const auto result = processor.execute(line);
```

这是 `main.cpp` 最核心的一行。

它把用户输入的一整行交给 `CommandProcessor`。

比如：

```text
line = "SET name mini-kv"
```

执行：

```cpp
processor.execute(line)
```

内部会走第一次讲过的逻辑：

```text
去掉前后空格
 -> 读取命令名 SET
 -> 读取 key = name
 -> 读取 value = mini-kv
 -> 调用 store.set("name", "mini-kv")
 -> 返回 OK inserted
```

返回值是：

```cpp
CommandResult
```

它里面有两个字段：

```text
response
should_close
```

比如 `SET name mini-kv` 返回：

```text
response = "OK inserted"
should_close = false
```

而 `EXIT` 返回：

```text
response = "BYE"
should_close = true
```

---

# 11. 打印执行结果

```cpp
if (!result.response.empty()) {
    std::cout << result.response << '\n';
}
```

如果命令有返回内容，就打印出来。

为什么要判断 `empty()`？

因为空行命令会返回空结果。

如果用户只是按回车，程序不会打印任何错误，只会继续等待下一条命令。

举几个例子：

```text
输入 SET name mini-kv
输出 OK inserted

输入 GET name
输出 mini-kv

输入 GET age
输出 (nil)

输入 UNKNOWN
输出 ERR unknown command
```

---

# 12. 判断是否退出

```cpp
if (result.should_close) {
    break;
}
```

如果命令处理结果告诉 CLI 应该关闭，就跳出循环。

比如用户输入：

```text
EXIT
```

`CommandProcessor` 返回：

```cpp
return {"BYE", true};
```

这里 `should_close` 是 `true`。

所以 `main.cpp` 会执行：

```cpp
break;
```

跳出 `while (true)`。

---

# 13. 程序正常结束

```cpp
return 0;
```

`return 0` 表示程序正常结束。

对操作系统来说：

```text
0 = 成功
非 0 = 出错
```

所以 CLI 正常退出时返回 0。

---

# 14. CLI 一次完整运行示例

假设启动程序后：

```text
mini-kv CLI
Commands:
  SET key value
  GET key
  DEL key
  SIZE
  HELP
  EXIT
mini-kv>
```

用户输入：

```text
SET name mini-kv
```

内部流程：

```text
main.cpp 读取整行
 -> line = "SET name mini-kv"
 -> processor.execute(line)
 -> command.cpp 调用 store.set("name", "mini-kv")
 -> 返回 "OK inserted"
 -> main.cpp 打印 OK inserted
```

然后用户输入：

```text
GET name
```

内部流程：

```text
main.cpp 读取整行
 -> line = "GET name"
 -> processor.execute(line)
 -> command.cpp 调用 store.get("name")
 -> 返回 "mini-kv"
 -> main.cpp 打印 mini-kv
```

最后用户输入：

```text
EXIT
```

内部流程：

```text
main.cpp 读取整行
 -> line = "EXIT"
 -> processor.execute(line)
 -> 返回 response = "BYE", should_close = true
 -> main.cpp 打印 BYE
 -> main.cpp break
 -> return 0
```

---

# 第二次总结

`main.cpp` 是 mini-kv 的命令行入口。

它本身不关心 `SET` 怎么保存，也不关心 `GET` 怎么查询。

它只负责：

```text
创建 Store
创建 CommandProcessor
循环读取用户输入
调用 processor.execute()
打印结果
根据 should_close 决定是否退出
```

一句话总结：

```text
main.cpp 是 mini-kv 的命令行外壳，它把用户输入的一行文本交给 CommandProcessor，再把结果显示给用户。
```
