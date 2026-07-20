# v1669：让三份审批回执共享事实，但不共享身份

## 一、本版目标、角色，以及它明确不是什么

v1669 是“三分优雅提升计划”的第二个结构性版本。v1668 已经证明，runtime receipt 中稳定的布尔边界可以从反复执行的 `fields.push_back(...)` 变成有顺序的数据 profile，同时保持 JSON 字节、字段顺序和摘要不变。本版继续沿这条路线，但把目标收得更准：只处理 signed-human approval artifact、credential-handle approval 与 endpoint-handle allowlist approval 三份连续合同回执，再顺手清除五个没有领域语义的一行式 digest 转发器。

这三份回执的角色都是“只读地证明某个审批前置合同已经被 mini-kv 消费或冻结，但绝不把审批结果变成执行”。它们会告诉 Node 控制面：当前读到了哪些上游证据、哪些条件仍未满足、哪些能力必须保持关闭。它们不是新的执行入口，不是 credential resolver 实现，不读取 credential value，不解析或连接 raw endpoint，不发起 DNS、HTTP 或 TCP，不建立 managed-audit connection，不写 Store、WAL、审批账本或生产记录，不执行 schema migration、rollback、restore、load、compact 或 setnxex，也不自动启动 Node、Java 或 mini-kv。它们更不取得订单、审计或审批 authority。

因此，本版的成功标准不能只是“代码少了两百行”。真正标准是：重复事实只声明一次，三个领域 owner 仍能分别阅读；完整回执的字段名、值、顺序、字节数、兼容差异和 digest 不发生漂移；任何未来回流或账本松弛都会被机械门拒绝。

## 二、起点：为什么这三个 owner 值得放在同一版

迁移前，`receipt_builder_census_contract` 给出的实现层账本是 814 次手工字段追加和 6 个本地 `receipt_digest` 包装器。逐份比较三个 approval owner 后，可以看到它们不是偶然出现几个同名字段，而是具有连续、顺序稳定的共同区段：最前面的只读与禁执行；随后是 runtime shell、resolver client 和 production readiness 全部关闭；再往后是 credential value 不可接受、不可读取、不可保存、不可渲染；之后是 raw endpoint 不可接受、不可解析、不可渲染；最后是外部请求、存储写入、审批账本、schema、恢复、自动启动和 authority 全部关闭。

这些共同区段合计在三个文件中重复 201 次。它们的名字、值和相对顺序都一致，且已有迁移前冻结的完整输出 oracle。满足“三次规则”、数据稳定、裁判独立三个条件，正适合抽取。

但三个 formatter 仍有不可混同的身份。signed-human owner 关心签名审批 artifact 的保存、验证与接受状态；credential-handle owner 关心 handle 是否保存、验证、解析和权威；endpoint owner 还要表达 allowlist authority，并在 raw endpoint 的接受与处理之间插入 `raw_endpoint_url_stored=false`。它们引用的 Node 版本、source contract、receipt version、warnings、recommendations 与 digest parts 也不相同。抽象应该共享稳定事实，不应该抹平这些区别。

## 三、入口与模块职责

新的私有入口是 `src/runtime_approval_receipt_profiles.hpp`。文件名长度为 37，符合新增文件 40 字符预算；内容只有 79 个非空行，没有新建 `.cpp`、全局注册器或第二套 renderer。它位于 receipt 实现层内部，没有进入 `include/minikv`，所以不会扩大 public API。

该文件定义六段 `inline constexpr BooleanField` 数组：`read_only_profile` 表达只读和禁执行；`readiness_profile` 表达 runtime shell、resolver 和 production readiness 未开启；`credential_value_profile` 表达凭据值不可进入系统；`raw_endpoint_acceptance` 单独表达 endpoint 不可被接受；`raw_endpoint_processing` 表达不可解析与渲染；`external_effects_profile` 表达网络、存储、审批账本、schema、恢复、自动启动和 authority 全部关闭。

行为入口仍是 v1668 已建立的 `runtime_receipt_json::append_boolean_fields`。它接受一个有序字段容器和一段 `BooleanField`，按数组顺序编码并追加。v1669 没有复制这个循环，也没有给 profile 增加回调、条件表达式或 receipt 类型判断。数据在新 header，唯一行为在原 builder，三个 formatter 只负责决定何时插入哪一段。

三个 owner 文件继续分别持有自己的 metadata、checks、warnings、recommendations、兼容规则和最终 JSON object。也就是说，profile 不知道 Node v315、v318 或 v321，不知道 fixture 路径，不计算 digest，更不能决定一份回执是否“ready”。它只保存跨 owner 已经相同的布尔事实。

## 四、为什么不能把三份 formatter 合成一个万能模板

最诱人的错误方案，是写一个 `format_approval_receipt(kind, node_version, flags...)`，再用大量参数和分支生成三份回执。表面上它可能继续减少代码，实际上会把三个清楚的合同变成一个条件矩阵。调用点会出现难以审阅的字符串、数字与布尔参数；模板内部需要知道哪些字段只属于 signed artifact、哪些属于 credential handle、endpoint 的 stored 字段应插在哪里、不同 compatibility rule 怎样选择。每新增一种 approval receipt，都可能继续给万能函数加一个模式。

当前方案刻意不追求这种“最大去重”。三个 formatter 的骨架仍可单独从上往下阅读：先是共同只读前缀，再是本地合同元数据，然后插入 readiness；接着出现 owner 自己的审批对象字段，再插入 credential 与 endpoint 关闭段；最后进入共同外部副作用尾部。维护者无需跳进一个类型分支森林才能知道当前回执拥有什么。

这也解释了为何 `raw_endpoint_acceptance` 只有一个字段，却仍单独成段。endpoint owner 必须在它之后插入 `raw_endpoint_url_stored=false`，然后才追加 processing 四字段。如果把 acceptance 与 processing 合成一张五字段表，本地 stored 字段只能被错误地移到表前或表后；若为此给 builder 加一个“中途回调”，复杂度会远大于收益。小而有语义的分段让历史顺序显式可见。

## 五、三条实际生成路径

signed-human 路径先调用 `read_only_profile`，随后保留 signed approval contract、上游 artifact 与 Node v315 相关字段，再显式追加 `ready_for_node_v315_before_upstream_echo=false`。之后消费 `readiness_profile`，保留五个 signed-artifact 专属状态，最后按顺序消费 credential、raw-endpoint 与 external-effects 三类关闭事实。最终 formatter 的角色仍是 signed artifact 证据，不会被另两个 owner 的 handle 字段污染。

credential-handle 路径采用同样的共同区段，但中间显式保留 `credential_handle_stored`、`validated`、`resolved`、`authoritative` 与 approval status authority 五项。这五项不是 profile 数据，因为 endpoint owner 使用另一组 handle/allowlist 字段，signed owner也没有它们。让它们留在调用点，阅读者一眼就能看到本合同究竟审批什么。

endpoint-allowlist 路径则保留六个 endpoint 与 allowlist 专属字段。它先追加 credential-value 关闭段，再追加单字段 acceptance 段，随后显式写入 `raw_endpoint_url_stored=false`，最后追加 processing 和 external-effects。这个顺序与迁移前完全一致，完整输出 oracle 会逐字节裁决。profile 的职责是复用两侧稳定事实，而不是夺走 stored 字段的位置所有权。

## 六、digest 为什么从六个收口到一个，而不是归零

迁移前仓库中存在六个名为 `receipt_digest` 的私有或 inline 函数。调用关系检查显示，其中 `runtime_credential_resolver_receipt_utils.hpp` 的 helper 被二十多个 credential-resolver formatter 使用。它不是孤立的一层转发，而是这一 family 的稳定入口；保留它能让大量 owner 继续使用同一命名和依赖边界，避免一次无价值的大范围触碰。

另外五个定义分别位于 approval boundary、implementation plan、implementation readiness、runtime evidence 和 sandbox owner。它们只做一件事：原样调用 `runtime_evidence::digest(prefix, parts)`，且只在本文件少量位置使用。没有参数转换、固定前缀、领域校验或错误处理。这样的包装不会隐藏复杂性，只增加跳转。v1669 删除这五个局部定义，让调用点直接使用既有底层函数。

为什么不继续把 canonical helper 也删掉？因为优雅不是把统计数字压到零。若为了 `wrappers=0` 修改二十多个稳定消费者，diff 会扩大，审阅成本上升，且不会增加任何领域表达。当前精确值 1 表示“只保留一份有真实共享面的入口”。未来如果 credential family 本身重构，可以在新的独立 oracle 下重新判断；本版不提前支付那项风险。

## 七、关键字段让控制面怎样理解

`read_only=true` 表示当前回执生成只读取冻结输入或计算内存中的证据，不代表整个 mini-kv 引擎没有写命令。`execution_allowed=false` 表示回执不能把审批、计划或 endpoint 转换成运行行为。`credential_value_read_allowed=false` 与 `credential_value_read=false` 同时说明既没有权限，也没有观察到读取；handle 与 value 被严格区分。

`raw_endpoint_url_accepted=false`、`parsed=false`、`rendered=false` 说明字符串不会进入网络配置，更不会变成 socket 连接。`external_request_sent=false` 与 `http_tcp_dial_allowed=false` 继续关闭网络面。`storage_write_allowed=false`、`write_commands_executed=false`、`runtime_write_observed=false` 关闭 Store 和运行写入；`approval_ledger_written=false` 与 `managed_audit_write_executed=false` 关闭审计副作用。

`restore_execution_allowed=false` 与 `load_restore_compact_executed=false` 说明即使控制面读到恢复相关证据，也不能执行管理命令。`node_auto_start_allowed=false`、`java_auto_start_allowed=false`、`mini_kv_auto_start_allowed=false` 防止证据消费演变成跨项目进程编排。`audit_authoritative=false` 和 `order_authoritative=false` 则明确 mini-kv 只能提供存储与恢复证据，不能替代 Java 或 Node 的业务权威。

`warnings`、`blockers` 与 `diagnostics` 没有进入 profile。它们包含各 owner 对缺失审批、未满足前置和建议动作的解释，属于动态或领域文本。把它们强行数据化会制造版本分支，也可能改变摘要。v1669 只处理稳定布尔边界，文本证据继续由原 formatter 持有。

## 八、机械门如何证明改进没有被“口头宣布”

实现前基线是 814 次手工追加与 6 个本地 digest wrapper。三份 approval owner 各移除 67 次重复追加，总数正好下降 201，最终为 613；五个局部包装删除后剩 1。`CMakeLists.txt` 把两个期望值同步收紧，`check_receipt_builder_census.cmake` 同时拒绝高于和低于 baseline 的实际值。

本版故意运行四个错误配置。把 manual push 期望设成 612，脚本报告实际 613 超过上限；设成 614，脚本要求把 stale baseline 收紧到 613。把 wrapper 期望设成 0，脚本报告实际 1 回归；设成 2，脚本要求收紧到 1。只有 613/1 能通过。这证明门不是只会显示绿色，也证明未来减少计数时必须在同一提交更新账本。

新增 header 只有 79 个非空行；所有被修改的 receipt 实现仍低于 800 非空行。新增文件名和新增概念名遵守 40 字符预算。三个历史长文件名属于已登记存量合同路径，本版没有为追求表面命名分数去重命名它们，也没有新增超长文件。

## 九、测试分层分别保护什么

第一层是三个具名合同测试：`signed_human_approval_artifact_contract_receipt_tests`、`credential_handle_approval_contract_receipt_tests` 与 `endpoint_handle_allowlist_approval_contract_receipt_tests`。它们保护每份回执的完整字段集合、历史顺序、摘要和关键边界。若 profile 放错位置、漏一个字段或改变一个值，测试会直接失败。

第二层是 `runtime_receipt_remaining_owner_baseline_tests` 与 `smokejson_command_receipt_tests`。前者从独立 baseline 观察仍未被共享模型吞掉的 owner 关系，后者通过真实命令聚合路径确认 receipt 改造没有破坏 SMOKEJSON。它们不是为迁移修改的新期望，而是迁移前就存在的裁判。

第三层是 57 项 receipt/release/runtime 组合门。它包含 builder、runtime-shell family、legacy resolver、所有后续 approval receipt、release verification manifest、managed-audit 聚合与 runtime smoke。五个 digest owner 的调用改写也在这里被实际执行，所以“编译通过但某条摘要路径没跑到”的风险被进一步压低。

第四层是最终全量 353 项 CTest 与真实 CLI。全量门覆盖 Store、WAL、snapshot、RESP、TCP、OSFS 和历史治理合同；CLI 直接运行最终 `minikv_cli.exe`，读取 `SMOKEJSON` 与 `CHECKJSON LOAD data/prod.snap`，确认只读、禁执行、禁恢复、无 WAL touch 和非订单权威仍成立。任何 source 变更后都先重建，再运行测试，不使用旧二进制。

## 十、输入、输出与维护方式

v1669 没有新增命令。用户输入仍可使用 `SMOKEJSON`、`CHECKJSON LOAD data/prod.snap` 和 `QUIT`；输出结构也不应改变。变化只发生在 formatter 内部：原来每个 owner 用几十次 push 重复表达关闭事实，现在调用六段有序 profile，builder 仍输出相同的 `OrderedJsonField` 序列。

后续维护时，如果某个 approval owner 新增真正专属字段，应继续在原 owner 的语义位置显式追加，而不是塞进共享 profile。如果三份 owner 都新增同名、同值、同顺序的稳定事实，才应先更新 family 设计说明、扩展相应 profile，并用现有 oracle 验证。如果某个边界需要打开，不能只把 profile 中的 false 改为 true；那代表合同与 authority 变化，必须另立功能版本、重新评审跨项目依赖和执行风险。

本版给 Node、Java 与 mini-kv 松耦合融合带来的价值可以用一句话概括：下游继续读取完全相同的只读审批证据，而 mini-kv 内部把重复安全事实压缩成可审计的数据段，并保留每份合同真正不同的领域身份。

## 十一、最终验证记录

本讲解主体在最终 verify 之前完成。随后单实例全目标增量构建退出码为 0，三个 approval 精确测试与 remaining-owner、SMOKEJSON 共 5/5 通过，receipt/release/runtime 组合门 57/57 通过。四条 census 故障注入均以非零退出证明 613/1 的上下界语义。最终全量 CTest 为 353/353、0 失败、240.39 秒；真实 CLI 递归检查三个执行信号各 37 次且全部为 false，CHECKJSON 不触碰 WAL，QUIT 返回 BYE。四张选择性截图和偏差记录位于 `f/1669/`；远端 CI 在 commit/tag/push 后作为最后一层证据。
