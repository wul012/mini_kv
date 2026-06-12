# v1611 production-excellence K1 required sanitizer gate

## 目标、角色和不是什么

v1611 的目标很集中：把 v1610 已经跑绿的 Ubuntu sanitizer 诊断轨道升级成 required gate。v1610 先接入 `MINIKV_SANITIZE`、新增 Ubuntu ASan/UBSan CI job，并用 `continue-on-error: true` 保护第一轮观察；远端 GitHub Actions 已经证明这条诊断轨道可以完成配置、构建和全量 CTest。v1611 因此不再继续停留在“看一看”的阶段，而是删除 `continue-on-error: true`，让 sanitizer job 失败时真正拉红 CI。

本版的角色是 K1 的收口版。它不是新的运行时 feature，不新增命令，不新增 JSON evidence，不改变 WAL、snapshot、RESP、TCP 或 store 行为。它也不是 coverage 版本，K2 还没有开始；更不是 K4 文件拆分，`command.cpp` 和 `shard_readiness.cpp` 在本版不会被动。v1611 只处理一件事：已经验证过的 sanitizer 轨道，从诊断模式变成强制质量门。

这个“不是什么”需要写清楚，因为很多工程质量版本容易顺手扩大范围。sanitizer required gate 的价值在于边界清晰：如果之后某个 C++ 改动引入越界访问、未定义行为或 sanitizer 可见的生命周期错误，它应该让 CI 失败；但这个门禁不意味着 mini-kv 可以读 Node/Java 文件，不意味着可以启动服务，不意味着可以执行 restore/load/compact，不意味着打开 router、写 store 或触 WAL。它只是对测试时被触发的 C++ 路径加一层运行时安全检查。

## 为什么 v1610 绿后才能做 v1611

K1 playbook 的顺序是“Start continue-on-error; fix every finding; then make it required”。这里的关键不是形式上先后分两个版本，而是避免把未知风险直接变成阻断项。v1610 刚加入 sanitizer 时，真实风险有三类：第一，Ubuntu 工具链是否能接受当前 CMake flag；第二，完整 CTest 是否会在 sanitizer 下暴露已有未定义行为；第三，sanitizer 构建时间是否会在 CI 预算内。v1610 的远端 CI 已经回答了这三个问题：Ubuntu 配置成功、sanitizer target set 构建成功、sanitizer CTest 成功。

如果 v1610 的 sanitizer job 失败，v1611 就不应该移除 `continue-on-error`，而应该先修 sanitizer findings。现在它绿了，继续保留 `continue-on-error` 反而会削弱 K1 的意义：CI 看起来有 sanitizer job，但这个 job 即使失败也不会阻断合并，久而久之就会变成装饰性检查。v1611 把这个口子关上，才算把 K1 从“有一条可运行命令”推进到“有一条机械执行的质量门”。

这也是为什么本版不再重复设计 `MINIKV_SANITIZE` 的实现。开关、Linux CI job、Windows MinGW 本地限制说明都已经在 v1610 完成；v1611 只做状态提升。好的收口版本不需要重新发明前一版的东西，它要把已经验证的信号纳入日常约束。

## CI 改动：删除 continue-on-error

本版最核心的代码改动在 `.github/workflows/ci.yml`：

```yaml
sanitizer:
  name: ubuntu sanitizer
  runs-on: ubuntu-latest
```

v1610 这里还有一行：

```yaml
continue-on-error: true
```

v1611 删除它。删除后，GitHub Actions 的默认语义就是 required：如果 `ubuntu sanitizer` job 的配置、构建或测试步骤失败，整个 workflow run 会失败。因为这个 workflow 已经在 push 上运行，K1 required gate 就具备了机械执行能力。它不是 README 里的愿望，也不是人类审查时才看的 checklist，而是每次 push 都会真实执行。

这条 job 仍然只在 `ubuntu-latest` 上运行，仍然使用 Debug，仍然传 `-DMINIKV_SANITIZE=ON`，仍然完整执行 CTest。v1611 没有扩大到 macOS sanitizer、Windows sanitizer 或 TSan，也没有把 sanitizer 混进三平台普通 matrix。这个选择保持了 K1 的主题纯度：先让 Linux ASan/UBSan required green，再由后续版本决定是否扩展 TSan 或其他平台。

## 测试改动：从要求诊断变成禁止弱化

`tests/dependabot_config_tests.cpp` 在 v1610 里新增了对 sanitizer job 的字符串断言，当时包括：

```cpp
assert_contains(ci_workflow, "continue-on-error: true");
```

这条断言在 v1610 是合理的，因为它保护“诊断轨道”的阶段状态。但 v1611 要把 sanitizer 变成 required gate，所以测试语义必须同步翻转：

```cpp
assert_not_contains(ci_workflow, "continue-on-error: true");
```

这个变化很小，却很关键。它意味着后续任何人想把 CI 静默放松，都不能只改 YAML；本地 CTest 会先失败。换句话说，required gate 不只是当前 YAML 状态，而是被测试守住的契约。mini-kv 过去大量治理证据都用测试固定 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal` 等边界字段；v1611 用同样思想固定 CI 门禁语义。

当然，这个 `assert_not_contains` 是全 workflow 级别的保护。它会阻止未来在同一 CI 文件里随意新增 `continue-on-error: true`。这有一点严格，但符合 production-excellence 阶段的目标：如果某个未来 job 确实需要诊断模式，应该在对应版本里明确解释原因，并调整测试为更精确的 job-scoped 断言，而不是让弱化字段无声出现。当前项目没有其他 continue-on-error 的正当用途，所以先选择更强的保护。

还有一个细节容易被忽略：这次测试没有只检查 `name: ubuntu sanitizer` 和三条命令继续存在，而是专门检查软失败字段不存在。前者只能证明 job 还在，不能证明 job 有门禁能力。CI 配置里最危险的退化，往往不是把某个 job 整个删掉，而是在保留名字和步骤的同时把它改成“失败也算过”。这种退化在人眼扫 YAML 时很难发现，因为截图、job 名称、日志入口都还在；只有 workflow 结论不再受它影响。v1611 把这类退化写成测试失败条件，等于把“看起来有 sanitizer”和“sanitizer 真正阻断 CI”区分开了。

这也解释了为什么本版虽然改动行数不多，仍然算一个完整版本，而不是附在 v1610 的尾巴上。v1610 的核心价值是接线和观察，v1611 的核心价值是执法和防回退。一个是让信号出现，一个是让信号有后果。二者在工程流程里是两个不同动作：前者适合在存在未知风险时用诊断模式保护主线，后者必须在真实运行绿过之后立刻收口，否则诊断轨道会长期悬空。把它们分开记录，后续回看时能清楚知道：如果 sanitizer 以后红了，不是“刚接入时还没稳定”，而是“已经从 v1611 起成为正式门禁”。

## 与边界字段的关系

v1611 没有新增 evidence command，因此不会新增新的 `read_only`、`execution_allowed`、`order_authoritative` 字段。但它保护这些字段背后的工程可信度。只读证据链如果运行在可能存在内存越界或未定义行为的 C++ 程序上，字段本身就会缺少一层底层保障。sanitizer required gate 不能证明所有路径都安全，但它能让已被测试覆盖的路径在每次 CI 中接受更严格的运行时检查。

对控制面来说，这些字段仍应这样理解：`read_only` 表示命令只读；`execution_allowed` 表示不能执行危险动作；`order_authoritative` 表示 mini-kv 不是订单权威；`mutates_store` 和 `touches_wal` 表示是否写内存状态或 WAL；`warnings`、`blockers`、`diagnostics` 是解释信息，不是自动执行入口。v1611 不改变任何字段含义，只是让这些字段的生成和测试路径多一道 ASan/UBSan 兜底。

这个区别非常重要。sanitizer 是工程质量工具，不是业务授权工具。即使 sanitizer job 绿，也不代表可以开放 managed audit connection、真实 endpoint、credential resolver 或 write routing。所有这些仍然受既有 no router、no write、no WAL、no execution 边界约束。K1 提升的是“代码在测试路径上的内存/未定义行为可信度”，不是“系统可以进入生产执行模式”。

## 本地验证与远端验证的分工

v1611 的本地验证仍然必须完整，因为本版修改了 C++ 测试文件和 CI YAML。首先用 CLion CMake 生成 `cmake-build-v1611`，确认默认 Debug 路径不受 sanitizer required gate 影响。然后构建全部目标，确保 `tests/dependabot_config_tests.cpp` 的新断言可以编译链接。接着跑 focused CTest，重点看 `dependabot_config_tests`，因为它直接保护 CI required 语义；同时可以带上 `command_tests`、`smokejson_command_receipt_tests`、`runtime_smoke_evidence_tests` 和 `metrics_file_tests`，确认常用命令与运行时只读证据没有被牵连。最后跑全量 CTest，确认 334 个目标继续通过。

本地仍然不会跑 Windows MinGW sanitizer build。v1610 已经把这个限制写进 CMake，`MINIKV_SANITIZE=ON` 在本机 GNU/Windows 下会快速失败，说明缺 `-lasan`/`-lubsan`。v1611 不需要重复把这条限制变成“失败截图”，因为本版没有改这部分逻辑。真正的 sanitizer required gate 必须由 GitHub Ubuntu CI 验证：推送后，如果 `ubuntu sanitizer` job 失败，workflow 会失败，K1 就不能算完成。

这形成了清晰分工：本地验证默认构建和测试、文档质量、C++ 测试断言；远端验证 required sanitizer gate 的真实 CI 行为。两边都需要。只有本地绿但远端失败，说明 K1 转正不成功；只有远端绿但本地没跑，则无法证明默认开发路径和新增断言没有问题。

## 为什么不把 K2 一起做掉

因为 K1 required gate 本身就是一个完整版本主题。K2 coverage 会引入新的 CMake option、coverage flags、gcovr/lcov 工具、CI artifact 和 baseline/floor 规则。把 K2 混进 v1611，会让一个本来很干净的“sanitizer 转 required”变成“sanitizer + coverage + 工具链 + baseline”大杂烩。如果 CI 失败，就不容易判断是 sanitizer required gate 失败，还是 coverage 配置失败。

更合理的节奏是：v1611 完成 K1；下一版开始 K2 coverage baseline。等 K2 完成并 CI 绿，就到了用户问的第一个 Claude review 节点。production-excellence program 的 review protocol 写得很明确：after each project's coverage milestone。也就是说，现在不用让 Claude review；应该在 mini-kv K2 覆盖率里程碑落地、进度表更新、CI 通过之后，再让 Claude 做一次 freshness review。K4 文件拆分完成后还有第二个重要 review 点。

v1611 对 K2 的意义也更具体。coverage baseline 会告诉我们哪些核心模块被测试覆盖、哪些 evidence/receipt 模块覆盖较低、哪些路径暂时只能通过 fixture 或 smoke 间接保护。如果没有 required sanitizer gate，coverage 只能回答“跑到了多少代码”，不能回答“跑到的代码有没有被更严格地检查”。现在 K1 已经收口，K2 的覆盖率数字就更有工程含义：被覆盖路径不仅在普通 Debug 下通过，也会在 Ubuntu ASan/UBSan 下通过。等 K2 完成后，Claude review 就有两类证据可以一起看：一类是覆盖率基线和阈值是否合理，另一类是 sanitizer required gate 是否已经稳定执行。

这也是我现在不建议叫 Claude review 的原因。v1611 只是 K1 终点，不是 program review checkpoint。提前让 Claude 在 K1 后 review，能看到 sanitizer 绿，但看不到 coverage baseline，也无法判断后续拆分安全网是否完整。更好的时机是 K2 落地后，让它审查“sanitizer + coverage”这套安全网是否足够支撑 K4 拆 `command.cpp` 和 `shard_readiness.cpp`。这样 review 才不会停留在单点 CI 绿，而是看阶段能力是否形成。

这种节奏也符合 mini-kv 的版本粒度规则：每版一个大小合适、能闭环验证的小主题。v1611 虽然改动行数不多，但它完成的是一条质量门从 diagnostic 到 required 的状态转换，必须有测试、讲解、归档、提交、tag、push 和 CI 验证，不能把它当成一句 YAML 小修。

## 风险和回滚

v1611 的主要风险是 sanitizer job 以后会成为真实阻断项。如果后续某个正常功能在 sanitizer 下失败，CI 会红。这正是 required gate 的意义，但也意味着维护者不能再忽略 sanitizer 输出。修复策略应该是先读 sanitizer 日志，定位具体未定义行为或内存问题；只有在明确是工具链误报或平台限制时，才考虑缩小测试范围或记录 deviation。不能因为 required gate 让 CI 红，就把 `continue-on-error` 偷偷加回来。

第二个风险是 `assert_not_contains(ci_workflow, "continue-on-error: true")` 比较强。它会阻止整个 workflow 中任何 continue-on-error 字段。当前阶段这是可以接受的，因为 production-excellence program 要求机械门禁，不鼓励软失败检查。如果未来确实需要某个实验性 job 软失败，应写清楚原因，并把测试改成只禁止 sanitizer job 下的 continue-on-error，而不是直接删断言。

回滚也很清楚：如果 v1611 远端 CI 证明 sanitizer required gate 不可接受，回滚本版即可恢复 v1610 的诊断模式。但在回滚之前必须先判断失败性质。若失败是 sanitizer 找到真实 bug，就应该修 bug 而不是回滚门禁；若失败是 CI runner 的外部工具链临时问题，可以短期回滚或暂缓 required，但要在 progress deviation 中记录。K1 的目标是稳定机械门，不是为了表面绿而逃开信号。

从 release closeout 角度看，本版还提供了一个后续维护信号：版本归档里既要保留 v1610 诊断绿的前置证据，也要保留 v1611 required CI 绿的最终证据。这样以后如果有人问“为什么当时敢把 sanitizer 设成 required”，不需要翻散落的终端历史，只要看 `f/1611/解释/说明.md` 和对应截图，就能看到前置 run、YAML diff、本地 CTest 和远端 required run 的完整链路。这个链路也提醒后续版本：质量门不是改完文件就结束，必须推送后看真实 CI 结论；否则进度表里的 completed 只是愿望，不是证据。

一句话总结：v1611 把 mini-kv 的 Ubuntu ASan/UBSan 轨道从 v1610 的诊断状态升级为 required CI gate，并用 C++ 测试防止 `continue-on-error` 静默回归，同时保持运行时命令、只读证据、归档路径、写入、WAL 和执行边界全部不变。
