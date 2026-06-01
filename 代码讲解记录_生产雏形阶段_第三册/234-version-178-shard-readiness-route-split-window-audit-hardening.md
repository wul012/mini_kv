# 234. mini-kv v178：route split window audit 计算化硬化

## 本版目标

v178 是 v177 之后的一版小重构/硬化：冻结 v177 为 `fixtures/release/shard-readiness-v177.json`，然后把 `nodeRouteSplitCompatibilityWindowAudit` 中的两个关键判断从手写常量改成由 `coveredNodeVersions` 计算得出。

这版不是什么：不是扩展到新的 Node 版本，不新增 evidence gate，不改变 `SHARDJSON` 命令路径，不启动 Java/mini-kv 服务，不打开 active router，不允许 write routing、LOAD、RESTORE、COMPACT 或 runtime execution。

## 为什么要做

v167-v177 连续推进了多个 Node route split window 版本，窗口长度已经到 Node v433-v458、共 26 个版本。继续靠人工维护 `contiguousNodeVersionWindow=true` 和 `duplicateWindowVersionsDetected=false` 会留下维护风险：后续追加 v459/v460 时，如果漏掉一个版本或重复写入一个版本，字符串断言可能还停留在“看起来正确”的状态。

v178 的改法是让审计字段由同一个版本向量推导：

- `contiguousNodeVersionWindow`：逐个解析 `"Node v###"`，要求每一项等于前一项加一。
- `duplicateWindowVersionsDetected`：双层扫描窗口列表，发现重复版本即返回 true。

这样窗口正文、窗口数量、连续性和重复检测都围绕同一个 `route_split_compatibility_window_versions()` 数据源收敛。

## 代码位置

`src/shard_readiness_node_compatibility.cpp` 新增了三个内部 helper：

- `parse_node_version_number(...)`：只接受 `Node v` 前缀加数字，解析失败返回 -1。
- `route_split_window_is_contiguous(...)`：验证版本号连续。
- `route_split_window_has_duplicates(...)`：验证窗口列表没有重复。

`format_route_split_compatibility_window_audit_json()` 不再手写这两个布尔字段，而是把 helper 的结果通过 `runtime_evidence::json_bool(...)` 输出到 JSON。字段名和值语义不变，所以外部消费者仍读原来的 contract，只是生成逻辑更可靠。

## 版本化证据

v178 的 rolling fixture 仍是 `fixtures/release/shard-readiness.json`，当前 digest 为 `fnv1a64:dd89f7c3bd63d7c3`。它的 source frozen baseline 是：

- `sourceFrozenReleaseVersion=v177`
- `sourceFrozenFixturePath=fixtures/release/shard-readiness-v177.json`
- `sourceFrozenWindowDigest=fnv1a64:e773708a4decc60e`

`fixtureParity.historicalFixturePaths` 增加 v177，`historicalFallback.previousConsumedReleaseVersion` 也切到 v177，表示后续消费者要用冻结文件判断 v177，不再依赖 rolling current。

## 测试与 smoke

`tests/command_tests.cpp` 更新命令层断言，确认 v178 的 window mode、source frozen v177、26 个窗口版本、computed audit 字段和 no-execution 边界都在 `SHARDJSON` 响应里。

`tests/shard_readiness_tests.cpp` 增加 v177 frozen fixture 校验，确保历史 fixture 仍是 v177 的 foundational audit route split 语义，而当前 v178 已经切换为 computed audit hardening 语义。

本版验证：

- 定向 CTest：`command_tests`、`shard_readiness_tests`，2/2 通过。
- 全量 CTest：71/71 通过。
- TCP smoke：临时端口 6451 读取 `SHARDJSON`，确认 v178、computed audit mode、连续窗口 true、重复版本 false、`readOnly=true`、`executionAllowed=false`、`fnv1a64:dd89f7c3bd63d7c3`，随后停止服务端并确认端口释放。

一句话总结：v178 没有扩大功能面，而是把持续扩展 Node route split window 的审计逻辑做得更可维护，让后续版本推进时更难把漏号或重复版本藏进只读证据链。
