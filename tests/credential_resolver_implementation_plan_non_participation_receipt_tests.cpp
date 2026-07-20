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

void assert_source_node_v283_shape(const std::string& text) {
    assert_contains(text, "\"source_plan\":\"Node v283 managed audit resolver implementation plan draft\"");
    assert_contains(
        text,
        "\"source_verification\":\"Node v282 approval-required implementation readiness upstream echo verification\"");
    assert_contains(
        text, "\"consumer_hint\":\"Node v284 credential resolver implementation plan upstream echo verification\"");
    assert_contains(text, "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-"
                          "implementation-plan-draft.v1\"");
    assert_contains(text, "\"source_route_path\":\"/api/v1/audit/"
                          "managed-audit-manual-sandbox-connection-credential-resolver-implementation-plan-draft\"");
    assert_contains(text, "\"source_plan_state\":\"credential-resolver-implementation-plan-draft-ready\"");
    assert_contains(text, "\"source_ready_for_implementation_plan_draft\":true");
    assert_contains(text, "\"source_plan_draft_only\":true");
    assert_contains(text, "\"source_read_only_plan_draft\":true");
    assert_contains(text, "\"source_implementation_plan_draft_only\":true");
    assert_contains(text, "\"source_ready_for_java_v121_mini_kv_v126_echo\":true");
    assert_contains(text, "\"source_ready_for_managed_audit_resolver_implementation\":false");
    assert_contains(text, "\"source_real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"source_test_only_fake_harness_allowed\":false");
    assert_contains(text, "\"source_execution_allowed\":false");
    assert_contains(text, "\"source_connects_managed_audit\":false");
    assert_contains(text, "\"source_reads_managed_audit_credential\":false");
    assert_contains(text, "\"source_stores_managed_audit_credential\":false");
    assert_contains(text, "\"source_credential_value_read\":false");
    assert_contains(text, "\"source_raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"source_raw_endpoint_url_rendered\":false");
    assert_contains(text, "\"source_external_request_sent\":false");
    assert_contains(text, "\"source_secret_provider_instantiated\":false");
    assert_contains(text, "\"source_resolver_client_instantiated\":false");
    assert_contains(text, "\"source_schema_migration_executed\":false");
    assert_contains(text, "\"source_approval_ledger_written\":false");
    assert_contains(text, "\"source_automatic_upstream_start\":false");
}

void assert_source_node_v282_shape(const std::string& text) {
    assert_contains(text, "\"source_node_v282_reference\":");
    assert_contains(text, "\"source_version\":\"Node v282\"");
    assert_contains(text, "\"verification_state\":\"credential-resolver-approval-required-implementation-readiness-"
                          "upstream-echo-verification-ready\"");
    assert_contains(text, "\"ready_for_node_v283_implementation_plan_draft\":true");
    assert_contains(text,
                    "\"verification_digest\":\"eab97ee07d89be2a915f42cd6f340aea36b79d0ad332e777826f91e760f7a511\"");
    assert_contains(text, "\"source_span\":\"Node v281 + Java v116 + mini-kv v122\"");
    assert_contains(text, "\"java_v116_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v122_non_participation_ready\":true");
    assert_contains(text, "\"implementation_still_blocked\":true");
    assert_contains(text, "\"check_count\":23");
    assert_contains(text, "\"passed_check_count\":23");
    assert_contains(text, "\"boundary_count\":6");
    assert_contains(text, "\"required_artifact_count\":18");
}

void assert_plan_draft_shape(const std::string& text) {
    assert_contains(text, "\"implementation_plan\":");
    assert_contains(text, "\"plan_version\":\"node-v283-credential-resolver-implementation-plan-draft.v1\"");
    assert_contains(text, "\"plan_mode\":\"implementation-plan-draft-only\"");
    assert_contains(text, "\"plan_digest\":\"152d4261b3c020159da2aebc2a5ef484dcb8f5381f90bf5f29cc21deb9f80edb\"");
    assert_contains(text, "\"interface_boundary_count\":7");
    assert_contains(text, "\"required_artifact_count\":21");
    assert_contains(text, "\"prohibited_action_count\":21");
    assert_contains(text, "\"interface_boundary_codes\":[\"CONFIG_HANDLE_CONTRACT\",\"CREDENTIAL_HANDLE_CONTRACT\","
                          "\"ENDPOINT_HANDLE_CONTRACT\",\"APPROVAL_ARTIFACT_CONTRACT\",\"FAILURE_TAXONOMY_CONTRACT\","
                          "\"ROLLBACK_GUARD_CONTRACT\",\"TEST_ONLY_FAKE_HARNESS_CONTRACT\"]");
    assert_contains(text, "\"config-handle-review-id\"");
    assert_contains(text, "\"credential-handle-review-id\"");
    assert_contains(text, "\"endpoint-handle-review-id\"");
    assert_contains(text, "\"operator-identity-binding\"");
    assert_contains(text, "\"failure-taxonomy-id\"");
    assert_contains(text, "\"rollback-abort-marker\"");
    assert_contains(text, "\"test-only-fake-harness-plan-id\"");
    assert_contains(text, "\"read-secret-env-value\"");
    assert_contains(text, "\"read-credential-value\"");
    assert_contains(text, "\"parse-raw-endpoint-url\"");
    assert_contains(text, "\"write-approval-ledger\"");
    assert_contains(text, "\"send-external-request\"");
    assert_contains(text, "\"execute-rollback\"");
    assert_contains(text, "\"instantiate-real-secret-provider\"");
    assert_contains(text, "\"CONFIG_HANDLE_CONTRACT\"");
    assert_contains(text, "\"CREDENTIAL_HANDLE_CONTRACT\"");
    assert_contains(text, "\"ENDPOINT_HANDLE_CONTRACT\"");
    assert_contains(text, "\"APPROVAL_ARTIFACT_CONTRACT\"");
    assert_contains(text, "\"FAILURE_TAXONOMY_CONTRACT\"");
    assert_contains(text, "\"ROLLBACK_GUARD_CONTRACT\"");
    assert_contains(text, "\"TEST_ONLY_FAKE_HARNESS_CONTRACT\"");
    assert_contains(text, "\"mini-kv-v126-consumes-node-v283-plan\"");
    assert_contains(text, "\"mini-kv-v126-no-storage-backend\"");
    assert_contains(text, "\"mini-kv-v126-no-secret-or-endpoint\"");
    assert_contains(text, "\"mini-kv-v126-no-write-command\"");
    assert_contains(text, "\"java-v121-consumes-node-v283-plan\"");
}

void assert_review_checks_and_summary(const std::string& text) {
    assert_contains(text, "\"implementation_plan_review\":");
    assert_contains(text, "\"review_mode\":\"node-v283-implementation-plan-draft-only\"");
    assert_contains(text, "\"consumed_node_version\":\"Node v282\"");
    assert_contains(text, "\"next_java_echo_version\":\"Java v121\"");
    assert_contains(text, "\"next_mini_kv_receipt_version\":\"mini-kv v126\"");
    assert_contains(text, "\"next_node_verification_version\":\"Node v284\"");
    assert_contains(text, "\"fake_harness_deferred_until\":\"Node v285\"");
    assert_contains(text, "\"review_digest\":\"f96ffbad4574e400216b0c6615f4c37fe979f9ede9797a039a5e55888d097a55\"");
    assert_contains(text, "\"source_node_v282_ready\":true");
    assert_contains(text, "\"source_node_v282_allows_plan_draft\":true");
    assert_contains(text, "\"source_node_v282_keeps_implementation_blocked\":true");
    assert_contains(text, "\"all_interface_boundaries_defined\":true");
    assert_contains(text, "\"all_required_artifacts_named\":true");
    assert_contains(text, "\"mini_kv_v126_receipt_requirements_defined\":true");
    assert_contains(text, "\"real_resolver_implementation_still_blocked\":true");
    assert_contains(text, "\"test_only_fake_harness_still_deferred\":true");
    assert_contains(text, "\"credential_value_still_forbidden\":true");
    assert_contains(text, "\"raw_endpoint_still_forbidden\":true");
    assert_contains(text, "\"external_request_still_forbidden\":true");
    assert_contains(text, "\"secret_provider_runtime_still_disabled\":true");
    assert_contains(text, "\"resolver_client_still_disabled\":true");
    assert_contains(text, "\"schema_migration_still_review_only\":true");
    assert_contains(text, "\"approval_ledger_write_still_review_only\":true");
    assert_contains(text, "\"check_count\":28");
    assert_contains(text, "\"passed_check_count\":28");
    assert_contains(text, "\"source_check_count\":23");
    assert_contains(text, "\"source_passed_check_count\":23");
    assert_contains(text, "\"java_echo_requirement_count\":4");
    assert_contains(text, "\"mini_kv_receipt_requirement_count\":4");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":2");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"production_blocker_codes\":[]");
    assert_contains(text, "\"IMPLEMENTATION_STILL_BLOCKED\"");
    assert_contains(text, "\"UPSTREAM_ECHO_REQUIRED\"");
    assert_contains(text, "\"RUN_PARALLEL_JAVA_V121_MINI_KV_V126\"");
    assert_contains(text, "\"VERIFY_WITH_NODE_V284_BEFORE_FAKE_HARNESS\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v121\",\"mini-kv v126\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"credential_resolver_implementation_plan_non_participation_receipt_only\":true");
    assert_contains(text, "\"implementation_plan_draft_only\":true");
    assert_contains(text, "\"read_only_plan_draft\":true");
    assert_contains(text, "\"consumes_node_v283_plan\":true");
    assert_contains(text, "\"ready_for_node_v284_upstream_echo_verification\":true");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"ready_for_managed_audit_resolver_implementation\":false");
    assert_contains(text, "\"real_resolver_implementation_allowed\":false");
    assert_contains(text, "\"test_only_fake_harness_allowed\":false");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"credential_value_read\":false");
    assert_contains(text, "\"credential_value_loaded\":false");
    assert_contains(text, "\"credential_value_stored\":false");
    assert_contains(text, "\"credential_value_included\":false");
    assert_contains(text, "\"raw_endpoint_url_parse_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_render_allowed\":false");
    assert_contains(text, "\"raw_endpoint_url_parsed\":false");
    assert_contains(text, "\"raw_endpoint_url_rendered\":false");
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
                              "credential-resolver-implementation-plan-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest =
        minikv::runtime_evidence_receipts::credential_resolver_implementation_plan_non_participation_receipt_digest(
            read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-implementation-plan-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v126\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-implementation-plan-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_implementation_plan_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v126\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"d/126/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_node_v283_shape(receipt);
    assert_source_node_v282_shape(receipt);
    assert_plan_draft_shape(receipt);
    assert_review_checks_and_summary(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver implementation plan non-participation receipt only");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"credential_resolver_implementation_plan_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v283_shape(smoke);
    assert_source_node_v282_shape(smoke);
    assert_plan_draft_shape(smoke);
    assert_review_checks_and_summary(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(
        smoke,
        "Node v284 may verify the mini-kv v126 credential resolver implementation plan non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_credential_resolver_implementation_plan_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-implementation-plan-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_implementation_plan_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest, "SMOKEJSON exposes credential_resolver_implementation_plan_non_participation_receipt for Node v284");
    assert_source_node_v283_shape(manifest);
    assert_source_node_v282_shape(manifest);
    assert_plan_draft_shape(manifest);
    assert_review_checks_and_summary(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_implementation_plan_non_participation_receipt\":");
    assert_contains(result.response, "\"receipt_fixture_path\":\"fixtures/release/"
                                     "credential-resolver-implementation-plan-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v126\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/126/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_node_v283_shape(result.response);
    assert_source_node_v282_shape(result.response);
    assert_plan_draft_shape(result.response);
    assert_review_checks_and_summary(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
    return 0;
}
