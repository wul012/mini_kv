# v1637+ 任务简报：mini-kv Production Excellence 收尾（receipts 收敛完成 + 文档诚实性刷新 + 轨道终审）

执行者：mini-kv 仓库的 Codex 会话。上级计划：
`D:\nodeproj\orderops-node\docs\plans\production-excellence-final-acceptance.md`（只读）。
基础 playbook 与进度账本不变：Node 仓库的 `production-excellence-minikv-playbook.md`（只读）
+ 本仓库 `docs/production-excellence-progress.md`（唯一进度写入点）。
K0–K6 已全部完成并通过评审；OSFS 课设 v1629–v1636 已交付并独立验证。本简报只做三件事：
把 receipts 收敛做完、把文档刷新到与当前树一致、准备轨道终审证据。全程遵守 AGENTS.md
版本纪律（重建后再测、全量 CTest、`f/<版本>/` 归档、第四册中文讲解、清理门、commit+tag+push、
中间版本不 block-watch CI）。

## 起点核对（每次会话开始）

`git log --oneline -5` + `git status` 确认干净树与最新 tag；若上一版半收尾，先补齐。
核对 `docs/receipts-consolidation-note.md` 的当前 slice 状态与本简报是否一致，不一致以
仓库事实为准并先在进度账本 Deviations 记录。

## 阶段一：receipts 收敛完成（预计 3–6 个版本）

1. **先做 canonical-surface 决定（v1637 起始动作）**：v1627/v1628 已把 no-network 漂移
   钉死为唯一一处 `Node v323\u0027s`（fixture）vs `Node v323's`（当前 formatter）拼写差。
   本版做出并固化显式决定——**以当前运行时 formatter 拼写为 canonical surface**；理由：
   公共运行时输出是下游真正消费的表面，且历史 fixture 冻结不可改写。执行方式：
   - 不改写任何已提交 fixture 字节；
   - parity harness 增加一个显式的、单点的、有注释的等价规则（仅允许该一处 `'`
     ↔ 撇号差异，多一处即 fail）；
   - 决定与理由写入 `docs/receipts-consolidation-note.md` 并由既有
     `receipts_consolidation_note_contract` 保护；digest、字段序、边界字段不变是硬门。
2. **随后按 note 的 slice 顺序迁移剩余 formatter**：v1637 已在 canonical 决策后把
   `runtime_credential_resolver_no_network_safety_fixture_receipts.cpp` 迁移到 builder；当前仓库事实是 Slice 0 基础设施已完成，
   `runtime_credential_resolver_abort_rollback_semantics_receipts.cpp` 与
   `runtime_credential_resolver_no_network_safety_fixture_receipts.cpp` 这 2 个 formatter 已迁移，
   v1638 已为 signed-human approval artifact、credential-handle approval、endpoint-handle allowlist approval
   三份审批合同建立迁移前 parity 基线，但还未迁移这三份 formatter。剩余 receipt formatter 以
   `docs/receipts-consolidation-note.md` 的最新 census 为准。每版一个连贯 slice（建议 4–8 个文件），每个文件迁移前
   先冻结 fixture 子对象字节+digest，迁移后逐字节 parity 证明；触发任何 stop condition
   （note 定义）立即停版并记录。
3. 收敛完成 = note 中 census 显示 28/28 已迁移（或对无法迁移者写明 waiver 及评审检查点），
   全量 CTest 绿，coverage floor（88）未跌。请求 Claude 检查点评审。

## 阶段二：文档诚实性刷新（1 版）

E7 门要求文档对照**当前树**重新核实，而不是写它们时的树：

1. `README.md`、`START_HERE.md`、`docs/CAPABILITY-SNAPSHOT.md`：核对 OSFS 现状描述
   （v1629 单级目录的旧描述必须更新为 v1631–v1636 的二级目录/用户表/间接块/FSCK 现实），
   版本指针、测试计数、能力清单逐条对树核实。
2. `docs/archive-retention-index.md` + `scripts/archive_inventory.py` 重新盘点一次：
   OSFS 期间 `f/1629–1636` 与 `课程设计交付/` 增长需入账；仍然只测量、告警、记录，
   不搬迁任何历史路径。
3. 三份 OSFS 简报（治理计划内）标注“已执行完毕”，`治理计划/README.md` 索引同步。
4. 门槛：新增/更新的每条能力主张要么有指向真实文件/测试的引用，要么删除。

## 阶段三：轨道终审准备（1 版）

1. 对照 E1–E10 逐门自审，产出 `docs/minikv-track-final-evidence.md`：每门的机械检查
   在哪、waiver 清单（`shard_readiness_boundary_field_names.cpp`、gcovr negative-hits
   workaround 均需引用原始评审记录）、census 数字、CI run 链接。
2. coverage 实际值若高出 floor 2 分以上，floor 上调（只升不降）。
3. 完成后请求 Claude 终审。终审通过前，任何文档不得宣称超出
   `single-project validation + cross-project contract alignment` 的成熟度。

## 本简报的失败条件

- parity 等价规则超出那一处已钉死的拼写差 = 立即停版回退。
- 为让迁移通过而改动既有测试期望或 fixture 字节 = 迁移错误，回退。
- 文档刷新中出现无文件证据的能力主张 = E7 不通过。
- ratchet/floor 放松 = 回退；只允许收紧。
