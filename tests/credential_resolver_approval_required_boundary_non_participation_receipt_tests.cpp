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

void assert_source_node_v274_shape(const std::string& text) {
    assert_contains(text, "\"source_verification\":\"Node v274 credential resolver disabled candidate upstream echo verification\"");
    assert_contains(text, "\"source_review\":\"Node v273 credential resolver disabled implementation candidate review\"");
    assert_contains(text,
                    "\"consumer_hint\":\"Node v275 credential resolver approval-required boundary upstream echo verification\"");
    assert_contains(
        text,
        "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-candidate-upstream-echo-verification.v1\"");
    assert_contains(
        text,
        "\"source_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-candidate-upstream-echo-verification\"");
    assert_contains(text, "\"source_verification_state\":\"credential-resolver-disabled-candidate-upstream-echo-verification-ready\"");
    assert_contains(text, "\"source_ready_for_disabled_candidate_upstream_echo_verification\":true");
    assert_contains(text, "\"source_read_only_upstream_echo_verification\":true");
    assert_contains(text, "\"source_disabled_candidate_echo_verification_only\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_secret_provider_instantiated\":false");
    assert_contains(text, "\"source_resolver_client_instantiated\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_approval_ledger_written\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
    assert_contains(text, "\"source_node_v274_verification\":");
    assert_contains(text, "\"check_count\":25");
    assert_contains(text, "\"passed_check_count\":25");
    assert_contains(text, "\"candidate_decision_count\":10");
    assert_contains(text, "\"candidate_ready_decision_count\":4");
    assert_contains(text, "\"approval_required_decision_count\":6");
    assert_contains(text, "\"request_field_count\":6");
    assert_contains(text, "\"response_field_count\":7");
    assert_contains(text, "\"failure_class_count\":6");
    assert_contains(text, "\"evidence_file_count\":12");
    assert_contains(text, "\"candidate_digest\":\"12862fa65cad3a8db92f88fba86b74c25e584f35614c7a47367d441358b8b7d7\"");
    assert_contains(text, "\"java_v113_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v120_non_participation_ready\":true");
    assert_contains(text, "\"java_echo_workflow_template_applied\":true");
}

void assert_boundary_details(const std::string& text) {
    assert_contains(text, "\"approval_required_boundaries\":");
    assert_contains(text, "\"boundary_count\":6");
    assert_contains(
        text,
        "\"boundary_codes\":[\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\",\"OPERATOR_APPROVAL\",\"ROLLBACK_BOUNDARY\",\"SCHEMA_MIGRATION_POLICY\",\"AUDIT_LEDGER_WRITE_POLICY\"]");
    assert_contains(text, "\"code\":\"CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"read-credential-value\",\"store-credential-value\",\"render-credential-value\"]");
    assert_contains(text, "\"code\":\"ENDPOINT_HANDLE\"");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"parse-raw-endpoint-url\",\"render-raw-endpoint-url\",\"connect-managed-audit\"]");
    assert_contains(text, "\"code\":\"OPERATOR_APPROVAL\"");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"execute-without-operator-marker\",\"auto-approve-operation\",\"auto-start-upstream\"]");
    assert_contains(text, "\"code\":\"ROLLBACK_BOUNDARY\"");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"execute-rollback\",\"deploy-resolver-without-abort-marker\",\"write-production-record\"]");
    assert_contains(text, "\"code\":\"SCHEMA_MIGRATION_POLICY\"");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"execute-schema-migration\",\"execute-sql\",\"mutate-managed-audit-schema\"]");
    assert_contains(text, "\"code\":\"AUDIT_LEDGER_WRITE_POLICY\"");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"write-approval-ledger\",\"write-managed-audit-state\",\"write-storage\"]");
    assert_contains(text, "\"approval_required\":true");
    assert_contains(text, "\"mini_kv_participates\":false");
}

void assert_checks_and_summary(const std::string& text) {
    assert_contains(text, "\"source_node_v274_ready\":true");
    assert_contains(text, "\"source_node_v274_read_only\":true");
    assert_contains(text, "\"approval_required_boundary_count_preserved\":true");
    assert_contains(text, "\"credential_handle_boundary_stays_approval_required\":true");
    assert_contains(text, "\"endpoint_handle_boundary_stays_approval_required\":true");
    assert_contains(text, "\"operator_approval_boundary_stays_approval_required\":true");
    assert_contains(text, "\"rollback_boundary_stays_approval_required\":true");
    assert_contains(text, "\"schema_migration_policy_stays_approval_required\":true");
    assert_contains(text, "\"audit_ledger_write_policy_stays_approval_required\":true");
    assert_contains(text, "\"credential_value_still_forbidden\":true");
    assert_contains(text, "\"raw_endpoint_still_forbidden\":true");
    assert_contains(text, "\"resolver_client_still_disabled\":true");
    assert_contains(text, "\"secret_provider_still_disabled\":true");
    assert_contains(text, "\"external_request_still_blocked\":true");
    assert_contains(text, "\"managed_audit_connection_still_blocked\":true");
    assert_contains(text, "\"storage_write_still_blocked\":true");
    assert_contains(text, "\"schema_migration_still_blocked\":true");
    assert_contains(text, "\"approval_ledger_write_still_blocked\":true");
    assert_contains(text, "\"load_compact_restore_setnxex_still_blocked\":true");
    assert_contains(text, "\"auto_start_still_blocked\":true");
    assert_contains(text, "\"order_authority_still_false\":true");
    assert_contains(text, "\"ready_for_java_v115_parallel_echo\":true");
    assert_contains(text, "\"ready_for_node_v275_upstream_echo\":true");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"production_blocker_codes\":[]");
    assert_contains(text, "\"APPROVAL_REQUIRED_BOUNDARY_REFINEMENT_ONLY\"");
    assert_contains(text, "\"REAL_RESOLVER_STILL_BLOCKED\"");
    assert_contains(text, "\"RUN_PARALLEL_JAVA_V115_MINI_KV_V121\"");
    assert_contains(text, "\"VERIFY_WITH_NODE_V275_AFTER_UPSTREAM_ECHO\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v115\",\"mini-kv v121\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"approval_required_boundary_non_participation_receipt_only\":true");
    assert_contains(text, "\"approval_required_boundary_refinement_only\":true");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"ready_for_approval_required_boundary_upstream_echo\":true");
    assert_contains(text, "\"ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"secret_provider_runtime_allowed\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"credential_value_stored\":false");
    assert_contains(text, "\"credential_value_included\":false");
    assert_contains(text, "\"raw_endpoint_url_parse_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"raw_endpoint_url_included\":false");
    assert_contains(text, "\"external_request_allowed\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"connects_managed_audit\":false");
    assert_contains(text, "\"managed_audit_storage_backend\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_written\":false");
    assert_contains(text, "\"schema_migration_allowed\":false");
    assert_contains(text, "\"schema_migration_executed\":false");
    assert_contains(text, "\"restore_execution_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"automatic_upstream_start_allowed\":false");
    assert_contains(text, "\"audit_authoritative\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "credential-resolver-approval-required-boundary-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest = minikv::runtime_evidence_receipts::
        credential_resolver_approval_required_boundary_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt,
                    "\"receipt_version\":\"mini-kv-credential-resolver-approval-required-boundary-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v121\"");
    assert_contains(receipt,
                    "\"path\":\"fixtures/release/credential-resolver-approval-required-boundary-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_approval_required_boundary_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v121\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/121/\"");
    assert_contains(receipt, "\"previous_mini_kv_release_version\":\"v120\"");
    assert_contains(receipt, "\"previous_mini_kv_receipt_digest\":\"fnv1a64:0f96618095243829\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v274_shape(receipt);
    assert_boundary_details(receipt);
    assert_checks_and_summary(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver approval-required boundary non-participation receipt only");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_approval_required_boundary_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v274_shape(smoke);
    assert_boundary_details(smoke);
    assert_checks_and_summary(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(
        smoke,
        "Node v275 may verify the mini-kv v121 credential resolver approval-required boundary non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_approval_required_boundary_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-approval-required-boundary-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_approval_required_boundary_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_approval_required_boundary_non_participation_receipt for Node v275");
    assert_source_node_v274_shape(manifest);
    assert_boundary_details(manifest);
    assert_checks_and_summary(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_approval_required_boundary_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-approval-required-boundary-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v121\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/121/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v274_shape(result.response);
    assert_boundary_details(result.response);
    assert_checks_and_summary(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
