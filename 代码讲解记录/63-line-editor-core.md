# 第六十三次讲解：第三十一版客户端行编辑核心

第六十二次讲了第三十版测试、README、归档和整体增删改。
这次讲第三十一版的核心代码：

```text
include/minikv/line_editor.hpp
src/line_editor.cpp
src/client_main.cpp
```

第三十版时，bundled TCP client 已经具备：

```text
连接服务端
可选 socket timeout
连接重试
:history / !! / !N 本地历史命令
--history-file 持久化历史
```

第三十一版继续补客户端交互体验：

```text
交互终端下支持本地行编辑
 -> Up / Down 浏览历史
 -> Left / Right 移动光标
 -> Home / End 跳到首尾
 -> Backspace / Delete 删除字符

非交互输入下保持 getline
 -> 管道输入
 -> 重定向 stdin
 -> 自动化 smoke test
```

一句话说，v31 把 `minikv_client` 从“只能一行一行输入”推进到“像正常命令行工具一样能编辑当前行”。

---

# 1. 为什么 v31 要新增 line_editor 模块

v30 之后 Roadmap 里有一项：

```text
Add interactive line editing for the bundled TCP client.
```

如果没有行编辑，客户端使用时有几个明显不舒服的点：

```text
打错一个字符只能整行重输
不能用左右方向键移动光标
不能用上下方向键翻历史
已经有 :history / !! / !N，但不能像 shell 一样直接调出历史行再编辑
```

所以 v31 做了一个小模块：

```text
line_editor
```

它不负责网络，也不负责命令语义。
它只负责：

```text
读取一行用户输入
在交互终端中处理编辑按键
必要时从历史里取上一条 / 下一条
最后返回完整字符串给 client_main
```

这样边界很清楚：

```text
client_main
 -> 连接服务端、发送命令、接收响应

client_history
 -> 管理已发送命令历史、:history / !! / !N

line_editor
 -> 管理当前正在编辑的这一行
```

---

# 2. 头文件暴露了三个入口

`include/minikv/line_editor.hpp` 暴露三类能力：

```cpp
class LineEditorBuffer;
class LineEditorHistoryNavigator;

bool read_client_line(std::string_view prompt,
                      const std::vector<std::string>& history_entries,
                      std::string& line);
```

它们的定位是：

```text
LineEditorBuffer
 -> 当前输入行的文本和光标位置

LineEditorHistoryNavigator
 -> Up / Down 历史浏览状态

read_client_line
 -> client_main 真正调用的读一行函数
```

为什么把 Buffer 和 Navigator 放到头文件？

```text
它们是纯逻辑
容易写单元测试
不依赖真实终端
```

真实键盘读取和终端控制仍然留在 `.cpp` 的匿名 namespace 里，不暴露给外部。

---

# 3. LineEditorBuffer 的角色

`LineEditorBuffer` 保存两个字段：

```cpp
std::string text_;
std::size_t cursor_ = 0;
```

它的含义是：

```text
text_
 -> 当前正在编辑的整行文本

cursor_
 -> 光标位置，范围是 0 到 text_.size()
```

注意 `cursor_` 不是字符下标，而是插入位置。

举例：

```text
text = "SET"

cursor = 0
 -> |SET

cursor = 1
 -> S|ET

cursor = 3
 -> SET|
```

所以光标可以等于 `text_.size()`，表示在行尾。

---

# 4. clear 和 set_text

清空当前行：

```cpp
void LineEditorBuffer::clear() {
    text_.clear();
    cursor_ = 0;
}
```

这用于重新开始一行。

设置整行文本：

```cpp
void LineEditorBuffer::set_text(std::string_view text) {
    text_ = std::string{text};
    cursor_ = text_.size();
}
```

`set_text` 主要给历史导航用。

当用户按 Up 调出一条历史命令时，比如：

```text
SET name mini-kv
```

编辑器会把整行替换为历史命令，并把光标放到行尾。
这符合常见 shell 行为：

```text
调出历史后，用户通常继续在末尾修改或直接回车
```

---

# 5. text / cursor / empty

三个只读方法：

```cpp
const std::string& text() const;
std::size_t cursor() const;
bool empty() const;
```

它们分别给外部读取：

```text
当前文本
当前光标
当前文本是否为空
```

其中 `read_client_line` 会在回车时使用：

```cpp
line = buffer.text();
```

在 EOF 时使用：

```cpp
if (buffer.empty()) {
    return false;
}
```

含义是：

```text
空行收到 EOF
 -> 表示用户真的结束输入

非空行收到 EOF
 -> 把当前编辑内容当作一行返回
```

---

# 6. insert 的插入逻辑

插入字符：

```cpp
void LineEditorBuffer::insert(char ch) {
    text_.insert(text_.begin() + static_cast<std::string::difference_type>(cursor_), ch);
    ++cursor_;
}
```

它不是简单追加到行尾，而是在光标处插入。

举例：

```text
text = "SET"
cursor = 2
insert('X')

结果：
text = "SEXT"
cursor = 3
```

这就是左右方向键有意义的原因：

```text
用户可以把光标移到中间，再插入字符
```

这里的 `static_cast<std::string::difference_type>` 是为了匹配 `std::string::iterator` 的偏移类型。

---

# 7. Backspace：erase_before_cursor

Backspace 删除光标前一个字符：

```cpp
bool LineEditorBuffer::erase_before_cursor() {
    if (cursor_ == 0) {
        return false;
    }

    text_.erase(text_.begin() + static_cast<std::string::difference_type>(cursor_ - 1));
    --cursor_;
    return true;
}
```

如果光标在最开头：

```text
|SET
```

前面没有字符可删，所以返回 `false`。

如果光标在中间：

```text
SE|T
```

Backspace 后：

```text
S|T
```

返回值的作用是告诉 `read_client_line`：

```text
内容或光标是否真的发生变化
```

只有变化时才重绘整行。

---

# 8. Delete：erase_at_cursor

Delete 删除光标处字符：

```cpp
bool LineEditorBuffer::erase_at_cursor() {
    if (cursor_ >= text_.size()) {
        return false;
    }

    text_.erase(text_.begin() + static_cast<std::string::difference_type>(cursor_));
    return true;
}
```

它和 Backspace 的区别是：

```text
Backspace
 -> 删除光标左边字符

Delete
 -> 删除光标右边，也就是光标当前指向的字符
```

举例：

```text
SE|T
Delete
 -> SE|
```

如果光标已经在行尾：

```text
SET|
```

右边没有字符可删，所以返回 `false`。

---

# 9. 左右移动和 Home / End

左移：

```cpp
bool LineEditorBuffer::move_left() {
    if (cursor_ == 0) {
        return false;
    }

    --cursor_;
    return true;
}
```

右移：

```cpp
bool LineEditorBuffer::move_right() {
    if (cursor_ >= text_.size()) {
        return false;
    }

    ++cursor_;
    return true;
}
```

Home / End：

```cpp
void LineEditorBuffer::move_home() {
    cursor_ = 0;
}

void LineEditorBuffer::move_end() {
    cursor_ = text_.size();
}
```

边界行为是：

```text
在最左边继续左移
 -> false，不重绘

在最右边继续右移
 -> false，不重绘

Home
 -> cursor = 0

End
 -> cursor = text.size()
```

---

# 10. LineEditorHistoryNavigator 的角色

`LineEditorHistoryNavigator` 保存：

```cpp
const std::vector<std::string>& entries_;
std::size_t index_;
std::string draft_;
bool draft_saved_ = false;
```

字段含义：

```text
entries_
 -> ClientHistory 中已有的历史命令，只引用，不复制

index_
 -> 当前浏览到哪一条历史

draft_
 -> 用户按 Up 之前，当前正在编辑但还没发送的文本

draft_saved_
 -> 是否已经保存过当前草稿
```

这里最关键的是 `draft_`。

用户可能正在输入：

```text
SET dra
```

然后按 Up 看历史，再按 Down 回来。
如果不保存草稿，`SET dra` 就丢了。

v31 的 navigator 会保存它。

---

# 11. previous：按 Up

按 Up 时调用：

```cpp
std::optional<std::string> LineEditorHistoryNavigator::previous(std::string_view current_text) {
    if (entries_.empty()) {
        return std::nullopt;
    }

    if (!draft_saved_) {
        draft_ = std::string{current_text};
        draft_saved_ = true;
        index_ = entries_.size();
    }

    if (index_ > 0) {
        --index_;
    }

    return entries_[index_];
}
```

流程是：

```text
没有历史
 -> 返回 nullopt

第一次按 Up
 -> 保存当前输入为 draft
 -> index 从 entries.size() 开始

然后 index 往前移动
 -> 返回上一条历史
```

假设历史是：

```text
1 PING one
2 SET name mini-kv
3 GET name
```

当前输入是：

```text
draft command
```

第一次 Up：

```text
GET name
```

第二次 Up：

```text
SET name mini-kv
```

第三次 Up：

```text
PING one
```

继续 Up：

```text
仍然 PING one
```

不会越界。

---

# 12. next：按 Down

按 Down 时调用：

```cpp
std::optional<std::string> LineEditorHistoryNavigator::next() {
    if (entries_.empty() || !draft_saved_) {
        return std::nullopt;
    }

    if (index_ < entries_.size()) {
        ++index_;
    }

    if (index_ >= entries_.size()) {
        return draft_;
    }

    return entries_[index_];
}
```

它的语义是：

```text
没进过历史浏览
 -> Down 没效果

正在历史里
 -> 往更新的历史走

走到最后一条之后
 -> 回到最初保存的 draft
```

沿用前面的例子：

```text
PING one
 -> Down 得到 SET name mini-kv
 -> Down 得到 GET name
 -> Down 回到 draft command
```

这让 Up/Down 的体验更接近 shell。

---

# 13. KeyKind 把不同平台按键统一起来

`line_editor.cpp` 里定义：

```cpp
enum class KeyKind {
    character,
    enter,
    backspace,
    delete_forward,
    left,
    right,
    up,
    down,
    home,
    end,
    eof,
    unknown
};
```

以及：

```cpp
struct KeyEvent {
    KeyKind kind = KeyKind::unknown;
    char character = '\0';
};
```

它的作用是屏蔽平台差异。

Windows 控制台方向键通常由 `_getch()` 返回扩展码。
Unix-like 终端方向键通常是 escape sequence，例如：

```text
Up    -> ESC [ A
Down  -> ESC [ B
Right -> ESC [ C
Left  -> ESC [ D
```

但上层编辑循环不关心这些细节。
它只处理统一后的：

```text
KeyKind::up
KeyKind::down
KeyKind::left
...
```

---

# 14. terminal_input_available 的保护

v31 不是任何时候都启用交互行编辑。

判断函数：

```cpp
bool terminal_input_available() {
#ifdef _WIN32
    const HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    DWORD output_mode = 0;
    return input != INVALID_HANDLE_VALUE && output != INVALID_HANDLE_VALUE &&
           GetConsoleMode(input, &mode) != 0 && GetConsoleMode(output, &output_mode) != 0;
#else
    return isatty(STDIN_FILENO) != 0 && isatty(STDOUT_FILENO) != 0;
#endif
}
```

这里同时检查：

```text
stdin 是终端
stdout 也是终端
```

为什么 stdout 也要检查？

```text
行编辑需要用 ANSI 控制序列重绘当前行
如果 stdout 被重定向到文件，控制序列会污染输出文件
```

所以只有真正交互使用时才启用行编辑。

脚本场景：

```powershell
Get-Content input.txt | minikv_client.exe ...
```

或者：

```powershell
minikv_client.exe ... < input.txt
```

都会走 fallback。

---

# 15. fallback_read_line 保持兼容

非交互时调用：

```cpp
bool fallback_read_line(std::string_view prompt, std::string& line) {
    std::cout << prompt;
    std::cout.flush();
    return static_cast<bool>(std::getline(std::cin, line));
}
```

这和 v31 之前的行为基本一致：

```text
打印 prompt
getline 读一行
返回成功或失败
```

它保证了两件事：

```text
已有自动化 smoke 不会因为行编辑破坏
用户用文件批量喂命令也仍然可用
```

这也是 v31 真实 smoke 专门验证的重点。

---

# 16. Windows 的 read_key

Windows 分支使用：

```cpp
#include <conio.h>
```

读取单键：

```cpp
const int ch = _getch();
```

普通键直接处理：

```cpp
if (ch == '\r' || ch == '\n') {
    return {KeyKind::enter, '\0'};
}
if (ch == '\b') {
    return {KeyKind::backspace, '\0'};
}
```

方向键、Home、End、Delete 等扩展键，`_getch()` 会先返回 `0` 或 `224`，然后需要再读一个 code：

```cpp
if (ch == 0 || ch == 224) {
    const int code = _getch();
    switch (code) {
    case 71:
        return {KeyKind::home, '\0'};
    case 72:
        return {KeyKind::up, '\0'};
    case 75:
        return {KeyKind::left, '\0'};
    case 77:
        return {KeyKind::right, '\0'};
    case 79:
        return {KeyKind::end, '\0'};
    case 80:
        return {KeyKind::down, '\0'};
    case 83:
        return {KeyKind::delete_forward, '\0'};
    default:
        return {KeyKind::unknown, '\0'};
    }
}
```

普通可打印 ASCII 字符：

```cpp
if (ch >= 32 && ch <= 126) {
    return {KeyKind::character, static_cast<char>(ch)};
}
```

当前版本主要覆盖 ASCII 命令输入，和 mini-kv 的命令格式一致。

---

# 17. Windows 虚拟终端输出

重绘行使用了 ANSI 控制序列：

```text
\r
\x1b[2K
\x1b[<n>D
```

Windows 控制台需要尽量启用虚拟终端输出：

```cpp
void enable_virtual_terminal_output() {
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (output != INVALID_HANDLE_VALUE && GetConsoleMode(output, &mode) != 0) {
        SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}
```

如果启用失败，程序也不会直接崩溃。
只是重绘效果可能受终端支持影响。

这符合客户端工具的策略：

```text
尽量增强体验
不因为终端能力不足影响基本命令发送
```

---

# 18. Unix-like 的 raw terminal mode

非 Windows 下使用：

```cpp
#include <termios.h>
#include <unistd.h>
```

`RawTerminalMode` 构造时保存原始终端配置：

```cpp
tcgetattr(STDIN_FILENO, &original_)
```

然后关闭 canonical mode 和 echo：

```cpp
raw.c_lflag &= ~(static_cast<tcflag_t>(ICANON) | static_cast<tcflag_t>(ECHO) |
                 static_cast<tcflag_t>(IEXTEN));
raw.c_iflag &= ~(static_cast<tcflag_t>(IXON) | static_cast<tcflag_t>(ICRNL));
raw.c_cc[VMIN] = 1;
raw.c_cc[VTIME] = 0;
```

含义是：

```text
ICANON off
 -> 不再等整行输入，按一个键就返回

ECHO off
 -> 终端不要自动回显字符，由 line_editor 自己重绘

VMIN = 1
 -> 至少读到 1 个字节才返回
```

析构时恢复终端：

```cpp
~RawTerminalMode() {
    if (enabled_) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_);
    }
}
```

这很重要。
如果不恢复，用户退出程序后 shell 可能还处于“不回显、不按行”的状态。

---

# 19. Unix-like 的 escape sequence 解析

方向键以 ESC 开头。

`read_key()` 里：

```cpp
if (ch == '\x1b') {
    return read_escape_sequence();
}
```

`read_escape_sequence()` 先读 `[`：

```cpp
char first = '\0';
if (!read_byte(first) || first != '[') {
    return {KeyKind::unknown, '\0'};
}
```

再读具体键：

```cpp
case 'A':
    return {KeyKind::up, '\0'};
case 'B':
    return {KeyKind::down, '\0'};
case 'C':
    return {KeyKind::right, '\0'};
case 'D':
    return {KeyKind::left, '\0'};
case 'F':
    return {KeyKind::end, '\0'};
case 'H':
    return {KeyKind::home, '\0'};
```

另外一些终端会用带 `~` 的形式：

```text
Home   -> ESC [ 1 ~ 或 ESC [ 7 ~
Delete -> ESC [ 3 ~
End    -> ESC [ 4 ~ 或 ESC [ 8 ~
```

所以代码也覆盖了这些情况。

---

# 20. redraw_line 的重绘方式

重绘函数：

```cpp
void redraw_line(std::string_view prompt, const LineEditorBuffer& buffer) {
    std::cout << '\r' << "\x1b[2K" << prompt << buffer.text();
    const std::size_t trailing = buffer.text().size() - buffer.cursor();
    if (trailing > 0) {
        std::cout << "\x1b[" << trailing << 'D';
    }
    std::cout.flush();
}
```

流程是：

```text
\r
 -> 回到行首

\x1b[2K
 -> 清除整行

prompt + buffer.text()
 -> 重新打印提示符和当前文本

如果光标不在行尾
 -> 用 \x1b[nD 把光标往左移 n 位
```

举例：

```text
prompt = mini-kv@127.0.0.1:6379>
text = SET name value
cursor = 4
```

行尾到光标的距离是：

```text
text.size() - cursor
```

重绘后把光标往左挪这些位，就能停在正确位置。

---

# 21. read_client_line 的总入口

真正给 `client_main` 调用的是：

```cpp
bool read_client_line(std::string_view prompt,
                      const std::vector<std::string>& history_entries,
                      std::string& line)
```

参数含义：

```text
prompt
 -> 要显示的提示符

history_entries
 -> 当前可浏览的历史命令

line
 -> 输出参数，保存最终读到的一行
```

开头先清空输出：

```cpp
line.clear();
```

然后判断是否能使用交互终端：

```cpp
if (!terminal_input_available()) {
    return fallback_read_line(prompt, line);
}
```

非 Windows 下再进入 raw mode：

```cpp
RawTerminalMode raw_mode;
if (!raw_mode.enabled()) {
    return fallback_read_line(prompt, line);
}
```

如果终端不支持 raw mode，也回退到 `getline`。

---

# 22. 编辑循环如何处理按键

初始化：

```cpp
LineEditorBuffer buffer;
LineEditorHistoryNavigator navigator{history_entries};
redraw_line(prompt, buffer);
```

然后进入循环：

```cpp
while (true) {
    const KeyEvent key = read_key();
    bool changed = false;

    switch (key.kind) {
        ...
    }

    if (changed) {
        redraw_line(prompt, buffer);
    }
}
```

核心策略是：

```text
读一个键
修改 buffer 或 navigator
如果有变化就重绘
回车时返回 true
空行 EOF 时返回 false
```

这样的结构很简单，也符合当前项目的轻量风格。

---

# 23. 普通字符和回车

普通字符：

```cpp
case KeyKind::character:
    buffer.insert(key.character);
    changed = true;
    break;
```

回车：

```cpp
case KeyKind::enter:
    line = buffer.text();
    std::cout << '\n';
    return true;
```

也就是说：

```text
字符只改变当前编辑缓冲区
回车才把整行交给 client_main
```

这和原来的 `std::getline` 行为保持一致：

```text
client_main 只看到最终的一行字符串
```

它并不知道这行是用户逐字打出来的，还是从历史里调出来再编辑的。

---

# 24. 删除和移动按键

删除：

```cpp
case KeyKind::backspace:
    changed = buffer.erase_before_cursor();
    break;
case KeyKind::delete_forward:
    changed = buffer.erase_at_cursor();
    break;
```

移动：

```cpp
case KeyKind::left:
    changed = buffer.move_left();
    break;
case KeyKind::right:
    changed = buffer.move_right();
    break;
case KeyKind::home:
    buffer.move_home();
    changed = true;
    break;
case KeyKind::end:
    buffer.move_end();
    changed = true;
    break;
```

这里 Home / End 总是标记为 changed。
即使当前位置已经在首尾，多重绘一次也没关系，逻辑更直观。

Left / Right 和删除函数则返回是否真的改变。

---

# 25. Up / Down 如何接入历史

Up：

```cpp
case KeyKind::up:
    if (const auto previous = navigator.previous(buffer.text())) {
        buffer.set_text(*previous);
        changed = true;
    }
    break;
```

Down：

```cpp
case KeyKind::down:
    if (const auto next = navigator.next()) {
        buffer.set_text(*next);
        changed = true;
    }
    break;
```

注意传给 `previous` 的是：

```cpp
buffer.text()
```

也就是当前草稿。

第一次按 Up 时，navigator 会保存这份草稿。
后面按 Down 走出历史时，就能回到它。

---

# 26. EOF 的处理

EOF：

```cpp
case KeyKind::eof:
    if (buffer.empty()) {
        return false;
    }
    line = buffer.text();
    std::cout << '\n';
    return true;
```

这里分两种：

```text
当前行为空
 -> 真的结束输入，返回 false

当前行非空
 -> 把当前文本当成一行，返回 true
```

这样用户在交互终端中即使按了 EOF，也不会丢掉已经输入的内容。

---

# 27. client_main 如何接入 read_client_line

v31 之前，客户端主循环是：

```cpp
std::cout << "mini-kv@" << options.host << ':' << options.port << "> ";
if (!std::getline(std::cin, line)) {
    std::cout << '\n';
    break;
}
```

v31 改成：

```cpp
const std::string prompt = "mini-kv@" + options.host + ':' + std::to_string(options.port) + "> ";
if (!minikv::read_client_line(prompt, history.entries(), line)) {
    std::cout << '\n';
    break;
}
```

变化有三点：

```text
prompt 先拼成字符串
把 history.entries() 传给 line_editor
用 read_client_line 替代 getline
```

后面的逻辑没有变：

```text
空白行跳过
resolve_client_input 处理 :history / !! / !N
send_all 发送命令
print_response 打印响应
保存 history_file
QUIT / EXIT 后退出
```

这说明 v31 把变化压在输入层，没有扩散到网络和命令处理层。

---

# 28. 为什么 history 仍由 ClientHistory 管理

`LineEditorHistoryNavigator` 没有自己保存历史文件，也没有自己决定哪些命令进入历史。

它只拿一个：

```cpp
const std::vector<std::string>& entries
```

这些 entries 来自：

```cpp
history.entries()
```

也就是 `ClientHistory`。

原因是历史语义已经在 `client_history` 模块里定义好了：

```text
什么命令会进入历史
:history 如何展示
!! 如何重复
!N 如何索引
--history-file 如何加载和保存
容量如何裁剪
```

行编辑只负责浏览这些历史。
它不重新发明历史规则。

---

# 29. 为什么没有引入第三方 readline

v31 没有引入 GNU Readline、linenoise 等外部依赖。

原因是当前项目定位是：

```text
C++20 / CMake 练习项目
跨 Windows / Linux / macOS 构建
尽量保持依赖轻
```

如果引入 readline，Windows 支持和 CI 依赖会变复杂。

当前手写实现覆盖了最需要的功能：

```text
左右移动
Home / End
Backspace / Delete
Up / Down 历史
非交互 fallback
```

对于 mini-kv 这个阶段，已经足够。

后续如果要做更复杂能力，比如：

```text
多字节 UTF-8 宽度
命令补全
反向搜索
多行编辑
```

再评估引入更成熟库也可以。

---

# 30. v31 核心增删改

新增：

```text
include/minikv/line_editor.hpp
 -> LineEditorBuffer
 -> LineEditorHistoryNavigator
 -> read_client_line

src/line_editor.cpp
 -> 行编辑缓冲区实现
 -> 历史导航实现
 -> Windows _getch 按键读取
 -> Unix-like raw terminal mode 和 escape sequence 解析
 -> ANSI 行重绘
 -> 非交互 fallback getline
```

修改：

```text
src/client_main.cpp
 -> 引入 line_editor.hpp
 -> 主循环用 read_client_line 读取输入
 -> 把 history.entries() 传入行编辑器
```

保持不变：

```text
ClientHistory 的历史保存规则
:history / !! / !N 语义
socket 连接和重试逻辑
send_all / read_line / print_response
服务端命令语义
非交互管道输入能力
```

---

# 第六十三次总结

第三十一版核心链路是：

```text
minikv_client 主循环
 -> 构造 prompt
 -> read_client_line(prompt, history.entries(), line)
 -> 交互终端：LineEditorBuffer + LineEditorHistoryNavigator 处理按键
 -> 非交互输入：fallback_read_line 使用 getline
 -> 返回完整 line
 -> resolve_client_input
 -> send_all 发给 server
```

一句话总结：

```text
第三十一版把客户端输入能力抽成 line_editor 模块，让真实终端使用时可以像普通命令行工具一样编辑当前行和浏览历史，同时保留管道和重定向输入的旧行为，避免破坏脚本化使用。
```
