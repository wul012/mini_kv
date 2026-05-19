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
    assert_contains(text, "\"source_contract\":\"Node v264 credential resolver test-only shell contract\"");
    assert_contains(text, "\"source_verification\":\"Node v263 disabled resolver precheck upstream echo verification\"");
    assert_contains(text, "\"consumer_hint\":\"Node v265 test-only resolver shell upstream echo verification\"");
    assert_contains(text, "\"source_contract_profile_version\":\"managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-contract.v1\"");
    assert_contains(text, "\"source_contract_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-contract\"");
    assert_contains(text, "\"source_contract_state\":\"sandbox-endpoint-credential-resolver-test-only-shell-contract-ready\"");
    assert_contains(text, "\"source_shell_mode\":\"test-only-fake-resolver-contract\"");
    assert_contains(text, "\"source_shell_name\":\"ManagedAuditSandboxEndpointCredentialResolverTestOnlyShell\"");
    assert_contains(text, "\"source_resolver_kind\":\"fake-in-memory\"");
    assert_contains(text, "\"source_ready_for_test_only_resolver_shell_contract\":true");
    assert_contains(text, "\"source_test_only_shell\":true");
    assert_contains(text, "\"source_read_only_contract\":true");
    assert_contains(text, "\"source_fake_resolver_only\":true");
    assert_contains(text, "\"source_handle_only_request\":true");
    assert_contains(text, "\"source_request_shape_field_count\":9");
    assert_contains(text, "\"source_response_shape_field_count\":13");
    assert_contains(text, "\"source_failure_mapping_count\":7");
    assert_contains(text, "\"source_guard_condition_count\":10");
    assert_contains(text, "\"source_check_count\":20");
    assert_contains(text, "\"source_passed_check_count\":20");
    assert_contains(text, "\"source_production_blocker_count\":0");
    assert_contains(text, "\"source_warning_count\":2");
    assert_contains(text, "\"source_recommendation_count\":2");
}

void assert_source_v263_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v263_ready\":true");
    assert_contains(text, "\"source_node_v263_verification_state\":\"sandbox-endpoint-credential-resolver-disabled-precheck-upstream-echo-verification-ready\"");
    assert_contains(text, "\"source_node_v263_verification_mode\":\"java-v106-plus-mini-kv-v115-disabled-credential-resolver-precheck-upstream-echo-verification-only\"");
    assert_contains(text, "\"source_node_v263_span\":\"Node v262 + Java v106 + mini-kv v115\"");
    assert_contains(text, "\"source_node_v262_ready\":true");
    assert_contains(text, "\"source_java_v106_echo_ready\":true");
    assert_contains(text, "\"source_mini_kv_v115_non_participation_ready\":true");
    assert_contains(text, "\"source_disabled_precheck_aligned\":true");
    assert_contains(text, "\"source_required_env_handles_aligned\":true");
    assert_contains(text, "\"source_opt_in_gates_aligned\":true");
    assert_contains(text, "\"source_failure_taxonomy_aligned\":true");
    assert_contains(text, "\"source_dry_run_response_shape_aligned\":true");
    assert_contains(text, "\"source_inherited_no_go_conditions_aligned\":true");
    assert_contains(text, "\"source_upstream_actions_still_disabled\":true");
    assert_contains(text, "\"source_node_v263_check_count\":19");
    assert_contains(text, "\"source_node_v263_passed_check_count\":19");
    assert_contains(text, "\"source_node_v263_production_blocker_count\":0");
}

void assert_shell_contract_details(const std::string& text) {
    assert_contains(text, "\"resolver_shell_contract\":");
    assert_contains(text, "\"shell_name\":\"ManagedAuditSandboxEndpointCredentialResolverTestOnlyShell\"");
    assert_contains(text, "\"shell_mode\":\"test-only-fake-resolver-contract\"");
    assert_contains(text, "\"resolver_kind\":\"fake-in-memory\"");
    assert_contains(text, "\"real_resolver_implemented\":false");
    assert_contains(text, "\"real_secret_provider_allowed\":false");
    assert_contains(text, "\"fake_resolver_only\":true");
    assert_contains(text, "\"test_only_shell\":true");
    assert_contains(text, "\"read_only_contract\":true");
    assert_contains(text, "\"handle_only_request\":true");
    assert_contains(text, "\"resolver_client_may_be_instantiated_for_production\":false");
    assert_contains(text, "\"credential_value_may_be_loaded\":false");
    assert_contains(text, "\"raw_endpoint_url_may_be_parsed\":false");
    assert_contains(text, "\"external_request_may_be_sent\":false");
    assert_contains(text, "\"request_shape_field_count\":9");
    assert_contains(text, "\"response_shape_field_count\":13");
    assert_contains(text, "\"failure_mapping_count\":7");
    assert_contains(text, "\"guard_condition_count\":10");
}

void assert_request_and_response_shape(const std::string& text) {
    assert_contains(text, "\"requestId\"");
    assert_contains(text, "\"operation\"");
    assert_contains(text, "\"credentialHandle\"");
    assert_contains(text, "\"endpointHandle\"");
    assert_contains(text, "\"resolverPolicyHandle\"");
    assert_contains(text, "\"approvalMarker\"");
    assert_contains(text, "\"approvalCorrelationId\"");
    assert_contains(text, "\"dryRun\"");
    assert_contains(text, "\"fakeResolverOnly\"");
    assert_contains(text, "\"credential_handle_only\":true");
    assert_contains(text, "\"credential_value_accepted\":false");
    assert_contains(text, "\"endpoint_handle_only\":true");
    assert_contains(text, "\"raw_endpoint_url_accepted\":false");
    assert_contains(text, "\"payload_may_contain_secrets\":false");
    assert_contains(text, "\"status\"");
    assert_contains(text, "\"code\"");
    assert_contains(text, "\"fake_resolver_response_only\":true");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"connects_managed_audit\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"production_record_written\":false");
}

void assert_failure_mapping_and_guards(const std::string& text) {
    assert_contains(text, "\"source_failure_code\":\"RESOLVER_DISABLED\"");
    assert_contains(text, "\"shell_failure_code\":\"TEST_ONLY_RESOLVER_DISABLED\"");
    assert_contains(text, "\"mapped_action\":\"return-fake-failure\"");
    assert_contains(text, "\"source_failure_code\":\"APPROVAL_MARKER_MISSING\"");
    assert_contains(text, "\"source_failure_code\":\"CREDENTIAL_HANDLE_MISSING\"");
    assert_contains(text, "\"source_failure_code\":\"CREDENTIAL_VALUE_REQUESTED\"");
    assert_contains(text, "\"source_failure_code\":\"RAW_ENDPOINT_URL_REQUESTED\"");
    assert_contains(text, "\"source_failure_code\":\"EXTERNAL_REQUEST_REQUESTED\"");
    assert_contains(text, "\"source_failure_code\":\"SCHEMA_MIGRATION_REQUESTED\"");
    assert_contains(text, "\"mapped_action\":\"pause-and-do-not-resolve\"");
    assert_contains(text, "\"retryable\":false");
    assert_contains(text, "\"code\":\"SOURCE_V263_READY\"");
    assert_contains(text, "\"code\":\"FAKE_RESOLVER_ONLY\"");
    assert_contains(text, "\"code\":\"CREDENTIAL_HANDLE_ONLY\"");
    assert_contains(text, "\"code\":\"ENDPOINT_HANDLE_ONLY\"");
    assert_contains(text, "\"code\":\"RESOLVER_POLICY_HANDLE_REQUIRED\"");
    assert_contains(text, "\"code\":\"APPROVAL_MARKER_REQUIRED\"");
    assert_contains(text, "\"code\":\"UPSTREAM_ACTIONS_DISABLED\"");
    assert_contains(text, "\"code\":\"NO_SECRET_PROVIDER\"");
    assert_contains(text, "\"code\":\"NO_EXTERNAL_REQUEST\"");
    assert_contains(text, "\"code\":\"NO_SCHEMA_MIGRATION\"");
}

void assert_probe_and_boundary_flags(const std::string& text) {
    assert_contains(text, "\"request_id\":\"managed-audit-v264-test-only-resolver-shell-probe\"");
    assert_contains(text, "\"accepted_by_fake_resolver\":true");
    assert_contains(text, "\"response_status\":\"fake-resolver-accepted\"");
    assert_contains(text, "\"response_code\":\"TEST_ONLY_FAKE_RESOLVER\"");
    assert_contains(text, "\"fake_resolver_probe_no_credential_read\":true");
    assert_contains(text, "\"fake_resolver_probe_no_external_request\":true");
    assert_contains(text, "\"fake_resolver_probe_no_production_write\":true");
    assert_contains(text, "\"test_only_resolver_shell_contract_only\":true");
    assert_contains(text, "\"fake_resolver_probe_executed\":false");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"connection_execution_allowed\":false");
    assert_contains(text, "\"write_commands_executed\":false");
    assert_contains(text, "\"admin_commands_executed\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"production_record_written\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_included\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"schema_migration_execution_allowed\":false");
    assert_contains(text, "\"restore_execution_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"order_authoritative\":false");
    assert_contains(text, "\"managed_audit_storage_backend\":false");
}

} // namespace

int main() {
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "test-only-resolver-shell-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest = minikv::runtime_evidence_receipts::
        test_only_resolver_shell_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-test-only-resolver-shell-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v116\"");
    assert_contains(receipt, "\"path\":\"fixtures/release/test-only-resolver-shell-non-participation-receipt.json\"");
    assert_contains(receipt, "\"test_only_resolver_shell_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v116\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/116/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_contract_shape(receipt);
    assert_source_v263_shape(receipt);
    assert_shell_contract_details(receipt);
    assert_request_and_response_shape(receipt);
    assert_failure_mapping_and_guards(receipt);
    assert_probe_and_boundary_flags(receipt);
    assert_contains(receipt, "test-only resolver shell non-participation receipt only");
    assert_contains(receipt, "does not instantiate a resolver client or secret provider");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v265 test-only resolver shell upstream echo verification\"");
    assert_contains(smoke, "\"test_only_resolver_shell_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_contract_shape(smoke);
    assert_source_v263_shape(smoke);
    assert_shell_contract_details(smoke);
    assert_probe_and_boundary_flags(smoke);
    assert_contains(smoke,
                    "Node v265 may verify the mini-kv v116 test-only resolver shell non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_test_only_resolver_shell_non_participation_receipt_tests\"");
    assert_contains(manifest,
                    "\"path\":\"fixtures/release/test-only-resolver-shell-non-participation-receipt.json\"");
    assert_contains(manifest, "\"test_only_resolver_shell_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest,
                    "SMOKEJSON exposes test_only_resolver_shell_non_participation_receipt for Node v265");
    assert_source_contract_shape(manifest);
    assert_source_v263_shape(manifest);
    assert_failure_mapping_and_guards(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"test_only_resolver_shell_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/test-only-resolver-shell-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v116\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/116/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_contract_shape(result.response);
    assert_source_v263_shape(result.response);
    assert_shell_contract_details(result.response);
    assert_request_and_response_shape(result.response);
    assert_failure_mapping_and_guards(result.response);
    assert_probe_and_boundary_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
