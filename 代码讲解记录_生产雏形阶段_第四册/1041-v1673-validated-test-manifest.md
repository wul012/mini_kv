# v1673 代码讲解：把 342 条测试身份变成受校验的数据

## 本版目标、角色，以及它不是什么

v1673 处理的是构建控制面的重复，不是运行时功能。重构前，根 `CMakeLists.txt` 同时承担项目选项、315 个产品翻译单元、五个可执行程序、九个特殊 smoke、三个 standalone 测试、342 个普通测试注册和若干 target 特例，物理长度达到 2,597 行。真正的问题并不是 CMake 语法本身，而是 342 段结构完全相同的调用把“测试是谁”和“怎样构建测试”混在了一起。维护者很难快速看出哪些内容是策略，哪些只是重复数据；新增或移动一条测试时，也容易漏掉名称唯一性、源文件存在性、source-dir 定义和 shard/legacy 双拓扑的一致性。

本版引入的抽象叫 `TestCaseManifest`。它负责保存普通测试的身份和顺序，`MinikvTesting.cmake` 负责解释这些身份，`MinikvTestSuite.cmake` 负责保留真正不规则的行为。它不是删除测试，不是把多个 CTest 合并成一个进程，不是改变测试期望，也不是重新定义 coverage 或 sanitizer 的运行方式。外部仍看到 354 个同名、同序的 CTest；默认模式仍由 8 个稳定 runner 执行一例一进程，显式 legacy 模式仍为每个普通 case 建立独立可执行程序。

本版也没有触碰 `src/`、`include/` 或 `fixtures/`，因此不会新增 router、写命令、网络访问、凭据读取、WAL/Snapshot/Restore 行为或执行权限。`read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal` 等运行证据字段的值和输出字节完全不在本版职责范围内。

## 为什么测试身份应该是数据，而测试行为必须留在代码里

重构前的一条普通注册大致如下：

```cmake
minikv_add_source_dir_test(
    minikv_release_verification_manifest_tests
    release_verification_manifest_tests
    tests/release_verification_manifest_tests.cpp
)
```

这四个事实中，`source_dir` 是构建种类，后面三项依次是私有 target、公开 CTest 名和主源文件。342 条调用的控制流完全一致，差异只在四个值。继续复制调用不会增加表达力，只会增加视觉噪声。于是 `config/test-cases.txt` 用五列保存事实：

```text
phase|kind|target|ctest_name|source
main|source_dir|minikv_release_verification_manifest_tests|release_verification_manifest_tests|tests/release_verification_manifest_tests.cpp
```

多出的 `phase` 不是随意标签。旧 CTest 顺序中，前八个普通 case 位于三个 CLI/OSFS smoke 之前，其余 334 个位于 smoke 之后。如果把 342 条一次性注册，测试集合虽然相同，公开顺序却会变化。因此清单只允许 `pre_smoke` 与 `main` 两个阶段：suite 先注册八条 `pre_smoke`，显式注册三个特殊 smoke，再注册 334 条 `main`。阶段只用于恢复原顺序，不能承载超时、环境变量、命令参数或其他行为。

行为必须留在代码里，是因为九个特殊测试并不共享统一形状。benchmark 带参数和正则，文档合同通过 `cmake -P` 运行不同脚本，OSFS smoke 需要镜像和脚本路径，server 非法日志级别测试带 `WILL_FAIL`。若强行把这些参数塞进万能表，会把简单的五列 schema 变成难懂的小语言。三个 standalone 测试也不链接 `minikv`，必须显式调用 standalone helper。command/shard suite 的 part manifest、runtime receipt 的私有 include、atomic writer 的 `src` include、elegance census 的编译定义同样是 target 行为，所以集中留在 130 行的 `MinikvTestSuite.cmake` 中。数据和行为的边界由此清楚：清单回答“普通 case 是谁、按什么顺序”，suite 回答“特殊 case 和 target 特例怎样构建”。

## 从 CMake 入口到 8 个 runner 的完整流程

根 `CMakeLists.txt` 现在只保留三行测试控制面入口：包含 `MinikvTesting.cmake`、启用 CTest、包含 `MinikvTestSuite.cmake`。配置阶段进入 suite 后，先创建 benchmark、文档合同和三个 standalone target，再调用：

```cmake
minikv_register_test_group("${minikv_test_manifest}" pre_smoke)
```

`minikv_register_test_group` 不直接相信文本。它先调用 `minikv_read_test_manifest` 读取整份清单，检查原始字节中是否存在 CR，逐行拆成五列，验证 phase/kind 白名单，验证 target 与 CTest 名只能使用安全标识符，验证 source 必须位于 `tests/` 且扩展名为 `.cpp`，再检查文件存在、target 唯一和 CTest 名唯一。最后它要求总数精确为 342、阶段精确为 8+334。任一条件不成立，CMake 在生成构建图之前就 `FATAL_ERROR`，不会留下“部分注册成功”的模糊状态。

解析通过后，当前阶段的 `linked` 行调用原有 `minikv_add_linked_test`，`source_dir` 行调用原有 `minikv_add_source_dir_test`。也就是说，本版没有复制 bundled/legacy 决策逻辑：默认 `MINIKV_BUNDLE_TESTS=ON` 时，旧 helper 继续把每个源建成 object target，用公开 CTest 名计算 SHA-256 前缀并分配到八个 shard；coverage、sanitizer 或显式 `MINIKV_BUNDLE_TESTS=OFF` 时，同一个 helper 继续建立独立 executable。两条拓扑读取同一份身份数据，不存在第二份清单。

当两个阶段都完成后，suite 为 command 和 shard target 追加原有 part 源，为 release manifest 验证追加本版拆出的静态检查源，为 atomic writer 追加私有 include，最后调用 `minikv_finalize_test_bundle()`。默认拓扑由全局属性收集 342 个 entry，再生成八个 runner；legacy 拓扑中 finalizer 立即返回。公开 CTest 仍逐条调用一个 case，不会在单进程里批量跑完多个测试，因此崩溃隔离、工作目录、环境变量和失败定位语义都保持不变。

## 顺序证明为什么不能只看“仍有 354 条”

“前后都是 354”只能证明数量，没有证明身份和顺序。v1673 在任何编辑前，用 v1672 树配置出旧 CTest 清单，把 `ctest -N` 的 354 个名称按原顺序写成 LF 文本。迁移后分别配置默认和 legacy 构建，再生成同格式文本并逐字节比较。三份文本均为 354 行，SHA-256 都是：

```text
f4ecf10465a185e4a3c04d5b0e0292ea47aba0a8d48e5992c7eb8d9159a3310e
```

这能发现删除、增加、改名和重排中的任何一种。永久架构门还会从 manifest 推导出完整顺序：先九个显式测试，再八个 `pre_smoke`，然后三个特殊 smoke，最后 334 个 `main`。测试运行时读取当前构建目录的 `CTestTestfile.cmake`，逐项对比生成顺序。这样以后有人交换 phase、把特殊 smoke 移到错误位置，或者在 suite 中绕过 manifest 追加普通 case，即使总数碰巧仍是 354，也会立即失败。

清单 schema 不是只写在文档里。CMake checker 对真实文件返回 342；故障注入分别制作 CRLF、未知 kind、重复 target 和缺失 source 四份临时清单，四次调用都必须非零退出并包含对应原因。C++ 架构测试再独立覆盖未知 phase、坏列数、重复 CTest 名和非法源路径。配置时的生产 parser 与测试中的结构模型互相校验：前者守住真实构建图，后者证明红路径确实存在。

## 一次失败怎样暴露旧的所有权假设

首次全量 CTest 得到 353/354，唯一失败是 `release_verification_manifest_tests`。它并不是产品行为回归，而是测试内部仍到根 `CMakeLists.txt` 查找若干 test target 名。目标身份已经迁入 `config/test-cases.txt`，旧断言因此准确地指出“所有权位置改变”。修复没有删除任何名称断言，也没有改 fixture：产品源仍在根 CMake，相关断言继续读根文件；测试 target 的全部原断言改为读取 manifest。这样测试验证的事实没有缩小，只是跟随事实的新 owner。

clang-format 随后又触发第二道门：被改动的 `release_verification_manifest_tests.cpp` 从原来的长行形态格式化后达到 1,090 个非空行，超过全仓库 1,000 行测试上限。最省事的做法是向 baseline 加一项，但那会违反 ratchet 只收紧和童子军规则。本版改为按职责拆分：`release_manifest_checks.cpp` 的 820 行只检查冻结 manifest、必需路径、CMake 产品源和测试身份；原文件的 280 行只运行真实 `INFOJSON`、`SMOKEJSON`、`STORAGEJSON` 与 GET 读路径。共享头只暴露 `verify_release_inputs` 和断言辅助函数，CMake 用 `target_sources` 把静态检查接回原 target。CTest 名、进程数和测试期望不变，零项尺寸豁免也继续为零。

这个插曲说明机械门的价值不只是“让 CI 变红”。测试注册重构碰到旧 owner 假设，所有权合同把它拦住；格式化使隐藏的体积问题显性化，尺寸合同又迫使代码真正拆开。若两次都用更新期望或增加 baseline 来绕过，根 CMake 虽然变短，整体设计反而会变差。

## 每一步的输入和输出是什么

从维护者视角看，配置阶段有一条很短但可追踪的数据管线。第一步的输入是 `config/test-cases.txt` 的原始字节，输出是经过验证的 342 条五列记录；若输入含 CRLF、坏列或重复身份，输出不是“尽量解析”，而是带原因的配置失败。第二步的输入是完整记录集和请求阶段，输出是当前阶段按原顺序调用的 8 条或 334 条注册；parser 每次都先验证全表，所以不能通过只读取某个阶段来掩盖另一个阶段的坏行。第三步的输入是 `kind`，输出是调用原有 linked 或 source-dir helper；这里不会自行决定 shard，也不会复制 compile definition。第四步的输入是 helper 创建的 object/executable target，输出是 CTest 注册和可选 shard 全局记录。最后 finalizer 的输入是 342 个全局 entry，输出才是八个 runner 源和八个 executable。

可以把它画成下面这条链：

```text
test-cases.txt 原始字节
        ↓  schema / LF / 唯一性 / 文件存在性
342 个普通 case（8 pre_smoke + 334 main）
        ↓  kind 分派，调用旧 helper
342 个 object target 或 342 个 legacy executable
        ↓  默认模式按公开 CTest 名稳定散列
8 个 runner + 342 条逐例 CTest
        ↓  插入 12 条显式 standalone/special
最终 354 条同名同序 CTest
```

运行阶段的输入仍是某一个公开 CTest 名。例如 CTest 调用 `release_verification_manifest_tests` 时，默认模式把这个名字传给它所属的 shard runner；runner 只查找并执行对应 entry，未知名字返回错误。legacy 模式直接运行同名 target 的 executable。两条路径的输出都是该 case 的原退出码和 stdout/stderr，不会把其他 case 的结果混入。manifest 自身在运行时不会被产品可执行程序读取，也不会成为动态插件目录，因此坏清单只能影响配置，不能改变线上命令分派。

## 为什么没有选择代码生成或万能 DSL

一种看似更彻底的方案，是由脚本从 tests 目录扫描所有 `.cpp` 自动生成 target。它的问题是文件名无法可靠表达 linked/source-dir、公开 CTest 名、特殊顺序和 target override；“约定优于配置”在这里会把关键差异藏起来。另一种方案是生成一份几百行 CMake 文件再 include，它虽然让根文件变短，却仍保留重复调用，而且生成结果与源数据容易发生双份所有权。v1673 的清单不生成需要提交的中间 CMake，构建图直接消费唯一数据源。

还可以把九个特殊测试也放进一套带可变参数的 DSL，例如增加 command、arguments、properties、environment、compile definitions 等列。这样 parser 很快会承担转义、列表嵌套和条件分支，维护者反而需要学习一门项目私有语言。当前边界刻意克制：只有出现 342 次且真正同构的普通身份才数据化；十二个不规则测试保持普通 CMake，任何人都能直接看到实际命令和属性。这个选择也符合三次规则的真正含义：消除稳定重复，而不是为了表驱动而把所有差异压平。

## 运行边界字段为什么仍要在讲解里明确

虽然本版没有改 JSON，构建重构仍可能间接漏测，所以边界不是一句“无行为变化”就结束。354 项列表中保留 no-network、abort/rollback、input-hardening、runtime no-start/no-write、restore boundary 和 release manifest 等 case；它们继续断言 `read_only=true`、`execution_allowed=false`、`order_authoritative=false`，以及 `mutates_store`、`touches_wal`、network、credential、router、restore 等禁止面。名称/顺序 oracle 证明这些 case 没有在搬迁中消失，full CTest 证明断言仍实际执行，v1672 的 `-UNDEBUG` 门又保证 Release 不会静默编译掉 `assert`。因此本版对边界的证据链是“身份仍在、执行仍发生、断言仍生效”，而不是重新生成一份容易自证的 JSON。

## 各类测试分别保护什么

`test_architecture_contract` 是本版主门。它解析真实清单，要求 342、8/334、245 linked/97 source-dir 的精确 census；校验 target/path 风险和 277 项历史长 target baseline；读取生成的 CTest 文件比较 354 项顺序；检查 parser、两个 group 调用、8-shard helper、legacy executable 路径和 Release 断言选项仍存在；并执行 schema、重复、缺文件、体积和路径预算红路径。

`release_verification_manifest_tests` 保护冻结 release fixture 与源码/测试所有权之间的联系。拆分后它仍检查原来每一个 fixture 字段、required path、产品翻译单元和 test target，只是静态检查与真实命令输出各自有了 owner。

`elegance_name_census_contract` 的安全候选证据也随 ownership 迁移：v1662 记录的那条长测试路径以前在根 CMake 中出现一次，现在必须在 `config/test-cases.txt` 中出现一次。TSV 的结论和 735/883 baseline 没变，只修正证据位置。

默认全量 CTest 验证 354/354；显式 legacy focused 选择 architecture、command part suite、release manifest 和 no-network receipt 四类代表测试，验证独立 executable 路径仍能编译运行。最终证据合同改为从 `MinikvTestSuite.cmake` 查找 builder census 与 docs honesty 特殊注册，同时 secret scan 扩到 `cmake/*.cmake`，避免构建逻辑拆出根文件后反而逃离高置信密钥扫描。

## 维护者以后怎样新增一条普通测试

若新测试只需要链接 `minikv`，在 `config/test-cases.txt` 的正确阶段新增一行 `linked`；若还需要 `MINIKV_SOURCE_DIR`，使用 `source_dir`。target 和公开 CTest 名不能重复，源文件必须已存在。通常新增项放在 `main`，只有确实必须位于三个 smoke 之前且能解释顺序合同，才可修改 `pre_smoke` census。新增后，342/334 等精确门必须在同一提交中有意识地收紧，生成顺序门会显示它插入的位置。

若测试需要独特命令参数、`WILL_FAIL`、脚本变量或不链接产品库，不要扩充 manifest 语法，应在 `MinikvTestSuite.cmake` 显式表达。若某个普通 target 需要附加源或 include，也在 suite 的 target-specific 区域追加行为，而不是把任意参数塞进数据行。这个判断标准让清单保持可读，也防止它演化成另一套隐式编程语言。

## 一句话价值

v1673 让 mini-kv 的测试身份成为可审计、可拒绝坏输入的单一数据源，同时保留所有特殊行为、双拓扑和 354 项公开合同，为后续 Node/Java/mini-kv 松耦合验证提供更清楚且不改变任何执行边界的构建底座。
