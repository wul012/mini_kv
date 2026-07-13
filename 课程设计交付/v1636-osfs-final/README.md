# mini-kv OSFS 课程设计最终交付包（v1636）

本目录对应课程要求中的“Linux 二级文件系统”课程设计。v1636 不是 v1633 的简单换名，而是在 v1634/v1635 已完成的实质机制上重建最终材料：磁盘格式 v3、一级间接块、只读 FSCK、root-only 用户创建、密码修改、UTF-8 演示 transcript、需求-证据矩阵和重新渲染的 DOCX/PDF 报告都已同步。

面向学生的机制讲解已在后续维护版整理到仓库根目录 `OSFS课程设计通俗教程/`。FinalShell 分段记录的 2026-07-13 复核说明位于 `finalshell-分段演示记录/00-复核说明.md`，其中如实披露了预置用户重复 USERADD 被旧 grep 隐藏的问题，并把新增间接块/损坏 FSCK 步骤标为待远端实跑。

## 主要文件

- `操作系统课程设计报告-OSFS-v1636.docx`：可编辑正式报告，姓名、学号、班级、教师保留填写位。
- `操作系统课程设计报告-OSFS-v1636.pdf`：与 DOCX 同内容的提交/预览版本，会在本版收尾时由 DOCX 渲染生成。
- `需求-证据矩阵.md`：逐项说明课程要求、实现位置、测试/演示证据和状态。
- `演示步骤.md`：课堂演示顺序、预期现象和讲解重点。
- `osfs-demo-script.txt`：可直接交给 `minikv_osfs --script` 的真实演示脚本。
- `osfs-demo-transcript.txt`：由 v1636 构建出的 `minikv_osfs.exe` 真实运行生成，UTF-8 保存，无 UTF-16 空字节问题。
- `提交清单.md`：提交前需要填写和核对的项目。
- `images/`：报告中的结构、磁盘布局、间接块和 FSCK/用户管理流程图。

## 复现

```powershell
$env:PATH='D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
$cmake='D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
& $cmake -S . -B cmake-build-osfs -G Ninja `
  '-DCMAKE_MAKE_PROGRAM:FILEPATH=D:\CLION\CLion 2025.1.1\bin\ninja\win\x64\ninja.exe' `
  '-DCMAKE_CXX_COMPILER:FILEPATH=D:\CLION\CLion 2025.1.1\bin\mingw\bin\g++.exe'
& $cmake --build cmake-build-osfs --target minikv_osfs minikv_osfs_tests
.\cmake-build-osfs\minikv_osfs.exe --disk osfs-course.img --format --blocks 160 --script .\课程设计交付\v1636-osfs-final\osfs-demo-script.txt
```

默认教学账号为 `root/root`、`alice/alice123`、`bob/bob123`。脚本中会由 root 新建 `carol` 并把密码改为 `carol456`，证明用户管理不是只写死三个账号。

## 已披露边界

OSFS 完成的是 MFD/UFD 两级目录，不支持任意深度目录树；每个普通文件支持八个直接块加一个一级间接块，不做二级/三级间接块；`FSCK` 是只读诊断，不自动修复；当前仍是单教学 block group，没有 EXT2 group descriptor、崩溃日志和多进程并发。它不连接 mini-kv 的 KV/WAL/snapshot/TCP/RESP 主线。
