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

void assert_source_node_v306_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v306_reference\":");
    assert_contains(text, "\"source_version\":\"Node v306\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-approval-prerequisite-artifact-intake-plan.v1\"");
    assert_contains(text, "\"plan_state\":\"approval-prerequisite-artifact-intake-plan-ready\"");
    assert_contains(
        text,
        "\"runtime_shell_chain_decision\":\"require-explicit-approval-prerequisites-before-runtime-shell\"");
    assert_contains(text, "\"ready_for_approval_prerequisite_artifact_intake_plan\":true");
    assert_contains(text, "\"approval_prerequisite_artifact_intake_plan_only\":true");
    assert_contains(text, "\"read_only_artifact_contract\":true");
    assert_contains(text, "\"next_java_version\":\"Java v142\"");
    assert_contains(text, "\"next_mini_kv_version\":\"mini-kv v135\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v307\"");
    assert_contains(text, "\"ready_for_parallel_java_v142_mini_kv_v135_echo\":true");
    assert_contains(text, "\"ready_for_node_v307_before_upstream_echo\":false");
    assert_contains(text, "\"artifact_name\":\"managed-audit-runtime-shell-approval-prerequisite-artifact\"");
    assert_contains(text, "\"artifact_version\":\"approval-prerequisite-artifact.v1\"");
    assert_contains(text, "\"intake_mode\":\"approval-prerequisite-artifact-intake-plan-only\"");
    assert_contains(text, "\"source_span\":\"Node v305 + planned Java v142 + planned mini-kv v135\"");
    assert_contains(text, "\"artifact_digest\":\"72f3e90606e40a978611fa4b8596c76c3ebc468124c4ead7bb9c4833130ee9c2\"");
    assert_contains(text, "\"required_field_count\":12");
    assert_contains(text, "\"prohibited_field_count\":8");
    assert_contains(text, "\"rejection_reason_count\":9");
    assert_contains(text, "\"no_go_boundary_count\":12");
    assert_contains(text, "\"upstream_echo_request_count\":2");
    assert_contains(text, "\"java_mini_kv_echo_can_run_in_parallel\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
    assert_contains(text, "\"consumer\":\"Java v142 + mini-kv v135, then Node v307\"");
    assert_contains(text, "\"proof_complete\":true");
}

void assert_artifact_contract_fields(const std::string& text) {
    assert_contains(text, "\"artifact_id\"");
    assert_contains(text, "\"source_node_verification\"");
    assert_contains(text, "\"operator_approval_reference\"");
    assert_contains(text, "\"credential_handle_review_status\"");
    assert_contains(text, "\"endpoint_handle_allowlist_review_status\"");
    assert_contains(text, "\"no_network_safety_test_reference\"");
    assert_contains(text, "\"manual_abort_semantics_reference\"");
    assert_contains(text, "\"rollback_semantics_reference\"");
    assert_contains(text, "\"java_echo_required_version\"");
    assert_contains(text, "\"mini_kv_receipt_required_version\"");
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
    assert_contains(text, "\"CREDENTIAL_VALUE_PRESENT\"");
    assert_contains(text, "\"RAW_ENDPOINT_URL_PRESENT\"");
    assert_contains(text, "\"JAVA_OR_MINIKV_ECHO_MISSING\"");
    assert_contains(text, "\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\"");
    assert_contains(text, "\"EXTERNAL_REQUEST_REQUESTED\"");
    assert_contains(text, "\"WRITE_OR_SCHEMA_MUTATION_REQUESTED\"");
}

void assert_source_node_v305_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v305_reference\":");
    assert_contains(text, "\"source_version\":\"Node v305\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-chain-stop-prerequisite-upstream-echo-verification.v1\"");
    assert_contains(
        text,
        "\"verification_state\":\"runtime-shell-chain-stop-prerequisite-upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_upstream_echo_verification\":true");
    assert_contains(text, "\"verification_digest\":\"75278ffa6fe777a5936b47382e49d4524dbc2bd6eec893f2b002c6323e47b5bb\"");
    assert_contains(text, "\"source_node_v304_decision_digest\":\"9212d0b804fdc1eda9098ac70d2441681730a98ff736776859811df9e288a654\"");
    assert_contains(text, "\"source_span\":\"Node v304 + Java v141 + mini-kv v134\"");
    assert_contains(text, "\"upstream_echo_aligned\":true");
    assert_contains(text, "\"prerequisite_gate_still_blocked\":true");
    assert_contains(text, "\"side_effect_boundaries_aligned\":true");
    assert_contains(text, "\"prerequisite_count\":6");
    assert_contains(text, "\"missing_runtime_prerequisite_count\":6");
    assert_contains(text, "\"no_go_condition_count\":8");
}

void assert_summary_and_boundaries(const std::string& text) {
    assert_contains(text, "\"check_count\":18");
    assert_contains(text, "\"passed_check_count\":18");
    assert_contains(text, "\"source_node_v306_check_count\":16");
    assert_contains(text, "\"source_node_v306_passed_check_count\":16");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"ARTIFACT_INTAKE_PLAN_DOES_NOT_AUTHORIZE_RUNTIME\"");
    assert_contains(text, "\"RUN_JAVA_V142_AND_MINI_KV_V135_IN_PARALLEL_AFTER_V306\"");
    assert_contains(text, "\"VERIFY_ARTIFACT_ECHO_WITH_NODE_V307\"");
    assert_contains(text, "\"next_required_evidence_versions\":[\"Java v142\",\"mini-kv v135\"]");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"approval_prerequisite_artifact_intake_non_participation_receipt_only\":true");
    assert_contains(text, "\"approval_prerequisite_artifact_intake_plan_only\":true");
    assert_contains(text, "\"read_only_artifact_contract\":true");
    assert_contains(text, "\"consumes_node_v306_approval_prerequisite_artifact_intake_plan\":true");
    assert_contains(text, "\"ready_for_node_v307_approval_prerequisite_artifact_upstream_echo_verification\":true");
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
                              "credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {"INFOJSON", "STORAGEJSON", "HEALTH", "STATSJSON"};
    const auto expected_digest =
        minikv::runtime_evidence_receipts::
            credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_digest(
                read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v135\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.json\"");
    assert_contains(
        receipt,
        "\"credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v135\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/135/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v306_shape(receipt);
    assert_artifact_contract_fields(receipt);
    assert_source_node_v305_shape(receipt);
    assert_summary_and_boundaries(receipt);
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(
        smoke,
        "\"credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v306_shape(smoke);
    assert_artifact_contract_fields(smoke);
    assert_source_node_v305_shape(smoke);
    assert_summary_and_boundaries(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_approval_prerequisite_artifact_intake_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.json\"");
    assert_contains(
        manifest,
        "\"credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt for Node v307");
    assert_source_node_v306_shape(manifest);
    assert_artifact_contract_fields(manifest);
    assert_source_node_v305_shape(manifest);
    assert_summary_and_boundaries(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(
        result.response,
        "\"credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt\":");
    assert_contains(result.response, "\"current_release_version\":\"v135\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/135/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v306_shape(result.response);
    assert_artifact_contract_fields(result.response);
    assert_source_node_v305_shape(result.response);
    assert_summary_and_boundaries(result.response);
    assert_contains(
        result.response,
        "\"node_consumption\":\"Node v315 may verify the mini-kv v138 credential resolver signed human approval artifact contract non-participation receipt");
    assert_contains(
        result.response,
        "Node v305 may verify the mini-kv v134 credential resolver runtime shell chain stop/prerequisite non-participation receipt");
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
