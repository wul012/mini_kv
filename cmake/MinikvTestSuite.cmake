set(minikv_test_manifest "${CMAKE_CURRENT_SOURCE_DIR}/config/test-cases.txt")

add_test(NAME benchmark_evidence_guard COMMAND minikv_benchmark --evidence-json 12 4)
set_tests_properties(benchmark_evidence_guard
    PROPERTIES
        PASS_REGULAR_EXPRESSION "\"evidence_type\":\"mini-kv-benchmark-evidence-guard\\.v1\".*\"wal_enabled\":false.*\"network_started\":false.*\"restore_executed\":false.*\"managed_audit_connection_opened\":false.*\"final_store_size\":0"
)
minikv_configure_test_runtime(benchmark_evidence_guard)

minikv_add_standalone_source_dir_test(
    minikv_dependabot_config_tests
    dependabot_config_tests
    tests/dependabot_config_tests.cpp
)

add_test(
    NAME project_orientation_doc_contract
    COMMAND ${CMAKE_COMMAND}
            -DDOC_PATH=${CMAKE_CURRENT_SOURCE_DIR}/项目通俗说明/mini-kv-项目机制与价值说明.md
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_project_orientation_doc.cmake
)

add_test(
    NAME project_docs_honesty_contract
    COMMAND ${CMAKE_COMMAND}
            -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_project_docs_honesty.cmake
)

add_test(
    NAME minikv_track_final_evidence_contract
    COMMAND ${CMAKE_COMMAND}
            -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_minikv_track_final_evidence.cmake
)

minikv_add_standalone_source_dir_test(
    minikv_test_architecture_tests
    test_architecture_contract
    tests/test_architecture_tests.cpp
)
target_compile_definitions(minikv_test_architecture_tests
    PRIVATE
        MINIKV_BINARY_DIR="${PROJECT_BINARY_DIR}"
)

add_test(
    NAME receipts_consolidation_note_contract
    COMMAND ${CMAKE_COMMAND}
            -DDOC_PATH=${CMAKE_CURRENT_SOURCE_DIR}/docs/receipts-consolidation-note.md
            -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_receipts_consolidation_note.cmake
)

add_test(
    NAME receipt_builder_census_contract
    COMMAND ${CMAKE_COMMAND}
            -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -DMIN_BUILDER_BACKED=27
            -DEXPECTED_MANUAL_FIELD_PUSHES=613
            -DEXPECTED_LOCAL_DIGEST_WRAPPERS=1
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_receipt_builder_census.cmake
)

minikv_add_standalone_source_dir_test(
    minikv_elegance_name_census
    elegance_name_census_contract
    tests/elegance_name_census_tests.cpp
)
target_compile_definitions(
    minikv_elegance_name_census
    PRIVATE
        MINIKV_NAME_FILE_MAX=735
        MINIKV_NAME_ID_MAX=883
)

minikv_register_test_group("${minikv_test_manifest}" pre_smoke)
target_include_directories(minikv_runtime_receipt_json_builder_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

add_test(
    NAME osfs_cli_smoke
    COMMAND ${CMAKE_COMMAND}
            -DOSFS_PATH=$<TARGET_FILE:minikv_osfs>
            -DDISK_PATH=${CMAKE_CURRENT_BINARY_DIR}/osfs-cli-smoke.img
            -DSCRIPT_PATH=${CMAKE_CURRENT_SOURCE_DIR}/tests/osfs_smoke_script.txt
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_osfs_smoke.cmake
)
minikv_configure_test_runtime(osfs_cli_smoke)

add_test(
    NAME cli_log_level_flag_smoke
    COMMAND ${CMAKE_COMMAND}
            -DCLI_PATH=$<TARGET_FILE:minikv_cli>
            -DINPUT_PATH=${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_quit_input.txt
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_cli_log_level_smoke.cmake
)
set_tests_properties(cli_log_level_flag_smoke
    PROPERTIES
        PASS_REGULAR_EXPRESSION "mini-kv CLI"
)
minikv_configure_test_runtime(cli_log_level_flag_smoke)

add_test(NAME server_log_level_invalid_smoke COMMAND minikv_server --log-level trace)
set_tests_properties(server_log_level_invalid_smoke
    PROPERTIES
        WILL_FAIL TRUE
)
minikv_configure_test_runtime(server_log_level_invalid_smoke)

minikv_register_test_group("${minikv_test_manifest}" main)

target_sources(minikv_release_verification_manifest_tests
    PRIVATE
        tests/release_manifest_checks.cpp
)

minikv_add_test_parts(
    minikv_command_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/config/command-test-parts.txt
)
minikv_add_test_parts(
    minikv_shard_readiness_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/config/shard-test-parts.txt
)
target_include_directories(minikv_atomic_file_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

minikv_finalize_test_bundle()
