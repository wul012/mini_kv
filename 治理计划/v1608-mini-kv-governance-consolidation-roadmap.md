# v1608 mini-kv 治理收口路线图

状态：active planning document。
执行侧：mini-kv / C++。
来源背景：只读读取 Node v2114 governance consolidation roadmap、Codex migration playbook，以及 `docs/plans2/v2114-governance-consolidation-pointer.md`。Node v2114 明确 Java 与 mini-kv 是 recommended parallel，不等待 Node；同时警告 Node 里已经存在大量指向 Java / mini-kv 归档树的绝对路径和 digest 引用，所以 sibling 项目的历史 archive 与 evidence JSON 不能被移动或重命名。

## 本路线图解决什么

Claude 的跨项目审计指出 mini-kv 的功能核心仍然干净，但治理证据、归档和版本化文档增长已经进入需要约束的阶段。本地核对结果如下：

- `src/command.cpp`：约 50KB，1072 行。
- `src/shard_readiness.cpp`：约 50KB，634 行。
- `src/`：304 个源文件。
- `include/minikv/`：258 个头文件。
- `tests/`：336 个测试文件。
- 仓库索引文件总数：9592。
- `e/` 归档根：5153 个文件，约 1,065,416,900 字节。
- `f/` 归档根：当前 25 个文件，约 979,961 字节。

这些数字说明两个问题。第一，`command.cpp` 和 `shard_readiness.cpp` 已经重新接近需要拆分的阈值，后续不能继续把新命令和新 evidence section 往里面堆。第二，`e/` 的体积已经不能通过“搬目录”来解决，因为历史路径可能被 Node 固定引用；正确动作是冻结历史、建立索引、停止无意义增长，并把未来归档写入明确的 active root。

## 本路线图不是什么

这不是一个新运行时功能版本。它不新增命令，不改 WAL，不改 snapshot，不改 restore，不改 TCP，不改 RESP，不改 `SHARDJSON` 输出，不执行 `LOAD` / `COMPACT` / `SETNXEX` / restore，不启动 Node / Java / mini-kv 服务，不读取 credential，不打开 raw endpoint，不创建 active router，不授予 execution authority，也不写 store。

这也不是一次历史归档清理。`a/` 到 `f/` 里的历史截图、解释、JSON evidence 和 fixture 不会被搬迁、重命名、压缩替换或删除。后续如果要释放磁盘空间，应先写单独的 owner approval / backup / external archive 方案；在没有明确授权前，mini-kv 仓库内的历史 evidence 只能索引和标记，不能破坏路径。

## 目标

1. 给 mini-kv 建立一条和 Node v2114 类似的治理收口线：先停止继续制造巨型文件和无边界归档，再分批做可验证拆分。
2. 把 `command.cpp` 的后续拆分从“凭感觉挪代码”改成“保持 public command API、输出字节、CHECKJSON/EXPLAINJSON/COMMANDSJSON 语义不变”的批处理。
3. 把 `shard_readiness.cpp` 的后续拆分从“继续堆 section”改成“按 section builder / source lineage / command catalog quality / current release summary 分层”。
4. 给 `e/` 和后续 active archive roots 定义保留策略：历史冻结、路径稳定、索引优先、禁止重排。
5. 给 Codex 后续执行留下可操作 playbook，避免再次进入“为了多版本而多版本”的细粒度循环。

## 约束

- 历史路径稳定性优先级高于目录整洁度。
- 不移动 `e/<version>/`，尤其不移动大尺寸 CLI smoke 截图；Node 可能引用这些路径和 digest。
- 不批量改名 `代码讲解记录_*`；旧材料通过 README / 索引串联。
- 不新增 echo / verification / readiness 长链，除非用户明确要求且有 necessity proof。
- 每个拆分版本必须保留原有命令名、函数签名、测试入口和 fixture 路径。
- 每个拆分版本要先做 focused parity，再做全量 CTest；如果只改文档，说明为何跳过构建。
- 每个非微小代码版本必须有 3000+ 中文讲解；如果讲不出 3000 字，就扩大真实工程工作，不硬凑。

## 拟议阶段

### Phase 0：路线图冻结（v1608）

本版本只新增治理路线图与执行 playbook，更新 README。它不改 C++ 行为、不改 fixture、不创建截图目录、不移动归档。完成标准是：

- 本路线图存在并记录量化基线。
- Codex playbook 存在并明确后续拆分顺序、验证命令和停止条件。
- README 记录 v1608 为治理收口计划版本。
- `git diff --check` 通过。

### Phase 1：command.cpp 分发拆分

目标是把 `execute_runtime_evidence_command` 的长 if-chain 进一步收敛为可维护结构。优先顺序：

1. 只读 evidence 命令 handler registry 或局部 helper 表。
2. usage 检查复用，减少每个命令重复写 `has_extra_token`。
3. 保留 `CommandProcessor::execute` 和 public command text 完全不变。
4. 迁移一小批最近新增的 long evidence 命令作为参考批次，而不是一次性重写全部。

验收：

- `COMMANDS` / `COMMANDSJSON` 对新旧命令可见性不变。
- `EXPLAINJSON` / `CHECKJSON` 对迁移命令输出不变。
- 迁移命令的 direct `CommandProcessor` 测试通过。
- CLI UTF-8 重定向 smoke 可执行，避免 PowerShell 管道首行污染误判。
- 全量 CTest 通过。

### Phase 2：shard_readiness.cpp section builder 拆分

目标是拆出当前 release summary、command catalog quality、source lineage、recent production-shard execution sections 等稳定子模块。拆分时不改变 `SHARDJSON` contract，不改 fixture 字段顺序，除非版本明确说明并同步测试。

验收：

- `fixtures/release/shard-readiness.json` 与 versioned fixture 的语义稳定。
- `tests/shard_readiness_tests.cpp` 中 current status、source freeze、command catalog quality、boundary 字段断言继续通过。
- 若必须改变字段顺序，必须用结构化断言证明语义不变，并在讲解中说明原因。

### Phase 3：archive retention policy

目标不是删 `e/`，而是写清楚以后怎么不继续失控：

- `e/` 标记为 historical frozen root。
- `f/` 继续承载当前 active versions，直到明确达到拥挤阈值。
- 后续如需要新 root，应新增 `g/<version>/图片/` 与 `g/<version>/解释/说明.md`，而不是重排旧目录。
- 每个 root 可以有小 README / index，记录版本范围和保留原因。
- 大图或长 smoke 截图允许在新版本中改为“精选截图 + 文本摘要 + 哈希”，但不能替换旧证据。

验收：

- 新增 retention 文档不触碰历史文件。
- README / AGENTS 的 active root 规则保持一致。
- Node pointer 中的“不要移动归档”警告被 mini-kv 本地计划吸收。

### Phase 4：final closeout

当 command 和 shard readiness 的第一轮拆分稳定后，再决定是否继续 Phase B：

- 如果最大文件已回到可读范围，停止。
- 如果只是为了降低文件数量而合并小文件，停止。
- 如果某类 helper 明显重复且有测试保护，再写 necessity proof 后继续。

## 停止条件

- 任何需要移动历史 archive 的方案必须停止。
- 任何会改变 Node 已引用路径或 digest 的方案必须停止。
- 任何会让 mini-kv 读取 Node f-folder、导入 Node route、启动 sibling 服务或执行生产动作的方案必须停止。
- 任何“为了填满版本数”的微小版本必须停止，改为加厚一个可解释版本。
- 任何拆分后缺少 focused parity test 的代码版本必须停止。

## 和 Node v2114 的关系

Node v2114 是 Node 内部 renderer consolidation，不需要 mini-kv 新证据，也不阻塞 mini-kv。mini-kv v1608 只是吸收它的治理经验：用路线图、playbook、ratchet/阈值、分批迁移和 closeout gate 控制增长。mini-kv 不参与 Node renderer 迁移，不修改 Node 文件，不验证 Node route 输出。

一句话总结：v1608 给 mini-kv 立下治理收口的施工图，后续拆 `command.cpp`、拆 `shard_readiness.cpp`、治理 `e/` 归档，都必须先保路径、保契约、保输出、保测试，再谈整洁。
