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

constexpr std::string_view disabled_adapter_client_receipt_consumer =
    "Node v254 disabled adapter client upstream echo verification";
constexpr std::string_view disabled_adapter_client_receipt_fixture_path =
    "fixtures/release/disabled-adapter-client-non-participation-receipt.json";
constexpr std::string_view disabled_adapter_client_receipt_release_version = "v111";
constexpr std::string_view disabled_adapter_client_receipt_artifact_path_hint = "c/111/";
constexpr std::string_view disabled_adapter_client_source_precheck =
    "Node v252 disabled adapter client precheck";
constexpr std::string_view disabled_adapter_client_source_shell =
    "Node v253 test-only adapter shell contract";
constexpr std::string_view disabled_adapter_client_precheck_profile_version =
    "managed-audit-manual-sandbox-connection-disabled-adapter-client-precheck.v1";
constexpr std::string_view disabled_adapter_client_precheck_state =
    "disabled-adapter-client-precheck-ready";
constexpr std::string_view disabled_adapter_client_adapter_mode =
    "disabled-client-precheck-only";
constexpr std::string_view disabled_adapter_client_shell_profile_version =
    "managed-audit-manual-sandbox-connection-test-only-adapter-shell-contract.v1";
constexpr std::string_view disabled_adapter_client_shell_state =
    "test-only-adapter-shell-contract-ready";
constexpr std::string_view disabled_adapter_client_shell_mode =
    "test-only-fake-transport-contract";
constexpr std::string_view disabled_adapter_client_transport_kind = "fake-in-memory";
constexpr std::string_view disabled_adapter_client_opt_in_gate =
    "ORDEROPS_MANAGED_AUDIT_ADAPTER_CLIENT_ENABLED";
constexpr std::string_view disabled_adapter_client_endpoint_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view disabled_adapter_client_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view disabled_adapter_client_owner_approval_field =
    "ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID";
constexpr std::string_view disabled_adapter_client_timeout_field =
    "ORDEROPS_MANAGED_AUDIT_TIMEOUT_BUDGET_MS";
constexpr std::string_view disabled_adapter_client_runtime_role =
    "runtime evidence provider only";
constexpr std::string_view disabled_adapter_client_boundary =
    "disabled adapter client non-participation receipt only; mini-kv stays runtime evidence only, is not auto-started by Node v252/v253, does not write storage, does not read credential values, does not execute restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view disabled_adapter_client_node_action =
    "verify mini-kv non-participation with Node v252 disabled client precheck and Node v253 fake transport shell before Node v254 upstream echo verification";
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
constexpr std::string_view fake_transport_packet_state =
    "fake-transport-adapter-dry-run-verification-packet-ready";
constexpr std::string_view fake_transport_packet_archive_state =
    "fake-transport-packet-archive-verification-ready";
constexpr std::string_view fake_transport_packet_mode =
    "fake-transport-adapter-dry-run-verification-only";
constexpr std::string_view fake_transport_packet_source_span = "Node v253 + Node v254";
constexpr std::string_view fake_transport_packet_request_id =
    "managed-audit-v255-fake-transport-dry-run";
constexpr std::string_view fake_transport_packet_operation =
    "managed-audit-sandbox-connection-dry-run";
constexpr std::string_view fake_transport_packet_transport_kind = "fake-in-memory";
constexpr std::string_view fake_transport_packet_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view fake_transport_packet_endpoint_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view fake_transport_packet_owner_approval =
    "owner-approval-artifact-review-only";
constexpr std::string_view fake_transport_packet_response_status =
    "fake-transport-dry-run-accepted";
constexpr std::string_view fake_transport_packet_response_code =
    "TEST_ONLY_FAKE_TRANSPORT_DRY_RUN";
constexpr std::string_view fake_transport_packet_budget_source = "operator-review-field";
constexpr std::string_view fake_transport_packet_runtime_role =
    "runtime evidence provider only";
constexpr std::string_view fake_transport_packet_boundary =
    "fake transport dry-run packet non-participation receipt only; mini-kv stays runtime evidence only, is not auto-started by Node v255/v256, does not write storage, does not read credential values, does not send external requests, does not create temporary cleanup artifacts, does not execute restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view fake_transport_packet_node_action =
    "verify mini-kv non-participation with Node v255 fake transport dry-run packet and Node v256 archive verification before Node v257 upstream echo verification";
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
constexpr std::string_view sandbox_endpoint_handle_review_state =
    "sandbox-endpoint-handle-preflight-review-ready";
constexpr std::string_view sandbox_endpoint_handle_review_mode =
    "sandbox-endpoint-handle-preflight-review-only";
constexpr std::string_view sandbox_endpoint_handle_source_span = "Node v257";
constexpr std::string_view sandbox_endpoint_handle_endpoint_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_owner_approval =
    "owner-approval-artifact-review-only";
constexpr std::string_view sandbox_endpoint_handle_schema_rehearsal =
    "schema-migration-rehearsal-review-only";
constexpr std::string_view sandbox_endpoint_handle_operator_window_marker =
    "manual-sandbox-endpoint-window-review-only";
constexpr std::string_view sandbox_endpoint_handle_network_allowlist =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_NETWORK_ALLOWLIST_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_tls_policy =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_TLS_POLICY_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_redaction_policy =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_REDACTION_POLICY_HANDLE";
constexpr std::string_view sandbox_endpoint_handle_runtime_role =
    "runtime evidence provider only";
constexpr std::string_view sandbox_endpoint_handle_boundary =
    "sandbox endpoint handle non-participation receipt only; mini-kv echoes Node v258 handle preflight boundaries, does not auto-start, does not write storage, does not read credential values, does not parse raw endpoint URLs, does not send external requests, does not execute restore/load/compact/SETNXEX, and does not become a managed audit storage backend";
constexpr std::string_view sandbox_endpoint_handle_node_action =
    "verify mini-kv non-participation with Node v258 sandbox endpoint handle preflight before Node v259 upstream echo verification";
constexpr int sandbox_endpoint_handle_required_review_item_count = 7;
constexpr int sandbox_endpoint_handle_completed_review_item_count = 7;
constexpr int sandbox_endpoint_handle_forbidden_operation_count = 7;
constexpr int sandbox_endpoint_handle_check_count = 19;
constexpr int sandbox_endpoint_handle_warning_count = 2;
constexpr int sandbox_endpoint_handle_recommendation_count = 2;
constexpr int sandbox_endpoint_handle_source_evidence_file_count = 6;
constexpr int sandbox_endpoint_handle_source_matched_snippet_count = 33;

} // namespace

std::string disabled_adapter_client_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
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
    return receipt_digest("mini-kv-disabled-adapter-client-non-participation", parts);
}

std::string format_disabled_adapter_client_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-disabled-adapter-client-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(disabled_adapter_client_receipt_fixture_path) +
           ",\"source_precheck\":" + field_string(disabled_adapter_client_source_precheck) +
           ",\"source_shell\":" + field_string(disabled_adapter_client_source_shell) +
           ",\"consumer_hint\":" + field_string(disabled_adapter_client_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" +
           field_string(disabled_adapter_client_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(disabled_adapter_client_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(disabled_adapter_client_runtime_role) +
           ",\"source_precheck_profile_version\":" +
           field_string(disabled_adapter_client_precheck_profile_version) +
           ",\"source_precheck_state\":" + field_string(disabled_adapter_client_precheck_state) +
           ",\"source_adapter_mode\":" + field_string(disabled_adapter_client_adapter_mode) +
           ",\"source_required_env_handle_count\":" +
           std::to_string(disabled_adapter_client_required_env_handle_count) +
           ",\"source_failure_class_count\":" +
           std::to_string(disabled_adapter_client_failure_class_count) +
           ",\"source_dry_run_response_field_count\":" +
           std::to_string(disabled_adapter_client_dry_run_response_field_count) +
           ",\"source_reused_no_go_condition_count\":" +
           std::to_string(disabled_adapter_client_reused_no_go_condition_count) +
           ",\"source_ready_for_disabled_adapter_client_precheck\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_read_only_precheck\":true"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_client_implementation_status\":\"not-implemented\""
           ",\"source_client_may_be_instantiated\":false"
           ",\"source_external_request_may_be_sent\":false"
           ",\"source_credential_value_may_be_loaded\":false"
           ",\"source_opt_in_gate_required\":true"
           ",\"source_opt_in_gate\":" + field_string(disabled_adapter_client_opt_in_gate) +
           ",\"source_opt_in_gate_default\":\"false\""
           ",\"source_precheck_treats_enabled_as_blocked\":true"
           ",\"required_env_handles\":[" +
           field_string(disabled_adapter_client_opt_in_gate) + "," +
           field_string(disabled_adapter_client_endpoint_handle) + "," +
           field_string(disabled_adapter_client_credential_handle) + "," +
           field_string(disabled_adapter_client_owner_approval_field) + "," +
           field_string(disabled_adapter_client_timeout_field) + "]"
           ",\"failure_taxonomy_codes\":[\"ADAPTER_CLIENT_DISABLED\","
           "\"CREDENTIAL_HANDLE_MISSING\",\"CREDENTIAL_VALUE_REQUESTED\","
           "\"ENDPOINT_HANDLE_MISSING\",\"SCHEMA_REHEARSAL_MISSING\","
           "\"MANUAL_WINDOW_NOT_OPEN\"]"
           ",\"dry_run_response_shape\":{\"ready_state\":\"disabled-adapter-client-precheck-ready\","
           "\"connection_attempted\":false,\"credential_value_read\":false,"
           "\"external_request_sent\":false,\"schema_migration_executed\":false}"
           ",\"source_shell_profile_version\":" +
           field_string(disabled_adapter_client_shell_profile_version) +
           ",\"source_shell_state\":" + field_string(disabled_adapter_client_shell_state) +
           ",\"source_shell_mode\":" + field_string(disabled_adapter_client_shell_mode) +
           ",\"source_transport_kind\":" + field_string(disabled_adapter_client_transport_kind) +
           ",\"source_request_shape_field_count\":" +
           std::to_string(disabled_adapter_client_request_shape_field_count) +
           ",\"source_response_shape_field_count\":" +
           std::to_string(disabled_adapter_client_response_shape_field_count) +
           ",\"source_failure_mapping_count\":" +
           std::to_string(disabled_adapter_client_failure_mapping_count) +
           ",\"source_guard_condition_count\":" +
           std::to_string(disabled_adapter_client_guard_condition_count) +
           ",\"source_fake_transport_only\":true"
           ",\"source_real_client_implemented\":false"
           ",\"source_real_transport_allowed\":false"
           ",\"source_client_may_be_instantiated_for_production\":false"
           ",\"source_fake_transport_probe_covered\":true"
           ",\"source_fake_transport_probe_no_external_request\":true"
           ",\"source_fake_transport_probe_no_credential_read\":true"
           ",\"source_fake_transport_probe_no_production_write\":true"
           ",\"request_shape\":{\"credential_handle_only\":true,"
           "\"credential_value_accepted\":false,\"endpoint_handle_only\":true,"
           "\"external_url_accepted\":false,\"payload_may_contain_secrets\":false}"
           ",\"response_shape\":{\"fake_transport_response_only\":true,"
           "\"connection_attempted\":false,\"external_request_sent\":false,"
           "\"credential_value_read\":false,\"schema_migration_executed\":false,"
           "\"production_record_written\":false}"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(disabled_adapter_client_non_participation_receipt_digest(read_commands)) +
           ",\"read_only\":true,\"execution_allowed\":false,\"dry_run_only\":true,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"connection_execution_allowed\":false,"
           "\"write_commands_executed\":false,\"admin_commands_executed\":false,"
           "\"runtime_write_observed\":false,\"managed_audit_store\":false,"
           "\"managed_audit_storage_backend\":false,\"sandbox_audit_storage_backend\":false,"
           "\"storage_write_allowed\":false,\"managed_audit_write_executed\":false,"
           "\"production_record_written\":false,"
           "\"sandbox_managed_audit_state_write_allowed\":false,"
           "\"credential_value_required\":false,\"credential_value_read_allowed\":false,"
           "\"credential_value_loaded\":false,\"external_request_sent\":false,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"order_authoritative\":false,"
           "\"boundary\":" + field_string(disabled_adapter_client_boundary) +
           ",\"node_action\":" + field_string(disabled_adapter_client_node_action) + "}";
}

std::string fake_transport_dry_run_packet_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
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
    return receipt_digest("mini-kv-fake-transport-dry-run-packet-non-participation", parts);
}

std::string format_fake_transport_dry_run_packet_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-fake-transport-dry-run-packet-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(fake_transport_packet_receipt_fixture_path) +
           ",\"source_packet\":" + field_string(fake_transport_packet_source_packet) +
           ",\"source_archive_verification\":" + field_string(fake_transport_packet_source_archive) +
           ",\"consumer_hint\":" + field_string(fake_transport_packet_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" +
           field_string(fake_transport_packet_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(fake_transport_packet_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(fake_transport_packet_runtime_role) +
           ",\"source_packet_profile_version\":" + field_string(fake_transport_packet_profile_version) +
           ",\"source_packet_state\":" + field_string(fake_transport_packet_state) +
           ",\"source_archive_state\":" + field_string(fake_transport_packet_archive_state) +
           ",\"source_archive_reruns_fake_transport_behavior\":false"
           ",\"source_packet_mode\":" + field_string(fake_transport_packet_mode) +
           ",\"source_span\":" + field_string(fake_transport_packet_source_span) +
           ",\"source_ready_for_fake_transport_packet\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_fake_transport_only\":true"
           ",\"source_dry_run_only\":true"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_request_shape_field_count\":" +
           std::to_string(fake_transport_packet_request_shape_field_count) +
           ",\"source_response_shape_field_count\":" +
           std::to_string(fake_transport_packet_response_shape_field_count) +
           ",\"source_failure_mapping_count\":" +
           std::to_string(fake_transport_packet_failure_mapping_count) +
           ",\"source_timeout_budget_ms\":" + std::to_string(fake_transport_packet_timeout_budget_ms) +
           ",\"source_cleanup_artifact_count\":" +
           std::to_string(fake_transport_packet_cleanup_artifact_count) +
           ",\"source_budget_spent\":false"
           ",\"source_timer_started\":false"
           ",\"source_cleanup_in_memory_only\":true"
           ",\"source_temporary_directory_created\":false"
           ",\"source_temporary_file_created\":false"
           ",\"source_cleanup_required\":false"
           ",\"source_cleanup_verified\":true"
           ",\"source_node_service_started_by_packet\":false"
           ",\"source_java_started\":false"
           ",\"source_mini_kv_started\":false"
           ",\"source_external_audit_service_started\":false"
           ",\"request\":{\"request_id\":" + field_string(fake_transport_packet_request_id) +
           ",\"operation\":" + field_string(fake_transport_packet_operation) +
           ",\"transport_kind\":" + field_string(fake_transport_packet_transport_kind) +
           ",\"credential_handle\":" + field_string(fake_transport_packet_credential_handle) +
           ",\"endpoint_handle\":" + field_string(fake_transport_packet_endpoint_handle) +
           ",\"owner_approval_artifact_id\":" + field_string(fake_transport_packet_owner_approval) +
           ",\"timeout_budget_ms\":" + std::to_string(fake_transport_packet_timeout_budget_ms) +
           ",\"dry_run\":true,\"fake_transport_only\":true,"
           "\"credential_value_included\":false,\"raw_endpoint_url_included\":false,"
           "\"payload_may_contain_secrets\":false,\"request_shape_field_count\":" +
           std::to_string(fake_transport_packet_request_shape_field_count) + "}"
           ",\"response\":{\"request_id\":" + field_string(fake_transport_packet_request_id) +
           ",\"status\":" + field_string(fake_transport_packet_response_status) +
           ",\"code\":" + field_string(fake_transport_packet_response_code) +
           ",\"fake_transport_only\":true,\"timeout_budget_ms\":" +
           std::to_string(fake_transport_packet_timeout_budget_ms) +
           ",\"connection_attempted\":false,\"external_request_sent\":false,"
           "\"credential_value_read\":false,\"schema_migration_executed\":false,"
           "\"production_record_written\":false,\"response_shape_field_count\":" +
           std::to_string(fake_transport_packet_response_shape_field_count) + "}"
           ",\"timeout_budget\":{\"timeout_budget_ms\":" +
           std::to_string(fake_transport_packet_timeout_budget_ms) +
           ",\"finite_budget\":true,\"budget_source\":" +
           field_string(fake_transport_packet_budget_source) +
           ",\"budget_spent\":false,\"timer_started\":false,\"timeout_classifiable\":true}"
           ",\"failure_mapping_verification\":{\"source_failure_mapping_count\":" +
           std::to_string(fake_transport_packet_failure_mapping_count) +
           ",\"mapped_failure_count\":" + std::to_string(fake_transport_packet_failure_mapping_count) +
           ",\"all_failures_non_retryable\":true,\"credential_value_request_still_blocked\":true,"
           "\"manual_window_closed_still_blocked\":true,\"failure_mapping_covered\":true}"
           ",\"cleanup\":{\"in_memory_only\":true,\"temporary_directory_created\":false,"
           "\"temporary_file_created\":false,\"cleanup_required\":false,\"cleanup_artifact_count\":" +
           std::to_string(fake_transport_packet_cleanup_artifact_count) +
           ",\"cleanup_verified\":true,\"node_service_started_by_packet\":false}"
           ",\"boundaries\":{\"connection_attempted\":false,\"external_request_sent\":false,"
           "\"credential_value_read\":false,\"schema_migration_executed\":false,"
           "\"production_record_written\":false,\"java_started\":false,\"mini_kv_started\":false,"
           "\"external_audit_service_started\":false}"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(fake_transport_dry_run_packet_non_participation_receipt_digest(read_commands)) +
           ",\"read_only\":true,\"execution_allowed\":false,\"dry_run_only\":true,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"external_audit_service_auto_start_allowed\":false,"
           "\"connection_execution_allowed\":false,\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,\"runtime_write_observed\":false,"
           "\"managed_audit_store\":false,\"managed_audit_storage_backend\":false,"
           "\"sandbox_audit_storage_backend\":false,\"storage_write_allowed\":false,"
           "\"managed_audit_write_executed\":false,\"production_record_written\":false,"
           "\"sandbox_managed_audit_state_write_allowed\":false,"
           "\"credential_value_required\":false,\"credential_value_read_allowed\":false,"
           "\"credential_value_loaded\":false,\"credential_value_included\":false,"
           "\"raw_endpoint_url_included\":false,\"payload_may_contain_secrets\":false,"
           "\"external_request_sent\":false,\"temporary_directory_created\":false,"
           "\"temporary_file_created\":false,\"cleanup_artifact_count\":0,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"order_authoritative\":false,"
           "\"boundary\":" + field_string(fake_transport_packet_boundary) +
           ",\"node_action\":" + field_string(fake_transport_packet_node_action) + "}";
}

std::string sandbox_endpoint_handle_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
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
    return receipt_digest("mini-kv-sandbox-endpoint-handle-non-participation", parts);
}

std::string format_sandbox_endpoint_handle_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-sandbox-endpoint-handle-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(sandbox_endpoint_handle_receipt_fixture_path) +
           ",\"source_preflight\":" + field_string(sandbox_endpoint_handle_source_preflight) +
           ",\"consumer_hint\":" + field_string(sandbox_endpoint_handle_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" +
           field_string(sandbox_endpoint_handle_receipt_release_version) +
           ",\"current_artifact_path_hint\":" +
           field_string(sandbox_endpoint_handle_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(sandbox_endpoint_handle_runtime_role) +
           ",\"source_preflight_profile_version\":" +
           field_string(sandbox_endpoint_handle_profile_version) +
           ",\"source_review_state\":" + field_string(sandbox_endpoint_handle_review_state) +
           ",\"source_review_mode\":" + field_string(sandbox_endpoint_handle_review_mode) +
           ",\"source_span\":" + field_string(sandbox_endpoint_handle_source_span) +
           ",\"source_ready_for_sandbox_endpoint_handle_preflight_review\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_read_only_preflight_review\":true"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_raw_endpoint_url_included\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_required_review_item_count\":" +
           std::to_string(sandbox_endpoint_handle_required_review_item_count) +
           ",\"source_completed_review_item_count\":" +
           std::to_string(sandbox_endpoint_handle_completed_review_item_count) +
           ",\"source_forbidden_operation_count\":" +
           std::to_string(sandbox_endpoint_handle_forbidden_operation_count) +
           ",\"source_check_count\":" + std::to_string(sandbox_endpoint_handle_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(sandbox_endpoint_handle_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"source_warning_count\":" + std::to_string(sandbox_endpoint_handle_warning_count) +
           ",\"source_recommendation_count\":" +
           std::to_string(sandbox_endpoint_handle_recommendation_count) +
           ",\"source_node_v257_ready\":true"
           ",\"source_node_v257_boundaries_aligned\":true"
           ",\"source_node_v257_evidence_file_count\":" +
           std::to_string(sandbox_endpoint_handle_source_evidence_file_count) +
           ",\"source_node_v257_matched_snippet_count\":" +
           std::to_string(sandbox_endpoint_handle_source_matched_snippet_count) +
           ",\"source_node_v257_ready_for_node_v258_preflight_review\":true"
           ",\"source_upstream_actions_still_disabled\":true"
           ",\"preflight_review\":{\"endpoint_handle\":" +
           field_string(sandbox_endpoint_handle_endpoint_handle) +
           ",\"credential_handle\":" + field_string(sandbox_endpoint_handle_credential_handle) +
           ",\"owner_approval_artifact_id\":" + field_string(sandbox_endpoint_handle_owner_approval) +
           ",\"schema_rehearsal_id\":" + field_string(sandbox_endpoint_handle_schema_rehearsal) +
           ",\"operator_window_marker\":" +
           field_string(sandbox_endpoint_handle_operator_window_marker) +
           ",\"endpoint_handle_reviewed\":true,\"credential_handle_reviewed\":true,"
           "\"owner_approval_artifact_reviewed\":true,\"review_value_only\":false}"
           ",\"network_allowlist_review\":{\"review_required\":true,\"allowlist_handle\":" +
           field_string(sandbox_endpoint_handle_network_allowlist) +
           ",\"raw_host_included\":false,\"cidr_included\":false,\"reviewed\":true}"
           ",\"tls_policy_review\":{\"review_required\":true,\"policy_handle\":" +
           field_string(sandbox_endpoint_handle_tls_policy) +
           ",\"certificate_material_included\":false,\"private_key_included\":false,\"reviewed\":true}"
           ",\"redaction_policy\":{\"review_required\":true,\"policy_handle\":" +
           field_string(sandbox_endpoint_handle_redaction_policy) +
           ",\"credential_value_redacted\":true,\"raw_endpoint_url_redacted\":true,"
           "\"payload_secret_redacted\":true,\"reviewed\":true}"
           ",\"operator_window\":{\"manual_window_required\":true,\"window_open\":false,"
           "\"execution_blocked_until_window_open\":true,\"operator_identity_required\":true,"
           "\"approval_correlation_required\":true,\"reviewed\":true}"
           ",\"required_review_items\":[\"endpoint handle review\",\"credential handle review\","
           "\"owner approval artifact review\",\"network allowlist review\",\"TLS policy review\","
           "\"redaction policy review\",\"operator window review\"]"
           ",\"forbidden_operations\":[\"read credential value\",\"parse raw endpoint URL\","
           "\"send real managed audit request\",\"execute schema migration\",\"write approval ledger\","
           "\"start Java or mini-kv\",\"promote mini-kv to managed audit storage backend\"]"
           ",\"next_required_echo_versions\":[\"Java v104\",\"mini-kv v113\"]"
           ",\"checks\":{\"endpoint_handle_only\":true,\"credential_handle_only\":true,"
           "\"owner_approval_artifact_present\":true,\"network_allowlist_review_ready\":true,"
           "\"tls_policy_review_ready\":true,\"redaction_policy_ready\":true,"
           "\"operator_window_review_ready\":true,\"no_raw_endpoint_url_parsed\":true,"
           "\"no_raw_endpoint_url_included\":true,\"no_credential_value_read\":true,"
           "\"no_external_request_sent\":true,\"no_schema_migration_executed\":true,"
           "\"no_upstream_auto_start\":true,\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,\"production_window_still_blocked\":true}"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(sandbox_endpoint_handle_non_participation_receipt_digest(read_commands)) +
           ",\"read_only\":true,\"execution_allowed\":false,\"dry_run_only\":true,"
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
           "\"credential_value_loaded\":false,\"credential_value_included\":false,"
           "\"raw_endpoint_url_parsed\":false,\"raw_endpoint_url_included\":false,"
           "\"external_request_sent\":false,\"schema_rehearsal_execution_allowed\":false,"
           "\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"order_authoritative\":false,"
           "\"boundary\":" + field_string(sandbox_endpoint_handle_boundary) +
           ",\"node_action\":" + field_string(sandbox_endpoint_handle_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
