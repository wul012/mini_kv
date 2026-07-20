# v1668 receipt 字段 profile 与优雅度 ratchet

## 背景与目标

mini-kv 的 27 个 runtime receipt formatter 已全部迁移到 ordered JSON builder，现有
`receipt_builder_census_contract` 能证明没有 formatter 退回字符串拼接。不过，字段追加仍有第二层重复：
31 个 receipt 源/辅助文件中存在 1056 处 `fields.push_back(...)`，其中四个共享边界 helper
占 242 处。它们表达的是稳定的只读、禁执行、禁写入、禁凭据、禁自动启动 profile，却仍以命令式代码逐字段展开。

本版建立布尔字段 profile 抽象，并迁移这四个高重复共享族。目标不是修改任何 receipt，
而是让边界数据声明一次、由一个渲染行为追加，并用只减不增的机械计数阻止命令式重复回流。

## Family 设计说明（先于实现，8 行）

1. 抽象名：`BooleanField` + `append_boolean_fields`。
2. 数据边界：每个 profile 只声明有序的字段名与布尔值。
3. 行为边界：ordered JSON builder 统一编码并按声明顺序追加字段。
4. profile 不拥有 receipt 元数据、digest、fixture 路径或 Node 版本语义。
5. formatter 仍拥有字段插入位置、嵌套对象和最终渲染。
6. 字段顺序、字段值、JSON 字节、digest 与 canonical compatibility rule 不变。
7. 历史 fixture、归档、公共 header、CLI/RESP 命令和执行边界不变。
8. 后续 formatter 只能复用此抽象，不得再创建第三套布尔字段追加器。

## 需求—证据矩阵

| 需求 | 实现 | 机械证据 | 状态 |
|---|---|---|---|
| 有序布尔 profile | `runtime_receipt_json_builder.*` | builder 单测固定前后字段顺序与 true/false 编码 | 已完成 |
| 迁移四个共享字段族 | credential、runtime-shell、sandbox、manual evidence helper | 三组 formatter exact/canonical parity oracle | 已完成 |
| 重复只减不增 | `check_receipt_builder_census.cmake` | 精确 814/6 baseline；813/815 两条故障注入 | 已完成 |
| 外部合同不变 | 不修改 public header / fixture | 空 diff + focused 5/5 + full CTest 353/353 + CLI smoke | 已完成 |
| 版本闭环 | 讲解、归档、提交、tag、push、CI | `f/1668/` + walkthrough + Actions | 本地完成；远端 CI 作为 tag 后证据 |

## 完成门槛

- manual receipt-field push 精确基线从 1056 至少下降 200，且 CTest 会拒绝增长或 stale baseline。
- 六个现有 local receipt-digest wrapper 数量不增加，并进入精确 shrink-only baseline。
- `runtime_receipt_json_builder_tests`、三个 family baseline、receipt census、命名 census 全绿。
- 全量 CTest、真实 CLI 只读边界、fixture/product diff、clang-format 和归档门全绿。
- 中文 walkthrough 在最终 verify 之前完成，解释 profile 为什么是数据而不是新执行入口。

## 失败条件

- 修改 fixture、测试期望、canonical rule 或字段顺序来适配实现：回退。
- 只把 push 语法换成另一种逐字段命令式 helper：不算完成。
- manual-push 计数下降但未在同一提交收紧 baseline：失败。
- 新增 router、write、WAL、network、credential read、execution 或 authority：失败。
