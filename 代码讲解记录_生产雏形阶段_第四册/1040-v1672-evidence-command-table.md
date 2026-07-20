# v1672 代码讲解：把只读证据命令从条件长链变成受约束的数据表

## 一、本版目标、角色定位，以及它不是什么

v1672 解决的是 `src/command.cpp` 中一种很典型的后期维护问题：系统功能没有坏，测试也
足够多，但同一种分派动作被重复写了几十次。旧实现中，从
`SHARDROUTEVERIFYREPORTCLOSEOUTJSON` 到 `COMMANDSJSON`，共有 62 个无参数只读命令，
每个分支都重复三步：比较命令名、检查是否多传参数、调用一个 formatter。随着跨项目
证据版本不断增加，这段代码最终形成了数百行 `if (command == ...)`。单个分支很容易读，
但整体不容易审计：想确认是否有重名、漏注册、先执行后校验，维护者只能逐段扫描。

本版把这 62 条稳定映射抽成私有 `EvidenceCommand` 表，让“哪个命令调用哪个 formatter”
成为数据，让“怎样查找、何时校验、怎样执行”只保留一份行为。它的角色只是命令层内部
的只读 formatter 索引，不是新的网络 router，不是插件系统，不允许运行时注册，也没有
获得写入、WAL、restore、credential 或订单权威。公共命令名、usage 文本、JSON 字节、
fixture、digest 和 `read_only` 等边界字段都必须原样保留。

## 二、入口在哪里，输入和输出分别是什么

用户入口仍是 `CommandProcessor::execute()`。例如输入：

```text
SHARDROUTEARCHIVEMAINTJSON
```

外层先去除空白并取得大写 command token，随后 `execute_trimmed()` 根据既有
`CommandDispatchVerb::RuntimeEvidence` 把请求交给
`CommandProcessor::execute_runtime_evidence_command()`。v1672 没有更改这两层，因此
普通 `GET`、`SET`、`SAVE`、`LOAD`、`COMPACT`、`EXPLAINJSON`、`CHECKJSON` 的解析和执行
路径完全不受影响。

对表内命令，运行时 evidence 方法现在只做下面这段统一流程：

```cpp
if (const auto* evidence = command_internal::find_evidence_command(command)) {
    if (has_extra_token(input)) {
        return usage(evidence->name);
    }
    return {evidence->format()};
}
```

输入是已经规范化的大写命令名和仍可读取的参数流。查找结果不是字符串响应，而是一条
只读描述：`name` 加 `std::string (*)()` formatter 指针。只有确认该命令存在且参数流中
没有额外 token 后，代码才调用 formatter。输出仍是原 formatter 返回的 `std::string`，
再按原样放进 `CommandResult.response`。这一顺序很关键：如果先生成证据再判断多余参数，
即使最终返回 usage 错误，也可能浪费大量计算，甚至让未来带缓存的 formatter 发生不该
发生的观察性动作。v1672 明确保留“先校验，后格式化”。

## 三、三个模块怎样分工

### 1. `command_evidence_dispatch.hpp`：最小私有契约

头文件只声明 `EvidenceFormatter`、`EvidenceCommand`、`evidence_commands()` 和
`find_evidence_command()`。它放在 `src/` 而不是公共 `include/minikv/`，表示这不是对外
API。`EvidenceCommand` 没有布尔权限、优先级或回调上下文，也不携带 `Store`、WAL、网络
客户端或 credential provider。这样做是刻意限制抽象能力：本版需要的只是无参数、无
状态 formatter 映射，不能借“通用化”之名造出可以执行任意操作的命令框架。

### 2. `command_evidence_dispatch.cpp`：数据 owner

该文件包含 62 个 formatter 对应的声明头，并建立一个编译期 `std::array`。表的顺序沿用
旧 `if` 链顺序，方便前后 oracle 对照和人工追溯。每一项都是命令字符串与原函数地址，
没有包装 lambda，没有第二份业务判断，也没有重新实现 formatter。复杂的公共长字符串
是已经发布的命令合同，本版不尝试改短；内部新增的 `EvidenceCommand`、
`EvidenceFormatter`、`unique_names` 等标识符都在 40 字符预算内。

数组后有两个编译期门：`commands.size() == 62` 固定迁移边界，`unique_names()` 两两比较
命令名并由 `static_assert` 阻止重复。查找函数使用一次 `std::find_if`，返回表中原元素的
地址，不复制 formatter，也不执行 formatter。62 项的线性查找成本相对于生成数 KB 到
数 MB 证据 JSON 的成本可以忽略；这里没有引入哈希表和静态初始化顺序复杂度，优先保持
确定性与可审计性。

### 3. `command.cpp`：保留有状态编排

并非所有 runtime evidence 命令都适合进表。`STATS`、`STATSJSON`、`SMOKEJSON`、
`STORAGEJSON`、`HEALTH` 需要读取 Store、WAL maintenance report、metrics 或 connection
stats；`INFO` 与 `INFOJSON` 需要 runtime info；`SHARDROUTE`、`SHARDROUTEVERIFYJSON`、
`SHARDROUTEVERIFYREPORTJSON` 需要一个 key 参数。这些分支继续由 `CommandProcessor`
持有，因为它拥有所需上下文和输入解析职责。把它们硬塞进无参数函数表，反而要引入捕获
对象、`std::function` 或通用 context bag，会让边界更模糊。

所以本版不是把全部条件语句赶尽杀绝，而是把同构的 62 项数据提走，让剩下 12 个
`if (command == ...)` 各自代表真实差异。结果是 `command.cpp` 从 732 个非空行降到 258，
新 dispatch 实现为 268 个物理行。旧单体缩小了约 65%，新文件也远低于巨型文件门槛，
职责边界比“换一个文件继续堆 if”清楚得多。

## 四、为什么公共行为能保持字节一致

最强的证据不是“所有函数名看起来一样”，而是迁移前后使用同一真实可执行程序输入做
整体比较。迁移前从 v1671 构建 CLI，按旧分支顺序提取 62 个命令，再追加 `QUIT`。这份
输入有 3,463 字节，SHA-256 为
`2e0b69c14ceaa1097b9b601a1133743248008525c94174e1c645822330d7455c`。CLI 产生 159 行、
2,093,273 字节的规范化输出，SHA-256 为
`b60a2ff0106c8803f879feb4034eed00c35b6bb319169382c5a577339c52f5b4`。

迁移后命令列表不再从旧条件链提取，而是从新 `EvidenceCommand` 表提取。条目数仍为 62，
去重后仍为 62，首项和末项仍相同。使用同一输入运行新 CLI，输出行数、字节数和整体
SHA-256 三项全部相等。这个比较覆盖了 formatter 返回的完整文本，而不只是抽查几个
`read_only` 片段。fixture 文件本身没有改动，已有 command、shard、receipt、release
manifest 测试又从结构和历史 digest 两侧提供了第二层保护。

参数错误也保持原样。旧分支写 `usage("COMMANDSJSON")`，新代码写
`usage(evidence->name)`；因为表项名称就是查找到的原命令名，返回仍是
`ERR usage: COMMANDSJSON`。对于未知名称，查表返回 `nullptr`，函数继续落到原有
`ERR unknown command`。没有表项会遮蔽带 key 命令，因为有状态分支在查表之前处理，且
表内根本不登记它们。

## 五、边界字段在控制面中应怎样理解

表驱动只改变 C++ 内部的选择方式，不改变证据含义。控制面读到 `read_only=true`，仍表示
该命令只形成观察证据；`execution_allowed=false` 表示它不能把计划升级为真实执行；
`order_authoritative=false` 表示 mini-kv 不成为 Java 订单事实源；`write_command=false`
和 `touches_wal=false` 表示本次只读命令既不改 Store，也不追加 WAL。涉及 receipt 的
`warnings`、`blockers`、`diagnostics` 仍由各自 formatter 负责，dispatch 表没有解释或
改写字段的权力。

因此，Node 或 Java 以后消费这些响应时，仍应把它们视为松耦合证据，而不是调用授权。
表中出现某个命令只说明 CLI 能格式化对应证据，不表示 router 已打开、网络已连接、凭据
已读取、rollback 已执行或生产动作已经发生。

## 六、测试分别保护什么

第一层是编译期结构检查。62 项数量和命令名唯一性由 `static_assert` 固定；函数指针类型
要求所有表项确实是无参数并返回字符串的 formatter。传入需要上下文的函数会直接编译
失败，不会在运行时靠约定猜测。

第二层是 command suite。`command_core_tests.cpp` 遍历整张表，验证每个 formatter 非空、
每个命令都存在于公开 `command_catalog`、集合插入不重复、按名称查找返回原表元素，并
验证 `UNKNOWN` 查找失败。这把“表”和“对外命令目录”连成双向可审计关系。新增的 6 条
断言同步把 command suite 的 `assert(` census 从 457 收紧到 463；没有删除旧断言来维持
漂亮数字。

第三层是 `test_architecture_contract`。它把 `command.cpp` 非空行上限钉在 260、同类
command 分支上限钉在 12、表项精确钉在 62，并要求两个编译期断言仍存在。以后有人把
第 63 个同构分支重新写回 `command.cpp`，或者绕过表另起一套分派，会在 CTest 中得到
明确错误，而不是等下一次人工优雅评审才发现。

第四层是真实运行与全量回归。legacy focused 构建验证 table、command suite 和架构门；
迁移前后 CLI 整体摘要证明 2 MB 级输出字节一致；标准默认拓扑执行全部 354 个 CTest，
覆盖 command、fixture、SHARDJSON、SMOKEJSON、CHECKJSON、WAL、snapshot、TCP 和 OSFS。
这些层次分别回答“结构是否正确”“目录是否完整”“字节是否一致”“全系统是否回归”。

## 七、验证中发现的 Release 断言问题

本版预验收第一次使用 `CMAKE_BUILD_TYPE=Release`。CMake 自动加入 `-DNDEBUG`，仓库测试
大量使用的标准 `assert` 因而被关闭。OSFS 测试在 `OPEN` 返回不符合预期时，本应先由
`assert(marker != npos)` 报出准确位置；断言关闭后继续把错误子串交给 `std::stoi`，最终
只得到模糊的 `invalid_argument`。这不是产品逻辑回归，却揭示 Release 测试过去可能
“看似运行、实际少断言”的证据缺口。

修复放在 `MinikvTesting.cmake` 的共享 target helper 中：MSVC 使用 `/UNDEBUG`，其他编译器
使用 `-UNDEBUG`，并让 bundled object、legacy executable、standalone test 三条创建路径
全部复用。它不会影响五个产品 executable，只保证任何构建类型下的测试断言都有效。
架构合同同时要求这个 helper 和 GCC/Clang 选项存在。修复后，同一 Release 配置下
OSFS、OSFS resilience、command 和架构合同 4/4 通过，形成真实红转绿，而不是切回默认
配置回避问题。

## 八、维护者以后怎样新增命令

如果新命令是无参数、无状态、只返回 formatter 字符串，先在 command catalog 声明公共
契约，再在 `commands` 表增加一项，并补对应 formatter 行为测试。数量变化必须同时更新
精确 census，且需要说明为何它属于现有抽象。如果新命令需要 key、Store、WAL 或连接
统计，不要把 context 强塞进本表，应让拥有上下文的 owner 显式处理，或先提取新的、边界
清楚的命令族。任何新增命令都必须继续满足 no router、no write、no credential、
no execution 的项目边界，除非未来有单独获批的合同迁移计划。

## 九、用一次真实维护场景理解审查成本为什么下降

假设下一版要增加一个只读的 `SHARDDELIVERYCHECKJSON`。在旧结构里，审查者先要确认
`command_catalog.cpp` 已声明命令，再到七百多行的 `command.cpp` 中寻找合适插入点，检查
前后几十个相似分支有没有命令拼写重复、usage 串错抄、formatter 调错 namespace，最后
还要确认新分支位于 unknown fallback 之前。由于每个分支都把数据和控制流揉在一起，
代码差异看似只有八行，审查者实际必须理解周围数百行，才能排除顺序和复制错误。

在 v1672 之后，同一变更的审查路径更短而且更机械。第一步在 catalog 中确认它的 category
是 read、`mutates_store=false`、`touches_wal=false`；第二步确认 formatter 自己只组装证据；
第三步在 `commands` 数组增加命令和函数地址。编译期唯一性会阻止重名，command suite 会
阻止 catalog 漏项，精确 62 项 census 会要求开发者把“为什么现在是 63”写入版本说明，
真实 CLI 测试负责输出。审查者不再需要通过插入位置推断优先级，因为带状态和带参数命令
根本不在同一个数据集合中。

这个变化也改善删除流程。若某条历史命令在未来经过正式废弃窗口移除，开发者必须同时
修改 catalog、表项、相关 formatter 测试和精确 census。只删表项会被数量门拦住，只删
catalog 会被 catalog membership 断言拦住，只删 formatter 会链接失败。也就是说，旧结构
依赖维护者记住四个同步点，新结构让四个同步点之间形成可以失败的联系。

## 十、为什么选择固定数组，而不是哈希表、巨型 switch 或运行时注册

`std::unordered_map` 可以把平均查找复杂度写成常数，但它需要动态初始化、哈希和节点或
桶存储；更重要的是，遍历顺序不直接表达历史发布顺序。这里每条命令最终会生成较大的
证据文本，几十次字符串比较相对于 formatter 成本极小，所以为了理论上的查找复杂度
引入动态容器没有实际收益。固定数组能在编译期检查数量和重复，内存布局连续，初始化
确定，阅读差异时也一眼能看到命令与 formatter 的对应关系。

大 `switch` 并不适合字符串，需要先建立另一层枚举映射；若把 62 个命令都加入枚举，
维护者仍要在 catalog、字符串到枚举、枚举到 formatter 三处同步，重复反而更多。把命令
散列成整数再 switch 还会引入碰撞证明和难懂的生成代码，不符合本项目“证据先于技巧”
的方向。

运行时注册最不合适。它会让静态初始化顺序、重复注册处理和插件生命周期进入问题域，
甚至可能允许某个翻译单元悄悄扩张命令能力。mini-kv 当前需要的是可冻结、可枚举、不能
动态扩权的只读表。固定 `std::array` 的能力刚好够用，少一分通用性反而多一分边界清晰。

## 十一、故障模型与每层机械门的报错位置

如果有人复制一项却忘记改命令名，`unique_names()` 在编译阶段失败；如果改了命令名但
没登记 catalog，`command_tests` 在遍历表时失败；如果把需要参数的函数错误塞进表，函数
指针类型或行为测试失败；如果又在 `command.cpp` 手写同构分支，12 分支或 260 行 ratchet
失败；如果 formatter 选错但类型相同，2,093,273 字节 oracle、现有 fixture/digest 测试
或对应 command 测试负责发现内容变化。不同门针对不同错误，没有用一个模糊的“全量测试
通过”替代设计约束。

Release 断言修复也遵循同样思路。只在本机改构建参数无法防止下一位维护者重现问题；把
`-UNDEBUG` 写进共享 target helper，三条测试创建路径都会自动继承。架构合同再检查 helper
调用和编译选项文本，使未来重写 CMake 时不能无声删除。若 MSVC 与 GCC/Clang 分支写反，
三系统 CI 会继续提供平台层证据。这样，一次偶然的本地配置失败被转化成长期、跨配置的
质量资产。

从九分评分角度看，本版真正购买的是“架构职责”和“重复与数据建模”两项，而不是用减少
行数直接换分。行数下降只是结果；决定得分的是数据与行为已分离、特殊命令保留明确 owner、
每种常见回归都有对应机械门、外部字节经过独立 oracle 证明不变。若只有 732 到 258 的
数字而没有这些约束，本版最多算一次文件搬运，不能算优雅提升。

一句话总结：v1672 用一张编译期只读表替代 62 份重复控制流，在完整保留 CLI 字节和权限
边界的同时，让 Node、Java、mini-kv 的松耦合证据入口更容易审计、扩展和防回流。
