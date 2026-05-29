# mini-kv Code Walkthroughs - Production Prototype Stage, Volume 3

This sibling folder continues the mini-kv production-prototype walkthrough series after Volume 2 became crowded.

## Entries

- `200-version-144-shard-readiness-read-only-prototype.md` - mini-kv v144 read-only shard readiness prototype for Node v370, `SHARDJSON`, fixture parity, command catalog exposure, tests, TCP smoke, and `e/144` archive.
- `201-version-145-shard-readiness-hardening.md` - mini-kv v145 `SHARDJSON` hardening with command catalog posture, fixture parity, archived Node evidence compatibility, read-only boundary fields, tests, TCP smoke, and `e/145` archive.
- `202-version-146-shard-readiness-historical-fallback.md` - mini-kv v146 freezes the Node-consumed v145 shard readiness evidence, rolls current `SHARDJSON` to v146, adds historical fallback semantics, tests, TCP smoke, and `e/146` archive.
- `203-version-147-shard-readiness-active-prototype-plan.md` - mini-kv v147 freezes Node-consumed v146 shard readiness evidence, rolls current `SHARDJSON` to v147, adds prerequisite-only `activePrototypePlan`, tests, TCP smoke, and `e/147` archive.
- `204-version-148-shard-readiness-active-plan-freeze.md` - mini-kv v148 freezes v147 `activePrototypePlan` evidence, rolls current `SHARDJSON` to v148, adds `activePrototypePlanFreeze`, tests, TCP smoke, and `e/148` archive.
- `205-version-149-shard-readiness-consumer-handoff.md` - mini-kv v149 freezes v148 `activePrototypePlanFreeze` evidence, rolls current `SHARDJSON` to v149, adds frozen-evidence-only `consumerHandoff`, tests, TCP smoke, and `e/149` archive.

## Rules

- Continue file numbering from Volume 2.
- Keep each walkthrough focused on purpose, boundaries, module roles, tests, smoke evidence, and downstream value.
- Do not move historical walkthroughs from earlier walkthrough folders.
