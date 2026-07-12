# mini-kv 优雅第二轮 —— 条件式收尾（≤2 版，有界）

执行者：`D:\C\mini-kv` 的 Codex 会话。2026-07-12 授权。从属于 program close：
Node 侧发起 capstone 复跑时，本仓库只需保持 CLI 输出合同稳定。

背景：第一轮（v1659–v1661）只整治了 top-5 路径，baseline 冻结在 735（文件名）+
883（公开标识符）。本轮回答一个问题：**还有没有值得改的安全候选**，有就再切一刀，
没有就诚实关闭（aiproj E-A2 先例——预算是上限不是配额）。

约束：AGENTS「Elegance Gates」节全部生效；fixture/CLI/RESP 输出字节不动；公共
header API 不动；跨平台文本工件行尾用 .gitattributes 或写入时钉死（内核反模式，
本仓库 v1659 已踩过）。

## E2-M1 —— pin 审计 + 条件分叉（1 版）

1. 从 census baseline 取未整治的前 10 个最长名（文件名与公开标识符各取若干），逐个
   写 pin 判定：被 CLI 命令/JSON 键/公开 header/census 合同/waiver 钉住 = 不可动；
   否则 = 安全候选。审计文档随本版提交（参照 aiproj `pin-audit.md`）。
2. 分叉：安全候选 ≥5 → 进入 E2-M2；<5 → 在本版直接记录"无足量安全候选"并关闭
   本轮（这是合法且预期的结果之一）。

### v1662 审计合同设计（先于实现）

- 抽象名：`PinAuditRow`，表示 census 候选、pin 类型、判定与机械证据。
- 数据边界：`config/elegance-round2-pin-audit.tsv` 只保存合并排序后的前 10 名与结论。
- 行为边界：既有 `elegance_name_census_contract` 重算排名并验证 TSV，不复制第二套 census。
- 排名规则：文件名与公开标识符合并，按长度降序、类别、名称升序，固定取 10 条。
- pin 规则：公开 header 路径/标识符不可动；仅构建注册的私有测试路径可列为安全候选。
- 分叉规则：测试机械统计 `safe` 数量；少于 5 时 E2-M2 必须保持未触发。
- 文本边界：baseline 与 TSV 均由 `.gitattributes` 固定为 LF，避免第三次 CRLF 漂移。

## E2-M2 —— 第二刀（1 版，仅当触发）

family 设计说明先于实现（优雅门第 4 条）。改名 top-5 安全候选：提取缺失概念、
更新引用；全量 CTest 绿 + 受影响 fixture 字节不变；baseline 与上限同一提交收紧。
之后硬停，请求评审。

## 失败条件

- 任何 CLI/receipt/RESP 输出字节、公开 API 签名变化 = 回退。
- pin 审计缺失或改名对象未经审计 = 版本无效。
- baseline 放松 = 回退；凑数改名（无审计支持的"安全"判定）= 失败。
- 超过 2 版 = 违反停止条件。

## 执行进度

- E2-M1 对应 v1662：合并 735 个文件名与 883 个公开标识符，按长度/类别/名称稳定取前 10；机械结果为 6F/4I、9 pinned/1 safe，候选 run `29189480986` 七项全绿。
- E2-M2：不触发。安全候选 `1 < 5`，唯一 safe 测试路径保留，不为消费版本额度而凑改名。
- Round 2 状态：READY FOR CLAUDE REVIEW；外部 verdict 保持 PENDING。
