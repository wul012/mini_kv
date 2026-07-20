# v1671 测试链接分片收口

## Step-0 与问题证据

v1670 已提交为 `474f5cb0`、tag `v1670` 已推送；Actions run `29725213320` 后续核对为 success。Node 最新 `docs/plans/readme-exhibition-brief.md` 已收口并明确 mini-kv 可独立维护，本版不新增跨项目 evidence、fixture、route 或执行权限。

当前默认 CMake 拓扑把 314 个产品源编进一个 `minikv` 静态库，再让 342 个测试 executable 各自链接该库。短路径 Ninja 干净基线为 1033 步、151.44 秒；只 touch `src/atomic_file_writer.cpp` 而不改变字节，仍触发 349 步、347 次 executable link、56.87 秒。全部 345 个测试 exe 合计 3,983,351,060 字节。问题不是测试数量，而是每个测试都购买一次最终链接和一份重复二进制。

## Family 设计说明（先于实现，8 行）

1. 抽象名：`TestLinkShard`，只存在于 CMake 私有测试基础设施。
2. 每个原测试继续是独立 object target，保留源文件、编译定义、include 和 target 名。
3. CTest name 仍是一测试一名字、一调用一进程；只把最终链接聚合为 8 个 runner。
4. test name 经稳定 SHA-256 前缀映射 shard，新增测试不会重排已有 case。
5. 原无参数 `main()` 在编译时改名为短 entry symbol，runner 只做名字到函数的分派。
6. 三个 standalone 测试、CMake script smoke 和五个产品 executable 不进入 bundle。
7. coverage/sanitizer lane 强制保留旧的一测试一 exe 拓扑，避免 gcov/ASan 全量注册副作用。
8. 不引入测试框架、进程内批量执行、共享测试状态或测试期望修改。

## 需求—证据矩阵

| 需求 | 实现 | 机械证据 | 最终状态 |
|---|---|---|---|
| 重复链接收口 | `cmake/MinikvTesting.cmake` + runner template | 默认配置生成 8 个 shard runner；核心 touch executable link 347→13 | 完成 |
| CTest 契约不变 | 原 helper 调用点不改名 | 前后 354 个 test name 与顺序逐项相同，全量 354/354 | 完成 |
| 编译属性不丢 | 每测试保留 object target | runtime builder include、atomic private include、command suite parts 与命名 census 路径继续通过 | 完成 |
| 失败隔离不降级 | 每个 CTest 仍启动一个 runner 进程且只调用一个 entry | runner 缺参/未知 case 均 exit 2；全量一例一进程通过 | 完成 |
| instrumented lane 安全 | fresh coverage/sanitize 默认关闭 bundle，冲突 fail-fast | 冲突 configure exit 1；显式 legacy 代表构建与 CTest 4/4 | 本地完成，待最终 CI 两条 lane |
| 收益真实可复算 | baseline/optimized 两棵短路径 Ninja build | clean 1033/350/151.44→707/16/130.15；touch 349/347/56.87→15/13/21.36；测试 exe 345/3,983,351,060→11/415,219,566 | 完成 |
| 架构防回流 | `test_architecture_contract` | 345 registrations、342 linked cases、8 shards、344 explicit mains、277/197 旧门同时通过 | 完成 |
| 版本闭环 | 中文讲解、选择性归档、commit/tag/push/CI | `f/1671/` 6 文件/76,288 字节 + walkthrough `1039` 3,000+ 汉字 + Actions | 本地完成，待提交与最终 CI |

## 采用门

- 默认 configure 0 warning，CTest inventory 与顺序必须逐项等于基线 354 项。
- 核心 touch 后 executable link 从 347 降到不超过 16，增量时间至少下降 50%。
- 默认测试二进制总量至少下降 70%，不得以删除测试或合并 CTest 名换取。
- 全量 CTest 不得比 v1670 的 180.07 秒慢 25% 以上；出现跨 case 静态初始化副作用则回退。
- `MINIKV_COVERAGE=ON` 与 `MINIKV_SANITIZE=ON` 必须选择 legacy topology；至少完成 legacy focused 构建和测试，最终由两条 CI lane 验证。

## 失败条件

- 在一个进程里连续执行多个测试、让失败污染后续 case：失败。
- 修改测试断言、fixture 或 public output 来适配 runner：失败。
- target-specific compile definitions/include 丢失，或 command/shard part manifest 不再编译：失败。
- 生成 entry 名超过 40 字符、hash 碰撞不报错、未知 case 静默成功：失败。
- 默认收益未过采用门，仍以“架构更优雅”为由保留复杂度：回退。
- coverage/sanitizer 被强制走 bundle，或旧拓扑无法配置构建：回退。

## 实施偏差与关闭方式

- 初版 runner 使用裸 `@变量@` 模板，格式化后生成文件残留标记；改为合法注释标记并在 CMake 中精确替换，最终 8 个 runner 残留为 0。
- 初版架构合同误用 C++23 的 `string_view::contains`；改为 C++20 `find`，没有提高语言标准。
- 宏改名后，108 个旧测试不再享有 `main` 隐式返回规则；全部补显式 `return 0`，并把 344 项 census 写入机械门。触碰后暴露的 79 个旧格式问题同步清偿，没有放宽 CI。
- CLI smoke 校验脚本误期待不存在的 `mutates_store`；按实际 schema 改查 `write_command=false`，不修改产品输出。
- 直接运行架构测试时有一次未注入 MinGW runtime PATH 的环境失败；按仓库本地工具链规则补 PATH 后输出完整合同并退出 0，不计为代码通过证据之前的成功。

## 本地采用结论

所有确定性采用门均通过：默认配置 0 warning；354 个名称与顺序差异 0；增量链接、增量耗时和测试二进制体积分别下降 96.25%、62.44% 和 89.58%；最终全量 CTest 354/354、130.96 秒；产品源、公开头和冻结 fixture 差异为 0。最终 review 点定义为 v1671 commit/tag/push 后七条 CI job 全绿，在此之前只称本地 closeout，不自授外部评审结论。
