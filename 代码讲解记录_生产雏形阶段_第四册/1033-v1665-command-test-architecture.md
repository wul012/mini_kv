# v1665：命令测试架构拆分与可失败的体积门

## 一、本版目标、角色定位和明确不做什么

v1665 处理的是测试架构，不是 mini-kv 的产品功能。版本开始时，`tests/command_tests.cpp` 有 4009 个物理行、3805 个非空行；它覆盖基础键值命令、过期、WAL、快照、命令目录、分片只读证据、跨项目回执和运行时指标，测试价值很高，但单个文件已经大到无法快速定位职责。另一个 `tests/shard_readiness_tests.cpp` 也有 4328 个非空行。本轮先解决 command 单体，并把第二个单体留给 v1666，避免把两个高风险拆分塞进同一版本。

这个版本的角色是“保持所有观察结果不变，只改变测试代码的组织方式”。它不是新的命令入口，不增加 router，不打开网络，不写 Store 或 WAL，不执行 LOAD、RESTORE、COMPACT、部署或回滚，不修改任何 `fixtures/release/*.json`，也不改变 Node、Java 或 OSFS 的权限边界。拆分成功的标准不是“新文件能编译”，而是原来测试过的内容一项不少、原来共享的状态仍然共享、原来的执行顺序仍然可见，而且以后不能悄悄长回另一个四千行文件。

因此，本版先写 `治理计划/v1665-test-architecture-brief.md`，冻结起点、断言数、体积数和失败条件，再做迁移。这里的关键判断是：测试代码同样属于长期维护代码，不能因为它“不进入生产二进制”就容忍无限膨胀；但测试拆分也不能追求表面上的短文件，而破坏它原本在验证的历史状态链。

## 二、为什么不能直接切成九个独立测试

最直觉的做法是把旧文件按行号切成九个 `.cpp`，每个文件各自创建 `Store` 和 `CommandProcessor`，再注册成九条 CTest。这个做法文件看起来最干净，却会改变测试语义。旧 `main()` 从头到尾只创建一次 Store 和 processor，后续很多断言依赖前面留下的状态。例如基础命令先写入和删除键，快照测试会清空并重新装载，运行时统计最后仍会看到既定的 live key 数；某些 metrics、uptime、命令 breakdown 和只读 evidence 断言也依赖同一个 processor 生命周期。

初次拆分时，聚焦测试确实暴露了这个问题：如果每个 part 都有独立 fixture，运行时部分看到的 store 大小与旧测试不一致。这个失败不是测试“太脆”，而是在提醒维护者：旧单体包含一条隐含的状态机。正确修复不是把期待值改成新数字，也不是在每个 part 里手工造出刚好能通过的状态，而是把原语义建模出来。

`tests/command_test_suites.hpp` 因此定义了共享 `CommandFixture`。它只拥有三样东西：一个 `Store`、引用该 Store 构造的 `CommandProcessor`，以及沿途复用的 `CommandResult`。新的 `tests/command_tests.cpp` 仍然只创建一次 fixture，再按旧源码顺序调用九个入口。每个 part 接收 `CommandFixture&`，通过引用继续操作同一对象。这种设计把过去藏在四千行 `main()` 里的生命周期变成显式契约：谁拥有状态、状态活多久、各段为什么不能任意并行，一眼就能看懂。

这也解释了为什么本版没有把九个 part 注册成九条 CTest。它们是一个测试场景的九个职责段，不是九个彼此隔离的测试进程。目标仍是 `command_tests` 一条 CTest，只是编译时由多个 translation unit 共同组成。这样既保留旧语义，也让编译错误、链接入口和文件职责变得局部。

## 三、manifest 如何把“文件清单”升级为架构合同

拆出多个源文件后，最常见的维护事故不是编译失败，而是“文件存在但没有运行”。例如维护者新增一个 part，却忘记把它加入 CMake；或者 CMake 编译了它，却忘记在 suite main 中调用入口。只靠人工 README 清单无法阻止这种漂移，所以 v1665 引入 `config/command-test-parts.txt`。

manifest 每行只有两个字段：源文件路径和唯一入口函数。它不保存 C++ 行为，也不复制断言；数据与行为分开。`cmake/MinikvTesting.cmake` 中的 `minikv_add_test_parts` 读取相同 manifest，拒绝列数错误和不存在的路径，再把所有 part 追加到现有 `minikv_command_tests` target。CMake 因而不再维护一份容易漏改的九文件列表。

同一份 manifest 还被 `tests/test_architecture_tests.cpp` 读取。结构合同逐行检查路径和入口都非空、路径不重复、入口名不重复；每个 part 必须恰好定义一次形如 `void entry(CommandFixture& fixture)` 的函数，suite main 必须恰好调用一次 `entry(fixture);`。这使“编译进来了但没执行”“调用了两次”“复制了同名入口”“manifest 写错路径”都成为会失败的 CTest，而不是代码评审时才可能发现的问题。

这个 family 在出现第三个文件以前就已经拥有共享抽象：manifest 是数据，`minikv_add_test_parts` 是构建 engine，`verify_suite` 是结构 engine，`CommandFixture` 是状态边界，九个 part 只是 case。它遵守三次规则，没有为每个 part 复制一套 runner、CMake target 或夹具代码。

## 四、九个 part 各自保护什么

`command_core_tests.cpp` 保留基础命令、TTL、SETNXEX、快照、WAL、INFO、COMMANDS 和核心错误路径。它是命令系统的基础行为层，仍会真实修改测试 Store；这里的 write 是单元测试内部对 KV 引擎功能的验证，不代表跨项目 evidence 获得写权限。

`command_shard_route_tests.cpp` 保护分片路由预览、校验报告和归档预览等较早一段只读合同。`command_shard_material_tests.cpp` 聚合候选材料、提交预检和 profile section 一类字段。`command_shard_receipt_tests.cpp` 聚合审批、回执、capture artifact 和 closeout evidence。`command_shard_supply_tests.cpp`、`command_shard_compat_tests.cpp`、`command_shard_freeze_tests.cpp` 和 `command_shard_closeout_tests.cpp` 分别覆盖 value supply、兼容交接、fixture 冻结以及最终审核收口链。它们按原始出现顺序排列，没有按名字重新排序，因为顺序本身也是迁移 oracle 的一部分。

`command_runtime_tests.cpp` 收住最后的运行时行为：请求统计、失败计数、延迟 bucket、真实 WAL 执行路径、有限 KEYSJSON inventory、参数错误、未知命令和 QUIT。它继续看到前八段留下的同一 Store/processor 状态，因此可以证明共享夹具不是形式上的引用，而是恢复了旧单体的完整生命周期。

拆分后的最大 part 为 576 个非空行，其余依次处在 568、548、546、494、317、275、273、259 行范围。这个大小不是为了追求每个文件一样长，而是按领域边界切分后的自然结果。短的 supply/compat/freeze part 没有被强行合并，因为它们在命令证据链中承担不同维护职责；长的 core/route part 也没有继续切到几十行，因为已经低于机械上限且可以通过命令域快速导航。

## 五、断言 census 为什么比“测试通过”更重要

重构后的测试通过，只能证明保留下来的断言没有失败，不能证明迁移时没有漏掉一段。假设某个 part 根本没有加入 manifest，同时那段源码也从旧文件删除，剩余测试仍可能全绿。因此简报先在原单体上统计两类稳定 token：457 个 `assert(` 和 2447 个 `assert_response_contains(`。

`test_architecture_contract` 会遍历 manifest 中所有 part，重新统计这两个 token，并要求 map 与冻结值完全相等。少一个会失败，多一个同样会失败。这里先使用精确等值而不是“至少不少于”，因为 v1665 是纯迁移版本，新增断言也会混淆“是否原样迁移”的判断。未来需要增加真实行为测试时，可以在同一功能版本中明确更新 census，并用新增测试意图解释增长；不能无声漂移。

两类 token 的职责也不同。普通 `assert` 包含精确响应、布尔状态、计数、文件存在和时间范围；`assert_response_contains` 主要保护巨大 JSON evidence 中的字段和边界短语。完整保留二者，意味着基础 KV 行为和 read-only 控制面证据都没有因为文件拆分而缩水。版本没有修改测试期待或 fixture 字节来求绿，而是让旧断言文本机械迁移。

## 六、1000 非空行体积门和 shrink-only baseline

文件拆完之后若没有后续约束，新的功能很可能继续往某个 part 末尾追加，几年后又回到四千行。`config/test-source-size-baseline.txt` 和 `test_architecture_contract` 因此共同建立 1000 非空行上限。扫描器递归读取所有 `tests/*.cpp`，只计算非空物理行，避免格式化空行影响结果，也不依赖平台换行符。

起点只有两个超限项：command 3805、shard readiness 4328。baseline 是精确值而不是宽松的“最多五千行”。如果超限文件增长一行，扫描器要求“同一提交必须 shrink”并失败；如果新文件超过 1000 行但没有 baseline，立即失败；如果文件已经降到 1000 以下却仍保留 baseline，按 stale entry 失败；如果 baseline 指向不存在的文件，也失败。合同内部还主动构造 1001 行无 baseline、1002 对 1001 增长、999 行仍留 baseline 三条红路径，证明错误分支不是写在代码里从未执行。

v1665 完成后，command 的 3805 项从 baseline 删除，只剩 shard-readiness 4328 一项。合同输出 `sources=353 limit=1000 oversized=1`，其中 sources 表示被扫描的测试 `.cpp` 文件数量，不是 CTest 数量；当前 CTest 注册数恰好也是 353，但两个 353 的含义不同，文档必须明确区分。v1666 拆完 shard 后 baseline 应只保留注释、oversized 归零；门槛不能回调，也不能给新单体新增豁免。

## 七、MinGW 的 0xc0000135 为什么一并修复

本版全量验证时出现了四个 Windows 本机失败：benchmark evidence、OSFS CLI smoke、CLI log-level smoke 和 server invalid-level smoke 返回 `0xc0000135`。直接运行某些测试、或手工把 MinGW `bin` 放进当前 PowerShell PATH 时可以通过，说明这不是产品逻辑失败，而是 CTest 子进程找不到 MinGW 运行时 DLL。若只在验证命令前手工设置 PATH，下一位维护者或 IDE 仍会重复踩坑；若复制 DLL 到 build 目录，又会制造难以清理的二进制副本。

`minikv_configure_test_runtime(test_name)` 读取当前 `CMAKE_CXX_COMPILER` 所在目录，并仅在 `MINGW` 下给已注册测试设置 `ENVIRONMENT_MODIFICATION`，把该目录 prepend 到测试进程 PATH。通用的 linked test 和 standalone source-dir test helper 自动调用它；四个历史手写 `add_test` 也显式调用同一 helper。Linux、macOS 和非 MinGW 编译器不进入该分支，命令参数、PASS_REGULAR_EXPRESSION、WILL_FAIL 属性和 timeout 都没有改变。

这个改动不是跳过测试，也不是把失败标成允许失败。恰恰相反，它让 CTest 在 Windows 上能实际启动原测试程序。四个曾失败的测试随后 4/4 通过；没有复制 DLL，没有关闭检查，没有改变 stdout 正则。它属于测试基础设施的可重复性修复，与 command 文件拆分共同服务于“从任意支持环境得到同一验证结果”。

## 八、边界字段和上下游应如何理解

命令 part 中大量 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers` 和 `diagnostics` 断言仍然存在。它们不是说 mini-kv 整体没有 SET、DEL 或 WAL；而是说特定 evidence/preview 命令只能描述风险和准备状态，不能借“查看证据”执行恢复、路由、写入或审批动作。

`read_only:true` 表示该证据调用本身只读。`execution_allowed:false` 表示返回一个计划、回执或 preflight 不等于获得执行许可。`order_authoritative:false` 表示 mini-kv 不是 Java 订单业务的权威来源。`mutates_store:false` 和 `touches_wal:false` 约束对应命令不会顺手修改 KV 或追加 WAL。warnings/blockers/diagnostics 则把缺失条件和未发生动作公开给 Node 等下游消费者，而不是用成功状态掩盖边界。

v1665 没有改这些字段的值、顺序或拼写。Node 若读取冻结 fixture 或真实只读输出，看到的字节和语义都不变；Java 不需要配合升级；OSFS 独立可执行文件也没有改动。唯一新增的“输出”是测试架构合同的本机摘要，它只服务维护者，不是公共协议。

## 九、验证分层与本版价值

聚焦层先运行 `test_architecture_contract` 和 `command_tests`。前者证明文件结构、调用图、断言 census 和体积门，后者真实执行全部 command 行为。环境修复层单独重跑四个曾返回 0xc0000135 的测试，证明 CTest 子进程确实能加载。全量层随后覆盖 Store、WAL、Snapshot、RESP/TCP、OSFS、fixture digest、receipt parity、文档 honesty、coverage/CI contract 和所有历史证据链。真实 CLI 层再输入 `SMOKEJSON`、`CHECKJSON GET demo`、`HEALTH`、`QUIT`，结构化检查只读、no execution、no authority、no write-command 和 no WAL-touch 边界。

归档只保存结论性截图和摘要，不保存数百行 build/CTest 原始日志，也不把 build 二进制放进 `f/1665`。讲解写在最终 verify 前，使最终验证会覆盖文档合同；commit/tag/push 后，中间版只确认 CI 已启动，v1666 开头再检查它的结论。若远端红灯，下一版首先修 CI，不能让连续推进掩盖失败。

一句话总结：v1665 没有增加一条产品命令，却把“一个人才能维护的四千行测试脚本”变成由共享状态、数据清单、精确断言和只减不增体积门共同约束的测试架构，为 Node、Java 与 mini-kv 的松耦合证据链保留了原字节和原边界，同时显著降低后续定位和扩展成本。
