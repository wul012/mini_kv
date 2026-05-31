cmake_minimum_required(VERSION 3.20)

project(mini_kv VERSION 0.102.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

configure_file(
    ${PROJECT_SOURCE_DIR}/include/minikv/version.hpp.in
    ${PROJECT_BINARY_DIR}/generated/include/minikv/version.hpp
    @ONLY
)

add_library(minikv
    src/client_history.cpp
    src/command.cpp
    src/command_contracts.cpp
    src/command_response_formatters.cpp
    src/command_smoke_formatters.cpp
    src/line_editor.cpp
    src/managed_audit_adapter_receipts.cpp
    src/managed_audit_connection_receipts.cpp
    src/managed_audit_core_receipts.cpp
    src/managed_audit_receipts.cpp
    src/metrics_file.cpp
    src/runtime_evidence.cpp
    src/runtime_credential_resolver_archive_receipts.cpp
    src/runtime_credential_resolver_approval_boundary_receipts.cpp
    src/runtime_credential_resolver_disabled_fake_harness_receipts.cpp
    src/runtime_credential_resolver_execution_denied_receipts.cpp
    src/runtime_credential_resolver_execution_denied_retention_receipts.cpp
    src/runtime_credential_resolver_disabled_runtime_shell_receipts.cpp
    src/runtime_credential_resolver_disabled_runtime_shell_candidate_gate_receipts.cpp
    src/runtime_credential_resolver_runtime_shell_decision_record_receipts.cpp
    src/runtime_credential_resolver_runtime_shell_post_decision_plan_receipts.cpp
    src/runtime_credential_resolver_runtime_shell_stop_prerequisite_receipts.cpp
    src/runtime_credential_resolver_approval_prerequisite_artifact_receipts.cpp
    src/runtime_credential_resolver_human_approval_artifact_receipts.cpp
    src/runtime_credential_resolver_human_approval_post_echo_receipts.cpp
    src/runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp
    src/runtime_credential_resolver_credential_handle_approval_receipts.cpp
    src/runtime_credential_resolver_endpoint_handle_allowlist_approval_receipts.cpp
    src/runtime_credential_resolver_no_network_safety_fixture_receipts.cpp
    src/runtime_credential_resolver_abort_rollback_semantics_receipts.cpp
    src/runtime_credential_resolver_input_hardening_candidate_gate_receipts.cpp
    src/runtime_credential_resolver_implementation_readiness_receipts.cpp
    src/runtime_credential_resolver_implementation_plan_receipts.cpp
    src/runtime_credential_resolver_decision_receipts.cpp
    src/runtime_credential_resolver_disabled_candidate_receipts.cpp
    src/runtime_credential_resolver_plan_receipts.cpp
    src/runtime_credential_resolver_receipts.cpp
    src/runtime_credential_resolver_test_only_shell_receipts.cpp
    src/runtime_evidence_receipts.cpp
    src/runtime_sandbox_receipts.cpp
    src/recovery_evidence.cpp
    src/resp.cpp
    src/shard_readiness.cpp
    src/shard_readiness_approval_inputs.cpp
    src/shard_readiness_boundary_fields.cpp
    src/shard_readiness_node_compatibility.cpp
    src/shard_readiness_slot_preview.cpp
    src/snapshot.cpp
    src/store.cpp
    src/tcp_server.cpp
    src/wal.cpp
)

target_include_directories(minikv
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}/generated/include
)

if(MSVC)
    target_compile_options(minikv PRIVATE /W4 /permissive-)
else()
    target_compile_options(minikv PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    target_compile_definitions(minikv PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
    target_link_libraries(minikv PUBLIC ws2_32)
endif()

add_executable(minikv_cli
    src/main.cpp
)

target_link_libraries(minikv_cli PRIVATE minikv)

add_executable(minikv_server
    src/server_main.cpp
)

target_link_libraries(minikv_server PRIVATE minikv)

add_executable(minikv_client
    src/client_main.cpp
)

target_link_libraries(minikv_client PRIVATE minikv)

add_executable(minikv_benchmark
    src/benchmark_main.cpp
)

target_link_libraries(minikv_benchmark PRIVATE minikv)

enable_testing()

add_test(NAME benchmark_evidence_guard COMMAND minikv_benchmark --evidence-json 12 4)

set_tests_properties(benchmark_evidence_guard
    PROPERTIES
        PASS_REGULAR_EXPRESSION "\"evidence_type\":\"mini-kv-benchmark-evidence-guard\\.v1\".*\"wal_enabled\":false.*\"network_started\":false.*\"restore_executed\":false.*\"managed_audit_connection_opened\":false.*\"final_store_size\":0"
)

add_executable(minikv_dependabot_config_tests
    tests/dependabot_config_tests.cpp
)

target_compile_definitions(minikv_dependabot_config_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME dependabot_config_tests COMMAND minikv_dependabot_config_tests)

add_executable(minikv_store_tests
    tests/store_tests.cpp
)

target_link_libraries(minikv_store_tests PRIVATE minikv)

add_test(NAME store_tests COMMAND minikv_store_tests)

add_executable(minikv_command_tests
    tests/command_tests.cpp
)

target_link_libraries(minikv_command_tests PRIVATE minikv)

target_compile_definitions(minikv_command_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME command_tests COMMAND minikv_command_tests)

add_executable(minikv_shard_readiness_tests
    tests/shard_readiness_tests.cpp
)

target_link_libraries(minikv_shard_readiness_tests PRIVATE minikv)

target_compile_definitions(minikv_shard_readiness_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME shard_readiness_tests COMMAND minikv_shard_readiness_tests)

add_executable(minikv_smokejson_command_receipt_tests
    tests/smokejson_command_receipt_tests.cpp
)

target_link_libraries(minikv_smokejson_command_receipt_tests PRIVATE minikv)

add_test(NAME smokejson_command_receipt_tests COMMAND minikv_smokejson_command_receipt_tests)

add_executable(minikv_managed_audit_receipts_tests
    tests/managed_audit_receipts_tests.cpp
)

target_link_libraries(minikv_managed_audit_receipts_tests PRIVATE minikv)

add_test(NAME managed_audit_receipts_tests COMMAND minikv_managed_audit_receipts_tests)

add_executable(minikv_readonly_fixture_tests
    tests/readonly_fixture_tests.cpp
)

target_link_libraries(minikv_readonly_fixture_tests PRIVATE minikv)

target_compile_definitions(minikv_readonly_fixture_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME readonly_fixture_tests COMMAND minikv_readonly_fixture_tests)

add_executable(minikv_wal_tests
    tests/wal_tests.cpp
)

target_link_libraries(minikv_wal_tests PRIVATE minikv)

add_test(NAME wal_tests COMMAND minikv_wal_tests)

add_executable(minikv_snapshot_tests
    tests/snapshot_tests.cpp
)

target_link_libraries(minikv_snapshot_tests PRIVATE minikv)

add_test(NAME snapshot_tests COMMAND minikv_snapshot_tests)

add_executable(minikv_stress_tests
    tests/stress_tests.cpp
)

target_link_libraries(minikv_stress_tests PRIVATE minikv)

add_test(NAME stress_tests COMMAND minikv_stress_tests)

add_executable(minikv_resp_tests
    tests/resp_tests.cpp
)

target_link_libraries(minikv_resp_tests PRIVATE minikv)

add_test(NAME resp_tests COMMAND minikv_resp_tests)

add_executable(minikv_tcp_server_tests
    tests/tcp_server_tests.cpp
)

target_link_libraries(minikv_tcp_server_tests PRIVATE minikv)

add_test(NAME tcp_server_tests COMMAND minikv_tcp_server_tests)

add_executable(minikv_tcp_resp_tests
    tests/tcp_resp_tests.cpp
)

target_link_libraries(minikv_tcp_resp_tests PRIVATE minikv)

add_test(NAME tcp_resp_tests COMMAND minikv_tcp_resp_tests)

add_executable(minikv_tcp_resp_compat_tests
    tests/tcp_resp_compat_tests.cpp
)

target_link_libraries(minikv_tcp_resp_compat_tests PRIVATE minikv)

add_test(NAME tcp_resp_compat_tests COMMAND minikv_tcp_resp_compat_tests)

add_executable(minikv_tcp_resp_concurrency_tests
    tests/tcp_resp_concurrency_tests.cpp
)

target_link_libraries(minikv_tcp_resp_concurrency_tests PRIVATE minikv)

add_test(NAME tcp_resp_concurrency_tests COMMAND minikv_tcp_resp_concurrency_tests)

add_executable(minikv_client_history_tests
    tests/client_history_tests.cpp
)

target_link_libraries(minikv_client_history_tests PRIVATE minikv)

add_test(NAME client_history_tests COMMAND minikv_client_history_tests)

add_executable(minikv_line_editor_tests
    tests/line_editor_tests.cpp
)

target_link_libraries(minikv_line_editor_tests PRIVATE minikv)

add_test(NAME line_editor_tests COMMAND minikv_line_editor_tests)

add_executable(minikv_metrics_file_tests
    tests/metrics_file_tests.cpp
)

target_link_libraries(minikv_metrics_file_tests PRIVATE minikv)

add_test(NAME metrics_file_tests COMMAND minikv_metrics_file_tests)

add_executable(minikv_recovery_evidence_tests
    tests/recovery_evidence_tests.cpp
)

target_link_libraries(minikv_recovery_evidence_tests PRIVATE minikv)

target_compile_definitions(minikv_recovery_evidence_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME recovery_evidence_tests COMMAND minikv_recovery_evidence_tests)

add_executable(minikv_recovery_fixture_index_tests
    tests/recovery_fixture_index_tests.cpp
)

target_link_libraries(minikv_recovery_fixture_index_tests PRIVATE minikv)

target_compile_definitions(minikv_recovery_fixture_index_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME recovery_fixture_index_tests COMMAND minikv_recovery_fixture_index_tests)

add_executable(minikv_ttl_token_recovery_tests
    tests/ttl_token_recovery_tests.cpp
)

target_link_libraries(minikv_ttl_token_recovery_tests PRIVATE minikv)

target_compile_definitions(minikv_ttl_token_recovery_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME ttl_token_recovery_tests COMMAND minikv_ttl_token_recovery_tests)

add_executable(minikv_release_verification_manifest_tests
    tests/release_verification_manifest_tests.cpp
)

target_link_libraries(minikv_release_verification_manifest_tests PRIVATE minikv)

target_compile_definitions(minikv_release_verification_manifest_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME release_verification_manifest_tests COMMAND minikv_release_verification_manifest_tests)

add_executable(minikv_runtime_artifact_rollback_evidence_tests
    tests/runtime_artifact_rollback_evidence_tests.cpp
)

target_link_libraries(minikv_runtime_artifact_rollback_evidence_tests PRIVATE minikv)

target_compile_definitions(minikv_runtime_artifact_rollback_evidence_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME runtime_artifact_rollback_evidence_tests COMMAND minikv_runtime_artifact_rollback_evidence_tests)

add_executable(minikv_runtime_artifact_bundle_manifest_tests
    tests/runtime_artifact_bundle_manifest_tests.cpp
)

target_link_libraries(minikv_runtime_artifact_bundle_manifest_tests PRIVATE minikv)

target_compile_definitions(minikv_runtime_artifact_bundle_manifest_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME runtime_artifact_bundle_manifest_tests COMMAND minikv_runtime_artifact_bundle_manifest_tests)

add_executable(minikv_restore_compatibility_handoff_tests
    tests/restore_compatibility_handoff_tests.cpp
)

target_link_libraries(minikv_restore_compatibility_handoff_tests PRIVATE minikv)

target_compile_definitions(minikv_restore_compatibility_handoff_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME restore_compatibility_handoff_tests COMMAND minikv_restore_compatibility_handoff_tests)

add_executable(minikv_restore_dry_run_operator_package_tests
    tests/restore_dry_run_operator_package_tests.cpp
)

target_link_libraries(minikv_restore_dry_run_operator_package_tests PRIVATE minikv)

target_compile_definitions(minikv_restore_dry_run_operator_package_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME restore_dry_run_operator_package_tests COMMAND minikv_restore_dry_run_operator_package_tests)

add_executable(minikv_artifact_digest_compatibility_matrix_tests
    tests/artifact_digest_compatibility_matrix_tests.cpp
)

target_link_libraries(minikv_artifact_digest_compatibility_matrix_tests PRIVATE minikv)

target_compile_definitions(minikv_artifact_digest_compatibility_matrix_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME artifact_digest_compatibility_matrix_tests COMMAND minikv_artifact_digest_compatibility_matrix_tests)

add_executable(minikv_release_artifact_digest_package_tests
    tests/release_artifact_digest_package_tests.cpp
)

target_link_libraries(minikv_release_artifact_digest_package_tests PRIVATE minikv)

target_compile_definitions(minikv_release_artifact_digest_package_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME release_artifact_digest_package_tests COMMAND minikv_release_artifact_digest_package_tests)

add_executable(minikv_restore_drill_evidence_tests
    tests/restore_drill_evidence_tests.cpp
)

target_link_libraries(minikv_restore_drill_evidence_tests PRIVATE minikv)

target_compile_definitions(minikv_restore_drill_evidence_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME restore_drill_evidence_tests COMMAND minikv_restore_drill_evidence_tests)

add_executable(minikv_restore_handoff_checklist_tests
    tests/restore_handoff_checklist_tests.cpp
)

target_link_libraries(minikv_restore_handoff_checklist_tests PRIVATE minikv)

target_compile_definitions(minikv_restore_handoff_checklist_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME restore_handoff_checklist_tests COMMAND minikv_restore_handoff_checklist_tests)

add_executable(minikv_restore_evidence_retention_tests
    tests/restore_evidence_retention_tests.cpp
)

target_link_libraries(minikv_restore_evidence_retention_tests PRIVATE minikv)

target_compile_definitions(minikv_restore_evidence_retention_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME restore_evidence_retention_tests COMMAND minikv_restore_evidence_retention_tests)

add_executable(minikv_retained_restore_artifact_digest_tests
    tests/retained_restore_artifact_digest_tests.cpp
)

target_link_libraries(minikv_retained_restore_artifact_digest_tests PRIVATE minikv)

target_compile_definitions(minikv_retained_restore_artifact_digest_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME retained_restore_artifact_digest_tests COMMAND minikv_retained_restore_artifact_digest_tests)

add_executable(minikv_restore_approval_boundary_tests
    tests/restore_approval_boundary_tests.cpp
)

target_link_libraries(minikv_restore_approval_boundary_tests PRIVATE minikv)

target_compile_definitions(minikv_restore_approval_boundary_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME restore_approval_boundary_tests COMMAND minikv_restore_approval_boundary_tests)

add_executable(minikv_restore_boundary_smoke_manifest_tests
    tests/restore_boundary_smoke_manifest_tests.cpp
)

target_link_libraries(minikv_restore_boundary_smoke_manifest_tests PRIVATE minikv)

target_compile_definitions(minikv_restore_boundary_smoke_manifest_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME restore_boundary_smoke_manifest_tests COMMAND minikv_restore_boundary_smoke_manifest_tests)

add_executable(minikv_runtime_smoke_evidence_tests
    tests/runtime_smoke_evidence_tests.cpp
)

target_link_libraries(minikv_runtime_smoke_evidence_tests PRIVATE minikv)

target_compile_definitions(minikv_runtime_smoke_evidence_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME runtime_smoke_evidence_tests COMMAND minikv_runtime_smoke_evidence_tests)

add_executable(minikv_current_runtime_fixture_rolling_guard_tests
    tests/current_runtime_fixture_rolling_guard_tests.cpp
)

target_link_libraries(minikv_current_runtime_fixture_rolling_guard_tests PRIVATE minikv)

target_compile_definitions(minikv_current_runtime_fixture_rolling_guard_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME current_runtime_fixture_rolling_guard_tests COMMAND minikv_current_runtime_fixture_rolling_guard_tests)

add_executable(minikv_runtime_no_start_no_write_follow_up_tests
    tests/runtime_no_start_no_write_follow_up_tests.cpp
)

target_link_libraries(minikv_runtime_no_start_no_write_follow_up_tests PRIVATE minikv)

target_compile_definitions(minikv_runtime_no_start_no_write_follow_up_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME runtime_no_start_no_write_follow_up_tests COMMAND minikv_runtime_no_start_no_write_follow_up_tests)

add_executable(minikv_operator_window_no_start_no_write_receipt_tests
    tests/operator_window_no_start_no_write_receipt_tests.cpp
)

target_link_libraries(minikv_operator_window_no_start_no_write_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_operator_window_no_start_no_write_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME operator_window_no_start_no_write_receipt_tests COMMAND minikv_operator_window_no_start_no_write_receipt_tests)

add_executable(minikv_manual_sandbox_dry_run_command_non_participation_receipt_tests
    tests/manual_sandbox_dry_run_command_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_manual_sandbox_dry_run_command_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_manual_sandbox_dry_run_command_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME manual_sandbox_dry_run_command_non_participation_receipt_tests COMMAND minikv_manual_sandbox_dry_run_command_non_participation_receipt_tests)

add_executable(minikv_manual_sandbox_connection_precheck_non_participation_receipt_tests
    tests/manual_sandbox_connection_precheck_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_manual_sandbox_connection_precheck_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_manual_sandbox_connection_precheck_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME manual_sandbox_connection_precheck_non_participation_receipt_tests COMMAND minikv_manual_sandbox_connection_precheck_non_participation_receipt_tests)

add_executable(minikv_disabled_adapter_client_non_participation_receipt_tests
    tests/disabled_adapter_client_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_disabled_adapter_client_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_disabled_adapter_client_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME disabled_adapter_client_non_participation_receipt_tests COMMAND minikv_disabled_adapter_client_non_participation_receipt_tests)

add_executable(minikv_fake_transport_dry_run_packet_non_participation_receipt_tests
    tests/fake_transport_dry_run_packet_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_fake_transport_dry_run_packet_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_fake_transport_dry_run_packet_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME fake_transport_dry_run_packet_non_participation_receipt_tests COMMAND minikv_fake_transport_dry_run_packet_non_participation_receipt_tests)

add_executable(minikv_sandbox_endpoint_handle_non_participation_receipt_tests
    tests/sandbox_endpoint_handle_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_sandbox_endpoint_handle_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_sandbox_endpoint_handle_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME sandbox_endpoint_handle_non_participation_receipt_tests COMMAND minikv_sandbox_endpoint_handle_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_non_participation_receipt_tests
    tests/credential_resolver_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_non_participation_receipt_tests COMMAND minikv_credential_resolver_non_participation_receipt_tests)

add_executable(minikv_disabled_credential_resolver_precheck_non_participation_receipt_tests
    tests/disabled_credential_resolver_precheck_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_disabled_credential_resolver_precheck_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_disabled_credential_resolver_precheck_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME disabled_credential_resolver_precheck_non_participation_receipt_tests COMMAND minikv_disabled_credential_resolver_precheck_non_participation_receipt_tests)

add_executable(minikv_test_only_resolver_shell_non_participation_receipt_tests
    tests/test_only_resolver_shell_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_test_only_resolver_shell_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_test_only_resolver_shell_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME test_only_resolver_shell_non_participation_receipt_tests COMMAND minikv_test_only_resolver_shell_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_fake_shell_archive_non_participation_receipt_tests
    tests/credential_resolver_fake_shell_archive_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_fake_shell_archive_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_fake_shell_archive_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_fake_shell_archive_non_participation_receipt_tests COMMAND minikv_credential_resolver_fake_shell_archive_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests
    tests/credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests COMMAND minikv_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests
    tests/credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests COMMAND minikv_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests
    tests/credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests COMMAND minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_approval_required_boundary_non_participation_receipt_tests
    tests/credential_resolver_approval_required_boundary_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_approval_required_boundary_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_approval_required_boundary_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_approval_required_boundary_non_participation_receipt_tests COMMAND minikv_credential_resolver_approval_required_boundary_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_impl_readiness_receipt_tests
    tests/credential_resolver_approval_required_implementation_readiness_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_impl_readiness_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_impl_readiness_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_approval_required_implementation_readiness_non_participation_receipt_tests COMMAND minikv_credential_resolver_impl_readiness_receipt_tests)

add_executable(minikv_credential_resolver_implementation_plan_non_participation_receipt_tests
    tests/credential_resolver_implementation_plan_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_implementation_plan_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_implementation_plan_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_implementation_plan_non_participation_receipt_tests COMMAND minikv_credential_resolver_implementation_plan_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_disabled_fake_harness_non_participation_receipt_tests
    tests/credential_resolver_disabled_fake_harness_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_disabled_fake_harness_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_disabled_fake_harness_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_disabled_fake_harness_non_participation_receipt_tests COMMAND minikv_credential_resolver_disabled_fake_harness_non_participation_receipt_tests)

add_executable(minikv_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests
    tests/credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests COMMAND minikv_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests)

add_executable(minikv_cred_res_exec_denied_retention_check_tests
    tests/credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_tests.cpp
)

target_link_libraries(minikv_cred_res_exec_denied_retention_check_tests PRIVATE minikv)

target_compile_definitions(minikv_cred_res_exec_denied_retention_check_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME cred_res_exec_denied_retention_check_tests COMMAND minikv_cred_res_exec_denied_retention_check_tests)

add_executable(minikv_disabled_runtime_shell_receipt_tests
    tests/credential_resolver_disabled_runtime_shell_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_disabled_runtime_shell_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_disabled_runtime_shell_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME disabled_runtime_shell_receipt_tests COMMAND minikv_disabled_runtime_shell_receipt_tests)

add_executable(minikv_disabled_runtime_shell_candidate_gate_receipt_tests
    tests/credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_disabled_runtime_shell_candidate_gate_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_disabled_runtime_shell_candidate_gate_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME disabled_runtime_shell_candidate_gate_receipt_tests COMMAND minikv_disabled_runtime_shell_candidate_gate_receipt_tests)

add_executable(minikv_runtime_shell_decision_record_receipt_tests
    tests/credential_resolver_runtime_shell_decision_record_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_runtime_shell_decision_record_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_runtime_shell_decision_record_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME runtime_shell_decision_record_receipt_tests COMMAND minikv_runtime_shell_decision_record_receipt_tests)

add_executable(minikv_runtime_shell_post_decision_plan_intake_receipt_tests
    tests/credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_runtime_shell_post_decision_plan_intake_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_runtime_shell_post_decision_plan_intake_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME runtime_shell_post_decision_plan_intake_receipt_tests COMMAND minikv_runtime_shell_post_decision_plan_intake_receipt_tests)

add_executable(minikv_runtime_shell_chain_stop_prerequisite_receipt_tests
    tests/credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_runtime_shell_chain_stop_prerequisite_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_runtime_shell_chain_stop_prerequisite_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME runtime_shell_chain_stop_prerequisite_receipt_tests COMMAND minikv_runtime_shell_chain_stop_prerequisite_receipt_tests)

add_executable(minikv_approval_prerequisite_artifact_intake_receipt_tests
    tests/credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_approval_prerequisite_artifact_intake_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_approval_prerequisite_artifact_intake_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME approval_prerequisite_artifact_intake_receipt_tests COMMAND minikv_approval_prerequisite_artifact_intake_receipt_tests)

add_executable(minikv_human_approval_artifact_review_receipt_tests
    tests/credential_resolver_human_approval_artifact_review_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_human_approval_artifact_review_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_human_approval_artifact_review_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME human_approval_artifact_review_receipt_tests COMMAND minikv_human_approval_artifact_review_receipt_tests)

add_executable(minikv_human_approval_post_echo_decision_receipt_tests
    tests/credential_resolver_human_approval_post_echo_decision_gate_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_human_approval_post_echo_decision_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_human_approval_post_echo_decision_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME human_approval_post_echo_decision_receipt_tests COMMAND minikv_human_approval_post_echo_decision_receipt_tests)

add_executable(minikv_signed_human_approval_artifact_contract_receipt_tests
    tests/credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_signed_human_approval_artifact_contract_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_signed_human_approval_artifact_contract_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME signed_human_approval_artifact_contract_receipt_tests COMMAND minikv_signed_human_approval_artifact_contract_receipt_tests)

add_executable(minikv_credential_handle_approval_contract_receipt_tests
    tests/credential_resolver_credential_handle_approval_contract_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_credential_handle_approval_contract_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_credential_handle_approval_contract_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME credential_handle_approval_contract_receipt_tests COMMAND minikv_credential_handle_approval_contract_receipt_tests)

add_executable(minikv_endpoint_handle_allowlist_approval_contract_receipt_tests
    tests/credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_endpoint_handle_allowlist_approval_contract_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_endpoint_handle_allowlist_approval_contract_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME endpoint_handle_allowlist_approval_contract_receipt_tests COMMAND minikv_endpoint_handle_allowlist_approval_contract_receipt_tests)

add_executable(minikv_no_network_safety_fixture_contract_receipt_tests
    tests/credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_no_network_safety_fixture_contract_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_no_network_safety_fixture_contract_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME no_network_safety_fixture_contract_receipt_tests COMMAND minikv_no_network_safety_fixture_contract_receipt_tests)

add_executable(minikv_abort_rollback_semantics_contract_receipt_tests
    tests/credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_abort_rollback_semantics_contract_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_abort_rollback_semantics_contract_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME abort_rollback_semantics_contract_receipt_tests COMMAND minikv_abort_rollback_semantics_contract_receipt_tests)

add_executable(minikv_input_hardening_candidate_gate_receipt_tests
    tests/credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_tests.cpp
)

target_link_libraries(minikv_input_hardening_candidate_gate_receipt_tests PRIVATE minikv)

target_compile_definitions(minikv_input_hardening_candidate_gate_receipt_tests
    PRIVATE
        MINIKV_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
)

add_test(NAME input_hardening_candidate_gate_receipt_tests COMMAND minikv_input_hardening_candidate_gate_receipt_tests)
