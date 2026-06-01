# mini-kv v205 node route catalog cleanup post-closeout verification checklist handoff

## Goal and Boundary

v205 freezes the v204 archive-readiness audit and publishes a verification-checklist handoff for the post-closeout continuity lane.

Latest Node plan checked: `docs/plans3/v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`. This remains mini-kv evidence only: no Node route, Java echo, approval chain, service launch, runtime probe, router, write path, or execution entry is opened.

## Frozen Source

`fixtures/release/shard-readiness-v204.json` is copied from the v204 rolling fixture before this version advances. Its digest is `fnv1a64:670b62f7c203b814`, and it remains the source for `sourceFrozenReleaseVersion=v204`.

The v205 rolling fixture advances the continuity stage to `post-closeout-verification-checklist-handoff`, sequence `5`, and release `v205`.

## Control-Plane Meaning

The handoff says later consumers can read a frozen verification-checklist checkpoint without trusting rolling current. Denial fields remain false for router activation, write routing, service start, runtime probe, LOAD/RESTORE/COMPACT, and execution.

## Tests and Smoke

Focused CTest passed 2/2. Full CTest passed 71/71. TCP smoke on 127.0.0.1:6478 returned v205 with digest fnv1a64:c00dd62f28564fed, fallback v204, readOnly=True, router/write disabled, and executionAllowed=False; the server was stopped after verification.

One sentence: v205 gives the Node v490-facing lane a frozen checklist handoff while mini-kv stays read-only evidence.
