function(minikv_configure_test_runtime test_name)
    if(MINGW)
        get_filename_component(mingw_runtime_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        set_tests_properties(${test_name}
            PROPERTIES
                ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${mingw_runtime_dir}"
        )
    endif()
endfunction()

function(minikv_enable_test_assertions target_name)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /UNDEBUG)
    else()
        target_compile_options(${target_name} PRIVATE -UNDEBUG)
    endif()
endfunction()

function(minikv_add_bundled_test target_name test_name source_file)
    if(NOT test_name MATCHES "^[A-Za-z0-9_]+$")
        message(FATAL_ERROR "bundled CTest name is not a safe runner key: ${test_name}")
    endif()

    string(SHA256 case_digest "${test_name}")
    string(SUBSTRING "${case_digest}" 0 12 case_key)
    string(SUBSTRING "${case_digest}" 0 6 shard_key)
    math(EXPR shard_index "0x${shard_key} % ${MINIKV_TEST_SHARD_COUNT}")
    set(entry_symbol "minikv_case_${case_key}")

    get_property(known_entries GLOBAL PROPERTY MINIKV_TEST_ENTRIES)
    if(entry_symbol IN_LIST known_entries)
        message(FATAL_ERROR "bundled test entry hash collision: ${test_name}")
    endif()
    set_property(GLOBAL APPEND PROPERTY MINIKV_TEST_ENTRIES "${entry_symbol}")
    set_property(GLOBAL APPEND PROPERTY MINIKV_TEST_SHARD_${shard_index}
        "${target_name}|${test_name}|${entry_symbol}"
    )

    add_library(${target_name} OBJECT
        ${source_file}
    )
    minikv_enable_test_assertions(${target_name})
    target_compile_definitions(${target_name} PRIVATE "main=${entry_symbol}")
    target_link_libraries(${target_name} PRIVATE minikv)

    add_test(NAME ${test_name} COMMAND minikv_test_shard_${shard_index} ${test_name})
    minikv_configure_test_runtime(${test_name})
endfunction()

function(minikv_add_linked_test target_name test_name source_file)
    if(MINIKV_BUNDLE_TESTS)
        minikv_add_bundled_test(${target_name} ${test_name} ${source_file})
    else()
        add_executable(${target_name}
            ${source_file}
        )
        minikv_enable_test_assertions(${target_name})

        target_link_libraries(${target_name} PRIVATE minikv)

        add_test(NAME ${test_name} COMMAND ${target_name})
        minikv_configure_test_runtime(${test_name})
    endif()
endfunction()

function(minikv_add_source_dir_test target_name test_name source_file)
    minikv_add_linked_test(${target_name} ${test_name} ${source_file})

    target_compile_definitions(${target_name}
        PRIVATE
            MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    )
endfunction()

function(minikv_add_standalone_source_dir_test target_name test_name source_file)
    add_executable(${target_name}
        ${source_file}
    )
    minikv_enable_test_assertions(${target_name})

    target_compile_definitions(${target_name}
        PRIVATE
            MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    )

    add_test(NAME ${test_name} COMMAND ${target_name})
    minikv_configure_test_runtime(${test_name})
endfunction()

function(minikv_read_test_manifest manifest_path out_rows)
    file(READ "${manifest_path}" manifest_hex HEX)
    string(FIND "${manifest_hex}" "0d" cr_offset)
    if(NOT cr_offset EQUAL -1)
        message(FATAL_ERROR "test case manifest must use LF line endings: ${manifest_path}")
    endif()

    file(STRINGS "${manifest_path}" manifest_rows)
    set(validated_rows)
    set(seen_targets)
    set(seen_names)
    set(pre_smoke_count 0)
    set(main_count 0)
    foreach(row IN LISTS manifest_rows)
        string(STRIP "${row}" row)
        if(row STREQUAL "" OR row MATCHES "^#")
            continue()
        endif()

        string(REPLACE "|" ";" columns "${row}")
        list(LENGTH columns column_count)
        if(NOT column_count EQUAL 5)
            message(FATAL_ERROR "test case manifest row must have five columns: ${row}")
        endif()

        list(GET columns 0 phase)
        list(GET columns 1 kind)
        list(GET columns 2 target_name)
        list(GET columns 3 test_name)
        list(GET columns 4 source_file)
        if(NOT phase STREQUAL "pre_smoke" AND NOT phase STREQUAL "main")
            message(FATAL_ERROR "unknown test case manifest phase: ${phase}")
        endif()
        if(NOT kind STREQUAL "linked" AND NOT kind STREQUAL "source_dir")
            message(FATAL_ERROR "unknown test case manifest kind: ${kind}")
        endif()
        if(NOT target_name MATCHES "^[A-Za-z0-9_]+$" OR NOT test_name MATCHES "^[A-Za-z0-9_]+$")
            message(FATAL_ERROR "test case manifest names must be safe identifiers: ${row}")
        endif()
        if(NOT source_file MATCHES "^tests/[A-Za-z0-9_./-]+[.]cpp$" OR source_file MATCHES "(^|/)[.][.](/|$)")
            message(FATAL_ERROR "invalid test case source path: ${source_file}")
        endif()
        if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${source_file}")
            message(FATAL_ERROR "test case source is missing: ${source_file}")
        endif()
        if(target_name IN_LIST seen_targets)
            message(FATAL_ERROR "duplicate test case target: ${target_name}")
        endif()
        if(test_name IN_LIST seen_names)
            message(FATAL_ERROR "duplicate public CTest name: ${test_name}")
        endif()

        list(APPEND seen_targets "${target_name}")
        list(APPEND seen_names "${test_name}")
        list(APPEND validated_rows "${row}")
        math(EXPR ${phase}_count "${${phase}_count} + 1")
    endforeach()

    list(LENGTH validated_rows case_count)
    if(NOT case_count EQUAL 342 OR NOT pre_smoke_count EQUAL 8 OR NOT main_count EQUAL 334)
        message(FATAL_ERROR
            "test case manifest census changed: total=${case_count}, pre_smoke=${pre_smoke_count}, main=${main_count}"
        )
    endif()
    set(${out_rows} "${validated_rows}" PARENT_SCOPE)
endfunction()

function(minikv_register_test_group manifest_path requested_phase)
    if(NOT requested_phase STREQUAL "pre_smoke" AND NOT requested_phase STREQUAL "main")
        message(FATAL_ERROR "unknown requested test phase: ${requested_phase}")
    endif()

    minikv_read_test_manifest("${manifest_path}" manifest_rows)
    foreach(row IN LISTS manifest_rows)
        string(REPLACE "|" ";" columns "${row}")
        list(GET columns 0 phase)
        if(NOT phase STREQUAL requested_phase)
            continue()
        endif()

        list(GET columns 1 kind)
        list(GET columns 2 target_name)
        list(GET columns 3 test_name)
        list(GET columns 4 source_file)
        if(kind STREQUAL "linked")
            minikv_add_linked_test(${target_name} ${test_name} ${source_file})
        else()
            minikv_add_source_dir_test(${target_name} ${test_name} ${source_file})
        endif()
    endforeach()
endfunction()

function(minikv_add_test_parts target_name manifest_path)
    file(STRINGS "${manifest_path}" manifest_rows)
    set(part_sources)
    foreach(row IN LISTS manifest_rows)
        string(STRIP "${row}" row)
        if(row STREQUAL "" OR row MATCHES "^#")
            continue()
        endif()
        string(REPLACE "|" ";" columns "${row}")
        list(LENGTH columns column_count)
        if(NOT column_count EQUAL 2)
            message(FATAL_ERROR "invalid test part manifest row: ${row}")
        endif()
        list(GET columns 0 source_path)
        if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${source_path}")
            message(FATAL_ERROR "test part source is missing: ${source_path}")
        endif()
        list(APPEND part_sources "${PROJECT_SOURCE_DIR}/${source_path}")
    endforeach()
    target_sources(${target_name} PRIVATE ${part_sources})
endfunction()

function(minikv_finalize_test_bundle)
    if(NOT MINIKV_BUNDLE_TESTS)
        return()
    endif()

    get_property(all_entries GLOBAL PROPERTY MINIKV_TEST_ENTRIES)
    list(LENGTH all_entries case_count)
    if(case_count EQUAL 0)
        message(FATAL_ERROR "MINIKV_BUNDLE_TESTS is enabled but no linked tests were registered")
    endif()

    math(EXPR last_shard "${MINIKV_TEST_SHARD_COUNT} - 1")
    foreach(shard_index RANGE 0 ${last_shard})
        get_property(shard_records GLOBAL PROPERTY MINIKV_TEST_SHARD_${shard_index})
        if(NOT shard_records)
            message(FATAL_ERROR "test link shard ${shard_index} is empty")
        endif()

        set(MINIKV_TEST_DECLARATIONS "")
        set(MINIKV_TEST_CASES "")
        set(shard_targets)
        foreach(record IN LISTS shard_records)
            string(REPLACE "|" ";" columns "${record}")
            list(GET columns 0 object_target)
            list(GET columns 1 test_name)
            list(GET columns 2 entry_symbol)
            string(APPEND MINIKV_TEST_DECLARATIONS "int ${entry_symbol}();\n")
            string(APPEND MINIKV_TEST_CASES "    {\"${test_name}\", &${entry_symbol}},\n")
            list(APPEND shard_targets ${object_target})
        endforeach()

        set(shard_target "minikv_test_shard_${shard_index}")
        set(runner_source "${PROJECT_BINARY_DIR}/generated/${shard_target}.cpp")
        file(READ "${PROJECT_SOURCE_DIR}/cmake/minikv_test_runner.cpp.in" runner_text)
        string(REPLACE "/*MINIKV_TEST_DECLARATIONS*/" "${MINIKV_TEST_DECLARATIONS}" runner_text "${runner_text}")
        string(REPLACE "/*MINIKV_TEST_CASES*/" "${MINIKV_TEST_CASES}" runner_text "${runner_text}")
        file(CONFIGURE OUTPUT "${runner_source}" CONTENT "${runner_text}" @ONLY)
        add_executable(${shard_target} "${runner_source}")
        target_link_libraries(${shard_target} PRIVATE minikv ${shard_targets})
    endforeach()

    message(STATUS "mini-kv test link topology: ${case_count} cases / ${MINIKV_TEST_SHARD_COUNT} stable shards")
endfunction()
