# mini-kv Production Excellence Track Evidence (v1658)

Status: READY FOR EXTERNAL REVIEW

External verdict: PENDING

本文件是 `治理计划/v1637-production-excellence-completion-brief.md` 阶段三的轨道证据入口。这里的
`GREEN` 只表示 mini-kv 执行方已找到可复现的机械检查并完成本地自审，不表示 Claude 或其他外部
评审者已经批准。对外成熟度仍严格表述为 **single-project validation + cross-project contract alignment**；
仓库没有把冻结 fixture、Node/Java 回显合同或本地 CLI smoke 冒充成真实四项目联合运行。

## Final-State Baseline

- K0-K6 均已完成；对应版本、原始检查点与偏差记录位于
  `docs/production-excellence-progress.md` 和只读上级 playbook。
- receipts 收敛状态为 **28 sources / 27 formatter owners / 27 builder-backed / 0 pending / 1 named no-formatter census exemption**。
  `receipt_builder_census_contract` 会在 owner 数变化、builder floor 回退、pending 重新出现或命名例外消失时失败。
- v1657 已把 README、START_HERE、能力快照、CHANGELOG、OSFS 简报状态和归档增长重新对到当前树；
  `project_docs_honesty_contract` 负责阻止这些指针静默漂移。
- v1658 把 TCP 服务默认绑定从外网可见的 `0.0.0.0` 收紧为 loopback `127.0.0.1`，并用真实
  `TcpServer::Options` 断言保护该默认值。显式传入其他 host 的能力仍保留，但安全文档要求外部网络暴露
  必须另行提供认证、TLS 与防火墙。
- 新鲜覆盖率证据来自绿色 GitHub Actions run
  [29095734744](https://github.com/wul012/mini_kv/actions/runs/29095734744)：
  **2345 core lines / 2122 executed / 90%**。简报规定实际值比旧 floor 高至少 2 分时只升不降，因此
  v1658 将门槛从 88 收紧为 `--fail-under-line 90`。

## E1-E10 Gate Matrix

| Gate | Self-audit | Mechanical check | Reproducible evidence |
|---|---|---|---|
| E1 Build & CI | GREEN | `.github/workflows/ci.yml` 在 push/PR 上运行 Linux、macOS、Windows 全构建与 CTest，另有必需的 sanitizer、coverage、format、archive jobs | `gh run view 29095734744`; 本地执行本文末尾的 configure/build/CTest 命令 |
| E2 Static analysis | GREEN | `clang-format changed files` job 调用 `scripts/check_clang_format_changed_files.py`; `minikv_dependabot_config_tests` 防止 job 或 action 版本静默弱化 | `python scripts/check_clang_format_changed_files.py --base v1657 --head HEAD`；CI format job |
| E3 Coverage | GREEN | Ubuntu gcovr 对 store/command/WAL/snapshot/RESP 核心集合执行全量 CTest并强制 `--fail-under-line 90`; workflow contract 固定 filter、floor 与 artifact | run 29095734744 的 `ubuntu coverage` log：2345/2122/90%；v1658 CI 必须再次通过收紧后的 floor |
| E4 Security & config | GREEN | `minikv_track_final_evidence_contract` 对 executable/config surfaces 做高置信秘密模式扫描；TCP 默认 loopback；logger 默认 warn；`docs/SECURITY.md` 给出威胁模型和非生产边界 | `ctest -R minikv_track_final_evidence_contract`; `minikv_tcp_server_tests`; 阅读 `docs/SECURITY.md` |
| E5 Observability | GREEN | 零依赖 logger 提供 error/warn/info/debug、UTC、thread id；TCP log 带 connection_id；HEALTH 和周期 metrics 暴露连接、命令、错误与延迟 | `minikv_log_tests`, `minikv_tcp_server_tests`, `minikv_command_tests`; CLI `HEALTH`/`STATSJSON` smoke |
| E6 Error handling | GREEN | SIGINT/SIGTERM 置停机标志，`request_stop()` 关闭 accept 循环；请求上限、空闲/半命令超时和连接回收均有真实 socket 测试 | `minikv_tcp_server_timeout_limit_tests`, `minikv_tcp_server_tests`, WAL/snapshot corruption tests |
| E7 Docs honesty | GREEN | `project_docs_honesty_contract` 对当前版本、测试数、OSFS 实现/测试、归档与简报状态做文件级核对；本文件拒绝联合运行和生产安全的过度主张 | `ctest -R project_docs_honesty_contract`; `docs/project-docs-honesty-matrix.md`; `docs/SECURITY.md` |
| E8 Release discipline | GREEN | CHANGELOG 明确 CMake `0.102.0` 兼容流与 git `vNNNN` 交付流；每版 commit/tag/push，final tag 和 CI 由外部评审复核 | `git show --check v1658`; `git tag --points-at v1658`; `gh run list --workflow CI` |
| E9 Code health | GREEN | final-evidence contract 对 `src/`、`include/` 非空行执行 800 行 ratchet，仅接受一个已评审 registry waiver；receipt builder census 固定 27/0/1 | `ctest -R "minikv_track_final_evidence_contract|receipt_builder_census_contract"`; `src/command.cpp` 为 732 非空行 |
| E10 Archive retention | GREEN | stdlib-only inventory 脚本、CI 8 MiB 最新版本预算警告、root ceilings 和冻结路径索引；历史 `a/`-`f/` 与 release fixture 不迁移 | `python scripts/archive_inventory.py --budget-mib 8 --strict`; `docs/archive-retention-index.md`; CI archive job |

## Program-Level Deviation And Waiver Ledger

**Exactly two program-level deviations require external acceptance.** 它们都来自原始 K2/K4 评审记录，
并由当前机械门限制范围；本版没有为了制造全绿而新增模糊豁免。

1. **E9 registry-only source-size waiver:** `src/shard_readiness_boundary_field_names.cpp` 当前约 829 个非空行。
   它是按历史字段顺序维护的静态边界名称注册表，不是控制流或执行入口。K4 评审明确接受保留，条件是
   程序终审引用原记录；`minikv_track_final_evidence_contract` 只允许这个精确路径超过 800，任何第二个文件
   超限会失败，该文件回落到阈值内时陈旧 waiver 也会失败。原始记录：
   `docs/production-excellence-progress.md` 的 K4 v1619/review 段与上级
   `production-excellence-minikv-playbook.md` K4 review row。
2. **E3 gcov/gcovr negative-hits workaround:** CI 保留
   `--gcov-ignore-parse-errors negative_hits.warn_once_per_file`。v1620 曾在 commit `f35dd0d3` 删除它，
   GitHub Actions run
   [27428515080](https://github.com/wul012/mini_kv/actions/runs/27428515080) 随即在
   `src/command_catalog.cpp` 的 gcov negative branch hit 上失败；恢复后覆盖率 build、CTest、report 与 floor
   仍全部必需。workflow contract 和本终审 contract 都要求参数保持精确，不能扩大为通用 parse-error 忽略。

**No third program-level waiver is claimed.** `runtime_credential_resolver_execution_denied_retention_receipts.cpp`
的 “1 named no-formatter census exemption” 只说明该源文件不拥有 public formatter，因此不属于 27 个迁移 owner；
它不是 E1-E10 质量豁免。Windows MinGW 无 ASan/UBSan 和本仓库 gcov sidecar 路径过长属于本地工具链
适用性说明；必需的 Ubuntu sanitizer/coverage jobs 仍会失败并阻断 CI，所以也不是放行红灯的 waiver。

## Requirement-To-Evidence Closure

| Completion-brief requirement | Implementation | Failing proof | State |
|---|---|---|---|
| 收敛全部 receipt owners | ordered JSON builder + 27 formatter owners | `receipt_builder_census_contract` + frozen family parity tests | closed in v1656 |
| 文档对当前树诚实 | v1657 README/START_HERE/capability/changelog/archive/OSFS reconciliation | `project_docs_honesty_contract` | closed in v1657 |
| E1-E10 逐门证据 | 本文件、CI workflow、SECURITY/TESTING、source-size/secret scan | `minikv_track_final_evidence_contract` | ready for external review in v1658 |
| coverage 只升不降 | 90% 新鲜基线，workflow floor 90 | `minikv_dependabot_config_tests` + Ubuntu coverage job | tightened in v1658 |
| 不夸大联合成熟度 | 明示 single-project + contract alignment | 本文件 contract 要求固定措辞 | closed locally; capstone belongs to Node program |

## Adversarial Self-Review

最强质疑不是“测试够不够多”，而是“大量治理证据是否掩盖了一个仍非生产数据库的事实”。答案必须是：
mini-kv 的核心存储、WAL、snapshot、RESP/TCP、恢复和 OSFS 教学层都有真实可执行测试，但它仍没有认证、
TLS、复制、多进程写协调、网络分区恢复或生产密钥管理；这些缺口在 `docs/SECURITY.md` 和
`docs/TESTING.md` 中明确列出。E4 的 GREEN 表示当前声明范围内的默认配置和边界是安全且受检的，不表示
项目可以直接暴露到不可信网络。跨项目方面，Node/Java fixture 与 digest 对齐也不等于 live integration；
只有上级 acceptance program 的 C1-C4 capstone 真正启动并消费 sibling runtime 后，系统级表述才可升级。

第二个质疑是 E9 是否用 “约 800 行” 偷换物理行数。终审门采用 K4 评审已经使用的非空物理行口径：
`src/command.cpp` 为 732，唯一 registry 为约 829 并逐路径豁免。脚本会阻止第二个超限文件，也会在豁免
不再需要时要求删除陈旧记录。因此这里不是把 894 个含空行/包含行的文本行隐藏掉，而是保持同一评审口径。

## Reviewer Reproduction Commands

```powershell
$env:PATH = 'D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
$cmake = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
$ctest = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\ctest.exe'
& $cmake -S . -B cmake-build-review -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
& $cmake --build cmake-build-review --parallel 4
& $ctest --test-dir cmake-build-review -R 'minikv_track_final_evidence_contract|project_docs_honesty_contract|receipt_builder_census_contract|minikv_dependabot_config_tests|minikv_tcp_server_tests|minikv_tcp_server_timeout_limit_tests|minikv_log_tests' --output-on-failure
& $ctest --test-dir cmake-build-review --output-on-failure --timeout 120 --progress
python scripts/archive_inventory.py --budget-mib 8 --strict
git show --check v1658
gh run list --workflow CI --limit 3
```

外部评审应独立执行这些命令、打开 CI coverage log 核对 TOTAL 行、确认 `v1658` tag 指向最终提交，并检查
工作树干净。完成这些动作之前，本文件的 `External verdict` 必须保持 `PENDING`。
