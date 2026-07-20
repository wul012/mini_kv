# v1668：把回执边界从重复动作变成有序数据

## 一、本版解决的不是“代码能不能跑”，而是“代码为什么值得继续维护”

v1656 已经完成 27 个 runtime receipt formatter 的 ordered builder 迁移，机械统计为 27 个 builder-backed owner、0 个 pending owner、1 个具名 no-formatter waiver。从功能角度看，这条链已经稳定：冻结 fixture 没被改写，公开命令仍输出原来的 JSON，摘要值和兼容规则都有测试保护，所有写入、路由、网络、凭据读取和执行权限也保持关闭。

但“全部用了 builder”不等于“内部已经优雅”。重新统计 receipt 实现后，可以看到 28 个 receipt 源文件和相关私有 helper 中仍有 1056 次 `fields.push_back(...)`。其中相当一部分并不是领域上各不相同的字段，而是反复声明 `read_only=true`、`execution_allowed=false`、`mutates_store=false`、`touches_wal=false` 这一类稳定边界。同一组事实被几十行命令式代码重复展开，读者要逐行检查名字、值和顺序，维护者也容易漏掉其中一项。

所以 v1668 的目标不是增加一种回执、不是增加执行入口，也不是改写旧输出。它只做一件结构性工作：把稳定的布尔边界表示为有顺序的数据 profile，再交给唯一的 builder 行为追加。formatter 仍拥有自己的领域内容和字段插入位置，profile 不知道 Node 版本、fixture 路径、摘要或命令语义。这样既减少重复，也不把所有 receipt 塞进一个万能 schema。

本版明确不做以下事情：不添加 router，不触发 Store 写入，不接触 WAL，不发起 DNS、HTTP 或 TCP，不读取 credential value，不执行 restore、load 或 compact，不自动启动 Node、Java 或 mini-kv，也不取得订单、审批或审计 authority。`BooleanField` 是内存中的描述数据，不是控制命令，更不是可以执行的计划。

## 二、起点为什么必须先量出来

原来的 `receipt_builder_census_contract` 只统计 formatter owner：一个 owner 包含 builder 头文件，就被视为已经迁移。这条门对防止 formatter 退回整段字符串拼接很有效，却看不到 builder 内部的细粒度重复。若继续只看 27/0/1，后续即使再复制数百个 `fields.push_back(...)`，owner census 也仍然会显示“全部完成”。

v1668 因此把统计范围扩展为两层。第一层保留原有的 28 个 receipt 源、27 个 formatter owner、27 个 builder-backed、0 pending、1 waiver。第二层扫描这些 `.cpp` 以及 receipt 私有支撑 `.hpp`，一共 31 个实现文件，精确统计两种待还债务：手工字段追加和本地 `receipt_digest` 包装器。迁移前分别是 1056 和 6，迁移四个高重复 helper 后分别是 814 和 6。

这里最重要的不是“少了 242 行调用”这个数字，而是数字的语义可复算。1056 到 814 不是根据 diff 行数估算，也不是人工在说明里填写，而是 CMake 脚本读取最终源文件后匹配实际调用。格式化导致的行移动、空行或注释不会被算成优雅度提升。只有命令式追加真正消失，计数才会下降。

## 三、核心抽象：数据和行为各自只做一件事

入口位于 `src/runtime_receipt_json_builder.hpp`。新增的 `BooleanField` 只有两个成员：`name` 表示 JSON 字段名，`value` 表示布尔值。它没有虚函数、继承、receipt 类型枚举和版本判断，也不持有输出缓冲区。它只是一个很薄的数据记录。

与之配套的 `append_boolean_fields` 位于同一 builder 的实现文件。函数接收目标 `std::vector<OrderedJsonField>` 和一个有序 `span`，先为目标向量预留空间，再按 profile 给出的顺序逐项把布尔值编码成 JSON value。另一个 `initializer_list` 重载只负责让短 profile 的调用更自然，最终仍收口到同一份追加逻辑。行为没有分叉，字段顺序也没有被容器重新排序。

为什么不直接使用 `std::map`？因为 receipt 的字节合同包含字段顺序，按 key 排序会改变输出。为什么不做一个接收几十个 bool 参数的万能函数？因为调用点会变成一串难以辨认的 `true, false, false`，字段含义反而消失。为什么不让 profile 直接返回完整 JSON？那会重新制造字符串片段，并夺走 formatter 对插入位置和 nested object 的所有权。当前设计刻意保持边界：profile 只声明“哪些布尔事实按什么顺序出现”，builder 只声明“怎样编码并追加”。

## 四、四个迁移族分别表达什么

### 1. credential common boundary

`runtime_credential_resolver_receipt_utils.hpp` 原来逐项追加一组所有 credential resolver receipt 共用的关闭边界。v1668 把它拆成 `common_credential_boundary_profile` 和 `closed_runtime_shell_profile`。前者说明凭据值、原始 endpoint、网络和存储权限处于什么状态；后者说明 runtime shell、写路径、WAL 和执行入口仍关闭。

两个 profile 分开而不是合成一张超大表，是因为它们的语义责任不同，调用点也可能只需要其中一段。formatter 仍在自己的专属字段之后调用 helper，因此历史插入点没有变化。profile 名控制在命名预算内，并直接表达概念，不再靠一个超长函数名把所有字段名串起来。

### 2. disabled runtime-shell family

`runtime_credential_resolver_disabled_runtime_shell_receipt_fields.hpp` 中重复最多的是 runtime-shell readiness 与 non-participation tail。它们过去以长串 push 表达“只提供证据，不连接、不执行、不接管 authority”。现在分别成为 `runtime_shell_readiness_profile` 和 `non_participation_tail_profile`。

这里不能把两段合并，因为中间仍有 family 专属字段。单元测试不仅要看最终集合包含哪些 key，还要保护 profile 被插入到前后字段之间时的顺序。这样未来维护者如果把 tail 提前，exact/canonical parity oracle 会在字节层失败，而不是等下游消费方发现摘要漂移。

### 2.1 `constexpr`、`span` 和生命周期为什么这样组合

这些 profile 使用静态 `constexpr` 数组，因为字段名和值在编译期已经确定，不需要每次生成 receipt 时重新分配一组描述对象。调用函数接收 `std::span<const BooleanField>`，它只借用连续数据，不复制 profile，也不取得所有权。数组具有静态生命周期，因此 span 在调用期间始终有效；builder 只在函数内部遍历，不保存 span，更不会把借用指针泄漏到 formatter 之外。

`append_boolean_fields` 会先按 profile 大小调用 `reserve`，这不是为了追逐微小性能数字，而是让一次批量追加具有明确的容量意图，避免向量扩容细节淹没领域代码。真正写入目标向量的是普通 `OrderedJsonField`，字段名被复制为拥有所有权的字符串，布尔值也立刻编码成 JSON 文本。因此 profile 只是声明模板，最终 receipt 不依赖临时对象或隐式全局状态。

如果未来某个字段值必须在运行时计算，就不应该把它硬塞进静态 profile。formatter 应先显式计算该值，再作为普通字段追加，或者建立一个名称清楚、输入清楚的领域函数。这个限制防止 profile 从“稳定事实表”逐渐膨胀成带条件、回调和副作用的脚本系统。优雅不是把所有代码数据化，而是让稳定数据与动态决策各自处在最容易理解的位置。

### 3. sandbox closed-side-effect family

`runtime_sandbox_receipt_utils.hpp` 原来有三段稳定事实：只读前缀、关闭写入、关闭执行。v1668 对应建立 `read_only_prefix_profile`、`closed_write_profile` 和 `closed_execution_profile`。它们看起来都包含很多 false，但这些 false 并不是噪声，而是控制面需要显式读取的安全承诺。

把它们变成 profile 后，审阅者可以在一张紧凑表中检查字段和值，formatter 代码则突出自己的 sandbox 元数据、请求形状和诊断内容。若某个未来需求真的要打开写入或执行，它不能悄悄修改通用 helper，因为所有使用该 profile 的 receipt oracle 都会同时暴露变化；正确做法是先重新定义领域合同并获得明确授权。

### 4. manual evidence family

`runtime_evidence_receipts.cpp` 持有 manual dry-run 和 manual precheck 两份较大的 receipt。v1668 只提取其中稳定且重复的 `manual_dry_run_boundary_profile` 与 `manual_precheck_boundary_profile`，没有搬走它们各自的 metadata、diagnostics、warnings 或兼容差异。

manual dry-run 仍保留 v1653 冻结的三字段 metadata 规则；本版没有把它扩大成新的 canonical waiver。换句话说，profile 迁移不能靠修改 fixture 或放宽比较器来通过。回执的字段顺序、字段值、顶层结构和 digest 仍由原 baseline 测试裁决。

## 五、机械门为什么同时拒绝“变坏”和“忘记记录变好”

`cmake/check_receipt_builder_census.cmake` 新增 `EXPECTED_MANUAL_FIELD_PUSHES` 与 `EXPECTED_LOCAL_DIGEST_WRAPPERS` 两个必传参数。脚本计算实际值后分三种情况处理。

第一，实际值大于 baseline，说明有人重新引入了命令式重复或本地摘要包装器，直接失败。第二，实际值等于 baseline，说明当前代码与账本一致，通过。第三，实际值小于 baseline，代码本身可能变好了，但门仍然失败并要求把 baseline 同步收紧。第三种设计很关键：如果只设置“不得超过 814”，下一版把计数降到 700 却忘记改门，未来又涨回 814 仍会被判为绿色，已经偿还的债会悄悄复发。

本版使用两个故障注入验证这条语义。把期望值临时设为 813，当前实际 814 会触发 regression；把期望值临时设为 815，当前实际 814 会触发 stale baseline。两条命令都必须非零退出，且报错分别指出“实现增长”和“请把 815 收紧到 814”。这比只运行一次绿色脚本更有说服力，因为它证明门确实会在应当失败时失败。

6 个本地 `receipt_digest` 包装器本版没有为了凑数字强行迁移。它们进入同一个精确门，意味着数量不能增长；下一版真正把它们收口到共享实现时，必须在同一提交把 6 收紧到新的实际值。未改动的债被诚实记录，比把多个独立风险塞进同一版更符合版本闭环。

## 六、测试分别保护什么

`runtime_receipt_json_builder_tests` 增加 profile 专项用例。测试先追加普通标量字段，再插入同时含 true 和 false 的布尔 profile，最后追加尾字段，然后对完整 JSON 字节进行断言。它保护三件事：布尔编码不是字符串，profile 内顺序不变，profile 在周围字段中的插入位置不变。

`runtime_receipt_runtime_shell_family_baseline_tests` 保护 disabled runtime-shell 链的完整输出和历史 family 关系；`runtime_receipt_legacy_resolver_chain_baseline_tests` 保护 credential resolver 的冻结字节、字段数和摘要；`runtime_receipt_remaining_owner_baseline_tests` 保护 manual evidence 与 sandbox 链，包括具名兼容规则不能扩张。它们不是为 v1668 修改的新期望，而是迁移前已经存在的 oracle。

`receipt_builder_census_contract` 保护结构债务，`elegance_name_census_contract` 与 `test_architecture_contract` 继续确保这次改造没有用超长新名字、巨型文件或新的构建 target 债交换表面上的去重。最终全量 CTest 用于确认 Store、WAL、snapshot、RESP、TCP、OSFS 和历史 fixture 没有被内部 helper 变化波及；真实 CLI 则从最终二进制读取 `SMOKEJSON` 与 `CHECKJSON`，验证 `read_only=true`、`execution_allowed=false`、不写 WAL 和不执行恢复等关键边界仍然成立。

## 七、如何阅读本版输出和证据

这一版没有新增用户可见命令，因此输入仍是既有 CLI 命令，例如 `SMOKEJSON`、`CHECKJSON LOAD data/prod.snap` 和 `QUIT`。输出也应保持原样：`SMOKEJSON` 返回聚合后的运行证据，`CHECKJSON` 只检查请求及计划，不执行 load，`QUIT` 正常结束会话。

对控制面而言，`read_only=true` 表示该证据路径只读取或计算；`execution_allowed=false` 表示它不能把计划变成执行；`order_authoritative=false` 表示 mini-kv 不是订单权威；`mutates_store=false` 与 `touches_wal=false` 分别说明不会改变内存存储和 WAL。`warnings`、`blockers`、`diagnostics` 仍由具体 formatter 生成，profile 不解释也不吞掉它们。因而本版改善的是生产证据代码的组织方式，不是改变这些字段的业务含义。

归档中的 focused test、full CTest、CLI boundary 和 census 截图分别回答“新抽象正确吗”“全仓库是否回归”“真实入口边界是否仍关闭”“重复是否真的减少”。说明文档必须给出命令、关键输出和意义，而不是只放一张绿色终端图。历史 `f/1656` 仍证明 owner 迁移完成，`f/1668` 只证明后续内部优雅度治理，二者不能互相冒充。

## 八、维护者接下来应该怎样继续

剩余 814 次追加不是要求一次性清零。正确节奏是优先找字段集合稳定、在多个 formatter 中重复、并且已有 exact/canonical oracle 的 family；先写不超过十行的 family 设计说明，再提取数据 profile，最后同步收紧 baseline。领域上真正独有、只出现一次且顺序清楚的字段可以保留显式追加，避免为了追求零计数制造难以理解的配置语言。

下一类高价值债是 6 个本地 digest 包装器和若干结构相似的 approval/credential receipt。是否合并必须先比较它们的参数、字段所有权和兼容规则；只有行为确实相同时才收口到共享 engine。三次规则继续生效：第三个结构相似实现出现前先抽象，但不能预先造一个覆盖所有未来 receipt 的框架。

本版给 Node、Java 与 mini-kv 松耦合融合带来的价值可以用一句话概括：外部消费到的只读证据字节和权限边界不变，mini-kv 内部却把重复的安全事实变成了可审计、可复用、会被机械棘轮保护的数据模型。

## 九、最终验证记录

讲解在最终 verify 之前完成。随后同一棵 `cmake-build-v1668` 构建树完成全目标构建，退出码 0，耗时 606.1 秒；focused receipt 门 5/5 通过；全量 CTest 353/353 通过、0 失败、耗时 153.88 秒。真实 CLI 依次运行 `SMOKEJSON`、`CHECKJSON LOAD data/prod.snap` 和 `QUIT`，结构化断言确认只读、禁执行、禁恢复、非订单权威、无 write/admin/runtime-write、无 WAL touch，最后返回 `BYE`。

clang-format dry-run 通过，`fixtures/release` 与 `include/minikv` diff 为空。四张选择性截图由 Playwright MCP 生成，原始巨型 JSON、全量日志和构建二进制没有进入归档。第一次 CLI 摘要解析使用了本机 PowerShell 5 不支持的 `ConvertFrom-Json -Depth` 参数，参数解析失败后改用兼容语法读取同一份原始日志并通过；该偏差属于验证命令兼容性，不是产品行为失败。
