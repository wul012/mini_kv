# v1618 production-excellence K4 command family executor split 代码讲解

## 本版目标、角色和不做事项

v1618 继续推进 production-excellence K4 的 command 侧拆分。v1617 已经把 `CommandProcessor::execute_trimmed` 从 `src/command.cpp` 拆到 `src/command_dispatch.cpp`，也把解析辅助函数和 WAL command gate 拆成内部模块；但那一刀只是把“命令分发函数”移出了旧大文件，真正的命令执行体仍集中在 `command_dispatch.cpp` 的一个大 switch 里。v1618 的目标是把这层再拆薄：`command_dispatch.cpp` 只负责把输入行解析成 command verb，然后按命令家族转交给专门 executor；具体的存储读写、TTL、snapshot/WAL 维护、keys/contract introspection 等行为，分别放到四个更小、更容易审查的 translation unit。

本版的角色是维护性重构，不是新功能版本。它不新增命令，不改变命令名称，不改变返回文本，不改 RESP/TCP 协议，不改 WAL 记录格式，不改 snapshot 文件格式，不改 `CHECKJSON` 的 read-only/execution boundary 字段，也不打开任何 shard router、write routing、restore/load/compact 自动执行或跨项目运行通道。换句话说，v1618 是把原来的命令执行路径“分房间放好”，不是让系统拥有新的权限。对 Node/Java/mini-kv 松耦合链路来说，这类版本最重要的价值不是输出新字段，而是降低后续继续维护命令面时的误伤概率。

## 入口命令和模块边界

外部入口仍然是 `CommandProcessor::execute(std::string_view line)`。它在 `src/command.cpp` 中保留三个职责：先通过 `trim_copy` 做空白规整，再用 `command_token` 记录 metrics bucket，随后调用 `execute_trimmed`，最后按耗时和结果更新 `CommandMetricsTracker`。这层没有被 v1618 改成“更聪明”的入口，因为 metrics、成功/失败统计和 `RESETSTATS` 自身不计入重置前数据的行为已经有既有测试覆盖，贸然改动反而会扩大风险面。

`execute_trimmed` 现在位于 `src/command_dispatch.cpp`。它读取首个 token，转成大写，通过 `command_catalog::lookup_dispatch_verb` 得到 `CommandDispatchVerb`，然后只做薄分发。`PING` 和 `QUIT` 仍留在这里，因为它们更像协议控制命令：`PING` 直接返回消息或 `PONG`，`QUIT` 只检查多余参数并设置 `should_close`。除此之外，普通业务命令不再在这个文件里展开长逻辑，而是交给四个 family executor。

第一类是 `src/command_string_ops.cpp`，负责 `SET`、`GET`、`DEL`。这组命令围绕字符串键值本体：`SET` 解析 key/value 并通过 `execute_with_wal` 写入，`GET` 做只读读取并在 key 不存在时返回 `(nil)`，`DEL` 在 key 存在时追加 WAL 删除记录并执行删除。这里最需要守住的是输出字节级语义：`SET` 仍返回 `OK inserted` 或 `OK updated`，`DEL` 仍返回 `1` 或 `0`，`GET` 仍不触碰 WAL。

第二类是 `src/command_expiry_ops.cpp`，负责 `SETNXEX`、`EXPIRE`、`TTL`。这组命令共同依赖 TTL 解析和 expires-at 格式化，因此留在同一文件更自然。`SETNXEX` 只有在 key 不存在时才生成 `SETEXAT` WAL 记录并写入带过期时间的值；如果 key 已存在，它仍通过 `std::nullopt` 让 `execute_with_wal` 返回 `0`，保持原有行为。`EXPIRE` 同样先确认 key 存在再生成 `EXPIREAT` WAL 记录；`TTL` 是只读查询，不写 WAL，不修改 store。

第三类是 `src/command_persistence_ops.cpp`，负责 `SAVE`、`LOAD`、`COMPACT`、`WALINFO`。这组命令和持久化边界相关：`SAVE`/`LOAD` 调用 `SnapshotFile`，`COMPACT`/`WALINFO` 需要访问 WAL 和 WAL gate。把它们从 dispatch 文件中移走后，维护者能一眼看清哪些命令可能触碰 snapshot 文件或 WAL maintenance。注意这不意味着 v1618 打开了新的执行通道；这些命令本来就存在，权限和用法没有变化，测试仍然验证 snapshot 与 WAL 行为不变。

第四类是 `src/command_introspection_ops.cpp`，负责 `SIZE`、`KEYS`、`KEYSJSON`、`RESETSTATS`、`EXPLAINJSON`、`CHECKJSON`、`HELP`。这组命令的共同点是“向控制面解释状态或契约”。`SIZE`/`KEYS` 读取当前 store，`EXPLAINJSON` 和 `CHECKJSON` 读取 command contract，`HELP` 读取 command catalog，`RESETSTATS` 只操作 metrics tracker。这里的关键边界是 `CHECKJSON` 继续由 `command_contracts::format_check_json(target_command, wal_ != nullptr)` 输出，所有 read-only、execution、write command、WAL enabled 语义都保持原样。

## 响应和证据结构保持不变

v1618 没有新增 fixture，也没有修改历史 fixture。命令返回文本仍由原来的 formatter 和 store/WAL/snapshot 调用链生成。比如 `KEYS` 仍使用 `command_response_formatters::format_keys` 或 `format_prefixed_keys`，`KEYSJSON` 仍使用 `format_keys_json`，`WALINFO` 仍使用 `format_walinfo`，`CHECKJSON` 仍使用 `command_contracts`。拆分后的 executor 只是把原 switch case 中的代码搬到对应家族文件里，并通过 private member 方法复用同一个 `CommandProcessor` 状态，包括 `store_`、`wal_`、`options_` 和 `metrics_tracker_`。

为了让编译边界清楚，`include/minikv/command.hpp` 只前置声明 `command_catalog::CommandDispatchVerb`，没有把完整 command catalog 头强塞进公共头。这样调用者仍只依赖 `CommandProcessor` 的公共 API；新增的 family executor 方法都在 private 区域，外部不能直接绕过 `execute` 或 `execute_trimmed` 调用它们。这个设计也保留了 metrics 入口的单一性：所有外部命令仍必须从 `execute` 进入，不能绕过耗时统计和 command bucket 记录。

## 核心执行流

一次 `SET family-key family-value` 的执行流现在是：`execute` 修剪输入并记录原始 command token，`execute_trimmed` 查 catalog 得到 `CommandDispatchVerb::Set`，薄分发到 `execute_string_command`，后者解析 key/value，调用 `execute_with_wal`。如果 WAL 未启用，mutation 直接写 store；如果 WAL 启用，则先进入 `wal_command_gate().enter_command()`，生成 `SET key value` WAL record，append 成功后再写 store，最后按需触发 auto compaction。这个路径和 v1617 以前一致，只是命令体所在文件从 dispatch switch 移到了 string executor。

一次 `SETNXEX ttl-key 30 ttl-value` 的执行流类似，但它属于 expiry executor。这里保留了两个细节：第一，秒数字段仍通过 `parse_positive_seconds` 解析，非法或缺失时返回 `ERR usage: SETNXEX key seconds value`；第二，WAL record 不是保存原始 TTL 秒数，而是用 `format_setex_at` 写成绝对 expires-at 时间，这与 WAL replay 期望一致。v1618 没有尝试重写 WAL 语义，也没有把 TTL 逻辑移进 store 之外的新抽象。

一次 `SAVE path` 或 `COMPACT` 的执行流进入 persistence executor。`SAVE` 和 `LOAD` 继续以路径字符串作为剩余整行参数，避免破坏路径中空格的兼容性；`COMPACT` 仍要求 WAL 启用，并在 compaction gate 里调用 `wal_->compact(store_, &compacted)`。这组命令能写文件或改 WAL 文件，但这不是证据消费面的隐式执行；它们仍是用户显式命令，`CHECKJSON` 也继续把相关 side effects 暴露出来。

一次 `CHECKJSON GET family-key` 的执行流进入 introspection executor。它不会执行 `GET`，也不会读取 key 的值，而是把目标命令字符串交给 `command_contracts::format_check_json`。因此输出中的 `read_only:true`、`execution_allowed:false`、`write_command:false`、`touches_wal:false`、`warnings:["not a write command"]` 等字段仍表达“控制面只看契约，不执行命令”。这是 Node/Java 侧最关心的边界之一，v1618 没有改变。

## 测试保护了什么

`command_tests` 保护普通命令输出、SET/GET/DEL、TTL、snapshot/WAL 等核心行为。它证明拆分后每个 family executor 的结果仍能被原有命令测试接受。`command_catalog_refactor_fixture_tests` 保护命令目录和契约输出，避免 `HELP`、`COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 因拆分而漂移。`readonly_fixture_tests` 和 `runtime_smoke_evidence_tests` 保护只读证据和 runtime evidence 输出，确认这次 command 侧重构没有误改证据通道。

`wal_tests` 和 `snapshot_tests` 分别保护 persistence executor 最敏感的两个边界：WAL append/compact/replay 与 snapshot save/load。`tcp_resp_tests` 证明通过 TCP/RESP 进入时，命令仍走同一个 `CommandProcessor` 路径。全量 `337/337` CTest 则额外覆盖大量 shard readiness、restore、credential resolver non-participation、runtime shell non-participation 等证据模块，说明这次拆分没有连带破坏后段 fixture。

本版还更新了 `.github/workflows/ci.yml` 的 coverage filter，把 `command_expiry_ops`、`command_introspection_ops`、`command_persistence_ops`、`command_string_ops` 纳入 Ubuntu coverage job。`tests/dependabot_config_tests.cpp` 同步校验这个 filter，避免未来有人只拆文件、不把新文件纳入 coverage gate。这里的重点不是追求覆盖率数字更漂亮，而是延续 K2 的承诺：被搬走的核心命令逻辑仍在同一条 coverage floor 下面。

## 边界字段怎么理解

本版没有直接新增 JSON 字段，但它继续保护几个既有字段的含义。`read_only` 表示证据或契约查询本身不修改状态；`execution_allowed` 表示控制面查看契约时不允许把目标命令实际执行起来；`write_command` 描述目标命令类别，而不是当前 `CHECKJSON` 行为；`touches_wal` 和 `append_when_enabled` 描述目标命令在 WAL 启用时的持久化副作用；`warnings` 继续说明 WAL disabled、not a write command 等解释性状态。拆分以后，这些字段仍由 `command_contracts` 集中生成，family executor 不私自拼接契约 JSON。

对 shard 方向而言，v1618 没有安装 active shard router，没有打开 write routing，没有给 mini-kv 增加 order authority，也没有让 Node 或 Java 能触发 mini-kv 的 restore/load/compact。`SHARDJSON`、`SHARDROUTE*` 和大量 non-participation receipt 仍在 runtime evidence 分支和 shard readiness 体系中，下一步 K4 会处理 `shard_readiness.cpp` 的拆分，但本版故意不把两个风险域混在一个提交里。

## 为什么不把所有命令一次性拆完

从代码体积看，`src/command.cpp` 在 v1618 之后仍然不算小，因为 runtime evidence 和大量 `SHARDROUTE*` receipt 命令还留在那里。这里没有继续把它们一口气拆掉，是有意控制风险。K4 的原始目标是先处理两个最危险的大文件：`command.cpp` 和 `shard_readiness.cpp`。v1617 已经把命令分发函数移出旧文件，v1618 再把普通命令执行家族拆出来；这两步解决的是“核心命令面”继续膨胀的问题。runtime evidence 分支和 shard route receipt 分支虽然也长，但它们的性质更接近证据目录和跨项目只读材料，不应该和普通存储命令在同一版里同时大搬家。

如果把 runtime evidence 分支也和本版四个 family executor 一起拆，短期看 diff 会更壮观，长期看反而难审。维护者很难一眼判断失败来自普通命令、证据命令，还是 shard readiness 的格式拼接。现在的切法把风险分成三段：第一段是 v1617 的分发/解析/WAL gate，第二段是 v1618 的普通命令家族，第三段才是后续的 shard readiness 格式与验证器。每段都有单独构建、定向测试、全量测试和真实 smoke，任何问题都能对应到清楚的版本边界。

这也是 mini-kv 作为学习型生产雏形项目需要建立的节奏：不是为了“拆分”而拆分，而是让每一刀都能回答三个问题：外部行为有没有变，证据链有没有变，后续维护者读代码是不是更容易。v1618 的答案是肯定的。外部行为由 command tests、fixture tests、CLI/TCP smoke 证明；证据链由 `CHECKJSON`、只读 fixture、runtime evidence tests 和全量 CTest 证明；维护性则体现在四个 family 文件的命名和职责边界上。

## 失败模式和排查方式

如果 v1618 这类拆分出错，最可能的失败模式有四类。第一类是解析状态被消费错位，比如 dispatch 已经读走 command token，family executor 又错误地重新读一次 command，导致 `SET key value` 变成缺参。v1618 避免了这个问题：`execute_trimmed` 只读取首个 command token，然后把同一个 `std::istringstream& input` 传给 family executor；executor 从剩余参数开始解析，行为和原 switch case 相同。定向 `command_tests` 能覆盖这类错误。

第二类是 WAL gate 或 WAL record 搬错。`SET`、`DEL`、`SETNXEX`、`EXPIRE` 都需要通过 `execute_with_wal` 保持“先 append，后 mutation”的语义，`COMPACT` 和 `WALINFO` 则需要直接进入 `wal_command_gate`。v1618 没有复制出新的 gate 实例，而是继续复用 v1617 的 `command_internal::wal_command_gate()`，这能避免不同 translation unit 各自持有静态 gate 导致互斥语义失效。`wal_tests` 和真实 CLI/TCP smoke 会覆盖这类路径。

第三类是 introspection 命令误执行目标命令。比如 `CHECKJSON SET a b` 只能描述 `SET` 是写命令、可能 touch WAL，不能真的写入 key。v1618 把 `CHECKJSON` 放进 introspection executor，就是为了让这类“解释命令”的边界集中可见。它只调用 `command_contracts::format_check_json`，不会走 string executor，也不会调用 `store_.set`。CLI smoke 中 `CHECKJSON SET family-key family-value` 返回契约 JSON，而不是写入新值，这正是它需要证明的行为。

第四类是 CI coverage 漏掉新文件。大文件拆分后，如果新 `.cpp` 没进入 gcovr filter，覆盖率报告可能表面仍绿，但真正被搬走的代码不再受 floor 约束。v1618 因此同步修改 `.github/workflows/ci.yml`，并让 `dependabot_config_tests.cpp` 检查 filter 中包含四个新 family 文件。这样后续如果有人改 CI filter、删掉新文件名字，测试会先失败，不会等到 review 时才靠人工发现。

## 后续维护者怎么读这组文件

后续如果要查普通 key/value 语义，先看 `src/command_string_ops.cpp`。这个文件应该只讨论 key/value 存取，不应该塞入 snapshot、WAL compact 或 shard preview 内容。如果未来新增类似 `MGET`、`APPEND`、`INCR` 这类字符串或键值语义命令，也应优先判断它是否属于 string family，并继续复用 `execute_with_wal` 的写入模板，而不是在 dispatch 里直接展开。

如果要查 TTL 或过期语义，看 `src/command_expiry_ops.cpp`。这里的共同语言是秒数解析、绝对过期时间、`SETEXAT`/`EXPIREAT` WAL record，以及 store 层的 `expire_at`、`ttl`、`set_if_absent`。未来如果出现 `PERSIST`、`EXPIREAT` 这类命令，也应先进入 expiry family，因为它们共享“时间边界”和“是否修改过期元数据”的判断。

如果要查磁盘或 WAL maintenance 行为，看 `src/command_persistence_ops.cpp`。这个文件的审查重点不是普通命令输出，而是显式用户命令是否可能触碰文件系统、WAL append/compact、snapshot save/load。它和证据消费面的 no-execution 边界要分开理解：用户在 CLI 里输入 `LOAD path` 是显式执行，`CHECKJSON LOAD path` 或 Node 读取 mini-kv evidence 则仍只能看到契约说明，不能触发 load。

如果要查控制面解释、帮助文本、keys 展示或 metrics reset，看 `src/command_introspection_ops.cpp`。这个文件里的命令多数是只读或元信息命令；唯一会修改内部状态的是 `RESETSTATS`，但它只清理 metrics tracker，不写 store、不写 WAL、不改 snapshot。把它们放在一起，可以让维护者更容易区分“业务状态修改”和“观测/解释状态修改”。

最后，`src/command_dispatch.cpp` 后续应该保持薄。它的理想状态是：解析 command token、查 catalog、把 verb 分发给对应 family，再处理 `PING`/`QUIT` 这种协议控制命令。只要 dispatch 又开始出现几十行命令细节，就说明拆分纪律开始滑坡，需要在当版立刻收住。

## 本版价值

v1618 的价值在于把 command 执行面从一个继续膨胀的 switch，压成“薄分发 + 四个小执行家族”。以后维护者查 `SET/GET/DEL` 不需要扫 snapshot/WAL maintenance，查 `COMPACT/WALINFO` 不需要穿过 `KEYSJSON` 和 `CHECKJSON`，查契约 introspection 也能更直接地看到它不执行目标命令。对后续 Node/Java/mini-kv 松耦合融合来说，本版让 mini-kv 的命令边界更容易审计，同时保持版本化证据、只读契约、no router、no write routing、no execution 的既有语义不变。
