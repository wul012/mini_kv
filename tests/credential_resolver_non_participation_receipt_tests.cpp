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

void assert_source_decision_shape(const std::string& text) {
    assert_contains(text, "\"source_decision\":\"Node v260 sandbox endpoint credential resolver decision record\"");
    assert_contains(text, "\"consumer_hint\":\"Node v261 credential resolver upstream echo verification\"");
    assert_contains(text, "\"source_decision_profile_version\":\"managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-decision-record.v1\"");
    assert_contains(text, "\"source_decision_state\":\"sandbox-endpoint-credential-resolver-decision-record-ready\"");
    assert_contains(text, "\"source_record_mode\":\"sandbox-endpoint-credential-resolver-decision-record-only\"");
    assert_contains(text, "\"source_decision_scope\":\"managed-audit-sandbox-endpoint-credential-resolver\"");
    assert_contains(text, "\"source_decision_status\":\"human-review-required-before-credential-resolution\"");
    assert_contains(text, "\"source_span\":\"Node v259 sandbox endpoint handle upstream echo verification\"");
    assert_contains(text, "\"source_ready_for_credential_resolver_decision_record\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_read_only_decision_record\":true");
    assert_contains(text, "\"source_credential_resolver_decision_only\":true");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_stores_managed_audit_credential\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_credential_value_loaded\":false");
    assert_contains(text, "\"source_credential_value_included\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_raw_endpoint_url_included\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
    assert_contains(text, "\"source_required_decision_field_count\":8");
    assert_contains(text, "\"source_explicit_no_go_condition_count\":9");
    assert_contains(text, "\"source_check_count\":20");
    assert_contains(text, "\"source_passed_check_count\":20");
    assert_contains(text, "\"source_production_blocker_count\":0");
    assert_contains(text, "\"source_warning_count\":2");
    assert_contains(text, "\"source_recommendation_count\":2");
    assert_contains(text, "\"source_node_v259_ready\":true");
    assert_contains(text, "\"source_node_v259_blocks_real_connection\":true");
    assert_contains(text, "\"source_node_v259_credential_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v259_raw_endpoint_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v259_write_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v259_auto_start_boundary_aligned\":true");
    assert_contains(text, "\"source_node_v259_keeps_mini_kv_non_participant\":true");
    assert_contains(text, "\"source_node_v259_evidence_file_count\":6");
    assert_contains(text, "\"source_node_v259_matched_snippet_count\":39");
    assert_contains(text, "\"source_upstream_actions_still_disabled\":true");
}

void assert_decision_record_shape(const std::string& text) {
    assert_contains(text, "\"endpoint_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE\"");
    assert_contains(text, "\"credential_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"resolver_policy_handle\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_RESOLVER_POLICY_HANDLE\"");
    assert_contains(text, "\"approval_marker\":\"ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_APPROVAL_MARKER\"");
    assert_contains(text, "\"operator_identity_required\":true");
    assert_contains(text, "\"approval_correlation_required\":true");
    assert_contains(text, "\"resolver_mode\":\"policy-record-only-no-value-read\"");
    assert_contains(text, "\"resolver_candidate_implementation\":\"not-implemented\"");
    assert_contains(text, "\"required_decision_field_count\":8");
    assert_contains(text, "\"explicit_no_go_condition_count\":9");
    assert_contains(text, "\"credential_value_may_be_read\":false");
    assert_contains(text, "\"credential_value_may_be_loaded\":false");
    assert_contains(text, "\"credential_value_may_be_stored\":false");
    assert_contains(text, "\"raw_endpoint_url_may_be_parsed\":false");
    assert_contains(text, "\"managed_audit_connection_may_open\":false");
    assert_contains(text, "\"external_request_may_be_sent\":false");
    assert_contains(text, "\"node_may_start_java_or_mini_kv\":false");
    assert_contains(text, "\"mini_kv_may_act_as_managed_audit_storage\":false");
    assert_contains(text, "\"approval_ledger_may_be_written\":false");
    assert_contains(text, "\"id\":\"endpoint-handle\"");
    assert_contains(text, "\"id\":\"credential-handle\"");
    assert_contains(text, "\"id\":\"resolver-policy-handle\"");
    assert_contains(text, "\"id\":\"approval-marker\"");
    assert_contains(text, "\"id\":\"operator-identity\"");
    assert_contains(text, "\"id\":\"approval-correlation\"");
    assert_contains(text, "\"id\":\"redaction-policy\"");
    assert_contains(text, "\"id\":\"fallback-rotation-plan\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_REQUIRED\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_REQUIRED\"");
    assert_contains(text, "\"REAL_CONNECTION_REQUIRED\"");
    assert_contains(text, "\"MINI_KV_BACKEND_REQUIRED\"");
    assert_contains(text, "\"PRODUCTION_WINDOW_REQUIRED\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v105\",\"mini-kv v114\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"dry_run_only\":true");
    assert_contains(text, "\"credential_resolver_decision_only\":true");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
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
    assert_contains(text, "\"approval_ledger_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_write_executed\":false");
    assert_contains(text, "\"credential_value_required\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"credential_value_stored\":false");
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
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "credential-resolver-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::credential_resolver_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-credential-resolver-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v114\"");
    assert_contains(receipt, "\"path\":\"fixtures/release/credential-resolver-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v114\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/114/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_decision_shape(receipt);
    assert_decision_record_shape(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "\"credential resolver non-participation receipt only\"");
    assert_contains(receipt, "does not instantiate a secret provider");
    assert_contains(receipt, "does not execute schema migration or restore/load/compact/SETNXEX");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v261 credential resolver upstream echo verification\"");
    assert_contains(smoke, "\"credential_resolver_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_decision_shape(smoke);
    assert_decision_record_shape(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke, "Node v261 may verify the mini-kv v114 credential resolver non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_credential_resolver_non_participation_receipt_tests\"");
    assert_contains(manifest, "\"path\":\"fixtures/release/credential-resolver-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(manifest, "SMOKEJSON exposes credential_resolver_non_participation_receipt for Node v261");
    assert_source_decision_shape(manifest);
    assert_decision_record_shape(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_non_participation_receipt\":");
    assert_contains(result.response,
                    "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v114\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/114/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_decision_shape(result.response);
    assert_decision_record_shape(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
