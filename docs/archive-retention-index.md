# mini-kv Archive Retention Index

This index is the K5 retention ledger for the path-stable archive roots in this repository. It records measured sizes after v1620 and turns the existing policy into an operator-facing reference.

Historical archive roots are evidence, not cleanup candidates. Node-side governance already has hardcoded references to mini-kv archive paths and release fixtures, so every existing path below remains stable unless a separate owner-approved archival migration plan is written and reviewed.

## Root Inventory

| Root | Version range | Files | Bytes | MiB | Ceiling MiB | Marker |
|---|---:|---:|---:|---:|---:|---|
| `a/` | 1-57 | 461 | 21,367,665 | 20.38 | 64 | FROZEN |
| `b/` | 58-62 | 45 | 3,812,703 | 3.64 | 16 | FROZEN |
| `c/` | 63-121 | 336 | 31,130,191 | 29.69 | 64 | FROZEN |
| `d/` | 122-143 | 152 | 13,506,894 | 12.88 | 64 | FROZEN |
| `e/` | 144-1596 | 5,153 | 1,065,416,900 | 1,016.06 | 1,200 | FROZEN HIGH-VOLUME |
| `f/` | 1597-1620 | 99 | 3,406,988 | 3.25 | 128 | ACTIVE ROOT; EXISTING VERSION DIRECTORIES FROZEN |

Latest measured version archive:

| Version archive | Files | Bytes | MiB | Budget MiB | State |
|---|---:|---:|---:|---:|---|
| `f/1620/` | 5 | 129,713 | 0.12 | 8 | within budget |

## Retention Rules

- Do not move, rename, delete, compress-replace, or rebalance historical archive roots `a/` through `f/`.
- Do not move or rewrite `fixtures/release/*.json`.
- Do not replace old PNG screenshots with newly compressed or re-rendered files.
- Do not put build binaries, raw temporary logs, `_render/`, `_logs/`, or local build directories into final version archives.
- For future versions, keep screenshots selective: configuration, build, focused tests, full tests when relevant, script output, and real smoke output only when it proves a behavior boundary.
- For long command outputs, render the asserted portions and key boundary fields in the screenshot page instead of archiving multi-megabyte raw output.
- If an active root becomes crowded, create the next sibling root only after updating AGENTS.md and this index; never migrate historical content as part of an ordinary version.

## Automated Inventory

`scripts/archive_inventory.py` is the source-of-truth measurement helper for future checks. It uses only the Python standard library and prints per-root sizes plus the newest version archive size.

Default warning budgets:

| Scope | Warning threshold |
|---|---:|
| Newest version archive | 8 MiB |
| `a/` | 64 MiB |
| `b/` | 16 MiB |
| `c/` | 64 MiB |
| `d/` | 64 MiB |
| `e/` | 1,200 MiB |
| `f/` | 128 MiB |

CI runs the inventory in warning mode. A warning is an operator signal, not a failed build; path stability remains the higher priority.
