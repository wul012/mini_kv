# mini-kv v479 Route Preview Archive Maintenance SHARDJSON Embedding Audit

v479 advances the read-only route preview archive maintenance ledger to $(System.Collections.Hashtable.Stage). It is not a router, not a write path, not a WAL writer, not a restore/load/compact entry point, and not Java order authority. The version exists so downstream consumers can understand the v441-v460 archive verification package while mini-kv keeps the runtime boundary closed.

## Files

- include/minikv/shard_route_preview_archive_maintenance.hpp exposes the maintenance formatter, stage catalog formatter, digest marker, and published stage count.
- src/shard_route_preview_archive_maintenance.cpp owns the 25-stage v461-v485 catalog and formats the current read-only evidence.
- 	ests/route_preview_evidence_assertions.hpp keeps repeated read-only and command-contract assertions out of the giant command and shard readiness tests.
- 	ests/shard_route_preview_archive_maintenance_tests.cpp is the focused behavior test for this command.

## Contract Fields

The control plane should read eadOnly=true, executionAllowed=false, ctiveRouterInstalled=false, writeRoutingAllowed=false, 	ouchesWal=false, and untimeArchiveWalkAllowed=false as hard boundaries. sourceFrozenFixturePath points at the previous versioned fixture for historical comparison. Archive root hints are documentation pointers only; the command does not walk the filesystem at runtime.

## Verification

This version rebuilds mini-kv, regenerates the rolling SHARDJSON fixture, runs the focused maintenance/command/shard/line-editor tests, runs full CTest, then verifies the command over CLI and owned TCP smoke. The owned TCP server is stopped after validation.

One sentence summary: v479 adds $(System.Collections.Hashtable.Scope) while preserving mini-kv's read-only/no-router/no-write/no-execution boundary.