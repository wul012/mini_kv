if(NOT DEFINED SOURCE_DIR)
    message(FATAL_ERROR "SOURCE_DIR is required")
endif()

function(require_file_fragment relative_path fragment)
    set(path "${SOURCE_DIR}/${relative_path}")
    if(NOT EXISTS "${path}")
        message(FATAL_ERROR "required final-evidence file is missing: ${relative_path}")
    endif()
    file(READ "${path}" text)
    string(FIND "${text}" "${fragment}" offset)
    if(offset EQUAL -1)
        message(FATAL_ERROR "${relative_path} is missing final-evidence fragment: ${fragment}")
    endif()
endfunction()

set(evidence_path "docs/minikv-track-final-evidence.md")
foreach(fragment IN ITEMS
        "Status: READY FOR EXTERNAL REVIEW"
        "External verdict: PENDING"
        "single-project validation + cross-project contract alignment"
        "| E1 Build & CI | GREEN |"
        "| E2 Static analysis | GREEN |"
        "| E3 Coverage | GREEN |"
        "| E4 Security & config | GREEN |"
        "| E5 Observability | GREEN |"
        "| E6 Error handling | GREEN |"
        "| E7 Docs honesty | GREEN |"
        "| E8 Release discipline | GREEN |"
        "| E9 Code health | GREEN |"
        "| E10 Archive retention | GREEN |"
        "2345 core lines / 2122 executed / 90%"
        "--fail-under-line 90"
        "28 sources / 27 formatter owners / 27 builder-backed / 0 pending / 1 named no-formatter census exemption"
        "Exactly two program-level deviations require external acceptance"
        "src/shard_readiness_boundary_field_names.cpp"
        "negative_hits.warn_once_per_file"
        "No third program-level waiver is claimed"
        "https://github.com/wul012/mini_kv/actions/runs/29095734744")
    require_file_fragment("${evidence_path}" "${fragment}")
endforeach()

foreach(fragment IN ITEMS
        "push:"
        "name: clang-format changed files"
        "- os: ubuntu-latest"
        "- os: macos-latest"
        "- os: windows-latest"
        "name: ubuntu sanitizer"
        "name: ubuntu coverage"
        "--gcov-ignore-parse-errors negative_hits.warn_once_per_file"
        "--fail-under-line 90"
        "python3 scripts/archive_inventory.py --budget-mib 8 --github-warning")
    require_file_fragment(".github/workflows/ci.yml" "${fragment}")
endforeach()

require_file_fragment("include/minikv/tcp_server.hpp" "std::string host = \"127.0.0.1\";")
require_file_fragment("tests/tcp_server_tests.cpp" "assert(secure_defaults.host == \"127.0.0.1\");")
require_file_fragment("include/minikv/log.hpp" "explicit Logger(LogLevel minimum_level = LogLevel::warn);")
require_file_fragment("src/log.cpp" "thread=")
require_file_fragment("src/tcp_server.cpp" "connection_id=")
require_file_fragment("src/tcp_server.cpp" "event=tcp_request_rejected")
require_file_fragment("src/server_main.cpp" "std::signal(SIGTERM, handle_shutdown_signal);")
require_file_fragment("tests/tcp_server_timeout_limit_tests.cpp" "reason=client_idle_timeout")
require_file_fragment("tests/tcp_server_timeout_limit_tests.cpp" "reason=command_timeout")
require_file_fragment("docs/SECURITY.md" "No production secret storage.")
require_file_fragment("docs/SECURITY.md" "not a hardened production database")
require_file_fragment("docs/SECURITY.md" "Do not place it directly on an untrusted network.")
require_file_fragment("docs/CHANGELOG.md" "Git delivery tags: `vNNNN`")
require_file_fragment("docs/project-docs-honesty-matrix.md" "project_docs_honesty_contract")
require_file_fragment("CMakeLists.txt" "-DMIN_BUILDER_BACKED=27")
require_file_fragment("CMakeLists.txt" "NAME receipt_builder_census_contract")
require_file_fragment("CMakeLists.txt" "NAME project_docs_honesty_contract")
require_file_fragment("scripts/archive_inventory.py" "budget-mib")
require_file_fragment("docs/archive-retention-index.md" "ACTIVE ROOT; EXISTING VERSION DIRECTORIES FROZEN")

# E4 uses a deliberately narrow, high-confidence scan over executable/configuration
# surfaces. Boundary prose and frozen fixtures are checked by their own contracts.
file(GLOB_RECURSE secret_scan_files
    "${SOURCE_DIR}/src/*.cpp"
    "${SOURCE_DIR}/src/*.hpp"
    "${SOURCE_DIR}/include/*.hpp"
    "${SOURCE_DIR}/.github/*.yml"
    "${SOURCE_DIR}/.github/*.yaml"
)
list(APPEND secret_scan_files "${SOURCE_DIR}/CMakeLists.txt")
foreach(path IN LISTS secret_scan_files)
    file(READ "${path}" text)
    foreach(pattern IN ITEMS
            "AKIA[0-9A-Z]"
            "ghp_[A-Za-z0-9]"
            "xox[baprs]-[A-Za-z0-9]"
            "(^|[^A-Za-z0-9])sk-[A-Za-z0-9][A-Za-z0-9][A-Za-z0-9][A-Za-z0-9][A-Za-z0-9][A-Za-z0-9][A-Za-z0-9][A-Za-z0-9]"
            "-----BEGIN (RSA |EC |OPENSSH )?PRIVATE KEY-----")
        string(REGEX MATCH "${pattern}" match "${text}")
        if(match)
            file(RELATIVE_PATH relative_path "${SOURCE_DIR}" "${path}")
            message(FATAL_ERROR "high-confidence secret pattern found in ${relative_path}: ${pattern}")
        endif()
    endforeach()
endforeach()

# The E9 threshold is measured as nonblank physical lines, matching the K4
# review's 732-line command.cpp accounting. One registry-only waiver is allowed.
file(GLOB_RECURSE source_files
    "${SOURCE_DIR}/src/*.cpp"
    "${SOURCE_DIR}/src/*.hpp"
    "${SOURCE_DIR}/include/*.hpp"
)
set(size_waiver_seen FALSE)
foreach(path IN LISTS source_files)
    file(STRINGS "${path}" lines)
    set(nonblank_lines 0)
    foreach(line IN LISTS lines)
        string(STRIP "${line}" stripped)
        if(NOT stripped STREQUAL "")
            math(EXPR nonblank_lines "${nonblank_lines} + 1")
        endif()
    endforeach()

    file(RELATIVE_PATH relative_path "${SOURCE_DIR}" "${path}")
    string(REPLACE "\\" "/" relative_path "${relative_path}")
    if(nonblank_lines GREATER 800)
        if(relative_path STREQUAL "src/shard_readiness_boundary_field_names.cpp")
            set(size_waiver_seen TRUE)
        else()
            message(FATAL_ERROR "E9 source-size limit exceeded without waiver: ${relative_path} (${nonblank_lines} nonblank lines)")
        endif()
    endif()
endforeach()

if(NOT size_waiver_seen)
    message(FATAL_ERROR "the sole E9 source-size waiver is stale or missing")
endif()

message(STATUS "mini-kv final evidence contract ok: E1-E10 evidence, 90% floor, safe defaults, two deviations, and E9 size ratchet")
