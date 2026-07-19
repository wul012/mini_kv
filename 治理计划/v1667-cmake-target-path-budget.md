# v1667 CMake 测试 target 路径预算简报

状态：本地收口完成，待 v1667 远端 CI。起点为 v1666（`67495bf0`）；v1666 CI 七个 job 已全部成功。本简报只处理干净
MinGW 配置阶段暴露的一条对象路径风险警告，不重开 v1662 已关闭的长文件名改造计划。

## Step-0 事实

- `CMakeLists.txt` 共有 344 个 `minikv_add_*_test` 注册。
- 其中 278 个内部 target 标识符超过 40 字符；这是存量，只能减少，不能新增。
- 当前最高平台无关风险分数为 200，定义为 `target.length + source_path.length`。
- 唯一达到 200 的注册同时触发 MinGW CMake 对象路径警告：内部 target 为
  `minikv_text_package_review_closeout_audit_validation_tests`，源文件路径长 142 字符。
- v1662 已明确冻结该 132 字符测试文件名：安全候选只有一个，不得为单次改名重开计划。

## 抽象设计（新增 family 前置说明，8 行）

1. 抽象名：`CMakeTestRegistration`，表示一个内部构建 target、公开 CTest 名和源文件路径。
2. 数据来自格式固定的 `minikv_add_*_test(...)` 三元组，不读取生成目录。
3. 行为一：解析并验证 target/test 唯一、源文件存在。
4. 行为二：所有超过 40 字符的内部 target 必须属于 shrink-only baseline。
5. 行为三：`target.length + source.length` 不得超过最终风险上限 197。
6. baseline 只保存存量长 target 名，不保存 CTest 名或源路径。
7. 公开 CTest 名继续完整表达业务语义，内部 target 只承担构建身份。
8. 源文件名仍受 v1662 stop condition 约束，本版不移动、不重命名。

## 需求—证据矩阵

| 需求 | 实现 | 会失败的机械证据 | 状态 |
|---|---|---|---|
| 消除现有对象路径警告 | 仅缩短问题注册的内部 target 至 40 字符以内 | 干净 configure 输出不得含 `CMAKE_OBJECT_PATH_MAX`/`cannot be safely placed` | 已通过：警告 0 |
| 冻结存量长 target | `config/test-target-name-baseline.txt` | 新超长 target 不在 baseline 时 `test_architecture_contract` 失败 | 已通过：278→277 |
| 防止路径风险回涨 | 风险分数上限 197 | 注入 198 分注册时架构合同必须失败 | 已通过：200→197 |
| 保留公开测试合同 | 第二参数 CTest 名不变 | `ctest -N` 仍为 353，目标测试名仍可定位 | 已通过：353、恰好一次 |
| 保留运行行为 | 不改 `src/`、`include/`、fixtures | 全量 CTest、真实 CLI 与零产品/fixture diff | 已通过：353/353、CLI PASS |
| 证明门真的会红 | 三条故障注入 | 新长 target、stale baseline、超风险分数分别命中指定错误 | 已通过 |

## 明确不做

- 不重命名 v1662 rank-10 测试源文件，不修改其 include、命名空间或业务断言。
- 不批量缩写 278 个存量 target，不制造不可读的数字或 hash target。
- 不修改公开 CTest 名、CLI/RESP/JSON、receipt 字节、release fixture 或历史归档路径。
- 不提高 `CMAKE_OBJECT_PATH_MAX` 掩盖风险，不把警告过滤掉，不降低 Windows CI 覆盖。

## 完成标准与失败条件

1. 长 target baseline 从 278 收紧到 277，且新 target 名不超过 40 字符。
2. 风险分数从 200 收紧到 197；机械门输出注册数、长名数和最高分。
3. 干净 configure 中原警告消失，完整 build、353 条 CTest、CLI 边界均通过。
4. 讲解先于最终 verify，中文不少于 3000 字；归档只保留选择性证据。
5. 若必须改源文件名、CTest 名、测试预期或 fixture 才能通过，立即回退本版。
6. 若只消掉当前警告却没有防回退门，本版不得标记完成。
