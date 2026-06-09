# mini-kv v1171-v1195 production live capture non-participation

## Version Goal

This batch turns the Node v1607-v1631 production live probe read-only window live capture module split into a mini-kv side evidence chain, but only as non-participation evidence.

The source for the chain is mini-kv v1170. v1170 already closed `SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON` and proved the value-supply profile-section integrity fixture was complete. v1171 freezes that source. v1172-v1194 record each Node live-capture split step as sibling context. v1195 closes the chain with a single command and one embedded `SHARDJSON` section proving that mini-kv did not participate in Node live capture.

This is not a new live-capture implementation. It is not a production probe runner, not a route into Node, not a reader for live endpoints, not a credential resolver, not a raw endpoint parser, not a service launcher, and not an active shard router. It does not allow write routing, does not touch WAL, and does not execute restore/load/compact behavior.

The practical purpose is to give Node/Java/mini-kv a stable evidence boundary: Node may split and test its own live-capture modules, while mini-kv publishes a versioned receipt that says "this C++ side observed the plan as read-only maintenance context and stayed out of the runtime path."

## Main Command

The new command is:

```text
SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON
```

Its contract is:

```text
shard-route-preview-production-live-capture-non-participation.v1
```

The command is exposed through the normal command catalog, JSON catalog, help output, and line editor completion. It is also embedded into top-level `SHARDJSON` as:

```json
"shardPreviewProductionLiveCaptureNonParticipation": { ... }
```

The command output is intentionally declarative. It does not inspect Node files at runtime, does not start a server, does not walk screenshot archives, and does not call sibling projects. The Node plan path appears as a source label, not as an execution dependency:

```text
docs/plans3/v1631-production-live-probe-read-only-window-live-capture-module-split-roadmap.md
```

## Source Freeze

The first guard is source continuity:

```json
"sourceIntegrityCommand": "SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON",
"sourceIntegrityReleaseVersion": "v1170",
"sourceIntegrityFixturePath": "fixtures/release/shard-readiness-v1170.json",
"sourceIntegrityPublishedStageCount": 10,
"sourceIntegrityComplete": true
```

Control-plane readers should treat this as the anchor for the whole chain. mini-kv v1171-v1195 does not invent a fresh source. It freezes the already-complete v1170 integrity fixture, then builds the live-capture non-participation chain forward from that fixture. If the source count or source completion flag changed, validation would fail before the v1195 evidence could be treated as complete.

## Module Split

The implementation is deliberately split instead of being appended into a large single formatter file.

`include/minikv/shard_preview_production_live_capture_non_participation.hpp` and `src/shard_preview_production_live_capture_non_participation.cpp` own the top-level evidence formatter. This layer assembles the command JSON, pulls in the current stage, references the source integrity command, emits global boundary flags, and wires the stage catalog, check catalog, and validation block together.

`include/minikv/shard_preview_production_live_capture_non_participation_stages.hpp` and `src/shard_preview_production_live_capture_non_participation_stages.cpp` own the 25-stage release catalog. The catalog maps v1171-v1195 to contiguous stage records, each with a source frozen release and source frozen fixture path. This is where the version chain is made auditable.

`include/minikv/shard_preview_production_live_capture_non_participation_checks.hpp` and `src/shard_preview_production_live_capture_non_participation_checks.cpp` own the 25 non-participation checks. Each check has a sequence, a check code, a source label, and a short `protects` statement. The formatter then applies the same false boundary fields to every check: no Node execution, no renderer execution, no catalog import, no live endpoint read, no raw endpoint parse/store, no credential/secret read, no runtime payload import, no active router, no write routing, no WAL touch, and no execution.

`include/minikv/shard_preview_production_live_capture_non_participation_validation.hpp` and `src/shard_preview_production_live_capture_non_participation_validation.cpp` own the validation summary. This prevents the large evidence formatter from also carrying the pass/fail math. The validation block checks three things: v1170 source integrity is locked, live capture remains closed, and counts are aligned at 25 planned checks and 25 published stages.

This split matters because the project has been accumulating long evidence formatters. The batch adds a full 25-version chain, but keeps the extension maintainable by separating stage data, check data, and validation logic.

## Stage Chain

The stage chain is a versioned fixture story:

```text
v1171 freezes v1170
v1172 freezes v1171
...
v1195 freezes v1194
```

The JSON exposes the chain through `stageCatalog` and `stageChain`. The important control fields are:

```json
"publishedStageCount": 25,
"plannedStageCount": 25,
"firstReleaseVersion": "v1171",
"lastPublishedReleaseVersion": "v1195",
"publishedCountWithinCatalog": true,
"publishedCountMatchesPlan": true,
"sequencesContiguous": true,
"releaseVersionsContiguous": true,
"sourceFrozenReleaseVersionsContiguous": true,
"sourceFrozenFixturePathsContiguous": true,
"chainComplete": true
```

These fields say that the chain is not a loose list of strings. It has ordered releases, contiguous source freezes, and an explicit published count. Older fixtures v1171-v1194 remain versioned release fixtures, while `fixtures/release/shard-readiness.json` now points at the final v1195 state.

## Node Context

Node v1607-v1631 split the production live probe read-only window live-capture implementation into smaller type, builder, and Markdown renderer modules. The mini-kv evidence records that context, but does not consume Node runtime behavior.

The command makes this distinction explicit:

```json
"sourceNodeRequiresFreshMiniKvEvidence": false,
"sourceNodeRequiresFreshJavaEvidence": false,
"sourceNodeStartsMiniKvService": false,
"sourceNodeStartsJavaService": false,
"sourceNodeRuntimeBehaviorAdded": false,
"sourceNodeModuleSplitOnly": true,
"sourceNodeHttpSmokeIsNodeOnly": true
```

Those fields are the reason this mini-kv batch is safe to advance independently. Node's HTTP smoke and module tests are Node-owned. mini-kv does not become a live-capture data source and does not approve or block Node code by executing anything.

## Boundary Fields

The most important false fields are repeated at the command top level, inside every check, and inside validation:

```json
"nodeLiveCaptureExecuted": false,
"nodeRendererExecuted": false,
"nodeCatalogImported": false,
"miniKvStartsLiveCapture": false,
"miniKvReadsRawEndpoint": false,
"miniKvServiceStarted": false,
"javaServiceStarted": false,
"siblingServiceStarted": false,
"liveEndpointRead": false,
"rawEndpointParsed": false,
"rawEndpointStored": false,
"credentialValueRead": false,
"secretValueRead": false,
"runtimePayloadImported": false,
"runtimePayloadAccepted": false,
"productionLiveReadAllowed": false,
"probeWindowOpened": false,
"activeRouterInstalled": false,
"writeRoutingAllowed": false,
"loadRestoreCompactAllowed": false,
"touchesWal": false,
"executionAllowed": false
```

For a control plane, these fields mean the evidence is safe to read but not safe to reinterpret as authority. `readOnly=true` means the command can be consumed as a receipt. `executionAllowed=false` means the receipt cannot be used as a trigger. `writeRoutingAllowed=false` and `touchesWal=false` mean the shard preview did not become a storage mutation path. `credentialValueRead=false`, `secretValueRead=false`, `rawEndpointParsed=false`, and `rawEndpointStored=false` mean the evidence names endpoint and credential boundaries without ever handling sensitive values.

## Command Wiring

`src/command.cpp` dispatches the new command and enforces the no-argument usage rule. This keeps it consistent with previous read-only evidence commands: the caller asks for a fixed receipt, not for parameterized runtime behavior.

`src/command_contracts.cpp` marks the command as a stable read command with no store mutation and no WAL touch. That matters for `COMMANDS`, `COMMANDSJSON`, and downstream command catalog tests.

`src/line_editor.cpp` adds completion for the command. This is not behavior by itself, but it prevents the CLI surface from drifting: if a command is public and stable, the interactive shell should know it.

`src/shard_readiness.cpp` advances the top-level release to v1195, embeds the new section, and moves the rolling current status to `route-preview-production-live-capture-non-participation-closeout-summary-read-only`. The older v1170 section still remains as source evidence; it is not overwritten.

## Fixtures

The fixture set now includes:

```text
fixtures/release/shard-readiness-v1171.json
...
fixtures/release/shard-readiness-v1195.json
fixtures/release/shard-readiness.json
```

Each versioned fixture corresponds to the published stage count for that release. The final rolling fixture is copied from v1195. Tests assert that the current fixture carries v1195 and that the source integrity fixture remains v1170 where expected.

The source freeze design is important: v1195 can mention v1170 as the source and v1194 as the immediate frozen fixture. That gives two different kinds of continuity. v1170 is the immutable prior completed evidence chain; v1194 is the immediate predecessor in the current live-capture non-participation chain.

## Tests

The focused tests cover four layers.

`production_live_capture_non_participation_stages_tests` verifies the stage catalog: count, first/last release, contiguous releases, contiguous source freezes, and read-only stage fields.

`production_live_capture_non_participation_checks_tests` verifies the check catalog: 25 checks, Node source labels, source fixture freeze, and repeated false non-participation flags.

`production_live_capture_non_participation_validation_tests` verifies the validation math: v1170 source count must be 10 and complete, Node must not require fresh mini-kv evidence, mini-kv must not start live capture or read raw endpoints, and 25/25 counts must align.

`production_live_capture_non_participation_tests` verifies the final command JSON: contract, command name, Node v1631 context, source v1170 fixture, release v1195, chain completion, module split declaration, and no live capture/no router/no write/no WAL/no execution flags.

`production_live_capture_non_participation_fixture_tests` verifies the generated fixtures, including the final rolling `fixtures/release/shard-readiness.json`.

Existing command, line editor, shard readiness, and profile-section fixture tests were updated so the public surface and rolling current fixture agree on v1195 while preserving older historical meanings.

## Runtime Smoke

The CLI smoke runs the real `minikv_cli.exe` and reads:

```text
SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON
SHARDJSON
QUIT
```

That proves the interactive binary returns the new command receipt and that top-level `SHARDJSON` embeds the new section.

The TCP smoke starts an owned loopback `minikv_server.exe` only for the normal mini-kv protocol path, queries the new read-only command through `minikv_client.exe`, then stops the owned server. This server startup is not a live-capture startup. It is the same mini-kv TCP verification path used to prove the command is reachable over the product protocol and remains read-only.

Both smoke paths assert the same core fields: v1195 release, v1170 source integrity fixture, no fresh mini-kv evidence requirement, no live capture start, no raw endpoint read, no write routing, no WAL touch, and no execution.

## Why This Helps Later Integration

This batch gives Node a clean companion receipt for a module-split cycle where no fresh C++ evidence was required. That sounds small, but it matters for long-term cross-project discipline. The receipt prevents a maintenance-only Node refactor from being misread as an integration event, and it prevents mini-kv from quietly becoming a live endpoint or credential participant.

For future Node/Java/mini-kv alignment, the value is a stable negative contract: mini-kv can publish evidence about what it did not do, with enough structure that downstream control planes can distinguish "safe to read" from "allowed to execute."

One-line summary: v1171-v1195 turns Node live-capture module-split context into versioned mini-kv non-participation evidence while keeping live reads, raw endpoints, credentials, routers, writes, WAL, and execution outside the C++ boundary.
