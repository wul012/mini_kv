# mini-kv v185 Node route catalog closeout snapshot

v185 freezes the v184 route catalog final closeout window and adds a new read-only snapshot for the Node v464 closeout validation result. It is evidence for downstream control-plane readers only. It is not a router, not a write path, not a runtime approval packet, and not permission to start Java or mini-kv services.

## Implementation

`src/shard_readiness_route_catalog_closeout.cpp` owns `nodeRouteCatalogCloseoutSnapshot`. The section records the Node v464 plan path, the frozen v184 fixture and digest, the v433-v464 route catalog batch, focused closeout counts, typecheck/build/full Vitest pass markers, and the explicit no-runtime flags.

`src/shard_readiness.cpp` now publishes release `v185`, status `node-route-catalog-closeout-snapshot-read-only`, and includes the closeout snapshot in the digest seed and final `SHARDJSON` response.

`src/shard_readiness_history.cpp` extends fixture parity to include `fixtures/release/shard-readiness-v184.json` and moves historical fallback to the frozen v184 evidence, so rolling current is still not used as a historical baseline.

`src/shard_readiness_boundary_fields.cpp` advances the boundary catalog to `read-only-boundary-fields.v12`. The added fields catalog only closed booleans from the snapshot: no fresh mini-kv evidence consumption, no runtime contract change, no services, no probe, no router, no write/admin commands, no LOAD/RESTORE/COMPACT, and no execution.

## Tests and Evidence

`tests/command_tests.cpp` and `tests/shard_readiness_tests.cpp` assert the v185 release, frozen v184 fixture, snapshot counts, digest baseline, boundary catalog v12, and the no-router/no-write/no-execution flags. The shard readiness fixture test also reads the frozen v184 fixture to prove the historical chain remains versioned.

Validation used the normal closure path: rebuild after C++ changes, regenerate `fixtures/release/shard-readiness.json`, run targeted `command_tests` and `shard_readiness_tests`, run full CTest, and perform a real TCP `SHARDJSON` smoke on loopback. v185 keeps mini-kv as a read-only evidence publisher for Node/Java coordination.
