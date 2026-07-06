if(NOT DEFINED DOC_PATH)
    message(FATAL_ERROR "DOC_PATH is required")
endif()

if(NOT DEFINED SOURCE_DIR)
    message(FATAL_ERROR "SOURCE_DIR is required")
endif()

if(NOT EXISTS "${DOC_PATH}")
    message(FATAL_ERROR "receipts consolidation note is missing: ${DOC_PATH}")
endif()

file(READ "${DOC_PATH}" doc_text)

set(required_fragments
    "# Runtime receipts 合并设计说明"
    "Necessary proof"
    "Shared formatter design"
    "Byte-identical output contract"
    "788,811"
    "706,166"
    "runtime_receipt_json_builder"
    "fixtures/release/*.json"
    "fixture parity harness"
    "不修改 fixture"
    "read_only"
    "execution_allowed"
    "no credential"
    "no network"
    "no write"
    "Slice 0"
    "Slice 1"
    "Line-count shrink is a review metric, not a stop condition"
    "+116 diff lines"
    "false shrink metric"
    "Node v323\\u0027s no-network fixture contract"
    "canonical runtime surface"
    "直接 apostrophe 拼写"
    "runtime_credential_resolver_no_network_safety_fixture_receipts.cpp"
    "full CTest"
)

foreach(fragment IN LISTS required_fragments)
    string(FIND "${doc_text}" "${fragment}" fragment_offset)
    if(fragment_offset EQUAL -1)
        message(FATAL_ERROR "receipts consolidation note is missing required fragment: ${fragment}")
    endif()
endforeach()

file(GLOB runtime_receipt_files "${SOURCE_DIR}/src/runtime_*_receipts.cpp")
list(LENGTH runtime_receipt_files runtime_receipt_count)
if(NOT runtime_receipt_count EQUAL 28)
    message(FATAL_ERROR "runtime receipt census changed: expected 28, found ${runtime_receipt_count}; update the design note before refactoring")
endif()

file(GLOB specialized_credential_receipt_files
    "${SOURCE_DIR}/src/runtime_credential_resolver_*_receipts.cpp"
)
list(LENGTH specialized_credential_receipt_files specialized_credential_receipt_count)
if(NOT specialized_credential_receipt_count EQUAL 25)
    message(FATAL_ERROR "specialized credential receipt census changed: expected 25, found ${specialized_credential_receipt_count}")
endif()

if(NOT EXISTS "${SOURCE_DIR}/src/runtime_credential_resolver_receipts.cpp")
    message(FATAL_ERROR "credential resolver base receipt file is missing")
endif()

message(STATUS "receipts consolidation note contract ok: 28 runtime receipt files, 26 credential resolver files")
