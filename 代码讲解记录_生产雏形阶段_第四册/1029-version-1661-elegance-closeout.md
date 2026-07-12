# mini-kv v1661 代码讲解：有界优雅计划的终态、证据与硬停止

## 一、本版目标、角色定位与明确不是什么

v1661 是 `elegance-hotspot-program-minikv.md` 的 E-M4，也是本计划的强制停止点。它不再重命名第六个文件，不继续追逐更低数字，而是把 v1659 至 v1660 已形成的机械门、真实 CI 失败与修复、top-five 偿债结果、终态存量和刻意保留原因组织成评审可以独立复现的闭环。执行者的职责是把事实准备充分，不是替 Claude 宣布最终 PASS。

这不是功能版本，也不是“mini-kv 已经完美”的包装。KV、WAL、Snapshot、RESP、TCP、OSFS、receipt 和 shard evidence 的运行路径没有新增逻辑；CLI、RESP/JSON 键、fixture、archive 和公共 C++ API 不改。`read_only`、`execution_allowed`、`activeRouterInstalled`、`writeRoutingAllowed` 等边界继续表达原有控制面含义。本版也不借 closeout 文档新增 waiver，不把扫描范围缩小，不通过删除 baseline 行制造下降。

最重要的诚实边界是：终态仍有 735 个超过 40 字符的文件 stem 和 883 个公共头文件可见长标识符。五个文件名的下降是真实、可证明但有限的结果。计划之所以现在停止，是因为授权从一开始就是 2 至 4 版的有界维护，而不是承诺在一个批次里重写整个历史 API。继续推进需要新的优先级判断、兼容策略和外部授权，不能由执行者把“持续优化”当成无限许可。

## 二、三版工作的输入、变换与输出

v1659 的输入是未被机械约束的历史命名债务。一次性扫描得到 740 个长文件名与 883 个头文件可见长标识符。变换是新增独立 C++ scanner、精确 baseline 和编译期 maximum；输出是第 351 条 CTest。正常树在 740/883 通过，临时注入第 741 个长文件立即失败并打印路径，删除探针后恢复绿色。这个版本购买的是“以后不能无声变差”的能力。

v1660 的输入是 census 稳定排序后的最差条目以及“不碰公共合同”的计划边界。过滤公共 headers 后，前五个可安全对象全部属于 compared-package evidence intake audit 的内部 `.cpp` 家族。变换是把 132 至 143 字符的私有路径改为 28 至 33 字符的 `compared_package_*` 路径，CMake 同步引用；五个文件内容 blob 不变。输出是 baseline 740→735、identifier 883→883，且刷新前合同精确报告五条 stale old path、没有任何 added violation。

v1661 的输入是已经通过本地完整验证、但 v1660 远端 changed-files format job 仍指出存量格式债务的 735/883 树。变换先对五个 renamed translation unit 执行 clang-format，再做终态计量、分类、需求-证据对账和 review pointer；输出包括格式达标源码、`docs/elegance-hotspot-closeout.md`、最终 archive 与外部 verdict 槽位。baseline 数字不变，运行行为必须重新验证。这样评审能区分“建门”“用门还债”“童子军修复并冻结结果”三个阶段。

## 三、终态 census 的机理与精确分类

文件 census 仍只统计 `src/`、`include/`、`tests/` 下 regular file 的 stem，扩展名与目录长度不计。终态 735 条按第一层目录分为：`include=218`、`src=241`、`tests=276`，三项之和精确等于 735。这个分类不是估算，也不是从 README 抄来的数字，而是从 baseline 的每一条 `F|path` 机械分组得到。

公共标识符 census 仍从 `include/` 头文件读取代码区域，剔除注释、普通/raw 字符串、字符字面量和 include 路径，再用 C++ identifier 词法模式去重。终态 883 没有下降，因为 v1660 明确不改公共 header。它们中 214 个以 `shard_` 开头，27 个是大写开头的类型式名称，其余 642 个是其他头文件可见标识符。分类只帮助评审理解形态，不改变 exact key set，也不构成三类 waiver。

baseline 的关键不是数字，而是键集合。current 新增一条长名会出现在 `current - baseline`；删除旧债却不更新 baseline 会出现在 `baseline - current`；用另一个长名替换旧名会同时出现 added 和 stale。CMake 的 735/883 maximum 再防止文本 baseline 被放大。v1661 没有运行 `--write-baseline`，因此终态文件与 v1660 相同；closeout 只是引用合同，不成为第二份可漂移的基线。

## 四、v1659 红灯为何是质量成果的一部分

本计划没有隐藏第一版远端红灯。v1659 GitHub Actions run `29176190797` 中，ubuntu、macOS、coverage、sanitizer、clang-format 和 archive inventory 六个 job 通过，Windows 失败。失败不是仓库突然多出数百个文件，而是 Git Windows checkout 把 baseline 变成 CRLF；`std::getline` 删除换行 `\n` 后保留 `\r`，导致 baseline 中每个键都比 live path 多一个不可见字符。

这个事故说明“本地 351/351”不能替代三平台 CI，也说明新 gate 自己同样需要跨平台测试。v1660 没有把 Windows job设成 optional，没有在 baseline 中加入带 `\r` 的第二套键，更没有对路径执行宽泛 trim。修复只删除行尾单个 carriage return，并通过 `check_crlf_baseline` 每次解析内存 CRLF 样本，精确要求两个示例键无残留。路径内部空格、大小写、斜杠和其他字符仍原样比较。

因此 v1659 红灯不是要从历史中抹掉的瑕疵，而是机械门发挥作用后的反馈。v1660 在 Windows 已越过 CRLF 问题，却又由 `clang-format changed files` 抓到五个 renamed legacy units 的存量格式债务。这同样不是关闭 gate 的理由：纯 rename 使它们成为 touched files，童子军规则要求离开时达标。v1661 直接格式化五个文件，再用 focused/full tests 与 fixture/CLI 证明格式变化不改合同。进度账本保留两次 failure chain，也不移动历史 tag 伪造全绿。

## 五、为什么保留 218 个 include 路径与 883 个标识符

公共 header 路径、namespace、类型和函数可能被仓库内外的源码消费者 include 或引用。重命名它们不是简单 `git mv`，而是 API 迁移：需要确定 canonical 新名、兼容 alias、弃用期限、调用方升级顺序、可能的 Node/Java 文档引用和删除旧名的条件。本计划没有这些授权和证据，所以保留是兼容性决策，不是声称长名合理。

closeout 特意使用“header-visible”而不是把全部 883 个称为公开函数。词法 scanner 会看到头文件中的 private member、模板参数、宏和 namespace，它采用保守可复现口径，不做不完整的 C++ 访问控制解析。外部评审若要开启下一轮，应先把 883 按真正 public API、实现细节和可兼容 alias 分类，再决定哪些可以安全减少。当前计划到点停止，不能为了数字下降偷换 scanner 语义。

最靠前的五个 retained 文件恰好是 v1660 已改 `.cpp` 所对应的公共 `.hpp`。这不是漏改，而是边界是否生效的直观证据：内部装配身份已经简化，外部源兼容身份仍然固定。若两侧一起改，五对 blob 相等与 fixture 不变仍不足以证明外部消费者可以编译，所以本版不做。

## 六、为什么保留 276 个测试路径与 241 个源码路径

测试文件是内部路径，理论上许多可以继续缩短，但每批都要更新 CMake、验证 test discovery、重新跑完整矩阵并维护 baseline。只改文件名不会自动改善生产架构，收益需要和 CI 时间、审查成本比较。本计划已经用一个 family 证明了流程，硬停止优先于为了扩大 diff 连续处理几十个测试文件。276 不是豁免清单，而是下一次规划可选择的债务池。

剩余 241 个 `src` 长路径也不能统称“被合同钉住”。其中有些与公共 header family 配对，有些是纯私有实现，有些承载历史 evidence 概念。v1660 只处理排序最前且引用面最简单的五个，未对其余项目逐个判定。closeout 明确说它们是 separately reviewable future debt，避免两种相反夸大：既不说全部不能改，也不说全部可以无风险批量改。

三次规则在这里同样重要。大量 stages/slots/guards/validation family 可能反映数据表可以共享，也可能只是不同领域恰好使用相同角色词。仅凭文件后缀计数不能证明实现重复。后续若做结构优化，必须先比较数据结构、控制流和输出合同，再决定 shared engine；不能把 E-M4 变成未经 oracle 的大合并。

## 七、需求-证据矩阵如何防止自我授奖

`docs/elegance-hotspot-closeout.md` 把七项要求映射到机械证据。40 字符 census 由故意注入失败证明；只减 baseline 由 exact set 与 maximum 证明；top-five 由 stale failure、v1660 的 100% Git rename 和 735 终态证明；跨平台由 v1659 Windows failure 与 CRLF self-check 证明；touched-file format 由 v1660 format failure 与 v1661 dry-run 证明；字节/运行稳定由 1514-file manifest、351 CTest 和真实 CLI 证明；硬停止由最多四版规则与 pending verdict 证明。

这份矩阵不会让 Markdown 自己成为事实源。`project_docs_honesty_contract` 只检查当前指针、终态数字、PENDING verdict 等关键片段仍存在；真正的命名集合由 C++ census 决定，fixture 字节由摘要和 parity tests 决定，行为由 CTest/CLI 决定，远端平台由 GitHub Actions 决定。任何一层失败都不能用“文档写了完成”覆盖。

最强评审质疑已经写入：五个相对 740 很少。回答不是回避，而是区分产出。直接债务减少只有五个；更大的价值是新增跨平台、可失败、精确集合的上游门，并证明一次安全还债流程。若把这描述成“命名治理完成”，就是过度主张；若说五个毫无价值，又忽略了 ratchet 让未来新增违规直接失败。closeout 采用中间的准确表述。

## 八、最终验证与评审者应该看什么

本地最终验证从干净 v1661 构建树配置并构建全部 targets，先跑 census、docs honesty、compared-package focused tests，再跑 351 条完整 CTest。fixture manifest 必须继续是 `8eb430889b84bec547abcd4d963636666edd897379769969cf4c715af7f8e00f`。真实 CLI 必须继续证明 SMOKEJSON read-only/no-write/no-admin，CHECKJSON LOAD 只预测不执行且 SIZE=0，SHARDJSON no active router/no write routing/no execution。

远端候选提交需要七个 job 全绿，尤其 Windows 必须证明 CRLF 修复生效，format job 必须证明五个 touched files 已达标。随后 final closeout 提交记录候选 run，tag `v1661` 指向最终文档，再跑 final commit 的 CI。执行者到这里停止，请 Claude 重跑 census、查看两条 failure/repair chain、检查 v1660 rename 记录与 v1661 fixture/runtime 稳定，再填写 PASS 或 findings。外部评审前状态只能是 READY FOR CLAUDE REVIEW / PENDING。

清理门同样属于交付。构建目录、Playwright snapshot 和临时日志删除；最终只保留三张选择性 PNG、解释、summary、walkthrough、closeout 文档、commit/tag 和远端 CI。没有服务器在后台运行，也不把二进制塞进 archive 冒充证据。

## 九、未来若重新授权，应该如何选择下一批

下一轮不应简单从 baseline 再取五个最长条目。v1660 能按长度选，是因为最前五个恰好属于同一内部 family，且精确搜索证明只有 CMake 与 baseline 引用。终态第一名已经变成公共 header，继续按长度会立刻碰到兼容表面。新的规划应先把候选分为三类：纯私有 `.cpp` 或测试路径、公共 header 文件路径、公共 header 内标识符；三类使用不同验收门，不能混成一个改名批次。

纯私有路径可以继续沿用 v1660 流程，但要优先选择能够改善构建诊断、IDE 导航或 family 一致性的对象，而不是只追求数量。每批先冻结 old blob、引用清单和受影响测试，再改路径、观察 stale-baseline 失败、收紧 maximum、跑完整矩阵。若发现第三个结构相似实现，先判断是否真的共享数据与控制流；只有可证明重复时才建 shared engine，不能因后缀相同就合并。

测试路径应单独成批。它们的外部兼容风险较低，却可能影响 CMake test target、正则选测命令、文档复现命令和 CI 日志。优先改导致 Windows object path 警告、日志截断或维护者难以区分的测试；仅为减少 baseline 数量而改几十个短生命周期测试，价值可能低于评审成本。每批仍要保持 test name 不变，除非另有消费者清单和迁移理由。

公共 header 与 883 个标识符需要完整 API 迁移简报。简报至少回答 canonical 新名、旧名 alias 是否可行、deprecated 周期、仓库内调用方数量、外部 include 风险、何时删除 alias、fixture/JSON 是否完全无关，以及 source compatibility 如何在三平台证明。若无法回答，就保留 baseline；baseline 不是催促执行者越权的排行榜，而是帮助所有者看见成本的账本。

还应先提升 scanner 的语义，再决定是否处理标识符。例如可用 clang tooling 区分真正公开声明与 private member，但只有新工具在 Windows、macOS、Ubuntu 都可复现且不引入脆弱依赖时才值得采用。切换口径必须并列展示旧、新 census 和键映射，不能让 883 因工具换算法突然下降后宣称“偿还完成”。任何新计划仍应有最多版本数和外部评审点，防止维护工作重新无限延长。

## 十、一句话价值总结

v1661 为 Node、Java 与 mini-kv 的松耦合维护留下了一条透明边界：mini-kv 以 735/883 的真实终态停下，靠跨平台 ratchet 防止继续变差，靠外部评审决定下一步，而不是用更多版本号掩盖剩余债务。
