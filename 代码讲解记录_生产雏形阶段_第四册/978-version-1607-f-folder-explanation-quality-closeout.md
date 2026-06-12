# v1607 f-folder 解释质量收口

## 目标、角色和不是什幺

v1607 的目标是把前面几轮被拆碎的 f-folder 解释质量工作收束成一个单独的大版本证据层。用户已经明确指出，连续五个很薄的版本如果只是在同一条质量规则上前后移动，本质上等同于一个版本，只是粒度被切碎了。这个判断是对的，所以本版不再继续把一个方向拆成 v1607 到 v1611，而是把“中文讲解、三千字下限、禁止硬凑、字数不够就加大版本工作量、截图目录只在有真实截图时创建、mini-kv 不扫描 Node f 文件夹、mini-kv 不导入 Node 质量路由、mini-kv 不改历史讲解”这些要求一起纳入一个读证据命令和一组可测试的 JSON 字段中。

这个模块的角色不是替代人工判断，而是把人工判断变成以后能被命令、fixture 和测试反复看见的边界。过去的问题并不是没有讲解，而是讲解和版本粒度之间缺少硬约束：版本可以做得很薄，讲解却被要求写成多篇；或者文档可以写得很短，靠习惯说“下次注意”。v1607 改变的是治理方式。它把“每个版本至少一篇三千字级别的中文讲解”记录成 `minimumWalkthroughChineseCharacterCount`，把“字数不够就加大版本工作量”记录成 `insufficientWordCountExpandsVersionWorkload`，把“禁止硬凑”记录成 `fillerPaddingAllowed:false`。后续维护者读到这些字段，不需要回忆这次对话，也能知道质量门槛在哪里。

本版明确不是新的执行入口。它不会启动 mini-kv 服务，不会启动 Node 或 Java，不会打开生产执行，不会安装 active shard router，不会路由写入，不会写 WAL，不会扫描 Node 的 f 文件夹，不会批量重写旧讲解，也不会把图片目录当作空壳先建出来。它只发布一个 read-only JSON 证据命令，让控制面能够判断当前 mini-kv 对“解释质量”和“版本粒度”的自我约束已经被显式冻结。这个“不是什么”很重要，因为文档治理一旦和执行治理混在一起，就会让一个本来只该说明的命令变成潜在的执行暗门。v1607 坚持把它留在元数据层。

## 为什么一个版本要做厚

这次版本粒度的核心变化，是从“推进多个小号版本”转向“一个版本承载完整闭环”。如果一个主题只有一句规则，例如“以后中文书写”，那单独发一个版本就太薄；如果再发一个版本写“至少三千字”，再发一个版本写“禁止硬凑”，再发一个版本写“截图不要挤进一个文件夹”，那每个版本都能跑测试，但工程价值会被编号稀释。真正的维护价值是把这些规则放到一个可解释、可验证、可追溯的质量包里，让命令输出、测试断言和讲解文档互相印证。

因此 v1607 采用五条内部质量轨道，但只占一个版本号。第一条轨道是源证据冻结：v1606 的 external artifact dry-run closeout 仍然是来源，它说明 mini-kv 已经把生产执行外部 artifact 的缺口收住。第二条轨道是中文讲解规则：后续非微小版本的讲解默认中文，不再用英文摘要作为主要讲解。第三条轨道是三千字下限：不是要求随便堆字，而是要求一个版本必须有足够实现、测试、归档、重构或边界说明，能够自然解释到这个量级。第四条轨道是归档布局：机器证据和人类解释分开，图片目录只有在确实生成截图时才出现。第五条轨道是执行边界：这个质量规则不能顺手打开服务、路由、写入、WAL 或生产执行。

这种做法也把“工作量不够”从一个主观感觉变成了工程动作。如果讲解写不到三千字，不能把同一句话改写十遍，也不能把无关历史流水账塞进去，而是说明这一版没有足够的真实工作，应当继续补实现、补测试、补拆分、补 fixture、补 CLI smoke、补归档说明，直到这一版确实值得一篇完整讲解。换句话说，讲解不是版本的装饰，而是版本是否足够成型的反向压力。

## 入口命令和响应结构

本版新增的入口命令是 `SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON`。它被接入 `src/command.cpp`，走普通的 `CommandProcessor` 分发路径；如果命令后面带额外 token，就返回 `ERR usage: SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON`。这个处理方式和前面的只读证据命令保持一致，避免出现“同名命令还接受参数”的歧义。命令元数据注册在 `src/command_catalog.cpp`，分类是 `read`，`mutates_store=false`，`touches_wal=false`，dispatch verb 是 `RuntimeEvidence`，不参与 key completion。这说明它是运行时可读取的证据，但不碰 store 和 WAL，也不需要用户传 key。

核心响应由 `src/shard_preview_f_folder_explanation_quality_closeout.cpp` 生成，顶层 contract 是 `shard-f-folder-explanation-quality-closeout.v1`。它先标明来源：`sourceNodePlan` 指向 Node 侧 v2103 的 f-folder 解释扩展收口计划，`sourceExternalArtifactCloseoutReleaseVersion` 指向 v1606，`sourceFrozenFixturePath` 指向 `fixtures/release/shard-readiness-v1606.json`。这里的含义是：mini-kv 只读了 Node 计划方向，并没有进入 Node 项目改文件；mini-kv 的本地来源仍然是自己的 v1606 release fixture，而不是把 Node 文档当成运行时输入。

响应里的 `granularityPolicy` 说明本版是单大版本模式。`singleVersionLargeScope:true` 表示一个版本可以承载多条内部质量轨道；`multiVersionFragmentationAllowed:false` 表示以后不应把同一条质量主题拆成许多薄版本来满足“推进数量”。`archivePolicy` 说明 f-root 归档如何处理：`archiveRoot:"f"`、`archiveDirectory:"f/1607/"`、`pictureDirectoryCreatedOnlyWithRealScreenshot:true`、`keepPictureDirsAbsentWithoutScreenshots:true`。这些字段把用户提到的“截图讲解不要挤入一个文件夹，可以自己建 f 文件夹”的要求进一步约束成版本目录，而不是所有图片和解释无差别堆到一个公共目录。

最关键的是解释质量字段。`chineseWritingRequired:true` 说明后续 mini-kv 讲解默认中文。`minimumWalkthroughChineseCharacterCount:3000` 说明一个非微小版本至少要有一篇三千字级别中文讲解。`insufficientWordCountExpandsVersionWorkload:true` 说明如果讲解无法自然达到这个量，版本应该继续加工作量，而不是降低标准。`fillerPaddingAllowed:false` 则明确禁止硬凑。`longChineseExplanationsRequired:true` 和 `shortExplanationsRejected:true` 继续保留旧的长讲解约束，但这次它们不再只是宽泛口号，而是和三千字下限、工作量加厚、禁止填充一起进入 validation。

## 模块职责拆分

本版没有把所有逻辑堆进一个巨型源文件。`include/minikv/shard_preview_f_folder_explanation_quality_closeout.hpp` 暴露对外接口，只声明格式化 JSON、状态、digest marker 和规则数量。这个头文件保持很薄，方便 `command.cpp` 和 `shard_readiness.cpp` 引入。实现文件 `src/shard_preview_f_folder_explanation_quality_closeout.cpp` 负责组装完整证据，它知道本版 release 是 v1607、来源 release 是 v1606，也知道要把规则、检查、验证三块拼进同一个 JSON。

规则目录放在 `src/shard_preview_f_folder_explanation_quality_closeout_rules.cpp`。这里记录 13 条规则，包括来源冻结、中文书写、三千字下限、字数不够扩工作量、禁止硬凑、单大版本、人与机器证据分离、图片目录必须有真实截图、f-root 版本目录、讲解结构保留、禁止扫描 Node 文件夹、禁止生产执行、短讲解失败。规则的价值在于给人看，它讲的是“以后应该怎幺办”。对应头文件只暴露 span 和格式化函数，外部调用者不需要知道内部数组形状。

检查目录放在 `src/shard_preview_f_folder_explanation_quality_closeout_checks.cpp`。这里记录 19 条检查，粒度比规则更接近验收项。比如 `minimum-3000-chinese-character-walkthrough` 保护讲解字数下限，`no-filler-padding` 保护禁止硬凑，`workload-expands-if-explanation-short` 保护“字数不够就加大版本工作量”，`no-node-folder-scan` 保护 mini-kv 不越界扫描 Node，`no-router-or-write`、`no-wal-touch` 和 `no-execution-authority` 保护运行时边界。检查 JSON 会在每条检查上重复带出关键布尔字段，让审阅者不用跳到 validation 才能看到边界。

验证逻辑放在 `src/shard_preview_f_folder_explanation_quality_closeout_validation.cpp`。它不是简单返回 true，而是把验证拆成几个锁：`sourceLocked` 确认 v1606 来源阶段完整；`explanationPolicyLocked` 同时要求 Node v2103 已观察、中文书写开启、三千字下限达到、工作量扩展规则开启、硬凑关闭、短讲解拒绝、人类解释和机器证据分离；`archivePolicyLocked` 要求图片目录不能空建；`granularityPolicyLocked` 要求单大版本而不是多小版本；`crossProjectBoundaryClosed` 要求不改历史、不扫 Node、不导入 Node 路由、不空建图片目录；`authorityBoundaryClosed` 要求没有服务启动、router、write、store mutation、WAL 或 execution。只有这些锁全部通过，`fFolderExplanationQualityCloseoutValidationPassed` 才会是 true。

## SHARDJSON 和命令目录接入

`src/shard_readiness.cpp` 把当前 release 滚到 v1607，并把顶层 `status` 改成 `f-folder-explanation-quality-closeout-single-large-version-read-only`。这意味着 `SHARDJSON` 现在以 v1607 作为当前证据状态，但旧的 v1606 external artifact closeout section 仍然存在。这样做可以同时满足两个目标：一方面，当前版本确实推进到了新的解释质量规则；另一方面，上一层生产执行外部 artifact 缺口仍然可以在同一个 JSON 里被看到，没有因为新版本而丢失。

`commandCatalogQuality` 也从 v1546 到 v1606 更新为 v1546 到 v1607，scope 改成包含 f-folder explanation quality gates，`commandCount` 通过 `command_catalog::count()` 得到，避免 digest 或质量字段继续写死旧的 93。命令目录本身新增一条 `SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON`，所以 `COMMANDS` 会显示 `command_count=94`，`COMMANDSJSON` 也会暴露新命令的 category、mutates_store 和 touches_wal 元数据。自动补全由同一份 command catalog 派生，`line_editor_tests.cpp` 也覆盖了 `SHARDF` 前缀。

这里有一个容易误读的点：`sourceNodePlan` 指向 Node v2103，但 mini-kv 没有读取 Node f 文件夹、没有调用 Node 质量路由、没有导入 TypeScript 模块。这个字段只是说明计划背景，和运行时依赖完全不同。真正的 mini-kv 来源是自己的 `fixtures/release/shard-readiness-v1606.json`。这个分层能避免跨项目集成被说得比实际更成熟：当前仍然是单项目验证加跨项目契约对齐，不是 Node/Java/mini-kv 联合运行。

## 关键字段怎样理解

`readOnly:true` 是整个命令的第一层语义。它表示命令只是读证据，不改变 store，不生成 runtime artifact，不创建 sibling 项目输出。`mutatesStore:false` 进一步说明命令不会走 `SET`、`DEL`、`LOAD` 这类写路径，也不会为了归档质量去写入存储状态。`touchesWal:false` 说明它不会产生 WAL 条目，也不会为了“证明没有写”去重放或 compact WAL。`executionAllowed:false` 是最重要的门：即使字段里提到 production execution、artifact、archive、Node plan，它也没有给任何执行动作授权。

`scansNodeFolders:false` 和 `importsNodeQualityRoute:false` 保护跨项目边界。mini-kv 可以读 Node 计划文件来判断方向，但不能顺手扫描 Node 的 f 文件夹，更不能把 Node 的质量路由或实现代码搬到 C++ 侧执行。`rewritesHistoricalWalkthroughs:false` 保护历史文档边界。过去 250 版讲解可以清算和索引，但如果用户没有要求迁移，就不应该为了“看起来整洁”批量改名、搬目录，制造巨大无意义 diff。`createsPictureDirsWithoutEvidence:false` 则解决截图目录的问题：有真实截图才创建图片目录，没截图就不要空建，避免归档看起来完整但实际没有证据。

`minimumWalkthroughChineseCharacterCount:3000` 不应该被理解为机械字数崇拜。它是一个工程信号：如果一个版本需要被称为中大版本，它应该有足够多的实现点、测试点、边界点和维护判断，可以让讲解自然展开到三千字级别。`fillerPaddingAllowed:false` 正是为了防止反向激励。如果维护者为了过测试而堆同义句，那说明版本质量没有提升，反而增加阅读负担。正确做法是加厚版本本身，例如补一层 validation 失败路径、补 CLI smoke、补 fixture 当前链断言、补归档说明、拆出重复逻辑、或者整理索引。

## 测试保护什幺

本版测试分成几类。`f_folder_explanation_quality_closeout_rules_tests.cpp` 保护规则目录的数量和核心 rule code，确保中文书写、三千字下限、工作量加厚、禁止硬凑都不是只写在文档里。`f_folder_explanation_quality_closeout_checks_tests.cpp` 保护 19 条检查和每条检查携带的边界字段，确保 `chineseWritingRequired`、`minimumWalkthroughChineseCharacterCount`、`insufficientWordCountExpandsVersionWorkload`、`fillerPaddingAllowed:false` 会出现在机器可读证据里。

`f_folder_explanation_quality_closeout_validation_tests.cpp` 保护失败路径。它不仅测 happy path，还逐个把来源不完整、未观察 Node v2103、中文规则关闭、三千字下限不足、工作量扩展关闭、硬凑允许、短讲解放行、扫描 Node、导入 Node 质量路由、启动服务、打开 router、允许写入、触碰 WAL、允许执行、规则数量不对、检查数量不对等情况改成坏值，然后确认 validation 会变成 false。这类测试的意义是防止以后有人只改一个字段让 JSON 看起来漂亮，却没有同步验收逻辑。

`f_folder_explanation_quality_closeout_tests.cpp` 保护命令本体，确认独立 JSON 命令能输出 contract、命令名、来源、三千字规则、单大版本策略、validation 和边界字段，并确认带多余参数会失败。`f_folder_explanation_quality_closeout_fixture_tests.cpp` 保护生成出来的 `shard-readiness.json` 和 `shard-readiness-v1607.json`，确保当前 fixture 真正滚到了 v1607。`command_tests.cpp`、`command_catalog_tests.cpp` 和 `line_editor_tests.cpp` 保护公共命令表面：`COMMANDS` 数量是 94，新命令在 `COMMANDSJSON` 可见，自动补全能从 catalog 里派生出 `SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON`。

新增的 `f_folder_explanation_quality_closeout_walkthrough_tests.cpp` 是文档质量的硬门。它直接读取本篇讲解，统计 UTF-8 中文字符数量，要求至少 3000 个中文字符，并检查“每个版本至少一篇三千字级别的中文讲解”“字数不够就加大版本工作量”“禁止硬凑”等关键句存在。这条测试不是为了奖励冗长，而是为了把用户新增规则落到可执行的检查里。以后如果有人把讲解改成很短的摘要，测试会直接失败。

## 真实运行和归档意义

本版后续闭环会重新配置和构建 `cmake-build-v1607`，跑新增 f-folder 质量测试、命令目录测试、SHARDJSON 测试、fixture 测试和全量 CTest，再用 CLI smoke 跑 `SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON`、`CHECKJSON SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON`、`SHARDJSON`、`COMMANDSJSON`。这些命令不是为了启动服务，而是为了证明同一份构建产物能够从用户可见入口读出相同契约：新命令可执行为只读证据，CHECKJSON 认定它不写、不触 WAL、不允许执行，SHARDJSON 嵌入了新 section，COMMANDSJSON 暴露了新命令元数据。

归档会放在 `f/1607/图片/` 和 `f/1607/解释/说明.md`。这次有构建、目标测试、全量测试和 CLI smoke 的真实截图，所以图片目录会真实存在；如果某个版本没有截图，就不应该空建图片目录。说明文档会用中文解释每张截图对应的命令、结果和意义，把机器输出和人类说明分开。这样的归档不会把截图塞到同一个公共文件夹，也不会把所有解释堆成一个越来越难翻的巨型文件。

## 给后续协作的价值

v1607 给后续 Node、Java、mini-kv 松耦合协作带来的价值，是把“版本要厚、讲解要中文且有足够信息量、不要硬凑、不要为了文档越界执行”变成 mini-kv 自己能证明的契约。Node 后续可以读取这个证据，知道 C++ 侧没有去碰 Node f-folder 或质量路由；Java 后续可以把它当作一个解释质量和执行边界的 echo 来源，而不是把它理解成生产执行授权；mini-kv 自己则可以在未来继续推进功能和重构时，用这条规则约束节奏：版本小了就继续加厚，讲解短了就回头补真实工程工作，边界字段一旦变红就不能收尾。

一句话总结：v1607 不是把五个碎版本换一个名字，而是把版本粒度、中文讲解、三千字质量门槛、禁止硬凑、f-root 归档和只读无执行边界合成一个可命令读取、可 fixture 冻结、可测试失败的单大版本规则。

## CI 收口补丁

版本收口时远端 GitHub Actions 暴露了一个额外问题：Ubuntu 和 macOS 已经通过，Windows 的 configure 和 build 也通过，但 Windows 的 `Test` 步骤长时间停在 in progress，且没有日志可取。这个现象不像普通断言失败，更像 Windows/MSVC Debug 路径下某条测试触发了等待。排查后最可疑的是 walkthrough 测试直接用中文目录字面量拼 `std::filesystem::path`，如果 MSVC 对 narrow 字符串路径的编码解释和仓库实际文件名不一致，`exists()` 失败后 debug assert 可能挂住 CI。

收口补丁没有降低质量门槛，而是把测试路径做得更稳：`f_folder_explanation_quality_closeout_walkthrough_tests` 改为从源码根递归寻找 ASCII 文件名 `978-version-1607-f-folder-explanation-quality-closeout.md`，找到后仍然读取真实中文讲解正文、统计中文字符数并检查关键中文规则句。这样测试继续证明“中文、三千字、禁止硬凑、字数不够加大工作量”，但不再把中文目录编码作为 Windows CI 的隐含前提。

同时 CI 的 Test 步骤增加 `timeout-minutes: 30`，CTest 增加 `--timeout 120 --progress`。这层改动不是为了掩盖失败，而是为了让以后任何测试卡住时能快速暴露最后执行到哪一条测试，避免再次出现长时间无日志挂起。这个补丁仍然属于 v1607 的质量收口，因为它服务的正是本版新增的讲解质量门槛，并把远端 CI 的可诊断性一起纳入完成标准。
