# mini-kv Security Notes

## Scope

mini-kv is a local C++ storage and recovery learning system. It is useful for runtime evidence, WAL/snapshot behavior, and cross-project contract alignment, but it is not a hardened production database.

The default threat model is deliberately small:

- Single process.
- Local operator or local test runner.
- No authentication.
- No TLS.
- No replication.
- No multi-tenant isolation.
- No production secret storage.

Any deployment that needs remote exposure, user identity, transport encryption, audit storage, or managed credentials must put those controls outside mini-kv or add them as a separate reviewed design. The current system must not be described as a secure remote service by default.

## Trust Boundaries

The runtime commands split into three broad categories:

- Store mutation commands such as `SET`, `DEL`, `EXPIRE`, and `SETNXEX`.
- Administrative or persistence commands such as `SAVE`, `LOAD`, `COMPACT`, and WAL repair/compaction options.
- Read-only evidence commands such as `INFOJSON`, `STATSJSON`, `SMOKEJSON`, `STORAGEJSON`, `COMMANDSJSON`, `EXPLAINJSON`, and `CHECKJSON`.

Read-only evidence is meant for controllers, reviewers, and sibling projects. It does not grant authority to mutate state, route writes, open credentials, or execute restore/deployment behavior.

Boundary fields should be interpreted literally:

- `read_only=true` means the evidence object is a report, not an execution request.
- `execution_allowed=false` means the report must not be treated as approval to execute restore, load, compact, deployment, rollback, service startup, schema rehearsal, or managed audit behavior.
- `order_authoritative=false` means Java remains the order authority; mini-kv evidence cannot settle order state.
- `mutates_store=false` means a command contract or evidence command is not a store write.
- `touches_wal=false` means the command contract or evidence command does not append to WAL.
- `warnings`, `blockers`, and `diagnostics` are review signals; they are not bypass switches.

## WAL Integrity

The WAL protects local recovery behavior, not adversarial tamper resistance.

- New WAL records include FNV checksums.
- Replay remains compatible with older plain records.
- Replay reports applied, skipped, truncated, and checksum-failed records.
- Failed or truncated records are skipped forward so a damaged tail does not force all previous valid records to be discarded.

FNV checksums catch accidental corruption and format drift. They are not cryptographic signatures. If a future deployment needs tamper evidence, it should add a signed manifest or authenticated log chain as a separate contract.

## Snapshot Atomicity

Snapshots are written through a temporary file and then atomically replace the target when the platform allows it.

- Windows uses replace semantics through the platform move operation.
- POSIX-like environments use rename-style replacement.
- Corrupt snapshot loads are rejected without replacing the current in-memory store.

This protects local crash consistency around snapshot replacement. It does not protect against arbitrary filesystem compromise or hostile local users.

## TCP Exposure

The TCP server is intended for local development, smoke tests, and controlled demos.

- Binding to `127.0.0.1` is the safest default for local work.
- Binding to `0.0.0.0` exposes the server to the network and should be treated as unsafe without an external firewall and authentication layer.
- Request size limits prevent unbounded request buffers.
- Optional client idle timeout closes idle or partial-command sockets when explicitly configured.

The TCP protocol has no authentication or encryption. Do not place it directly on an untrusted network.

## Cross-Project Evidence Use

Node and Java may consume mini-kv evidence as read-only context. That consumption must preserve these boundaries:

- Node must not use mini-kv evidence as permission to start mini-kv.
- Node must not use mini-kv evidence as permission to open credentials or raw endpoints.
- Java remains the order authority.
- mini-kv does not become a managed audit store.
- Restore, load, compact, rollback, deployment, and production-window behavior require separate approval and are not authorized by evidence JSON.

When in doubt, treat mini-kv evidence as a signed-off checklist candidate only after a separate review confirms the exact command, environment, and operator approval path.
