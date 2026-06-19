# 代码讲解记录：生产雏形阶段第四册

本册从 v1565 之后启用，用来承接新的 mini-kv 代码讲解记录。第三册已经过度集中，后续不再追加。

## 当前状态

- v1625: [993-version-1625-runtime-receipts-consolidation-design-gate.md](993-version-1625-runtime-receipts-consolidation-design-gate.md) - 对 28 个 runtime receipt 文件和 26 个 credential-resolver receipt 文件做量化 census，冻结 ordered zero-dependency builder、fixture 子对象 byte parity、分批迁移顺序和停止条件，并用 `receipts_consolidation_note_contract` 守住设计，不改 runtime formatter、fixture、digest 或执行边界。
- v1624: [992-version-1624-project-orientation-executable-examples.md](992-version-1624-project-orientation-executable-examples.md) - 将 v1623 的通俗导览示例接入真实 `Store` + `CommandProcessor` 执行路径，新增 `project_orientation_examples_tests`，证明 `SET`/`GET`/`INFOJSON`/`CHECKJSON LOAD`/`SMOKEJSON` 的输入输出与边界解释一致，同时守住 no LOAD execution、no store replacement、no WAL touch、no service startup、no router、no write expansion、no execution authority。
- v1620: [990-version-1620-production-excellence-k5-archive-retention-inventory.md](990-version-1620-production-excellence-k5-archive-retention-inventory.md) - 完成 production-excellence K5 归档保留库存：新增 `docs/archive-retention-index.md` 和 `scripts/archive_inventory.py`，给 `a/`-`f/` 历史根建立冻结/活动状态与容量提醒，并在 CI 中以 warning-only 方式守住前向归档预算；K4 后复测证明 gcov negative-hits workaround 仍需保留，因此继续由契约测试保护。
- v1619: [989-version-1619-production-excellence-k4-shard-readiness-split.md](989-version-1619-production-excellence-k4-shard-readiness-split.md) - 完成 production-excellence K4 的 shard-readiness 拆分：`shard_readiness.cpp` 退成公开 fixture-path shim，SHARDJSON 拼装、digest 证据链、边界/catalog helper 分别进入独立 translation unit，并把 `shard_readiness_boundary_field_names.cpp` 的后续 E9 迁移豁免写清楚，保持 read-only/no-router/no-write/no-WAL/no-execution 不变。
- v1617: [987-version-1617-production-excellence-k4-command-dispatch-split.md](987-version-1617-production-excellence-k4-command-dispatch-split.md) - 启动 production-excellence K4 第一刀：把 `execute_trimmed` 拆到 `command_dispatch.cpp`，把 parse/error helper 和 WAL gate 拆成内部模块，并把 coverage filter 跟进到新 command 核心文件，保持命令输出、fixture、WAL、snapshot、TCP/RESP 和执行边界不变。
- v1618: [988-version-1618-production-excellence-k4-command-family-executor-split.md](988-version-1618-production-excellence-k4-command-family-executor-split.md) - 继续 K4 command 侧拆分：`command_dispatch.cpp` 降为薄分发器，string/expiry/persistence/introspection 四类命令执行体分别进入独立 translation unit，同时更新 CMake、coverage filter 和 CI contract test，保持 public command output、CHECKJSON、WAL/snapshot、CLI/TCP 行为不变。
- v1616: [986-version-1616-production-excellence-k3-zero-dependency-logger.md](986-version-1616-production-excellence-k3-zero-dependency-logger.md) - 完成 production-excellence K3 零依赖分级日志：新增 `minikv::Logger`，给 CLI/server 接入 `--log-level`，把 TCP 运行日志导向 stderr，同时保持 stdout 证据契约不变。
- v1615: [985-version-1615-post-k2-actions-runtime-readiness.md](985-version-1615-post-k2-actions-runtime-readiness.md) - 收掉 K2 review 后的 GitHub Actions Node 20 runtime 时间敏感提醒，升级 checkout/upload-artifact action tag，并用 CTest 防止旧 action tag 回归。
- v1614: [984-version-1614-production-excellence-k2-coverage-floor-closure.md](984-version-1614-production-excellence-k2-coverage-floor-closure.md) - 冻结 v1613 远端 coverage artifact 的 90% core baseline，并把 Ubuntu coverage CI 升级为 `--fail-under-line 88` 硬门槛，完成 K2 后等待 Claude review。
- v1613: [983-version-1613-production-excellence-k2-coverage-report-hardening.md](983-version-1613-production-excellence-k2-coverage-report-hardening.md) - 修复 v1612 远端 coverage report 在 GCC/gcov negative branch-hit 输出上的 `gcovr` parse failure，保留 required coverage job 和 full CTest，只把该已知工具链问题降级为 warning。
- v1612: [982-version-1612-production-excellence-k2-coverage-baseline.md](982-version-1612-production-excellence-k2-coverage-baseline.md) - 启动 production-excellence K2 coverage baseline，新增 `MINIKV_COVERAGE`、Ubuntu coverage CI 与 core `gcovr` artifact，明确 Windows MinGW coverage path-limit fast-fail，并等待远端 baseline 后再固化 floor。
- v1611: [981-version-1611-production-excellence-k1-required-sanitizer-gate.md](981-version-1611-production-excellence-k1-required-sanitizer-gate.md) - 将 v1610 已跑绿的 Ubuntu ASan/UBSan 诊断轨道转成 required gate，测试禁止 `continue-on-error` 静默回归，并保持运行时命令、归档路径、写入、WAL 与执行边界不变。
- v1610: [980-version-1610-production-excellence-k1-sanitizer-bootstrap.md](980-version-1610-production-excellence-k1-sanitizer-bootstrap.md) - 完成 production-excellence K1 sanitizer 诊断引导：新增 `MINIKV_SANITIZE`，接入 Ubuntu ASan/UBSan CI 诊断轨道，收敛 Windows MinGW sanitizer runtime 限制，并保持默认构建、运行时命令、归档路径、写入与执行边界不变。
- v1609: [979-version-1609-production-excellence-k0-quick-wins.md](979-version-1609-production-excellence-k0-quick-wins.md) - 完成 production-excellence K0 快速胜利：刷新 START_HERE、建立本地进度账本、将当前 runtime archive hint 改为 CMake 配置但保持 `c/102/` 输出不变、统一最后一个 include guard、接入 changed-file clang-format CI，并保持 no contract / no archive move / no execution 边界。
- v1607: [978-version-1607-f-folder-explanation-quality-closeout.md](978-version-1607-f-folder-explanation-quality-closeout.md) - 将 f-folder 解释质量收口成一个单大版本证据层，新增 `SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON`，冻结 v1606 为来源，写入中文讲解、三千字下限、禁止硬凑、字数不够就加大版本工作量、截图目录必须有真实截图、mini-kv 不扫描 Node f-folder、不导入 Node 质量路由、不启动服务、不路由写入、不触 WAL、不允许执行等边界。
- v1602-v1606: [977-version-1602-1606-production-shard-execution-external-artifact-dry-run-closeout.md](977-version-1602-1606-production-shard-execution-external-artifact-dry-run-closeout.md) - Node v2093 dry-run closeout is frozen as mini-kv read-only evidence with five fixtures, split modules, f-root per-version archives, and no external artifact parse/store/execute, router, write, WAL, or execution authority.
- v1597-v1601: [976-version-1597-1601-production-shard-execution-mini-kv-owner-receipt-bundle.md](976-version-1597-1601-production-shard-execution-mini-kv-owner-receipt-bundle.md) - The v1596 owner receipt request packet is satisfied only for mini-kv-owned abort, rollback, lifecycle, and cleanup-no-start proof slots, with split modules, five versioned fixtures, f-root command archives, and no Java, cross-project, router, write, WAL, or execution authority.
- v1592-v1596: [975-version-1592-1596-production-shard-execution-owner-receipt-request-packet.md](975-version-1592-1596-production-shard-execution-owner-receipt-request-packet.md) - Node v2087 owner receipt request gaps are frozen as local mini-kv request-only evidence with split modules, five versioned fixtures, per-version command archives, and no signed receipt, router, write, WAL, or execution authority.
- v1586-v1591: [974-version-1586-1591-code-walkthrough-quality-gate-non-participation.md](974-version-1586-1591-code-walkthrough-quality-gate-non-participation.md) - Node v2077 documentation-quality closeout is recorded as a local mini-kv walkthrough quality gate with one read-only command, six versioned fixtures, split modules, and no sibling file scan or execution authority.
- v1566-v1585: [973-version-1566-1585-runtime-execution-packet-approval-gate-archive-verification-non-participation.md](973-version-1566-1585-runtime-execution-packet-approval-gate-archive-verification-non-participation.md) - Node v399 runtime execution packet approval gate archive-verification context is frozen as mini-kv read-only non-participation evidence with split modules, fixtures, tests, and boundary assertions.

可以没有讲解：如果某轮只是索引、归档说明、链接修复、文档治理或极小维护，且没有新增代码语义，可以不写代码讲解。

但只要写讲解，必须参照：

```text
D:\nodeproj\orderops-node\代码讲解记录\107-production-readiness-summary-v3-v103.md
```

对应模板在：

```text
代码讲解记录_治理/production-readiness-walkthrough-template.md
```

## 新增条目格式

```markdown
- vNNNN: [973-version-NNNN-topic.md](973-version-NNNN-topic.md) - one-sentence summary.
```

## 写作底线

- 开头说明目标、角色、为什么做，以及“不是什么”。
- v1607 起，mini-kv 非微小版本的代码讲解默认中文书写，每个版本至少保留一篇三千字级别讲解。
- 如果讲解无法自然达到三千字级别，说明本版工作量不够，应继续加厚实现、测试、fixture、归档、重构或边界说明，禁止硬凑。
- 必须讲入口、响应/证据结构、核心流程、边界字段和测试覆盖。
- 测试部分不能只列命令，要说明断言保护什么行为。
- 不要把纯归档输出、截图清单或字段清单伪装成代码讲解。
- 单册超过 150 篇或 README 超过 50KB 时，优先开下一册。
