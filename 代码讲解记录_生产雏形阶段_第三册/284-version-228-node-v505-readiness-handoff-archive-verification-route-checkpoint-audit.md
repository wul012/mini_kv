# mini-kv v228: audit node v505 readiness handoff archive verification route checkpoint evidence

## 1. Goal and non-goals

v228 audits the Node v505 readiness handoff archive verification route checkpoint from frozen v227 evidence. It continues the clean mini-kv evidence line after v220 and aligns the current SHARDJSON package with the Node v505 readiness handoff evidence archive verification route plan.

This is not a new router, write path, control-plane executor, Node route, Java endpoint, service startup path, managed-audit adapter, deployment command, or rollback mechanism. It does not read credential values, parse raw endpoints, execute LOAD/RESTORE/COMPACT, or mutate production data. The only live process used in validation is the local mini-kv TCP smoke server, and it is stopped during cleanup.

The active Node plan is:

`	ext
docs/plans3/v505-post-java-mini-kv-route-catalog-cleanup-readiness-handoff-evidence-archive-verification-route-roadmap.md
`

That plan says Java and mini-kv are recommended parallel. For mini-kv, the right response is durable read-only evidence, not live sibling consumption.

## 2. Frozen source baseline

The frozen source file is:

`	ext
fixtures/release/shard-readiness-v227.json
`

It preserves v227 before v228 updates the rolling fixture. v228 records it as:

`	ext
sourceFrozenReleaseVersion = v227
sourceFrozenFixturePath = fixtures/release/shard-readiness-v227.json
sourceFrozenDigest = fnv1a64:2b6e2366ce59bc20
`

The new rolling current fixture remains:

`	ext
fixtures/release/shard-readiness.json
`

Its digest is:

`	ext
fnv1a64:7a4fe3614ff923cd
`

This two-file shape keeps current evidence and source evidence separate. Later consumers can verify both the immutable previous release and the current package.

## 3. Stage descriptor

The current stage descriptor lives in:

`	ext
src/shard_readiness_route_catalog_post_closeout_stage.cpp
`

v228 sets:

`	ext
continuityStage = post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-checkpoint-audit
stageSequence = 28
stageReleaseVersion = v228
stageScope = audit the Node v505 readiness handoff archive verification route checkpoint from frozen v227 evidence
sourceFrozenReleaseVersion = v227
sourceFrozenFixturePath = fixtures/release/shard-readiness-v227.json
sourceFrozenDigest = fnv1a64:2b6e2366ce59bc20
`

This descriptor stays intentionally small. It gives each release a precise version identity without growing the main SHARDJSON formatter into a giant literal block.

## 4. JSON assembly and history

The rolling release identity and digest seed are in:

`	ext
src/shard_readiness.cpp
`

The history and fallback fields are in:

`	ext
src/shard_readiness_history.cpp
`

v228 adds `fixtures/release/shard-readiness-v227.json` to the historical fixture list, moves the fallback to v227, and states the Node consumer position as evidence-only. The wording deliberately says that Node may consume the package later; it does not claim a fresh Node runtime read happened in this C++ release.

The post-closeout JSON now points at Node v505 through:

`	ext
sourceNodePlan = docs/plans3/v505-post-java-mini-kv-route-catalog-cleanup-readiness-handoff-evidence-archive-verification-route-roadmap.md
nodeBatchCloseoutVersion = Node v505
`

That is a plan alignment marker, not an execution permission.

## 5. Boundary interpretation

The important fields remain hard denials:

`	ext
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
`

Control-plane consumers should read v228 as evidence only. They must not treat it as approval to route writes, start services, execute storage commands, run restore drills, or elevate mini-kv into Java order authority.

## 6. Tests and smoke

The focused tests are:

`	ext
tests/command_tests.cpp
tests/shard_readiness_tests.cpp
`

They protect the command-facing SHARDJSON output, the rolling fixture, the frozen historical fixture path, the v505 plan path, the stage sequence, the source digest, and the no-router/no-write/no-execution flags.

The full CTest run passed 71/71 tests. That broader run matters because evidence updates live beside real storage behavior. Passing the full suite proves no accidental change in store behavior, WAL, snapshot, restore, TCP protocol, runtime smoke, credential resolver non-participation, no-network safety, abort/rollback semantics, or input hardening.

The TCP smoke starts a real localhost server and sends:

`	ext
SHARDJSON
QUIT
`

The parsed result confirms v228, digest `fnv1a64:7a4fe3614ff923cd`, source frozen v227, stage sequence 28, and the same read-only boundary flags.

## 7. Archive and downstream value

The archive for this version is:

`	ext
e/228/图片/
e/228/解释/说明.md
`

The screenshots cover build, targeted tests, full tests, TCP smoke, and parsed evidence summary. This gives future Node/Java/mini-kv work a durable evidence package while mini-kv stays read-only, non-authoritative, and non-executing.

One sentence summary: v228 strengthens the v505-aligned readiness handoff evidence chain without opening a router, writer, executor, credential path, or order authority.