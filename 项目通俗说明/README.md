# mini-kv 项目通俗说明索引

这个文件夹保存面向新读者、维护者和跨项目协作者的通俗说明。它不替代根目录 `README.md`、`docs/CHANGELOG.md` 或运行证据归档；它的作用是把 mini-kv 的目标、价值、输入输出和关键机理讲清楚。

## 当前说明

- `mini-kv-项目机制与价值说明.md`：解释 mini-kv 是什么、为什么有价值、普通命令/WAL/snapshot/TCP/RESP/只读证据命令分别如何输入和输出，以及为什么 `CHECKJSON` / `SMOKEJSON` 只做边界证明而不是执行生产动作；v1624 起，其中的最小命令故事由 `project_orientation_examples_tests` 可执行验证。

## 维护规则

- 优先用中文书写。
- 讲机理，不写营销口号。
- 可以补图，但图必须服务于输入、输出、状态变化和边界说明。
- 不把版本流水账塞到这里；版本历史继续放在 `docs/CHANGELOG.md`。
- 不把真实运行证据塞到这里；截图和命令归档继续放在 `f/<version>/`。
