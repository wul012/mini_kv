# mini-kv 优雅第二轮 Pin 审计（v1662）

状态：READY FOR CLAUDE REVIEW。候选 CI：GREEN。E2-M2：NOT TRIGGERED。外部 verdict：PENDING。

## 审计问题

第一轮把长文件名 baseline 从 740 收紧到 735，但这不等于剩余 735 个文件名都值得继续改。本轮只回答一个有界问题：当前最极端的十个长名中，是否至少存在五个不受公开合同、CLI/JSON 字节、waiver 或专门 census 合同钉住的安全候选？只有答案为“是”才允许再做一版改名；否则必须保留债务并关闭本轮。

审计不靠手工挑选。`elegance_name_census_contract` 读取 `config/elegance-name-baseline.txt`，把文件名和公开 header 可见标识符合并，按以下稳定规则排序：

1. 长度降序；文件名长度取 stem，标识符长度取 token 本身。
2. 同长度时按类别升序，即 `F` 先于 `I`。
3. 类别仍相同时按完整名称升序。
4. 固定取前 10 条，不按预期结论换样本。

机械结果是 6 个文件名、4 个公开标识符。它们的详细数据保存在 `config/elegance-round2-pin-audit.tsv`，测试会逐行重算 rank、kind、length 和 name；任何 baseline 漂移、排序变化或手工替换候选都会让 CTest 失败。

## 前十名与 Pin 判定

| Rank | 类别 | 长度 | 候选 | 判定 | Pin / 证据 |
|---:|:---:|---:|---|---|---|
| 1 | F | 143 | `include/minikv/...compared_package_evidence_intake_audit_validation.hpp` | pinned | 公开 header 路径；完整路径见 TSV |
| 2 | I | 143 | `...compared_package_evidence_intake_audit_validation` | pinned | 标识符直接声明在 rank 1 的公开 header 中 |
| 3 | F | 139 | `include/minikv/...compared_package_evidence_intake_audit_guards.hpp` | pinned | 公开 header 路径 |
| 4 | F | 139 | `include/minikv/...compared_package_evidence_intake_audit_stages.hpp` | pinned | 公开 header 路径 |
| 5 | I | 139 | `...compared_package_evidence_intake_audit_guards` | pinned | 标识符直接声明在 rank 3 的公开 header 中 |
| 6 | I | 139 | `...compared_package_evidence_intake_audit_stages` | pinned | 标识符直接声明在 rank 4 的公开 header 中 |
| 7 | F | 138 | `include/minikv/...compared_package_evidence_intake_audit_slots.hpp` | pinned | 公开 header 路径 |
| 8 | I | 138 | `...compared_package_evidence_intake_audit_slots` | pinned | 标识符直接声明在 rank 7 的公开 header 中 |
| 9 | F | 132 | `include/minikv/...compared_package_evidence_intake_audit.hpp` | pinned | 公开 header 路径 |
| 10 | F | 132 | `tests/...review_closeout_audit_validation_tests.cpp` | safe | 私有测试路径；完整路径只在 `CMakeLists.txt` 注册一次 |

这里的 `pinned` 不表示名字优雅，也不创造新 waiver。它只表示本轮不具备公共 API alias、弃用周期和消费者迁移授权，不能把公开 header 路径或 namespace token 当作私有实现路径直接重命名。rank 10 则不同：它是测试翻译单元的构建身份，文件内容仍通过公开 header 调用验证逻辑；路径本身没有进入 CLI、RESP、JSON、fixture 或公共 include 表面，因此从技术上属于安全候选。

## 条件分叉

机械统计为：

```text
pinned = 9
safe = 1
required_for_E2_M2 = 5
branch = close
```

安全候选只有 1 个，小于简报要求的 5 个。按照 `治理计划/elegance-round2-minikv.md`，E2-M2 不触发，rank 10 也不单独改名。原因不是它不能改，而是单独为一个测试路径制造版本、归档和 CI 成本，无法形成足够高价值的第二刀；“最多两版”是预算上限，不是必须消费的配额。

## 需求—证据矩阵

| 要求 | 实现位置 | 会失败的机械证据 | 状态 |
|---|---|---|---|
| 从现有 baseline 选前十名 | `rank_baseline()` | TSV 任一 rank/长度/名称漂移会失败 | 完整 |
| 文件名与公开标识符均进入样本 | 合并排序得到 6F/4I | 合计或类别数不是 6/4 会失败 | 完整 |
| 每项有 pin 判定 | TSV 的 `decision/pin/mechanical_evidence` | 非法枚举、缺失路径或 header 中找不到标识符会失败 | 完整 |
| 安全候选有可复核依据 | rank 10 指向 `CMakeLists.txt` | 完整测试路径不是恰好注册一次会失败 | 完整 |
| 条件分叉不可主观改写 | `AuditSummary` 固定统计 9/1 | safe 不是 1 或 branch 不再 close 会失败 | 完整 |
| 跨平台文本不再漂移 | `.gitattributes` | census 同时要求 baseline 与 TSV 的 `eol=lf` 规则存在 | 完整 |
| 不改变运行合同 | 本版无 `src/`、`include/`、fixture 改动 | 351/351 CTest、1514-file manifest 与真实 CLI 边界 | 完整 |

## 明确不做

- 不重命名 rank 10 来凑一个“做了优化”的版本。
- 不为公开 header 临时发明短 alias，因为 alias 会扩大而不是缩小 API 表面。
- 不改 CLI 命令、RESP/JSON key、receipt 字节、release fixture 或历史归档路径。
- 不放松 735/883 baseline，也不把 pinned 条目移出 scanner 范围。
- 不进入 E2-M2，不创建第三个本轮版本，不提前打开 Stage 2 运行实态。

## 最强质疑

最强质疑是：“前十名中有多个文件路径和 namespace 来自同一 compared-package 家族，是否属于重复采样，从而人为降低 safe 数量？”答案是：样本由合并排序算法直接产生，测试锁定 tie-break，执行者没有按家族去重或挑选的自由。简报要求审计的是最长的“名”，不是最长的十个业务概念；文件路径和公开 namespace 分别代表两种迁移成本。若人为按家族去重，反而会改变已授权的候选定义并把低排名对象抬入样本。当前结论因此是显著但有限的阴性结果：它能证明第二刀阈值未达到，不能证明 735 个存量长文件名永远没有其他安全候选。

## 复现命令

```powershell
cmake -S . -B cmake-build-review -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-review --target minikv_elegance_name_census --parallel 4
cmake-build-review\minikv_elegance_name_census.exe
ctest --test-dir cmake-build-review -R '^elegance_name_census_contract$' --output-on-failure
```

预期摘要为 `files=735 public_ids=883 budget=40 pin_audit=9/1 branch=close`。执行者只提交这个可复核结论，不替 Claude 授予 Round 2 PASS。

候选提交 `92db8fcf` 已通过 GitHub Actions run [`29189480986`](https://github.com/wul012/mini_kv/actions/runs/29189480986)。七个 job 全部 success：archive inventory、ubuntu coverage、ubuntu-latest、ubuntu sanitizer、windows-latest、clang-format changed files、macos-latest。Windows lane 在真实 checkout 下通过拒绝 CR 的 TSV 合同，证明 LF 规则不只是本机声明。该远端证据不替代外部 Claude verdict。
