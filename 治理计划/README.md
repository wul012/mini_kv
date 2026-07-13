# mini-kv 治理计划索引

本目录保存 mini-kv 自己的治理、拆分、归档保留和 Codex 执行 playbook。它和 `代码讲解记录_*` 分开：讲解记录解释已经完成的版本，治理计划约束未来怎么做。

## 当前计划

- `v1608-mini-kv-governance-consolidation-roadmap.md`：mini-kv 侧治理收口路线图，响应 Node v2114 consolidation 指针和 Claude 的跨项目审计结论。
- `v1608-codex-consolidation-playbook.md`：后续 Codex 执行拆分/归档保留工作的步骤清单。
- `v1608-archive-retention-policy.md`：针对 `e/` 约 1.1GB 归档根和未来 active root 的路径稳定、冻结、索引、轮转规则。
- `v1631-osfs-coursework-completion-brief.md`：OSFS 课设从 v1630 草稿推进到“实验二 Linux 二级文件系统”实质达标的执行简报（二级目录 MFD/UFD、磁盘用户表 + login、fd 读写指针、交付材料重做、需求-证据矩阵）。已于 v1631-v1633 执行完毕并通过独立复核。
- `v1634-osfs-capacity-extension-brief.md`：OSFS 课设容量扩展简报（一级间接块、FSCK 一致性检查、USERADD/PASSWD 用户管理、交付刷新），把矩阵中教学简化项升级为完整实现。已于 v1634-v1636 执行完毕并通过独立复核。
- `v1637-production-excellence-completion-brief.md`：Production Excellence 轨道收尾简报。阶段一 receipts consolidation 已于 v1656 以 27/0/1 census 完成，阶段二文档诚实性刷新于 v1657 完成，阶段三 E1-E10 证据于 v1658 完成；Claude 于 2026-07-11 独立终审 PASS，mini-kv Stage-1 轨道关闭。成熟度仍为单项目验证加跨项目合同对齐，Stage-2 继续等待四项目 capstone。上级计划见 Node 仓库 `docs/plans/production-excellence-final-acceptance.md`。
- `stage2-minikv-operational-brief.md`：**未激活（门控：Stage-1 终审 PASS + capstone PASS）**。Stage-2 运行实态简报：Linux 发布包与服务化、kill-9/备份恢复演练、RESP 与 OSFS fuzz、性能 SLO、14 天 soak 参与。上级计划见 Node 仓库 `docs/plans/stage2-operational-reality-program.md`。
- `elegance-hotspot-program-minikv.md`：Production Excellence 后的有界优雅热点整治；最多 4 版，先落命名 census 与只减不增 baseline，再修复 top-5 非合同长名，最终停在 Claude review 点。
- `elegance-round2-minikv.md`：优雅第二轮条件式收尾；最多 2 版，先机械审计剩余最长名的 pin 状态，只有安全候选达到 5 个才允许第二刀，否则在审计版诚实关闭。
- `v1663-osfs-teaching-evidence.md`：OSFS 讲解与证据加厚简报；历史版本讲解不移动，新建按机制组织的通俗教程，并补 fd 会话、组权限、目录扩容、稀疏区间和 inode 位图损坏测试，同时诚实复核 FinalShell 分段记录。

## 当前硬边界

- 不移动、不重命名 `a/`、`b/`、`c/`、`d/`、`e/`、`f/` 下任何历史归档。
- 不移动、不重命名 `fixtures/release/*.json` 历史证据。
- 不为了“清理体积”破坏 Node 对 mini-kv 历史路径和 digest 的引用。
- 后续拆分 `src/command.cpp` 或 `src/shard_readiness.cpp` 必须先证明 public command output、fixture、CHECKJSON、SHARDJSON 和测试行为不变。
- 归档治理优先做索引、冻结、阈值和后续 active root 轮转，不做历史搬迁。
