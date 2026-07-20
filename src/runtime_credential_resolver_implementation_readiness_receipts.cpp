#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_receipt_json_builder.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

std::string field_string(std::string_view value) { return runtime_evidence::json_string(value); }

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
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review";
constexpr std::string_view readiness_source_v275_route_path =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-approval-required-boundary-upstream-echo-verification";
constexpr std::string_view readiness_active_plan = "docs/plans2/v280-post-status-routes-quality-roadmap.md";
constexpr std::string_view readiness_runtime_role = "runtime evidence provider only";
constexpr std::string_view readiness_previous_mini_kv_digest = "fnv1a64:4d759e7da4e5d5c0";
constexpr std::string_view readiness_previous_mini_kv_release = "v121";
constexpr std::string_view readiness_source_span = "Node v275";
constexpr std::string_view readiness_source_v275_span = "Node v274 + Java v115 + mini-kv v121";
constexpr std::string_view readiness_review_mode = "node-v281-approval-required-implementation-readiness-review-only";
constexpr std::string_view readiness_stage = "pre-implementation-echo-ready";
constexpr std::string_view readiness_implementation_stage = "blocked-until-java-v116-mini-kv-v122-and-node-v282";
constexpr std::string_view readiness_boundary =
    "credential resolver approval-required implementation readiness non-participation receipt only; mini-kv echoes "
    "Node v281 implementation readiness review, confirms the six approval-required boundaries and eighteen required "
    "artifacts, does not implement or invoke a resolver, does not read credential values, does not parse raw endpoint "
    "URLs, does not instantiate a resolver client or secret provider, does not send external requests, does not "
    "connect managed audit, does not write storage or approval ledger records, does not execute schema migration or "
    "LOAD/COMPACT/RESTORE/SETNXEX, and does not become a managed audit storage backend or order authority";
constexpr std::string_view readiness_node_action =
    "verify mini-kv v122 approval-required implementation readiness non-participation with Java v116 before Node v282 "
    "upstream echo verification";

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
           "\"java_v116_echo_hint\":\"Echo operator approval marker and manual-window evidence without executing "
           "ledger writes.\","
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
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_approval_required_implementation_"
           "readiness_review\":true}";
}

void append_readiness_non_participation_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back({"implementation_readiness_review_only", json_bool(true)});
    fields.push_back({"approval_required_implementation_readiness_non_participation_receipt_only", json_bool(true)});
    fields.push_back({"read_only_implementation_readiness_review", json_bool(true)});
    fields.push_back({"ready_for_java_v116_mini_kv_v122_echo", json_bool(true)});
    fields.push_back({"ready_for_approval_required_implementation_readiness_upstream_echo", json_bool(true)});
    fields.push_back({"ready_for_managed_audit_resolver_implementation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)});
    fields.push_back({"ready_for_production_audit", json_bool(false)});
    fields.push_back({"ready_for_production_window", json_bool(false)});
    fields.push_back({"ready_for_production_operations", json_bool(false)});
    fields.push_back({"real_resolver_implementation_allowed", json_bool(false)});
    fields.push_back({"credential_resolver_implemented", json_bool(false)});
    fields.push_back({"credential_resolver_invoked", json_bool(false)});
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_runtime_allowed", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_stored", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parse_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_included", json_bool(false)});
    fields.push_back({"external_request_allowed", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    fields.push_back({"connects_managed_audit", json_bool(false)});
    fields.push_back({"reads_managed_audit_credential", json_bool(false)});
    fields.push_back({"stores_managed_audit_credential", json_bool(false)});
    fields.push_back({"managed_audit_store", json_bool(false)});
    fields.push_back({"managed_audit_storage_backend", json_bool(false)});
    fields.push_back({"sandbox_audit_storage_backend", json_bool(false)});
    fields.push_back({"storage_write_allowed", json_bool(false)});
    fields.push_back({"write_commands_executed", json_bool(false)});
    fields.push_back({"admin_commands_executed", json_bool(false)});
    fields.push_back({"runtime_write_observed", json_bool(false)});
    fields.push_back({"approval_ledger_write_allowed", json_bool(false)});
    fields.push_back({"approval_ledger_written", json_bool(false)});
    fields.push_back({"approval_ledger_write_executed", json_bool(false)});
    fields.push_back({"managed_audit_write_executed", json_bool(false)});
    fields.push_back({"production_record_written", json_bool(false)});
    fields.push_back({"schema_migration_allowed", json_bool(false)});
    fields.push_back({"schema_migration_executed", json_bool(false)});
    fields.push_back({"schema_rehearsal_execution_allowed", json_bool(false)});
    fields.push_back({"restore_execution_allowed", json_bool(false)});
    fields.push_back({"load_restore_compact_executed", json_bool(false)});
    fields.push_back({"setnxex_execution_allowed", json_bool(false)});
    fields.push_back({"node_auto_start_allowed", json_bool(false)});
    fields.push_back({"java_auto_start_allowed", json_bool(false)});
    fields.push_back({"mini_kv_auto_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start", json_bool(false)});
    fields.push_back({"audit_authoritative", json_bool(false)});
    fields.push_back({"order_authoritative", json_bool(false)});
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
    return runtime_evidence::digest(
        "mini-kv-credential-resolver-approval-required-implementation-readiness-non-participation", parts);
}

std::string format_credential_resolver_approval_required_implementation_readiness_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v275 = json_object({
        {"source_version", json_string("Node v275")},
        {"verification_state",
         json_string("credential-resolver-approval-required-boundary-upstream-echo-verification-ready")},
        {"ready_for_approval_required_boundary_upstream_echo_verification", json_bool(true)},
        {"source_span", json_string(readiness_source_v275_span)},
        {"source_check_count", json_integer(readiness_source_v275_check_count)},
        {"source_passed_check_count", json_integer(readiness_source_v275_check_count)},
        {"approval_required_boundary_count", json_integer(readiness_boundary_count)},
        {"approval_required_boundary_codes", format_approval_required_boundaries_json()},
        {"approval_required_requirement_codes", format_requirement_codes_json()},
        {"approval_required_boundary_scope_aligned", json_bool(true)},
        {"approval_required_explanations_aligned", json_bool(true)},
        {"prohibited_runtime_actions_aligned", json_bool(true)},
        {"credential_boundary_aligned", json_bool(true)},
        {"raw_endpoint_boundary_aligned", json_bool(true)},
        {"resolver_boundary_aligned", json_bool(true)},
        {"connection_boundary_aligned", json_bool(true)},
        {"write_boundary_aligned", json_bool(true)},
        {"auto_start_boundary_aligned", json_bool(true)},
        {"real_resolver_implementation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"reads_managed_audit_credential", json_bool(false)},
        {"stores_managed_audit_credential", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });
    const auto boundary_readiness = json_object({
        {"boundary_count", json_integer(readiness_boundary_count)},
        {"echo_ready_boundary_count", json_integer(readiness_echo_ready_boundary_count)},
        {"blocked_for_implementation_boundary_count", json_integer(readiness_blocked_boundary_count)},
        {"required_artifact_count", json_integer(readiness_required_artifact_count)},
        {"boundary_codes", format_approval_required_boundaries_json()},
        {"required_artifact_codes", format_required_artifact_codes_json()},
        {"details", format_boundary_readiness_json()},
    });
    const auto summary = json_object({
        {"check_count", json_integer(readiness_check_count)},
        {"passed_check_count", json_integer(readiness_check_count)},
        {"source_check_count", json_integer(readiness_source_v275_check_count)},
        {"source_passed_check_count", json_integer(readiness_source_v275_check_count)},
        {"boundary_count", json_integer(readiness_boundary_count)},
        {"echo_ready_boundary_count", json_integer(readiness_echo_ready_boundary_count)},
        {"blocked_for_implementation_boundary_count", json_integer(readiness_blocked_boundary_count)},
        {"required_artifact_count", json_integer(readiness_required_artifact_count)},
        {"java_v116_echo_hint_count", json_integer(readiness_java_v116_hint_count)},
        {"mini_kv_v122_receipt_hint_count", json_integer(readiness_mini_kv_v122_hint_count)},
        {"production_blocker_count", json_integer(readiness_production_blocker_count)},
        {"warning_count", json_integer(readiness_warning_count)},
        {"recommendation_count", json_integer(readiness_recommendation_count)},
    });
    const auto warnings = json_array({
        json_object({{"code", json_string("IMPLEMENTATION_STILL_BLOCKED")}, {"severity", json_string("warning")}}),
        json_object({{"code", json_string("ROUTE_SPLIT_QUALITY_LINE_CLOSED")}, {"severity", json_string("warning")}}),
    });
    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_JAVA_V116_AND_MINI_KV_V122_IN_PARALLEL")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("VERIFY_WITH_NODE_V282_BEFORE_IMPLEMENTATION_DRAFT")},
                     {"severity", json_string("recommendation")}}),
    });
    const auto evidence_endpoints = json_object({
        {"approval_required_implementation_readiness_review_json", json_string(readiness_route_path)},
        {"approval_required_implementation_readiness_review_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-"
                     "implementation-readiness-review?format=markdown")},
        {"source_node_v275_json", json_string(readiness_source_v275_route_path)},
        {"source_node_v275_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-"
                     "boundary-upstream-echo-verification?format=markdown")},
        {"active_plan", json_string(readiness_active_plan)},
        {"recommended_parallel_java_v116", json_string("Java v116 approval-required implementation readiness echo")},
        {"recommended_parallel_mini_kv_v122",
         json_string("mini-kv v122 approval-required implementation non-participation readiness receipt")},
        {"next_node_verification_version", json_string("Node v282")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-approval-required-implementation-readiness-non-participation-"
                     "receipt.v1")},
        {"receipt_fixture_path", json_string(readiness_receipt_fixture_path)},
        {"source_review", json_string(readiness_source_review)},
        {"source_verification", json_string(readiness_source_verification)},
        {"consumer_hint", json_string(readiness_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(readiness_receipt_release_version)},
        {"current_artifact_path_hint", json_string(readiness_receipt_artifact_path_hint)},
        {"previous_mini_kv_release_version", json_string(readiness_previous_mini_kv_release)},
        {"previous_mini_kv_receipt_digest", json_string(readiness_previous_mini_kv_digest)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(readiness_runtime_role)},
        {"source_profile_version", json_string(readiness_profile_version)},
        {"source_route_path", json_string(readiness_route_path)},
        {"source_verification_route_path", json_string(readiness_source_v275_route_path)},
        {"source_review_state", json_string(readiness_review_state)},
        {"source_ready_for_approval_required_implementation_readiness_review", json_bool(true)},
        {"source_read_only_implementation_readiness_review", json_bool(true)},
        {"source_ready_for_java_v116_mini_kv_v122_echo", json_bool(true)},
        {"source_ready_for_managed_audit_resolver_implementation", json_bool(false)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_ready_for_production_audit", json_bool(false)},
        {"source_ready_for_production_window", json_bool(false)},
        {"source_ready_for_production_operations", json_bool(false)},
        {"source_real_resolver_implementation_allowed", json_bool(false)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_secret_provider_instantiated", json_bool(false)},
        {"source_resolver_client_instantiated", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_approval_ledger_written", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_node_v275", source_node_v275},
        {"readiness_review", format_readiness_review_json()},
        {"boundary_readiness", boundary_readiness},
        {"checks", format_readiness_checks_json()},
        {"summary", summary},
        {"production_blocker_codes", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_echo_versions", json_array({json_string("Java v116"), json_string("mini-kv v122")})},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_approval_required_implementation_readiness_non_participation_receipt_digest(
             read_commands))},
    };
    append_readiness_non_participation_fields(fields);
    fields.push_back({"boundary", json_string(readiness_boundary)});
    fields.push_back({"node_action", json_string(readiness_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
