# v1631+ 任务简报：把 OSFS 课程设计从 v1630 草稿推进到真正满足“实验二 Linux 二级文件系统”的最终交付

> 执行状态：已执行完毕。v1631-v1633 已完成二级 MFD/UFD、磁盘用户表与认证、fd 位移读写、交付重做，并通过独立复核；本文件保留为只读历史执行依据。

本简报由用户交给 Codex 执行，是本轮工作的权威任务指令。审计背景：Claude 已逐条核对过 v1629/v1630 的代码与交付材料，本简报中的缺口描述以代码事实为准；执行完成后 Claude 会做独立复核（重新构建、跑测试、跑真实 demo、审报告），所以所有验收证据必须可独立复现。

## 背景与用户指令

用户对 v1629/v1630 的 OSFS 课设交付**不满意**，明确授权：继续执行，直到课设实质达标为止。这条用户指令优先于 Node 计划轮转——本轮无论 `D:\nodeproj\orderops-node\docs\plans` 轮到谁，都要推进 mini-kv 的 OSFS 课设（仍按 AGENTS.md 只读参考该计划目录，但不得因此停止）。

AGENTS.md 中新增的 **Substantive Deliverable Quality Gate**（commit 770d2aaa）正是针对 v1630 这种“材料齐全但实质不达标就打包”的情况写的。本轮必须严格执行该 gate。

## 起点状态

- 开始前先 `git log --oneline -10` 确认当前状态。本简报写作时 HEAD 是 `770d2aaa`，工作树除本简报和 `治理计划/README.md` 的索引行外应是干净的；这两个文件随第一个功能版本一起提交。
- 如果发现已有 v1631+ 的 OSFS 相关提交（例如另一个会话已开工），先读懂已完成的部分，从中断处继续，不要重做或回退已验证的工作。
- 开始前自行核对下文“实质缺口”描述与代码现状是否一致。

## 课程要求（验收基准）

原始课程 PDF 不在仓库内。已知的要求引文在 `docs/osfs-coursework-delivery-v1630.md`：

> “实验二 Linux 二级文件系统”要求用二进制文件模拟磁盘空间，保存用户信息、inode 信息、超级块信息和文件信息，实现 `dir`、`create`、`delete`、`open`、`close`、`read`、`write`、`login` 等命令，并支持源文件读写保护。

“二级文件系统”是经典操作系统课设题：**二级目录结构 = MFD（主文件目录：用户名 + 指向该用户 UFD 的入口）+ UFD（每个用户自己的文件目录）**。这是题目的定义性特征。

## 当前实现的实质缺口（Claude 已逐条对照代码核实）

1. **没有二级目录结构（最大缺口）**。`include/minikv/osfs/filesystem.hpp` 只有单一 root directory（`SuperBlock::root_inode`，`list_directory()` 无路径参数，`DirectoryEntry` 扁平）。所有用户的文件混在一个目录里。评分老师第一眼就会发现“二级文件系统”没有二级目录。
2. **用户信息没有保存在磁盘镜像里**。`uid_for_user(user)` 直接从用户名派生 uid；没有磁盘上的用户表，没有密码。`LOGIN guest` 不做任何认证，与“保存用户信息……实现 login”不符。
3. **fd 没有读写指针**。`CommandProcessor::Handle` 只有 `{name, readable, writable}`；READ/WRITE 是整文件读写。课设期望 open/close/read/write 带读写位移（顺序读、追加/续写）。
4. v1630 的报告、演示脚本、transcript、PDF 都是围绕上述不完整实质打包的，需要在功能补齐后**全部重新生成**。

## 本轮要做什么（验收门槛）

按 AGENTS.md 的版本纪律拆成 2~3 个连贯版本（建议拆法，可按你判断调整，但最终验收门槛不变）：

### 功能版本 A：二级目录 + 磁盘上的用户表 + 真正的 login

- 磁盘镜像内新增用户表（superblock 版本号提升；用户表占专用块或保留 inode，具体布局你定，但必须真正落盘、重开磁盘后仍在）。
- 用户记录至少含：用户名、uid、密码（课程级简化哈希即可，报告中注明是教学简化）、指向该用户 UFD 的 inode。
- 实现 MFD/UFD 二级目录：每个用户有自己的 UFD（真实的磁盘目录结构，有自己的 inode 和数据块，不是内存 hack）。`create/delete/open` 作用于当前登录用户的 UFD。
- `login <user> <password>`：密码错误必须失败；成功后切换当前用户与当前 UFD。格式化时预置至少 root + 2 个普通用户（或提供 `useradd`，你选）。
- `dir`：列当前用户 UFD（文件名、inode、物理块号、保护码、owner、长度）。可选 `dir <user>` 按权限查看他人目录。
- 读写保护跨用户仍然有效：A 用户不能写 B 用户 0600 的文件；用真实命令验证。

### 功能版本 B：fd 读写指针

- `Handle` 增加读/写位移；`read <fd> [len]` 从当前位移读并推进；`write <fd> <text>` 支持追加/续写语义；`open` 模式决定初始位移。保持课程级简单性即可，不要过度设计。

### 交付版本 C：重新生成全部课设材料

- 重新生成报告 docx（`tools/build_osfs_coursework_report.py`）+ LibreOffice 导出 PDF：总体设计、磁盘布局（含用户表和 UFD 块）、数据结构、核心算法（login、二级目录查找、位移读写）、测试章节全部更新；三张结构图如与新布局不符必须更新。
- 重新录制真实 demo transcript（用重新构建的 `minikv_osfs`，演示脚本必须包含：格式化、login 成功/失败、二级目录隔离、权限拒绝、位移读写、dir 输出）。
- 新交付目录（例如 `课程设计交付/v<版本>-osfs-final/`），旧的 `课程设计交付/v1630-osfs-final/README.md` 顶部加一行“已被 v<版本> 取代（superseded）”，其余文件原地保留，不删除不改名。
- `演示步骤.md`、`提交清单.md` 同步更新。

### 明确不做（防止镀金）

- 不做多级（>2 级）目录、不做间接块（8 个直接块限制保留）、不做 fsck、不做崩溃恢复日志——这些继续写进报告的“设计边界与后续改进”。
- 不碰 mini-kv 的 KV/WAL/snapshot/TCP/RESP 主线，不碰 Node/Java/aiproj。

## 测试与证据要求

- `tests/osfs_tests.cpp` 与 `osfs_cli_smoke` 扩展覆盖：错误密码登录失败、二级目录隔离（A 建的文件不出现在 B 的 dir 里）、跨用户权限拒绝、位移读写、重开磁盘后用户表和 UFD 仍在。
- 每个版本：CMake 重建（源码变更后禁止用旧 exe）、全量 CTest、真实 CLI smoke、按 `f/<版本>/图片/ + 解释/说明.md` 归档、`代码讲解记录_生产雏形阶段_第四册/` 中文讲解（≥3000 字规则照旧）、干净工作树、commit + tag + push。
- 本机工具链按 AGENTS.md：CLion 捆绑 CMake/MinGW，先把 `D:\CLION\CLion 2025.1.1\bin\mingw\bin` 加进 PATH。

## 最终收尾（必须做完才算结束）

1. 按 Quality Gate 输出**需求-证据矩阵**：逐条列出课程要求 → 实现位置 → 测试/演示证据 → 状态（完整/部分/豁免），写进交付目录或 docs。
2. 对抗性自审：写出“评分老师最强的一条质疑”，并给出已闭环的回应。
3. 最终回复里简要说明：读取了哪个 Node 计划文件、各版本号与提交、需求-证据矩阵结论、还有什么已披露的边界，方便 Claude 复核。
