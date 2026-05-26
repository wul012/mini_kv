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

void assert_source_node_v329_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v329_reference\":");
    assert_contains(text, "\"source_version\":\"Node v329\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-implementation-candidate-gate-input-hardening-decision.v1\"");
    assert_contains(text, "\"candidate_gate_state\":\"implementation-candidate-gate-input-hardening-decision-ready\"");
    assert_contains(text,
                    "\"candidate_gate_decision\":\"require-input-export-hardening-before-disabled-runtime-design\"");
    assert_contains(text, "\"read_only_decision_record\":true");
    assert_contains(text, "\"implementation_candidate_gate_only\":true");
    assert_contains(text, "\"consumes_node_v328_final_prerequisite_closure_review\":true");
    assert_contains(text, "\"ready_for_parallel_java_v151_mini_kv_v143_echo_request\":true");
    assert_contains(text, "\"ready_for_node_v330_candidate_gate_upstream_alignment\":false");
    assert_contains(text, "\"ready_for_disabled_runtime_shell_design_draft\":false");
    assert_contains(text,
                    "\"decision_digest\":\"e92109116e9d106ffdaa5691aa2eaa626fb595f8a9f4020dbf903fbce7874e87\"");
    assert_contains(text, "\"record_mode\":\"implementation-candidate-gate-input-hardening-decision-only\"");
    assert_contains(text, "\"all_prerequisites_closed\":true");
    assert_contains(text, "\"candidate_gate_entered\":true");
    assert_contains(text, "\"allows_node_v330_before_upstream_echo\":false");
    assert_contains(text, "\"allows_disabled_runtime_shell_design_draft\":false");
    assert_contains(text, "\"allows_real_resolver_implementation\":false");
    assert_contains(text, "\"allows_mini_kv_write_or_admin_command\":false");
    assert_contains(text, "\"input_hardening_requirement_count\":4");
    assert_contains(text, "\"no_go_condition_count\":7");
}

void assert_source_node_v328_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v328_reference\":");
    assert_contains(text, "\"source_version\":\"Node v328\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-final-prerequisite-closure-review.v1\"");
    assert_contains(text, "\"review_state\":\"final-prerequisite-closure-review-ready\"");
    assert_contains(text, "\"all_prerequisites_closed\":true");
    assert_contains(text, "\"ready_for_implementation_candidate_gate\":true");
    assert_contains(
        text,
        "\"closure_digest\":\"15d924b9d529f4d60dd43010b82689f5173d4f2583f8ae31f070f877141a6d14\"");
    assert_contains(text, "\"completed_prerequisite_count\":6");
    assert_contains(text, "\"remaining_prerequisite_count\":0");
    assert_contains(text, "\"original_prerequisite_count\":6");
    assert_contains(text, "\"next_step_mode\":\"implementation-candidate-gate-only\"");
    assert_contains(text, "\"source_check_count\":18");
    assert_contains(text, "\"source_passed_check_count\":18");
}

void assert_stable_current_receipt_shape(const std::string& text) {
    assert_contains(text, "\"requirement_id\":\"mini-kv-stable-current-receipt\"");
    assert_contains(text, "\"requested_version\":\"mini-kv v143\"");
    assert_contains(text, "\"status\":\"required-and-echoed\"");
    assert_contains(text, "\"stable_current_receipt_path_required\":true");
    assert_contains(text, "\"stable_current_receipt_export_required\":true");
    assert_contains(text, "\"stable_current_receipt_export_ready\":true");
    assert_contains(
        text,
        "\"runtime_smoke_field\":\"credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt\"");
    assert_contains(text, "fixtures/release/runtime-smoke-evidence.json");
    assert_contains(text, "fixtures/release/verification-manifest.json");
    assert_contains(text, "\"mini_kv_v143_ready_for_node_v330_candidate_gate_upstream_alignment\":true");
    assert_contains(text, "\"ready_for_node_v330_candidate_gate_upstream_alignment_after_java_v151\":true");
    assert_contains(text, "\"ready_for_node_v330_before_java_v151\":false");
}

void assert_boundaries(const std::string& text) {
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"implementation_candidate_gate_input_hardening_non_participation_receipt_only\":true");
    assert_contains(text, "\"read_only_input_hardening_decision_echo\":true");
    assert_contains(text, "\"runtime_shell_design_draft_allowed\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"http_request_sent\":false");
    assert_contains(text, "\"tcp_connection_attempted\":false");
    assert_contains(text, "\"approval_ledger_written\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"rollback_execution_allowed\":false");
    assert_contains(text, "\"deployment_action_allowed\":false");
    assert_contains(text, "\"java_sql_execution_allowed\":false");
    assert_contains(text, "\"mini_kv_write_command_allowed\":false");
    assert_contains(text, "\"mini_kv_load_allowed\":false");
    assert_contains(text, "\"mini_kv_compact_allowed\":false");
    assert_contains(text, "\"mini_kv_restore_allowed\":false");
    assert_contains(text, "\"mini_kv_setnxex_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"automatic_upstream_start\":false");
    assert_contains(text, "\"audit_authoritative\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

void assert_summary_and_checks(const std::string& text) {
    assert_contains(text, "\"check_count\":16");
    assert_contains(text, "\"passed_check_count\":16");
    assert_contains(text, "\"source_node_v328_check_count\":18");
    assert_contains(text, "\"input_hardening_requirement_count\":4");
    assert_contains(text, "\"no_go_condition_count\":7");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"stable_current_receipt_requirement_echoed\":true");
    assert_contains(text, "\"stable_current_receipt_export_paths_documented\":true");
    assert_contains(text, "\"node_v330_waits_for_upstream_echo\":true");
    assert_contains(text, "\"runtime_design_still_blocked\":true");
    assert_contains(text, "\"mini_kv_admin_write_still_closed\":true");
    assert_contains(text, "\"load_compact_restore_setnxex_still_blocked\":true");
    assert_contains(text, "\"CANDIDATE_GATE_IS_NOT_RUNTIME_DESIGN\"");
    assert_contains(text, "\"PROVIDE_MINI_KV_STABLE_CURRENT_RECEIPT_EXPORT\"");
    assert_contains(text, "\"LET_NODE_V330_CONSUME_JAVA_V151_AND_MINI_KV_V143\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v151\",\"mini-kv v143\"]");
}

} // namespace

int main() {
    const auto receipt_path =
        std::filesystem::path{"fixtures"} / "release" /
        "credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v143\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.json\"");
    assert_contains(
        receipt,
        "\"credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v143\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/143/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v329_shape(receipt);
    assert_source_node_v328_shape(receipt);
    assert_stable_current_receipt_shape(receipt);
    assert_summary_and_checks(receipt);
    assert_boundaries(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");
    assert_not_contains(receipt, "runtime_shell_command\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(
        smoke,
        "\"credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v329_shape(smoke);
    assert_source_node_v328_shape(smoke);
    assert_stable_current_receipt_shape(smoke);
    assert_summary_and_checks(smoke);
    assert_boundaries(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_input_hardening_candidate_gate_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.json\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt for Node v330");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v329_shape(manifest);
    assert_source_node_v328_shape(manifest);
    assert_stable_current_receipt_shape(manifest);
    assert_summary_and_checks(manifest);
    assert_boundaries(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(
        result.response,
        "\"credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt\":");
    assert_contains(result.response, "\"current_release_version\":\"v143\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/143/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v329_shape(result.response);
    assert_source_node_v328_shape(result.response);
    assert_stable_current_receipt_shape(result.response);
    assert_summary_and_checks(result.response);
    assert_boundaries(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v330 may verify the mini-kv v143 credential resolver implementation candidate gate input-hardening non-participation receipt");
    assert_contains(
        result.response,
        "Node v327 may verify the mini-kv v142 credential resolver abort/rollback semantics contract non-participation receipt");
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
