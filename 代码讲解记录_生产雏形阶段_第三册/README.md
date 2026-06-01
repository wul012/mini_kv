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
- `213-version-157-shard-readiness-approval-input-template-echo.md` - mini-kv v157 freezes v156 final input evidence, rolls current `SHARDJSON` to v157, adds Node v402 approval-input template validator echo, tests, TCP smoke, and `e/157` archive.
- `214-version-158-shard-readiness-canonical-approval-input-precheck.md` - mini-kv v158 freezes v157 template echo evidence, splits approval-input formatters, rolls current `SHARDJSON` to v158, adds canonical approval input precheck, tests, TCP smoke, and `e/158` archive.
- `215-version-159-shard-readiness-node-route-group-split-compatibility.md` - mini-kv v159 freezes v158 canonical input precheck evidence, adds Node v418 route-group split compatibility evidence, tests, TCP smoke, and `e/159` archive.
- `216-version-160-shard-readiness-boundary-catalog-split.md` - mini-kv v160 freezes v159 route split compatibility evidence, splits the read-only boundary field catalog out of the main `SHARDJSON` formatter, tests, TCP smoke, and `e/160` archive.
- `217-version-161-shard-readiness-boundary-catalog-index.md` - mini-kv v161 freezes v160 boundary catalog split evidence, adds a shared-vector boundary catalog index/count, tests, TCP smoke, and `e/161` archive.
- `218-version-162-shard-readiness-slot-table-preview.md` - mini-kv v162 freezes v161 boundary catalog index evidence, adds a read-only 16-slot table preview, splits slot/shard preview formatting, tests, TCP smoke, and `e/162` archive.
- `219-version-163-shard-readiness-slot-table-preview-audit.md` - mini-kv v163 freezes v162 slot preview evidence, adds a read-only slot table consistency audit, tests, TCP smoke, and `e/163` archive.
- `220-version-164-shard-readiness-slot-preview-audit-maintenance.md` - mini-kv v164 freezes v163 slot preview audit evidence, splits the audit formatter, adds read-only maintenance evidence, tests, TCP smoke, and `e/164` archive.
- `221-version-165-shard-readiness-release-catalog.md` - mini-kv v165 freezes v164 audit-maintenance evidence, adds a versioned shard-readiness release catalog, tests, TCP smoke, and `e/165` archive.
- `222-version-166-shard-readiness-release-catalog-audit.md` - mini-kv v166 freezes v165 release catalog evidence, adds a read-only release catalog consistency audit, tests, TCP smoke, and `e/166` archive.
- `223-version-167-shard-readiness-node-route-split-window.md` - mini-kv v167 freezes v166 release catalog audit evidence, adds Node v433-v440 route-split compatibility window evidence, tests, TCP smoke, and `e/167` archive.
- `224-version-168-shard-readiness-node-route-split-window-v441.md` - mini-kv v168 freezes v167 route-split compatibility evidence, extends the read-only Node route-split window through v441, tests, TCP smoke, and `e/168` archive.
- `225-version-169-shard-readiness-node-route-split-window-audit.md` - mini-kv v169 freezes v168 route-split window evidence, adds a read-only route-split window consistency audit, tests, TCP smoke, and `e/169` archive.
- `226-version-170-shard-readiness-node-route-split-window-v442.md` - mini-kv v170 freezes v169 route-split window audit evidence, extends the read-only Node route-split window through v442, tests, TCP smoke, and `e/170` archive.
- `227-version-171-shard-readiness-node-route-split-window-v444-refactor.md` - mini-kv v171 freezes v170 route-split window evidence, extends the read-only Node route-split window through v444, centralizes route split metadata, tests, TCP smoke, and `e/171` archive.
- `228-version-172-shard-readiness-node-route-split-window-v446.md` - mini-kv v172 freezes v171 route-split window evidence, extends the read-only Node route-split window through v446, tests, TCP smoke, and `e/172` archive.
- `229-version-173-shard-readiness-node-route-split-window-v447.md` - mini-kv v173 freezes v172 route-split window evidence, extends the read-only Node route-split window through v447, tests, TCP smoke, and `e/173` archive.
- `230-version-174-shard-readiness-node-route-split-window-v448.md` - mini-kv v174 freezes v173 route-split window evidence, extends the read-only Node route-split window through v448, tests, TCP smoke, and `e/174` archive.
- `231-version-175-shard-readiness-node-route-split-window-v449.md` - mini-kv v175 freezes v174 route-split window evidence, extends the read-only Node route-split window through v449, tests, TCP smoke, and `e/175` archive.
- `232-version-176-shard-readiness-node-route-split-window-v457.md` - mini-kv v176 freezes v175 route-split window evidence, extends the read-only Node route-split window through v457, tests, TCP smoke, and `e/176` archive.
- `233-version-177-shard-readiness-node-route-split-window-v458.md` - mini-kv v177 freezes v176 route-split window evidence, extends the read-only Node route-split window through v458, tests, TCP smoke, and `e/177` archive.
- `234-version-178-shard-readiness-route-split-window-audit-hardening.md` - mini-kv v178 freezes v177 route-split window evidence, computes route window contiguity and duplicate audit fields from the shared version vector, tests, TCP smoke, and `e/178` archive.
- `235-version-179-shard-readiness-route-split-window-numeric-span-audit.md` - mini-kv v179 freezes v178 route split window audit evidence, adds numeric span audit fields, advances the boundary catalog to v10, tests, TCP smoke, and `e/179` archive.
- `236-version-180-shard-readiness-history-formatter-split.md` - mini-kv v180 freezes v179 numeric span audit evidence, splits shard-readiness history/fixture/archive formatters, adds history maintenance evidence, tests, TCP smoke, and `e/180` archive.

## Rules

- Continue file numbering from Volume 2.
- Keep each walkthrough focused on purpose, boundaries, module roles, tests, smoke evidence, and downstream value.
- Do not move historical walkthroughs from earlier walkthrough folders.
