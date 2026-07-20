# v1671 测试链接分片：保留 354 个测试语义，只消除 342 次重复链接

## 1. 本版目标、角色和明确不做什么

v1671 处理的不是“测试太多”，而是“同一份产品静态库被重复链接太多次”。当前 mini-kv 有 314 个产品源文件，默认测试清单中有 342 个普通测试会链接 `minikv` 静态库。旧拓扑为每个测试生成一个独立可执行文件，因此一次产品源文件改动即使没有改变测试源码，也会让 Ninja 重新归档静态库并重新链接 342 个测试程序。测试覆盖面越完整，增量构建反而越慢；这是一种由链接拓扑造成的工程税，不是业务复杂度本身。

本版引入的抽象叫 `TestLinkShard`。它只存在于 CMake 的私有测试基础设施中：每个原测试仍单独编译成 object target，CTest 的公开名字、顺序、进程隔离、超时和属性都不改变，但最终链接被稳定收口到 8 个 shard runner。测试数量没有减少，断言没有合并到一个长生命周期进程里，fixture 没有改写，产品源码和运行输出也没有变化。

还要明确四件“不是什么”。第一，这不是新测试框架，项目仍使用现有的 `assert`、fixture 和 CTest。第二，这不是把 342 个 case 放进一个进程连续执行；每次 CTest 仍启动一个新 runner 进程，而且 runner 只分派命令行指定的一个 case。第三，这不是 coverage 或 sanitizer 的新执行模型；两条 instrumented lane 自动退回旧的一测试一可执行文件拓扑。第四，这不是公开 API 或运行证据版本，`INFOJSON`、`SMOKEJSON`、`CHECKJSON`、WAL、Snapshot、RESP、TCP 和 OSFS 的字节与权限边界均不参与本次改动。

## 2. 先量化问题，而不是凭感觉重构

短路径 Ninja 干净构建的旧基线有 1033 个步骤、350 次可执行文件链接，耗时 151.44 秒。全部 345 个旧测试可执行文件共占 3,983,351,060 字节，约 3.71 GiB。为了测增量成本，本版只 touch `src/atomic_file_writer.cpp`，随后确认文件内容摘要没有变化；旧拓扑仍执行 349 个 Ninja 步骤，其中 347 次是可执行文件链接，耗时 56.87 秒。

这里的关键不是绝对秒数，因为本机负载会波动，而是工作结构：一个产品对象变化导致三百多次相同静态库链接。只优化编译并行度或启用缓存只能缩短一部分时间，不能消除重复的最终链接。反过来，如果为了数字好看而删测试、合并 CTest 名称或在同一进程连续运行多个 case，又会牺牲失败定位和状态隔离。因此采用门同时约束收益和语义：核心 touch 后的 executable link 不得超过 16 次，耗时至少下降 50%，测试二进制至少下降 70%，354 个 CTest 名称和顺序必须逐项相同，完整 CTest 不得比 v1670 的 180.07 秒慢 25% 以上。

最终默认拓扑的干净构建为 707 个步骤、16 次可执行文件链接、130.15 秒；核心 touch 后为 15 个步骤、13 次可执行文件链接、21.36 秒。相对旧基线，增量步骤下降 95.7%，增量链接下降 96.3%，增量耗时下降 62.4%。测试可执行文件由 345 个/3,983,351,060 字节降到 11 个/415,219,566 字节，体积下降约 89.6%。最终完整 CTest 为 354/354，耗时 130.96 秒，不仅没有越过退化门，还比 v1670 的参考值更短。这里没有把时间波动包装成稳定性能承诺；真正可长期复算的收益是链接次数、目标数量和二进制体积。

## 3. CMake 入口：默认分片，instrumented 自动回退

入口位于 `CMakeLists.txt` 顶部的 `MINIKV_BUNDLE_TESTS`。普通配置默认打开；在新构建目录中，当 `MINIKV_SANITIZE` 或 `MINIKV_COVERAGE` 打开时，默认值变为关闭。若复用曾缓存 bundle ON 的目录，或同时显式要求 instrumented 和 bundle，配置阶段会立即报错，而不是悄悄选择一个语义；维护者应显式关闭 bundle 或使用独立的插桩目录。这个红路已经真实运行，退出码为 1，错误明确要求 `MINIKV_BUNDLE_TESTS=OFF`。

为什么 instrumented 不复用 shard？coverage 会为每个编译单元生成并合并 gcov 侧车数据，sanitizer 也会在进程启动时注册整份镜像的全局对象和插桩元数据。把几十个测试对象链接到同一 runner 虽然仍只调用一个入口，却会把未执行 case 的对象一并放入镜像，改变 coverage 分母和 sanitizer 初始化面。普通三系统 CI 购买链接收益，coverage/sanitizer 则购买最容易解释的一测试一镜像隔离；两条路径由同一份 CMake helper 维护，不靠 CI 文档约定。

需要临时恢复旧开发体验时，也可以在普通构建中显式使用 `-DMINIKV_BUNDLE_TESTS=OFF`。v1671 对这条路径做了真实配置、编译和 CTest，而不是只检查 option 字符串。legacy 与默认模式均注册 354 个测试，名称和顺序差异为零；架构合同、receipt builder、command 和 atomic-file 四个代表测试在两种拓扑下分别 4/4 通过。这也说明旧的单测试可执行文件仍可作为诊断逃生通道，只是不再是普通全量构建的默认成本。

## 4. 每个测试仍有自己的 object target

`cmake/MinikvTesting.cmake` 没有把 342 个源文件粗暴拼成八个大源文件。`minikv_add_bundled_test` 仍以原来的内部 target 名建立 `OBJECT` library，并继续对这个 target 施加 `target_compile_definitions`、`target_include_directories`、`target_sources` 和 `target_link_libraries` 的 usage requirements。这样 `runtime_receipt_json_builder_tests` 的私有 `src/` include、`atomic_file_tests` 的私有实现 include，以及 command/shard manifest 追加的 suite part 都留在原 owner 上。

这层保留非常重要。若直接让八个 runner 吞下全部源文件，target-specific include 或编译定义容易在迁移中丢失，测试可能不是编译失败，而是走到错误的条件分支。Object target 让“编译身份”和“最终链接身份”分开：前者仍是一测试一 target，后者才按 shard 聚合。v1667 已经建立的内部 target 长名与对象路径预算也因此继续有效，而公开 CTest 名从未被内部短名替代。

原测试中的 `main` 通过 target 私有宏改名为十二位 SHA 前缀生成的 `minikv_case_<hash>`。名字长度受控，且不会把历史名词串继续复制到符号表。配置阶段维护全局 entry 集合，出现 hash 碰撞会直接失败。分片索引使用同一 SHA-256 的前六位对 8 取模；这比按注册序号每 43 个切片更稳定，因为中间新增一个测试不会让其后的全部 case 重排。当前八个 shard 的分布是 53、43、35、42、47、48、31、43，总和严格为 342，没有空 shard。

## 5. runner 为什么仍保持一测试一进程

`cmake/minikv_test_runner.cpp.in` 是一个很小的生成模板。CMake 为每个 shard 写入入口声明和 `{CTest 名称, 函数指针}` 表，再生成 `minikv_test_shard_0` 到 `minikv_test_shard_7`。CTest 注册命令形如 `minikv_test_shard_3 some_test_name`。runner 检查参数必须恰好为一个测试名，线性查找本 shard 的表，找到后立即 `return test.entry()`；它不会循环执行其余 case，也不会把一次进程里的 Store、静态计数器、临时目录或环境变量留给下一条 CTest。

缺少参数和未知 case 都返回 2，且输出不同的错误信息。本版真实执行了两个红路：无参数输出 usage，未知名字输出 `unknown bundled test case`。这保护了一个容易被忽略的风险：若未知名字静默成功，CTest 注册拼写漂移就会产生“绿色但根本没跑”的空测试。runner 返回测试入口的原始退出码，因此断言失败或显式非零返回仍直接传给 CTest。

CTest 的 `WILL_FAIL`、`PASS_REGULAR_EXPRESSION`、超时和 MinGW PATH 注入属于测试注册属性，不属于 executable。v1671 没有重建或搬移这些属性，只把 `COMMAND` 从原 target 改为 shard target 加测试名。前后 `ctest -N` 输出都提取了 354 个名称，并按原序逐项比较，差异为零。换句话说，测试发现、筛选、失败定位和 CI 报告的公开坐标没有变化，变化只发生在链接器看见的最终镜像数量上。

## 6. 为什么必须修改 108 个旧测试的 return

C++ 对真正名为 `main` 的函数有一条特殊规则：执行流到达函数末尾等价于 `return 0`。宏把 `main` 改名为普通 entry 后，这条特殊规则不再适用。第一轮原型编译准确暴露了这个差异：两个测试报出“非 void 函数执行到末尾”的警告。压掉警告会让普通函数落入未定义返回路径；让 runner 忽略返回值也不能消除函数本身的未定义行为。

因此本版对所有无参测试入口做了完整 census：344 个 `int main()` 必须以显式 `return 0;` 结束，其中 108 个历史文件原先依赖特殊 main 语义，本版各增加一条返回。`test_architecture_contract` 每次扫描测试源，任何无参入口缺少显式返回都会失败；观察数量不是 344 也会要求同步更新合同。这样以后新增可 bundle 的测试不会重复踩坑，standalone 无参测试也采用相同清晰规则。

触碰 108 个历史文件后，changed-files format gate 同时发现其中 79 个带有旧格式债。按照童子军规则，本版没有给这些文件申请豁免，而是对全部 109 个改动 C++ 文件执行同一 `clang-format`。测试目录最终呈现 988 行新增、648 行删除，净增 340；除架构合同与 108 个显式返回外，其余都是机械换行和缩进。没有 fixture、字符串常量内容、断言期望或产品源文件因格式清理而改变。最终 dry-run `--Werror` 对 109 个文件全绿。

## 7. 架构合同具体保护什么

`tests/test_architecture_tests.cpp` 现在同时检查七类事实。第一，CMake 中普通 linked/source-dir helper 调用恰好为 342，防止新增测试忘记进入分片合同。第二，shard 数固定为 8，option、instrumented 冲突门和最终器调用都必须存在。第三，helper 必须保留 SHA 映射、object target、main 改名、legacy executable 路径和 shard target。第四，runner 必须保留参数拒绝、单入口分派和未知 case 红路。

第五，344 个无参 main 必须显式返回；这是从原型失败提升成永久规则。第六，原有 345 个 CMake helper 注册、277 个存量长内部 target、197 对象路径风险分数、1000 非空行上限和零尺寸豁免继续检查。第七，command 与 shard suite 的 manifest、调用图和 457/2447、8/3710/2 断言 census 不因链接重构而弱化。合同自身输出 `linked_cases=342 shards=8 explicit_mains=344`，让评审者无需读 CMake 也能复算当前拓扑。

这里特意没有把耗时写成硬编码 CTest 断言，因为 CI 主机和本机调度会波动；时间采用版本证据和采用门审查。名称、数量、源结构、红路和边界则是确定性事实，适合机械检查。把稳定事实与环境测量分开，能避免“为了守住一次跑分而让测试随机红”的伪严格。

## 8. 原型偏差与为什么保留记录

实现过程中有两次正常但值得记录的原型失败。第一次 runner 模板直接使用 `@VAR@` 占位，`clang-format` 把未展开片段当成 C++ 语法处理，生成文件仍残留标记。修复方式不是绕开格式门，而是把占位点改成合法注释，再由 CMake 读取模板、精确替换两个注释标记并执行 `file(CONFIGURE)`。最终八个生成 runner 的残留标记数为零。

第二次架构测试使用了 `std::string_view::contains`，但项目标准是 C++20，该成员属于更新标准。编译红灯后改为 `find(...) == npos`，没有提高语言标准或增加兼容宏。两次偏差都发生在最终验证之前，且都产生了更明确的实现规则；它们不是测试失败豁免，也没有修改断言来迁就实现。

## 9. 输入、输出与维护者操作方式

默认输入仍是熟悉的三条命令：`cmake -S . -B build`、`cmake --build build`、`ctest --test-dir build`。配置输出新增一行 `mini-kv test link topology: 342 cases / 8 stable shards`，用于确认默认拓扑。构建目录里不再出现 342 个普通测试 exe，而是八个 shard runner、三个 standalone 测试程序和五个产品程序；CTest 输出仍列出 354 个原名称。

若维护者需要直接运行旧的某个测试 exe，输入改为 `cmake -S . -B build-legacy -DMINIKV_BUNDLE_TESTS=OFF`，随后构建对应 target。新建目录并输入 `MINIKV_COVERAGE=ON` 或 `MINIKV_SANITIZE=ON` 时，CMake 默认选择 legacy；若旧 cache 或命令行仍把 bundle 设为 ON，则配置失败。这个行为写入 `docs/TESTING.md`，避免用户把“默认不再生成每个 exe”误判为测试被删除。

本版输出的价值可以概括为：测试行为和公开坐标不变，普通开发循环少做三百多次链接；需要插桩或单 exe 调试时仍有明确、真实验证过的旧路径。

## 10. 边界与对松耦合协作的价值

v1671 没有触碰 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers` 或 `diagnostics` 的运行含义。它也没有添加 router、网络调用、凭据读取、写路由、恢复/压缩执行入口，既不启动 Node/Java，也不改变 mini-kv 作为存储与证据提供方的职责。真实 CLI smoke 仍会检查只读证据命令不写 Store、不触碰 WAL、不获得订单 authority。

对 Node、Java、mini-kv 的松耦合协作而言，本版最直接的价值不是新增一个跨项目字段，而是让 C++ 侧在保持 354 条证据坐标完全稳定的前提下显著缩短本地和三系统普通构建成本，从而让后续契约审查更快、更愿意频繁运行，同时不把 coverage、sanitizer 或失败隔离作为交换条件。
