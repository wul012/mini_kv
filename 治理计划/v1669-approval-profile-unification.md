# v1669 approval receipt profile 与 digest 单点收口

## 背景与目标

v1668 已把四个共享 helper family 的手工字段追加从 1056 降至 814，并建立精确 814/6 棘轮。重新复算后，signed-human、credential-handle、endpoint-allowlist 三份连续 approval contract receipt 各有 78 至 82 个布尔字段，其中 68 个同名同值。它们不是三个可合并的完整 formatter，但包含多个顺序稳定的共同区段。

本版提取这些共同区段为私有有序 profile，局部 contract、Node 版本、compatibility rule 与插入位置继续由原 owner 持有。同时清理 6 个 `receipt_digest` 转发器：保留被二十多份 credential-resolver formatter 共同消费的 canonical wrapper，只删除另外 5 个局部复制，避免为了追求零计数触碰整条稳定链。

## Family 设计说明（先于实现，9 行）

1. 抽象名：`approval_receipt_profile`。
2. 数据边界：只保存三份 approval receipt 中同名、同值、同顺序的布尔区段。
3. 行为边界：继续使用 v1668 的 `append_boolean_fields`，不创建第二个 renderer。
4. read-only/readiness/credential/raw-endpoint/external-effects 分段，允许局部字段留在历史插入点。
5. formatter 仍拥有 receipt metadata、Node 版本、digest parts、warnings、recommendations 与最终对象。
6. endpoint 的 `raw_endpoint_url_stored` 保留在 acceptance 与 processing profile 之间。
7. signed artifact 与 credential/endpoint handle 的专属字段不进入共享表。
8. canonical digest wrapper 保留一份，其它 owner 直接调用既有 `runtime_evidence::digest`。
9. fixture、公共 header、字段顺序、字节、digest、兼容规则和执行边界不变。

## 需求—证据矩阵

| 需求 | 实现 | 机械证据 | 初始状态 |
|---|---|---|---|
| 三份 approval 共同边界数据化 | 新私有 profile header + 三个 formatter | 三个具名 receipt test + remaining-owner oracle | 已完成 |
| 保留局部插入顺序 | endpoint acceptance/stored/processing 分段；其它局部字段留原位 | raw exact/canonical parity、字段数与 digest | 已完成 |
| digest 转发器单点 | 保留 credential canonical wrapper，删除 5 个局部 wrapper | census 6→1，增长和 stale baseline 都失败 | 已完成 |
| 重复继续偿还 | CMake 精确 manual-push baseline | 814→613，并同步收紧 | 已完成 |
| 外部合同稳定 | 不改 fixture/public header/test expectation | 空 diff + focused/full CTest + CLI | 已完成：57/57、353/353 与真实 CLI 全绿 |
| 版本闭环 | 中文讲解、选择性归档、commit/tag/push/CI | `f/1669/` + walkthrough + Actions | 本地归档完成；远端 CI 作为 tag 后证据 |

## 完成门槛

- 三个 approval formatter 的 201 个共同 push 由具名 profile 消费，局部字段仍显式可读。
- manual-push 精确 baseline 从 814 收紧到实际值；local digest wrapper 从 6 收紧到 1。
- 低一位与高一位的故障注入分别证明 regression/stale 两条红路径。
- 三个具名 receipt 测试、remaining-owner oracle、SMOKEJSON/manifest 相关门、全量 CTest 和真实 CLI 全绿。
- 新头文件与所有新增标识符不超过 40 字符，任何被改源码保持低于 800 非空行。

## 失败条件

- 修改 fixture、测试期望或 canonical compatibility rule 适配重构：回退。
- 把三份完整 formatter 参数化成版本分支或万能 schema：回退。
- 为了 digest=0 批量触碰共享 wrapper 的二十多个稳定消费者：超范围。
- profile 改变任何局部字段插入点、布尔值、字节或 digest：失败。
- baseline 未随实际降低在同一提交收紧：失败。
