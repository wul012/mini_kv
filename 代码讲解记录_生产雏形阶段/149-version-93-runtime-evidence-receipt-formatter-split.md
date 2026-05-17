# mini-kv v93 runtime evidence receipt formatter 拆分

本版目标是继续把 `src/command.cpp` 从“所有运行时证据都堆在一起”的形态里拆开一步。v92 已经把 managed audit receipt 的 JSON/digest formatter 拆到了独立模块；v93 接着拆运行时证据收据：live-read session hint、SMOKEJSON failure taxonomy、operator-window proof、binary provenance、retention provenance check、retention replay marker 等。

这不是新的执行入口，不是新的 read/write 权限，不是 Node/Java 的调用适配器，也不是恢复、加载、压缩、迁移或 managed audit 写入能力。`INFOJSON` 和 `SMOKEJSON` 的字段契约保持不扩展，write/admin/WAL/snapshot/restore 处理路径保持不动，mini-kv 仍然只是只读运行时证据提供者。

## 本版处在什么位置

mini-kv 现在已经有较长的运行时证据链：CI evidence、artifact retention、live-read session、binary provenance、retention provenance、managed audit restore boundary、non-authoritative storage、command dispatch quality、adapter shell guard、external/sandbox adapter non-participation 等。问题是这些证据长期集中在 `src/command.cpp`，文件不断变大，后续维护时很容易把“命令聚合”和“证据格式化”混在一起。

v93 的合理拆分幅度是：只拆运行时证据 receipt formatter，不同时拆 command parser、CHECKJSON、STORAGEJSON、WAL 或 TCP。这样改动足够有价值，也不会把多个风险面塞进同一版。

## 文件职责

`include/minikv/runtime_evidence_receipts.hpp` 是新的只读证据 formatter 头文件。它暴露的是纯格式化函数，例如 `format_live_read_session_hint_json()`、`format_smoke_failure_taxonomy_json()`、`format_runtime_binary_provenance_hint_json()` 和 `format_runtime_retention_provenance_check_json()`。这些函数只返回 JSON 字符串或 digest，不接触 store，不执行命令。

`src/runtime_evidence_receipts.cpp` 承接了原来 `src/command.cpp` 中的运行时证据结构和 digest 逻辑。它复用 `minikv::runtime_evidence` 里的 `json_string`、`json_bool`、`json_string_array` 和 `digest` helper，统一生成 `fnv1a64` 证据指纹。这里的 `source_version`、`artifact_path_hint`、`runtime_binary_hint` 随 v93 更新为 `0.93.0`、`c/93/` 和 `cmake-build-v93/...`。

`src/command.cpp` 现在只保留运行时命令聚合职责。`INFOJSON` 仍然组装 identity、server、store、WAL、metrics、diagnostics 等主结构，然后调用 runtime evidence receipts 模块补上 CI/artifact/binary/retention 证据。`SMOKEJSON` 仍然组装 runtime smoke 主体、real-read 命令清单、forbidden commands、diagnostics 等，然后调用新模块补上 live-read session、operator window、failure taxonomy 和 provenance 证据。

`src/managed_audit_receipts.cpp` 仍然负责 managed audit receipt 链。v93 只把当前 artifact hint 和 digest 链推进到 `c/93/`，没有改变 managed audit receipt 的字段含义。

## 控制面如何理解关键字段

`read_only=true` 和 `execution_allowed=false` 仍然表示这些对象只是可读证据，不是执行许可。Node 可以读取它们来判断 mini-kv 是否满足前置边界，但不能因此自动启动 mini-kv、执行 restore、上传 CI artifact、打开生产窗口或写 managed audit state。

`order_authoritative=false` 继续强调 mini-kv 不是 Java 订单权威。即便 `SMOKEJSON` 暴露 sandbox adapter non-participation receipt，也只是说明 mini-kv 不参与 sandbox audit storage，不读取 credential，不执行 schema migration，不写 sandbox managed audit state。

`receipt_digest`、`provenance_digest`、`check_digest` 和 `marker_digest` 是字段集合的稳定证据指纹，不是鉴权签名。v93 中这些 digest 变化主要来自版本号和 `c/93/` artifact path hint 更新，以及 formatter 模块边界变化后的 fixture 对齐。

`runtime evidence receipt formatter split only` 和 `no INFOJSON/SMOKEJSON field contract expansion from runtime evidence receipt split` 是本版新增的边界说明。它们告诉控制面：这里发生的是代码组织拆分，不是运行时字段扩张，也不是新的执行路径。

## 测试和真实运行

`tests/command_tests.cpp` 继续直接断言 `INFOJSON` 和 `SMOKEJSON` 中的 v93 digest、read-only/no-execution 字段、sandbox adapter non-participation receipt 以及 `GET restore:real-read-token` 不产生值。

`tests/release_verification_manifest_tests.cpp` 和 `tests/runtime_smoke_evidence_tests.cpp` 更新到 v93，新增断言确认 `include/minikv/runtime_evidence_receipts.hpp`、`src/runtime_evidence_receipts.cpp` 被 release manifest 记录，并确认 runtime evidence receipt split 只是一项 formatter split，不扩展 `INFOJSON` / `SMOKEJSON` 字段契约。

真实 smoke 使用 `minikv_server` 和 `minikv_client` 执行 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`。结果证明 v93 运行时仍然只提供只读证据，`GET restore:real-read-token` 仍为 `(nil)`，没有执行 LOAD、COMPACT、SETNXEX、RESTORE 或 managed audit 写入。

## 一句话总结

v93 把 runtime evidence receipt formatter 从 `src/command.cpp` 中拆出来，让 mini-kv 的只读证据链更可维护，同时保持 Node/Java/mini-kv 之间的松耦合边界不变。
