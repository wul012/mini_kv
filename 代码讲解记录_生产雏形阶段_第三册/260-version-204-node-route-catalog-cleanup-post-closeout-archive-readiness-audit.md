# mini-kv v204 node route catalog cleanup post-closeout archive readiness audit

## Goal and Boundary

v204 freezes the v203 archive-readiness snapshot and audits that the post-closeout continuity lane remains frozen, versioned, and read-only.

The latest Node plan checked was `docs/plans3/v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`. Node v490 exposes archive verification through the normal audit route surface, but mini-kv v204 does not open any route, Java echo, approval chain, service launch, runtime probe, router, write path, or execution entry.

## Frozen Source

`fixtures/release/shard-readiness-v203.json` is copied from the v203 rolling fixture before this version advances. Its digest is `fnv1a64:bed1ac036b8f548e`, and it remains the source for `sourceFrozenReleaseVersion=v203`.

The v204 rolling fixture reports digest `fnv1a64:670b62f7c203b814` and advances the continuity stage to `post-closeout-archive-readiness-audit`, sequence `4`, and release `v204`.

## Control-Plane Meaning

This version proves the archive-readiness checkpoint can be audited from a frozen v203 fixture without trusting rolling current. The denial fields remain visible: `routerActivationAllowed=false`, `writeRoutingAllowed=false`, `executionAllowed=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, and `loadRestoreCompactAllowed=false`.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` checks the in-process JSON, v203 frozen fixture, fallback path, source digest, stage sequence, tracked post-closeout range, and no-runtime fields.

`tests/command_tests.cpp` checks the public `SHARDJSON` surface for the same archive-readiness audit.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6477` returned v204 with digest `fnv1a64:670b62f7c203b814`, fallback v203, `readOnly=True`, router/write disabled, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v204 audits the Node v490-facing archive-readiness checkpoint while preserving mini-kv as read-only evidence, not runtime authority.
