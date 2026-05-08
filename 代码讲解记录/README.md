# mini-kv 代码讲解记录

本目录用于永久记录对 `mini-kv` 项目的分次代码讲解。

讲解风格参考项目根目录下的 `解释代码格式说明`：

```text
先说明文件或类的角色
再给核心流程
然后逐段解释代码
最后做一句话总结
```

## 讲解目录

```text
01-store-and-command.md
 -> Store 与 CommandProcessor：内存数据库和命令解析核心

02-cli-main.md
 -> main.cpp：本地 CLI 命令行入口

03-server-main.md
 -> server_main.cpp：TCP 服务端启动入口

04-tcp-server.md
 -> tcp_server.hpp / tcp_server.cpp：TCP 网络通信层

05-tests-build-docs.md
 -> tests、CMakeLists.txt、README.md、AGENTS.md 等辅助文件

06-client-main.md
 -> client_main.cpp：第二版新增 TCP 客户端入口

07-version-2-build-docs.md
 -> 第二版 CMakeLists.txt、README.md 等构建和说明文件改动

08-ttl-store-command.md
 -> 第三版 TTL：Store 过期时间和 CommandProcessor 新命令

09-version-3-tests-docs.md
 -> 第三版测试、README、a/3 归档和整体增删改

10-wal-core.md
 -> 第四版 WAL：WriteAheadLog、CommandProcessor 写前日志和 EXPIREAT

11-version-4-tests-docs.md
 -> 第四版入口参数、wal_tests、README、a/4 归档和整体增删改
```

## 项目整体理解

`mini-kv` 是一个 C++20 / CMake 编写的迷你 Redis 风格内存键值服务。

它的核心链路是：

```text
用户输入命令
 -> CommandProcessor 解析命令
 -> Store 读写内存数据
 -> CLI 或 TCP Server 把结果返回给用户
```

第三版增加了 TTL 链路：

```text
EXPIRE key seconds
 -> CommandProcessor 解析秒数
 -> Store 记录 expires_at

TTL key
 -> Store 检查 key 是否存在、是否过期、是否有 expires_at
 -> CommandProcessor 返回 -2 / -1 / 剩余秒数
```

第四版增加了 WAL 链路：

```text
程序启动并传入 wal_path
 -> WriteAheadLog replay 到 Store
 -> CommandProcessor 持有 WAL 指针

SET / DEL / EXPIRE
 -> 先 append WAL
 -> 再修改 Store

程序下次启动
 -> replay SET / DEL / EXPIREAT
 -> 恢复未过期数据
```

从运行方式看，它现在有三种入口：

```text
CLI 模式
 -> main.cpp
 -> 读取键盘输入
 -> 调用 CommandProcessor
 -> 打印结果

TCP Server 模式
 -> server_main.cpp
 -> TcpServer 监听端口
 -> 客户端通过 TCP 发送命令
 -> 调用 CommandProcessor
 -> 通过 socket 返回结果

TCP Client 模式
 -> client_main.cpp
 -> 连接 minikv_server
 -> 读取用户输入
 -> 通过 TCP 发送命令
 -> 打印服务端响应
```

从代码职责看：

```text
include/minikv/store.hpp
src/store.cpp
 -> 保存 key-value 数据，并在第三版支持 TTL 过期时间

include/minikv/command.hpp
src/command.cpp
 -> 解析 SET / GET / DEL / EXPIRE / TTL / SIZE / HELP / EXIT 命令，并在第四版支持可选 WAL

src/main.cpp
 -> 本地命令行交互入口，第四版支持可选 WAL 路径

include/minikv/tcp_server.hpp
src/tcp_server.cpp
 -> 网络服务端和 socket 通信

src/server_main.cpp
 -> TCP 服务端启动器，第四版支持可选 WAL 路径

src/client_main.cpp
 -> TCP 客户端启动器

include/minikv/wal.hpp
src/wal.cpp
 -> 第四版 WAL 持久化模块，负责 append 和 replay

tests/
 -> 验证 Store、CommandProcessor 和 WAL 行为

CMakeLists.txt
 -> 构建核心库、CLI、服务端、客户端和测试目标
```
