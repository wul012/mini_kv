# v1672-v1674 九分优雅收口计划

## 目标与评分边界

本计划把“做到 9 分”解释为可复现的内部工程门槛，不把版本数量、文档长度或主观观感
当作分数。满分 10 分，按合同安全 2.0、架构职责 2.0、重复与数据建模 1.5、构建与测试
设计 1.5、防回流机械门 1.0、跨平台 CI 1.0、文档证据 0.5、范围克制 0.5 计分。
候选总分必须达到 9.0；只要 fixture/public output 漂移、全量 CTest 不绿、七条 CI 未绿、
ratchet 放松或历史归档被移动，最高只能记 8.9，不能宣布到点。

本轮不重开第三轮 retro naming。v1662 已证明最长名 top 10 中 9 个由公共 header/API
钉住，剩余一个安全候选不足以形成有意义批次。当前最高收益热点是两种结构重复：

- `src/command.cpp` 仍以约 60 个同构 `if (command == ...)` 分派只读 evidence formatter；
- 根 `CMakeLists.txt` 有 2,596 行，v1671 时有 314 个产品源和 342 个普通测试注册与策略混排；
  v1672 合法新增一个 dispatch 翻译单元，因此 v1674 的现实基线是 315，不沿用旧数字。

## v1672：只读 evidence 命令表

Family 设计说明（先于实现，8 行）：

1. 抽象名：`EvidenceCommand`，表示外部命令名与无参数 formatter 的一条映射。
2. 数据边界：命令名和函数指针只存在于私有 `.cpp` 表中。
3. 行为边界：查表只返回 formatter，不提前执行 formatter。
4. 参数校验仍由 `CommandProcessor` 在调用 formatter 前完成。
5. STATS/INFO、带 key 的 route 命令和 WAL 维护仍由原 owner 处理。
6. 表内命令必须唯一、必须存在于公开 command catalog。
7. 未知命令仍返回原错误；usage、响应字节和执行边界不变。
8. 这不是新 router、插件系统、动态注册或执行入口。

验收：冻结迁移前命令集合和真实 CLI 输出摘要；把无参分派改为数据表；`command.cpp`
同构分支显著下降；在测试架构门中加入表项唯一性、catalog 完整性、command.cpp 体积与
分支数量 ratchet；focused/full CTest 和真实 CLI 前后字节摘要相等。

## v1673：测试注册 manifest

Family 设计说明（先于实现，8 行）：

1. 抽象名：`TestCaseManifest`，一行只描述 phase、kind、target、CTest name、source。
2. 数据边界：普通 linked/source-dir case 的顺序和身份在一个 LF 清单中。
3. 行为边界：`MinikvTesting.cmake` 负责解析、校验并调用现有注册 helper。
4. 342 个 linked case 的 CTest 名称、顺序、source、target 必须逐项不变。
5. `pre_smoke/main` phase 只恢复特殊 smoke 两侧的原相对顺序；script smoke、standalone
   test 和 target-specific override 仍显式留在构建模块。
6. 清单拒绝未知 kind、重复 target/name、缺失 source、列数漂移和 CRLF。
7. coverage/sanitizer 旧拓扑与默认 8-shard 拓扑继续消费同一清单。
8. 这不是删除测试、合并测试进程或改变期望。

验收：根脚本删除数千行重复调用；manifest census 精确等于 342；前后 `ctest -N` 名称与
顺序逐项相同；故障注入证明重复、缺文件、未知 kind 会失败；默认和 legacy focused
拓扑都通过。

## v1674：产品源清单与九分门

Family 设计说明（先于实现，8 行）：

1. 抽象名：`ProductSourceManifest`，只声明 `minikv` 静态库拥有的翻译单元。
2. 数据边界：每行一个 `src/*.cpp`，固定 LF，保留现有链接输入顺序。
3. 行为边界：CMake loader 校验路径、扩展名、重复项和未登记产品源。
4. 五个 executable 的 main 源不属于静态库清单。
5. tests、fixture、archive 和生成文件不得进入产品清单。
6. 根 CMake 只保留项目策略、产品 target、少量特殊测试和模块 include。
7. 根脚本行数建立只减不增 ratchet，模块不得靠搬出另一个巨型文件规避。
8. 这不改变库符号、公共 API、命令输出、WAL/snapshot/OSFS 行为。

验收：315 个产品翻译单元前后集合/顺序一致；未登记、重复和错误扩展名红路径成立；根
`CMakeLists.txt` 目标不超过 650 行，任何单个新增 CMake 模块不超过 500 行；九分评分表
逐项链接机械证据。最后一版执行全量 CTest、真实 CLI/持久化 smoke、格式/空白检查，
commit/tag/push 并等待七条远端 CI 全绿后停靠。

## 需求-证据矩阵

| 需求 | 实现面 | 机械证据 | 初态 |
|---|---|---|---|
| 只读命令分派去重复 | v1672 私有 dispatch 表 | 62 项命令/catalog census、2,093,273 字节 CLI 摘要、full CTest | 完成 |
| 测试身份数据化 | v1673 test manifest | 342 项逐项 oracle、parser 红路径、`ctest -N` diff | 完成 |
| 产品 ownership 数据化 | v1674 source manifest | 315 项集合/顺序、遗漏/重复/扩展名红路径 | 缺失 |
| 根构建控制面可读 | 根 CMake + 小模块 | 650 行上限与模块 500 行上限 | v1673 已达 436 行；v1674 固化 ratchet |
| 外部合同不变 | CLI/fixture/public header | hash/diff、focused/full CTest | 必须保持 |
| 跨平台成立 | GitHub Actions | v1674 七条 job 全绿 | 缺失 |

## 失败条件

- 修改测试期望、fixture 字节、canonical compatibility rule 或公共输出以适配重构：回退。
- 只把 2,000 行从根 CMake 搬进另一个 2,000 行 `.cmake`：不计收益。
- manifest 没有 schema、唯一性、存在性和顺序检查：版本无效。
- formatter 在参数检查前执行，或查表引入可写/网络/WAL/执行行为：回退。
- 任一版本新增无关功能、放松已有 baseline、移动 `a/` 到 `f/` 历史证据：回退。
- 最终 CI 未全绿却按 9 分到点交付：失败。
