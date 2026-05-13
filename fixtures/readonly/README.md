# Read-only evidence fixtures

This directory contains stable read-only sample responses for external control-plane smoke tests.

The package is intentionally narrow:

```text
CHECKJSON SET orderops:1 value
CHECKJSON GET orderops:1
INFOJSON
STATSJSON
runtime-read-field-guide.json
```

`CHECKJSON` samples continue to live under `fixtures/checkjson/` because they are execution-contract fixtures. `fixtures/readonly/index.json` points at those existing samples and at the two runtime read samples in this directory.
From v59 onward, `INFOJSON` and `STATSJSON` runtime responses carry their own `schema_version`, `read_only`, `execution_allowed`, `order_authoritative`, `evidence_type`, and `diagnostics` fields so a live probe can verify the no-write boundary from the response itself.
From v60 onward, `runtime-read-field-guide.json` explains how downstream control planes should interpret the stable fields, dynamic fields, and read-only smoke semantics without adding any new mini-kv server command.

## infojson-empty-inline.json

Command:

```text
INFOJSON
```

Runtime shape:

```text
empty inline processor
no WAL path
metrics disabled
live_keys=0
uptime_seconds=0 in the fixture test
read_only=true
execution_allowed=false
order_authoritative=false
evidence_type=runtime_identity
diagnostics.write_commands_executed=false
```

`uptime_seconds` is a dynamic runtime field. The test pins it to `0` by constructing a processor whose `started_at` is slightly in the future, so the sample stays stable while still comparing against the real command output.

## statsjson-empty-inline.json

Command:

```text
STATSJSON
```

Runtime shape:

```text
empty inline processor
no WAL path
no connection stats provider
captured before STATSJSON command accounting is recorded
read_only=true
execution_allowed=false
order_authoritative=false
evidence_type=runtime_metrics
diagnostics.write_commands_executed=false
```

Latency fields are runtime counters. This empty fixture intentionally uses zero counters so downstream control planes can validate the JSON shape and no-write boundaries without treating it as production pass evidence.

## runtime-read-field-guide.json

Purpose:

```text
explain INFOJSON fields
explain STATSJSON fields
explain read-only smoke semantics
support Node v159 release evidence review
```

The guide is explanatory evidence, not a runtime endpoint. It records how to read stable fields such as `schema_version`, `read_only`, `execution_allowed`, `order_authoritative`, `evidence_type`, `version`, `wal.enabled`, `commands.total_commands`, and `connection_stats.available`.

It also records dynamic fields that live capture should not compare exactly:

```text
server.uptime_seconds
commands.total_latency_ns
commands.avg_latency_ns
commands.max_latency_ns
commands.breakdown[*].*_latency_ns
```

The read-only smoke section allows:

```text
INFOJSON
STATSJSON
CHECKJSON GET orderops:1
HEALTH
QUIT
```

and states that `SET`, `DEL`, and `EXPIRE` are not used by this smoke.

## Boundaries

These samples do not authorize operations:

```text
read_only=true
execution_allowed=false
write_execution_allowed=false
order_authoritative=false
```

They are shape and explanation evidence for Node v159 release evidence review, not proof that production operations are ready.
