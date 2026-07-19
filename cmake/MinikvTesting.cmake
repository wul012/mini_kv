function(minikv_configure_test_runtime test_name)
    if(MINGW)
        get_filename_component(mingw_runtime_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
        set_tests_properties(${test_name}
            PROPERTIES
                ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${mingw_runtime_dir}"
        )
    endif()
endfunction()

function(minikv_add_linked_test target_name test_name source_file)
    add_executable(${target_name}
        ${source_file}
    )

    target_link_libraries(${target_name} PRIVATE minikv)

    add_test(NAME ${test_name} COMMAND ${target_name})
    minikv_configure_test_runtime(${test_name})
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
