# mini-kv v110 Dependabot GitHub Actions maintenance

本版目标是补安全维护配置：mini-kv 已经有 GitHub Actions CI，但还没有自动依赖更新提醒。v110 给仓库增加 Dependabot，仅覆盖 GitHub Actions，并用 CTest 守住配置边界。

这不是依赖升级版本，不是业务功能，不是 runtime evidence 变更，不启动 Node/Java/mini-kv 服务，不修改 WAL、snapshot、restore、command dispatch，也不引入 npm、Maven、pip、cargo、Docker、vcpkg 或 Conan 管理。

## 入口

新增入口是：

```text
.github/dependabot.yml
```

它只配置一个 ecosystem：

```text
package-ecosystem: "github-actions"
directory: "/"
```

这说明 Dependabot 只检查 `.github/workflows` 里的 action 版本，比如 `actions/checkout@v4`。mini-kv 当前没有 C++ 包管理器，因此不提前配置 vcpkg/conan；也不是 Node 或 Java 项目，所以不配置 npm/Maven。

## 维护策略

配置策略保持保守：

- `interval=weekly`
- `timezone=Asia/Shanghai`
- `open-pull-requests-limit=5`
- minor/patch action updates 进入 `github-actions-minor-patch` 分组
- semver-major 自动版本更新通过 `version-update:semver-major` 忽略
- PR label 使用 `dependencies` 和 `github-actions`
- commit message prefix 使用 `ci`

控制面读到这个配置时应该理解为：mini-kv 只是在补 GitHub Actions 依赖提醒，不是在自动升级 C++ runtime dependency，也不是引入新的构建生态。

## 测试

`tests/dependabot_config_tests.cpp` 是一个小型配置守卫。它读取：

- `.github/dependabot.yml`
- `.github/workflows/ci.yml`

测试断言 Dependabot 配置必须包含 GitHub Actions ecosystem、weekly schedule、Asia/Shanghai timezone、minor/patch 分组、semver-major ignore 和 labels；同时明确禁止出现 npm、Maven、pip、cargo、Docker 等 ecosystem。

测试还确认 CI 仍然是原来的 CMake/CTest 基线：`actions/checkout@v4`、`cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug` 和 `ctest --test-dir build -C Debug --output-on-failure` 都必须存在。

## CMake 接线

`CMakeLists.txt` 新增：

```text
minikv_dependabot_config_tests
dependabot_config_tests
```

该测试不链接 `minikv` 库，也不启动服务，只读取仓库文本文件。这让它非常便宜，适合作为安全维护配置的回归保护。

## 归档

本版运行截图和说明写入：

- `c/110/图片/`
- `c/110/解释/说明.md`

归档包含 CMake configure、build、全量 CTest、以及 Dependabot 配置 smoke。smoke 读取 `.github/dependabot.yml` 并验证只包含 GitHub Actions 维护范围。

## 一句话总结

v110 给 mini-kv 补上 GitHub Actions 依赖维护的安全网，同时用测试防止配置越界到尚未采用的包管理生态，为后续三项目安全维护批次提供低风险 C++ 侧闭环。
