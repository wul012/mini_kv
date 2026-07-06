from __future__ import annotations

import textwrap
from pathlib import Path

from docx import Document
from docx.enum.section import WD_SECTION
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor
from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
DELIVERY = ROOT / "课程设计交付" / "v1630-osfs-final"
IMAGE_DIR = DELIVERY / "images"
REPORT_DOCX = DELIVERY / "操作系统课程设计报告-OSFS-v1630.docx"


def ensure_dirs() -> None:
    DELIVERY.mkdir(parents=True, exist_ok=True)
    IMAGE_DIR.mkdir(parents=True, exist_ok=True)


def font(size: int, bold: bool = False) -> ImageFont.FreeTypeFont:
    candidates = [
        Path("C:/Windows/Fonts/simhei.ttf") if bold else Path("C:/Windows/Fonts/simsun.ttc"),
        Path("C:/Windows/Fonts/msyh.ttc"),
        Path("C:/Windows/Fonts/arial.ttf"),
    ]
    for candidate in candidates:
        if candidate.exists():
            return ImageFont.truetype(str(candidate), size)
    return ImageFont.load_default()


def wrap_text(text: str, width: int) -> list[str]:
    lines: list[str] = []
    for paragraph in text.splitlines():
        if not paragraph:
            lines.append("")
            continue
        lines.extend(textwrap.wrap(paragraph, width=width, break_long_words=False, replace_whitespace=False))
    return lines


def draw_box(draw: ImageDraw.ImageDraw, xy: tuple[int, int, int, int], text: str, fill: str, outline: str = "#1f2937") -> None:
    draw.rounded_rectangle(xy, radius=16, fill=fill, outline=outline, width=2)
    x1, y1, x2, y2 = xy
    title_font = font(28, bold=True)
    body_font = font(21)
    parts = text.split("\n", 1)
    title = parts[0]
    body = parts[1] if len(parts) > 1 else ""
    draw.text((x1 + 24, y1 + 18), title, fill="#111827", font=title_font)
    if body:
        y = y1 + 58
        for line in wrap_text(body, 22):
            draw.text((x1 + 24, y), line, fill="#374151", font=body_font)
            y += 30


def draw_arrow(draw: ImageDraw.ImageDraw, start: tuple[int, int], end: tuple[int, int], color: str = "#374151") -> None:
    draw.line([start, end], fill=color, width=4)
    ex, ey = end
    sx, sy = start
    if ex == sx:
        direction = 1 if ey > sy else -1
        points = [(ex, ey), (ex - 10, ey - 18 * direction), (ex + 10, ey - 18 * direction)]
    else:
        direction = 1 if ex > sx else -1
        points = [(ex, ey), (ex - 18 * direction, ey - 10), (ex - 18 * direction, ey + 10)]
    draw.polygon(points, fill=color)


def create_architecture_diagram() -> Path:
    path = IMAGE_DIR / "01-osfs-system-architecture.png"
    image = Image.new("RGB", (1500, 900), "#f8fafc")
    draw = ImageDraw.Draw(image)
    draw.text((60, 45), "OSFS 课程设计系统结构", fill="#0f172a", font=font(42, bold=True))
    draw.text((60, 100), "新增 OSFS 旁路模块独立服务课设，不改变 mini-kv 原 KV/WAL/TCP 主线。", fill="#475569", font=font(24))

    boxes = [
        ((90, 170, 510, 300), "用户/演示者\n输入 LOGIN、CREATE、OPEN、WRITE、READ、DIR 等命令。", "#dbeafe"),
        ((90, 365, 510, 515), "CommandProcessor\n解析命令，维护当前用户和 fd 句柄表，只调用 OSFS API。", "#e0f2fe"),
        ((90, 590, 510, 750), "FileSystem\n管理超级块、位图、inode、目录项、权限和直接数据块。", "#dcfce7"),
        ((780, 590, 1200, 750), "VirtualDisk\n把块读写落到二进制镜像文件，模拟磁盘设备。", "#fef3c7"),
        ((780, 365, 1200, 515), "磁盘镜像 osfs-course.img\n保存文件系统全部状态，退出程序后仍可再次打开。", "#ffedd5"),
        ((780, 170, 1200, 300), "原 mini-kv 主线\nStore、WAL、Snapshot、TCP/RESP 不被 OSFS 牵动。", "#f3e8ff"),
    ]
    for xy, text, fill in boxes:
        draw_box(draw, xy, text, fill)
    draw_arrow(draw, (300, 305), (300, 360))
    draw_arrow(draw, (300, 520), (300, 585))
    draw_arrow(draw, (515, 670), (775, 670))
    draw_arrow(draw, (990, 585), (990, 520))
    draw.line([(580, 140), (580, 790)], fill="#94a3b8", width=3)
    draw.text((605, 790), "边界：OSFS 是课程设计入口，不授予 KV 写路由、WAL、网络或 managed-audit 执行权。", fill="#334155", font=font(22))
    image.save(path)
    return path


def create_disk_layout_diagram() -> Path:
    path = IMAGE_DIR / "02-osfs-disk-layout.png"
    image = Image.new("RGB", (1500, 820), "#ffffff")
    draw = ImageDraw.Draw(image)
    draw.text((60, 45), "虚拟磁盘布局", fill="#111827", font=font(42, bold=True))
    draw.text((60, 100), "二进制文件被切成固定大小块，OSFS 在这些块上组织元数据和文件内容。", fill="#4b5563", font=font(24))

    x, y, h = 80, 190, 120
    widths = [170, 170, 300, 170, 170, 170, 170]
    labels = [
        ("Block 0", "Superblock\n总块数、inode 数、表位置"),
        ("Block 1", "Block bitmap\n记录数据块占用状态"),
        ("Block 2..N", "Inode table\n保存类型、权限、owner、size、直接块号"),
        ("Data", "Root dir\n目录项 name + inode"),
        ("Data", "file block 1\n文件正文"),
        ("Data", "file block 2\n文件正文"),
        ("...", "free blocks\n等待分配"),
    ]
    colors = ["#bfdbfe", "#bbf7d0", "#fde68a", "#fed7aa", "#e9d5ff", "#e9d5ff", "#e5e7eb"]
    cx = x
    for width, (top, bottom), color in zip(widths, labels, colors):
        draw.rounded_rectangle((cx, y, cx + width, y + h), radius=10, fill=color, outline="#1f2937", width=2)
        draw.text((cx + 16, y + 14), top, fill="#111827", font=font(24, bold=True))
        yy = y + 48
        for line in bottom.split("\n"):
            draw.text((cx + 16, yy), line, fill="#374151", font=font(19))
            yy += 26
        cx += width + 14

    detail = [
        ("Superblock", "让程序知道整块“磁盘”怎么解释，避免把普通二进制文件误当文件系统。"),
        ("Bitmap", "写文件前先找空闲块；空间不足时直接拒绝，旧内容保持不变。"),
        ("Inode", "把文件名和文件内容分开，目录只保存名字到 inode 的索引。"),
        ("Direct blocks", "本课设版本采用直接块，逻辑清楚，便于解释和测试。"),
    ]
    y2 = 390
    for i, (title, body) in enumerate(detail):
        xx = 100 + (i % 2) * 680
        yy = y2 + (i // 2) * 160
        draw_box(draw, (xx, yy, xx + 600, yy + 115), f"{title}\n{body}", "#f8fafc", "#64748b")
    image.save(path)
    return path


def create_write_flow_diagram() -> Path:
    path = IMAGE_DIR / "03-osfs-write-flow.png"
    image = Image.new("RGB", (1500, 900), "#f8fafc")
    draw = ImageDraw.Draw(image)
    draw.text((60, 45), "WRITE 命令处理流程", fill="#0f172a", font=font(42, bold=True))
    draw.text((60, 100), "重点展示权限检查和空间预检查，避免失败写入破坏旧文件。", fill="#475569", font=font(24))

    nodes = [
        ((80, 190, 390, 300), "1. 输入命令\nWRITE fd text"),
        ((520, 190, 830, 300), "2. 查 fd\n确认已 open 且允许写"),
        ((960, 190, 1270, 300), "3. 权限判断\nowner/other write bit"),
        ((960, 420, 1270, 540), "4. 空间预检查\n空闲块 + 可回收旧块 >= 需要块"),
        ((520, 420, 830, 540), "5. 分配并写块\n更新 inode size 和 direct blocks"),
        ((80, 420, 390, 540), "6. 返回结果\nOK wrote N bytes"),
        ((520, 660, 830, 780), "失败路径\n权限不足或空间不足时返回 ERR，旧内容不变"),
    ]
    for xy, text in nodes:
        fill = "#fee2e2" if text.startswith("失败") else "#dbeafe"
        draw_box(draw, xy, text, fill)
    draw_arrow(draw, (390, 245), (515, 245))
    draw_arrow(draw, (830, 245), (955, 245))
    draw_arrow(draw, (1115, 305), (1115, 415))
    draw_arrow(draw, (960, 480), (835, 480))
    draw_arrow(draw, (520, 480), (395, 480))
    draw_arrow(draw, (675, 545), (675, 655), "#b91c1c")
    draw.text((900, 610), "关键边界：失败不是半写入，测试会检查旧内容仍能读出。", fill="#991b1b", font=font(24, bold=True))
    image.save(path)
    return path


def set_cell_shading(cell, color: str) -> None:
    tc_pr = cell._tc.get_or_add_tcPr()
    shd = OxmlElement("w:shd")
    shd.set(qn("w:fill"), color)
    tc_pr.append(shd)


def set_run_font(run, name: str = "SimSun", size: float | None = None, bold: bool | None = None) -> None:
    run.font.name = name
    run._element.rPr.rFonts.set(qn("w:eastAsia"), name)
    if size is not None:
        run.font.size = Pt(size)
    if bold is not None:
        run.bold = bold


def add_para(doc: Document, text: str, style: str | None = None) -> None:
    p = doc.add_paragraph(style=style)
    p.paragraph_format.first_line_indent = Cm(0.74) if style is None else None
    p.paragraph_format.line_spacing = 1.25
    p.paragraph_format.space_after = Pt(6)
    run = p.add_run(text)
    set_run_font(run, "SimSun", 10.5)


def add_heading(doc: Document, text: str, level: int) -> None:
    p = doc.add_heading(level=level)
    run = p.add_run(text)
    set_run_font(run, "SimHei", 14 if level == 1 else 12, True)


def add_caption(doc: Document, text: str) -> None:
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = p.add_run(text)
    set_run_font(run, "SimSun", 9)
    run.font.color.rgb = RGBColor(75, 85, 99)


def add_figure(doc: Document, image_path: Path, caption: str, width_cm: float = 14.0) -> None:
    p = doc.add_paragraph()
    p.alignment = WD_ALIGN_PARAGRAPH.CENTER
    p.add_run().add_picture(str(image_path), width=Cm(width_cm))
    add_caption(doc, caption)


def read_optional_text(path: Path) -> str:
    if not path.exists():
        return ""
    return path.read_text(encoding="utf-8", errors="replace").strip()


def add_key_value_table(doc: Document, rows: list[tuple[str, str]]) -> None:
    table = doc.add_table(rows=1, cols=2)
    table.style = "Table Grid"
    hdr = table.rows[0].cells
    hdr[0].text = "要求或模块"
    hdr[1].text = "本项目中的对应实现"
    for cell in hdr:
        set_cell_shading(cell, "D9EAF7")
        for para in cell.paragraphs:
            for run in para.runs:
                set_run_font(run, "SimHei", 10.5, True)
    for left, right in rows:
        cells = table.add_row().cells
        cells[0].text = left
        cells[1].text = right
        for cell in cells:
            for para in cell.paragraphs:
                for run in para.runs:
                    set_run_font(run, "SimSun", 10)


def build_report() -> None:
    ensure_dirs()
    architecture = create_architecture_diagram()
    disk_layout = create_disk_layout_diagram()
    write_flow = create_write_flow_diagram()

    demo_image = ROOT / "f" / "1630" / "图片" / "03-osfs-course-demo.png"
    ctest_image = ROOT / "f" / "1630" / "图片" / "05-full-ctest-summary.png"
    fallback_demo = ROOT / "f" / "1629" / "图片" / "04-osfs-cli-smoke.png"
    fallback_ctest = ROOT / "f" / "1629" / "图片" / "05-full-ctest-summary.png"
    if not demo_image.exists():
        demo_image = fallback_demo
    if not ctest_image.exists():
        ctest_image = fallback_ctest

    transcript = read_optional_text(DELIVERY / "osfs-demo-transcript.txt")

    doc = Document()
    section = doc.sections[0]
    section.top_margin = Cm(2.4)
    section.bottom_margin = Cm(2.2)
    section.left_margin = Cm(2.6)
    section.right_margin = Cm(2.4)

    styles = doc.styles
    styles["Normal"].font.name = "SimSun"
    styles["Normal"]._element.rPr.rFonts.set(qn("w:eastAsia"), "SimSun")
    styles["Normal"].font.size = Pt(10.5)

    title = doc.add_paragraph()
    title.alignment = WD_ALIGN_PARAGRAPH.CENTER
    title.paragraph_format.space_after = Pt(18)
    run = title.add_run("操作系统课程设计报告")
    set_run_font(run, "SimHei", 24, True)

    subtitle = doc.add_paragraph()
    subtitle.alignment = WD_ALIGN_PARAGRAPH.CENTER
    run = subtitle.add_run("基于虚拟磁盘的简易二级文件系统设计与实现")
    set_run_font(run, "SimHei", 18, True)

    info = [
        ("课程名称", "操作系统课程设计"),
        ("实验题目", "实验二 Linux 二级文件系统"),
        ("项目名称", "mini-kv OSFS course filesystem"),
        ("学生姓名", "【请填写】"),
        ("学号", "【请填写】"),
        ("班级", "【请填写】"),
        ("指导教师", "【请填写】"),
        ("完成日期", "2026 年 7 月"),
    ]
    table = doc.add_table(rows=len(info), cols=2)
    table.style = "Table Grid"
    for i, (k, v) in enumerate(info):
        table.rows[i].cells[0].text = k
        table.rows[i].cells[1].text = v
        set_cell_shading(table.rows[i].cells[0], "F1F5F9")
        for cell in table.rows[i].cells:
            for para in cell.paragraphs:
                for run in para.runs:
                    set_run_font(run, "SimSun", 11)

    doc.add_section(WD_SECTION.NEW_PAGE)
    add_heading(doc, "摘要", 1)
    add_para(
        doc,
        "本课程设计围绕“Linux 二级文件系统”实验展开，目标是在不修改操作系统内核的前提下，"
        "用一个二进制文件模拟磁盘空间，并在其上实现一个简化但结构清楚的多用户文件系统。"
        "系统被命名为 OSFS，作为 mini-kv 项目的独立旁路模块存在。它不接入原来的 KV、WAL、"
        "TCP 服务链路，而是通过 minikv_osfs 命令行程序提供 format、login、dir、create、delete、"
        "open、read、write、close、chmod 等演示入口。这样处理的好处是边界清楚：课程设计要解释"
        "文件系统，原项目核心仍然保持稳定。"
    )
    add_para(
        doc,
        "OSFS 的磁盘布局包含超级块、块位图、inode 表、根目录和数据块。文件名保存在目录项中，"
        "文件类型、权限、owner、长度、时间戳和直接数据块号保存在 inode 中，文件正文则写入数据块。"
        "权限方面，系统实现了 root 用户、普通用户、owner 位和 other 位的基本读写控制。测试部分覆盖"
        "格式化、持久化、目录列出、文件读写、权限拒绝、空间不足失败保持旧内容等行为，能够对应课设"
        "要求中的“二进制文件模拟磁盘”“inode 信息”“超级块信息”“文件信息”“用户登录”和“读写保护”。"
    )
    add_para(doc, "关键词：操作系统；二级文件系统；虚拟磁盘；inode；权限控制；C++")

    add_heading(doc, "1 选题来源与需求分析", 1)
    add_para(
        doc,
        "课程设计要求中，实验二要求模仿 Linux EXT2 文件系统，用二进制文件模拟磁盘空间，保存用户信息、"
        "inode 信息、超级块信息和文件信息，并实现 dir、create、delete、open、close、read、write、login"
        " 等命令。这个题目比单纯写一个命令行工具更接近操作系统课程的核心，因为它迫使程序把“文件”拆成"
        "元数据和数据块两部分来处理。学习这部分之前，文件往往只是一个路径和一段内容；做完这个设计后，"
        "会更容易理解为什么真实文件系统需要位图、inode 表和目录项这些结构。"
    )
    add_para(
        doc,
        "mini-kv 原本已经有 WAL、snapshot、restore 等存储相关能力，但它的主任务是键值存储，不适合直接"
        "伪装成操作系统文件系统。如果为了课设把目录、权限、inode 全塞进 KV 命令里，项目会变得混乱。"
        "因此本设计选择增加一个独立 OSFS 层：它复用仓库已有的 CMake、CTest 和工程纪律，但运行入口、磁盘"
        "镜像和测试都与 KV 主线隔离。这样既能满足课设要求，也不会把原项目变成一个难维护的混合系统。"
    )

    add_key_value_table(
        doc,
        [
            ("二进制文件模拟文件系统", "VirtualDisk 按固定块大小创建和读写 osfs-course.img。"),
            ("超级块信息", "Block 0 保存 magic、block size、block count、inode count、inode 表起止位置和 root inode。"),
            ("block/inode 管理", "Block bitmap 记录数据块占用；inode 表保存每个文件或目录的元数据。"),
            ("文件和目录操作", "CommandProcessor 暴露 DIR、CREATE、DELETE、OPEN、READ、WRITE、CLOSE。"),
            ("用户交互", "minikv_osfs 提供交互 shell；LOGIN 切换 root 或普通用户。"),
            ("读写保护", "CHMOD 修改保护码；READ/WRITE 根据 owner/other 权限判断是否允许。"),
            ("系统测试", "osfs_tests、osfs_cli_smoke 和全量 CTest 共同验证功能与边界。"),
        ],
    )

    add_heading(doc, "2 总体结构设计", 1)
    add_para(
        doc,
        "OSFS 分成三层：最外层是命令处理层，负责把用户输入的字符串转成具体文件操作；中间层是 FileSystem，"
        "负责维护超级块、位图、inode、目录项和权限；最底层是 VirtualDisk，它只关心按块读写二进制文件。"
        "这种分层让报告和代码都比较容易解释。比如 WRITE 命令失败时，不需要在命令层猜测磁盘状态，而是由"
        "FileSystem 统一判断权限和空间；VirtualDisk 也不需要理解文件名，只处理块号和字节数组。"
    )
    add_figure(doc, architecture, "图 1 OSFS 课程设计系统结构", 14.5)

    add_heading(doc, "3 虚拟磁盘与数据结构设计", 1)
    add_para(
        doc,
        "虚拟磁盘采用固定块组织。当前默认块大小为 512 字节，演示时通常创建 96 或 256 个块。Block 0 是"
        "超级块，Block 1 是块位图，后面一段区域保存 inode 表，剩余区域作为数据块。根目录本身也是一个"
        "目录 inode，它的数据块里保存目录项。目录项只保存文件名、inode 编号和是否有效；真正的文件长度、"
        "权限、owner、时间戳和数据块列表都在 inode 中。这样的设计比直接把“文件名和内容”连在一起更接近"
        "课程中讲到的文件系统思路。"
    )
    add_para(
        doc,
        "为了让实现可控，本版只使用直接数据块。真实 EXT2 inode 会包含 12 个直接块以及多级间接块，"
        "但课设演示重点是理解文件系统内部结构，而不是追求大文件容量。直接块方案更适合课堂答辩：它能"
        "清楚展示位图如何分配块、inode 如何记录块号、目录如何通过 inode 找到文件，同时避免间接块把"
        "讲解重点拉得过散。"
    )
    add_figure(doc, disk_layout, "图 2 OSFS 虚拟磁盘布局", 14.5)

    add_heading(doc, "4 核心操作流程", 1)
    add_para(
        doc,
        "格式化时，程序先创建指定大小的二进制文件，再写入超级块、初始化位图、清空 inode 表，并创建 root"
        " 目录。创建文件时，系统分配一个空闲 inode，在根目录中添加目录项。打开文件时，命令层记录一个 fd"
        " 到 inode 的映射；后续 READ 和 WRITE 通过 fd 找回 inode。删除文件时，系统释放 inode 和它占用的"
        "数据块，并把目录项标记为无效。"
    )
    add_para(
        doc,
        "WRITE 是最值得说明的流程。它不只是把字符串写到文件末尾，而是先检查 fd 是否存在、打开模式是否"
        "允许写、当前用户是否有写权限，然后计算新内容需要多少数据块。空间不足时，程序会在释放旧块之前"
        "直接返回错误，这一点在测试中专门覆盖。这个细节看起来不大，但它说明实现时考虑了失败场景，不会"
        "出现写一半失败导致旧文件被破坏的问题。"
    )
    add_figure(doc, write_flow, "图 3 WRITE 命令处理流程", 14.5)

    add_heading(doc, "5 权限与用户模型", 1)
    add_para(
        doc,
        "用户模型故意保持简单。root 用户 uid 为 0，普通用户名通过稳定哈希得到 uid。每个文件保存 owner"
        " 和八进制保护码，例如 0644 或 0600。读写判断时，root 直接通过；如果当前用户是 owner，则检查"
        " owner read/write 位；如果不是 owner，则检查 other read/write 位。本设计没有引入 group，是因为"
        "课程要求强调读写保护，owner/other 已经足以演示保护码的作用。"
    )
    add_para(
        doc,
        "演示中可以先用 root 创建并写入 report 文件，再把权限改成 0600，随后 LOGIN guest 并尝试 READ。"
        "guest 不是 owner，other read 位又被关闭，因此系统返回 permission denied。这条演示路径比单纯"
        "展示 chmod 更有说服力，因为它把用户切换、保护码和读操作连接在一起，能说明权限不是文档里的字段，"
        "而是实际参与了命令执行。"
    )

    add_heading(doc, "6 关键模块说明", 1)
    add_key_value_table(
        doc,
        [
            ("include/minikv/osfs/virtual_disk.hpp", "声明虚拟磁盘接口，提供 create、open、read_block、write_block。"),
            ("src/osfs_virtual_disk.cpp", "负责磁盘镜像大小校验、块边界校验和二进制读写。"),
            ("include/minikv/osfs/filesystem.hpp", "声明文件系统 API、FileInfo、DirectoryEntry、Stat、OpenMode 等类型。"),
            ("src/osfs_filesystem.cpp", "实现超级块、位图、inode 表、目录项、文件读写、权限和空间分配。"),
            ("include/minikv/osfs/command_processor.hpp", "声明课程演示命令处理器和命令返回结构。"),
            ("src/osfs_command_processor.cpp", "解析 LOGIN、DIR、CREATE、OPEN、WRITE、READ、CHMOD 等命令并维护 fd 表。"),
            ("src/osfs_main.cpp", "提供 minikv_osfs 命令行入口，支持 --disk、--format、--blocks、--script。"),
            ("tests/osfs_tests.cpp", "用断言覆盖持久化、权限、目录行、空间失败等关键行为。"),
        ],
    )

    add_heading(doc, "7 测试与演示结果", 1)
    add_para(
        doc,
        "本设计不只依赖一次手工演示。自动化测试先验证底层行为，例如格式化后重新打开磁盘仍能读到文件，"
        "chmod 后普通用户会被拒绝，空间不足时旧内容不被破坏。脚本 smoke 再从 CLI 入口跑一遍接近课堂演示的"
        "命令序列。最后，全量 CTest 确认新增 OSFS 没有破坏 mini-kv 原有 KV、WAL、snapshot、TCP 和证据链测试。"
    )
    if demo_image.exists():
        add_figure(doc, demo_image, "图 4 OSFS 命令行演示结果", 14.5)
    if transcript:
        add_para(
            doc,
            "演示输出中可以看到 CREATE、OPEN、WRITE、READ、CHMOD、LOGIN、权限拒绝和 DIR 列表都按预期执行。"
            "其中 report 文件在 chmod 0600 后，guest 用户读取失败，说明保护码不是静态展示字段。"
        )
    if ctest_image.exists():
        add_figure(doc, ctest_image, "图 5 全量 CTest 验证证据", 14.5)

    add_heading(doc, "8 运行与复现步骤", 1)
    add_para(
        doc,
        "在 Linux 环境中，可以使用 CMake 构建 minikv_osfs 目标，然后用脚本运行演示命令。Windows 本地验证"
        "使用 CLion 捆绑 MinGW 工具链完成，GitHub Actions 也覆盖 Ubuntu、macOS 和 Windows 构建测试。课堂"
        "演示时建议只展示 OSFS 入口，不需要启动 mini-kv TCP 服务。"
    )
    code = doc.add_paragraph()
    code.paragraph_format.left_indent = Cm(0.8)
    run = code.add_run(
        "cmake -S . -B build\n"
        "cmake --build build --target minikv_osfs\n"
        "./build/minikv_osfs --disk data/osfs-course.img --format --blocks 96 --script tests/osfs_smoke_script.txt"
    )
    set_run_font(run, "Consolas", 9)

    add_heading(doc, "9 不足与改进方向", 1)
    add_para(
        doc,
        "当前 OSFS 适合作为课程设计主体，但它不是完整生产文件系统。它只实现根目录，没有多级目录；inode 使用"
        "直接数据块，没有间接块；权限模型也只区分 owner 和 other，没有 group。笔者认为这些限制是可以接受的，"
        "因为本次课程设计的关键是讲清楚文件系统内部结构和命令行为，而不是复刻完整 EXT2。"
    )
    add_para(
        doc,
        "如果继续扩展，比较合理的顺序是先加 mkdir、cd、pwd，把根目录扩成多级目录；随后再加 group 权限和"
        "间接块。另一个方向是增加 fsck 类检查命令，用来发现位图、inode 表和目录项之间的不一致。这样的扩展"
        "会更接近真实文件系统维护，但也会显著增加测试和答辩解释成本。"
    )

    add_heading(doc, "10 总结", 1)
    add_para(
        doc,
        "通过这次设计，我对文件系统的理解从“文件路径加文件内容”推进到了“磁盘块、元数据和权限共同工作”。"
        "OSFS 的实现不复杂，但它把课程中的几个关键点连在了一起：超级块说明磁盘如何被解释，位图说明空间如何"
        "被分配，inode 说明文件属性和数据块如何绑定，目录项说明文件名如何映射到 inode，权限检查则说明多用户"
        "环境下为什么不能只关心数据是否存在。"
    )
    add_para(
        doc,
        "这版设计也保留了工程上的克制。它没有把 OSFS 混进 mini-kv 原来的 KV 命令，也没有启动网络服务或打开"
        "额外执行链路。对课程设计来说，这使系统边界更好讲；对原项目来说，也避免了为了完成作业而破坏已有结构。"
    )

    add_heading(doc, "参考资料", 1)
    refs = [
        "操作系统课程设计要求 2026，课程讲义 PDF。",
        "Linux Kernel 源码目录与文件系统子系统课程资料。",
        "mini-kv 项目源码：include/minikv/osfs、src/osfs_*.cpp、tests/osfs_tests.cpp。",
    ]
    for item in refs:
        p = doc.add_paragraph(style=None)
        p.paragraph_format.left_indent = Cm(0.6)
        run = p.add_run(item)
        set_run_font(run, "SimSun", 10.5)

    doc.save(REPORT_DOCX)


if __name__ == "__main__":
    build_report()
    print(REPORT_DOCX)
