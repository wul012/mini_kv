# mini-kv Testing Guide

## Strategy

mini-kv tests are intentionally layered. The suite protects both normal C++ runtime behavior and the evidence contracts consumed by downstream tools.

The core layers are:

- Unit-style command, store, WAL, snapshot, RESP, TCP, client, and metrics tests.
- Fixture and release evidence tests that keep JSON shape, boundary fields, and historical receipt chains stable.
- CLI and TCP smoke tests that prove real binaries can expose the expected behavior.
- CI contract tests that prevent workflow, coverage, sanitizer, format, archive-inventory, and dependency-maintenance gates from silently weakening.

The current default CTest inventory is 354. The original K5/K6 suite has since gained executable orientation, OSFS, receipt-parity/census, documentation-honesty, E1-E10 final-evidence, OSFS resilience, test-architecture, and atomic-file contracts. The count is a navigation aid rather than a quality claim; the focused behavior and failure condition of each gate remain the evidence.

The test-architecture contract scans all test translation units and enforces a 1,000-nonblank-line ceiling with an empty current waiver baseline. Command and shard-readiness suites compile from versioned manifests; every part entry must have one definition and one call in the suite graph. Exact assertion-token censuses, plus explicit dual shard fixture/runtime wrapper calls, make omitted parts and accidental migration shrinkage fail mechanically. The same contract parses all 345 `minikv_add_*_test` registrations, requires every internal target over 40 characters to belong to the 277-entry shrink-only baseline, and caps `target.length + source_path.length` at the exact current maximum of 197. Public CTest names stay independent from shorter private build identities; injected new-long-name, stale-baseline, and over-budget registrations prove all three failure paths.

## Test Link Topology

Ordinary builds set `MINIKV_BUNDLE_TESTS=ON` by default. The 342 tests that link `minikv` remain separate object targets, preserving their source lists, private include paths, compile definitions, and suite-part manifests. A stable SHA-256 prefix maps each public CTest name to one of 8 runner executables; each CTest invocation starts a fresh runner process and dispatches exactly one named entry. The three standalone test programs and nine direct CMake-script tests remain outside the bundle, so the public inventory stays at 354.

The architecture contract pins 342 linked cases, 8 shards, 344 no-argument test entries with explicit `return 0`, and the runner's missing-argument and unknown-case red paths. Default and legacy `ctest -N` inventories must contain the same 354 names in the same order. This is link deduplication, not in-process batch execution.

Use `-DMINIKV_BUNDLE_TESTS=OFF` when a debugger or local workflow specifically needs one executable per test. In a fresh build directory, `MINIKV_SANITIZE=ON` and `MINIKV_COVERAGE=ON` default to that legacy topology so gcov sidecars and sanitizer image initialization retain their existing isolation. A reused cache may still contain bundle ON; that combination, or an explicit instrumented-plus-bundle request, is a configuration error rather than a silent precedence rule. Set bundle OFF explicitly or keep instrumented lanes in separate build directories.

Receipt structure has two independent protection layers. The owner census requires 27 builder-backed formatter owners, zero pending owners, and one named no-formatter waiver. Its implementation census scans the receipt `.cpp` files and private receipt support headers, then requires exactly 613 manual `fields.push_back(...)` calls and one canonical `receipt_digest(...)` wrapper. A count above the baseline is regression; a count below it is also a failure until the same change tightens the checked-in baseline. Deliberate 612/614 manual-push probes and 0/2 digest-wrapper probes protect both red paths, so future cleanup cannot claim progress while leaving a stale floor behind or delete a genuinely shared helper merely to reach zero.

Atomic persistence has a separate private-infrastructure layer. `atomic_file_tests` drives successful replacement, second-commit rejection, abandoned and bad-stream cleanup, parent creation/failure, replace failure, unique temporary paths, and a source-ownership contract that rejects copied platform/temp helpers in `wal.cpp` or `snapshot.cpp`. The existing WAL and Snapshot tests then prove integration behavior, count-output stability, format compatibility, and zero temp-file leakage. CI coverage includes `atomic_file_writer.cpp` in the same 90% core floor as Store, command, WAL, Snapshot, and RESP.

## Fixture And Evidence Tests

Fixture tests protect historical contracts. They should prefer structured checks when the fixture is JSON or table-like, and string-fragment assertions only when the output is intentionally plain text or the risk does not justify a parser.

Evidence tests should keep these questions visible:

- Does the report remain read-only?
- Does it avoid write routing?
- Does it avoid WAL touch unless the command is explicitly a WAL-backed mutation?
- Does it avoid restore/load/compact execution?
- Does it keep Java order authority separate from mini-kv storage evidence?
- Does it avoid Node, Java, or sibling repository runtime execution?

When fixture expectations change, the test should explain whether the change is a real contract advance, a version rollover, or a maintenance-only refactor. Silent fixture churn is a risk.

## TCP And Runtime Tests

TCP tests use real sockets so they can catch behavior that pure command tests miss:

- Inline text and RESP request handling.
- Connection accepted/closed accounting.
- Active, total, and peak connection metrics.
- Request-size rejection.
- Idle and partial-command timeout closure when configured.
- Sequential-connection cleanup so detached client threads do not leak active sockets.

The timeout/limit test intentionally uses a short configured timeout. The default server option keeps idle timeout disabled so historical local clients and existing smoke tests are not surprised by a new default disconnect policy.
The server host default is independently asserted as `127.0.0.1`; tests that need `localhost`, an ephemeral port, or another bind target set it explicitly.

## CI Lanes

GitHub Actions should keep these signals visible:

- Linux, macOS, and Windows CMake build plus CTest using the default 8-runner link topology.
- Ubuntu sanitizer lane using the legacy per-test-executable topology.
- Ubuntu coverage lane using the legacy per-test-executable topology with a 90% floor for core modules, tightened in v1658 from the fresh 2345-line / 2122-executed / 90% CI result.
- Changed-file clang-format gate.
- Archive inventory warning lane.

The coverage lane still carries `--gcov-ignore-parse-errors negative_hits.warn_once_per_file` because the post-K4 retest showed gcov/gcovr negative-hit parser output still fails in `src/command_catalog.cpp`. That workaround is a documented toolchain deviation, not a permission to ignore test failures.

## Not Tested By This Project

mini-kv currently does not claim coverage for:

- Multi-process write coordination.
- Network partition recovery.
- TLS/authentication.
- Production secret handling.
- Remote hostile network exposure.
- Replication or cluster failover.
- Java order execution.
- Node service startup, deployment, rollback, or production-window orchestration.

Those behaviors require separate system-level tests if the project ever chooses to implement them.

## Cross-Project Alignment

The mini-kv CI suite remains single-project validation. Separately, the reviewed env-gated capstone starts a fresh real `minikv_cli` process from Node and consumes `SMOKEJSON` plus `CHECKJSON` while Java retains order authority and the no-write/no-restore boundaries are asserted. That evidence supports only the repository's stated label: verified read-only cross-project integration on one machine, with no execution authority.

It does not establish production deployment, multi-host behavior, automatic service startup, credential reads, rollback execution, managed-audit writes, or permission for Node/Java to trigger mini-kv writes, restore, load, or compact. Those claims still require separate system-level evidence.
