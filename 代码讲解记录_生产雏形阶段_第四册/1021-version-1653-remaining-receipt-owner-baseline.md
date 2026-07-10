# v1653 代码讲解：剩余 receipt owner 全量冻结与历史漂移落地

## 一、本版目标、角色定位和明确不做事项

v1653 的目标不是再迁移一批 formatter，而是给 v1652 之后剩余的全部 consolidation 对象建立一把独立、可复现、会失败的尺子。v1652 已把 builder census 推进到 18 个 builder-backed owner、9 个 pending owner 和 1 个无目标 formatter 的命名 waiver。此时若直接修改剩余九个文件，执行者既负责改变生产实现，又负责在同一提交里定义“什么算没有变化”，容易出现测试和实现互相迁就。v1653 因而只新增迁移前 oracle：先观察未修改 formatter 与冻结 fixture 的真实关系，再把尺寸、字段、摘要、兼容差异和安全边界写成机械断言。

本版不是新的命令入口，不改变 `SMOKEJSON` 或 `CHECKJSON` 响应，不实现 credential resolver，不实例化 provider/client，不读取 credential value，不接收或解析 raw endpoint URL，不发送 HTTP/TCP，不连接 managed audit，不写 Store、WAL、approval ledger 或 schema，不执行 LOAD、COMPACT、RESTORE、SETNXEX，不自动启动 Node、Java 或 mini-kv，也不取得 managed-audit authority、router authority 或 order authority。九个 production owner、十三个 public formatter、全部 release fixture 和领域测试在本版都保持原样。新增内容只位于 test、CMake 注册、维护说明、证据归档和讲解层。

## 二、为什么九个 owner 实际对应十三个 formatter

机械 census 以源文件作为 owner 单位：`src/runtime_*_receipts.cpp` 一共有二十八个候选源文件，其中二十七个拥有目标 public receipt formatter，一个 execution-denied retention 文件没有目标 formatter并作为唯一 waiver。v1652 后还剩九个 owner，但 owner 不等于 formatter。五个 implementation-chain 文件各拥有一份完整 receipt；`runtime_credential_resolver_receipts.cpp` 同时拥有基础 decision receipt 和 disabled precheck receipt；`runtime_evidence_receipts.cpp` 拥有 manual dry-run command 与 manual connection precheck 两份 receipt；`runtime_sandbox_receipts.cpp` 拥有 disabled adapter、fake transport packet、sandbox endpoint handle 三份 receipt；test-only shell 文件再提供一份，因此总数是十三。

本版没有为了让表格好看而按物理文件逐个建九个小测试，也没有把十三个对象塞回已经冻结的 runtime-shell 或 legacy-resolver oracle。新的 `runtime_receipt_remaining_owner_baseline_tests.cpp` 以“剩余 owner 的共同迁移门”为职责，内部用 `family` 把对象分成 base resolver、implementation chain、sandbox chain 三组。这样后续失败信息既能指向具体 object field，也能说明它属于哪条迁移链；旧 oracle 继续只保护旧 family，避免测试文件长期变成无法定位的巨大杂物箱。

## 三、测试入口的输入和输出是什么

测试有两类输入。第一类是 `fixtures/release/` 下十三个已经提交的 standalone JSON fixture，每个文件都有 wrapper 元数据和一个完整嵌套 receipt object。第二类是同一组标准只读命令 `INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON`，它们被传给十三个现有 public formatter，生成当前运行时 receipt object。测试不通过命令路由间接寻找 formatter，而是保存类型明确的函数指针；如果签名变化或链接目标丢失，编译或链接阶段就会失败。

测试的正常输出是一行一份的基线摘要，列出 family、object field、fixture/runtime 字节数、顶层字段数和兼容类型。真正的判定输出是进程退出码和 CTest 结果：所有断言成立时返回成功；任一 fixture 提取失败、大小变化、字段变化、摘要变化、兼容规则失配或安全边界缺失时立即失败。它不会写 fixture，不会生成新的运行时证据，也不会把测量结果反馈给 production formatter，因此不存在测试执行后修改系统状态的闭环。

## 四、fixture 子对象如何被完整而不是模糊地提取

测试复用 `receipt_fixture_parity.hpp` 的 `extract_json_object_field`。提取器从 wrapper 中按精确字段名定位目标值，确认目标确实以对象开始，然后逐字节跟踪对象和数组深度、字符串状态及反斜杠转义，直到找到与起始花括号对应的结束位置。它不会用“找到下一处右花括号”这种脆弱截断，也不会先把 JSON 解析成 map 后重新序列化，因为 map 重排会抹掉字段顺序和 compact whitespace 这两类本次恰好要保护的表面。

遇到字段重复、目标不是对象、字符串未终止、嵌套未闭合或字段不存在时，提取器返回空值，测试立即断言失败。输出仍然是 fixture 中原始字节的拥有型字符串，所以普通 apostrophe、`\u0027`、逗号位置、数组顺序和每个 nested object 的 compact 拼写都被保留。后续 builder 迁移若只改变一个逗号或一个字段插入点，也无法被“解析后语义相同”掩盖。

## 五、Candidate 表冻结了哪些维度

每一行 `Candidate` 都保存 family、fixture 文件名、object field、fixture 摘要、runtime 摘要、两侧字节数、两侧顶层字段数、兼容规则和 formatter 函数指针。摘要不是 public `receipt_digest` 的替代品，而是测试专用的完整对象指纹：`stable_fixture_digest` 把带 object field 的 v1653 前缀和整段原始对象作为长度分隔输入交给项目现有 FNV-1a 实现。前缀使不同对象即便偶然拥有相同正文也不会共享测试身份，长度分隔避免简单拼接产生边界歧义。

大小先捕捉增删和 escaping 长度变化，字段数区分顶层 schema 漂移与 nested 文本漂移，完整对象摘要给归档和 reviewer 提供短指纹，最后的 raw equality 或命名 canonical equality兜住所有未分类变化。四层检查故意有重叠：重叠不是无意义重复，而是让失败更快暴露类型。比如字段数不变而摘要变化通常意味着顺序、值或空白改变；大小和字段数同时变化更像字段增删；摘要不变而某个安全 marker 缺失在计算上几乎不可能，但显式 marker 仍让边界意图可以直接审查。

## 六、十一份 exact 对象说明了什么

十三份对象中有十一份在首次测量时 fixture/runtime 原始字节完全一致。base resolver 的 disabled precheck 是 10291 字节、118 字段，test-only shell 是 11201 字节、133 字段。implementation chain 的 implementation readiness、implementation plan、disabled fake harness、execution denied、input hardening 分别是 16889/19163/13870/15264/21562 字节，对应 112/121/135/138/113 个字段。sandbox chain 中 connection precheck、disabled adapter、fake transport、endpoint handle 分别是 4176/5361/6040/6340 字节，对应 74/92/92/92 个字段。

这些对象的 fixture/runtime 摘要相同，`\u0027` 计数都为零，而且 compatibility rule 必须是 `exact`。后续迁移不得给其中任何一个临时增加 apostrophe、whitespace、字段忽略或排序规则。只要 builder 输出与 fixture 有一字节不同，测试就失败；如果执行者想通过修改 Candidate 常量或 fixture 让它通过，git diff 会直接显示测试裁判被改动，违反本计划“迁移不得修改 v1653 oracle”的失败条件。

## 七、基础 resolver 的字段顺序漂移到底是什么

`credential_resolver_non_participation_receipt` 两侧都是 7944 字节和 104 个顶层字段，解析后的 key 集合和值集合完全相同，public `receipt_digest` 也相同。首个原始差异出现在 receipt digest 后：fixture 顺序是 `read_only`、`execution_allowed`、`dry_run_only`、`credential_resolver_decision_only`；当前 formatter 顺序则先输出 `credential_resolver_decision_only`，再输出前三个通用边界字段。它不是字段增删、值变化、escaping 差异或执行能力变化，而是一个布尔字段的历史插入位置差。

测试没有使用“忽略对象字段顺序”的通用 JSON 比较，因为那会让其他 103 个字段的未来重排也悄悄通过。`resolver_decision_field_order` 规则只识别一段包含固定 receipt digest 和上述五个字段的完整 compact 片段，确认 runtime 顺序尚未提前出现在 fixture 后，执行一次精确替换。替换后的整对象必须逐字节等于当前 formatter。规则计数还必须恰好为一，所以第二份对象不能借用它，原对象出现第二处顺序差也无法被吸收。

## 八、manual dry-run 三个冻结元数据字段漂移是什么

`manual_sandbox_dry_run_command_non_participation_receipt` 的 fixture 是 3199 字节、56 字段，当前 runtime 是 3060 字节、53 字段。结构化核对证明 fixture 独有 `runtime_project_version="0.102.0"`、`current_runtime_fixture_release_version="v102"`、`current_runtime_fixture_artifact_path_hint="c/102/"` 三项；runtime 没有独有字段，其余共有字段和值完全一致。这说明它是早期 standalone fixture 保留了三项当时归档元数据，而现有 public formatter 长期没有输出这些字段，不是 v1653 新制造的退化。

本版选择当前 formatter 作为 canonical runtime surface，原因是 SMOKEJSON 和真实 CLI 消费的是当前输出，而 frozen fixture 作为历史证据不得为了视觉一致被回写。`frozen_runtime_metadata_fields` 规则只各删除一次三个“固定字段名+固定值+尾逗号”片段；删除后必须与 runtime raw equality。它不是“fixture 多出来什么都忽略”，因为字段名改动、值改动、出现第四个字段、字段重复或不同位置的额外差异都不会匹配。两侧字节数、字段数和各自摘要也分别冻结，未来谁把三个字段悄悄补回 runtime 或从 fixture 删除，都会先于 canonical 比较失败。

## 九、为什么不在基线版顺手修复这两份漂移

把 fixture 重写成 runtime 会破坏冻结路径与下游 digest 证据；把 runtime 改成 fixture 又会在一个“测试基线版”里改变公开 SMOKEJSON 表面，并可能影响 Node 已冻结的消费字节。更重要的是，两种差异都不妨碍后续 builder 按当前 runtime surface 做结构迁移：本版已经能用单用途规则把“历史 fixture”与“当前运行面”同时锁住。此时最安全的选择不是追求仓库里所有 JSON 看起来完全一致，而是把现实差异变成有名字、有范围、有使用次数、有失败条件的契约。

这也解释了为什么规则位于新 family baseline，而不被提升为通用 parity helper。通用 helper 应表达多个 family 真正相同的机制，例如 fail-closed 对象提取和 exactly-once 替换；具体哪个字段历史顺序不同、哪三个元数据只在 v107 fixture 出现，是 domain history，不应污染其他 family。未来如果产品决定统一这些表面，应另开明确 contract migration，更新下游证据并让 reviewer 审核，不能借 builder consolidation 偷渡。

## 十、十类安全边界如何保护 read-only、no router、no write、no execution

每个 runtime object 都必须含有十个共同 marker：`read_only=true`、`execution_allowed=false`、`credential_value_read_allowed=false`、`managed_audit_store=false`、`storage_write_allowed=false`、`write_commands_executed=false`、`runtime_write_observed=false`、`load_restore_compact_executed=false`、`node_auto_start_allowed=false`、`order_authoritative=false`。这些字段把用户关心的四条核心结论拆成可审查事实，而不是只靠文档口号。

`read_only` 表明 receipt 是观察和回显证据；`managed_audit_store=false`、`storage_write_allowed=false` 与 `order_authoritative=false` 共同说明 mini-kv 没有成为 managed-audit router/storage authority 或订单权威；`write_commands_executed=false`、`runtime_write_observed=false` 证明没有业务写入和隐藏写信号；`execution_allowed=false`、`load_restore_compact_executed=false` 证明 receipt 不能触发恢复或管理命令；credential 与 auto-start 两项防止所谓“只读检查”偷偷读取秘密或启动上游。raw whole-object gate 保护所有字段，显式十项 gate 则让最关键边界在测试代码和失败原因中保持醒目。

## 十一、新测试与已有领域测试如何分工

十三份 receipt 已有各自领域测试，通常检查来源 Node 版本、counts、route path、warning/recommendation、receipt digest、standalone fixture、SMOKEJSON 嵌入、runtime-smoke evidence 和 verification manifest。它们擅长解释业务合同：某个 readiness 为什么仍 blocked，某个 fake transport 为什么只能在内存，某个 endpoint handle 为什么不能解析 raw URL。它们不一定枚举所有字段，也不保证未列出的字段顺序和 compact whitespace。

新 baseline 只负责完整对象表面和共同安全底线，不复制每个领域断言。focused lane 会同时运行新 baseline、builder census、通用 builder test、十三份领域测试以及 SMOKEJSON/runtime-smoke/manifest 消费测试。这样失败可以分层定位：完整字节变了看 baseline；领域含义缺失看对应 receipt test；聚合入口不再携带对象看下游消费 test；owner 没有真实使用 builder 或 floor 被放宽看 census。full CTest 再覆盖 Store、WAL、snapshot、RESP/TCP、OSFS 和其他治理证据，证明新增 oracle 没有扰动项目其余机制。

## 十二、census 为什么仍必须保持 18/9/1

v1653 新增了一个链接完整 core 的测试 executable，却没有让任何 pending production source include `runtime_receipt_json_builder.hpp`。所以 builder-backed 仍为十八，pending 仍为九，唯一 waiver 仍是没有目标 formatter 的 execution-denied retention owner。若把 floor 从十八提前改成二十七，CMake census 会按源文件真实扫描结果失败；若为了通过而把 tests 目录纳入 owner 统计，就是把证据基础设施冒充生产迁移。

保持 18/9/1 是本版诚实性的组成部分。版本价值不取决于 ratchet 数字每次都上升：先冻结十三份对象、发现两处历史漂移并把它们限制为单用途规则，本身就是下一阶段能安全加速的前置工程。只有后续某批 production owner 真正采用 ordered builder，且 v1653 oracle 与领域测试不改，floor 才能按 owner 数量单向收紧。

## 十三、后续三批迁移怎样读取这份基线

第一批 base resolver 包含基础 decision、disabled precheck 和 test-only shell 三份 formatter，涉及两个 owner。迁移时基础 decision 必须保持当前 runtime 字段顺序，并继续只使用它自己的命名顺序规则；另外两份必须 exact。第二批 implementation chain 包含 readiness、implementation plan、disabled fake harness、execution denied 和 input hardening 五份 formatter，五份当前都是 exact，适合按历史链统一迁移但保留各文件本地 boundary 所有权。第三批 sandbox chain 包含 manual dry-run、connection precheck、disabled adapter、fake transport 和 endpoint handle，两个聚合 owner 内共有五份 formatter；manual dry-run沿用三字段规则，另外四份 exact。

每一批可以把完整 nested object、array 和顶层字段改成 `json_object`、`json_array` 与 `OrderedJsonField`，可以删除只服务旧拼接的无大括号 fragment helper，也可以在逐项确认同名、同值、同顺序时复用无参数 family helper。不得创建携带大量布尔开关的万能 schema，不得让一个 helper 根据版本号动态省略字段，不得把不同插入位置的 boundary 强行合并。任一 Candidate 常量、fixture、compatibility rule、领域期望或 public digest 需要改动，说明迁移没有保持表面，应缩小范围或回退，而不是改裁判。

## 十四、跨项目输入输出与成熟度表述

这些 receipt 的文本引用了 Node v241、v243、v245、v252、v255、v258、v260、v262、v264、v281、v283、v288、v290、v329 等历史证据，以及对应 Java echo。它们是本仓已经冻结的 contract provenance，不表示 v1653 启动 Node 或 Java，也不表示本轮发生实时三项目联调。测试输入只来自 mini-kv 本仓 fixture 和 formatter，输出只证明 single-project validation 加 cross-project contract alignment。

下游有四类消费面：standalone fixture 供版本归档，SMOKEJSON 供真实 CLI 只读读取，runtime-smoke-evidence fixture 供离线稳定校验，verification manifest 固定路径和摘要关系。v1653 直接保护 standalone nested object，已有领域和聚合测试保护其余三面。后续 Node integration capstone 若执行真实 mini-kv CLI，会读到当前 canonical runtime surface；本版让该 surface 在 consolidation 期间保持稳定，同时不声称 mini-kv 自己已经跨过真实联合测试门槛。

## 十五、对抗性自审与本版价值

最强质疑之一是：两条 compatibility rule 是否只是给不一致开绿灯。回答取决于规则是否可泛化。本版规则都不可泛化：resolver 规则绑定固定 receipt digest、固定五字段片段和唯一顺序替换；manual dry-run 规则绑定三个固定字段名、固定值和 exactly-once 删除。两条规则各只能使用一次，另外十一份对象强制 exact，所有对象还分别冻结两侧大小、字段数和摘要。出现任何第二种差异都不会被当前代码吸收，因此它们是对历史现实的精确建模，而不是宽松比较。

第二个质疑是：一次冻结十三份会不会形成新的大测试。该文件的数据表较长，但机制只有 fixture 读取、单用途 canonical 映射、统一断言三部分；它复用既有 extractor 和 parity support，没有复制 JSON 扫描器。family 字段和十三行 Candidate 让扩展面一眼可见；这份测试在三批迁移完成后仍作为完整回归门，不需要继续无限追加，因为 census 目标是 pending 归零而不是再新增 receipt 链。

第三个质疑是：基线是否只看格式，没有证明安全。完整 raw equality 已覆盖全部输出，十类显式 marker 又直接守住只读、无存储/路由权威、无写入、无 credential、无恢复执行、无自动启动和无订单权威；focused 与 full CTest 再证明命令聚合和存储核心未被触碰。v1653 的价值因此不是“新增一个测试文件”，而是把最后九个 owner 的迁移条件从口头约定升级成十三份带独立指纹、两条有限历史规则和共同关闭边界的机械合同。

一句话总结：v1653 在不改变任何 runtime receipt 的前提下，先把剩余十三个 formatter 的真实输入、原始输出、两处历史漂移和 read-only/no-router/no-write/no-execution 底线全部冻结，为接下来三批 ordered-builder 迁移建立了不可随实现一起移动的裁判。
