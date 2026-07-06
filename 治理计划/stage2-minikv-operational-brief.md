# Stage 2 mini-kv 简报：真实服务化、模糊测试、性能 SLO、恢复演练

状态：**未激活 — 门控中**。激活条件：Stage-1 mini-kv 轨道终审 PASS（见
`v1637-production-excellence-completion-brief.md` 阶段三）+ 集成 capstone PASS。
执行者：mini-kv 仓库的 Codex 会话。上级计划：Node 仓库
`docs/plans/stage2-operational-reality-program.md`（只读）。Stage-1 所有门
（coverage floor、ratchet、格式、归档、讲解、清理门）在 Stage-2 期间继续生效，回退即版本无效。
边界不变：mini-kv 不获得 order authority；evidence 只读；CHECKJSON 永不执行 LOAD；
部署不授予任何跨项目执行权。

## 第 0 步：重新接地（强制，第一版）

1. 确认 Stage-1 PASS 记录与 capstone PASS；缺任一，停止 — 本简报未激活。
2. 核对当前 tag/CI/树；读取 Node 仓库 `docs/ops/vm-census.md` 的容量决定
   （mini-kv 二进制极小，任何 fallback 档位都应包含 mini-kv）。

## 阶段一：服务化打包（2 版）

1. 发布工件：一个可复现的 Linux 发布包（`minikv_server` + `minikv_cli` 二进制、
   配置模板、systemd unit 模板、数据目录布局说明、版本与构建信息文件），由 CI 的
   Ubuntu job 构建并以 digest 记录。可选：轻量容器镜像（scratch/distroless + 静态或
   自带运行库），供 compose 使用；二进制与镜像二选一由 VM census 决定，两者的构建都必须脚本化。
2. 服务合同：`minikv_server` 以 systemd/compose 方式长期运行——工作目录、WAL/snapshot
   路径、`--log-level`、client idle timeout、绑定 localhost（对外一律经 Node 侧代理）。
   健康探测 = RESP `PING`（monitoring loop 使用）。
3. 验收：VM（或本地 compose）上服务开机自启、`PING` 通、重启后数据完好。

## 阶段二：崩溃恢复与备份演练（1–2 版）

1. kill -9 恢复演练（脚本化、可重跑）：写入负载进行中 `kill -9` 服务进程 → 重启 →
   WAL replay/snapshot 恢复 → 数据一致性校验（写入前后 key 集合与值的 digest 对账）→
   产出带时间戳与 digest 的演练回执。演练必须真实产生部分写入风险窗口，不允许在安静点 kill。
2. 备份/恢复演练：snapshot + WAL 归档到备份目录 → 在全新数据目录恢复 → CHECKJSON +
   逐 key 对账 → 回执。备份脚本进入发布包。
3. 每类演练至少 3 次连续成功才算通过；任何一次数据不一致 = 停止，先修复后重来。

## 阶段三：模糊测试（2 版；mini-kv 独有的深度项）

1. RESP 解析器 fuzz target（libFuzzer，Ubuntu CI 的独立 job）：以现有 RESP 测试用例
   构建种子语料（提交入库）；CI 每次 push 跑短时预算（如 5 分钟），每周定时任务跑长时预算；
   崩溃/ASan 报告 = CI 红。已有的 sanitizer lane（K1）直接复用。
2. OSFS 镜像/命令 fuzz target：喂随机损坏的 `.img` 与随机命令序列，目标是 open/FSCK/
   命令处理永不崩溃、永不越界（可以报错，不可以 UB）——FSCK 的存在让这个 target 特别有价值。
3. 验收：累计 ≥ 24 CPU 小时无未修复崩溃；每个被 fuzz 发现并修复的 bug 都要固化为
   常规回归测试用例。发现 0 个 bug 不是失败——但语料与预算证据必须真实。

## 阶段四：性能 SLO（1–2 版）

1. 用现有 benchmark 二进制扩展出可重跑的负载矩阵：N 并发客户端 × 读写比 × 值大小，
   在 VM 与开发机各测一轮；提交 p50/p99 延迟与 ops/s 基线（数字来自测量，不来自期望）。
2. SLO 写入版本库 + 复测脚本；复测须在声明方差内可复现。性能回归 gate 先 warn-only，
   稳定两个版本后转为硬门。idle-timeout 与高并发的交互至少覆盖一个用例。

## 阶段五：soak 参与 + 收尾（1 版 + soak 期）

1. 14 天 soak 期间：mini-kv 服务持续运行于 VM；每周一次 kill-恢复或备份-恢复演练回执；
   Node monitoring loop 的 PING 记录即是 mini-kv 的存活证据。
2. 收尾：`docs/stage2-minikv-final-evidence.md`（演练回执、fuzz 预算与语料证据、SLO
   基线、发布包 digest），请求 Claude 终审（终审会重跑一次恢复演练与一次负载复测）。

## 失败条件

- 演练在"安静点"kill、或恢复校验只对 key 计数不对值 digest = 回执无效。
- fuzz 语料/预算不可复现（没有提交语料、没有 CI 记录）= 不算证据。
- SLO 数字无法用提交的脚本复现 = 不算证据。
- 服务对公网直接可达（未经代理与认证）= 阶段失败。
- Stage-1 任何 ratchet/floor/门回退 = 版本无效。
