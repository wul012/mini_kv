# mini-kv v542 route-preview-import-preflight-explain-contract

本版目标是把 `SHARDROUTEIMPORTPREFLIGHTJSON` 的只读导入预检证据推进到 `route-preview-import-preflight-explain-contract`。它的角色是给控制面一个可读、可冻结、可测试的预检结论：operator evidence import preflight 可以被识别，但 mini-kv 仍不导入任何值。

它不是什么也要说清楚：它不是新的执行入口，不是 active shard router，不是 write routing，不是 manual evidence entry，不接收 runtime payload 或 synthetic evidence，不保存 credential、secret、raw endpoint，也不会启动服务、读取生产端点、执行 restore/load/compact。这个边界和 Node v886 计划一致，mini-kv 只给出 C++ 侧的版本化证据。

## 代码结构

`src/shard_route_preview_operator_import_preflight.cpp` 是本版主入口，`format_import_preflight_json()` 负责输出 `shard-route-preview-operator-import-preflight.v1`。它读取的是编译期 stage table 和 source worksheet verifier 的发布计数，不做运行时文件遍历，所以 `sourceWorksheetFixturePath` 只是版本化证据路径，不是运行时读取动作。

`include/minikv/shard_route_preview_operator_import_preflight.hpp` 暴露四个小接口：stage catalog、主 JSON、digest marker 和 published stage count。这样 `src/shard_readiness.cpp` 可以嵌入 section，也可以把 digest marker 纳入整体 digest seed。

`src/route_preview_evidence_boundary.cpp` 是本轮维护拆分点。它把 read-only/no router/no write/no execution 这组重复字段收敛成 `format_no_execution_boundary_fields()`，worksheet verifier 和 import preflight 都调用同一份字段，后续再扩展 route-preview 证据时不需要继续复制长尾 JSON 字段。

`src/command.cpp` 把 `SHARDROUTEIMPORTPREFLIGHTJSON` 接到 RuntimeEvidence 分支；`src/command_contracts.cpp` 把它登记为 read、mutates_store=false、touches_wal=false；`src/line_editor.cpp` 给 CLI 补全加上同名候选。这三个入口共同保证命令可见但不可写。

## 字段解释

`readyForOperatorEvidenceImportPreflight=true` 只表示控制面可以读取预检证据，不表示可以导入值。与之配套的 `readyForEvidenceImport=false`、`operatorEvidenceValuesAccepted=false`、`manualEvidenceImportAllowed=false`、`runtimePayloadAccepted=false`、`syntheticEvidenceAccepted=false` 共同形成 fail-closed 解释。

`secretValueStored=false`、`credentialValueStored=false`、`rawEndpointStored=false` 说明预检只处理标签和契约，不保存敏感值。`normalizerExecutes=false` 和 `normalizationExecutionAllowed=false` 则说明所谓 normalizer 仍是说明性合同，不是可运行转换器。

`activeRouterInstalled=false`、`writeRoutingAllowed=false`、`touchesWal=false`、`executionAllowed=false` 是 mini-kv 的硬边界：它没有写入、没有 WAL、没有 router、没有执行权。

## 测试和证据

`tests/shard_route_preview_operator_import_preflight_tests.cpp` 检查 source worksheet v535、Node v886 plan、25 个 operator preflight slot、0 个 accepted/imported value，以及所有 no import/no runtime/no execution 字段。

`tests/route_preview_evidence_boundary_tests.cpp` 专门盯共享 boundary helper，防止后续某个证据命令漏掉 no router/no write/no execution 字段。

`tests/command_tests.cpp`、`tests/shard_readiness_tests.cpp` 和 `tests/line_editor_tests.cpp` 覆盖命令目录、SHARDJSON 嵌入、fixture parity 和 CLI 补全。归档中的 CLI/TCP smoke 进一步证明新命令通过真实命令路径读取，但没有写 store 或触碰 WAL。

一句话总结：v542 让 import preflight 证据链向前推进一格，同时把重复边界字段收敛到共享 helper，让 Node/Java/mini-kv 后续松耦合消费时更容易判断“可读，但不可导入、不可执行”。
