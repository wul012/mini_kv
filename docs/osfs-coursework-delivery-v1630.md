# OSFS coursework delivery v1630

> Superseded：本页记录的是单根目录阶段草稿。最终交付与准确需求结论见 `docs/osfs-coursework-delivery-v1633.md` 和 `课程设计交付/v1633-osfs-final/`。

v1630 是 v1629 OSFS 功能层之后的课程设计交付收尾版。它不新增文件系统行为，重点把现有 OSFS 能力整理成老师能验收、学生能演示、平台能提交的材料。

## 对应课程要求

课程 PDF 中“实验二 Linux 二级文件系统”要求用二进制文件模拟磁盘空间，保存用户信息、inode 信息、超级块信息和文件信息，实现 `dir`、`create`、`delete`、`open`、`close`、`read`、`write`、`login` 等命令，并支持源文件读写保护。v1629 的 OSFS 已覆盖这些核心项，v1630 负责把证据整理到报告、图和演示脚本中。

## 交付目录

```text
课程设计交付/v1630-osfs-final/
```

关键文件：

- `操作系统课程设计报告-OSFS-v1630.docx`
- `操作系统课程设计报告-OSFS-v1630.pdf`
- `演示步骤.md`
- `提交清单.md`
- `osfs-demo-script.txt`
- `osfs-demo-transcript.txt`
- `images/`

## 边界

OSFS 是独立课设入口，不改变 mini-kv KV/WAL/TCP 主线，不启动服务端，不打开 Node/Java managed-audit 链路，不给外部系统写入或执行权限。后续如果继续扩展，应该优先补多级目录、group 权限或 fsck 检查，而不是把 OSFS 与原 KV 命令混在一起。
