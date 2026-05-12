# 第一百零八次讲解：第五十三版 CHECKJSON read-command fixture

第五十三版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v75-post-fixture-diagnostics-roadmap.md
```

计划要求 mini-kv v53 新增一个 read-command CHECKJSON sample，用于 Node v78 的 fixture scenario matrix。

本版只新增 mini-kv 自己的 fixture / sample，不修改 Node，不执行 mini-kv 写命令。

---

# 1. 新增 read fixture

新增文件：

```text
fixtures/checkjson/get-orderops-read-contract.json
```

对应命令：

```text
CHECKJSON GET orderops:1
```

这个样本和 v52 的写命令样本放在同一个目录：

```text
fixtures/checkjson/
```

这样 Node 后续读取 mini-kv fixture 时，可以用一个目录拿到 write / read 两类场景。

---

# 2. 为什么 side_effects 是 store_read

本版没有把 read command 的 side effect 手写成空数组。

原因是 mini-kv 当前稳定解释语义里：

```text
GET
 -> side_effects=["store_read"]
 -> side_effect_count=1
```

`store_read` 表示它会读取 Store，但不会写 Store，也不会触碰 WAL。

所以 read fixture 里的关键字段是：

```text
read_only=true
execution_allowed=false
write_command=false
side_effects=["store_read"]
side_effect_count=1
wal.durability=not_applicable
warnings=["not a write command"]
```

这和最新计划修正后的要求一致：样本必须来自当前真实 `CHECKJSON GET` 语义。

---

# 3. digest

样本里的 digest 是：

```text
fnv1a64:e8d7a3d61e2a8111
```

它来自：

```text
CHECKJSON GET orderops:1
```

这个 digest 和 `EXPLAINJSON GET orderops:1` 的 command digest 保持一致，因为 `CHECKJSON` 复用的是同一个 command explain 结果。

---

# 4. command_tests 的新增断言

原来 v51 已经有片段级断言：

```text
CHECKJSON GET orderops:1
 -> command=GET
 -> write_command=false
 -> durability=not_applicable
```

v53 在这里新增完整 fixture 比对：

```cpp
const auto checkjson_get_fixture =
    read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "get-orderops-read-contract.json");
assert(result.response == checkjson_get_fixture);
```

意思是：

```text
真实 CHECKJSON GET 响应必须和 read fixture 一模一样。
```

如果未来字段、顺序、digest、warning 或 side_effect_count 漂移，测试会立刻失败。

---

# 5. fixture README

`fixtures/checkjson/README.md` 补充了第二个样本：

```text
get-orderops-read-contract.json
```

并说明：

```text
这是 read-command contract
不触碰 WAL
wal.durability=not_applicable
side_effect 是 store_read
```

这给 Node v78 的多场景矩阵一个清楚的来源说明。

---

# 6. 一句话回看

v53 把 CHECKJSON fixture 从“只有写命令样本”扩成“写/读两个场景”：

```text
SET orderops:1 value
 -> write-command contract

GET orderops:1
 -> read-command contract
```

这让后续 Node scenario matrix 能同时展示 mini-kv 写命令和读命令两种只读执行前证据。
