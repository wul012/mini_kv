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
    src/route_preview_evidence_boundary.cpp
    src/shard_readiness.cpp
    src/shard_readiness_approval_inputs.cpp
    src/shard_readiness_boundary_catalog_index.cpp
    src/shard_readiness_boundary_catalog_maintenance.cpp
    src/shard_readiness_boundary_catalog_groups.cpp
    src/shard_readiness_boundary_field_names.cpp
    src/shard_readiness_boundary_fields.cpp
    src/shard_readiness_history.cpp
    src/shard_readiness_lifecycle_sections.cpp
    src/shard_readiness_node_compatibility.cpp
    src/shard_readiness_route_catalog_cleanup_catalog.cpp
    src/shard_readiness_route_catalog_evidence_chain.cpp
    src/shard_readiness_route_catalog_post_closeout.cpp
    src/shard_readiness_route_catalog_post_closeout_stage.cpp
    src/shard_readiness_runtime_gate_sections.cpp
    src/shard_readiness_release_catalog.cpp
    src/shard_readiness_route_catalog_closeout.cpp
    src/shard_readiness_slot_preview.cpp
    src/shard_readiness_slot_preview_audit.cpp
    src/shard_readiness_summary_sections.cpp
    src/shard_route_preview.cpp
    src/shard_route_preview_archive_maintenance.cpp
    src/shard_route_preview_archive_maintenance_verification.cpp
    src/shard_route_preview_audit_closeout_archive_verification.cpp
    src/shard_route_preview_stage_chain.cpp
    src/shard_route_preview_operator_import_preflight.cpp
    src/shard_route_preview_worksheet_verification.cpp
    src/shard_route_preview_verification.cpp
    src/shard_route_preview_verification_report.cpp
    src/shard_route_preview_verification_report_archive.cpp
    src/shard_route_preview_verification_report_archive_closeout.cpp
    src/shard_route_preview_verification_report_archive_closeout_verification.cpp
    src/shard_route_preview_verification_report_archive_closeout_verification_audit.cpp
    src/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout.cpp
    src/shard_route_preview_verification_report_closeout.cpp
    src/shard_route_preview_stage_catalog.cpp
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

include(cmake/MinikvTesting.cmake)

minikv_add_standalone_source_dir_test(
    minikv_dependabot_config_tests
    dependabot_config_tests
    tests/dependabot_config_tests.cpp
)

minikv_add_linked_test(
    minikv_store_tests
    store_tests
    tests/store_tests.cpp
)

minikv_add_source_dir_test(
    minikv_command_tests
    command_tests
    tests/command_tests.cpp
)

minikv_add_source_dir_test(
    minikv_shard_readiness_tests
    shard_readiness_tests
    tests/shard_readiness_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_readiness_post_closeout_continuity_tests
    shard_readiness_post_closeout_continuity_tests
    tests/shard_readiness_post_closeout_continuity_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_tests
    shard_route_preview_tests
    tests/shard_route_preview_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_verification_tests
    shard_route_preview_verification_tests
    tests/shard_route_preview_verification_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_verification_report_tests
    shard_route_preview_verification_report_tests
    tests/shard_route_preview_verification_report_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_verification_report_closeout_tests
    shard_route_preview_verification_report_closeout_tests
    tests/shard_route_preview_verification_report_closeout_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_verification_report_archive_tests
    shard_route_preview_verification_report_archive_tests
    tests/shard_route_preview_verification_report_archive_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_verification_report_archive_closeout_tests
    shard_route_preview_verification_report_archive_closeout_tests
    tests/shard_route_preview_verification_report_archive_closeout_tests.cpp
)
minikv_add_linked_test(
    minikv_shard_route_preview_verification_report_archive_closeout_verification_tests
    shard_route_preview_verification_report_archive_closeout_verification_tests
    tests/shard_route_preview_verification_report_archive_closeout_verification_tests.cpp
)
minikv_add_linked_test(
    minikv_shard_route_preview_verification_report_archive_closeout_verification_audit_tests
    shard_route_preview_verification_report_archive_closeout_verification_audit_tests
    tests/shard_route_preview_verification_report_archive_closeout_verification_audit_tests.cpp
)
minikv_add_linked_test(
    minikv_shard_route_preview_audit_closeout_tests
    shard_route_preview_audit_closeout_tests
    tests/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout_tests.cpp
)
minikv_add_linked_test(
    minikv_shard_route_preview_audit_closeout_archive_verification_tests
    shard_route_preview_audit_closeout_archive_verification_tests
    tests/shard_route_preview_audit_closeout_archive_verification_tests.cpp
)
minikv_add_linked_test(
    minikv_shard_route_preview_archive_maintenance_tests
    shard_route_preview_archive_maintenance_tests
    tests/shard_route_preview_archive_maintenance_tests.cpp
)
minikv_add_linked_test(
    minikv_shard_route_preview_archive_maintenance_verification_tests
    shard_route_preview_archive_maintenance_verification_tests
    tests/shard_route_preview_archive_maintenance_verification_tests.cpp
)
minikv_add_linked_test(
    minikv_route_preview_evidence_boundary_tests
    route_preview_evidence_boundary_tests
    tests/route_preview_evidence_boundary_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_import_preflight_tests
    shard_route_preview_operator_import_preflight_tests
    tests/shard_route_preview_operator_import_preflight_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_worksheet_verification_tests
    shard_route_preview_worksheet_verification_tests
    tests/shard_route_preview_worksheet_verification_tests.cpp
)

minikv_add_linked_test(
    minikv_smokejson_command_receipt_tests
    smokejson_command_receipt_tests
    tests/smokejson_command_receipt_tests.cpp
)

minikv_add_linked_test(
    minikv_managed_audit_receipts_tests
    managed_audit_receipts_tests
    tests/managed_audit_receipts_tests.cpp
)

minikv_add_source_dir_test(
    minikv_readonly_fixture_tests
    readonly_fixture_tests
    tests/readonly_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_wal_tests
    wal_tests
    tests/wal_tests.cpp
)

minikv_add_linked_test(
    minikv_snapshot_tests
    snapshot_tests
    tests/snapshot_tests.cpp
)

minikv_add_linked_test(
    minikv_stress_tests
    stress_tests
    tests/stress_tests.cpp
)

minikv_add_linked_test(
    minikv_resp_tests
    resp_tests
    tests/resp_tests.cpp
)

minikv_add_linked_test(
    minikv_tcp_server_tests
    tcp_server_tests
    tests/tcp_server_tests.cpp
)

minikv_add_linked_test(
    minikv_tcp_resp_tests
    tcp_resp_tests
    tests/tcp_resp_tests.cpp
)

minikv_add_linked_test(
    minikv_tcp_resp_compat_tests
    tcp_resp_compat_tests
    tests/tcp_resp_compat_tests.cpp
)

minikv_add_linked_test(
    minikv_tcp_resp_concurrency_tests
    tcp_resp_concurrency_tests
    tests/tcp_resp_concurrency_tests.cpp
)

minikv_add_linked_test(
    minikv_client_history_tests
    client_history_tests
    tests/client_history_tests.cpp
)

minikv_add_linked_test(
    minikv_line_editor_tests
    line_editor_tests
    tests/line_editor_tests.cpp
)

minikv_add_linked_test(
    minikv_metrics_file_tests
    metrics_file_tests
    tests/metrics_file_tests.cpp
)

minikv_add_source_dir_test(
    minikv_recovery_evidence_tests
    recovery_evidence_tests
    tests/recovery_evidence_tests.cpp
)

minikv_add_source_dir_test(
    minikv_recovery_fixture_index_tests
    recovery_fixture_index_tests
    tests/recovery_fixture_index_tests.cpp
)

minikv_add_source_dir_test(
    minikv_ttl_token_recovery_tests
    ttl_token_recovery_tests
    tests/ttl_token_recovery_tests.cpp
)

minikv_add_source_dir_test(
    minikv_release_verification_manifest_tests
    release_verification_manifest_tests
    tests/release_verification_manifest_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_artifact_rollback_evidence_tests
    runtime_artifact_rollback_evidence_tests
    tests/runtime_artifact_rollback_evidence_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_artifact_bundle_manifest_tests
    runtime_artifact_bundle_manifest_tests
    tests/runtime_artifact_bundle_manifest_tests.cpp
)

minikv_add_source_dir_test(
    minikv_restore_compatibility_handoff_tests
    restore_compatibility_handoff_tests
    tests/restore_compatibility_handoff_tests.cpp
)

minikv_add_source_dir_test(
    minikv_restore_dry_run_operator_package_tests
    restore_dry_run_operator_package_tests
    tests/restore_dry_run_operator_package_tests.cpp
)

minikv_add_source_dir_test(
    minikv_artifact_digest_compatibility_matrix_tests
    artifact_digest_compatibility_matrix_tests
    tests/artifact_digest_compatibility_matrix_tests.cpp
)

minikv_add_source_dir_test(
    minikv_release_artifact_digest_package_tests
    release_artifact_digest_package_tests
    tests/release_artifact_digest_package_tests.cpp
)

minikv_add_source_dir_test(
    minikv_restore_drill_evidence_tests
    restore_drill_evidence_tests
    tests/restore_drill_evidence_tests.cpp
)

minikv_add_source_dir_test(
    minikv_restore_handoff_checklist_tests
    restore_handoff_checklist_tests
    tests/restore_handoff_checklist_tests.cpp
)

minikv_add_source_dir_test(
    minikv_restore_evidence_retention_tests
    restore_evidence_retention_tests
    tests/restore_evidence_retention_tests.cpp
)

minikv_add_source_dir_test(
    minikv_retained_restore_artifact_digest_tests
    retained_restore_artifact_digest_tests
    tests/retained_restore_artifact_digest_tests.cpp
)

minikv_add_source_dir_test(
    minikv_restore_approval_boundary_tests
    restore_approval_boundary_tests
    tests/restore_approval_boundary_tests.cpp
)

minikv_add_source_dir_test(
    minikv_restore_boundary_smoke_manifest_tests
    restore_boundary_smoke_manifest_tests
    tests/restore_boundary_smoke_manifest_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_smoke_evidence_tests
    runtime_smoke_evidence_tests
    tests/runtime_smoke_evidence_tests.cpp
)

minikv_add_source_dir_test(
    minikv_current_runtime_fixture_rolling_guard_tests
    current_runtime_fixture_rolling_guard_tests
    tests/current_runtime_fixture_rolling_guard_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_no_start_no_write_follow_up_tests
    runtime_no_start_no_write_follow_up_tests
    tests/runtime_no_start_no_write_follow_up_tests.cpp
)

minikv_add_source_dir_test(
    minikv_operator_window_no_start_no_write_receipt_tests
    operator_window_no_start_no_write_receipt_tests
    tests/operator_window_no_start_no_write_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_manual_sandbox_dry_run_command_non_participation_receipt_tests
    manual_sandbox_dry_run_command_non_participation_receipt_tests
    tests/manual_sandbox_dry_run_command_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_manual_sandbox_connection_precheck_non_participation_receipt_tests
    manual_sandbox_connection_precheck_non_participation_receipt_tests
    tests/manual_sandbox_connection_precheck_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_disabled_adapter_client_non_participation_receipt_tests
    disabled_adapter_client_non_participation_receipt_tests
    tests/disabled_adapter_client_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_fake_transport_dry_run_packet_non_participation_receipt_tests
    fake_transport_dry_run_packet_non_participation_receipt_tests
    tests/fake_transport_dry_run_packet_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_sandbox_endpoint_handle_non_participation_receipt_tests
    sandbox_endpoint_handle_non_participation_receipt_tests
    tests/sandbox_endpoint_handle_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_non_participation_receipt_tests
    credential_resolver_non_participation_receipt_tests
    tests/credential_resolver_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_disabled_credential_resolver_precheck_non_participation_receipt_tests
    disabled_credential_resolver_precheck_non_participation_receipt_tests
    tests/disabled_credential_resolver_precheck_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_test_only_resolver_shell_non_participation_receipt_tests
    test_only_resolver_shell_non_participation_receipt_tests
    tests/test_only_resolver_shell_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_fake_shell_archive_non_participation_receipt_tests
    credential_resolver_fake_shell_archive_non_participation_receipt_tests
    tests/credential_resolver_fake_shell_archive_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests
    credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests
    tests/credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests
    credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests
    tests/credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests
    credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests
    tests/credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_approval_required_boundary_non_participation_receipt_tests
    credential_resolver_approval_required_boundary_non_participation_receipt_tests
    tests/credential_resolver_approval_required_boundary_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_impl_readiness_receipt_tests
    credential_resolver_approval_required_implementation_readiness_non_participation_receipt_tests
    tests/credential_resolver_approval_required_implementation_readiness_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_implementation_plan_non_participation_receipt_tests
    credential_resolver_implementation_plan_non_participation_receipt_tests
    tests/credential_resolver_implementation_plan_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_disabled_fake_harness_non_participation_receipt_tests
    credential_resolver_disabled_fake_harness_non_participation_receipt_tests
    tests/credential_resolver_disabled_fake_harness_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests
    credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests
    tests/credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_cred_res_exec_denied_retention_check_tests
    cred_res_exec_denied_retention_check_tests
    tests/credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_tests.cpp
)

minikv_add_source_dir_test(
    minikv_disabled_runtime_shell_receipt_tests
    disabled_runtime_shell_receipt_tests
    tests/credential_resolver_disabled_runtime_shell_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_disabled_runtime_shell_candidate_gate_receipt_tests
    disabled_runtime_shell_candidate_gate_receipt_tests
    tests/credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_shell_decision_record_receipt_tests
    runtime_shell_decision_record_receipt_tests
    tests/credential_resolver_runtime_shell_decision_record_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_shell_post_decision_plan_intake_receipt_tests
    runtime_shell_post_decision_plan_intake_receipt_tests
    tests/credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_shell_chain_stop_prerequisite_receipt_tests
    runtime_shell_chain_stop_prerequisite_receipt_tests
    tests/credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_approval_prerequisite_artifact_intake_receipt_tests
    approval_prerequisite_artifact_intake_receipt_tests
    tests/credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_human_approval_artifact_review_receipt_tests
    human_approval_artifact_review_receipt_tests
    tests/credential_resolver_human_approval_artifact_review_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_human_approval_post_echo_decision_receipt_tests
    human_approval_post_echo_decision_receipt_tests
    tests/credential_resolver_human_approval_post_echo_decision_gate_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_signed_human_approval_artifact_contract_receipt_tests
    signed_human_approval_artifact_contract_receipt_tests
    tests/credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_credential_handle_approval_contract_receipt_tests
    credential_handle_approval_contract_receipt_tests
    tests/credential_resolver_credential_handle_approval_contract_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_endpoint_handle_allowlist_approval_contract_receipt_tests
    endpoint_handle_allowlist_approval_contract_receipt_tests
    tests/credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_no_network_safety_fixture_contract_receipt_tests
    no_network_safety_fixture_contract_receipt_tests
    tests/credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_abort_rollback_semantics_contract_receipt_tests
    abort_rollback_semantics_contract_receipt_tests
    tests/credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_tests.cpp
)

minikv_add_source_dir_test(
    minikv_input_hardening_candidate_gate_receipt_tests
    input_hardening_candidate_gate_receipt_tests
    tests/credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_tests.cpp
)
