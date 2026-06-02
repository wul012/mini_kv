# Progress Direction: v248-v262 Node v549 Alignment

## Direction

The next mini-kv run should align with the current Node v549 plan: latest sibling live-smoke archive verification route archive verification. Node v549 verifies Node-owned archive files and does not require Java or mini-kv to start.

For mini-kv, that means the correct contribution is not a live integration path. The correct contribution is a fresh sequence of versioned, read-only SHARDJSON evidence that can be consumed by Node later without giving Node authority over mini-kv execution.

## Boundaries

The v248-v262 run should keep these boundaries unchanged:

- `readOnly=true`
- `executionAllowed=false`
- `routerActivationAllowed=false`
- `writeRoutingAllowed=false`
- no Java start
- no mini-kv service start outside local validation smoke
- no raw endpoint, credential value, deployment, rollback, restore, load, compact, or write-routing authority

## Version Shape

Each version should freeze the previous rolling fixture, update the current descriptor, rebuild, run targeted tests, run full CTest, run a local TCP smoke, archive five screenshots under `e/<version>/图片/`, write `e/<version>/解释/说明.md`, update the walkthrough index, commit, tag, push, and observe CI.

The first version should also switch the active post-closeout continuity evidence from Node v522 to Node v549 so the source plan matches the latest Node archive-verification direction.

## One-Line Value

v248-v262 should make mini-kv a clean, current, versioned evidence supplier for the Node v549 line without becoming a runtime dependency or execution participant.
