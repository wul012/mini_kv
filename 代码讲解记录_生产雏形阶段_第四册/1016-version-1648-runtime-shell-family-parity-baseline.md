# v1648 代码讲解：runtime-shell 五回执迁移前字节基线与测试职责拆分

## 一、本版目标、角色和“不是什么”

v1648 暂停生产 formatter 迁移，先为 runtime-shell 连续链上的五份回执建立可复现的迁移前基线。五份回执依次是 disabled runtime shell、disabled runtime shell candidate gate、runtime shell decision record、post-decision plan intake，以及 chain stop-or-prerequisite。它们分别承接 Node v295 到 v304 的只读决策证据，在 mini-kv 中形成从“禁用设计”到“前置条件不足，继续停止”的完整历史链。

本版的角色是测试架构优化和迁移风险清零。它不把五个旧字符串 formatter 改成 builder，不修改任何 release fixture，不提高 builder census，也不增加新命令、新 router 或新执行入口。它更不实现 runtime shell，不实例化 credential provider 或 resolver client，不读取 credential value，不解析 raw endpoint，不发送 HTTP/TCP，不写 Store、WAL、approval ledger 或 schema，不执行 LOAD、COMPACT、RESTORE、SETNXEX，不自动启动 Node/Java/mini-kv，也不取得 audit authority 或 order authority。

之所以单独做这一版，是因为 v1647 刚关闭过一处只有一个空格的 runtime 漂移。若直接假定相邻五份回执同样干净，再批量改生产代码，一旦 parity 失败，就难以区分失败来自历史 fixture 漂移、formatter 既有缺陷，还是 builder 迁移错误。v1648 把“先测量，再迁移”固化成机械门，后续版本只需要在已知基线上改变一种变量。

## 二、五份回执为何是一族

第一份 disabled runtime shell receipt 说明 mini-kv 只回显 Node v295 的设计审查，不参与实现。第二份 candidate gate receipt 消费 Node v297 的候选门，列出 disabled-by-default、人工批准、abort 语义、no-network 测试等条件仍未满足。第三份 decision record receipt 消费 Node v299 的决定，继续把 runtime shell 保持在 blocked 状态。第四份 post-decision plan intake receipt 消费 Node v301 的延续计划，只允许 Java 与 mini-kv 提供只读 echo。第五份 stop-or-prerequisite receipt 消费 Node v304 的结论，明确缺少六项前置证据时必须停止 runtime shell 链。

这五份输出的业务文字不同，字段数量也不完全相同，但安全边界一致：它们都是 evidence receipt，不是 command；都是 non-participation，不是 implementation；都把 credential、raw endpoint、network、write、schema、restore 和自动启动保持关闭。按这条连续链建立一个家族测试，比把断言继续塞进通用 builder 测试更容易看清迁移范围，也能防止下一版只迁移其中一份却误称整族完成。

## 三、输入、入口和输出怎样流动

测试入口是新文件 `tests/runtime_receipt_runtime_shell_family_baseline_tests.cpp`。它给五个 public formatter 传入同一组标准只读命令：`INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON`。这组输入不是让 formatter 执行命令，而是作为已经发生过的只读观察清单参与 digest 和证据描述。每个 public formatter 仍由 `include/minikv/runtime_evidence_receipts.hpp` 声明，由各自生产源文件实现。

对每个候选，测试先通过 `release_fixture_path` 找到冻结文件，再用 `read_fixture_text` 读取原始文本。`extract_json_object_field` 只提取外层 fixture 中与 public formatter 对应的嵌套对象，不比较外层 release 元数据。随后调用 formatter 得到当前 runtime object。两边进入四层检查：固定 fixture 字节数、带 v1648 前缀的稳定摘要、顶层字段数，以及完整原始对象相等。

输出不是一份新 fixture，也不是测试生成的黄金文件。测试成功时不写磁盘，只返回进程退出码 0；任意字节、字段、摘要或边界变化都会触发断言失败。这样输入是冻结 fixture 加标准只读命令，处理是结构提取与 public formatter 调用，输出是可供 CTest 判断的确定性 parity 结果，整个过程没有网络、存储写入或命令执行副作用。

## 四、共享测试支撑为何需要拆出

v1647 时，`runtime_receipt_json_builder_tests.cpp` 同时承担 builder 基元测试、fixture object 提取、稳定摘要、顶层字段计数、具名 apostrophe 兼容和多个 receipt family 基线。继续把五个候选加进去会让一个通用测试文件同时知道越来越多领域回执，职责开始混合。本版将真正通用的五个 helper 移到 `tests/runtime_receipt_parity_support.hpp`：`stable_fixture_digest`、`count_occurrences`、`count_top_level_fields`、`replace_once` 和 `named_apostrophe_runtime_surface_canonical_json`。

这个 header 只属于测试，不进入 public include，也不会增加生产库 API。helper 仍使用项目已有的 FNV digest、严格 exactly-once 替换和 JSON 字符串/嵌套深度感知字段计数，没有引入第三方 JSON 库或宽松 normalizer。原 builder 测试从 447 行降到 369 行，保留 builder escaping、raw nested value、提取器 fail-closed、no-network/abort 基线和 approval family 基线；新家族门为 100 行，共享支撑为 93 行。总代码略有增长，是因为增加了五份独立机械证据，不是把同一逻辑复制两遍。

拆分后的边界很清楚：通用 builder 测试负责“工具是否正确”；runtime-shell family 测试负责“这五份领域输出当前是什么”；各自原有领域测试继续负责“命令与下游消费表面是否仍包含必需字段”。以后再增加候选，优先进入对应 family 文件，而不是重新膨胀通用测试。

## 五、冻结矩阵记录了什么

v1648 的实测矩阵如下：

| 回执 | fixture 字节 | 顶层字段 | v1648 摘要 | runtime parity |
| --- | ---: | ---: | --- | --- |
| disabled runtime shell | 12523 | 110 | `fnv1a64:08807cb6305602e8` | 完全相等 |
| candidate gate | 14936 | 114 | `fnv1a64:d1a2681a73de4b8e` | 完全相等 |
| decision record | 14208 | 113 | `fnv1a64:9926939aac05efd6` | 完全相等 |
| post-decision plan intake | 14084 | 113 | `fnv1a64:e6c73dbf0f4bc4d1` | 完全相等 |
| stop-or-prerequisite | 16306 | 115 | `fnv1a64:b6984418a3b69de7` | 完全相等 |

摘要前缀包含 v1648 和候选名称，因此它表示“v1648 对这一个 fixture 子对象的冻结判断”，不会与其他版本或对象的相同字节混淆。字段数保护 schema 表面，字节数能快速识别增删，摘要便于归档和人工复核，完整对象相等则是最终门。四者有一定重叠，但失败时提供的定位层次不同。

五份 fixture 和 runtime 中 `\\u0027` 出现次数均为 0。这一点也被断言冻结。它与 no-network、signed-human、approval-prerequisite 等已知 family 不同，不需要 `Node vNNN\\u0027s` 到直接 apostrophe 的兼容转换。后续迁移若出现 apostrophe 或任何其他差异，不能复制旧 family 的 waiver；必须先证明差异来源并另行决策。

## 六、完整字节相等到底证明了什么

完整相等证明，在标准只读命令输入下，当前生产 formatter 输出与提交的 fixture 子对象在字段名称、字段顺序、JSON compact 形式、字符串转义、数字和布尔文本、嵌套对象/数组顺序以及 digest 字节上完全一致。它比“解析后对象相等”更严格，因为额外空格、不同转义或字段换序都会失败。v1647 的 `} },` 缺陷若存在于这一族，会被这道门立即发现。

但它不证明未来 builder 改写自动正确，也不证明所有可能输入都等价。它冻结的是项目实际使用的标准 read-command surface，后续每次迁移仍要重新构建、运行对应领域测试和完整 CTest。它也不证明 fixture 业务内容永远正确；fixture 是跨项目历史合同，本版只证明 runtime 没有偏离它。若合同本身要升级，必须走单独的版本化 fixture 流程，而不能借重构版本静默改写。

因此本版把五个候选标记为“迁移前 parity 已知”，而不是“迁移完成”。builder census 仍为 28 sources、27 formatter owners、9 builder-backed、18 pending、1 named waiver。没有提高 floor，正是为了让度量只反映生产 formatter 真正迁移的数量，不把新增测试冒充迁移进度。

## 七、关键边界字段如何解释

每个 candidate 都必须含 `read_only=true` 和 `execution_allowed=false`。前者表示输出只能被读取和核验，后者明确否定从 receipt 触发动作。`runtime_shell_implemented=false` 证明没有实现 shell；各回执自己的 `*_non_participation_receipt_only=true` 把角色限制在对应历史阶段，而不是通用运行许可。

`credential_value_read=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false` 分别关闭秘密读取、原始地址解析和外部连接。`approval_ledger_written=false` 关闭人工批准账本写入，`automatic_upstream_start=false` 关闭自动拉起其他项目。原领域测试还继续检查 Store/WAL、schema migration、restore/load/compact、SETNXEX、managed audit 和 authority 等更宽边界；本版家族门选择最稳定的核心交集，避免错误地要求五份历史 schema 拥有完全相同的所有字段。

控制面读到这些回执时，应把它们理解为“mini-kv 能证明自己没有参与”，不能据此路由到运行时 shell，也不能把 `ready_for_node_vNNN` 一类字段解释成 execution approval。ready 表示下一步可以核验某份上游证据，execution_allowed 才是动作边界，而这里始终为 false。

## 八、每类测试分别保护什么

新增 `runtime_receipt_runtime_shell_family_baseline_tests` 保护五个 formatter 与 fixture 子对象的原始字节一致，并冻结五组尺寸、摘要、字段数和核心边界。原有 `runtime_receipt_json_builder_tests` 在 helper 抽出后仍保护 JSON escaping、嵌套 raw value 不被二次转义、提取器对缺字段、数组、重复字段和损坏 JSON fail closed，以及已有 family 的具名 canonical 规则。

五个领域测试分别保护 public command response、fixture digest、source Node shape、release/version hint、完整 boundary 和 downstream 嵌入。`receipt_builder_census_contract` 继续报告 9/18/1，证明本版没有虚报生产迁移。focused lane 将通用 builder、家族 baseline 和五个领域测试合并，结果为 7/7。预检 full CTest 为 346/346，覆盖 Store、command、WAL、snapshot、RESP/TCP、OSFS、shard readiness、archive、release manifest 和其余 receipt family。

这些门不能相互替代。摘要相同在理论上仍依赖 hash，完整对象比较给出直接证据；字段数相同不保证字段内容正确；领域 fragment 测试不保证 compact bytes；全量 CTest 能发现外溢，却不说明五个对象为何可迁移。把职责拆开后，失败信息会更接近根因，也减少维护者在一个 400 多行测试中寻找领域上下文的时间。

## 九、失败信号如何定位到真实原因

若 `expected_fixture_size` 首先失败，应先检查 frozen fixture 是否被改动，而不是立刻调整常量。版本规则禁止为了迁移通过而改 fixture，因此任何 fixture diff 都要追溯到独立合同变更。若字节数不变但摘要失败，常见原因是字段顺序、同长度文本或布尔值发生变化；这时完整对象比较仍会失败，维护者应在 fixture 与 runtime 的首个不同位置定位生产输出，不能用重新计算摘要掩盖差异。

若 fixture 字节和摘要保持，但 runtime 字节数或字段数失败，说明 public formatter 已经漂移。字段数增加通常意味着新字段进入合同，字段数减少则可能是拼接遗漏；两者都不是普通重构。若 fixture/runtime 字段数相同、长度也相同而整对象失败，仍可能存在字段换序或等长替换，必须按原始字节处理，不能退回解析后比较。若 `\\u0027` 计数变成非零，则表示出现新的转义表面；本族没有已批准兼容规则，应先暂停，而不是复用其他 family 的 apostrophe helper。

若完整 parity 仍通过但核心边界断言失败，通常说明候选描述中的 field 名写错，或某份历史 schema 并不拥有该交集字段。v1648 初次测量时 candidate gate 的 mode fragment 就暴露了这种测试描述错误：真实字段是 `runtime_shell_candidate_gate_non_participation_receipt_only`，不是带 `disabled_` 前缀的猜测名称。修正的是新测试的字段描述，生产代码和 fixture 均未改变。这个小红灯说明基线必须从实际输出中取证，不能只根据文件名推断 schema。

若新增 family test 通过而原五个领域测试失败，则问题多半在外层 command response、release hint、digest 嵌入或下游消费路径；family test 只比较 formatter 对象，不替代完整 command surface。反过来，领域 fragment 测试通过而 exact parity 失败，说明关键字段仍在但字节合同已漂移。最终 full CTest 再检查共享 helper 抽取是否影响其他 family。按这个顺序判断，可以避免看到一个红灯就同时修改 fixture、formatter 和测试，保留单变量修复的可审查性。

## 十、对抗性自审

最强质疑是：这一版没有改生产代码，是否只是为了凑版本。答案要看它是否降低了下一步不可见风险。这里新增的不是说明性文档，而是第 346 个可执行 CTest；任意候选 fixture 被改、formatter 多一个空格、字段换序、摘要变化、边界翻转，都会使版本机械失败。同时通用测试 helper 被抽出，原集中测试减少 78 行，后续 family 不再继续挤入同一文件。这是测试资产和维护结构的实质变化。

第二个质疑是固定字节数、摘要、字段数和完整相等是否过度重复。若只保留完整相等，失败只能告诉维护者“两边不同”；固定元数据可以快速区分 fixture 自身变化、schema 增删和纯字节漂移，并能写入归档供外部评审复算。它们都由同一原始 fixture 动态计算和断言，没有复制 fixture 内容，更没有生成第二份黄金文件。

第三个质疑是共享 header 会不会成为新的杂物箱。它目前只有五个无领域名称、已被至少两个测试使用的函数。候选描述、fixture 名、field 名、摘要和边界断言仍留在 family 测试。若未来 helper 只服务单个 family，就应留在该源文件，不应继续塞入共享 header。这个所有权规则比单纯追求 DRY 更重要。

## 十一、后续最优迁移顺序

下一版可以先迁移 disabled runtime shell 与 candidate gate 两份相邻回执。它们是链首，字段数 110/114，当前无任何 canonical waiver，适合验证 shared builder 模式是否能在这族复用。decision record、post-decision intake 和 stop-or-prerequisite 则构成后一段决策链，可在前两份成功后再判断是否同版迁移，避免五个大 formatter 同时变化使审查面过宽。

每次生产迁移都必须保持本版测试不动，先让新 formatter 通过 exact parity，再提高 census floor。若某份迁移需要改 fixture、增加 whitespace/apostrophe normalizer、删除字段数断言或放宽完整相等，应立即停止并单独查漂移。只有 formatter 真实引入 ordered builder 后，9/18/1 才能收紧，测试基线本身不能计入 builder-backed 数量。

一句话总结：v1648 用独立家族门冻结五份 runtime-shell 回执的真实字节、摘要、字段数和安全边界，并把通用 parity helper 从集中测试中拆出，为后续 Node/Java/mini-kv 松耦合证据迁移提供无漂移、可复算、不会打开 router、write、network、credential 或 execution 的起跑线。
