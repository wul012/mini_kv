# mini-kv Testing Guide

## Strategy

mini-kv tests are intentionally layered. The suite protects both normal C++ runtime behavior and the evidence contracts consumed by downstream tools.

The core layers are:

- Unit-style command, store, WAL, snapshot, RESP, TCP, client, and metrics tests.
- Fixture and release evidence tests that keep JSON shape, boundary fields, and historical receipt chains stable.
- CLI and TCP smoke tests that prove real binaries can expose the expected behavior.
- CI contract tests that prevent workflow, coverage, sanitizer, format, archive-inventory, and dependency-maintenance gates from silently weakening.

The current default CTest inventory is 353. The original K5/K6 suite has since gained executable orientation, OSFS, receipt-parity/census, documentation-honesty, E1-E10 final-evidence, OSFS resilience, and test-architecture contracts. The count is a navigation aid rather than a quality claim; the focused behavior and failure condition of each gate remain the evidence.

The test-architecture contract scans all test translation units and enforces a 1,000-nonblank-line ceiling with an empty current waiver baseline. Command and shard-readiness suites compile from versioned manifests; every part entry must have one definition and one call in the suite graph. Exact assertion-token censuses, plus explicit dual shard fixture/runtime wrapper calls, make omitted parts and accidental migration shrinkage fail mechanically. The same contract parses all 344 `minikv_add_*_test` registrations, requires every internal target over 40 characters to belong to the 277-entry shrink-only baseline, and caps `target.length + source_path.length` at the exact current maximum of 197. Public CTest names stay independent from shorter private build identities; injected new-long-name, stale-baseline, and over-budget registrations prove all three failure paths.

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

- Linux, macOS, and Windows CMake build plus CTest.
- Ubuntu sanitizer lane.
- Ubuntu coverage lane with a 90% floor for core modules, tightened in v1658 from the fresh 2345-line / 2122-executed / 90% CI result.
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

The tested state should be described as single-project validation plus cross-project contract alignment unless there is real integration evidence. Valid integration evidence would be a command or test proving Node actually consumes mini-kv runtime output, Java actually preserves order authority, and no side project can trigger mini-kv writes, restore/load/compact execution, service startup, credential reads, deployment, rollback, or managed audit writes.

Until that exists, mini-kv evidence remains a local C++ proof surface prepared for downstream review, not a live four-project integration guarantee.
