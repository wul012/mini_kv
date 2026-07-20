function(minikv_configure_test_runtime test_name)
    if(MINGW)
        get_filename_component(mingw_runtime_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        set_tests_properties(${test_name}
            PROPERTIES
                ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${mingw_runtime_dir}"
        )
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

    target_compile_definitions(${target_name}
        PRIVATE
            MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    )

    add_test(NAME ${test_name} COMMAND ${target_name})
    minikv_configure_test_runtime(${test_name})
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
