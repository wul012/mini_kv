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
    assert_contains(text, "\"source_precheck\":\"Node v252 disabled adapter client precheck\"");
    assert_contains(text, "\"source_shell\":\"Node v253 test-only adapter shell contract\"");
    assert_contains(text, "\"consumer_hint\":\"Node v254 disabled adapter client upstream echo verification\"");
    assert_contains(text, "\"source_precheck_profile_version\":\"managed-audit-manual-sandbox-connection-disabled-"
                          "adapter-client-precheck.v1\"");
    assert_contains(text, "\"source_precheck_state\":\"disabled-adapter-client-precheck-ready\"");
    assert_contains(text, "\"source_adapter_mode\":\"disabled-client-precheck-only\"");
    assert_contains(text, "\"source_required_env_handle_count\":5");
    assert_contains(text, "\"source_failure_class_count\":6");
    assert_contains(text, "\"source_dry_run_response_field_count\":10");
    assert_contains(text, "\"source_reused_no_go_condition_count\":8");
    assert_contains(text, "\"source_ready_for_disabled_adapter_client_precheck\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_read_only_precheck\":true");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_stores_managed_audit_credential\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
    assert_contains(text, "\"source_client_implementation_status\":\"not-implemented\"");
    assert_contains(text, "\"source_client_may_be_instantiated\":false");
    assert_contains(text, "\"source_external_request_may_be_sent\":false");
    assert_contains(text, "\"source_credential_value_may_be_loaded\":false");
    assert_contains(text, "\"source_opt_in_gate_required\":true");
    assert_contains(text, "\"source_opt_in_gate\":\"ORDEROPS_MANAGED_AUDIT_ADAPTER_CLIENT_ENABLED\"");
    assert_contains(text, "\"source_opt_in_gate_default\":\"false\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_TIMEOUT_BUDGET_MS\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_REQUESTED\"");
    assert_contains(text, "\"MANUAL_WINDOW_NOT_OPEN\"");
}

void assert_source_shell_shape(const std::string& text) {
    assert_contains(text, "\"source_shell_profile_version\":\"managed-audit-manual-sandbox-connection-test-only-"
                          "adapter-shell-contract.v1\"");
    assert_contains(text, "\"source_shell_state\":\"test-only-adapter-shell-contract-ready\"");
    assert_contains(text, "\"source_shell_mode\":\"test-only-fake-transport-contract\"");
    assert_contains(text, "\"source_transport_kind\":\"fake-in-memory\"");
    assert_contains(text, "\"source_request_shape_field_count\":8");
    assert_contains(text, "\"source_response_shape_field_count\":9");
    assert_contains(text, "\"source_failure_mapping_count\":6");
    assert_contains(text, "\"source_guard_condition_count\":7");
    assert_contains(text, "\"source_fake_transport_only\":true");
    assert_contains(text, "\"source_real_client_implemented\":false");
    assert_contains(text, "\"source_real_transport_allowed\":false");
    assert_contains(text, "\"source_client_may_be_instantiated_for_production\":false");
    assert_contains(text, "\"source_fake_transport_probe_covered\":true");
    assert_contains(text, "\"source_fake_transport_probe_no_external_request\":true");
    assert_contains(text, "\"source_fake_transport_probe_no_credential_read\":true");
    assert_contains(text, "\"source_fake_transport_probe_no_production_write\":true");
    assert_contains(text, "\"credential_handle_only\":true");
    assert_contains(text, "\"credential_value_accepted\":false");
    assert_contains(text, "\"endpoint_handle_only\":true");
    assert_contains(text, "\"external_url_accepted\":false");
    assert_contains(text, "\"payload_may_contain_secrets\":false");
    assert_contains(text, "\"fake_transport_response_only\":true");
    assert_contains(text, "\"production_record_written\":false");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"dry_run_only\":true");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"connection_execution_allowed\":false");
    assert_contains(text, "\"write_commands_executed\":false");
    assert_contains(text, "\"admin_commands_executed\":false");
    assert_contains(text, "\"runtime_write_observed\":false");
    assert_contains(text, "\"managed_audit_store\":false");
    assert_contains(text, "\"managed_audit_storage_backend\":false");
    assert_contains(text, "\"sandbox_audit_storage_backend\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"managed_audit_write_executed\":false");
    assert_contains(text, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(text, "\"credential_value_required\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(text, "\"schema_migration_execution_allowed\":false");
    assert_contains(text, "\"restore_execution_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path =
        std::filesystem::path{"fixtures"} / "release" / "disabled-adapter-client-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::disabled_adapter_client_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-disabled-adapter-client-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v111\"");
    assert_contains(receipt, "\"path\":\"fixtures/release/disabled-adapter-client-non-participation-receipt.json\"");
    assert_contains(receipt, "\"current_release_version\":\"v111\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/111/\"");
    assert_contains(receipt, "\"current_runtime_fixture_release_version\":\"v102\"");
    assert_contains(receipt, "\"current_runtime_fixture_artifact_path_hint\":\"c/102/\"");
    assert_contains(receipt, "\"current_live_read_session_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_precheck_shape(receipt);
    assert_source_shell_shape(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "\"disabled adapter client non-participation receipt only\"");
    assert_contains(receipt, "\"no LOAD/COMPACT/RESTORE/SETNXEX execution\"");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "external_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v254 disabled adapter client upstream echo verification\"");
    assert_contains(smoke, "\"disabled_adapter_client_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_precheck_shape(smoke);
    assert_source_shell_shape(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke, "Node v254 may verify the mini-kv v111 disabled adapter client non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_disabled_adapter_client_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"path\":\"fixtures/release/disabled-adapter-client-non-participation-receipt.json\"");
    assert_contains(manifest, "\"disabled_adapter_client_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest, "SMOKEJSON exposes disabled_adapter_client_non_participation_receipt for Node v254");
    assert_source_precheck_shape(manifest);
    assert_source_shell_shape(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"disabled_adapter_client_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/disabled-adapter-client-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v111\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/111/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_precheck_shape(result.response);
    assert_source_shell_shape(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
    return 0;
}
