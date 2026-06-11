# 第 1546-1565 版代码讲解：command catalog single-source refactor

本版目标是把 mini-kv 长期分散在多个文件里的“命令目录”收束成一个单一来源。之前 `src/command.cpp` 有一份分发表，`src/command_contracts.cpp` 有一份命令契约表，`src/line_editor.cpp` 又有一份补全候选和 key 参数补全判断。它们描述的是同一批 88 个命令，但维护位置分散，后续每增加一个只读证据命令，都要同步改好几处，最容易出错的不是业务逻辑，而是目录、HELP、COMMANDSJSON、EXPLAINJSON、CHECKJSON、补全策略之间的轻微不一致。

本版不是新增执行入口，不是新增 read/write 路由，不是改写存储语义，不是开放 LOAD/RESTORE/COMPACT 参与证据生成，也不是让 mini-kv 去扫描 Node 或 Java。它做的是可维护性拆分：把同一份命令事实集中到 `command_catalog`，再让既有消费者从这里读取，最后用测试和 SHARDJSON 的 `commandCatalogQuality` 字段证明这只是结构优化，不改变运行时命令行为。

## 上游计划和版本定位

本轮推进前只读查看了 Node 计划目录：

```text
D:\nodeproj\orderops-node\docs\plans\README.md
D:\nodeproj\orderops-node\docs\plans3\v2023-v2042-readiness-gate-split-roadmap.md
```

最新计划的主线是 Node 侧 `managedAuditManualSandboxConnectionReadinessGate` 的拆分维护，建议 Java 和 mini-kv 可以并行做各自的维护型推进，但不要求 mini-kv 读取新 sibling 运行证据、启动服务或修改 Node/Java。mini-kv 因此选择了一个纯 C++ 侧、可验证、边界清楚的重构主题：命令目录单一来源。

版本范围 v1546-v1565 作为一组维护版本发布。它没有新增业务证据命令，而是冻结“重构质量证据”：当前 release version 滚到 v1565，`fixtures/release/shard-readiness-v1546.json` 到 `fixtures/release/shard-readiness-v1565.json` 都保留，当前 `fixtures/release/shard-readiness.json` 代表 v1565。

## 单一来源模块

核心新增文件是：

```text
include/minikv/command_catalog.hpp
src/command_catalog.cpp
```

`CommandCatalogEntry` 现在统一承载命令名、usage、category、是否修改 store、是否触碰 WAL、是否 stable、描述、分发 verb、是否参与 key 补全：

```cpp
struct CommandCatalogEntry {
    std::string_view name;
    std::string_view usage;
    std::string_view category;
    bool mutates_store;
    bool touches_wal;
    bool stable;
    std::string_view description;
    CommandDispatchVerb dispatch_verb;
    bool key_completion;
};
```

这几个字段对应控制面最关心的边界问题：命令是不是 read-only，是否可能改 store，是否触碰 WAL，是否只是稳定的元数据接口，是否需要把 key 作为后续参数补全对象。原来这些含义散落在不同调用点里，现在每个命令只定义一次。

公开 API 也刻意保持窄：

```text
entries()
find()
is_known()
count()
lookup_dispatch_verb()
is_key_completion_command()
command_names()
help_text()
```

这让调用方只表达自己的需求：分发层问 dispatch verb，契约层问 entries，补全层问 command names 和 key completion，HELP 问 help_text。调用方不再复制命令列表，也不需要知道目录如何存储。

## 命令分发层

`src/command.cpp` 的变化是把本地命令分发表删除，改为：

```cpp
using command_catalog::CommandDispatchVerb;
...
switch (command_catalog::lookup_dispatch_verb(command)) {
```

对 `PING`、`SET`、`GET`、`SHARDJSON`、长证据命令等所有既有路径来说，最终进入的 handler 没变。变化只是“如何识别命令”从本地数组迁到 `command_catalog`。

`metrics_command_name()` 也改为 `command_catalog::is_known()`。这点小但重要：指标里的 known/UNKNOWN 判断和真实命令目录现在同源，不会出现“命令可以执行，但指标归 UNKNOWN”或“指标认识，分发不认识”的维护偏差。

`CommandProcessor::help_text()` 直接返回 `command_catalog::help_text()`。HELP 文本因此从同一份 usage/description 生成，避免 HELP 和 COMMANDSJSON 对同一命令给出不同说明。

## 契约层

`src/command_contracts.cpp` 原先维护了一份独立的 88-entry catalog。本版删除这份重复数据，改由：

```cpp
const auto& catalog = command_catalog::entries();
const auto* entry = command_catalog::find(command);
```

`COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 的输出语义保持不变，但来源改成 `command_catalog`。这意味着当后续新增一个证据命令时，只要它出现在 catalog，契约层就会自然拿到同样的 name、usage、category、mutates_store、touches_wal、stable 和 description。

`include/minikv/command_contracts.hpp` 通过别名继续暴露 `CommandCatalogEntry`：

```cpp
using CommandCatalogEntry = command_catalog::CommandCatalogEntry;
```

这是为了保护既有测试和外部 include 关系，不让一次内部拆分变成公共头文件的大面积破坏。

## 行编辑和补全

`src/line_editor.cpp` 删除了本地 command list 和 key-command list。默认补全候选来自：

```cpp
command_catalog::command_names()
```

key 参数补全判断来自：

```cpp
command_catalog::is_key_completion_command(command)
```

这保持了两个边界：第一，补全只消费命令目录，不反向影响命令执行；第二，key completion 是 catalog 里的布尔事实，不需要行编辑器用字符串集合再维护一遍。

`default_client_completion_options()` 仍然追加 `:history`，因为这是客户端行编辑自己的特殊命令，不属于服务端命令目录。这个分界保留后，catalog 不会混入非服务端命令。

## SHARDJSON 质量证据

`src/shard_readiness.cpp` 的 top-level `releaseVersion` 滚到 `v1565`，并新增 `commandCatalogQuality`：

```json
"commandCatalogQuality": {
  "releaseRangeStart": "v1546",
  "releaseRangeEnd": "v1565",
  "scope": "command-catalog-single-source-refactor",
  "commandCount": 88,
  "dispatchCatalogSplit": true,
  "contractCatalogDeduplicated": true,
  "helpTextGeneratedFromCatalog": true,
  "completionCandidatesGeneratedFromCatalog": true,
  "keyCompletionPolicyGeneratedFromCatalog": true,
  "runtimeCommandBehaviorChanged": false,
  "storageBehaviorChanged": false,
  "walBehaviorChanged": false,
  "routerBehaviorChanged": false,
  "nodeOrJavaFilesModified": false
}
```

这个字段是给控制面读的“重构证明”，不是新的执行授权。`runtimeCommandBehaviorChanged=false` 表示命令分发结果不应变化；`storageBehaviorChanged=false` 和 `walBehaviorChanged=false` 表示这轮没有调整 store/WAL 语义；`routerBehaviorChanged=false` 表示没有装 active router；`nodeOrJavaFilesModified=false` 表示 mini-kv 只在本项目内闭环。

这里也把 digest 加入了 command catalog quality 的三段事实：v1546-v1565 refactor、命令数 88、HELP/completion/contracts 已同源。它让 SHARDJSON 当前证据能被上层系统快速判断为“维护重构已完成”，而不是误读成新的运行能力。

## Fixture 链

本版保留：

```text
fixtures/release/shard-readiness-v1546.json
...
fixtures/release/shard-readiness-v1565.json
fixtures/release/shard-readiness.json
```

这些 fixture 冻结的是同一个重构事实在 v1546-v1565 的版本链。每个历史 fixture 的 top-level `releaseVersion` 对应自己的版本；当前 fixture 是 v1565。旧的 precheck upstream receipt verification section 没被覆盖，它仍然保留 v1545 的历史意义；新证据放在 `commandCatalogQuality`，避免用重构质量字段去改写旧业务证据链。

## 测试覆盖

新增 `tests/command_catalog_tests.cpp`，重点保护五类行为：

```text
catalog count remains 88
dispatch verb mapping matches existing handlers
HELP and command_names are generated from the catalog
line-editor completion consumes catalog names and key-completion policy
command_contracts text/JSON/explain/check surfaces still expose the same command metadata
```

新增 `tests/command_catalog_refactor_fixture_tests.cpp`，从源码目录读取 v1546-v1565 fixture 和当前 fixture，确认：

```text
commandCatalogQuality exists
release range is v1546-v1565
commandCount is 88
single-source/dedup/help/completion flags are true
runtime/storage/WAL/router behavior change flags are false
nodeOrJavaFilesModified is false
current release is v1565
old precheck section still keeps its v1545 meaning
```

既有测试也同步加固：`command_tests` 和 `shard_readiness_tests` 校验 SHARDJSON 内的新质量字段；两个历史 fixture 测试确认当前 fixture 已滚到 v1565，同时旧 section 不被重构证据覆盖。

## 真实验证

本版验证包括：

```text
targeted CTest: command_catalog_tests, command_catalog_refactor_fixture_tests, command_tests,
                line_editor_tests, shard_readiness_tests, and adjacent fixture/route-preview checks
full CTest: all tests pass
CLI smoke: HELP, COMMANDS, COMMANDSJSON, SHARDJSON, CHECKJSON, QUIT
TCP smoke: COMMANDS, COMMANDSJSON, CHECKJSON, HELP, QUIT with temporary server stopped after verification
```

TCP smoke 没把完整 `SHARDJSON` 放进最终 TCP 断言，是因为现有 client 对超长单行响应的展示有长度限制；完整 SHARDJSON 已由 CLI smoke 和 fixture tests 覆盖。TCP smoke 保留命令目录、COMMANDSJSON、CHECKJSON 和 HELP 的跨进程验证，正好覆盖这轮重构最关心的共享 catalog surface。

## 一句话总结

v1546-v1565 把 88 个命令的分发、契约、HELP 和补全事实收束到同一个 catalog，让后续 mini-kv 继续加证据命令时少改重复表、少冒漂移风险，同时继续守住 no router、no write、no WAL、no execution、no Node/Java mutation 的边界。
