#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::append_common_credential_boundary_fields;
using credential_resolver_detail::DigestPart;
using credential_resolver_detail::receipt_digest;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

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
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-contract";
constexpr std::string_view test_only_resolver_shell_state =
    "sandbox-endpoint-credential-resolver-test-only-shell-contract-ready";
constexpr std::string_view test_only_resolver_shell_mode = "test-only-fake-resolver-contract";
constexpr std::string_view test_only_resolver_shell_name = "ManagedAuditSandboxEndpointCredentialResolverTestOnlyShell";
constexpr std::string_view test_only_resolver_shell_kind = "fake-in-memory";
constexpr std::string_view test_only_resolver_shell_probe_id = "managed-audit-v264-test-only-resolver-shell-probe";
constexpr std::string_view test_only_resolver_shell_probe_status = "fake-resolver-accepted";
constexpr std::string_view test_only_resolver_shell_probe_code = "TEST_ONLY_FAKE_RESOLVER";
constexpr std::string_view test_only_resolver_shell_source_v263_state =
    "sandbox-endpoint-credential-resolver-disabled-precheck-upstream-echo-verification-ready";
constexpr std::string_view test_only_resolver_shell_source_v263_mode =
    "java-v106-plus-mini-kv-v115-disabled-credential-resolver-precheck-upstream-echo-verification-only";
constexpr std::string_view test_only_resolver_shell_source_span = "Node v262 + Java v106 + mini-kv v115";
constexpr std::string_view test_only_resolver_shell_runtime_role = "runtime evidence provider only";
constexpr std::string_view test_only_resolver_shell_boundary =
    "test-only resolver shell non-participation receipt only; mini-kv echoes Node v264 fake resolver shell contract "
    "boundaries, does not instantiate a resolver client or secret provider, does not accept or read credential values, "
    "does not parse raw endpoint URLs, does not send external requests, does not connect managed audit, does not write "
    "storage or production records, does not execute schema migration or restore/load/compact/SETNXEX, and does not "
    "become a managed audit storage backend";
constexpr std::string_view test_only_resolver_shell_node_action =
    "verify mini-kv non-participation with Node v264 test-only resolver shell contract before Node v265 upstream echo "
    "verification";
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

std::string test_only_resolver_shell_non_participation_receipt_digest(const std::vector<std::string>& read_commands) {
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

std::string
format_test_only_resolver_shell_non_participation_receipt_json(const std::vector<std::string>& read_commands) {
    const auto resolver_shell_contract = json_object({
        {"shell_name", json_string(test_only_resolver_shell_name)},
        {"shell_mode", json_string(test_only_resolver_shell_mode)},
        {"resolver_kind", json_string(test_only_resolver_shell_kind)},
        {"real_resolver_implemented", json_bool(false)},
        {"real_secret_provider_allowed", json_bool(false)},
        {"fake_resolver_only", json_bool(true)},
        {"test_only_shell", json_bool(true)},
        {"read_only_contract", json_bool(true)},
        {"handle_only_request", json_bool(true)},
        {"resolver_client_may_be_instantiated_for_production", json_bool(false)},
        {"secret_provider_may_be_instantiated", json_bool(false)},
        {"credential_value_may_be_loaded", json_bool(false)},
        {"raw_endpoint_url_may_be_parsed", json_bool(false)},
        {"external_request_may_be_sent", json_bool(false)},
        {"request_shape_field_count", json_integer(test_only_resolver_shell_request_shape_field_count)},
        {"response_shape_field_count", json_integer(test_only_resolver_shell_response_shape_field_count)},
        {"failure_mapping_count", json_integer(test_only_resolver_shell_failure_mapping_count)},
        {"guard_condition_count", json_integer(test_only_resolver_shell_guard_condition_count)},
    });
    const auto request_shape = json_object({
        {"fields", format_test_only_resolver_request_fields_json()},
        {"credential_handle_only", json_bool(true)},
        {"credential_value_accepted", json_bool(false)},
        {"endpoint_handle_only", json_bool(true)},
        {"raw_endpoint_url_accepted", json_bool(false)},
        {"resolver_policy_handle_required", json_bool(true)},
        {"approval_marker_required", json_bool(true)},
        {"approval_correlation_id_required", json_bool(true)},
        {"dry_run_required", json_bool(true)},
        {"fake_resolver_only_required", json_bool(true)},
        {"payload_may_contain_secrets", json_bool(false)},
    });
    const auto response_shape = json_object({
        {"fields", format_test_only_resolver_response_fields_json()},
        {"fake_resolver_response_only", json_bool(true)},
        {"fake_resolver_only", json_bool(true)},
        {"resolver_client_instantiated", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"credential_value_loaded", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"production_record_written", json_bool(false)},
    });
    const auto fake_resolver_probe = json_object({
        {"request_id", json_string(test_only_resolver_shell_probe_id)},
        {"resolver_kind", json_string(test_only_resolver_shell_kind)},
        {"accepted_by_fake_resolver", json_bool(true)},
        {"response_status", json_string(test_only_resolver_shell_probe_status)},
        {"response_code", json_string(test_only_resolver_shell_probe_code)},
        {"resolver_client_instantiated", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"credential_value_loaded", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"production_record_written", json_bool(false)},
    });
    const auto checks = json_object({
        {"source_node_v263_ready", json_bool(true)},
        {"source_still_blocks_credential_resolution", json_bool(true)},
        {"source_still_blocks_credential_value", json_bool(true)},
        {"source_still_blocks_raw_endpoint", json_bool(true)},
        {"source_still_blocks_connection", json_bool(true)},
        {"source_still_blocks_writes", json_bool(true)},
        {"source_still_blocks_auto_start", json_bool(true)},
        {"fake_resolver_only", json_bool(true)},
        {"request_shape_handle_only", json_bool(true)},
        {"response_shape_no_side_effects", json_bool(true)},
        {"failure_mapping_covers_source_taxonomy", json_bool(true)},
        {"guard_conditions_declared", json_bool(true)},
        {"fake_resolver_probe_covered", json_bool(true)},
        {"fake_resolver_probe_no_credential_read", json_bool(true)},
        {"fake_resolver_probe_no_external_request", json_bool(true)},
        {"fake_resolver_probe_no_production_write", json_bool(true)},
        {"upstream_actions_still_disabled", json_bool(true)},
        {"production_audit_still_blocked", json_bool(true)},
        {"production_window_still_blocked", json_bool(true)},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string("mini-kv-test-only-resolver-shell-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(test_only_resolver_shell_receipt_fixture_path)},
        {"source_contract", json_string(test_only_resolver_shell_source_contract)},
        {"source_verification", json_string(test_only_resolver_shell_source_verification)},
        {"consumer_hint", json_string(test_only_resolver_shell_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(test_only_resolver_shell_receipt_release_version)},
        {"current_artifact_path_hint", json_string(test_only_resolver_shell_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(test_only_resolver_shell_runtime_role)},
        {"source_contract_profile_version", json_string(test_only_resolver_shell_profile_version)},
        {"source_contract_route_path", json_string(test_only_resolver_shell_route_path)},
        {"source_contract_state", json_string(test_only_resolver_shell_state)},
        {"source_shell_mode", json_string(test_only_resolver_shell_mode)},
        {"source_shell_name", json_string(test_only_resolver_shell_name)},
        {"source_resolver_kind", json_string(test_only_resolver_shell_kind)},
        {"source_ready_for_test_only_resolver_shell_contract", json_bool(true)},
        {"source_test_only_shell", json_bool(true)},
        {"source_read_only_contract", json_bool(true)},
        {"source_fake_resolver_only", json_bool(true)},
        {"source_handle_only_request", json_bool(true)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_ready_for_production_audit", json_bool(false)},
        {"source_ready_for_production_window", json_bool(false)},
        {"source_credential_resolver_execution_allowed", json_bool(false)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_credential_value_loaded", json_bool(false)},
        {"source_credential_value_stored", json_bool(false)},
        {"source_credential_value_included", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_raw_endpoint_url_included", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_secret_provider_instantiated", json_bool(false)},
        {"source_resolver_client_instantiated", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_production_record_written", json_bool(false)},
        {"source_request_shape_field_count", json_integer(test_only_resolver_shell_request_shape_field_count)},
        {"source_response_shape_field_count", json_integer(test_only_resolver_shell_response_shape_field_count)},
        {"source_failure_mapping_count", json_integer(test_only_resolver_shell_failure_mapping_count)},
        {"source_guard_condition_count", json_integer(test_only_resolver_shell_guard_condition_count)},
        {"source_check_count", json_integer(test_only_resolver_shell_check_count)},
        {"source_passed_check_count", json_integer(test_only_resolver_shell_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"source_warning_count", json_integer(test_only_resolver_shell_warning_count)},
        {"source_recommendation_count", json_integer(test_only_resolver_shell_recommendation_count)},
        {"source_node_v263_ready", json_bool(true)},
        {"source_node_v263_verification_state", json_string(test_only_resolver_shell_source_v263_state)},
        {"source_node_v263_verification_mode", json_string(test_only_resolver_shell_source_v263_mode)},
        {"source_node_v263_span", json_string(test_only_resolver_shell_source_span)},
        {"source_node_v262_ready", json_bool(true)},
        {"source_java_v106_echo_ready", json_bool(true)},
        {"source_mini_kv_v115_non_participation_ready", json_bool(true)},
        {"source_disabled_precheck_aligned", json_bool(true)},
        {"source_required_env_handles_aligned", json_bool(true)},
        {"source_opt_in_gates_aligned", json_bool(true)},
        {"source_failure_taxonomy_aligned", json_bool(true)},
        {"source_dry_run_response_shape_aligned", json_bool(true)},
        {"source_inherited_no_go_conditions_aligned", json_bool(true)},
        {"source_credential_boundary_aligned", json_bool(true)},
        {"source_raw_endpoint_boundary_aligned", json_bool(true)},
        {"source_connection_boundary_aligned", json_bool(true)},
        {"source_write_boundary_aligned", json_bool(true)},
        {"source_auto_start_boundary_aligned", json_bool(true)},
        {"source_upstream_actions_still_disabled", json_bool(true)},
        {"source_node_v263_check_count", json_integer(test_only_resolver_shell_source_v263_check_count)},
        {"source_node_v263_passed_check_count", json_integer(test_only_resolver_shell_source_v263_check_count)},
        {"source_node_v263_production_blocker_count", json_integer(0)},
        {"resolver_shell_contract", resolver_shell_contract},
        {"request_shape", request_shape},
        {"response_shape", response_shape},
        {"failure_mapping", format_test_only_resolver_failure_mapping_json()},
        {"guard_conditions", format_test_only_resolver_guard_conditions_json()},
        {"fake_resolver_probe", fake_resolver_probe},
        {"checks", checks},
        {"next_required_echo_versions", json_array({json_string("Java v107"), json_string("mini-kv v116")})},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest", json_string(test_only_resolver_shell_non_participation_receipt_digest(read_commands))},
        {"test_only_resolver_shell_contract_only", json_bool(true)},
        {"test_only_shell", json_bool(true)},
        {"fake_resolver_only", json_bool(true)},
        {"handle_only_request", json_bool(true)},
    };
    append_common_credential_boundary_fields(fields);
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"fake_resolver_probe_executed", json_bool(false)});
    fields.push_back({"boundary", json_string(test_only_resolver_shell_boundary)});
    fields.push_back({"node_action", json_string(test_only_resolver_shell_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
