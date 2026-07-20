# v1674 产品源码清单与九分机械门代码讲解

## 本版目标、角色定位，以及它不是什么

v1674 的目标不是增加一个用户可见命令，而是把“哪些翻译单元组成 `minikv` 静态库”从根
`CMakeLists.txt` 的三百多行重复文本，提升为一份有结构、有校验、有反向盘点的源码所有权
清单。此前根脚本同时承担编译器策略、产品 target、五个可执行程序、测试策略、三百多条
产品源和三百多条测试注册，阅读者很难区分哪一行是策略、哪一行只是数据。v1673 已把
342 条普通测试身份迁入 `config/test-cases.txt`；v1674 完成另一半，把静态库源身份迁入
`config/product-sources.txt`，并用 `cmake/MinikvSources.cmake` 负责行为。

这不是动态插件系统，不在运行期扫描目录，也不会根据文件名自动执行未知代码。清单只在
CMake 配置阶段读取；配置成功后，`add_library(minikv ${MINIKV_PRODUCT_SOURCES})` 得到的
仍是迁移前同一批、同一顺序的翻译单元。它不是新的 router、命令分派器、网络入口、写入
入口或恢复入口；不改变 Store、WAL、Snapshot、RESP、TCP、OSFS，也不授予 mini-kv 订单、
审计、发布或审批 authority。产品 `src/*.cpp`、公共 `include/*.hpp` 与冻结 fixture 在本版
保持零改动。

## Step-0 为什么把 315 纠正成 316

计划初稿把 v1674 基线写成 315。真正执行前没有照抄这个数字，而是同时数了三份现实：

1. 根 `add_library(minikv ...)` 中的源条目；
2. 磁盘 `src/*.cpp` 扣除五个 executable main 后的集合；
3. v1672 提交新增的 `command_evidence_dispatch.cpp`。

前两份现实都等于 316，集合双向差集为空。追溯提交后可以解释差一的来源：v1671 时已有
315 个产品源，v1672 为 62 个只读 evidence 命令表合法新增一个 dispatch 翻译单元，因此
新基线应是 316。正确做法是把计划中的算术记录改正，而不是删除一个真实源文件去“满足
文档”。这个对账过程写入计划、证据摘要和讲解，后续看到 316 的人不必再次猜测。

## 为什么源码身份应该是数据，而构建行为必须留在 CMake 里

`config/product-sources.txt` 的每一行只表达一个事实：某个 `src/*.cpp` 属于 `minikv`
静态库。它没有条件语句、编译选项、平台分支或 target 行为。顺序沿用迁移前根脚本，从
`src/atomic_file_writer.cpp` 开始，以 `src/wal.cpp` 结束，共 316 行，文件由
`.gitattributes` 固定为 LF。数据文件的好处不是少写几个字符，而是让代码评审可以直接回答：
本版新增、删除或移动了哪个产品 owner，而不必从几百行 CMake 调用中辨认括号和缩进。

行为留在 `cmake/MinikvSources.cmake`。入口函数是：

```cmake
minikv_read_product_sources(
    "${PROJECT_SOURCE_DIR}/config/product-sources.txt"
    "${PROJECT_SOURCE_DIR}"
    MINIKV_PRODUCT_SOURCES
)
```

它的输入是清单路径和源码根目录，输出是保序的 `MINIKV_PRODUCT_SOURCES` 列表。调用者不
接触 parser 细节，只把输出交给 `add_library`。这种分界使数据可以机械比较，行为可以被
集中测试，而根脚本只保留项目级意图。迁移后根 `CMakeLists.txt` 从 436 个物理行降到 127
行；不是把 316 行塞进另一个 2000 行模块，新 source loader 只有 62 行，共享行尾模块 12
行，测试模块 257 行，suite 模块 135 行，全部低于 500 行门槛。

## loader 如何从输入走到输出

`minikv_read_product_sources` 按以下顺序 fail closed：

第一步，调用 `minikv_require_lf_file` 做字节级行尾检查。它把文件读取为十六进制字节序列，
再按两个 hex 字符切成一个真实字节，检查是否出现 `0d`。这里故意不对文本内容直接搜索
回车，因为 CMake 的文本读取可能先规范化 CRLF；也不在整串 hex 中随意查 `0d`，因为跨
字节半个 nibble 的偶然组合会产生假阳性。这个共享函数同时服务测试清单与产品源清单，
消除了两份近似但语义不完全一致的实现。

第二步，逐行验证路径。合法形态只能是 `src/[A-Za-z0-9_]+.cpp`，所以目录穿越、子目录、
反斜杠、空行、注释、其他扩展名都不能悄悄进入。五个 main 文件被单独列为
`MINIKV_PRODUCT_MAIN_SOURCES`：`main.cpp`、`server_main.cpp`、`client_main.cpp`、
`benchmark_main.cpp` 和 `osfs_main.cpp` 各自属于 executable，不得混入静态库造成重复
main 或错误 ownership。

第三步，验证每个文件真实存在，并用已收集列表拒绝重复。随后要求清单条目数精确为 316。
数量门不是完整性的全部证据，因为“删一个旧源、加一个错误源”仍可能保持 316，所以最后
还有反向盘点：CMake 用 glob 获取磁盘全部 `src/*.cpp`，移除五个 main，对磁盘集合和清单
集合分别排序后比较。这样会同时抓住两类漂移：清单写了已经删除的 stale source，以及磁盘
新出现但没有声明 owner 的 unregistered source。glob 使用 `CONFIGURE_DEPENDS`，普通构建
目录新增 `.cpp` 时会触发重新配置；script mode 则使用普通 glob，便于故障注入独立运行。

校验全部通过后，输出仍使用原始清单顺序，而不是排序后的集合。排序只用于证明集合相等，
不会改变静态库归档顺序。`f/1674/证据/product-sources-before.txt` 保存迁移前根脚本顺序，
迁移后清单会另存 after oracle；两份 316 行文本逐项相同，给本次重构提供可复现的迁移证据。

## 一次具体输入与输出示例

假设清单尾部是：

```text
src/snapshot.cpp
src/store.cpp
src/tcp_server.cpp
src/wal.cpp
```

配置命令读取后，输出列表尾部保持同样顺序。`add_library` 只接收这份列表，不扫描 fixture、
tests、archive 或生成目录。若维护者新建 `src/cache_index.cpp` 却忘记加入清单，下一次配置
会在创建产品 target 前报告 `product source manifest has an unregistered or stale source`；
若把 `src/main.cpp` 加进清单，会报告 executable main 不允许进入；若复制一行，会在数量
比较之前直接报告 duplicate。错误发生在构建控制面最早能解释的位置，而不是链接阶段出现
模糊的缺符号或重复符号错误。

## 六类真实红路径保护什么

本版没有只在 C++ 测试里模拟 parser。`cmake/check_product_sources.cmake` 可以在 script
mode 独立调用真实 loader，故障注入分别证明：

- CRLF 文件被字节级 LF 门拒绝；
- 把最后一项改成第一项会触发重复源错误；
- 把 `.cpp` 改成 `.c` 会触发扩展名错误；
- 写入不存在的 `src/not_real.cpp` 会触发缺失文件错误；
- 少一行会触发 316 census 错误；
- 在临时源码根额外创建 `src/unexpected.cpp` 会触发未登记磁盘源错误。

永久的 `test_architecture_contract` 又用独立 C++ parser 覆盖 CRLF、错误扩展名、main 混入、
缺失、重复与不完整清单。CMake loader 和 C++ gate 不是复制同一段实现后互相自证：前者决定
真实配置是否成立，后者从仓库结构角度验证配置文件、边界数量和模块 wiring。任何一边被
误改，另一边仍可给出不同来源的失败证据。

## v1673 CI 为什么红，以及 v1674 怎样真正修复

v1673 本地默认/legacy 都能生成同序 354 个测试名，但远端 macOS 和 Windows 暴露了两个
设计缺口。macOS 使用的 CMake generator 输出了不同语法的 `CTestTestfile.cmake`，旧架构
测试把生成器内部文本当稳定接口解析，结果误报 `generated CTest name/order drift`。Windows
则在 `--timeout 120` 下让架构测试超时；该测试对 342 个 manifest source 做存在性检查，
随后 target 检查又做一遍，最后为了 main contract 再读取全部测试源，hosted runner 的磁盘
与杀毒开销被重复放大。

macOS 修复没有添加第二套 parser。`minikv_add_test` 成为唯一真实 `add_test` 包装器：它先
验证名称安全和不重复，再调用 CMake 内建命令，并把名称追加到全局 `MINIKV_CTEST_NAMES`
属性。suite 最后把该属性按插入顺序写到构建目录。CMake 的 `DIRECTORY TESTS` 属性经过实测
会按字母排序，因此没有被误用；全局属性明确保存注册顺序。生成文件在 Windows 可使用
CRLF、在 Unix 使用 LF，C++ 端按行解析并比较 354 个安全名称，不把构建目录的本地行尾当
公共合同。该序列与 v1673 oracle 逐项相同。

Windows 修复也不只是把 timeout 从 120 改成 300。`scan_test_sources` 现在每个测试源只读
一次，同时得到非空行数、是否包含无参数 main、main 是否显式 `return 0`，并生成内存中的
路径集合。manifest 和 target 校验复用该集合，不再发出数百次重复 `is_regular_file`；本机
focused 运行由约一秒进一步稳定在 0.4 到 0.9 秒。300 秒 test property 只是 hosted 环境的
最后上界，不是性能问题的遮羞布。

## 发布断言为什么要改 owner，而不是改期望

`release_manifest_checks.cpp` 过去从根 CMake 查找
`command_contracts.cpp`、`managed_audit_receipts.cpp` 等关键 owner。产品源身份迁走后，继续
读根脚本会造成 stale assertion。v1674 保留所有原关键文件断言，只把数据来源切换到
`config/product-sources.txt`；根脚本仍断言必须 include source module，并通过
`${MINIKV_PRODUCT_SOURCES}` 创建库。第一次全量 CTest 中，最终证据合同也因为仍搜索旧
`NAME receipt_builder_census_contract` 语法而失败；修复同样没有删除测试，而是要求 suite
通过 `minikv_add_test` 注册原名称，并要求包装器函数存在。测试期望与运行行为都没有为迁移
让步。

## 本版测试层次与每类断言的意义

配置层验证清单可读、316 census、五 main 隔离和磁盘集合完整。架构测试验证独立 parser、
六类红路径、根脚本 150 行、四个参与模块 500 行、354 名称顺序、342 test case、8 shard、
62 evidence command 与既有 test source/name ratchet。发布测试验证关键产品 owner 仍在清单，
并继续运行真实 CLI receipt 断言。focused 组覆盖架构、release manifest、Store、Command 和
no-network receipt，证明构建治理没有绕开核心与边界测试。全量 354 条覆盖 WAL、Snapshot、
RESP、TCP、OSFS、fixture、archive 与治理合同；Release legacy 拓扑会再次证明优化构建中
`assert` 没被 `NDEBUG` 静默关闭。最后的真实 CLI/WAL reopen smoke 会写入临时 WAL、退出、
重开并读取同一 key，同时运行只读 `SMOKEJSON`/`CHECKJSON`，随后删除临时数据。

## 边界字段和控制面应怎样理解

本版不新增 JSON 字段，因此 `read_only`、`execution_allowed`、`order_authoritative`、
`mutates_store`、`touches_wal`、`warnings`、`blockers` 与 `diagnostics` 的公共含义没有变化。
构建 manifest 也不能被 Node 或 Java 当成运行授权：它只是“哪些 C++ 文件组成静态库”的
编译期事实。只读 evidence 命令仍由 v1672 固定表查找，参数校验仍在 formatter 前执行；
真实 `SET` 仍会按原规则写 Store/WAL，`CHECKJSON SET ...` 仍只解释边界而不执行。没有 active
router、managed-audit connection、credential value、raw endpoint、自动启动或跨项目写入。

## 九分评分为什么是 9.1，而不是自称满分

评分表把满分拆成八类。合同安全 1.9、架构职责 1.7、重复与数据建模 1.2、构建测试 1.5、
防回流 1.0、跨平台 CI 1.0、文档证据 0.4、范围克制 0.4，总计 9.1。扣分是有意保留的：
仓库仍有 735 个历史长文件名、883 个公共 header 可见长标识符和 277 个长内部 test target；
它们受 shrink-only baseline 约束，但不能因为本轮没增长就宣称已经消失。课程型 KV/OSFS
也不是生产数据库，文档和自评不能替代外部 review。

更重要的是，CI 的 1.0 分只有 v1674 commit 对应的 Linux、macOS、Windows、sanitizer、
coverage、format 与 archive 七条 job 全绿后才生效。提交前本地即使全部通过，也只能记
8.1，不能宣布九分到点。这个条件使评分本身成为可失败门，而不是作者写在 Markdown 里的
赞美。

## 后续新增、删除或重命名源码时怎样维护

精确 316 census 不是禁止项目继续增长，而是要求增长同时说明所有权。以后新增普通产品
翻译单元时，应先判断它是否真有独立职责：如果只是第三份结构相似 formatter，应先扩展
既有 engine 或把差异写成数据；如果确实形成新的 owner，再把 `.cpp` 放入 `src/`，在清单
中选择与依赖和职责相邻的位置登记，并把 `MINIKV_PRODUCT_SOURCE_COUNT` 与架构常量一起
从 316 收紧到新现实值。配置阶段的磁盘反查会保证“只加文件不登记”立刻失败，而 code
review 能从一行 manifest diff 看出新增 owner。

删除源码时顺序相反：先把调用者迁走并证明公共符号和行为已由其他 owner 接管，再删除
磁盘文件及清单行，同时下调精确 census。只删磁盘文件会在存在性检查失败，只删清单行会
在反向集合检查失败，因此不存在半迁移后仍能绿色提交的路径。重命名则视为一次删除加一次
新增；如果名字属于冻结公共合同或 Node 的历史绝对路径证据，不应借本清单重构擅自改名，
必须另开合同迁移计划。

新增第六个 executable main 也不能直接塞进产品清单。维护者要在根 CMake 显式创建新的
可执行 target，并把它的 main 路径加入排除集合；这样“库的组成”和“程序入口”仍是两种
不同事实。若根脚本因此接近 150 行，应该提取一个有明确概念的小模块并同步体积门，而不
是放宽 ratchet。模块若接近 500 行，也应先检查其中是否混入大块数据或第三个相似 family。

每次清单变化至少需要三份证据：配置成功说明 loader 接受新现实，架构测试说明集合、体积
和红路径仍有效，真实 focused/full 测试说明链接结果和运行合同没有退化。只有纯重排但集合
不变时，还要解释为什么需要改变静态库输入顺序，并冻结前后 oracle；没有明确收益的重排
应被拒绝，因为它只制造审计噪声。这样的维护流程让 exact count 成为变化的审计点，而不是
阻止变化的魔法数字。

## 一句话价值

v1674 让 mini-kv 的产品源码、测试身份和只读 evidence 分派分别由清晰的数据模型承载，
同时把真实构建行为、运行边界与跨平台验证留在各自 owner 中，为后续 Node、Java、mini-kv
松耦合对齐提供一份可审计、无新增执行权且不依赖生成器偶然文本的 C++ 构建基线。
