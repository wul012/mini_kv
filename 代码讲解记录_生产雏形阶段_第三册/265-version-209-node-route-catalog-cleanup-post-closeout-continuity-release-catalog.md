# mini-kv v209 node route catalog cleanup post-closeout continuity release catalog

## Goal and Boundary

v209 freezes the v208 archive verification route audit and catalogs the post-closeout continuity release lane.

Latest Node plan checked: `docs/plans3/v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`. This remains mini-kv evidence only: no Node route, Java echo, approval chain, service launch, runtime probe, router, write path, or execution entry is opened.

## Frozen Source

`fixtures/release/shard-readiness-v208.json` is copied from the v208 rolling fixture before this version advances. Its digest is `fnv1a64:ef5973d3894665a6`, and it remains the source for `sourceFrozenReleaseVersion=v208`.

The v209 rolling fixture advances the continuity stage to `post-closeout-continuity-release-catalog`, sequence `9`, and release `v209`.

## Control-Plane Meaning

The catalog gives downstream readers a compact release-directory checkpoint for the post-closeout lane. It is still not an execution plan: the denial flags keep router activation, write routing, service start, runtime probe, LOAD/RESTORE/COMPACT, and execution closed.

## Tests and Smoke

Focused CTest passed 2/2. Full CTest passed 71/71. TCP smoke on 127.0.0.1:6482 returned v209 with digest `fnv1a64:6c283479e8bb1988`, fallback v208, readOnly=True, router/write disabled, and executionAllowed=False; the server was stopped after verification.

One sentence: v209 catalogs post-closeout continuity releases while mini-kv remains read-only evidence.