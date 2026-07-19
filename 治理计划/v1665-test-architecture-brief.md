# v1665-v1666 测试架构收口简报

执行状态：v1665 已完成 command suite、MinGW CTest runtime 与版本闭环；v1666 已完成 shard
机械拆分和 focused 验证，最终全量/归档/CI 收尾进行中。

## 背景与指令

用户授权持续优化至工程上满意。v1664 后，产品源码已经受 800 非空行 ratchet 保护，
剩余最高价值债务集中在 `tests/command_tests.cpp` 与
`tests/shard_readiness_tests.cpp` 两个 4000 行级单体。本轮只优化测试结构，不改变产品
行为、公共命令、fixture 字节、WAL/snapshot/restore 语义或跨项目证据合同。

## Step-0 起点

- HEAD/tag：`77e31bec` / `v1664`；远端同 SHA，GitHub Actions 七项全绿。
- 工作树已有用户修改：`治理计划/readme-exhibition-brief.md`，本轮不得覆盖或提交。
- 初始非空行：command tests 3805，shard-readiness tests 4328。
- 初始断言 oracle：command `assert=457`、`assert_response_contains=2447`；shard
  `assert=8`、`assert_contains=3710`、fixture-diff helper 调用/定义合计 2。
- 最新 Node 展示简报允许各仓独立维护；endpoint-handle 计划没有本轮新增依赖。

## Family 设计（先于实现）

1. 抽象名：`TestSuitePart`，manifest 每行只保存源文件路径和唯一入口函数。
2. 数据边界：manifest 决定编译与执行清单；CMake checker 负责结构、断言与体积规则。
3. 行为边界：共享 fixture/support 只管理 setup 与历史 fixture 读取；part 只保留断言。
4. 第三个 part 出现前必须已有共享 manifest/checker，禁止复制独立 runner。
5. 产品模块、fixture 和公开输出不属于这个 family，不得随拆分修改。

## 需求-证据矩阵

| 需求 | 实现 | 机械证据 | 完成条件 |
|---|---|---|---|
| 冻结迁移 oracle | suite manifest + assertion census | 删除断言或漏掉入口时 CTest 失败 | 两套 suite 精确计数 |
| 阻止测试重新膨胀 | 1000 非空行 test-size ratchet | 新增第二个超限文件或 baseline 放松时失败 | v1666 零豁免 |
| 拆 command 单体 | core/shard/runtime 具名 parts | command target 编译 manifest 全部 parts | 每个 part <=1000 |
| 拆 shard 单体 | contract/history/smoke 具名 parts | shard target 编译并调用全部 parts | 每个 part <=1000 |
| 合同不变 | 原断言文本机械迁移 | focused/full CTest + fixture digest manifest | 断言计数不减、fixture 0 diff |
| 收口可复核 | 中文讲解、归档、账本、CI | 本地命令与远端 run | v1666 review 点 |

## 版本边界

### v1665：command 测试架构

- 先在未拆树上落 test-size baseline 与通用 suite-layout checker。
- 把 command 单体拆为具名 part；共享 setup 进入一个 support 层。
- baseline 从 command+shard 两项收紧为只剩 shard 一项。

### v1666：shard-readiness 测试架构

- 把当前 JSON 合同按字段域拆分，把 v144-v200 历史 fixture 按版本段拆分，把真实
  `SHARDJSON` 命令 smoke 独立出来。
- baseline 收紧为空；所有 `tests/*.cpp` 均不超过 1000 非空行。
- 到此停止 retro test-split，不顺手拆 800 行以下文件。

实际落点：五个 current-contract 字段域、v144-v164 与 v165-v200 两段历史 fixture、一个真实
命令 smoke，共八个 manifest part；最大 part 906 非空行。`assert=8`、`assert_contains=3710`、
fixture-difference helper 定义/调用共 2 次保持不变，fixture 与运行时 SHARDJSON 仍各经过一次
完整 wrapper。体积 baseline 已收紧为空，scanner 输出 `sources=361 limit=1000 oversized=0`。

## 最终验证

1. 干净配置和构建。
2. suite-layout、test-size、command、shard-readiness focused CTest。
3. 全量 CTest、fixture manifest、archive inventory、clang-format。
4. 真实 CLI `SMOKEJSON` / `CHECKJSON`，确认 read-only、no router、no write、no execution。
5. 讲解先于最终 verify；随后归档、commit/tag/push，最终版本等待 CI。

## 失败条件

- 为拆分删除、弱化或改写测试期望 = 回退。
- part 已编译但未由 suite main 调用 = 失败。
- assertion census 下降、baseline 增长或阈值放松 = 失败。
- 产生新的 >1000 非空行测试文件 = 失败。
- 修改 fixture、公共 command 输出或产品源码来适配拆分 = 失败。
- 覆盖用户未提交 README 简报修改 = 失败。
- v1666 后继续无目标拆小文件 = 违反停止条件。
