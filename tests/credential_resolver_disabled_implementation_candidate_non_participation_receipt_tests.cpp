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

void assert_source_review_shape(const std::string& text) {
    assert_contains(text, "\"source_review\":\"Node v273 credential resolver disabled implementation candidate review\"");
    assert_contains(text,
                    "\"source_node_v272\":\"Node v272 credential resolver pre-implementation plan intake upstream echo verification\"");
    assert_contains(text,
                    "\"consumer_hint\":\"Node v274 credential resolver disabled candidate upstream echo verification\"");
    assert_contains(text,
                    "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-disabled-implementation-candidate-review.v1\"");
    assert_contains(
        text,
        "\"source_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-implementation-candidate-review\"");
    assert_contains(
        text,
        "\"source_review_state\":\"credential-resolver-disabled-implementation-candidate-review-ready\"");
    assert_contains(text, "\"source_ready_for_disabled_implementation_candidate_review\":true");
    assert_contains(text, "\"source_disabled_implementation_candidate_review_only\":true");
    assert_contains(text, "\"source_read_only_candidate_review\":true");
    assert_contains(text, "\"source_ready_for_disabled_resolver_interface_candidate\":true");
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
}

void assert_source_node_v272_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v272_reference\":");
    assert_contains(text, "\"source_version\":\"Node v272\"");
    assert_contains(
        text,
        "\"profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake-upstream-echo-verification.v1\"");
    assert_contains(
        text,
        "\"verification_state\":\"credential-resolver-pre-implementation-plan-intake-upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_plan_intake_upstream_echo_verification\":true");
    assert_contains(text,
                    "\"verification_digest\":\"af7613ac0245fbc1b1ed709b5ea73d7907ab3b84918939c54f57e194446df6f1\"");
    assert_contains(text, "\"source_span\":\"Node v270 + Java v112 + mini-kv v119\"");
    assert_contains(text, "\"source_node_v270_ready\":true");
    assert_contains(text, "\"java_v112_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v119_non_participation_ready\":true");
    assert_contains(text, "\"plan_intake_state_aligned\":true");
    assert_contains(text, "\"plan_counts_aligned\":true");
    assert_contains(text, "\"boundary_codes_aligned\":true");
    assert_contains(text, "\"requirement_codes_aligned\":true");
    assert_contains(text, "\"credential_boundary_aligned\":true");
    assert_contains(text, "\"raw_endpoint_boundary_aligned\":true");
    assert_contains(text, "\"resolver_boundary_aligned\":true");
    assert_contains(text, "\"connection_boundary_aligned\":true");
    assert_contains(text, "\"write_boundary_aligned\":true");
    assert_contains(text, "\"auto_start_boundary_aligned\":true");
    assert_contains(text, "\"check_count\":22");
    assert_contains(text, "\"source_check_count\":26");
    assert_contains(text, "\"plan_version\":\"node-v270-credential-resolver-pre-implementation-plan-intake.v1\"");
    assert_contains(text, "\"plan_mode\":\"plan-intake-only\"");
    assert_contains(text, "\"plan_digest\":\"3aef2d1d10fa5a0063f9be43af49122de0b38999b0d065a8addb5c3ed1f95ad8\"");
    assert_contains(text, "\"intake_digest\":\"43c40f8aee830a93fa36845366c948abca1353555756154b8dcbb8053a17e603\"");
    assert_contains(text, "\"boundary_count\":10");
    assert_contains(text, "\"defined_boundary_count\":10");
    assert_contains(text, "\"missing_boundary_count\":0");
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
}

void assert_candidate_shape(const std::string& text) {
    assert_contains(text, "\"disabled_implementation_candidate\":");
    assert_contains(text,
                    "\"candidate_version\":\"node-v273-credential-resolver-disabled-implementation-candidate-review.v1\"");
    assert_contains(text, "\"candidate_mode\":\"disabled-interface-and-fake-wiring-review-only\"");
    assert_contains(text, "\"candidate_digest\":\"12862fa65cad3a8db92f88fba86b74c25e584f35614c7a47367d441358b8b7d7\"");
    assert_contains(text, "\"candidate_decision_count\":10");
    assert_contains(text, "\"candidate_ready_decision_count\":4");
    assert_contains(text, "\"approval_required_decision_count\":6");
    assert_contains(text,
                    "\"candidate_ready_boundary_codes\":[\"PLAN_DOCUMENT\",\"DISABLED_SECRET_PROVIDER_STUB\",\"REDACTION_POLICY\",\"EXTERNAL_REQUEST_SIMULATION\"]");
    assert_contains(
        text,
        "\"approval_required_boundary_codes\":[\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\",\"OPERATOR_APPROVAL\",\"ROLLBACK_BOUNDARY\",\"SCHEMA_MIGRATION_POLICY\",\"AUDIT_LEDGER_WRITE_POLICY\"]");
    assert_contains(text, "\"disposition\":\"disabled-candidate-ready\"");
    assert_contains(text, "\"disposition\":\"approval-required\"");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"read-credential-value\",\"store-credential-value\"]");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"parse-raw-endpoint-url\",\"render-raw-endpoint-url\"]");
    assert_contains(text, "\"prohibited_runtime_actions\":[\"send-external-request\",\"connect-managed-audit\"]");
    assert_contains(text, "\"interface_version\":\"disabled-credential-resolver-interface-candidate.v1\"");
    assert_contains(text,
                    "\"request_fields\":[\"credentialHandle\",\"endpointHandle\",\"resolverPolicyHandle\",\"operatorIdentity\",\"approvalCorrelationId\",\"manualWindowMarker\"]");
    assert_contains(
        text,
        "\"response_fields\":[\"resolverState\",\"resolvedCredentialValue\",\"rawEndpointUrl\",\"redactionApplied\",\"externalRequestSent\",\"failureClass\",\"auditDigest\"]");
    assert_contains(
        text,
        "\"failure_classes\":[\"disabled-by-config\",\"missing-credential-handle\",\"missing-endpoint-handle\",\"operator-approval-required\",\"manual-window-required\",\"real-runtime-forbidden\"]");
    assert_contains(text, "\"request_field_count\":6");
    assert_contains(text, "\"response_field_count\":7");
    assert_contains(text, "\"failure_class_count\":6");
    assert_contains(text, "\"handle_only_request\":true");
    assert_contains(text, "\"includes_credential_value\":false");
    assert_contains(text, "\"includes_raw_endpoint_url\":false");
    assert_contains(text, "\"sends_external_request\":false");
    assert_contains(text, "\"instantiates_secret_provider\":false");
    assert_contains(text, "\"instantiates_resolver_client\":false");
    assert_contains(text, "\"review_version\":\"disabled-credential-resolver-fake-wiring-review.v1\"");
    assert_contains(text, "\"fake_wiring_review_only\":true");
    assert_contains(text, "\"fake_runtime_instantiated\":false");
    assert_contains(text, "\"real_secret_provider_allowed\":false");
    assert_contains(text, "\"real_managed_audit_transport_allowed\":false");
    assert_contains(text, "\"cleanup_artifact_count\":0");
}

void assert_checks_and_summary(const std::string& text) {
    assert_contains(text, "\"source_node_v272_ready\":true");
    assert_contains(text, "\"source_node_v272_keeps_read_only_echo_only\":true");
    assert_contains(text, "\"source_node_v272_keeps_real_resolver_blocked\":true");
    assert_contains(text, "\"source_node_v272_keeps_boundary_alignment\":true");
    assert_contains(text, "\"all_candidate_decisions_covered\":true");
    assert_contains(text, "\"candidate_ready_boundaries_limited\":true");
    assert_contains(text, "\"approval_required_boundaries_preserved\":true");
    assert_contains(text, "\"interface_shape_handle_only\":true");
    assert_contains(text, "\"fake_wiring_review_only\":true");
    assert_contains(text, "\"credential_value_still_forbidden\":true");
    assert_contains(text, "\"raw_endpoint_still_forbidden\":true");
    assert_contains(text, "\"secret_provider_runtime_still_disabled\":true");
    assert_contains(text, "\"resolver_client_still_disabled\":true");
    assert_contains(text, "\"external_request_still_blocked\":true");
    assert_contains(text, "\"schema_migration_still_blocked\":true");
    assert_contains(text, "\"ledger_write_still_blocked\":true");
    assert_contains(text, "\"upstream_probes_still_disabled\":true");
    assert_contains(text, "\"upstream_actions_still_disabled\":true");
    assert_contains(text, "\"production_audit_still_blocked\":true");
    assert_contains(text, "\"production_window_still_blocked\":true");
    assert_contains(
        text,
        "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_implementation_candidate_review\":true");
    assert_contains(text, "\"check_count\":21");
    assert_contains(text, "\"passed_check_count\":21");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"production_blocker_codes\":[]");
    assert_contains(text, "\"DISABLED_CANDIDATE_REVIEW_ONLY\"");
    assert_contains(text, "\"APPROVAL_BOUNDARIES_REMAIN\"");
    assert_contains(text, "\"RUN_PARALLEL_JAVA_V113_MINI_KV_V120\"");
    assert_contains(text, "\"VERIFY_WITH_NODE_V274_AFTER_UPSTREAM_ECHO\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v113\",\"mini-kv v120\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text,
                    "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt_only\":true");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"disabled_implementation_candidate_review_only\":true");
    assert_contains(text, "\"read_only_candidate_review\":true");
    assert_contains(text, "\"ready_for_disabled_resolver_interface_candidate\":true");
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
    assert_contains(text, "\"reads_managed_audit_credential\":false");
    assert_contains(text, "\"stores_managed_audit_credential\":false");
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
                              "credential-resolver-disabled-implementation-candidate-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest = minikv::runtime_evidence_receipts::
        credential_resolver_disabled_implementation_candidate_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-disabled-implementation-candidate-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v120\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-disabled-implementation-candidate-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v120\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/120/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_review_shape(receipt);
    assert_source_node_v272_shape(receipt);
    assert_candidate_shape(receipt);
    assert_checks_and_summary(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver disabled implementation candidate non-participation receipt only");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_review_shape(smoke);
    assert_source_node_v272_shape(smoke);
    assert_candidate_shape(smoke);
    assert_checks_and_summary(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(
        smoke,
        "Node v274 may verify the mini-kv v120 credential resolver disabled implementation candidate non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(
        manifest,
        "\"minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-disabled-implementation-candidate-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_disabled_implementation_candidate_non_participation_receipt for Node v274");
    assert_source_review_shape(manifest);
    assert_source_node_v272_shape(manifest);
    assert_candidate_shape(manifest);
    assert_checks_and_summary(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-disabled-implementation-candidate-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v120\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/120/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_review_shape(result.response);
    assert_source_node_v272_shape(result.response);
    assert_candidate_shape(result.response);
    assert_checks_and_summary(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
