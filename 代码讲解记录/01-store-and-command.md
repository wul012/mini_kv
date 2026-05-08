# 第一次讲解：Store 和 CommandProcessor

第一次重点讲解 4 个文件：

```text
include/minikv/store.hpp
src/store.cpp
include/minikv/command.hpp
src/command.cpp
```

它们是项目最核心的两层：

```text
Store
 -> 负责保存数据

CommandProcessor
 -> 负责解析命令并调用 Store
```

核心流程是：

```text
用户输入一行命令
 -> CommandProcessor::execute(line)
 -> 解析出命令名、key、value
 -> 根据命令调用 Store
 -> Store 修改或读取 unordered_map
 -> CommandProcessor 返回文本结果
```

比如输入：

```text
SET name mini-kv
```

内部流程是：

```text
SET name mini-kv
 -> command.cpp 识别出 SET
 -> key = name
 -> value = mini-kv
 -> 调用 store_.set("name", "mini-kv")
 -> Store 把 name -> mini-kv 存进 unordered_map
 -> 返回 OK inserted
```

---

# 1. `store.hpp`：声明内存数据库

先看类定义：

```cpp
class Store {
```

`Store` 就是项目里的内存数据库。

它不是负责解析命令的，也不是负责网络通信的。它只负责一件事：

```text
保存 key-value 数据
```

可以把它理解成：

```text
Store = 一个带锁的 unordered_map
```

## 对外提供的方法

```cpp
bool set(std::string_view key, std::string_view value);
```

这个函数负责写入数据。

如果 key 原来不存在，它会新增。

如果 key 已经存在，它会更新。

返回值表示：

```text
true  = 新插入
false = 更新已有 key
```

---

```cpp
std::optional<std::string> get(std::string_view key) const;
```

这个函数负责读取数据。

为什么返回 `std::optional<std::string>`？

因为 key 可能不存在。

所以结果有两种情况：

```text
找到了 -> optional 里面有 string
没找到 -> std::nullopt
```

这比直接返回空字符串更清楚，因为空字符串也可能是合法 value。

---

```cpp
bool erase(std::string_view key);
```

这个函数负责删除 key。

返回值表示：

```text
true  = 删除成功，key 原来存在
false = 删除失败，key 原来不存在
```

---

```cpp
bool contains(std::string_view key) const;
```

这个函数负责判断 key 是否存在。

---

```cpp
std::size_t size() const;
```

这个函数返回当前数据库里有多少个 key。

---

```cpp
void clear();
```

这个函数清空所有数据。

---

```cpp
std::vector<std::pair<std::string, std::string>> snapshot() const;
```

这个函数返回当前所有 key-value 的快照。

它不是返回内部的 `unordered_map`，而是复制一份出来。

这样外部代码不会直接破坏内部数据结构。

## 私有成员

```cpp
mutable std::mutex mutex_;
```

这是互斥锁。

因为 TCP 服务端可能会同时服务多个客户端，每个客户端一个线程。

如果多个线程同时访问 `Store`，就可能出现数据竞争。

所以每次读写数据前都要加锁。

这里用了 `mutable`，是因为一些函数是 `const`，比如：

```cpp
get(...)
size(...)
snapshot(...)
```

这些函数逻辑上不修改数据库内容，但它们需要给 mutex 加锁。

如果没有 `mutable`，`const` 函数里不能修改 `mutex_` 的状态。

---

```cpp
std::unordered_map<std::string, std::string> data_;
```

这是真正保存数据的容器。

结构类似：

```text
name -> mini-kv
lang -> cpp20
```

一句话总结：`store.hpp` 定义了一个线程安全内存 KV 仓库的接口。

---

# 2. `store.cpp`：实现内存数据库

先看 `set`：

```cpp
bool Store::set(std::string_view key, std::string_view value) {
```

这是写入函数。

第一步检查 key：

```cpp
if (key.empty()) {
    return false;
}
```

空 key 不允许写入。

然后加锁：

```cpp
std::lock_guard lock(mutex_);
```

`std::lock_guard` 是 C++ 的自动锁。

可以理解成：

```text
进入作用域时自动加锁
离开作用域时自动解锁
```

这样即使中间发生返回，也不会忘记解锁。

核心写入逻辑：

```cpp
const auto [it, inserted] = data_.insert_or_assign(std::string{key}, std::string{value});
```

`insert_or_assign` 的意思是：

```text
如果 key 不存在：插入
如果 key 已存在：覆盖 value
```

它返回的 `inserted` 表示是不是新插入。

所以：

```cpp
return inserted;
```

如果第一次执行：

```text
SET name mini-kv
```

返回 `true`。

如果第二次执行：

```text
SET name redis-ish
```

返回 `false`。

---

再看 `get`：

```cpp
std::optional<std::string> Store::get(std::string_view key) const {
```

先加锁：

```cpp
std::lock_guard lock(mutex_);
```

然后查找：

```cpp
const auto it = data_.find(std::string{key});
```

如果没找到：

```cpp
if (it == data_.end()) {
    return std::nullopt;
}
```

如果找到了：

```cpp
return it->second;
```

这里返回的是 value 的复制。

这意味着外部拿到的是一份字符串副本，不会直接引用内部数据。

---

再看 `erase`：

```cpp
bool Store::erase(std::string_view key) {
```

逻辑很短：

```cpp
std::lock_guard lock(mutex_);
return data_.erase(std::string{key}) > 0;
```

`unordered_map::erase` 会返回删除了几个元素。

因为 key 唯一，所以结果只可能是：

```text
0 = 没删到
1 = 删除成功
```

---

再看 `contains`：

```cpp
bool Store::contains(std::string_view key) const {
```

它只判断 key 是否存在：

```cpp
return data_.find(std::string{key}) != data_.end();
```

---

再看 `size`：

```cpp
std::size_t Store::size() const {
```

它返回当前 key 数量：

```cpp
return data_.size();
```

---

再看 `clear`：

```cpp
void Store::clear() {
```

它清空所有数据：

```cpp
data_.clear();
```

---

最后看 `snapshot`：

```cpp
std::vector<std::pair<std::string, std::string>> Store::snapshot() const {
```

它先复制所有数据：

```cpp
for (const auto& [key, value] : data_) {
    items.emplace_back(key, value);
}
```

然后排序：

```cpp
std::ranges::sort(items, {}, &std::pair<std::string, std::string>::first);
```

为什么要排序？

因为 `unordered_map` 本身是无序的。

如果直接返回，顺序可能不稳定。

排序后，测试和展示结果更稳定。

一句话总结：`store.cpp` 实现了一个加锁保护的内存 key-value 数据库。

---

# 3. `command.hpp`：声明命令处理器

先看返回结果：

```cpp
struct CommandResult {
    std::string response;
    bool should_close = false;
};
```

每执行一条命令，都会返回一个 `CommandResult`。

它有两个字段。

第一个是：

```cpp
std::string response;
```

这是要返回给用户的文本。

比如：

```text
OK inserted
mini-kv
(nil)
ERR unknown command
```

第二个是：

```cpp
bool should_close = false;
```

它表示执行完这条命令后，连接或 CLI 是否应该退出。

比如：

```text
GET name
```

不需要退出，所以 `should_close = false`。

但是：

```text
QUIT
```

需要退出，所以 `should_close = true`。

---

再看类：

```cpp
class CommandProcessor {
```

它是命令处理器。

可以理解成：

```text
CommandProcessor = 命令翻译器
```

它把用户输入的字符串翻译成对 `Store` 的调用。

构造函数：

```cpp
explicit CommandProcessor(Store& store);
```

创建命令处理器时，需要传入一个 `Store`。

也就是说，`CommandProcessor` 自己不保存数据，它只是操作别人传进来的数据库。

执行函数：

```cpp
CommandResult execute(std::string_view line);
```

这是最重要的函数。

它接收一整行命令，比如：

```text
SET name mini-kv
```

然后返回执行结果。

帮助文本：

```cpp
static std::string help_text();
```

这个函数返回支持的命令列表。

它是 `static`，说明调用它不需要先创建 `CommandProcessor` 对象。

私有成员：

```cpp
Store& store_;
```

这里保存的是 `Store` 的引用。

意思是：

```text
CommandProcessor 不拥有 Store
它只是借用 Store
```

一句话总结：`command.hpp` 定义了命令处理器的接口，它负责把文本命令变成数据库操作。

---

# 4. `command.cpp`：实现命令解析

这个文件是第一次讲解的重点。

它的总流程是：

```text
输入一行字符串
 -> 去掉前后空白
 -> 拿到第一个单词作为命令名
 -> 命令名转大写
 -> 根据命令分支处理
 -> 调用 Store
 -> 返回 CommandResult
```

## 辅助函数：去掉前后空格

```cpp
std::string trim_copy(std::string_view text) {
```

这个函数负责清理输入前后的空白。

比如用户输入：

```text
   GET name
```

它会变成：

```text
GET name
```

前半段：

```cpp
while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())) != 0) {
    text.remove_prefix(1);
}
```

不断检查最前面的字符是不是空白。

如果是，就把开头去掉一个字符。

后半段：

```cpp
while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())) != 0) {
    text.remove_suffix(1);
}
```

不断检查最后面的字符是不是空白。

如果是，就把末尾去掉一个字符。

最后：

```cpp
return std::string{text};
```

返回清理后的字符串。

## 辅助函数：命令转大写

```cpp
std::string to_upper(std::string_view text) {
```

这个函数把命令名转成大写。

比如：

```text
get
Get
GET
```

都会变成：

```text
GET
```

所以项目命令大小写不敏感。

核心逻辑：

```cpp
for (char& ch : result) {
    ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}
```

逐个字符转换成大写。

## 辅助函数：检查多余参数

```cpp
bool has_extra_token(std::istringstream& input) {
```

它用于判断命令后面有没有多余内容。

比如：

```text
GET name extra
```

`GET` 只应该有一个 key。

这里的 `extra` 就是多余参数。

函数内部：

```cpp
std::string extra;
return static_cast<bool>(input >> extra);
```

如果还能继续读出一个单词，说明有多余参数。

## 辅助函数：返回用法错误

```cpp
CommandResult usage(std::string_view command) {
```

这个函数统一生成错误提示。

比如：

```cpp
return usage("GET key");
```

会返回：

```text
ERR usage: GET key
```

---

# 5. `CommandProcessor::execute`

这是核心函数：

```cpp
CommandResult CommandProcessor::execute(std::string_view line) {
```

它接收一行命令。

第一步：

```cpp
const std::string trimmed = trim_copy(line);
```

先去掉前后空白。

如果用户输入空行：

```cpp
if (trimmed.empty()) {
    return {};
}
```

直接返回空结果。

这表示：

```text
没有响应
不退出
```

然后创建字符串输入流：

```cpp
std::istringstream input{trimmed};
```

它可以像读普通输入一样，从字符串里一个词一个词读取。

读取命令名：

```cpp
std::string command;
input >> command;
command = to_upper(command);
```

比如：

```text
set name mini-kv
```

这里会得到：

```text
SET
```

## 处理 `SET`

```cpp
if (command == "SET") {
```

先读取 key：

```cpp
std::string key;
input >> key;
```

然后读取 value：

```cpp
std::string value;
std::getline(input >> std::ws, value);
```

这里很关键。

`SET` 的 value 不是只读一个单词，而是读取后面整段内容。

所以：

```text
SET message hello world
```

会得到：

```text
key = message
value = hello world
```

如果 key 或 value 为空：

```cpp
if (key.empty() || value.empty()) {
    return usage("SET key value");
}
```

返回用法错误。

然后真正写入：

```cpp
const bool inserted = store_.set(key, value);
```

如果是新 key：

```text
OK inserted
```

如果是更新已有 key：

```text
OK updated
```

## 处理 `GET`

```cpp
if (command == "GET") {
```

读取 key，并检查参数是否合法：

```cpp
if (key.empty() || has_extra_token(input)) {
    return usage("GET key");
}
```

然后读取数据：

```cpp
const auto value = store_.get(key);
```

如果没找到：

```text
(nil)
```

如果找到了，直接返回 value。

## 处理 `DEL`

`DEL` 删除一个 key。

返回规则是：

```text
1 = 删除成功
0 = key 不存在
```

## 处理 `SIZE`

`SIZE` 不需要参数。

它返回当前 key 数量：

```cpp
return {std::to_string(store_.size())};
```

## 处理 `HELP`

`HELP` 返回支持的命令说明。

## 处理 `EXIT` 和 `QUIT`

```cpp
if (command == "EXIT" || command == "QUIT") {
```

这两个命令都表示退出。

如果参数正确：

```cpp
return {"BYE", true};
```

这里返回两个信息：

```text
response = BYE
should_close = true
```

CLI 或 TCP 服务端看到 `should_close = true` 后，就会关闭当前会话。

## 未知命令

如果前面所有分支都不匹配：

```cpp
return {"ERR unknown command"};
```

比如输入：

```text
HELLO
```

就会返回：

```text
ERR unknown command
```

---

# 第一次总结

这一次讲的是项目最核心的数据与命令层：

```text
Store
  负责保存数据
  内部是 unordered_map
  使用 mutex 保证线程安全

CommandProcessor
  负责解析用户命令
  根据 SET / GET / DEL / SIZE / HELP / EXIT 调用 Store
  返回 CommandResult 给 CLI 或 TCP Server
```

整个流程一句话总结：

```text
Store 是仓库，CommandProcessor 是前台接待员。用户说“SET name mini-kv”，CommandProcessor 听懂命令后，让 Store 把 name=mini-kv 存起来，再把结果返回给用户。
```
