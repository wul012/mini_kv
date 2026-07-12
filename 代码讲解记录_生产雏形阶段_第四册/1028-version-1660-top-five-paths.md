# mini-kv v1660 代码讲解：偿还第一批五个内部路径命名债务

## 一、本版目标、角色定位与明确不做

v1660 是有界优雅计划的 E-M2。v1659 已经把 40 字符预算、740 个长文件名和 883 个公共头文件可见长标识符变成会失败的 CTest 合同，本版第一次使用这套仪器偿还真实债务。目标不是“随便挑五个看起来长的名字”，而是按 census 的稳定排序选择五个最差、又没有被外部合同钉住的内部实现路径，将它们统一收束到一个能被维护者理解的概念：`compared package intake audit`。

这仍然不是功能版本。没有新增命令、字段、路由或运行开关，没有调整 receipt 语义，没有改 Store、WAL、Snapshot、RESP、TCP 或 OSFS。它不读取 credential value，不安装 active shard router，不允许 write routing，不执行 LOAD、COMPACT、RESTORE、部署或回滚，也不让 mini-kv 获得 Java order authority。五个 translation unit 的文件内容保持逐字节不变，公共头文件路径、namespace、struct 和函数签名全部保留，所以外部 C++ 使用者看到的源码接口与 v1659 完全一致。

本版也不借“优雅”之名进行大范围重构。五个文件本来就已经按 main、stages、slots、guards、validation 拆开，职责边界是合理的；问题仅在于内部 `.cpp` 路径把整个业务历史串进了文件名。若强行合并文件，会破坏现有模块所有权；若顺手重命名公共 namespace，又会扩大兼容性风险。因此这里最优的工程动作就是窄而完整的路径整理：只改内部身份，不改行为和外部身份。

## 二、五个目标如何由机械 census 选出

v1659 baseline 中所有文件条目都采用仓库相对路径，排序时先按 stem 长度从大到小，再以路径作为稳定次序。最前面同时出现公共头文件和私有实现文件。计划明确禁止触碰公共 header API，所以筛选时跳过 `include/minikv/` 的条目，只看没有 fixture、archive、waiver 或外部脚本引用的 `src/*.cpp`。得到的前五名恰好属于同一个 compared-package evidence intake audit 家族：validation 为 143 字符，guards 与 stages 各 139 字符，slots 为 138 字符，主 audit 为 132 字符。

选择同一 family 不是人为凑批次。全仓库精确路径搜索证明，每个旧 `.cpp` 文件名只有两个消费者：`CMakeLists.txt` 的源列表，以及 `config/elegance-name-baseline.txt` 的债务条目。代码中的 `#include` 指向对应公共 `.hpp`，不是 `.cpp` 路径；测试也通过公共头文件和 namespace 使用实现。换言之，这五个源文件名是构建系统的私有装配细节，不是 CLI、JSON、fixture 或 C++ include contract。

CodeGraph 的预改动上下文进一步确认，该区域真正向外暴露的是 `ComparedPackageEvidenceIntakeSlot`、`ComparedPackageEvidenceIntakeGuard` 等公共结构，而这些结构位于长路径公共头文件中。本版没有改它们。CodeGraph 不能替代编译和测试，但它帮助区分“文件名很长”与“公共符号可改”这两个完全不同的问题；随后 `rg` 精确路径搜索负责验证实际文件引用，CMake 和 CTest 再负责验证构建与行为。

## 三、family 设计说明与新路径

按照优雅门的要求，改名之前先在执行计划写了不超过十行的 family 设计说明。抽象名是 `compared package intake audit`；主文件聚合现有审计流程，stages 提供阶段表，slots 提供证据槽位，guards 提供阻塞条件，validation 提供输入与一致性校验。数据边界是公共 header、namespace、struct、函数签名与输出字段原样保留，行为边界是五个 translation unit 的内容不变。

五个路径对应关系如下：

| 角色 | 旧 stem 长度 | 新文件 | 新 stem 长度 |
|---|---:|---|---:|
| 主审计聚合 | 132 | `src/compared_package_intake_audit.cpp` | 29 |
| 阶段定义 | 139 | `src/compared_package_audit_stages.cpp` | 29 |
| 槽位定义 | 138 | `src/compared_package_audit_slots.cpp` | 28 |
| 守卫定义 | 139 | `src/compared_package_audit_guards.cpp` | 29 |
| 校验逻辑 | 143 | `src/compared_package_audit_validation.cpp` | 33 |

新名字没有把信息压缩成模糊缩写。`compared_package` 保留最重要的领域限定，`audit` 表示五个文件属于同一审计机制，末尾角色词继续表达职责。主文件额外保留 `intake`，说明它是证据接收流程入口。所有 stem 都明显低于 40 字符预算，也没有为了过门使用 `cpia_*` 这类只有作者知道含义的缩写。

这里没有新建第六个 wrapper 或“兼容转发 `.cpp`”。CMake 直接从旧路径切到新路径，旧文件消失。保留旧文件再 include 新文件虽然能减少某些构建脚本改动，却会留下重复 translation unit、潜在重复定义和一份新的 waiver，等于用脚手架掩盖债务。路径不是运行时动态查找合同，直接迁移才是干净做法。

## 四、为什么公共长 namespace 与 header 本版必须保留

五个新 `.cpp` 内仍会出现很长的 `minikv::shard_route_preview_...` namespace，也会 include 对应长 `.hpp`。表面上看，这似乎没有贯彻 40 字符预算；实际上这是计划边界和兼容性判断的结果。namespace、类型名和 include 路径位于公共头文件表面，已有测试、其他 translation unit 乃至潜在外部消费者都可能依赖。直接改名会改变源码兼容合同，需要 alias 生命周期、弃用策略和独立迁移计划，不能与内部路径清理混在一起。

优雅门对存量采用 baseline 只减不增，而不是要求任何被看见的旧名字立即消失。v1660 缩短的是确认可安全修改的私有文件 stem；883 个公共头文件可见长标识符保持不动，正是机械结果应该呈现的事实。若为了让两个数字都下降而偷偷排除公共头文件，或者批量改公共符号再修改测试迎合，就违反了计划的失败条件。保留风险较高的债务并写明原因，比追求漂亮数字更专业。

童子军规则也应按实际所有权理解。五个 translation unit 的源码字节没有编辑，只有文件系统身份被治理；其中继承的长 namespace 是公共合同，不是本版新增或修改的标识符。新路径本身完全达标，CMake 新引用也达标。未来若专门授权公共 API 迁移，才应在兼容层、调用方和测试齐备后收紧 identifier baseline，而不是在本版本越权。

## 五、baseline 如何证明是“还债”而不是“换债”

执行顺序刻意保留一次预期失败。完成 `git mv` 和 CMake 路径更新后，先使用 v1659 scanner 对当前树运行正常模式，此时 current census 已经没有五个旧路径，而 baseline 仍保留它们。合同返回失败，标题是 `file names baseline is stale; shrink it in this commit`，随后逐条列出五个旧 `src/...cpp` 路径。输出中没有 `new violations`，这证明新路径都在预算内，也没有用五个新超长名字替换旧名字。

确认失败集合精确等于五条后，才显式运行 `--write-baseline`。机械结果是 `files=735 public_ids=883`：文件债务从 740 减少五个，公共标识符债务不变。接着 CMake 的 `MINIKV_NAME_FILE_MAX` 从 740 收紧到 735，`MINIKV_NAME_ID_MAX` 保持 883。baseline 精确集合和编译期 maximum 同一提交变化，因此以后手工加回旧路径、引入新长路径或把上限放松都会产生可审查 diff，并由 CTest 阻止无意回归。

这套证据比 `git diff --stat` 更强。Git 的 rename detection 能证明文件内容相似甚至 100% 相同，但它不理解命名预算，也不能阻止未来新增第 736 个长名。census 负责长期政策，Git object identity 负责本次内容未变，完整测试负责行为未变，三者各自回答不同问题，不能互相替代。

## 六、字节稳定、fixture 稳定与运行边界

路径改名后，对五个旧版本 blob 与五个当前文件执行 Git object hash 对照，可以直接证明每对内容对象一致。因为没有修改 include、namespace 或函数体，编译后的链接符号和调用图应保持不变，但“应该”仍不足以交付；全目标构建必须重新编译新路径，确保 CMake 没有漏项、重复项或平台路径错误。

release fixture 采用与 v1659 相同的确定性 manifest 算法：对 1514 个 tracked `fixtures/release/*.json` 逐文件计算 SHA-256，再对排序后的 path/hash 清单计算总摘要。v1659 起点是 `8eb430889b84bec547abcd4d963636666edd897379769969cf4c715af7f8e00f`，v1660 必须完全一致。只比较 `git status` 不够，因为生成脚本或换行工具可能重写大量内容；聚合摘要能把任何单字节变化变成总摘要变化。

真实 CLI smoke 继续执行 SMOKEJSON、`CHECKJSON LOAD data/prod.snap` 后立即 SIZE，以及 SHARDJSON。期望不是“命令能启动”这么浅，而是具体边界保持：SMOKEJSON 的 `read_only=true`、`execution_allowed=false`、write/admin false；CHECKJSON 只预测 LOAD 会替换 store，自己不执行，SIZE 仍为 0；SHARDJSON 的 `activeRouterInstalled=false`、`writeRoutingAllowed=false` 和 `executionAllowed=false`。路径重命名若意外漏编某个审计模块，相关 shard/readiness 测试和真实 JSON 证据会一起暴露问题。

## 七、测试层次各自保护什么

第一层是 `elegance_name_census_contract`，它保护 735/883 精确集合和只减不增 maximum。第二层是 compared-package intake audit 的现有 focused tests，包括主 audit、stages、slots、guards 与 validation 测试；这些测试通过公共头文件调用原符号，能证明路径变化没有改变各子模块数据和校验行为。第三层是 shard readiness、route preview 和 runtime evidence 链路，它们保护聚合输出和下游消费面。

第四层是 351 条完整 CTest，覆盖 KV、WAL、Snapshot、RESP、TCP、OSFS、receipt parity、release fixture、文档 honesty、E1-E10 与 archive contract。第五层是三系统和专用 lane 的 GitHub Actions：ubuntu、macOS、Windows、sanitizer、coverage、clang-format 和 archive inventory。内部路径在 Windows MinGW 上尤其值得重新构建，因为原仓库已有超长 object path 警告；虽然本版五个源文件不是警告中那条测试路径，但缩短路径不会增加此类风险，CI 可以确认三平台构建图仍完整。

v1659 的远端结果给了这套门第一次真正的反证。ubuntu、macOS、coverage、sanitizer、format 和 archive 六个 job 都通过，Windows 却把 baseline 的全部长名当成新增项。日志显示文件路径本身一致，差异来自 GitHub Windows checkout 的 CRLF：`std::getline` 删除 `\n`，却把 `\r` 留在 baseline 集合末尾，导致 `current` 与 `baseline` 的字符串永不相等。本地工作树保持 LF，所以 351/351 没有暴露它。

v1660 立即把这个红灯纳入同一命名门维护范围。`parse_baseline` 在每行读取后只删除一个末尾 `\r`，不会 trim 路径内部空格、改变大小写或放宽其他字节；`check_crlf_baseline` 每次运行都解析一份内存 CRLF 样本，精确要求 `F|src/example.cpp` 与 `I|ExampleName` 两个键没有残留行尾。这样即使开发机仍使用 LF，Windows 分支也已经成为本地 CTest 可执行的固定样本，而不是再次等远端偶然发现。

本版没有通过修改测试期望或 fixture 来“适配”改名。测试源码无需知道 `.cpp` 文件名，只有 CMake 改变；若某个测试失败，正确动作是找出源列表或链接问题，而不是更新行为断言。这与 receipt migration 先冻结 oracle 的纪律一致：验证面先于改动存在，改动服从验证面。

## 八、维护价值与硬停止意识

从 132 至 143 字符降到 28 至 33 字符，最直接的收益是构建日志、IDE 标签、错误信息和文件导航重新可读。更重要的是，新命名把真正的抽象提到前面：维护者先看到 `compared_package_audit`，再看到 stages/slots/guards/validation 角色，而不是穿过十几个历史名词才能找到末尾差异。这减少认知负担，也降低 Windows object path 和工具截断对诊断信息的干扰。

但五个文件只占 740 个起始债务的一小部分，不能被描述成全仓库已经优雅。计划最多四版，本批次选择三版闭环：v1659 建门，v1660 证明 top-5 还债流程，v1661 冻结终态并请求外部评审。终态仍有 735 个长文件和 883 个长公共标识符，其中很多受公共 API、fixture、历史 archive 或独立迁移成本约束。收口文档会按类别说明刻意保留原因，而不是新增数百个 waiver。

## 九、把一次路径迁移的输入与输出讲透

以 validation 文件为例，迁移输入有三部分。第一部分是 Git 中旧路径对应的 blob `8aecb0a1f35603515e6f9dde1e18f56a3bd127f8`；第二部分是 CMake 源列表里的旧路径；第三部分是 baseline 中以 `F|src/...validation.cpp` 保存的债务键。操作过程先由 `git mv` 把同一 blob 放到 `src/compared_package_audit_validation.cpp`，然后只把 CMake 的路径字符串指向新位置。此时生产输入内容没有变化，但 census 输入集合已经变化，所以第一次运行会报告旧 baseline 键陈旧。

迁移输出同样分成三部分。Git 输出是旧、新文件 blob 完全相等，表示函数体、include、namespace、注释和换行都没变；CMake 输出是新路径被编译成原 target 的一个 object，链接到同一个 `minikv` library；census 输出是少一条 `F|` 债务且没有多出新条目。只有这三部分同时成立，才能说“路径已经缩短且实现未改”。如果只看到 Git rename，却忘了 CMake，构建会缺实现；如果只改 CMake 又保留旧文件，债务没有偿还；如果手工删 baseline 却没有移动文件，census 会把旧路径作为新增违规重新报出。

其余四个文件走完全相同的数据流，但没有复制五套迁移脚本或兼容包装。它们以一个显式 move 列表完成，列表在执行前检查源存在、目标不存在，任何一步失败立即停止。输出后再逐对比较 HEAD 旧 blob 和当前新 blob，五对结果全部为 true。这个过程展示了为什么“看似只是改名”仍需要输入、变换、输出和失败路径：工程质量不取决于改动行数，而取决于风险是否被机械证据封闭。

从运行时视角看，编译器读取新文件路径只是定位源码文本，C++ namespace 和符号修饰名仍由未改变的文件内容决定。链接器不会把源文件 basename 写成公共函数 API，CommandProcessor 也不会在运行时按 `.cpp` 名查找审计模块。因此用户输入的 SHARDJSON 命令、内部调用的审计函数、最终 JSON 输出之间没有新增转换层。这里的“输入输出不变”不是抽象口号，而是从源码定位、目标构建、符号链接到命令响应的每一层都能解释。

## 十、一句话价值总结

v1660 为 Node、Java 与 mini-kv 的松耦合协作证明了一条可复制的维护路径：只缩短 C++ 私有装配身份，机械收紧债务基线，同时让公共 API、fixture、no router、no write 与 no execution 合同保持逐字节稳定。
