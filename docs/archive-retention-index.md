# mini-kv Archive Retention Index

This index is the K5 retention ledger for the path-stable archive roots in this repository. It records the original v1620 baseline and the v1657 post-OSFS remeasurement, turning the existing policy into an operator-facing reference.

Historical archive roots are evidence, not cleanup candidates. Node-side governance already has hardcoded references to mini-kv archive paths and release fixtures, so every existing path below remains stable unless a separate owner-approved archival migration plan is written and reviewed.

## Root Inventory

| Root | Version range | Files | Bytes | MiB | Ceiling MiB | Marker |
|---|---:|---:|---:|---:|---:|---|
| `a/` | 1-57 | 461 | 21,367,665 | 20.38 | 64 | FROZEN |
| `b/` | 58-62 | 45 | 3,812,703 | 3.64 | 16 | FROZEN |
| `c/` | 63-121 | 336 | 31,130,191 | 29.69 | 64 | FROZEN |
| `d/` | 122-143 | 152 | 13,506,894 | 12.88 | 64 | FROZEN |
| `e/` | 144-1596 | 5,153 | 1,065,416,900 | 1,016.06 | 1,200 | FROZEN HIGH-VOLUME |
| `f/` | 1597-1657 | 299 | 10,584,325 | 10.09 | 128 | ACTIVE ROOT; EXISTING VERSION DIRECTORIES FROZEN |

Latest measured version archive:

| Version archive | Files | Bytes | MiB | Budget MiB | State |
|---|---:|---:|---:|---:|---|
| `f/1657/` | 6 | 82,874 | 0.08 | 8 | within budget |

The table above is the direct output of `python scripts/archive_inventory.py` on 2026-07-10 after the six v1657 evidence files were added. The script emitted `warnings: none`; no historical path was moved or rewritten. The immediately preceding v1656 baseline (293 files / 10,501,451 bytes) is retained in `f/1657/证据/summary.txt` and `03-archive-growth.png` with an explicit pre-v1657 timestamp scope.

## OSFS Coursework Growth Accounting

The OSFS implementation and final-package work happened after the original K5 snapshot, so v1657 measures it explicitly instead of leaving the active-root numbers frozen at v1620.

| Version archive | Files | Bytes | MiB | Evidence role |
|---|---:|---:|---:|---|
| `f/1629/` | 6 | 118,841 | 0.11 | independent OSFS baseline |
| `f/1630/` | 6 | 95,833 | 0.09 | first draft package evidence |
| `f/1631/` | 3 | 177,458 | 0.17 | persistent users and MFD/UFD |
| `f/1632/` | 3 | 211,286 | 0.20 | descriptor offsets and range I/O |
| `f/1633/` | 6 | 1,020,857 | 0.97 | substantive coursework package |
| `f/1634/` | 5 | 374,288 | 0.36 | single-indirect block evidence |
| `f/1635/` | 6 | 508,472 | 0.48 | FSCK and user administration |
| `f/1636/` | 7 | 933,181 | 0.89 | refreshed final package evidence |
| **`f/1629-1636` total** | **42** | **3,440,216** | **3.28** | OSFS implementation and delivery interval |

`课程设计交付/` is a submission-artifact tree rather than an `a/`-`f/` screenshot root, so `archive_inventory.py` intentionally does not merge it into the root totals. A separate read-only filesystem measurement on 2026-07-10 found **102 files / 5,445,971 bytes / 5.19 MiB**. The authoritative current package is `课程设计交付/v1636-osfs-final/`; older v1630 and v1633 packages remain path-stable superseded history. This accounting is descriptive only and does not authorize deletion, renaming, or compression replacement.

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
