# mini-kv v200 command archive

## 01-cmake-configure-build.png

Command: configure and build `cmake-build-v200` with the CLion bundled CMake, Ninja, and MinGW runtime on PATH.

Result: configuration completed and all v200 binaries were rebuilt from the updated sources.

Meaning: the closeout-audit formatter, boundary catalog update, frozen v199 fixture, and test changes were verified from a fresh build.

## 02-shardjson-fixture.png

Command: run `minikv_cli.exe`, issue `SHARDJSON`, write `fixtures/release/shard-readiness.json`, and summarize the generated fixture.

Result: the fixture reports `releaseVersion=v200`, status `node-route-catalog-cleanup-evidence-batch-closeout-audit-read-only`, digest `fnv1a64:d1e889711b5d8574`, audit source `v199`, boundary catalog `read-only-boundary-fields.v28`, 800 fields, 39 groups, `readOnly=True`, and `executionAllowed=False`.

Meaning: the rolling fixture now records a read-only audit of the v194-v198 cleanup evidence batch closeout without adding runtime authority.

## 03-targeted-ctest.png

Command: run focused CTest for `command_tests` and `shard_readiness_tests`.

Result: both targeted suites passed.

Meaning: the public command contract, frozen v199 fixture parity, batch closeout audit section, route/catalog window, and boundary catalog counts are covered before broader regression testing.

## 04-full-ctest.png

Command: run the full CTest suite from `cmake-build-v200`.

Result: all 71 tests passed.

Meaning: v200 did not regress existing storage, WAL, snapshot, restore, TCP, command, retention, or release-evidence behavior.

## 05-tcp-smoke.png

Command: start `minikv_server.exe` on `127.0.0.1:6473`, request `SHARDJSON` through `minikv_client.exe`, summarize the response, and stop the server.

Result: TCP smoke returned v200 with the same read-only batch closeout audit, `routerActivationAllowed=False`, `writeRoutingAllowed=False`, `executionAllowed=False`, and the server was stopped after verification.

Meaning: the externally visible TCP command path matches the fixture and still opens no write, router, admin, LOAD, RESTORE, COMPACT, or execution behavior.

## Process Notes

The first TCP smoke wrapper attempt used the same file for server stdout and stderr; PowerShell rejected that before starting a process. The smoke was rerun with separate stdout/stderr files and passed.

Playwright MCP produced the PNG screenshots from temporary local HTML pages served by a short-lived `python -m http.server` process. That render server was stopped after screenshots were captured.
