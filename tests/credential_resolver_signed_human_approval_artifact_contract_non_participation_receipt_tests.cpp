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

void assert_source_node_v314_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v314_reference\":");
    assert_contains(text, "\"source_version\":\"Node v314\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-signed-human-approval-artifact-contract-intake.v1\"");
    assert_contains(text, "\"contract_state\":\"signed-human-approval-artifact-contract-intake-ready\"");
    assert_contains(
        text,
        "\"governance_chain_decision\":\"continue-only-for-signed-human-approval-artifact-contract-intake\"");
    assert_contains(text, "\"target_prerequisite_id\":\"signed-human-approval-artifact\"");
    assert_contains(text, "\"next_java_version\":\"Java v145\"");
    assert_contains(text, "\"next_mini_kv_version\":\"mini-kv v138\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v315\"");
    assert_contains(text, "\"ready_for_parallel_java_v145_mini_kv_v138_echo\":true");
    assert_contains(text, "\"ready_for_node_v315_before_upstream_echo\":false");
    assert_contains(text, "\"contract_digest\":\"72498e59c086eadd4d44e80789120de195af1a0b70dd49346b837e2bc8ed4666\"");
    assert_contains(text, "\"artifact_name\":\"managed-audit-signed-human-approval-artifact\"");
    assert_contains(text, "\"artifact_version\":\"signed-human-approval-artifact.v1\"");
    assert_contains(text, "\"contract_mode\":\"signed-human-approval-artifact-contract-intake-only\"");
    assert_contains(text, "\"source_span\":\"Node v312 + Node v313 catalog\"");
    assert_contains(text, "\"required_field_count\":11");
    assert_contains(text, "\"prohibited_field_count\":8");
    assert_contains(text, "\"rejection_reason_count\":5");
    assert_contains(text, "\"no_go_boundary_count\":8");
    assert_contains(text, "\"upstream_echo_request_count\":2");
}

void assert_contract_fields(const std::string& text) {
    assert_contains(text, "\"artifact_id\"");
    assert_contains(text, "\"approval_correlation_id\"");
    assert_contains(text, "\"operator_identity_handle\"");
    assert_contains(text, "\"signer_identity_handle\"");
    assert_contains(text, "\"policy_version\"");
    assert_contains(text, "\"artifact_digest\"");
    assert_contains(text, "\"issued_at\"");
    assert_contains(text, "\"expires_at\"");
    assert_contains(text, "\"review_status\"");
    assert_contains(text, "\"no_network_assertion\"");
    assert_contains(text, "\"rollback_abort_reference\"");
    assert_contains(text, "\"credential_value\"");
    assert_contains(text, "\"raw_endpoint_url\"");
    assert_contains(text, "\"signing_private_key\"");
    assert_contains(text, "\"secret_provider_config\"");
    assert_contains(text, "\"resolver_client_config\"");
    assert_contains(text, "\"external_request_payload\"");
    assert_contains(text, "\"approval_ledger_mutation\"");
    assert_contains(text, "\"schema_migration_sql\"");
    assert_contains(text, "\"SIGNED_ARTIFACT_MISSING\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_PRESENT\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_PRESENT\"");
    assert_contains(text, "\"RUNTIME_IMPLEMENTATION_PRESENT\"");
    assert_contains(text, "\"WRITE_OR_MIGRATION_PRESENT\"");
    assert_contains(text, "\"runtime_shell_implementation\"");
    assert_contains(text, "\"runtime_shell_invocation\"");
    assert_contains(text, "\"provider_client_instantiation\"");
    assert_contains(text, "\"ledger_or_schema_write\"");
}

void assert_source_node_v312_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v312_reference\":");
    assert_contains(text, "\"source_version\":\"Node v312\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-governance-stop-prerequisite-closure-decision.v1\"");
    assert_contains(text, "\"decision_state\":\"human-approval-artifact-review-governance-stop-prerequisite-closure-decision-ready\"");
    assert_contains(text, "\"decision_digest\":\"152d7517c07119df360446a29c508f5d3d9a78a28adfc6137ea0b0254508b0c6\"");
    assert_contains(text, "\"source_verification_digest\":\"8292327cdb44e1d37ead67ff5a0444c08625860c62a3648846801a84f5a6f194\"");
    assert_contains(text, "\"completed_prerequisite_count\":1");
    assert_contains(text, "\"remaining_prerequisite_count\":5");
    assert_contains(text, "\"original_prerequisite_count\":6");
    assert_contains(text, "\"no_go_condition_count\":9");
    assert_contains(text, "\"chain_continuation_allowed\":false");
    assert_contains(text, "\"next_concrete_prerequisite_contract_required\":true");
    assert_contains(text, "\"java-mini-kv-decision-echo\"");
    assert_contains(text, "\"credential-handle-approval\"");
    assert_contains(text, "\"endpoint-handle-allowlist-approval\"");
    assert_contains(text, "\"no-network-safety-fixture\"");
    assert_contains(text, "\"abort-rollback-semantics\"");
    assert_contains(text, "\"source_check_count\":16");
    assert_contains(text, "\"source_node_v311_check_count\":23");
}

void assert_transition_summary_and_boundaries(const std::string& text) {
    assert_contains(text, "\"prerequisite_id\":\"signed-human-approval-artifact\"");
    assert_contains(text, "\"before_v314\":\"still-missing\"");
    assert_contains(text, "\"after_v314\":\"contract-intake-defined\"");
    assert_contains(text, "\"closure_requires_upstream_echo\":true");
    assert_contains(text, "\"closes_credential_handle_approval\":false");
    assert_contains(text, "\"closes_endpoint_handle_allowlist_approval\":false");
    assert_contains(text, "\"closes_no_network_safety_fixture\":false");
    assert_contains(text, "\"closes_abort_rollback_semantics\":false");
    assert_contains(text, "\"check_count\":18");
    assert_contains(text, "\"passed_check_count\":18");
    assert_contains(text, "\"source_node_v312_check_count\":16");
    assert_contains(text, "\"source_node_v312_passed_check_count\":16");
    assert_contains(text, "\"source_remaining_prerequisite_count\":5");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"SIGNED_ARTIFACT_CONTRACT_DOES_NOT_CLOSE_ALL_PREREQUISITES\"");
    assert_contains(text, "\"RUN_JAVA_V145_AND_MINI_KV_V138_AFTER_V314_ARCHIVE\"");
    assert_contains(text, "\"KEEP_SIGNED_ARTIFACT_CONTRACT_NON_SECRET\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v145\",\"mini-kv v138\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"signed_human_approval_artifact_contract_non_participation_receipt_only\":true");
    assert_contains(text, "\"signed_human_approval_artifact_stored\":false");
    assert_contains(text, "\"signed_human_approval_artifact_validated\":false");
    assert_contains(text, "\"signed_human_approval_artifact_authoritative\":false");
    assert_contains(text, "\"signed_artifact_signature_verified\":false");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"credential_value_accepted\":false");
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
        "credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v138\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v138\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/138/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v314_shape(receipt);
    assert_contract_fields(receipt);
    assert_source_node_v312_shape(receipt);
    assert_transition_summary_and_boundaries(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");
    assert_not_contains(receipt, "signing_private_key\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v314_shape(smoke);
    assert_contract_fields(smoke);
    assert_source_node_v312_shape(smoke);
    assert_transition_summary_and_boundaries(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_signed_human_approval_artifact_contract_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt for Node v315");
    assert_source_node_v314_shape(manifest);
    assert_contract_fields(manifest);
    assert_source_node_v312_shape(manifest);
    assert_transition_summary_and_boundaries(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt\":");
    assert_contains(result.response, "\"current_release_version\":\"v138\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/138/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v314_shape(result.response);
    assert_contract_fields(result.response);
    assert_source_node_v312_shape(result.response);
    assert_transition_summary_and_boundaries(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v315 may verify the mini-kv v138 credential resolver signed human approval artifact contract non-participation receipt");
    assert_contains(
        result.response,
        "Node v311 may verify the mini-kv v137 credential resolver human approval artifact review post-echo decision gate non-participation receipt");
    assert_not_contains(result.response, "credential_value\":\"");
    assert_not_contains(result.response, "raw_endpoint_url\":\"");
    assert_not_contains(result.response, "signing_private_key\":\"");

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
