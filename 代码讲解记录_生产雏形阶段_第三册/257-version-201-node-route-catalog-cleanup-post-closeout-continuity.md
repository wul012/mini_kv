# mini-kv v201 node route catalog cleanup post-closeout continuity

## Goal and Boundary

v201 freezes v200 and adds `nodeRouteCatalogCleanupPostCloseoutContinuity`, a read-only continuity ledger for work after the Node v480 cleanup evidence batch closeout.

The latest Node plan checked was `docs/plans3/v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md`. v201 does not invent a new Node dependency. It keeps the C++ side ready for a future Node v481+ consumer while still opening no route, router, write path, service launch, runtime probe, approval gate, or execution entry.

## Module Split

The new formatter lives in `src/shard_readiness_route_catalog_post_closeout.cpp` with a matching public declaration in `include/minikv/shard_readiness_route_catalog_post_closeout.hpp`.

This is deliberately separate from the already-large cleanup catalog formatter. The split keeps the v201-v210 continuity work from making `shard_readiness_route_catalog_cleanup_catalog.cpp` harder to maintain.

## Frozen Baseline

`fixtures/release/shard-readiness-v200.json` freezes the v200 closeout audit before the rolling fixture advances.

The frozen baseline records digest `fnv1a64:d1e889711b5d8574`, boundary catalog `read-only-boundary-fields.v28`, 800 fields, and 39 groups.

The v201 rolling fixture reports digest `fnv1a64:9a3abb5ab3aaeb1c`, boundary catalog `read-only-boundary-fields.v29`, 821 fields, and 40 groups.

## Control-Plane Meaning

Control-plane readers should treat v201 as the first post-closeout continuity checkpoint. It says the v200 audit remains the only frozen source baseline, Node v480 validation is preserved, archive verifier checks remain 16/16, and later Node batches can consume a versioned continuity fixture.

The important denial fields remain visible: `routerActivationAllowed=false`, `writeRoutingAllowed=false`, `executionAllowed=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, and `loadRestoreCompactAllowed=false`.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` checks the in-process JSON, the frozen v200 fixture, the new continuity section, boundary catalog v29 counts, and continuity boundary fields.

`tests/command_tests.cpp` checks that the public `SHARDJSON` command exposes the same continuity metadata and unchanged read-only/no-execution boundaries.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6474` returned v201 with digest `fnv1a64:9a3abb5ab3aaeb1c`, boundary catalog v29, 821 fields, 40 groups, `readOnly=True`, router/write disabled, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v201 creates a maintainable post-closeout evidence lane without letting that lane become runtime authority.
