# 第四批：权限、用户与 FSCK Q&A

## Q37：老师问：权限是怎么表示的？

答：权限保存在 inode 的 `mode` 字段里，采用类似 Unix 的 owner/group/other 读写位，比如 `0600` 表示只有 owner 可读写。命令层的 `CHMOD` 会修改 mode；`DIR` 和 `STAT` 会显示 mode、owner、size 等信息；读写前会用当前 uid/gid 和 inode owner/mode 判断是否允许。

## Q38：老师问：Bob 为什么打不开 Alice 的文件？

答：Alice 把 `report` 改成 `0600` 后，只有 owner Alice 有读写权限。Bob 即使通过 `OPEN alice/report r` 指定跨用户路径，也会先定位到 Alice 的文件 inode，再做权限检查。因为 Bob 不是 owner，也没有 group/other read 位，所以返回 permission denied。

## Q39：老师问：root 有什么特殊权限？

答：root 的 uid 是 0，可以审查其他用户 UFD，也可以创建用户和重置任意用户密码。普通文件删除也允许 owner 或 root 执行。这个设计是教学版权限模型，不等于完整 Linux capability，但足够表达 root 与普通用户的权限差异。

## Q40：老师问：USERADD 做了什么？

答：`USERADD user password` 只能由 root 执行。它会检查用户名和密码，确认 user table 还有空位；然后分配一个新的 UFD inode 和目录数据块，把用户记录写入 user table，把用户名到 UFD inode 的目录项写入 MFD，再更新 user_count、bitmap 和 superblock。Carol 能重开后登录，说明它不是临时内存用户。

## Q41：老师问：PASSWD 怎么控制权限？

答：`PASSWD [user] password` 支持两种情况：root 可以改任意用户密码；普通用户只能改自己的密码。如果普通用户尝试改别人密码，会返回 permission denied。密码改完后写回 user table，重开镜像后仍然用新密码认证。

## Q42：老师问：密码哈希安全吗？

答：这里要如实回答：它是教学用途的哈希，不是生产级密码方案。它的作用是避免明文密码直接参与比较，并让用户表认证流程完整。真实系统应使用带盐、慢哈希、权限隔离和更严格的密钥管理，但这超出了本课设的核心目标。

## Q43：老师问：FSCK 检查哪些东西？

答：它检查六类关系：superblock 几何是否合理；inode bitmap 是否和 used inode 一致；block bitmap 是否和元数据块、目录块、文件数据块、间接块一致；文件 size 和引用的数据块数是否匹配；user table、UFD 指针和 MFD 目录项是否互相对应；MFD/UFD 目录项是否指向有效 inode。

## Q44：老师问：FSCK 会不会修改磁盘？

答：不会，它是只读检查。它读取磁盘上已有结构，在内存中重新构造“期望状态”，然后输出 OK 或 ERROR 明细。它不自动修复 bitmap，也不重写 inode。这样设计更适合课设答辩：先把检测逻辑做透明，避免自动修复把错误掩盖掉。

## Q45：老师问：如果 block bitmap 错了，FSCK 怎么发现？

答：FSCK 会扫描所有有效 inode，收集目录块、文件 direct blocks、indirect block 本身和 indirect block 里的数据块，得到一份 expected block bitmap。然后逐块比较 expected bitmap 和磁盘 block bitmap。独立验证里手动翻转过某个文件数据块对应的 bitmap 位，FSCK 报出了 block bitmap mismatch。

## Q46：老师问：如果 user table 和 MFD 不一致，FSCK 能发现吗？

答：能。FSCK 会先读取 MFD 目录项，建立 username 到 UFD inode 的映射；再遍历 user table，检查每个用户的 `ufd_inode` 是否是有效目录 inode，owner uid 是否和用户 uid 匹配，MFD 中是否有同名目录项且 inode 一致。如果 Carol 在 user table 里有记录但 MFD 没有对应项，就会报错。

## Q47：老师问：如果文件 size 和块数不匹配呢？

答：FSCK 会根据 inode.size 和 block size 计算理论上需要多少数据块，再和 inode direct/indirect 实际引用的数据块数量比较。比如 size 需要 9 个数据块，但 inode 只引用 8 个，或者 size 还没超过直接块容量却保留了 indirect block，都会被认为是异常。

## Q48：老师问：权限检查放在读写前还是读写后？

答：放在读写前。`write_file_range`、`read_file_range` 都会先定位文件 inode，然后调用权限判断。只有权限通过才会分配块、读块或写块。这样不会出现“已经写了一半才返回 permission denied”的问题。

