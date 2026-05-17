# mini-kv v100 current runtime fixture rolling guard

This version exists to make one cross-project rule explicit: the current mini-kv runtime fixture may roll with each release, but the historical consumed receipt digests that Node has already relied on must not drift. v100 is for Node v235 manual sandbox connection precondition intake.

It is not a new execution entry point, not a command dispatch table migration, not a restore/load/compact path, not credential access, not schema rehearsal, not managed audit storage, and not Java order authority. The version keeps the same read-only control-plane posture: `read_only=true`, `execution_allowed=false`, `restore_execution_allowed=false`, and `order_authoritative=false`.

## Release Fixture

`fixtures/release/current-runtime-fixture-rolling-guard.json` is the new v100 contract sample. It records the current release identity (`0.100.0`, `v100`, `c/100/`, `mini-kv-live-read-v100`) and the current digest set for binary provenance, retention provenance, replay marker, restore boundary, non-authoritative storage, command dispatch quality, adapter shell, external adapter, sandbox adapter, sandbox connection echo, and no-start guard.

The same fixture also lists the historical anchors that must remain stable: v84 retention check, v85 restore boundary marker, v86 non-authoritative storage receipt, v87 command dispatch receipt, v88 adapter shell receipt, v89 external adapter receipt, v90 sandbox adapter receipt, v95 sandbox connection echo consumed receipt, and v96 no-start consumed marker. That distinction is the whole point of the version: current values roll; consumed history stays fixed.

## Manifest And Runtime Evidence

`fixtures/release/verification-manifest.json` now registers `minikv_current_runtime_fixture_rolling_guard_tests`, adds the guard fixture to the release fixture inventory, and adds `current_runtime_fixture_rolling_guard` metadata for Node v235.

`fixtures/release/runtime-smoke-evidence.json` also points to the guard fixture and keeps the runtime read target at `SMOKEJSON`, `INFOJSON`, `STORAGEJSON`, `HEALTH`, and `GET restore:real-read-token`. It remains read-only evidence. Node may use it to verify preconditions, but not as permission to connect, read credentials, run schema rehearsal, write managed audit state, restore, or auto-start mini-kv/Java/Node.

The C++ runtime receipt formatters in `src/runtime_evidence_receipts.cpp` and `src/managed_audit_receipts.cpp` only roll current version/path/digest hints to v100. They do not add write behavior, change admin behavior, or alter WAL/snapshot/restore semantics.

## Test Coverage

`tests/current_runtime_fixture_rolling_guard_tests.cpp` is the focused executable guard. It checks three layers:

- the new guard fixture contains v100 current evidence and all historical consumed digest anchors;
- the release manifest and runtime smoke fixture both publish the guard and Node v235 consumption hint;
- a real in-process `SMOKEJSON` response still exposes v100 current evidence, stable historical consumed digests, and no-execution boundaries.

The test also verifies `GET restore:real-read-token` remains `(nil)`, which keeps the restore-boundary smoke token from being created as a side effect.

The wider suite still protects the older contracts: `runtime_smoke_evidence_tests` covers failure taxonomy, live-read session, managed audit receipts, and no-start guard fields; `release_verification_manifest_tests` covers release inventory and targeted tests; `command_tests` and the WAL tests continue to protect write-command behavior and v99 execute-with-wal regression coverage.

## Verification

The v100 validation chain was CMake configure, CMake build, full CTest, and real TCP smoke. CTest passed 33/33 after adding the new guard test. The real smoke started `minikv_server` on `127.0.0.1:6500`, ran read-only client commands, observed `0.100.0` and `mini-kv-live-read-v100`, verified `GET restore:real-read-token` returned `(nil)`, and stopped the server.

One-sentence summary: v100 gives Node v235 a stable way to consume rolling mini-kv current evidence without losing the historical digest anchors that keep the managed-audit sandbox chain honest and non-authoritative.
