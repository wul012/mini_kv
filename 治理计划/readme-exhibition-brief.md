# README 展示简报 —— 一个 maintenance 版本（2026-07-13 授权）

执行状态：已执行；目标版本 `v1664`，commit/tag/push 与对应 GitHub Actions run 构成版本闭环证据。

## Step-0 对账与偏差

- 起点为 `901408e8`，`master` 与远端一致；最近一次 CI run `29220020244` 七项全绿。
- 工作树唯一未跟踪文件是本简报；`v1663` 仍是最新 tag，下一维护版本使用 `v1664`。
- 简报中的 CTest `351` 已过期。v1663 新增 `osfs_resilience_tests` 后，当前 CMake 注册数和
  `project_docs_honesty_contract` 都是 **352**；执行以仓库事实为准，不允许把合同倒退到 351。
- 四项目 capstone 已在 2026-07-13 对最终状态复跑通过。README 只使用获准的精确成熟度
  标签，不把环境门控、单机、只读验证扩写成生产部署或执行 authority。

目标：让 GitHub 首页在 30 秒内向人类访客说清：这是什么、有多扎实、每条主张如何
亲手验证。仓库 description 与 topics 已在 GitHub 侧设置完毕；本版重构 `README.md`
成为展品。当前 README 以「Recent versions」开头——这是给维护者的，不是给访客的；
版本流水移到后部或链接到 CHANGELOG。

硬约束（这就是为什么要走版本而不是热改）：
- `project_docs_honesty_contract` 钉住 README 的能力主张与版本指针——改版前先读该
  合同覆盖哪些句子；被钉句子逐字保留或随合同同步（同一提交内，理由写明），推送前
  本地跑全量 CTest。
- 引用的每个数字都必须来自已提交的 census/report 并给出命令或文件链接。
- 优雅门生效；不新增能力主张；「教学/单机、非生产」边界句保留。

## 目标结构（自上而下）

1. 徽章行：
   `[![CI](https://github.com/wul012/mini_kv/actions/workflows/ci.yml/badge.svg)](https://github.com/wul012/mini_kv/actions/workflows/ci.yml)`
   加 shields.io 静态徽章：coverage `90%`、CTest `351`、平台 `Linux|macOS|Windows`。
2. Hero（中文 3–4 句 + 一段英文镜像）：从零手写的 C++ KV 引擎（WAL 崩溃恢复、
   snapshot、RESP/TCP、loopback 默认）+ OSFS 两级目录课程设计；被 Node capstone
   以真实 CLI 只读验证过。
3. 亮点表：coverage floor 90（CI 强制）/ 350+ CTest / 800 行 ratchet 仅 1 豁免 /
   receipt builder 27/27 合同 / 命名 census 735+883 只减不增——每行链接到对应
   机械门（CTest 名或 ci.yml 步骤）。
4. Mermaid 架构图：client → RESP parser → command engine → store → WAL/snapshot；
   旁路：OSFS 子系统（MFD/UFD/间接块/FSCK）；下方：CTest 合同层。
5. Quickstart：cmake 配置/构建/全量 CTest、CLI smoke（SMOKEJSON/HEALTH）、
   两条合同验证命令、archive_inventory。
6. 证据地图：`docs/minikv-track-final-evidence.md`（含外部 PASS verdict）、
   `docs/SECURITY.md`、`docs/receipts-consolidation-note.md`、OSFS 三份简报。
7. 边界节：保留现有诚实表述（默认 loopback、无认证/TLS、非生产、课程/教学定位）。

## 明确不做

- 不修改 `src/`、`include/`、`tests/`、fixture、命令响应或 CMake 项目版本。
- 不把 OSFS 说成 KV 持久化后端，不把 capstone 说成持续在线集成或生产运行。
- 不移动历史归档和讲解；不为首页视觉效果引入图片生成、前端依赖或动态统计服务。
- 不启动 Stage 2，不改变 coverage、尺寸、receipt 或命名 ratchet 的口径。

## 需求-证据矩阵

| 需求 | 实现位置 | 机械或可复现证据 | 当前状态 |
|---|---|---|---|
| 访客 30 秒理解项目 | `README.md` hero、概览表、架构图 | GFM 渲染、链接审计、`f/1664/图片/01-readme-hero.png` | 完整 |
| 每个数字有已提交来源 | README 每个指标链接到 gate/report | focused 4/4 + receipt 28/27/27/0/1 + elegance 735/883 | 完整 |
| Quickstart 可运行 | README build/CTest/CLI 命令 | 干净 1,011-step build、真实 CLI、全量 352/352 | 完整 |
| 成熟度不越界 | README 精确 maturity line 与边界节 | Node program-close 记录 + docs honesty contract | 完整 |
| 维护者历史不占首屏 | 最近版本移到末尾，细节链接到专门文档 | README 由 527 行收敛为约 136 行；本地链接全存在 | 完整 |
| 版本闭环 | changelog、账本、讲解、`f/1664`、tag/CI | 3239 中文字符讲解、6-file selective archive、cleanup/git/CI | 收尾中 |

## 仪式

一个版本：commit/tag/push、七 job CI 绿、讲解先于最终验证、账本收口。用户侧手动
跟进（非 codex）：GitHub Settings → Social preview 上传一张预览图。

## 失败条件

- 为迁就改写而放松 docs 合同 = 失败（被钉句子逐字搬运）。
- 无已提交来源的数字 = 失败。推送后 CI 红 = 收口违规。

## 本地验收记录

- Step-0：起点 commit/remote/CI 对齐；唯一 351 -> 352 偏差已记录并按现实修正。
- GFM 展示：README 经已认证 GitHub Markdown API 渲染，Playwright 浏览器快照确认中文、表格、
  徽章、maturity line 与相对链接；本地链接审计无缺失。
- 干净构建：CMake configure 成功；全目标 1,011 steps 成功。第一次遗漏 MinGW PATH 的
  编译器自检失败已在 `f/1664/解释/说明.md` 记录，scratch build 删除后从干净目录重试。
- focused：四项机械合同 4/4 通过，2.58 秒；receipt census 为 28/27/27/0/1，elegance
  census 为 735/883，注册测试数为 352。
- 真实 CLI：SMOKEJSON 为只读、禁止执行、无订单权威；CHECKJSON GET 为非写、无 WAL；
  HEALTH OK，QUIT BYE。
- full CTest：352/352 通过，0 失败，217.03 秒；最终文档修改后还会再次运行收口验证。
- archive：`f/1664/` 6 文件、127,258 bytes、0.12 MiB，strict inventory 无 warning。
