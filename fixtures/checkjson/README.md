# CHECKJSON fixtures

This directory contains stable sample responses for external control-plane smoke tests.

## set-orderops-write-contract.json

Command:

```text
CHECKJSON SET orderops:1 value
```

Runtime shape:

```text
No WAL path is configured, so wal.enabled=false and wal.durability=memory_only.
The command is read-only: read_only=true and execution_allowed=false.
```

The `command_tests` executable compares this file with the real `CHECKJSON` response so Node smoke tests can rely on the sample without silently drifting away from mini-kv output.
