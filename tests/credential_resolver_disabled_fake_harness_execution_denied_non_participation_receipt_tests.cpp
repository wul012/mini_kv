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

void assert_source_node_v289_shape(const std::string& text) {
    assert_contains(text, "\"source_version\":\"Node v289\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract-upstream-echo-verification.v1\"");
    assert_contains(text, "\"verification_state\":\"disabled-fake-harness-contract-upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_disabled_fake_harness_contract_upstream_echo_verification\":true");
    assert_contains(
        text,
        "\"verification_digest\":\"e5b596b65f68e753459be163f8a58d1f677098e64b3be01d4e83737ef8c4d94f\"");
    assert_contains(text, "\"source_span\":\"Node v288 + Java v122-v126 + mini-kv v127\"");
    assert_contains(text, "\"source_check_count\":29");
    assert_contains(text, "\"source_passed_check_count\":29");
    assert_contains(text, "\"java_evidence_ready\":true");
    assert_contains(text, "\"mini_kv_non_participation_ready\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
    assert_contains(text, "\"ready_for_next_disabled_fake_harness_planning\":true");
}

void assert_preflight_shape(const std::string& text) {
    assert_contains(text, "\"preflight_digest\":\"21ea6d7a6c641489048fc2540126e2f461909246fdd2129bcb0fc6d1ffbbf1cb\"");
    assert_contains(
        text,
        "\"preflight_mode\":\"disabled-fake-harness-execution-denied-route-preflight-only\"");
    assert_contains(text, "\"route_surface\":\"audit-json-markdown-route\"");
    assert_contains(
        text,
        "\"route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-execution-denied-route-preflight\"");
    assert_contains(text, "\"http_method\":\"GET\"");
    assert_contains(text, "\"format_modes\":[\"json\",\"markdown\"]");
    assert_contains(text, "\"route_registered\":true");
    assert_contains(text, "\"route_read_only\":true");
    assert_contains(text, "\"route_execution_denied\":true");
    assert_contains(text, "\"execution_denied_reason_count\":8");
    assert_contains(text, "\"simulated_attempt_count\":8");
    assert_contains(text, "\"denied_attempt_count\":8");
    assert_contains(text, "\"actual_execution_attempt_count\":0");
    assert_contains(text, "\"approval_gate_required\":true");
    assert_contains(text, "\"approval_gate_satisfied\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_parse_allowed\":false");
    assert_contains(text, "\"provider_client_instantiation_allowed\":false");
    assert_contains(text, "\"http_tcp_dial_allowed\":false");
    assert_contains(text, "\"ledger_write_allowed\":false");
    assert_contains(text, "\"schema_migration_allowed\":false");
    assert_contains(text, "\"fake_harness_runtime_implementation_allowed\":false");
    assert_contains(text, "\"fake_harness_runtime_invocation_allowed\":false");
    assert_contains(text, "\"automatic_upstream_start_allowed\":false");
}

void assert_summary_and_flags(const std::string& text) {
    assert_contains(text, "\"check_count\":25");
    assert_contains(text, "\"passed_check_count\":25");
    assert_contains(text, "\"simulated_attempt_count\":8");
    assert_contains(text, "\"denied_attempt_count\":8");
    assert_contains(text, "\"actual_execution_attempt_count\":0");
    assert_contains(text, "\"denial_reason_count\":8");
    assert_contains(text, "\"source_check_count\":29");
    assert_contains(text, "\"source_passed_check_count\":29");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"EXECUTION_DENIED_PREFLIGHT_IS_NOT_RUNTIME\"");
    assert_contains(text, "\"JAVA_QUALITY_QUEUE_NOT_RUNTIME_ECHO\"");
    assert_contains(text, "\"RUN_JAVA_V127_MINI_KV_V128_IN_PARALLEL\"");
    assert_contains(text, "\"WAIT_FOR_UPSTREAM_STATE_BEFORE_NODE_V291\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v127\",\"mini-kv v128\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"execution_denied_non_participation_receipt_only\":true");
    assert_contains(text, "\"execution_denied_route_preflight_only\":true");
    assert_contains(text, "\"consumes_node_v290_execution_denied_route_preflight\":true");
    assert_contains(text, "\"ready_for_node_v291_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_java_v127_mini_kv_v128_parallel_evidence\":true");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"test_only_fake_harness_allowed\":false");
    assert_contains(text, "\"fake_harness_runtime_enabled\":false");
    assert_contains(text, "\"fake_harness_invocation_allowed\":false");
    assert_contains(text, "\"execute_fake_harness_runtime_allowed\":false");
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
                              "credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v128\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v128\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/128/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v289_shape(receipt);
    assert_preflight_shape(receipt);
    assert_summary_and_flags(receipt);
    assert_contains(receipt, "execution-denied non-participation receipt only");
    assert_contains(receipt, "does not implement or run a fake harness runtime");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v289_shape(smoke);
    assert_preflight_shape(smoke);
    assert_summary_and_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt for Node v291");
    assert_source_node_v289_shape(manifest);
    assert_preflight_shape(manifest);
    assert_summary_and_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v128\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/128/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v289_shape(result.response);
    assert_preflight_shape(result.response);
    assert_summary_and_flags(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v291 may verify the mini-kv v128 credential resolver disabled fake harness execution-denied non-participation receipt");
    assert_contains(result.response, "Node v290 disabled fake harness execution-denied route preflight");
    assert_contains(result.response, "Node v289 may verify the mini-kv v127 credential resolver disabled fake harness non-participation receipt");
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
