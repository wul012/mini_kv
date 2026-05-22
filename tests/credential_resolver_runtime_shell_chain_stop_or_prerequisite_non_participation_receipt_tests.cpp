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

void assert_source_node_v304_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v304_reference\":");
    assert_contains(text, "\"source_version\":\"Node v304\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-chain-stop-or-prerequisite-decision-record.v1\"");
    assert_contains(
        text,
        "\"decision_record_state\":\"runtime-shell-chain-stop-or-prerequisite-decision-record-ready\"");
    assert_contains(
        text,
        "\"runtime_shell_chain_decision\":\"require-explicit-approval-prerequisites-before-runtime-shell\"");
    assert_contains(
        text,
        "\"decision_digest\":\"9212d0b804fdc1eda9098ac70d2441681730a98ff736776859811df9e288a654\"");
    assert_contains(text, "\"record_mode\":\"runtime-shell-chain-stop-or-prerequisite-decision-record-only\"");
    assert_contains(text, "\"selected_path\":\"continue-only-as-blocked-prerequisite-review\"");
    assert_contains(text, "\"stop_runtime_shell_chain_without_prerequisites\":true");
    assert_contains(text, "\"allows_parallel_java_v141_mini_kv_v134_echo_request\":true");
    assert_contains(text, "\"ready_for_node_v305_stop_prerequisite_upstream_echo_verification\":false");
    assert_contains(text, "\"allows_node_v305_before_upstream_echo\":false");
    assert_contains(text, "\"allows_disabled_runtime_shell_implementation\":false");
    assert_contains(text, "\"allows_disabled_runtime_shell_invocation\":false");
    assert_contains(text, "\"allows_real_resolver_implementation\":false");
    assert_contains(text, "\"allows_secret_provider_instantiation\":false");
    assert_contains(text, "\"allows_resolver_client_instantiation\":false");
    assert_contains(text, "\"allows_credential_value_read\":false");
    assert_contains(text, "\"allows_raw_endpoint_url_parse\":false");
    assert_contains(text, "\"allows_external_request\":false");
    assert_contains(text, "\"allows_managed_audit_connection\":false");
    assert_contains(text, "\"allows_schema_migration\":false");
    assert_contains(text, "\"allows_approval_ledger_write\":false");
    assert_contains(text, "\"allows_automatic_upstream_start\":false");
    assert_contains(text, "\"prerequisite_count\":6");
    assert_contains(text, "\"missing_runtime_prerequisite_count\":6");
    assert_contains(text, "\"no_go_condition_count\":8");
    assert_contains(text, "\"operator-approval-artifact\"");
    assert_contains(text, "\"credential-handle-readiness\"");
    assert_contains(text, "\"raw-endpoint-allowlist-review\"");
    assert_contains(text, "\"no-network-test-fixture\"");
    assert_contains(text, "\"manual-abort-and-rollback-semantics\"");
    assert_contains(text, "\"java-mini-kv-prerequisite-echo\"");
    assert_contains(text, "\"status\":\"documented-missing\"");
    assert_contains(text, "\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\"");
    assert_contains(text, "\"RUNTIME_SHELL_INVOCATION_REQUESTED\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_READ_REQUESTED\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_PARSE_REQUESTED\"");
    assert_contains(text, "\"PROVIDER_CLIENT_INSTANTIATION_REQUESTED\"");
    assert_contains(text, "\"EXTERNAL_REQUEST_REQUESTED\"");
    assert_contains(text, "\"LEDGER_OR_SCHEMA_WRITE_REQUESTED\"");
    assert_contains(text, "\"MINIKV_WRITE_OR_AUTHORITY_REQUESTED\"");
    assert_contains(text, "\"proof_complete\":true");
}

void assert_source_node_v303_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v303_reference\":");
    assert_contains(text, "\"source_version\":\"Node v303\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-plan-intake-upstream-echo-verification.v1\"");
    assert_contains(
        text,
        "\"verification_state\":\"runtime-shell-post-decision-plan-intake-upstream-echo-verification-ready\"");
    assert_contains(
        text,
        "\"verification_digest\":\"9a2f807d01e34c632ee02ac40db98f5a217a7f186d6305c6b97309a6a7d257df\"");
    assert_contains(text, "\"source_span\":\"Node v301 + Node v302 + Java v136 + mini-kv v133\"");
    assert_contains(text, "\"active_node_verification_version\":\"Node v303\"");
    assert_contains(text, "\"legacy_node_v302_consumer_marker_accepted\":true");
    assert_contains(text, "\"java_v136_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v133_receipt_ready\":true");
    assert_contains(text, "\"upstream_echo_aligned\":true");
    assert_contains(text, "\"side_effect_boundaries_aligned\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
    assert_contains(text, "\"check_count\":23");
    assert_contains(text, "\"passed_check_count\":23");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
}

void assert_summary_and_flags(const std::string& text) {
    assert_contains(text, "\"check_count\":17");
    assert_contains(text, "\"passed_check_count\":17");
    assert_contains(text, "\"source_node_v303_check_count\":23");
    assert_contains(text, "\"source_node_v303_passed_check_count\":23");
    assert_contains(text, "\"source_node_v303_warning_count\":2");
    assert_contains(text, "\"source_node_v303_recommendation_count\":2");
    assert_contains(text, "\"prerequisite_count\":6");
    assert_contains(text, "\"missing_runtime_prerequisite_count\":6");
    assert_contains(text, "\"no_go_condition_count\":8");
    assert_contains(text, "\"PREREQUISITE_DECISION_DOES_NOT_AUTHORIZE_RUNTIME\"");
    assert_contains(text, "\"RUN_JAVA_V141_AND_MINIKV_V134_IN_PARALLEL\"");
    assert_contains(text, "\"KEEP_RUNTIME_SHELL_BLOCKED\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v141\",\"mini-kv v134\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_only\":true");
    assert_contains(text, "\"runtime_shell_chain_stop_or_prerequisite_decision_record_only\":true");
    assert_contains(text, "\"stop_or_prerequisite_decision_record_only\":true");
    assert_contains(
        text,
        "\"runtime_shell_chain_decision\":\"require-explicit-approval-prerequisites-before-runtime-shell\"");
    assert_contains(text, "\"selected_path\":\"continue-only-as-blocked-prerequisite-review\"");
    assert_contains(text, "\"consumes_node_v304_runtime_shell_chain_stop_or_prerequisite_decision_record\":true");
    assert_contains(text, "\"consumes_node_v303_post_decision_plan_intake_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_node_v305_stop_prerequisite_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_node_v305_runtime_shell_implementation\":false");
    assert_contains(text, "\"ready_for_disabled_runtime_shell_implementation\":false");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_enabled\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"runtime_shell_implementation_allowed\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_parse_allowed\":false");
    assert_contains(text, "\"provider_client_instantiation_allowed\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"connects_managed_audit\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"approval_gate_required\":true");
    assert_contains(text, "\"approval_gate_satisfied\":false");
    assert_contains(text, "\"approval_ledger_written\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"audit_authoritative\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v134\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json\"");
    assert_contains(
        receipt,
        "\"credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v134\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/134/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v304_shape(receipt);
    assert_source_node_v303_shape(receipt);
    assert_summary_and_flags(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(
        smoke,
        "\"credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v304_shape(smoke);
    assert_source_node_v303_shape(smoke);
    assert_summary_and_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_runtime_shell_chain_stop_prerequisite_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json\"");
    assert_contains(
        manifest,
        "\"credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt for Node v305");
    assert_source_node_v304_shape(manifest);
    assert_source_node_v303_shape(manifest);
    assert_summary_and_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(
        result.response,
        "\"credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v134\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/134/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v304_shape(result.response);
    assert_source_node_v303_shape(result.response);
    assert_summary_and_flags(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v309 may verify the mini-kv v136 credential resolver human approval artifact review non-participation receipt");
    assert_contains(
        result.response,
        "Node v305 may verify the mini-kv v134 credential resolver runtime shell chain stop/prerequisite non-participation receipt");
    assert_contains(
        result.response,
        "Node v302 may verify the mini-kv v133 credential resolver runtime shell post-decision plan intake non-participation receipt");
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
