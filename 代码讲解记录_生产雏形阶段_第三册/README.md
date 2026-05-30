# mini-kv Code Walkthroughs - Production Prototype Stage, Volume 3

This sibling folder continues the mini-kv production-prototype walkthrough series after Volume 2 became crowded.

## Entries

- `200-version-144-shard-readiness-read-only-prototype.md` - mini-kv v144 read-only shard readiness prototype for Node v370, `SHARDJSON`, fixture parity, command catalog exposure, tests, TCP smoke, and `e/144` archive.
- `201-version-145-shard-readiness-hardening.md` - mini-kv v145 `SHARDJSON` hardening with command catalog posture, fixture parity, archived Node evidence compatibility, read-only boundary fields, tests, TCP smoke, and `e/145` archive.
- `202-version-146-shard-readiness-historical-fallback.md` - mini-kv v146 freezes the Node-consumed v145 shard readiness evidence, rolls current `SHARDJSON` to v146, adds historical fallback semantics, tests, TCP smoke, and `e/146` archive.
- `203-version-147-shard-readiness-active-prototype-plan.md` - mini-kv v147 freezes Node-consumed v146 shard readiness evidence, rolls current `SHARDJSON` to v147, adds prerequisite-only `activePrototypePlan`, tests, TCP smoke, and `e/147` archive.
- `204-version-148-shard-readiness-active-plan-freeze.md` - mini-kv v148 freezes v147 `activePrototypePlan` evidence, rolls current `SHARDJSON` to v148, adds `activePrototypePlanFreeze`, tests, TCP smoke, and `e/148` archive.
- `205-version-149-shard-readiness-consumer-handoff.md` - mini-kv v149 freezes v148 `activePrototypePlanFreeze` evidence, rolls current `SHARDJSON` to v149, adds frozen-evidence-only `consumerHandoff`, tests, TCP smoke, and `e/149` archive.
- `206-version-150-shard-readiness-live-read-gate-plan.md` - mini-kv v150 freezes v149 `consumerHandoff` evidence, rolls current `SHARDJSON` to v150, adds read-only `liveReadGatePlan` prerequisites, tests, TCP smoke, and `e/150` archive.
- `207-version-151-shard-readiness-service-lifecycle-template.md` - mini-kv v151 freezes v150 `liveReadGatePlan` evidence, rolls current `SHARDJSON` to v151, adds template-only `operatorServiceLifecycleTemplate`, tests, TCP smoke, and `e/151` archive.
- `208-version-152-shard-readiness-declared-lifecycle-evidence.md` - mini-kv v152 freezes v151 `operatorServiceLifecycleTemplate` evidence, rolls current `SHARDJSON` to v152, adds declared operator lifecycle evidence without runtime approval, tests, TCP smoke, and `e/152` archive.
- `209-version-153-shard-readiness-runtime-execution-artifact-preflight.md` - mini-kv v153 freezes v152 declared lifecycle evidence, rolls current `SHARDJSON` to v153, adds blocked 0-of-6 runtime execution artifact intake preflight, tests, TCP smoke, and `e/153` archive.
- `210-version-154-shard-readiness-mini-kv-runtime-execution-candidate.md` - mini-kv v154 freezes v153 blocked preflight evidence, rolls current `SHARDJSON` to v154, adds mini-kv-side runtime execution artifact candidates without approval or runtime start, tests, TCP smoke, and `e/154` archive.
- `211-version-155-shard-readiness-approval-gate-input-precheck.md` - mini-kv v155 freezes v154 candidate-only evidence, rolls current `SHARDJSON` to v155, adds blocked 0-of-3 approval gate input precheck, tests, TCP smoke, and `e/155` archive.
- `212-version-156-shard-readiness-final-mini-kv-approval-input.md` - mini-kv v156 freezes v155 precheck-only evidence, rolls current `SHARDJSON` to v156, adds final mini-kv approval gate input while global runtime remains blocked, tests, TCP smoke, and `e/156` archive.

## Rules

- Continue file numbering from Volume 2.
- Keep each walkthrough focused on purpose, boundaries, module roles, tests, smoke evidence, and downstream value.
- Do not move historical walkthroughs from earlier walkthrough folders.
