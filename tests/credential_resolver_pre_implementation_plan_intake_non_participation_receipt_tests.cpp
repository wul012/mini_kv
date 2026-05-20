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

void assert_source_plan_shape(const std::string& text) {
    assert_contains(text, "\"source_plan_intake\":\"Node v270 credential resolver pre-implementation plan intake\"");
    assert_contains(
        text,
        "\"source_node_v269\":\"Node v269 credential resolver production-readiness blocked-decision upstream echo verification\"");
    assert_contains(
        text,
        "\"consumer_hint\":\"Node v272 credential resolver pre-implementation intake upstream echo verification\"");
    assert_contains(text,
                    "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake.v1\"");
    assert_contains(
        text,
        "\"source_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake\"");
    assert_contains(text,
                    "\"source_plan_intake_state\":\"credential-resolver-pre-implementation-plan-intake-ready\"");
    assert_contains(text, "\"source_ready_for_plan_intake\":true");
    assert_contains(text, "\"source_plan_intake_only\":true");
    assert_contains(text, "\"source_read_only_plan_intake\":true");
    assert_contains(text, "\"source_ready_for_credential_resolver_pre_implementation_plan\":true");
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

void assert_source_node_v269_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v269_reference\":");
    assert_contains(text, "\"source_version\":\"Node v269\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-production-readiness-blocked-decision-upstream-echo-verification.v1\"");
    assert_contains(
        text,
        "\"verification_state\":\"credential-resolver-production-readiness-blocked-decision-upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_blocked_decision_upstream_echo_verification\":true");
    assert_contains(text,
                    "\"verification_digest\":\"6acb216ec39c0abb11b880ee9f968f85cec25faf35c5b13f3fc4b460aee489d0\"");
    assert_contains(text, "\"source_span\":\"Node v268 + Java v111 + mini-kv v118\"");
    assert_contains(text, "\"source_node_v268_ready\":true");
    assert_contains(text, "\"java_v111_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v118_non_participation_ready\":true");
    assert_contains(text, "\"blocked_decision_aligned\":true");
    assert_contains(text, "\"missing_requirement_blockers_aligned\":true");
    assert_contains(text, "\"read_only_decision_gate_aligned\":true");
    assert_contains(text, "\"credential_boundary_aligned\":true");
    assert_contains(text, "\"raw_endpoint_boundary_aligned\":true");
    assert_contains(text, "\"resolver_boundary_aligned\":true");
    assert_contains(text, "\"connection_boundary_aligned\":true");
    assert_contains(text, "\"write_boundary_aligned\":true");
    assert_contains(text, "\"auto_start_boundary_aligned\":true");
    assert_contains(text, "\"check_count\":22");
    assert_contains(text, "\"passed_check_count\":22");
    assert_contains(text, "\"source_check_count\":25");
    assert_contains(text, "\"source_passed_check_count\":15");
    assert_contains(text, "\"missing_pre_implementation_requirement_count\":10");
    assert_contains(text, "\"ready_for_credential_resolver_pre_implementation_plan\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
}

void assert_plan_intake_shape(const std::string& text) {
    assert_contains(text, "\"pre_implementation_plan\":");
    assert_contains(text, "\"plan_version\":\"node-v270-credential-resolver-pre-implementation-plan-intake.v1\"");
    assert_contains(text, "\"plan_mode\":\"plan-intake-only\"");
    assert_contains(text, "\"plan_digest\":\"3aef2d1d10fa5a0063f9be43af49122de0b38999b0d065a8addb5c3ed1f95ad8\"");
    assert_contains(text, "\"boundary_count\":10");
    assert_contains(text, "\"defined_boundary_count\":10");
    assert_contains(text, "\"all_required_boundaries_defined\":true");
    assert_contains(text, "\"boundary_status\":\"defined-for-review\"");
    assert_contains(text, "\"PLAN_DOCUMENT\"");
    assert_contains(text, "\"CREDENTIAL_HANDLE\"");
    assert_contains(text, "\"ENDPOINT_HANDLE\"");
    assert_contains(text, "\"DISABLED_SECRET_PROVIDER_STUB\"");
    assert_contains(text, "\"OPERATOR_APPROVAL\"");
    assert_contains(text, "\"ROLLBACK_BOUNDARY\"");
    assert_contains(text, "\"REDACTION_POLICY\"");
    assert_contains(text, "\"EXTERNAL_REQUEST_SIMULATION\"");
    assert_contains(text, "\"SCHEMA_MIGRATION_POLICY\"");
    assert_contains(text, "\"AUDIT_LEDGER_WRITE_POLICY\"");
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
    assert_contains(text, "\"plan_intake\":");
    assert_contains(text, "\"intake_mode\":\"node-v270-plan-intake-only\"");
    assert_contains(text, "\"intake_digest\":\"43c40f8aee830a93fa36845366c948abca1353555756154b8dcbb8053a17e603\"");
    assert_contains(text, "\"required_boundary_count\":10");
    assert_contains(text, "\"missing_boundary_count\":0");
    assert_contains(text, "\"plan_document_present\":true");
    assert_contains(text, "\"credential_handle_boundary_defined\":true");
    assert_contains(text, "\"endpoint_handle_boundary_defined\":true");
    assert_contains(text, "\"secret_provider_stub_defined\":true");
    assert_contains(text, "\"operator_approval_boundary_defined\":true");
    assert_contains(text, "\"rollback_boundary_defined\":true");
    assert_contains(text, "\"redaction_policy_defined\":true");
    assert_contains(text, "\"external_request_simulation_defined\":true");
    assert_contains(text, "\"schema_migration_policy_defined\":true");
    assert_contains(text, "\"audit_ledger_write_policy_defined\":true");
    assert_contains(text, "\"next_java_echo_version\":\"Java v112\"");
    assert_contains(text, "\"next_mini_kv_receipt_version\":\"mini-kv v119\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v272\"");
}

void assert_checks_and_summary(const std::string& text) {
    assert_contains(text, "\"source_node_v269_ready\":true");
    assert_contains(text, "\"source_node_v269_keeps_blocked_decision\":true");
    assert_contains(text, "\"source_node_v269_keeps_real_resolver_blocked\":true");
    assert_contains(text, "\"all_ten_boundaries_defined\":true");
    assert_contains(text, "\"credential_value_still_forbidden\":true");
    assert_contains(text, "\"raw_endpoint_still_forbidden\":true");
    assert_contains(text, "\"secret_provider_runtime_still_disabled\":true");
    assert_contains(text, "\"real_resolver_client_still_disabled\":true");
    assert_contains(text, "\"external_request_still_simulation_only\":true");
    assert_contains(text, "\"schema_migration_still_review_only\":true");
    assert_contains(text, "\"audit_ledger_write_still_review_only\":true");
    assert_contains(text, "\"upstream_probes_still_disabled\":true");
    assert_contains(text, "\"upstream_actions_still_disabled\":true");
    assert_contains(text, "\"production_audit_still_blocked\":true");
    assert_contains(text, "\"production_window_still_blocked\":true");
    assert_contains(text, "\"check_count\":26");
    assert_contains(text, "\"passed_check_count\":26");
    assert_contains(text, "\"prohibited_action_count\":21");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"production_blocker_codes\":[]");
    assert_contains(text, "\"PLAN_INTAKE_ONLY\"");
    assert_contains(text, "\"UPSTREAM_ECHO_REQUIRED_BEFORE_V272\"");
    assert_contains(text, "\"RUN_V271_STATUS_ROUTES_QUALITY_BRANCH\"");
    assert_contains(text, "\"RUN_PARALLEL_JAVA_V112_MINI_KV_V119\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v112\",\"mini-kv v119\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt_only\":true");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"plan_intake_only\":true");
    assert_contains(text, "\"read_only_plan_intake\":true");
    assert_contains(text, "\"ready_for_credential_resolver_pre_implementation_plan\":true");
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
                              "credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest = minikv::runtime_evidence_receipts::
        credential_resolver_pre_implementation_plan_intake_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-pre-implementation-plan-intake-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v119\"");
    assert_contains(receipt,
                    "\"path\":\"fixtures/release/credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v119\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/119/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_plan_shape(receipt);
    assert_source_node_v269_shape(receipt);
    assert_plan_intake_shape(receipt);
    assert_checks_and_summary(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver pre-implementation plan intake non-participation receipt only");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_plan_shape(smoke);
    assert_source_node_v269_shape(smoke);
    assert_plan_intake_shape(smoke);
    assert_checks_and_summary(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke,
                    "Node v272 may verify the mini-kv v119 credential resolver pre-implementation plan intake non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_pre_implementation_plan_intake_non_participation_receipt for Node v272");
    assert_source_plan_shape(manifest);
    assert_plan_intake_shape(manifest);
    assert_checks_and_summary(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response,
                    "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v119\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/119/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_plan_shape(result.response);
    assert_source_node_v269_shape(result.response);
    assert_plan_intake_shape(result.response);
    assert_checks_and_summary(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
