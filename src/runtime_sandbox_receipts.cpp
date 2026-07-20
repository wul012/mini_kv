#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_receipt_json_builder.hpp"
#include "runtime_sandbox_receipt_utils.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;
using sandbox_receipt_detail::append_closed_execution_tail;
using sandbox_receipt_detail::append_closed_write_fields;
using sandbox_receipt_detail::append_read_only_prefix;

constexpr std::string_view disabled_adapter_client_receipt_consumer =
    "Node v254 disabled adapter client upstream echo verification";
constexpr std::string_view disabled_adapter_client_receipt_fixture_path =
    "fixtures/release/disabled-adapter-client-non-participation-receipt.json";
constexpr std::string_view disabled_adapter_client_receipt_release_version = "v111";
constexpr std::string_view disabled_adapter_client_receipt_artifact_path_hint = "c/111/";
constexpr std::string_view disabled_adapter_client_source_precheck = "Node v252 disabled adapter client precheck";
constexpr std::string_view disabled_adapter_client_source_shell = "Node v253 test-only adapter shell contract";
constexpr std::string_view disabled_adapter_client_precheck_profile_version =
    "managed-audit-manual-sandbox-connection-disabled-adapter-client-precheck.v1";
constexpr std::string_view disabled_adapter_client_precheck_state = "disabled-adapter-client-precheck-ready";
constexpr std::string_view disabled_adapter_client_adapter_mode = "disabled-client-precheck-only";
constexpr std::string_view disabled_adapter_client_shell_profile_version =
    "managed-audit-manual-sandbox-connection-test-only-adapter-shell-contract.v1";
constexpr std::string_view disabled_adapter_client_shell_state = "test-only-adapter-shell-contract-ready";
constexpr std::string_view disabled_adapter_client_shell_mode = "test-only-fake-transport-contract";
constexpr std::string_view disabled_adapter_client_transport_kind = "fake-in-memory";
constexpr std::string_view disabled_adapter_client_opt_in_gate = "ORDEROPS_MANAGED_AUDIT_ADAPTER_CLIENT_ENABLED";
constexpr std::string_view disabled_adapter_client_endpoint_handle = "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view disabled_adapter_client_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view disabled_adapter_client_owner_approval_field =
    "ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID";
constexpr std::string_view disabled_adapter_client_timeout_field = "ORDEROPS_MANAGED_AUDIT_TIMEOUT_BUDGET_MS";
constexpr std::string_view disabled_adapter_client_runtime_role = "runtime evidence provider only";
constexpr std::string_view disabled_adapter_client_boundary =
    "disabled adapter client non-participation receipt only; mini-kv stays runtime evidence only, is not auto-started "
    "by Node v252/v253, does not write storage, does not read credential values, does not execute "
    "restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view disabled_adapter_client_node_action =
    "verify mini-kv non-participation with Node v252 disabled client precheck and Node v253 fake transport shell "
    "before Node v254 upstream echo verification";
constexpr int disabled_adapter_client_required_env_handle_count = 5;
constexpr int disabled_adapter_client_failure_class_count = 6;
constexpr int disabled_adapter_client_dry_run_response_field_count = 10;
constexpr int disabled_adapter_client_reused_no_go_condition_count = 8;
constexpr int disabled_adapter_client_request_shape_field_count = 8;
constexpr int disabled_adapter_client_response_shape_field_count = 9;
constexpr int disabled_adapter_client_failure_mapping_count = 6;
constexpr int disabled_adapter_client_guard_condition_count = 7;

constexpr std::string_view fake_transport_packet_receipt_consumer =
    "Node v257 fake transport packet upstream echo verification";
constexpr std::string_view fake_transport_packet_receipt_fixture_path =
    "fixtures/release/fake-transport-dry-run-packet-non-participation-receipt.json";
constexpr std::string_view fake_transport_packet_receipt_release_version = "v112";
constexpr std::string_view fake_transport_packet_receipt_artifact_path_hint = "c/112/";
constexpr std::string_view fake_transport_packet_source_packet =
    "Node v255 fake transport adapter dry-run verification packet";
constexpr std::string_view fake_transport_packet_source_archive =
    "Node v256 fake transport packet archive verification";
constexpr std::string_view fake_transport_packet_profile_version =
    "managed-audit-manual-sandbox-connection-fake-transport-adapter-dry-run-verification-packet.v1";
constexpr std::string_view fake_transport_packet_state = "fake-transport-adapter-dry-run-verification-packet-ready";
constexpr std::string_view fake_transport_packet_archive_state = "fake-transport-packet-archive-verification-ready";
constexpr std::string_view fake_transport_packet_mode = "fake-transport-adapter-dry-run-verification-only";
constexpr std::string_view fake_transport_packet_source_span = "Node v253 + Node v254";
constexpr std::string_view fake_transport_packet_request_id = "managed-audit-v255-fake-transport-dry-run";
constexpr std::string_view fake_transport_packet_operation = "managed-audit-sandbox-connection-dry-run";
constexpr std::string_view fake_transport_packet_transport_kind = "fake-in-memory";
constexpr std::string_view fake_transport_packet_credential_handle = "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view fake_transport_packet_endpoint_handle = "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view fake_transport_packet_owner_approval = "owner-approval-artifact-review-only";
constexpr std::string_view fake_transport_packet_response_status = "fake-transport-dry-run-accepted";
constexpr std::string_view fake_transport_packet_response_code = "TEST_ONLY_FAKE_TRANSPORT_DRY_RUN";
constexpr std::string_view fake_transport_packet_budget_source = "operator-review-field";
constexpr std::string_view fake_transport_packet_runtime_role = "runtime evidence provider only";
constexpr std::string_view fake_transport_packet_boundary =
    "fake transport dry-run packet non-participation receipt only; mini-kv stays runtime evidence only, is not "
    "auto-started by Node v255/v256, does not write storage, does not read credential values, does not send external "
    "requests, does not create temporary cleanup artifacts, does not execute restore/load/compact/SETNXEX, and does "
    "not become a managed audit storage backend";
constexpr std::string_view fake_transport_packet_node_action =
    "verify mini-kv non-participation with Node v255 fake transport dry-run packet and Node v256 archive verification "
    "before Node v257 upstream echo verification";
constexpr int fake_transport_packet_request_shape_field_count = 8;
constexpr int fake_transport_packet_response_shape_field_count = 9;
constexpr int fake_transport_packet_failure_mapping_count = 6;
constexpr int fake_transport_packet_timeout_budget_ms = 15000;
constexpr int fake_transport_packet_cleanup_artifact_count = 0;

constexpr std::string_view sandbox_endpoint_handle_receipt_consumer =
    "Node v259 sandbox endpoint handle upstream echo verification";
constexpr std::string_view sandbox_endpoint_handle_receipt_fixture_path =
    "fixtures/release/sandbox-endpoint-handle-non-participation-receipt.json";
constexpr std::string_view sandbox_endpoint_handle_receipt_release_version = "v113";
constexpr std::string_view sandbox_endpoint_handle_receipt_artifact_path_hint = "c/113/";
constexpr std::string_view sandbox_endpoint_handle_source_preflight =
    "Node v258 sandbox endpoint handle preflight review";
constexpr std::string_view sandbox_endpoint_handle_profile_version =
    "managed-audit-manual-sandbox-connection-sandbox-endpoint-handle-preflight-review.v1";
constexpr std::string_view sandbox_endpoint_handle_review_state = "sandbox-endpoint-handle-preflight-review-ready";
constexpr std::string_view sandbox_endpoint_handle_review_mode = "sandbox-endpoint-handle-preflight-review-only";
constexpr std::string_view sandbox_endpoint_handle_source_span = "Node v257";
constexpr std::string_view sandbox_endpoint_handle_endpoint_handle = "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_owner_approval = "owner-approval-artifact-review-only";
constexpr std::string_view sandbox_endpoint_handle_schema_rehearsal = "schema-migration-rehearsal-review-only";
constexpr std::string_view sandbox_endpoint_handle_operator_window_marker =
    "manual-sandbox-endpoint-window-review-only";
constexpr std::string_view sandbox_endpoint_handle_network_allowlist =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_NETWORK_ALLOWLIST_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_tls_policy = "ORDEROPS_MANAGED_AUDIT_SANDBOX_TLS_POLICY_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_redaction_policy =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_REDACTION_POLICY_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_runtime_role = "runtime evidence provider only";
constexpr std::string_view sandbox_endpoint_handle_boundary =
    "sandbox endpoint handle non-participation receipt only; mini-kv echoes Node v258 handle preflight boundaries, "
    "does not auto-start, does not write storage, does not read credential values, does not parse raw endpoint URLs, "
    "does not send external requests, does not execute restore/load/compact/SETNXEX, and does not become a managed "
    "audit storage backend";
constexpr std::string_view sandbox_endpoint_handle_node_action =
    "verify mini-kv non-participation with Node v258 sandbox endpoint handle preflight before Node v259 upstream echo "
    "verification";
constexpr int sandbox_endpoint_handle_required_review_item_count = 7;
constexpr int sandbox_endpoint_handle_completed_review_item_count = 7;
constexpr int sandbox_endpoint_handle_forbidden_operation_count = 7;
constexpr int sandbox_endpoint_handle_check_count = 19;
constexpr int sandbox_endpoint_handle_warning_count = 2;
constexpr int sandbox_endpoint_handle_recommendation_count = 2;
constexpr int sandbox_endpoint_handle_source_evidence_file_count = 6;
constexpr int sandbox_endpoint_handle_source_matched_snippet_count = 33;

} // namespace

std::string disabled_adapter_client_non_participation_receipt_digest(const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{disabled_adapter_client_receipt_release_version}},
        {std::string{disabled_adapter_client_receipt_artifact_path_hint}},
        {std::string{disabled_adapter_client_source_precheck}},
        {std::string{disabled_adapter_client_source_shell}},
        {std::string{disabled_adapter_client_precheck_profile_version}},
        {std::string{disabled_adapter_client_precheck_state}},
        {std::string{disabled_adapter_client_adapter_mode}},
        {std::string{disabled_adapter_client_shell_profile_version}},
        {std::string{disabled_adapter_client_shell_state}},
        {std::string{disabled_adapter_client_shell_mode}},
        {std::string{disabled_adapter_client_transport_kind}},
        {std::string{disabled_adapter_client_opt_in_gate}},
        {std::string{disabled_adapter_client_endpoint_handle}},
        {std::string{disabled_adapter_client_credential_handle}},
        {std::string{disabled_adapter_client_owner_approval_field}},
        {std::string{disabled_adapter_client_timeout_field}},
        {std::to_string(disabled_adapter_client_required_env_handle_count)},
        {std::to_string(disabled_adapter_client_failure_class_count)},
        {std::to_string(disabled_adapter_client_dry_run_response_field_count)},
        {std::to_string(disabled_adapter_client_reused_no_go_condition_count)},
        {std::to_string(disabled_adapter_client_request_shape_field_count)},
        {std::to_string(disabled_adapter_client_response_shape_field_count)},
        {std::to_string(disabled_adapter_client_failure_mapping_count)},
        {std::to_string(disabled_adapter_client_guard_condition_count)},
        {"false"},
        {"false"},
        {"false"},
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
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return runtime_evidence::digest("mini-kv-disabled-adapter-client-non-participation", parts);
}

std::string
format_disabled_adapter_client_non_participation_receipt_json(const std::vector<std::string>& read_commands) {
    const auto dry_run_response_shape = json_object({
        {"ready_state", json_string("disabled-adapter-client-precheck-ready")},
        {"connection_attempted", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
    });
    const auto request_shape = json_object({
        {"credential_handle_only", json_bool(true)},
        {"credential_value_accepted", json_bool(false)},
        {"endpoint_handle_only", json_bool(true)},
        {"external_url_accepted", json_bool(false)},
        {"payload_may_contain_secrets", json_bool(false)},
    });
    const auto response_shape = json_object({
        {"fake_transport_response_only", json_bool(true)},
        {"connection_attempted", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"production_record_written", json_bool(false)},
    });
    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string("mini-kv-disabled-adapter-client-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(disabled_adapter_client_receipt_fixture_path)},
        {"source_precheck", json_string(disabled_adapter_client_source_precheck)},
        {"source_shell", json_string(disabled_adapter_client_source_shell)},
        {"consumer_hint", json_string(disabled_adapter_client_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(disabled_adapter_client_receipt_release_version)},
        {"current_artifact_path_hint", json_string(disabled_adapter_client_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(disabled_adapter_client_runtime_role)},
        {"source_precheck_profile_version", json_string(disabled_adapter_client_precheck_profile_version)},
        {"source_precheck_state", json_string(disabled_adapter_client_precheck_state)},
        {"source_adapter_mode", json_string(disabled_adapter_client_adapter_mode)},
        {"source_required_env_handle_count", json_integer(disabled_adapter_client_required_env_handle_count)},
        {"source_failure_class_count", json_integer(disabled_adapter_client_failure_class_count)},
        {"source_dry_run_response_field_count", json_integer(disabled_adapter_client_dry_run_response_field_count)},
        {"source_reused_no_go_condition_count", json_integer(disabled_adapter_client_reused_no_go_condition_count)},
        {"source_ready_for_disabled_adapter_client_precheck", json_bool(true)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_read_only_precheck", json_bool(true)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_client_implementation_status", json_string("not-implemented")},
        {"source_client_may_be_instantiated", json_bool(false)},
        {"source_external_request_may_be_sent", json_bool(false)},
        {"source_credential_value_may_be_loaded", json_bool(false)},
        {"source_opt_in_gate_required", json_bool(true)},
        {"source_opt_in_gate", json_string(disabled_adapter_client_opt_in_gate)},
        {"source_opt_in_gate_default", json_string("false")},
        {"source_precheck_treats_enabled_as_blocked", json_bool(true)},
        {"required_env_handles", json_array({json_string(disabled_adapter_client_opt_in_gate),
                                             json_string(disabled_adapter_client_endpoint_handle),
                                             json_string(disabled_adapter_client_credential_handle),
                                             json_string(disabled_adapter_client_owner_approval_field),
                                             json_string(disabled_adapter_client_timeout_field)})},
        {"failure_taxonomy_codes",
         json_array({json_string("ADAPTER_CLIENT_DISABLED"), json_string("CREDENTIAL_HANDLE_MISSING"),
                     json_string("CREDENTIAL_VALUE_REQUESTED"), json_string("ENDPOINT_HANDLE_MISSING"),
                     json_string("SCHEMA_REHEARSAL_MISSING"), json_string("MANUAL_WINDOW_NOT_OPEN")})},
        {"dry_run_response_shape", dry_run_response_shape},
        {"source_shell_profile_version", json_string(disabled_adapter_client_shell_profile_version)},
        {"source_shell_state", json_string(disabled_adapter_client_shell_state)},
        {"source_shell_mode", json_string(disabled_adapter_client_shell_mode)},
        {"source_transport_kind", json_string(disabled_adapter_client_transport_kind)},
        {"source_request_shape_field_count", json_integer(disabled_adapter_client_request_shape_field_count)},
        {"source_response_shape_field_count", json_integer(disabled_adapter_client_response_shape_field_count)},
        {"source_failure_mapping_count", json_integer(disabled_adapter_client_failure_mapping_count)},
        {"source_guard_condition_count", json_integer(disabled_adapter_client_guard_condition_count)},
        {"source_fake_transport_only", json_bool(true)},
        {"source_real_client_implemented", json_bool(false)},
        {"source_real_transport_allowed", json_bool(false)},
        {"source_client_may_be_instantiated_for_production", json_bool(false)},
        {"source_fake_transport_probe_covered", json_bool(true)},
        {"source_fake_transport_probe_no_external_request", json_bool(true)},
        {"source_fake_transport_probe_no_credential_read", json_bool(true)},
        {"source_fake_transport_probe_no_production_write", json_bool(true)},
        {"request_shape", request_shape},
        {"response_shape", response_shape},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest", json_string(disabled_adapter_client_non_participation_receipt_digest(read_commands))},
    };
    append_read_only_prefix(fields);
    append_closed_write_fields(fields);
    fields.push_back({"sandbox_managed_audit_state_write_allowed", json_bool(false)});
    fields.push_back({"credential_value_required", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    append_closed_execution_tail(fields);
    fields.push_back({"boundary", json_string(disabled_adapter_client_boundary)});
    fields.push_back({"node_action", json_string(disabled_adapter_client_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

std::string
fake_transport_dry_run_packet_non_participation_receipt_digest(const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{fake_transport_packet_receipt_release_version}},
        {std::string{fake_transport_packet_receipt_artifact_path_hint}},
        {std::string{fake_transport_packet_source_packet}},
        {std::string{fake_transport_packet_source_archive}},
        {std::string{fake_transport_packet_profile_version}},
        {std::string{fake_transport_packet_state}},
        {std::string{fake_transport_packet_archive_state}},
        {std::string{fake_transport_packet_mode}},
        {std::string{fake_transport_packet_source_span}},
        {std::string{fake_transport_packet_request_id}},
        {std::string{fake_transport_packet_operation}},
        {std::string{fake_transport_packet_transport_kind}},
        {std::string{fake_transport_packet_credential_handle}},
        {std::string{fake_transport_packet_endpoint_handle}},
        {std::string{fake_transport_packet_owner_approval}},
        {std::string{fake_transport_packet_response_status}},
        {std::string{fake_transport_packet_response_code}},
        {std::string{fake_transport_packet_budget_source}},
        {std::to_string(fake_transport_packet_request_shape_field_count)},
        {std::to_string(fake_transport_packet_response_shape_field_count)},
        {std::to_string(fake_transport_packet_failure_mapping_count)},
        {std::to_string(fake_transport_packet_timeout_budget_ms)},
        {std::to_string(fake_transport_packet_cleanup_artifact_count)},
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
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return runtime_evidence::digest("mini-kv-fake-transport-dry-run-packet-non-participation", parts);
}

std::string
format_fake_transport_dry_run_packet_non_participation_receipt_json(const std::vector<std::string>& read_commands) {
    const auto request = json_object({
        {"request_id", json_string(fake_transport_packet_request_id)},
        {"operation", json_string(fake_transport_packet_operation)},
        {"transport_kind", json_string(fake_transport_packet_transport_kind)},
        {"credential_handle", json_string(fake_transport_packet_credential_handle)},
        {"endpoint_handle", json_string(fake_transport_packet_endpoint_handle)},
        {"owner_approval_artifact_id", json_string(fake_transport_packet_owner_approval)},
        {"timeout_budget_ms", json_integer(fake_transport_packet_timeout_budget_ms)},
        {"dry_run", json_bool(true)},
        {"fake_transport_only", json_bool(true)},
        {"credential_value_included", json_bool(false)},
        {"raw_endpoint_url_included", json_bool(false)},
        {"payload_may_contain_secrets", json_bool(false)},
        {"request_shape_field_count", json_integer(fake_transport_packet_request_shape_field_count)},
    });
    const auto response = json_object({
        {"request_id", json_string(fake_transport_packet_request_id)},
        {"status", json_string(fake_transport_packet_response_status)},
        {"code", json_string(fake_transport_packet_response_code)},
        {"fake_transport_only", json_bool(true)},
        {"timeout_budget_ms", json_integer(fake_transport_packet_timeout_budget_ms)},
        {"connection_attempted", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"production_record_written", json_bool(false)},
        {"response_shape_field_count", json_integer(fake_transport_packet_response_shape_field_count)},
    });
    const auto timeout_budget = json_object({
        {"timeout_budget_ms", json_integer(fake_transport_packet_timeout_budget_ms)},
        {"finite_budget", json_bool(true)},
        {"budget_source", json_string(fake_transport_packet_budget_source)},
        {"budget_spent", json_bool(false)},
        {"timer_started", json_bool(false)},
        {"timeout_classifiable", json_bool(true)},
    });
    const auto failure_mapping_verification = json_object({
        {"source_failure_mapping_count", json_integer(fake_transport_packet_failure_mapping_count)},
        {"mapped_failure_count", json_integer(fake_transport_packet_failure_mapping_count)},
        {"all_failures_non_retryable", json_bool(true)},
        {"credential_value_request_still_blocked", json_bool(true)},
        {"manual_window_closed_still_blocked", json_bool(true)},
        {"failure_mapping_covered", json_bool(true)},
    });
    const auto cleanup = json_object({
        {"in_memory_only", json_bool(true)},
        {"temporary_directory_created", json_bool(false)},
        {"temporary_file_created", json_bool(false)},
        {"cleanup_required", json_bool(false)},
        {"cleanup_artifact_count", json_integer(fake_transport_packet_cleanup_artifact_count)},
        {"cleanup_verified", json_bool(true)},
        {"node_service_started_by_packet", json_bool(false)},
    });
    const auto boundaries = json_object({
        {"connection_attempted", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"production_record_written", json_bool(false)},
        {"java_started", json_bool(false)},
        {"mini_kv_started", json_bool(false)},
        {"external_audit_service_started", json_bool(false)},
    });
    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string("mini-kv-fake-transport-dry-run-packet-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(fake_transport_packet_receipt_fixture_path)},
        {"source_packet", json_string(fake_transport_packet_source_packet)},
        {"source_archive_verification", json_string(fake_transport_packet_source_archive)},
        {"consumer_hint", json_string(fake_transport_packet_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(fake_transport_packet_receipt_release_version)},
        {"current_artifact_path_hint", json_string(fake_transport_packet_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(fake_transport_packet_runtime_role)},
        {"source_packet_profile_version", json_string(fake_transport_packet_profile_version)},
        {"source_packet_state", json_string(fake_transport_packet_state)},
        {"source_archive_state", json_string(fake_transport_packet_archive_state)},
        {"source_archive_reruns_fake_transport_behavior", json_bool(false)},
        {"source_packet_mode", json_string(fake_transport_packet_mode)},
        {"source_span", json_string(fake_transport_packet_source_span)},
        {"source_ready_for_fake_transport_packet", json_bool(true)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_fake_transport_only", json_bool(true)},
        {"source_dry_run_only", json_bool(true)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_request_shape_field_count", json_integer(fake_transport_packet_request_shape_field_count)},
        {"source_response_shape_field_count", json_integer(fake_transport_packet_response_shape_field_count)},
        {"source_failure_mapping_count", json_integer(fake_transport_packet_failure_mapping_count)},
        {"source_timeout_budget_ms", json_integer(fake_transport_packet_timeout_budget_ms)},
        {"source_cleanup_artifact_count", json_integer(fake_transport_packet_cleanup_artifact_count)},
        {"source_budget_spent", json_bool(false)},
        {"source_timer_started", json_bool(false)},
        {"source_cleanup_in_memory_only", json_bool(true)},
        {"source_temporary_directory_created", json_bool(false)},
        {"source_temporary_file_created", json_bool(false)},
        {"source_cleanup_required", json_bool(false)},
        {"source_cleanup_verified", json_bool(true)},
        {"source_node_service_started_by_packet", json_bool(false)},
        {"source_java_started", json_bool(false)},
        {"source_mini_kv_started", json_bool(false)},
        {"source_external_audit_service_started", json_bool(false)},
        {"request", request},
        {"response", response},
        {"timeout_budget", timeout_budget},
        {"failure_mapping_verification", failure_mapping_verification},
        {"cleanup", cleanup},
        {"boundaries", boundaries},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest", json_string(fake_transport_dry_run_packet_non_participation_receipt_digest(read_commands))},
    };
    append_read_only_prefix(fields);
    fields.push_back({"external_audit_service_auto_start_allowed", json_bool(false)});
    append_closed_write_fields(fields);
    fields.push_back({"sandbox_managed_audit_state_write_allowed", json_bool(false)});
    fields.push_back({"credential_value_required", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"raw_endpoint_url_included", json_bool(false)});
    fields.push_back({"payload_may_contain_secrets", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    fields.push_back({"temporary_directory_created", json_bool(false)});
    fields.push_back({"temporary_file_created", json_bool(false)});
    fields.push_back({"cleanup_artifact_count", json_integer(0)});
    append_closed_execution_tail(fields);
    fields.push_back({"boundary", json_string(fake_transport_packet_boundary)});
    fields.push_back({"node_action", json_string(fake_transport_packet_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

std::string sandbox_endpoint_handle_non_participation_receipt_digest(const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{sandbox_endpoint_handle_receipt_release_version}},
        {std::string{sandbox_endpoint_handle_receipt_artifact_path_hint}},
        {std::string{sandbox_endpoint_handle_source_preflight}},
        {std::string{sandbox_endpoint_handle_profile_version}},
        {std::string{sandbox_endpoint_handle_review_state}},
        {std::string{sandbox_endpoint_handle_review_mode}},
        {std::string{sandbox_endpoint_handle_source_span}},
        {std::string{sandbox_endpoint_handle_endpoint_handle}},
        {std::string{sandbox_endpoint_handle_credential_handle}},
        {std::string{sandbox_endpoint_handle_owner_approval}},
        {std::string{sandbox_endpoint_handle_schema_rehearsal}},
        {std::string{sandbox_endpoint_handle_operator_window_marker}},
        {std::string{sandbox_endpoint_handle_network_allowlist}},
        {std::string{sandbox_endpoint_handle_tls_policy}},
        {std::string{sandbox_endpoint_handle_redaction_policy}},
        {std::to_string(sandbox_endpoint_handle_required_review_item_count)},
        {std::to_string(sandbox_endpoint_handle_completed_review_item_count)},
        {std::to_string(sandbox_endpoint_handle_forbidden_operation_count)},
        {std::to_string(sandbox_endpoint_handle_check_count)},
        {std::to_string(sandbox_endpoint_handle_warning_count)},
        {std::to_string(sandbox_endpoint_handle_recommendation_count)},
        {"true"},
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
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return runtime_evidence::digest("mini-kv-sandbox-endpoint-handle-non-participation", parts);
}

std::string
format_sandbox_endpoint_handle_non_participation_receipt_json(const std::vector<std::string>& read_commands) {
    const auto preflight_review = json_object({
        {"endpoint_handle", json_string(sandbox_endpoint_handle_endpoint_handle)},
        {"credential_handle", json_string(sandbox_endpoint_handle_credential_handle)},
        {"owner_approval_artifact_id", json_string(sandbox_endpoint_handle_owner_approval)},
        {"schema_rehearsal_id", json_string(sandbox_endpoint_handle_schema_rehearsal)},
        {"operator_window_marker", json_string(sandbox_endpoint_handle_operator_window_marker)},
        {"endpoint_handle_reviewed", json_bool(true)},
        {"credential_handle_reviewed", json_bool(true)},
        {"owner_approval_artifact_reviewed", json_bool(true)},
        {"review_value_only", json_bool(false)},
    });
    const auto network_allowlist_review = json_object({
        {"review_required", json_bool(true)},
        {"allowlist_handle", json_string(sandbox_endpoint_handle_network_allowlist)},
        {"raw_host_included", json_bool(false)},
        {"cidr_included", json_bool(false)},
        {"reviewed", json_bool(true)},
    });
    const auto tls_policy_review = json_object({
        {"review_required", json_bool(true)},
        {"policy_handle", json_string(sandbox_endpoint_handle_tls_policy)},
        {"certificate_material_included", json_bool(false)},
        {"private_key_included", json_bool(false)},
        {"reviewed", json_bool(true)},
    });
    const auto redaction_policy = json_object({
        {"review_required", json_bool(true)},
        {"policy_handle", json_string(sandbox_endpoint_handle_redaction_policy)},
        {"credential_value_redacted", json_bool(true)},
        {"raw_endpoint_url_redacted", json_bool(true)},
        {"payload_secret_redacted", json_bool(true)},
        {"reviewed", json_bool(true)},
    });
    const auto operator_window = json_object({
        {"manual_window_required", json_bool(true)},
        {"window_open", json_bool(false)},
        {"execution_blocked_until_window_open", json_bool(true)},
        {"operator_identity_required", json_bool(true)},
        {"approval_correlation_required", json_bool(true)},
        {"reviewed", json_bool(true)},
    });
    const auto checks = json_object({
        {"endpoint_handle_only", json_bool(true)},
        {"credential_handle_only", json_bool(true)},
        {"owner_approval_artifact_present", json_bool(true)},
        {"network_allowlist_review_ready", json_bool(true)},
        {"tls_policy_review_ready", json_bool(true)},
        {"redaction_policy_ready", json_bool(true)},
        {"operator_window_review_ready", json_bool(true)},
        {"no_raw_endpoint_url_parsed", json_bool(true)},
        {"no_raw_endpoint_url_included", json_bool(true)},
        {"no_credential_value_read", json_bool(true)},
        {"no_external_request_sent", json_bool(true)},
        {"no_schema_migration_executed", json_bool(true)},
        {"no_upstream_auto_start", json_bool(true)},
        {"upstream_actions_still_disabled", json_bool(true)},
        {"production_audit_still_blocked", json_bool(true)},
        {"production_window_still_blocked", json_bool(true)},
    });
    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string("mini-kv-sandbox-endpoint-handle-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(sandbox_endpoint_handle_receipt_fixture_path)},
        {"source_preflight", json_string(sandbox_endpoint_handle_source_preflight)},
        {"consumer_hint", json_string(sandbox_endpoint_handle_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(sandbox_endpoint_handle_receipt_release_version)},
        {"current_artifact_path_hint", json_string(sandbox_endpoint_handle_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(sandbox_endpoint_handle_runtime_role)},
        {"source_preflight_profile_version", json_string(sandbox_endpoint_handle_profile_version)},
        {"source_review_state", json_string(sandbox_endpoint_handle_review_state)},
        {"source_review_mode", json_string(sandbox_endpoint_handle_review_mode)},
        {"source_span", json_string(sandbox_endpoint_handle_source_span)},
        {"source_ready_for_sandbox_endpoint_handle_preflight_review", json_bool(true)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_read_only_preflight_review", json_bool(true)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_raw_endpoint_url_included", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_required_review_item_count", json_integer(sandbox_endpoint_handle_required_review_item_count)},
        {"source_completed_review_item_count", json_integer(sandbox_endpoint_handle_completed_review_item_count)},
        {"source_forbidden_operation_count", json_integer(sandbox_endpoint_handle_forbidden_operation_count)},
        {"source_check_count", json_integer(sandbox_endpoint_handle_check_count)},
        {"source_passed_check_count", json_integer(sandbox_endpoint_handle_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"source_warning_count", json_integer(sandbox_endpoint_handle_warning_count)},
        {"source_recommendation_count", json_integer(sandbox_endpoint_handle_recommendation_count)},
        {"source_node_v257_ready", json_bool(true)},
        {"source_node_v257_boundaries_aligned", json_bool(true)},
        {"source_node_v257_evidence_file_count", json_integer(sandbox_endpoint_handle_source_evidence_file_count)},
        {"source_node_v257_matched_snippet_count", json_integer(sandbox_endpoint_handle_source_matched_snippet_count)},
        {"source_node_v257_ready_for_node_v258_preflight_review", json_bool(true)},
        {"source_upstream_actions_still_disabled", json_bool(true)},
        {"preflight_review", preflight_review},
        {"network_allowlist_review", network_allowlist_review},
        {"tls_policy_review", tls_policy_review},
        {"redaction_policy", redaction_policy},
        {"operator_window", operator_window},
        {"required_review_items",
         json_array({json_string("endpoint handle review"), json_string("credential handle review"),
                     json_string("owner approval artifact review"), json_string("network allowlist review"),
                     json_string("TLS policy review"), json_string("redaction policy review"),
                     json_string("operator window review")})},
        {"forbidden_operations",
         json_array({json_string("read credential value"), json_string("parse raw endpoint URL"),
                     json_string("send real managed audit request"), json_string("execute schema migration"),
                     json_string("write approval ledger"), json_string("start Java or mini-kv"),
                     json_string("promote mini-kv to managed audit storage backend")})},
        {"next_required_echo_versions", json_array({json_string("Java v104"), json_string("mini-kv v113")})},
        {"checks", checks},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest", json_string(sandbox_endpoint_handle_non_participation_receipt_digest(read_commands))},
    };
    append_read_only_prefix(fields);
    fields.push_back({"external_audit_service_auto_start_allowed", json_bool(false)});
    append_closed_write_fields(fields);
    fields.push_back({"approval_ledger_write_allowed", json_bool(false)});
    fields.push_back({"approval_ledger_write_executed", json_bool(false)});
    fields.push_back({"sandbox_managed_audit_state_write_allowed", json_bool(false)});
    fields.push_back({"credential_value_required", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_included", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    append_closed_execution_tail(fields);
    fields.push_back({"boundary", json_string(sandbox_endpoint_handle_boundary)});
    fields.push_back({"node_action", json_string(sandbox_endpoint_handle_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
