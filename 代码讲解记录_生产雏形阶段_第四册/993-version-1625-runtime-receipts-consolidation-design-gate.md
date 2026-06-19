# v1625 Runtime receipts 合并设计闸门讲解

## 本版目标、角色和“不是什么”

v1625 的目标不是再给 mini-kv 增加一份 runtime evidence，也不是继续延长 Node、Java、mini-kv 之间的 echo/verification 链，而是停下来处理已经累积到清晰可见的 C++ 维护债：`src/runtime_*_receipts.cpp` 家族已经有 28 个文件、788,811 字节，其中 credential resolver 相关部分占 26 个文件、706,166 字节。它们承载的边界非常重要，持续证明 `read_only=true`、`execution_allowed=false`、不读取 credential value、不解析 raw endpoint、不连接 managed audit、不写 Store/WAL、不执行 `LOAD`/`COMPACT`/`RESTORE`/`SETNXEX`、不自动启动 sibling 服务；但实现方式主要还是数百行字符串拼接。边界语义越重要，越不能长期依靠复制粘贴维护。

本版的角色是“重构前设计闸门”。它先回答四个问题：重复到底有多大；哪些部分真的相同；哪些部分必须留在领域文件里；后续如何证明输出一个字节都没变。只有这四个问题写清楚，v1626 之后才有资格开始动 formatter。

这版不修改任何 receipt formatter，不修改 fixture，不修改 digest，不修改 `SMOKEJSON`、`INFOJSON` 或 verification manifest，不新增 command、router 或 runtime shell，也不启动 Node/Java/mini-kv 服务。它不会把只读证据升级成执行入口，不会借“重构”之名改变字段顺序，更不会为了让新 builder 好看而重写历史 fixture。

## 为什么现在要做：从感觉转成库存证据

production-excellence K4 原计划已经把 30 多个 `runtime_*_receipts.cpp` 识别为可选 stretch scope，并明确要求：先写 necessity proof 和 shared-formatter design，再尝试合并。v1617-v1619 先完成 command/shard readiness 拆分，v1620-v1622 完成 archive、文档和 release discipline，v1623-v1624 又把项目导览做成可维护、可执行入口。到 v1625，安全网已经够强，剩下最大的代码结构问题正是 receipts family。

这次没有只数文件名，而是进一步统计了结构。25 个专题 credential resolver 文件中，18 个已经 include `runtime_credential_resolver_receipt_utils.hpp`，说明作者已经多次遇到同一类重复；7 个仍各自声明 `field_string`、`receipt_digest` 或相关包装。专题文件里合计约 180 个 `format_*` helper，17 个文件有 `format_checks_json`，20 个文件有关闭边界块。换句话说，重复不仅是每个文件都叫 receipts，而是每个文件都在重复做相同的六步：声明元数据，拼来源引用，拼检查和 summary，构建 digest parts，拼顶层 JSON，最后再铺一大组 false 边界。

这种代码短期很好写。复制上一版，把 Node 版本号、fixture path、字段列表和几个 warning 换掉，一版就能工作。但长期代价很明确。第一，共同边界加一个字段时，维护者必须判断 26 个文件哪些要加、加在哪里、顺序是否一致。第二，digest 输入和 JSON 输出是两套手写顺序，可能出现输出看起来正确但 digest 没跟上的错误。第三，字段片段测试很多，容易让人产生“已经很安全”的错觉，却没有完整证明 formatter 返回值与 fixture 内嵌对象逐字节相同。第四，新文件继续照旧复制会让维护成本按版本线性上升。

所以本版的价值不在“又多了一篇文档”，而在于把以后每一刀重构的合法范围和失败信号固定下来。没有这个闸门，直接抽 builder 很容易抽成一个上百 optional 字段的万能对象，表面减少行数，实际把复杂度藏进配置。

## 入口：docs 设计说明和 CTest 合同

本版的主入口是 `docs/receipts-consolidation-note.md`。它不是泛泛的重构愿景，而是包含可核对数字、目标模块形状、迁移批次、测试 oracle 和停止条件的施工说明。维护者从 `START_HERE.md` 和 README 能直接找到它。

第二个入口是 `cmake/check_receipts_consolidation_note.cmake`，注册为 CTest `receipts_consolidation_note_contract`。这个脚本做两类保护。

第一类是文档语义保护。脚本读取 Markdown，要求它继续包含 `Necessary proof`、`Shared formatter design`、`Byte-identical output contract`、fixture parity harness、Slice 0/1、full CTest 和 no credential/no network/no write 等关键片段。这样后续更新设计时，不能把最关键的安全条件删成一句“抽公共方法即可”。

第二类是库存保护。脚本在配置后的测试阶段 glob `src/runtime_*_receipts.cpp`，要求当前基线仍是 28 个；再 glob `runtime_credential_resolver_*_receipts.cpp`，要求专题文件仍是 25 个，并确认基础 `runtime_credential_resolver_receipts.cpp` 仍存在。未来真正开始合并或拆文件时，这个测试会主动失败，提醒执行者先同步设计说明和 census，而不是让文档悄悄过期。它不是禁止重构，而是要求重构显式更新基线。

## 共享 formatter 为什么必须是“有序拼装器”

最危险的设计误区，是把当前代码理解成“我们缺一个 JSON 库”。mini-kv 当前 JSON 输出不是普通内部对象，它被 fixture、manifest、SMOKEJSON、下游路径和 digest 共同引用。即使两个 JSON 在语义上等价，只要字段顺序、转义或空白变化，历史字节就不同。用 `std::map` 会按 key 排序，用 unordered container 会产生不稳定顺序，用结构化 JSON 库重新序列化也可能改变 escape 或数字格式。

因此设计说明提出的不是通用 JSON DOM，而是一个很小的 ordered builder。核心输入是按顺序排列的 `OrderedJsonField`：字段名加已经编码的 JSON value。对象 formatter 只做三件事：按输入顺序写 key，写冒号和值，在字段间写逗号。它不排序，不推导，不省略，也不美化输出。

为什么 `value_json` 要明确是“已经编码好的值”？因为 receipt 里既有普通字符串，也有嵌套对象和数组。如果 builder 把所有 value 都当字符串，会把 `{...}` 再转义成字符串；如果把所有值都当 raw JSON，又可能让普通文本漏转义。设计要求调用侧显式选择：普通文本先走 `runtime_evidence::json_string`，布尔和数字走专门 helper，嵌套对象/数组由 builder 生成。类型意图在调用点可见，错误更容易被测试发现。

这个 builder 也不能知道 Node v326、abort/rollback、signed approval 或 runtime shell 的业务含义。领域文件仍要保留 receipt/version/source 常量、required/prohibited/check 等领域 table、digest prefix 和 digest part 顺序。共享层只抽机械拼装，不抽业务决策。这样才能避免形成一个参数长到不可维护的 `buildCredentialResolverReceipt(spec, 40 booleans...)`。

## 为什么 fixture 是 oracle，但现有测试还不够

当前 credential resolver 测试质量并不差。以 abort/rollback 和 no-network receipt 测试为例，它们会读取 standalone fixture、runtime smoke fixture 和 verification manifest，核对 source version、profile、digest、required/prohibited fields、summary、warning、recommendation 和大量关闭边界；还会创建真实 `Store` 与 `CommandProcessor` 执行 `SMOKEJSON`，最后用 `GET restore:real-read-token` 和 `CHECKJSON SMOKEJSON` 证明没有写入或执行升级。

这些测试保护的是语义完整性和运行时暴露路径，非常有价值。但它们主要使用 `assert_contains`。如果 formatter 在不影响所有被检查片段的情况下调整了字段顺序、少了一个未被点名的 false 字段，或者改变某个转义，测试可能仍然通过。对于普通内部 JSON 这也许可以接受；对于已经冻结的 release fixture 和 digest evidence，不够。

因此设计说明把 `fixtures/release/*.json` 定义为真正 oracle，并要求 v1626 的 Slice 0 先补 fixture parity harness。测试 helper 需要从 standalone fixture 中找到目标字段后的嵌套对象，用状态机处理字符串、反斜线和花括号层级，提取完整 object 原始字节，再和 public formatter 返回值做 `==`。不能把 fixture parse 成无序对象再序列化，因为那会丢掉要保护的字段顺序。

这个顺序很重要：先让测试能够精确失败，再迁移代码。否则第一次 builder 迁移如果改变了一个逗号位置，现有 fragment tests 可能看不出来，等 Node 或历史 digest 消费时才发现就太晚了。

## 后续切片为什么从两个最新合同开始

设计把第一批真实迁移候选定为：

```text
runtime_credential_resolver_no_network_safety_fixture_receipts.cpp
runtime_credential_resolver_abort_rollback_semantics_receipts.cpp
```

这两个文件都已经使用共同 utils，结构也高度相似：都有 required fields、prohibited fields、rejection reasons、no-go boundaries、upstream echo requests、prerequisite transition、necessity proof、summary、checks 和一大组关闭字段。它们是验证 builder 的好样本，因为重复足够明显，同时各自又有 no-network 与 abort/rollback 的领域差异。

第一批不选老文件，是为了避免同时解决两件事：既统一旧 helper，又引入新 builder。一次只改变一个变量，失败更容易定位。如果最新合同 pair 能在 fixture byte parity、focused receipt tests、SMOKEJSON、runtime smoke fixture、manifest 和 full CTest 下保持完全一致，再进入 approval artifact 组。反过来，如果这两个最同构文件都无法干净迁移，就说明 builder 设计不对，应停止，而不是继续扩大适配面。

approval artifact 组之后才是 runtime shell blocked decision 组，最后处理 7 个没有复用现有 utils 的 legacy 专题文件和基础 receipt 文件。这个顺序体现的是风险递增：先做同构、现代、测试强的文件，再碰结构差异和历史实现。

## 边界字段如何被控制面理解

本版没有生成新的 runtime response，但设计仍明确要求后续 builder 不得“智能补全”边界字段。原因是这些字段不是装饰，而是控制面判断。

`read_only=true` 表示 receipt 可读，不表示底层命令永远不会写。`execution_allowed=false` 表示这份 evidence 本身不能授权执行。`order_authoritative=false` 和 `audit_authoritative=false` 表示 mini-kv 不接管 Java 订单权威或 managed audit 权威。`storage_write_allowed=false`、`write_commands_executed=false`、`load_restore_compact_executed=false` 则分别说明授权、观察到的执行和管理命令执行都关闭。`credential_value_read=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`automatic_upstream_start=false` 把 secret、endpoint、network 和 sibling lifecycle 边界关死。

共享 formatter 可以减少这些字段的机械重复，但不能根据“这是 credential resolver receipt”自动推导所有值。某些历史 receipt 字段集合不同，名字也不同；自动补字段会改变字节和历史语义。设计因此要求共同 boundary block 只能在原文件已经拥有完全相同字节序列时复用，否则保留局部 section。

## 失败模式推演：为什么这些闸门不能省

第一类失败是字段重排。假设迁移者把一个 receipt 的字段先放进 `std::map`，得到的 JSON 在浏览器和普通 parser 看来完全合法，所有 `assert_contains` 也可能继续通过，但 `current_release_version`、`runtime_role`、`source_node_reference` 的相对位置已经变化。对只按语义解析 JSON 的消费者，这也许无影响；对冻结 fixture、整串摘要和人工 diff，它就是一次契约变化。ordered builder 的意义，是把“保持插入顺序”从口头要求变成数据结构选择。

第二类失败是二次转义。领域 helper 已经返回 `{"check_count":21}`，如果通用 builder 把它当普通文本，就会输出 `"{\"check_count\":21}"`。外层仍是合法 JSON，甚至某些字符串片段仍存在，但对象变成了字符串，下游读取 `check_count` 会失败。因此 `value_json` 和普通 string 必须在类型和调用方式上明确区分，builder 测试也必须覆盖嵌套对象不被二次转义。

第三类失败是 digest 漂移。formatter 迁移可能保持可见 JSON 不变，却顺手把 digest parts 改成从新 spec 自动遍历。只要遍历顺序与旧手写顺序不同，`receipt_digest` 就会改变。历史 fixture 和 Node 消费记录中已经冻结这些 digest，所以设计要求 digest parts 暂时继续留在领域文件里；只有单独写 necessity proof 和历史兼容方案，才允许进一步抽象 digest。

第四类失败是万能 spec。为了让 26 个文件都调用一个函数，开发者可能不断给 `ReceiptSpec` 增加 optional 字段：有的 receipt 用 prerequisite transition，有的用 archive summary，有的用 fake harness denial，有的用 approval artifact。最终 spec 会拥有几十个 `std::optional` 和布尔开关，调用点只能靠注释解释哪些组合合法。这种抽象减少了文件行数，却增加了非法状态数量。v1625 因此限定每批只抽同构结构，例外继续保留局部 formatter；一个 builder 不能表达某个 section，并不等于 builder 失败。

第五类失败是为了过测试修改 fixture。迁移后若 byte parity 失败，最省事的做法是重新生成 fixture，再更新 digest 和 expectation。这样测试会重新变绿，但历史证据已经被重写，Node 的路径和摘要引用也可能失效。项目硬规则明确把 fixture 当 oracle：迁移错了就修代码，不允许把 oracle 改成新答案。只有一个真正新增契约的独立功能版本，经过跨项目计划确认后，才可以产生新 fixture；重构版本没有这个权限。

第六类失败是把验证做成运行时动作。为了证明 receipt 还能输出，不需要启动 TCP server、连接 Node、读取 Java，也不需要执行 `LOAD` 或创建 managed audit client。focused formatter test、fixture parity、`CommandProcessor` 的 `SMOKEJSON` 和 full CTest 已经能覆盖这次内部重构。若某批迁移声称必须读取 credential、打开网络或启动 sibling 服务才能验证，说明范围已经越过内部质量优化，必须停下来重新规划。

这些失败模式也是 Slice 0 必须单独成版的原因。先把 ordered builder 和 byte extractor 的错误空间压小，后续每批 receipt 迁移才是在一张稳定安全网上工作，而不是一边换结构、一边猜输出、一边重写基准。

## 测试类别分别保护什么

`receipts_consolidation_note_contract` 保护设计不能退化，也保护文件库存变化必须被记录。它是设计层测试。

未来 builder 单元测试保护有序字段、escape、raw nested JSON、空数组、false/0 和 malformed input fail closed。它是机械拼装层测试。

fixture parity harness 保护完整 formatter 输出的原始字节。它是迁移正确性的核心 oracle。

现有单 receipt focused tests 保护来源版本、领域字段、digest、warnings、recommendations 和 no-side-effect 边界。它们是业务语义层测试。

`smokejson_command_receipt_tests` 保护 receipt 仍从真实 runtime command 暴露；`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 保护 committed fixture/manifest 消费路径；full CTest 则检查 builder 没有影响 command、WAL、snapshot、RESP、TCP 和其他 evidence family。

远端 sanitizer、coverage、format gate 继续负责内存/未定义行为、核心覆盖率和风格回归。v1625 本身不改 C++ runtime，但仍跑完整 CTest，因为新增了 CMake contract test，也要证明配置和全套注册没有问题。

## 本版价值与后续融合意义

v1625 让 mini-kv 从“知道 receipts 很重复”前进到“知道哪些重复可以抽、哪些领域必须保留、如何逐字节证明、什么时候必须停”。这比立刻少几百行更重要，因为 receipts 是跨项目松耦合证据的一部分，错误抽象会比重复代码更危险。

对 Node 来说，后续重构不得改变它可能读取的 fixture path、field order、digest 和 no-side-effect 字段。对 Java 来说，mini-kv 仍只提供非权威、非执行的存储/证据边界，不会因为 builder 出现而接管订单或审计决策。对 mini-kv 自己来说，这条路线最终能减少新增一份 receipt 就复制数百行字符串的成本，同时保留零依赖和 C++ 可读性。

一句话总结：v1625 没有搬一行 receipt 实现，却把下一轮真正重构需要的量化基线、有序 builder、fixture 字节 oracle、分批顺序和停止条件全部钉牢，为 Node/Java/mini-kv 继续松耦合消费同一份只读证据提供更可维护、但不改变任何契约的 C++ 基础。
