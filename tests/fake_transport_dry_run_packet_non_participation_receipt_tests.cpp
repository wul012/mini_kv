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

void assert_source_packet_shape(const std::string& text) {
    assert_contains(text, "\"source_packet\":\"Node v255 fake transport adapter dry-run verification packet\"");
    assert_contains(text, "\"source_archive_verification\":\"Node v256 fake transport packet archive verification\"");
    assert_contains(text, "\"consumer_hint\":\"Node v257 fake transport packet upstream echo verification\"");
    assert_contains(text, "\"source_packet_profile_version\":\"managed-audit-manual-sandbox-connection-fake-transport-adapter-dry-run-verification-packet.v1\"");
    assert_contains(text, "\"source_packet_state\":\"fake-transport-adapter-dry-run-verification-packet-ready\"");
    assert_contains(text, "\"source_archive_state\":\"fake-transport-packet-archive-verification-ready\"");
    assert_contains(text, "\"source_archive_reruns_fake_transport_behavior\":false");
    assert_contains(text, "\"source_packet_mode\":\"fake-transport-adapter-dry-run-verification-only\"");
    assert_contains(text, "\"source_span\":\"Node v253 + Node v254\"");
    assert_contains(text, "\"source_ready_for_fake_transport_packet\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_fake_transport_only\":true");
    assert_contains(text, "\"source_dry_run_only\":true");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
    assert_contains(text, "\"source_request_shape_field_count\":8");
    assert_contains(text, "\"source_response_shape_field_count\":9");
    assert_contains(text, "\"source_failure_mapping_count\":6");
    assert_contains(text, "\"source_timeout_budget_ms\":15000");
    assert_contains(text, "\"source_cleanup_artifact_count\":0");
    assert_contains(text, "\"source_budget_spent\":false");
    assert_contains(text, "\"source_timer_started\":false");
    assert_contains(text, "\"source_cleanup_in_memory_only\":true");
    assert_contains(text, "\"source_temporary_directory_created\":false");
    assert_contains(text, "\"source_temporary_file_created\":false");
    assert_contains(text, "\"source_node_service_started_by_packet\":false");
    assert_contains(text, "\"source_java_started\":false");
    assert_contains(text, "\"source_mini_kv_started\":false");
    assert_contains(text, "\"source_external_audit_service_started\":false");
}

void assert_request_response_shape(const std::string& text) {
    assert_contains(text, "\"request_id\":\"managed-audit-v255-fake-transport-dry-run\"");
    assert_contains(text, "\"operation\":\"managed-audit-sandbox-connection-dry-run\"");
    assert_contains(text, "\"transport_kind\":\"fake-in-memory\"");
    assert_contains(text, "\"credential_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"endpoint_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE\"");
    assert_contains(text, "\"owner_approval_artifact_id\":\"owner-approval-artifact-review-only\"");
    assert_contains(text, "\"timeout_budget_ms\":15000");
    assert_contains(text, "\"credential_value_included\":false");
    assert_contains(text, "\"raw_endpoint_url_included\":false");
    assert_contains(text, "\"payload_may_contain_secrets\":false");
    assert_contains(text, "\"status\":\"fake-transport-dry-run-accepted\"");
    assert_contains(text, "\"code\":\"TEST_ONLY_FAKE_TRANSPORT_DRY_RUN\"");
    assert_contains(text, "\"connection_attempted\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"production_record_written\":false");
    assert_contains(text, "\"budget_source\":\"operator-review-field\"");
    assert_contains(text, "\"budget_spent\":false");
    assert_contains(text, "\"timer_started\":false");
    assert_contains(text, "\"timeout_classifiable\":true");
    assert_contains(text, "\"mapped_failure_count\":6");
    assert_contains(text, "\"cleanup_artifact_count\":0");
    assert_contains(text, "\"java_started\":false");
    assert_contains(text, "\"mini_kv_started\":false");
    assert_contains(text, "\"external_audit_service_started\":false");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"dry_run_only\":true");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"external_audit_service_auto_start_allowed\":false");
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
    assert_contains(text, "\"temporary_directory_created\":false");
    assert_contains(text, "\"temporary_file_created\":false");
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
                              "fake-transport-dry-run-packet-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::fake_transport_dry_run_packet_non_participation_receipt_digest(
            read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-fake-transport-dry-run-packet-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v112\"");
    assert_contains(receipt, "\"path\":\"fixtures/release/fake-transport-dry-run-packet-non-participation-receipt.json\"");
    assert_contains(receipt, "\"current_release_version\":\"v112\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/112/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_packet_shape(receipt);
    assert_request_response_shape(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "\"fake transport dry-run packet non-participation receipt only\"");
    assert_contains(receipt, "\"no LOAD/COMPACT/RESTORE/SETNXEX execution\"");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v257 fake transport packet upstream echo verification\"");
    assert_contains(smoke, "\"fake_transport_dry_run_packet_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_packet_shape(smoke);
    assert_request_response_shape(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke, "Node v257 may verify the mini-kv v112 fake transport dry-run packet non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_fake_transport_dry_run_packet_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"path\":\"fixtures/release/fake-transport-dry-run-packet-non-participation-receipt.json\"");
    assert_contains(manifest, "\"fake_transport_dry_run_packet_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest, "SMOKEJSON exposes fake_transport_dry_run_packet_non_participation_receipt for Node v257");
    assert_source_packet_shape(manifest);
    assert_request_response_shape(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"fake_transport_dry_run_packet_non_participation_receipt\":");
    assert_contains(result.response, "\"receipt_fixture_path\":\"fixtures/release/fake-transport-dry-run-packet-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v112\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/112/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_packet_shape(result.response);
    assert_request_response_shape(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
