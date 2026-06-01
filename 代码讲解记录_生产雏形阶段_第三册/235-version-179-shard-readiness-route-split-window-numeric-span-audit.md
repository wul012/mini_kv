# 235. mini-kv v179 route split window 数字跨度审计

## 本版目标

v179 的目标是把 v178 的 `nodeRouteSplitCompatibilityWindowAudit` 证据冻结成 `fixtures/release/shard-readiness-v178.json`，再把当前 `SHARDJSON` 往前推进为更可机读的 route split window 数字跨度审计。

它不是什么也同样重要：它不是新的 shard router，不是 Node/Java 的服务启动信号，不是写路由、运行探测或执行许可，也不是把 mini-kv 变成订单或审计权威。它只让控制面能更直接地验证 Node v433-v458 这一段 route-registration-only 窗口是否数字上连续、范围是否可解析、数量是否与范围匹配。

## 冻结与滚动证据

`fixtures/release/shard-readiness-v178.json` 保存了上一版的 rolling `SHARDJSON` 输出，作为本版的历史基线。当前 rolling fixture 仍写在 `fixtures/release/shard-readiness.json`，但 `releaseVersion` 前进到 `v179`，digest 前进到 `fnv1a64:d88350b581f2c458`。

`src/shard_readiness.cpp` 负责把这些版本关系写进主证据：当前版本、fixture parity、historical fallback、diagnostics、digest 种子和状态文案都统一切到 v179。这里的关键边界是：rolling fixture 可以更新，但历史消费基线必须引用冻结文件，不能让 downstream consumer 依赖会漂移的当前文件。

## Node compatibility audit

`src/shard_readiness_node_compatibility.cpp` 继续承载 Node route split window 的只读证据。v179 没有改变窗口成员，仍是 Node v433-v458 共 26 个 route split/refactor 版本；变化在于 audit 里增加了更结构化的数字字段：

- `windowRangeStartNumber=433`
- `windowRangeEndNumber=458`
- `computedWindowVersionSpan=26`
- `windowRangeNumbersParseable=true`
- `windowCountMatchesRange=true`

这些字段来自同一份 covered-version vector 和 `Node vNNN` 标签解析结果，用来避免控制面只靠字符串说明判断范围。控制面读到它们时，应理解为“这个窗口的数字范围可以被解析，并且成员数量等于 end-start+1”；它仍不能理解为 active shard routing 已经可用。

## Boundary catalog

`src/shard_readiness_boundary_fields.cpp` 把目录版本推进到 `read-only-boundary-fields.v10`，字段数推进到 422，组数保持 22。新增的两个边界字段分别是：

- `nodeRouteSplitCompatibilityWindowAudit.windowRangeNumbersParseable`
- `nodeRouteSplitCompatibilityWindowAudit.windowCountMatchesRange`

这让目录本身也能声明新增 audit 字段属于 read-only boundary surface。控制面如果只读取 boundary catalog，也可以看到这些字段是证据字段，而不是写命令、运行入口或路由开关。

## 测试保护

`tests/command_tests.cpp` 验证 `SHARDJSON` 命令路径暴露 v179 版本、numeric span mode、v178 frozen source、v10 boundary catalog、422 字段数和新增数字跨度字段。

`tests/shard_readiness_tests.cpp` 做更完整的 fixture 级验证：当前 fixture 必须不同于冻结的 v178 fixture；冻结 v178 仍保留 v178 digest 和旧 audit 语义；当前 v179 必须包含数字范围、跨度和两个布尔审计字段，同时继续断言 `readOnly=true`、`executionAllowed=false`、无 active router、无 write routing、无 write/admin command、无 LOAD/RESTORE/COMPACT 执行。

定向测试证明新增字段本身正确，全量 71 条 CTest 证明它没有碰坏 WAL/TCP/恢复/运行证据等既有行为。真实 TCP smoke 再确认 `minikv_client` 经服务端读取到的仍是只读 `SHARDJSON` 证据，并且 smoke 后服务进程被停止。

## 归档证据

`e/179/图片/` 保存了构建、fixture 生成、定向测试、全量测试和 TCP smoke 的五张命令截图；`e/179/解释/说明.md` 逐张解释命令、结果和意义。归档只保留最终证据，构建目录、临时日志、渲染 HTML 和截图辅助进程在收口前清理。

一句话总结：v179 让 Node v433-v458 route split window 从“字符串上说明连续”前进到“字段上证明数字跨度一致”，同时继续保持 mini-kv 只读、无路由、无写入、无执行。
