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

void assert_source_node_v281_shape(const std::string& text) {
    assert_contains(text, "\"source_review\":\"Node v281 credential resolver approval-required implementation readiness review\"");
    assert_contains(text, "\"source_verification\":\"Node v275 credential resolver approval-required boundary upstream echo verification\"");
    assert_contains(
        text,
        "\"consumer_hint\":\"Node v282 credential resolver approval-required implementation readiness upstream echo verification\"");
    assert_contains(
        text,
        "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review.v1\"");
    assert_contains(
        text,
        "\"source_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review\"");
    assert_contains(text, "\"source_review_state\":\"credential-resolver-approval-required-implementation-readiness-review-ready\"");
    assert_contains(text, "\"source_ready_for_approval_required_implementation_readiness_review\":true");
    assert_contains(text, "\"source_read_only_implementation_readiness_review\":true");
    assert_contains(text, "\"source_ready_for_java_v116_mini_kv_v122_echo\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_resolver_implementation\":false");
    assert_contains(text, "\"source_ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(text, "\"source_real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_secret_provider_instantiated\":false");
    assert_contains(text, "\"source_resolver_client_instantiated\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_approval_ledger_written\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
}

void assert_source_node_v275_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v275\":");
    assert_contains(text, "\"source_version\":\"Node v275\"");
    assert_contains(text, "\"verification_state\":\"credential-resolver-approval-required-boundary-upstream-echo-verification-ready\"");
    assert_contains(text, "\"source_span\":\"Node v274 + Java v115 + mini-kv v121\"");
    assert_contains(text, "\"source_check_count\":26");
    assert_contains(text, "\"source_passed_check_count\":26");
    assert_contains(text, "\"approval_required_boundary_count\":6");
    assert_contains(text, "\"approval_required_boundary_scope_aligned\":true");
    assert_contains(text, "\"approval_required_explanations_aligned\":true");
    assert_contains(text, "\"prohibited_runtime_actions_aligned\":true");
    assert_contains(text, "\"credential_boundary_aligned\":true");
    assert_contains(text, "\"raw_endpoint_boundary_aligned\":true");
    assert_contains(text, "\"resolver_boundary_aligned\":true");
    assert_contains(text, "\"connection_boundary_aligned\":true");
    assert_contains(text, "\"write_boundary_aligned\":true");
    assert_contains(text, "\"auto_start_boundary_aligned\":true");
}

void assert_readiness_review_and_boundaries(const std::string& text) {
    assert_contains(text, "\"readiness_review\":");
    assert_contains(text, "\"review_mode\":\"node-v281-approval-required-implementation-readiness-review-only\"");
    assert_contains(text, "\"readiness_stage\":\"pre-implementation-echo-ready\"");
    assert_contains(text, "\"implementation_stage\":\"blocked-until-java-v116-mini-kv-v122-and-node-v282\"");
    assert_contains(text, "\"all_approval_required_boundaries_echo_ready\":true");
    assert_contains(text, "\"all_approval_required_boundaries_implementation_blocked\":true");
    assert_contains(text, "\"all_required_artifacts_named\":true");
    assert_contains(text, "\"java_v116_echo_recommended\":true");
    assert_contains(text, "\"mini_kv_v122_receipt_recommended\":true");
    assert_contains(text, "\"node_v282_verification_required\":true");
    assert_contains(text, "\"route_split_quality_line_closed\":true");

    assert_contains(text, "\"boundary_readiness\":");
    assert_contains(text, "\"boundary_count\":6");
    assert_contains(text, "\"echo_ready_boundary_count\":6");
    assert_contains(text, "\"blocked_for_implementation_boundary_count\":6");
    assert_contains(text, "\"required_artifact_count\":18");
    assert_contains(
        text,
        "\"boundary_codes\":[\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\",\"OPERATOR_APPROVAL\",\"ROLLBACK_BOUNDARY\",\"SCHEMA_MIGRATION_POLICY\",\"AUDIT_LEDGER_WRITE_POLICY\"]");
    assert_contains(text, "\"credential-handle-review-id\"");
    assert_contains(text, "\"credential-value-redaction-contract\"");
    assert_contains(text, "\"operator-visible-secret-value-prohibition\"");
    assert_contains(text, "\"endpoint-handle-review-id\"");
    assert_contains(text, "\"allowlist-review-status\"");
    assert_contains(text, "\"raw-endpoint-redaction-contract\"");
    assert_contains(text, "\"operator-identity-binding\"");
    assert_contains(text, "\"approval-correlation-marker\"");
    assert_contains(text, "\"manual-window-open-marker\"");
    assert_contains(text, "\"rollback-abort-marker\"");
    assert_contains(text, "\"restore-point-review-id\"");
    assert_contains(text, "\"manual-rollback-runbook-reference\"");
    assert_contains(text, "\"schema-migration-rehearsal-id\"");
    assert_contains(text, "\"migration-review-status\"");
    assert_contains(text, "\"sql-execution-prohibition-marker\"");
    assert_contains(text, "\"approval-ledger-write-policy-id\"");
    assert_contains(text, "\"audit-store-write-prohibition-marker\"");
    assert_contains(text, "\"write-path-owner-review\"");
    assert_contains(text, "\"mini_kv_v122_receipt_hint\":\"Confirm no credential value load/store/include behavior.\"");
    assert_contains(text, "\"mini_kv_v122_receipt_hint\":\"Confirm no raw endpoint parse/include/connect behavior.\"");
    assert_contains(text, "\"mini_kv_v122_receipt_hint\":\"Confirm no auto-start and no approval side effects.\"");
    assert_contains(text, "\"mini_kv_v122_receipt_hint\":\"Confirm no LOAD/RESTORE/COMPACT and no authority over rollback state.\"");
    assert_contains(text, "\"mini_kv_v122_receipt_hint\":\"Confirm no admin command or schema/storage mutation participates.\"");
    assert_contains(text, "\"mini_kv_v122_receipt_hint\":\"Confirm no storage/backend/write participation.\"");
    assert_contains(text, "\"ready_for_runtime_implementation\":false");
}

void assert_checks_and_summary(const std::string& text) {
    assert_contains(text, "\"source_node_v275_ready\":true");
    assert_contains(text, "\"source_boundary_count_expected\":true");
    assert_contains(text, "\"source_boundary_codes_aligned\":true");
    assert_contains(text, "\"source_keeps_read_only_echo_only\":true");
    assert_contains(text, "\"source_keeps_real_resolver_blocked\":true");
    assert_contains(text, "\"source_keeps_credential_boundary_closed\":true");
    assert_contains(text, "\"source_keeps_raw_endpoint_boundary_closed\":true");
    assert_contains(text, "\"source_keeps_connection_boundary_closed\":true");
    assert_contains(text, "\"source_keeps_write_boundary_closed\":true");
    assert_contains(text, "\"source_keeps_auto_start_boundary_closed\":true");
    assert_contains(text, "\"all_boundaries_echo_ready_for_java_v116\":true");
    assert_contains(text, "\"all_boundaries_echo_ready_for_mini_kv_v122\":true");
    assert_contains(text, "\"all_boundaries_still_blocked_for_runtime_implementation\":true");
    assert_contains(text, "\"all_required_artifacts_named\":true");
    assert_contains(text, "\"upstream_probes_still_disabled\":true");
    assert_contains(text, "\"upstream_actions_still_disabled\":true");
    assert_contains(text, "\"production_audit_still_blocked\":true");
    assert_contains(text, "\"production_window_still_blocked\":true");
    assert_contains(text, "\"check_count\":21");
    assert_contains(text, "\"passed_check_count\":21");
    assert_contains(text, "\"source_check_count\":26");
    assert_contains(text, "\"source_passed_check_count\":26");
    assert_contains(text, "\"java_v116_echo_hint_count\":6");
    assert_contains(text, "\"mini_kv_v122_receipt_hint_count\":6");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"production_blocker_codes\":[]");
    assert_contains(text, "\"IMPLEMENTATION_STILL_BLOCKED\"");
    assert_contains(text, "\"ROUTE_SPLIT_QUALITY_LINE_CLOSED\"");
    assert_contains(text, "\"RUN_JAVA_V116_AND_MINI_KV_V122_IN_PARALLEL\"");
    assert_contains(text, "\"VERIFY_WITH_NODE_V282_BEFORE_IMPLEMENTATION_DRAFT\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v116\",\"mini-kv v122\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"approval_required_implementation_readiness_non_participation_receipt_only\":true");
    assert_contains(text, "\"implementation_readiness_review_only\":true");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"ready_for_java_v116_mini_kv_v122_echo\":true");
    assert_contains(text, "\"ready_for_managed_audit_resolver_implementation\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
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
    const auto receipt_path =
        std::filesystem::path{"fixtures"} / "release" /
        "credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest = minikv::runtime_evidence_receipts::
        credential_resolver_approval_required_implementation_readiness_non_participation_receipt_digest(
            read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-approval-required-implementation-readiness-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v122\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json\"");
    assert_contains(
        receipt,
        "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v122\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/122/\"");
    assert_contains(receipt, "\"previous_mini_kv_release_version\":\"v121\"");
    assert_contains(receipt, "\"previous_mini_kv_receipt_digest\":\"fnv1a64:4d759e7da4e5d5c0\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v281_shape(receipt);
    assert_source_node_v275_shape(receipt);
    assert_readiness_review_and_boundaries(receipt);
    assert_checks_and_summary(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver approval-required implementation readiness non-participation receipt only");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(
        smoke,
        "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v281_shape(smoke);
    assert_source_node_v275_shape(smoke);
    assert_readiness_review_and_boundaries(smoke);
    assert_checks_and_summary(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(
        smoke,
        "Node v282 may verify the mini-kv v122 credential resolver approval-required implementation readiness non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_credential_resolver_impl_readiness_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json\"");
    assert_contains(
        manifest,
        "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_approval_required_implementation_readiness_non_participation_receipt for Node v282");
    assert_source_node_v281_shape(manifest);
    assert_source_node_v275_shape(manifest);
    assert_readiness_review_and_boundaries(manifest);
    assert_checks_and_summary(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(
        result.response,
        "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v122\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/122/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v281_shape(result.response);
    assert_source_node_v275_shape(result.response);
    assert_readiness_review_and_boundaries(result.response);
    assert_checks_and_summary(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
