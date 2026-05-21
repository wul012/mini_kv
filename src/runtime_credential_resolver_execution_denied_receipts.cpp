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

constexpr std::string_view execution_denied_receipt_consumer =
    "Node v291 execution-denied upstream echo verification";
constexpr std::string_view execution_denied_receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json";
constexpr std::string_view execution_denied_receipt_release_version = "v128";
constexpr std::string_view execution_denied_receipt_artifact_path_hint = "d/128/";
constexpr std::string_view execution_denied_source_preflight =
    "Node v290 disabled fake harness execution-denied route preflight";
constexpr std::string_view execution_denied_source_echo =
    "Node v289 disabled fake harness contract upstream echo verification";
constexpr std::string_view execution_denied_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-execution-denied-route-preflight.v1";
constexpr std::string_view execution_denied_source_v289_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract-upstream-echo-verification.v1";
constexpr std::string_view execution_denied_preflight_state =
    "disabled-fake-harness-execution-denied-route-preflight-ready";
constexpr std::string_view execution_denied_source_v289_state =
    "disabled-fake-harness-contract-upstream-echo-verification-ready";
constexpr std::string_view execution_denied_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-execution-denied-route-preflight";
constexpr std::string_view execution_denied_source_v289_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract-upstream-echo-verification";
constexpr std::string_view execution_denied_preflight_digest =
    "21ea6d7a6c641489048fc2540126e2f461909246fdd2129bcb0fc6d1ffbbf1cb";
constexpr std::string_view execution_denied_source_v289_digest =
    "e5b596b65f68e753459be163f8a58d1f677098e64b3be01d4e83737ef8c4d94f";
constexpr std::string_view execution_denied_preflight_mode =
    "disabled-fake-harness-execution-denied-route-preflight-only";
constexpr std::string_view execution_denied_route_surface = "audit-json-markdown-route";
constexpr std::string_view execution_denied_active_plan =
    "docs/plans2/v289-post-disabled-fake-harness-echo-roadmap.md";
constexpr std::string_view execution_denied_runtime_role = "runtime evidence provider only";
constexpr std::string_view execution_denied_boundary =
    "credential resolver disabled fake harness execution-denied non-participation receipt only; mini-kv echoes Node v290 execution-denied route preflight and preflightDigest, does not implement or run a fake harness runtime, does not instantiate real or fake secret providers or resolver clients, does not read, store, or render credential values, does not parse or render raw endpoint URLs, does not send HTTP/TCP or other external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view execution_denied_node_action =
    "verify mini-kv v128 execution-denied non-participation with Java quality evidence before Node v291 upstream echo verification";

constexpr int execution_denied_check_count = 25;
constexpr int execution_denied_source_check_count = 29;
constexpr int execution_denied_simulated_attempt_count = 8;
constexpr int execution_denied_denied_attempt_count = 8;
constexpr int execution_denied_actual_execution_attempt_count = 0;
constexpr int execution_denied_denial_reason_count = 8;
constexpr int execution_denied_production_blocker_count = 0;
constexpr int execution_denied_warning_count = 2;
constexpr int execution_denied_recommendation_count = 2;

std::string format_denial_reasons_json() {
    return "[\"real-approval-gate-not-satisfied\",\"credential-value-read-forbidden\","
           "\"raw-endpoint-url-parse-forbidden\",\"provider-client-instantiation-forbidden\","
           "\"http-tcp-dial-forbidden\",\"approval-ledger-and-schema-migration-forbidden\","
           "\"disabled-fake-harness-runtime-absent\",\"automatic-upstream-start-forbidden\"]";
}

std::string format_simulated_route_attempts_json() {
    return "[{\"id\":\"approval-gate\",\"surface\":\"node-route\","
           "\"requested_operation\":\"treat the disabled fake harness route as an execution approval\","
           "\"denied_by\":\"approval gate remains absent and v290 is a preflight report only\","
           "\"source_evidence\":\"Node v289 disabled fake harness upstream echo verification\","
           "\"denied\":true},"
           "{\"id\":\"credential-value\",\"surface\":\"credential-boundary\","
           "\"requested_operation\":\"read, store, or render the managed audit credential value\","
           "\"denied_by\":\"credential handle review is allowed; credential value access is denied\","
           "\"source_evidence\":\"Node v288 contract + mini-kv v127 non-participation\","
           "\"denied\":true},"
           "{\"id\":\"raw-endpoint-url\",\"surface\":\"endpoint-boundary\","
           "\"requested_operation\":\"parse or render a raw managed audit endpoint URL\","
           "\"denied_by\":\"endpoint handle review is allowed; raw endpoint URL access is denied\","
           "\"source_evidence\":\"Node v288 contract + Java v126 boundary catalog\","
           "\"denied\":true},"
           "{\"id\":\"provider-client\",\"surface\":\"provider-client-boundary\","
           "\"requested_operation\":\"instantiate a real or fake secret provider/resolver client\","
           "\"denied_by\":\"provider and client instantiation remain explicitly blocked\","
           "\"source_evidence\":\"Node v289 upstream echo verification\","
           "\"denied\":true},"
           "{\"id\":\"http-tcp\",\"surface\":\"network-boundary\","
           "\"requested_operation\":\"dial HTTP/TCP to managed audit or an upstream fake harness\","
           "\"denied_by\":\"external request sending remains forbidden\","
           "\"source_evidence\":\"Node v289 upstream echo verification\","
           "\"denied\":true},"
           "{\"id\":\"ledger-schema\",\"surface\":\"write-boundary\","
           "\"requested_operation\":\"write approval ledger rows or execute schema migration SQL\","
           "\"denied_by\":\"ledger writes and schema migration remain forbidden\","
           "\"source_evidence\":\"Java v122-v126 evidence and Node v289 verification\","
           "\"denied\":true},"
           "{\"id\":\"fake-harness-runtime\",\"surface\":\"runtime-boundary\","
           "\"requested_operation\":\"execute a disabled fake harness runtime\","
           "\"denied_by\":\"fake harness runtime implementation is absent and invocation is denied\","
           "\"source_evidence\":\"Node v288 disabled fake harness contract\","
           "\"denied\":true},"
           "{\"id\":\"automatic-upstream-start\",\"surface\":\"runtime-boundary\","
           "\"requested_operation\":\"auto-start Java, mini-kv, or managed audit services\","
           "\"denied_by\":\"automatic upstream start remains forbidden\","
           "\"source_evidence\":\"Node v289 upstream echo verification\","
           "\"denied\":true}]";
}

std::string format_checks_json() {
    return "{\"source_node_v289_ready\":true,"
           "\"source_node_v289_digest_valid\":true,"
           "\"source_node_v289_keeps_runtime_blocked\":true,"
           "\"source_node_v289_keeps_connection_blocked\":true,"
           "\"source_node_v289_keeps_credential_boundary_closed\":true,"
           "\"source_node_v289_keeps_endpoint_boundary_closed\":true,"
           "\"source_node_v289_keeps_writes_blocked\":true,"
           "\"route_registered_as_audit_json_markdown\":true,"
           "\"route_read_only_get_only\":true,\"route_execution_denied\":true,"
           "\"all_denied_attempts_simulated_only\":true,"
           "\"all_denied_attempts_blocked\":true,"
           "\"approval_gate_still_required\":true,"
           "\"credential_value_still_forbidden\":true,"
           "\"raw_endpoint_still_forbidden\":true,"
           "\"provider_client_still_forbidden\":true,"
           "\"http_tcp_still_forbidden\":true,"
           "\"ledger_schema_still_forbidden\":true,"
           "\"fake_harness_runtime_still_absent\":true,"
           "\"automatic_upstream_start_still_blocked\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_fake_harness_execution_denied_route_preflight\":true}";
}

std::string format_non_participation_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"execution_denied_non_participation_receipt_only\":true,"
           "\"execution_denied_route_preflight_only\":true,"
           "\"consumes_node_v290_execution_denied_route_preflight\":true,"
           "\"ready_for_node_v291_upstream_echo_verification\":true,"
           "\"ready_for_java_v127_mini_kv_v128_parallel_evidence\":true,"
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

std::string credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{execution_denied_receipt_release_version}},
        {std::string{execution_denied_receipt_artifact_path_hint}},
        {std::string{execution_denied_source_preflight}},
        {std::string{execution_denied_source_echo}},
        {std::string{execution_denied_profile_version}},
        {std::string{execution_denied_route_path}},
        {std::string{execution_denied_preflight_state}},
        {std::string{execution_denied_source_v289_digest}},
        {std::string{execution_denied_preflight_digest}},
        {std::string{execution_denied_preflight_mode}},
        {std::string{execution_denied_route_surface}},
        {std::to_string(execution_denied_check_count)},
        {std::to_string(execution_denied_source_check_count)},
        {std::to_string(execution_denied_simulated_attempt_count)},
        {std::to_string(execution_denied_denied_attempt_count)},
        {std::to_string(execution_denied_actual_execution_attempt_count)},
        {std::to_string(execution_denied_denial_reason_count)},
        {std::to_string(execution_denied_production_blocker_count)},
        {"execution-denied-route-preflight-only"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest(
        "mini-kv-credential-resolver-disabled-fake-harness-execution-denied-non-participation",
        parts);
}

std::string format_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(execution_denied_receipt_fixture_path) +
           ",\"source_preflight\":" + field_string(execution_denied_source_preflight) +
           ",\"source_upstream_echo\":" + field_string(execution_denied_source_echo) +
           ",\"consumer_hint\":" + field_string(execution_denied_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(execution_denied_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(execution_denied_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(execution_denied_runtime_role) +
           ",\"source_profile_version\":" + field_string(execution_denied_profile_version) +
           ",\"source_route_path\":" + field_string(execution_denied_route_path) +
           ",\"source_preflight_state\":" + field_string(execution_denied_preflight_state) +
           ",\"source_ready_for_execution_denied_route_preflight\":true"
           ",\"source_read_only_route_preflight\":true"
           ",\"source_execution_denied_route_preflight_only\":true"
           ",\"source_consumes_node_v289_disabled_fake_harness_contract_upstream_echo_verification\":true"
           ",\"source_ready_for_java_v127_mini_kv_v128_parallel_evidence\":true"
           ",\"source_ready_for_managed_audit_resolver_implementation\":false"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
           ",\"source_ready_for_production_operations\":false"
           ",\"source_real_resolver_implementation_allowed\":false"
           ",\"source_test_only_fake_harness_allowed\":false"
           ",\"source_test_only_fake_harness_execution_allowed\":false"
           ",\"source_fake_harness_runtime_enabled\":false"
           ",\"source_fake_harness_invocation_allowed\":false"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_credential_value_provided\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_raw_endpoint_url_rendered\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_secret_provider_instantiated\":false"
           ",\"source_resolver_client_instantiated\":false"
           ",\"source_fake_secret_provider_instantiated\":false"
           ",\"source_fake_resolver_client_instantiated\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_approval_ledger_written\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_node_v289_reference\":{\"source_version\":\"Node v289\","
           "\"profile_version\":" + field_string(execution_denied_source_v289_profile_version) +
           ",\"verification_state\":" + field_string(execution_denied_source_v289_state) +
           ",\"ready_for_disabled_fake_harness_contract_upstream_echo_verification\":true"
           ",\"verification_digest\":" + field_string(execution_denied_source_v289_digest) +
           ",\"source_span\":\"Node v288 + Java v122-v126 + mini-kv v127\""
           ",\"source_check_count\":" + std::to_string(execution_denied_source_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(execution_denied_source_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"java_evidence_ready\":true"
           ",\"mini_kv_non_participation_ready\":true"
           ",\"implementation_still_blocked\":true"
           ",\"ready_for_next_disabled_fake_harness_planning\":true"
           ",\"ready_for_managed_audit_resolver_implementation\":false"
           ",\"ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"ready_for_production_audit\":false"
           ",\"ready_for_production_window\":false"
           ",\"ready_for_production_operations\":false"
           ",\"real_resolver_implementation_allowed\":false"
           ",\"test_only_fake_harness_allowed\":false"
           ",\"test_only_fake_harness_execution_allowed\":false"
           ",\"fake_harness_runtime_enabled\":false"
           ",\"fake_harness_invocation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"credential_value_provided\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"raw_endpoint_url_rendered\":false"
           ",\"external_request_sent\":false"
           ",\"secret_provider_instantiated\":false"
           ",\"resolver_client_instantiated\":false"
           ",\"fake_secret_provider_instantiated\":false"
           ",\"fake_resolver_client_instantiated\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"execution_denied_route_preflight\":{\"preflight_digest\":" +
           field_string(execution_denied_preflight_digest) +
           ",\"preflight_mode\":" + field_string(execution_denied_preflight_mode) +
           ",\"source_span\":\"Node v289\""
           ",\"route_surface\":" + field_string(execution_denied_route_surface) +
           ",\"route_path\":" + field_string(execution_denied_route_path) +
           ",\"http_method\":\"GET\""
           ",\"format_modes\":[\"json\",\"markdown\"]"
           ",\"route_registered\":true,\"route_read_only\":true,"
           "\"route_execution_denied\":true"
           ",\"execution_denied_reason_count\":" +
           std::to_string(execution_denied_denial_reason_count) +
           ",\"simulated_attempt_count\":" +
           std::to_string(execution_denied_simulated_attempt_count) +
           ",\"denied_attempt_count\":" + std::to_string(execution_denied_denied_attempt_count) +
           ",\"actual_execution_attempt_count\":0"
           ",\"approval_gate_required\":true,\"approval_gate_satisfied\":false"
           ",\"credential_value_read_allowed\":false"
           ",\"raw_endpoint_url_parse_allowed\":false"
           ",\"provider_client_instantiation_allowed\":false"
           ",\"http_tcp_dial_allowed\":false"
           ",\"ledger_write_allowed\":false"
           ",\"schema_migration_allowed\":false"
           ",\"fake_harness_runtime_implementation_allowed\":false"
           ",\"fake_harness_runtime_invocation_allowed\":false"
           ",\"automatic_upstream_start_allowed\":false"
           ",\"denial_reasons\":" + format_denial_reasons_json() +
           ",\"simulated_route_attempts\":" + format_simulated_route_attempts_json() + "}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(execution_denied_check_count) +
           ",\"passed_check_count\":" + std::to_string(execution_denied_check_count) +
           ",\"simulated_attempt_count\":" +
           std::to_string(execution_denied_simulated_attempt_count) +
           ",\"denied_attempt_count\":" + std::to_string(execution_denied_denied_attempt_count) +
           ",\"actual_execution_attempt_count\":0"
           ",\"denial_reason_count\":" +
           std::to_string(execution_denied_denial_reason_count) +
           ",\"source_check_count\":" + std::to_string(execution_denied_source_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(execution_denied_source_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"production_blocker_count\":0"
           ",\"warning_count\":" + std::to_string(execution_denied_warning_count) +
           ",\"recommendation_count\":" + std::to_string(execution_denied_recommendation_count) +
           "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"EXECUTION_DENIED_PREFLIGHT_IS_NOT_RUNTIME\",\"severity\":\"warning\"},"
           "{\"code\":\"JAVA_QUALITY_QUEUE_NOT_RUNTIME_ECHO\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V127_MINI_KV_V128_IN_PARALLEL\","
           "\"severity\":\"recommendation\"},{\"code\":\"WAIT_FOR_UPSTREAM_STATE_BEFORE_NODE_V291\","
           "\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"execution_denied_route_preflight_json\":" +
           field_string(execution_denied_route_path) +
           ",\"execution_denied_route_preflight_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-execution-denied-route-preflight?format=markdown\","
           "\"source_node_v289_json\":" + field_string(execution_denied_source_v289_route_path) +
           ",\"source_node_v289_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract-upstream-echo-verification?format=markdown\","
           "\"active_plan\":" + field_string(execution_denied_active_plan) +
           ",\"next_recommended_parallel\":\"Java v127 + mini-kv v128\","
           "\"next_node_verification\":\"Node v291\"}"
           ",\"next_required_evidence_versions\":[\"Java v127\",\"mini-kv v128\"]"
           ",\"next_actions\":[\"Archive Node v290 as a read-only execution-denied route preflight.\","
           "\"Recommend parallel Java v127 quality stopgap plus mini-kv v128 non-participation receipt after v290 is archived.\","
           "\"Keep Java v127 focused on LiveAggregationIntegrationTests split and mini-kv v128 focused on execution-denied non-participation.\","
           "\"Do not instantiate fake harness runtime, fake providers, fake resolver clients, real providers, or real resolver clients.\","
           "\"Do not read credential values, parse raw endpoint URLs, send HTTP/TCP, connect managed audit, write ledgers, run schema migration, or auto-start upstream services.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_non_participation_flags_json() +
           ",\"boundary\":" + field_string(execution_denied_boundary) +
           ",\"node_action\":" + field_string(execution_denied_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
