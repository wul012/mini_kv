# v686 route-preview-value-supply-signed-approval-capture-preflight-request-id-input

## 本版目标

v686 把 v685 的 signed approval template 继续向后一层推进，新增 `SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON`，用于表达“未来如果要捕获签名审批，捕获前必须先具备哪些输入、策略和审计声明”。

它不是签名捕获入口，不是 approval grant，不导入 operator value，不存储 raw signature、secret、credential 或 endpoint，也不启动 Java/Node/mini-kv 服务。mini-kv 在这里仍然只是提供版本化、只读、可审计的证据。

## 入口和聚合

入口文件是 `src/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.cpp`，对外声明在 `include/minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp`。

主模块做四件事：

1. 固定 v686-v710 的 25 个阶段表，第一阶段是 capture preflight request id input。
2. 绑定 source evidence：`SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON`、`v685`、`fixtures/release/shard-readiness-v685.json`。
3. 输出当前阶段、release range、source digest marker、stage chain、helper catalog 和控制面解释。
4. 追加共享 no-execution boundary 字段，确保 `readOnly=true`、`executionAllowed=false`、router/write/WAL/restore/load/compact/service start 全部保持关闭。

## 拆分 helper

本版没有把所有字段堆在主文件里，而是拆成四个小模块：

- `*_inputs.cpp` 负责 25 个 capture preflight input，包括 request id、source template digest、source review digest、operator identity/role、timestamp、capture window、channel policy、signature algorithm、source file/snippet、redacted digest、value shape、provenance 和 closeout boundary。
- `*_policies.cpp` 负责 fail-closed policy 和 rejection code，强调缺输入、缺 digest、raw signature、approval grant、value submission、write route、sibling mutation 都必须被拒绝。
- `*_attestations.cpp` 负责控制面审查声明：读到这些字段时，只能理解为“未来需要人工/上游证据补足”，不能理解为 mini-kv 已经生成或接受了这些值。
- `*_validation.cpp` 负责 catalog 数量、发布阶段范围和 release complete 状态，避免主模块把数量逻辑写散。

这种拆分让后续 v687-v710 只推进阶段数和 release evidence，不需要反复打开一个更大的巨型源文件塞字段。

## 命令和 SHARDJSON

`src/command.cpp` 把新命令加入 runtime evidence 分发。它不接受参数，有多余 token 时返回 usage error；正常执行时只返回 capture-preflight JSON，不碰 store 和 WAL。

`src/command_contracts.cpp` 把命令登记为 `category=read`、`mutates_store=false`、`touches_wal=false`。因此 `EXPLAINJSON` 和 `CHECKJSON` 会把它解释为 metadata read，并给出 `execution_allowed=false`。

`src/shard_readiness.cpp` 把顶层 release 推到 `v686`，status 推到 `route-preview-value-supply-signed-approval-capture-preflight-request-id-input-read-only`，并新增 `shardRoutePreviewOperatorValueSupplySignedApprovalCapturePreflight` 单节嵌入。旧的 signed approval template section 仍然保留，作为 source evidence，而不是被覆盖。

## 测试保护

新增 5 个 focused tests：

- inputs test 证明 request id 第一阶段发布，closeout 第 25 阶段未发布，并且所有 input 都是 present/captured=false。
- policies test 证明 rejection code 和 blocks capture/grant/import/runtime/router/write/execution 的边界。
- attestations test 证明 operator review required 但 present=false，且 sibling mutation/value import/execution 不允许。
- validation test 证明 25/25/25 catalog 对齐，partial stage 时 releaseComplete=false。
- command test 证明命令 dispatch、usage error、COMMANDS/COMMANDSJSON、EXPLAIN/CHECK、SHARDJSON 单节嵌入和 read-only boundary。

同时更新了历史 value-supply/template tests 的顶层 status 断言：它们继续证明旧 section 存在，但不再把滚动 status 锁死在 v685。这符合当前 SHARDJSON 的滚动证据字段规则。

## 真实验证

本版使用 CLion MinGW/Ninja 工具链重新构建，并跑过：

- focused tests：7/7 通过。
- full CTest：107/107 通过。
- CLI smoke：直接读取 capture-preflight JSON、EXPLAINJSON、CHECKJSON、SHARDJSON。
- TCP smoke：通过 `COMMANDS`、`EXPLAINJSON`、`CHECKJSON` 证明新命令在 TCP 上可见且仍是 read/no-mutate/no-WAL/no-execution；临时 server 验证后已停止。

一句话总结：v686 让 signed approval capture 的前置条件有了版本化、可读、可测试的 C++ 证据入口，但仍不打开签名捕获、approval grant、value import、路由写入或任何执行能力。
