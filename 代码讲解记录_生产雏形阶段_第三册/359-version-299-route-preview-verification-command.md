# mini-kv v299 Route Preview Verification Command

v299 的目标是把 v298 的 `shardRoutePreview` 成果向前推进一格：先冻结 `fixtures/release/shard-readiness-v298.json`，再给控制面一个可以直接询问的只读验证命令 `SHARDROUTEVERIFYJSON key`。它的定位是验证 `SHARDROUTEJSON` 的字段一致性，不是 active shard router，不是写路由，不是 WAL 写入，不是 restore/load/compact 执行入口，也不是 Node 或 Java 的运行时授权。

本版继续沿用最新 Node v549 计划的边界：mini-kv 可以并行补强证据链，但不启动 Node/Java，不消费真实服务路由，不打开 deployment、rollback、managed audit connection 或 raw endpoint。v299 因此只在 C++ 本侧产出版本化 evidence。

## 冻结基线

`fixtures/release/shard-readiness-v298.json` 是本版最重要的基线文件。它保存 v298 的 route preview rollout package audit 状态，current `fixtures/release/shard-readiness.json` 则滚动到 v299。

这条冻结关系写进 `src/shard_readiness_route_catalog_post_closeout_stage.cpp`：

- `sourceFrozenReleaseVersion` 是 `v298`
- `sourceFrozenFixturePath` 是 `fixtures/release/shard-readiness-v298.json`
- `continuityStage` 是 `feature-read-only-shard-route-preview-verification-command-contract`
- `stageReleaseVersion` 是 `v299`

这里修正了一个容易误导维护者的点：v299 的后续 ledger 应该引用 v298 的真实冻结 fixture 和 digest，而不是继续把旧基线当成当前来源。

## 新增 verifier 模块

新模块在 `include/minikv/shard_route_preview_verification.hpp` 和 `src/shard_route_preview_verification.cpp`。

它的核心职责有三层：

1. `verify_key(key)` 调用既有 `shard_route_preview::preview_key(key)`，读取 route preview 的槽位、shard、hash 和 pinned sample 状态。
2. 将读取结果转成九个检查项：`slot_within_range`、`single_shard`、`preview_only`、`pinned_sample_consistent`、`hash_present`、`read_only`、`no_active_router`、`no_write_routing`、`no_execution`。
3. `format_verification_json(key)` 和 `format_rollout_json()` 输出控制面可消费的 JSON。

关键点是 verifier 不自己计算路由，也不写任何 storage。它只是复用 `SHARDROUTEJSON` 已经公开的 preview 语义，再把“这些字段为什么可信”展开成可断言的检查列表。这样 Node 或 Java 以后读到它时，可以知道这是一个验证 receipt，而不是一个可以执行路由的 endpoint。

## 命令入口

`src/command.cpp` 新增 `SHARDROUTEVERIFYJSON` 的 dispatch 项和处理分支：

- 参数格式固定为 `SHARDROUTEVERIFYJSON key`
- 多余参数直接返回 usage
- 成功时返回 `shard_route_preview_verification::format_verification_json(key)`

同时 `src/command_contracts.cpp` 把它登记为 read command：

- `category=read`
- `mutates_store=false`
- `touches_wal=false`
- `stable=true`
- `side_effects=["metadata_read"]`

这使 `COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 都能看到相同边界。`src/line_editor.cpp` 和 `tests/line_editor_tests.cpp` 也补上补全候选，避免命令面和交互提示不一致。

## SHARDJSON 集成

`src/shard_readiness.cpp` 把 current release 从 v298 推到 v299，并在 `shardRoutePreview` 后新增 `shardRoutePreviewVerification` 段。

这个段不是覆盖旧 route preview rollout，而是另起一个 verification rollout：

- `rolloutMode=read-only-shard-route-preview-verification-rollout`
- `verifiedCommand=SHARDROUTEJSON`
- `commands=["SHARDROUTEVERIFYJSON"]`
- `publishedStageCount=1`
- `rolloutStage=route-preview-verification-command-contract`
- `readOnly=true`
- `mutatesStore=false`
- `touchesWal=false`
- `executesRoute=false`
- `activeRouterInstalled=false`
- `writeRoutingAllowed=false`
- `executionAllowed=false`

也就是说，v279-v298 的 route preview 线保持原样，v299 开始的是一条新的 verification 线。这样后续 v300-v318 可以继续增长 stage catalog，而不会把旧 preview rollout 的含义搅乱。

## 测试保护

`tests/shard_route_preview_verification_tests.cpp` 是本版新增的集中测试：

- 直接验证 `verify_key` 的九项检查全部通过。
- 验证 `format_verification_json` 输出 `command`、`checkedCommand`、`passedCount`、`executionAllowed=false` 等关键字段。
- 验证 rollout JSON 的 source frozen、stage、commands 和 no-router/no-write/no-execution 边界。
- 验证命令 usage、正常命令、`EXPLAINJSON`、`CHECKJSON`。
- 用 `DBSIZE` 前后比较证明命令不改变 store。

旧测试也同步扩展：

- `command_tests` 校验 command count 从 32 到 33，并确认 catalog/explain/check 都识别新命令。
- `shard_readiness_tests` 校验 current fixture、历史 fallback、post-closeout ledger、COMMANDS/COMMANDSJSON 和命令直接调用。
- `shard_readiness_post_closeout_continuity_tests` 校验 v299 continuity ledger。
- `line_editor_tests` 校验 `SHARDROUTEV` 能补全到 `SHARDROUTEVERIFYJSON`。

定向测试 6/6 通过，全量 CTest 74/74 通过。

## 真实 smoke

CLI smoke 运行：

```text
SHARDROUTEVERIFYJSON orderops:alpha
SHARDROUTEJSON orderops:alpha
CHECKJSON SHARDROUTEVERIFYJSON orderops:alpha
```

TCP smoke 短暂启动 `minikv_server.exe 6529 127.0.0.1`，用 `minikv_client.exe` 发送同样的 verifier/read preview 命令，验证后停止服务端。

两条 smoke 都证明 `SHARDROUTEVERIFYJSON` 返回 `passed=true`、`checkCount=9`、`executionAllowed=false`。TCP 进程没有被保留。

一句话总结：v299 把 route preview 从“能读预览”推进到“能读并验证预览”，但仍严格保持只读、no router、no write、no execution，为后续 Node/Java/mini-kv 松耦合消费提供更硬的证据面。
