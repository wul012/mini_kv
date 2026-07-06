# OSFS coursework delivery v1633

v1633 是 `治理计划/v1631-osfs-coursework-completion-brief.md` 的交付收口。它只在 v1631 的磁盘用户表/MFD/UFD 与 v1632 的 fd 位移/range I/O 已通过测试后生成材料，取代 v1630 的单根目录草稿包。

交付目录：`课程设计交付/v1633-osfs-final/`。

关键结论：课程 PDF 明确要求的二进制磁盘、用户信息、inode、超级块、文件信息、MFD/UFD 二级目录、login、dir/create/delete/open/close/read/write 和读写保护均有源码、测试及真实 CLI 证据。完整 EXT2 group descriptor、间接块、fsck、崩溃日志和多进程并发未实现，均在报告与需求矩阵中列为教学简化或后续工作。

旧目录 `课程设计交付/v1630-osfs-final/` 原地保留并标注 superseded，不再作为提交入口。
