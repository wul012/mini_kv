# 磁盘布局、位图与 inode

OSFS 的“磁盘”是一个普通二进制文件。`VirtualDisk` 把它切成固定大小的块，默认每块 512 字节。上层代码不直接到处计算文件偏移，而是调用 `read_block(block_index)` 和 `write_block(block_index, bytes)`。物理偏移只有一个公式：

```text
字节偏移 = 物理块号 * block_size
```

## 用默认交付参数算一次

v1636 演示通常使用 160 个块、64 个 inode、512 字节块。`InodeDisk` 固定 96 字节，所以 inode 表需要：

```text
64 * 96 = 6144 字节
6144 / 512 = 12 个块
```

于是磁盘前部布局为：

| 区域 | 块号 | 内容 |
|---|---:|---|
| SuperBlock | 0 | 魔数、版本、几何、区域起点、空闲计数、MFD inode |
| block bitmap | 1 | 每一位表示一个物理块是否占用 |
| inode bitmap | 2 | 每一位表示一个 inode 是否占用 |
| user table | 3 | 最多八条 64 字节用户记录 |
| inode table | 4-15 | 64 个 96 字节 inode |
| data area | 16-159 | MFD、UFD、普通文件数据和间接块 |

格式化时先为 MFD 和 root、Alice、Bob 三个 UFD 各分配一个数据块。因此第一个普通文件的数据块通常从后面继续分配。真实 transcript 里的 `report 4 20 0600 1000 13` 就表示 inode 4 的第一个物理数据块是 20。这个数字不是写死的；前面分配顺序或目录增长改变后，它也会改变。

```mermaid
flowchart LR
    B0["Block 0\nSuperBlock"] --> B1["Block 1\nblock bitmap"]
    B1 --> B2["Block 2\ninode bitmap"]
    B2 --> B3["Block 3\nuser table"]
    B3 --> IT["Block 4..15\ninode table"]
    IT --> DA["Block 16..159\ndata area"]
```

## 四个落盘结构

它们定义在 `src/osfs_disk_layout.hpp`，使用紧凑布局并由 `static_assert` 固定尺寸：

```cpp
static_assert(sizeof(SuperBlockDisk) == 512);
static_assert(sizeof(InodeDisk) == 96);
static_assert(sizeof(DirectoryEntryDisk) == 64);
static_assert(sizeof(UserRecordDisk) == 64);
```

这四个断言很有价值。磁盘格式依赖字段的字节位置，如果编译器因为对齐策略把 `InodeDisk` 从 96 字节变成 104 字节，旧镜像就会被错误解析。编译期断言会在产生坏镜像前直接让构建失败。

### SuperBlockDisk

超级块是整张磁盘的目录。它记录 `MKVOSFS` 魔数、磁盘格式版本 3、块大小、块数、inode 数、双位图位置、用户表与 inode 表范围、数据区起点、MFD inode、空闲块数、空闲 inode 数和用户数。

打开已有镜像时，程序先按 512 字节读取块 0，拿到真实 `block_size`，再重新打开磁盘。`read_super_block` 还会检查魔数、版本和镜像几何。不匹配时返回错误，而不是猜测格式。

### InodeDisk

inode 回答“这个文件是什么”，不保存文件名。核心字段是：

```text
used / type
mode
owner_uid / owner_gid
size
created_at / accessed_at / modified_at
direct[0..7]
indirect_block
```

文件名在 UFD 的 `DirectoryEntryDisk` 中，目录项只保存 `name + inode`。这样，同一个 inode 的元数据和数据块定位不会散落在目录字符串里。

### DirectoryEntryDisk

每条目录项 64 字节，其中名字最多 54 个有效字符，另外保存 `used` 和 inode 号。512 字节块正好放八条目录项。因此 Alice 创建前八个空文件时，原 UFD 块仍够用；创建第九个文件时才会再分配一个目录块。新增的 resilience 测试正是按这个边界验证，而不是随便创建两个文件就声称支持目录增长。

### UserRecordDisk

每条用户记录也是 64 字节，保存 uid、gid、UFD inode、教学用密码摘要和用户名。用户表固定占一个 512 字节块，所以当前最多八个用户。测试会把用户加满并要求下一次 `USERADD` 返回 `user table is full`。

## 两个位图为什么都要有

inode bitmap 管“文件身份槽位”，block bitmap 管“物理空间”。空文件创建后会占一个 inode，但不一定占普通文件数据块；写入第一个字节时才需要数据块。删除文件则要释放 inode 和所有直接、间接数据块。

位图中的第 `index` 位位于：

```text
字节下标 = index / 8
位下标   = index % 8
掩码     = 1 << 位下标
```

分配函数从合法范围向后找第一个 0 位并置 1；释放时清 0。超级块还保存空闲计数，让 `DIR` 之外的管理逻辑可以快速读取容量。不过这个计数可能与位图漂移，所以 FSCK 会重新计算并比较，不能只信超级块自报数字。

## 格式化的先后顺序

`FileSystem::format` 先验证几何，再创建指定大小的镜像。它把元数据区标记为占用，然后依次分配 MFD、三个预置用户的 UFD inode 和目录块，写用户表与 MFD 映射，最后回写超级块、位图、inode 表和目录数据。

格式化后的关系不是“root 目录下放所有普通文件”，而是：

```text
MFD inode -> root UFD inode
          -> alice UFD inode
          -> bob UFD inode

Alice UFD inode -> report file inode -> data blocks
```

下一篇 [03-用户表与二级目录.md](03-用户表与二级目录.md) 专门解释这组关系如何参与登录、DIR 和跨用户访问。
