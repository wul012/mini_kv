# OSFS v1636 Linux 复刻命令（FinalShell 版）

本文记录本次在远程 Linux 上验证 OSFS 课设包时使用的核心命令。  
用途：你可以在 FinalShell 登录 `azureuser@myvps` 后，按块复制粘贴，复现构建、测试、演示、持久化复开和报告材料检查。

说明：

- 下面命令默认在远程 Ubuntu 服务器执行。
- 项目目录默认是 `/home/azureuser/mini-kv`。
- 交付目录不手写中文路径，而是用 `find . -type d -name v1636-osfs-final` 自动定位，避免编码问题。
- `/tmp/osfs-v1636-linux-demo.img` 是临时演示磁盘镜像，可以重复删除重建。
- 本文只列 FinalShell/Linux 可复刻命令；我本地把日志渲染成 PNG 截图的 Edge/HTML 命令不适合在 FinalShell 里执行。

## 0. 首次准备环境（如果已经装过可以跳过）

```bash
sudo apt-get update
sudo DEBIAN_FRONTEND=noninteractive apt-get install -y \
  build-essential \
  cmake \
  ninja-build \
  python3-pip \
  python3-venv \
  python3-docx \
  python3-pil \
  libreoffice-writer \
  poppler-utils \
  fonts-noto-cjk \
  fontconfig \
  clang-format
```

预期结果：

- `apt-get` 正常结束，没有 `E: Unable to locate package` 或安装失败。
- 这一步提供 C++ 编译、CMake/CTest、Ninja、报告/PDF 检查、中文字体和 Python 报告脚本依赖。

## 1. 获取或确认项目目录

如果服务器上还没有项目：

```bash
cd /home/azureuser
git clone https://github.com/wul012/mini_kv mini-kv
cd /home/azureuser/mini-kv
```

如果已经有项目：

```bash
cd /home/azureuser/mini-kv
git status --short
git branch --show-current
git rev-parse --short HEAD
git log -1 --oneline
```

预期结果：

- 当前目录是 `/home/azureuser/mini-kv`。
- 本次证据对应提交为 `5559b14f`。
- `git status --short` 如果只看到 `build-osfs-linux/`，说明只是本地构建目录；源码本身没有被修改。

## 2. 检查 Linux 工具链

```bash
cd /home/azureuser/mini-kv

echo '[environment]'
uname -a
sed -n '1,8p' /etc/os-release
echo PWD=$PWD
echo HOME=$HOME

cmake --version | head -1
ctest --version | head -1
g++ --version | head -1
ninja --version
python3 --version
pip3 --version
clang-format --version
soffice --headless --version
pdftoppm -v 2>&1 | head -1
fc-match 'Noto Sans CJK SC' | head -1
codex --version
```

预期结果：

- 系统显示 Ubuntu 24.04。
- 能看到 `cmake version 3.28.3`、`g++ 13.3.0`、`ninja 1.11.1`。
- `python3`、`soffice`、`pdftoppm`、`Noto Sans CJK SC` 都能正常输出。
- `codex --version` 能显示 `codex-cli 0.142.5` 左右的版本。

这一步证明 Linux 运行、构建、报告/PDF 检查的基础环境是齐的。

## 3. 定位 v1636 交付包文件

```bash
cd /home/azureuser/mini-kv

echo '[v1636 package files]'
delivery=$(find . -type d -name v1636-osfs-final | head -1)
test -n "$delivery"
echo delivery=$delivery
find "$delivery" -maxdepth 2 -type f | sort | sed -n '1,120p'
```

预期结果：

- 能输出类似：

```text
delivery=./课程设计交付/v1636-osfs-final
```

或者因为仓库历史编码显示为类似 `./璇剧▼.../v1636-osfs-final`，但只要末尾是 `v1636-osfs-final` 就可以。

- 文件列表里应包含：
  - `osfs-demo-script.txt`
  - `osfs-demo-transcript.txt`
  - `操作系统课程设计报告-OSFS-v1636.docx`
  - `操作系统课程设计报告-OSFS-v1636.pdf`
  - `images/*.png`
  - `需求-证据矩阵.md`
  - `演示步骤.md`

这一步证明最终课设材料包在 Linux 仓库中可定位。

## 4. CMake 配置并编译 OSFS 目标

```bash
cd /home/azureuser/mini-kv

cmake -S . -B build-osfs-linux -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-osfs-linux --target minikv_osfs minikv_osfs_tests --parallel 1
ls -lh build-osfs-linux/minikv_osfs build-osfs-linux/minikv_osfs_tests
```

预期结果：

- CMake 输出 `Configuring done`、`Generating done`。
- 编译最终看到类似：

```text
[318/318] Linking CXX executable minikv_osfs_tests
```

- `ls -lh` 能看到两个可执行文件：
  - `build-osfs-linux/minikv_osfs`
  - `build-osfs-linux/minikv_osfs_tests`

解释：

- `minikv_osfs` 是课设演示入口。
- `minikv_osfs_tests` 是 OSFS 结构、登录、MFD/UFD、权限、fd 位移、FSCK、用户管理等测试入口。
- 服务器内存较小，所以使用 `--parallel 1`，慢一点但更稳。

## 5. 查看 OSFS 相关 CTest 是否注册

```bash
cd /home/azureuser/mini-kv

ctest --test-dir build-osfs-linux -N -R '^(osfs_tests|osfs_cli_smoke)$'
```

预期结果：

```text
Test #8: osfs_tests
Test #9: osfs_cli_smoke
Total Tests: 2
```

解释：

- `osfs_tests` 是 C++ 层的核心测试。
- `osfs_cli_smoke` 是真实 CLI 脚本 smoke，能证明命令行演示入口可用。

## 6. 运行 OSFS 单元/集成测试

```bash
cd /home/azureuser/mini-kv

ctest --test-dir build-osfs-linux -R '^osfs_tests$' --output-on-failure --timeout 120
```

预期结果：

```text
100% tests passed, 0 tests failed out of 1
```

解释：

这一步证明 OSFS 的核心行为通过测试保护，包括用户表、MFD/UFD、权限、fd offset、间接块、FSCK、USERADD/PASSWD 和持久化等。

## 7. 运行 OSFS CLI smoke

```bash
cd /home/azureuser/mini-kv

ctest --test-dir build-osfs-linux -R '^osfs_cli_smoke$' --output-on-failure --timeout 120
```

预期结果：

```text
100% tests passed, 0 tests failed out of 1
```

解释：

这一步不是只测 C++ 函数，而是让 `minikv_osfs` 真实执行脚本，证明 CLI 入口和课程演示路径可运行。

## 8. 运行 v1636 正式 demo 脚本

```bash
cd /home/azureuser/mini-kv

delivery=$(find . -type d -name v1636-osfs-final | head -1)
test -n "$delivery"
echo delivery=$delivery

rm -f /tmp/osfs-v1636-linux-demo.img
./build-osfs-linux/minikv_osfs \
  --disk /tmp/osfs-v1636-linux-demo.img \
  --format \
  --blocks 160 \
  --script "$delivery/osfs-demo-script.txt"

ls -lh /tmp/osfs-v1636-linux-demo.img
```

预期结果中应能看到这些关键片段：

```text
ERR login required
ERR invalid credentials
LOGIN OK user=alice
OK created report
OK wrote ... offset=...
fd=... read_offset=... write_offset=...
FSCK OK checks=6
ERR permission denied
ERR USERADD requires root
LOGIN OK user=root
OK user added carol
OK password changed
LOGIN OK user=carol
```

解释：

- `ERR login required`：未登录不能绕过认证操作文件。
- `ERR invalid credentials`：错误密码会被拒绝。
- `LOGIN OK user=alice`：磁盘用户表认证成功。
- `report` 和 `TELL/READ/WRITE`：证明 fd 位移不是整文件假读写。
- `bigfile` 和 `DIRECT8|INDIRECT-BEGIN`：证明越过 8 个直接块后进入一级间接块。
- `FSCK OK checks=6`：只读一致性检查通过。
- Bob 权限失败：证明跨用户 UFD 和权限隔离。
- root 新增 Carol 并改密码：证明用户表不是写死的三个账号。

## 9. 复开同一个镜像验证持久化

注意：这一步依赖第 8 步生成的 `/tmp/osfs-v1636-linux-demo.img`，不要提前删除它。

```bash
cd /home/azureuser/mini-kv

tmp=/tmp/osfs-v1636-persistence-check.txt
cat > "$tmp" <<'EOF'
WHOAMI
LOGIN carol carol456
DIR
LOGIN alice alice123
DIR
OPEN bigfile r
SEEK 0 4088
READ 0 96
FSCK
QUIT
EOF

./build-osfs-linux/minikv_osfs \
  --disk /tmp/osfs-v1636-linux-demo.img \
  --script "$tmp"

rm -f "$tmp"
```

预期结果：

- `WHOAMI` 一开始显示未登录。
- `LOGIN carol carol456` 成功，说明第 8 步 root 创建和改密后的 Carol 落盘了。
- Alice 重新登录后还能看到之前创建的文件。
- `OPEN bigfile r`、`SEEK 0 4088`、`READ 0 96` 能读到跨直接块/间接块边界内容。
- `FSCK OK checks=6` 仍然通过。

解释：

这一步证明用户表、MFD/UFD、inode、数据块、一级间接块不是一次性内存状态，而是保存在同一个 `.img` 镜像里。

## 10. 检查报告/PDF 材料和文档工具链

```bash
cd /home/azureuser/mini-kv

delivery=$(find . -type d -name v1636-osfs-final | head -1)
test -n "$delivery"
echo delivery=$delivery

echo '[python modules]'
python3 -c 'import docx, PIL.Image; print("python-docx OK"); print("Pillow OK")'

echo '[report files]'
find "$delivery" -maxdepth 2 -type f \( -name '*.docx' -o -name '*.pdf' -o -name '*.png' \) -exec ls -lh {} +

echo '[pdf info]'
pdf=$(find "$delivery" -maxdepth 1 -name '*.pdf' | head -1)
pdfinfo "$pdf" | sed -n '1,25p'

echo '[libreoffice]'
soffice --headless --version
```

预期结果：

- Python 输出：

```text
python-docx OK
Pillow OK
```

- 能列出 v1636 的 DOCX、PDF 和 `images/*.png`。
- `pdfinfo` 能读取 PDF 页数、页面尺寸等信息。
- `soffice --headless --version` 能输出 LibreOffice 版本。

解释：

这一步证明 Linux 上不只可以跑程序，也具备检查报告/PDF和图像材料的基础工具链。

## 11. 最后清理临时文件并确认没有后台进程

```bash
rm -f /tmp/osfs-v1636-linux-demo.img /tmp/osfs-v1636-persistence-check.txt

echo '[background process check]'
pgrep -a -u azureuser 'cmake|ninja|c\+\+|g\+\+|soffice|minikv' || true

echo '[disk]'
df -h /home/azureuser | tail -1

echo '[memory]'
free -h | sed -n '1,2p'
```

预期结果：

- `pgrep` 没有输出，或没有仍在运行的 `cmake/ninja/soffice/minikv` 进程。
- `df -h` 显示磁盘空间仍充足。
- `free -h` 显示当前内存状态。

解释：

这一步是收尾检查：临时镜像删除，后台进程不残留，避免后续演示互相影响。

## 一键版：只跑核心验收

如果你不想分块复制，可以用下面这段一次性跑核心验收。它会构建、跑两个 OSFS CTest、跑正式 demo、再做复开持久化检查。

```bash
set -eo pipefail
cd /home/azureuser/mini-kv

delivery=$(find . -type d -name v1636-osfs-final | head -1)
test -n "$delivery"
echo "delivery=$delivery"

cmake -S . -B build-osfs-linux -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build-osfs-linux --target minikv_osfs minikv_osfs_tests --parallel 1

ctest --test-dir build-osfs-linux -R '^osfs_tests$' --output-on-failure --timeout 120
ctest --test-dir build-osfs-linux -R '^osfs_cli_smoke$' --output-on-failure --timeout 120

rm -f /tmp/osfs-v1636-linux-demo.img
./build-osfs-linux/minikv_osfs \
  --disk /tmp/osfs-v1636-linux-demo.img \
  --format \
  --blocks 160 \
  --script "$delivery/osfs-demo-script.txt"

tmp=/tmp/osfs-v1636-persistence-check.txt
cat > "$tmp" <<'EOF'
WHOAMI
LOGIN carol carol456
DIR
LOGIN alice alice123
DIR
OPEN bigfile r
SEEK 0 4088
READ 0 96
FSCK
QUIT
EOF

./build-osfs-linux/minikv_osfs --disk /tmp/osfs-v1636-linux-demo.img --script "$tmp"
rm -f "$tmp" /tmp/osfs-v1636-linux-demo.img

echo '[DONE] OSFS v1636 Linux reproduction finished.'
```

一键版最终预期：

```text
100% tests passed
FSCK OK checks=6
[DONE] OSFS v1636 Linux reproduction finished.
```

如果出现失败：

- `delivery` 为空：说明当前仓库里没有 `v1636-osfs-final` 目录，先确认是否在 `/home/azureuser/mini-kv`。
- 找不到 `cmake/g++/ninja`：先执行第 0 步安装依赖。
- 找不到 `minikv_osfs`：先执行第 4 步构建。
- `LOGIN carol carol456` 失败：说明第 8 步的格式化 demo 没跑成功，重新从第 8 步开始。
