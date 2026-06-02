# Boundary Catalog Group Table Split

## Goal

mini-kv v269 is a maintenance-only split/optimization release. It does not add an execution entry, change SHARDJSON read-only boundaries, enable router/write routing, allow LOAD/RESTORE/COMPACT, or start services. It freezes v268 as a versioned fixture and keeps this refactor inside the Node v549 follow-up evidence chain.

## Maintenance Value

freezes v268 as a versioned source fixture and moves the boundary catalog group table into a dedicated source file while preserving read-only/no-router/no-write/no-execution boundaries.

The key review question is not what new feature exists. The question is whether the maintenance surface became smaller: repeated helpers are reduced, oversized files are easier to split next, test failures are easier to locate, and evidence fields remain safe for read-only Node/Java consumption.

## Boundary Reading

- readOnly=true and executionAllowed=false remain the first control-plane boundary checks.
- routerActivationAllowed=false, writeRoutingAllowed=false, and writeCommandsAllowed=false show that the maintenance refactor did not become runtime routing.
- sourceFrozenReleaseVersion=v268 and sourceFrozenFixturePath=fixtures/release/shard-readiness-v268.json show that this release advances from frozen evidence instead of consuming rolling current.

## Verification

This release rebuilds mini-kv, runs targeted tests, full CTest, and a real TCP read-only SHARDJSON smoke. Archive screenshots and the explanation are stored under the versioned e/269 archive folder.

One sentence: v269 lowers maintenance cost while keeping mini-kv as a read-only cross-project evidence provider.