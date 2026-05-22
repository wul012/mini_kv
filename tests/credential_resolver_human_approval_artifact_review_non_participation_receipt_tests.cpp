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

void assert_source_node_v308_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v308_reference\":");
    assert_contains(text, "\"source_version\":\"Node v308\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-packet.v1\"");
    assert_contains(text, "\"review_packet_state\":\"human-approval-artifact-review-packet-ready\"");
    assert_contains(
        text,
        "\"runtime_shell_chain_decision\":\"require-explicit-approval-prerequisites-before-runtime-shell\"");
    assert_contains(text, "\"ready_for_human_approval_artifact_review_packet\":true");
    assert_contains(text, "\"human_approval_artifact_review_packet_only\":true");
    assert_contains(text, "\"read_only_review_packet_contract\":true");
    assert_contains(text, "\"next_java_version\":\"Java v143\"");
    assert_contains(text, "\"next_mini_kv_version\":\"mini-kv v136\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v309\"");
    assert_contains(text, "\"ready_for_parallel_java_v143_mini_kv_v136_echo\":true");
    assert_contains(text, "\"ready_for_node_v309_before_upstream_echo\":false");
    assert_contains(text, "\"packet_name\":\"managed-audit-runtime-shell-human-approval-artifact-review-packet\"");
    assert_contains(text, "\"packet_version\":\"human-approval-artifact-review-packet.v1\"");
    assert_contains(text, "\"review_mode\":\"human-approval-artifact-review-packet-contract-only\"");
    assert_contains(text, "\"packet_digest\":\"b0dda954c509337c96a645c177be521d0a200d8f8e6d52081ed8c0df9a43ccf3\"");
    assert_contains(text, "\"required_field_count\":9");
    assert_contains(text, "\"prohibited_field_count\":9");
    assert_contains(text, "\"rejection_reason_count\":13");
    assert_contains(text, "\"missing_field_check_count\":9");
    assert_contains(text, "\"no_go_boundary_count\":12");
    assert_contains(text, "\"upstream_echo_request_count\":2");
    assert_contains(text, "\"java_mini_kv_echo_can_run_in_parallel\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
    assert_contains(text, "\"consumer\":\"Java v143 + mini-kv v136, then Node v309\"");
    assert_contains(text, "\"proof_complete\":true");
}

void assert_review_packet_contract_fields(const std::string& text) {
    assert_contains(text, "\"artifact_id\"");
    assert_contains(text, "\"operator_approval_reference\"");
    assert_contains(text, "\"credential_handle_review_status\"");
    assert_contains(text, "\"endpoint_handle_allowlist_review_status\"");
    assert_contains(text, "\"no_network_safety_test_reference\"");
    assert_contains(text, "\"manual_abort_semantics_reference\"");
    assert_contains(text, "\"rollback_semantics_reference\"");
    assert_contains(text, "\"created_by_operator_identity\"");
    assert_contains(text, "\"audit_correlation_id\"");
    assert_contains(text, "\"credential_value\"");
    assert_contains(text, "\"raw_endpoint_url\"");
    assert_contains(text, "\"secret_provider_config\"");
    assert_contains(text, "\"resolver_client_config\"");
    assert_contains(text, "\"external_request_payload\"");
    assert_contains(text, "\"approval_ledger_mutation\"");
    assert_contains(text, "\"schema_migration_sql\"");
    assert_contains(text, "\"mini_kv_write_command\"");
    assert_contains(text, "\"runtime_shell_invocation_request\"");
    assert_contains(text, "\"MISSING_ARTIFACT_ID\"");
    assert_contains(text, "\"MISSING_OPERATOR_APPROVAL_REFERENCE\"");
    assert_contains(text, "\"MISSING_CREDENTIAL_HANDLE_REVIEW_STATUS\"");
    assert_contains(text, "\"MISSING_ENDPOINT_HANDLE_ALLOWLIST_REVIEW_STATUS\"");
    assert_contains(text, "\"MISSING_NO_NETWORK_SAFETY_TEST_REFERENCE\"");
    assert_contains(text, "\"MISSING_ABORT_OR_ROLLBACK_SEMANTICS\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_PRESENT\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_PRESENT\"");
    assert_contains(text, "\"PROVIDER_OR_CLIENT_CONFIG_PRESENT\"");
    assert_contains(text, "\"EXTERNAL_REQUEST_REQUESTED\"");
    assert_contains(text, "\"WRITE_OR_SCHEMA_MUTATION_REQUESTED\"");
    assert_contains(text, "\"MINI_KV_WRITE_OR_AUTHORITY_REQUESTED\"");
    assert_contains(text, "\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\"");
}

void assert_source_node_v307_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v307_reference\":");
    assert_contains(text, "\"source_version\":\"Node v307\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-approval-prerequisite-artifact-upstream-echo-verification.v1\"");
    assert_contains(
        text,
        "\"verification_state\":\"approval-prerequisite-artifact-upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_upstream_echo_verification\":true");
    assert_contains(text, "\"verification_digest\":\"6c9e5563358402eb2a8d0ac9dc8235db83aab4d0f5634b53c7ff4da9a6b3c362\"");
    assert_contains(text, "\"source_span\":\"Node v306 + Java v142 + mini-kv v135\"");
    assert_contains(text, "\"upstream_echo_aligned\":true");
    assert_contains(text, "\"artifact_contract_aligned\":true");
    assert_contains(text, "\"side_effect_boundaries_aligned\":true");
    assert_contains(text, "\"source_node_v306_artifact_digest\":\"72f3e90606e40a978611fa4b8596c76c3ebc468124c4ead7bb9c4833130ee9c2\"");
    assert_contains(text, "\"source_node_v306_plan_state\":\"approval-prerequisite-artifact-intake-plan-ready\"");
    assert_contains(text, "\"source_node_v307_check_count\":23");
    assert_contains(text, "\"source_node_v307_passed_check_count\":23");
    assert_contains(text, "\"source_node_v306_check_count\":16");
    assert_contains(text, "\"source_node_v306_passed_check_count\":16");
}

void assert_summary_and_boundaries(const std::string& text) {
    assert_contains(text, "\"check_count\":18");
    assert_contains(text, "\"passed_check_count\":18");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"HUMAN_APPROVAL_REVIEW_PACKET_DOES_NOT_AUTHORIZE_RUNTIME\"");
    assert_contains(text, "\"RUN_JAVA_V143_AND_MINI_KV_V136_IN_PARALLEL_AFTER_V308\"");
    assert_contains(text, "\"VERIFY_HUMAN_APPROVAL_ARTIFACT_REVIEW_ECHO_WITH_NODE_V309\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v143\",\"mini-kv v136\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"human_approval_artifact_review_non_participation_receipt_only\":true");
    assert_contains(text, "\"human_approval_artifact_review_packet_only\":true");
    assert_contains(text, "\"read_only_review_packet_contract\":true");
    assert_contains(text, "\"consumes_node_v308_human_approval_artifact_review_packet\":true");
    assert_contains(text, "\"ready_for_node_v309_human_approval_artifact_review_upstream_echo_verification\":true");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"credential_value_accepted\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"raw_endpoint_url_accepted\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"external_request_payload_accepted\":false");
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
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "credential-resolver-human-approval-artifact-review-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_human_approval_artifact_review_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-human-approval-artifact-review-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v136\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-human-approval-artifact-review-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_human_approval_artifact_review_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v136\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/136/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v308_shape(receipt);
    assert_review_packet_contract_fields(receipt);
    assert_source_node_v307_shape(receipt);
    assert_summary_and_boundaries(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_human_approval_artifact_review_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v308_shape(smoke);
    assert_review_packet_contract_fields(smoke);
    assert_source_node_v307_shape(smoke);
    assert_summary_and_boundaries(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_human_approval_artifact_review_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-human-approval-artifact-review-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_human_approval_artifact_review_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_human_approval_artifact_review_non_participation_receipt for Node v309");
    assert_source_node_v308_shape(manifest);
    assert_review_packet_contract_fields(manifest);
    assert_source_node_v307_shape(manifest);
    assert_summary_and_boundaries(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_human_approval_artifact_review_non_participation_receipt\":");
    assert_contains(result.response, "\"current_release_version\":\"v136\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/136/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v308_shape(result.response);
    assert_review_packet_contract_fields(result.response);
    assert_source_node_v307_shape(result.response);
    assert_summary_and_boundaries(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v311 may verify the mini-kv v137 credential resolver human approval artifact review post-echo decision gate non-participation receipt");
    assert_contains(
        result.response,
        "Node v309 may verify the mini-kv v136 credential resolver human approval artifact review non-participation receipt");
    assert_contains(
        result.response,
        "Node v307 may verify the mini-kv v135 credential resolver approval prerequisite artifact intake non-participation receipt");
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
