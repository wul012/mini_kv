# mini-kv v106 command dispatch table

本版目标是把 `src/command.cpp` 顶层命令分发从长 `if (command == "...")` 链改成一张本地 dispatch table 加 `CommandDispatchVerb` switch。它是质量优化，不是功能扩张。

这不是新命令，不改 parser，不改响应文本，不改 WAL / snapshot / runtime evidence 行为，不打开 LOAD / COMPACT / RESTORE / SETNXEX 之外的新路径，也不改变 Node / Java 的只读边界。

## 入口

`execute_trimmed()` 仍然是命令执行入口。变化在入口前半段：

- `command_dispatch_table` 负责把命令名映射到 `CommandDispatchVerb`。
- `lookup_command_dispatch_verb()` 做命令查找。
- `execute_trimmed()` 通过 `switch` 进入各个命令分支。

这样以后检查支持了哪些命令时，先看 dispatch table 就够了；不需要从一串长 if 链里人工追。

## 保留不动的行为

本版故意没有把每个 handler 拆到新文件，因为那会让 v106 从“分发质量优化”膨胀成更大的模块拆分。各命令内部逻辑保持原位：

- `SET`、`SETNXEX`、`DEL`、`EXPIRE` 仍走 `execute_with_wal()`。
- `COMPACT` 仍走 v105 的 compaction gate。
- `STATS`、`SMOKEJSON`、`INFOJSON`、`COMMANDSJSON` 等 runtime evidence 命令仍走 `execute_runtime_evidence_command()`。
- `EXPLAINJSON` / `CHECKJSON` 仍从 `command_contracts` 生成只读契约。
- `EXIT` / `QUIT` 仍返回 `BYE`，带额外参数时继续返回原来的 usage 形状。

## 测试与真实运行

验证分三层：

1. CMake configure/build：确认 dispatch table 引入 `<array>` 后所有目标正常编译链接。
2. CTest：`35/35` 全过，覆盖 command、WAL、TCP、fixture、runtime evidence、restore/release 证据。
3. 真实 smoke：启动 server，用 client 依次执行 `PING dispatch`、`SET route value`、`GET route`、`KEYSJSON route`、`COMMANDSJSON`、`HEALTH`、`QUIT`，确认 meta/read/write/runtime evidence/session 多类命令都能通过新 dispatch 路径。

## 归档

- 截图：`c/106/图片/01-cmake-configure.png`、`02-cmake-build.png`、`03-ctest.png`、`04-runtime-smoke.png`
- 说明：`c/106/解释/说明.md`

## 一句话总结

v106 让 command routing 从长条件链变成可审计的表驱动入口，保持所有外部行为不变，同时让后续命令维护更清楚。
