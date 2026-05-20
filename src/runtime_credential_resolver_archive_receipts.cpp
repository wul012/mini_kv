#include "minikv/runtime_evidence_receipts.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;

std::string field_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string receipt_digest(std::string_view prefix, const std::vector<DigestPart>& parts) {
    return runtime_evidence::digest(prefix, parts);
}

constexpr std::string_view archive_receipt_consumer =
    "Node v267 credential resolver fake-shell archive upstream echo verification";
constexpr std::string_view archive_receipt_fixture_path =
    "fixtures/release/credential-resolver-fake-shell-archive-non-participation-receipt.json";
constexpr std::string_view archive_receipt_release_version = "v117";
constexpr std::string_view archive_receipt_artifact_path_hint = "c/117/";
constexpr std::string_view archive_source_verification =
    "Node v266 credential resolver fake-shell archive verification";
constexpr std::string_view archive_source_contract =
    "Node v264 credential resolver test-only shell contract";
constexpr std::string_view archive_source_upstream_echo =
    "Node v265 test-only resolver shell upstream echo verification";
constexpr std::string_view archive_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-verification.v1";
constexpr std::string_view archive_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-fake-shell-archive-verification";
constexpr std::string_view archive_state =
    "credential-resolver-fake-shell-archive-verification-ready";
constexpr std::string_view archive_evidence_span =
    "Node v264 credential resolver fake shell contract + Node v265 upstream echo archive";
constexpr std::string_view archive_node_v264_contract_digest =
    "12d264c30d3d9a83ab90ac8b0ca6ccfccbe4ff36f5216bb53d13ec42bcccf14b";
constexpr std::string_view archive_node_v265_verification_digest =
    "2c15bc316448bf31a7d67633931bff291035cfdb473f11b87e94b610113aa28e";
constexpr std::string_view archive_verification_digest =
    "e58e721868c6e2779b78e0e1219c2a5a82c1064660523ed428b8eff7e6123f97";
constexpr std::string_view archive_source_v265_mode =
    "java-v107-plus-mini-kv-v116-test-only-resolver-shell-upstream-echo-verification-only";
constexpr std::string_view archive_source_span = "Node v264 + Java v107 + mini-kv v116";
constexpr std::string_view archive_runtime_role = "runtime evidence provider only";
constexpr std::string_view archive_boundary =
    "credential resolver fake-shell archive non-participation receipt only; mini-kv echoes Node v266 archive verification boundaries, reads no archive files at runtime, does not rerun fake resolver shell behavior, does not instantiate a resolver client or secret provider, does not accept or read credential values, does not parse raw endpoint URLs, does not send external requests, does not connect managed audit, does not write storage or production records, does not execute schema migration or restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view archive_node_action =
    "verify mini-kv non-participation with Node v266 credential resolver fake-shell archive verification before Node v267 upstream echo verification";
constexpr int archive_node_v264_request_shape_field_count = 9;
constexpr int archive_node_v264_response_shape_field_count = 13;
constexpr int archive_node_v264_failure_mapping_count = 7;
constexpr int archive_node_v264_guard_condition_count = 10;
constexpr int archive_node_v265_check_count = 20;
constexpr int archive_check_count = 28;
constexpr int archive_file_count = 9;
constexpr int archive_required_snippet_count = 24;
constexpr int archive_warning_count = 1;
constexpr int archive_recommendation_count = 2;

std::string format_common_archive_boundary_json() {
    return "\"read_only\":true,\"execution_allowed\":false,\"archive_verification_only\":true,"
           "\"archive_files_read_by_mini_kv\":false,\"archive_verification_reruns_fake_shell_behavior\":false,"
           "\"credential_resolver_implemented\":false,\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,\"secret_provider_instantiated\":false,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"external_audit_service_auto_start_allowed\":false,"
           "\"connection_execution_allowed\":false,\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,\"runtime_write_observed\":false,"
           "\"managed_audit_store\":false,\"managed_audit_storage_backend\":false,"
           "\"sandbox_audit_storage_backend\":false,\"storage_write_allowed\":false,"
           "\"managed_audit_write_executed\":false,\"production_record_written\":false,"
           "\"approval_ledger_write_allowed\":false,\"approval_ledger_write_executed\":false,"
           "\"sandbox_managed_audit_state_write_allowed\":false,"
           "\"credential_value_required\":false,\"credential_value_read_allowed\":false,"
           "\"credential_value_loaded\":false,\"credential_value_stored\":false,"
           "\"credential_value_included\":false,\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_included\":false,\"external_request_sent\":false,"
           "\"schema_rehearsal_execution_allowed\":false,\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"order_authoritative\":false";
}

std::string format_archive_files_json() {
    return "[{\"id\":\"v264-html-archive\",\"path\":\"c/264/sandbox-endpoint-credential-resolver-test-only-shell-contract-v264.html\",\"exists\":true,\"size_bytes\":3696,\"digest\":\"dff3ed1303149bc5aa2dfaf72d3fd95a33d9a3ebedf4fc84756e68b17b41696e\"},"
           "{\"id\":\"v264-screenshot\",\"path\":\"c/264/pictures/sandbox-endpoint-credential-resolver-test-only-shell-contract-v264.png\",\"exists\":true,\"size_bytes\":32911,\"digest\":\"fbbb87cc823c3a440239a7181ede23f1f040026057d6df44a9c70707c51f023b\"},"
           "{\"id\":\"v264-explanation\",\"path\":\"c/264/explanation/sandbox-endpoint-credential-resolver-test-only-shell-contract-v264.md\",\"exists\":true,\"size_bytes\":5121,\"digest\":\"055c6b0efcb7d692fb8ff76423099c642a94b7f7e0b9e9af86d03746c3ae531e\"},"
           "{\"id\":\"v264-code-walkthrough\",\"path\":\"code-explanation-production-prototype/268-sandbox-endpoint-credential-resolver-test-only-shell-contract-v264.md\",\"exists\":true,\"size_bytes\":9624,\"digest\":\"01a7ee143deb0c4f7d2a047ecfb04b774dafbde8b9759831cbc7c86d89be3551\"},"
           "{\"id\":\"v265-html-archive\",\"path\":\"c/265/sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification-v265.html\",\"exists\":true,\"size_bytes\":12427,\"digest\":\"7b8a37a909bab429510204ffb890e75dd43758b95aa277ca2906838ae482f25f\"},"
           "{\"id\":\"v265-screenshot\",\"path\":\"c/265/pictures/sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification-v265.png\",\"exists\":true,\"size_bytes\":34249,\"digest\":\"f8404f959e5b412036ebf567d692a1da0912215a8b6c168417bccdde99fdac74\"},"
           "{\"id\":\"v265-explanation\",\"path\":\"c/265/explanation/sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification-v265.md\",\"exists\":true,\"size_bytes\":5787,\"digest\":\"47a2d94560a262ac433b1dc25352e26a881e0d40cc42add279e8061065dd5289\"},"
           "{\"id\":\"v265-code-walkthrough\",\"path\":\"code-explanation-production-prototype/269-sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification-v265.md\",\"exists\":true,\"size_bytes\":11180,\"digest\":\"966ed8ba13aed763eefa0569345a3b89963e400cd03bf9e727a871ec88ecb78a\"},"
           "{\"id\":\"active-plan\",\"path\":\"docs/plans/v263-post-disabled-resolver-echo-roadmap.md\",\"exists\":true,\"size_bytes\":9534,\"digest\":\"ef914293ce52cc3993af29f5450e8823a4f80542d6eaa0fa54b4ec92db67deb0\"}]";
}

std::string format_archive_snippet_groups_json() {
    return "[{\"id\":\"v264-archive-snippets\",\"matched\":true,\"snippet_count\":6},"
           "{\"id\":\"v264-walkthrough-snippets\",\"matched\":true,\"snippet_count\":4},"
           "{\"id\":\"v265-archive-snippets\",\"matched\":true,\"snippet_count\":6},"
           "{\"id\":\"v265-walkthrough-snippets\",\"matched\":true,\"snippet_count\":4},"
           "{\"id\":\"plan-snippets\",\"matched\":true,\"snippet_count\":4}]";
}

std::string format_archive_checks_json() {
    return "{\"source_node_v264_ready\":true,\"source_node_v264_digest_valid\":true,"
           "\"source_node_v265_ready\":true,\"source_node_v265_digest_valid\":true,"
           "\"source_node_v265_consumes_upstream_echoes\":true,"
           "\"archive_files_present\":true,\"archive_files_non_empty\":true,"
           "\"v264_html_present\":true,\"v264_screenshot_present\":true,"
           "\"v264_screenshot_non_empty\":true,\"v264_explanation_present\":true,"
           "\"v264_code_walkthrough_present\":true,\"v265_html_present\":true,"
           "\"v265_screenshot_present\":true,\"v265_screenshot_non_empty\":true,"
           "\"v265_explanation_present\":true,\"v265_code_walkthrough_present\":true,"
           "\"archive_snippets_matched\":true,\"v264_archive_records_fake_shell_contract\":true,"
           "\"v265_archive_records_upstream_echo_verification\":true,"
           "\"walkthroughs_record_implementation_and_verification\":true,"
           "\"active_plan_points_to_v266_archive_verification\":true,"
           "\"route_responses_verified\":true,\"no_archive_verification_fake_shell_rerun\":true,"
           "\"upstream_actions_still_disabled\":true,\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_credential_resolver_fake_shell_archive_verification\":true}";
}

} // namespace

std::string credential_resolver_fake_shell_archive_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{archive_receipt_release_version}},
        {std::string{archive_receipt_artifact_path_hint}},
        {std::string{archive_source_verification}},
        {std::string{archive_source_contract}},
        {std::string{archive_source_upstream_echo}},
        {std::string{archive_profile_version}},
        {std::string{archive_route_path}},
        {std::string{archive_state}},
        {std::string{archive_evidence_span}},
        {std::string{archive_node_v264_contract_digest}},
        {std::string{archive_node_v265_verification_digest}},
        {std::string{archive_verification_digest}},
        {std::string{archive_source_v265_mode}},
        {std::string{archive_source_span}},
        {std::to_string(archive_node_v264_request_shape_field_count)},
        {std::to_string(archive_node_v264_response_shape_field_count)},
        {std::to_string(archive_node_v264_failure_mapping_count)},
        {std::to_string(archive_node_v264_guard_condition_count)},
        {std::to_string(archive_node_v265_check_count)},
        {std::to_string(archive_check_count)},
        {std::to_string(archive_file_count)},
        {std::to_string(archive_required_snippet_count)},
        {std::to_string(archive_warning_count)},
        {std::to_string(archive_recommendation_count)},
        {"true"},
        {"true"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-fake-shell-archive-non-participation", parts);
}

std::string format_credential_resolver_fake_shell_archive_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-fake-shell-archive-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(archive_receipt_fixture_path) +
           ",\"source_archive_verification\":" + field_string(archive_source_verification) +
           ",\"source_contract\":" + field_string(archive_source_contract) +
           ",\"source_upstream_echo\":" + field_string(archive_source_upstream_echo) +
           ",\"consumer_hint\":" + field_string(archive_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(archive_receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(archive_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(archive_runtime_role) +
           ",\"source_archive_profile_version\":" + field_string(archive_profile_version) +
           ",\"source_archive_route_path\":" + field_string(archive_route_path) +
           ",\"source_archive_verification_state\":" + field_string(archive_state) +
           ",\"source_ready_for_credential_resolver_fake_shell_archive_verification\":true"
           ",\"source_read_only_archive_verification\":true"
           ",\"source_archive_verification_reruns_fake_shell_behavior\":false"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_secret_provider_instantiated\":false"
           ",\"source_resolver_client_instantiated\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
           ",\"source_archive_verification_digest\":" + field_string(archive_verification_digest) +
           ",\"source_evidence_span\":" + field_string(archive_evidence_span) +
           ",\"source_node_v264_contract_digest\":" +
           field_string(archive_node_v264_contract_digest) +
           ",\"source_node_v265_verification_digest\":" +
           field_string(archive_node_v265_verification_digest) +
           ",\"source_node_v264\":{\"ready\":true,\"profile_version\":\"managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-contract.v1\","
           "\"shell_contract_state\":\"sandbox-endpoint-credential-resolver-test-only-shell-contract-ready\","
           "\"shell_mode\":\"test-only-fake-resolver-contract\",\"resolver_kind\":\"fake-in-memory\","
           "\"request_shape_field_count\":" +
           std::to_string(archive_node_v264_request_shape_field_count) +
           ",\"response_shape_field_count\":" +
           std::to_string(archive_node_v264_response_shape_field_count) +
           ",\"failure_mapping_count\":" +
           std::to_string(archive_node_v264_failure_mapping_count) +
           ",\"guard_condition_count\":" +
           std::to_string(archive_node_v264_guard_condition_count) +
           ",\"fake_resolver_only\":true,\"handle_only_request\":true,"
           "\"credential_resolver_execution_allowed\":false,\"credential_value_read\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"secret_provider_instantiated\":false,\"resolver_client_instantiated\":false,"
           "\"connects_managed_audit\":false,\"schema_migration_executed\":false,"
           "\"automatic_upstream_start\":false}"
           ",\"source_node_v265\":{\"ready\":true,\"profile_version\":\"managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification.v1\","
           "\"verification_state\":\"sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification-ready\","
           "\"verification_mode\":" + field_string(archive_source_v265_mode) +
           ",\"source_span\":" + field_string(archive_source_span) +
           ",\"source_node_v264_ready\":true,\"java_v107_echo_ready\":true,"
           "\"mini_kv_v116_non_participation_ready\":true,"
           "\"java_v109_optimization_context_ready\":true,"
           "\"request_shape_field_count\":" +
           std::to_string(archive_node_v264_request_shape_field_count) +
           ",\"response_shape_field_count\":" +
           std::to_string(archive_node_v264_response_shape_field_count) +
           ",\"failure_mapping_count\":" +
           std::to_string(archive_node_v264_failure_mapping_count) +
           ",\"guard_condition_count\":" +
           std::to_string(archive_node_v264_guard_condition_count) +
           ",\"check_count\":" + std::to_string(archive_node_v265_check_count) +
           ",\"passed_check_count\":" + std::to_string(archive_node_v265_check_count) +
           ",\"production_blocker_count\":0,\"credential_boundary_aligned\":true,"
           "\"raw_endpoint_boundary_aligned\":true,\"connection_boundary_aligned\":true,"
           "\"write_boundary_aligned\":true,\"auto_start_boundary_aligned\":true,"
           "\"credential_resolver_execution_allowed\":false,\"credential_value_read\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"secret_provider_instantiated\":false,\"resolver_client_instantiated\":false,"
           "\"connects_managed_audit\":false,\"schema_migration_executed\":false,"
           "\"automatic_upstream_start\":false}"
           ",\"archived_evidence\":{\"archive_roots\":[\"c/264/\",\"c/265/\"],"
           "\"source_versions\":[\"Node v264\",\"Node v265\"],\"archive_file_count\":" +
           std::to_string(archive_file_count) +
           ",\"required_snippet_count\":" + std::to_string(archive_required_snippet_count) +
           ",\"matched_snippet_count\":" + std::to_string(archive_required_snippet_count) +
           ",\"files\":" + format_archive_files_json() +
           ",\"snippet_groups\":" + format_archive_snippet_groups_json() + "}"
           ",\"archive_verification\":{\"archive_verification_digest\":" +
           field_string(archive_verification_digest) +
           ",\"evidence_span\":" + field_string(archive_evidence_span) +
           ",\"source_node_v264_contract_digest\":" +
           field_string(archive_node_v264_contract_digest) +
           ",\"source_node_v265_verification_digest\":" +
           field_string(archive_node_v265_verification_digest) +
           ",\"source_node_v264_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-contract\","
           "\"source_node_v265_route_path\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-test-only-shell-upstream-echo-verification\","
           "\"archive_verification_reads_files_only\":true,"
           "\"archive_verification_reruns_fake_shell_behavior\":false,"
           "\"upstream_actions_enabled\":false,\"production_audit_allowed\":false}"
           ",\"checks\":" + format_archive_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(archive_check_count) +
           ",\"passed_check_count\":" + std::to_string(archive_check_count) +
           ",\"archive_file_count\":" + std::to_string(archive_file_count) +
           ",\"required_snippet_count\":" + std::to_string(archive_required_snippet_count) +
           ",\"matched_snippet_count\":" + std::to_string(archive_required_snippet_count) +
           ",\"production_blocker_count\":0,\"warning_count\":" +
           std::to_string(archive_warning_count) +
           ",\"recommendation_count\":" + std::to_string(archive_recommendation_count) + "}"
           ",\"warnings\":[{\"code\":\"ARCHIVE_VERIFICATION_ONLY\",\"severity\":\"warning\","
           "\"message\":\"This profile verifies v264/v265 archive files and route shape only; it does not execute fake shell behavior.\"}]"
           ",\"recommendations\":[{\"code\":\"WRITE_POST_V266_PLAN\",\"severity\":\"recommendation\"},"
           "{\"code\":\"KEEP_REAL_RESOLVER_OUT_OF_SCOPE\",\"severity\":\"recommendation\"}]"
           ",\"next_required_echo_versions\":[\"Java v110\",\"mini-kv v117\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_fake_shell_archive_non_participation_receipt_digest(
               read_commands)) +
           ",\"credential_resolver_fake_shell_archive_non_participation_receipt_only\":true," +
           format_common_archive_boundary_json() +
           ",\"boundary\":" + field_string(archive_boundary) +
           ",\"node_action\":" + field_string(archive_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
