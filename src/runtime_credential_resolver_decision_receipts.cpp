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

constexpr std::string_view blocked_receipt_consumer =
    "Node v269 credential resolver production-readiness blocked-decision upstream echo verification";
constexpr std::string_view blocked_receipt_fixture_path =
    "fixtures/release/credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json";
constexpr std::string_view blocked_receipt_release_version = "v118";
constexpr std::string_view blocked_receipt_artifact_path_hint = "c/118/";
constexpr std::string_view blocked_source_decision =
    "Node v268 credential resolver production readiness decision gate";
constexpr std::string_view blocked_source_upstream_echo =
    "Node v267 credential resolver fake-shell archive upstream echo verification";
constexpr std::string_view blocked_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-decision-gate.v1";
constexpr std::string_view blocked_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-decision-gate";
constexpr std::string_view blocked_decision_gate_state = "blocked";
constexpr std::string_view blocked_readiness_decision = "blocked";
constexpr std::string_view blocked_decision_mode = "node-v268-production-readiness-decision-gate-only";
constexpr std::string_view blocked_source_span = "Node v267";
constexpr std::string_view blocked_source_v267_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-upstream-echo-verification.v1";
constexpr std::string_view blocked_source_v267_state =
    "credential-resolver-fake-shell-archive-upstream-echo-verification-ready";
constexpr std::string_view blocked_source_v267_span = "Node v266 + Java v110 + mini-kv v117";
constexpr std::string_view blocked_source_v267_digest =
    "cfd4ff6b893469476d122647f082f590ae381407d697f40f7378a0b4b31e7a39";
constexpr std::string_view blocked_decision_digest =
    "2c35360789f4f6755fa66360a669968accfa7135a1f645287203801cd04de702";
constexpr std::string_view blocked_active_plan = "docs/plans/v266-post-fake-shell-archive-roadmap.md";
constexpr std::string_view blocked_next_plan =
    "docs/plans/v268-post-production-readiness-decision-roadmap.md";
constexpr std::string_view blocked_runtime_role = "runtime evidence provider only";
constexpr std::string_view blocked_decision_reason =
    "Node v267 proves the fake-shell archive echo chain is aligned, but no approved pre-implementation plan defines credential handle, endpoint handle, secret provider stub, operator approval, rollback, redaction, simulation, schema migration, or ledger write boundaries.";
constexpr std::string_view blocked_boundary =
    "credential resolver production-readiness blocked-decision non-participation receipt only; mini-kv echoes Node v268 blocked decision gate boundaries, does not implement a resolver, does not instantiate a resolver client or secret provider, does not read credential values, does not parse raw endpoint URLs, does not send external requests, does not connect managed audit, does not write storage or approval ledger records, does not execute schema migration or restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view blocked_node_action =
    "verify mini-kv non-participation with Node v268 blocked production readiness decision before Node v269 upstream echo verification";
constexpr int blocked_source_check_count = 18;
constexpr int blocked_source_archive_file_count = 9;
constexpr int blocked_source_evidence_file_count = 7;
constexpr int blocked_source_required_snippet_count = 24;
constexpr int blocked_source_matched_snippet_count = 32;
constexpr int blocked_check_count = 25;
constexpr int blocked_passed_check_count = 15;
constexpr int blocked_missing_requirement_count = 10;
constexpr int blocked_production_blocker_count = 10;
constexpr int blocked_warning_count = 2;
constexpr int blocked_recommendation_count = 2;

std::string format_common_blocked_boundary_json() {
    return "\"read_only\":true,\"execution_allowed\":false,\"blocked_decision_only\":true,"
           "\"production_readiness_gate_only\":true,\"read_only_decision_gate\":true,"
           "\"ready_for_credential_resolver_pre_implementation_plan\":false,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"ready_for_production_audit\":false,\"ready_for_production_window\":false,"
           "\"ready_for_production_operations\":false,"
           "\"real_resolver_implementation_allowed\":false,"
           "\"credential_resolver_implemented\":false,\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,\"secret_provider_instantiated\":false,"
           "\"secret_provider_stub_defined\":false,\"secret_provider_runtime_allowed\":false,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"external_audit_service_auto_start_allowed\":false,"
           "\"connection_execution_allowed\":false,\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,\"runtime_write_observed\":false,"
           "\"managed_audit_store\":false,\"managed_audit_storage_backend\":false,"
           "\"sandbox_audit_storage_backend\":false,\"storage_write_allowed\":false,"
           "\"managed_audit_write_executed\":false,\"production_record_written\":false,"
           "\"approval_ledger_write_allowed\":false,\"approval_ledger_write_executed\":false,"
           "\"sandbox_managed_audit_state_write_allowed\":false,"
           "\"credential_value_required\":false,\"credential_value_read_allowed\":false,"
           "\"credential_value_loaded\":false,\"credential_value_stored\":false,"
           "\"credential_value_included\":false,\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_included\":false,\"external_request_sent\":false,"
           "\"schema_rehearsal_execution_allowed\":false,\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"order_authoritative\":false";
}

std::string format_pre_implementation_requirements_json() {
    return "{\"plan_document_present\":false,\"credential_handle_boundary_defined\":false,"
           "\"endpoint_handle_boundary_defined\":false,\"secret_provider_stub_defined\":false,"
           "\"operator_approval_boundary_defined\":false,\"rollback_boundary_defined\":false,"
           "\"redaction_policy_defined\":false,\"external_request_simulation_defined\":false,"
           "\"schema_migration_policy_defined\":false,\"audit_ledger_write_policy_defined\":false,"
           "\"missing_requirement_count\":" +
           std::to_string(blocked_missing_requirement_count) + "}";
}

std::string format_blocker_codes_json() {
    return "[\"REAL_RESOLVER_PRE_IMPLEMENTATION_PLAN_MISSING\","
           "\"CREDENTIAL_HANDLE_BOUNDARY_MISSING\",\"ENDPOINT_HANDLE_BOUNDARY_MISSING\","
           "\"SECRET_PROVIDER_STUB_MISSING\",\"OPERATOR_APPROVAL_BOUNDARY_MISSING\","
           "\"ROLLBACK_BOUNDARY_MISSING\",\"REDACTION_POLICY_MISSING\","
           "\"EXTERNAL_REQUEST_SIMULATION_PLAN_MISSING\",\"SCHEMA_MIGRATION_POLICY_MISSING\","
           "\"AUDIT_LEDGER_WRITE_POLICY_MISSING\"]";
}

std::string format_blocked_checks_json() {
    return "{\"decision_gate_evaluated\":true,\"source_node_v267_ready\":true,"
           "\"source_node_v267_blocks_real_resolver\":true,\"archive_echo_chain_ready\":true,"
           "\"credential_boundary_still_closed\":true,\"raw_endpoint_boundary_still_closed\":true,"
           "\"resolver_boundary_still_closed\":true,\"connection_boundary_still_closed\":true,"
           "\"write_boundary_still_closed\":true,\"auto_start_boundary_still_closed\":true,"
           "\"upstream_probes_still_disabled\":true,\"upstream_actions_still_disabled\":true,"
           "\"pre_implementation_plan_present\":false,\"credential_handle_boundary_defined\":false,"
           "\"endpoint_handle_boundary_defined\":false,\"secret_provider_stub_defined\":false,"
           "\"operator_approval_boundary_defined\":false,\"rollback_boundary_defined\":false,"
           "\"redaction_policy_defined\":false,\"external_request_simulation_defined\":false,"
           "\"schema_migration_policy_defined\":false,\"audit_ledger_write_policy_defined\":false,"
           "\"production_audit_still_blocked\":true,\"production_window_still_blocked\":true,"
           "\"real_resolver_implementation_still_blocked\":true}";
}

} // namespace

std::string credential_resolver_production_readiness_blocked_decision_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{blocked_receipt_release_version}},
        {std::string{blocked_receipt_artifact_path_hint}},
        {std::string{blocked_source_decision}},
        {std::string{blocked_source_upstream_echo}},
        {std::string{blocked_profile_version}},
        {std::string{blocked_route_path}},
        {std::string{blocked_decision_gate_state}},
        {std::string{blocked_readiness_decision}},
        {std::string{blocked_decision_mode}},
        {std::string{blocked_source_v267_digest}},
        {std::string{blocked_decision_digest}},
        {std::to_string(blocked_source_check_count)},
        {std::to_string(blocked_source_archive_file_count)},
        {std::to_string(blocked_source_evidence_file_count)},
        {std::to_string(blocked_source_required_snippet_count)},
        {std::to_string(blocked_source_matched_snippet_count)},
        {std::to_string(blocked_check_count)},
        {std::to_string(blocked_passed_check_count)},
        {std::to_string(blocked_missing_requirement_count)},
        {std::to_string(blocked_production_blocker_count)},
        {std::to_string(blocked_warning_count)},
        {std::to_string(blocked_recommendation_count)},
        {"blocked"},
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
    return receipt_digest("mini-kv-credential-resolver-production-readiness-blocked-decision-non-participation",
                          parts);
}

std::string format_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-production-readiness-blocked-decision-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(blocked_receipt_fixture_path) +
           ",\"source_decision_gate\":" + field_string(blocked_source_decision) +
           ",\"source_upstream_echo\":" + field_string(blocked_source_upstream_echo) +
           ",\"consumer_hint\":" + field_string(blocked_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(blocked_receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(blocked_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(blocked_runtime_role) +
           ",\"source_profile_version\":" + field_string(blocked_profile_version) +
           ",\"source_route_path\":" + field_string(blocked_route_path) +
           ",\"source_decision_gate_state\":" + field_string(blocked_decision_gate_state) +
           ",\"source_readiness_decision\":" + field_string(blocked_readiness_decision) +
           ",\"source_decision_gate_evaluated\":true"
           ",\"source_production_readiness_gate_only\":true"
           ",\"source_read_only_decision_gate\":true"
           ",\"source_ready_for_credential_resolver_pre_implementation_plan\":false"
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
           ",\"source_automatic_upstream_start\":false"
           ",\"source_node_v267\":{\"source_version\":\"Node v267\","
           "\"profile_version\":" + field_string(blocked_source_v267_profile_version) +
           ",\"verification_state\":" + field_string(blocked_source_v267_state) +
           ",\"ready_for_upstream_echo_verification\":true"
           ",\"verification_digest\":" + field_string(blocked_source_v267_digest) +
           ",\"source_span\":" + field_string(blocked_source_v267_span) +
           ",\"source_node_v266_ready\":true,\"java_v110_echo_ready\":true,"
           "\"mini_kv_v117_non_participation_ready\":true,"
           "\"archive_counts_aligned\":true,\"archive_snippets_aligned\":true,"
           "\"archive_no_rerun_aligned\":true,\"credential_boundary_aligned\":true,"
           "\"raw_endpoint_boundary_aligned\":true,\"connection_boundary_aligned\":true,"
           "\"write_boundary_aligned\":true,\"auto_start_boundary_aligned\":true,"
           "\"node_v267_blocks_real_resolver\":true,\"check_count\":" +
           std::to_string(blocked_source_check_count) +
           ",\"passed_check_count\":" + std::to_string(blocked_source_check_count) +
           ",\"archive_file_count\":" + std::to_string(blocked_source_archive_file_count) +
           ",\"evidence_file_count\":" + std::to_string(blocked_source_evidence_file_count) +
           ",\"required_snippet_count\":" + std::to_string(blocked_source_required_snippet_count) +
           ",\"matched_snippet_count\":" + std::to_string(blocked_source_matched_snippet_count) +
           ",\"production_blocker_count\":0,\"warning_count\":2,\"recommendation_count\":2,"
           "\"read_only_upstream_echo_verification\":true,\"archive_verification_only\":true,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"execution_allowed\":false,\"connects_managed_audit\":false,"
           "\"reads_managed_audit_credential\":false,\"stores_managed_audit_credential\":false,"
           "\"credential_value_read\":false,\"raw_endpoint_url_parsed\":false,"
           "\"external_request_sent\":false,\"secret_provider_instantiated\":false,"
           "\"resolver_client_instantiated\":false,\"schema_migration_executed\":false,"
           "\"automatic_upstream_start\":false}"
           ",\"pre_implementation_requirements\":" + format_pre_implementation_requirements_json() +
           ",\"production_readiness_decision\":{\"decision_digest\":" +
           field_string(blocked_decision_digest) +
           ",\"decision_mode\":" + field_string(blocked_decision_mode) +
           ",\"source_span\":" + field_string(blocked_source_span) +
           ",\"decision\":\"blocked\",\"reason\":" + field_string(blocked_decision_reason) +
           ",\"allows_real_resolver_pre_implementation_plan\":false,"
           "\"allows_real_credential_resolver_implementation\":false,"
           "\"allows_secret_provider_stub\":false,\"allows_secret_provider_runtime\":false,"
           "\"allows_credential_value_read\":false,\"allows_raw_endpoint_url_parse\":false,"
           "\"allows_external_request\":false,\"allows_managed_audit_connection\":false,"
           "\"allows_schema_migration\":false,\"allows_approval_ledger_write\":false,"
           "\"allows_automatic_upstream_start\":false,\"next_plan_required_before_implementation\":true}"
           ",\"checks\":" + format_blocked_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(blocked_check_count) +
           ",\"passed_check_count\":" + std::to_string(blocked_passed_check_count) +
           ",\"source_check_count\":" + std::to_string(blocked_source_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(blocked_source_check_count) +
           ",\"archive_file_count\":" + std::to_string(blocked_source_archive_file_count) +
           ",\"evidence_file_count\":" + std::to_string(blocked_source_evidence_file_count) +
           ",\"required_snippet_count\":" + std::to_string(blocked_source_required_snippet_count) +
           ",\"matched_snippet_count\":" + std::to_string(blocked_source_matched_snippet_count) +
           ",\"missing_pre_implementation_requirement_count\":" +
           std::to_string(blocked_missing_requirement_count) +
           ",\"production_blocker_count\":" + std::to_string(blocked_production_blocker_count) +
           ",\"warning_count\":" + std::to_string(blocked_warning_count) +
           ",\"recommendation_count\":" + std::to_string(blocked_recommendation_count) + "}"
           ",\"production_blocker_codes\":" + format_blocker_codes_json() +
           ",\"warnings\":[{\"code\":\"DECISION_GATE_ONLY\",\"severity\":\"warning\"},"
           "{\"code\":\"SOURCE_CHAIN_READY_BUT_NOT_PRODUCTION_READY\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"WRITE_SUCCESSOR_PLAN\",\"severity\":\"recommendation\"},"
           "{\"code\":\"REQUEST_PARALLEL_UPSTREAM_ECHO\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"production_readiness_decision_gate_json\":" +
           field_string(blocked_route_path) +
           ",\"production_readiness_decision_gate_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-decision-gate?format=markdown\","
           "\"source_node_v267_json\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-upstream-echo-verification\","
           "\"source_node_v267_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-upstream-echo-verification?format=markdown\","
           "\"active_plan\":" + field_string(blocked_active_plan) +
           ",\"next_plan\":" + field_string(blocked_next_plan) + "}"
           ",\"next_required_echo_versions\":[\"Java v111\",\"mini-kv v118\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_production_readiness_blocked_decision_non_participation_receipt_digest(
                   read_commands)) +
           ",\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt_only\":true," +
           format_common_blocked_boundary_json() +
           ",\"boundary\":" + field_string(blocked_boundary) +
           ",\"node_action\":" + field_string(blocked_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
