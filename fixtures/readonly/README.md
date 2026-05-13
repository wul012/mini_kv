# Read-only evidence fixtures

This directory contains stable read-only sample responses for external control-plane smoke tests.

The package is intentionally narrow:

```text
CHECKJSON SET orderops:1 value
CHECKJSON GET orderops:1
INFOJSON
STATSJSON
```

`CHECKJSON` samples continue to live under `fixtures/checkjson/` because they are execution-contract fixtures. `fixtures/readonly/index.json` points at those existing samples and at the two runtime read samples in this directory.

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
```

Latency fields are runtime counters. This empty fixture intentionally uses zero counters so downstream control planes can validate the JSON shape and no-write boundaries without treating it as production pass evidence.

## Boundaries

These samples do not authorize operations:

```text
read_only=true
execution_allowed=false
write_execution_allowed=false
order_authoritative=false
```

They are shape evidence for Node v152/v153 preparation, not proof that production operations are ready.
