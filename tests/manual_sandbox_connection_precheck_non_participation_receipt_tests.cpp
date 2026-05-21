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

void assert_precheck_packet_shape(const std::string& text) {
    assert_contains(text, "\"source_packet\":\"Node v245 manual sandbox connection precheck packet\"");
    assert_contains(text, "\"source_verification\":\"Node v244 manual sandbox dry-run command upstream echo verification\"");
    assert_contains(text, "\"consumer_hint\":\"Node v246 manual sandbox connection precheck upstream receipt verification\"");
    assert_contains(text, "\"source_precheck_profile_version\":\"managed-audit-manual-sandbox-connection-precheck-packet.v1\"");
    assert_contains(text, "\"source_precheck_state\":\"manual-sandbox-connection-precheck-packet-ready\"");
    assert_contains(text, "\"source_precheck_packet_mode\":\"manual-sandbox-connection-precheck-packet-only\"");
    assert_contains(text, "\"source_precheck_source_span\":\"Node v244 upstream echo verification\"");
    assert_contains(text, "\"source_precheck_item_count\":7");
    assert_contains(text, "\"source_required_operator_field_count\":6");
    assert_contains(text, "\"source_timeout_budget_ms\":15000");
    assert_contains(text, "\"source_ready_for_precheck_packet\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_read_only_precheck_packet\":true");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_stores_managed_audit_credential\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
    assert_contains(text, "\"source_precheck_digest_required\":true");
    assert_contains(text, "\"source_precheck_digest_value_read\":false");
    assert_contains(text, "\"source_node_v244_ready\":true");
    assert_contains(text, "\"source_node_v244_boundaries_aligned\":true");
    assert_contains(text, "\"source_boundary_dry_run_only\":true");
    assert_contains(text, "\"source_boundary_actual_connection_attempted\":false");
    assert_contains(text, "\"source_boundary_managed_audit_state_write_requested\":false");
    assert_contains(text, "\"source_boundary_schema_migration_requested\":false");
    assert_contains(text, "\"source_boundary_approval_ledger_write_requested\":false");
    assert_contains(text, "\"source_boundary_java_sql_execution_requested\":false");
    assert_contains(text, "\"source_boundary_mini_kv_write_permission_requested\":false");
    assert_contains(text, "\"source_boundary_upstream_service_auto_start_requested\":false");
    assert_contains(text, "\"owner approval artifact\"");
    assert_contains(text, "\"credential handle review\"");
    assert_contains(text, "\"schema migration rehearsal id\"");
    assert_contains(text, "\"operator window\"");
    assert_contains(text, "\"rollback path\"");
    assert_contains(text, "\"manual abort marker\"");
    assert_contains(text, "\"timeout policy\"");
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
    assert_contains(text, "\"managed_audit_storage_backend\":false");
    assert_contains(text, "\"sandbox_audit_storage_backend\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"managed_audit_write_executed\":false");
    assert_contains(text, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(text, "\"credential_value_required\":false");
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
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "manual-sandbox-connection-precheck-non-participation-receipt.json";
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
            manual_sandbox_connection_precheck_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-manual-sandbox-connection-precheck-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v108\"");
    assert_contains(receipt, "\"path\":\"fixtures/release/manual-sandbox-connection-precheck-non-participation-receipt.json\"");
    assert_contains(receipt, "\"current_release_version\":\"v108\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/108/\"");
    assert_contains(receipt, "\"current_runtime_fixture_release_version\":\"v102\"");
    assert_contains(receipt, "\"current_runtime_fixture_artifact_path_hint\":\"c/102/\"");
    assert_contains(receipt, "\"current_live_read_session_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_precheck_packet_shape(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "\"precheck non-participation receipt only\"");
    assert_contains(receipt, "\"Node v245 precheck packet field echo only\"");
    assert_contains(receipt, "\"no LOAD/COMPACT/RESTORE/SETNXEX execution\"");
    assert_not_contains(receipt, "credential_value\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v246 manual sandbox connection precheck upstream receipt verification\"");
    assert_contains(smoke, "\"manual_sandbox_connection_precheck_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_precheck_packet_shape(smoke);
    assert_non_participation_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_manual_sandbox_connection_precheck_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"path\":\"fixtures/release/manual-sandbox-connection-precheck-non-participation-receipt.json\"");
    assert_contains(manifest, "\"manual_sandbox_connection_precheck_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest, "SMOKEJSON exposes manual_sandbox_connection_precheck_non_participation_receipt for Node v246");
    assert_precheck_packet_shape(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"manual_sandbox_connection_precheck_non_participation_receipt\":");
    assert_contains(result.response, "\"receipt_fixture_path\":\"fixtures/release/manual-sandbox-connection-precheck-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v108\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/108/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_precheck_packet_shape(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
