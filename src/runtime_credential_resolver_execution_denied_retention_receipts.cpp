#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"

#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::field_string;
using credential_resolver_detail::receipt_digest;

constexpr std::string_view retention_consumer =
    "Node v293 fake harness readiness blocked decision upstream echo verification";
constexpr std::string_view retention_fixture_path =
    "fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json";
constexpr std::string_view retention_release_version = "v129";
constexpr std::string_view retention_artifact_path_hint = "d/129/";
constexpr std::string_view retention_source_decision =
    "Node v292 credential resolver fake harness readiness decision record";
constexpr std::string_view retention_source_verification =
    "Node v291 execution-denied upstream echo verification";
constexpr std::string_view retention_source_receipt =
    "mini-kv v128 credential resolver disabled fake harness execution-denied non-participation receipt";
constexpr std::string_view retention_runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v292_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-fake-harness-readiness-decision-record.v1";
constexpr std::string_view node_v292_state = "fake-harness-readiness-decision-record-ready";
constexpr std::string_view node_v292_decision_digest =
    "a21613fd08151de0690e5f83826f4b5d6a62131b52273b3c368130197949798d";
constexpr std::string_view node_v292_decision_reason =
    "Node v291 consumed Node v290, Java v127-v130, and mini-kv v128, but Java still lacks a direct execution-denied echo; Node must not enter disabled runtime shell planning from quality evidence alone.";

constexpr std::string_view node_v291_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-execution-denied-upstream-echo-verification.v1";
constexpr std::string_view node_v291_verification_digest =
    "8f84fc7940c72f02b3e229da3f02468488fe5789e4699a17d3ae07585689d613";
constexpr std::string_view v128_receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json";
constexpr std::string_view v128_receipt_digest = "fnv1a64:d60057a3760cad62";
constexpr std::string_view v128_preflight_digest =
    "21ea6d7a6c641489048fc2540126e2f461909246fdd2129bcb0fc6d1ffbbf1cb";

constexpr std::string_view node_v292_decision_record_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-fake-harness-readiness-decision-record";
constexpr std::string_view node_v291_verification_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-execution-denied-upstream-echo-verification";
constexpr std::string_view active_plan =
    "docs/plans2/v292-post-fake-harness-readiness-decision-roadmap.md";
constexpr std::string_view next_node_verification = "Node v293";

constexpr int retention_check_count = 18;
constexpr int node_v292_check_count = 18;
constexpr int node_v291_check_count = 19;
constexpr int node_v291_passed_check_count = 17;
constexpr int v128_check_count = 25;
constexpr int required_evidence_count = 4;
constexpr int missing_required_evidence_count = 1;
constexpr int no_go_condition_count = 8;
constexpr int production_blocker_count = 1;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view retention_boundary =
    "credential resolver disabled fake harness execution-denied receipt verification retention check only; mini-kv verifies the v128 receipt remains available and digest-stable for Node v293 consumption, echoes Node v292 blocked readiness decision, does not implement or run a fake harness runtime, does not instantiate real or fake secret providers or resolver clients, does not read, store, or render credential values, does not parse or render raw endpoint URLs, does not send HTTP/TCP or other external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view retention_node_action =
    "verify mini-kv v129 receipt retention check with Java v131 direct execution-denied echo before Node v293 blocked decision upstream echo verification";

std::string format_required_evidence_json() {
    return "[{\"id\":\"node-v290-execution-denied-route-preflight\",\"present\":true},"
           "{\"id\":\"java-v127-v130-quality-evidence\",\"present\":true},"
           "{\"id\":\"mini-kv-v128-execution-denied-receipt\",\"present\":true},"
           "{\"id\":\"java-direct-execution-denied-echo\",\"present\":false}]";
}

std::string format_no_go_conditions_json() {
    return "[\"JAVA_EXECUTION_DENIED_ECHO_MISSING\",\"CREDENTIAL_VALUE_REQUIRED\","
           "\"RAW_ENDPOINT_URL_REQUIRED\",\"FAKE_HARNESS_RUNTIME_REQUIRED\","
           "\"PROVIDER_CLIENT_REQUIRED\",\"HTTP_TCP_REQUIRED\","
           "\"LEDGER_SCHEMA_WRITE_REQUIRED\",\"AUTOMATIC_UPSTREAM_START_REQUIRED\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v292_ready\":true,"
           "\"source_node_v292_blocked_as_expected\":true,"
           "\"source_node_v292_blocks_runtime_shell\":true,"
           "\"source_node_v292_blocks_credential_endpoint\":true,"
           "\"source_node_v292_blocks_connection_writes_and_auto_start\":true,"
           "\"source_node_v291_loaded\":true,"
           "\"source_node_v291_blocked_as_expected\":true,"
           "\"v128_receipt_fixture_present\":true,"
           "\"v128_receipt_digest_stable\":true,"
           "\"v128_preflight_digest_stable\":true,"
           "\"v128_summary_counts_stable\":true,"
           "\"retention_fixture_path_declared\":true,"
           "\"runtime_smoke_exposes_v128_receipt\":true,"
           "\"release_manifest_exposes_v128_receipt\":true,"
           "\"java_direct_execution_denied_echo_missing_recorded\":true,"
           "\"side_effect_boundaries_closed\":true,"
           "\"load_compact_restore_setnxex_still_blocked\":true,"
           "\"ready_for_node_v293_blocked_decision_upstream_echo\":true}";
}

std::string format_closed_side_effect_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"receipt_verification_retention_check_only\":true,"
           "\"retention_check_only\":true,"
           "\"consumes_node_v292_fake_harness_readiness_decision_record\":true,"
           "\"consumes_node_v291_execution_denied_upstream_echo_verification\":true,"
           "\"consumes_mini_kv_v128_execution_denied_receipt\":true,"
           "\"ready_for_node_v293_blocked_decision_upstream_echo_verification\":true,"
           "\"ready_for_disabled_runtime_shell_planning\":false,"
           "\"ready_for_managed_audit_resolver_implementation\":false,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"ready_for_production_audit\":false,"
           "\"ready_for_production_window\":false,"
           "\"ready_for_production_operations\":false,"
           "\"real_resolver_implementation_allowed\":false,"
           "\"test_only_fake_harness_allowed\":false,"
           "\"test_only_fake_harness_execution_allowed\":false,"
           "\"fake_harness_runtime_enabled\":false,"
           "\"fake_harness_invocation_allowed\":false,"
           "\"fake_harness_runtime_implemented\":false,"
           "\"fake_harness_runtime_invoked\":false,"
           "\"execute_fake_harness_runtime_allowed\":false,"
           "\"actual_execution_attempt_count\":0,"
           "\"credential_resolver_implemented\":false,"
           "\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,"
           "\"secret_provider_instantiated\":false,"
           "\"fake_secret_provider_instantiated\":false,"
           "\"fake_resolver_client_instantiated\":false,"
           "\"provider_client_instantiation_allowed\":false,"
           "\"credential_value_read_allowed\":false,"
           "\"credential_value_read\":false,"
           "\"credential_value_provided\":false,"
           "\"credential_value_loaded\":false,"
           "\"credential_value_stored\":false,"
           "\"credential_value_included\":false,"
           "\"credential_value_rendered\":false,"
           "\"raw_endpoint_url_parse_allowed\":false,"
           "\"raw_endpoint_url_render_allowed\":false,"
           "\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_rendered\":false,"
           "\"raw_endpoint_url_provided\":false,"
           "\"raw_endpoint_url_included\":false,"
           "\"external_request_allowed\":false,"
           "\"external_request_sent\":false,"
           "\"http_tcp_dial_allowed\":false,"
           "\"connects_managed_audit\":false,"
           "\"reads_managed_audit_credential\":false,"
           "\"stores_managed_audit_credential\":false,"
           "\"managed_audit_store\":false,"
           "\"managed_audit_storage_backend\":false,"
           "\"sandbox_audit_storage_backend\":false,"
           "\"storage_write_allowed\":false,"
           "\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,"
           "\"runtime_write_observed\":false,"
           "\"approval_gate_required\":true,"
           "\"approval_gate_satisfied\":false,"
           "\"approval_ledger_write_allowed\":false,"
           "\"approval_ledger_written\":false,"
           "\"approval_ledger_write_executed\":false,"
           "\"managed_audit_write_executed\":false,"
           "\"production_record_written\":false,"
           "\"schema_migration_allowed\":false,"
           "\"schema_migration_executed\":false,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,"
           "\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,"
           "\"node_auto_start_allowed\":false,"
           "\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,"
           "\"automatic_upstream_start_allowed\":false,"
           "\"automatic_upstream_start\":false,"
           "\"audit_authoritative\":false,"
           "\"order_authoritative\":false";
}

} // namespace

std::string credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{retention_release_version}},
        {std::string{retention_artifact_path_hint}},
        {std::string{retention_source_decision}},
        {std::string{retention_source_verification}},
        {std::string{retention_source_receipt}},
        {std::string{node_v292_profile_version}},
        {std::string{node_v292_state}},
        {std::string{node_v292_decision_digest}},
        {std::string{node_v291_profile_version}},
        {std::string{node_v291_verification_digest}},
        {std::string{v128_receipt_fixture_path}},
        {std::string{v128_receipt_digest}},
        {std::string{v128_preflight_digest}},
        {std::to_string(retention_check_count)},
        {std::to_string(node_v292_check_count)},
        {std::to_string(node_v291_check_count)},
        {std::to_string(node_v291_passed_check_count)},
        {std::to_string(v128_check_count)},
        {std::to_string(required_evidence_count)},
        {std::to_string(missing_required_evidence_count)},
        {std::to_string(no_go_condition_count)},
        {std::to_string(production_blocker_count)},
        {"receipt-verification-retention-check-only"},
        {"blocked"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest(
        "mini-kv-credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check",
        parts);
}

std::string format_credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.v1\","
           "\"receipt_fixture_path\":" + field_string(retention_fixture_path) +
           ",\"source_decision_record\":" + field_string(retention_source_decision) +
           ",\"source_verification\":" + field_string(retention_source_verification) +
           ",\"source_mini_kv_receipt\":" + field_string(retention_source_receipt) +
           ",\"consumer_hint\":" + field_string(retention_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(retention_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(retention_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(retention_runtime_role) +
           ",\"source_node_v292_reference\":{\"source_version\":\"Node v292\","
           "\"profile_version\":" + field_string(node_v292_profile_version) +
           ",\"decision_record_state\":" + field_string(node_v292_state) +
           ",\"readiness_decision\":\"blocked\""
           ",\"decision_digest\":" + field_string(node_v292_decision_digest) +
           ",\"decision_reason\":" + field_string(node_v292_decision_reason) +
           ",\"source_span\":\"Node v287-v291\""
           ",\"check_count\":" + std::to_string(node_v292_check_count) +
           ",\"passed_check_count\":" + std::to_string(node_v292_check_count) +
           ",\"required_evidence_count\":" + std::to_string(required_evidence_count) +
           ",\"missing_required_evidence_count\":" +
           std::to_string(missing_required_evidence_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"read_only_decision_record\":true"
           ",\"fake_harness_readiness_decision_only\":true"
           ",\"java_direct_execution_denied_echo_missing\":true"
           ",\"mini_kv_v128_non_participation_ready\":true"
           ",\"side_effect_boundaries_aligned\":true"
           ",\"implementation_still_blocked\":true"
           ",\"required_evidence\":" + format_required_evidence_json() +
           ",\"explicit_no_go_conditions\":" + format_no_go_conditions_json() + "}"
           ",\"source_node_v291_reference\":{\"source_version\":\"Node v291\","
           "\"profile_version\":" + field_string(node_v291_profile_version) +
           ",\"verification_state\":\"blocked\""
           ",\"verification_digest\":" + field_string(node_v291_verification_digest) +
           ",\"source_span\":\"Node v290 + Java v127-v130 + mini-kv v128\""
           ",\"source_check_count\":" + std::to_string(node_v291_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(node_v291_passed_check_count) +
           ",\"source_production_blocker_count\":1"
           ",\"read_only_upstream_echo_verification\":true"
           ",\"execution_denied_upstream_echo_verification_only\":true"
           ",\"java_execution_denied_echo_missing\":true"
           ",\"java_execution_denied_echo_present\":false"
           ",\"mini_kv_v128_non_participation_ready\":true"
           ",\"mini_kv_preflight_digest_aligned\":true"
           ",\"side_effect_boundaries_aligned\":true"
           ",\"implementation_still_blocked\":true}"
           ",\"source_mini_kv_v128_receipt\":{\"release_version\":\"v128\","
           "\"fixture_path\":" + field_string(v128_receipt_fixture_path) +
           ",\"artifact_path_hint\":\"d/128/\""
           ",\"receipt_digest\":" + field_string(v128_receipt_digest) +
           ",\"preflight_digest\":" + field_string(v128_preflight_digest) +
           ",\"source_node_v289_digest\":\"e5b596b65f68e753459be163f8a58d1f677098e64b3be01d4e83737ef8c4d94f\""
           ",\"check_count\":" + std::to_string(v128_check_count) +
           ",\"passed_check_count\":" + std::to_string(v128_check_count) +
           ",\"simulated_attempt_count\":8"
           ",\"denied_attempt_count\":8"
           ",\"actual_execution_attempt_count\":0"
           ",\"production_blocker_count\":0"
           ",\"warning_count\":2"
           ",\"recommendation_count\":2"
           ",\"read_only\":true"
           ",\"execution_allowed\":false"
           ",\"load_restore_compact_executed\":false"
           ",\"setnxex_execution_allowed\":false}"
           ",\"retention_check\":{\"check_mode\":\"v128-receipt-verification-retention-check-only\","
           "\"retention_scope\":\"fixtures/release plus runtime SMOKEJSON and release manifest references\""
           ",\"historical_fixture_friendly\":true"
           ",\"standalone_fixture_path\":" + field_string(retention_fixture_path) +
           ",\"retained_fixture_path\":" + field_string(v128_receipt_fixture_path) +
           ",\"retained_artifact_path_hint\":\"d/128/\""
           ",\"current_artifact_path_hint\":" + field_string(retention_artifact_path_hint) +
           ",\"verifies_standalone_fixture_present\":true"
           ",\"verifies_runtime_smoke_exposes_v128_receipt\":true"
           ",\"verifies_release_manifest_exposes_v128_receipt\":true"
           ",\"verifies_digest_stable\":true"
           ",\"verifies_preflight_digest_stable\":true"
           ",\"verifies_node_v292_blocked_decision\":true"
           ",\"verifies_java_direct_execution_denied_echo_missing\":true"
           ",\"verifies_no_runtime_shell_planning\":true"
           ",\"verification_count\":8}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(retention_check_count) +
           ",\"passed_check_count\":" + std::to_string(retention_check_count) +
           ",\"source_node_v292_check_count\":" + std::to_string(node_v292_check_count) +
           ",\"source_node_v292_passed_check_count\":" +
           std::to_string(node_v292_check_count) +
           ",\"source_node_v292_production_blocker_count\":1"
           ",\"source_node_v291_check_count\":" + std::to_string(node_v291_check_count) +
           ",\"source_node_v291_passed_check_count\":" +
           std::to_string(node_v291_passed_check_count) +
           ",\"source_node_v291_production_blocker_count\":1"
           ",\"v128_receipt_check_count\":" + std::to_string(v128_check_count) +
           ",\"v128_receipt_passed_check_count\":" + std::to_string(v128_check_count) +
           ",\"required_evidence_count\":" + std::to_string(required_evidence_count) +
           ",\"missing_required_evidence_count\":" +
           std::to_string(missing_required_evidence_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[{\"code\":\"JAVA_EXECUTION_DENIED_ECHO_MISSING\",\"severity\":\"blocker\"}]"
           ",\"warnings\":[{\"code\":\"V128_RECEIPT_RETENTION_CHECK_IS_NOT_RUNTIME\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V131_MINI_KV_V129_IN_PARALLEL\",\"severity\":\"recommendation\"},"
           "{\"code\":\"WAIT_FOR_NODE_V293_BLOCKED_DECISION_UPSTREAM_ECHO\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"fake_harness_readiness_decision_record_json\":" +
           field_string(node_v292_decision_record_path) +
           ",\"fake_harness_readiness_decision_record_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-fake-harness-readiness-decision-record?format=markdown\","
           "\"source_node_v291_json\":" + field_string(node_v291_verification_path) +
           ",\"source_node_v291_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-execution-denied-upstream-echo-verification?format=markdown\","
           "\"source_mini_kv_v128_fixture\":" + field_string(v128_receipt_fixture_path) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"next_recommended_parallel\":\"Java v131 + mini-kv v129\","
           "\"next_node_verification\":" + field_string(next_node_verification) + "}"
           ",\"next_required_evidence_versions\":[\"Java v131\",\"mini-kv v129\"]"
           ",\"next_actions\":[\"Archive mini-kv v129 as a receipt verification and retention check.\","
           "\"Keep Node v293 blocked-decision upstream echo verification read-only until Java v131 direct execution-denied echo is available.\","
           "\"Do not instantiate fake harness runtime, fake providers, fake resolver clients, real providers, or real resolver clients.\","
           "\"Do not read credential values, parse raw endpoint URLs, send HTTP/TCP, connect managed audit, write ledgers, run schema migration, or auto-start upstream services.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_digest(
                   read_commands)) +
           "," + format_closed_side_effect_flags_json() +
           ",\"boundary\":" + field_string(retention_boundary) +
           ",\"node_action\":" + field_string(retention_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
