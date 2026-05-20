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

constexpr std::string_view readiness_receipt_consumer =
    "Node v282 credential resolver approval-required implementation readiness upstream echo verification";
constexpr std::string_view readiness_receipt_fixture_path =
    "fixtures/release/credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json";
constexpr std::string_view readiness_receipt_release_version = "v122";
constexpr std::string_view readiness_receipt_artifact_path_hint = "d/122/";
constexpr std::string_view readiness_source_review =
    "Node v281 credential resolver approval-required implementation readiness review";
constexpr std::string_view readiness_source_verification =
    "Node v275 credential resolver approval-required boundary upstream echo verification";
constexpr std::string_view readiness_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review.v1";
constexpr std::string_view readiness_review_state =
    "credential-resolver-approval-required-implementation-readiness-review-ready";
constexpr std::string_view readiness_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review";
constexpr std::string_view readiness_source_v275_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-boundary-upstream-echo-verification";
constexpr std::string_view readiness_active_plan =
    "docs/plans2/v280-post-status-routes-quality-roadmap.md";
constexpr std::string_view readiness_runtime_role = "runtime evidence provider only";
constexpr std::string_view readiness_previous_mini_kv_digest = "fnv1a64:4d759e7da4e5d5c0";
constexpr std::string_view readiness_previous_mini_kv_release = "v121";
constexpr std::string_view readiness_source_span = "Node v275";
constexpr std::string_view readiness_source_v275_span = "Node v274 + Java v115 + mini-kv v121";
constexpr std::string_view readiness_review_mode =
    "node-v281-approval-required-implementation-readiness-review-only";
constexpr std::string_view readiness_stage = "pre-implementation-echo-ready";
constexpr std::string_view readiness_implementation_stage =
    "blocked-until-java-v116-mini-kv-v122-and-node-v282";
constexpr std::string_view readiness_boundary =
    "credential resolver approval-required implementation readiness non-participation receipt only; mini-kv echoes Node v281 implementation readiness review, confirms the six approval-required boundaries and eighteen required artifacts, does not implement or invoke a resolver, does not read credential values, does not parse raw endpoint URLs, does not instantiate a resolver client or secret provider, does not send external requests, does not connect managed audit, does not write storage or approval ledger records, does not execute schema migration or LOAD/COMPACT/RESTORE/SETNXEX, and does not become a managed audit storage backend or order authority";
constexpr std::string_view readiness_node_action =
    "verify mini-kv v122 approval-required implementation readiness non-participation with Java v116 before Node v282 upstream echo verification";

constexpr int readiness_check_count = 21;
constexpr int readiness_boundary_count = 6;
constexpr int readiness_echo_ready_boundary_count = 6;
constexpr int readiness_blocked_boundary_count = 6;
constexpr int readiness_required_artifact_count = 18;
constexpr int readiness_java_v116_hint_count = 6;
constexpr int readiness_mini_kv_v122_hint_count = 6;
constexpr int readiness_source_v275_check_count = 26;
constexpr int readiness_production_blocker_count = 0;
constexpr int readiness_warning_count = 2;
constexpr int readiness_recommendation_count = 2;

std::string format_approval_required_boundaries_json() {
    return "[\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\",\"OPERATOR_APPROVAL\","
           "\"ROLLBACK_BOUNDARY\",\"SCHEMA_MIGRATION_POLICY\",\"AUDIT_LEDGER_WRITE_POLICY\"]";
}

std::string format_requirement_codes_json() {
    return "[\"CREDENTIAL_HANDLE_BOUNDARY_MISSING\",\"ENDPOINT_HANDLE_BOUNDARY_MISSING\","
           "\"OPERATOR_APPROVAL_BOUNDARY_MISSING\",\"ROLLBACK_BOUNDARY_MISSING\","
           "\"SCHEMA_MIGRATION_POLICY_MISSING\",\"AUDIT_LEDGER_WRITE_POLICY_MISSING\"]";
}

std::string format_required_artifact_codes_json() {
    return "[\"credential-handle-review-id\",\"credential-value-redaction-contract\","
           "\"operator-visible-secret-value-prohibition\",\"endpoint-handle-review-id\","
           "\"allowlist-review-status\",\"raw-endpoint-redaction-contract\","
           "\"operator-identity-binding\",\"approval-correlation-marker\","
           "\"manual-window-open-marker\",\"rollback-abort-marker\","
           "\"restore-point-review-id\",\"manual-rollback-runbook-reference\","
           "\"schema-migration-rehearsal-id\",\"migration-review-status\","
           "\"sql-execution-prohibition-marker\",\"approval-ledger-write-policy-id\","
           "\"audit-store-write-prohibition-marker\",\"write-path-owner-review\"]";
}

std::string format_boundary_readiness_json() {
    return "[{\"code\":\"CREDENTIAL_HANDLE\",\"requirement_from_v268\":\"CREDENTIAL_HANDLE_BOUNDARY_MISSING\","
           "\"readiness_state\":\"echo-ready-implementation-blocked\","
           "\"implementation_disposition\":\"requires-explicit-follow-up-artifacts\","
           "\"owner\":\"security\",\"required_artifacts\":[\"credential-handle-review-id\","
           "\"credential-value-redaction-contract\",\"operator-visible-secret-value-prohibition\"],"
           "\"java_v116_echo_hint\":\"Echo credential handle review id without credential value fields.\","
           "\"mini_kv_v122_receipt_hint\":\"Confirm no credential value load/store/include behavior.\","
           "\"node_v282_verification_hint\":\"Verify handle-only evidence and value-redaction invariants.\","
           "\"prohibited_runtime_actions\":[\"read-credential-value\",\"store-credential-value\","
           "\"render-credential-value\"],\"ready_for_java_v116_echo\":true,"
           "\"ready_for_mini_kv_v122_receipt\":true,\"ready_for_node_v282_verification\":false,"
           "\"ready_for_runtime_implementation\":false},"
           "{\"code\":\"ENDPOINT_HANDLE\",\"requirement_from_v268\":\"ENDPOINT_HANDLE_BOUNDARY_MISSING\","
           "\"readiness_state\":\"echo-ready-implementation-blocked\","
           "\"implementation_disposition\":\"requires-explicit-follow-up-artifacts\","
           "\"owner\":\"security\",\"required_artifacts\":[\"endpoint-handle-review-id\","
           "\"allowlist-review-status\",\"raw-endpoint-redaction-contract\"],"
           "\"java_v116_echo_hint\":\"Echo endpoint handle and allowlist review status without raw URL.\","
           "\"mini_kv_v122_receipt_hint\":\"Confirm no raw endpoint parse/include/connect behavior.\","
           "\"node_v282_verification_hint\":\"Verify handle-only endpoint evidence and no raw URL shape drift.\","
           "\"prohibited_runtime_actions\":[\"parse-raw-endpoint-url\",\"render-raw-endpoint-url\","
           "\"connect-managed-audit\"],\"ready_for_java_v116_echo\":true,"
           "\"ready_for_mini_kv_v122_receipt\":true,\"ready_for_node_v282_verification\":false,"
           "\"ready_for_runtime_implementation\":false},"
           "{\"code\":\"OPERATOR_APPROVAL\",\"requirement_from_v268\":\"OPERATOR_APPROVAL_BOUNDARY_MISSING\","
           "\"readiness_state\":\"echo-ready-implementation-blocked\","
           "\"implementation_disposition\":\"requires-explicit-follow-up-artifacts\","
           "\"owner\":\"operator\",\"required_artifacts\":[\"operator-identity-binding\","
           "\"approval-correlation-marker\",\"manual-window-open-marker\"],"
           "\"java_v116_echo_hint\":\"Echo operator approval marker and manual-window evidence without executing ledger writes.\","
           "\"mini_kv_v122_receipt_hint\":\"Confirm no auto-start and no approval side effects.\","
           "\"node_v282_verification_hint\":\"Verify operator marker completeness before any later dry-run shell.\","
           "\"prohibited_runtime_actions\":[\"execute-without-operator-marker\","
           "\"auto-approve-operation\",\"auto-start-upstream\"],\"ready_for_java_v116_echo\":true,"
           "\"ready_for_mini_kv_v122_receipt\":true,\"ready_for_node_v282_verification\":false,"
           "\"ready_for_runtime_implementation\":false},"
           "{\"code\":\"ROLLBACK_BOUNDARY\",\"requirement_from_v268\":\"ROLLBACK_BOUNDARY_MISSING\","
           "\"readiness_state\":\"echo-ready-implementation-blocked\","
           "\"implementation_disposition\":\"requires-explicit-follow-up-artifacts\","
           "\"owner\":\"release-manager\",\"required_artifacts\":[\"rollback-abort-marker\","
           "\"restore-point-review-id\",\"manual-rollback-runbook-reference\"],"
           "\"java_v116_echo_hint\":\"Echo rollback abort marker and restore review id without executing rollback.\","
           "\"mini_kv_v122_receipt_hint\":\"Confirm no LOAD/RESTORE/COMPACT and no authority over rollback state.\","
           "\"node_v282_verification_hint\":\"Verify rollback guard evidence stays separate from execution.\","
           "\"prohibited_runtime_actions\":[\"execute-rollback\","
           "\"deploy-resolver-without-abort-marker\",\"write-production-record\"],"
           "\"ready_for_java_v116_echo\":true,\"ready_for_mini_kv_v122_receipt\":true,"
           "\"ready_for_node_v282_verification\":false,\"ready_for_runtime_implementation\":false},"
           "{\"code\":\"SCHEMA_MIGRATION_POLICY\","
           "\"requirement_from_v268\":\"SCHEMA_MIGRATION_POLICY_MISSING\","
           "\"readiness_state\":\"echo-ready-implementation-blocked\","
           "\"implementation_disposition\":\"requires-explicit-follow-up-artifacts\","
           "\"owner\":\"release-manager\",\"required_artifacts\":[\"schema-migration-rehearsal-id\","
           "\"migration-review-status\",\"sql-execution-prohibition-marker\"],"
           "\"java_v116_echo_hint\":\"Echo schema migration rehearsal id without executing SQL.\","
           "\"mini_kv_v122_receipt_hint\":\"Confirm no admin command or schema/storage mutation participates.\","
           "\"node_v282_verification_hint\":\"Verify schema migration remains review-only.\","
           "\"prohibited_runtime_actions\":[\"execute-schema-migration\",\"execute-sql\","
           "\"mutate-managed-audit-schema\"],\"ready_for_java_v116_echo\":true,"
           "\"ready_for_mini_kv_v122_receipt\":true,\"ready_for_node_v282_verification\":false,"
           "\"ready_for_runtime_implementation\":false},"
           "{\"code\":\"AUDIT_LEDGER_WRITE_POLICY\","
           "\"requirement_from_v268\":\"AUDIT_LEDGER_WRITE_POLICY_MISSING\","
           "\"readiness_state\":\"echo-ready-implementation-blocked\","
           "\"implementation_disposition\":\"requires-explicit-follow-up-artifacts\","
           "\"owner\":\"node\",\"required_artifacts\":[\"approval-ledger-write-policy-id\","
           "\"audit-store-write-prohibition-marker\",\"write-path-owner-review\"],"
           "\"java_v116_echo_hint\":\"Echo ledger write policy id without writing approval ledger.\","
           "\"mini_kv_v122_receipt_hint\":\"Confirm no storage/backend/write participation.\","
           "\"node_v282_verification_hint\":\"Verify all write paths stay blocked until an explicit later plan.\","
           "\"prohibited_runtime_actions\":[\"write-approval-ledger\","
           "\"write-managed-audit-state\",\"write-storage\"],\"ready_for_java_v116_echo\":true,"
           "\"ready_for_mini_kv_v122_receipt\":true,\"ready_for_node_v282_verification\":false,"
           "\"ready_for_runtime_implementation\":false}]";
}

std::string format_readiness_review_json() {
    return "{\"review_mode\":" + field_string(readiness_review_mode) +
           ",\"source_span\":" + field_string(readiness_source_span) +
           ",\"readiness_stage\":" + field_string(readiness_stage) +
           ",\"implementation_stage\":" + field_string(readiness_implementation_stage) +
           ",\"all_approval_required_boundaries_echo_ready\":true,"
           "\"all_approval_required_boundaries_implementation_blocked\":true,"
           "\"all_required_artifacts_named\":true,\"java_v116_echo_recommended\":true,"
           "\"mini_kv_v122_receipt_recommended\":true,\"node_v282_verification_required\":true,"
           "\"route_split_quality_line_closed\":true}";
}

std::string format_readiness_checks_json() {
    return "{\"source_node_v275_ready\":true,\"source_boundary_count_expected\":true,"
           "\"source_boundary_codes_aligned\":true,\"source_keeps_read_only_echo_only\":true,"
           "\"source_keeps_real_resolver_blocked\":true,\"source_keeps_credential_boundary_closed\":true,"
           "\"source_keeps_raw_endpoint_boundary_closed\":true,\"source_keeps_connection_boundary_closed\":true,"
           "\"source_keeps_write_boundary_closed\":true,\"source_keeps_auto_start_boundary_closed\":true,"
           "\"boundary_readiness_count_expected\":true,\"all_boundaries_echo_ready_for_java_v116\":true,"
           "\"all_boundaries_echo_ready_for_mini_kv_v122\":true,"
           "\"all_boundaries_still_blocked_for_runtime_implementation\":true,"
           "\"all_required_artifacts_named\":true,\"route_split_quality_line_closed\":true,"
           "\"upstream_probes_still_disabled\":true,\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,\"production_window_still_blocked\":true,"
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_approval_required_implementation_readiness_review\":true}";
}

std::string format_readiness_non_participation_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"implementation_readiness_review_only\":true,"
           "\"approval_required_implementation_readiness_non_participation_receipt_only\":true,"
           "\"read_only_implementation_readiness_review\":true,"
           "\"ready_for_java_v116_mini_kv_v122_echo\":true,"
           "\"ready_for_approval_required_implementation_readiness_upstream_echo\":true,"
           "\"ready_for_managed_audit_resolver_implementation\":false,"
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

std::string credential_resolver_approval_required_implementation_readiness_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{readiness_receipt_release_version}},
        {std::string{readiness_receipt_artifact_path_hint}},
        {std::string{readiness_source_review}},
        {std::string{readiness_profile_version}},
        {std::string{readiness_review_state}},
        {std::string{readiness_review_mode}},
        {std::string{readiness_stage}},
        {std::string{readiness_implementation_stage}},
        {std::string{readiness_previous_mini_kv_digest}},
        {std::to_string(readiness_check_count)},
        {std::to_string(readiness_boundary_count)},
        {std::to_string(readiness_required_artifact_count)},
        {std::to_string(readiness_java_v116_hint_count)},
        {std::to_string(readiness_mini_kv_v122_hint_count)},
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
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest(
        "mini-kv-credential-resolver-approval-required-implementation-readiness-non-participation",
        parts);
}

std::string format_credential_resolver_approval_required_implementation_readiness_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-approval-required-implementation-readiness-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(readiness_receipt_fixture_path) +
           ",\"source_review\":" + field_string(readiness_source_review) +
           ",\"source_verification\":" + field_string(readiness_source_verification) +
           ",\"consumer_hint\":" + field_string(readiness_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(readiness_receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(readiness_receipt_artifact_path_hint) +
           ",\"previous_mini_kv_release_version\":" + field_string(readiness_previous_mini_kv_release) +
           ",\"previous_mini_kv_receipt_digest\":" + field_string(readiness_previous_mini_kv_digest) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(readiness_runtime_role) +
           ",\"source_profile_version\":" + field_string(readiness_profile_version) +
           ",\"source_route_path\":" + field_string(readiness_route_path) +
           ",\"source_verification_route_path\":" + field_string(readiness_source_v275_route_path) +
           ",\"source_review_state\":" + field_string(readiness_review_state) +
           ",\"source_ready_for_approval_required_implementation_readiness_review\":true"
           ",\"source_read_only_implementation_readiness_review\":true"
           ",\"source_ready_for_java_v116_mini_kv_v122_echo\":true"
           ",\"source_ready_for_managed_audit_resolver_implementation\":false"
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
           ",\"source_node_v275\":{\"source_version\":\"Node v275\","
           "\"verification_state\":\"credential-resolver-approval-required-boundary-upstream-echo-verification-ready\","
           "\"ready_for_approval_required_boundary_upstream_echo_verification\":true,"
           "\"source_span\":" + field_string(readiness_source_v275_span) +
           ",\"source_check_count\":" + std::to_string(readiness_source_v275_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(readiness_source_v275_check_count) +
           ",\"approval_required_boundary_count\":" + std::to_string(readiness_boundary_count) +
           ",\"approval_required_boundary_codes\":" + format_approval_required_boundaries_json() +
           ",\"approval_required_requirement_codes\":" + format_requirement_codes_json() +
           ",\"approval_required_boundary_scope_aligned\":true,"
           "\"approval_required_explanations_aligned\":true,"
           "\"prohibited_runtime_actions_aligned\":true,"
           "\"credential_boundary_aligned\":true,\"raw_endpoint_boundary_aligned\":true,"
           "\"resolver_boundary_aligned\":true,\"connection_boundary_aligned\":true,"
           "\"write_boundary_aligned\":true,\"auto_start_boundary_aligned\":true,"
           "\"real_resolver_implementation_allowed\":false,\"execution_allowed\":false,"
           "\"connects_managed_audit\":false,\"reads_managed_audit_credential\":false,"
           "\"stores_managed_audit_credential\":false,\"credential_value_read\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"secret_provider_instantiated\":false,\"resolver_client_instantiated\":false,"
           "\"schema_migration_executed\":false,\"approval_ledger_written\":false,"
           "\"automatic_upstream_start\":false}"
           ",\"readiness_review\":" + format_readiness_review_json() +
           ",\"boundary_readiness\":{\"boundary_count\":" + std::to_string(readiness_boundary_count) +
           ",\"echo_ready_boundary_count\":" + std::to_string(readiness_echo_ready_boundary_count) +
           ",\"blocked_for_implementation_boundary_count\":" +
           std::to_string(readiness_blocked_boundary_count) +
           ",\"required_artifact_count\":" + std::to_string(readiness_required_artifact_count) +
           ",\"boundary_codes\":" + format_approval_required_boundaries_json() +
           ",\"required_artifact_codes\":" + format_required_artifact_codes_json() +
           ",\"details\":" + format_boundary_readiness_json() + "}"
           ",\"checks\":" + format_readiness_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(readiness_check_count) +
           ",\"passed_check_count\":" + std::to_string(readiness_check_count) +
           ",\"source_check_count\":" + std::to_string(readiness_source_v275_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(readiness_source_v275_check_count) +
           ",\"boundary_count\":" + std::to_string(readiness_boundary_count) +
           ",\"echo_ready_boundary_count\":" + std::to_string(readiness_echo_ready_boundary_count) +
           ",\"blocked_for_implementation_boundary_count\":" +
           std::to_string(readiness_blocked_boundary_count) +
           ",\"required_artifact_count\":" + std::to_string(readiness_required_artifact_count) +
           ",\"java_v116_echo_hint_count\":" + std::to_string(readiness_java_v116_hint_count) +
           ",\"mini_kv_v122_receipt_hint_count\":" +
           std::to_string(readiness_mini_kv_v122_hint_count) +
           ",\"production_blocker_count\":" + std::to_string(readiness_production_blocker_count) +
           ",\"warning_count\":" + std::to_string(readiness_warning_count) +
           ",\"recommendation_count\":" + std::to_string(readiness_recommendation_count) + "}"
           ",\"production_blocker_codes\":[]"
           ",\"warnings\":[{\"code\":\"IMPLEMENTATION_STILL_BLOCKED\","
           "\"severity\":\"warning\"},{\"code\":\"ROUTE_SPLIT_QUALITY_LINE_CLOSED\","
           "\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V116_AND_MINI_KV_V122_IN_PARALLEL\","
           "\"severity\":\"recommendation\"},{\"code\":\"VERIFY_WITH_NODE_V282_BEFORE_IMPLEMENTATION_DRAFT\","
           "\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"approval_required_implementation_readiness_review_json\":" +
           field_string(readiness_route_path) +
           ",\"approval_required_implementation_readiness_review_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review?format=markdown\","
           "\"source_node_v275_json\":" + field_string(readiness_source_v275_route_path) +
           ",\"source_node_v275_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-boundary-upstream-echo-verification?format=markdown\","
           "\"active_plan\":" + field_string(readiness_active_plan) +
           ",\"recommended_parallel_java_v116\":\"Java v116 approval-required implementation readiness echo\","
           "\"recommended_parallel_mini_kv_v122\":\"mini-kv v122 approval-required implementation non-participation readiness receipt\","
           "\"next_node_verification_version\":\"Node v282\"}"
           ",\"next_required_echo_versions\":[\"Java v116\",\"mini-kv v122\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_approval_required_implementation_readiness_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_readiness_non_participation_flags_json() +
           ",\"boundary\":" + field_string(readiness_boundary) +
           ",\"node_action\":" + field_string(readiness_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
