function(minikv_add_linked_test target_name test_name source_file)
    add_executable(${target_name}
        ${source_file}
    )

    target_link_libraries(${target_name} PRIVATE minikv)

    add_test(NAME ${test_name} COMMAND ${target_name})
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
endfunction()
