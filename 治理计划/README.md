# mini-kv 治理计划索引

本目录保存 mini-kv 自己的治理、拆分、归档保留和 Codex 执行 playbook。它和 `代码讲解记录_*` 分开：讲解记录解释已经完成的版本，治理计划约束未来怎么做。

## 当前计划

- `v1608-mini-kv-governance-consolidation-roadmap.md`：mini-kv 侧治理收口路线图，响应 Node v2114 consolidation 指针和 Claude 的跨项目审计结论。
- `v1608-codex-consolidation-playbook.md`：后续 Codex 执行拆分/归档保留工作的步骤清单。
- `v1608-archive-retention-policy.md`：针对 `e/` 约 1.1GB 归档根和未来 active root 的路径稳定、冻结、索引、轮转规则。
- `v1631-osfs-coursework-completion-brief.md`：OSFS 课设从 v1630 草稿推进到“实验二 Linux 二级文件系统”实质达标的执行简报（二级目录 MFD/UFD、磁盘用户表 + login、fd 读写指针、交付材料重做、需求-证据矩阵）。

## 当前硬边界

- 不移动、不重命名 `a/`、`b/`、`c/`、`d/`、`e/`、`f/` 下任何历史归档。
- 不移动、不重命名 `fixtures/release/*.json` 历史证据。
- 不为了“清理体积”破坏 Node 对 mini-kv 历史路径和 digest 的引用。
- 后续拆分 `src/command.cpp` 或 `src/shard_readiness.cpp` 必须先证明 public command output、fixture、CHECKJSON、SHARDJSON 和测试行为不变。
- 归档治理优先做索引、冻结、阈值和后续 active root 轮转，不做历史搬迁。
