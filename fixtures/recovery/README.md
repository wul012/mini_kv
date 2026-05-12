# Recovery fixtures

`restart-recovery-evidence.json` is a stable mini-kv restart recovery evidence sample.

The sample models this flow:

```text
1. Save a manual snapshot with recovery:base.
2. Append one WAL record for recovery:delta.
3. Simulate restart by loading the snapshot and replaying the WAL.
4. Compare before/after key count and store digest.
```

The fixture is intentionally not a new server command and not a write permission model. It is a read-only evidence shape for downstream control-plane smoke tests:

```text
read_only=true
execution_allowed=false
recovered=true
digests_match=true
diagnostics.order_authoritative=false
diagnostics.write_commands_executed_by_evidence=false
```
