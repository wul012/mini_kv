# mini-kv v220: post-closeout continuity v211-v219 batch package audit

## 1. Goal and non-goals

v220 is the audit companion to v219. v219 packaged the v211-v218 post-closeout continuity run; v220 freezes that package as a stable source fixture and audits the next rolling current fixture as the v211-v219 batch package. The point is to make the evidence chain more durable for future Node-side verification without increasing mini-kv's runtime authority.

This version is not a new router, write path, control-plane executor, Node route, Java endpoint, service startup path, managed-audit adapter, deployment command, or rollback mechanism. It does not read credential values, parse raw endpoints, open a network connection other than the local TCP smoke test, execute LOAD/RESTORE/COMPACT, or mutate production data. It remains a C++ read-only evidence release.

The active Node plan remains `docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md`. That plan says Java and mini-kv may proceed in parallel and that Node does not need fresh sibling evidence for this closeout archive step. v220 respects that by auditing mini-kv's own fixture chain rather than pretending to perform live Node consumption.

## 2. Frozen source baseline

The new frozen source file is:

```text
fixtures/release/shard-readiness-v219.json
```

It preserves the exact v219 rolling SHARDJSON fixture before v220 updates the current one. v220 records that baseline as:

```text
sourceFrozenReleaseVersion = v219
sourceFrozenFixturePath = fixtures/release/shard-readiness-v219.json
sourceFrozenDigest = fnv1a64:78b10360a474df83
```

The new rolling current fixture is:

```text
fixtures/release/shard-readiness.json
```

Its v220 digest is:

```text
fnv1a64:d9ecc44211f0fe02
```

This gives future readers two stable anchors: the immutable v219 source and the v220 audit result. If a later Node version checks the package, it can verify both the previous evidence file and the current audit fixture.

## 3. Stage descriptor update

The stage descriptor is still isolated in:

```text
src/shard_readiness_route_catalog_post_closeout_stage.cpp
```

v220 updates it to:

```text
continuityStage = post-closeout-continuity-v211-v219-batch-package-audit
stageSequence = 20
stageReleaseVersion = v220
stageSummary = audit the v211-v219 post-closeout continuity batch package from frozen v219 evidence
sourceFrozenReleaseVersion = v219
sourceFrozenFixturePath = fixtures/release/shard-readiness-v219.json
sourceFrozenDigest = fnv1a64:78b10360a474df83
sourceFrozenReason = frozen v219 fixture is the source baseline
```

Keeping this descriptor in a small module is paying off here. The audit step can be expressed by changing a handful of stable fields instead of letting the main JSON formatter absorb more one-off literals. That is the right refactoring rhythm for this codebase: after many evidence versions, the hot metadata surface stays understandable.

## 4. Rolling JSON and history fields

The rolling release identity and digest seed are in:

```text
src/shard_readiness.cpp
```

v220 changes the release to `v220`, changes the digest seed to `mini-kv-shard-readiness-v220`, and advances the digest inputs around historical fallback, boundary catalog index, compatibility window, and post-closeout continuity. These values make the digest deterministic: if someone changes a boundary field or stage identity later, the digest moves.

The history and fallback fields are in:

```text
src/shard_readiness_history.cpp
```

v220 extends `historicalFixturePaths` with `fixtures/release/shard-readiness-v219.json`, changes the fallback to `previousConsumedReleaseVersion=v219`, and states that Node v499+ may consume v220 as route catalog cleanup continuity batch package audit evidence only. The wording stays conservative. "May consume" is a compatibility statement; it is not a claim that Node has already read the file in a live runtime run.

## 5. Boundary interpretation

The most important fields remain denials:

```text
readOnly = true
executionAllowed = false
activeRouterInstalled = false
routerActivationAllowed = false
writeRoutingAllowed = false
writeCommandsAllowed = false
adminCommandsAllowed = false
loadRestoreCompactAllowed = false
startsServices = false
runtimeProbeAllowed = false
liveReadAllowed = false
```

Control-plane consumers should interpret v220 as evidence they can inspect. They must not treat it as approval to route writes, start services, execute storage commands, run restore drills, or elevate mini-kv into Java order authority.

This is the line that keeps the project honest: mini-kv is proving storage-side boundaries and evidence continuity; it is not becoming the orchestrator for Node or Java.

## 6. Tests

The focused tests are:

```text
tests/command_tests.cpp
tests/shard_readiness_tests.cpp
```

`command_tests` checks the runtime command output. For v220, it verifies the release version, source frozen v219 fields, v219 digest, audit stage name, stage sequence 20, range end v220, release count 20, Node v498 batch closeout alignment, and no-router/no-write/no-execution flags.

`shard_readiness_tests` checks the fixture contract. It verifies that the current fixture and command output agree, that `fixtures/release/shard-readiness-v219.json` is now part of the historical fixture list, that the historical fallback points to v219, and that the node consumer sentence remains evidence-only.

The full CTest run passed 71/71 tests. That broad run is important because v220 touched evidence fields in a storage project with many older guarantees. Passing the full suite proves no accidental change in WAL, snapshot, restore, TCP protocol, runtime smoke, credential resolver non-participation, no-network safety, abort/rollback semantics, or input hardening.

## 7. Real TCP smoke

The TCP smoke starts a real server and client:

```text
minikv_server.exe 6493 127.0.0.1
minikv_client.exe 127.0.0.1 6493
```

The client sends:

```text
SHARDJSON
QUIT
```

The parsed result confirms:

```text
releaseVersion = v220
evidenceDigest = fnv1a64:d9ecc44211f0fe02
continuityStage = post-closeout-continuity-v211-v219-batch-package-audit
stageSequence = 20
sourceFrozenReleaseVersion = v219
sourceFrozenDigest = fnv1a64:78b10360a474df83
trackedPostCloseoutRangeEnd = v220
trackedPostCloseoutReleaseCount = 20
readOnly = true
executionAllowed = false
routerActivationAllowed = false
writeRoutingAllowed = false
```

This verifies the live command surface while still staying within the read-only test boundary. The server is stopped immediately after the smoke run.

## 8. Archive and downstream value

The archive for this version is:

```text
e/220/图片/
e/220/解释/说明.md
```

The retained screenshots show the build, targeted tests, full tests, TCP smoke, and parsed summary. The explanation file states what each artifact proves and what it deliberately does not prove.

For the wider Node/Java/mini-kv chain, v220 closes the requested ten-version run with a durable audit package: Node can later verify the continuity evidence against immutable mini-kv fixtures, while mini-kv stays read-only, non-authoritative, and non-executing.

One sentence summary: v220 audits the v211-v219 continuity package from a frozen v219 baseline and keeps the boundary crisp: evidence can be read, but no router, writer, executor, credential path, or order authority is opened.
