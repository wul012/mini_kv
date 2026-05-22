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

void assert_source_node_v299_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v299_reference\":");
    assert_contains(text, "\"source_version\":\"Node v299\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-decision-record.v1\"");
    assert_contains(text, "\"decision_record_state\":\"runtime-shell-candidate-gate-decision-record-ready\"");
    assert_contains(text, "\"runtime_shell_decision\":\"blocked\"");
    assert_contains(
        text,
        "\"decision_digest\":\"4f6f73fa2806a9ba74174d7bbab17b43459bd1d790237276d95a3937c646e9c0\"");
    assert_contains(text, "\"record_mode\":\"runtime-shell-candidate-gate-decision-record-only\"");
    assert_contains(text, "\"decision_scope\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell\"");
    assert_contains(text, "\"source_span\":\"Node v297-v298 + Java v134 + mini-kv v131\"");
    assert_contains(text, "\"upstream_echo_verified\":true");
    assert_contains(text, "\"ready_for_parallel_java_v135_mini_kv_v132_echo_request\":true");
    assert_contains(text, "\"ready_for_node_v300_runtime_shell_decision_record_upstream_echo_verification\":false");
    assert_contains(text, "\"allows_node_v300_before_upstream_echo\":false");
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
    assert_contains(text, "\"required_evidence_count\":4");
    assert_contains(text, "\"missing_required_evidence_count\":0");
    assert_contains(text, "\"no_go_condition_count\":6");
    assert_contains(text, "\"node-v298-upstream-echo-ready\"");
    assert_contains(text, "\"java-v134-echo-ready\"");
    assert_contains(text, "\"mini-kv-v131-receipt-ready\"");
    assert_contains(text, "\"runtime-shell-still-blocked\"");
    assert_contains(text, "\"RUNTIME_SHELL_IMPLEMENTATION_REQUIRED\"");
    assert_contains(text, "\"CREDENTIAL_VALUE_REQUIRED\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_REQUIRED\"");
    assert_contains(text, "\"MANAGED_AUDIT_CONNECTION_REQUIRED\"");
    assert_contains(text, "\"LEDGER_SCHEMA_WRITE_REQUIRED\"");
    assert_contains(text, "\"AUTOSTART_REQUIRED\"");
}

void assert_source_node_v298_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v298_reference\":");
    assert_contains(text, "\"source_version\":\"Node v298\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-upstream-echo-verification.v1\"");
    assert_contains(text, "\"verification_state\":\"runtime-shell-candidate-gate-upstream-echo-verification-ready\"");
    assert_contains(
        text,
        "\"verification_digest\":\"fdf53be9d847694a7301731be659f694854a95d9b5a7a62b73dd069c6f08aa76\"");
    assert_contains(text, "\"source_span\":\"Node v297 + Java v134 + mini-kv v131\"");
    assert_contains(text, "\"consumes_java_v134_runtime_shell_candidate_gate_echo\":true");
    assert_contains(text, "\"consumes_mini_kv_v131_runtime_shell_candidate_gate_non_participation_receipt\":true");
    assert_contains(text, "\"ready_for_node_v299_runtime_shell_candidate_gate_decision_record\":true");
    assert_contains(text, "\"ready_for_node_v299_runtime_shell_implementation\":false");
    assert_contains(text, "\"check_count\":26");
    assert_contains(text, "\"passed_check_count\":26");
    assert_contains(text, "\"required_gate_count\":5");
    assert_contains(text, "\"documented_gate_count\":5");
    assert_contains(text, "\"runtime_prerequisite_satisfied_count\":0");
    assert_contains(text, "\"implementation_allowed_gate_count\":0");
}

void assert_summary_and_flags(const std::string& text) {
    assert_contains(text, "\"check_count\":14");
    assert_contains(text, "\"passed_check_count\":14");
    assert_contains(text, "\"required_evidence_count\":4");
    assert_contains(text, "\"missing_required_evidence_count\":0");
    assert_contains(text, "\"no_go_condition_count\":6");
    assert_contains(text, "\"source_node_v298_check_count\":26");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"DECISION_RECORD_ONLY_DOES_NOT_AUTHORIZE_RUNTIME\"");
    assert_contains(text, "\"RUN_JAVA_V135_AND_MINIKV_V132_IN_PARALLEL\"");
    assert_contains(text, "\"KEEP_NODE_V300_BEHIND_PARALLEL_EVIDENCE\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v135\",\"mini-kv v132\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"runtime_shell_decision_record_non_participation_receipt_only\":true");
    assert_contains(text, "\"runtime_shell_candidate_gate_decision_record_only\":true");
    assert_contains(text, "\"decision_record_only\":true");
    assert_contains(text, "\"runtime_shell_decision\":\"blocked\"");
    assert_contains(text, "\"consumes_node_v299_runtime_shell_candidate_gate_decision_record\":true");
    assert_contains(text, "\"ready_for_node_v300_runtime_shell_decision_record_upstream_echo_verification\":true");
    assert_contains(text, "\"ready_for_node_v300_runtime_shell_implementation\":false");
    assert_contains(text, "\"ready_for_disabled_runtime_shell_implementation\":false");
    assert_contains(text, "\"runtime_shell_implemented\":false");
    assert_contains(text, "\"runtime_shell_enabled\":false");
    assert_contains(text, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(text, "\"runtime_shell_implementation_allowed\":false");
    assert_contains(text, "\"disabled_runtime_shell_participates\":false");
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
                              "credential-resolver-runtime-shell-decision-record-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_runtime_shell_decision_record_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-runtime-shell-decision-record-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v132\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-runtime-shell-decision-record-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_runtime_shell_decision_record_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v132\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/132/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v299_shape(receipt);
    assert_source_node_v298_shape(receipt);
    assert_summary_and_flags(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_runtime_shell_decision_record_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v299_shape(smoke);
    assert_source_node_v298_shape(smoke);
    assert_summary_and_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_runtime_shell_decision_record_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-runtime-shell-decision-record-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_runtime_shell_decision_record_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_runtime_shell_decision_record_non_participation_receipt for Node v300");
    assert_source_node_v299_shape(manifest);
    assert_source_node_v298_shape(manifest);
    assert_summary_and_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_runtime_shell_decision_record_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-runtime-shell-decision-record-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v132\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/132/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v299_shape(result.response);
    assert_source_node_v298_shape(result.response);
    assert_summary_and_flags(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v311 may verify the mini-kv v137 credential resolver human approval artifact review post-echo decision gate non-participation receipt");
    assert_contains(
        result.response,
        "Node v305 may verify the mini-kv v134 credential resolver runtime shell chain stop/prerequisite non-participation receipt");
    assert_contains(
        result.response,
        "Node v302 may verify the mini-kv v133 credential resolver runtime shell post-decision plan intake non-participation receipt");
    assert_contains(
        result.response,
        "Node v300 may verify the mini-kv v132 credential resolver runtime shell decision record non-participation receipt");
    assert_contains(
        result.response,
        "Node v298 may verify the mini-kv v131 credential resolver disabled runtime shell candidate gate non-participation receipt");
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
