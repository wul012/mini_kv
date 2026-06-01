# mini-kv v207 node route catalog cleanup post-closeout archive verification route checkpoint

## Goal and Boundary

v207 freezes the v206 verification-checklist audit and publishes an archive-verification-route checkpoint for the post-closeout continuity lane.

Latest Node plan checked: `docs/plans3/v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`. This remains mini-kv evidence only: no Node route, Java echo, approval chain, service launch, runtime probe, router, write path, or execution entry is opened.

## Frozen Source

`fixtures/release/shard-readiness-v206.json` is copied from the v206 rolling fixture before this version advances. Its digest is `fnv1a64:1e5f3dc941b1a90e`, and it remains the source for `sourceFrozenReleaseVersion=v206`.

The v207 rolling fixture advances the continuity stage to `post-closeout-archive-verification-route-checkpoint`, sequence `7`, and release `v207`.

## Control-Plane Meaning

The checkpoint says the Node v490 archive verification route lane can read a frozen mini-kv source fixture, but it cannot infer router activation, write routing, service startup, runtime probe, or execution permission from that evidence.

## Tests and Smoke

Focused CTest passed 2/2. Full CTest passed 71/71. TCP smoke on 127.0.0.1:6480 returned v207 with digest `fnv1a64:16dd9ba05e5b3fe4`, fallback v206, readOnly=True, router/write disabled, and executionAllowed=False; the server was stopped after verification.

One sentence: v207 checkpoints the archive verification route evidence while mini-kv stays a read-only witness.