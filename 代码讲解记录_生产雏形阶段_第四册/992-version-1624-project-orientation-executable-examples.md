# v1624 项目导览示例可执行化讲解

## 本版目标、角色和“不是什么”

v1624 的目标很明确：把 v1623 新增的通俗项目导览从“人读了觉得清楚”推进到“代码能证明它没有说偏”。v1623 已经在 `项目通俗说明/mini-kv-项目机制与价值说明.md` 里解释了 mini-kv 的核心输入输出：普通命令怎么进 `Store`，WAL 为什么先记录再变更，snapshot 为什么是显式动作，TCP/RESP 为什么只是协议外壳，`INFOJSON`、`CHECKJSON`、`SMOKEJSON` 这类 JSON 证据命令为什么只服务控制面理解而不是执行生产动作。那一版还加了 `project_orientation_doc_contract`，用 CMake 脚本读 Markdown，防止未来有人把关键章节删掉。

但文档契约只能证明“说明还在”。它不能证明说明中的最小故事仍然和真实代码路径一致。比如导览里说 `CHECKJSON LOAD data/prod.snap` 只是解释危险命令，不会真的执行 `LOAD`，文档契约能保证这句话还在，却不能发现未来某次重构把 `CHECKJSON` 错接到了 persistence 执行路径。v1624 就补上这层缺口：新增 `tests/project_orientation_examples_tests.cpp`，直接创建 `Store` 和 `CommandProcessor`，按导览中的最小读写故事执行命令，然后断言普通写、普通读、只读证据、危险命令解释、综合边界证据都仍然匹配。

这版的角色是“导览可信度测试”和“维护者上手路径测试”。它不是新命令，不是新 TCP 接口，不是 Node 或 Java 的消费入口，不是运行时路由器，不是恢复执行入口，也不是给 `LOAD`、`COMPACT`、`RESTORE` 或 managed audit 写入开后门。它不修改 WAL、snapshot、server、client、RESP 协议，也不改任何历史 fixture。它只把文档里最容易被读者照着试的一组输入输出，变成 CTest 可以重复运行的证据。

## 入口：CMake 如何把导览示例接进测试套件

入口在 `CMakeLists.txt`。本版新增了一段 `minikv_add_linked_test`：

```cmake
minikv_add_linked_test(
    minikv_project_orientation_examples_tests
    project_orientation_examples_tests
    tests/project_orientation_examples_tests.cpp
)
```

这里沿用项目现有的 linked test 注册方式，而不是临时写一个脚本或单独命令。这样做有三个好处。第一，它会链接 mini-kv 的真实库代码，测试走的是 `Store`、`CommandProcessor` 和各个 command translation unit，而不是伪造输出。第二，它进入 CTest 总表，和三百多条既有回归测试一起运行，版本收口时不会被遗漏。第三，它不会启动 server，不需要端口，不依赖外部文件，也不会写临时 WAL，因此它适合成为“最小导览实验”：读者看到的输入输出是核心命令层自己的行为，而不是某个复杂环境的偶然表现。

这段注册紧跟在 `project_orientation_doc_contract` 之后，也有意形成两层保护：文档契约保证导览仍然讲清楚必须讲的机制和边界；示例测试保证导览中的最小命令故事仍然能在真实代码里跑通。一个守说明，一个守行为，两者合在一起让项目入口更可靠。

## 测试主体：Store 和 CommandProcessor 的最小闭环

`tests/project_orientation_examples_tests.cpp` 的主体没有引入新 helper，也没有绕过命令层。它直接做：

```cpp
minikv::Store store;
minikv::CommandProcessor processor{store};
```

这意味着后续所有命令都要通过 `CommandProcessor::execute` 进入现有分发逻辑。普通字符串命令会走 `src/command_dispatch.cpp` 的分发，再落到 `src/command_string_ops.cpp` 等命令族实现；`INFOJSON`、`CHECKJSON`、`SMOKEJSON` 这样的证据命令会走 introspection 侧实现和 contract formatter。测试没有直接调用 `Store::set` 或 `Store::get` 来偷懒，因为那样只能证明容器能读写，证明不了命令文本、分发、响应字符串和控制面字段仍然一致。

第一步是：

```text
SET order:100 paid
```

预期响应是：

```text
OK inserted
```

并且 `store.size()` 等于 1。这个断言保护的是最基础的写路径：命令文本被解析成 key/value，写入内存 Store，而且响应仍然是当前公共输出。这里没有启用 WAL 文件，所以测试并不声称证明 WAL append；它证明的是导览里“普通写命令会改变内存 Store”这一点。

第二步是：

```text
GET order:100
```

预期响应是：

```text
paid
```

这个断言保护读路径，也让后面的危险命令检查有了一个可观察的状态锚点。因为 Store 里已经有 `order:100=paid`，如果后续 `CHECKJSON LOAD data/prod.snap` 被错误地执行成真实加载，Store 很可能被替换、清空或发生其他变化；测试最后再次读取同一个 key，就能把这种误接线暴露出来。

## INFOJSON：运行时身份不是执行授权

第三步调用：

```text
INFOJSON
```

测试断言响应中包含：

```json
"read_only":true
"execution_allowed":false
"order_authoritative":false
```

这些字段是 mini-kv 在跨项目松耦合里最重要的自我定位。`read_only=true` 表示这份 JSON 证据可以被控制面读取；`execution_allowed=false` 表示读取这份证据不等于允许触发动作；`order_authoritative=false` 表示 mini-kv 不成为 Java 订单系统的业务权威。对 Node 或 Java 这样的下游消费者来说，这些字段的意义不是“mini-kv 很强，所以可以让它接管流程”，而是“mini-kv 明确告诉你它只提供存储和证据边界，不提供订单决策和执行许可”。

v1624 把这些字段放进导览示例测试，是为了防止说明和真实输出分叉。未来如果有人重构 INFOJSON，把字段名改了、删了，或者错误地把 `execution_allowed` 改成 true，测试会在最小入口层失败。这个失败会比集成阶段更早、更便宜，也更容易定位。

## CHECKJSON LOAD：解释危险命令，但不执行危险动作

第四步是本版最关键的断言：

```text
CHECKJSON LOAD data/prod.snap
```

从用户视角看，`LOAD data/prod.snap` 是一个危险命令，因为真实 `LOAD` 会从 snapshot 读取数据并替换 Store。可是 `CHECKJSON LOAD data/prod.snap` 的角色完全不同：它只是让控制面看见“如果这是 LOAD，它有哪些风险、是否允许执行、会影响什么”。所以测试断言响应包含：

```json
"read_only":true
"execution_allowed":false
"command":"LOAD"
"write_command":true
"store_replace_from_snapshot"
"write command is not wal-backed"
```

这里每个字段都很有意义。`command:"LOAD"` 说明 CHECKJSON 正确识别了被分析的命令。`write_command:true` 和 `store_replace_from_snapshot` 说明它没有把 LOAD 粉饰成安全读操作，而是如实标出它会替换 Store。`write command is not wal-backed` 继续提醒这类动作不走普通 WAL 追加路径。最关键的是 `read_only=true` 和 `execution_allowed=false`：它告诉下游，这份响应只是风险解释，不是执行批准。

测试随后立刻断言 `store.size()` 仍然是 1，再执行一次：

```text
GET order:100
```

预期仍然是：

```text
paid
```

这一步是“机理透明”的核心证据。它不是只看 JSON 字段自称没执行，而是用 Store 状态反查：如果 CHECKJSON 真正进入 `LOAD` 执行路径，前面写入的 `order:100` 就有可能不再保持原状。现在它还在，说明 CHECKJSON 在命令分析层停住了，没有加载 snapshot，没有替换 Store，没有触碰 WAL，也没有把危险动作推进到执行层。

## SMOKEJSON：综合证据仍是只读边界证明

最后测试调用：

```text
SMOKEJSON
```

并断言响应包含多组边界字段：

```json
"read_only":true
"execution_allowed":false
"restore_execution_allowed":false
"order_authoritative":false
"store":{"live_keys":1,"order_authoritative":false}
"forbidden_commands":["LOAD","COMPACT","SETNXEX","RESTORE"]
"load_restore_compact_executed":false
"managed_audit_write_executed":false
"node_auto_start_allowed":false
"java_auto_start_allowed":false
"mini_kv_auto_start_allowed":false
```

`SMOKEJSON` 在这个项目里不是“冒烟后顺便执行一下危险动作”。它是一个综合只读证据：告诉控制面当前 Store 有多少 live key，哪些命令属于禁止自动执行，是否发生过 load/restore/compact 执行，是否发生 managed audit 写入，Node/Java/mini-kv 是否允许自动启动。v1624 的测试把这些字段和前面的最小写入连起来：Store 里确实有一个 live key，但这不意味着 mini-kv 获得订单权威；危险命令确实被列出来，但这不意味着它们被执行；自动启动字段都是否定值，说明这条 C++ 测试没有悄悄把跨项目服务链拉起来。

这组断言也保护了 v1623 导览里最重要的读法：当 Node 或 Java 将来读取 mini-kv 的证据时，它应该把这些 JSON 当成“边界说明书”，而不是当成“操作按钮”。如果有人改动 SMOKEJSON，让 forbidden commands 消失，或者让 auto-start 字段变成允许，测试就会提醒这已经不再是原来的安全语义。

## 文档契约的同步加固

除了新增可执行测试，本版还更新了 `cmake/check_project_orientation_doc.cmake`。原本这个脚本只要求导览里出现项目机制、架构图、WAL、Snapshot、TCP/RESP、CHECKJSON、SMOKEJSON 和关键边界字段；v1624 增加了三个片段：

```text
project_orientation_examples_tests
SET order:100 paid
GET order:100
```

这不是为了让文档越来越臃肿，而是为了确保“可执行导览实验”不会变成隐藏知识。测试存在，文档也要告诉维护者它存在；文档写了例子，测试也要证明例子还是真的。两边互相牵制，能减少后续维护时常见的漂移：代码悄悄改了，说明没更新；或者说明写得很漂亮，但没人知道它是否还能跑。

对应的 `项目通俗说明/mini-kv-项目机制与价值说明.md` 新增了“可执行最小导览实验”一节，用表格把每一步输入、预期输出或状态、说明写清楚。这个表格服务的是新读者：他们不用先理解全套 WAL、snapshot、shard readiness、archive retention，就能看见一个最小闭环。普通命令会改变 Store；证据命令会解释状态；危险命令可以被分析但不会被执行；综合证据会把 no auto-start、no managed audit、no restore/compact execution 这些边界亮出来。

`项目通俗说明/README.md` 也补了一句，说明 v1624 起导览中的最小命令故事由 `project_orientation_examples_tests` 验证。这一层索引很小，但能让后来者从文件夹入口就知道：这里不是随手写的解释文，而是有测试兜底的维护面。

## 本版没有打开哪些边界

v1624 最需要强调的是“加了测试，不等于加了能力”。它没有新增 runtime command，没有修改 `LOAD`、`COMPACT`、`RESTORE` 的执行语义，没有启动 TCP server，没有读取真实 snapshot 文件，没有生成 WAL 文件，没有写 managed audit，没有导入 Node 代码，没有扫描 Java 项目，没有访问 credential，没有创建 raw endpoint，也没有改任何跨项目 evidence schema。

它确实执行了 `SET order:100 paid`，但这是测试内部创建的内存 Store，用来证明普通命令路径仍然工作。它确实调用了 `CHECKJSON LOAD data/prod.snap`，但这个命令只返回分析 JSON，测试还用后续 `GET order:100` 反证 Store 没被替换。它确实调用了 `SMOKEJSON`，但 `SMOKEJSON` 返回的是只读状态报告，不会触发 load、restore、compact 或服务启动。

这也是 mini-kv 当前工程价值的一部分：它不只是能做 KV 存储，还在不断把“哪些事情能做、哪些事情不能自动做、哪些输出只是证据”写成可验证契约。对于一个未来可能被 Node 或 Java 读取的 C++ 组件来说，这比单纯多加几个命令更重要。

## 测试覆盖保护了什么

本版最直接的新增测试是 `project_orientation_examples_tests`。它保护五类行为。

第一，保护命令输入输出的可读示例：`SET order:100 paid` 仍然返回 `OK inserted`，`GET order:100` 仍然返回 `paid`。这让导览里给新读者看的例子不是虚构文本。

第二，保护运行时身份字段：`INFOJSON` 必须继续暴露 `read_only=true`、`execution_allowed=false`、`order_authoritative=false`。这几个字段是 mini-kv 和订单平台保持边界的基础。

第三，保护危险命令分析语义：`CHECKJSON LOAD data/prod.snap` 必须承认 LOAD 是写类/替换 Store 风险，同时必须保留 `execution_allowed=false`。这避免未来有人把风险解释误改成执行入口。

第四，保护状态未被危险分析改变：CHECKJSON 之后 Store 仍然有一个 key，`GET order:100` 仍然返回 `paid`。这比只看字段更强，因为它验证了没有实际替换 Store。

第五，保护综合 smoke 边界：`SMOKEJSON` 必须继续声明 forbidden commands、no auto-start、no managed audit write、no restore/compact execution。它让测试覆盖从单个命令扩展到控制面理解 mini-kv 时最关心的一组安全栏杆。

除了新增测试，全量 CTest 也很关键。v1624 本地跑过 340/340，说明新增测试没有破坏既有 Store、command、WAL、snapshot、RESP、TCP、fixture、readiness 和 governance evidence 契约。对这个仓库来说，全量测试不是形式主义，因为很多历史版本的价值都沉淀在回归测试里：一条小测试新增，如果把老边界碰坏，全量 CTest 会比人工阅读更诚实。

## 给后续维护者的读法

如果后续有人想理解 mini-kv，推荐顺序是：先读 `项目通俗说明/mini-kv-项目机制与价值说明.md`，知道输入、输出、WAL、snapshot、TCP/RESP、证据命令和边界字段分别是什么；再打开 `tests/project_orientation_examples_tests.cpp`，看这份说明里最小的一组例子如何被真实代码验证；然后再进入 `src/command_dispatch.cpp`、`src/command_string_ops.cpp`、`src/command_introspection_ops.cpp` 和 `src/command_contracts.cpp` 看分发、执行、分析和格式化细节。

这样读的好处是不会一上来被上千条 governance fixture 淹没，也不会误以为 mini-kv 的价值只在“写了很多报告”。它真正有价值的地方，是把一个 C++ KV 核心、恢复机制、协议入口和跨项目只读证据边界连成可测试的链。v1624 做的就是把这条链的入口变得更可靠：文档讲给人听，测试讲给机器听，两边说的是同一件事。

一句话总结：v1624 让 mini-kv 的通俗导览不再只是说明文字，而是被真实 `Store` + `CommandProcessor` 路径验证的最小证据入口，为后续 Node/Java/mini-kv 松耦合融合提供更可信、更不易漂移的只读边界样例。
