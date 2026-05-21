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

void assert_source_node_v301_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v301_reference\":");
    assert_contains(text, "\"source_version\":\"Node v301\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-continuation-plan-intake.v1\"");
    assert_contains(text, "\"plan_intake_state\":\"runtime-shell-post-decision-continuation-plan-intake-ready\"");
    assert_contains(
        text,
        "\"intake_digest\":\"ce85fa1c85cb2759abda5289f9bfec0cf162b71f2da0a12dfdf2ffe7cee8d65a\"");
    assert_contains(
        text,
        "\"proof_digest\":\"9b7d9215e58ce16ad0c2b67f4560005ae3b1436fbf4087fb0ebdfca5c841d00e\"");
    assert_contains(text, "\"intake_mode\":\"runtime-shell-post-decision-continuation-plan-intake-only\"");
    assert_contains(text, "\"selected_continuation_decision\":\"continue-blocked-planning\"");
    assert_contains(text, "\"decision_option_count\":4");
    assert_contains(text, "\"selected_decision_option_count\":1");
    assert_contains(text, "\"rejected_runtime_implementation_option_count\":1");
    assert_contains(text, "\"next_java_echo_version\":\"Java v136\"");
    assert_contains(text, "\"next_mini_kv_receipt_version\":\"mini-kv v133\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v302\"");
    assert_contains(text, "\"CONTINUE_BLOCKED_PLANNING\"");
    assert_contains(text, "\"PAUSE_RUNTIME_SHELL_CHAIN\"");
    assert_contains(text, "\"REQUIRE_EXPLICIT_APPROVAL_PREREQUISITES\"");
    assert_contains(text, "\"IMPLEMENT_RUNTIME_SHELL_NOW\"");
    assert_contains(text, "\"status\":\"rejected\"");
    assert_contains(text, "\"check_count\":25");
    assert_contains(text, "\"passed_check_count\":25");
    assert_contains(text, "\"proof_complete\":true");
    assert_contains(text, "\"read_only_plan_intake\":true");
    assert_contains(text, "\"runtime_shell_post_decision_continuation_plan_intake_only\":true");
}

void assert_source_node_v300_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v300_reference\":");
    assert_contains(text, "\"source_version\":\"Node v300\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-decision-record-upstream-echo-verification.v1\"");
    assert_contains(text, "\"verification_state\":\"runtime-shell-decision-record-upstream-echo-verification-ready\"");
    assert_contains(
        text,
        "\"verification_digest\":\"a7d1cc22ce3f8d6da30d5a91d2d1f8fc13e480babfac8296c4b8ca86519d4f8c\"");
    assert_contains(text, "\"source_span\":\"Node v299 + Java v135 + mini-kv v132\"");
    assert_contains(text, "\"consumes_node_v299_runtime_shell_candidate_gate_decision_record\":true");
    assert_contains(text, "\"consumes_java_v135_runtime_shell_decision_record_echo\":true");
    assert_contains(text, "\"consumes_mini_kv_v132_runtime_shell_decision_record_non_participation_receipt\":true");
    assert_contains(text, "\"ready_for_post_runtime_shell_decision_plan\":true");
    assert_contains(text, "\"upstream_echo_aligned\":true");
    assert_contains(text, "\"blocked_decision_aligned\":true");
    assert_contains(text, "\"side_effect_boundaries_aligned\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
    assert_contains(text, "\"check_count\":19");
    assert_contains(text, "\"passed_check_count\":19");
}

void assert_summary_and_flags(const std::string& text) {
    assert_contains(text, "\"check_count\":25");
    assert_contains(text, "\"passed_check_count\":25");
    assert_contains(text, "\"source_node_v300_check_count\":19");
    assert_contains(text, "\"source_node_v300_passed_check_count\":19");
    assert_contains(text, "\"continuation_option_count\":4");
    assert_contains(text, "\"selected_continuation_option_count\":1");
    assert_contains(text, "\"rejected_runtime_implementation_option_count\":1");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"CONTINUATION_PLAN_DOES_NOT_AUTHORIZE_RUNTIME\"");
    assert_contains(text, "\"REQUEST_PARALLEL_JAVA_MINI_KV_ECHO\"");
    assert_contains(text, "\"STOP_CHAIN_AFTER_V302_WITHOUT_NEW_BLOCKER\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v136\",\"mini-kv v133\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"runtime_shell_post_decision_plan_intake_non_participation_receipt_only\":true");
    assert_contains(text, "\"runtime_shell_post_decision_continuation_plan_intake_only\":true");
    assert_contains(text, "\"post_decision_plan_intake_only\":true");
    assert_contains(text, "\"selected_continuation_decision\":\"continue-blocked-planning\"");
    assert_contains(text, "\"consumes_node_v301_runtime_shell_post_decision_plan_intake\":true");
    assert_contains(text, "\"ready_for_node_v302_post_decision_plan_intake_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_node_v302_runtime_shell_implementation\":false");
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
                              "credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v133\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v133\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/133/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v301_shape(receipt);
    assert_source_node_v300_shape(receipt);
    assert_summary_and_flags(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v301_shape(smoke);
    assert_source_node_v300_shape(smoke);
    assert_summary_and_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_runtime_shell_post_decision_plan_intake_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt for Node v302");
    assert_source_node_v301_shape(manifest);
    assert_source_node_v300_shape(manifest);
    assert_summary_and_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v133\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/133/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v301_shape(result.response);
    assert_source_node_v300_shape(result.response);
    assert_summary_and_flags(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v302 may verify the mini-kv v133 credential resolver runtime shell post-decision plan intake non-participation receipt");
    assert_contains(
        result.response,
        "Node v300 may verify the mini-kv v132 credential resolver runtime shell decision record non-participation receipt");
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
