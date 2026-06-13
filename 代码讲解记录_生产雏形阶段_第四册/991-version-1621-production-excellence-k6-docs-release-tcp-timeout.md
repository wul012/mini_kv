# 第一千六百二十一版代码讲解：production-excellence K6 文档发布纪律与 TCP 超时强化

本版是 mini-kv 生产卓越计划（production-excellence）K 系列的收官里程碑 K6，主题
为「文档与发布纪律」，同时落地了计划中 K6 第五条要求的服务端超时与连接泄漏防护。
本版由规划方（Claude）以执行者身份完成收尾：在本地 CLion 工具链上重新构建、运行
全量 CTest、撰写本讲解、归档证据并提交打标签 v1621。CMake 工程版本号继续保持
`0.102.0`，与 git 标签 vNNNN 构成既有的双轨方案（详见下文版本一致性一节）。

## 本版主题与入口模块

K6 涉及两类入口。其一是文档入口：`README.md` 不再承载千余行历史变更，改由
`docs/CHANGELOG.md` 作为长历史的唯一归宿；新增 `docs/SECURITY.md`、
`docs/TESTING.md`、`docs/CAPABILITY-SNAPSHOT.md` 三份面向审计者与新读者的纲领性
文档。其二是运行时入口：`minikv_server` 的 `ServerOptions` 新增
`client_idle_timeout` 字段，贯穿到 `serve_client` 客户端服务循环，用于在连接空闲或
命令半包滞留过久时主动回收连接。两类入口都遵循「不改变既有契约」的硬规则——文档
搬运不触碰任何被测试解析的内容，超时字段默认值为零即默认关闭。

## README 变更日志拆分

历史上 `README.md` 体积接近 600KB，顶部堆叠了一千余条版本条目，使其既无法作为项目
概览，也难以阅读。本版将这部分长历史整体迁出，落入新建的 `docs/CHANGELOG.md`
（约 510KB），其开头明确写明「本文件承载原先位于 README 顶部的长版本历史」，并保留
一个「Version Scheme」小节解释版本方案。`README.md` 因此净减少约 1502 行，回归到
概览、构建运行说明、架构与最近版本摘要的本职。迁移前已按 K6 计划检查 `tests/` 与
`src/` 是否存在解析 README 的载荷性代码，确认没有任何测试或源码依赖 README 的具体
内容（统计结果为零），因此搬运是安全的、不破坏任何 fixture。

## 新增三份治理文档

`docs/SECURITY.md`（mini-kv Security Notes）给出威胁模型：单进程、本地优先、设计上
不含鉴权 / TLS / 复制；并解释 WAL 原子性（FNV 校验和、失败前向恢复）、快照原子替换
（Windows 下 MoveFileExW、POSIX 下 rename），以及 read_only / execution_allowed /
mutates_store / touches_wal 等边界标记对审计者的含义。`docs/TESTING.md`（mini-kv
Testing Guide）阐述本仓库 fixture / evidence 测试策略、三百多个 CTest 目标的布局，
以及刻意不覆盖的范围（多进程、网络分区），并点明四项目之间的契约对齐规则。
`docs/CAPABILITY-SNAPSHOT.md`（mini-kv Capability Snapshot）则把当前能力边界做成一页
快照，便于上层（orderops-node、advanced-order-platform）在不深入源码的情况下了解
mini-kv 现状。这三份文档都是只增不改的纲领，不改变任何运行输出。

## 版本号双轨与一致性

mini-kv 一直存在 CMake 工程版本（`0.102.0`）与 git 标签（vNNNN，本版为 v1621）两套
编号。K6 的版本一致性要求是「二选一并写下来」。本仓库选择保留双轨并在
`docs/CHANGELOG.md` 头部说明：CMake 的语义化版本用于构建产物标识，git 标签 vNNNN 才
是面向迭代循环的权威版本号。因此本版不上调 CMake 版本号，这是被记录在案的既定政策，
而非疏漏。

## TCP 客户端空闲与命令超时强化

服务端的核心改动集中在 `src/tcp_server.cpp`。原先用于监听套接字的
`wait_for_listener` 被泛化为 `wait_for_readable`，使其可对任意套接字做带超时的可读性
等待——这是复用而非新增机制。`serve_client` 新增形参 `client_idle_timeout`，在每轮
读取前调用 `wait_for_readable(client, client_idle_timeout)`：当超时且无数据可读时，
区分两种情形回收连接——若此前已积累半包字节（`pending` 非空），判定为
`command_timeout`，对以 `*` 起始的 RESP 数组半包回送 `-ERR command timeout\r\n` 后
关闭；若连一个字节都没等到，判定为纯 `client_idle_timeout` 静默关闭。两种情形都通过
新辅助函数 `client_timeout_fields` 输出结构化日志，包含 `pending_bytes` 与
`client_idle_timeout_ms` 字段，并标注 `reason=command_timeout` 或
`reason=client_idle_timeout`，与 K3 引入的分级日志器协同，便于运维定位。该能力通过
`ServerOptions::client_idle_timeout`（默认 `{0}` 即关闭）opt-in，因此对既有部署零行为
变化，只有显式配置后才生效，严格符合默认安全的设计取向。

## 测试设计：tcp_server_timeout_limit_tests

新增 `tests/tcp_server_timeout_limit_tests.cpp`，并相应扩展 `tests/tcp_test_support.hpp`
的测试夹具（新增对超时配置的支持）。该测试基于既有的 `TestTcpServerHarness`、
`connect_test_socket`、`exchange_raw`、`receive_all` 等夹具，验证四类场景：其一，空闲
客户端连接不发送任何数据，服务端在 `client_idle_timeout`（测试取 50ms）后主动关闭，
客户端读到空响应，且日志出现 `reason=client_idle_timeout` 与
`client_idle_timeout_ms=50`；其二，发送半包命令后滞留，触发 `command timeout`，客户端
收到 `ERR command timeout`，日志出现 `reason=command_timeout` 与 `pending_bytes=20`；
其三，正常请求在开启超时的前提下仍能完整往返（响应内容与 `BYE` 均在）；其四，连续
多次顺序连接后统计量正确——`total_connections` 等于预期、`active_connections` 归零、
`peak_connections` 至少为一，并出现 `event=tcp_client_closed`，从而断言不存在套接字
泄漏。这恰好覆盖 K6 计划第五条「服务端空闲 / 命令超时强制执行，以及约百次顺序连接下
的无泄漏断言」。

## 测试与构建证明（含 Windows 本地偏差）

本地以 CLion 自带工具链（cmake 3.x、mingw g++、MinGW Makefiles 生成器，
`mingw/bin` 已加入 PATH 以提供运行期 DLL）增量重建 `cmake-build-v1621`，构建在十秒内
成功（退出码 0）。随后运行全量 CTest 共 338 个目标，其中 337 个通过；唯一未通过的是
`cli_log_level_flag_smoke`，表现为 120 秒超时。经定位，该用例定义为
`add_test(NAME cli_log_level_flag_smoke COMMAND minikv_cli --log-level debug)`，未对标准
输入做重定向；在 Linux CI 上 ctest 会把 stdin 接到 /dev/null，CLI 立即读到 EOF 后正常
退出，而在本机 Windows 下 ctest 未提供 EOF，交互式 REPL 持续等待输入而挂起。该用例
运行的是 `minikv_cli` 二进制（依赖 `command.cpp`，不依赖本版改动的
`tcp_server.cpp`），因此本版增量构建根本未重新编译它，可排除 K6 回归。为佐证 CLI 本身
健康，单独以管道喂入输入运行 `minikv_cli --log-level debug`，进程零秒内以退出码 0 干净
结束。结论：该失败是既有的、与 K6 无关的 Windows 本地环境限制，在作为权威来源的
Ubuntu CI 上为绿色，已记入偏差。由于本人（执行者）无法采集 CLI 截图，本版归档以文本
证据替代截图，并在偏差中说明。

## 归档与前向政策

按 K5 建立的前向归档政策与 AGENTS 版本收尾约定，本版在 `f/1621/` 下建立归档：
`解释/说明.md` 记录本版主题、改动清单与验证结论，`证据/full-ctest-summary.txt` 留存
全量 CTest 摘要文本（337/338，附偏差说明）。归档遵守「原始输出截断到被断言部分、不放
二进制」的政策。需要强调的硬规则：`a/`–`f/` 归档以及证据文件的路径与摘要均被
orderops-node 以 439 处硬编码引用并断言其摘要，本版只在 `f/` 下新增 v1621 目录，未移动
或重命名任何既有归档，符合冻结约束。

## 对四项目松耦合协作的价值

K6 收官使 mini-kv 的「生产卓越」十项门禁（CI、消毒器、覆盖率、文档诚实、可观测性、
发布纪律等）全部落地。文档拆分让上层项目可以仅凭 SECURITY / TESTING /
CAPABILITY-SNAPSHOT 三页快照理解 mini-kv 的能力与边界，而无需阅读巨型 README；TCP
空闲与命令超时则为未来把 mini-kv 暴露到更长链路时提供了连接回收与泄漏防护的安全垫，
且默认关闭、向后兼容。至此 mini-kv 的 K0–K6 全部完成，剩余的收据族（runtime receipts）
合并属于可选的延伸范围，不在生产卓越必修项之内。
