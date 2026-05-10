# 第六十九次讲解：第三十四版客户端命令补全核心

第六十三次讲了客户端行编辑：

```text
LineEditorBuffer
LineEditorHistoryNavigator
read_client_line
```

第三十四版在这条链路上继续增强交互体验：

```text
Tab 命令补全
```

涉及的核心文件是：

```text
include/minikv/line_editor.hpp
src/line_editor.cpp
```

一句话总结：

```text
v34 让 bundled TCP client 在交互式终端里可以按 Tab 补全命令前缀，同时保持重定向输入仍然是普通 getline。
```

---

# 1. 为什么补全放在线编辑器里

客户端主循环本来是：

```text
client_main
 -> 生成 prompt
 -> read_client_line(prompt, history.entries(), line)
 -> resolve_client_input
 -> send_all
 -> print_response
```

行编辑器已经负责：

```text
读取按键
维护当前行文本
移动光标
浏览历史
重画 prompt
```

Tab 补全同样属于“当前行编辑”能力，所以 v34 把它放在：

```text
line_editor.hpp
line_editor.cpp
```

而不是放到 `client_main.cpp`。

这样主循环不用知道按键细节。

---

# 2. LineEditorCompletion 的角色

头文件新增：

```cpp
class LineEditorCompletion {
public:
    explicit LineEditorCompletion(std::vector<std::string> candidates);

    std::optional<std::string> complete(std::string_view text, std::size_t cursor) const;

private:
    std::vector<std::string> candidates_;
};
```

它只做一件事：

```text
给定当前文本和光标位置
判断是否能补全
如果能，返回补全后的整行文本
如果不能，返回 std::nullopt
```

把补全逻辑做成纯函数风格的类，有两个好处：

```text
不用依赖真实终端就能测试
read_client_line 只负责把 Tab 按键接到 complete
```

---

# 3. 默认补全候选

`line_editor.cpp` 中有默认候选：

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
COMPACT
WALINFO
STATS
HEALTH
HELP
EXIT
QUIT
:history
```

这些候选覆盖两类输入：

```text
服务端命令
 -> PING / GET / SET / ...

客户端本地命令
 -> :history
```

`!!` 和 `!N` 没有加入候选，因为它们是历史引用语法，不是固定命令名。

---

# 4. complete 的第一步：确认光标位置

补全函数先处理几个无效场景：

```text
cursor > text.size()
 -> 无效，返回 nullopt

candidates 为空
 -> 没东西可补，返回 nullopt
```

然后找到第一个 token：

```text
跳过行首空白
找到第一个非空白 token 的开始
找到 token 结束位置
```

v34 的补全只补第一个 token。

这意味着：

```text
PI
 -> 可以补成 PING

SET name
 -> 光标在参数里时不补
```

这是保守设计，避免误改 key 或 value。

---

# 5. 为什么只在 token 末尾补全

代码里有一个判断：

```text
cursor != token_end
 -> 返回 nullopt
```

也就是说，只有光标在命令词末尾时才补全。

例如：

```text
PI|
 -> 可以补全

P|ING
 -> 不补全
```

这样可以避免“在单词中间按 Tab 后整段文本被重写”的意外体验。

---

# 6. 大小写处理

补全匹配使用：

```text
to_upper_copy
```

所以用户输入小写也能补：

```text
pi
 -> PING

hea
 -> HEALTH
```

补全结果使用候选里的写法。

所以服务端命令会输出大写：

```text
PING
HEALTH
```

本地命令保持原样：

```text
:history
```

---

# 7. 唯一匹配如何补全

如果只有一个候选匹配：

```text
PI
 -> PING

:h
 -> :history
```

v34 会返回：

```text
PING 
:history 
```

注意末尾多一个空格。

这是为了让用户可以继续输入参数，减少一次手动敲空格。

---

# 8. 多个匹配如何补全

如果有多个候选匹配，会计算最长公共前缀。

例如候选里有：

```text
EXIT
EXPIRE
```

用户输入：

```text
E
```

最长公共前缀是：

```text
EX
```

所以补全结果是：

```text
EX
```

如果公共前缀没有变长，就不做任何修改：

```text
S
 -> SET / SAVE / SIZE / STATS
 -> 公共前缀仍然是 S
 -> 不补
```

这避免 Tab 在歧义很大的时候乱跳。

---

# 9. Tab 按键如何接入

`KeyKind` 新增：

```cpp
tab
```

Windows 和非 Windows 的 `read_key` 都识别：

```text
'\t'
```

主循环中新增处理：

```text
case KeyKind::tab:
    completion.complete(buffer.text(), buffer.cursor())
    如果有结果，就 buffer.set_text(...)
    changed = true
```

只要 `changed` 为 true，原来的重画逻辑就会执行：

```text
redraw_line(prompt, buffer)
```

因此补全后的文本会立刻出现在当前行。

---

# 10. 非交互输入不受影响

`read_client_line` 开头仍然先判断：

```text
terminal_input_available()
```

如果 stdin 或 stdout 不是终端：

```text
fallback_read_line(prompt, line)
```

也就是继续用：

```text
std::getline(std::cin, line)
```

所以脚本、管道输入、smoke test 不会因为 Tab 补全改变行为。

---

# 11. 一句话总结

第三十四版给客户端行编辑器补上了一个小而实用的交互能力：

```text
Tab
 -> 补全第一个命令 token
 -> 唯一匹配补全成完整命令并加空格
 -> 多匹配时只扩展最长公共前缀
 -> 参数位置不补全
 -> 重定向输入不受影响
```

这让 `minikv_client` 更接近一个顺手的交互式客户端，而不是只会逐行发送文本的简易工具。
