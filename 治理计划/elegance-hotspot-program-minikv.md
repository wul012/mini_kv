# mini-kv 优雅热点计划（E-track，2–4 版，有界）

执行者：`D:\C\mini-kv` 的 Codex 会话。2026-07-12 授权：Java 轨道收尾期间，本计划是
mini-kv 被批准的维护工作；它从属于 program close——若 Node 侧发起 capstone 复跑，
mini-kv 侧只需保证 CLI 输出合同稳定（本计划的改动本就不允许碰它）。

模板：Java 可维护性计划（v1834–v1837）——先 census、只减不增的 baseline、top-N 整治、
硬停。这不是无限期美化：最多 4 版，之后无论剩多少债都停下请求评审。

约束规则：AGENTS.md「Elegance Gates」节；所有既有硬规则（fixture 字节不动、
receipt/CLI 输出字节稳定、coverage floor 90 不跌、ratchet 只收紧、讲解先于最终
verify、每版 commit/tag/push/CI 绿）。

## E-M1 —— 落地机械门（1 版，最先做）

设计说明（先于实现）：
- 抽象名：`elegance name census`，只负责发现与历史基线比对，不负责自动改名。
- 数据边界：`src/`、`include/`、`tests/` 的文件名，以及 `include/` 头文件可见标识符。
- 词法边界：公共头文件先剔除注释、字符串、字符字面量与 `#include` 路径，再统计标识符。
- 行为边界：新增或替换长名失败；删除旧长名时必须在同一提交收紧 baseline 和上限。
- 非目标：不解析 ABI、不改 CLI/JSON/fixture 字节、不把实现文件局部变量误称为公共标识符。

1. 把评审者在 AGENTS.md 未提交的 Elegance Gates 节随本版一起提交。
2. 新增 CTest 合同（参照 `receipt_builder_census_contract`）：census 全部
   `src/`、`include/`、`tests/` 中超过 40 字符的文件名与公开标识符；存量清单进
   只减不增的 baseline 文件；新增违规 = 测试失败。
3. baseline 数字写进版本 note——那就是债务合同。

## E-M2..E-M3 —— top-5 热点整治（1–2 版）

v1660 family 设计说明（先于改名）：
- 抽象名：`compared package intake audit`，负责比较包证据接收审计的内部实现组织。
- 行为边界：main 聚合流程；stages、slots、guards、validation 保持现有单一职责。
- 数据边界：公共 header、namespace、struct、函数签名与输出字段原样保留。
- 路径边界：只把五个 CMake 私有 `.cpp` 路径缩短到 40 字符内。
- 非目标：不合并模块、不改实现字节、不新增 wrapper、alias、waiver 或运行能力。

1. 从 census 取 5 个最差且没有外部合同钉住名字的命名违规（不碰：CLI 命令名、
   RESP/JSON 输出键、公开 header 的 API、census/waiver 合同点名的文件）：提取缺失
   概念、改名、更新引用；全量 CTest 绿 + 受影响 fixture 字节不变作为证据。
2. 若 census 暴露仍存的结构重复 family（receipts 之外），建共享实现，成员数据化。
3. 每版收缩 baseline，同一提交里 ratchet。

## E-M4 —— 收口（1 版）

最终 census 入档；baseline 钉在终态；版本 note 一页总结：债务起点→终点、
刻意保留项及原因。请求 Claude 评审。

## 失败条件

- 任何 CLI 输出字节、receipt fixture 字节、公开 API 签名变化 = 回退。
- 改名的唯一证据是"能编译" = 版本无效；必须全量 CTest + fixture 字节对照。
- baseline 放松、或用 waiver 代替修复却不写可查证理由 = 失败。
- 超过 4 版不评审 = 违反停止条件。
- 优雅工作与任何功能/无关重构混版 = 回退。

## 执行进度

- E-M1 对应 v1659：本地 351/351；远端 run `29176190797` 六个 job 绿、Windows 因 CRLF baseline 行尾解析失败。该真实 portability 缺口由 v1660 立即修复，不移动历史 tag。
- E-M2 对应 v1660：五个内部 `.cpp` 路径已缩短，baseline 740→735，公共 header 与 883 个标识符不动；修复 CRLF 后，run `29176799196` 又由 changed-files gate 抓到五个旧文件的存量格式债务。
- E-M4 对应 v1661：先格式化这五个 touched units，再冻结 735（include 218 / src 241 / tests 276）与 883 终态；写入两次 CI 失败/修复、保留原因和硬停止证据，最终 CI 后停在 Claude review 点。
