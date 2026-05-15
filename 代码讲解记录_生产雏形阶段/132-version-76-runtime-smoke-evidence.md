# 第七十六版 Runtime Smoke Evidence

本版目标是把 mini-kv 从“多条只读命令可被控制面读取”，推进到“有一个稳定的只读聚合入口可被 Node v191 真实 TCP read adapter rehearsal 读取”。核心新增是 `SMOKEJSON`：它把 `INFOJSON`、`STORAGEJSON`、`HEALTH`、`STATSJSON` 这类只读运行信号的关键边界聚合成一个 JSON。

它不是什么也要讲清楚：`SMOKEJSON` 不是 restore 执行入口，不执行 `LOAD`、`COMPACT`、`SETNXEX` 或 restore，不启动 Node/Java，不读取生产密钥，也不让 mini-kv 成为 Java 订单权威。它只是一个真实运行时的只读观察面，方便后续 Node 在安全开关下读取。

## 本版所处阶段

跨项目计划 `v185-post-real-read-rehearsal-roadmap.md` 明确要求 mini-kv v76 学习 Java v66 的节奏：少堆静态 fixture，多做真实只读能力。v75 已经给出 restore boundary smoke manifest，但那仍偏“清单”。v76 开始提供一个可真实连 TCP 服务读取的运行时命令，作为 Node v191 real HTTP/TCP read adapter rehearsal 的输入。

## 1. `src/command.cpp`

`command_catalog` 新增：

```text
SMOKEJSON(category=meta, mutates_store=false, touches_wal=true)
```

这里把它归为 `meta`，因为它服务于控制面观察；`touches_wal=true` 表示它可能读取 WAL maintenance metadata，不代表会写 WAL。`explain_command()` 也把它标记为三类 side effects：

```text
metadata_read
store_read
wal_metadata_read_when_enabled
```

这和 `STORAGEJSON` 保持一致：控制面可以知道它会看运行状态、store 数量和 WAL 元数据，但不会修改数据。

核心格式化函数是 `format_smoke_json()`。它输出这些关键字段：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
evidence_type=runtime_smoke
version=0.76.0
real_read.commands=[INFOJSON, STORAGEJSON, HEALTH, STATSJSON]
real_read.forbidden_commands=[LOAD, COMPACT, SETNXEX, RESTORE]
write_commands_executed=false
admin_commands_executed=false
runtime_write_observed=false
```

控制面读到这些字段时，应该理解为：mini-kv 当前允许被真实读取，但只允许读取证据；遇到 `LOAD/COMPACT/SETNXEX/RESTORE` 仍必须停在解释、审批或人工边界之外，不能通过这个命令间接执行。

## 2. `src/line_editor.cpp`

客户端 Tab 补全加入 `SMOKEJSON`。这是体验配套，不改变协议，也不新增任何写行为。交互式客户端里输入 `SMOKEJ<Tab>` 可以补成 `SMOKEJSON`，方便本地 smoke 和后续人工验证。

## 3. `fixtures/release/runtime-smoke-evidence.json`

这个 fixture 是 v76 给 Node v191 的静态说明和验收清单。它声明：

```text
consumer_hint=Node v191 real HTTP/TCP read adapter rehearsal
default_enabled=false
node_may_read_when=mini-kv is already running and user enabled a safe real-read window
node_must_not=auto-start mini-kv, execute LOAD/COMPACT/SETNXEX/restore, or treat mini-kv as Java order authority
```

它和 `SMOKEJSON` 的关系是：fixture 描述“应该怎么消费”，命令提供“真实运行时读取结果”。这避免 Node 只依赖静态文件，也避免 mini-kv 越界变成执行面。

## 4. `fixtures/release/verification-manifest.json`

release verification manifest 升到 v76，构建目录切换为 `cmake-build-v76`，read-only smoke 主命令切换为：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

这里保留 `GET restore:real-read-token` 是为了证明本轮 smoke 没有执行 `SETNXEX`：如果 token 不存在，说明 read adapter rehearsal 没有偷偷制造运行时写入。

## 5. 测试保护

`command_tests.cpp` 保护三件事：

```text
COMMANDS / COMMANDSJSON 能暴露 SMOKEJSON 元数据
EXPLAINJSON SMOKEJSON 能说明只读 side effects
SMOKEJSON 本身返回 read_only / execution_allowed / forbidden_commands / no-write diagnostics
```

`runtime_smoke_evidence_tests.cpp` 是本版专项测试。它一边检查 `runtime-smoke-evidence.json` 的 Node v191 消费边界，一边用真实 `CommandProcessor` 执行 `SMOKEJSON`、`GET restore:real-read-token`、`CHECKJSON SMOKEJSON`。这组断言保护的是：新增入口可真实读取、不会执行写命令、不会把 mini-kv 标成 order authoritative。

`release_verification_manifest_tests.cpp` 则把 v76 manifest、CMake 版本、目标测试、`SMOKEJSON` smoke 命令和 no-restore 边界串起来，避免后续版本只改代码忘了更新交付清单。

## 6. 真实运行

本轮真实 smoke 启动：

```text
minikv_server 6416 127.0.0.1
minikv_client 127.0.0.1 6416 5000 --connect-retries 10 --retry-delay-ms 100
```

实际执行 `SMOKEJSON` 后看到 `version:"0.76.0"`、`read_only:true`、`execution_allowed:false`、`restore_execution_allowed:false`、`order_authoritative:false`，并且 `GET restore:real-read-token` 返回 `(nil)`。这证明 v76 是真实运行时只读入口，不是写入或恢复入口。

## 一句话总结

v76 把 mini-kv 给 Node 的证据从“静态清单 + 多条只读命令”推进到“一个可真实 TCP 读取的只读聚合入口”，为后续 Node v191 的默认关闭 real-read adapter rehearsal 做好 C++ 侧边界。
