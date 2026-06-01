# mini-kv v201 command archive

## 01-cmake-configure-build.png

Command: configure and build `cmake-build-v201` with the CLion bundled CMake, Ninja, and MinGW runtime on PATH.

Result: configuration completed and all v201 binaries were rebuilt from the updated sources.

Meaning: the new post-closeout continuity formatter module, boundary catalog update, frozen v200 fixture, and tests were verified from a fresh build.

## 02-shardjson-fixture.png

Command: run `minikv_cli.exe`, issue `SHARDJSON`, write `fixtures/release/shard-readiness.json`, and summarize the generated fixture.

Result: the fixture reports `releaseVersion=v201`, status `node-route-catalog-cleanup-post-closeout-continuity-read-only`, digest `fnv1a64:9a3abb5ab3aaeb1c`, stage `post-closeout-continuity-ledger-intake`, boundary catalog `read-only-boundary-fields.v29`, 821 fields, 40 groups, `readOnly=True`, and `executionAllowed=False`.

Meaning: the rolling fixture now records the first post-closeout continuity checkpoint from the frozen v200 audit fixture without adding runtime authority.

## 03-targeted-ctest.png

Command: run focused CTest for `command_tests` and `shard_readiness_tests`.

Result: both targeted suites passed.

Meaning: the public command contract, frozen v200 fixture parity, continuity section, route/catalog window, and boundary catalog counts are covered before broader regression testing.

## 04-full-ctest.png

Command: run the full CTest suite from `cmake-build-v201`.

Result: all 71 tests passed.

Meaning: v201 did not regress existing storage, WAL, snapshot, restore, TCP, command, retention, or release-evidence behavior.

## 05-tcp-smoke.png

Command: start `minikv_server.exe` on `127.0.0.1:6474`, request `SHARDJSON` through `minikv_client.exe`, summarize the response, and stop the server.

Result: TCP smoke returned v201 with the same read-only post-closeout continuity evidence, `routerActivationAllowed=False`, `writeRoutingAllowed=False`, `executionAllowed=False`, and the server was stopped after verification.

Meaning: the externally visible TCP command path matches the fixture and still opens no write, router, admin, LOAD, RESTORE, COMPACT, or execution behavior.

## Process Notes

Playwright MCP produced the PNG screenshots from temporary local HTML pages served by a short-lived `python -m http.server` process. That render server was stopped after screenshots were captured.
