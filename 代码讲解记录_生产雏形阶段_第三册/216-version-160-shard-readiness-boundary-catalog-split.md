# mini-kv v160: boundary field catalog split

v160 is a maintenance version after the Node v418 route-group split context. It freezes v159 as a versioned fixture and then reduces the size of the main shard-readiness formatter by moving the long `readOnlyBoundaryFields` catalog into a dedicated module.

This is not runtime approval, not a new evidence gate, and not a router implementation. It does not change `SHARDJSON` command usage, does not change the current fixture path, does not start Java or mini-kv on the declared approval port, does not run a runtime probe, does not enable live read, does not route writes, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`include/minikv/shard_readiness_boundary_fields.hpp` declares two formatter functions for the catalog area. The header is deliberately small: callers only ask for ready-to-append JSON strings and do not learn about the internal field list representation.

`src/shard_readiness_boundary_fields.cpp` owns the extracted `readOnlyBoundaryFields` list. It also formats `boundaryCatalogMaintenance`, which states that the extraction is contract-preserving: `publicShardJsonContractChanged=false`, `fixturePathChanged=false`, `shardJsonCommandChanged=false`, and all runtime/router/write/execution flags remain false.

`src/shard_readiness.cpp` keeps the top-level `SHARDJSON` ordering, release version, digest, historical fixture chain, and all existing evidence sections. After v160 it delegates only the boundary catalog JSON to the new module, taking the main formatter from 991 lines down to 744 lines.

`CMakeLists.txt` adds the new `.cpp` file to the `minikv` library so `minikv_cli`, tests, and the TCP server all use the same formatter.

## Evidence Contract

The current `releaseVersion` is now `v160`, with status `boundary-field-catalog-split-read-only`.

`fixtures/release/shard-readiness-v159.json` freezes the previous route-group split compatibility evidence with digest `fnv1a64:0741c3475ee371e2`. The rolling current fixture remains `fixtures/release/shard-readiness.json`, now regenerated with digest `fnv1a64:c96ae9497cfd6386`.

`historicalFallback` moves from v158 to v159 and keeps `rollingCurrentUsedForHistoricalBaseline=false`. This matters for Node consumers because v159 remains the immutable baseline for route split compatibility while v160 is allowed to be a maintenance evolution.

`boundaryCatalogMaintenance` is the new control-plane clue. A consumer should read it as: the field catalog moved in the C++ source, but `readOnlyBoundaryFields` is still published, the command and fixture path did not change, Node v418 compatibility is preserved, and runtime execution remains unavailable.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` now validates the v160 current fixture, the added v159 frozen fixture, the new maintenance object, and the preserved read-only field names.

`tests/command_tests.cpp` verifies the same v160 fields through the real `CommandProcessor` path, so the evidence is not only a fixture string update.

The closure run used focused CTest for `command_tests|shard_readiness_tests`, then full CTest with 71/71 passing. A real TCP smoke started `minikv_server` only on temporary port `6431`, read `SHARDJSON`, confirmed `boundaryCatalogMaintenance` and `executionAllowed=false`, checked a missing key returned `(nil)`, then stopped the owned process. Port `6424` remained unstarted.

In one sentence: v160 pays down shard-readiness formatter debt while keeping the same frozen, read-only, no-router, no-write, no-execution contract available to Node and Java consumers.
