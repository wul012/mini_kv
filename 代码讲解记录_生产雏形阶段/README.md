# mini-kv 代码讲解记录_生产雏形阶段
- `171-version-115-disabled-credential-resolver-precheck-non-participation-receipt.md`：v115 增加给 Node v263 消费的只读 disabled credential resolver precheck 非参与回执，回显 Node v262 disabled resolver precheck 的 env handle、opt-in gate、failure class、dry-run response shape 和 inherited no-go 条件，同时证明 mini-kv 不实例化 resolver client 或 secret provider、不调用 resolver、不 auto-start、不写 storage、不读取/加载/存储/包含 credential value、不解析 raw endpoint URL、不发送外部请求、不执行 schema migration 或 LOAD/COMPACT/RESTORE/SETNXEX，也不成为 managed audit storage；本版也把 credential resolver 收据拆到 `src/runtime_credential_resolver_receipts.cpp`，让 `src/runtime_sandbox_receipts.cpp` 维持在 1000 行以下。
- `170-version-114-credential-resolver-non-participation-receipt.md`：v114 增加给 Node v261 消费的只读 credential resolver 非参与回执，回显 Node v260 credential resolver decision 字段和 no-go 条件，同时证明 mini-kv 不实例化 secret provider、不调用 resolver、不 auto-start、不写 storage、不读取/加载/存储/包含 credential value、不解析 raw endpoint URL、不发送外部请求、不执行 schema migration 或 LOAD/COMPACT/RESTORE/SETNXEX，也不成为 managed audit storage。
- `169-version-113-sandbox-endpoint-handle-non-participation-receipt.md`：v113 增加给 Node v259 消费的只读 sandbox endpoint handle 非参与回执，回显 Node v258 endpoint-handle preflight review 计数和 handle/policy/window 字段，同时证明 mini-kv 不 auto-start、不写 storage、不读取 credential value、不解析 raw endpoint URL、不发送外部请求、不执行 LOAD/COMPACT/RESTORE/SETNXEX，也不成为 managed audit storage。
- `168-version-112-fake-transport-packet-non-participation-receipt.md`: v112 adds a read-only fake transport dry-run packet non-participation receipt for Node v257, echoing Node v255 packet and Node v256 archive verification counts while proving mini-kv does not auto-start, write storage, read credential values, send external requests, create cleanup artifacts, execute LOAD/COMPACT/RESTORE/SETNXEX, or become managed audit storage.
- `167-version-111-disabled-adapter-client-non-participation-receipt.md`: v111 adds a read-only disabled adapter client non-participation receipt for Node v254, echoing Node v252 disabled client precheck and Node v253 fake transport shell counts while proving mini-kv does not auto-start, write storage, read credential values, send external requests, execute LOAD/COMPACT/RESTORE/SETNXEX, or become managed audit storage.
- `166-version-110-dependabot-github-actions-maintenance.md`: v110 adds GitHub Actions-only Dependabot maintenance plus `dependabot_config_tests`, protecting weekly grouped minor/patch action updates, semver-major auto-update ignore rules, and the boundary that mini-kv has not adopted npm/Maven/C++ package ecosystems.
- `165-version-109-benchmark-evidence-guard.md`: v109 adds `minikv_benchmark --evidence-json` plus a CTest `benchmark_evidence_guard`, keeping the default benchmark table unchanged while proving the benchmark stays local, no-WAL, no-network, no-restore, no-credential, no-managed-audit-connection, and non-order-authoritative.
- `164-version-108-manual-sandbox-precheck-non-participation-receipt.md`: v108 adds a read-only manual sandbox connection precheck non-participation receipt for Node v246, echoing the Node v245 precheck packet shape while proving mini-kv does not auto-start, write storage, read credential values, execute LOAD/COMPACT/RESTORE/SETNXEX, or become managed audit storage.
- `163-version-107-manual-sandbox-command-non-participation-receipt.md`: v107 adds a read-only manual sandbox dry-run command non-participation receipt for Node v244, echoing the Node v241 disabled command package shape while proving mini-kv does not auto-start, write storage, read credential values, execute LOAD/COMPACT/RESTORE/SETNXEX, or become managed audit storage.
- `162-version-106-command-dispatch-table.md`: v106 replaces the top-level `execute_trimmed` command-name if chain with a local dispatch table plus `CommandDispatchVerb` switch, preserving parser behavior, responses, WAL/snapshot/runtime evidence boundaries, command metrics, and TCP smoke behavior.
- `161-version-105-wal-auto-compact-gate.md`: v105 replaces the single WAL command mutex with an explicit command/compaction gate, keeping append-before-mutation ordering intact while moving automatic/manual compaction out of the normal WAL write command critical section and adding a concurrent auto-compaction replay regression.
- `160-version-104-managed-audit-receipt-implementation-split.md`: v104 splits the managed audit receipt implementation out of one 900+ line translation unit into boundary-specific source files plus a tiny coordinator, while keeping the public header contract, digest shapes, JSON field sets, INFOJSON/SMOKEJSON boundaries, and Node v239 no-start/no-write evidence chain unchanged.
- `156-version-100-current-runtime-fixture-rolling-guard.md`: v100 adds a current runtime fixture rolling guard for Node v235, allowing current version/artifact/digest evidence to roll while keeping historical consumed digest anchors fixed.
- `155-version-99-wal-helper-regression-evidence.md`: v99 adds execute-with-wal regression coverage for SET, SETNXEX, DEL, and EXPIRE usage-error, no-op miss, expired-key miss, and append-before-mutation behavior while rolling current runtime evidence to `0.99.0` / `v99` for Node v234 blocked execution rehearsal.
- `154-version-98-execute-with-wal-helper.md`: v98 adds `CommandProcessor::execute_with_wal` to consolidate SET/SETNXEX/DEL/EXPIRE WAL and no-WAL branches while preserving append-before-mutation order, no-op miss behavior, write responses, runtime JSON contracts, snapshot/restore/admin boundaries, and Node/Java non-authoritative limits.
- `153-version-97-sandbox-connection-no-start-guard.md`: v97 adds `managed_audit_sandbox_connection_no_start_guard_receipt` for Node v231, consumes the historical v96 echo marker digest, and keeps manual window closed by default with no Node/Java/mini-kv auto-start, no connection execution, no credential value read, no schema rehearsal, no managed audit write, no restore/load/compact, and no Java order authority.
- `152-version-96-sandbox-connection-receipt-echo-marker.md`: v96 adds `managed_audit_sandbox_connection_receipt_echo_marker` for Node v229, consumes the historical v95 sandbox receipt digest, and keeps mini-kv read-only with no connection execution, credential value read, schema rehearsal, managed audit write, restore/load/compact, or Java order authority.
## 最新新增

- `151-version-95-string-utils-and-version-sweep.md`：说明 v95 如何把 `trim_copy` / `to_upper` 收拢到 `include/minikv/string_utils.hpp`，并把版本、fixture、runtime evidence 和测试同步到 `0.95.0` / `v95`，同时把 `command.cpp` 继续压到 1000 行以下。
- `150-version-94-command-contract-formatter-split.md`：说明 v94 如何把 command catalog、`EXPLAINJSON`、`CHECKJSON` 的命令契约 formatter 拆到 `include/minikv/command_contracts.hpp` / `src/command_contracts.cpp`，并把运行响应 formatter 拆到 `include/minikv/command_response_formatters.hpp` / `src/command_response_formatters.cpp`，让 `command.cpp` 降到 713 个物理行，同时保持 CHECKJSON/EXPLAINJSON 字段契约、运行响应契约、command digest、write/admin/WAL/snapshot/restore 和 Node/Java 边界不变。
- `149-version-93-runtime-evidence-receipt-formatter-split.md`：说明 v93 如何把 live-read session、failure taxonomy、operator-window proof、binary provenance、retention provenance 等 runtime evidence receipt 的 JSON/digest formatter 从 `src/command.cpp` 拆到 `include/minikv/runtime_evidence_receipts.hpp` 和 `src/runtime_evidence_receipts.cpp`，保持 `INFOJSON` / `SMOKEJSON` 字段契约不扩张，也不触碰 write/admin/WAL/snapshot/restore 或 Node/Java 边界。
- `148-version-92-managed-audit-receipt-formatter-split.md`：说明 v92 如何把 managed audit receipt 的 JSON/digest 格式化从 `src/command.cpp` 拆到 `include/minikv/managed_audit_receipts.hpp` 和 `src/managed_audit_receipts.cpp`，保持 `INFOJSON` / `SMOKEJSON` 字段契约不扩张，同时继续证明 mini-kv 不参与 sandbox adapter storage、credential read、schema migration 或 managed audit state write。
- `145-version-89-adapter-shell-non-storage-guard-receipt.md`：说明 v89 如何在 `INFOJSON` / `SMOKEJSON` / release fixture 中补入 `adapter_shell_non_storage_guard_receipt`，让 Node v221 只读确认 mini-kv 不是 adapter shell storage backend，不写 local dry-run records / managed audit state，也不改变 write/admin/WAL/snapshot/restore 或 Java order authority 边界。
- `144-version-88-command-dispatch-quality-receipt.md`：说明 v88 如何把 runtime evidence 命令归入独立读侧分发路径，并在 `INFOJSON` / `SMOKEJSON` / release fixture 中补入 `command_dispatch_quality_receipt`，让 Node v219 只读确认本版没有改变 write/admin/WAL/snapshot/restore，也没有让 mini-kv 成为 managed audit store 或 Java order authority。
- `143-version-87-managed-audit-adapter-non-authoritative-storage-receipt.md`：说明 v87 如何在 `INFOJSON` / `SMOKEJSON` 中补充 `managed_audit_adapter_non_authoritative_storage_receipt`，让 Node v217 只读确认 mini-kv 不是 managed audit store，不允许 storage write/admin/restore，也不承担 Java order authority。

本目录从 v57 之后作为新的代码讲解入口使用，和旧目录同级。
目录名里的“生产雏形阶段”表示当前项目进度：mini-kv 已经不只是内存 KV 练手项目，而是进入带 TCP/RESP、WAL、Snapshot、指标、只读 evidence、恢复证据和跨项目控制面消费雏形的阶段。

```text
D:\C\mini-kv\代码讲解记录
D:\C\mini-kv\代码讲解记录_生产雏形阶段
```

旧目录保留 v1-v56 的历史讲解，不再继续堆新文件。

## 写入规则

后续每次推进 mini-kv 版本时，新的代码讲解文件写入本目录。
如果项目进入新的阶段，再新建同级目录，不继续塞进旧阶段目录。目录命名格式为：

```text
代码讲解记录_阶段名称
```

示例：

```text
代码讲解记录_生产雏形阶段
代码讲解记录_生产强化阶段
代码讲解记录_多项目融合阶段
```

文件命名模式继续沿用旧目录：

```text
113-version-57-主题.md
114-version-58-主题.md
115-version-59-主题.md
```

讲解结构继续沿用旧模式，并参考 Java 侧生产雏形阶段讲解密度：

```text
先说明本版目标、角色定位和不做事项
再说明本版所处项目进度
再按文件或模块讲清入口、数据结构、核心流程
然后用代码契约解释关键字段和控制面如何理解
再说明测试、真实运行、归档和成熟度变化
最后做一句话总结
```

也就是说，本目录不是只写“代码改了什么”，还要明确说明“本版让 mini-kv 的生产雏形阶段推进到了什么程度”。

## 当前项目进度基线

截至 v56，mini-kv 已经从基础 C++ KV 项目推进到具备网络协议、持久化恢复、运行观测和控制面只读证据的 Redis-like KV 练手产品雏形。

当前主线能力：

```text
KV 核心
 -> SET / GET / DEL
 -> EXPIRE / TTL
 -> SIZE
 -> KEYS / KEYSJSON
 -> 线程安全 Store
 -> 过期 key 清理

协议与客户端
 -> 本地 CLI
 -> TCP server
 -> TCP client
 -> Inline text protocol
 -> RESP parser
 -> RESP TCP compatibility tests
 -> 并发 RESP client tests
 -> 客户端历史记录
 -> key cache
 -> Tab 补全和行编辑

持久化与恢复
 -> WAL append
 -> WAL2 checksum
 -> 旧 WAL replay compatibility
 -> WAL replay report
 -> WAL repair
 -> WAL compaction
 -> auto compact
 -> manual Snapshot SAVE / LOAD
 -> atomic snapshot save
 -> restart recovery evidence sample
 -> recovery fixture index

运行观测
 -> HEALTH
 -> STATS / STATSJSON
 -> INFO / INFOJSON
 -> COMMANDS / COMMANDSJSON
 -> command metrics
 -> connection metrics
 -> metrics file export
 -> JSONL metrics export
 -> metrics rotation

控制面只读证据
 -> EXPLAINJSON
 -> CHECKJSON
 -> CHECKJSON write/read fixtures
 -> STORAGEJSON
 -> recovery evidence fixture
 -> recovery fixture index
 -> read_only / execution_allowed / order_authoritative 边界字段
```

成熟度判断：

```text
KV 基础能力：中高成熟
TCP / RESP 协议练习：中等偏高成熟
WAL / Snapshot / restart recovery：中等成熟，已有证据样本
运行观测与 evidence：中高成熟
真实生产安全：仍需继续补强
跨项目融合：mini-kv 适合作为自研 KV、缓存、幂等/观测实验层和 Node 控制面的只读证据上游
```

还没有完成的方向：

```text
真实认证、授权和多租户隔离
更完整的 crash consistency 故障注入
更多恢复失败样本和异常矩阵
长时间压测、资源限制和性能回归记录
更完整 Redis 命令兼容性
集群、副本、高可用
生产部署、备份、监控和告警链路
作为缓存/幂等适配层时和 Java / Node 的稳定契约
```

## 后续讲解索引

新版本讲解从这里继续追加：

```text
113-version-57-recovery-index-boundaries.md
 -> 第五十七版 recovery index cost 与 retention boundary：restart/replay 成本、retention 边界、Node v107 消费提示、测试和归档

114-version-58-readonly-evidence-fixtures.md
 -> 第五十八版只读 evidence fixture 包：CHECKJSON、INFOJSON、STATSJSON 样本、动态字段边界、Node v152/v153 消费提示、测试和归档

115-version-59-runtime-read-self-description.md
 -> 第五十九版运行时只读自描述增强：INFOJSON/STATSJSON schema、read_only、execution_allowed、order_authoritative、diagnostics、Node v156 消费提示、测试和归档

116-version-60-runtime-read-field-guide.md
 -> 第六十版运行时只读字段说明增强：INFOJSON/STATSJSON 字段说明、动态字段解释、read-only smoke 边界、Node v159 release evidence review 消费提示、测试和归档

117-version-61-ttl-token-primitive.md
 -> 第六十一版短 TTL Token Primitive：SETNXEX 原子占位、过期后 reclaim、SETEXAT WAL replay、TTL token evidence、非订单权威边界、测试和归档

118-version-62-ttl-recovery-evidence.md
 -> 第六十二版 TTL Token Recovery Evidence：WAL/Snapshot/restart 恢复证据、过期 token 不复活、compact 后 TTL replay、Node v161 消费提示、测试和归档

119-version-63-release-verification-manifest.md
 -> 第六十三版 Release Verification Manifest：CMake/CTest/只读 smoke/fixture-version 清单、Node v162 消费提示、无新增 runtime 命令边界、测试和归档

120-version-64-runtime-artifact-rollback-evidence.md
 -> 第六十四版 Runtime Artifact Rollback Evidence：binary/version、WAL、Snapshot、fixture 版本回滚边界、Node v163 消费提示、只读 CHECKJSON smoke、测试和归档

121-version-65-runtime-artifact-bundle-manifest.md
 -> 第六十五版 Runtime Artifact Bundle Manifest：binary version、release manifest、rollback evidence、WAL/Snapshot compatibility、CTest/read-only smoke、fixture inventory bundle、Node v164 消费提示、测试和归档

122-version-66-restore-compatibility-handoff.md
 -> 第六十六版 Restore Compatibility Handoff：binary/WAL/Snapshot/fixture 兼容性人工确认样本、Node v166 消费提示、只读 CHECKJSON smoke、测试和归档

123-version-67-restore-dry-run-operator-package.md
 -> 第六十七版 Restore Dry-Run Operator Package：restore target、artifact digest 占位、WAL/Snapshot compatibility、CHECKJSON dry-run 命令、Node v167 消费提示、测试和归档

124-version-68-artifact-digest-compatibility-matrix.md
 -> 第六十八版 Artifact Digest Compatibility Matrix：binary/WAL/Snapshot/fixture digest 矩阵、Node v168 消费提示、只读 CHECKJSON smoke、测试和归档

125-version-69-release-artifact-digest-package.md
 -> 第六十九版 Release Artifact Digest Package：binary/WAL/Snapshot/fixture digest 包、restore drill command profile、operator confirmation、Node v170 消费提示、测试和归档

126-version-70-restore-drill-evidence.md
 -> 第七十版 Restore Drill Evidence：restore target、digest comparison placeholder、operator confirmation、Node v173 消费提示、测试和归档

127-version-71-restore-handoff-checklist.md
 -> 第七十一版 Restore Handoff Checklist：restore operator、artifact digest target、Snapshot/WAL review placeholder、CHECKJSON 风险确认、Node v175 消费提示、测试和归档

128-version-72-restore-evidence-retention.md
 -> 第七十二版 Restore Evidence Retention：restore checklist retention id、artifact digest placeholder、Snapshot/WAL review retention、CHECKJSON 风险证据保留、Node v178 消费提示、测试和归档

129-version-73-retained-restore-artifact-digest.md
 -> 第七十三版 Retained Restore Artifact Digest：restore artifact digest placeholder、Snapshot/WAL review digest、restore target placeholder、retention owner、Node v180 消费提示、测试和归档

130-version-74-restore-approval-boundary.md
 -> 第七十四版 Restore Approval Boundary：restore approver placeholder、restore target placeholder、artifact digest placeholder、Java transaction chain disconnected、Node v182 消费提示、测试和归档

131-version-75-restore-boundary-smoke-manifest.md
 -> 第七十五版 Restore Boundary Smoke Manifest：read-only smoke commands、runtime/client target、restore target placeholder、real-read token no-write proof、Node v185 消费提示、测试和归档

132-version-76-runtime-smoke-evidence.md
 -> 第七十六版 Runtime Smoke Evidence：SMOKEJSON 只读聚合入口、runtime smoke fixture、Node v191 real TCP read adapter rehearsal 消费边界、测试和归档

133-version-77-runtime-smoke-failure-taxonomy.md
 -> 第七十七版 Runtime Smoke Failure Taxonomy：SMOKEJSON failure_taxonomy、Node v193 失败分类消费边界、unsafe-surface/write-risk 字段、测试和归档

134-version-78-runtime-smoke-taxonomy-digest.md
 -> 第七十八版 Runtime Smoke Taxonomy Digest：SMOKEJSON taxonomy_digest、verification sample、Node v196 imported-window 结果导入前校验、测试和归档

135-version-79-operator-window-identity-neutral-proof.md
 -> 第七十九版 Operator-Window Identity-Neutral Proof：SMOKEJSON operator_window、identity_neutral_proof、Node v200 归档前身份中性证明、fixture/manifest、测试和归档。

136-version-80-ci-evidence-hint.md
 -> 第八十版 CI Evidence Hint：INFOJSON/SMOKEJSON ci_evidence、artifact_path_hint、no_restore_proof、upload_allowed、Node v201 manifest verification、fixture/manifest、测试和归档。

137-version-81-artifact-retention-evidence.md
 -> 第八十一版 Artifact Retention Evidence：INFOJSON/SMOKEJSON artifact_retention、artifact_root、artifact_path_hint、retention_days、github_artifact_upload_attempted、production_window_allowed、Node v203 retention gate、fixture/manifest、测试和归档。

138-version-82-live-read-session-hint.md
 -> 第八十二版 Live-Read Session Hint：SMOKEJSON live_read_session、session_id_echo、server_uptime_bucket、read_command_list_digest、write_commands_allowed、auto_start_allowed、Node v205 real-read execution packet、fixture/manifest、测试和归档。

139-version-83-binary-provenance-hint.md
 -> 第八十三版 Binary Provenance Hint：INFOJSON/SMOKEJSON binary_provenance、runtime_binary_hint、release_manifest_path、runtime_smoke_evidence_path、provenance_digest、production_binary_claimed=false、Node v208 managed audit persistence boundary candidate、fixture/manifest、测试和归档。

140-version-84-retention-provenance-check.md
 -> 第八十四版 Runtime Evidence Retention / Provenance Check：INFOJSON/SMOKEJSON retention_provenance_check、expected_binary_provenance_digest、check_digest、managed_audit_write_executed=false、Node v211 managed audit identity approval provenance dry-run packet、fixture/manifest、测试和归档。
```

后续版本继续在本目录追加。

## 一句话总览

旧目录记录“mini-kv 如何一步步长到 v56”，本目录从 v57 开始继续记录“每版代码怎么实现、生产雏形阶段推进到哪里、成熟度发生了什么变化”。

## 最新新增

- `141-version-85-retention-provenance-replay-marker.md`：说明 v85 如何在 `INFOJSON` / `SMOKEJSON` 中补充 `retention_provenance_replay_marker`，让 Node v213 能只读确认 Node v211 已消费过 mini-kv v84 retention provenance evidence，并继续保持不执行 restore、不写 managed audit、不进入 Java 订单权威链路。
- `142-version-86-managed-audit-adapter-restore-boundary-receipt.md`：说明 v86 如何在 `INFOJSON` / `SMOKEJSON` 中补充 `managed_audit_adapter_restore_boundary_receipt`，让 Node v215 能只读确认 mini-kv 不执行 restore/write、不写 managed audit、不进入 Java 订单权威链路。
- `146-version-90-external-adapter-non-participation-receipt.md`：说明 v90 如何在 `INFOJSON` / `SMOKEJSON` 中补充 `managed_audit_external_adapter_non_participation_receipt`，让 Node v223 能只读确认 mini-kv 不是 external adapter storage backend、不读取 credentials、不执行 migration、不参与真实 external adapter，并继续保持不写 managed audit、不过 Java order authority 边界。

- `147-version-91-sandbox-adapter-runtime-evidence-guard.md`：说明 v91 如何在 `INFOJSON` / `SMOKEJSON` 中加入 `managed_audit_sandbox_adapter_non_participation_receipt`，让 Node v225 只读确认 mini-kv 不是 sandbox audit storage backend、不读 sandbox/production credentials、不执行 schema migration、不写 managed audit state，并继续保持不执行 restore、不进入 Java order authority 边界。

## v101 latest addition

- `157-version-101-runtime-no-start-no-write-follow-up.md`: explains v101 `SMOKEJSON.runtime_no_start_no_write_follow_up`, `fixtures/release/runtime-no-start-no-write-follow-up.json`, Node v236 dry-run envelope handle consumption for Node v237 readiness, no-start/no-write boundary fields, tests, real TCP smoke, and archive evidence.

## v102 latest addition

- `158-version-102-operator-window-no-start-no-write-receipt.md`: explains v102 `SMOKEJSON.operator_window_no_start_no_write_receipt`, `fixtures/release/operator-window-no-start-no-write-receipt.json`, Node v238 checklist echo consumption for Node v239 verification, no-start/no-write/no-credential/no-schema/no-restore boundary fields, tests, real TCP smoke, and archive evidence.

## v103 latest addition

- `159-version-103-current-version-path-hint.md`: explains v103 maintenance cleanup for `current_version_path_hint`, reducing repeated current runtime artifact path literals in production formatters while keeping the Node v239 v102 evidence contract unchanged.

## v116 latest addition

- `172-version-116-test-only-resolver-shell-non-participation-receipt.md`: explains v116 `SMOKEJSON.test_only_resolver_shell_non_participation_receipt`, `fixtures/release/test-only-resolver-shell-non-participation-receipt.json`, Node v264 test-only fake resolver shell contract consumption for Node v265 verification, request/response/failure/guard/probe fields, no-start/no-write/no-credential/no-resolver/no-schema/no-restore boundaries, tests, real TCP smoke, and archive evidence.

## v117 latest addition

- `173-version-117-credential-resolver-fake-shell-archive-non-participation-receipt.md`: explains v117 `SMOKEJSON.credential_resolver_fake_shell_archive_non_participation_receipt`, `fixtures/release/credential-resolver-fake-shell-archive-non-participation-receipt.json`, Node v266 fake-shell archive verification consumption for Node v267 verification, v264/v265 archive file and snippet counts, no archive runtime read/no fake-shell rerun/no-start/no-write/no-credential/no-resolver/no-schema/no-restore boundaries, tests, real TCP smoke, and archive evidence.
