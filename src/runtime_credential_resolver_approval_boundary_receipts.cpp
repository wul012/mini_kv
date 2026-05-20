#include "minikv/runtime_evidence_receipts.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;

std::string field_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string receipt_digest(std::string_view prefix, const std::vector<DigestPart>& parts) {
    return runtime_evidence::digest(prefix, parts);
}

constexpr std::string_view approval_boundary_receipt_consumer =
    "Node v275 credential resolver approval-required boundary upstream echo verification";
constexpr std::string_view approval_boundary_receipt_fixture_path =
    "fixtures/release/credential-resolver-approval-required-boundary-non-participation-receipt.json";
constexpr std::string_view approval_boundary_receipt_release_version = "v121";
constexpr std::string_view approval_boundary_receipt_artifact_path_hint = "c/121/";
constexpr std::string_view approval_boundary_source_verification =
    "Node v274 credential resolver disabled candidate upstream echo verification";
constexpr std::string_view approval_boundary_source_review =
    "Node v273 credential resolver disabled implementation candidate review";
constexpr std::string_view approval_boundary_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-candidate-upstream-echo-verification.v1";
constexpr std::string_view approval_boundary_verification_state =
    "credential-resolver-disabled-candidate-upstream-echo-verification-ready";
constexpr std::string_view approval_boundary_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-candidate-upstream-echo-verification";
constexpr std::string_view approval_boundary_active_plan =
    "docs/plans/v274-post-disabled-candidate-echo-roadmap.md";
constexpr std::string_view approval_boundary_runtime_role = "runtime evidence provider only";
constexpr std::string_view approval_boundary_candidate_digest =
    "12862fa65cad3a8db92f88fba86b74c25e584f35614c7a47367d441358b8b7d7";
constexpr std::string_view approval_boundary_v120_digest = "fnv1a64:0f96618095243829";
constexpr std::string_view approval_boundary_boundary =
    "credential resolver approval-required boundary non-participation receipt only; mini-kv echoes Node v274 approval-required boundary verification, does not implement or invoke a resolver, does not read credential values, does not parse raw endpoint URLs, does not send external requests, does not write storage or approval ledger records, does not execute schema migration or LOAD/COMPACT/RESTORE/SETNXEX, and does not become a managed audit storage backend or order authority";
constexpr std::string_view approval_boundary_node_action =
    "verify mini-kv approval-required boundary non-participation with Java v115 before Node v275 upstream echo verification";
constexpr int approval_boundary_check_count = 25;
constexpr int approval_boundary_candidate_decision_count = 10;
constexpr int approval_boundary_candidate_ready_decision_count = 4;
constexpr int approval_boundary_approval_required_decision_count = 6;
constexpr int approval_boundary_request_field_count = 6;
constexpr int approval_boundary_response_field_count = 7;
constexpr int approval_boundary_failure_class_count = 6;
constexpr int approval_boundary_evidence_file_count = 12;
constexpr int approval_boundary_production_blocker_count = 0;
constexpr int approval_boundary_warning_count = 2;
constexpr int approval_boundary_recommendation_count = 2;

std::string format_candidate_ready_boundaries_json() {
    return "[\"PLAN_DOCUMENT\",\"DISABLED_SECRET_PROVIDER_STUB\","
           "\"REDACTION_POLICY\",\"EXTERNAL_REQUEST_SIMULATION\"]";
}

std::string format_approval_required_boundaries_json() {
    return "[\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\",\"OPERATOR_APPROVAL\","
           "\"ROLLBACK_BOUNDARY\",\"SCHEMA_MIGRATION_POLICY\",\"AUDIT_LEDGER_WRITE_POLICY\"]";
}

std::string format_approval_required_boundary_details_json() {
    return "[{\"code\":\"CREDENTIAL_HANDLE\",\"owner\":\"security\","
           "\"reason\":\"credential handle approval is required before any credential value can be resolved\","
           "\"mini_kv_position\":\"handle names may be echoed as evidence; credential values are never read, stored, or rendered\","
           "\"prohibited_runtime_actions\":[\"read-credential-value\",\"store-credential-value\",\"render-credential-value\"],"
           "\"read_only\":true,\"approval_required\":true,\"mini_kv_participates\":false},"
           "{\"code\":\"ENDPOINT_HANDLE\",\"owner\":\"security\","
           "\"reason\":\"endpoint handle approval is required before any raw endpoint URL can be resolved\","
           "\"mini_kv_position\":\"endpoint handles may be echoed as evidence; raw endpoint URLs are never parsed or rendered\","
           "\"prohibited_runtime_actions\":[\"parse-raw-endpoint-url\",\"render-raw-endpoint-url\",\"connect-managed-audit\"],"
           "\"read_only\":true,\"approval_required\":true,\"mini_kv_participates\":false},"
           "{\"code\":\"OPERATOR_APPROVAL\",\"owner\":\"operator\","
           "\"reason\":\"operator approval is required before any manual managed-audit action can proceed\","
           "\"mini_kv_position\":\"operator markers may be referenced as evidence only; mini-kv cannot approve or execute operations\","
           "\"prohibited_runtime_actions\":[\"execute-without-operator-marker\",\"auto-approve-operation\",\"auto-start-upstream\"],"
           "\"read_only\":true,\"approval_required\":true,\"mini_kv_participates\":false},"
           "{\"code\":\"ROLLBACK_BOUNDARY\",\"owner\":\"release-manager\","
           "\"reason\":\"rollback and abort boundaries must be approved before resolver work can affect runtime systems\","
           "\"mini_kv_position\":\"rollback markers may be echoed as evidence only; mini-kv does not deploy or roll back resolver behavior\","
           "\"prohibited_runtime_actions\":[\"execute-rollback\",\"deploy-resolver-without-abort-marker\",\"write-production-record\"],"
           "\"read_only\":true,\"approval_required\":true,\"mini_kv_participates\":false},"
           "{\"code\":\"SCHEMA_MIGRATION_POLICY\",\"owner\":\"release-manager\","
           "\"reason\":\"schema migration policy approval is required before SQL or schema changes can be considered\","
           "\"mini_kv_position\":\"schema policy can be reported as blocked evidence only; mini-kv never executes SQL or schema migration\","
           "\"prohibited_runtime_actions\":[\"execute-schema-migration\",\"execute-sql\",\"mutate-managed-audit-schema\"],"
           "\"read_only\":true,\"approval_required\":true,\"mini_kv_participates\":false},"
           "{\"code\":\"AUDIT_LEDGER_WRITE_POLICY\",\"owner\":\"release-manager\","
           "\"reason\":\"audit ledger write policy approval is required before any ledger or managed-audit state write\","
           "\"mini_kv_position\":\"ledger write policy is evidence only; mini-kv never writes approval ledger or managed-audit state\","
           "\"prohibited_runtime_actions\":[\"write-approval-ledger\",\"write-managed-audit-state\",\"write-storage\"],"
           "\"read_only\":true,\"approval_required\":true,\"mini_kv_participates\":false}]";
}

std::string format_source_node_v274_checks_json() {
    return "{\"source_node_v273_ready\":true,\"source_node_v273_keeps_review_only\":true,"
           "\"source_node_v273_keeps_real_resolver_blocked\":true,"
           "\"source_node_v273_keeps_boundary_alignment\":true,"
           "\"java_v113_echo_ready\":true,\"mini_kv_v120_non_participation_ready\":true,"
           "\"candidate_counts_aligned\":true,\"candidate_ready_boundary_codes_aligned\":true,"
           "\"approval_required_boundary_codes_aligned\":true,\"interface_shape_aligned\":true,"
           "\"fake_wiring_aligned\":true,\"credential_boundary_aligned\":true,"
           "\"raw_endpoint_boundary_aligned\":true,\"connection_boundary_aligned\":true,"
           "\"write_boundary_aligned\":true,\"auto_start_boundary_aligned\":true,"
           "\"java_echo_workflow_template_applied\":true,\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,\"real_resolver_implementation_still_blocked\":true,"
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_candidate_upstream_echo_verification\":true}";
}

std::string format_approval_boundary_checks_json() {
    return "{\"source_node_v274_ready\":true,\"source_node_v274_read_only\":true,"
           "\"approval_required_boundary_count_preserved\":true,"
           "\"credential_handle_boundary_stays_approval_required\":true,"
           "\"endpoint_handle_boundary_stays_approval_required\":true,"
           "\"operator_approval_boundary_stays_approval_required\":true,"
           "\"rollback_boundary_stays_approval_required\":true,"
           "\"schema_migration_policy_stays_approval_required\":true,"
           "\"audit_ledger_write_policy_stays_approval_required\":true,"
           "\"credential_value_still_forbidden\":true,\"raw_endpoint_still_forbidden\":true,"
           "\"resolver_client_still_disabled\":true,\"secret_provider_still_disabled\":true,"
           "\"external_request_still_blocked\":true,\"managed_audit_connection_still_blocked\":true,"
           "\"storage_write_still_blocked\":true,\"schema_migration_still_blocked\":true,"
           "\"approval_ledger_write_still_blocked\":true,\"load_compact_restore_setnxex_still_blocked\":true,"
           "\"auto_start_still_blocked\":true,\"order_authority_still_false\":true,"
           "\"ready_for_java_v115_parallel_echo\":true,\"ready_for_node_v275_upstream_echo\":true}";
}

std::string format_approval_boundary_non_participation_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"approval_required_boundary_non_participation_receipt_only\":true,"
           "\"approval_required_boundary_refinement_only\":true,"
           "\"ready_for_approval_required_boundary_upstream_echo\":true,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"ready_for_production_audit\":false,\"ready_for_production_window\":false,"
           "\"ready_for_production_operations\":false,\"real_resolver_implementation_allowed\":false,"
           "\"credential_resolver_implemented\":false,\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,\"secret_provider_instantiated\":false,"
           "\"secret_provider_runtime_allowed\":false,\"credential_value_read_allowed\":false,"
           "\"credential_value_loaded\":false,\"credential_value_stored\":false,"
           "\"credential_value_included\":false,\"raw_endpoint_url_parse_allowed\":false,"
           "\"raw_endpoint_url_parsed\":false,\"raw_endpoint_url_included\":false,"
           "\"external_request_allowed\":false,\"external_request_sent\":false,"
           "\"connects_managed_audit\":false,\"reads_managed_audit_credential\":false,"
           "\"stores_managed_audit_credential\":false,\"managed_audit_store\":false,"
           "\"managed_audit_storage_backend\":false,\"sandbox_audit_storage_backend\":false,"
           "\"storage_write_allowed\":false,\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,\"runtime_write_observed\":false,"
           "\"approval_ledger_write_allowed\":false,\"approval_ledger_written\":false,"
           "\"approval_ledger_write_executed\":false,\"managed_audit_write_executed\":false,"
           "\"production_record_written\":false,\"schema_migration_allowed\":false,"
           "\"schema_migration_executed\":false,\"schema_rehearsal_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"node_auto_start_allowed\":false,"
           "\"java_auto_start_allowed\":false,\"mini_kv_auto_start_allowed\":false,"
           "\"automatic_upstream_start_allowed\":false,\"automatic_upstream_start\":false,"
           "\"audit_authoritative\":false,\"order_authoritative\":false";
}

} // namespace

std::string credential_resolver_approval_required_boundary_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{approval_boundary_receipt_release_version}},
        {std::string{approval_boundary_receipt_artifact_path_hint}},
        {std::string{approval_boundary_source_verification}},
        {std::string{approval_boundary_profile_version}},
        {std::string{approval_boundary_verification_state}},
        {std::string{approval_boundary_candidate_digest}},
        {std::string{approval_boundary_v120_digest}},
        {std::to_string(approval_boundary_check_count)},
        {std::to_string(approval_boundary_candidate_decision_count)},
        {std::to_string(approval_boundary_candidate_ready_decision_count)},
        {std::to_string(approval_boundary_approval_required_decision_count)},
        {std::to_string(approval_boundary_request_field_count)},
        {std::to_string(approval_boundary_response_field_count)},
        {std::to_string(approval_boundary_failure_class_count)},
        {"CREDENTIAL_HANDLE"},
        {"ENDPOINT_HANDLE"},
        {"OPERATOR_APPROVAL"},
        {"ROLLBACK_BOUNDARY"},
        {"SCHEMA_MIGRATION_POLICY"},
        {"AUDIT_LEDGER_WRITE_POLICY"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-approval-required-boundary-non-participation", parts);
}

std::string format_credential_resolver_approval_required_boundary_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-approval-required-boundary-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(approval_boundary_receipt_fixture_path) +
           ",\"source_verification\":" + field_string(approval_boundary_source_verification) +
           ",\"source_review\":" + field_string(approval_boundary_source_review) +
           ",\"consumer_hint\":" + field_string(approval_boundary_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(approval_boundary_receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(approval_boundary_receipt_artifact_path_hint) +
           ",\"previous_mini_kv_release_version\":\"v120\""
           ",\"previous_mini_kv_receipt_digest\":" + field_string(approval_boundary_v120_digest) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(approval_boundary_runtime_role) +
           ",\"source_profile_version\":" + field_string(approval_boundary_profile_version) +
           ",\"source_route_path\":" + field_string(approval_boundary_route_path) +
           ",\"source_verification_state\":" + field_string(approval_boundary_verification_state) +
           ",\"source_ready_for_disabled_candidate_upstream_echo_verification\":true"
           ",\"source_read_only_upstream_echo_verification\":true"
           ",\"source_disabled_candidate_echo_verification_only\":true"
           ",\"source_ready_for_disabled_resolver_interface_candidate\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
           ",\"source_ready_for_production_operations\":false"
           ",\"source_real_resolver_implementation_allowed\":false"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_secret_provider_instantiated\":false"
           ",\"source_resolver_client_instantiated\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_approval_ledger_written\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_node_v274_verification\":{\"profile_version\":" +
           field_string(approval_boundary_profile_version) +
           ",\"verification_state\":" + field_string(approval_boundary_verification_state) +
           ",\"ready\":true,\"read_only_upstream_echo_verification\":true,"
           "\"disabled_candidate_echo_verification_only\":true,"
           "\"check_count\":" + std::to_string(approval_boundary_check_count) +
           ",\"passed_check_count\":" + std::to_string(approval_boundary_check_count) +
           ",\"candidate_decision_count\":" +
           std::to_string(approval_boundary_candidate_decision_count) +
           ",\"candidate_ready_decision_count\":" +
           std::to_string(approval_boundary_candidate_ready_decision_count) +
           ",\"approval_required_decision_count\":" +
           std::to_string(approval_boundary_approval_required_decision_count) +
           ",\"request_field_count\":" + std::to_string(approval_boundary_request_field_count) +
           ",\"response_field_count\":" + std::to_string(approval_boundary_response_field_count) +
           ",\"failure_class_count\":" + std::to_string(approval_boundary_failure_class_count) +
           ",\"evidence_file_count\":" + std::to_string(approval_boundary_evidence_file_count) +
           ",\"production_blocker_count\":0,\"warning_count\":" +
           std::to_string(approval_boundary_warning_count) +
           ",\"recommendation_count\":" + std::to_string(approval_boundary_recommendation_count) +
           ",\"candidate_ready_boundary_codes\":" + format_candidate_ready_boundaries_json() +
           ",\"approval_required_boundary_codes\":" + format_approval_required_boundaries_json() +
           ",\"candidate_digest\":" + field_string(approval_boundary_candidate_digest) +
           ",\"java_v113_echo_ready\":true,\"mini_kv_v120_non_participation_ready\":true,"
           "\"candidate_counts_aligned\":true,\"candidate_ready_boundary_codes_aligned\":true,"
           "\"approval_required_boundary_codes_aligned\":true,\"interface_shape_aligned\":true,"
           "\"fake_wiring_aligned\":true,\"credential_boundary_aligned\":true,"
           "\"raw_endpoint_boundary_aligned\":true,\"connection_boundary_aligned\":true,"
           "\"write_boundary_aligned\":true,\"auto_start_boundary_aligned\":true,"
           "\"java_echo_workflow_template_applied\":true}"
           ",\"approval_required_boundaries\":{\"boundary_count\":" +
           std::to_string(approval_boundary_approval_required_decision_count) +
           ",\"boundary_codes\":" + format_approval_required_boundaries_json() +
           ",\"details\":" + format_approval_required_boundary_details_json() + "}"
           ",\"checks\":" + format_approval_boundary_checks_json() +
           ",\"source_node_v274_checks\":" + format_source_node_v274_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(approval_boundary_check_count) +
           ",\"passed_check_count\":" + std::to_string(approval_boundary_check_count) +
           ",\"source_check_count\":" + std::to_string(approval_boundary_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(approval_boundary_check_count) +
           ",\"candidate_decision_count\":" +
           std::to_string(approval_boundary_candidate_decision_count) +
           ",\"candidate_ready_decision_count\":" +
           std::to_string(approval_boundary_candidate_ready_decision_count) +
           ",\"approval_required_decision_count\":" +
           std::to_string(approval_boundary_approval_required_decision_count) +
           ",\"request_field_count\":" + std::to_string(approval_boundary_request_field_count) +
           ",\"response_field_count\":" + std::to_string(approval_boundary_response_field_count) +
           ",\"failure_class_count\":" + std::to_string(approval_boundary_failure_class_count) +
           ",\"production_blocker_count\":" +
           std::to_string(approval_boundary_production_blocker_count) +
           ",\"warning_count\":" + std::to_string(approval_boundary_warning_count) +
           ",\"recommendation_count\":" + std::to_string(approval_boundary_recommendation_count) +
           "}"
           ",\"production_blocker_codes\":[]"
           ",\"warnings\":[{\"code\":\"APPROVAL_REQUIRED_BOUNDARY_REFINEMENT_ONLY\","
           "\"severity\":\"warning\"},{\"code\":\"REAL_RESOLVER_STILL_BLOCKED\","
           "\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_PARALLEL_JAVA_V115_MINI_KV_V121\","
           "\"severity\":\"recommendation\"},{\"code\":\"VERIFY_WITH_NODE_V275_AFTER_UPSTREAM_ECHO\","
           "\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"source_node_v274_json\":" +
           field_string(approval_boundary_route_path) +
           ",\"source_node_v274_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-candidate-upstream-echo-verification?format=markdown\","
           "\"active_plan\":" + field_string(approval_boundary_active_plan) +
           ",\"recommended_parallel_java_v115\":\"Java v115 read-only echo refinement for six approval-required boundaries\","
           "\"recommended_parallel_mini_kv_v121\":\"mini-kv v121 non-participation receipt for six approval-required boundaries\"}"
           ",\"next_required_echo_versions\":[\"Java v115\",\"mini-kv v121\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_approval_required_boundary_non_participation_receipt_digest(
               read_commands)) +
           "," + format_approval_boundary_non_participation_flags_json() +
           ",\"boundary\":" + field_string(approval_boundary_boundary) +
           ",\"node_action\":" + field_string(approval_boundary_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
