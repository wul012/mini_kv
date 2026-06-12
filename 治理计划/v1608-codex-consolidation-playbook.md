# v1608 Codex consolidation playbook

本 playbook 给后续 Codex 执行 mini-kv 治理收口时使用。它不是授权一次性大重构，而是规定每一批如何小步推进、如何验证、如何停手。

## Step 0：每批开始前

1. 只读读取 Node 最新计划入口：

   ```text
   D:\nodeproj\orderops-node\docs\plans\README.md
   D:\nodeproj\orderops-node\docs\plans2\README.md
   D:\nodeproj\orderops-node\docs\plans3\README.md
   ```

2. 如果 latest concrete plan 是 Node 内部 consolidation，并声明 Java / mini-kv recommended parallel，则 mini-kv 可以继续自己的治理拆分。
3. 如果 latest plan 要求 mini-kv 提供新 evidence，则优先判断是否与本治理计划冲突。
4. 如果 latest plan 要求移动历史 archive、读取 Node f-folder、启动 sibling 服务或执行生产动作，停止并让用户确认。

## Step 1：不要碰这些东西

- 不移动、不重命名、不删除 `a/`、`b/`、`c/`、`d/`、`e/`、`f/` 的历史内容。
- 不移动、不重命名 `fixtures/release/*.json`。
- 不批量迁移旧 `代码讲解记录_*` 文件。
- 不直接改 Node / Java / aiproj。
- 不为了清理仓库体积而破坏路径稳定。

## Step 2：command.cpp 拆分批次

适合拆的代码：

- 只读 runtime evidence command handler。
- 重复的 usage guard。
- 与 store/WAL/snapshot 无关的 JSON evidence formatter dispatch。

不适合拆的代码：

- `SET` / `DEL` / `EXPIRE` / `SETNXEX` 的 WAL append-before-mutation 顺序。
- `SAVE` / `LOAD` / `COMPACT` 的行为。
- TCP/RESP 行为。
- 任何没有 focused test 的公共命令行为。

推荐每批流程：

```powershell
$cmake = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
$ctest = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\ctest.exe'
$env:PATH = 'D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
& $cmake -S . -B cmake-build-vNEXT -G 'MinGW Makefiles'
& $cmake --build cmake-build-vNEXT -- -j 4
& $ctest --test-dir cmake-build-vNEXT -R 'command_tests|command_catalog_tests|line_editor_tests|f_folder_explanation_quality_closeout' --output-on-failure --timeout 120 --progress
& $ctest --test-dir cmake-build-vNEXT --output-on-failure --timeout 120 --progress
```

CLI smoke 使用 UTF-8 无 BOM 输入文件重定向，不使用 PowerShell 直接管道：

```powershell
[System.IO.File]::WriteAllText(
  'cli-smoke-input.txt',
  "COMMANDSJSON`nCHECKJSON SHARDJSON`nQUIT`n",
  [System.Text.UTF8Encoding]::new($false)
)
cmd /c ".\cmake-build-vNEXT\minikv_cli.exe < cli-smoke-input.txt"
```

## Step 3：shard_readiness.cpp 拆分批次

适合拆的 section：

- current release summary。
- command catalog quality。
- source frozen fixture lineage。
- 最近生产 shard execution evidence section。
- diagnostics / boundary policy helper。

每批只拆一个主题。拆完后必须证明：

- `SHARDJSON` 可执行。
- current fixture 仍是当前版本。
- 旧 source fixture 仍被冻结。
- `readOnly`、`executionAllowed:false`、`mutatesStore:false`、`touchesWal:false`、`writeRoutingAllowed:false` 保持。
- 不启动服务、不读取 sibling archive、不导入 Node / Java 模块。

## Step 4：archive retention 批次

只允许做：

- 新增 root-level README / index。
- 给 active root 写版本范围说明。
- 给未来 root 轮转写规则。
- 给大截图的未来策略写“精选截图 + 文本摘要 + 哈希”建议。

禁止做：

- 移动 `e/<version>/`。
- 删除旧截图。
- 把旧 PNG 替换成压缩版。
- 改写历史解释路径。
- 修改 Node 已可能引用的旧 evidence JSON。

## Step 5：每批 closeout

代码批次：

- focused CTest。
- full CTest。
- CLI smoke。
- 3000+ 中文讲解。
- 版本归档截图和说明。
- cleanup gate。
- commit / tag / push。
- CI 如果失败，修复后再收口。

文档批次：

- `git diff --check`。
- 检查文档路径和关键规则存在。
- 说明没有 C++ 行为变化，因此不运行 CMake/CTest，或在风险较高时主动跑 targeted CTest。
- cleanup gate。
- commit / tag / push。

## Step 6：停止条件

看到以下任一情况立即停：

- 需要移动历史 archive 才能继续。
- 拆分导致 public command output 变化，但没有明确计划和测试。
- 新增文件只是转移复杂度，没有减少 `command.cpp` 或 `shard_readiness.cpp` 的真实负担。
- 版本太薄，讲解无法自然达到 3000 字。
- CI 失败但没有日志或可诊断信息。

## Step 7：推荐提交信息

```text
docs: add mini-kv governance consolidation roadmap v1608
refactor: split runtime evidence command handlers v1609
refactor: split shard readiness section builders v1610
docs: add mini-kv archive retention index v1611
```

一句话：后续 Codex 推 mini-kv 治理时，先保契约，再拆结构；先保历史路径，再谈归档整洁；先让测试能失败，再让版本能解释。
