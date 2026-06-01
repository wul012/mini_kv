# mini-kv v208 node route catalog cleanup post-closeout archive verification route audit

## Goal and Boundary

v208 freezes the v207 archive verification route checkpoint and audits that checkpoint as a versioned post-closeout continuity artifact.

Latest Node plan checked: `docs/plans3/v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`. This remains mini-kv evidence only: no Node route, Java echo, approval chain, service launch, runtime probe, router, write path, or execution entry is opened.

## Frozen Source

`fixtures/release/shard-readiness-v207.json` is copied from the v207 rolling fixture before this version advances. Its digest is `fnv1a64:16dd9ba05e5b3fe4`, and it remains the source for `sourceFrozenReleaseVersion=v207`.

The v208 rolling fixture advances the continuity stage to `post-closeout-archive-verification-route-audit`, sequence `8`, and release `v208`.

## Control-Plane Meaning

The audit confirms the archive verification route checkpoint is pinned to a frozen v207 fixture with expected digest, fallback, range, and denial flags. It remains evidence metadata: no router, write routing, service startup, runtime probe, or execution authority is opened.

## Tests and Smoke

Focused CTest passed 2/2. Full CTest passed 71/71. TCP smoke on 127.0.0.1:6481 returned v208 with digest `fnv1a64:ef5973d3894665a6`, fallback v207, readOnly=True, router/write disabled, and executionAllowed=False; the server was stopped after verification.

One sentence: v208 audits the archive verification route evidence while keeping mini-kv strictly read-only.