# mini-kv v98 execute-with-wal helper

本版目标是把 `SET`、`SETNXEX`、`DEL`、`EXPIRE` 里重复的 WAL / no-WAL 分支收敛到 `CommandProcessor::execute_with_wal`。它不是新的命令入口，不改变 public command 契约，不改变 WAL record 格式，不绕过 append-before-mutation 顺序，不触碰 snapshot / restore / admin，不给 Node 或 Java 增加执行权限，也不让 mini-kv 变成 order authority。

为什么做这一步：前几版已经把 receipt formatter、runtime evidence formatter、command contract formatter 和 response formatter 从 `command.cpp` 里拆出去了，`command.cpp` 已经从大文件压力中退下来。v98 继续做的是更小颗粒的质量重构：把写命令里“有 WAL 先 append，再 mutation，再 auto compact；无 WAL 直接 mutation”的重复流程收成一个内部 helper，让后续维护 WAL 写路径时少改多个分支。

## 实现位置

`include/minikv/command.hpp` 新增私有 helper 声明：

```cpp
CommandResult execute_with_wal(std::function<std::optional<std::string>()> wal_record,
                               std::function<CommandResult()> mutation);
```

这里用 `std::optional<std::string>` 表达“这个写命令在当前状态下是否真的需要追加 WAL”。`SET` 永远会生成 record；`SETNXEX`、`DEL`、`EXPIRE` 在 key 已存在或不存在等 no-op 场景下可以返回 `std::nullopt`。

`src/command.cpp` 实现 helper。无 WAL 时直接运行 `mutation()`；有 WAL 时先拿 `wal_command_mutex()`，再调用 `wal_record()` 判断是否要写记录。若没有 record，直接返回 `"0"`；若有 record，先 `wal_->append()`，失败则返回 `wal_error()`，成功后再执行 mutation 并调用 `auto_compact_wal_if_needed()`。

这个顺序很关键：v98 没有把 mutation 提前到 WAL append 之前，也没有让 no-op miss 产生空 WAL 记录。也就是说，原来靠 WAL 恢复保障的写入顺序仍然成立，原来 `SETNXEX` 重复 claim、`DEL missing`、`EXPIRE missing` 返回 `0` 的语义也仍然成立。

## 四个调用点

`SET` 的 lambda 总是生成 `SET key value`，mutation 继续调用 `store_.set()` 并返回 `OK inserted` 或 `OK updated`。

`SETNXEX` 先解析正 TTL，再计算过期时间。WAL lambda 在 key 已存在时返回 `std::nullopt`，使重复 claim 不追加 WAL，仍返回 `0`；key 不存在时生成 `SETEXAT` record，mutation 再调用 `store_.set_if_absent()`。

`DEL` 在 key 不存在时返回 `std::nullopt`，继续返回 `0`，不追加 WAL；key 存在时生成 `DEL key`，mutation 执行 `store_.erase()`。

`EXPIRE` 在 key 不存在时返回 `std::nullopt`，继续返回 `0`，不追加 WAL；key 存在时生成 `EXPIREAT key epoch_ms`，mutation 执行 `store_.expire_at()`。

## 契约边界

对控制面来说，v98 只能理解为内部写路径去重，不是新的执行面。`fixtures/release/verification-manifest.json` 和 `fixtures/release/runtime-smoke-evidence.json` 记录了 `write_wal_helper="CommandProcessor::execute_with_wal"`、作用范围 `["SET","SETNXEX","DEL","EXPIRE"]`、`write_wal_helper_behavior_preserved=true`，以及边界说明：`execute-with-wal helper only`、`SET/SETNXEX/DEL/EXPIRE behavior preserved`、`no no-op WAL record append for SETNXEX/DEL/EXPIRE misses`。

`read_only`、`execution_allowed`、`restore_execution_allowed`、`order_authoritative` 这些字段没有因为 helper 出现而改变。Node v233 仍然只能把 mini-kv 当作 runtime evidence provider；历史 Node v231 no-start guard receipt 继续保持 manual window closed by default、no Node/Java/mini-kv auto-start、no sandbox connection execution、no credential value read、no schema rehearsal、no managed audit write、no restore/load/compact、no Java order authority。

## 测试和真实运行

`tests/wal_tests.cpp` 新增回归断言：`SETNXEX token 60 duplicate`、`DEL missing`、`EXPIRE missing 60` 都返回 `0`，并且 `wal.maintenance_report(store).records == 7`。这个断言保护的不是“返回值看起来一样”这么浅的行为，而是确认 no-op miss 没有悄悄写入多余 WAL record。

`minikv_command_tests`、`minikv_release_verification_manifest_tests`、`minikv_runtime_smoke_evidence_tests` 继续保护 runtime JSON、release fixture、digest 和 no-start guard 链路；`minikv_readonly_fixture_tests` 继续保护只读 INFOJSON fixture。

本轮验证结果是 CMake configure 成功、CMake build 成功、32/32 CTest 通过。真实 TCP smoke 使用 `minikv_server 6498 127.0.0.1` 和 `minikv_client 127.0.0.1 6498 5000 --connect-retries 10 --retry-delay-ms 100`，执行 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`。结果显示 v98 只读 evidence 正常，`GET restore:real-read-token` 返回 `(nil)`，没有执行 `SETNXEX`、`LOAD`、`COMPACT` 或 restore。

## 文件规模判断

本轮拆分后的关键行数是：`src/command.cpp` 557 行，`src/managed_audit_receipts.cpp` 929 行，`tests/command_tests.cpp` 860 行，`tests/runtime_smoke_evidence_tests.cpp` 624 行，`tests/release_verification_manifest_tests.cpp` 572 行。当前 `include/`、`src/`、`tests/` 下没有超过 1000 行的文件，所以继续硬拆并不必要。下一次真正值得拆的候选，是继续增长后的 managed audit receipt formatter 或证据类测试，而不是已经收敛到 557 行的 `command.cpp`。

一句话总结：v98 把 WAL 写命令的重复控制流收成一个可验证的内部 helper，在不扩大任何 Node/Java/restore 权限的前提下，让 mini-kv 的生产雏形阶段多了一层可维护性和行为证据。
