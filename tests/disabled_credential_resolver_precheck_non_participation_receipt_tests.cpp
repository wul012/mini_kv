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

void assert_source_precheck_shape(const std::string& text) {
    assert_contains(text, "\"source_precheck\":\"Node v262 disabled credential resolver precheck\"");
    assert_contains(text, "\"source_verification\":\"Node v261 credential resolver upstream echo verification\"");
    assert_contains(text, "\"consumer_hint\":\"Node v263 disabled credential resolver upstream echo verification\"");
    assert_contains(text, "\"source_precheck_profile_version\":\"managed-audit-manual-sandbox-connection-sandbox-"
                          "endpoint-credential-resolver-disabled-precheck.v1\"");
    assert_contains(text, "\"source_precheck_state\":\"sandbox-endpoint-credential-resolver-disabled-precheck-ready\"");
    assert_contains(text, "\"source_precheck_mode\":\"sandbox-endpoint-credential-resolver-disabled-precheck-only\"");
    assert_contains(text, "\"source_span\":\"Node v260 + Java v105 + mini-kv v114\"");
    assert_contains(text, "\"source_ready_for_disabled_credential_resolver_precheck\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_read_only_disabled_precheck\":true");
    assert_contains(text, "\"source_disabled_credential_resolver_precheck_only\":true");
    assert_contains(text, "\"source_credential_resolver_execution_allowed\":false");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_stores_managed_audit_credential\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_credential_value_loaded\":false");
    assert_contains(text, "\"source_credential_value_stored\":false");
    assert_contains(text, "\"source_credential_value_included\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_raw_endpoint_url_included\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_secret_provider_instantiated\":false");
    assert_contains(text, "\"source_resolver_client_instantiated\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
    assert_contains(text, "\"source_required_env_handle_count\":6");
    assert_contains(text, "\"source_opt_in_gate_count\":2");
    assert_contains(text, "\"source_failure_class_count\":7");
    assert_contains(text, "\"source_dry_run_response_field_count\":12");
    assert_contains(text, "\"source_inherited_no_go_condition_count\":9");
    assert_contains(text, "\"source_check_count\":24");
    assert_contains(text, "\"source_passed_check_count\":24");
    assert_contains(text, "\"source_production_blocker_count\":0");
    assert_contains(text, "\"source_warning_count\":2");
    assert_contains(text, "\"source_recommendation_count\":2");
    assert_contains(text, "\"source_node_v261_ready\":true");
    assert_contains(text, "\"source_node_v261_blocks_credential_resolution\":true");
    assert_contains(text, "\"source_node_v261_credential_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v261_raw_endpoint_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v261_connection_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v261_write_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v261_auto_start_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v261_upstream_actions_still_disabled\":true");
}

void assert_precheck_shape(const std::string& text) {
    assert_contains(text, "\"precheck_mode\":\"sandbox-endpoint-credential-resolver-disabled-precheck-only\"");
    assert_contains(text, "\"ready_state\":\"sandbox-endpoint-credential-resolver-disabled-precheck-ready\"");
    assert_contains(text, "\"resolver_implementation_status\":\"not-implemented\"");
    assert_contains(text, "\"secret_provider_implementation_status\":\"not-implemented\"");
    assert_contains(text, "\"resolver_client_may_be_instantiated\":false");
    assert_contains(text, "\"secret_provider_may_be_instantiated\":false");
    assert_contains(text, "\"credential_value_may_be_loaded\":false");
    assert_contains(text, "\"raw_endpoint_url_may_be_parsed\":false");
    assert_contains(text, "\"external_request_may_be_sent\":false");
    assert_contains(text, "\"opt_in_gate_required\":true");
    assert_contains(text, "\"required_env_handle_count\":6");
    assert_contains(text, "\"opt_in_gate_count\":2");
    assert_contains(text, "\"failure_class_count\":7");
    assert_contains(text, "\"dry_run_response_field_count\":12");
    assert_contains(text, "\"inherited_no_go_condition_count\":9");
    assert_contains(text, "\"name\":\"ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_ENABLED\"");
    assert_contains(text, "\"name\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_RESOLUTION_ENABLED\"");
    assert_contains(text, "\"name\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE\"");
    assert_contains(text, "\"name\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"name\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_RESOLVER_POLICY_HANDLE\"");
    assert_contains(text, "\"name\":\"ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_APPROVAL_MARKER\"");
    assert_contains(text, "\"value_required_for_precheck\":false");
    assert_contains(text, "\"credential_value\":false");
    assert_contains(text, "\"raw_endpoint_value\":false");
    assert_contains(text, "\"gate_name\":\"ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_ENABLED\"");
    assert_contains(text, "\"gate_name\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_RESOLUTION_ENABLED\"");
    assert_contains(text, "\"required_value_for_future_resolver\":\"true\"");
    assert_contains(text, "\"current_default\":\"false\"");
    assert_contains(text, "\"precheck_treats_enabled_as_blocked\":true");
    assert_contains(text, "\"operator_approval_required\":true");
    assert_contains(text, "\"RESOLVER_DISABLED\"");
    assert_contains(text, "\"APPROVAL_MARKER_MISSING\"");
    assert_contains(text, "\"CREDENTIAL_HANDLE_MISSING\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_REQUESTED\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_REQUESTED\"");
    assert_contains(text, "\"EXTERNAL_REQUEST_REQUESTED\"");
    assert_contains(text, "\"SCHEMA_MIGRATION_REQUESTED\"");
    assert_contains(text, "\"readyState\"");
    assert_contains(text, "\"resolverMode\"");
    assert_contains(text, "\"resolverClientInstantiated\"");
    assert_contains(text, "\"secretProviderInstantiated\"");
    assert_contains(text, "\"failureClassCount\"");
    assert_contains(text, "\"nextAction\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_REQUIRED\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_REQUIRED\"");
    assert_contains(text, "\"MINI_KV_BACKEND_REQUIRED\"");
    assert_contains(text, "\"PRODUCTION_WINDOW_REQUIRED\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v106\",\"mini-kv v115\"]");
}

void assert_checks_and_flags(const std::string& text) {
    assert_contains(text, "\"source_verification_still_blocks_credential_resolution\":true");
    assert_contains(text, "\"source_verification_still_blocks_credential_value\":true");
    assert_contains(text, "\"source_verification_still_blocks_raw_endpoint\":true");
    assert_contains(text, "\"source_verification_still_blocks_connection\":true");
    assert_contains(text, "\"source_verification_still_blocks_writes\":true");
    assert_contains(text, "\"source_verification_still_blocks_auto_start\":true");
    assert_contains(text, "\"required_env_handles_declared\":true");
    assert_contains(text, "\"env_handles_remain_handle_only\":true");
    assert_contains(text, "\"opt_in_gates_declared\":true");
    assert_contains(text, "\"opt_in_gates_default_disabled\":true");
    assert_contains(text, "\"failure_taxonomy_declared\":true");
    assert_contains(text, "\"dry_run_response_shape_declared\":true");
    assert_contains(text, "\"resolver_implementation_still_absent\":true");
    assert_contains(text, "\"secret_provider_implementation_still_absent\":true");
    assert_contains(text, "\"resolver_client_instantiation_blocked\":true");
    assert_contains(text, "\"secret_provider_instantiation_blocked\":true");
    assert_contains(text, "\"credential_value_load_blocked\":true");
    assert_contains(text, "\"raw_endpoint_parse_blocked\":true");
    assert_contains(text, "\"external_request_blocked\":true");
    assert_contains(text, "\"upstream_actions_still_disabled\":true");
    assert_contains(text, "\"disabled_credential_resolver_precheck_only\":true");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"connection_execution_allowed\":false");
    assert_contains(text, "\"write_commands_executed\":false");
    assert_contains(text, "\"admin_commands_executed\":false");
    assert_contains(text, "\"runtime_write_observed\":false");
    assert_contains(text, "\"managed_audit_storage_backend\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"managed_audit_write_executed\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"credential_value_stored\":false");
    assert_contains(text, "\"credential_value_included\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"raw_endpoint_url_included\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"schema_migration_execution_allowed\":false");
    assert_contains(text, "\"restore_execution_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "disabled-credential-resolver-precheck-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::disabled_credential_resolver_precheck_non_participation_receipt_digest(
            read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt, "\"receipt_version\":\"mini-kv-disabled-credential-resolver-precheck-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v115\"");
    assert_contains(
        receipt, "\"path\":\"fixtures/release/disabled-credential-resolver-precheck-non-participation-receipt.json\"");
    assert_contains(receipt, "\"disabled_credential_resolver_precheck_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v115\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/115/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_precheck_shape(receipt);
    assert_precheck_shape(receipt);
    assert_checks_and_flags(receipt);
    assert_contains(receipt, "disabled credential resolver precheck non-participation receipt only");
    assert_contains(receipt, "does not instantiate a secret provider or resolver client");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v263 disabled credential resolver upstream echo verification\"");
    assert_contains(smoke, "\"disabled_credential_resolver_precheck_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_precheck_shape(smoke);
    assert_precheck_shape(smoke);
    assert_checks_and_flags(smoke);
    assert_contains(
        smoke, "Node v263 may verify the mini-kv v115 disabled credential resolver precheck non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_disabled_credential_resolver_precheck_non_participation_receipt_tests\"");
    assert_contains(
        manifest, "\"path\":\"fixtures/release/disabled-credential-resolver-precheck-non-participation-receipt.json\"");
    assert_contains(manifest, "\"disabled_credential_resolver_precheck_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest,
                    "SMOKEJSON exposes disabled_credential_resolver_precheck_non_participation_receipt for Node v263");
    assert_source_precheck_shape(manifest);
    assert_precheck_shape(manifest);
    assert_checks_and_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"disabled_credential_resolver_precheck_non_participation_receipt\":");
    assert_contains(result.response, "\"receipt_fixture_path\":\"fixtures/release/"
                                     "disabled-credential-resolver-precheck-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v115\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/115/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_precheck_shape(result.response);
    assert_precheck_shape(result.response);
    assert_checks_and_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
    return 0;
}
