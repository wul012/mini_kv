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
using credential_resolver_detail::receipt_digest;

constexpr std::string_view disabled_fake_harness_receipt_consumer =
    "Node v289 disabled fake harness contract upstream echo verification";
constexpr std::string_view disabled_fake_harness_receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-fake-harness-non-participation-receipt.json";
constexpr std::string_view disabled_fake_harness_receipt_release_version = "v127";
constexpr std::string_view disabled_fake_harness_receipt_artifact_path_hint = "d/127/";
constexpr std::string_view disabled_fake_harness_source_contract =
    "Node v288 disabled fake harness contract";
constexpr std::string_view disabled_fake_harness_source_precheck =
    "Node v287 test-only fake harness precheck";
constexpr std::string_view disabled_fake_harness_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract.v1";
constexpr std::string_view disabled_fake_harness_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract";
constexpr std::string_view disabled_fake_harness_source_v287_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-harness-precheck";
constexpr std::string_view disabled_fake_harness_contract_state =
    "disabled-fake-harness-contract-ready";
constexpr std::string_view disabled_fake_harness_precheck_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-harness-precheck.v1";
constexpr std::string_view disabled_fake_harness_precheck_state =
    "test-only-fake-harness-precheck-ready";
constexpr std::string_view disabled_fake_harness_contract_digest =
    "2ebb03732323ee1f05715ec8f29843670f9131c9d212f144728fc327b4ceefb0";
constexpr std::string_view disabled_fake_harness_precheck_digest =
    "9cfcbdf067028c52a4465b0a21ffbaaa713270690a11638b7068e65510d391a5";
constexpr std::string_view disabled_fake_harness_contract_mode =
    "disabled-test-only-fake-harness-contract-only";
constexpr std::string_view disabled_fake_harness_contract_name =
    "ManagedAuditCredentialResolverDisabledFakeHarnessContract";
constexpr std::string_view disabled_fake_harness_runtime_toggle_name =
    "ORDEROPS_MANAGED_AUDIT_TEST_ONLY_FAKE_HARNESS_ENABLED";
constexpr std::string_view disabled_fake_harness_invocation_state = "disabled";
constexpr std::string_view disabled_fake_harness_active_plan =
    "docs/plans2/v287-post-test-only-fake-harness-precheck-roadmap.md";
constexpr std::string_view disabled_fake_harness_runtime_role =
    "runtime evidence provider only";
constexpr std::string_view disabled_fake_harness_boundary =
    "credential resolver disabled fake harness non-participation receipt only; mini-kv echoes Node v288 disabled fake harness contract and contractDigest, does not implement or run a fake harness runtime, does not instantiate real or fake secret providers or resolver clients, does not read, store, or render credential values, does not parse or render raw endpoint URLs, does not send external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view disabled_fake_harness_node_action =
    "verify mini-kv v127 disabled fake harness non-participation with Java v122-v126 before Node v289 upstream echo verification";

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
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_fake_harness_contract\":true}";
}

std::string format_non_participation_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"disabled_fake_harness_non_participation_receipt_only\":true,"
           "\"disabled_fake_harness_contract_only\":true,"
           "\"consumes_node_v288_disabled_fake_harness_contract\":true,"
           "\"ready_for_node_v289_upstream_echo_verification\":true,"
           "\"ready_for_java_v122_mini_kv_v127_parallel_echo\":true,"
           "\"ready_for_managed_audit_resolver_implementation\":false,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"ready_for_production_audit\":false,"
           "\"ready_for_production_window\":false,"
           "\"ready_for_production_operations\":false,"
           "\"real_resolver_implementation_allowed\":false,"
           "\"test_only_fake_harness_allowed\":false,"
           "\"test_only_fake_harness_execution_allowed\":false,"
           "\"fake_harness_runtime_enabled\":false,"
           "\"fake_harness_invocation_allowed\":false,"
           "\"fake_harness_runtime_implemented\":false,"
           "\"fake_harness_runtime_invoked\":false,"
           "\"execute_fake_harness_runtime_allowed\":false,"
           "\"credential_resolver_implemented\":false,"
           "\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,"
           "\"secret_provider_instantiated\":false,"
           "\"fake_secret_provider_instantiated\":false,"
           "\"fake_resolver_client_instantiated\":false,"
           "\"credential_value_read_allowed\":false,"
           "\"credential_value_read\":false,"
           "\"credential_value_provided\":false,"
           "\"credential_value_loaded\":false,"
           "\"credential_value_stored\":false,"
           "\"credential_value_included\":false,"
           "\"credential_value_rendered\":false,"
           "\"raw_endpoint_url_parse_allowed\":false,"
           "\"raw_endpoint_url_render_allowed\":false,"
           "\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_rendered\":false,"
           "\"raw_endpoint_url_provided\":false,"
           "\"raw_endpoint_url_included\":false,"
           "\"external_request_allowed\":false,"
           "\"external_request_sent\":false,"
           "\"http_tcp_dial_allowed\":false,"
           "\"connects_managed_audit\":false,"
           "\"reads_managed_audit_credential\":false,"
           "\"stores_managed_audit_credential\":false,"
           "\"managed_audit_store\":false,"
           "\"managed_audit_storage_backend\":false,"
           "\"sandbox_audit_storage_backend\":false,"
           "\"storage_write_allowed\":false,"
           "\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,"
           "\"runtime_write_observed\":false,"
           "\"approval_ledger_write_allowed\":false,"
           "\"approval_ledger_written\":false,"
           "\"approval_ledger_write_executed\":false,"
           "\"managed_audit_write_executed\":false,"
           "\"production_record_written\":false,"
           "\"schema_migration_allowed\":false,"
           "\"schema_migration_executed\":false,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,"
           "\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,"
           "\"node_auto_start_allowed\":false,"
           "\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,"
           "\"automatic_upstream_start_allowed\":false,"
           "\"automatic_upstream_start\":false,"
           "\"audit_authoritative\":false,"
           "\"order_authoritative\":false";
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
    return "{\"receipt_version\":\"mini-kv-credential-resolver-disabled-fake-harness-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(disabled_fake_harness_receipt_fixture_path) +
           ",\"source_contract\":" + field_string(disabled_fake_harness_source_contract) +
           ",\"source_precheck\":" + field_string(disabled_fake_harness_source_precheck) +
           ",\"consumer_hint\":" + field_string(disabled_fake_harness_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" +
           field_string(disabled_fake_harness_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(disabled_fake_harness_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(disabled_fake_harness_runtime_role) +
           ",\"source_profile_version\":" + field_string(disabled_fake_harness_profile_version) +
           ",\"source_route_path\":" + field_string(disabled_fake_harness_route_path) +
           ",\"source_precheck_route_path\":" +
           field_string(disabled_fake_harness_source_v287_route_path) +
           ",\"source_contract_state\":" +
           field_string(disabled_fake_harness_contract_state) +
           ",\"source_ready_for_disabled_fake_harness_contract\":true"
           ",\"source_read_only_contract\":true"
           ",\"source_disabled_fake_harness_contract_only\":true"
           ",\"source_consumes_node_v287_test_only_fake_harness_precheck\":true"
           ",\"source_ready_for_java_v122_mini_kv_v127_parallel_echo\":true"
           ",\"source_ready_for_managed_audit_resolver_implementation\":false"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
           ",\"source_ready_for_production_operations\":false"
           ",\"source_real_resolver_implementation_allowed\":false"
           ",\"source_test_only_fake_harness_allowed\":false"
           ",\"source_test_only_fake_harness_execution_allowed\":false"
           ",\"source_fake_harness_runtime_enabled\":false"
           ",\"source_fake_harness_invocation_allowed\":false"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_credential_value_provided\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_raw_endpoint_url_rendered\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_secret_provider_instantiated\":false"
           ",\"source_resolver_client_instantiated\":false"
           ",\"source_fake_secret_provider_instantiated\":false"
           ",\"source_fake_resolver_client_instantiated\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_approval_ledger_written\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_node_v287_reference\":{\"source_version\":\"Node v287\","
           "\"profile_version\":" + field_string(disabled_fake_harness_precheck_profile_version) +
           ",\"precheck_state\":" + field_string(disabled_fake_harness_precheck_state) +
           ",\"ready_for_test_only_fake_harness_precheck\":true"
           ",\"ready_for_disabled_fake_harness_contract\":true"
           ",\"precheck_digest\":" + field_string(disabled_fake_harness_precheck_digest) +
           ",\"source_check_count\":" + std::to_string(disabled_fake_harness_source_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(disabled_fake_harness_source_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"source_warning_count\":2"
           ",\"source_recommendation_count\":2"
           ",\"immediate_java_echo_required\":false"
           ",\"immediate_mini_kv_echo_required\":false"
           ",\"recommended_parallel_version_count\":2"
           ",\"fake_harness_runtime_enabled\":false"
           ",\"fake_harness_invocation_allowed\":false"
           ",\"test_only_fake_harness_execution_allowed\":false"
           ",\"real_resolver_implementation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"credential_value_provided\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"raw_endpoint_url_rendered\":false"
           ",\"external_request_sent\":false"
           ",\"secret_provider_instantiated\":false"
           ",\"resolver_client_instantiated\":false"
           ",\"fake_secret_provider_instantiated\":false"
           ",\"fake_resolver_client_instantiated\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"disabled_fake_harness_contract\":{\"contract_digest\":" +
           field_string(disabled_fake_harness_contract_digest) +
           ",\"contract_mode\":" + field_string(disabled_fake_harness_contract_mode) +
           ",\"source_span\":\"Node v287\""
           ",\"contract_name\":" + field_string(disabled_fake_harness_contract_name) +
           ",\"runtime_toggle_name\":" + field_string(disabled_fake_harness_runtime_toggle_name) +
           ",\"default_runtime_toggle_value\":false"
           ",\"invocation_state\":" + field_string(disabled_fake_harness_invocation_state) +
           ",\"runtime_implementation_present\":false"
           ",\"runtime_invocation_allowed\":false"
           ",\"required_inputs\":" + format_required_inputs_json() +
           ",\"allowed_outputs\":" + format_allowed_outputs_json() +
           ",\"prohibited_inputs\":" + format_prohibited_inputs_json() +
           ",\"required_artifacts\":" + format_required_artifacts_json() +
           ",\"contract_assertions\":" + format_contract_assertions_json() +
           ",\"prohibited_actions\":" + format_prohibited_actions_json() +
           ",\"credential_boundary\":{\"credential_handle_only\":true,"
           "\"credential_value_read\":false,\"credential_value_provided\":false,"
           "\"credential_value_stored\":false}"
           ",\"endpoint_boundary\":{\"endpoint_handle_only\":true,"
           "\"raw_endpoint_url_parsed\":false,\"raw_endpoint_url_rendered\":false,"
           "\"raw_endpoint_url_provided\":false}"
           ",\"provider_client_boundary\":{\"real_secret_provider_instantiated\":false,"
           "\"real_resolver_client_instantiated\":false,"
           "\"fake_secret_provider_instantiated\":false,"
           "\"fake_resolver_client_instantiated\":false}"
           ",\"network_boundary\":{\"external_request_sent\":false,"
           "\"connects_managed_audit\":false,\"http_tcp_dial_allowed\":false}"
           ",\"write_boundary\":{\"execution_allowed\":false,"
           "\"schema_migration_executed\":false,\"approval_ledger_written\":false}"
           ",\"auto_start_boundary\":{\"automatic_upstream_start\":false}}"
           ",\"upstream_echo_requirement\":{\"decision_mode\":\"recommended-parallel-upstream-echo-required\","
           "\"java_echo_required_now\":true,\"mini_kv_echo_required_now\":true,"
           "\"recommended_parallel_versions\":[\"Java v122 integration-tests split plus disabled fake harness echo marker\","
           "\"mini-kv v127 disabled fake harness non-participation receipt\"],"
           "\"node_verification_version\":\"Node v289\","
           "\"reason\":\"Node v288 is the first disabled fake harness contract. It still does not execute runtime code, but Java and mini-kv should now echo/non-participate against this concrete contract before Node v289 verifies cross-project alignment.\"}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(disabled_fake_harness_check_count) +
           ",\"passed_check_count\":" + std::to_string(disabled_fake_harness_check_count) +
           ",\"required_input_count\":" +
           std::to_string(disabled_fake_harness_required_input_count) +
           ",\"allowed_output_count\":" +
           std::to_string(disabled_fake_harness_allowed_output_count) +
           ",\"prohibited_input_count\":" +
           std::to_string(disabled_fake_harness_prohibited_input_count) +
           ",\"required_artifact_count\":" +
           std::to_string(disabled_fake_harness_required_artifact_count) +
           ",\"contract_assertion_count\":" +
           std::to_string(disabled_fake_harness_contract_assertion_count) +
           ",\"prohibited_action_count\":" +
           std::to_string(disabled_fake_harness_prohibited_action_count) +
           ",\"source_check_count\":" + std::to_string(disabled_fake_harness_source_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(disabled_fake_harness_source_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"java_echo_required_now\":true"
           ",\"mini_kv_echo_required_now\":true"
           ",\"recommended_parallel_version_count\":" +
           std::to_string(disabled_fake_harness_recommended_parallel_version_count) +
           ",\"production_blocker_count\":0"
           ",\"warning_count\":" + std::to_string(disabled_fake_harness_warning_count) +
           ",\"recommendation_count\":" +
           std::to_string(disabled_fake_harness_recommendation_count) +
           "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"CONTRACT_ONLY_NO_RUNTIME\",\"severity\":\"warning\"},"
           "{\"code\":\"UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V289\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_PARALLEL_JAVA_V122_MINI_KV_V127\","
           "\"severity\":\"recommendation\"},{\"code\":\"WAIT_FOR_NODE_V289_VERIFICATION\","
           "\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"disabled_fake_harness_contract_json\":" +
           field_string(disabled_fake_harness_route_path) +
           ",\"disabled_fake_harness_contract_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract?format=markdown\","
           "\"source_node_v287_json\":" + field_string(disabled_fake_harness_source_v287_route_path) +
           ",\"source_node_v287_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-harness-precheck?format=markdown\","
           "\"active_plan\":" + field_string(disabled_fake_harness_active_plan) +
           ",\"next_recommended_parallel\":\"Java v122 + mini-kv v127\","
           "\"next_node_verification\":\"Node v289\"}"
           ",\"next_required_echo_versions\":[\"Java v122\",\"mini-kv v127\"]"
           ",\"binary_provenance_digest\":" +
           field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" +
           field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" +
           field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_disabled_fake_harness_non_participation_receipt_digest(
               read_commands)) +
           "," + format_non_participation_flags_json() +
           ",\"boundary\":" + field_string(disabled_fake_harness_boundary) +
           ",\"node_action\":" + field_string(disabled_fake_harness_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
