# mini-kv v148: shard-readiness active plan freeze

v148 的目标很窄：把 v147 的 `activePrototypePlan` 证据冻结成 `fixtures/release/shard-readiness-v147.json`，再把 current `SHARDJSON` 滚到 v148。这样 Node v380 如果选择消费新的完成证据，就可以读冻结文件，而不是读滚动 current。

本版不是什么：不是 active shard router，不是多目录存储，不是多进程服务编排，不是 write routing，不是执行授权，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，不写订单或审计状态，也不让 mini-kv 成为业务权威。

`src/shard_readiness.cpp` 仍保持一个窄接口：`fixture_path()` 和 `format_json()`。内部只是沿着既有小 formatter 延伸：`format_fixture_parity_json()` 把历史路径扩到 v144-v147，`format_archive_compatibility_json()` 把 Node archive 范围扩到 v370-v379，`format_historical_fallback_json()` 指向 frozen v147。

本版新增的核心 formatter 是 `format_active_prototype_plan_freeze_json()`。它记录 `frozenReleaseVersion=v147`、`frozenFixturePath=fixtures/release/shard-readiness-v147.json`、`frozenStatus=active-prototype-prerequisite-read-only`，并把 `frozenActiveShardPrototypeAllowed`、`frozenRouterActivationAllowed`、`frozenShardDirectoryCreationAllowed`、`frozenMultiProcessStartAllowed`、`frozenWriteRoutingAllowed` 全部固定为 false。

current `SHARDJSON` 现在是 `releaseVersion=v148`、`status=active-prototype-plan-frozen-read-only`。它仍然声明 `readOnly=true`、`executionAllowed=false`、`shardEnabled=false`、`shardCount=1`、`slotCount=16`，样例路由仍只指向 `shard-0`。这说明 slot table 仍是 evidence，不是活跃路由。

`readOnlyBoundaryFields` 继续扩展到 frozen plan 维度，新增 `activePrototypePlan.writeRoutingAllowed`、`activePrototypePlanFreeze.rollingCurrentUsedForFrozenBaseline`、`activePrototypePlanFreeze.frozenRouterActivationAllowed`、`activePrototypePlanFreeze.frozenWriteRoutingAllowed`。这些字段让下游可以直接断言 frozen baseline 没有偷偷打开 router 或 write path。

`fixtures/release/shard-readiness-v147.json` 保存 v147 的完整 activePrototypePlan evidence，包括 `status=active-prototype-prerequisite-read-only`、`activePrototypePlan.writeRoutingAllowed=false` 和 `evidenceDigest=fnv1a64:e4a386fc9add4eaf`。current v148 的 digest 是 `fnv1a64:8270eeebbf1a7f29`，测试会防止 v147 frozen fixture 和 v148 current 混用。

`tests/shard_readiness_tests.cpp` 同时保护 current v148、v144 prototype、v145 hardening、v146 historical fallback 和 v147 active plan freeze baseline。`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径确认 `SHARDJSON` 暴露 v148、`activePrototypePlan`、`activePrototypePlanFreeze` 和 no write/no router 边界。

验证闭环包括 CMake 配置、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6418 127.0.0.1`，用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`，确认 v148 字段走真实命令路径可见；服务端验证后停止，端口无残留。截图和说明归档在 `e/148/图片/` 与 `e/148/解释/说明.md`。

一句话总结：v148 把 activePrototypePlan 从“current 可见”推进到“历史可冻结、下游可复现”，同时继续把 router、write 和 execution 全部关在只读证据边界之外。
