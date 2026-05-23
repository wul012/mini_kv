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

void assert_source_node_v317_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v317_reference\":");
    assert_contains(text, "\"source_version\":\"Node v317\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-credential-handle-approval-contract-intake.v1\"");
    assert_contains(text, "\"contract_state\":\"credential-handle-approval-contract-intake-ready\"");
    assert_contains(
        text,
        "\"governance_chain_decision\":\"continue-only-for-credential-handle-approval-contract-intake\"");
    assert_contains(text, "\"target_prerequisite_id\":\"credential-handle-approval\"");
    assert_contains(text, "\"next_java_version\":\"Java v146\"");
    assert_contains(text, "\"next_mini_kv_version\":\"mini-kv v139\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v318\"");
    assert_contains(text, "\"ready_for_parallel_java_v146_mini_kv_v139_echo\":true");
    assert_contains(text, "\"ready_for_node_v318_before_upstream_echo\":false");
    assert_contains(text, "\"contract_digest\":\"298ffb48a00aab4f4630b42fc7b48805185d50a5465938768bd78943e05ae817\"");
    assert_contains(text, "\"contract_name\":\"managed-audit-credential-handle-approval\"");
    assert_contains(text, "\"contract_version\":\"credential-handle-approval.v1\"");
    assert_contains(text, "\"contract_mode\":\"credential-handle-approval-contract-intake-only\"");
    assert_contains(text, "\"source_span\":\"Node v316 closure review + Node v313 catalog\"");
    assert_contains(text, "\"required_field_count\":10");
    assert_contains(text, "\"prohibited_field_count\":8");
    assert_contains(text, "\"rejection_reason_count\":5");
    assert_contains(text, "\"no_go_boundary_count\":9");
    assert_contains(text, "\"upstream_echo_request_count\":2");
}

void assert_contract_fields(const std::string& text) {
    assert_contains(text, "\"credential_handle\"");
    assert_contains(text, "\"approval_correlation_id\"");
    assert_contains(text, "\"operator_identity_handle\"");
    assert_contains(text, "\"reviewer_identity_handle\"");
    assert_contains(text, "\"policy_version\"");
    assert_contains(text, "\"approval_status\"");
    assert_contains(text, "\"issued_at\"");
    assert_contains(text, "\"expires_at\"");
    assert_contains(text, "\"revocation_marker\"");
    assert_contains(text, "\"audit_digest\"");
    assert_contains(text, "\"credential_value\"");
    assert_contains(text, "\"raw_endpoint_url\"");
    assert_contains(text, "\"secret_provider_config\"");
    assert_contains(text, "\"resolver_client_config\"");
    assert_contains(text, "\"provider_client_runtime_binding\"");
    assert_contains(text, "\"external_request_payload\"");
    assert_contains(text, "\"approval_ledger_mutation\"");
    assert_contains(text, "\"schema_migration_sql\"");
    assert_contains(text, "\"CREDENTIAL_HANDLE_MISSING\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_PRESENT\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_PRESENT\"");
    assert_contains(text, "\"PROVIDER_CLIENT_CONFIG_PRESENT\"");
    assert_contains(text, "\"WRITE_OR_MIGRATION_PRESENT\"");
    assert_contains(text, "\"credential_value_read\"");
    assert_contains(text, "\"raw_endpoint_url_parse\"");
    assert_contains(text, "\"secret_provider_instantiation\"");
    assert_contains(text, "\"resolver_client_instantiation\"");
    assert_contains(text, "\"ledger_or_schema_write\"");
    assert_contains(text, "\"runtime_shell_invocation\"");
}

void assert_source_node_v316_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v316_reference\":");
    assert_contains(text, "\"source_version\":\"Node v316\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-signed-human-approval-artifact-prerequisite-closure-review.v1\"");
    assert_contains(text, "\"review_state\":\"signed-human-approval-artifact-prerequisite-closure-review-ready\"");
    assert_contains(text, "\"review_digest\":\"63de25a3ff87d5d9ea8243d0195f3f646bc3bb08ab2aae76533f9a871674444d\"");
    assert_contains(text, "\"completed_prerequisite_count\":2");
    assert_contains(text, "\"remaining_prerequisite_count\":4");
    assert_contains(text, "\"original_prerequisite_count\":6");
    assert_contains(text, "\"next_concrete_prerequisite_id\":\"credential-handle-approval\"");
    assert_contains(text, "\"chain_continuation_allowed\":true");
    assert_contains(text, "\"runtime_shell_still_blocked\":true");
    assert_contains(text, "\"java-mini-kv-decision-echo\"");
    assert_contains(text, "\"signed-human-approval-artifact\"");
    assert_contains(text, "\"endpoint-handle-allowlist-approval\"");
    assert_contains(text, "\"no-network-safety-fixture\"");
    assert_contains(text, "\"abort-rollback-semantics\"");
    assert_contains(text, "\"source_check_count\":17");
    assert_contains(text, "\"source_node_v315_check_count\":23");
}

void assert_transition_summary_and_boundaries(const std::string& text) {
    assert_contains(text, "\"prerequisite_id\":\"credential-handle-approval\"");
    assert_contains(text, "\"before_v317\":\"still-missing\"");
    assert_contains(text, "\"after_v317\":\"contract-intake-defined\"");
    assert_contains(text, "\"closure_requires_upstream_echo\":true");
    assert_contains(text, "\"preserves_signed_human_approval_artifact_closure\":true");
    assert_contains(text, "\"closes_endpoint_handle_allowlist_approval\":false");
    assert_contains(text, "\"closes_no_network_safety_fixture\":false");
    assert_contains(text, "\"closes_abort_rollback_semantics\":false");
    assert_contains(text, "\"check_count\":20");
    assert_contains(text, "\"passed_check_count\":20");
    assert_contains(text, "\"source_node_v316_check_count\":17");
    assert_contains(text, "\"source_node_v316_passed_check_count\":17");
    assert_contains(text, "\"source_remaining_prerequisite_count\":4");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"CREDENTIAL_HANDLE_CONTRACT_DOES_NOT_CLOSE_ALL_PREREQUISITES\"");
    assert_contains(text, "\"CREDENTIAL_HANDLE_APPROVAL_IS_NOT_CREDENTIAL_RESOLUTION\"");
    assert_contains(text, "\"RUN_JAVA_V146_AND_MINI_KV_V139_AFTER_V317_ARCHIVE\"");
    assert_contains(text, "\"KEEP_CREDENTIAL_HANDLE_APPROVAL_NON_SECRET\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v146\",\"mini-kv v139\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"credential_handle_approval_contract_non_participation_receipt_only\":true");
    assert_contains(text, "\"credential_handle_stored\":false");
    assert_contains(text, "\"credential_handle_validated\":false");
    assert_contains(text, "\"credential_handle_resolved\":false");
    assert_contains(text, "\"credential_handle_authoritative\":false");
    assert_contains(text, "\"credential_approval_status_authoritative\":false");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"credential_value_accepted\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"raw_endpoint_url_accepted\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"provider_client_instantiation_allowed\":false");
    assert_contains(text, "\"connects_managed_audit\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_written\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"automatic_upstream_start\":false");
    assert_contains(text, "\"audit_authoritative\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path =
        std::filesystem::path{"fixtures"} / "release" /
        "credential-resolver-credential-handle-approval-contract-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_credential_handle_approval_contract_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-credential-handle-approval-contract-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v139\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-credential-handle-approval-contract-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_credential_handle_approval_contract_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v139\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/139/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v317_shape(receipt);
    assert_contract_fields(receipt);
    assert_source_node_v316_shape(receipt);
    assert_transition_summary_and_boundaries(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_credential_handle_approval_contract_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v317_shape(smoke);
    assert_contract_fields(smoke);
    assert_source_node_v316_shape(smoke);
    assert_transition_summary_and_boundaries(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_credential_handle_approval_contract_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-credential-handle-approval-contract-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_credential_handle_approval_contract_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_credential_handle_approval_contract_non_participation_receipt for Node v318");
    assert_source_node_v317_shape(manifest);
    assert_contract_fields(manifest);
    assert_source_node_v316_shape(manifest);
    assert_transition_summary_and_boundaries(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_credential_handle_approval_contract_non_participation_receipt\":");
    assert_contains(result.response, "\"current_release_version\":\"v139\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/139/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v317_shape(result.response);
    assert_contract_fields(result.response);
    assert_source_node_v316_shape(result.response);
    assert_transition_summary_and_boundaries(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v318 may verify the mini-kv v139 credential resolver credential-handle approval contract non-participation receipt");
    assert_contains(
        result.response,
        "Node v315 may verify the mini-kv v138 credential resolver signed human approval artifact contract non-participation receipt");
    assert_not_contains(result.response, "credential_value\":\"");
    assert_not_contains(result.response, "raw_endpoint_url\":\"");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SMOKEJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\",\"store_read\",\"wal_metadata_read_when_enabled\"]");
    assert_contains(result.response, "\"warnings\":[\"not a write command\"]");

    return 0;
}
