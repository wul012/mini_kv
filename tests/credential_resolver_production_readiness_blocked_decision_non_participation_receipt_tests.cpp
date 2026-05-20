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

void assert_source_decision_gate_shape(const std::string& text) {
    assert_contains(text, "\"source_decision_gate\":\"Node v268 credential resolver production readiness decision gate\"");
    assert_contains(text, "\"source_upstream_echo\":\"Node v267 credential resolver fake-shell archive upstream echo verification\"");
    assert_contains(text, "\"consumer_hint\":\"Node v269 credential resolver production-readiness blocked-decision upstream echo verification\"");
    assert_contains(text, "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-decision-gate.v1\"");
    assert_contains(text, "\"source_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-decision-gate\"");
    assert_contains(text, "\"source_decision_gate_state\":\"blocked\"");
    assert_contains(text, "\"source_readiness_decision\":\"blocked\"");
    assert_contains(text, "\"source_decision_gate_evaluated\":true");
    assert_contains(text, "\"source_production_readiness_gate_only\":true");
    assert_contains(text, "\"source_read_only_decision_gate\":true");
    assert_contains(text, "\"source_ready_for_credential_resolver_pre_implementation_plan\":false");
    assert_contains(text, "\"source_real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_stores_managed_audit_credential\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_secret_provider_instantiated\":false");
    assert_contains(text, "\"source_resolver_client_instantiated\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
}

void assert_source_node_v267_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v267\":");
    assert_contains(text, "\"source_version\":\"Node v267\"");
    assert_contains(text, "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-upstream-echo-verification.v1\"");
    assert_contains(text, "\"verification_state\":\"credential-resolver-fake-shell-archive-upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_upstream_echo_verification\":true");
    assert_contains(text, "\"verification_digest\":\"cfd4ff6b893469476d122647f082f590ae381407d697f40f7378a0b4b31e7a39\"");
    assert_contains(text, "\"source_span\":\"Node v266 + Java v110 + mini-kv v117\"");
    assert_contains(text, "\"source_node_v266_ready\":true");
    assert_contains(text, "\"java_v110_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v117_non_participation_ready\":true");
    assert_contains(text, "\"archive_counts_aligned\":true");
    assert_contains(text, "\"archive_snippets_aligned\":true");
    assert_contains(text, "\"archive_no_rerun_aligned\":true");
    assert_contains(text, "\"credential_boundary_aligned\":true");
    assert_contains(text, "\"raw_endpoint_boundary_aligned\":true");
    assert_contains(text, "\"connection_boundary_aligned\":true");
    assert_contains(text, "\"write_boundary_aligned\":true");
    assert_contains(text, "\"auto_start_boundary_aligned\":true");
    assert_contains(text, "\"node_v267_blocks_real_resolver\":true");
    assert_contains(text, "\"check_count\":18");
    assert_contains(text, "\"passed_check_count\":18");
    assert_contains(text, "\"archive_file_count\":9");
    assert_contains(text, "\"evidence_file_count\":7");
    assert_contains(text, "\"required_snippet_count\":24");
    assert_contains(text, "\"matched_snippet_count\":32");
    assert_contains(text, "\"production_blocker_count\":0");
}

void assert_blocked_decision_shape(const std::string& text) {
    assert_contains(text, "\"pre_implementation_requirements\":");
    assert_contains(text, "\"plan_document_present\":false");
    assert_contains(text, "\"credential_handle_boundary_defined\":false");
    assert_contains(text, "\"endpoint_handle_boundary_defined\":false");
    assert_contains(text, "\"secret_provider_stub_defined\":false");
    assert_contains(text, "\"operator_approval_boundary_defined\":false");
    assert_contains(text, "\"rollback_boundary_defined\":false");
    assert_contains(text, "\"redaction_policy_defined\":false");
    assert_contains(text, "\"external_request_simulation_defined\":false");
    assert_contains(text, "\"schema_migration_policy_defined\":false");
    assert_contains(text, "\"audit_ledger_write_policy_defined\":false");
    assert_contains(text, "\"missing_requirement_count\":10");
    assert_contains(text, "\"production_readiness_decision\":");
    assert_contains(text, "\"decision_digest\":\"2c35360789f4f6755fa66360a669968accfa7135a1f645287203801cd04de702\"");
    assert_contains(text, "\"decision_mode\":\"node-v268-production-readiness-decision-gate-only\"");
    assert_contains(text, "\"decision\":\"blocked\"");
    assert_contains(text, "\"allows_real_resolver_pre_implementation_plan\":false");
    assert_contains(text, "\"allows_real_credential_resolver_implementation\":false");
    assert_contains(text, "\"allows_secret_provider_stub\":false");
    assert_contains(text, "\"allows_secret_provider_runtime\":false");
    assert_contains(text, "\"allows_credential_value_read\":false");
    assert_contains(text, "\"allows_raw_endpoint_url_parse\":false");
    assert_contains(text, "\"allows_external_request\":false");
    assert_contains(text, "\"allows_managed_audit_connection\":false");
    assert_contains(text, "\"allows_schema_migration\":false");
    assert_contains(text, "\"allows_approval_ledger_write\":false");
    assert_contains(text, "\"allows_automatic_upstream_start\":false");
    assert_contains(text, "\"next_plan_required_before_implementation\":true");
}

void assert_checks_summary_and_blockers(const std::string& text) {
    assert_contains(text, "\"decision_gate_evaluated\":true");
    assert_contains(text, "\"source_node_v267_ready\":true");
    assert_contains(text, "\"source_node_v267_blocks_real_resolver\":true");
    assert_contains(text, "\"archive_echo_chain_ready\":true");
    assert_contains(text, "\"credential_boundary_still_closed\":true");
    assert_contains(text, "\"raw_endpoint_boundary_still_closed\":true");
    assert_contains(text, "\"resolver_boundary_still_closed\":true");
    assert_contains(text, "\"connection_boundary_still_closed\":true");
    assert_contains(text, "\"write_boundary_still_closed\":true");
    assert_contains(text, "\"auto_start_boundary_still_closed\":true");
    assert_contains(text, "\"pre_implementation_plan_present\":false");
    assert_contains(text, "\"production_audit_still_blocked\":true");
    assert_contains(text, "\"production_window_still_blocked\":true");
    assert_contains(text, "\"real_resolver_implementation_still_blocked\":true");
    assert_contains(text, "\"check_count\":25");
    assert_contains(text, "\"passed_check_count\":15");
    assert_contains(text, "\"missing_pre_implementation_requirement_count\":10");
    assert_contains(text, "\"production_blocker_count\":10");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"REAL_RESOLVER_PRE_IMPLEMENTATION_PLAN_MISSING\"");
    assert_contains(text, "\"CREDENTIAL_HANDLE_BOUNDARY_MISSING\"");
    assert_contains(text, "\"ENDPOINT_HANDLE_BOUNDARY_MISSING\"");
    assert_contains(text, "\"SECRET_PROVIDER_STUB_MISSING\"");
    assert_contains(text, "\"OPERATOR_APPROVAL_BOUNDARY_MISSING\"");
    assert_contains(text, "\"ROLLBACK_BOUNDARY_MISSING\"");
    assert_contains(text, "\"REDACTION_POLICY_MISSING\"");
    assert_contains(text, "\"EXTERNAL_REQUEST_SIMULATION_PLAN_MISSING\"");
    assert_contains(text, "\"SCHEMA_MIGRATION_POLICY_MISSING\"");
    assert_contains(text, "\"AUDIT_LEDGER_WRITE_POLICY_MISSING\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v111\",\"mini-kv v118\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt_only\":true");
    assert_contains(text, "\"blocked_decision_only\":true");
    assert_contains(text, "\"production_readiness_gate_only\":true");
    assert_contains(text, "\"read_only_decision_gate\":true");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"ready_for_credential_resolver_pre_implementation_plan\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"secret_provider_stub_defined\":false");
    assert_contains(text, "\"secret_provider_runtime_allowed\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"connection_execution_allowed\":false");
    assert_contains(text, "\"write_commands_executed\":false");
    assert_contains(text, "\"admin_commands_executed\":false");
    assert_contains(text, "\"runtime_write_observed\":false");
    assert_contains(text, "\"managed_audit_storage_backend\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"managed_audit_write_executed\":false");
    assert_contains(text, "\"approval_ledger_write_allowed\":false");
    assert_contains(text, "\"approval_ledger_write_executed\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"credential_value_stored\":false");
    assert_contains(text, "\"credential_value_included\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"raw_endpoint_url_included\":false");
    assert_contains(text, "\"external_request_sent\":false");
    assert_contains(text, "\"schema_migration_execution_allowed\":false");
    assert_contains(text, "\"restore_execution_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest = minikv::runtime_evidence_receipts::
        credential_resolver_production_readiness_blocked_decision_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-production-readiness-blocked-decision-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v118\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json\"");
    assert_contains(receipt,
                    "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v118\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/118/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_decision_gate_shape(receipt);
    assert_source_node_v267_shape(receipt);
    assert_blocked_decision_shape(receipt);
    assert_checks_summary_and_blockers(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver production-readiness blocked-decision non-participation receipt only");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke,
                    "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_decision_gate_shape(smoke);
    assert_blocked_decision_shape(smoke);
    assert_checks_summary_and_blockers(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke,
                    "Node v269 may verify the mini-kv v118 credential resolver production-readiness blocked-decision non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json\"");
    assert_contains(manifest,
                    "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_production_readiness_blocked_decision_non_participation_receipt for Node v269");
    assert_source_decision_gate_shape(manifest);
    assert_checks_summary_and_blockers(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(
        result.response,
        "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v118\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/118/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_decision_gate_shape(result.response);
    assert_source_node_v267_shape(result.response);
    assert_blocked_decision_shape(result.response);
    assert_checks_summary_and_blockers(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
