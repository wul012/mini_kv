# 第一千六百二十版代码讲解：production-excellence K5 归档保留库存

本版目标是把 mini-kv 长期积累的命令截图、解释文档和历史证据根从“大家知道不要乱动”的口头纪律，推进到“有索引、有脚本、有 CI 提醒、有本地测试保护”的工程纪律。它的角色不是新增存储能力，也不是给控制面增加一个运行接口，而是给项目自身的证据资产建一层可复核的保留边界：哪些目录是冻结历史，哪个目录是当前活动根，每个根大概有多大，新版本归档是否开始失控，CI 看到异常时应该提醒而不是直接破坏交付节奏。

这一版尤其要先讲清楚“不是什么”。它不是 archive migration，不会移动 `a/` 到 `f/` 任何既有版本目录；不是压缩替换，不会把历史 PNG、解释文档或 fixture 打包后删除原路径；不是清理脚本，不会为了节省空间去删除旧证据；也不是新的 mini-kv 命令，不会打开 TCP server、不会执行 CLI 写入、不会触碰 WAL、不会参与 restore/load/compact。K5 的核心价值恰好在于克制：历史路径已经被 Node 侧和跨项目证据引用，mini-kv 此时最需要的是知道它们在哪里、状态是什么、以后新增归档怎样保持小而清楚，而不是做一场看似整洁但会破坏证据链的大搬家。

## 入口模块与执行方式

本版新增的可执行入口是：

```text
scripts/archive_inventory.py
```

它是一个 Python 标准库脚本，不依赖第三方包，不需要 CMake 构建，也不需要 mini-kv 运行时。入口命令可以直接在仓库根目录执行：

```powershell
python scripts\archive_inventory.py --budget-mib 8 --github-warning
```

CI 中对应的 Linux 命令是：

```bash
python3 scripts/archive_inventory.py --budget-mib 8 --github-warning
```

这里的 `--budget-mib 8` 是“最新版本归档目录”的前向预算，不是历史根总量限制。原因很简单：`e/` 已经在过去积累到约 1GB，历史上它承担了大量版本截图，回头压缩或搬迁会破坏路径稳定性；但从 K5 往后，每个新版本应当只保留能证明结论的选择性截图和说明，不应该把 build 目录、完整 raw log、临时渲染文件或二进制产物长期放进归档。因此脚本把“历史根容量”和“最新版本预算”分开看：历史根通过文档上限提醒，最新版本通过 8 MiB 预算约束新行为。

脚本支持 `--github-warning`，意味着发现超预算时会输出 GitHub Actions 能识别的 `::warning` 注解；它还支持 `--strict`，本地可以把 warning 提升为失败。CI 默认没有使用 `--strict`，这是 K5 的关键设计：归档库存是治理提醒，不应该因为一次截图略大就让主线构建立刻失败。真正需要失败的是编译、CTest、sanitizer、coverage floor、格式 gate 这些直接保护代码正确性的轨道。归档任务负责把趋势亮出来，让下一版修，而不是把证据治理变成新的脆弱阻塞点。

## 归档索引结构

本版新增的长期文档是：

```text
docs/archive-retention-index.md
```

这个索引把当前仓库中的 `a/`、`b/`、`c/`、`d/`、`e/`、`f/` 六个归档根全部列出来，记录每个根的版本范围、文件数量、字节数、MiB 大小、容量上限和状态。索引里的几个状态不是装饰词，而是后续维护者必须遵守的边界字段。

`FROZEN` 表示该根已经是历史证据，不应在普通功能版本里移动、改名、重排或压缩替换。比如 `a/` 到 `d/` 虽然体量不大，但它们承载早期版本截图路径；`e/` 虽然很大，但它正是过去长期写入的历史事实，不能因为当前觉得目录臃肿就批量改路径。`FROZEN HIGH-VOLUME` 用在 `e/`，强调它容量高但仍然冻结。`ACTIVE_ROOT_EXISTING_VERSIONS_FROZEN` 用在 `f/`，意思是未来新版本继续写 `f/<version>/图片/` 和 `f/<version>/解释/说明.md`，但已经存在的 `f/1597` 到 `f/1619` 仍然按历史证据看待，不因为 `f/` 还是活动根就可以回头整理旧目录。

索引也明确了下一次根轮换规则：不要随手创建 `g/`。只有当 `f/` 真的需要轮换时，必须先更新 `docs/archive-retention-index.md` 和 `AGENTS.md`，把新根的活动状态、历史根的冻结状态和前向保留政策写清楚，再开始写新目录。这个顺序很重要，因为归档根不是临时输出路径，而是跨版本证据地址；先写规则再落证据，后续 Node、Java 或人工 review 才知道该相信哪条路径。

## 脚本核心流程

`archive_inventory.py` 的核心流程很小，但边界比较明确。第一步是从仓库根目录扫描单字母归档根，只识别 `a` 到 `z` 这类目录；第二步进入每个根，找到名字能解析为整数的版本目录；第三步递归统计文件数量和字节数；第四步计算版本范围和 MiB；第五步找出所有归档根里版本号最大的那个目录，作为“最新版本归档”；第六步把根目录上限和最新版本预算合并成 warning 列表；最后按固定文本格式输出。

这个输出格式故意保持朴素：

```text
mini-kv archive inventory
root versions files bytes mib ceiling_mib status
...
latest_version f/1619 files=7 bytes=252151 mib=0.24 budget_mib=8.00
warnings: none
```

它没有输出 JSON，主要是因为 K5 当前只需要人和 CI annotation 能看懂，不需要下游控制面消费。更重要的是，这个脚本不是 mini-kv 业务证据命令，不应该让 Node 或 Java 误以为它是新的契约响应。未来如果真的要把归档库存变成跨项目 evidence，可以再单独设计 schema、字段语义、fixture 和只读边界；v1620 只做本仓库治理，不提前打开跨项目接口。

根目录容量上限内置在脚本默认配置里：`a/`、`c/`、`d/` 默认 64 MiB，`b/` 默认 16 MiB，`e/` 默认 1200 MiB，`f/` 默认 128 MiB。这个配置不是为了宣称这些大小永远合理，而是把当前历史现实和未来预期分开。`e/` 的上限高，是因为它已经是高容量历史根；`f/` 的上限低一些，是因为从现在起要维持更克制的归档风格。脚本还提供 `--root-ceiling-mib ROOT=MIB`，以后如果维护者需要临时调整某个根的预算，可以在不改源码的情况下试跑。

## CI 契约与 gcov workaround 退场

本版修改了：

```text
.github/workflows/ci.yml
tests/dependabot_config_tests.cpp
```

CI 新增 `archive inventory` job，运行在 Ubuntu 上，只做 checkout 和 Python 脚本检查。它不依赖 CMake，不下载 artifact，不运行服务，也不读取外部网络。这个 job 的输出如果有 warning，会在 GitHub Actions 页面上标注出来；如果没有 warning，就像 v1620 本地截图那样显示 `warnings: none`。由于没有 `--strict`，它是提醒轨道，不是失败轨道。

同一个版本还处理了 K4 review 后留下的 advisory：v1613 为了绕过 GCC/gcov 在旧 `command.cpp` 上产生的 negative branch-hit parse 问题，曾经给 gcovr 加过：

```text
--gcov-ignore-parse-errors negative_hits.warn_once_per_file
```

K4 已经把 `command.cpp` 拆开，所以 v1620 重新测试这个 workaround 是否还需要。做法不是只从 workflow 中删掉参数，而是在 `dependabot_config_tests.cpp` 中增加反向断言，确认 CI 文本中不再出现 `--gcov-ignore-parse-errors` 和 `negative_hits.warn_once_per_file`。这让“workaround 已退场”变成测试保护的事实。后续如果远端 coverage 因同类 gcovr 问题重新失败，维护者必须有意识地把参数和测试一起改回来，并在进度文档中写明限制，而不是悄悄恢复一个没人记得来源的 CI flag。

这个动作也体现了 K5 的维护思路：不是把脚本和文档写完就算，而是顺手把上一阶段 review 中明确提出的临时债务闭环。K1 到 K4 建起的 sanitizer、coverage、format 和 split safety net 现在开始服务于维护动作本身。

## AGENTS 与前向归档政策

本版同步更新 `AGENTS.md`，把当前活动归档根记录为：

```text
f/<version>/图片/
f/<version>/解释/说明.md
```

并写入 v1620 之后的前向策略：未来新版本截图应当选择性、压缩化，只保留能支撑断言的关键命令输出；原始命令输出在截图页面里截断到被断言的部分；最终归档不得包含 build binaries、临时 raw logs 或渲染中间文件；历史归档根仍然保持路径稳定，不做追溯压缩、移动、改名或删除。

这段规则看起来偏流程，但对 mini-kv 很重要。过去大量版本每次都截图、讲解、归档，速度很快，证据也丰富，但长期看会造成目录膨胀、README 过载、截图堆叠和人工 review 成本升高。K5 不是否定过去的做法，而是在保留历史的前提下给未来加刹车：证据要能证明结论，不需要把所有过程碎片都永久保存；说明文档要解释为什么截图足够，不需要把每条流水线输出都复制一遍。

## 运行边界字段

虽然 v1620 没有新增 JSON evidence，但它仍然有一组控制面应当读懂的边界含义。

`read_only` 在本版体现为：脚本只扫描文件系统元数据和文件大小，不修改归档目录内容。`execution_allowed=false` 的含义是：库存脚本不是执行授权，不会让 restore、load、compact 或 TCP 命令变得可执行。`order_authoritative=false` 的含义是：mini-kv 的归档库存不参与 Java 订单事实判断，也不成为 Node 控制面上的订单权威。`mutates_store=false` 表示脚本不写 KV store，不创建数据键，不触发命令执行器。`touches_wal=false` 表示脚本不打开 WAL、不 append WAL、不做 snapshot。`warnings` 的含义是治理提醒而非业务 blocker；当最新归档超过预算或根目录超过上限时，它提醒维护者下一版处理，但 CI 默认不把它升级为失败。`diagnostics` 在本版对应脚本输出的 root inventory、latest_version 和 warning 列表，它们帮助维护者判断趋势，而不是驱动业务流程。

这些边界之所以要写清楚，是因为 mini-kv 现在已经不只是一个命令练习项目，而是被 Node 和 Java 作为 evidence 链的一部分引用。越是治理能力变多，越要防止“看起来像控制面”的东西被误解为“可以执行操作”。v1620 的库存脚本只能告诉你证据目录现在是什么样，不能授权你改证据，更不能授权你操作数据。

## 测试与归档证明

本地验证分成四层。

第一层是脚本自身：

```powershell
python scripts\archive_inventory.py --budget-mib 8 --strict
```

这个命令证明当前仓库没有超过预算或根目录上限的 warning，并且 strict 模式下会以失败码暴露问题。第二层是 CI contract test：

```powershell
ctest --test-dir cmake-build-v1620 -C Debug --output-on-failure --timeout 120 -R dependabot_config_tests
```

它保护 workflow 中必须有 archive inventory job，也保护 coverage workaround 已经退场。第三层是全量 CTest：

```powershell
ctest --test-dir cmake-build-v1620 -C Debug --output-on-failure --timeout 120 --progress
```

本地结果为 `337/337` 通过，说明本版文档、脚本和 CI 文本变更没有破坏现有命令、fixture、WAL、snapshot、TCP、运行态只读证据和治理测试。第四层是 `git diff --check`，它确认本版文档没有 trailing whitespace 之类的低级质量瑕疵。此前 v247 的讲解文档曾被指出尾随空白，这类问题不影响源码行为，但会降低长期文档质量；v1620 把它纳入收口动作，是为了让治理文档自身也遵守治理纪律。

归档截图放在：

```text
f/1620/图片/
f/1620/解释/说明.md
```

四张截图分别证明库存脚本、聚焦测试、全量测试和 diff check。截图生成时先把命令输出压成短日志，再渲染成本地 HTML，由 Playwright MCP 截图。临时 HTTP 服务只绑定 `127.0.0.1`，截图完成后关闭；`_logs` 和 `_render` 属于临时材料，最终清理时删除，只保留 PNG 和说明文档。这个流程本身也在实践 K5 的政策：长期留结论证据，不长期留过程垃圾。

## 对后续松耦合融合的价值

v1620 给 mini-kv 的价值不是“多一个功能”，而是“证据资产终于有了保留秩序”。以后 Node 或 Java 读取 mini-kv 的 release fixture、runtime evidence、归档说明时，可以继续依赖旧路径不被突然搬迁；同时 mini-kv 自己也能看到新版本归档是否开始变胖，及时调整截图和解释粒度。对于四项目松耦合融合来说，这是一块很实在的基础设施：上游证据不乱动，下游引用才稳定；新证据不过度膨胀，review 才能持续；治理任务只读、no router、no write、no WAL、no execution，才不会把文档工程误扩成运行权限。

一句话总结：v1620 把 mini-kv 的历史归档从“靠人记住不要动”升级为“索引可查、脚本可测、CI 可提醒、边界可解释”的只读治理能力，为后续 Node/Java/mini-kv 的松耦合证据链提供更稳定、更可维护的底座。
