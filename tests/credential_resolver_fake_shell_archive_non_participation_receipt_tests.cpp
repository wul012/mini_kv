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

void assert_source_archive_shape(const std::string& text) {
    assert_contains(text, "\"source_archive_verification\":\"Node v266 credential resolver fake-shell archive verification\"");
    assert_contains(text, "\"source_contract\":\"Node v264 credential resolver test-only shell contract\"");
    assert_contains(text, "\"source_upstream_echo\":\"Node v265 test-only resolver shell upstream echo verification\"");
    assert_contains(text, "\"consumer_hint\":\"Node v267 credential resolver fake-shell archive upstream echo verification\"");
    assert_contains(text, "\"source_archive_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-verification.v1\"");
    assert_contains(text, "\"source_archive_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-verification\"");
    assert_contains(text, "\"source_archive_verification_state\":\"credential-resolver-fake-shell-archive-verification-ready\"");
    assert_contains(text, "\"source_ready_for_credential_resolver_fake_shell_archive_verification\":true");
    assert_contains(text, "\"source_read_only_archive_verification\":true");
    assert_contains(text, "\"source_archive_verification_reruns_fake_shell_behavior\":false");
    assert_contains(text, "\"source_archive_verification_digest\":\"e58e721868c6e2779b78e0e1219c2a5a82c1064660523ed428b8eff7e6123f97\"");
    assert_contains(text, "\"source_evidence_span\":\"Node v264 credential resolver fake shell contract + Node v265 upstream echo archive\"");
    assert_contains(text, "\"source_node_v264_contract_digest\":\"12d264c30d3d9a83ab90ac8b0ca6ccfccbe4ff36f5216bb53d13ec42bcccf14b\"");
    assert_contains(text, "\"source_node_v265_verification_digest\":\"2c15bc316448bf31a7d67633931bff291035cfdb473f11b87e94b610113aa28e\"");
}

void assert_source_node_summaries(const std::string& text) {
    assert_contains(text, "\"source_node_v264\":");
    assert_contains(text, "\"shell_contract_state\":\"sandbox-endpoint-credential-resolver-test-only-shell-contract-ready\"");
    assert_contains(text, "\"shell_mode\":\"test-only-fake-resolver-contract\"");
    assert_contains(text, "\"resolver_kind\":\"fake-in-memory\"");
    assert_contains(text, "\"request_shape_field_count\":9");
    assert_contains(text, "\"response_shape_field_count\":13");
    assert_contains(text, "\"failure_mapping_count\":7");
    assert_contains(text, "\"guard_condition_count\":10");
    assert_contains(text, "\"fake_resolver_only\":true");
    assert_contains(text, "\"handle_only_request\":true");
    assert_contains(text, "\"source_node_v265\":");
    assert_contains(text, "\"verification_state\":\"sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification-ready\"");
    assert_contains(text, "\"verification_mode\":\"java-v107-plus-mini-kv-v116-test-only-resolver-shell-upstream-echo-verification-only\"");
    assert_contains(text, "\"source_span\":\"Node v264 + Java v107 + mini-kv v116\"");
    assert_contains(text, "\"source_node_v264_ready\":true");
    assert_contains(text, "\"java_v107_echo_ready\":true");
    assert_contains(text, "\"mini_kv_v116_non_participation_ready\":true");
    assert_contains(text, "\"java_v109_optimization_context_ready\":true");
    assert_contains(text, "\"check_count\":20");
    assert_contains(text, "\"passed_check_count\":20");
    assert_contains(text, "\"credential_boundary_aligned\":true");
    assert_contains(text, "\"raw_endpoint_boundary_aligned\":true");
    assert_contains(text, "\"connection_boundary_aligned\":true");
    assert_contains(text, "\"write_boundary_aligned\":true");
    assert_contains(text, "\"auto_start_boundary_aligned\":true");
}

void assert_archive_evidence(const std::string& text) {
    assert_contains(text, "\"archive_roots\":[\"c/264/\",\"c/265/\"]");
    assert_contains(text, "\"source_versions\":[\"Node v264\",\"Node v265\"]");
    assert_contains(text, "\"archive_file_count\":9");
    assert_contains(text, "\"required_snippet_count\":24");
    assert_contains(text, "\"matched_snippet_count\":24");
    assert_contains(text, "\"id\":\"v264-html-archive\"");
    assert_contains(text, "\"digest\":\"dff3ed1303149bc5aa2dfaf72d3fd95a33d9a3ebedf4fc84756e68b17b41696e\"");
    assert_contains(text, "\"id\":\"v264-screenshot\"");
    assert_contains(text, "\"digest\":\"fbbb87cc823c3a440239a7181ede23f1f040026057d6df44a9c70707c51f023b\"");
    assert_contains(text, "\"id\":\"v264-explanation\"");
    assert_contains(text, "\"id\":\"v264-code-walkthrough\"");
    assert_contains(text, "\"id\":\"v265-html-archive\"");
    assert_contains(text, "\"digest\":\"7b8a37a909bab429510204ffb890e75dd43758b95aa277ca2906838ae482f25f\"");
    assert_contains(text, "\"id\":\"v265-screenshot\"");
    assert_contains(text, "\"id\":\"v265-explanation\"");
    assert_contains(text, "\"id\":\"v265-code-walkthrough\"");
    assert_contains(text, "\"id\":\"active-plan\"");
    assert_contains(text, "\"id\":\"v264-archive-snippets\"");
    assert_contains(text, "\"id\":\"v265-archive-snippets\"");
    assert_contains(text, "\"id\":\"plan-snippets\"");
}

void assert_checks_and_summary(const std::string& text) {
    assert_contains(text, "\"source_node_v264_ready\":true");
    assert_contains(text, "\"source_node_v264_digest_valid\":true");
    assert_contains(text, "\"source_node_v265_ready\":true");
    assert_contains(text, "\"source_node_v265_digest_valid\":true");
    assert_contains(text, "\"source_node_v265_consumes_upstream_echoes\":true");
    assert_contains(text, "\"archive_files_present\":true");
    assert_contains(text, "\"archive_files_non_empty\":true");
    assert_contains(text, "\"archive_snippets_matched\":true");
    assert_contains(text, "\"v264_archive_records_fake_shell_contract\":true");
    assert_contains(text, "\"v265_archive_records_upstream_echo_verification\":true");
    assert_contains(text, "\"walkthroughs_record_implementation_and_verification\":true");
    assert_contains(text, "\"active_plan_points_to_v266_archive_verification\":true");
    assert_contains(text, "\"route_responses_verified\":true");
    assert_contains(text, "\"no_archive_verification_fake_shell_rerun\":true");
    assert_contains(text, "\"upstream_actions_still_disabled\":true");
    assert_contains(text, "\"production_audit_still_blocked\":true");
    assert_contains(text, "\"production_window_still_blocked\":true");
    assert_contains(text, "\"ready_for_credential_resolver_fake_shell_archive_verification\":true");
    assert_contains(text, "\"check_count\":28");
    assert_contains(text, "\"passed_check_count\":28");
    assert_contains(text, "\"production_blocker_count\":0");
    assert_contains(text, "\"warning_count\":1");
    assert_contains(text, "\"recommendation_count\":2");
    assert_contains(text, "\"code\":\"ARCHIVE_VERIFICATION_ONLY\"");
    assert_contains(text, "\"code\":\"WRITE_POST_V266_PLAN\"");
    assert_contains(text, "\"code\":\"KEEP_REAL_RESOLVER_OUT_OF_SCOPE\"");
    assert_contains(text, "\"next_required_echo_versions\":[\"Java v110\",\"mini-kv v117\"]");
}

void assert_non_participation_flags(const std::string& text) {
    assert_contains(text, "\"credential_resolver_fake_shell_archive_non_participation_receipt_only\":true");
    assert_contains(text, "\"archive_verification_only\":true");
    assert_contains(text, "\"archive_files_read_by_mini_kv\":false");
    assert_contains(text, "\"archive_verification_reruns_fake_shell_behavior\":false");
    assert_contains(text, "\"credential_resolver_implemented\":false");
    assert_contains(text, "\"credential_resolver_invoked\":false");
    assert_contains(text, "\"resolver_client_instantiated\":false");
    assert_contains(text, "\"secret_provider_instantiated\":false");
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"external_audit_service_auto_start_allowed\":false");
    assert_contains(text, "\"connection_execution_allowed\":false");
    assert_contains(text, "\"write_commands_executed\":false");
    assert_contains(text, "\"admin_commands_executed\":false");
    assert_contains(text, "\"runtime_write_observed\":false");
    assert_contains(text, "\"managed_audit_storage_backend\":false");
    assert_contains(text, "\"sandbox_audit_storage_backend\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"managed_audit_write_executed\":false");
    assert_contains(text, "\"production_record_written\":false");
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
                              "credential-resolver-fake-shell-archive-non-participation-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const std::vector<std::string> read_commands = {
        "INFOJSON",
        "STORAGEJSON",
        "HEALTH",
        "STATSJSON",
    };
    const auto expected_digest = minikv::runtime_evidence_receipts::
        credential_resolver_fake_shell_archive_non_participation_receipt_digest(read_commands);

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(
        receipt,
        "\"receipt_version\":\"mini-kv-credential-resolver-fake-shell-archive-non-participation-receipt.v1\"");
    assert_contains(receipt, "\"release_version\":\"v117\"");
    assert_contains(
        receipt,
        "\"path\":\"fixtures/release/credential-resolver-fake-shell-archive-non-participation-receipt.json\"");
    assert_contains(receipt, "\"credential_resolver_fake_shell_archive_non_participation_receipt\":");
    assert_contains(receipt, "\"current_release_version\":\"v117\"");
    assert_contains(receipt, "\"current_artifact_path_hint\":\"c/117/\"");
    assert_contains(receipt, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(receipt, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_source_archive_shape(receipt);
    assert_source_node_summaries(receipt);
    assert_archive_evidence(receipt);
    assert_checks_and_summary(receipt);
    assert_non_participation_flags(receipt);
    assert_contains(receipt, "credential resolver fake-shell archive non-participation receipt only");
    assert_contains(receipt, "does not rerun fake resolver shell behavior");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_not_contains(receipt, "raw_endpoint_url\":\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"consumer_hint\":\"Node v267 credential resolver fake-shell archive upstream echo verification\"");
    assert_contains(smoke, "\"credential_resolver_fake_shell_archive_non_participation_receipt\":");
    assert_contains(smoke, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_archive_shape(smoke);
    assert_archive_evidence(smoke);
    assert_checks_and_summary(smoke);
    assert_non_participation_flags(smoke);
    assert_contains(smoke,
                    "Node v267 may verify the mini-kv v117 credential resolver fake-shell archive non-participation receipt");

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_credential_resolver_fake_shell_archive_non_participation_receipt_tests\"");
    assert_contains(
        manifest,
        "\"path\":\"fixtures/release/credential-resolver-fake-shell-archive-non-participation-receipt.json\"");
    assert_contains(manifest, "\"credential_resolver_fake_shell_archive_non_participation_receipt\":");
    assert_contains(manifest, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_contains(
        manifest,
        "SMOKEJSON exposes credential_resolver_fake_shell_archive_non_participation_receipt for Node v267");
    assert_source_archive_shape(manifest);
    assert_checks_and_summary(manifest);
    assert_non_participation_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"credential_resolver_fake_shell_archive_non_participation_receipt\":");
    assert_contains(
        result.response,
        "\"receipt_fixture_path\":\"fixtures/release/credential-resolver-fake-shell-archive-non-participation-receipt.json\"");
    assert_contains(result.response, "\"current_release_version\":\"v117\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/117/\"");
    assert_contains(result.response, "\"receipt_digest\":\"" + expected_digest + "\"");
    assert_source_archive_shape(result.response);
    assert_source_node_summaries(result.response);
    assert_archive_evidence(result.response);
    assert_checks_and_summary(result.response);
    assert_non_participation_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
