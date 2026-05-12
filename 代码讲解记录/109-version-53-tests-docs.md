# 第一百零九次讲解：第五十三版测试、README、归档和整体增删改

第一百零八次讲的是 v53 核心：

```text
fixtures/checkjson/get-orderops-read-contract.json
```

这一篇讲配套修改：

```text
CMakeLists.txt
tests/command_tests.cpp
README.md
a/53/解释/说明.md
a/53/图片/
代码讲解记录/README.md
```

---

# 1. CMake 版本号

版本号从：

```text
0.52.0
```

提升到：

```text
0.53.0
```

这让 `INFO` / `INFOJSON` 继续能反映当前版本。

---

# 2. 测试新增了什么

`command_tests` 继续使用 v52 增加的：

```cpp
read_fixture_text(...)
```

v53 新增读取：

```text
fixtures/checkjson/get-orderops-read-contract.json
```

并做完整响应比对。

这不是只检查几个字段，而是确保完整 JSON 样本和运行时响应完全一致。

---

# 3. 真实 smoke 怎么验证

本版真实 smoke 使用：

```text
minikv_server.exe
minikv_client.exe
```

流程：

```text
启动 server
发送 CHECKJSON GET orderops:1
读取 fixtures/checkjson/get-orderops-read-contract.json
比较 client 返回 JSON 与 fixture 是否一致
发送 GET orderops:1
确认仍然是 (nil)
发送 QUIT
停止 server
```

这里的重点是：

```text
CHECKJSON GET 是 contract 查询
GET orderops:1 是真实读
两者都不写入数据
```

---

# 4. README 更新

README 当前版本改成：

```text
Version 53
```

能力描述改成：

```text
stable CHECKJSON write/read fixture samples
```

并新增：

```text
fixtures/checkjson/get-orderops-read-contract.json
```

说明这个 read fixture 包含：

```text
side_effects=["store_read"]
side_effect_count=1
```

---

# 5. Roadmap 更新

Roadmap 从“等待 Node 消费单个 CHECKJSON fixture”推进为：

```text
等待 Node v78 消费 CHECKJSON write/read fixtures 做 scenario matrix
```

这和最新计划一致：

```text
Node v78：Fixture scenario matrix
```

---

# 6. 归档内容

本轮归档放在：

```text
a/53/图片/
a/53/解释/说明.md
```

截图覆盖：

```text
cmake configure
cmake build
ctest
command fixture tests
read fixture smoke
version/docs check
code explanation check
cross roadmap check
```

---

# 7. 一句话回看

v53 是一个合适大小的小版本：

```text
新增一个 read-command fixture
用测试锁住真实输出
用 server/client smoke 验证实际链路
同步 README、讲解和归档
不扩大到 Node/Java
```
