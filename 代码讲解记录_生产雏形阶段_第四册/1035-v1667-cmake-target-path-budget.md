# v1667：把 CMake 对象路径警告变成可持续收紧的构建合同

## 一、本版到底解决什么问题

v1667 的改动表面上很小：`CMakeLists.txt` 里一个内部 target 从
`minikv_text_package_review_closeout_audit_validation_tests` 改成
`minikv_tp_closeout_validation_tests`。如果只看这一行，很容易误以为这只是一次无关紧要的
缩写。但真正的问题并不是名字看起来长，而是干净配置 `cmake-build-v1666` 时，CMake 明确
发出对象文件路径风险警告：某个 target 的构建目录已经占去 107 个字符，再叠加 142 字符
的测试源路径和 `.obj` 后缀，生成器无法保证对象文件可以安全放置。这个警告在 Linux 上
未必立刻失败，在 Windows、不同工作目录深度或不同生成器下却可能从提示升级成无法编译。

因此，本版的角色是“构建身份与路径预算治理”，不是产品功能版本，也不是再开一轮全仓库
改名。它不增加命令，不改变 KV、WAL、snapshot、RESP、TCP 或 OSFS 行为；不修改任何
release fixture；不改变公开 CTest 名；也不重新命名 v1662 已审计并冻结的 132 字符测试
源文件。它做的是把一次真实警告追到形成机理，再留下会失败的机械合同，防止同类风险以
另一个 target 名重新出现。

本版尤其不是什么：不是通过设置更大的 `CMAKE_OBJECT_PATH_MAX` 把风险藏起来，不是过滤
CMake stderr，不是把长文件移出扫描目录，不是把完整 CTest 名压成难以检索的编号，也不是
推翻 Claude 对 v1662 第二轮优雅治理的 stop condition。v1662 的结论是前十个长名里只有
一个安全文件路径，达不到五个候选的批量改名阈值，所以不得为消费版本而单独改那个文件。
v1667 尊重这个结论：源路径一字未动，变化仅发生在没有公开语义的内部构建 target 上。

## 二、为什么一条测试会同时有三个名字

理解本版前，先把 CMake 注册中的三个参数分开。典型注册是：

```cmake
minikv_add_linked_test(
    minikv_tp_closeout_validation_tests
    text_package_review_closeout_audit_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation_tests.cpp
)
```

第一个参数是内部构建 target。CMake 用它创建 `CMakeFiles/<target>.dir`，编译器日志和对象
目录都会带上它。它只需要在当前构建图中唯一、可辨识，并不承担完整业务叙事。第二个参数
是公开 CTest 名。开发者运行 `ctest -R text_package_review_closeout_audit_validation_tests`
时依靠的是它，所以它必须保持稳定和清楚。第三个参数是源文件路径；它进入源码引用、
include 关系、v1662 命名 baseline 和历史审计证据，迁移成本最高。

旧注册把三个层次都写成近似完整的名词串。结果是内部 target 重复携带了公开测试名已经
表达的全部语义，而对象目录又额外复制这串文字。v1667 把第一参数压到 35 字符：`tp`
表示 text package，`closeout_validation` 保留足够定位信息，`tests` 说明目标类型。公开
CTest 名和源路径仍保留完整语义。这不是用短名牺牲可读性，而是让不同身份各自承担正确的
信息量：构建 target 负责唯一和定位，CTest 名负责用户检索，源路径负责代码所有权。

## 三、警告为什么由“组合长度”触发

只检查 target 是否超过 40 字符还不够。对象路径由构建根目录、`CMakeFiles`、target 目录、
源文件相对路径和对象后缀共同组成。一个 80 字符 target 配短源文件可能仍能生成；一个
58 字符 target 叠加 142 字符源路径就可能越过生成器的安全边界。为了让合同不依赖某台
机器的绝对工作目录，本版定义了平台无关的风险分数：

```text
object_path_score = internal_target.length + source_path.length
```

它不是声称 197 等于 Windows 的绝对路径上限，而是一个对当前仓库可重复计算的相对
ratchet。v1666 的最高分是 200，且正好对应真实警告。缩短内部 target 后，该注册降到
177；全仓库新的最高分是 197，于是 `object_path_score_limit` 收紧为 197。任何新注册如果
超过 197，`test_architecture_contract` 会直接失败。

这个上限还有“必须同步收紧”的反向检查。假如未来维护者又还掉一个高分 target，使真实
最高分从 197 下降到 190，却忘了修改常量，合同会报“budget must shrink in the same
commit”。也就是说，197 不是永久豁免，而是当前树的精确屋顶；代码变好时，屋顶必须跟着
下降，不能留下可供后来回涨的空位。

## 四、`CMakeTestRegistration` 如何读取构建图

入口仍是已有的 `tests/test_architecture_tests.cpp`，没有另造第 354 个测试。新增结构
`CMakeTestRegistration` 只保存三项数据：`target`、`test_name` 和 `source`。解析器读取
受版本控制的 `CMakeLists.txt`，识别三种统一 helper：`minikv_add_linked_test`、
`minikv_add_source_dir_test`、`minikv_add_standalone_source_dir_test`。每个注册必须恰好包含
三个独占一行的参数并以右括号结束；如果有人把多个参数挤到同一行、漏掉参数或改变注册
结构，解析器会显式失败，而不是悄悄少统计一批测试。

为什么不解析生成目录？因为 `cmake-build-*` 是临时产物，不应成为仓库合同的数据源；CI
和本机生成器也可能不同。为什么不把所有注册抄进一份新 manifest？那会产生第二事实源，
CMake 改了而 manifest 忘记改，反而引入漂移。读取 `CMakeLists.txt` 的受控 helper 语法，
让编译注册和治理检查共享同一份事实。这个解析器不是通用 CMake 语言解释器，也没有假装
支持任意宏；它只处理本仓库已经统一的三参数注册形式，边界窄而可测试。

解析完成后，`verify_test_targets` 逐项检查内部 target 唯一、公开 CTest 名唯一、源文件
真实存在。当前得到 344 个 helper 注册；直接使用 `add_test` 的少量文档或 benchmark
合同不在对象目录风险模型中，因为它们不通过这些 helper 创建独立测试可执行文件。这个
口径写进输出 `cmake_tests=344`，评审者可以用 PowerShell 或 C++ 合同独立复算。

## 五、277 项 baseline 如何做到只减不增

静态盘点发现 344 个 helper 注册中，有 278 个内部 target 超过 40 字符。一次性把它们都
改成编号或 hash 会制造巨大、难审查且不易回滚的 CMake diff，也会让构建日志失去定位
价值。因此 v1667 不假装一版消灭所有历史债务，而是新增
`config/test-target-name-baseline.txt`，逐行列出本版完成后仍存在的 277 个超长 target。

机械规则分为三层。第一，长度不超过 40 的 target 不得进入 baseline，防止把正常名字也
变成豁免。第二，运行时发现的每个超长 target 都必须已经列在 baseline；新增长名、把旧
长名换成另一个长名，都会因“不在名单”失败。第三，baseline 的每一项必须在当前注册中
仍然存在；某个长 target 被缩短后，如果维护者没有在同一提交删掉旧账，会因 stale entry
失败。本版恰好移除触发警告的旧 target，所以基线从原始 278 收紧为 277。

baseline 是存量债务清单，不是优雅认证。277 仍然是很大的数字，但它比一个只写
`long_target_count=277` 的常量更可审计：评审者能看到具体名字，新增名称也不能用“数量
没变”掩盖。它与已有 735 个长文件名、883 个公共标识符 baseline 的治理思想一致，却保持
口径独立：本文件只管 CMake 内部 target，不改变 v1659-v1662 已评审的文件/API census。

## 六、三条故障注入分别保护什么

一个合同只在绿色树上运行还不够，因为可能检查条件永远进不去。`verify_red_paths` 因此在
原有三条测试源码体积故障注入之外，新增三条 target 场景。

第一条构造 41 字符的新内部 target，给它一个真实存在的 `tests/store_tests.cpp`。源文件
有效、路径分数也很低，唯一错误就是长 target 不在 baseline；预期必须命中 `new long
internal`。这证明“新增长名”门确实会红。

第二条不给任何注册，却向 baseline 放入那个 41 字符名称。预期命中 `stale internal`。
这证明还债后不能把名单原样留下，也防止 baseline 越积越多失去意义。

第三条构造恰好 40 字符的合规 target，再配出总分 198 的源路径。它不会触发长名规则，
而会独立命中 `object path score`。这样能证明路径预算不是长名检查的同义重复：即便 target
本身合规，和深源路径组合后仍必须受限。

三条注入都在内存数据上执行，不创建临时源码，不污染测试目录，也不依赖 Windows 才能
触发。真实 configure 则负责补上环境层证据：日志中
`CMAKE_OBJECT_PATH_MAX`、`cannot be safely placed` 和 `object file directory` 三类片段
出现次数必须为零。内存红灯证明规则会失败，干净配置证明真实症状已消失，两者缺一不可。

## 七、为什么保留完整公开 CTest 名

内部 target 缩短后，最需要防的回归是维护者找不到原来的测试。CMake helper 最终仍执行：

```cmake
add_test(NAME ${test_name} COMMAND ${target_name})
```

第二参数没有改，所以 `ctest -N` 仍报告 353 条测试，并且
`text_package_review_closeout_audit_validation_tests` 恰好出现一次。聚焦 CTest 同时运行
`test_architecture_contract` 和这个完整名字，两条均通过。前者证明治理规则，后者证明实际
业务测试仍由新可执行文件执行。构建日志会显示较短的
`minikv_tp_closeout_validation_tests.exe`，CTest 报告仍显示完整语义名；这正是两个身份分离
后的预期结果。

本版也没有创建 ALIAS 或额外 custom target 来假装兼容旧内部名。仓库全文盘点显示旧内部
target 只在注册处出现一次，没有脚本、文档或其他 target 依赖它。保留一个没有消费者的
别名只会扩大构建表面，并让路径治理结果变得含糊。真正需要兼容的是公开 CTest 名，它已
原样保留。

## 八、测试、构建与边界证据怎样分层

本版最终验证分四层。第一层是静态和聚焦层：clang-format 检查改动 C++，架构可执行文件
输出 `sources=361 limit=1000 oversized=0 cmake_tests=344 long_targets=277
max_path_score=197`，并运行六条红路径。第二层是干净 CMake 层：从空的
`cmake-build-v1667` 配置，确认对象路径警告为零，再完整构建所有目标。第三层是完整 CTest
层：353 条用例全部运行，而不是只运行改名目标。第四层是真实 CLI 层：新编译的
`minikv_cli` 依次执行 `SMOKEJSON`、`CHECKJSON GET demo`、`HEALTH`、`QUIT`，结构化解析
关键字段。

真实 CLI 仍应给出 `read_only=true`、`execution_allowed=false`、
`order_authoritative=false`。`CHECKJSON GET demo` 只解释命令，不执行 GET，更不执行写入；
因此 `write_command=false`、`wal.touches_wal=false`。本版没有 router，未新增 network
连接，没有读取 credential，没有触发 LOAD、COMPACT、RESTORE、SETNXEX，也没有改变
WAL、snapshot 或 OSFS 磁盘状态。这里运行 CLI 的意义不是因为 CMake target 改名会自然
改变业务逻辑，而是用真实新二进制证明构建图重接后仍指向同一产品边界。

对每类测试要区分它保护的对象。`test_architecture_contract` 保护 CMake 注册形状、命名
baseline、路径分数、测试源体积和 command/shard suite census；目标业务测试保护 text
package closeout validation 的字段与格式；全量 CTest 保护仓库其他行为没有被构建身份
变化牵连；CLI smoke 保护最终用户入口的只读和无执行边界。截图只是这些命令的选择性
摘要，不是通过结论本身。

## 九、维护者以后怎样新增或缩短 target

新增测试时，优先让内部 target 不超过 40 字符，公开 CTest 名可以保留清楚完整的领域
表达，源文件按已有所有权边界命名。若 target 和源路径总分超过 197，应先提取较短且仍可
识别的内部构建概念，不能把新名字写进 baseline。baseline 只接纳 v1667 已存在的历史项，
不是申请豁免的入口。

偿还旧 target 时，应在同一提交完成三件事：把第一参数改到 40 字符以内，从 baseline
删除旧名字，并观察真实最高路径分数。如果被偿还项正好是最高分，还要同步下调
`object_path_score_limit`。架构合同会分别对遗漏的 baseline 删除和未收紧的上限报错，
因此维护者不需要靠记忆执行这套纪律。

如果某个长名字属于公开 CTest 名或源文件/API 路径，不能机械套用本版做法。公开名要先
审计消费者，源路径要遵守 elegance pin 与历史证据稳定性。v1667 证明的是“内部构建身份
可以比公开叙事短”，不是“所有长名字都应该随便缩写”。这条边界能避免优雅治理反过来
破坏可读性和兼容性。

## 十、最强质疑与回答

最强质疑一：为什么不直接把 CMake 警告当作偶发提示？因为它来自确定的字符串组合，且
构建目录再深几级就可能成为平台失败。现在已有真实触发证据，继续忽略等于把可预见问题
留给另一台机器。缩短内部 target 不触及运行合同，成本低，修复合理。

最强质疑二：为什么不把 278 个长 target 一次清空？因为自动 hash 或编号虽然短，却会让
编译日志难以追踪；人工批量缩写又会形成数百个低信息 diff，审查成本高于当前收益。先用
baseline 阻止继续增长，再按真实路径风险逐项还债，是更可逆的线路。

最强质疑三：197 是否是假精确？它不是操作系统的绝对上限，而是当前仓库的可重复相对
指标。真实 CMake 零警告验证负责环境结论，197 ratchet 负责未来趋势，两者组合才完整。
文档没有声称“任何绝对路径下永不超限”，只声称当前工作目录和 CI 构建面已消除已知警告，
以后不会在这个组合指标上回涨。

最强质疑四：改的是 CMake 一行，为什么需要完整测试？因为 target 是业务测试可执行文件
的构建身份，拼错会导致目标缺失、CTest 指向错误程序或 CI 少跑测试。聚焦验证能快速发现
直接问题，全量验证确认 353 条注册和跨平台构建图没有旁路。完整验证不是按改动行数计费，
而是按错误可能影响的契约面计费。

## 十一、一句话价值

v1667 在不改变任何产品、fixture、公开测试名或执行权限的前提下，消除了真实 Windows
对象路径风险，并把 278→277 与 200→197 变成可持续收紧的机械合同，为 Node、Java 与
mini-kv 的松耦合只读验证保留了更稳定、更可维护的 C++ 构建底座。
