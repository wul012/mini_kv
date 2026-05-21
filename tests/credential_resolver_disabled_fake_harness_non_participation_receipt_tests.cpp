#include "minikv/command.hpp"
#include "minikv/runtime_evidence_receipts.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());
    std::ostringstream output;
    output << input.rdbuf();
    auto text = output.str();
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

void assert_not_contains(const std::string& text, std::string_view forbidden) {
    if (text.find(std::string{forbidden}) != std::string::npos) {
        std::cerr << "unexpected text: " << forbidden << '\n';
    }
    assert(text.find(std::string{forbidden}) == std::string::npos);
}

void assert_path_exists(const std::filesystem::path& relative_path) {
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path));
}

void assert_source_contract_shape(const std::string& text) {
    assert_contains(text, "\"source_contract\":\"Node v288 disabled fake harness contract\"");
    assert_contains(text, "\"source_precheck\":\"Node v287 test-only fake harness precheck\"");
    assert_contains(text, "\"consumer_hint\":\"Node v289 disabled fake harness contract upstream echo verification\"");
    assert_contains(
        text,
        "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract.v1\"");
    assert_contains(
        text,
        "\"source_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract\"");
    assert_contains(
        text,
        "\"source_precheck_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-harness-precheck\"");
    assert_contains(text, "\"source_contract_state\":\"disabled-fake-harness-contract-ready\"");
    assert_contains(text, "\"source_ready_for_disabled_fake_harness_contract\":true");
    assert_contains(text, "\"source_read_only_contract\":true");
    assert_contains(text, "\"source_disabled_fake_harness_contract_only\":true");
    assert_contains(text, "\"source_consumes_node_v287_test_only_fake_harness_precheck\":true");
    assert_contains(text, "\"source_ready_for_java_v122_mini_kv_v127_parallel_echo\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_resolver_implementation\":false");
    assert_contains(text, "\"source_test_only_fake_harness_execution_allowed\":false");
    assert_contains(text, "\"source_fake_harness_runtime_enabled\":false");
    assert_contains(text, "\"source_fake_harness_invocation_allowed\":false");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_credential_value_provided\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_raw_endpoint_url_rendered\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_secret_provider_instantiated\":false");
    assert_contains(text, "\"source_resolver_client_instantiated\":false");
    assert_contains(text, "\"source_fake_secret_provider_instantiated\":false");
    assert_contains(text, "\"source_fake_resolver_client_instantiated\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_approval_ledger_written\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
}

void assert_source_node_v287_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v287_reference\":");
    assert_contains(text, "\"source_version\":\"Node v287\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-test-only-fake-harness-precheck.v1\"");
    assert_contains(text, "\"precheck_state\":\"test-only-fake-harness-precheck-ready\"");
    assert_contains(text, "\"ready_for_test_only_fake_harness_precheck\":true");
    assert_contains(text, "\"ready_for_disabled_fake_harness_contract\":true");
    assert_contains(
        text,
        "\"precheck_digest\":\"9cfcbdf067028c52a4465b0a21ffbaaa713270690a11638b7068e65510d391a5\"");
    assert_contains(text, "\"source_check_count\":21");
    assert_contains(text, "\"source_passed_check_count\":21");
    assert_contains(text, "\"source_production_blocker_count\":0");
    assert_contains(text, "\"immediate_java_echo_required\":false");
    assert_contains(text, "\"immediate_mini_kv_echo_required\":false");
    assert_contains(text, "\"recommended_parallel_version_count\":2");
    assert_contains(text, "\"fake_harness_runtime_enabled\":false");
    assert_contains(text, "\"fake_harness_invocation_allowed\":false");
    assert_contains(text, "\"test_only_fake_harness_execution_allowed\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"execution_allowed\":false");
}

void assert_contract_details(const std::string& text) {
    assert_contains(text, "\"disabled_fake_harness_contract\":");
    assert_contains(
        text,
        "\"contract_digest\":\"2ebb03732323ee1f05715ec8f29843670f9131c9d212f144728fc327b4ceefb0\"");
    assert_contains(text, "\"contract_mode\":\"disabled-test-only-fake-harness-contract-only\"");
    assert_contains(text, "\"source_span\":\"Node v287\"");
    assert_contains(text, "\"contract_name\":\"ManagedAuditCredentialResolverDisabledFakeHarnessContract\"");
    assert_contains(text, "\"runtime_toggle_name\":\"ORDEROPS_MANAGED_AUDIT_TEST_ONLY_FAKE_HARNESS_ENABLED\"");
    assert_contains(text, "\"default_runtime_toggle_value\":false");
    assert_contains(text, "\"invocation_state\":\"disabled\"");
    assert_contains(text, "\"runtime_implementation_present\":false");
    assert_contains(text, "\"runtime_invocation_allowed\":false");
    assert_contains(text, "\"required_inputs\":[\"fake-credential-handle\",\"fake-endpoint-handle\",\"operator-approval-artifact-reference\",\"failure-taxonomy-simulation-map\",\"rollback-abort-marker\",\"disabled-runtime-toggle-state\"]");
    assert_contains(text, "\"allowed_outputs\":[\"disabled-fake-harness-contract-digest\",\"disabled-runtime-toggle-state\",\"side-effect-boundary-summary\",\"upstream-echo-requirement\",\"blocked-runtime-reason\"]");
    assert_contains(text, "\"prohibited_inputs\":[\"credential-value\",\"raw-endpoint-url\",\"secret-provider-instance\",\"resolver-client-instance\",\"managed-audit-http-client\",\"approval-ledger-write-request\"]");
    assert_contains(text, "\"disabled-fake-harness-contract-id\"");
    assert_contains(text, "\"disabled-runtime-toggle-proof\"");
    assert_contains(text, "\"java-v122-echo-marker-requirement\"");
    assert_contains(text, "\"mini-kv-v127-non-participation-receipt-requirement\"");
    assert_contains(text, "\"fake-harness-runtime-defaults-disabled\"");
    assert_contains(text, "\"parallel-java-mini-kv-echo-required-before-node-v289\"");
    assert_contains(text, "\"execute-fake-harness-runtime\"");
    assert_contains(text, "\"credential_boundary\":{\"credential_handle_only\":true,\"credential_value_read\":false,\"credential_value_provided\":false,\"credential_value_stored\":false}");
    assert_contains(text, "\"endpoint_boundary\":{\"endpoint_handle_only\":true,\"raw_endpoint_url_parsed\":false,\"raw_endpoint_url_rendered\":false,\"raw_endpoint_url_provided\":false}");
    assert_contains(text, "\"provider_client_boundary\":{\"real_secret_provider_instantiated\":false,\"real_resolver_client_instantiated\":false,\"fake_secret_provider_instantiated\":false,\"fake_resolver_client_instantiated\":false}");
    assert_contains(text, "\"network_boundary\":{\"external_request_sent\":false,\"connects_managed_audit\":false,\"http_tcp_dial_allowed\":false}");
    assert_contains(text, "\"write_boundary\":{\"execution_allowed\":false,\"schema_migration_executed\":false,\"approval_ledger_written\":false}");
}

void assert_checks_and_summary(const std::string& text) {
    assert_contains(text, "\"source_node_v287_ready\":true");
    assert_contains(text, "\"source_node_v287_keeps_runtime_blocked\":true");
    assert_contains(text, "\"source_node_v287_allows_contract_only\":true");
    assert_contains(text, "\"contract_digest_valid\":true");
    assert_contains(text, "\"contract_default_disabled\":true");
    assert_contains(text, "\"contract_invocation_blocked\":true");
    assert_contains(text, "\"runtime_implementation_absent\":true");
    assert_contains(text, "\"credential_boundary_closed\":true");
    assert_contains(text, "\"raw_endpoint_boundary_closed\":true");
    assert_contains(text, "\"provider_client_boundary_closed\":true");
    assert_contains(text, "\"network_boundary_closed\":true");
    assert_contains(text, "\"write_boundary_closed\":true");
    assert_contains(text, "\"auto_start_boundary_closed\":true");
    assert_contains(text, "\"upstream_echo_required_for_java_and_mini_kv\":true");
    assert_contains(text, "\"recommended_parallel_explicit\":true");
    assert_contains(text, "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_fake_harness_contract\":true");
    assert_contains(text, "\"check_count\":26");
    assert_contains(text, "\"passed_check_count\":26");
    assert_contains(text, "\"required_input_count\":6");
    assert_contains(text, "\"allowed_output_count\":5");
    assert_contains(text, "\"prohibited_input_count\":6");
    assert_contains(text, "\"required_artifact_count\":9");
    assert_contains(text, "\"contract_assertion_count\":10");
    assert_contains(text, "\"prohibited_action_count\":15");
    assert_contains(text, "\"source_check_count\":21");
    assert_contains(text, "\"source_passed_check_count\":21");
    assert_contains(text, "\"java_echo_required_now\":true");
    assert_contains(text, "\"mini_kv_echo_required_now\":true");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"CONTRACT_ONLY_NO_RUNTIME\"");
    assert_contains(text, "\"UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V289\"");
    assert_contains(text, "\"RUN_PARALLEL_JAVA_V122_MINI_KV_V127\"");
    assert_contains(text, "\"WAIT_FOR_NODE_V289_VERIFICATION\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v122\",\"mini-kv v127\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"disabled_fake_harness_non_participation_receipt_only\":true");
    assert_contains(text, "\"disabled_fake_harness_contract_only\":true");
    assert_contains(text, "\"consumes_node_v288_disabled_fake_harness_contract\":true");
    assert_contains(text, "\"ready_for_node_v289_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_java_v122_mini_kv_v127_parallel_echo\":true");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"test_only_fake_harness_allowed\":false");
    assert_contains(text, "\"test_only_fake_harness_execution_allowed\":false");
    assert_contains(text, "\"fake_harness_runtime_enabled\":false");
    assert_contains(text, "\"fake_harness_invocation_allowed\":false");
    assert_contains(text, "\"fake_harness_runtime_implemented\":false");
    assert_contains(text, "\"fake_harness_runtime_invoked\":false");
    assert_contains(text, "\"execute_fake_harness_runtime_allowed\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"fake_secret_provider_instantiated\":false");
    assert_contains(text, "\"fake_resolver_client_instantiated\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"credential_value_provided\":false");
    assert_contains(text, "\"credential_value_stored\":false");
    assert_contains(text, "\"credential_value_rendered\":false");
    assert_contains(text, "\"raw_endpoint_url_parse_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_render_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"raw_endpoint_url_rendered\":false");
    assert_contains(text, "\"external_request_allowed\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"http_tcp_dial_allowed\":false");
    assert_contains(text, "\"connects_managed_audit\":false");
    assert_contains(text, "\"managed_audit_storage_backend\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_written\":false");
    assert_contains(text, "\"schema_migration_allowed\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"restore_execution_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"audit_authoritative\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path =
        std::filesystem::path{"fixtures"} / "release" /
        "credential-resolver-disabled-fake-harness-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_disabled_fake_harness_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-credential-resolver-disabled-fake-harness-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v127\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-disabled-fake-harness-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_disabled_fake_harness_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v127\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/127/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_contract_shape(receipt);
    assert_source_node_v287_shape(receipt);
    assert_contract_details(receipt);
    assert_checks_and_summary(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver disabled fake harness non-participation receipt only");
    assert_contains(receipt, "does not implement or run a fake harness runtime");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_disabled_fake_harness_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_contract_shape(smoke);
    assert_source_node_v287_shape(smoke);
    assert_contract_details(smoke);
    assert_checks_and_summary(smoke);
    assert_non_participation_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_credential_resolver_disabled_fake_harness_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-disabled-fake-harness-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_disabled_fake_harness_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_fake_harness_non_participation_receipt for Node v289");
    assert_source_contract_shape(manifest);
    assert_source_node_v287_shape(manifest);
    assert_contract_details(manifest);
    assert_checks_and_summary(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_disabled_fake_harness_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-disabled-fake-harness-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v127\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/127/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_contract_shape(result.response);
    assert_source_node_v287_shape(result.response);
    assert_contract_details(result.response);
    assert_checks_and_summary(result.response);
    assert_non_participation_flags(result.response);
    assert_contains(result.response,
                    "Node v289 may verify the mini-kv v127 credential resolver disabled fake harness non-participation receipt");
    assert_contains(result.response, "Node v284 may verify the mini-kv v126 credential resolver implementation plan non-participation receipt");
    assert_not_contains(result.response, "credential_value\":\"");
    assert_not_contains(result.response, "raw_endpoint_url\":\"");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SMOKEJSON\"");
    assert_contains(result.response, "\"write_command\":false");

    return 0;
}
