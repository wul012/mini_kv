# mini-kv v1662 代码讲解：把“还要不要继续改名”变成可执行的 Pin 审计

## 一、本版目标、角色定位与明确不是什么

v1662 不增加新的 KV 命令，也不扩展 WAL、Snapshot、RESP、TCP 或 OSFS 能力。它承担的是优雅第二轮的条件入口：第一轮已经把长文件名从 740 个降到 735 个，并把 883 个公开 header 可见长标识符冻结在只减不增的 baseline 中；第二轮不能因为仓库里仍有长名，就默认继续批量改名。本版要先回答更重要的问题：当前最长的十个名字里，究竟有多少是可以在不迁移公共 API、不改变 CLI/JSON/fixture 字节的前提下安全处理的？

本版的角色是“改名前的机械审计门”。它不是新的执行入口，不是新的 router，不读取 credential value，不连接网络，不触发 restore/load/compact，不写 Store/WAL，也不授予 Java 或 Node 订单权威。它同样不是为了让版本看起来有产出而挑一个文件改名。治理计划已经把条件写死：安全候选至少五个才允许进入 E2-M2；不足五个就应当在审计版关闭。换句话说，本版输出的是一个可复核决策，而不是一定要输出一批 rename。

这一定位很重要。存量债务整治最容易出现两种对称的误判：一种是看到 735 就说“还有很多，继续改”；另一种是看到公共 header 很长就说“全都不能动”。前者忽略合同成本，后者又会掩盖内部测试路径仍可能安全。v1662 既不夸大，也不低估，而是把选择集合、排序规则、pin 分类和分叉阈值一起写进测试。

## 二、入口在哪里：沿用现有 census，而不是复制第二套扫描器

本版唯一的执行入口仍是 `elegance_name_census_contract`，对应源码 `tests/elegance_name_census_tests.cpp`。v1659 已经在这里实现两类真实 census：

1. 遍历 `src/`、`include/`、`tests/`，统计 stem 超过 40 个字符的文件名；
2. 只扫描 `include/` 下的公开 header，在去掉注释、字符串、字符字面量和 include 路径后，统计超过 40 个字符的标识符 token。

v1662 没有再创建一个“看起来独立、实际上重复”的 scanner。它复用 `parse_baseline()` 得到的 `Census`，新增 `rank_baseline()` 将文件名集合和公开标识符集合转换为统一的 `RankedName`。这体现了数据和行为的清晰分界：旧 baseline 仍是全部存量的事实集合，新 TSV 只是前十名审计结论，排名行为只存在于一个 C++ 合同中。若以后 baseline 合法收紧，审计 TSV 没有同步更新，测试会因 ranking drift 失败，不会产生两份各说各话的统计口径。

入口的实际输出也被加厚为：

```text
elegance census: files=735 public_ids=883 budget=40 pin_audit=9/1 branch=close
```

前半段继续证明原有 ratchet；`pin_audit=9/1` 表示九个 pinned、一个 safe；`branch=close` 则是条件阈值的直接结果。这里的 close 只关闭 Round 2，不等于把所有存量债务宣称为“已经优雅”。

## 三、核心数据结构：RankedName、PinAuditRow 与 AuditSummary

`RankedName` 只表达排序所需的三个事实：类别、长度和名称。类别用 `F` 表示文件名，用 `I` 表示公开标识符。文件名长度沿用 census 的定义，取完整路径最后一段的 stem，不把 `include/minikv/` 目录前缀和 `.hpp/.cpp` 扩展名算进去；标识符长度则直接取 token 长度。这个区别不是细节，如果把完整路径长度混进来，深目录会被错误放大；如果把扩展名算进去，又会让同一概念因为 `.hpp` 与 `.cpp` 得到不同预算。

排序比较器有三层，而且顺序固定：先按长度降序，再按类别升序，最后按名称升序。为什么还要规定两个 tie-break？因为第 139 位附近同时存在 guards/stages 的文件名和 namespace，若只写“按长度排序”，不同标准库实现或不同输入容器顺序都可能让前十边界漂移。稳定 tie-break 让 Windows、Ubuntu、macOS 以及后续维护者得到同一组候选，也阻止执行者通过调整同长度条目的顺序来换取预期结论。

`PinAuditRow` 对应 `config/elegance-round2-pin-audit.tsv` 的一行，保存 rank、kind、length、name、decision、pin 和 mechanical_evidence。TSV 是数据，不执行任何判断；C++ 测试才是行为。选择 TSV 而不是在 Markdown 表里解析，原因是字段边界清晰、Tab 分隔可确定、审阅 diff 简单，并且不需要为了审计引入 JSON 库。文档可以使用省略号改善阅读，TSV 必须保存完整路径和完整标识符，这样机械验证不会依赖人类摘要。

`AuditSummary` 只聚合四个数量：文件名数、标识符数、pinned 数和 safe 数。测试要求它们精确等于 6、4、9、1。它没有“预计”“大约”或默认值；只要候选集合、pin 分类或阈值语义发生变化，就必须在同一提交中重新审计，而不是让旧结论继续显示绿色。

## 四、前十名如何产生：不是五加五的手工配额

治理简报要求前十名中同时包含文件名和公开标识符，但没有授权人为各取五个。v1662 采用更不容易被操纵的方式：把 735 个文件名和 883 个标识符合并后统一排序，直接截取前十。实际结果自然形成 6 个文件名和 4 个标识符。

前九项都属于 compared-package evidence intake audit 的公开 header 表面。其中有 validation、guards、stages、slots 等 `.hpp` 路径，也有直接声明在这些 header 中的 namespace token。文件路径的 pin 类型是 `public_header_path`：它位于 `include/minikv/`，外部消费者可能直接 include。标识符的 pin 类型是 `public_header_identifier`：测试不仅检查 evidence header 存在，还会读取文件并确认完整 token 确实出现。这样不能把“听起来像公共 API”当作证据，pin 必须落到真实文件和真实声明。

第十项是 `tests/...review_closeout_audit_validation_tests.cpp`。它仍然很长，但性质不同：这是私有测试翻译单元路径，完整路径只在 `CMakeLists.txt` 注册一次，文件内部调用的仍是另一个公开 header。改这个测试文件名本身不会改变命令、JSON key、fixture 或 namespace，所以判定为 `safe` 是诚实的。与此同时，本版不把“safe”误写成“必须立刻改”。安全只描述可迁移性，不代表收益足以支付一次独立版本的构建、全量测试、三系统 CI、归档与评审成本。

## 五、为什么一个 safe 不足以触发 E2-M2

E2-M2 的门槛是至少五个安全候选。这个数字的作用不是追求漂亮的批量大小，而是防止治理工作退化成逐个文件的版本仪式。五个候选可以形成一个有内聚性的 rename slice：统一更新构建引用、一次收紧 baseline、一次验证 fixture/CLI 边界。只有一个候选时，代码收益只是一条测试路径变短，运行设计没有改善，且仍要承担完整版本闭环成本。

因此 `safe=1<5` 时的正确输出是 `branch=close`。测试不会自动重命名第十项，也不会把它从 baseline 移除。它保留为未来更大、真正有内聚性的测试文件整理上下文。这个结论也说明“预算是上限而非配额”的实际含义：计划允许最多两版，不表示执行者必须制造第二版。若为了满足视觉上的推进感而单独改 rank 10，反而违反简报中的“凑数改名 = 失败”。

需要特别区分两句话：“本轮没有足量安全候选”与“仓库里没有其他安全候选”。v1662 只证明前十样本中安全项不足五个。第 11 名以后可能仍有私有 `.cpp` 或测试路径，但它们不属于本轮授权的候选集合。若未来要继续，必须另写计划、重新说明收益与停止条件，不能借本版结论无限外推。

## 六、跨平台文本边界：从容错升级为源头固定

v1659 的第一次远端失败来自 Windows checkout 的 CRLF：baseline parser 当时把行尾 `\r` 留在路径中。v1660 增加了单个 CR 归一化和内存样本，这保证解析器健壮；Claude 的 Round 1 评审又把跨仓库重复出现的教训升格为方法内核：baseline、fixture、census 这类文本工件必须用 `.gitattributes` 或确定性 writer 在源头钉死行尾。

v1662 因而在 `.gitattributes` 中明确加入 baseline 和新 TSV 的 `text eol=lf`。census 合同还会读取属性文件，缺少任一规则就失败；TSV parser 对 `\r` 零容忍。两层职责不同：Git 属性保证跨平台 checkout 一致，parser 拒绝 TSV 漂移；旧 baseline parser 继续保留 CRLF 容错，是为了历史可用性和防御性，并不与源头固定冲突。这样不是“遇到第三次再修”，而是在新文本工件生成时购买可移植性。

## 七、测试分别保护什么

第一类断言保护候选选择。测试从真实 baseline 重算 1,618 个条目的统一排名，要求 TSV 恰好十行，并逐行比较 rank、kind、length、name。手工交换同长度条目、删掉一个公开标识符、把第十一名抬进来都会失败。

第二类断言保护 pin 证据。公开 header 路径必须位于 `include/minikv/` 且文件真实存在；公开标识符必须能在指定 header 的真实字节中找到。它防止文档随意说“这是 API”，却没有机械落点。

第三类断言保护 safe 判定。唯一 safe 项必须是 `tests/` 路径，pin 必须为 `none`，机械证据必须指向 `CMakeLists.txt`，且完整路径只能注册一次。这不是完整的语义证明，但足以证明路径身份属于私有构建装配，而不是运行协议。

第四类断言保护分叉。6F/4I、9 pinned/1 safe 都是精确值；safe 数量没有达到五时输出 close。未来若 baseline 收紧后前十发生变化，维护者必须更新 TSV、文档和分叉判断，不能只改一句说明。

第五类证据保护本版没有越界。最终验证必须重新构建、运行全部 351 条 CTest、计算冻结 fixture manifest，并执行真实 SMOKEJSON、CHECKJSON LOAD 和 SHARDJSON。census 绿只能证明治理合同，不能单独证明 no router、no write、no execution；这些运行边界仍由真实 CLI 输出承担。

## 八、输入、输出与上下游如何理解

本版输入有三层：第一层是 v1661 冻结的 `config/elegance-name-baseline.txt`；第二层是 Round 2 简报的五候选阈值；第三层是公开 header、CMake 注册和 `.gitattributes` 的当前仓库事实。执行时不读取 Node/Java 运行服务，也不消费新的跨项目 fixture。Node 最新 Round 2 计划只要求 mini-kv 在 capstone 复跑期间保持 CLI 输出稳定，本版与该要求一致。

输出同样分三层：机器数据是 TSV，机器裁决是 census executable 的 `pin_audit=9/1 branch=close`，面向维护者的解释是 `docs/elegance-round2-pin-audit.md`。三层互相引用但不互相替代：TSV 没有散文理由，测试不承担长篇解释，Markdown 不负责计算真值。

控制面看到 `branch=close` 时，应理解为“Round 2 的 E2-M2 未获触发条件”，而不是“执行被阻塞”或“所有优雅债务清零”。`read_only`、`execution_allowed`、`order_authoritative` 等运行字段不会因审计变化；它们仍应分别保持只读、禁止执行、无订单权威。任何未来消费者也不应把本版审计结果当作启动 Java、连接网络或写入存储的信号。

## 九、对抗性自审与维护价值

最强质疑是：前十名里文件路径和 namespace 多次来自同一个家族，是否重复采样导致 safe 数量偏低？回应不是主观解释，而是固定算法：审计对象是“名字”而非“去重后的业务概念”，文件路径和 namespace 分别代表 include 兼容与符号兼容，迁移成本不同。若按家族去重，就会擅自改变简报定义，并把更低排名对象推入样本。测试锁定 tie-break 后，执行者无法为获得某个分叉结果而换样本。

另一个质疑是：既然 rank 10 已证明 safe，为什么不顺手改掉？因为版本边界要求一个大小合适、能体现独立价值的主题。单个测试路径改名不改善业务结构，反而产生全套发布仪式；保留它并记录证据，比制造一个低价值“优化版”更符合顶级工程判断。童子军规则也没有被绕开：本版触碰的是 census 测试，离开时它增加了结构化审计且仍低于巨型文件风险；没有触碰 rank 10 文件，所以没有义务借题扩张。

一句话总结：v1662 给后续 Node/Java/mini-kv 松耦合协作带来的价值，不是多一个短文件名，而是一个跨平台、可失败、不会自我凑数的停止判据，让 mini-kv 在 capstone 复跑前保持合同稳定，同时把未来是否继续偿还命名债务留给新的、明确授权的高价值计划。
