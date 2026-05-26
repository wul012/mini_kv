#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace minikv::runtime_evidence_receipts {

std::string read_command_list_digest(const std::vector<std::string>& commands);
std::string format_live_read_session_hint_json(std::int64_t uptime_seconds,
                                               const std::vector<std::string>& read_commands);

std::string smoke_failure_taxonomy_digest();
std::string format_smoke_failure_taxonomy_json();
std::string format_smoke_operator_window_proof_json();

std::string format_runtime_ci_evidence_hint_json();
std::string format_runtime_artifact_retention_evidence_json();

std::string binary_provenance_digest();
std::string format_runtime_binary_provenance_hint_json();

std::string retention_provenance_check_digest();
std::string format_runtime_retention_provenance_check_json();

std::string retention_provenance_replay_marker_digest();
std::string format_runtime_retention_provenance_replay_marker_json();

std::string manual_sandbox_dry_run_command_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_manual_sandbox_dry_run_command_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string manual_sandbox_connection_precheck_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_manual_sandbox_connection_precheck_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string disabled_adapter_client_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_disabled_adapter_client_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string fake_transport_dry_run_packet_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_fake_transport_dry_run_packet_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string sandbox_endpoint_handle_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_sandbox_endpoint_handle_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string disabled_credential_resolver_precheck_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_disabled_credential_resolver_precheck_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string test_only_resolver_shell_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_test_only_resolver_shell_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_fake_shell_archive_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_fake_shell_archive_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_production_readiness_blocked_decision_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_pre_implementation_plan_intake_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_disabled_implementation_candidate_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_disabled_implementation_candidate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_approval_required_boundary_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_approval_required_boundary_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_approval_required_implementation_readiness_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_approval_required_implementation_readiness_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_implementation_plan_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_implementation_plan_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_disabled_fake_harness_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_disabled_fake_harness_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_disabled_runtime_shell_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_disabled_runtime_shell_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_runtime_shell_decision_record_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_runtime_shell_decision_record_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_human_approval_artifact_review_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_human_approval_artifact_review_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_credential_handle_approval_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_credential_handle_approval_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

std::string credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands);
std::string format_credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_json(
    const std::vector<std::string>& read_commands);

} // namespace minikv::runtime_evidence_receipts
