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

void assert_source_node_v326_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v326_reference\":");
    assert_contains(text, "\"source_version\":\"Node v326\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-abort-rollback-semantics-contract-intake.v1\"");
    assert_contains(text, "\"contract_state\":\"abort-rollback-semantics-contract-intake-ready\"");
    assert_contains(
        text,
        "\"governance_chain_decision\":\"continue-only-for-abort-rollback-semantics-contract-intake\"");
    assert_contains(text, "\"target_prerequisite_id\":\"abort-rollback-semantics\"");
    assert_contains(text, "\"next_java_version\":\"Java v150\"");
    assert_contains(text, "\"next_mini_kv_version\":\"mini-kv v142\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v327\"");
    assert_contains(text, "\"ready_for_parallel_java_v150_mini_kv_v142_echo\":true");
    assert_contains(text, "\"ready_for_node_v327_before_upstream_echo\":false");
    assert_contains(text, "\"ready_for_final_prerequisite_closure_review\":false");
    assert_contains(
        text,
        "\"contract_digest\":\"fe05bcfd65aabf56ef170bf458837053a11edf0ae44ad203a88d4ecd284299f9\"");
    assert_contains(text, "\"contract_name\":\"managed-audit-abort-rollback-semantics\"");
    assert_contains(text, "\"contract_version\":\"abort-rollback-semantics.v1\"");
    assert_contains(text, "\"contract_mode\":\"abort-rollback-semantics-contract-intake-only\"");
    assert_contains(text, "\"source_span\":\"Node v325 closure review + Node v313 catalog\"");
    assert_contains(text, "\"required_field_count\":10");
    assert_contains(text, "\"prohibited_field_count\":14");
    assert_contains(text, "\"rejection_reason_count\":6");
    assert_contains(text, "\"no_go_boundary_count\":11");
    assert_contains(text, "\"upstream_echo_request_count\":2");
    assert_contains(text, "\"abort_rollback_execution_allowed\":false");
}

void assert_contract_fields(const std::string& text) {
    assert_contains(text, "\"manual_abort_marker\"");
    assert_contains(text, "\"rollback_runbook_reference\"");
    assert_contains(text, "\"operator_confirmation_handle\"");
    assert_contains(text, "\"approval_correlation_id\"");
    assert_contains(text, "\"cleanup_evidence_marker\"");
    assert_contains(text, "\"idempotent_noop_failure_policy\"");
    assert_contains(text, "\"rollback_authority_boundary\"");
    assert_contains(text, "\"abort_reason_code\"");
    assert_contains(text, "\"recovery_checkpoint_reference\"");
    assert_contains(text, "\"audit_digest\"");
    assert_contains(text, "\"credential_value\"");
    assert_contains(text, "\"raw_endpoint_url\"");
    assert_contains(text, "\"runtime_shell_command\"");
    assert_contains(text, "\"shell_script_body\"");
    assert_contains(text, "\"secret_provider_config\"");
    assert_contains(text, "\"resolver_client_config\"");
    assert_contains(text, "\"deployment_action\"");
    assert_contains(text, "\"rollback_execution_action\"");
    assert_contains(text, "\"mini_kv_write_command\"");
    assert_contains(text, "\"java_sql_execution\"");
    assert_contains(text, "\"MANUAL_ABORT_MARKER_MISSING\"");
    assert_contains(text, "\"ROLLBACK_RUNBOOK_REFERENCE_MISSING\"");
    assert_contains(text, "\"RUNTIME_SHELL_COMMAND_PRESENT\"");
    assert_contains(text, "\"NETWORK_OR_PROVIDER_ACTION_PRESENT\"");
    assert_contains(text, "\"WRITE_OR_ROLLBACK_ACTION_PRESENT\"");
    assert_contains(text, "\"runtime_shell_command_render\"");
    assert_contains(text, "\"http_or_tcp_execution\"");
    assert_contains(text, "\"rollback_execution\"");
}

void assert_source_node_v325_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v325_reference\":");
    assert_contains(text, "\"source_version\":\"Node v325\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-no-network-safety-fixture-prerequisite-closure-review.v1\"");
    assert_contains(text, "\"review_state\":\"no-network-safety-fixture-prerequisite-closure-review-ready\"");
    assert_contains(text, "\"review_digest\":\"5781245b6dd5b67d6e2985e7e6f70e942defcd4ea95a09dc516743abf7abf0ca\"");
    assert_contains(text, "\"prerequisite_closure_decision\":\"advance-no-network-safety-fixture-only\"");
    assert_contains(text, "\"completed_prerequisite_count\":5");
    assert_contains(text, "\"remaining_prerequisite_count\":1");
    assert_contains(text, "\"original_prerequisite_count\":6");
    assert_contains(text, "\"next_concrete_prerequisite_id\":\"abort-rollback-semantics\"");
    assert_contains(text, "\"next_node_version_suggested\":\"Node v326\"");
    assert_contains(text, "\"chain_continuation_allowed\":true");
    assert_contains(text, "\"runtime_shell_still_blocked\":true");
    assert_contains(text, "\"java-mini-kv-decision-echo\"");
    assert_contains(text, "\"signed-human-approval-artifact\"");
    assert_contains(text, "\"credential-handle-approval\"");
    assert_contains(text, "\"endpoint-handle-allowlist-approval\"");
    assert_contains(text, "\"no-network-safety-fixture\"");
    assert_contains(text, "\"abort-rollback-semantics\"");
    assert_contains(text, "\"source_check_count\":17");
    assert_contains(text, "\"source_node_v324_check_count\":23");
}

void assert_transition_summary_and_boundaries(const std::string& text) {
    assert_contains(text, "\"prerequisite_id\":\"abort-rollback-semantics\"");
    assert_contains(text, "\"before_v326\":\"still-missing\"");
    assert_contains(text, "\"after_v326\":\"contract-intake-defined\"");
    assert_contains(text, "\"closure_requires_upstream_echo\":true");
    assert_contains(text, "\"completed_prerequisite_count_before_v326\":5");
    assert_contains(text, "\"remaining_prerequisite_count_before_v326\":1");
    assert_contains(text, "\"preserves_no_network_safety_fixture_closure\":true");
    assert_contains(text, "\"closes_abort_rollback_semantics\":false");
    assert_contains(text, "\"check_count\":21");
    assert_contains(text, "\"passed_check_count\":21");
    assert_contains(text, "\"source_node_v325_check_count\":17");
    assert_contains(text, "\"source_node_v325_passed_check_count\":17");
    assert_contains(text, "\"source_remaining_prerequisite_count\":1");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"ABORT_ROLLBACK_SEMANTICS_CONTRACT_DOES_NOT_EXECUTE_ROLLBACK\"");
    assert_contains(text, "\"FINAL_PREREQUISITE_CONTRACT_DOES_NOT_APPROVE_RUNTIME\"");
    assert_contains(text, "\"RUN_JAVA_V150_AND_MINI_KV_V142_AFTER_V326_ARCHIVE\"");
    assert_contains(text, "\"KEEP_ABORT_ROLLBACK_SEMANTICS_CONTRACT_NON_EXECUTING\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v150\",\"mini-kv v142\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"abort_rollback_semantics_contract_non_participation_receipt_only\":true");
    assert_contains(text, "\"abort_rollback_semantics_executed\":false");
    assert_contains(text, "\"abort_rollback_execution_allowed\":false");
    assert_contains(text, "\"abort_rollback_authority\":false");
    assert_contains(text, "\"rollback_execution_allowed\":false");
    assert_contains(text, "\"rollback_executed\":false");
    assert_contains(text, "\"deployment_action_allowed\":false");
    assert_contains(text, "\"java_sql_execution_allowed\":false");
    assert_contains(text, "\"mini_kv_write_command_allowed\":false");
    assert_contains(text, "\"runtime_shell_command_rendered\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"http_request_sent\":false");
    assert_contains(text, "\"tcp_connection_attempted\":false");
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
        "credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v142\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_abort_rollback_semantics_contract_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v142\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/142/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v326_shape(receipt);
    assert_contract_fields(receipt);
    assert_source_node_v325_shape(receipt);
    assert_transition_summary_and_boundaries(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");
    assert_not_contains(receipt, "runtime_shell_command\":\"");
    assert_not_contains(receipt, "shell_script_body\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_abort_rollback_semantics_contract_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v326_shape(smoke);
    assert_contract_fields(smoke);
    assert_source_node_v325_shape(smoke);
    assert_transition_summary_and_boundaries(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_abort_rollback_semantics_contract_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_abort_rollback_semantics_contract_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_abort_rollback_semantics_contract_non_participation_receipt for Node v327");
    assert_source_node_v326_shape(manifest);
    assert_contract_fields(manifest);
    assert_source_node_v325_shape(manifest);
    assert_transition_summary_and_boundaries(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_abort_rollback_semantics_contract_non_participation_receipt\":");
    assert_contains(result.response, "\"current_release_version\":\"v142\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/142/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v326_shape(result.response);
    assert_contract_fields(result.response);
    assert_source_node_v325_shape(result.response);
    assert_transition_summary_and_boundaries(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v330 may verify the mini-kv v143 credential resolver implementation candidate gate input-hardening non-participation receipt");
    assert_contains(
        result.response,
        "Node v327 may verify the mini-kv v142 credential resolver abort/rollback semantics contract non-participation receipt");
    assert_contains(
        result.response,
        "Node v324 may verify the mini-kv v141 credential resolver no-network safety fixture contract non-participation receipt");
    assert_not_contains(result.response, "credential_value\":\"");
    assert_not_contains(result.response, "raw_endpoint_url\":\"");
    assert_not_contains(result.response, "runtime_shell_command\":\"");

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
