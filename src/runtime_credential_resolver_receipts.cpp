#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::field_string;
using credential_resolver_detail::format_common_credential_boundary_json;
using credential_resolver_detail::format_no_go_conditions_json;
using credential_resolver_detail::receipt_digest;

constexpr std::string_view credential_resolver_receipt_consumer =
    "Node v261 credential resolver upstream echo verification";
constexpr std::string_view credential_resolver_receipt_fixture_path =
    "fixtures/release/credential-resolver-non-participation-receipt.json";
constexpr std::string_view credential_resolver_receipt_release_version = "v114";
constexpr std::string_view credential_resolver_receipt_artifact_path_hint = "c/114/";
constexpr std::string_view credential_resolver_source_decision =
    "Node v260 sandbox endpoint credential resolver decision record";
constexpr std::string_view credential_resolver_profile_version =
    "managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-decision-record.v1";
constexpr std::string_view credential_resolver_decision_state =
    "sandbox-endpoint-credential-resolver-decision-record-ready";
constexpr std::string_view credential_resolver_record_mode =
    "sandbox-endpoint-credential-resolver-decision-record-only";
constexpr std::string_view credential_resolver_decision_scope =
    "managed-audit-sandbox-endpoint-credential-resolver";
constexpr std::string_view credential_resolver_decision_status =
    "human-review-required-before-credential-resolution";
constexpr std::string_view credential_resolver_source_span =
    "Node v259 sandbox endpoint handle upstream echo verification";
constexpr std::string_view credential_resolver_endpoint_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view credential_resolver_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view credential_resolver_policy_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_RESOLVER_POLICY_HANDLE";
constexpr std::string_view credential_resolver_approval_marker =
    "ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_APPROVAL_MARKER";
constexpr std::string_view credential_resolver_operator_identity_evidence = "operator-header";
constexpr std::string_view credential_resolver_approval_correlation_evidence =
    "approval-correlation-header";
constexpr std::string_view credential_resolver_policy_evidence = "policy-handle-only";
constexpr std::string_view credential_resolver_redaction_policy_evidence = "redaction-reviewed";
constexpr std::string_view credential_resolver_fallback_rotation_plan_evidence = "plan-handle-only";
constexpr std::string_view credential_resolver_runtime_role = "runtime evidence provider only";
constexpr std::string_view credential_resolver_boundary =
    "credential resolver non-participation receipt only; mini-kv echoes Node v260 credential resolver decision boundaries, does not auto-start, does not write storage, does not read, load, store, or include credential values, does not parse raw endpoint URLs, does not instantiate a secret provider, does not invoke a credential resolver, does not send external requests, does not execute schema migration or restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view credential_resolver_node_action =
    "verify mini-kv non-participation with Node v260 credential resolver decision record before Node v261 upstream echo verification";
constexpr int credential_resolver_required_decision_field_count = 8;
constexpr int credential_resolver_explicit_no_go_condition_count = 9;
constexpr int credential_resolver_check_count = 20;
constexpr int credential_resolver_warning_count = 2;
constexpr int credential_resolver_recommendation_count = 2;
constexpr int credential_resolver_source_evidence_file_count = 6;
constexpr int credential_resolver_source_matched_snippet_count = 39;

constexpr std::string_view disabled_credential_resolver_receipt_consumer =
    "Node v263 disabled credential resolver upstream echo verification";
constexpr std::string_view disabled_credential_resolver_receipt_fixture_path =
    "fixtures/release/disabled-credential-resolver-precheck-non-participation-receipt.json";
constexpr std::string_view disabled_credential_resolver_receipt_release_version = "v115";
constexpr std::string_view disabled_credential_resolver_receipt_artifact_path_hint = "c/115/";
constexpr std::string_view disabled_credential_resolver_source_precheck =
    "Node v262 disabled credential resolver precheck";
constexpr std::string_view disabled_credential_resolver_source_verification =
    "Node v261 credential resolver upstream echo verification";
constexpr std::string_view disabled_credential_resolver_profile_version =
    "managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-disabled-precheck.v1";
constexpr std::string_view disabled_credential_resolver_precheck_state =
    "sandbox-endpoint-credential-resolver-disabled-precheck-ready";
constexpr std::string_view disabled_credential_resolver_precheck_mode =
    "sandbox-endpoint-credential-resolver-disabled-precheck-only";
constexpr std::string_view disabled_credential_resolver_source_span =
    "Node v260 + Java v105 + mini-kv v114";
constexpr std::string_view disabled_credential_resolver_endpoint_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view disabled_credential_resolver_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view disabled_credential_resolver_policy_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_RESOLVER_POLICY_HANDLE";
constexpr std::string_view disabled_credential_resolver_approval_marker =
    "ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_APPROVAL_MARKER";
constexpr std::string_view disabled_credential_resolver_gate =
    "ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_ENABLED";
constexpr std::string_view disabled_credential_resolver_endpoint_gate =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_RESOLUTION_ENABLED";
constexpr std::string_view disabled_credential_resolver_ready_state =
    "sandbox-endpoint-credential-resolver-disabled-precheck-ready";
constexpr std::string_view disabled_credential_resolver_runtime_role =
    "runtime evidence provider only";
constexpr std::string_view disabled_credential_resolver_boundary =
    "disabled credential resolver precheck non-participation receipt only; mini-kv echoes Node v262 disabled resolver precheck boundaries, does not auto-start, does not write storage, does not read, load, store, or include credential values, does not parse raw endpoint URLs, does not instantiate a secret provider or resolver client, does not send external requests, does not execute schema migration or restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view disabled_credential_resolver_node_action =
    "verify mini-kv non-participation with Node v262 disabled credential resolver precheck before Node v263 upstream echo verification";
constexpr int disabled_credential_resolver_required_env_handle_count = 6;
constexpr int disabled_credential_resolver_opt_in_gate_count = 2;
constexpr int disabled_credential_resolver_failure_class_count = 7;
constexpr int disabled_credential_resolver_dry_run_response_field_count = 12;
constexpr int disabled_credential_resolver_inherited_no_go_condition_count = 9;
constexpr int disabled_credential_resolver_check_count = 24;
constexpr int disabled_credential_resolver_warning_count = 2;
constexpr int disabled_credential_resolver_recommendation_count = 2;
constexpr int disabled_credential_resolver_source_v261_check_count = 31;
constexpr int disabled_credential_resolver_source_v261_warning_count = 2;
constexpr int disabled_credential_resolver_source_v261_recommendation_count = 2;

} // namespace

std::string credential_resolver_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{credential_resolver_receipt_release_version}},
        {std::string{credential_resolver_receipt_artifact_path_hint}},
        {std::string{credential_resolver_source_decision}},
        {std::string{credential_resolver_profile_version}},
        {std::string{credential_resolver_decision_state}},
        {std::string{credential_resolver_record_mode}},
        {std::string{credential_resolver_decision_scope}},
        {std::string{credential_resolver_decision_status}},
        {std::string{credential_resolver_source_span}},
        {std::string{credential_resolver_endpoint_handle}},
        {std::string{credential_resolver_credential_handle}},
        {std::string{credential_resolver_policy_handle}},
        {std::string{credential_resolver_approval_marker}},
        {std::string{credential_resolver_operator_identity_evidence}},
        {std::string{credential_resolver_approval_correlation_evidence}},
        {std::string{credential_resolver_policy_evidence}},
        {std::string{credential_resolver_redaction_policy_evidence}},
        {std::string{credential_resolver_fallback_rotation_plan_evidence}},
        {std::to_string(credential_resolver_required_decision_field_count)},
        {std::to_string(credential_resolver_explicit_no_go_condition_count)},
        {std::to_string(credential_resolver_check_count)},
        {std::to_string(credential_resolver_warning_count)},
        {std::to_string(credential_resolver_recommendation_count)},
        {std::to_string(credential_resolver_source_evidence_file_count)},
        {std::to_string(credential_resolver_source_matched_snippet_count)},
        {"true"},
        {"true"},
        {"true"},
        {"true"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-non-participation", parts);
}

std::string format_credential_resolver_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(credential_resolver_receipt_fixture_path) +
           ",\"source_decision\":" + field_string(credential_resolver_source_decision) +
           ",\"consumer_hint\":" + field_string(credential_resolver_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" +
           field_string(credential_resolver_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(credential_resolver_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(credential_resolver_runtime_role) +
           ",\"source_decision_profile_version\":" +
           field_string(credential_resolver_profile_version) +
           ",\"source_decision_state\":" + field_string(credential_resolver_decision_state) +
           ",\"source_record_mode\":" + field_string(credential_resolver_record_mode) +
           ",\"source_decision_scope\":" + field_string(credential_resolver_decision_scope) +
           ",\"source_decision_status\":" + field_string(credential_resolver_decision_status) +
           ",\"source_span\":" + field_string(credential_resolver_source_span) +
           ",\"source_ready_for_credential_resolver_decision_record\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
           ",\"source_read_only_decision_record\":true"
           ",\"source_credential_resolver_decision_only\":true"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_credential_value_loaded\":false"
           ",\"source_credential_value_included\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_raw_endpoint_url_included\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_required_decision_field_count\":" +
           std::to_string(credential_resolver_required_decision_field_count) +
           ",\"source_explicit_no_go_condition_count\":" +
           std::to_string(credential_resolver_explicit_no_go_condition_count) +
           ",\"source_check_count\":" + std::to_string(credential_resolver_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(credential_resolver_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"source_warning_count\":" + std::to_string(credential_resolver_warning_count) +
           ",\"source_recommendation_count\":" +
           std::to_string(credential_resolver_recommendation_count) +
           ",\"source_node_v259_ready\":true"
           ",\"source_node_v259_blocks_real_connection\":true"
           ",\"source_node_v259_credential_boundary_aligned\":true"
           ",\"source_node_v259_raw_endpoint_boundary_aligned\":true"
           ",\"source_node_v259_write_boundary_aligned\":true"
           ",\"source_node_v259_auto_start_boundary_aligned\":true"
           ",\"source_node_v259_keeps_mini_kv_non_participant\":true"
           ",\"source_node_v259_evidence_file_count\":" +
           std::to_string(credential_resolver_source_evidence_file_count) +
           ",\"source_node_v259_matched_snippet_count\":" +
           std::to_string(credential_resolver_source_matched_snippet_count) +
           ",\"source_node_v259_ready_for_node_v260_decision_record\":true"
           ",\"source_upstream_actions_still_disabled\":true"
           ",\"decision_record\":{\"record_mode\":" +
           field_string(credential_resolver_record_mode) +
           ",\"decision_scope\":" + field_string(credential_resolver_decision_scope) +
           ",\"decision_status\":" + field_string(credential_resolver_decision_status) +
           ",\"source_span\":" + field_string(credential_resolver_source_span) +
           ",\"endpoint_handle\":" + field_string(credential_resolver_endpoint_handle) +
           ",\"credential_handle\":" + field_string(credential_resolver_credential_handle) +
           ",\"resolver_policy_handle\":" + field_string(credential_resolver_policy_handle) +
           ",\"approval_marker\":" + field_string(credential_resolver_approval_marker) +
           ",\"operator_identity_required\":true,\"approval_correlation_required\":true,"
           "\"resolver_mode\":\"policy-record-only-no-value-read\","
           "\"resolver_candidate_implementation\":\"not-implemented\","
           "\"required_decision_field_count\":" +
           std::to_string(credential_resolver_required_decision_field_count) +
           ",\"explicit_no_go_condition_count\":" +
           std::to_string(credential_resolver_explicit_no_go_condition_count) +
           ",\"credential_value_may_be_read\":false,\"credential_value_may_be_loaded\":false,"
           "\"credential_value_may_be_stored\":false,\"raw_endpoint_url_may_be_parsed\":false,"
           "\"managed_audit_connection_may_open\":false,\"schema_migration_may_execute\":false,"
           "\"external_request_may_be_sent\":false,\"node_may_start_java_or_mini_kv\":false,"
           "\"mini_kv_may_act_as_managed_audit_storage\":false,"
           "\"approval_ledger_may_be_written\":false}"
           ",\"required_decision_fields\":[{\"id\":\"endpoint-handle\",\"accepted_evidence\":\"handle-aligned\"},"
           "{\"id\":\"credential-handle\",\"accepted_evidence\":\"handle-aligned\"},"
           "{\"id\":\"resolver-policy-handle\",\"accepted_evidence\":" +
           field_string(credential_resolver_policy_evidence) +
           "},{\"id\":\"approval-marker\",\"accepted_evidence\":\"approval-marker-only\"},"
           "{\"id\":\"operator-identity\",\"accepted_evidence\":" +
           field_string(credential_resolver_operator_identity_evidence) +
           "},{\"id\":\"approval-correlation\",\"accepted_evidence\":" +
           field_string(credential_resolver_approval_correlation_evidence) +
           "},{\"id\":\"redaction-policy\",\"accepted_evidence\":" +
           field_string(credential_resolver_redaction_policy_evidence) +
           "},{\"id\":\"fallback-rotation-plan\",\"accepted_evidence\":" +
           field_string(credential_resolver_fallback_rotation_plan_evidence) + "}]"
           ",\"explicit_no_go_conditions\":" + format_no_go_conditions_json() +
           ",\"checks\":{\"source_node_v259_ready\":true,"
           "\"source_node_v259_still_blocks_connection\":true,"
           "\"source_node_v259_still_blocks_credential_value\":true,"
           "\"source_node_v259_still_blocks_raw_endpoint\":true,"
           "\"source_node_v259_still_blocks_writes\":true,"
           "\"source_node_v259_still_blocks_auto_start\":true,"
           "\"source_node_v259_keeps_mini_kv_non_participant\":true,"
           "\"endpoint_handle_recorded\":true,\"credential_handle_recorded\":true,"
           "\"resolver_policy_recorded\":true,\"approval_marker_recorded\":true,"
           "\"operator_identity_requirement_recorded\":true,"
           "\"approval_correlation_requirement_recorded\":true,"
           "\"resolver_mode_still_policy_only\":true,"
           "\"explicit_no_go_conditions_recorded\":true,"
           "\"decision_record_still_read_only\":true,\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,\"production_window_still_blocked\":true}"
           ",\"next_required_echo_versions\":[\"Java v105\",\"mini-kv v114\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_non_participation_receipt_digest(read_commands)) +
           ",\"credential_resolver_decision_only\":true," + format_common_credential_boundary_json() +
           ",\"boundary\":" + field_string(credential_resolver_boundary) +
           ",\"node_action\":" + field_string(credential_resolver_node_action) + "}";
}

std::string disabled_credential_resolver_precheck_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{disabled_credential_resolver_receipt_release_version}},
        {std::string{disabled_credential_resolver_receipt_artifact_path_hint}},
        {std::string{disabled_credential_resolver_source_precheck}},
        {std::string{disabled_credential_resolver_source_verification}},
        {std::string{disabled_credential_resolver_profile_version}},
        {std::string{disabled_credential_resolver_precheck_state}},
        {std::string{disabled_credential_resolver_precheck_mode}},
        {std::string{disabled_credential_resolver_source_span}},
        {std::string{disabled_credential_resolver_gate}},
        {std::string{disabled_credential_resolver_endpoint_gate}},
        {std::string{disabled_credential_resolver_endpoint_handle}},
        {std::string{disabled_credential_resolver_credential_handle}},
        {std::string{disabled_credential_resolver_policy_handle}},
        {std::string{disabled_credential_resolver_approval_marker}},
        {std::to_string(disabled_credential_resolver_required_env_handle_count)},
        {std::to_string(disabled_credential_resolver_opt_in_gate_count)},
        {std::to_string(disabled_credential_resolver_failure_class_count)},
        {std::to_string(disabled_credential_resolver_dry_run_response_field_count)},
        {std::to_string(disabled_credential_resolver_inherited_no_go_condition_count)},
        {std::to_string(disabled_credential_resolver_check_count)},
        {std::to_string(disabled_credential_resolver_warning_count)},
        {std::to_string(disabled_credential_resolver_recommendation_count)},
        {"not-implemented"},
        {"not-implemented"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-disabled-credential-resolver-precheck-non-participation", parts);
}

std::string format_disabled_credential_resolver_precheck_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-disabled-credential-resolver-precheck-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" +
           field_string(disabled_credential_resolver_receipt_fixture_path) +
           ",\"source_precheck\":" + field_string(disabled_credential_resolver_source_precheck) +
           ",\"source_verification\":" + field_string(disabled_credential_resolver_source_verification) +
           ",\"consumer_hint\":" + field_string(disabled_credential_resolver_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" +
           field_string(disabled_credential_resolver_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(disabled_credential_resolver_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(disabled_credential_resolver_runtime_role) +
           ",\"source_precheck_profile_version\":" +
           field_string(disabled_credential_resolver_profile_version) +
           ",\"source_precheck_state\":" +
           field_string(disabled_credential_resolver_precheck_state) +
           ",\"source_precheck_mode\":" +
           field_string(disabled_credential_resolver_precheck_mode) +
           ",\"source_span\":" + field_string(disabled_credential_resolver_source_span) +
           ",\"source_ready_for_disabled_credential_resolver_precheck\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
           ",\"source_read_only_disabled_precheck\":true"
           ",\"source_disabled_credential_resolver_precheck_only\":true"
           ",\"source_credential_resolver_execution_allowed\":false"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_credential_value_loaded\":false"
           ",\"source_credential_value_stored\":false"
           ",\"source_credential_value_included\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_raw_endpoint_url_included\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_secret_provider_instantiated\":false"
           ",\"source_resolver_client_instantiated\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_required_env_handle_count\":" +
           std::to_string(disabled_credential_resolver_required_env_handle_count) +
           ",\"source_opt_in_gate_count\":" +
           std::to_string(disabled_credential_resolver_opt_in_gate_count) +
           ",\"source_failure_class_count\":" +
           std::to_string(disabled_credential_resolver_failure_class_count) +
           ",\"source_dry_run_response_field_count\":" +
           std::to_string(disabled_credential_resolver_dry_run_response_field_count) +
           ",\"source_inherited_no_go_condition_count\":" +
           std::to_string(disabled_credential_resolver_inherited_no_go_condition_count) +
           ",\"source_check_count\":" + std::to_string(disabled_credential_resolver_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(disabled_credential_resolver_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"source_warning_count\":" + std::to_string(disabled_credential_resolver_warning_count) +
           ",\"source_recommendation_count\":" +
           std::to_string(disabled_credential_resolver_recommendation_count) +
           ",\"source_node_v261_ready\":true"
           ",\"source_node_v261_verification_mode\":\"java-v105-plus-mini-kv-v114-credential-resolver-upstream-echo-verification-only\""
           ",\"source_node_v261_span\":" + field_string(disabled_credential_resolver_source_span) +
           ",\"source_node_v261_blocks_credential_resolution\":true"
           ",\"source_node_v261_credential_boundary_aligned\":true"
           ",\"source_node_v261_raw_endpoint_boundary_aligned\":true"
           ",\"source_node_v261_connection_boundary_aligned\":true"
           ",\"source_node_v261_write_boundary_aligned\":true"
           ",\"source_node_v261_auto_start_boundary_aligned\":true"
           ",\"source_node_v261_upstream_actions_still_disabled\":true"
           ",\"source_node_v261_check_count\":" +
           std::to_string(disabled_credential_resolver_source_v261_check_count) +
           ",\"source_node_v261_passed_check_count\":" +
           std::to_string(disabled_credential_resolver_source_v261_check_count) +
           ",\"source_node_v261_production_blocker_count\":0"
           ",\"source_node_v261_warning_count\":" +
           std::to_string(disabled_credential_resolver_source_v261_warning_count) +
           ",\"source_node_v261_recommendation_count\":" +
           std::to_string(disabled_credential_resolver_source_v261_recommendation_count) +
           ",\"disabled_precheck\":{\"precheck_mode\":" +
           field_string(disabled_credential_resolver_precheck_mode) +
           ",\"ready_state\":" + field_string(disabled_credential_resolver_ready_state) +
           ",\"resolver_implementation_status\":\"not-implemented\","
           "\"secret_provider_implementation_status\":\"not-implemented\","
           "\"resolver_client_may_be_instantiated\":false,"
           "\"secret_provider_may_be_instantiated\":false,"
           "\"credential_value_may_be_loaded\":false,"
           "\"raw_endpoint_url_may_be_parsed\":false,"
           "\"external_request_may_be_sent\":false,"
           "\"opt_in_gate_required\":true,"
           "\"required_env_handle_count\":" +
           std::to_string(disabled_credential_resolver_required_env_handle_count) +
           ",\"opt_in_gate_count\":" +
           std::to_string(disabled_credential_resolver_opt_in_gate_count) +
           ",\"failure_class_count\":" +
           std::to_string(disabled_credential_resolver_failure_class_count) +
           ",\"dry_run_response_field_count\":" +
           std::to_string(disabled_credential_resolver_dry_run_response_field_count) +
           ",\"inherited_no_go_condition_count\":" +
           std::to_string(disabled_credential_resolver_inherited_no_go_condition_count) + "}"
           ",\"required_env_handles\":[{\"name\":" + field_string(disabled_credential_resolver_gate) +
           ",\"purpose\":\"future opt-in gate for the credential resolver\","
           "\"value_required_for_precheck\":false,\"credential_value\":false,\"raw_endpoint_value\":false,"
           "\"required_before_real_resolver\":false},{\"name\":" +
           field_string(disabled_credential_resolver_endpoint_gate) +
           ",\"purpose\":\"future opt-in gate for sandbox endpoint resolution\","
           "\"value_required_for_precheck\":false,\"credential_value\":false,\"raw_endpoint_value\":false,"
           "\"required_before_real_resolver\":false},{\"name\":" +
           field_string(disabled_credential_resolver_endpoint_handle) +
           ",\"purpose\":\"handle for the approved sandbox endpoint\","
           "\"value_required_for_precheck\":false,\"credential_value\":false,\"raw_endpoint_value\":false,"
           "\"required_before_real_resolver\":true},{\"name\":" +
           field_string(disabled_credential_resolver_credential_handle) +
           ",\"purpose\":\"handle for the approved sandbox credential\","
           "\"value_required_for_precheck\":false,\"credential_value\":false,\"raw_endpoint_value\":false,"
           "\"required_before_real_resolver\":true},{\"name\":" +
           field_string(disabled_credential_resolver_policy_handle) +
           ",\"purpose\":\"handle for the resolver policy review\","
           "\"value_required_for_precheck\":false,\"credential_value\":false,\"raw_endpoint_value\":false,"
           "\"required_before_real_resolver\":true},{\"name\":" +
           field_string(disabled_credential_resolver_approval_marker) +
           ",\"purpose\":\"operator approval marker for future resolver design\","
           "\"value_required_for_precheck\":false,\"credential_value\":false,\"raw_endpoint_value\":false,"
           "\"required_before_real_resolver\":true}]"
           ",\"opt_in_gates\":[{\"gate_name\":" + field_string(disabled_credential_resolver_gate) +
           ",\"required_value_for_future_resolver\":\"true\",\"current_default\":\"false\","
           "\"precheck_treats_enabled_as_blocked\":true,\"operator_approval_required\":true},"
           "{\"gate_name\":" + field_string(disabled_credential_resolver_endpoint_gate) +
           ",\"required_value_for_future_resolver\":\"true\",\"current_default\":\"false\","
           "\"precheck_treats_enabled_as_blocked\":true,\"operator_approval_required\":true}]"
           ",\"failure_taxonomy_codes\":[\"RESOLVER_DISABLED\",\"APPROVAL_MARKER_MISSING\","
           "\"CREDENTIAL_HANDLE_MISSING\",\"CREDENTIAL_VALUE_REQUESTED\","
           "\"RAW_ENDPOINT_URL_REQUESTED\",\"EXTERNAL_REQUEST_REQUESTED\","
           "\"SCHEMA_MIGRATION_REQUESTED\"]"
           ",\"dry_run_response_shape\":{\"fields\":[\"readyState\",\"resolverMode\","
           "\"resolverClientInstantiated\",\"secretProviderInstantiated\",\"credentialValueRead\","
           "\"credentialValueLoaded\",\"rawEndpointUrlParsed\",\"externalRequestSent\","
           "\"connectsManagedAudit\",\"schemaMigrationExecuted\",\"failureClassCount\","
           "\"nextAction\"],\"ready_state\":" + field_string(disabled_credential_resolver_ready_state) +
           ",\"resolver_client_instantiated\":false,\"secret_provider_instantiated\":false,"
           "\"credential_value_read\":false,\"credential_value_loaded\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"connects_managed_audit\":false,\"schema_migration_executed\":false}"
           ",\"inherited_no_go_conditions\":" + format_no_go_conditions_json() +
           ",\"checks\":{\"source_node_v261_ready\":true,"
           "\"source_verification_still_blocks_credential_resolution\":true,"
           "\"source_verification_still_blocks_credential_value\":true,"
           "\"source_verification_still_blocks_raw_endpoint\":true,"
           "\"source_verification_still_blocks_connection\":true,"
           "\"source_verification_still_blocks_writes\":true,"
           "\"source_verification_still_blocks_auto_start\":true,"
           "\"required_env_handles_declared\":true,\"env_handles_remain_handle_only\":true,"
           "\"opt_in_gates_declared\":true,\"opt_in_gates_default_disabled\":true,"
           "\"failure_taxonomy_declared\":true,\"dry_run_response_shape_declared\":true,"
           "\"resolver_implementation_still_absent\":true,"
           "\"secret_provider_implementation_still_absent\":true,"
           "\"resolver_client_instantiation_blocked\":true,"
           "\"secret_provider_instantiation_blocked\":true,"
           "\"credential_value_load_blocked\":true,\"raw_endpoint_parse_blocked\":true,"
           "\"external_request_blocked\":true,\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,\"production_window_still_blocked\":true}"
           ",\"next_required_echo_versions\":[\"Java v106\",\"mini-kv v115\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(disabled_credential_resolver_precheck_non_participation_receipt_digest(
               read_commands)) +
           ",\"disabled_credential_resolver_precheck_only\":true," +
           format_common_credential_boundary_json() +
           ",\"resolver_client_instantiated\":false"
           ",\"boundary\":" + field_string(disabled_credential_resolver_boundary) +
           ",\"node_action\":" + field_string(disabled_credential_resolver_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
