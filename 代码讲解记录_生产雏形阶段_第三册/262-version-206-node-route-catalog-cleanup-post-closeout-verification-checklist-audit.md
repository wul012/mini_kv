# mini-kv v206 node route catalog cleanup post-closeout verification checklist audit

## Goal and Boundary

v206 freezes the v205 verification-checklist handoff and audits that handoff as a versioned post-closeout continuity checkpoint.

Latest Node plan checked: `docs/plans3/v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`. This remains mini-kv evidence only: no Node route, Java echo, approval chain, service launch, runtime probe, router, write path, or execution entry is opened.

## Frozen Source

`fixtures/release/shard-readiness-v205.json` is copied from the v205 rolling fixture before this version advances. Its digest is `fnv1a64:c00dd62f28564fed`, and it remains the source for `sourceFrozenReleaseVersion=v205`.

The v206 rolling fixture advances the continuity stage to `post-closeout-verification-checklist-audit`, sequence `6`, and release `v206`.

## Control-Plane Meaning

The audit tells a downstream reader that the verification-checklist handoff is not just present, but pinned to a frozen v205 fixture with the expected fallback, source digest, post-closeout range, and denial flags. The control plane should still read this as evidence metadata only, not as approval to call a router, write path, service lifecycle, or runtime execution packet.

## Tests and Smoke

Focused CTest passed 2/2. Full CTest passed 71/71. TCP smoke on 127.0.0.1:6479 returned v206 with digest `fnv1a64:1e5f3dc941b1a90e`, fallback v205, readOnly=True, router/write disabled, and executionAllowed=False; the server was stopped after verification.

One sentence: v206 turns the v205 checklist handoff into audited frozen evidence while mini-kv stays a read-only witness.
