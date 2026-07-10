# mini-kv v1645 代码讲解：approval lifecycle 迁移前 parity 与 census 机械门

## 1. 本版目标与边界

v1645 是一个“先把裁判建好”的重构准备版本。它不修改任何 production receipt formatter，不改 release fixture，不改 digest 输入，不新增命令，也不扩大 router、Store/WAL、credential、raw endpoint、network、schema、restore/load/compact、SETNXEX 或 execution authority。目标是回答两个此前只能靠人工说明的问题：下一组 approval lifecycle formatter 是否真的可以同批迁移，以及“28/28 收敛”里的 28 到底怎样机械复现。

本版选择四份候选：approval-required boundary、approval-prerequisite artifact intake、human approval artifact review、human approval post-echo decision gate。它们属于同一审批生命周期，但历史版本不同、JSON 形状不同，不能因为文件名相似就默认 byte parity。v1645 先读取 frozen fixture 子对象，再调用当前 public formatter，比较大小、版本化 digest、顶层字段数、转义差异和 closed boundary fragments。只有基线明确的文件才允许进入 v1646。

## 2. 四份 fixture 的真实结果

approval-required boundary 是最干净的样本：fixture 子对象 12,949 字节、108 个顶层字段，当前 formatter 与它逐字节完全相等，且两侧都没有 `\u0027`。这意味着 v1646 可直接使用 exact byte oracle，任何一字节变化都失败。

approval-prerequisite artifact 为 16,483 字节、107 字段。fixture 中只有一处 `Node v306\u0027s non-secret artifact contract`，runtime surface 是 `Node v306's non-secret artifact contract`。human approval review 为 17,116 字节、107 字段，唯一差异是 Node v308 的同类 boundary 短语。两者都满足：fixture 只出现一次 legacy phrase、runtime 只出现一次 canonical phrase、runtime 不含 legacy 字节、字段数相等、fixture 比 runtime 多五字节；完成 exactly-once 命名替换后整对象相等。

post-echo 的诊断则阻止了盲目批量迁移。fixture 是 14,169 字节、101 字段，runtime 是 14,165 字节。单个 `\u0027` 变成 apostrophe 应缩短五字节，理论 runtime 应为 14,164；实际多出一字节。逐字节 mismatch 定位到 Node v310 summary 与 `source_node_v309_reference` 之间：fixture 是 compact `}},`，runtime 是 `} },`，多了一个空格。

这个空格不属于 apostrophe compatibility。若测试把两种差异一起“规范化后通过”，就等于给一个 family 两条 parity waiver，违反当前计划的 fail condition。v1645 因此把 post-echo 标为 migration-ready=false：测试仍冻结它的 fixture size、digest、字段数、legacy phrase 和空格位置，并证明漂移范围只有这两处，但不会把它纳入 v1646。

## 3. 通用 helper 如何收敛规则而不扩大规则

此前 no-network 和 signed-human 各有一个几乎相同的函数：断言 fixture 只有一处 `\u0027`，断言 legacy phrase 恰好一次、runtime canonical phrase 尚未出现，然后 exactly-once 替换。v1645 抽出 `named_apostrophe_runtime_surface_canonical_json`，减少重复的控制逻辑。

这个 helper 接收的不是任意正则，也不会把全部 `\u0027` 替换成 apostrophe。每个 family 必须在测试顶部声明完整 legacy phrase 与完整 runtime phrase。helper 先统计次数，再调用 `replace_once`；`replace_once` 自己还会确认没有第二次出现。于是复用的是“严格单次替换机制”，不是“所有撇号差异都可接受”的政策。

四候选用一个结构化 case table 描述 fixture 名、子对象字段名、版本化 digest prefix、预期 digest、预期大小、字段数、legacy/canonical phrase、模式字段和 formatter 函数指针。测试循环统一执行公共断言，但 exact、single-apostrophe-ready 与 post-echo-blocked 三种状态仍明确分支，不把不同语义揉成一个布尔开关。

## 4. 为什么 digest、大小和字段数要同时冻结

只比较字段数不够：字段值改变、顺序改变或字符串转义改变都可能保持字段数。只比较大小也不够：一处增长和另一处缩短可以抵消。只比较 digest 可以发现变化，却无法快速说明变化属于漏字段、转义还是结构层级。

v1645 因此组合四类证据。fixture size 给出稳定字节长度；以版本化 prefix 计算的 FNV digest 钉住完整子对象；`count_top_level_fields` 用一个理解嵌套对象、数组和 escaped string 的扫描器固定顶层 shape；最后 public formatter 与 exact/canonical fixture 做整字符串比较。失败时维护者可以从哪一层变化迅速定位问题，而不是面对一个模糊的 snapshot mismatch。

边界 fragment 断言还要求 `read_only=true`、`execution_allowed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`automatic_upstream_start=false` 以及各 receipt 自己的 mode field。它们不能替代整对象 parity，却能让测试失败信息和安全意图保持可读。

## 5. 28/28 census 的真实含义

旧 note 写着 runtime receipt 全家族 28 个文件，最终要达到 28/28 migrated 或 waiver 齐备。但人工 `rg` 容易混入三个 managed-audit receipt 文件，也容易遗漏一个没有 public JSON formatter 的 retention translation unit。v1645 新增 `cmake/check_receipt_builder_census.cmake`，把口径变成 CMake 可执行门。

脚本只枚举 `src/runtime_*_receipts.cpp`，当前正好 28 个。它读取每个文件，识别 `std::string format_*_receipt_json(...)` owner，并检查是否包含 `runtime_receipt_json_builder.hpp`。结果是 27 个 formatter owner、5 个 builder-backed、22 个 pending。剩余一个 `runtime_credential_resolver_execution_denied_retention_receipts.cpp` 没有 JSON formatter，因此不是“尚未迁移的 formatter”，而是唯一命名 waiver。

CTest `receipt_builder_census_contract` 固定总文件数 28、formatter owner 27、waiver 数 1 和 waiver 精确文件名，并设置 `MIN_BUILDER_BACKED=5`。若有人删除 builder include、悄悄新增 waiver 或新增 receipt 文件却不更新计划，测试会失败。后续 v1646 迁移三份后必须把 floor 提到 8；ratchet 只升不降。

## 6. 输入、输出和失败条件

census 的输入是当前源码树，不读取进度账本。输出包括 sources、formatter owners、builder-backed、pending、waivers 五个计数，以及三份文件列表。它可以通过 `cmake -DSOURCE_DIR=... -DMIN_BUILDER_BACKED=5 -P cmake/check_receipt_builder_census.cmake` 独立复现，也作为普通 CTest 自动运行。

parity test 的输入是四份 frozen release fixture、四个 public formatter 与标准 read-command list。输出不是新 JSON 文件，而是一组会失败的断言。以下任一情况都会使测试失败：fixture 字节被改、digest 变化、字段数变化、named legacy phrase 多一处、runtime 出现 `\u0027`、closed boundary 丢失、exact candidate 不再 exact，或 post-echo 的空格漂移换了位置。

这两个门互补。census 回答“迁移了多少、还剩谁”；parity 回答“候选迁移时必须保持什么”。没有 census，版本可以一直迁移却无法判断终点；没有 parity，census 数字增长可能以公共输出漂移为代价。

## 7. 需求到证据矩阵

| 要求 | 实现 | 机械证据 | 状态 |
| --- | --- | --- | --- |
| 冻结四候选 fixture | case table 的 size/digest/field count | runtime receipt builder test | 完整 |
| 每个 family 最多一条命名 apostrophe 规则 | 独立 legacy/canonical 常量 + exactly-once helper | occurrence 与整对象比较 | 完整 |
| 不掩盖第二类漂移 | post-echo compact/spaced transition 独立断言 | migration-ready=false 分支 | 完整 |
| 机械复现 28 文件口径 | CMake census script | receipt_builder_census_contract | 完整 |
| ratchet 只升不降 | MIN_BUILDER_BACKED=5 | 删除 builder include 会失败 | 完整 |
| 不改生产 surface | 只改 tests/CMake/docs | fixture、formatter、digest 无生产 diff | 完整 |

## 8. 对抗性自审与下一版

最强质疑是：v1645 没改生产代码，是否工作量不足？如果它只新增几条字符串断言，答案会是肯定的。但本版实际解决了两类会让后续批量重构失真的基础问题：它发现了此前未记录的 post-echo 一字节 runtime 漂移，并把“28/28”从散文口径变成会失败的 census ratchet。没有这一步，v1646 很可能把四份文件一起迁移，再为了通过测试引入第二个 waiver，或者错误地把无 formatter 的 retention 文件算作 pending。

下一版范围已经由证据决定：approval-required boundary、approval-prerequisite artifact、human approval review 三份迁移；builder floor 从 5 提到 8；post-echo 不动。随后需用独立版本解决它的 compact whitespace 决策，再决定迁移。这样的切分不是小粒度仪式，而是 stop condition 触发后的风险隔离。

一句话总结：v1645 没有扩大 mini-kv 的任何运行能力，却把下一轮重构的候选字节、历史差异、阻塞项和全局进度全部变成机械证据，为 Node/Java/mini-kv 松耦合证据链提供了比版本号更可信的收敛尺度。
