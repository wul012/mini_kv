#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"

#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::field_string;
using credential_resolver_detail::format_common_credential_boundary_json;
using credential_resolver_detail::receipt_digest;

constexpr std::string_view test_only_resolver_shell_receipt_consumer =
    "Node v265 test-only resolver shell upstream echo verification";
constexpr std::string_view test_only_resolver_shell_receipt_fixture_path =
    "fixtures/release/test-only-resolver-shell-non-participation-receipt.json";
constexpr std::string_view test_only_resolver_shell_receipt_release_version = "v116";
constexpr std::string_view test_only_resolver_shell_receipt_artifact_path_hint = "c/116/";
constexpr std::string_view test_only_resolver_shell_source_contract =
    "Node v264 credential resolver test-only shell contract";
constexpr std::string_view test_only_resolver_shell_source_verification =
    "Node v263 disabled resolver precheck upstream echo verification";
constexpr std::string_view test_only_resolver_shell_profile_version =
    "managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-contract.v1";
constexpr std::string_view test_only_resolver_shell_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-contract";
constexpr std::string_view test_only_resolver_shell_state =
    "sandbox-endpoint-credential-resolver-test-only-shell-contract-ready";
constexpr std::string_view test_only_resolver_shell_mode =
    "test-only-fake-resolver-contract";
constexpr std::string_view test_only_resolver_shell_name =
    "ManagedAuditSandboxEndpointCredentialResolverTestOnlyShell";
constexpr std::string_view test_only_resolver_shell_kind = "fake-in-memory";
constexpr std::string_view test_only_resolver_shell_probe_id =
    "managed-audit-v264-test-only-resolver-shell-probe";
constexpr std::string_view test_only_resolver_shell_probe_status =
    "fake-resolver-accepted";
constexpr std::string_view test_only_resolver_shell_probe_code =
    "TEST_ONLY_FAKE_RESOLVER";
constexpr std::string_view test_only_resolver_shell_source_v263_state =
    "sandbox-endpoint-credential-resolver-disabled-precheck-upstream-echo-verification-ready";
constexpr std::string_view test_only_resolver_shell_source_v263_mode =
    "java-v106-plus-mini-kv-v115-disabled-credential-resolver-precheck-upstream-echo-verification-only";
constexpr std::string_view test_only_resolver_shell_source_span =
    "Node v262 + Java v106 + mini-kv v115";
constexpr std::string_view test_only_resolver_shell_runtime_role =
    "runtime evidence provider only";
constexpr std::string_view test_only_resolver_shell_boundary =
    "test-only resolver shell non-participation receipt only; mini-kv echoes Node v264 fake resolver shell contract boundaries, does not instantiate a resolver client or secret provider, does not accept or read credential values, does not parse raw endpoint URLs, does not send external requests, does not connect managed audit, does not write storage or production records, does not execute schema migration or restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view test_only_resolver_shell_node_action =
    "verify mini-kv non-participation with Node v264 test-only resolver shell contract before Node v265 upstream echo verification";
constexpr int test_only_resolver_shell_request_shape_field_count = 9;
constexpr int test_only_resolver_shell_response_shape_field_count = 13;
constexpr int test_only_resolver_shell_failure_mapping_count = 7;
constexpr int test_only_resolver_shell_guard_condition_count = 10;
constexpr int test_only_resolver_shell_check_count = 20;
constexpr int test_only_resolver_shell_warning_count = 2;
constexpr int test_only_resolver_shell_recommendation_count = 2;
constexpr int test_only_resolver_shell_source_v263_check_count = 19;

std::string format_test_only_resolver_request_fields_json() {
    return "[\"requestId\",\"operation\",\"credentialHandle\",\"endpointHandle\","
           "\"resolverPolicyHandle\",\"approvalMarker\",\"approvalCorrelationId\","
           "\"dryRun\",\"fakeResolverOnly\"]";
}

std::string format_test_only_resolver_response_fields_json() {
    return "[\"requestId\",\"status\",\"code\",\"fakeResolverOnly\","
           "\"resolverClientInstantiated\",\"secretProviderInstantiated\","
           "\"credentialValueRead\",\"credentialValueLoaded\",\"rawEndpointUrlParsed\","
           "\"externalRequestSent\",\"connectsManagedAudit\",\"schemaMigrationExecuted\","
           "\"productionRecordWritten\"]";
}

std::string format_test_only_resolver_failure_mapping_json() {
    return "[{\"source_failure_code\":\"RESOLVER_DISABLED\","
           "\"shell_failure_code\":\"TEST_ONLY_RESOLVER_DISABLED\","
           "\"mapped_action\":\"return-fake-failure\",\"retryable\":false},"
           "{\"source_failure_code\":\"APPROVAL_MARKER_MISSING\","
           "\"shell_failure_code\":\"TEST_ONLY_APPROVAL_MARKER_MISSING\","
           "\"mapped_action\":\"pause-and-do-not-resolve\",\"retryable\":false},"
           "{\"source_failure_code\":\"CREDENTIAL_HANDLE_MISSING\","
           "\"shell_failure_code\":\"TEST_ONLY_CREDENTIAL_HANDLE_MISSING\","
           "\"mapped_action\":\"pause-and-do-not-resolve\",\"retryable\":false},"
           "{\"source_failure_code\":\"CREDENTIAL_VALUE_REQUESTED\","
           "\"shell_failure_code\":\"TEST_ONLY_CREDENTIAL_VALUE_REQUESTED\","
           "\"mapped_action\":\"pause-and-do-not-resolve\",\"retryable\":false},"
           "{\"source_failure_code\":\"RAW_ENDPOINT_URL_REQUESTED\","
           "\"shell_failure_code\":\"TEST_ONLY_RAW_ENDPOINT_URL_REQUESTED\","
           "\"mapped_action\":\"pause-and-do-not-resolve\",\"retryable\":false},"
           "{\"source_failure_code\":\"EXTERNAL_REQUEST_REQUESTED\","
           "\"shell_failure_code\":\"TEST_ONLY_EXTERNAL_REQUEST_REQUESTED\","
           "\"mapped_action\":\"pause-and-do-not-resolve\",\"retryable\":false},"
           "{\"source_failure_code\":\"SCHEMA_MIGRATION_REQUESTED\","
           "\"shell_failure_code\":\"TEST_ONLY_SCHEMA_MIGRATION_REQUESTED\","
           "\"mapped_action\":\"pause-and-do-not-resolve\",\"retryable\":false}]";
}

std::string format_test_only_resolver_guard_conditions_json() {
    return "[{\"code\":\"SOURCE_V263_READY\",\"required\":true,\"value\":true},"
           "{\"code\":\"FAKE_RESOLVER_ONLY\",\"required\":true,\"value\":true},"
           "{\"code\":\"CREDENTIAL_HANDLE_ONLY\",\"required\":true,\"value\":true},"
           "{\"code\":\"ENDPOINT_HANDLE_ONLY\",\"required\":true,\"value\":true},"
           "{\"code\":\"RESOLVER_POLICY_HANDLE_REQUIRED\",\"required\":true,\"value\":true},"
           "{\"code\":\"APPROVAL_MARKER_REQUIRED\",\"required\":true,\"value\":true},"
           "{\"code\":\"UPSTREAM_ACTIONS_DISABLED\",\"required\":true,\"value\":true},"
           "{\"code\":\"NO_SECRET_PROVIDER\",\"required\":true,\"value\":true},"
           "{\"code\":\"NO_EXTERNAL_REQUEST\",\"required\":true,\"value\":true},"
           "{\"code\":\"NO_SCHEMA_MIGRATION\",\"required\":true,\"value\":true}]";
}

} // namespace

std::string test_only_resolver_shell_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{test_only_resolver_shell_receipt_release_version}},
        {std::string{test_only_resolver_shell_receipt_artifact_path_hint}},
        {std::string{test_only_resolver_shell_source_contract}},
        {std::string{test_only_resolver_shell_source_verification}},
        {std::string{test_only_resolver_shell_profile_version}},
        {std::string{test_only_resolver_shell_route_path}},
        {std::string{test_only_resolver_shell_state}},
        {std::string{test_only_resolver_shell_mode}},
        {std::string{test_only_resolver_shell_name}},
        {std::string{test_only_resolver_shell_kind}},
        {std::string{test_only_resolver_shell_probe_id}},
        {std::string{test_only_resolver_shell_probe_status}},
        {std::string{test_only_resolver_shell_probe_code}},
        {std::string{test_only_resolver_shell_source_v263_state}},
        {std::string{test_only_resolver_shell_source_v263_mode}},
        {std::string{test_only_resolver_shell_source_span}},
        {std::to_string(test_only_resolver_shell_request_shape_field_count)},
        {std::to_string(test_only_resolver_shell_response_shape_field_count)},
        {std::to_string(test_only_resolver_shell_failure_mapping_count)},
        {std::to_string(test_only_resolver_shell_guard_condition_count)},
        {std::to_string(test_only_resolver_shell_check_count)},
        {std::to_string(test_only_resolver_shell_warning_count)},
        {std::to_string(test_only_resolver_shell_recommendation_count)},
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
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-test-only-resolver-shell-non-participation", parts);
}

std::string format_test_only_resolver_shell_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-test-only-resolver-shell-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(test_only_resolver_shell_receipt_fixture_path) +
           ",\"source_contract\":" + field_string(test_only_resolver_shell_source_contract) +
           ",\"source_verification\":" + field_string(test_only_resolver_shell_source_verification) +
           ",\"consumer_hint\":" + field_string(test_only_resolver_shell_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" +
           field_string(test_only_resolver_shell_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(test_only_resolver_shell_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(test_only_resolver_shell_runtime_role) +
           ",\"source_contract_profile_version\":" +
           field_string(test_only_resolver_shell_profile_version) +
           ",\"source_contract_route_path\":" + field_string(test_only_resolver_shell_route_path) +
           ",\"source_contract_state\":" + field_string(test_only_resolver_shell_state) +
           ",\"source_shell_mode\":" + field_string(test_only_resolver_shell_mode) +
           ",\"source_shell_name\":" + field_string(test_only_resolver_shell_name) +
           ",\"source_resolver_kind\":" + field_string(test_only_resolver_shell_kind) +
           ",\"source_ready_for_test_only_resolver_shell_contract\":true"
           ",\"source_test_only_shell\":true"
           ",\"source_read_only_contract\":true"
           ",\"source_fake_resolver_only\":true"
           ",\"source_handle_only_request\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
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
           ",\"source_production_record_written\":false"
           ",\"source_request_shape_field_count\":" +
           std::to_string(test_only_resolver_shell_request_shape_field_count) +
           ",\"source_response_shape_field_count\":" +
           std::to_string(test_only_resolver_shell_response_shape_field_count) +
           ",\"source_failure_mapping_count\":" +
           std::to_string(test_only_resolver_shell_failure_mapping_count) +
           ",\"source_guard_condition_count\":" +
           std::to_string(test_only_resolver_shell_guard_condition_count) +
           ",\"source_check_count\":" + std::to_string(test_only_resolver_shell_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(test_only_resolver_shell_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"source_warning_count\":" + std::to_string(test_only_resolver_shell_warning_count) +
           ",\"source_recommendation_count\":" +
           std::to_string(test_only_resolver_shell_recommendation_count) +
           ",\"source_node_v263_ready\":true"
           ",\"source_node_v263_verification_state\":" +
           field_string(test_only_resolver_shell_source_v263_state) +
           ",\"source_node_v263_verification_mode\":" +
           field_string(test_only_resolver_shell_source_v263_mode) +
           ",\"source_node_v263_span\":" + field_string(test_only_resolver_shell_source_span) +
           ",\"source_node_v262_ready\":true"
           ",\"source_java_v106_echo_ready\":true"
           ",\"source_mini_kv_v115_non_participation_ready\":true"
           ",\"source_disabled_precheck_aligned\":true"
           ",\"source_required_env_handles_aligned\":true"
           ",\"source_opt_in_gates_aligned\":true"
           ",\"source_failure_taxonomy_aligned\":true"
           ",\"source_dry_run_response_shape_aligned\":true"
           ",\"source_inherited_no_go_conditions_aligned\":true"
           ",\"source_credential_boundary_aligned\":true"
           ",\"source_raw_endpoint_boundary_aligned\":true"
           ",\"source_connection_boundary_aligned\":true"
           ",\"source_write_boundary_aligned\":true"
           ",\"source_auto_start_boundary_aligned\":true"
           ",\"source_upstream_actions_still_disabled\":true"
           ",\"source_node_v263_check_count\":" +
           std::to_string(test_only_resolver_shell_source_v263_check_count) +
           ",\"source_node_v263_passed_check_count\":" +
           std::to_string(test_only_resolver_shell_source_v263_check_count) +
           ",\"source_node_v263_production_blocker_count\":0"
           ",\"resolver_shell_contract\":{\"shell_name\":" +
           field_string(test_only_resolver_shell_name) +
           ",\"shell_mode\":" + field_string(test_only_resolver_shell_mode) +
           ",\"resolver_kind\":" + field_string(test_only_resolver_shell_kind) +
           ",\"real_resolver_implemented\":false,\"real_secret_provider_allowed\":false,"
           "\"fake_resolver_only\":true,\"test_only_shell\":true,\"read_only_contract\":true,"
           "\"handle_only_request\":true,\"resolver_client_may_be_instantiated_for_production\":false,"
           "\"secret_provider_may_be_instantiated\":false,\"credential_value_may_be_loaded\":false,"
           "\"raw_endpoint_url_may_be_parsed\":false,\"external_request_may_be_sent\":false,"
           "\"request_shape_field_count\":" +
           std::to_string(test_only_resolver_shell_request_shape_field_count) +
           ",\"response_shape_field_count\":" +
           std::to_string(test_only_resolver_shell_response_shape_field_count) +
           ",\"failure_mapping_count\":" +
           std::to_string(test_only_resolver_shell_failure_mapping_count) +
           ",\"guard_condition_count\":" +
           std::to_string(test_only_resolver_shell_guard_condition_count) + "}"
           ",\"request_shape\":{\"fields\":" + format_test_only_resolver_request_fields_json() +
           ",\"credential_handle_only\":true,\"credential_value_accepted\":false,"
           "\"endpoint_handle_only\":true,\"raw_endpoint_url_accepted\":false,"
           "\"resolver_policy_handle_required\":true,\"approval_marker_required\":true,"
           "\"approval_correlation_id_required\":true,\"dry_run_required\":true,"
           "\"fake_resolver_only_required\":true,\"payload_may_contain_secrets\":false}"
           ",\"response_shape\":{\"fields\":" + format_test_only_resolver_response_fields_json() +
           ",\"fake_resolver_response_only\":true,\"fake_resolver_only\":true,"
           "\"resolver_client_instantiated\":false,\"secret_provider_instantiated\":false,"
           "\"credential_value_read\":false,\"credential_value_loaded\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"connects_managed_audit\":false,\"schema_migration_executed\":false,"
           "\"production_record_written\":false}"
           ",\"failure_mapping\":" + format_test_only_resolver_failure_mapping_json() +
           ",\"guard_conditions\":" + format_test_only_resolver_guard_conditions_json() +
           ",\"fake_resolver_probe\":{\"request_id\":" +
           field_string(test_only_resolver_shell_probe_id) +
           ",\"resolver_kind\":" + field_string(test_only_resolver_shell_kind) +
           ",\"accepted_by_fake_resolver\":true,\"response_status\":" +
           field_string(test_only_resolver_shell_probe_status) +
           ",\"response_code\":" + field_string(test_only_resolver_shell_probe_code) +
           ",\"resolver_client_instantiated\":false,\"secret_provider_instantiated\":false,"
           "\"credential_value_read\":false,\"credential_value_loaded\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"connects_managed_audit\":false,\"schema_migration_executed\":false,"
           "\"production_record_written\":false}"
           ",\"checks\":{\"source_node_v263_ready\":true,"
           "\"source_still_blocks_credential_resolution\":true,"
           "\"source_still_blocks_credential_value\":true,"
           "\"source_still_blocks_raw_endpoint\":true,"
           "\"source_still_blocks_connection\":true,\"source_still_blocks_writes\":true,"
           "\"source_still_blocks_auto_start\":true,\"fake_resolver_only\":true,"
           "\"request_shape_handle_only\":true,\"response_shape_no_side_effects\":true,"
           "\"failure_mapping_covers_source_taxonomy\":true,\"guard_conditions_declared\":true,"
           "\"fake_resolver_probe_covered\":true,\"fake_resolver_probe_no_credential_read\":true,"
           "\"fake_resolver_probe_no_external_request\":true,"
           "\"fake_resolver_probe_no_production_write\":true,"
           "\"upstream_actions_still_disabled\":true,\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true}"
           ",\"next_required_echo_versions\":[\"Java v107\",\"mini-kv v116\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(test_only_resolver_shell_non_participation_receipt_digest(read_commands)) +
           ",\"test_only_resolver_shell_contract_only\":true"
           ",\"test_only_shell\":true,\"fake_resolver_only\":true,\"handle_only_request\":true," +
           format_common_credential_boundary_json() +
           ",\"resolver_client_instantiated\":false,\"fake_resolver_probe_executed\":false"
           ",\"boundary\":" + field_string(test_only_resolver_shell_boundary) +
           ",\"node_action\":" + field_string(test_only_resolver_shell_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
