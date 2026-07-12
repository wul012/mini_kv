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
        "tests/osfs_tests.cpp")
    if(NOT EXISTS "${SOURCE_DIR}/${path}")
        message(FATAL_ERROR "documented OSFS evidence path is missing: ${path}")
    endif()
endforeach()

require_file_fragment("README.md" "- v1661:")
require_file_fragment("README.md" "351 registered CTest tests")
require_file_fragment("README.md" "27 builder-backed / 0 pending / 1 named no-formatter waiver")
require_file_fragment("START_HERE.md" "Current focus: **v1661 bounded elegance program ready for Claude review**")
require_file_fragment("START_HERE.md" "Independent OSFS course-design filesystem")
require_file_fragment("START_HERE.md" "351 registered tests")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "## Independent OSFS course layer")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "src/osfs_fsck.cpp")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "tests/osfs_tests.cpp")
require_file_fragment("docs/CAPABILITY-SNAPSHOT.md" "8 + block_size / 4")
require_file_fragment("docs/archive-retention-index.md" "1597-1661")
require_file_fragment("docs/archive-retention-index.md" "10,821,570")
require_file_fragment("docs/archive-retention-index.md" "57,132")
require_file_fragment("docs/archive-retention-index.md" "3,440,216")
require_file_fragment("docs/archive-retention-index.md" "5,445,971")
require_file_fragment("docs/project-docs-honesty-matrix.md" "Only two OSFS execution briefs exist")
require_file_fragment("docs/elegance-hotspot-closeout.md" "Status: READY FOR CLAUDE REVIEW. Candidate CI: GREEN. External verdict: PENDING.")
require_file_fragment("docs/elegance-hotspot-closeout.md" "29177475570")
require_file_fragment("docs/elegance-hotspot-closeout.md" "| filename stems over 40 | 740 | 735 | -5 |")
require_file_fragment("docs/elegance-hotspot-closeout.md" "include=218`, `src=241`, `tests=276")
require_file_fragment("docs/elegance-hotspot-closeout.md" "The executor does not award the final verdict")
require_file_fragment("docs/CHANGELOG.md" "- v1656:")
require_file_fragment("治理计划/v1631-osfs-coursework-completion-brief.md" "执行状态：已执行完毕")
require_file_fragment("治理计划/v1634-osfs-capacity-extension-brief.md" "执行状态：已执行完毕")

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

message(STATUS "project docs honesty contract ok: v1661 pointers, OSFS evidence, archive growth, and bounded review state")
