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

if(MINIKV_SANITIZE AND MINIKV_COVERAGE)
    message(FATAL_ERROR "MINIKV_SANITIZE and MINIKV_COVERAGE are separate CI lanes; enable only one at a time")
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
    src/client_history.cpp
    src/command.cpp
    src/command_catalog.cpp
    src/command_contracts.cpp
    src/command_dispatch.cpp
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
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards.cpp
    src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation.cpp
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
        MINIKV_NAME_FILE_MAX=740
        MINIKV_NAME_ID_MAX=883
)

minikv_add_linked_test(
    minikv_project_orientation_examples_tests
    project_orientation_examples_tests
    tests/project_orientation_examples_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_receipt_json_builder_tests
    runtime_receipt_json_builder_tests
    tests/runtime_receipt_json_builder_tests.cpp
)

target_include_directories(minikv_runtime_receipt_json_builder_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

minikv_add_source_dir_test(
    minikv_runtime_receipt_runtime_shell_family_baseline_tests
    runtime_receipt_runtime_shell_family_baseline_tests
    tests/runtime_receipt_runtime_shell_family_baseline_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_receipt_legacy_resolver_chain_baseline_tests
    runtime_receipt_legacy_resolver_chain_baseline_tests
    tests/runtime_receipt_legacy_resolver_chain_baseline_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_receipt_remaining_owner_baseline_tests
    runtime_receipt_remaining_owner_baseline_tests
    tests/runtime_receipt_remaining_owner_baseline_tests.cpp
)

minikv_add_linked_test(
    minikv_log_tests
    log_tests
    tests/log_tests.cpp
)

minikv_add_linked_test(
    minikv_osfs_tests
    osfs_tests
    tests/osfs_tests.cpp
)

add_test(
    NAME osfs_cli_smoke
    COMMAND ${CMAKE_COMMAND}
            -DOSFS_PATH=$<TARGET_FILE:minikv_osfs>
            -DDISK_PATH=${CMAKE_CURRENT_BINARY_DIR}/osfs-cli-smoke.img
            -DSCRIPT_PATH=${CMAKE_CURRENT_SOURCE_DIR}/tests/osfs_smoke_script.txt
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_osfs_smoke.cmake
)

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

add_test(NAME server_log_level_invalid_smoke COMMAND minikv_server --log-level trace)
set_tests_properties(server_log_level_invalid_smoke
    PROPERTIES
        WILL_FAIL TRUE
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

minikv_add_linked_test(
    minikv_command_catalog_tests
    command_catalog_tests
    tests/command_catalog_tests.cpp
)

minikv_add_source_dir_test(
    minikv_command_catalog_refactor_fixture_tests
    command_catalog_refactor_fixture_tests
    tests/command_catalog_refactor_fixture_tests.cpp
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
    minikv_shard_route_preview_operator_value_draft_slots_tests
    shard_route_preview_operator_value_draft_slots_tests
    tests/shard_route_preview_operator_value_draft_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_draft_tests
    shard_route_preview_operator_value_draft_tests
    tests/shard_route_preview_operator_value_draft_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_envelope_slots_tests
    shard_route_preview_operator_value_supply_envelope_slots_tests
    tests/shard_route_preview_operator_value_supply_envelope_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_envelope_tests
    shard_route_preview_operator_value_supply_envelope_tests
    tests/shard_route_preview_operator_value_supply_envelope_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_precheck_requirements_tests
    shard_route_preview_operator_value_supply_precheck_requirements_tests
    tests/shard_route_preview_operator_value_supply_precheck_requirements_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_precheck_slots_tests
    shard_route_preview_operator_value_supply_precheck_slots_tests
    tests/shard_route_preview_operator_value_supply_precheck_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_precheck_tests
    shard_route_preview_operator_value_supply_precheck_tests
    tests/shard_route_preview_operator_value_supply_precheck_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_approval_template_fields_tests
    shard_route_preview_operator_value_supply_approval_template_fields_tests
    tests/shard_route_preview_operator_value_supply_approval_template_fields_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_approval_template_slots_tests
    shard_route_preview_operator_value_supply_approval_template_slots_tests
    tests/shard_route_preview_operator_value_supply_approval_template_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_approval_template_tests
    shard_route_preview_operator_value_supply_approval_template_tests
    tests/shard_route_preview_operator_value_supply_approval_template_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_signed_approval_template_fields_tests
    shard_route_preview_operator_value_supply_signed_approval_template_fields_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_template_fields_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_signed_approval_template_clauses_tests
    shard_route_preview_operator_value_supply_signed_approval_template_clauses_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_template_clauses_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_signed_approval_template_controls_tests
    shard_route_preview_operator_value_supply_signed_approval_template_controls_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_template_controls_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_signed_approval_template_validation_tests
    shard_route_preview_operator_value_supply_signed_approval_template_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_template_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_shard_route_preview_operator_value_supply_signed_approval_template_tests
    shard_route_preview_operator_value_supply_signed_approval_template_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_template_tests.cpp
)

minikv_add_linked_test(
    minikv_capture_preflight_inputs_tests
    capture_preflight_inputs_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs_tests.cpp
)

minikv_add_linked_test(
    minikv_capture_preflight_policies_tests
    capture_preflight_policies_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies_tests.cpp
)

minikv_add_linked_test(
    minikv_capture_preflight_attestations_tests
    capture_preflight_attestations_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations_tests.cpp
)

minikv_add_linked_test(
    minikv_capture_preflight_validation_tests
    capture_preflight_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_capture_preflight_tests
    capture_preflight_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_tests.cpp
)

minikv_add_linked_test(
    minikv_artifact_preflight_fragments_tests
    artifact_preflight_fragments_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments_tests.cpp
)

minikv_add_linked_test(
    minikv_artifact_preflight_gates_tests
    artifact_preflight_gates_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates_tests.cpp
)

minikv_add_linked_test(
    minikv_artifact_preflight_seals_tests
    artifact_preflight_seals_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals_tests.cpp
)

minikv_add_linked_test(
    minikv_artifact_preflight_stages_tests
    artifact_preflight_stages_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_artifact_preflight_validation_tests
    artifact_preflight_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_artifact_preflight_tests
    artifact_preflight_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_tests.cpp
)

minikv_add_linked_test(
    minikv_authoring_readiness_stages_tests
    authoring_readiness_stages_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_authoring_readiness_requirements_tests
    authoring_readiness_requirements_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements_tests.cpp
)

minikv_add_linked_test(
    minikv_authoring_readiness_blockers_tests
    authoring_readiness_blockers_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers_tests.cpp
)

minikv_add_linked_test(
    minikv_authoring_readiness_profiles_tests
    authoring_readiness_profiles_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles_tests.cpp
)

minikv_add_linked_test(
    minikv_authoring_readiness_validation_tests
    authoring_readiness_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_authoring_readiness_tests
    authoring_readiness_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_tests.cpp
)

minikv_add_linked_test(
    minikv_instruction_preflight_stages_tests
    instruction_preflight_stages_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_instruction_preflight_slots_tests
    instruction_preflight_slots_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_instruction_preflight_guards_tests
    instruction_preflight_guards_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards_tests.cpp
)

minikv_add_linked_test(
    minikv_instruction_preflight_profiles_tests
    instruction_preflight_profiles_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles_tests.cpp
)

minikv_add_linked_test(
    minikv_instruction_preflight_validation_tests
    instruction_preflight_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_instruction_preflight_tests
    instruction_preflight_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_preflight_stages_tests
    text_package_review_preflight_stages_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_preflight_slots_tests
    text_package_review_preflight_slots_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_preflight_guards_tests
    text_package_review_preflight_guards_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_preflight_profiles_tests
    text_package_review_preflight_profiles_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_preflight_validation_tests
    text_package_review_preflight_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_preflight_tests
    text_package_review_preflight_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_closeout_audit_stages_tests
    text_package_review_closeout_audit_stages_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_closeout_audit_checks_tests
    text_package_review_closeout_audit_checks_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_closeout_audit_blockers_tests
    text_package_review_closeout_audit_blockers_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_closeout_audit_profiles_tests
    text_package_review_closeout_audit_profiles_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_profiles_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_closeout_audit_validation_tests
    text_package_review_closeout_audit_validation_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_text_package_review_closeout_audit_tests
    text_package_review_closeout_audit_tests
    tests/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_tests.cpp
)

minikv_add_linked_test(
    minikv_tp_comp_audit_stages_tests
    tp_comp_audit_stages_tests
    tests/text_package_comparison_closeout_audit_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_tp_comp_audit_checks_tests
    tp_comp_audit_checks_tests
    tests/text_package_comparison_closeout_audit_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_tp_comp_audit_blockers_tests
    tp_comp_audit_blockers_tests
    tests/text_package_comparison_closeout_audit_blockers_tests.cpp
)

minikv_add_linked_test(
    minikv_tp_comp_audit_profiles_tests
    tp_comp_audit_profiles_tests
    tests/text_package_comparison_closeout_audit_profiles_tests.cpp
)

minikv_add_linked_test(
    minikv_tp_comp_audit_validation_tests
    tp_comp_audit_validation_tests
    tests/text_package_comparison_closeout_audit_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_tp_comp_audit_tests
    tp_comp_audit_tests
    tests/text_package_comparison_closeout_audit_tests.cpp
)

minikv_add_linked_test(
    minikv_comp_pkg_intake_stages_tests
    comp_pkg_intake_stages_tests
    tests/compared_package_evidence_intake_audit_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_comp_pkg_intake_slots_tests
    comp_pkg_intake_slots_tests
    tests/compared_package_evidence_intake_audit_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_comp_pkg_intake_guards_tests
    comp_pkg_intake_guards_tests
    tests/compared_package_evidence_intake_audit_guards_tests.cpp
)

minikv_add_linked_test(
    minikv_comp_pkg_intake_validation_tests
    comp_pkg_intake_validation_tests
    tests/compared_package_evidence_intake_audit_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_comp_pkg_intake_tests
    comp_pkg_intake_tests
    tests/compared_package_evidence_intake_audit_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_stages_tests
    candidate_request_package_stages_tests
    tests/candidate_request_package_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_items_tests
    candidate_request_package_items_tests
    tests/candidate_request_package_items_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_checks_tests
    candidate_request_package_checks_tests
    tests/candidate_request_package_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_gates_tests
    candidate_request_package_gates_tests
    tests/candidate_request_package_gates_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_validation_tests
    candidate_request_package_validation_tests
    tests/candidate_request_package_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_tests
    candidate_request_package_tests
    tests/candidate_request_package_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_integrity_stages_tests
    candidate_request_package_integrity_stages_tests
    tests/candidate_request_package_integrity_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_integrity_checks_tests
    candidate_request_package_integrity_checks_tests
    tests/candidate_request_package_integrity_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_integrity_validation_tests
    candidate_request_package_integrity_validation_tests
    tests/candidate_request_package_integrity_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_request_package_integrity_tests
    candidate_request_package_integrity_tests
    tests/candidate_request_package_integrity_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_request_package_integrity_fixture_tests
    candidate_request_package_integrity_fixture_tests
    tests/candidate_request_package_integrity_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_submission_precheck_checkpoints_tests
    candidate_submission_precheck_checkpoints_tests
    tests/candidate_submission_precheck_checkpoints_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_submission_precheck_validators_tests
    candidate_submission_precheck_validators_tests
    tests/candidate_submission_precheck_validators_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_submission_precheck_validation_tests
    candidate_submission_precheck_validation_tests
    tests/candidate_submission_precheck_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_submission_precheck_tests
    candidate_submission_precheck_tests
    tests/candidate_submission_precheck_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_submission_precheck_fixture_tests
    candidate_submission_precheck_fixture_tests
    tests/candidate_submission_precheck_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_intake_packet_stages_tests
    candidate_intake_packet_stages_tests
    tests/candidate_intake_packet_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_intake_packet_slots_tests
    candidate_intake_packet_slots_tests
    tests/candidate_intake_packet_slots_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_intake_packet_guards_tests
    candidate_intake_packet_guards_tests
    tests/candidate_intake_packet_guards_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_intake_packet_validation_tests
    candidate_intake_packet_validation_tests
    tests/candidate_intake_packet_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_intake_packet_tests
    candidate_intake_packet_tests
    tests/candidate_intake_packet_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_intake_packet_fixture_tests
    candidate_intake_packet_fixture_tests
    tests/candidate_intake_packet_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_stages_tests
    candidate_material_request_stages_tests
    tests/candidate_material_request_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_items_tests
    candidate_material_request_items_tests
    tests/candidate_material_request_items_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_checks_tests
    candidate_material_request_checks_tests
    tests/candidate_material_request_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_validation_tests
    candidate_material_request_validation_tests
    tests/candidate_material_request_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_tests
    candidate_material_request_tests
    tests/candidate_material_request_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_material_request_fixture_tests
    candidate_material_request_fixture_tests
    tests/candidate_material_request_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_integrity_stages_tests
    candidate_material_request_integrity_stages_tests
    tests/candidate_material_request_integrity_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_integrity_checks_tests
    candidate_material_request_integrity_checks_tests
    tests/candidate_material_request_integrity_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_integrity_validation_tests
    candidate_material_request_integrity_validation_tests
    tests/candidate_material_request_integrity_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_request_integrity_tests
    candidate_material_request_integrity_tests
    tests/candidate_material_request_integrity_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_material_request_integrity_fixture_tests
    candidate_material_request_integrity_fixture_tests
    tests/candidate_material_request_integrity_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_checkpoints_tests
    candidate_material_submission_precheck_checkpoints_tests
    tests/candidate_material_submission_precheck_checkpoints_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_validators_tests
    candidate_material_submission_precheck_validators_tests
    tests/candidate_material_submission_precheck_validators_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_validation_tests
    candidate_material_submission_precheck_validation_tests
    tests/candidate_material_submission_precheck_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_tests
    candidate_material_submission_precheck_tests
    tests/candidate_material_submission_precheck_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_material_submission_precheck_fixture_tests
    candidate_material_submission_precheck_fixture_tests
    tests/candidate_material_submission_precheck_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_integrity_stages_tests
    candidate_material_submission_precheck_integrity_stages_tests
    tests/candidate_material_submission_precheck_integrity_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_integrity_checks_tests
    candidate_material_submission_precheck_integrity_checks_tests
    tests/candidate_material_submission_precheck_integrity_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_integrity_validation_tests
    candidate_material_submission_precheck_integrity_validation_tests
    tests/candidate_material_submission_precheck_integrity_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_material_submission_precheck_integrity_tests
    candidate_material_submission_precheck_integrity_tests
    tests/candidate_material_submission_precheck_integrity_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_material_submission_precheck_integrity_fixture_tests
    candidate_material_submission_precheck_integrity_fixture_tests
    tests/candidate_material_submission_precheck_integrity_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_profile_section_stages_tests
    candidate_profile_section_stages_tests
    tests/candidate_profile_section_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_profile_section_checks_tests
    candidate_profile_section_checks_tests
    tests/candidate_profile_section_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_profile_section_validation_tests
    candidate_profile_section_validation_tests
    tests/candidate_profile_section_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_candidate_profile_section_tests
    candidate_profile_section_tests
    tests/candidate_profile_section_tests.cpp
)

minikv_add_source_dir_test(
    minikv_candidate_profile_section_fixture_tests
    candidate_profile_section_fixture_tests
    tests/candidate_profile_section_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_stages_tests
    signed_approval_artifact_draft_profile_section_stages_tests
    tests/signed_approval_artifact_draft_profile_section_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_checks_tests
    signed_approval_artifact_draft_profile_section_checks_tests
    tests/signed_approval_artifact_draft_profile_section_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_validation_tests
    signed_approval_artifact_draft_profile_section_validation_tests
    tests/signed_approval_artifact_draft_profile_section_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_tests
    signed_approval_artifact_draft_profile_section_tests
    tests/signed_approval_artifact_draft_profile_section_tests.cpp
)

minikv_add_source_dir_test(
    minikv_signed_approval_artifact_draft_profile_section_fixture_tests
    signed_approval_artifact_draft_profile_section_fixture_tests
    tests/signed_approval_artifact_draft_profile_section_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_integrity_stages_tests
    signed_approval_artifact_draft_profile_section_integrity_stages_tests
    tests/signed_approval_artifact_draft_profile_section_integrity_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_integrity_checks_tests
    signed_approval_artifact_draft_profile_section_integrity_checks_tests
    tests/signed_approval_artifact_draft_profile_section_integrity_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_integrity_validation_tests
    signed_approval_artifact_draft_profile_section_integrity_validation_tests
    tests/signed_approval_artifact_draft_profile_section_integrity_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_profile_section_integrity_tests
    signed_approval_artifact_draft_profile_section_integrity_tests
    tests/signed_approval_artifact_draft_profile_section_integrity_tests.cpp
)

minikv_add_source_dir_test(
    minikv_signed_approval_artifact_draft_profile_section_integrity_fixture_tests
    signed_approval_artifact_draft_profile_section_integrity_fixture_tests
    tests/signed_approval_artifact_draft_profile_section_integrity_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_text_package_profile_section_stages_tests
    signed_approval_artifact_draft_text_package_profile_section_stages_tests
    tests/signed_approval_artifact_draft_text_package_profile_section_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_text_package_profile_section_checks_tests
    signed_approval_artifact_draft_text_package_profile_section_checks_tests
    tests/signed_approval_artifact_draft_text_package_profile_section_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_text_package_profile_section_validation_tests
    signed_approval_artifact_draft_text_package_profile_section_validation_tests
    tests/signed_approval_artifact_draft_text_package_profile_section_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_signed_approval_artifact_draft_text_package_profile_section_tests
    signed_approval_artifact_draft_text_package_profile_section_tests
    tests/signed_approval_artifact_draft_text_package_profile_section_tests.cpp
)

minikv_add_source_dir_test(
    minikv_signed_approval_artifact_draft_text_package_profile_section_fixture_tests
    signed_approval_artifact_draft_text_package_profile_section_fixture_tests
    tests/signed_approval_artifact_draft_text_package_profile_section_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_approval_profile_section_stages_tests
    operator_value_supply_approval_profile_section_stages_tests
    tests/operator_value_supply_approval_profile_section_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_approval_profile_section_checks_tests
    operator_value_supply_approval_profile_section_checks_tests
    tests/operator_value_supply_approval_profile_section_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_approval_profile_section_validation_tests
    operator_value_supply_approval_profile_section_validation_tests
    tests/operator_value_supply_approval_profile_section_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_approval_profile_section_tests
    operator_value_supply_approval_profile_section_tests
    tests/operator_value_supply_approval_profile_section_tests.cpp
)

minikv_add_source_dir_test(
    minikv_operator_value_supply_approval_profile_section_fixture_tests
    operator_value_supply_approval_profile_section_fixture_tests
    tests/operator_value_supply_approval_profile_section_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_stages_tests
    operator_value_supply_profile_section_stages_tests
    tests/operator_value_supply_profile_section_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_checks_tests
    operator_value_supply_profile_section_checks_tests
    tests/operator_value_supply_profile_section_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_validation_tests
    operator_value_supply_profile_section_validation_tests
    tests/operator_value_supply_profile_section_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_tests
    operator_value_supply_profile_section_tests
    tests/operator_value_supply_profile_section_tests.cpp
)

minikv_add_source_dir_test(
    minikv_operator_value_supply_profile_section_fixture_tests
    operator_value_supply_profile_section_fixture_tests
    tests/operator_value_supply_profile_section_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_integrity_stages_tests
    operator_value_supply_profile_section_integrity_stages_tests
    tests/operator_value_supply_profile_section_integrity_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_integrity_checks_tests
    operator_value_supply_profile_section_integrity_checks_tests
    tests/operator_value_supply_profile_section_integrity_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_integrity_validation_tests
    operator_value_supply_profile_section_integrity_validation_tests
    tests/operator_value_supply_profile_section_integrity_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_operator_value_supply_profile_section_integrity_tests
    operator_value_supply_profile_section_integrity_tests
    tests/operator_value_supply_profile_section_integrity_tests.cpp
)

minikv_add_source_dir_test(
    minikv_operator_value_supply_profile_section_integrity_fixture_tests
    operator_value_supply_profile_section_integrity_fixture_tests
    tests/operator_value_supply_profile_section_integrity_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_non_participation_stages_tests
    production_live_capture_non_participation_stages_tests
    tests/production_live_capture_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_non_participation_checks_tests
    production_live_capture_non_participation_checks_tests
    tests/production_live_capture_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_non_participation_validation_tests
    production_live_capture_non_participation_validation_tests
    tests/production_live_capture_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_non_participation_tests
    production_live_capture_non_participation_tests
    tests/production_live_capture_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_production_live_capture_non_participation_fixture_tests
    production_live_capture_non_participation_fixture_tests
    tests/production_live_capture_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_non_participation_stages_tests
    production_live_capture_archive_non_participation_stages_tests
    tests/production_live_capture_archive_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_non_participation_checks_tests
    production_live_capture_archive_non_participation_checks_tests
    tests/production_live_capture_archive_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_non_participation_validation_tests
    production_live_capture_archive_non_participation_validation_tests
    tests/production_live_capture_archive_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_non_participation_tests
    production_live_capture_archive_non_participation_tests
    tests/production_live_capture_archive_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_production_live_capture_archive_non_participation_fixture_tests
    production_live_capture_archive_non_participation_fixture_tests
    tests/production_live_capture_archive_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_verification_non_participation_stages_tests
    production_live_capture_archive_verification_non_participation_stages_tests
    tests/production_live_capture_archive_verification_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_verification_non_participation_checks_tests
    production_live_capture_archive_verification_non_participation_checks_tests
    tests/production_live_capture_archive_verification_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_verification_non_participation_validation_tests
    production_live_capture_archive_verification_non_participation_validation_tests
    tests/production_live_capture_archive_verification_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_archive_verification_non_participation_tests
    production_live_capture_archive_verification_non_participation_tests
    tests/production_live_capture_archive_verification_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_production_live_capture_archive_verification_non_participation_fixture_tests
    production_live_capture_archive_verification_non_participation_fixture_tests
    tests/production_live_capture_archive_verification_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_release_evidence_review_non_participation_stages_tests
    production_live_capture_release_evidence_review_non_participation_stages_tests
    tests/production_live_capture_release_evidence_review_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_release_evidence_review_non_participation_checks_tests
    production_live_capture_release_evidence_review_non_participation_checks_tests
    tests/production_live_capture_release_evidence_review_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_release_evidence_review_non_participation_validation_tests
    production_live_capture_release_evidence_review_non_participation_validation_tests
    tests/production_live_capture_release_evidence_review_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_production_live_capture_release_evidence_review_non_participation_tests
    production_live_capture_release_evidence_review_non_participation_tests
    tests/production_live_capture_release_evidence_review_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_production_live_capture_release_evidence_review_non_participation_fixture_tests
    production_live_capture_release_evidence_review_non_participation_fixture_tests
    tests/production_live_capture_release_evidence_review_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_catalog_entry_group_split_non_participation_stages_tests
    catalog_entry_group_split_non_participation_stages_tests
    tests/catalog_entry_group_split_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_catalog_entry_group_split_non_participation_checks_tests
    catalog_entry_group_split_non_participation_checks_tests
    tests/catalog_entry_group_split_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_catalog_entry_group_split_non_participation_validation_tests
    catalog_entry_group_split_non_participation_validation_tests
    tests/catalog_entry_group_split_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_catalog_entry_group_split_non_participation_tests
    catalog_entry_group_split_non_participation_tests
    tests/catalog_entry_group_split_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_catalog_entry_group_split_non_participation_fixture_tests
    catalog_entry_group_split_non_participation_fixture_tests
    tests/catalog_entry_group_split_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_non_participation_stages_tests
    type_barrel_split_non_participation_stages_tests
    tests/type_barrel_split_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_non_participation_checks_tests
    type_barrel_split_non_participation_checks_tests
    tests/type_barrel_split_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_non_participation_validation_tests
    type_barrel_split_non_participation_validation_tests
    tests/type_barrel_split_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_non_participation_tests
    type_barrel_split_non_participation_tests
    tests/type_barrel_split_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_type_barrel_split_non_participation_fixture_tests
    type_barrel_split_non_participation_fixture_tests
    tests/type_barrel_split_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_non_participation_stages_tests
    type_barrel_split_follow_up_non_participation_stages_tests
    tests/type_barrel_split_follow_up_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_non_participation_checks_tests
    type_barrel_split_follow_up_non_participation_checks_tests
    tests/type_barrel_split_follow_up_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_non_participation_validation_tests
    type_barrel_split_follow_up_non_participation_validation_tests
    tests/type_barrel_split_follow_up_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_non_participation_tests
    type_barrel_split_follow_up_non_participation_tests
    tests/type_barrel_split_follow_up_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_type_barrel_split_follow_up_non_participation_fixture_tests
    type_barrel_split_follow_up_non_participation_fixture_tests
    tests/type_barrel_split_follow_up_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_fixture_audit_stages_tests
    type_barrel_split_follow_up_fixture_audit_stages_tests
    tests/type_barrel_split_follow_up_fixture_audit_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_fixture_audit_checks_tests
    type_barrel_split_follow_up_fixture_audit_checks_tests
    tests/type_barrel_split_follow_up_fixture_audit_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_fixture_audit_validation_tests
    type_barrel_split_follow_up_fixture_audit_validation_tests
    tests/type_barrel_split_follow_up_fixture_audit_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_type_barrel_split_follow_up_fixture_audit_tests
    type_barrel_split_follow_up_fixture_audit_tests
    tests/type_barrel_split_follow_up_fixture_audit_tests.cpp
)

minikv_add_source_dir_test(
    minikv_type_barrel_split_follow_up_fixture_audit_fixture_tests
    type_barrel_split_follow_up_fixture_audit_fixture_tests
    tests/type_barrel_split_follow_up_fixture_audit_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_disabled_precheck_upstream_echo_non_participation_stages_tests
    disabled_precheck_upstream_echo_non_participation_stages_tests
    tests/disabled_precheck_upstream_echo_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_disabled_precheck_upstream_echo_non_participation_checks_tests
    disabled_precheck_upstream_echo_non_participation_checks_tests
    tests/disabled_precheck_upstream_echo_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_disabled_precheck_upstream_echo_non_participation_validation_tests
    disabled_precheck_upstream_echo_non_participation_validation_tests
    tests/disabled_precheck_upstream_echo_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_disabled_precheck_upstream_echo_non_participation_tests
    disabled_precheck_upstream_echo_non_participation_tests
    tests/disabled_precheck_upstream_echo_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_disabled_precheck_upstream_echo_non_participation_fixture_tests
    disabled_precheck_upstream_echo_non_participation_fixture_tests
    tests/disabled_precheck_upstream_echo_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages_tests
    sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages_tests
    tests/sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_tests
    sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_tests
    tests/sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_tests
    sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_tests
    tests/sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_tests
    sandbox_endpoint_credential_resolver_upstream_echo_non_participation_tests
    tests/sandbox_endpoint_credential_resolver_upstream_echo_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_fixture_tests
    sandbox_endpoint_credential_resolver_upstream_echo_non_participation_fixture_tests
    tests/sandbox_endpoint_credential_resolver_upstream_echo_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_implementation_plan_upstream_echo_closeout_non_participation_stages_tests
    implementation_plan_upstream_echo_closeout_non_participation_stages_tests
    tests/implementation_plan_upstream_echo_closeout_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_implementation_plan_upstream_echo_closeout_non_participation_checks_tests
    implementation_plan_upstream_echo_closeout_non_participation_checks_tests
    tests/implementation_plan_upstream_echo_closeout_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_implementation_plan_upstream_echo_closeout_non_participation_validation_tests
    implementation_plan_upstream_echo_closeout_non_participation_validation_tests
    tests/implementation_plan_upstream_echo_closeout_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_implementation_plan_upstream_echo_closeout_non_participation_tests
    implementation_plan_upstream_echo_closeout_non_participation_tests
    tests/implementation_plan_upstream_echo_closeout_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_implementation_plan_upstream_echo_closeout_non_participation_fixture_tests
    implementation_plan_upstream_echo_closeout_non_participation_fixture_tests
    tests/implementation_plan_upstream_echo_closeout_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_release_window_readiness_packet_split_non_participation_stages_tests
    release_window_readiness_packet_split_non_participation_stages_tests
    tests/release_window_readiness_packet_split_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_release_window_readiness_packet_split_non_participation_checks_tests
    release_window_readiness_packet_split_non_participation_checks_tests
    tests/release_window_readiness_packet_split_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_release_window_readiness_packet_split_non_participation_validation_tests
    release_window_readiness_packet_split_non_participation_validation_tests
    tests/release_window_readiness_packet_split_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_release_window_readiness_packet_split_non_participation_tests
    release_window_readiness_packet_split_non_participation_tests
    tests/release_window_readiness_packet_split_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_release_window_readiness_packet_split_non_participation_fixture_tests
    release_window_readiness_packet_split_non_participation_fixture_tests
    tests/release_window_readiness_packet_split_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_dfh_echo_split_np_stages_tests
    dfh_echo_split_np_stages_tests
    tests/disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_dfh_echo_split_np_checks_tests
    dfh_echo_split_np_checks_tests
    tests/disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_dfh_echo_split_np_validation_tests
    dfh_echo_split_np_validation_tests
    tests/disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_dfh_echo_split_np_tests
    dfh_echo_split_np_tests
    tests/disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_dfh_echo_split_np_fixture_tests
    dfh_echo_split_np_fixture_tests
    tests/disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_blocked_rehearsal_split_np_stages_tests
    blocked_rehearsal_split_np_stages_tests
    tests/blocked_execution_rehearsal_split_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_blocked_rehearsal_split_np_checks_tests
    blocked_rehearsal_split_np_checks_tests
    tests/blocked_execution_rehearsal_split_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_blocked_rehearsal_split_np_validation_tests
    blocked_rehearsal_split_np_validation_tests
    tests/blocked_execution_rehearsal_split_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_blocked_rehearsal_split_np_tests
    blocked_rehearsal_split_np_tests
    tests/blocked_execution_rehearsal_split_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_blocked_rehearsal_split_np_fixture_tests
    blocked_rehearsal_split_np_fixture_tests
    tests/blocked_execution_rehearsal_split_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_precheck_receipt_split_np_stages_tests
    precheck_receipt_split_np_stages_tests
    tests/precheck_upstream_receipt_verification_split_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_precheck_receipt_split_np_checks_tests
    precheck_receipt_split_np_checks_tests
    tests/precheck_upstream_receipt_verification_split_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_precheck_receipt_split_np_validation_tests
    precheck_receipt_split_np_validation_tests
    tests/precheck_upstream_receipt_verification_split_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_precheck_receipt_split_np_tests
    precheck_receipt_split_np_tests
    tests/precheck_upstream_receipt_verification_split_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_precheck_receipt_split_np_fixture_tests
    precheck_receipt_split_np_fixture_tests
    tests/precheck_upstream_receipt_verification_split_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_runtime_approval_gate_archive_np_stages_tests
    runtime_approval_gate_archive_np_stages_tests
    tests/runtime_execution_packet_approval_gate_archive_verification_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_runtime_approval_gate_archive_np_checks_tests
    runtime_approval_gate_archive_np_checks_tests
    tests/runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_runtime_approval_gate_archive_np_validation_tests
    runtime_approval_gate_archive_np_validation_tests
    tests/runtime_execution_packet_approval_gate_archive_verification_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_runtime_approval_gate_archive_np_tests
    runtime_approval_gate_archive_np_tests
    tests/runtime_execution_packet_approval_gate_archive_verification_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_runtime_approval_gate_archive_np_fixture_tests
    runtime_approval_gate_archive_np_fixture_tests
    tests/runtime_execution_packet_approval_gate_archive_verification_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_code_walkthrough_quality_gate_np_stages_tests
    code_walkthrough_quality_gate_np_stages_tests
    tests/code_walkthrough_quality_gate_non_participation_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_code_walkthrough_quality_gate_np_checks_tests
    code_walkthrough_quality_gate_np_checks_tests
    tests/code_walkthrough_quality_gate_non_participation_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_code_walkthrough_quality_gate_np_validation_tests
    code_walkthrough_quality_gate_np_validation_tests
    tests/code_walkthrough_quality_gate_non_participation_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_code_walkthrough_quality_gate_np_tests
    code_walkthrough_quality_gate_np_tests
    tests/code_walkthrough_quality_gate_non_participation_tests.cpp
)

minikv_add_source_dir_test(
    minikv_code_walkthrough_quality_gate_np_fixture_tests
    code_walkthrough_quality_gate_np_fixture_tests
    tests/code_walkthrough_quality_gate_non_participation_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_owner_receipt_request_packet_stages_tests
    owner_receipt_request_packet_stages_tests
    tests/production_shard_execution_owner_receipt_request_packet_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_owner_receipt_request_packet_checks_tests
    owner_receipt_request_packet_checks_tests
    tests/production_shard_execution_owner_receipt_request_packet_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_owner_receipt_request_packet_validation_tests
    owner_receipt_request_packet_validation_tests
    tests/production_shard_execution_owner_receipt_request_packet_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_owner_receipt_request_packet_tests
    owner_receipt_request_packet_tests
    tests/production_shard_execution_owner_receipt_request_packet_tests.cpp
)

minikv_add_source_dir_test(
    minikv_owner_receipt_request_packet_fixture_tests
    owner_receipt_request_packet_fixture_tests
    tests/production_shard_execution_owner_receipt_request_packet_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_mini_kv_owner_receipt_bundle_stages_tests
    mini_kv_owner_receipt_bundle_stages_tests
    tests/production_shard_execution_mini_kv_owner_receipt_bundle_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_mini_kv_owner_receipt_bundle_checks_tests
    mini_kv_owner_receipt_bundle_checks_tests
    tests/production_shard_execution_mini_kv_owner_receipt_bundle_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_mini_kv_owner_receipt_bundle_validation_tests
    mini_kv_owner_receipt_bundle_validation_tests
    tests/production_shard_execution_mini_kv_owner_receipt_bundle_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_mini_kv_owner_receipt_bundle_tests
    mini_kv_owner_receipt_bundle_tests
    tests/production_shard_execution_mini_kv_owner_receipt_bundle_tests.cpp
)

minikv_add_source_dir_test(
    minikv_mini_kv_owner_receipt_bundle_fixture_tests
    mini_kv_owner_receipt_bundle_fixture_tests
    tests/production_shard_execution_mini_kv_owner_receipt_bundle_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_external_artifact_dry_run_closeout_stages_tests
    external_artifact_dry_run_closeout_stages_tests
    tests/production_shard_execution_external_artifact_dry_run_closeout_stages_tests.cpp
)

minikv_add_linked_test(
    minikv_external_artifact_dry_run_closeout_checks_tests
    external_artifact_dry_run_closeout_checks_tests
    tests/production_shard_execution_external_artifact_dry_run_closeout_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_external_artifact_dry_run_closeout_validation_tests
    external_artifact_dry_run_closeout_validation_tests
    tests/production_shard_execution_external_artifact_dry_run_closeout_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_external_artifact_dry_run_closeout_tests
    external_artifact_dry_run_closeout_tests
    tests/production_shard_execution_external_artifact_dry_run_closeout_tests.cpp
)

minikv_add_source_dir_test(
    minikv_external_artifact_dry_run_closeout_fixture_tests
    external_artifact_dry_run_closeout_fixture_tests
    tests/production_shard_execution_external_artifact_dry_run_closeout_fixture_tests.cpp
)

minikv_add_linked_test(
    minikv_f_folder_explanation_quality_closeout_rules_tests
    f_folder_explanation_quality_closeout_rules_tests
    tests/f_folder_explanation_quality_closeout_rules_tests.cpp
)

minikv_add_linked_test(
    minikv_f_folder_explanation_quality_closeout_checks_tests
    f_folder_explanation_quality_closeout_checks_tests
    tests/f_folder_explanation_quality_closeout_checks_tests.cpp
)

minikv_add_linked_test(
    minikv_f_folder_explanation_quality_closeout_validation_tests
    f_folder_explanation_quality_closeout_validation_tests
    tests/f_folder_explanation_quality_closeout_validation_tests.cpp
)

minikv_add_linked_test(
    minikv_f_folder_explanation_quality_closeout_tests
    f_folder_explanation_quality_closeout_tests
    tests/f_folder_explanation_quality_closeout_tests.cpp
)

minikv_add_source_dir_test(
    minikv_f_folder_explanation_quality_closeout_fixture_tests
    f_folder_explanation_quality_closeout_fixture_tests
    tests/f_folder_explanation_quality_closeout_fixture_tests.cpp
)

minikv_add_source_dir_test(
    minikv_f_folder_explanation_quality_closeout_walkthrough_tests
    f_folder_explanation_quality_closeout_walkthrough_tests
    tests/f_folder_explanation_quality_closeout_walkthrough_tests.cpp
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
    minikv_tcp_server_timeout_limit_tests
    tcp_server_timeout_limit_tests
    tests/tcp_server_timeout_limit_tests.cpp
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
