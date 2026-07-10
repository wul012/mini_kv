#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::append_common_credential_boundary_fields;
using credential_resolver_detail::DigestPart;
using credential_resolver_detail::format_no_go_conditions_json;
using credential_resolver_detail::receipt_digest;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

constexpr std::string_view credential_resolver_receipt_consumer =
    "Node v261 credential resolver upstream echo verification";
constexpr std::string_view credential_resolver_receipt_fixture_path =
    "fixtures/release/credential-resolver-non-participation-receipt.json";
constexpr std::string_view credential_resolver_receipt_release_version = "v114";
constexpr std::string_view credential_resolver_receipt_artifact_path_hint = "c/114/";
constexpr std::string_view credential_resolver_source_decision =
    "Node v260 sandbox endpoint credential resolver decision record";
constexpr std::string_view credential_resolver_profile_version =
    "managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-decision-record.v1";
constexpr std::string_view credential_resolver_decision_state =
    "sandbox-endpoint-credential-resolver-decision-record-ready";
constexpr std::string_view credential_resolver_record_mode =
    "sandbox-endpoint-credential-resolver-decision-record-only";
constexpr std::string_view credential_resolver_decision_scope = "managed-audit-sandbox-endpoint-credential-resolver";
constexpr std::string_view credential_resolver_decision_status = "human-review-required-before-credential-resolution";
constexpr std::string_view credential_resolver_source_span =
    "Node v259 sandbox endpoint handle upstream echo verification";
constexpr std::string_view credential_resolver_endpoint_handle = "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view credential_resolver_credential_handle = "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view credential_resolver_policy_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_RESOLVER_POLICY_HANDLE";
constexpr std::string_view credential_resolver_approval_marker =
    "ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_APPROVAL_MARKER";
constexpr std::string_view credential_resolver_operator_identity_evidence = "operator-header";
constexpr std::string_view credential_resolver_approval_correlation_evidence = "approval-correlation-header";
constexpr std::string_view credential_resolver_policy_evidence = "policy-handle-only";
constexpr std::string_view credential_resolver_redaction_policy_evidence = "redaction-reviewed";
constexpr std::string_view credential_resolver_fallback_rotation_plan_evidence = "plan-handle-only";
constexpr std::string_view credential_resolver_runtime_role = "runtime evidence provider only";
constexpr std::string_view credential_resolver_boundary =
    "credential resolver non-participation receipt only; mini-kv echoes Node v260 credential resolver decision "
    "boundaries, does not auto-start, does not write storage, does not read, load, store, or include credential "
    "values, does not parse raw endpoint URLs, does not instantiate a secret provider, does not invoke a credential "
    "resolver, does not send external requests, does not execute schema migration or restore/load/compact/SETNXEX, and "
    "does not become a managed audit storage backend";
constexpr std::string_view credential_resolver_node_action =
    "verify mini-kv non-participation with Node v260 credential resolver decision record before Node v261 upstream "
    "echo verification";
constexpr int credential_resolver_required_decision_field_count = 8;
constexpr int credential_resolver_explicit_no_go_condition_count = 9;
constexpr int credential_resolver_check_count = 20;
constexpr int credential_resolver_warning_count = 2;
constexpr int credential_resolver_recommendation_count = 2;
constexpr int credential_resolver_source_evidence_file_count = 6;
constexpr int credential_resolver_source_matched_snippet_count = 39;

constexpr std::string_view disabled_credential_resolver_receipt_consumer =
    "Node v263 disabled credential resolver upstream echo verification";
constexpr std::string_view disabled_credential_resolver_receipt_fixture_path =
    "fixtures/release/disabled-credential-resolver-precheck-non-participation-receipt.json";
constexpr std::string_view disabled_credential_resolver_receipt_release_version = "v115";
constexpr std::string_view disabled_credential_resolver_receipt_artifact_path_hint = "c/115/";
constexpr std::string_view disabled_credential_resolver_source_precheck =
    "Node v262 disabled credential resolver precheck";
constexpr std::string_view disabled_credential_resolver_source_verification =
    "Node v261 credential resolver upstream echo verification";
constexpr std::string_view disabled_credential_resolver_profile_version =
    "managed-audit-manual-sandbox-connection-sandbox-endpoint-credential-resolver-disabled-precheck.v1";
constexpr std::string_view disabled_credential_resolver_precheck_state =
    "sandbox-endpoint-credential-resolver-disabled-precheck-ready";
constexpr std::string_view disabled_credential_resolver_precheck_mode =
    "sandbox-endpoint-credential-resolver-disabled-precheck-only";
constexpr std::string_view disabled_credential_resolver_source_span = "Node v260 + Java v105 + mini-kv v114";
constexpr std::string_view disabled_credential_resolver_endpoint_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_HANDLE";
constexpr std::string_view disabled_credential_resolver_credential_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE";
constexpr std::string_view disabled_credential_resolver_policy_handle =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_RESOLVER_POLICY_HANDLE";
constexpr std::string_view disabled_credential_resolver_approval_marker =
    "ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_APPROVAL_MARKER";
constexpr std::string_view disabled_credential_resolver_gate = "ORDEROPS_MANAGED_AUDIT_CREDENTIAL_RESOLVER_ENABLED";
constexpr std::string_view disabled_credential_resolver_endpoint_gate =
    "ORDEROPS_MANAGED_AUDIT_SANDBOX_ENDPOINT_RESOLUTION_ENABLED";
constexpr std::string_view disabled_credential_resolver_ready_state =
    "sandbox-endpoint-credential-resolver-disabled-precheck-ready";
constexpr std::string_view disabled_credential_resolver_runtime_role = "runtime evidence provider only";
constexpr std::string_view disabled_credential_resolver_boundary =
    "disabled credential resolver precheck non-participation receipt only; mini-kv echoes Node v262 disabled resolver "
    "precheck boundaries, does not auto-start, does not write storage, does not read, load, store, or include "
    "credential values, does not parse raw endpoint URLs, does not instantiate a secret provider or resolver client, "
    "does not send external requests, does not execute schema migration or restore/load/compact/SETNXEX, and does not "
    "become a managed audit storage backend";
constexpr std::string_view disabled_credential_resolver_node_action =
    "verify mini-kv non-participation with Node v262 disabled credential resolver precheck before Node v263 upstream "
    "echo verification";
constexpr int disabled_credential_resolver_required_env_handle_count = 6;
constexpr int disabled_credential_resolver_opt_in_gate_count = 2;
constexpr int disabled_credential_resolver_failure_class_count = 7;
constexpr int disabled_credential_resolver_dry_run_response_field_count = 12;
constexpr int disabled_credential_resolver_inherited_no_go_condition_count = 9;
constexpr int disabled_credential_resolver_check_count = 24;
constexpr int disabled_credential_resolver_warning_count = 2;
constexpr int disabled_credential_resolver_recommendation_count = 2;
constexpr int disabled_credential_resolver_source_v261_check_count = 31;
constexpr int disabled_credential_resolver_source_v261_warning_count = 2;
constexpr int disabled_credential_resolver_source_v261_recommendation_count = 2;

} // namespace

std::string credential_resolver_non_participation_receipt_digest(const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{credential_resolver_receipt_release_version}},
        {std::string{credential_resolver_receipt_artifact_path_hint}},
        {std::string{credential_resolver_source_decision}},
        {std::string{credential_resolver_profile_version}},
        {std::string{credential_resolver_decision_state}},
        {std::string{credential_resolver_record_mode}},
        {std::string{credential_resolver_decision_scope}},
        {std::string{credential_resolver_decision_status}},
        {std::string{credential_resolver_source_span}},
        {std::string{credential_resolver_endpoint_handle}},
        {std::string{credential_resolver_credential_handle}},
        {std::string{credential_resolver_policy_handle}},
        {std::string{credential_resolver_approval_marker}},
        {std::string{credential_resolver_operator_identity_evidence}},
        {std::string{credential_resolver_approval_correlation_evidence}},
        {std::string{credential_resolver_policy_evidence}},
        {std::string{credential_resolver_redaction_policy_evidence}},
        {std::string{credential_resolver_fallback_rotation_plan_evidence}},
        {std::to_string(credential_resolver_required_decision_field_count)},
        {std::to_string(credential_resolver_explicit_no_go_condition_count)},
        {std::to_string(credential_resolver_check_count)},
        {std::to_string(credential_resolver_warning_count)},
        {std::to_string(credential_resolver_recommendation_count)},
        {std::to_string(credential_resolver_source_evidence_file_count)},
        {std::to_string(credential_resolver_source_matched_snippet_count)},
        {"true"},
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
    return receipt_digest("mini-kv-credential-resolver-non-participation", parts);
}

std::string format_credential_resolver_non_participation_receipt_json(const std::vector<std::string>& read_commands) {
    const auto decision_record = json_object({
        {"record_mode", json_string(credential_resolver_record_mode)},
        {"decision_scope", json_string(credential_resolver_decision_scope)},
        {"decision_status", json_string(credential_resolver_decision_status)},
        {"source_span", json_string(credential_resolver_source_span)},
        {"endpoint_handle", json_string(credential_resolver_endpoint_handle)},
        {"credential_handle", json_string(credential_resolver_credential_handle)},
        {"resolver_policy_handle", json_string(credential_resolver_policy_handle)},
        {"approval_marker", json_string(credential_resolver_approval_marker)},
        {"operator_identity_required", json_bool(true)},
        {"approval_correlation_required", json_bool(true)},
        {"resolver_mode", json_string("policy-record-only-no-value-read")},
        {"resolver_candidate_implementation", json_string("not-implemented")},
        {"required_decision_field_count", json_integer(credential_resolver_required_decision_field_count)},
        {"explicit_no_go_condition_count", json_integer(credential_resolver_explicit_no_go_condition_count)},
        {"credential_value_may_be_read", json_bool(false)},
        {"credential_value_may_be_loaded", json_bool(false)},
        {"credential_value_may_be_stored", json_bool(false)},
        {"raw_endpoint_url_may_be_parsed", json_bool(false)},
        {"managed_audit_connection_may_open", json_bool(false)},
        {"schema_migration_may_execute", json_bool(false)},
        {"external_request_may_be_sent", json_bool(false)},
        {"node_may_start_java_or_mini_kv", json_bool(false)},
        {"mini_kv_may_act_as_managed_audit_storage", json_bool(false)},
        {"approval_ledger_may_be_written", json_bool(false)},
    });
    const auto required_decision_fields = json_array({
        json_object({{"id", json_string("endpoint-handle")}, {"accepted_evidence", json_string("handle-aligned")}}),
        json_object({{"id", json_string("credential-handle")}, {"accepted_evidence", json_string("handle-aligned")}}),
        json_object({{"id", json_string("resolver-policy-handle")},
                     {"accepted_evidence", json_string(credential_resolver_policy_evidence)}}),
        json_object(
            {{"id", json_string("approval-marker")}, {"accepted_evidence", json_string("approval-marker-only")}}),
        json_object({{"id", json_string("operator-identity")},
                     {"accepted_evidence", json_string(credential_resolver_operator_identity_evidence)}}),
        json_object({{"id", json_string("approval-correlation")},
                     {"accepted_evidence", json_string(credential_resolver_approval_correlation_evidence)}}),
        json_object({{"id", json_string("redaction-policy")},
                     {"accepted_evidence", json_string(credential_resolver_redaction_policy_evidence)}}),
        json_object({{"id", json_string("fallback-rotation-plan")},
                     {"accepted_evidence", json_string(credential_resolver_fallback_rotation_plan_evidence)}}),
    });
    const auto checks = json_object({
        {"source_node_v259_ready", json_bool(true)},
        {"source_node_v259_still_blocks_connection", json_bool(true)},
        {"source_node_v259_still_blocks_credential_value", json_bool(true)},
        {"source_node_v259_still_blocks_raw_endpoint", json_bool(true)},
        {"source_node_v259_still_blocks_writes", json_bool(true)},
        {"source_node_v259_still_blocks_auto_start", json_bool(true)},
        {"source_node_v259_keeps_mini_kv_non_participant", json_bool(true)},
        {"endpoint_handle_recorded", json_bool(true)},
        {"credential_handle_recorded", json_bool(true)},
        {"resolver_policy_recorded", json_bool(true)},
        {"approval_marker_recorded", json_bool(true)},
        {"operator_identity_requirement_recorded", json_bool(true)},
        {"approval_correlation_requirement_recorded", json_bool(true)},
        {"resolver_mode_still_policy_only", json_bool(true)},
        {"explicit_no_go_conditions_recorded", json_bool(true)},
        {"decision_record_still_read_only", json_bool(true)},
        {"upstream_actions_still_disabled", json_bool(true)},
        {"production_audit_still_blocked", json_bool(true)},
        {"production_window_still_blocked", json_bool(true)},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string("mini-kv-credential-resolver-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(credential_resolver_receipt_fixture_path)},
        {"source_decision", json_string(credential_resolver_source_decision)},
        {"consumer_hint", json_string(credential_resolver_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(credential_resolver_receipt_release_version)},
        {"current_artifact_path_hint", json_string(credential_resolver_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(credential_resolver_runtime_role)},
        {"source_decision_profile_version", json_string(credential_resolver_profile_version)},
        {"source_decision_state", json_string(credential_resolver_decision_state)},
        {"source_record_mode", json_string(credential_resolver_record_mode)},
        {"source_decision_scope", json_string(credential_resolver_decision_scope)},
        {"source_decision_status", json_string(credential_resolver_decision_status)},
        {"source_span", json_string(credential_resolver_source_span)},
        {"source_ready_for_credential_resolver_decision_record", json_bool(true)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_ready_for_production_audit", json_bool(false)},
        {"source_ready_for_production_window", json_bool(false)},
        {"source_read_only_decision_record", json_bool(true)},
        {"source_credential_resolver_decision_only", json_bool(true)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_credential_value_loaded", json_bool(false)},
        {"source_credential_value_included", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_raw_endpoint_url_included", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_required_decision_field_count", json_integer(credential_resolver_required_decision_field_count)},
        {"source_explicit_no_go_condition_count", json_integer(credential_resolver_explicit_no_go_condition_count)},
        {"source_check_count", json_integer(credential_resolver_check_count)},
        {"source_passed_check_count", json_integer(credential_resolver_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"source_warning_count", json_integer(credential_resolver_warning_count)},
        {"source_recommendation_count", json_integer(credential_resolver_recommendation_count)},
        {"source_node_v259_ready", json_bool(true)},
        {"source_node_v259_blocks_real_connection", json_bool(true)},
        {"source_node_v259_credential_boundary_aligned", json_bool(true)},
        {"source_node_v259_raw_endpoint_boundary_aligned", json_bool(true)},
        {"source_node_v259_write_boundary_aligned", json_bool(true)},
        {"source_node_v259_auto_start_boundary_aligned", json_bool(true)},
        {"source_node_v259_keeps_mini_kv_non_participant", json_bool(true)},
        {"source_node_v259_evidence_file_count", json_integer(credential_resolver_source_evidence_file_count)},
        {"source_node_v259_matched_snippet_count", json_integer(credential_resolver_source_matched_snippet_count)},
        {"source_node_v259_ready_for_node_v260_decision_record", json_bool(true)},
        {"source_upstream_actions_still_disabled", json_bool(true)},
        {"decision_record", decision_record},
        {"required_decision_fields", required_decision_fields},
        {"explicit_no_go_conditions", format_no_go_conditions_json()},
        {"checks", checks},
        {"next_required_echo_versions", json_array({json_string("Java v105"), json_string("mini-kv v114")})},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest", json_string(credential_resolver_non_participation_receipt_digest(read_commands))},
        {"credential_resolver_decision_only", json_bool(true)},
    };
    append_common_credential_boundary_fields(fields);
    fields.push_back({"boundary", json_string(credential_resolver_boundary)});
    fields.push_back({"node_action", json_string(credential_resolver_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

std::string
disabled_credential_resolver_precheck_non_participation_receipt_digest(const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{disabled_credential_resolver_receipt_release_version}},
        {std::string{disabled_credential_resolver_receipt_artifact_path_hint}},
        {std::string{disabled_credential_resolver_source_precheck}},
        {std::string{disabled_credential_resolver_source_verification}},
        {std::string{disabled_credential_resolver_profile_version}},
        {std::string{disabled_credential_resolver_precheck_state}},
        {std::string{disabled_credential_resolver_precheck_mode}},
        {std::string{disabled_credential_resolver_source_span}},
        {std::string{disabled_credential_resolver_gate}},
        {std::string{disabled_credential_resolver_endpoint_gate}},
        {std::string{disabled_credential_resolver_endpoint_handle}},
        {std::string{disabled_credential_resolver_credential_handle}},
        {std::string{disabled_credential_resolver_policy_handle}},
        {std::string{disabled_credential_resolver_approval_marker}},
        {std::to_string(disabled_credential_resolver_required_env_handle_count)},
        {std::to_string(disabled_credential_resolver_opt_in_gate_count)},
        {std::to_string(disabled_credential_resolver_failure_class_count)},
        {std::to_string(disabled_credential_resolver_dry_run_response_field_count)},
        {std::to_string(disabled_credential_resolver_inherited_no_go_condition_count)},
        {std::to_string(disabled_credential_resolver_check_count)},
        {std::to_string(disabled_credential_resolver_warning_count)},
        {std::to_string(disabled_credential_resolver_recommendation_count)},
        {"not-implemented"},
        {"not-implemented"},
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
    return receipt_digest("mini-kv-disabled-credential-resolver-precheck-non-participation", parts);
}

std::string format_disabled_credential_resolver_precheck_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto disabled_precheck = json_object({
        {"precheck_mode", json_string(disabled_credential_resolver_precheck_mode)},
        {"ready_state", json_string(disabled_credential_resolver_ready_state)},
        {"resolver_implementation_status", json_string("not-implemented")},
        {"secret_provider_implementation_status", json_string("not-implemented")},
        {"resolver_client_may_be_instantiated", json_bool(false)},
        {"secret_provider_may_be_instantiated", json_bool(false)},
        {"credential_value_may_be_loaded", json_bool(false)},
        {"raw_endpoint_url_may_be_parsed", json_bool(false)},
        {"external_request_may_be_sent", json_bool(false)},
        {"opt_in_gate_required", json_bool(true)},
        {"required_env_handle_count", json_integer(disabled_credential_resolver_required_env_handle_count)},
        {"opt_in_gate_count", json_integer(disabled_credential_resolver_opt_in_gate_count)},
        {"failure_class_count", json_integer(disabled_credential_resolver_failure_class_count)},
        {"dry_run_response_field_count", json_integer(disabled_credential_resolver_dry_run_response_field_count)},
        {"inherited_no_go_condition_count", json_integer(disabled_credential_resolver_inherited_no_go_condition_count)},
    });
    const auto required_env_handles = json_array({
        json_object({{"name", json_string(disabled_credential_resolver_gate)},
                     {"purpose", json_string("future opt-in gate for the credential resolver")},
                     {"value_required_for_precheck", json_bool(false)},
                     {"credential_value", json_bool(false)},
                     {"raw_endpoint_value", json_bool(false)},
                     {"required_before_real_resolver", json_bool(false)}}),
        json_object({{"name", json_string(disabled_credential_resolver_endpoint_gate)},
                     {"purpose", json_string("future opt-in gate for sandbox endpoint resolution")},
                     {"value_required_for_precheck", json_bool(false)},
                     {"credential_value", json_bool(false)},
                     {"raw_endpoint_value", json_bool(false)},
                     {"required_before_real_resolver", json_bool(false)}}),
        json_object({{"name", json_string(disabled_credential_resolver_endpoint_handle)},
                     {"purpose", json_string("handle for the approved sandbox endpoint")},
                     {"value_required_for_precheck", json_bool(false)},
                     {"credential_value", json_bool(false)},
                     {"raw_endpoint_value", json_bool(false)},
                     {"required_before_real_resolver", json_bool(true)}}),
        json_object({{"name", json_string(disabled_credential_resolver_credential_handle)},
                     {"purpose", json_string("handle for the approved sandbox credential")},
                     {"value_required_for_precheck", json_bool(false)},
                     {"credential_value", json_bool(false)},
                     {"raw_endpoint_value", json_bool(false)},
                     {"required_before_real_resolver", json_bool(true)}}),
        json_object({{"name", json_string(disabled_credential_resolver_policy_handle)},
                     {"purpose", json_string("handle for the resolver policy review")},
                     {"value_required_for_precheck", json_bool(false)},
                     {"credential_value", json_bool(false)},
                     {"raw_endpoint_value", json_bool(false)},
                     {"required_before_real_resolver", json_bool(true)}}),
        json_object({{"name", json_string(disabled_credential_resolver_approval_marker)},
                     {"purpose", json_string("operator approval marker for future resolver design")},
                     {"value_required_for_precheck", json_bool(false)},
                     {"credential_value", json_bool(false)},
                     {"raw_endpoint_value", json_bool(false)},
                     {"required_before_real_resolver", json_bool(true)}}),
    });
    const auto opt_in_gates = json_array({
        json_object({{"gate_name", json_string(disabled_credential_resolver_gate)},
                     {"required_value_for_future_resolver", json_string("true")},
                     {"current_default", json_string("false")},
                     {"precheck_treats_enabled_as_blocked", json_bool(true)},
                     {"operator_approval_required", json_bool(true)}}),
        json_object({{"gate_name", json_string(disabled_credential_resolver_endpoint_gate)},
                     {"required_value_for_future_resolver", json_string("true")},
                     {"current_default", json_string("false")},
                     {"precheck_treats_enabled_as_blocked", json_bool(true)},
                     {"operator_approval_required", json_bool(true)}}),
    });
    const auto failure_taxonomy_codes = json_array({
        json_string("RESOLVER_DISABLED"),
        json_string("APPROVAL_MARKER_MISSING"),
        json_string("CREDENTIAL_HANDLE_MISSING"),
        json_string("CREDENTIAL_VALUE_REQUESTED"),
        json_string("RAW_ENDPOINT_URL_REQUESTED"),
        json_string("EXTERNAL_REQUEST_REQUESTED"),
        json_string("SCHEMA_MIGRATION_REQUESTED"),
    });
    const auto dry_run_response_shape = json_object({
        {"fields", json_array({
                       json_string("readyState"),
                       json_string("resolverMode"),
                       json_string("resolverClientInstantiated"),
                       json_string("secretProviderInstantiated"),
                       json_string("credentialValueRead"),
                       json_string("credentialValueLoaded"),
                       json_string("rawEndpointUrlParsed"),
                       json_string("externalRequestSent"),
                       json_string("connectsManagedAudit"),
                       json_string("schemaMigrationExecuted"),
                       json_string("failureClassCount"),
                       json_string("nextAction"),
                   })},
        {"ready_state", json_string(disabled_credential_resolver_ready_state)},
        {"resolver_client_instantiated", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"credential_value_loaded", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
    });
    const auto checks = json_object({
        {"source_node_v261_ready", json_bool(true)},
        {"source_verification_still_blocks_credential_resolution", json_bool(true)},
        {"source_verification_still_blocks_credential_value", json_bool(true)},
        {"source_verification_still_blocks_raw_endpoint", json_bool(true)},
        {"source_verification_still_blocks_connection", json_bool(true)},
        {"source_verification_still_blocks_writes", json_bool(true)},
        {"source_verification_still_blocks_auto_start", json_bool(true)},
        {"required_env_handles_declared", json_bool(true)},
        {"env_handles_remain_handle_only", json_bool(true)},
        {"opt_in_gates_declared", json_bool(true)},
        {"opt_in_gates_default_disabled", json_bool(true)},
        {"failure_taxonomy_declared", json_bool(true)},
        {"dry_run_response_shape_declared", json_bool(true)},
        {"resolver_implementation_still_absent", json_bool(true)},
        {"secret_provider_implementation_still_absent", json_bool(true)},
        {"resolver_client_instantiation_blocked", json_bool(true)},
        {"secret_provider_instantiation_blocked", json_bool(true)},
        {"credential_value_load_blocked", json_bool(true)},
        {"raw_endpoint_parse_blocked", json_bool(true)},
        {"external_request_blocked", json_bool(true)},
        {"upstream_actions_still_disabled", json_bool(true)},
        {"production_audit_still_blocked", json_bool(true)},
        {"production_window_still_blocked", json_bool(true)},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string("mini-kv-disabled-credential-resolver-precheck-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(disabled_credential_resolver_receipt_fixture_path)},
        {"source_precheck", json_string(disabled_credential_resolver_source_precheck)},
        {"source_verification", json_string(disabled_credential_resolver_source_verification)},
        {"consumer_hint", json_string(disabled_credential_resolver_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(disabled_credential_resolver_receipt_release_version)},
        {"current_artifact_path_hint", json_string(disabled_credential_resolver_receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(disabled_credential_resolver_runtime_role)},
        {"source_precheck_profile_version", json_string(disabled_credential_resolver_profile_version)},
        {"source_precheck_state", json_string(disabled_credential_resolver_precheck_state)},
        {"source_precheck_mode", json_string(disabled_credential_resolver_precheck_mode)},
        {"source_span", json_string(disabled_credential_resolver_source_span)},
        {"source_ready_for_disabled_credential_resolver_precheck", json_bool(true)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_ready_for_production_audit", json_bool(false)},
        {"source_ready_for_production_window", json_bool(false)},
        {"source_read_only_disabled_precheck", json_bool(true)},
        {"source_disabled_credential_resolver_precheck_only", json_bool(true)},
        {"source_credential_resolver_execution_allowed", json_bool(false)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_credential_value_loaded", json_bool(false)},
        {"source_credential_value_stored", json_bool(false)},
        {"source_credential_value_included", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_raw_endpoint_url_included", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_secret_provider_instantiated", json_bool(false)},
        {"source_resolver_client_instantiated", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_required_env_handle_count", json_integer(disabled_credential_resolver_required_env_handle_count)},
        {"source_opt_in_gate_count", json_integer(disabled_credential_resolver_opt_in_gate_count)},
        {"source_failure_class_count", json_integer(disabled_credential_resolver_failure_class_count)},
        {"source_dry_run_response_field_count",
         json_integer(disabled_credential_resolver_dry_run_response_field_count)},
        {"source_inherited_no_go_condition_count",
         json_integer(disabled_credential_resolver_inherited_no_go_condition_count)},
        {"source_check_count", json_integer(disabled_credential_resolver_check_count)},
        {"source_passed_check_count", json_integer(disabled_credential_resolver_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"source_warning_count", json_integer(disabled_credential_resolver_warning_count)},
        {"source_recommendation_count", json_integer(disabled_credential_resolver_recommendation_count)},
        {"source_node_v261_ready", json_bool(true)},
        {"source_node_v261_verification_mode",
         json_string("java-v105-plus-mini-kv-v114-credential-resolver-upstream-echo-verification-only")},
        {"source_node_v261_span", json_string(disabled_credential_resolver_source_span)},
        {"source_node_v261_blocks_credential_resolution", json_bool(true)},
        {"source_node_v261_credential_boundary_aligned", json_bool(true)},
        {"source_node_v261_raw_endpoint_boundary_aligned", json_bool(true)},
        {"source_node_v261_connection_boundary_aligned", json_bool(true)},
        {"source_node_v261_write_boundary_aligned", json_bool(true)},
        {"source_node_v261_auto_start_boundary_aligned", json_bool(true)},
        {"source_node_v261_upstream_actions_still_disabled", json_bool(true)},
        {"source_node_v261_check_count", json_integer(disabled_credential_resolver_source_v261_check_count)},
        {"source_node_v261_passed_check_count", json_integer(disabled_credential_resolver_source_v261_check_count)},
        {"source_node_v261_production_blocker_count", json_integer(0)},
        {"source_node_v261_warning_count", json_integer(disabled_credential_resolver_source_v261_warning_count)},
        {"source_node_v261_recommendation_count",
         json_integer(disabled_credential_resolver_source_v261_recommendation_count)},
        {"disabled_precheck", disabled_precheck},
        {"required_env_handles", required_env_handles},
        {"opt_in_gates", opt_in_gates},
        {"failure_taxonomy_codes", failure_taxonomy_codes},
        {"dry_run_response_shape", dry_run_response_shape},
        {"inherited_no_go_conditions", format_no_go_conditions_json()},
        {"checks", checks},
        {"next_required_echo_versions", json_array({json_string("Java v106"), json_string("mini-kv v115")})},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(disabled_credential_resolver_precheck_non_participation_receipt_digest(read_commands))},
        {"disabled_credential_resolver_precheck_only", json_bool(true)},
    };
    append_common_credential_boundary_fields(fields);
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"boundary", json_string(disabled_credential_resolver_boundary)});
    fields.push_back({"node_action", json_string(disabled_credential_resolver_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
