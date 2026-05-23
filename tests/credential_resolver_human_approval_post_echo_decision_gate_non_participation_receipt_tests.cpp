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

void assert_source_node_v310_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v310_reference\":");
    assert_contains(text, "\"source_version\":\"Node v310\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-post-echo-decision-gate.v1\"");
    assert_contains(text, "\"decision_gate_state\":\"human-approval-artifact-review-post-echo-decision-gate-ready\"");
    assert_contains(
        text,
        "\"runtime_shell_chain_decision\":\"require-explicit-approval-prerequisites-before-runtime-shell\"");
    assert_contains(text, "\"ready_for_post_echo_decision_gate\":true");
    assert_contains(text, "\"read_only_decision_gate\":true");
    assert_contains(text, "\"human_approval_artifact_review_post_echo_decision_gate_only\":true");
    assert_contains(text, "\"ready_for_parallel_java_v144_mini_kv_v137_echo_request\":true");
    assert_contains(text, "\"ready_for_node_v311_before_upstream_echo\":false");
    assert_contains(text, "\"decision_digest\":\"91c0927b63fee6dd0fb44e27cd82c7f700ceeeb4cef750606777bd4d3436102c\"");
    assert_contains(text, "\"gate_mode\":\"human-approval-artifact-review-post-echo-decision-gate-only\"");
    assert_contains(text, "\"decision\":\"continue-only-as-blocked-post-echo-prerequisite-review\"");
    assert_contains(text, "\"selected_path\":\"request-read-only-upstream-decision-echo-before-any-runtime-shell\"");
    assert_contains(text, "\"source_span\":\"Node v308 + Java v143 + mini-kv v136 + Node v309\"");
    assert_contains(text, "\"allows_parallel_java_v144_mini_kv_v137_echo_request\":true");
    assert_contains(text, "\"allows_node_v311_before_upstream_echo\":false");
    assert_contains(text, "\"allows_disabled_runtime_shell_implementation\":false");
    assert_contains(text, "\"allows_disabled_runtime_shell_invocation\":false");
    assert_contains(text, "\"allows_credential_value_read\":false");
    assert_contains(text, "\"allows_raw_endpoint_url_parse\":false");
    assert_contains(text, "\"allows_managed_audit_connection\":false");
    assert_contains(text, "\"prerequisite_count\":6");
    assert_contains(text, "\"missing_prerequisite_count\":6");
    assert_contains(text, "\"no_go_condition_count\":9");
}

void assert_prerequisites_and_no_go(const std::string& text) {
    assert_contains(text, "\"signed-human-approval-artifact\"");
    assert_contains(text, "\"credential-handle-approval\"");
    assert_contains(text, "\"endpoint-handle-allowlist-approval\"");
    assert_contains(text, "\"no-network-safety-fixture\"");
    assert_contains(text, "\"abort-rollback-semantics\"");
    assert_contains(text, "\"java-mini-kv-decision-echo\"");
    assert_contains(text, "\"status\":\"documented-missing\"");
    assert_contains(text, "\"required_before_runtime_shell\":true");
    assert_contains(text, "\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\"");
    assert_contains(text, "\"RUNTIME_SHELL_INVOCATION_REQUESTED\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_READ_REQUESTED\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_PARSE_REQUESTED\"");
    assert_contains(text, "\"PROVIDER_CLIENT_INSTANTIATION_REQUESTED\"");
    assert_contains(text, "\"EXTERNAL_REQUEST_REQUESTED\"");
    assert_contains(text, "\"LEDGER_OR_SCHEMA_WRITE_REQUESTED\"");
    assert_contains(text, "\"MINIKV_WRITE_OR_AUTHORITY_REQUESTED\"");
    assert_contains(text, "\"AUTOMATIC_UPSTREAM_START_REQUESTED\"");
}

void assert_source_node_v309_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v309_reference\":");
    assert_contains(text, "\"source_version\":\"Node v309\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-upstream-echo-verification.v1\"");
    assert_contains(text, "\"verification_state\":\"human-approval-artifact-review-upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_human_approval_artifact_review_upstream_echo_verification\":true");
    assert_contains(text, "\"read_only_upstream_echo_verification\":true");
    assert_contains(text, "\"verification_digest\":\"b306972aeab587fff6905759ad0fa2968af235cb13f09063c1c6c3af14e7913d\"");
    assert_contains(text, "\"source_span\":\"Node v308 + Java v143 + mini-kv v136\"");
    assert_contains(text, "\"source_node_v308_review_packet_digest\":\"b0dda954c509337c96a645c177be521d0a200d8f8e6d52081ed8c0df9a43ccf3\"");
    assert_contains(text, "\"source_node_v308_required_field_count\":9");
    assert_contains(text, "\"source_node_v308_prohibited_field_count\":9");
    assert_contains(text, "\"source_node_v308_rejection_reason_count\":13");
    assert_contains(text, "\"source_node_v308_missing_field_check_count\":9");
    assert_contains(text, "\"source_node_v308_no_go_boundary_count\":12");
    assert_contains(text, "\"source_node_v308_upstream_echo_request_count\":2");
    assert_contains(text, "\"source_check_count\":23");
    assert_contains(text, "\"source_passed_check_count\":23");
    assert_contains(text, "\"source_node_v308_check_count\":18");
    assert_contains(text, "\"source_node_v308_passed_check_count\":18");
    assert_contains(text, "\"java_v143_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v136_receipt_ready\":true");
    assert_contains(text, "\"upstream_echo_aligned\":true");
    assert_contains(text, "\"review_packet_contract_aligned\":true");
    assert_contains(text, "\"side_effect_boundaries_aligned\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
}

void assert_summary_and_boundaries(const std::string& text) {
    assert_contains(text, "\"check_count\":17");
    assert_contains(text, "\"passed_check_count\":17");
    assert_contains(text, "\"source_check_count\":23");
    assert_contains(text, "\"source_passed_check_count\":23");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"POST_ECHO_DECISION_DOES_NOT_AUTHORIZE_RUNTIME\"");
    assert_contains(text, "\"RUN_JAVA_V144_AND_MINI_KV_V137_IN_PARALLEL\"");
    assert_contains(text, "\"KEEP_RUNTIME_SHELL_BLOCKED\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v144\",\"mini-kv v137\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_only\":true");
    assert_contains(text, "\"read_only_decision_gate\":true");
    assert_contains(text, "\"consumes_node_v310_human_approval_artifact_review_post_echo_decision_gate\":true");
    assert_contains(text, "\"ready_for_node_v311_human_approval_artifact_review_post_echo_decision_upstream_echo_verification\":true");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"credential_value_accepted\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"raw_endpoint_url_accepted\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
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
        "credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v137\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.json\"");
    assert_contains(
        receipt,
        "\"credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v137\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/137/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v310_shape(receipt);
    assert_prerequisites_and_no_go(receipt);
    assert_source_node_v309_shape(receipt);
    assert_summary_and_boundaries(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(
        smoke,
        "\"credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v310_shape(smoke);
    assert_prerequisites_and_no_go(smoke);
    assert_source_node_v309_shape(smoke);
    assert_summary_and_boundaries(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_human_approval_post_echo_decision_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.json\"");
    assert_contains(
        manifest,
        "\"credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt for Node v311");
    assert_source_node_v310_shape(manifest);
    assert_prerequisites_and_no_go(manifest);
    assert_source_node_v309_shape(manifest);
    assert_summary_and_boundaries(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(
        result.response,
        "\"credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt\":");
    assert_contains(result.response, "\"current_release_version\":\"v137\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/137/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v310_shape(result.response);
    assert_prerequisites_and_no_go(result.response);
    assert_source_node_v309_shape(result.response);
    assert_summary_and_boundaries(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v315 may verify the mini-kv v138 credential resolver signed human approval artifact contract non-participation receipt");
    assert_contains(
        result.response,
        "Node v311 may verify the mini-kv v137 credential resolver human approval artifact review post-echo decision gate non-participation receipt");
    assert_contains(
        result.response,
        "Node v309 may verify the mini-kv v136 credential resolver human approval artifact review non-participation receipt");
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
