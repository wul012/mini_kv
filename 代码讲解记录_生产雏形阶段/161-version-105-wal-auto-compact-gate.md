# mini-kv v105 WAL auto compact gate

本版目标是处理 WAL 写路径里的一个真实质量点：原来的 `execute_with_wal()` 用一个全局 WAL command mutex 包住 WAL append、store mutation 和后续 `auto_compact_wal_if_needed()`。这样安全，但自动 compact 会在正常写命令临界段里做磁盘 IO。

本版不是改存储语义，不是开放新的写能力，不是改变 runtime evidence，也不是让 compaction 可以随意和写入并发。WAL 的关键安全约束仍然是：一条写命令必须先 append WAL，再改 Store，而且这个 append+mutation 的顺序不能被别的写命令或 compact 插入。

## 入口与结构

`src/command.cpp` 新增了一个小的 `WalCommandGate`：

- `enter_command()` 用于普通 WAL-backed command，保护 append + mutation 这个短区间。
- `enter_compaction()` 用于 `COMPACT` 和自动 compact，明确标记这是 compaction scope。
- `Scope` 是 RAII，异常或提前返回时也能释放 gate。

这比原来的单个 mutex 更明确：正常写命令的关键区间只覆盖 WAL record 生成、append 和 Store mutation；`auto_compact_wal_if_needed()` 在这个短区间之后运行。

## 为什么不能完全放开并行

看起来可以简单把 compact 移到锁外，但那会有数据风险：

1. 写命令 B 已经 append 到 WAL，但还没 mutation。
2. 自动 compact 在中间 snapshot Store。
3. compact 用旧 Store 状态重写 WAL。
4. B 再完成 mutation。

这样 B 的 WAL record 可能被 compact 覆盖掉，运行内存状态和 WAL replay 结果就会分叉。所以 v105 的做法是“缩短普通写命令关键区间”，而不是把 compaction 与 append/mutation 的安全边界完全拆开。

## 测试

原有 WAL 和 command 回归继续覆盖：

- SET / SETNXEX / DEL / EXPIRE 的 WAL append-before-mutation 行为。
- auto compact 的 `auto_compactions`、`records_removed`、`bytes_saved` 等统计。
- `WALINFO` / `HEALTH` 响应形状。

本版还在 `tests/wal_tests.cpp` 补了并发回归：4 个线程共享一个 `CommandProcessor` 写同一个 key，触发多次 auto compact，最后断言：

- 没有死锁。
- WAL 最终压到 1 条记录。
- auto compaction 发生过。
- 重新 replay WAL 后的 Store 值等于真实运行中的最终值。

## 归档

- 截图：`c/105/图片/01-cmake-configure.png`、`02-cmake-build.png`、`03-ctest.png`、`04-runtime-smoke.png`
- 说明：`c/105/解释/说明.md`

## 一句话总结

v105 把 WAL 写命令的安全边界表达得更精确：append+mutation 仍然原子，compaction 不再混在普通写命令临界段里，同时 replay 正确性和只读证据链都保持不变。
