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

void assert_node_v292_shape(const std::string& text) {
    assert_contains(text, "\"source_version\":\"Node v292\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-fake-harness-readiness-decision-record.v1\"");
    assert_contains(text, "\"decision_record_state\":\"fake-harness-readiness-decision-record-ready\"");
    assert_contains(text, "\"readiness_decision\":\"blocked\"");
    assert_contains(
        text,
        "\"decision_digest\":\"a21613fd08151de0690e5f83826f4b5d6a62131b52273b3c368130197949798d\"");
    assert_contains(text, "\"source_span\":\"Node v287-v291\"");
    assert_contains(text, "\"required_evidence_count\":4");
    assert_contains(text, "\"missing_required_evidence_count\":1");
    assert_contains(text, "\"no_go_condition_count\":8");
    assert_contains(text, "\"production_blocker_count\":1");
    assert_contains(text, "\"read_only_decision_record\":true");
    assert_contains(text, "\"fake_harness_readiness_decision_only\":true");
    assert_contains(text, "\"java_direct_execution_denied_echo_missing\":true");
    assert_contains(text, "\"mini_kv_v128_non_participation_ready\":true");
    assert_contains(text, "\"side_effect_boundaries_aligned\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
    assert_contains(text, "\"id\":\"java-direct-execution-denied-echo\",\"present\":false");
    assert_contains(text, "\"JAVA_EXECUTION_DENIED_ECHO_MISSING\"");
}

void assert_node_v291_shape(const std::string& text) {
    assert_contains(text, "\"source_version\":\"Node v291\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-execution-denied-upstream-echo-verification.v1\"");
    assert_contains(text, "\"verification_state\":\"blocked\"");
    assert_contains(
        text,
        "\"verification_digest\":\"8f84fc7940c72f02b3e229da3f02468488fe5789e4699a17d3ae07585689d613\"");
    assert_contains(text, "\"source_span\":\"Node v290 + Java v127-v130 + mini-kv v128\"");
    assert_contains(text, "\"source_check_count\":19");
    assert_contains(text, "\"source_passed_check_count\":17");
    assert_contains(text, "\"read_only_upstream_echo_verification\":true");
    assert_contains(text, "\"execution_denied_upstream_echo_verification_only\":true");
    assert_contains(text, "\"java_execution_denied_echo_missing\":true");
    assert_contains(text, "\"java_execution_denied_echo_present\":false");
    assert_contains(text, "\"mini_kv_preflight_digest_aligned\":true");
}

void assert_v128_retention_shape(const std::string& text) {
    assert_contains(text, "\"source_mini_kv_v128_receipt\":");
    assert_contains(
        text,
        "\"fixture_path\":\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json\"");
    assert_contains(text, "\"artifact_path_hint\":\"d/128/\"");
    assert_contains(text, "\"receipt_digest\":\"fnv1a64:d60057a3760cad62\"");
    assert_contains(
        text,
        "\"preflight_digest\":\"21ea6d7a6c641489048fc2540126e2f461909246fdd2129bcb0fc6d1ffbbf1cb\"");
    assert_contains(text, "\"check_count\":25");
    assert_contains(text, "\"passed_check_count\":25");
    assert_contains(text, "\"simulated_attempt_count\":8");
    assert_contains(text, "\"denied_attempt_count\":8");
    assert_contains(text, "\"actual_execution_attempt_count\":0");
    assert_contains(text, "\"retention_check\":{\"check_mode\":\"v128-receipt-verification-retention-check-only\"");
    assert_contains(text, "\"historical_fixture_friendly\":true");
    assert_contains(text, "\"verifies_runtime_smoke_exposes_v128_receipt\":true");
    assert_contains(text, "\"verifies_release_manifest_exposes_v128_receipt\":true");
    assert_contains(text, "\"verifies_digest_stable\":true");
    assert_contains(text, "\"verifies_preflight_digest_stable\":true");
    assert_contains(text, "\"verifies_no_runtime_shell_planning\":true");
    assert_contains(text, "\"verification_count\":8");
}

void assert_summary_and_flags(const std::string& text) {
    assert_contains(text, "\"check_count\":18");
    assert_contains(text, "\"passed_check_count\":18");
    assert_contains(text, "\"source_node_v292_check_count\":18");
    assert_contains(text, "\"source_node_v291_check_count\":19");
    assert_contains(text, "\"source_node_v291_passed_check_count\":17");
    assert_contains(text, "\"v128_receipt_check_count\":25");
    assert_contains(text, "\"required_evidence_count\":4");
    assert_contains(text, "\"missing_required_evidence_count\":1");
    assert_contains(text, "\"no_go_condition_count\":8");
    assert_contains(text, "\"production_blocker_count\":1");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"V128_RECEIPT_RETENTION_CHECK_IS_NOT_RUNTIME\"");
    assert_contains(text, "\"RUN_JAVA_V131_MINI_KV_V129_IN_PARALLEL\"");
    assert_contains(text, "\"WAIT_FOR_NODE_V293_BLOCKED_DECISION_UPSTREAM_ECHO\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v131\",\"mini-kv v129\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"receipt_verification_retention_check_only\":true");
    assert_contains(text, "\"retention_check_only\":true");
    assert_contains(text, "\"consumes_node_v292_fake_harness_readiness_decision_record\":true");
    assert_contains(text, "\"consumes_node_v291_execution_denied_upstream_echo_verification\":true");
    assert_contains(text, "\"consumes_mini_kv_v128_execution_denied_receipt\":true");
    assert_contains(text, "\"ready_for_node_v293_blocked_decision_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_disabled_runtime_shell_planning\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"test_only_fake_harness_execution_allowed\":false");
    assert_contains(text, "\"fake_harness_runtime_enabled\":false");
    assert_contains(text, "\"fake_harness_invocation_allowed\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"connects_managed_audit\":false");
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
                              "credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json";
    const auto source_receipt_path = std::filesystem::path{"fixtures"} / "release" /
                                     "credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(source_receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.v1\"");
    assert_contains(receipt, "\"release_version\":\"v129\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json\"");
    assert_contains(
        receipt,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\":");
    assert_contains(receipt, "\"current_release_version\":\"v129\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/129/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_node_v292_shape(receipt);
    assert_node_v291_shape(receipt);
    assert_v128_retention_shape(receipt);
    assert_summary_and_flags(receipt);
    assert_contains(receipt, "receipt verification retention check only");
    assert_contains(receipt, "does not implement or run a fake harness runtime");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(
        smoke,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_node_v292_shape(smoke);
    assert_node_v291_shape(smoke);
    assert_v128_retention_shape(smoke);
    assert_summary_and_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(
        manifest,
        "\"minikv_cred_res_exec_denied_retention_check_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json\"");
    assert_contains(
        manifest,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check for Node v293");
    assert_node_v292_shape(manifest);
    assert_node_v291_shape(manifest);
    assert_v128_retention_shape(manifest);
    assert_summary_and_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(
        result.response,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v129\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/129/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_node_v292_shape(result.response);
    assert_node_v291_shape(result.response);
    assert_v128_retention_shape(result.response);
    assert_summary_and_flags(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v300 may verify the mini-kv v132 credential resolver runtime shell decision record non-participation receipt");
    assert_contains(
        result.response,
        "Node v298 may verify the mini-kv v131 credential resolver disabled runtime shell candidate gate non-participation receipt");
    assert_contains(
        result.response,
        "Node v296 may verify the mini-kv v130 credential resolver disabled runtime shell non-participation receipt");
    assert_contains(
        result.response,
        "Node v293 may verify the mini-kv v129 credential resolver disabled fake harness execution-denied receipt verification/retention check");
    assert_contains(
        result.response,
        "Node v291 may verify the mini-kv v128 credential resolver disabled fake harness execution-denied non-participation receipt");
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
