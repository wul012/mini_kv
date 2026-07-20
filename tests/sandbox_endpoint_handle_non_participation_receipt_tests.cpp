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

void assert_source_preflight_shape(const std::string& text) {
    assert_contains(text, "\"source_preflight\":\"Node v258 sandbox endpoint handle preflight review\"");
    assert_contains(text, "\"consumer_hint\":\"Node v259 sandbox endpoint handle upstream echo verification\"");
    assert_contains(text, "\"source_preflight_profile_version\":\"managed-audit-manual-sandbox-connection-sandbox-"
                          "endpoint-handle-preflight-review.v1\"");
    assert_contains(text, "\"source_review_state\":\"sandbox-endpoint-handle-preflight-review-ready\"");
    assert_contains(text, "\"source_review_mode\":\"sandbox-endpoint-handle-preflight-review-only\"");
    assert_contains(text, "\"source_span\":\"Node v257\"");
    assert_contains(text, "\"source_ready_for_sandbox_endpoint_handle_preflight_review\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_read_only_preflight_review\":true");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_stores_managed_audit_credential\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_raw_endpoint_url_included\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_required_review_item_count\":7");
    assert_contains(text, "\"source_completed_review_item_count\":7");
    assert_contains(text, "\"source_forbidden_operation_count\":7");
    assert_contains(text, "\"source_check_count\":19");
    assert_contains(text, "\"source_passed_check_count\":19");
    assert_contains(text, "\"source_production_blocker_count\":0");
    assert_contains(text, "\"source_warning_count\":2");
    assert_contains(text, "\"source_recommendation_count\":2");
    assert_contains(text, "\"source_node_v257_ready\":true");
    assert_contains(text, "\"source_node_v257_boundaries_aligned\":true");
    assert_contains(text, "\"source_node_v257_evidence_file_count\":6");
    assert_contains(text, "\"source_node_v257_matched_snippet_count\":33");
    assert_contains(text, "\"source_node_v257_ready_for_node_v258_preflight_review\":true");
    assert_contains(text, "\"source_upstream_actions_still_disabled\":true");
}

void assert_review_shape(const std::string& text) {
    assert_contains(text, "\"endpoint_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE\"");
    assert_contains(text, "\"credential_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"owner_approval_artifact_id\":\"owner-approval-artifact-review-only\"");
    assert_contains(text, "\"schema_rehearsal_id\":\"schema-migration-rehearsal-review-only\"");
    assert_contains(text, "\"operator_window_marker\":\"manual-sandbox-endpoint-window-review-only\"");
    assert_contains(text, "\"endpoint_handle_reviewed\":true");
    assert_contains(text, "\"credential_handle_reviewed\":true");
    assert_contains(text, "\"owner_approval_artifact_reviewed\":true");
    assert_contains(text, "\"allowlist_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_NETWORK_ALLOWLIST_HANDLE\"");
    assert_contains(text, "\"raw_host_included\":false");
    assert_contains(text, "\"cidr_included\":false");
    assert_contains(text, "\"policy_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_TLS_POLICY_HANDLE\"");
    assert_contains(text, "\"certificate_material_included\":false");
    assert_contains(text, "\"private_key_included\":false");
    assert_contains(text, "\"policy_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_REDACTION_POLICY_HANDLE\"");
    assert_contains(text, "\"credential_value_redacted\":true");
    assert_contains(text, "\"raw_endpoint_url_redacted\":true");
    assert_contains(text, "\"payload_secret_redacted\":true");
    assert_contains(text, "\"manual_window_required\":true");
    assert_contains(text, "\"window_open\":false");
    assert_contains(text, "\"execution_blocked_until_window_open\":true");
    assert_contains(text, "\"operator_identity_required\":true");
    assert_contains(text, "\"approval_correlation_required\":true");
    assert_contains(text, "\"endpoint handle review\"");
    assert_contains(text, "\"network allowlist review\"");
    assert_contains(text, "\"TLS policy review\"");
    assert_contains(text, "\"redaction policy review\"");
    assert_contains(text, "\"operator window review\"");
    assert_contains(text, "\"start Java or mini-kv\"");
    assert_contains(text, "\"promote mini-kv to managed audit storage backend\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v104\",\"mini-kv v113\"]");
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
    assert_contains(text, "\"production_record_written\":false");
    assert_contains(text, "\"approval_ledger_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_write_executed\":false");
    assert_contains(text, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(text, "\"credential_value_required\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"credential_value_included\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"raw_endpoint_url_included\":false");
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
        std::filesystem::path{"fixtures"} / "release" / "sandbox-endpoint-handle-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::sandbox_endpoint_handle_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-sandbox-endpoint-handle-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v113\"");
    assert_contains(receipt, "\"path\":\"fixtures/release/sandbox-endpoint-handle-non-participation-receipt.json\"");
    assert_contains(receipt, "\"current_release_version\":\"v113\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/113/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_preflight_shape(receipt);
    assert_review_shape(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "\"sandbox endpoint handle non-participation receipt only\"");
    assert_contains(receipt, "does not parse raw endpoint URLs");
    assert_contains(receipt, "does not execute restore/load/compact/SETNXEX");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v259 sandbox endpoint handle upstream echo verification\"");
    assert_contains(smoke, "\"sandbox_endpoint_handle_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_preflight_shape(smoke);
    assert_review_shape(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke, "Node v259 may verify the mini-kv v113 sandbox endpoint handle non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_sandbox_endpoint_handle_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"path\":\"fixtures/release/sandbox-endpoint-handle-non-participation-receipt.json\"");
    assert_contains(manifest, "\"sandbox_endpoint_handle_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest, "SMOKEJSON exposes sandbox_endpoint_handle_non_participation_receipt for Node v259");
    assert_source_preflight_shape(manifest);
    assert_review_shape(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"sandbox_endpoint_handle_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/sandbox-endpoint-handle-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v113\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/113/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_preflight_shape(result.response);
    assert_review_shape(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
    return 0;
}
