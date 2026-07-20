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
- `readme-exhibition-brief.md`：v1664 README 展示维护简报；把 GitHub 首页从版本流水改为访客导向的能力、架构、复现和证据地图，并用现有合同钉住 352 测试、90% 核心覆盖率下限、27/0/1 receipt census、735/883 命名基线与获准的只读 capstone 成熟度标签。
- `v1665-test-architecture-brief.md`：v1665-v1666 测试架构收口；以 suite manifest、断言 census 和 1000 非空行 ratchet 拆解 command/shard-readiness 两个 4000 行级测试单体，产品代码与 fixture 合同保持不变。

- `v1667-cmake-target-path-budget.md`：测试构建身份收口；保留 v1662 长源文件名与公开 CTest 名，只缩短触发对象路径警告的内部 target，并新增长 target shrink-only baseline、路径风险分数门和故障注入。
- `v1668-receipt-field-profiles.md`：三分优雅提升计划的第一阶段；把 receipt 共享布尔边界从逐字段命令式追加改为有序数据 profile，并以精确 manual-push/digest-wrapper ratchet 阻止重复回流，外部 JSON、digest 与 fixture 字节保持不变。
- `v1669-approval-profile-unification.md`：提取三份 approval contract receipt 的共同有序边界区段，保留局部插入点，并把重复 digest 转发器从 6 收口到唯一 canonical wrapper。
- `v1670-atomic-file-writer.md`：统一 WAL compact 与 Snapshot save 的临时文件、flush/close、原子替换和失败清理协议，并用独立故障测试保护旧目标不被失败写覆盖。
- `v1671-test-link-shards.md`：把 342 个重复链接 `minikv` 的测试 executable 改为独立 object target 加 8 个稳定哈希 link shard，保留 354 个 CTest 名、测试进程隔离和 instrumented lane 旧拓扑，并用真实核心 touch 对比链接次数、时间与磁盘体积。

## 当前硬边界

- 不移动、不重命名 `a/`、`b/`、`c/`、`d/`、`e/`、`f/` 下任何历史归档。
- 不移动、不重命名 `fixtures/release/*.json` 历史证据。
- 不为了“清理体积”破坏 Node 对 mini-kv 历史路径和 digest 的引用。
- 后续拆分 `src/command.cpp` 或 `src/shard_readiness.cpp` 必须先证明 public command output、fixture、CHECKJSON、SHARDJSON 和测试行为不变。
- 归档治理优先做索引、冻结、阈值和后续 active root 轮转，不做历史搬迁。
