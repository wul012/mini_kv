#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::receipt_digest;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

constexpr std::string_view execution_denied_receipt_consumer = "Node v291 execution-denied upstream echo verification";
constexpr std::string_view execution_denied_receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json";
constexpr std::string_view execution_denied_receipt_release_version = "v128";
constexpr std::string_view execution_denied_receipt_artifact_path_hint = "d/128/";
constexpr std::string_view execution_denied_source_preflight =
    "Node v290 disabled fake harness execution-denied route preflight";
constexpr std::string_view execution_denied_source_echo =
    "Node v289 disabled fake harness contract upstream echo verification";
constexpr std::string_view execution_denied_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-execution-denied-route-"
    "preflight.v1";
constexpr std::string_view execution_denied_source_v289_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract-upstream-echo-"
    "verification.v1";
constexpr std::string_view execution_denied_preflight_state =
    "disabled-fake-harness-execution-denied-route-preflight-ready";
constexpr std::string_view execution_denied_source_v289_state =
    "disabled-fake-harness-contract-upstream-echo-verification-ready";
constexpr std::string_view execution_denied_route_path = "/api/v1/audit/"
                                                         "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                         "disabled-fake-harness-execution-denied-route-preflight";
constexpr std::string_view execution_denied_source_v289_route_path =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-harness-contract-upstream-echo-"
    "verification";
constexpr std::string_view execution_denied_preflight_digest =
    "21ea6d7a6c641489048fc2540126e2f461909246fdd2129bcb0fc6d1ffbbf1cb";
constexpr std::string_view execution_denied_source_v289_digest =
    "e5b596b65f68e753459be163f8a58d1f677098e64b3be01d4e83737ef8c4d94f";
constexpr std::string_view execution_denied_preflight_mode =
    "disabled-fake-harness-execution-denied-route-preflight-only";
constexpr std::string_view execution_denied_route_surface = "audit-json-markdown-route";
constexpr std::string_view execution_denied_active_plan = "docs/plans2/v289-post-disabled-fake-harness-echo-roadmap.md";
constexpr std::string_view execution_denied_runtime_role = "runtime evidence provider only";
constexpr std::string_view execution_denied_boundary =
    "credential resolver disabled fake harness execution-denied non-participation receipt only; mini-kv echoes Node "
    "v290 execution-denied route preflight and preflightDigest, does not implement or run a fake harness runtime, does "
    "not instantiate real or fake secret providers or resolver clients, does not read, store, or render credential "
    "values, does not parse or render raw endpoint URLs, does not send HTTP/TCP or other external requests, does not "
    "connect managed audit, does not write storage, approval ledger, or schema state, does not execute "
    "LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view execution_denied_node_action =
    "verify mini-kv v128 execution-denied non-participation with Java quality evidence before Node v291 upstream echo "
    "verification";

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
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_fake_harness_execution_"
           "denied_route_preflight\":true}";
}

void append_non_participation_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back({"execution_denied_non_participation_receipt_only", json_bool(true)});
    fields.push_back({"execution_denied_route_preflight_only", json_bool(true)});
    fields.push_back({"consumes_node_v290_execution_denied_route_preflight", json_bool(true)});
    fields.push_back({"ready_for_node_v291_upstream_echo_verification", json_bool(true)});
    fields.push_back({"ready_for_java_v127_mini_kv_v128_parallel_evidence", json_bool(true)});
    fields.push_back({"ready_for_managed_audit_resolver_implementation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)});
    fields.push_back({"ready_for_production_audit", json_bool(false)});
    fields.push_back({"ready_for_production_window", json_bool(false)});
    fields.push_back({"ready_for_production_operations", json_bool(false)});
    fields.push_back({"real_resolver_implementation_allowed", json_bool(false)});
    fields.push_back({"test_only_fake_harness_allowed", json_bool(false)});
    fields.push_back({"test_only_fake_harness_execution_allowed", json_bool(false)});
    fields.push_back({"fake_harness_runtime_enabled", json_bool(false)});
    fields.push_back({"fake_harness_invocation_allowed", json_bool(false)});
    fields.push_back({"fake_harness_runtime_implemented", json_bool(false)});
    fields.push_back({"fake_harness_runtime_invoked", json_bool(false)});
    fields.push_back({"execute_fake_harness_runtime_allowed", json_bool(false)});
    fields.push_back({"actual_execution_attempt_count", json_integer(0)});
    fields.push_back({"credential_resolver_implemented", json_bool(false)});
    fields.push_back({"credential_resolver_invoked", json_bool(false)});
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_resolver_client_instantiated", json_bool(false)});
    fields.push_back({"provider_client_instantiation_allowed", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_read", json_bool(false)});
    fields.push_back({"credential_value_provided", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_stored", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"credential_value_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parse_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_render_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_provided", json_bool(false)});
    fields.push_back({"raw_endpoint_url_included", json_bool(false)});
    fields.push_back({"external_request_allowed", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    fields.push_back({"http_tcp_dial_allowed", json_bool(false)});
    fields.push_back({"connects_managed_audit", json_bool(false)});
    fields.push_back({"reads_managed_audit_credential", json_bool(false)});
    fields.push_back({"stores_managed_audit_credential", json_bool(false)});
    fields.push_back({"managed_audit_store", json_bool(false)});
    fields.push_back({"managed_audit_storage_backend", json_bool(false)});
    fields.push_back({"sandbox_audit_storage_backend", json_bool(false)});
    fields.push_back({"storage_write_allowed", json_bool(false)});
    fields.push_back({"write_commands_executed", json_bool(false)});
    fields.push_back({"admin_commands_executed", json_bool(false)});
    fields.push_back({"runtime_write_observed", json_bool(false)});
    fields.push_back({"approval_gate_required", json_bool(true)});
    fields.push_back({"approval_gate_satisfied", json_bool(false)});
    fields.push_back({"approval_ledger_write_allowed", json_bool(false)});
    fields.push_back({"approval_ledger_written", json_bool(false)});
    fields.push_back({"approval_ledger_write_executed", json_bool(false)});
    fields.push_back({"managed_audit_write_executed", json_bool(false)});
    fields.push_back({"production_record_written", json_bool(false)});
    fields.push_back({"schema_migration_allowed", json_bool(false)});
    fields.push_back({"schema_migration_executed", json_bool(false)});
    fields.push_back({"schema_rehearsal_execution_allowed", json_bool(false)});
    fields.push_back({"restore_execution_allowed", json_bool(false)});
    fields.push_back({"load_restore_compact_executed", json_bool(false)});
    fields.push_back({"setnxex_execution_allowed", json_bool(false)});
    fields.push_back({"node_auto_start_allowed", json_bool(false)});
    fields.push_back({"java_auto_start_allowed", json_bool(false)});
    fields.push_back({"mini_kv_auto_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start", json_bool(false)});
    fields.push_back({"audit_authoritative", json_bool(false)});
    fields.push_back({"order_authoritative", json_bool(false)});
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
    return receipt_digest("mini-kv-credential-resolver-disabled-fake-harness-execution-denied-non-participation",
                          parts);
}

std::string format_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v289_reference = json_object({
        {"source_version", json_string("Node v289")},
        {"profile_version", json_string(execution_denied_source_v289_profile_version)},
        {"verification_state", json_string(execution_denied_source_v289_state)},
        {"ready_for_disabled_fake_harness_contract_upstream_echo_verification", json_bool(true)},
        {"verification_digest", json_string(execution_denied_source_v289_digest)},
        {"source_span", json_string("Node v288 + Java v122-v126 + mini-kv v127")},
        {"source_check_count", json_integer(execution_denied_source_check_count)},
        {"source_passed_check_count", json_integer(execution_denied_source_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"java_evidence_ready", json_bool(true)},
        {"mini_kv_non_participation_ready", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
        {"ready_for_next_disabled_fake_harness_planning", json_bool(true)},
        {"ready_for_managed_audit_resolver_implementation", json_bool(false)},
        {"ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"ready_for_production_audit", json_bool(false)},
        {"ready_for_production_window", json_bool(false)},
        {"ready_for_production_operations", json_bool(false)},
        {"real_resolver_implementation_allowed", json_bool(false)},
        {"test_only_fake_harness_allowed", json_bool(false)},
        {"test_only_fake_harness_execution_allowed", json_bool(false)},
        {"fake_harness_runtime_enabled", json_bool(false)},
        {"fake_harness_invocation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"credential_value_provided", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"raw_endpoint_url_rendered", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"fake_secret_provider_instantiated", json_bool(false)},
        {"fake_resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });
    const auto execution_denied_route_preflight = json_object({
        {"preflight_digest", json_string(execution_denied_preflight_digest)},
        {"preflight_mode", json_string(execution_denied_preflight_mode)},
        {"source_span", json_string("Node v289")},
        {"route_surface", json_string(execution_denied_route_surface)},
        {"route_path", json_string(execution_denied_route_path)},
        {"http_method", json_string("GET")},
        {"format_modes", json_array({json_string("json"), json_string("markdown")})},
        {"route_registered", json_bool(true)},
        {"route_read_only", json_bool(true)},
        {"route_execution_denied", json_bool(true)},
        {"execution_denied_reason_count", json_integer(execution_denied_denial_reason_count)},
        {"simulated_attempt_count", json_integer(execution_denied_simulated_attempt_count)},
        {"denied_attempt_count", json_integer(execution_denied_denied_attempt_count)},
        {"actual_execution_attempt_count", json_integer(0)},
        {"approval_gate_required", json_bool(true)},
        {"approval_gate_satisfied", json_bool(false)},
        {"credential_value_read_allowed", json_bool(false)},
        {"raw_endpoint_url_parse_allowed", json_bool(false)},
        {"provider_client_instantiation_allowed", json_bool(false)},
        {"http_tcp_dial_allowed", json_bool(false)},
        {"ledger_write_allowed", json_bool(false)},
        {"schema_migration_allowed", json_bool(false)},
        {"fake_harness_runtime_implementation_allowed", json_bool(false)},
        {"fake_harness_runtime_invocation_allowed", json_bool(false)},
        {"automatic_upstream_start_allowed", json_bool(false)},
        {"denial_reasons", format_denial_reasons_json()},
        {"simulated_route_attempts", format_simulated_route_attempts_json()},
    });
    const auto summary = json_object({
        {"check_count", json_integer(execution_denied_check_count)},
        {"passed_check_count", json_integer(execution_denied_check_count)},
        {"simulated_attempt_count", json_integer(execution_denied_simulated_attempt_count)},
        {"denied_attempt_count", json_integer(execution_denied_denied_attempt_count)},
        {"actual_execution_attempt_count", json_integer(0)},
        {"denial_reason_count", json_integer(execution_denied_denial_reason_count)},
        {"source_check_count", json_integer(execution_denied_source_check_count)},
        {"source_passed_check_count", json_integer(execution_denied_source_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"production_blocker_count", json_integer(0)},
        {"warning_count", json_integer(execution_denied_warning_count)},
        {"recommendation_count", json_integer(execution_denied_recommendation_count)},
    });
    const auto evidence_endpoints = json_object({
        {"execution_denied_route_preflight_json", json_string(execution_denied_route_path)},
        {"execution_denied_route_preflight_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-"
                     "harness-execution-denied-route-preflight?format=markdown")},
        {"source_node_v289_json", json_string(execution_denied_source_v289_route_path)},
        {"source_node_v289_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-fake-"
                     "harness-contract-upstream-echo-verification?format=markdown")},
        {"active_plan", json_string(execution_denied_active_plan)},
        {"next_recommended_parallel", json_string("Java v127 + mini-kv v128")},
        {"next_node_verification", json_string("Node v291")},
    });
    const auto next_actions = json_array({
        json_string("Archive Node v290 as a read-only execution-denied route preflight."),
        json_string("Recommend parallel Java v127 quality stopgap plus mini-kv v128 non-participation receipt "
                    "after v290 is archived."),
        json_string("Keep Java v127 focused on LiveAggregationIntegrationTests split and mini-kv v128 focused on "
                    "execution-denied non-participation."),
        json_string("Do not instantiate fake harness runtime, fake providers, fake resolver clients, real providers, "
                    "or real resolver clients."),
        json_string("Do not read credential values, parse raw endpoint URLs, send HTTP/TCP, connect managed audit, "
                    "write ledgers, run schema migration, or auto-start upstream services."),
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-disabled-fake-harness-execution-denied-non-participation-"
                     "receipt.v1")},
        {"receipt_fixture_path", json_string(execution_denied_receipt_fixture_path)},
        {"source_preflight", json_string(execution_denied_source_preflight)},
        {"source_upstream_echo", json_string(execution_denied_source_echo)},
        {"consumer_hint", json_string(execution_denied_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(execution_denied_receipt_release_version)},
        {"current_artifact_path_hint", json_string(execution_denied_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(execution_denied_runtime_role)},
        {"source_profile_version", json_string(execution_denied_profile_version)},
        {"source_route_path", json_string(execution_denied_route_path)},
        {"source_preflight_state", json_string(execution_denied_preflight_state)},
        {"source_ready_for_execution_denied_route_preflight", json_bool(true)},
        {"source_read_only_route_preflight", json_bool(true)},
        {"source_execution_denied_route_preflight_only", json_bool(true)},
        {"source_consumes_node_v289_disabled_fake_harness_contract_upstream_echo_verification", json_bool(true)},
        {"source_ready_for_java_v127_mini_kv_v128_parallel_evidence", json_bool(true)},
        {"source_ready_for_managed_audit_resolver_implementation", json_bool(false)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_ready_for_production_audit", json_bool(false)},
        {"source_ready_for_production_window", json_bool(false)},
        {"source_ready_for_production_operations", json_bool(false)},
        {"source_real_resolver_implementation_allowed", json_bool(false)},
        {"source_test_only_fake_harness_allowed", json_bool(false)},
        {"source_test_only_fake_harness_execution_allowed", json_bool(false)},
        {"source_fake_harness_runtime_enabled", json_bool(false)},
        {"source_fake_harness_invocation_allowed", json_bool(false)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_credential_value_provided", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_raw_endpoint_url_rendered", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_secret_provider_instantiated", json_bool(false)},
        {"source_resolver_client_instantiated", json_bool(false)},
        {"source_fake_secret_provider_instantiated", json_bool(false)},
        {"source_fake_resolver_client_instantiated", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_approval_ledger_written", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_node_v289_reference", source_node_v289_reference},
        {"execution_denied_route_preflight", execution_denied_route_preflight},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", json_array({json_object({{"code", json_string("EXECUTION_DENIED_PREFLIGHT_IS_NOT_RUNTIME")},
                                              {"severity", json_string("warning")}}),
                                 json_object({{"code", json_string("JAVA_QUALITY_QUEUE_NOT_RUNTIME_ECHO")},
                                              {"severity", json_string("warning")}})})},
        {"recommendations", json_array({json_object({{"code", json_string("RUN_JAVA_V127_MINI_KV_V128_IN_PARALLEL")},
                                                     {"severity", json_string("recommendation")}}),
                                        json_object({{"code", json_string("WAIT_FOR_UPSTREAM_STATE_BEFORE_NODE_V291")},
                                                     {"severity", json_string("recommendation")}})})},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", json_array({json_string("Java v127"), json_string("mini-kv v128")})},
        {"next_actions", next_actions},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_digest(
             read_commands))},
    };
    append_non_participation_fields(fields);
    fields.push_back({"boundary", json_string(execution_denied_boundary)});
    fields.push_back({"node_action", json_string(execution_denied_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
