# 第一百零六次讲解：第五十二版 CHECKJSON fixture 样本

第五十二版按最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v71-post-execution-contract-roadmap.md
```

计划要求 mini-kv v52 做：

```text
为 CHECKJSON 增加稳定 fixture / sample response。
样本字段覆盖 schema_version、command_digest、read_only、execution_allowed、
side_effects、side_effect_count、wal.durability。
```

本版没有改 Node，也没有执行真实写命令，只让 mini-kv 自己产出一个能被后续 Node smoke 读取的稳定样本。

---

# 1. 新增 fixture 目录

新增目录：

```text
fixtures/checkjson/
```

里面放两份文件：

```text
README.md
set-orderops-write-contract.json
```

`README.md` 说明这个样本来自什么命令、处于什么运行条件、为什么可以给外部控制面 smoke 使用。

---

# 2. 稳定样本内容

样本文件是：

```text
fixtures/checkjson/set-orderops-write-contract.json
```

对应命令：

```text
CHECKJSON SET orderops:1 value
```

样本是紧凑 JSON，一行保存，和运行时真实响应保持同一种格式。

关键字段包括：

```text
schema_version=1
read_only=true
execution_allowed=false
command_digest=fnv1a64:9f5c7ff14c0ebc5e
side_effects=["store_write","wal_append_when_enabled"]
side_effect_count=2
wal.durability=memory_only
```

这里 `memory_only` 是因为样本定义为 no-WAL 场景：

```text
wal.enabled=false
```

---

# 3. 为什么不做成新的运行命令

v52 只需要稳定样本，不需要再给 mini-kv 增加新命令。

原因是：

```text
CHECKJSON 本身已经能实时生成 contract。
Node v74 后续需要的是 fixture-driven smoke 的稳定输入。
```

所以本版最小闭环是：

```text
真实 CHECKJSON 输出
 -> 写成 fixture
 -> 测试锁住 fixture 不漂移
```

---

# 4. CMakeLists.txt 的改动

版本号从：

```text
0.51.0
```

提升到：

```text
0.52.0
```

并给 `minikv_command_tests` 增加编译定义：

```cmake
MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
```

这样测试无论从 build 目录还是源码目录启动，都能稳定找到：

```text
fixtures/checkjson/set-orderops-write-contract.json
```

---

# 5. command_tests 怎么锁住样本

新增 helper：

```cpp
read_fixture_text(...)
```

它做三件事：

```text
1. 用 MINIKV_SOURCE_DIR 定位源码目录
2. 读取 fixture 文件
3. 去掉末尾换行，避免 Windows/Linux 行尾差异影响比对
```

然后在 `CHECKJSON SET orderops:1 value` 测试处增加：

```cpp
assert(result.response == checkjson_set_fixture);
```

这个断言的意义很直接：

```text
只要 CHECKJSON 输出字段、顺序、digest、warning 或 WAL contract 改了，
测试就会失败，提醒同步 fixture 和 Node smoke。
```

---

# 6. 一句话回看

v52 把 v51 的运行时 `CHECKJSON` contract 固化成一个可复用样本：

```text
v51：能生成 CHECKJSON contract
v52：给 CHECKJSON contract 提供稳定 fixture，并用测试防漂移
```
