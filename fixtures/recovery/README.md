# Recovery fixtures

`index.json` is the stable recovery fixture index for downstream control-plane smoke tests. It lists the available recovery evidence fixture, its relative path, evidence version, digest, expected recovery status, restart/replay sample cost, retention boundary, and operational boundaries.

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

The index keeps those boundaries visible before a downstream tool opens the concrete fixture:

```text
read_only=true
execution_allowed=false
order_authoritative=false
consumer_hint=Node v107 production readiness summary v4
```

The index also explains the current restart/replay cost and retention boundary:

```text
snapshot_loads=1
wal_records_replayed=1
expected_replay_complexity=O(snapshot_keys + wal_records)
measured_in_fixture_only=true
production_retention_policy=not_defined
backup_or_rotation_policy=not_included
```

These fields keep the sample honest: it proves a small restart recovery path, not a production retention or backup policy.
