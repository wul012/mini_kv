#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::receipt_digest;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

constexpr std::string_view disabled_fake_harness_receipt_consumer =
    "Node v289 disabled fake harness contract upstream echo verification";
constexpr std::string_view disabled_fake_harness_receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-fake-harness-non-participation-receipt.json";
constexpr std::string_view disabled_fake_harness_receipt_release_version = "v127";
constexpr std::string_view disabled_fake_harness_receipt_artifact_path_hint = "d/127/";
constexpr std::string_view disabled_fake_harness_source_contract = "Node v288 disabled fake harness contract";
constexpr std::string_view disabled_fake_harness_source_precheck = "Node v287 test-only fake harness precheck";
constexpr std::string_view disabled_fake_harness_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract.v1";
constexpr std::string_view disabled_fake_harness_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract";
constexpr std::string_view disabled_fake_harness_source_v287_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-harness-precheck";
constexpr std::string_view disabled_fake_harness_contract_state = "disabled-fake-harness-contract-ready";
constexpr std::string_view disabled_fake_harness_precheck_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-harness-precheck.v1";
constexpr std::string_view disabled_fake_harness_precheck_state = "test-only-fake-harness-precheck-ready";
constexpr std::string_view disabled_fake_harness_contract_digest =
    "2ebb03732323ee1f05715ec8f29843670f9131c9d212f144728fc327b4ceefb0";
constexpr std::string_view disabled_fake_harness_precheck_digest =
    "9cfcbdf067028c52a4465b0a21ffbaaa713270690a11638b7068e65510d391a5";
constexpr std::string_view disabled_fake_harness_contract_mode = "disabled-test-only-fake-harness-contract-only";
constexpr std::string_view disabled_fake_harness_contract_name =
    "ManagedAuditCredentialResolverDisabledFakeHarnessContract";
constexpr std::string_view disabled_fake_harness_runtime_toggle_name =
    "ORDEROPS_MANAGED_AUDIT_TEST_ONLY_FAKE_HARNESS_ENABLED";
constexpr std::string_view disabled_fake_harness_invocation_state = "disabled";
constexpr std::string_view disabled_fake_harness_active_plan =
    "docs/plans2/v287-post-test-only-fake-harness-precheck-roadmap.md";
constexpr std::string_view disabled_fake_harness_runtime_role = "runtime evidence provider only";
constexpr std::string_view disabled_fake_harness_boundary =
    "credential resolver disabled fake harness non-participation receipt only; mini-kv echoes Node v288 disabled fake "
    "harness contract and contractDigest, does not implement or run a fake harness runtime, does not instantiate real "
    "or fake secret providers or resolver clients, does not read, store, or render credential values, does not parse "
    "or render raw endpoint URLs, does not send external requests, does not connect managed audit, does not write "
    "storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become "
    "audit or order authority";
constexpr std::string_view disabled_fake_harness_node_action =
    "verify mini-kv v127 disabled fake harness non-participation with Java v122-v126 before Node v289 upstream echo "
    "verification";

constexpr int disabled_fake_harness_check_count = 26;
constexpr int disabled_fake_harness_source_check_count = 21;
constexpr int disabled_fake_harness_required_input_count = 6;
constexpr int disabled_fake_harness_allowed_output_count = 5;
constexpr int disabled_fake_harness_prohibited_input_count = 6;
constexpr int disabled_fake_harness_required_artifact_count = 9;
constexpr int disabled_fake_harness_contract_assertion_count = 10;
constexpr int disabled_fake_harness_prohibited_action_count = 15;
constexpr int disabled_fake_harness_recommended_parallel_version_count = 2;
constexpr int disabled_fake_harness_production_blocker_count = 0;
constexpr int disabled_fake_harness_warning_count = 2;
constexpr int disabled_fake_harness_recommendation_count = 2;

std::string format_required_inputs_json() {
    return "[\"fake-credential-handle\",\"fake-endpoint-handle\","
           "\"operator-approval-artifact-reference\","
           "\"failure-taxonomy-simulation-map\",\"rollback-abort-marker\","
           "\"disabled-runtime-toggle-state\"]";
}

std::string format_allowed_outputs_json() {
    return "[\"disabled-fake-harness-contract-digest\","
           "\"disabled-runtime-toggle-state\",\"side-effect-boundary-summary\","
           "\"upstream-echo-requirement\",\"blocked-runtime-reason\"]";
}

std::string format_prohibited_inputs_json() {
    return "[\"credential-value\",\"raw-endpoint-url\",\"secret-provider-instance\","
           "\"resolver-client-instance\",\"managed-audit-http-client\","
           "\"approval-ledger-write-request\"]";
}

std::string format_required_artifacts_json() {
    return "[\"disabled-fake-harness-contract-id\",\"disabled-runtime-toggle-proof\","
           "\"credential-handle-fixture\",\"endpoint-handle-fixture\","
           "\"operator-approval-artifact-reference\","
           "\"failure-taxonomy-simulation-map\",\"side-effect-boundary-proof\","
           "\"java-v122-echo-marker-requirement\","
           "\"mini-kv-v127-non-participation-receipt-requirement\"]";
}

std::string format_contract_assertions_json() {
    return "[\"fake-harness-runtime-defaults-disabled\","
           "\"fake-harness-runtime-implementation-absent\","
           "\"fake-harness-invocation-blocked\",\"credential-value-never-read\","
           "\"raw-endpoint-url-never-parsed\","
           "\"provider-client-instantiation-blocked\",\"external-network-blocked\","
           "\"ledger-and-schema-writes-blocked\","
           "\"automatic-upstream-start-blocked\","
           "\"parallel-java-mini-kv-echo-required-before-node-v289\"]";
}

std::string format_prohibited_actions_json() {
    return "[\"read-credential-value\",\"store-credential-value\","
           "\"render-credential-value\",\"parse-raw-endpoint-url\","
           "\"render-raw-endpoint-url\",\"instantiate-real-secret-provider\","
           "\"instantiate-real-resolver-client\","
           "\"instantiate-fake-secret-provider\","
           "\"instantiate-fake-resolver-client\",\"send-external-request\","
           "\"connect-managed-audit\",\"write-approval-ledger\","
           "\"execute-schema-migration\",\"execute-fake-harness-runtime\","
           "\"auto-start-upstream\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v287_ready\":true,"
           "\"source_node_v287_keeps_runtime_blocked\":true,"
           "\"source_node_v287_allows_contract_only\":true,"
           "\"contract_digest_valid\":true,\"contract_default_disabled\":true,"
           "\"contract_invocation_blocked\":true,\"runtime_implementation_absent\":true,"
           "\"credential_boundary_closed\":true,\"raw_endpoint_boundary_closed\":true,"
           "\"provider_client_boundary_closed\":true,\"network_boundary_closed\":true,"
           "\"write_boundary_closed\":true,\"auto_start_boundary_closed\":true,"
           "\"required_inputs_named\":true,\"allowed_outputs_named\":true,"
           "\"prohibited_inputs_named\":true,\"required_artifacts_named\":true,"
           "\"contract_assertions_named\":true,\"prohibited_actions_named\":true,"
           "\"upstream_echo_required_for_java_and_mini_kv\":true,"
           "\"recommended_parallel_explicit\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_fake_harness_contract\":"
           "true}";
}

void append_non_participation_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back({"disabled_fake_harness_non_participation_receipt_only", json_bool(true)});
    fields.push_back({"disabled_fake_harness_contract_only", json_bool(true)});
    fields.push_back({"consumes_node_v288_disabled_fake_harness_contract", json_bool(true)});
    fields.push_back({"ready_for_node_v289_upstream_echo_verification", json_bool(true)});
    fields.push_back({"ready_for_java_v122_mini_kv_v127_parallel_echo", json_bool(true)});
    fields.push_back({"ready_for_managed_audit_resolver_implementation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)});
    fields.push_back({"ready_for_production_audit", json_bool(false)});
    fields.push_back({"ready_for_production_window", json_bool(false)});
    fields.push_back({"ready_for_production_operations", json_bool(false)});
    fields.push_back({"real_resolver_implementation_allowed", json_bool(false)});
    fields.push_back({"test_only_fake_harness_allowed", json_bool(false)});
    fields.push_back({"test_only_fake_harness_execution_allowed", json_bool(false)});
    fields.push_back({"fake_harness_runtime_enabled", json_bool(false)});
    fields.push_back({"fake_harness_invocation_allowed", json_bool(false)});
    fields.push_back({"fake_harness_runtime_implemented", json_bool(false)});
    fields.push_back({"fake_harness_runtime_invoked", json_bool(false)});
    fields.push_back({"execute_fake_harness_runtime_allowed", json_bool(false)});
    fields.push_back({"credential_resolver_implemented", json_bool(false)});
    fields.push_back({"credential_resolver_invoked", json_bool(false)});
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_resolver_client_instantiated", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_read", json_bool(false)});
    fields.push_back({"credential_value_provided", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_stored", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"credential_value_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parse_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_render_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_provided", json_bool(false)});
    fields.push_back({"raw_endpoint_url_included", json_bool(false)});
    fields.push_back({"external_request_allowed", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    fields.push_back({"http_tcp_dial_allowed", json_bool(false)});
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

std::string credential_resolver_disabled_fake_harness_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{disabled_fake_harness_receipt_release_version}},
        {std::string{disabled_fake_harness_receipt_artifact_path_hint}},
        {std::string{disabled_fake_harness_source_contract}},
        {std::string{disabled_fake_harness_source_precheck}},
        {std::string{disabled_fake_harness_profile_version}},
        {std::string{disabled_fake_harness_route_path}},
        {std::string{disabled_fake_harness_contract_state}},
        {std::string{disabled_fake_harness_precheck_digest}},
        {std::string{disabled_fake_harness_contract_digest}},
        {std::string{disabled_fake_harness_contract_mode}},
        {std::string{disabled_fake_harness_contract_name}},
        {std::string{disabled_fake_harness_runtime_toggle_name}},
        {std::to_string(disabled_fake_harness_check_count)},
        {std::to_string(disabled_fake_harness_source_check_count)},
        {std::to_string(disabled_fake_harness_required_input_count)},
        {std::to_string(disabled_fake_harness_allowed_output_count)},
        {std::to_string(disabled_fake_harness_prohibited_input_count)},
        {std::to_string(disabled_fake_harness_required_artifact_count)},
        {std::to_string(disabled_fake_harness_contract_assertion_count)},
        {std::to_string(disabled_fake_harness_prohibited_action_count)},
        {std::to_string(disabled_fake_harness_recommended_parallel_version_count)},
        {std::to_string(disabled_fake_harness_production_blocker_count)},
        {"disabled-fake-harness-contract-only"},
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
    return receipt_digest("mini-kv-credential-resolver-disabled-fake-harness-non-participation", parts);
}

std::string format_credential_resolver_disabled_fake_harness_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v287_reference = json_object({
        {"source_version", json_string("Node v287")},
        {"profile_version", json_string(disabled_fake_harness_precheck_profile_version)},
        {"precheck_state", json_string(disabled_fake_harness_precheck_state)},
        {"ready_for_test_only_fake_harness_precheck", json_bool(true)},
        {"ready_for_disabled_fake_harness_contract", json_bool(true)},
        {"precheck_digest", json_string(disabled_fake_harness_precheck_digest)},
        {"source_check_count", json_integer(disabled_fake_harness_source_check_count)},
        {"source_passed_check_count", json_integer(disabled_fake_harness_source_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"source_warning_count", json_integer(2)},
        {"source_recommendation_count", json_integer(2)},
        {"immediate_java_echo_required", json_bool(false)},
        {"immediate_mini_kv_echo_required", json_bool(false)},
        {"recommended_parallel_version_count", json_integer(2)},
        {"fake_harness_runtime_enabled", json_bool(false)},
        {"fake_harness_invocation_allowed", json_bool(false)},
        {"test_only_fake_harness_execution_allowed", json_bool(false)},
        {"real_resolver_implementation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"credential_value_provided", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"raw_endpoint_url_rendered", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"fake_secret_provider_instantiated", json_bool(false)},
        {"fake_resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });
    const auto credential_boundary = json_object({
        {"credential_handle_only", json_bool(true)},
        {"credential_value_read", json_bool(false)},
        {"credential_value_provided", json_bool(false)},
        {"credential_value_stored", json_bool(false)},
    });
    const auto endpoint_boundary = json_object({
        {"endpoint_handle_only", json_bool(true)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"raw_endpoint_url_rendered", json_bool(false)},
        {"raw_endpoint_url_provided", json_bool(false)},
    });
    const auto provider_client_boundary = json_object({
        {"real_secret_provider_instantiated", json_bool(false)},
        {"real_resolver_client_instantiated", json_bool(false)},
        {"fake_secret_provider_instantiated", json_bool(false)},
        {"fake_resolver_client_instantiated", json_bool(false)},
    });
    const auto network_boundary = json_object({
        {"external_request_sent", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"http_tcp_dial_allowed", json_bool(false)},
    });
    const auto write_boundary = json_object({
        {"execution_allowed", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
    });
    const auto disabled_fake_harness_contract = json_object({
        {"contract_digest", json_string(disabled_fake_harness_contract_digest)},
        {"contract_mode", json_string(disabled_fake_harness_contract_mode)},
        {"source_span", json_string("Node v287")},
        {"contract_name", json_string(disabled_fake_harness_contract_name)},
        {"runtime_toggle_name", json_string(disabled_fake_harness_runtime_toggle_name)},
        {"default_runtime_toggle_value", json_bool(false)},
        {"invocation_state", json_string(disabled_fake_harness_invocation_state)},
        {"runtime_implementation_present", json_bool(false)},
        {"runtime_invocation_allowed", json_bool(false)},
        {"required_inputs", format_required_inputs_json()},
        {"allowed_outputs", format_allowed_outputs_json()},
        {"prohibited_inputs", format_prohibited_inputs_json()},
        {"required_artifacts", format_required_artifacts_json()},
        {"contract_assertions", format_contract_assertions_json()},
        {"prohibited_actions", format_prohibited_actions_json()},
        {"credential_boundary", credential_boundary},
        {"endpoint_boundary", endpoint_boundary},
        {"provider_client_boundary", provider_client_boundary},
        {"network_boundary", network_boundary},
        {"write_boundary", write_boundary},
        {"auto_start_boundary", json_object({{"automatic_upstream_start", json_bool(false)}})},
    });
    const auto upstream_echo_requirement = json_object({
        {"decision_mode", json_string("recommended-parallel-upstream-echo-required")},
        {"java_echo_required_now", json_bool(true)},
        {"mini_kv_echo_required_now", json_bool(true)},
        {"recommended_parallel_versions",
         json_array({json_string("Java v122 integration-tests split plus disabled fake harness echo marker"),
                     json_string("mini-kv v127 disabled fake harness non-participation receipt")})},
        {"node_verification_version", json_string("Node v289")},
        {"reason",
         json_string("Node v288 is the first disabled fake harness contract. It still does not execute runtime code, "
                     "but Java and mini-kv should now echo/non-participate against this concrete contract before "
                     "Node v289 verifies cross-project alignment.")},
    });
    const auto summary = json_object({
        {"check_count", json_integer(disabled_fake_harness_check_count)},
        {"passed_check_count", json_integer(disabled_fake_harness_check_count)},
        {"required_input_count", json_integer(disabled_fake_harness_required_input_count)},
        {"allowed_output_count", json_integer(disabled_fake_harness_allowed_output_count)},
        {"prohibited_input_count", json_integer(disabled_fake_harness_prohibited_input_count)},
        {"required_artifact_count", json_integer(disabled_fake_harness_required_artifact_count)},
        {"contract_assertion_count", json_integer(disabled_fake_harness_contract_assertion_count)},
        {"prohibited_action_count", json_integer(disabled_fake_harness_prohibited_action_count)},
        {"source_check_count", json_integer(disabled_fake_harness_source_check_count)},
        {"source_passed_check_count", json_integer(disabled_fake_harness_source_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"java_echo_required_now", json_bool(true)},
        {"mini_kv_echo_required_now", json_bool(true)},
        {"recommended_parallel_version_count", json_integer(disabled_fake_harness_recommended_parallel_version_count)},
        {"production_blocker_count", json_integer(0)},
        {"warning_count", json_integer(disabled_fake_harness_warning_count)},
        {"recommendation_count", json_integer(disabled_fake_harness_recommendation_count)},
    });
    const auto evidence_endpoints = json_object({
        {"disabled_fake_harness_contract_json", json_string(disabled_fake_harness_route_path)},
        {"disabled_fake_harness_contract_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-"
                     "harness-contract?format=markdown")},
        {"source_node_v287_json", json_string(disabled_fake_harness_source_v287_route_path)},
        {"source_node_v287_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-"
                     "harness-precheck?format=markdown")},
        {"active_plan", json_string(disabled_fake_harness_active_plan)},
        {"next_recommended_parallel", json_string("Java v122 + mini-kv v127")},
        {"next_node_verification", json_string("Node v289")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-disabled-fake-harness-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(disabled_fake_harness_receipt_fixture_path)},
        {"source_contract", json_string(disabled_fake_harness_source_contract)},
        {"source_precheck", json_string(disabled_fake_harness_source_precheck)},
        {"consumer_hint", json_string(disabled_fake_harness_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(disabled_fake_harness_receipt_release_version)},
        {"current_artifact_path_hint", json_string(disabled_fake_harness_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(disabled_fake_harness_runtime_role)},
        {"source_profile_version", json_string(disabled_fake_harness_profile_version)},
        {"source_route_path", json_string(disabled_fake_harness_route_path)},
        {"source_precheck_route_path", json_string(disabled_fake_harness_source_v287_route_path)},
        {"source_contract_state", json_string(disabled_fake_harness_contract_state)},
        {"source_ready_for_disabled_fake_harness_contract", json_bool(true)},
        {"source_read_only_contract", json_bool(true)},
        {"source_disabled_fake_harness_contract_only", json_bool(true)},
        {"source_consumes_node_v287_test_only_fake_harness_precheck", json_bool(true)},
        {"source_ready_for_java_v122_mini_kv_v127_parallel_echo", json_bool(true)},
        {"source_ready_for_managed_audit_resolver_implementation", json_bool(false)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_ready_for_production_audit", json_bool(false)},
        {"source_ready_for_production_window", json_bool(false)},
        {"source_ready_for_production_operations", json_bool(false)},
        {"source_real_resolver_implementation_allowed", json_bool(false)},
        {"source_test_only_fake_harness_allowed", json_bool(false)},
        {"source_test_only_fake_harness_execution_allowed", json_bool(false)},
        {"source_fake_harness_runtime_enabled", json_bool(false)},
        {"source_fake_harness_invocation_allowed", json_bool(false)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_credential_value_provided", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_raw_endpoint_url_rendered", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_secret_provider_instantiated", json_bool(false)},
        {"source_resolver_client_instantiated", json_bool(false)},
        {"source_fake_secret_provider_instantiated", json_bool(false)},
        {"source_fake_resolver_client_instantiated", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_approval_ledger_written", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_node_v287_reference", source_node_v287_reference},
        {"disabled_fake_harness_contract", disabled_fake_harness_contract},
        {"upstream_echo_requirement", upstream_echo_requirement},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", json_array({json_object({{"code", json_string("CONTRACT_ONLY_NO_RUNTIME")},
                                              {"severity", json_string("warning")}}),
                                 json_object({{"code", json_string("UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V289")},
                                              {"severity", json_string("warning")}})})},
        {"recommendations", json_array({json_object({{"code", json_string("RUN_PARALLEL_JAVA_V122_MINI_KV_V127")},
                                                     {"severity", json_string("recommendation")}}),
                                        json_object({{"code", json_string("WAIT_FOR_NODE_V289_VERIFICATION")},
                                                     {"severity", json_string("recommendation")}})})},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_echo_versions", json_array({json_string("Java v122"), json_string("mini-kv v127")})},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_disabled_fake_harness_non_participation_receipt_digest(read_commands))},
    };
    append_non_participation_fields(fields);
    fields.push_back({"boundary", json_string(disabled_fake_harness_boundary)});
    fields.push_back({"node_action", json_string(disabled_fake_harness_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
