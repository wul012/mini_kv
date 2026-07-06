# OSFS 操作系统课程设计交付包 v1630

本目录用于提交和答辩 mini-kv v1629 之后整理出的 OSFS 课设成果。主题对应课程要求中的“实验二 Linux 二级文件系统”：用二进制文件模拟磁盘空间，在其上实现超级块、块位图、inode 表、目录项、文件读写、用户登录和读写权限。

## 文件说明

- `操作系统课程设计报告-OSFS-v1630.docx`：正式可编辑报告，由 `tools/build_osfs_coursework_report.py` 生成。
- `操作系统课程设计报告-OSFS-v1630.pdf`：由 LibreOffice 从 docx 导出的打印版。
- `演示步骤.md`：课堂演示时可以照着执行的命令和讲解顺序。
- `提交清单.md`：纸质版、电子版、源码和成绩考核表的提交核对清单。
- `osfs-demo-script.txt`：OSFS 命令脚本，配合 `minikv_osfs --script` 使用。
- `osfs-demo-transcript.txt`：本版真实演示输出，生成报告时会作为结果依据。
- `images/`：报告中的结构图、磁盘布局图和写入流程图。

## 当前边界

OSFS 是独立课程设计入口，不改变 mini-kv 的 KV、WAL、snapshot、restore、TCP/RESP 主线。演示时只需要运行 `minikv_osfs`，不需要启动 `minikv_server`，也不需要打开任何 Node/Java managed-audit 链路。
