# 第四十七次讲解：第二十三版 WAL checksum 核心

第四十六次讲了第二十二版测试、README、归档和整体增删改。

这次讲第二十三版的核心代码：

```text
include/minikv/wal.hpp
src/wal.cpp
src/main.cpp
src/server_main.cpp
```

第二十三版没有新增用户命令。

它改的是 WAL 记录格式和 replay 观测：

```text
旧格式：
SET key value
DEL key
EXPIREAT key epoch_millis

新格式：
WAL2 <checksum> <record>
```

比如：

```text
WAL2 775d3cab2dd9031d SET good value
```

核心目标是：

```text
新写入 WAL 记录带 checksum
旧格式 WAL 仍然可以 replay
checksum 不匹配的 WAL2 记录跳过并计数
```

---

# 1. 为什么要给 WAL 加 checksum

第十九版已经让 WAL replay 更保守：

```text
坏记录跳过
尾部半条记录跳过
replay 报告 applied / skipped / truncated
```

但它只能识别明显坏掉的文本。

比如：

```text
GARBAGE record
EXPIREAT key not-a-number
SET partial value
```

如果一条记录仍然长得像合法命令，但内容被改坏了，旧逻辑看不出来。

例如原来是：

```text
SET name old
```

被意外改成：

```text
SET name new
```

语法仍然合法，旧 replay 会直接应用。

第二十三版给新 WAL 记录加 checksum，就是为了识别这种“格式还像命令，但内容已变”的情况。

---

# 2. 为什么新格式要叫 WAL2

新记录前缀是：

```cpp
constexpr std::string_view wal2_prefix = "WAL2 ";
```

这表示：

```text
这是一条带 checksum 的第二版 WAL 记录
```

旧格式没有前缀：

```text
SET name mini-kv
DEL name
EXPIREAT name 123
```

新格式有前缀：

```text
WAL2 <checksum> <record>
```

这样 replay 时可以一眼判断：

```text
以 WAL2 开头
 -> 按新格式验证 checksum

不以 WAL2 开头
 -> 当作旧格式裸记录兼容 replay
```

---

# 3. 为什么不用尾部 checksum

一种常见想法是：

```text
SET key value checksum
```

但这个项目的 `SET` value 可以包含空格。

比如：

```text
SET phrase hello from wal
```

如果把 checksum 放在尾部，就很难区分：

```text
value 的最后一部分
checksum
```

所以第二十三版选择前缀包装格式：

```text
WAL2 checksum 原始记录
```

这样原始记录本身完全保持原样。

---

# 4. WalReplayReport 新增字段

头文件里：

```cpp
struct WalReplayReport {
    std::size_t applied_records = 0;
    std::size_t skipped_records = 0;
    std::size_t truncated_records = 0;
    std::size_t checksum_failed_records = 0;
};
```

前三个字段来自第十九版。

第二十三版新增：

```text
checksum_failed_records
```

它表示：

```text
有多少条 WAL2 记录因为 checksum 不匹配或 checksum 格式不合法被跳过
```

注意它不是独立于 skipped 的总数。

checksum failed 的记录也会计入：

```text
skipped_records
```

因为它确实被跳过了。

---

# 5. 新增头文件

`src/wal.cpp` 新增：

```cpp
#include <cstdint>
#include <iomanip>
#include <optional>
```

`<cstdint>` 用于：

```cpp
std::uint64_t
```

checksum 是 64 位整数。

`<iomanip>` 用于：

```cpp
std::hex
std::setfill
std::setw
```

也就是把 checksum 格式化成 16 位十六进制字符串。

`<optional>` 用于：

```cpp
std::optional<std::uint64_t>
```

解析 checksum 失败时返回 `std::nullopt`。

---

# 6. FNV-1a 常量

新增：

```cpp
constexpr std::uint64_t fnv_offset_basis = 14695981039346656037ull;
constexpr std::uint64_t fnv_prime = 1099511628211ull;
```

这是 64-bit FNV-1a hash 的两个常量。

FNV-1a 不是密码学签名。

它的定位是：

```text
轻量
实现简单
足够发现常见意外损坏
```

它不能防恶意攻击者。

但能帮助发现：

```text
文件被意外改写
局部字节损坏
手工误改 payload
```

---

# 7. DecodedWalRecord 的角色

新增：

```cpp
struct DecodedWalRecord {
    bool valid = false;
    bool checksum_failed = false;
    std::string record;
};
```

这个结构用于 `decode_wal_record()` 返回结果。

它表达三件事：

```text
valid
 -> 这条行能不能作为 WAL 记录 replay

checksum_failed
 -> 如果不能 replay，是否因为 checksum 失败

record
 -> 解码后的原始 WAL 记录
```

对于旧格式：

```text
SET legacy old
```

返回：

```text
valid = true
checksum_failed = false
record = "SET legacy old"
```

对于 checksum 不匹配的 WAL2：

```text
valid = false
checksum_failed = true
record = ""
```

---

# 8. wal_checksum

核心 hash 函数：

```cpp
std::uint64_t wal_checksum(std::string_view text) {
    std::uint64_t hash = fnv_offset_basis;
    for (const unsigned char ch : text) {
        hash ^= static_cast<std::uint64_t>(ch);
        hash *= fnv_prime;
    }
    return hash;
}
```

流程是：

```text
从 offset basis 开始
逐字节 XOR
再乘 fnv_prime
最后得到 64-bit hash
```

它对原始 WAL 记录文本计算。

比如：

```text
SET good value
```

checksum 就只覆盖这段 payload。

前面的：

```text
WAL2 <checksum>
```

不参与 checksum。

---

# 9. 为什么遍历 unsigned char

循环写成：

```cpp
for (const unsigned char ch : text)
```

这是为了让字节值稳定。

如果直接用 `char`，在不同平台上 `char` 可能是 signed，也可能是 unsigned。

hash 这种逐字节算法更适合用：

```text
unsigned char
```

来表达“这是字节”。

---

# 10. format_checksum

格式化函数：

```cpp
std::string format_checksum(std::uint64_t checksum) {
    std::ostringstream output;
    output << std::hex << std::nouppercase << std::setfill('0') << std::setw(16) << checksum;
    return output.str();
}
```

它把 64-bit checksum 变成 16 位十六进制字符串。

例如：

```text
775d3cab2dd9031d
```

几个格式控制：

```text
std::hex
 -> 十六进制

std::nouppercase
 -> a-f 小写

std::setfill('0')
 -> 不足位补 0

std::setw(16)
 -> 宽度 16
```

固定 16 位的好处是：

```text
decode 时知道 checksum 正好占 16 个字符
后面第 17 个字符必须是空格
```

---

# 11. parse_checksum

解析函数：

```cpp
std::optional<std::uint64_t> parse_checksum(std::string_view text)
```

第一步：

```cpp
if (text.size() != 16) {
    return std::nullopt;
}
```

checksum 必须正好 16 位。

然后逐字符解析十六进制：

```cpp
if (ch >= '0' && ch <= '9') ...
else if (ch >= 'a' && ch <= 'f') ...
else if (ch >= 'A' && ch <= 'F') ...
else return std::nullopt;
```

它同时接受大小写十六进制。

虽然写入时固定小写，但读取时宽容一点。

---

# 12. 位移累积

解析中：

```cpp
value = (value << 4) | digit;
```

每个十六进制字符代表 4 bit。

所以每读一个字符：

```text
左移 4 位
合并当前 digit
```

16 个十六进制字符正好是：

```text
16 * 4 = 64 bit
```

---

# 13. encode_wal_record

写入前编码：

```cpp
std::string encode_wal_record(std::string_view record) {
    return std::string{wal2_prefix} + format_checksum(wal_checksum(record)) + " " + std::string{record};
}
```

输入：

```text
SET good value
```

输出：

```text
WAL2 775d3cab2dd9031d SET good value
```

这里的 `record` 是原始 WAL 记录。

它来自 `CommandProcessor`：

```text
SET key value
DEL key
EXPIREAT key epoch_millis
```

---

# 14. append 改为写 WAL2

旧 append：

```cpp
output << record << '\n';
```

新 append：

```cpp
output << encode_wal_record(record) << '\n';
```

所以从第二十三版开始，新写入 WAL 都是 WAL2。

但 `CommandProcessor` 不需要知道 WAL2。

它仍然调用：

```cpp
wal_->append("SET name mini-kv")
```

WAL 模块内部负责：

```text
计算 checksum
包装 WAL2 前缀
写入文件
```

---

# 15. decode_wal_record 兼容旧格式

解码函数第一段：

```cpp
if (line.substr(0, wal2_prefix.size()) != wal2_prefix) {
    return DecodedWalRecord{true, false, std::string{line}};
}
```

如果一行不是以：

```text
WAL2 
```

开头，就按旧格式处理。

也就是说，旧 WAL 文件里的：

```text
SET kept value
DEL phrase
EXPIREAT name 123
```

仍然能 replay。

这非常重要。

否则一升级到 v23，用户过去生成的 WAL 就不能恢复了。

---

# 16. decode_wal_record 检查 WAL2 结构

如果是 WAL2，先取 payload：

```cpp
const auto payload = line.substr(wal2_prefix.size());
```

然后检查：

```cpp
if (payload.size() < 17 || payload[16] != ' ') {
    return DecodedWalRecord{false, true, {}};
}
```

为什么是 17？

```text
16 位 checksum
1 个空格
```

如果没有 16 位 checksum 或第 17 个字符不是空格，说明 WAL2 结构坏了。

这种情况计为：

```text
checksum_failed = true
```

因为它属于 WAL2 校验结构失败。

---

# 17. 解析 expected checksum

接着：

```cpp
const auto expected = parse_checksum(payload.substr(0, 16));
if (!expected.has_value()) {
    return DecodedWalRecord{false, true, {}};
}
```

前 16 个字符必须是合法十六进制。

如果不是：

```text
WAL2 zzzzzzzzzzzzzzzz SET name value
```

就返回：

```text
valid = false
checksum_failed = true
```

---

# 18. 取出原始 record

然后：

```cpp
std::string record{payload.substr(17)};
```

跳过：

```text
16 位 checksum + 1 个空格
```

剩下就是原始 WAL record。

例如：

```text
payload = "775d3cab2dd9031d SET good value"
record = "SET good value"
```

---

# 19. 对比 checksum

核心判断：

```cpp
if (wal_checksum(record) != *expected) {
    return DecodedWalRecord{false, true, {}};
}
```

如果 record 被改过：

```text
原来 checksum 对应 SET bad original
文件里 payload 被改成 SET bad tampered
```

重新计算的 checksum 就和 expected 不一样。

这条记录会被跳过。

---

# 20. checksum 成功后的返回

校验通过：

```cpp
return DecodedWalRecord{true, false, std::move(record)};
```

此时：

```text
valid = true
checksum_failed = false
record = 原始 WAL 命令
```

后面 replay 逻辑会继续调用：

```cpp
replay_record(store, decoded.record)
```

也就是说 WAL2 只是外层包装。

真正修改 Store 的仍然是原来的 `replay_record()`。

---

# 21. replay_with_report 的新流程

第十九版 replay 流程是：

```text
读所有行
检查尾部半条
跳过空白行
replay_record
统计 applied / skipped / truncated
```

第二十三版在空白行之后多了一步：

```cpp
const auto decoded = decode_wal_record(record);
if (!decoded.valid) {
    ++report.skipped_records;
    if (decoded.checksum_failed) {
        ++report.checksum_failed_records;
    }
    continue;
}
```

也就是：

```text
先 decode
decode 失败就跳过
如果是 checksum 失败，再加 checksum_failed_records
```

---

# 22. replay_record 保持原样

通过 decode 后：

```cpp
if (replay_record(store, decoded.record)) {
    ++report.applied_records;
} else {
    ++report.skipped_records;
}
```

这里沿用旧逻辑。

所以无论是：

```text
旧格式 SET kept value
新格式 WAL2 checksum SET kept value
```

最终都会进入同一个：

```text
replay_record
```

这让行为保持一致。

---

# 23. 旧坏记录不计 checksum_failed

如果旧格式是：

```text
GARBAGE record
```

它不以 `WAL2 ` 开头。

所以 decode 会返回：

```text
valid = true
record = "GARBAGE record"
```

然后 `replay_record()` 返回 false。

统计结果是：

```text
skipped_records +1
checksum_failed_records 不变
```

这很合理。

因为旧格式没有 checksum。

它只是普通坏记录，不是 checksum 失败记录。

---

# 24. 尾部半条记录仍然先处理

第十九版逻辑仍然在：

```cpp
if (!has_final_newline(path_) && !lines.empty()) {
    lines.pop_back();
    ++report.skipped_records;
    ++report.truncated_records;
}
```

这一步发生在 decode 之前。

也就是说：

```text
文件末尾没有换行
 -> 最后一行直接视为疑似半条记录
 -> 不尝试 checksum
 -> truncated_records +1
```

这保持了第十九版“尾部半条记录更保守”的行为。

---

# 25. CLI 启动输出更新

`src/main.cpp` 里启动 WAL 后输出：

```cpp
std::cout << "WAL: " << wal->path().string() << " (" << replay.applied_records
          << " records replayed, " << replay.skipped_records << " skipped, "
          << replay.truncated_records << " truncated, " << replay.checksum_failed_records
          << " checksum failed)\n";
```

旧输出只有：

```text
records replayed
skipped
truncated
```

新输出多了：

```text
checksum failed
```

例如：

```text
WAL: mini.wal (2 records replayed, 1 skipped, 0 truncated, 1 checksum failed)
```

---

# 26. server 启动日志更新

`src/server_main.cpp` 里：

```cpp
std::cout << "event=wal_replay path=" << quote_value(wal->path().string())
          << " records=" << replay.applied_records << " skipped=" << replay.skipped_records
          << " truncated=" << replay.truncated_records
          << " checksum_failed=" << replay.checksum_failed_records << '\n';
```

结构化日志新增字段：

```text
checksum_failed=
```

例如：

```text
event=wal_replay path="data/mini.wal" records=2 skipped=1 truncated=0 checksum_failed=1
```

这让服务端启动时也能观测 WAL checksum 问题。

---

# 27. 为什么 replay() 旧接口不用改

头文件里仍然有：

```cpp
std::size_t replay(Store& store) const;
```

实现仍然是：

```cpp
return replay_with_report(store).applied_records;
```

所以旧调用方只关心：

```text
成功应用了多少条记录
```

不需要改。

需要详细观测的入口用：

```cpp
replay_with_report()
```

---

# 28. 新格式和旧格式的兼容边界

兼容规则是：

```text
行以 "WAL2 " 开头
 -> 必须符合 WAL2 checksum 格式
 -> checksum 不匹配跳过

行不以 "WAL2 " 开头
 -> 按旧格式 naked record replay
 -> 由 replay_record 判断语法是否有效
```

这意味着旧 WAL 文件可以继续使用。

但如果旧文件里刚好有一条命令以：

```text
WAL2 
```

开头，就会被当成新格式。

当前 mini-kv 的 WAL 只写：

```text
SET
DEL
EXPIREAT
```

所以不会冲突。

---

# 29. WAL2 checksum 的安全边界

FNV-1a checksum 可以发现普通损坏。

但它不是安全认证。

它不能防止恶意攻击者：

```text
修改 payload
重新计算 checksum
写回 WAL
```

如果未来要防恶意篡改，需要：

```text
HMAC
密钥管理
签名
```

这不属于当前 mini-kv 的目标。

当前目标是：

```text
恢复时发现意外损坏
跳过坏记录
把数量报告出来
```

---

# 30. 第二十三版核心增删改

新增：

```text
WAL2 记录前缀
FNV-1a checksum
format_checksum
parse_checksum
encode_wal_record
decode_wal_record
checksum_failed_records
```

修改：

```text
WriteAheadLog::append
 -> 写裸 record
 -> 改为写 WAL2 checksum record

WriteAheadLog::replay_with_report
 -> replay_record 前先 decode
 -> checksum mismatch 跳过并计数

CLI / server WAL replay 输出
 -> 增加 checksum failed / checksum_failed
```

保持不变：

```text
CommandProcessor 调用 wal_->append 的方式
旧格式 WAL replay 兼容
尾部半条记录处理
replay() 返回 applied_records
```

---

# 第四十七次总结

第二十三版 WAL checksum 的核心是：

```text
append
 -> 原始 record
 -> FNV-1a checksum
 -> WAL2 checksum record 写入文件

replay
 -> 旧格式直接 replay
 -> WAL2 先验证 checksum
 -> mismatch 跳过并计入 checksum_failed_records
```

一句话总结：

```text
第二十三版把 WAL 从“只靠文本语法判断坏记录”推进到“新记录带 checksum，恢复时能发现并跳过 payload 被篡改或损坏的 WAL2 记录”，同时保持旧 WAL 文件可 replay。
```
