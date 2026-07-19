if(NOT DEFINED SOURCE_DIR)
    message(FATAL_ERROR "SOURCE_DIR is required")
endif()

function(require_file_fragment relative_path fragment)
    set(path "${SOURCE_DIR}/${relative_path}")
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "required project document is missing: ${relative_path}")
    endif()
    file(READ "${path}" text)
    string(FIND "${text}" "${fragment}" offset)
    if(offset EQUAL -1)
        message(FATAL_ERROR "${relative_path} is missing current-tree evidence fragment: ${fragment}")
    endif()
endfunction()

foreach(path IN ITEMS
        "src/osfs_filesystem.cpp"
        "src/osfs_directory.cpp"
        "src/osfs_file_io.cpp"
        "src/osfs_fsck.cpp"
        "src/osfs_user_admin.cpp"
        "src/osfs_command_processor.cpp"
        "include/minikv/osfs/command_processor.hpp"
        "tests/osfs_tests.cpp"
        "tests/osfs_test_support.hpp"
        "tests/osfs_resilience_tests.cpp")
    if(NOT EXISTS "${SOURCE_DIR}/${path}")
        message(FATAL_ERROR "documented OSFS evidence path is missing: ${path}")
    endif()
endforeach()

require_file_fragment("README.md" "- v1663:")
require_file_fragment("README.md" "353 registered CTest tests")
require_file_fragment("README.md" "27 builder-backed / 0 pending / 1 named no-formatter waiver")
require_file_fragment("README.md" "- v1664:")
require_file_fragment("README.md" "- v1665:")
require_file_fragment("README.md" "- v1666:")
require_file_fragment("README.md" "Maturity: single-project validation + verified read-only cross-project integration (env-gated, single machine, no execution authority).")
require_file_fragment("README.md" "2345 行、执行 2122 行、90%；CI 强制 **90% floor**")
require_file_fragment("README.md" "存量长文件名 735、公共头文件可见长标识符 883")
require_file_fragment("README.md" "默认只监听 loopback，但协议本身没有认证或 TLS")
require_file_fragment("README.md" "not a hardened production database")
require_file_fragment("README.md" "program-close evidence")
require_file_fragment("START_HERE.md" "Current focus: **v1666 zero-baseline test architecture closeout**")
require_file_fragment("START_HERE.md" "Independent OSFS course-design filesystem")
require_file_fragment("START_HERE.md" "353 registered tests")
require_file_fragment("START_HERE.md" "OSFS课程设计通俗教程/")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "## Independent OSFS course layer")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "src/osfs_fsck.cpp")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "tests/osfs_tests.cpp")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "8 + block_size / 4")
require_file_fragment("docs/archive-retention-index.md" "1597-1666")
require_file_fragment("docs/archive-retention-index.md" "11,316,168")
require_file_fragment("docs/archive-retention-index.md" "89,788")
require_file_fragment("docs/archive-retention-index.md" "3,440,216")
require_file_fragment("docs/archive-retention-index.md" "5,445,971")
require_file_fragment("docs/project-docs-honesty-matrix.md" "Only two OSFS execution briefs exist")
require_file_fragment("docs/elegance-hotspot-closeout.md" "Status: EXTERNALLY REVIEWED PASS (Claude, 2026-07-12). Final CI: GREEN.")
require_file_fragment("docs/elegance-hotspot-closeout.md" "29177475570")
require_file_fragment("docs/elegance-hotspot-closeout.md" "| filename stems over 40 | 740 | 735 | -5 |")
require_file_fragment("docs/elegance-hotspot-closeout.md" "include=218`, `src=241`, `tests=276")
require_file_fragment("docs/elegance-hotspot-closeout.md" "The executor did not award this verdict")
require_file_fragment("docs/elegance-round2-pin-audit.md" "E2-M2：NOT TRIGGERED")
require_file_fragment("docs/elegance-round2-pin-audit.md" "safe = 1")
require_file_fragment("docs/elegance-round2-pin-audit.md" "pin_audit=9/1 branch=close")
require_file_fragment("docs/elegance-round2-pin-audit.md" "29189480986")
require_file_fragment("docs/CHANGELOG.md" "- v1656:")
require_file_fragment("治理计划/v1631-osfs-coursework-completion-brief.md" "执行状态：已执行完毕")
require_file_fragment("治理计划/v1634-osfs-capacity-extension-brief.md" "执行状态：已执行完毕")
require_file_fragment("治理计划/v1663-osfs-teaching-evidence.md" "需求-证据矩阵")
require_file_fragment("治理计划/readme-exhibition-brief.md" "需求-证据矩阵")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/README.md" "1031-v1663-osfs-tutorial-resilience.md")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/1031-v1663-osfs-tutorial-resilience.md" "注册测试总数从 351 变为 352")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/README.md" "1032-v1664-readme-exhibition.md")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/1032-v1664-readme-exhibition.md" "README 为什么也需要可执行证据")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/README.md" "1033-v1665-command-test-architecture.md")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/1033-v1665-command-test-architecture.md" "为什么不能直接切成九个独立测试")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/README.md" "1034-v1666-shard-test-architecture.md")
require_file_fragment("代码讲解记录_生产雏形阶段_第四册/1034-v1666-shard-test-architecture.md" "为什么 shard 拆分比 command 更危险")

require_file_fragment("OSFS课程设计通俗教程/README.md" "## 证据强度")
require_file_fragment("OSFS课程设计通俗教程/00-十分钟看懂OSFS.md" "不是网络路由协议 OSPF")
require_file_fragment("OSFS课程设计通俗教程/01-CREATE到WRITE完整流程.md" "CommandProcessor::execute")
require_file_fragment("OSFS课程设计通俗教程/02-磁盘布局与inode.md" "sizeof(InodeDisk) == 96")
require_file_fragment("OSFS课程设计通俗教程/03-用户表与二级目录.md" "MFD 和用户表为什么要同时存在")
require_file_fragment("OSFS课程设计通俗教程/04-文件描述符与区间读写.md" "read_offset")
require_file_fragment("OSFS课程设计通俗教程/05-直接块与一级间接块.md" "8 + block_size / 4")
require_file_fragment("OSFS课程设计通俗教程/06-FSCK与用户管理.md" "FSCK ERROR checks=6")
require_file_fragment("OSFS课程设计通俗教程/07-测试地图与答辩问答.md" "osfs_resilience_tests")
require_file_fragment("课程设计交付/v1636-osfs-final/finalshell-分段演示记录/README.md" "step08-indirect-boundary")
require_file_fragment("课程设计交付/v1636-osfs-final/finalshell-分段演示记录/README.md" "step09-fsck-corruption")
require_file_fragment("课程设计交付/v1636-osfs-final/finalshell-分段演示记录/00-复核说明.md" "不能用重复 USERADD 行证明创建账号成功")

file(READ "${SOURCE_DIR}/tests/osfs_tests.cpp" osfs_tests)
foreach(fragment IN ITEMS
        "test_disk_users_and_two_level_directories"
        "test_authenticated_command_shell"
        "test_descriptor_offsets_and_range_io"
        "test_indirect_blocks_persist_and_release"
        "test_fsck_reports_ok_and_detects_corruption"
        "test_useradd_passwd_and_fsck_command")
    string(FIND "${osfs_tests}" "${fragment}" offset)
    if(offset EQUAL -1)
        message(FATAL_ERROR "documented OSFS test evidence is missing: ${fragment}")
    endif()
endforeach()

file(READ "${SOURCE_DIR}/tests/osfs_resilience_tests.cpp" osfs_resilience_tests)
foreach(fragment IN ITEMS
        "test_login_switch_closes_descriptors"
        "test_group_permissions_and_root_override"
        "test_directory_growth_and_slot_reuse"
        "test_sparse_range_write_zero_fills_gap"
        "test_fsck_finds_inode_bitmap_corruption")
    string(FIND "${osfs_resilience_tests}" "${fragment}" offset)
    if(offset EQUAL -1)
        message(FATAL_ERROR "documented OSFS resilience evidence is missing: ${fragment}")
    endif()
endforeach()

message(STATUS "project docs honesty contract ok: v1664 exhibit, 353 tests, reviewed capstone label, OSFS resilience, and zero-baseline test architecture")
