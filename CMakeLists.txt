cmake_minimum_required(VERSION 3.20)

project(mini_kv VERSION 0.102.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(MINIKV_CURRENT_ARCHIVE_VERSION
    "c/102/"
    CACHE STRING "Archive path hint for the frozen current runtime fixture exposed in generated version.hpp"
)

option(MINIKV_SANITIZE "Enable AddressSanitizer and UndefinedBehaviorSanitizer for GCC/Clang Debug checks" OFF)
option(MINIKV_COVERAGE "Enable gcov-compatible coverage instrumentation for GCC/Clang Debug checks" OFF)

set(_minikv_bundle_tests_default ON)
if(MINIKV_SANITIZE OR MINIKV_COVERAGE)
    set(_minikv_bundle_tests_default OFF)
endif()
option(MINIKV_BUNDLE_TESTS "Link ordinary tests through stable shared runners" ${_minikv_bundle_tests_default})
unset(_minikv_bundle_tests_default)
set(MINIKV_TEST_SHARD_COUNT 8)

if(MINIKV_SANITIZE AND MINIKV_COVERAGE)
    message(FATAL_ERROR "MINIKV_SANITIZE and MINIKV_COVERAGE are separate CI lanes; enable only one at a time")
endif()

if((MINIKV_SANITIZE OR MINIKV_COVERAGE) AND MINIKV_BUNDLE_TESTS)
    message(FATAL_ERROR "instrumented lanes require MINIKV_BUNDLE_TESTS=OFF to preserve per-test gcov/sanitizer isolation")
endif()

if(MINIKV_SANITIZE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            message(FATAL_ERROR "MINIKV_SANITIZE requires ASan/UBSan runtime libraries; the bundled Windows MinGW toolchain does not provide -lasan/-lubsan. Use the Ubuntu sanitizer CI job.")
        endif()
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    else()
        message(FATAL_ERROR "MINIKV_SANITIZE requires a GCC or Clang compatible compiler")
    endif()
endif()

if(MINIKV_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            message(FATAL_ERROR "MINIKV_COVERAGE writes gcov sidecar files whose paths exceed the bundled Windows MinGW limit in this repository. Use the Ubuntu coverage CI job.")
        endif()
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    else()
        message(FATAL_ERROR "MINIKV_COVERAGE requires a GCC or Clang compatible compiler")
    endif()
endif()

configure_file(
    ${PROJECT_SOURCE_DIR}/include/minikv/version.hpp.in
    ${PROJECT_BINARY_DIR}/generated/include/minikv/version.hpp
    @ONLY
)

add_library(minikv
    src/atomic_file_writer.cpp
    src/client_history.cpp
    src/command.cpp
    src/command_catalog.cpp
    src/command_contracts.cpp
    src/command_dispatch.cpp
    src/command_evidence_dispatch.cpp
    src/command_expiry_ops.cpp
    src/command_introspection_ops.cpp
    src/command_parse_helpers.cpp
    src/command_persistence_ops.cpp
    src/command_response_formatters.cpp
    src/command_smoke_formatters.cpp
    src/command_string_ops.cpp
    src/command_wal_gate.cpp
    src/line_editor.cpp
    src/log.cpp
    src/osfs_command_processor.cpp
    src/osfs_directory.cpp
    src/osfs_disk_layout.cpp
    src/osfs_file_io.cpp
    src/osfs_fsck.cpp
    src/osfs_filesystem.cpp
    src/osfs_user_admin.cpp
    src/osfs_virtual_disk.cpp
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
    src/runtime_evidence_session.cpp
    src/runtime_receipt_json_builder.cpp
    src/runtime_sandbox_receipts.cpp
    src/recovery_evidence.cpp
    src/resp.cpp
    src/route_preview_evidence_boundary.cpp
    src/shard_readiness.cpp
    src/shard_readiness_approval_inputs.cpp
    src/shard_readiness_boundary_validators.cpp
    src/shard_readiness_boundary_catalog_index.cpp
    src/shard_readiness_boundary_catalog_maintenance.cpp
    src/shard_readiness_boundary_catalog_groups.cpp
    src/shard_readiness_boundary_field_names.cpp
    src/shard_readiness_boundary_fields.cpp
    src/shard_readiness_core_digest.cpp
    src/shard_readiness_history.cpp
    src/shard_readiness_json.cpp
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
    src/shard_route_preview_operator_value_draft.cpp
    src/shard_route_preview_operator_value_draft_slots.cpp
    src/shard_route_preview_operator_value_supply_approval_template.cpp
    src/shard_route_preview_operator_value_supply_approval_template_fields.cpp
    src/shard_route_preview_operator_value_supply_approval_template_slots.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_template.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_template_clauses.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_template_controls.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_template_fields.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_template_validation.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_stages.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_validation.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_stages.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_blockers.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_validation.cpp
    src/compared_package_intake_audit.cpp
    src/compared_package_audit_stages.cpp
    src/compared_package_audit_slots.cpp
    src/compared_package_audit_guards.cpp
    src/compared_package_audit_validation.cpp
    src/shard_preview_candidate_request_package.cpp
    src/shard_preview_candidate_request_package_stages.cpp
    src/shard_preview_candidate_request_package_items.cpp
    src/shard_preview_candidate_request_package_checks.cpp
    src/shard_preview_candidate_request_package_gates.cpp
    src/shard_preview_candidate_request_package_validation.cpp
    src/shard_preview_candidate_request_package_integrity.cpp
    src/shard_preview_candidate_request_package_integrity_stages.cpp
    src/shard_preview_candidate_request_package_integrity_checks.cpp
    src/shard_preview_candidate_request_package_integrity_validation.cpp
    src/shard_preview_candidate_submission_precheck.cpp
    src/shard_preview_candidate_submission_precheck_checkpoints.cpp
    src/shard_preview_candidate_submission_precheck_validators.cpp
    src/shard_preview_candidate_submission_precheck_validation.cpp
    src/shard_preview_candidate_intake_packet.cpp
    src/shard_preview_candidate_intake_packet_stages.cpp
    src/shard_preview_candidate_intake_packet_slots.cpp
    src/shard_preview_candidate_intake_packet_guards.cpp
    src/shard_preview_candidate_intake_packet_validation.cpp
    src/shard_preview_candidate_material_request.cpp
    src/shard_preview_candidate_material_request_stages.cpp
    src/shard_preview_candidate_material_request_items.cpp
    src/shard_preview_candidate_material_request_checks.cpp
    src/shard_preview_candidate_material_request_validation.cpp
    src/shard_preview_candidate_material_request_integrity.cpp
    src/shard_preview_candidate_material_request_integrity_stages.cpp
    src/shard_preview_candidate_material_request_integrity_checks.cpp
    src/shard_preview_candidate_material_request_integrity_validation.cpp
    src/shard_preview_candidate_material_submission_precheck.cpp
    src/shard_preview_candidate_material_submission_precheck_checkpoints.cpp
    src/shard_preview_candidate_material_submission_precheck_validators.cpp
    src/shard_preview_candidate_material_submission_precheck_validation.cpp
    src/shard_preview_candidate_material_submission_precheck_integrity.cpp
    src/shard_preview_candidate_material_submission_precheck_integrity_stages.cpp
    src/shard_preview_candidate_material_submission_precheck_integrity_checks.cpp
    src/shard_preview_candidate_material_submission_precheck_integrity_validation.cpp
    src/shard_preview_candidate_profile_section.cpp
    src/shard_preview_candidate_profile_section_stages.cpp
    src/shard_preview_candidate_profile_section_checks.cpp
    src/shard_preview_candidate_profile_section_validation.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section_stages.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section_checks.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section_validation.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section_integrity.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks.cpp
    src/shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation.cpp
    src/shard_preview_signed_approval_artifact_draft_text_package_profile_section.cpp
    src/shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages.cpp
    src/shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks.cpp
    src/shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation.cpp
    src/shard_preview_operator_value_supply_approval_profile_section.cpp
    src/shard_preview_operator_value_supply_approval_profile_section_stages.cpp
    src/shard_preview_operator_value_supply_approval_profile_section_checks.cpp
    src/shard_preview_operator_value_supply_approval_profile_section_validation.cpp
    src/shard_preview_operator_value_supply_profile_section.cpp
    src/shard_preview_operator_value_supply_profile_section_stages.cpp
    src/shard_preview_operator_value_supply_profile_section_checks.cpp
    src/shard_preview_operator_value_supply_profile_section_validation.cpp
    src/shard_preview_operator_value_supply_profile_section_integrity.cpp
    src/shard_preview_operator_value_supply_profile_section_integrity_stages.cpp
    src/shard_preview_operator_value_supply_profile_section_integrity_checks.cpp
    src/shard_preview_operator_value_supply_profile_section_integrity_validation.cpp
    src/shard_preview_production_live_capture_non_participation.cpp
    src/shard_preview_production_live_capture_non_participation_stages.cpp
    src/shard_preview_production_live_capture_non_participation_checks.cpp
    src/shard_preview_production_live_capture_non_participation_validation.cpp
    src/shard_preview_production_live_capture_archive_non_participation.cpp
    src/shard_preview_production_live_capture_archive_non_participation_stages.cpp
    src/shard_preview_production_live_capture_archive_non_participation_checks.cpp
    src/shard_preview_production_live_capture_archive_non_participation_validation.cpp
    src/shard_preview_production_live_capture_archive_verification_non_participation.cpp
    src/shard_preview_production_live_capture_archive_verification_non_participation_stages.cpp
    src/shard_preview_production_live_capture_archive_verification_non_participation_checks.cpp
    src/shard_preview_production_live_capture_archive_verification_non_participation_validation.cpp
    src/shard_preview_production_live_capture_release_evidence_review_non_participation.cpp
    src/shard_preview_production_live_capture_release_evidence_review_non_participation_stages.cpp
    src/shard_preview_production_live_capture_release_evidence_review_non_participation_checks.cpp
    src/shard_preview_production_live_capture_release_evidence_review_non_participation_validation.cpp
    src/shard_preview_catalog_entry_group_split_non_participation.cpp
    src/shard_preview_catalog_entry_group_split_non_participation_stages.cpp
    src/shard_preview_catalog_entry_group_split_non_participation_checks.cpp
    src/shard_preview_catalog_entry_group_split_non_participation_validation.cpp
    src/shard_preview_type_barrel_split_non_participation.cpp
    src/shard_preview_type_barrel_split_non_participation_stages.cpp
    src/shard_preview_type_barrel_split_non_participation_checks.cpp
    src/shard_preview_type_barrel_split_non_participation_validation.cpp
    src/shard_preview_type_barrel_split_follow_up_non_participation.cpp
    src/shard_preview_type_barrel_split_follow_up_non_participation_stages.cpp
    src/shard_preview_type_barrel_split_follow_up_non_participation_checks.cpp
    src/shard_preview_type_barrel_split_follow_up_non_participation_validation.cpp
    src/shard_preview_type_barrel_split_follow_up_fixture_audit.cpp
    src/shard_preview_type_barrel_split_follow_up_fixture_audit_stages.cpp
    src/shard_preview_type_barrel_split_follow_up_fixture_audit_checks.cpp
    src/shard_preview_type_barrel_split_follow_up_fixture_audit_validation.cpp
    src/shard_preview_disabled_precheck_upstream_echo_non_participation.cpp
    src/shard_preview_disabled_precheck_upstream_echo_non_participation_stages.cpp
    src/shard_preview_disabled_precheck_upstream_echo_non_participation_checks.cpp
    src/shard_preview_disabled_precheck_upstream_echo_non_participation_validation.cpp
    src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.cpp
    src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages.cpp
    src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks.cpp
    src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation.cpp
    src/shard_preview_implementation_plan_upstream_echo_closeout_non_participation.cpp
    src/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages.cpp
    src/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks.cpp
    src/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation.cpp
    src/shard_preview_release_window_readiness_packet_split_non_participation.cpp
    src/shard_preview_release_window_readiness_packet_split_non_participation_stages.cpp
    src/shard_preview_release_window_readiness_packet_split_non_participation_checks.cpp
    src/shard_preview_release_window_readiness_packet_split_non_participation_validation.cpp
    src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation.cpp
    src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages.cpp
    src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks.cpp
    src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation.cpp
    src/shard_preview_blocked_execution_rehearsal_split_non_participation.cpp
    src/shard_preview_blocked_execution_rehearsal_split_non_participation_stages.cpp
    src/shard_preview_blocked_execution_rehearsal_split_non_participation_checks.cpp
    src/shard_preview_blocked_execution_rehearsal_split_non_participation_validation.cpp
    src/shard_preview_precheck_upstream_receipt_verification_split_non_participation.cpp
    src/shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages.cpp
    src/shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks.cpp
    src/shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation.cpp
    src/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation.cpp
    src/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_stages.cpp
    src/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks.cpp
    src/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation.cpp
    src/shard_preview_code_walkthrough_quality_gate_non_participation.cpp
    src/shard_preview_code_walkthrough_quality_gate_non_participation_stages.cpp
    src/shard_preview_code_walkthrough_quality_gate_non_participation_checks.cpp
    src/shard_preview_code_walkthrough_quality_gate_non_participation_validation.cpp
    src/shard_preview_production_shard_execution_owner_receipt_request_packet.cpp
    src/shard_preview_production_shard_execution_owner_receipt_request_packet_stages.cpp
    src/shard_preview_production_shard_execution_owner_receipt_request_packet_checks.cpp
    src/shard_preview_production_shard_execution_owner_receipt_request_packet_validation.cpp
    src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.cpp
    src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages.cpp
    src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks.cpp
    src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation.cpp
    src/shard_preview_production_shard_execution_external_artifact_dry_run_closeout.cpp
    src/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_stages.cpp
    src/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_checks.cpp
    src/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation.cpp
    src/shard_preview_f_folder_explanation_quality_closeout.cpp
    src/shard_preview_f_folder_explanation_quality_closeout_rules.cpp
    src/shard_preview_f_folder_explanation_quality_closeout_checks.cpp
    src/shard_preview_f_folder_explanation_quality_closeout_validation.cpp
    src/shard_route_preview_operator_value_supply_envelope.cpp
    src/shard_route_preview_operator_value_supply_envelope_slots.cpp
    src/shard_route_preview_operator_value_supply_precheck.cpp
    src/shard_route_preview_operator_value_supply_precheck_requirements.cpp
    src/shard_route_preview_operator_value_supply_precheck_slots.cpp
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

add_executable(minikv_osfs
    src/osfs_main.cpp
)

target_link_libraries(minikv_osfs PRIVATE minikv)

if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(minikv_osfs PRIVATE -municode)
endif()

include(cmake/MinikvTesting.cmake)

enable_testing()
include(cmake/MinikvTestSuite.cmake)
