# v1331-v1360 catalog entry group split non-participation follow-up

This version range closes a follow-up evidence chain after v1330. The point is not to create a new catalog endpoint, route, registry, storage backend, or execution path. It keeps the previous catalog entry group split evidence consumable as versioned mini-kv metadata while proving that mini-kv still does not read Node catalog endpoints, import Node catalog records, render Node Markdown, execute Node tests, write WAL, install a router, or start sibling services.

The latest Node plan inspection found `docs/plans3/v1821-approval-decision-prerequisite-gate-split-roadmap.md`. That plan says Java and mini-kv may continue independently and do not need fresh sibling evidence for the Node maintainability split. This mini-kv batch therefore stays inside C++ ownership: it only advances its own read-only evidence chain and does not modify Node or Java.

The implementation keeps the existing four-part module split:

- `src/shard_preview_catalog_entry_group_split_non_participation.cpp` formats the public JSON, status, digest marker, stage-chain report, and validation block.
- `src/shard_preview_catalog_entry_group_split_non_participation_stages.cpp` owns the v1331-v1360 stage catalog. Stage 1 freezes `fixtures/release/shard-readiness-v1330.json`; stage 30 closes on v1360.
- `src/shard_preview_catalog_entry_group_split_non_participation_checks.cpp` owns the 30 follow-up check records and the JSON formatting for each check.
- `src/shard_preview_catalog_entry_group_split_non_participation_validation.cpp` keeps the validation rule small: source v1295 must remain locked, catalog split actions must stay closed, and the 30 planned/published counts must align.

For control-plane readers, the important fields are:

- `readOnly=true`
- `executionAllowed=false`
- `activeRouterInstalled=false`
- `catalogRouterInstalled=false`
- `writeRoutingAllowed=false`
- `touchesWal=false`
- `miniKvStartsCatalogSplit=false`
- `miniKvConsumesCatalogRecords=false`
- `miniKvReadsCatalogEndpoint=false`
- `freshMiniKvEvidenceRequiredByNode=false`

The tests protect three layers. Stage tests prove the v1331-v1360 range is contiguous and that every stage freezes the previous versioned fixture. Check tests prove the follow-up check catalog has exactly 30 entries and cannot be rendered past its boundary. Validation tests prove the report only passes when source lock, side-effect closure, and counts all hold at once. Command and SHARDJSON tests prove the current public output now reports v1360 while preserving the same no-router/no-write/no-execution contract.

The fixture chain will be regenerated after the build so `fixtures/release/shard-readiness.json` and `fixtures/release/shard-readiness-v1331.json` through `fixtures/release/shard-readiness-v1360.json` match the compiled formatter. CLI and TCP smoke then prove the same contract through real command paths, not just direct formatter calls.

In one sentence: v1331-v1360 turns the v1330 catalog split evidence into a follow-up current chain without letting mini-kv become a Node catalog consumer, router, writer, WAL participant, or executor.
