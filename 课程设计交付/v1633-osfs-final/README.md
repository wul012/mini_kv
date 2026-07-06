# mini-kv OSFS 课程设计最终交付包（v1633）

本目录对应课程要求中的“实验二 Linux 二级文件系统”。v1633 交付建立在 v1631、v1632 的功能实现上，不再沿用 v1630 的单根目录说明：磁盘格式 v2 已保存持久化用户表、MFD、每用户 UFD、block/inode 双位图、inode 表和文件数据；命令行支持密码登录、目录隔离、权限拒绝以及带读写位移的 fd。

## 主要文件

- `操作系统课程设计报告-OSFS-v1633.docx`：可编辑正式报告，姓名、学号、班级、教师保留填写位。
- `操作系统课程设计报告-OSFS-v1633.pdf`：与 DOCX 同内容的提交/预览版本。
- `需求-证据矩阵.md`：课程要求、实现位置、测试/演示证据和状态。
- `演示步骤.md`：课堂演示顺序、预期现象和讲解重点。
- `osfs-demo-script.txt`：可直接交给 `minikv_osfs --script` 的脚本。
- `osfs-demo-transcript.txt`：由真实 v1632 功能可执行程序运行生成的输出。
- `提交清单.md`：提交前需要填写和核对的项目。
- `images/`：报告中的系统结构、磁盘布局和 fd 位移图。

## 复现

```powershell
$env:PATH='D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
$cmake='D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
& $cmake -S . -B cmake-build-osfs -G "MinGW Makefiles"
& $cmake --build cmake-build-osfs --target minikv_osfs minikv_osfs_tests
.\cmake-build-osfs\minikv_osfs.exe --disk osfs-course.img --format --blocks 96 --script .\课程设计交付\v1633-osfs-final\osfs-demo-script.txt
```

默认教学账号为 `root/root`、`alice/alice123`、`bob/bob123`。这些密码仅用于课堂演示，报告明确说明密码哈希不是生产级安全实现。

## 已披露边界

OSFS 完成的是 MFD/UFD 两级目录，不支持任意深度子目录；inode 使用八个直接块；没有间接块、fsck、崩溃恢复日志或多进程并发。它不连接 mini-kv 的 KV/WAL/snapshot/TCP/RESP 主线。
