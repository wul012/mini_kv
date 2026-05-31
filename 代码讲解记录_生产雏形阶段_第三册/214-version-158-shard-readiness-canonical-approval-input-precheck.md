# mini-kv v158: canonical approval input precheck

v158 follows the Node v403 plan. Node v403 consumed mini-kv v157 as template compatibility evidence, but the three real canonical approval input files are still missing. mini-kv therefore freezes v157 and publishes a read-only canonical input precheck.

This version is not runtime approval and not a live-read gate. It does not write canonical Node input files, copy templates into canonical paths, start Java, start mini-kv on the declared approval port, run runtime probes, enable active shard routing, write storage, execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`, or make mini-kv order/audit authoritative.

## Split

`src/shard_readiness.cpp` was close to a large-file risk point, so v158 moves approval-input JSON helpers into `src/shard_readiness_approval_inputs.cpp` with declarations in `include/minikv/shard_readiness_approval_inputs.hpp`.

The split is intentionally small. `src/shard_readiness.cpp` still owns the top-level `SHARDJSON` contract, digest composition, historical fixture chain, and field ordering. The new helper file owns only final-input freeze, template echo, template echo freeze, and canonical approval input precheck formatting.

## New Evidence

`runtimeExecutionApprovalInputTemplateValidatorEchoFreeze` freezes v157. It records that the v157 template echo had three template-only inputs, no canonical runtime input, no shared approval correlation id, no template-as-approval, no canonical file writes, no Java/mini-kv start, no router, no write, and no execution.

`runtimeExecutionCanonicalApprovalInputPrecheck` is the new v158 object. It points at Node v403 compatibility intake and the canonical root `e/398/input`. It declares the three required canonical files and keeps `presentCanonicalInputCount=0`, `missingCanonicalInputCount=3`, `canonicalApprovalInputsComplete=false`, and `sharedApprovalCorrelationIdValidated=false`.

Control-plane readers should treat this object as a fail-closed blocker. Template compatibility evidence can help validate shape later, but it is not approval. Runtime remains blocked until real canonical inputs exist, share one approval correlation id, contain concrete approval values, keep smoke commands GET-only, and require cleanup proof after an approved start.

## Fixture

`fixtures/release/shard-readiness-v157.json` stores the frozen v157 template-validator echo with digest `fnv1a64:1d8cdf7f597f837e`.

The current `fixtures/release/shard-readiness.json` is generated from the real `minikv_cli.exe` `SHARDJSON` path and has digest `fnv1a64:bce4629123167bc6`.

## Tests

`tests/shard_readiness_tests.cpp` now reads historical fixtures through v157 and asserts that v157 remains the frozen template echo baseline. It also checks the v158 canonical precheck: three required canonical paths, 0/3 present, no shared correlation validation, no template compatibility evidence accepted as approval, no runtime packet, no mini-kv start, no router, no write, and no execution.

`tests/command_tests.cpp` verifies the same contract through the real `CommandProcessor` `SHARDJSON` path.

The validation loop includes CMake configure, fixture regeneration, targeted `command_tests` + `shard_readiness_tests`, full 71-test CTest, and TCP smoke on a temporary port. TCP smoke reads v158 `SHARDJSON`, confirms the canonical precheck fields, confirms a GET probe stays `(nil)`, and confirms the declared approval port `6424` never becomes LISTENING.

In one sentence: v158 turns the absence of real canonical approval inputs into versioned, test-covered C++ evidence while keeping template evidence safely non-executable.
