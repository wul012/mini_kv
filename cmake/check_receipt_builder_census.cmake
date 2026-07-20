if(NOT DEFINED SOURCE_DIR OR SOURCE_DIR STREQUAL "")
    message(FATAL_ERROR "SOURCE_DIR is required")
endif()

if(NOT DEFINED MIN_BUILDER_BACKED)
    set(MIN_BUILDER_BACKED 5)
endif()
if(NOT DEFINED EXPECTED_MANUAL_FIELD_PUSHES)
    message(FATAL_ERROR "EXPECTED_MANUAL_FIELD_PUSHES is required")
endif()
if(NOT DEFINED EXPECTED_LOCAL_DIGEST_WRAPPERS)
    message(FATAL_ERROR "EXPECTED_LOCAL_DIGEST_WRAPPERS is required")
endif()

file(GLOB receipt_files "${SOURCE_DIR}/src/runtime_*_receipts.cpp")
list(SORT receipt_files)
list(LENGTH receipt_files receipt_file_count)

file(GLOB receipt_support_files "${SOURCE_DIR}/src/runtime_*receipt*.hpp")
list(FILTER receipt_support_files EXCLUDE REGEX "runtime_receipt_json_builder[.]hpp$")
list(SORT receipt_support_files)

set(receipt_implementation_files ${receipt_files} ${receipt_support_files})
list(LENGTH receipt_implementation_files receipt_implementation_file_count)
set(manual_field_push_count 0)
set(local_digest_wrapper_count 0)
foreach(implementation_file IN LISTS receipt_implementation_files)
    file(READ "${implementation_file}" implementation_source)
    string(REGEX MATCHALL "fields[.]push_back[(]" field_pushes "${implementation_source}")
    list(LENGTH field_pushes file_push_count)
    math(EXPR manual_field_push_count "${manual_field_push_count} + ${file_push_count}")

    string(REGEX MATCHALL
        "std::string[ \t]+receipt_digest[ \t\r\n]*[(]"
        digest_wrappers
        "${implementation_source}"
    )
    list(LENGTH digest_wrappers file_digest_count)
    math(EXPR local_digest_wrapper_count "${local_digest_wrapper_count} + ${file_digest_count}")
endforeach()

set(formatter_files)
set(builder_backed_files)
set(pending_files)
set(waiver_files)

foreach(receipt_file IN LISTS receipt_files)
    file(READ "${receipt_file}" receipt_source)
    get_filename_component(receipt_name "${receipt_file}" NAME)

    if(receipt_source MATCHES "std::string[ \t\r\n]+format_[A-Za-z0-9_]+_receipt_json[ \t\r\n]*\\(")
        list(APPEND formatter_files "${receipt_name}")
        if(receipt_source MATCHES "runtime_receipt_json_builder.hpp")
            list(APPEND builder_backed_files "${receipt_name}")
        else()
            list(APPEND pending_files "${receipt_name}")
        endif()
    else()
        list(APPEND waiver_files "${receipt_name}")
    endif()
endforeach()

list(LENGTH formatter_files formatter_file_count)
list(LENGTH builder_backed_files builder_backed_count)
list(LENGTH pending_files pending_count)
list(LENGTH waiver_files waiver_count)
list(SORT waiver_files)

set(expected_waiver "runtime_credential_resolver_execution_denied_retention_receipts.cpp")

if(NOT receipt_file_count EQUAL 28)
    message(FATAL_ERROR "runtime receipt source census changed: expected 28, got ${receipt_file_count}")
endif()
if(NOT formatter_file_count EQUAL 27)
    message(FATAL_ERROR "formatter owner census changed: expected 27, got ${formatter_file_count}")
endif()
if(NOT waiver_count EQUAL 1 OR NOT "${waiver_files}" STREQUAL "${expected_waiver}")
    message(FATAL_ERROR "receipt census waiver changed: expected ${expected_waiver}, got ${waiver_files}")
endif()
if(builder_backed_count LESS MIN_BUILDER_BACKED)
    message(FATAL_ERROR
        "builder-backed ratchet regressed: floor ${MIN_BUILDER_BACKED}, got ${builder_backed_count}")
endif()
if(manual_field_push_count GREATER EXPECTED_MANUAL_FIELD_PUSHES)
    message(FATAL_ERROR
        "manual receipt field pushes regressed: expected at most ${EXPECTED_MANUAL_FIELD_PUSHES}, "
        "got ${manual_field_push_count}")
endif()
if(manual_field_push_count LESS EXPECTED_MANUAL_FIELD_PUSHES)
    message(FATAL_ERROR
        "manual receipt field push baseline is stale: tighten ${EXPECTED_MANUAL_FIELD_PUSHES} "
        "to ${manual_field_push_count}")
endif()
if(local_digest_wrapper_count GREATER EXPECTED_LOCAL_DIGEST_WRAPPERS)
    message(FATAL_ERROR
        "local receipt digest wrappers regressed: expected at most ${EXPECTED_LOCAL_DIGEST_WRAPPERS}, "
        "got ${local_digest_wrapper_count}")
endif()
if(local_digest_wrapper_count LESS EXPECTED_LOCAL_DIGEST_WRAPPERS)
    message(FATAL_ERROR
        "local receipt digest wrapper baseline is stale: tighten ${EXPECTED_LOCAL_DIGEST_WRAPPERS} "
        "to ${local_digest_wrapper_count}")
endif()

message(STATUS
    "receipt builder census: sources=${receipt_file_count} formatter_owners=${formatter_file_count} "
    "builder_backed=${builder_backed_count} pending=${pending_count} waivers=${waiver_count}")
message(STATUS
    "receipt elegance census: implementation_files=${receipt_implementation_file_count} "
    "manual_field_pushes=${manual_field_push_count} local_digest_wrappers=${local_digest_wrapper_count}")
message(STATUS "builder-backed files: ${builder_backed_files}")
message(STATUS "pending files: ${pending_files}")
message(STATUS "waiver files: ${waiver_files}")
