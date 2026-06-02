# Walkthrough Markdown Generation Guardrail

## Guardrail

Future walkthrough generation must avoid PowerShell backtick interpolation inside Markdown text. A helper may still be used for repetitive evidence versions, but it should generate prose with plain text, explicit UTF-8 writes, and no accidental escape sequences.

## Checks

Before committing generated walkthroughs, run a hygiene check that detects:

- trailing whitespace
- non-newline control characters
- broken path tokens such as `fixtures` losing the leading `f`
- broken test paths such as `tests/` losing the leading `t`
- broken boundary fields such as `readOnly`, `routerActivationAllowed`, `trackedPostCloseoutReleaseCount`, or `nodeBatchCloseoutVersion`

## Engineering Judgment

This is a small process correction, not a change in architecture. The right response is to keep the useful version automation, tighten the text generation and checks, and continue the evidence run only after the generated records are clean.

## One-Line Value

The guardrail lets mini-kv continue moving quickly while keeping generated documentation reviewable, stable, and worthy of the same evidence chain as the code.
