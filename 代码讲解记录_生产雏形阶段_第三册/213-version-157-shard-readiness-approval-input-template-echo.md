# mini-kv v157: shard-readiness approval input template echo

v157 follows the Node v402 plan. Node v402 publishes machine-checkable templates for three still-missing runtime approval inputs, but those templates are not the real canonical approval files. mini-kv therefore does the C++ side of the same boundary: it freezes v156 as the final mini-kv approval-gate input evidence, then rolls current `SHARDJSON` forward with a read-only template-validator echo.

This version is not a runtime approval, not a router switch, not a live-read gate, and not a write path. It does not create Node canonical input files, copy templates into canonical paths, start Java, start mini-kv on the declared approval port, run a runtime probe, enable active shard routing, write storage, execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`, or make mini-kv order/audit authoritative.

## Implementation Entry

`src/shard_readiness.cpp` remains the only implementation entry for `SHARDJSON`. v157 updates `releaseVersion` to `v157` and `status` to `runtime-execution-approval-input-template-validator-echo-read-only`.

The historical fixture list now includes `fixtures/release/shard-readiness-v156.json`. `archiveCompatibility` preserves Node v370-v402, and `historicalFallback` points to v156 with the meaning that Node v401 consumed it as final mini-kv approval input completion evidence. That keeps rolling current evidence from replacing the frozen baseline.

## New Fields

`miniKvFinalApprovalGateInputFreeze` freezes v156. It preserves the final mini-kv input as complete, including the declared loopback host/port, GET-only smoke command, service owner, and cleanup rules. The same object keeps the important runtime guards false: cleanup proof is absent, runtime gate approval is absent, the packet is not executable, Java/mini-kv service start is false, probe/live-read/router/write/execution are false, and the frozen baseline is not rolling current.

`runtimeExecutionApprovalInputTemplateValidatorEcho` mirrors Node v402 template semantics. It records the three template archive paths under `e/402/input-templates/` and the three canonical target paths under `e/398/input/`. The control plane should read this as: templates exist for validation shape only; canonical runtime input is still absent.

The echo explicitly keeps `templateCopiedToCanonicalInput=false`, `sharedApprovalCorrelationIdPresent=false`, `templatesAuthorizeRuntime=false`, `templateDigestAcceptedAsApproval=false`, `writesCanonicalApprovalInputFiles=false`, `changesNodeInputTemplateFiles=false`, `runtimeExecutionPacketExecutable=false`, `startsJavaService=false`, `startsMiniKvService=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, and `executionAllowed=false`.

## Fixture

`fixtures/release/shard-readiness-v156.json` stores the full v156 final mini-kv approval-gate input, with digest `fnv1a64:f240d1fe1b25fab7`.

The current `fixtures/release/shard-readiness.json` is regenerated from the real `minikv_cli.exe` `SHARDJSON` path and has digest `fnv1a64:1d8cdf7f597f837e`.

## Tests

`tests/shard_readiness_tests.cpp` checks the current v157 contract, reads all historical fixtures through v156, and asserts that v156 remains the frozen final mini-kv input baseline. It also verifies the new template echo fields: three templates, three canonical targets, no canonical input, no shared correlation id, no template-as-approval, no Node template mutation, no router, no write, and no execution.

`tests/command_tests.cpp` checks the real `CommandProcessor` path for the same `SHARDJSON` evidence. This protects the command surface, not only the direct formatter.

The validation loop for this version includes CMake configure, fixture regeneration, targeted `command_tests` + `shard_readiness_tests`, full 71-test CTest, and TCP smoke on a temporary port. The TCP smoke reads `SHARDJSON`, verifies v157/template fields, confirms a GET probe remains `(nil)`, and confirms the declared approval port `6424` never becomes LISTENING.

In one sentence: v157 lets Node/Java/mini-kv agree on the exact template-only approval-input boundary while keeping actual runtime execution locked behind real canonical inputs and operator correlation.
