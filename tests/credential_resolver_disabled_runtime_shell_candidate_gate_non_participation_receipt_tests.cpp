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

void assert_source_node_v297_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v297_reference\":");
    assert_contains(text, "\"source_version\":\"Node v297\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-implementation-candidate-gate.v1\"");
    assert_contains(text, "\"candidate_gate_state\":\"disabled-runtime-shell-implementation-candidate-gate-reviewed\"");
    assert_contains(text, "\"gate_version\":\"node-v297-disabled-runtime-shell-implementation-candidate-gate.v1\"");
    assert_contains(text, "\"gate_mode\":\"candidate-gate-only-default-blocked\"");
    assert_contains(
        text,
        "\"gate_decision\":\"blocked-request-parallel-java-v134-and-mini-kv-v131-before-implementation\"");
    assert_contains(
        text,
        "\"gate_digest\":\"651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22\"");
    assert_contains(text, "\"blocker\":\"candidate-gate-lacks-upstream-echo-and-runtime-prerequisite-proof\"");
    assert_contains(text, "\"cannot_reuse_existing_report_reason\":");
    assert_contains(text, "\"DEDICATED_DISABLED_BY_DEFAULT_FLAG\"");
    assert_contains(text, "\"OPERATOR_APPROVAL\"");
    assert_contains(text, "\"ABORT_SEMANTICS\"");
    assert_contains(text, "\"NO_NETWORK_TESTS\"");
    assert_contains(text, "\"HISTORICAL_FALLBACK_EVIDENCE\"");
    assert_contains(text, "\"required_gate_count\":5");
    assert_contains(text, "\"documented_gate_count\":5");
    assert_contains(text, "\"review_evidence_satisfied_count\":5");
    assert_contains(text, "\"runtime_prerequisite_satisfied_count\":0");
    assert_contains(text, "\"implementation_allowed_gate_count\":0");
    assert_contains(text, "\"stop_condition_count\":6");
    assert_contains(text, "\"ready_for_parallel_java_v134_mini_kv_v131_echo_request\":true");
    assert_contains(text, "\"ready_for_node_v298_runtime_shell_implementation\":false");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"approval_ledger_written\":false");
    assert_contains(text, "\"automatic_upstream_start\":false");
}

void assert_source_node_v296_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v296_reference\":");
    assert_contains(text, "\"source_version\":\"Node v296\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-upstream-echo-verification.v1\"");
    assert_contains(text, "\"verification_state\":\"disabled-runtime-shell-upstream-echo-verification-ready\"");
    assert_contains(
        text,
        "\"verification_digest\":\"9bbba02d888b5c986026f3e80e0c07df27d08699405d9989cc42e9bd1c8c6f7b\"");
    assert_contains(text, "\"plan_version_correction_applied\":true");
    assert_contains(text, "\"planned_java_version\":\"Java v132\"");
    assert_contains(text, "\"actual_java_echo_version\":\"Java v133\"");
    assert_contains(text, "\"consumes_mini_kv_v130\":true");
    assert_contains(text, "\"ready_for_node_v297_candidate_gate\":true");
    assert_contains(text, "\"ready_for_node_v297_runtime_shell_implementation\":false");
    assert_contains(text, "\"check_count\":24");
    assert_contains(text, "\"passed_check_count\":24");
}

void assert_summary_and_flags(const std::string& text) {
    assert_contains(text, "\"check_count\":27");
    assert_contains(text, "\"passed_check_count\":27");
    assert_contains(text, "\"source_node_v297_check_count\":27");
    assert_contains(text, "\"source_node_v296_check_count\":24");
    assert_contains(text, "\"required_gate_count\":5");
    assert_contains(text, "\"documented_gate_count\":5");
    assert_contains(text, "\"review_evidence_satisfied_count\":5");
    assert_contains(text, "\"runtime_prerequisite_satisfied_count\":0");
    assert_contains(text, "\"implementation_allowed_gate_count\":0");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"CANDIDATE_GATE_ONLY_DEFAULT_BLOCKED\"");
    assert_contains(text, "\"UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V298\"");
    assert_contains(text, "\"RUN_PARALLEL_JAVA_V134_MINI_KV_V131\"");
    assert_contains(text, "\"VERIFY_WITH_NODE_V298_AFTER_UPSTREAM_ECHO\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v134\",\"mini-kv v131\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"candidate_gate_non_participation_receipt_only\":true");
    assert_contains(text, "\"runtime_shell_candidate_gate_non_participation_receipt_only\":true");
    assert_contains(text, "\"consumes_node_v297_disabled_runtime_shell_implementation_candidate_gate\":true");
    assert_contains(text, "\"ready_for_node_v298_runtime_shell_candidate_gate_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_node_v298_runtime_shell_implementation\":false");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_enabled\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"runtime_prerequisite_satisfied\":false");
    assert_contains(text, "\"implementation_allowed\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_parse_allowed\":false");
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
                              "credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v131\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v131\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/131/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v297_shape(receipt);
    assert_source_node_v296_shape(receipt);
    assert_summary_and_flags(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v297_shape(smoke);
    assert_source_node_v296_shape(smoke);
    assert_summary_and_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_disabled_runtime_shell_candidate_gate_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt for Node v298");
    assert_source_node_v297_shape(manifest);
    assert_source_node_v296_shape(manifest);
    assert_summary_and_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v131\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/131/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v297_shape(result.response);
    assert_source_node_v296_shape(result.response);
    assert_summary_and_flags(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v298 may verify the mini-kv v131 credential resolver disabled runtime shell candidate gate non-participation receipt");
    assert_contains(
        result.response,
        "Node v296 may verify the mini-kv v130 credential resolver disabled runtime shell non-participation receipt");
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
