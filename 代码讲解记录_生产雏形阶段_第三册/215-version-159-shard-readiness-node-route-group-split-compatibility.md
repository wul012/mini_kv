# mini-kv v159: Node route-group split compatibility evidence

v159 follows the current Node v418 route-group split plan. Node v418 is a route-registration refactor only, so mini-kv does not need to change runtime behavior. This version freezes v158 and publishes read-only compatibility evidence that the split Node route group may keep consuming the same `SHARDJSON` contract.

This version is not runtime approval, not a new evidence gate, and not a live-read gate. It does not change API paths, start Java, start mini-kv on the declared approval port, run runtime probes, enable active shard routing, write storage, execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`, or make mini-kv order/audit authoritative.

## New Module

`include/minikv/shard_readiness_node_compatibility.hpp` and `src/shard_readiness_node_compatibility.cpp` hold the Node compatibility formatter. Keeping this out of `src/shard_readiness.cpp` prevents the main `SHARDJSON` file from growing again after the v158 approval-input split.

`src/shard_readiness.cpp` still owns the top-level contract, ordering, digest, historical fixture chain, and boundary field list. It now delegates only the v418 compatibility object to the new helper.

## Evidence

`nodeRouteGroupSplitCompatibility` records that Node v418 is `route-registration-refactor-only`. It keeps `nodeApiPathChanged=false`, `nodeResponseShapeChanged=false`, `nodeAddsEvidenceGate=false`, `nodeStartsJavaService=false`, and `nodeStartsMiniKvService=false`.

On the mini-kv side, the same object keeps `miniKvContractChangedForNodeRouteSplit=false`, `miniKvFixturePathChanged=false`, `miniKvShardJsonCommandChanged=false`, and `miniKvRequiresNodeRouteTableChange=false`.

Runtime remains blocked through the usual guards: canonical approval inputs are incomplete, templates are not canonical inputs, runtime packet is not executable, service start is false, runtime probe/live read/router/write/execution are false.

## Fixture And Tests

`fixtures/release/shard-readiness-v158.json` stores the frozen v158 canonical approval input precheck with digest `fnv1a64:bce4629123167bc6`.

The current `fixtures/release/shard-readiness.json` is regenerated from real `minikv_cli.exe` `SHARDJSON` output and has digest `fnv1a64:0741c3475ee371e2`.

`tests/shard_readiness_tests.cpp` validates current v159, all historical shard-readiness fixtures through v158, and the new route split compatibility object. `tests/command_tests.cpp` verifies the same object through the real `CommandProcessor` command surface.

The closure loop includes targeted 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6430`. The declared approval port `6424` remains unstarted, and the GET probe remains `(nil)`.

In one sentence: v159 lets Node reorganize route registration around mini-kv evidence without changing mini-kv's read-only runtime boundary.
