# v1670 WAL/Snapshot 原子文件写入收口

## 背景与目标

`wal.cpp` 与 `snapshot.cpp` 当前各自维护一份临时文件 RAII、临时路径生成、Windows/POSIX 替换函数，以及相同的 flush/close/rename 失败控制流。两份实现功能稳定，但第三次使用前应先形成唯一基础设施。本版提取私有 `AtomicFileWriter`，让 WAL compact 与 Snapshot save 只负责序列化领域数据；临时文件生命周期、提交和失败清理由同一实现负责。

本版不修改 WAL2 或 snapshot 文件格式，不改变 public header、命令输出、自动 compact 决策、replay/load 语义，也不把 OSFS 镜像写入并入该抽象。它不是事务系统、跨文件提交、备份策略或生产级崩溃一致性承诺；“atomic”只指同目录临时文件完成后，以平台替换原语切换单个目标路径。

## Family 设计说明（先于实现，8 行）

1. 抽象名：`AtomicFileWriter`，位于私有 `src/`，不扩 public API。
2. 数据边界：只持有 target、同目录唯一 temp path、`ofstream` 与提交状态。
3. 行为边界：构造时准备父目录并打开 temp；`commit` 统一 flush、close、replace。
4. 析构只做失败/放弃后的 best-effort temp cleanup，不删除已提交 target。
5. WAL 与 Snapshot 仍分别拥有编码、字段顺序、过期时间和计数语义。
6. Windows 继续使用 `MoveFileExW(REPLACE_EXISTING|WRITE_THROUGH)`；POSIX 继续使用 rename。
7. 临时 tag 只影响内部文件名：Snapshot 保留 `.tmp.`，WAL 保留 `.compact.tmp.`。
8. 不加入 callback、模式枚举、异常跨边界传播或多文件事务。

## 需求—证据矩阵

| 需求 | 实现 | 机械证据 | 初始状态 |
|---|---|---|---|
| 单一原子写 engine | `src/atomic_file_writer.*` | 独立 CTest + 两个 owner 不再定义 temp/replace helper | 完成 |
| 成功替换与重复提交边界 | writer success case | 新内容替换旧目标、temp 消失、第二次 commit 失败 | 完成 |
| 放弃与坏流清理 | writer RAII | 原目标不变、临时文件离开作用域后消失 | 完成 |
| replace/父目录失败安全 | 目录目标与文件阻塞父路径 | commit/open 失败，原对象保留且无 temp 泄漏 | 完成 |
| WAL/Snapshot 集成稳定 | 两个调用点只保留编码 | 原 `wal_tests`、`snapshot_tests` 与 temp census | 完成 |
| coverage 与测试架构诚实 | CI filter + contract expectation | 新源进入 core filter，CTest inventory 353→354 | 完成 |
| 版本闭环 | 中文讲解、归档、commit/tag/push/CI | `f/1670/` + walkthrough `1038` + Actions | 本地完成，远端由 v1670 tag 验证 |

## 本地验证结果

- 新 writer 直属与 owner/恢复聚焦集合 17/17 通过；全量 CTest 354/354、0 失败、180.07 秒。
- 真实 CLI 完成 `SAVE -> 修改 -> LOAD -> COMPACT -> 重开 WAL replay`，两键恢复，2 条记录重放，临时文件残留为 0。
- 改动 C++ 全部通过 clang-format dry-run；`fixtures/release` 与公共头文件未改。
- 中文讲解 `1038` 含 3956 个汉字，在最终全量 verify 前完成。

## 完成门槛

- 两份复制的 Temp class、temp-path factory 和 replace helper 从 WAL/Snapshot owner 删除。
- 新基础设施与测试新增源码合计不超过 400 行；新增文件和标识符不超过 40 字符。
- 独立 writer 六类故障/成功路径、WAL、Snapshot、coverage contract、全量 CTest 与真实 SAVE/LOAD/COMPACT smoke 全绿。
- `fixtures/release`、WAL2/snapshot 格式、public header、命令响应和历史 archive 不变。
- 讲解在最终 verify 前完成，并明确原子替换不等于多文件事务或完整掉电持久性。

## 失败条件

- 用删除/覆盖原目标再 rename 的非原子回退适配 Windows：失败。
- writer 接管 WAL/Snapshot 编码或引入 kind 分支：回退。
- 为测试方便暴露 public fault-injection API：回退。
- commit 失败后旧目标改变，或 temp 文件残留：失败。
- 新源未进入 coverage filter、测试期望被弱化、fixture 被改写：失败。
