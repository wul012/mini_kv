# mini-kv v141: no-network safety fixture contract 非参与回执

v141 的目标是给 Node v324 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v323 的 `no-network safety fixture contract intake`，确认自己理解 no-network safety fixture 的字段、拒绝码、禁止动作和来源链，但不把自己变成 fixture 执行器、网络安全权威、transport denial policy 权威或 cleanup marker 权威。

这版不是什么：不是 runtime shell，不是 credential resolver 实现，不是 no-network fixture 运行器，不打开 socket，不发送 HTTP/TCP，不做 DNS/TLS transport work，不读取 credential value，不解析 raw endpoint URL，不实例化 secret provider 或 resolver client，不写 approval ledger/schema/storage，不自动启动 Node/Java/mini-kv，也不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`。

`include/minikv/runtime_evidence_receipts.hpp` 只新增 v141 的 digest 和 JSON formatter 声明。入口仍然属于 runtime evidence receipt 家族，不改变 Store、WAL、snapshot、restore、TCP 协议或命令分发的行为。

`src/runtime_credential_resolver_no_network_safety_fixture_receipts.cpp` 是本版核心实现。它把 Node v323 contract intake、Node v322 endpoint-handle allowlist approval prerequisite closure review、Node v313 catalog 指向、字段计数、warning/recommendation 和关闭边界集中在一个独立翻译单元里。这样 v140 的 endpoint-handle allowlist approval receipt 文件不再膨胀，新增代码也保持在可维护范围内。

这份回执最重要的控制面语义有三组。第一组是 Node v323 合同事实：`contract_digest=73dcd413298048da6283f81bd0c3b759b9c1c2d360eb1382983d7db7349b2b88`、10 个 required fields、12 个 prohibited fields、6 个 rejection reasons、10 个 no-go boundaries、21/21 checks。第二组是 prerequisite 状态：`no-network-safety-fixture` 从 `still-missing` 进入 `contract-intake-defined`，但需要 Java v149 和 mini-kv v141 upstream echo 后，Node v324 才能做下一步验证；`abort-rollback-semantics` 仍未关闭。第三组是 mini-kv 自身边界：`network_safety_fixture_executed=false`、`network_fixture_execution_allowed=false`、`network_safety_authority=false`、`http_request_sent=false`、`tcp_connection_attempted=false`、`network_socket_opened=false`、`credential_value_read=false`、`raw_endpoint_url_parsed=false`。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_no_network_safety_fixture_contract_non_participation_receipt` 字段，`diagnostics.notes` 增加 v141 标记，`node_consumption` 首句滚动到 Node v324，同时保留 Node v321/v140 和更早历史消费语义。v140 的测试同步改成检查历史句子仍存在，而不是要求它永远是 `node_consumption` 的第一句。

`fixtures/release/credential-resolver-no-network-safety-fixture-contract-non-participation-receipt.json` 是 standalone fixture；`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 嵌入同一份 receipt。这样 Node v324 可以从独立 fixture、runtime smoke 或 manifest 三个入口读到同一 digest 和同一组关闭边界。

`tests/credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，覆盖 Node v323 contract digest、Node v322 review digest、字段计数、prerequisite transition、v141 digest，以及 network/credential/raw URL/provider/client/write/authority 的关闭边界。`tests/smokejson_command_receipt_tests.cpp`、`tests/runtime_smoke_evidence_tests.cpp`、`tests/release_verification_manifest_tests.cpp` 继续保护聚合入口和归档 fixture。

验证闭环包括 clean configure/build、定向 CTest、全量 CTest、真实 CLI smoke 和真实 TCP smoke。CLI/TCP smoke 证明 `SMOKEJSON` 暴露 v141 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`GET restore:real-read-token` 返回 `(nil)`，没有写入 restore token 或触发恢复、压缩、写命令。归档截图放在 `d/141/图片/`，说明放在 `d/141/解释/说明.md`。

一句话总结，v141 给 Node v324 提供了 no-network safety fixture contract 的 mini-kv 侧只读非参与证据，同时把 fixture execution、socket、HTTP/TCP、DNS/TLS、credential、raw endpoint、provider/client、ledger/schema/storage、auto-start、restore/compact/write 命令和 audit/order authority 边界继续锁住。
