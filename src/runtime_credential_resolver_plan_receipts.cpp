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

constexpr std::string_view plan_receipt_consumer =
    "Node v272 credential resolver pre-implementation intake upstream echo verification";
constexpr std::string_view plan_receipt_fixture_path =
    "fixtures/release/credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json";
constexpr std::string_view plan_receipt_release_version = "v119";
constexpr std::string_view plan_receipt_artifact_path_hint = "c/119/";
constexpr std::string_view plan_source =
    "Node v270 credential resolver pre-implementation plan intake";
constexpr std::string_view plan_source_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake.v1";
constexpr std::string_view plan_source_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake";
constexpr std::string_view plan_source_state = "credential-resolver-pre-implementation-plan-intake-ready";
constexpr std::string_view plan_source_v269 =
    "Node v269 credential resolver production-readiness blocked-decision upstream echo verification";
constexpr std::string_view plan_source_v269_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-blocked-decision-upstream-echo-verification.v1";
constexpr std::string_view plan_source_v269_state =
    "credential-resolver-production-readiness-blocked-decision-upstream-echo-verification-ready";
constexpr std::string_view plan_source_v269_span = "Node v268 + Java v111 + mini-kv v118";
constexpr std::string_view plan_source_v269_digest =
    "6acb216ec39c0abb11b880ee9f968f85cec25faf35c5b13f3fc4b460aee489d0";
constexpr std::string_view plan_version =
    "node-v270-credential-resolver-pre-implementation-plan-intake.v1";
constexpr std::string_view plan_mode = "plan-intake-only";
constexpr std::string_view plan_intake_mode = "node-v270-plan-intake-only";
constexpr std::string_view plan_digest =
    "3aef2d1d10fa5a0063f9be43af49122de0b38999b0d065a8addb5c3ed1f95ad8";
constexpr std::string_view plan_intake_digest =
    "43c40f8aee830a93fa36845366c948abca1353555756154b8dcbb8053a17e603";
constexpr std::string_view plan_active_plan =
    "docs/plans/v269-post-blocked-decision-upstream-echo-roadmap.md";
constexpr std::string_view plan_runtime_role = "runtime evidence provider only";
constexpr std::string_view plan_boundary =
    "credential resolver pre-implementation plan intake non-participation receipt only; mini-kv echoes Node v270 plan intake boundaries, does not carry resolver secrets, endpoint values, external request state, managed audit state, approval ledger state, audit or order authority, does not implement a resolver, instantiate a resolver client or secret provider, parse raw endpoint URLs, send external requests, write storage, or execute LOAD/COMPACT/RESTORE/SETNXEX";
constexpr std::string_view plan_node_action =
    "verify mini-kv non-participation with Node v270 credential resolver pre-implementation plan intake before Node v272 upstream echo verification";
constexpr int plan_source_check_count = 22;
constexpr int plan_source_passed_check_count = 22;
constexpr int plan_source_v268_check_count = 25;
constexpr int plan_source_v268_passed_check_count = 15;
constexpr int plan_source_missing_requirement_count = 10;
constexpr int plan_check_count = 26;
constexpr int plan_passed_check_count = 26;
constexpr int plan_boundary_count = 10;
constexpr int plan_defined_boundary_count = 10;
constexpr int plan_missing_boundary_count = 0;
constexpr int plan_prohibited_action_count = 21;
constexpr int plan_production_blocker_count = 0;
constexpr int plan_warning_count = 2;
constexpr int plan_recommendation_count = 2;

std::string format_boundary_codes_json() {
    return "[\"PLAN_DOCUMENT\",\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\","
           "\"DISABLED_SECRET_PROVIDER_STUB\",\"OPERATOR_APPROVAL\",\"ROLLBACK_BOUNDARY\","
           "\"REDACTION_POLICY\",\"EXTERNAL_REQUEST_SIMULATION\",\"SCHEMA_MIGRATION_POLICY\","
           "\"AUDIT_LEDGER_WRITE_POLICY\"]";
}

std::string format_requirement_codes_json() {
    return "[\"REAL_RESOLVER_PRE_IMPLEMENTATION_PLAN_MISSING\","
           "\"CREDENTIAL_HANDLE_BOUNDARY_MISSING\",\"ENDPOINT_HANDLE_BOUNDARY_MISSING\","
           "\"SECRET_PROVIDER_STUB_MISSING\",\"OPERATOR_APPROVAL_BOUNDARY_MISSING\","
           "\"ROLLBACK_BOUNDARY_MISSING\",\"REDACTION_POLICY_MISSING\","
           "\"EXTERNAL_REQUEST_SIMULATION_PLAN_MISSING\",\"SCHEMA_MIGRATION_POLICY_MISSING\","
           "\"AUDIT_LEDGER_WRITE_POLICY_MISSING\"]";
}

std::string format_plan_intake_flags_json() {
    return "\"plan_document_present\":true,\"credential_handle_boundary_defined\":true,"
           "\"endpoint_handle_boundary_defined\":true,\"secret_provider_stub_defined\":true,"
           "\"operator_approval_boundary_defined\":true,\"rollback_boundary_defined\":true,"
           "\"redaction_policy_defined\":true,\"external_request_simulation_defined\":true,"
           "\"schema_migration_policy_defined\":true,\"audit_ledger_write_policy_defined\":true";
}

std::string format_plan_checks_json() {
    return "{\"source_node_v269_ready\":true,\"source_node_v269_keeps_blocked_decision\":true,"
           "\"source_node_v269_keeps_real_resolver_blocked\":true," +
           format_plan_intake_flags_json() +
           ",\"all_ten_boundaries_defined\":true,\"credential_value_still_forbidden\":true,"
           "\"raw_endpoint_still_forbidden\":true,\"secret_provider_runtime_still_disabled\":true,"
           "\"real_resolver_client_still_disabled\":true,\"external_request_still_simulation_only\":true,"
           "\"schema_migration_still_review_only\":true,\"audit_ledger_write_still_review_only\":true,"
           "\"upstream_probes_still_disabled\":true,\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,\"production_window_still_blocked\":true,"
           "\"ready_for_plan_intake\":true}";
}

std::string format_plan_non_participation_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,\"plan_intake_only\":true,"
           "\"read_only_plan_intake\":true,\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt_only\":true,"
           "\"ready_for_credential_resolver_pre_implementation_plan\":true,"
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
           "\"connects_managed_audit\":false,\"managed_audit_store\":false,"
           "\"managed_audit_storage_backend\":false,\"sandbox_audit_storage_backend\":false,"
           "\"storage_write_allowed\":false,\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,\"runtime_write_observed\":false,"
           "\"approval_ledger_write_allowed\":false,\"approval_ledger_written\":false,"
           "\"managed_audit_write_executed\":false,\"production_record_written\":false,"
           "\"schema_migration_allowed\":false,\"schema_migration_executed\":false,"
           "\"schema_rehearsal_execution_allowed\":false,\"restore_execution_allowed\":false,"
           "\"load_restore_compact_executed\":false,\"setnxex_execution_allowed\":false,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"automatic_upstream_start_allowed\":false,"
           "\"automatic_upstream_start\":false,\"audit_authoritative\":false,\"order_authoritative\":false";
}

} // namespace

std::string credential_resolver_pre_implementation_plan_intake_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{plan_receipt_release_version}},
        {std::string{plan_receipt_artifact_path_hint}},
        {std::string{plan_source}},
        {std::string{plan_source_profile_version}},
        {std::string{plan_source_route_path}},
        {std::string{plan_source_state}},
        {std::string{plan_source_v269_digest}},
        {std::string{plan_digest}},
        {std::string{plan_intake_digest}},
        {std::to_string(plan_check_count)},
        {std::to_string(plan_passed_check_count)},
        {std::to_string(plan_boundary_count)},
        {std::to_string(plan_defined_boundary_count)},
        {std::to_string(plan_missing_boundary_count)},
        {std::to_string(plan_prohibited_action_count)},
        {std::to_string(plan_production_blocker_count)},
        {"defined-for-review"},
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
    return receipt_digest("mini-kv-credential-resolver-pre-implementation-plan-intake-non-participation",
                          parts);
}

std::string format_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-pre-implementation-plan-intake-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(plan_receipt_fixture_path) +
           ",\"source_plan_intake\":" + field_string(plan_source) +
           ",\"source_node_v269\":" + field_string(plan_source_v269) +
           ",\"consumer_hint\":" + field_string(plan_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(plan_receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(plan_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(plan_runtime_role) +
           ",\"source_profile_version\":" + field_string(plan_source_profile_version) +
           ",\"source_route_path\":" + field_string(plan_source_route_path) +
           ",\"source_plan_intake_state\":" + field_string(plan_source_state) +
           ",\"source_ready_for_plan_intake\":true"
           ",\"source_plan_intake_only\":true"
           ",\"source_read_only_plan_intake\":true"
           ",\"source_ready_for_credential_resolver_pre_implementation_plan\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
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
           ",\"source_node_v269_reference\":{\"source_version\":\"Node v269\","
           "\"profile_version\":" + field_string(plan_source_v269_profile_version) +
           ",\"verification_state\":" + field_string(plan_source_v269_state) +
           ",\"ready_for_blocked_decision_upstream_echo_verification\":true"
           ",\"verification_digest\":" + field_string(plan_source_v269_digest) +
           ",\"source_span\":" + field_string(plan_source_v269_span) +
           ",\"source_node_v268_ready\":true,\"java_v111_echo_ready\":true,"
           "\"mini_kv_v118_non_participation_ready\":true,\"blocked_decision_aligned\":true,"
           "\"missing_requirement_blockers_aligned\":true,\"read_only_decision_gate_aligned\":true,"
           "\"credential_boundary_aligned\":true,\"raw_endpoint_boundary_aligned\":true,"
           "\"resolver_boundary_aligned\":true,\"connection_boundary_aligned\":true,"
           "\"write_boundary_aligned\":true,\"auto_start_boundary_aligned\":true,"
           "\"check_count\":" + std::to_string(plan_source_check_count) +
           ",\"passed_check_count\":" + std::to_string(plan_source_passed_check_count) +
           ",\"source_check_count\":" + std::to_string(plan_source_v268_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(plan_source_v268_passed_check_count) +
           ",\"missing_pre_implementation_requirement_count\":" +
           std::to_string(plan_source_missing_requirement_count) +
           ",\"production_blocker_count\":0,\"warning_count\":2,\"recommendation_count\":2,"
           "\"ready_for_credential_resolver_pre_implementation_plan\":false,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"real_resolver_implementation_allowed\":false,\"execution_allowed\":false,"
           "\"connects_managed_audit\":false,\"credential_value_read\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"secret_provider_instantiated\":false,\"resolver_client_instantiated\":false,"
           "\"schema_migration_executed\":false,\"automatic_upstream_start\":false}"
           ",\"pre_implementation_plan\":{\"plan_version\":" + field_string(plan_version) +
           ",\"plan_mode\":" + field_string(plan_mode) +
           ",\"source_span\":\"Node v269\",\"plan_digest\":" + field_string(plan_digest) +
           ",\"boundary_count\":" + std::to_string(plan_boundary_count) +
           ",\"defined_boundary_count\":" + std::to_string(plan_defined_boundary_count) +
           ",\"all_required_boundaries_defined\":true,\"boundary_status\":\"defined-for-review\","
           "\"boundary_codes\":" + format_boundary_codes_json() +
           ",\"requirement_codes\":" + format_requirement_codes_json() +
           ",\"real_resolver_implementation_allowed\":false,\"secret_provider_runtime_allowed\":false,"
           "\"credential_value_read_allowed\":false,\"raw_endpoint_url_parse_allowed\":false,"
           "\"external_request_allowed\":false,\"schema_migration_allowed\":false,"
           "\"approval_ledger_write_allowed\":false,\"automatic_upstream_start_allowed\":false}"
           ",\"plan_intake\":{\"intake_mode\":" + field_string(plan_intake_mode) +
           ",\"consumed_node_version\":\"Node v269\",\"intake_digest\":" +
           field_string(plan_intake_digest) +
           ",\"required_boundary_count\":" + std::to_string(plan_boundary_count) +
           ",\"defined_boundary_count\":" + std::to_string(plan_defined_boundary_count) +
           ",\"missing_boundary_count\":" + std::to_string(plan_missing_boundary_count) + "," +
           format_plan_intake_flags_json() +
           ",\"next_java_echo_version\":\"Java v112\",\"next_mini_kv_receipt_version\":\"mini-kv v119\","
           "\"next_node_verification_version\":\"Node v272\"}"
           ",\"checks\":" + format_plan_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(plan_check_count) +
           ",\"passed_check_count\":" + std::to_string(plan_passed_check_count) +
           ",\"source_check_count\":" + std::to_string(plan_source_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(plan_source_passed_check_count) +
           ",\"boundary_count\":" + std::to_string(plan_boundary_count) +
           ",\"defined_boundary_count\":" + std::to_string(plan_defined_boundary_count) +
           ",\"missing_boundary_count\":" + std::to_string(plan_missing_boundary_count) +
           ",\"prohibited_action_count\":" + std::to_string(plan_prohibited_action_count) +
           ",\"production_blocker_count\":" + std::to_string(plan_production_blocker_count) +
           ",\"warning_count\":" + std::to_string(plan_warning_count) +
           ",\"recommendation_count\":" + std::to_string(plan_recommendation_count) + "}"
           ",\"production_blocker_codes\":[]"
           ",\"warnings\":[{\"code\":\"PLAN_INTAKE_ONLY\",\"severity\":\"warning\"},"
           "{\"code\":\"UPSTREAM_ECHO_REQUIRED_BEFORE_V272\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_V271_STATUS_ROUTES_QUALITY_BRANCH\",\"severity\":\"recommendation\"},"
           "{\"code\":\"RUN_PARALLEL_JAVA_V112_MINI_KV_V119\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"pre_implementation_plan_intake_json\":" +
           field_string(plan_source_route_path) +
           ",\"pre_implementation_plan_intake_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake?format=markdown\","
           "\"source_node_v269_json\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-blocked-decision-upstream-echo-verification\","
           "\"source_node_v269_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-blocked-decision-upstream-echo-verification?format=markdown\","
           "\"active_plan\":" + field_string(plan_active_plan) +
           ",\"next_quality_branch\":\"Node v271 statusRoutes split pre-quality branch\","
           "\"recommended_parallel_java_v112\":\"Java v112 read-only echo for Node v270 plan intake\","
           "\"recommended_parallel_mini_kv_v119\":\"mini-kv v119 non-participation receipt for Node v270 plan intake\"}"
           ",\"next_required_echo_versions\":[\"Java v112\",\"mini-kv v119\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_pre_implementation_plan_intake_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_plan_non_participation_flags_json() +
           ",\"boundary\":" + field_string(plan_boundary) +
           ",\"node_action\":" + field_string(plan_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
