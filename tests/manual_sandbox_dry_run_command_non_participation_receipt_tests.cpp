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

void assert_command_package_shape(const std::string& text) {
    assert_contains(text, "\"source_package\":\"Node v241 manual sandbox connection dry-run command package\"");
    assert_contains(text,
                    "\"source_verification\":\"Node v243 manual sandbox dry-run command package verification report\"");
    assert_contains(text, "\"consumer_hint\":\"Node v244 manual sandbox dry-run command upstream echo verification\"");
    assert_contains(text, "\"source_package_mode\":\"manual-sandbox-connection-disabled-dry-run-command-package\"");
    assert_contains(text, "\"source_package_source_span\":\"Node v239 + Node v240 + Java v97 + mini-kv v106\"");
    assert_contains(text, "\"source_package_command_count\":6");
    assert_contains(text, "\"source_package_disabled_command_count\":6");
    assert_contains(text, "\"source_package_disabled_by_default\":true");
    assert_contains(text, "\"source_package_dry_run_only\":true");
    assert_contains(text, "\"source_package_carries_credential_value\":false");
    assert_contains(text, "\"source_package_actual_connection_attempted\":false");
    assert_contains(text, "\"source_package_schema_migration_requested\":false");
    assert_contains(text, "\"source_package_managed_audit_state_write_requested\":false");
    assert_contains(text, "\"source_package_upstream_service_auto_start_requested\":false");
    assert_contains(text, "\"source_package_mini_kv_write_permission_requested\":false");
    assert_contains(text, "\"source_package_digest_required\":true");
    assert_contains(text, "\"source_package_digest_value_read\":false");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_SCHEMA_REHEARSAL_ID\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_ROLLBACK_PATH_ID\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_TIMEOUT_BUDGET_MS\"");
    assert_contains(text, "\"ORDEROPS_MANAGED_AUDIT_MANUAL_ABORT\"");
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
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"managed_audit_write_executed\":false");
    assert_contains(text, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
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
        std::filesystem::path{"fixtures"} / "release" / "manual-sandbox-dry-run-command-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::manual_sandbox_dry_run_command_non_participation_receipt_digest(
            read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt,
                    "\"receipt_version\":\"mini-kv-manual-sandbox-dry-run-command-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v107\"");
    assert_contains(receipt,
                    "\"path\":\"fixtures/release/manual-sandbox-dry-run-command-non-participation-receipt.json\"");
    assert_contains(receipt, "\"current_release_version\":\"v107\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/107/\"");
    assert_contains(receipt, "\"current_runtime_fixture_release_version\":\"v102\"");
    assert_contains(receipt, "\"current_runtime_fixture_artifact_path_hint\":\"c/102/\"");
    assert_contains(receipt, "\"current_live_read_session_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_command_package_shape(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "\"non-participation receipt only\"");
    assert_contains(receipt, "\"Node v241 command package field echo only\"");
    assert_contains(receipt, "\"no LOAD/COMPACT/RESTORE/SETNXEX execution\"");
    assert_not_contains(receipt, "credential_value\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke,
                    "\"consumer_hint\":\"Node v246 manual sandbox connection precheck upstream receipt verification\"");
    assert_contains(smoke, "\"manual_sandbox_dry_run_command_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_command_package_shape(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke, "\"manual_sandbox_dry_run_command_non_participation_receipt\"");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_manual_sandbox_dry_run_command_non_participation_receipt_tests\"");
    assert_contains(manifest,
                    "\"path\":\"fixtures/release/manual-sandbox-dry-run-command-non-participation-receipt.json\"");
    assert_contains(manifest, "\"manual_sandbox_dry_run_command_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest,
                    "SMOKEJSON exposes manual_sandbox_dry_run_command_non_participation_receipt for Node v244");
    assert_command_package_shape(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"manual_sandbox_dry_run_command_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/manual-sandbox-dry-run-command-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v107\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/107/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_command_package_shape(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
    return 0;
}
