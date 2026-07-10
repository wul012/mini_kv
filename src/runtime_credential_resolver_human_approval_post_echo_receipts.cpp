#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/version.hpp"

#include <span>
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

constexpr std::string_view receipt_version =
    "mini-kv-credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer =
    "Node v311 human approval artifact review post-echo decision upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/"
    "credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v137";
constexpr std::string_view receipt_artifact_path_hint = "d/137/";
constexpr std::string_view source_decision_gate = "Node v310 human approval artifact review post-echo decision gate";
constexpr std::string_view source_upstream_echo = "Node v309 human approval artifact review upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v310_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "human-approval-artifact-review-post-echo-decision-gate.v1";
constexpr std::string_view node_v310_decision_gate_state =
    "human-approval-artifact-review-post-echo-decision-gate-ready";
constexpr std::string_view node_v310_decision_digest =
    "91c0927b63fee6dd0fb44e27cd82c7f700ceeeb4cef750606777bd4d3436102c";
constexpr std::string_view node_v310_gate_mode = "human-approval-artifact-review-post-echo-decision-gate-only";
constexpr std::string_view node_v310_decision = "continue-only-as-blocked-post-echo-prerequisite-review";
constexpr std::string_view node_v310_selected_path =
    "request-read-only-upstream-decision-echo-before-any-runtime-shell";
constexpr std::string_view node_v310_source_span = "Node v308 + Java v143 + mini-kv v136 + Node v309";
constexpr std::string_view node_v310_reason =
    "Node v309 aligned Java v143 and mini-kv v136 with the Node v308 review packet, but it did not provide a signed "
    "artifact instance, credential-handle approval, endpoint allowlist approval, no-network safety evidence, abort "
    "semantics, or upstream echo of this decision gate.";

constexpr std::string_view node_v309_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "human-approval-artifact-review-upstream-echo-verification.v1";
constexpr std::string_view node_v309_verification_state =
    "human-approval-artifact-review-upstream-echo-verification-ready";
constexpr std::string_view node_v309_verification_digest =
    "b306972aeab587fff6905759ad0fa2968af235cb13f09063c1c6c3af14e7913d";
constexpr std::string_view node_v309_source_span = "Node v308 + Java v143 + mini-kv v136";
constexpr std::string_view node_v308_packet_digest = "b0dda954c509337c96a645c177be521d0a200d8f8e6d52081ed8c0df9a43ccf3";
constexpr std::string_view runtime_shell_chain_decision =
    "require-explicit-approval-prerequisites-before-runtime-shell";

constexpr std::string_view active_plan =
    "docs/plans2/v309-post-human-approval-artifact-review-upstream-echo-roadmap.md";
constexpr std::string_view node_v310_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-human-"
                                            "approval-artifact-review-post-echo-decision-gate";
constexpr std::string_view node_v310_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-human-"
                                                "approval-artifact-review-post-echo-decision-gate?format=markdown";
constexpr std::string_view node_v309_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-human-"
                                            "approval-artifact-review-upstream-echo-verification";
constexpr std::string_view node_v309_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-human-"
                                                "approval-artifact-review-upstream-echo-verification?format=markdown";

constexpr int check_count = 17;
constexpr int source_check_count = 23;
constexpr int source_node_v308_check_count = 18;
constexpr int required_field_count = 9;
constexpr int prohibited_field_count = 9;
constexpr int rejection_reason_count = 13;
constexpr int missing_field_check_count = 9;
constexpr int no_go_boundary_count = 12;
constexpr int upstream_echo_request_count = 2;
constexpr int prerequisite_count = 6;
constexpr int missing_prerequisite_count = 6;
constexpr int no_go_condition_count = 9;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver human approval artifact review post-echo decision gate non-participation receipt only; "
    "mini-kv echoes Node v310's blocked post-echo decision gate, records six documented-missing prerequisites and nine "
    "no-go conditions, and does not accept credential values, raw endpoint URLs, provider/client configuration, "
    "HTTP/TCP payloads, ledger mutations, schema SQL, mini-kv write commands, runtime shell invocation requests, "
    "authority claims, or LOAD/COMPACT/RESTORE/SETNXEX execution";
constexpr std::string_view node_action =
    "verify mini-kv v137 human approval artifact review post-echo decision gate non-participation receipt with Java "
    "v144 before Node v311 upstream echo verification";

std::string format_required_prerequisites_json() {
    return "[{\"id\":\"signed-human-approval-artifact\",\"status\":\"documented-missing\",\"required_before_runtime_"
           "shell\":true},"
           "{\"id\":\"credential-handle-approval\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":"
           "true},"
           "{\"id\":\"endpoint-handle-allowlist-approval\",\"status\":\"documented-missing\",\"required_before_runtime_"
           "shell\":true},"
           "{\"id\":\"no-network-safety-fixture\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":"
           "true},"
           "{\"id\":\"abort-rollback-semantics\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":"
           "true},"
           "{\"id\":\"java-mini-kv-decision-echo\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":"
           "true}]";
}

std::string format_no_go_conditions_json() {
    return "[\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\","
           "\"RUNTIME_SHELL_INVOCATION_REQUESTED\","
           "\"CREDENTIAL_VALUE_READ_REQUESTED\","
           "\"RAW_ENDPOINT_URL_PARSE_REQUESTED\","
           "\"PROVIDER_CLIENT_INSTANTIATION_REQUESTED\","
           "\"EXTERNAL_REQUEST_REQUESTED\","
           "\"LEDGER_OR_SCHEMA_WRITE_REQUESTED\","
           "\"MINIKV_WRITE_OR_AUTHORITY_REQUESTED\","
           "\"AUTOMATIC_UPSTREAM_START_REQUESTED\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v309_loaded\":true,"
           "\"source_node_v309_ready\":true,"
           "\"source_node_v309_upstream_echo_aligned\":true,"
           "\"source_node_v309_keeps_runtime_blocked\":true,"
           "\"source_node_v309_keeps_side_effects_closed\":true,"
           "\"decision_selects_post_echo_prerequisite_gate\":true,"
           "\"decision_gate_blocks_runtime_shell\":true,"
           "\"decision_gate_still_read_only\":true,"
           "\"post_echo_prerequisites_documented\":true,"
           "\"missing_prerequisites_block_implementation\":true,"
           "\"necessity_proof_complete\":true,"
           "\"parallel_java_v144_mini_kv_v137_echo_recommended\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v311_human_approval_artifact_review_post_echo_decision_upstream_echo_verification\":true}";
}

void append_closed_boundary_flag_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back(
        {"human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_only", json_bool(true)});
    fields.push_back({"read_only_decision_gate", json_bool(true)});
    fields.push_back({"consumes_node_v310_human_approval_artifact_review_post_echo_decision_gate", json_bool(true)});
    fields.push_back({"consumes_node_v309_human_approval_artifact_review_upstream_echo_verification", json_bool(true)});
    fields.push_back(
        {"ready_for_node_v311_human_approval_artifact_review_post_echo_decision_upstream_echo_verification",
         json_bool(true)});
    fields.push_back({"ready_for_node_v311_before_upstream_echo", json_bool(false)});
    fields.push_back({"ready_for_disabled_runtime_shell_implementation", json_bool(false)});
    fields.push_back({"ready_for_disabled_runtime_shell_invocation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_resolver_implementation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)});
    fields.push_back({"ready_for_production_audit", json_bool(false)});
    fields.push_back({"ready_for_production_window", json_bool(false)});
    fields.push_back({"ready_for_production_operations", json_bool(false)});
    fields.push_back({"runtime_shell_implemented", json_bool(false)});
    fields.push_back({"runtime_shell_enabled", json_bool(false)});
    fields.push_back({"runtime_shell_invocation_allowed", json_bool(false)});
    fields.push_back({"runtime_shell_implementation_allowed", json_bool(false)});
    fields.push_back({"real_resolver_implementation_allowed", json_bool(false)});
    fields.push_back({"credential_resolver_implemented", json_bool(false)});
    fields.push_back({"credential_resolver_invoked", json_bool(false)});
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_resolver_client_instantiated", json_bool(false)});
    fields.push_back({"provider_client_instantiation_allowed", json_bool(false)});
    fields.push_back({"credential_value_accepted", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_read", json_bool(false)});
    fields.push_back({"credential_value_provided", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_stored", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"credential_value_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_accepted", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parse_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_render_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_rendered", json_bool(false)});
    fields.push_back({"external_request_payload_accepted", json_bool(false)});
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
    fields.push_back({"approval_ledger_write_allowed", json_bool(false)});
    fields.push_back({"approval_ledger_written", json_bool(false)});
    fields.push_back({"approval_ledger_write_executed", json_bool(false)});
    fields.push_back({"managed_audit_write_executed", json_bool(false)});
    fields.push_back({"production_record_written", json_bool(false)});
    fields.push_back({"schema_migration_allowed", json_bool(false)});
    fields.push_back({"schema_migration_executed", json_bool(false)});
    fields.push_back({"schema_rehearsal_execution_allowed", json_bool(false)});
    fields.push_back({"schema_migration_execution_allowed", json_bool(false)});
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

std::string credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_decision_gate}},
        {std::string{source_upstream_echo}},
        {std::string{node_v310_profile_version}},
        {std::string{node_v310_decision_gate_state}},
        {std::string{node_v310_decision_digest}},
        {std::string{node_v310_decision}},
        {std::string{node_v310_selected_path}},
        {std::string{node_v309_profile_version}},
        {std::string{node_v309_verification_state}},
        {std::string{node_v309_verification_digest}},
        {std::string{node_v308_packet_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(prerequisite_count)},
        {std::to_string(missing_prerequisite_count)},
        {std::to_string(no_go_condition_count)},
        {std::to_string(required_field_count)},
        {std::to_string(prohibited_field_count)},
        {std::to_string(no_go_boundary_count)},
        {"human-approval-artifact-review-post-echo-decision-gate-non-participation-only"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-human-approval-post-echo-decision", parts);
}

std::string
format_credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto decision_gate = json_object({
        {"decision_digest", json_string(node_v310_decision_digest)},
        {"gate_mode", json_string(node_v310_gate_mode)},
        {"source_span", json_string(node_v310_source_span)},
        {"decision", json_string(node_v310_decision)},
        {"selected_path", json_string(node_v310_selected_path)},
        {"decision_reason", json_string(node_v310_reason)},
        {"allows_parallel_java_v144_mini_kv_v137_echo_request", json_bool(true)},
        {"allows_node_v311_before_upstream_echo", json_bool(false)},
        {"allows_disabled_runtime_shell_implementation", json_bool(false)},
        {"allows_disabled_runtime_shell_invocation", json_bool(false)},
        {"allows_real_resolver_implementation", json_bool(false)},
        {"allows_secret_provider_instantiation", json_bool(false)},
        {"allows_resolver_client_instantiation", json_bool(false)},
        {"allows_credential_value_read", json_bool(false)},
        {"allows_raw_endpoint_url_parse", json_bool(false)},
        {"allows_external_request", json_bool(false)},
        {"allows_managed_audit_connection", json_bool(false)},
        {"allows_schema_migration", json_bool(false)},
        {"allows_approval_ledger_write", json_bool(false)},
        {"allows_automatic_upstream_start", json_bool(false)},
        {"required_prerequisites", format_required_prerequisites_json()},
        {"explicit_no_go_conditions", format_no_go_conditions_json()},
        {"prerequisite_count", json_integer(prerequisite_count)},
        {"missing_prerequisite_count", json_integer(missing_prerequisite_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
    });

    const auto necessity_proof = json_object({
        {"proof_complete", json_bool(true)},
        {"blocker_resolved",
         json_string("v309 aligned Java v143 and mini-kv v136 echoes, but it did not decide whether post-echo work "
                     "may continue or which approval prerequisites remain missing.")},
        {"consumer", json_string("Java v144 and mini-kv v137, then Node v311")},
        {"why_v309_cannot_be_reused",
         json_string("v309 is an upstream echo verification only; it proves alignment but does not publish a "
                     "decision gate that upstreams can echo before any later runtime-shell discussion.")},
        {"existing_report_reuse_decision",
         json_string("Reuse v309 as source evidence, but create v310 as the minimal decision layer that names the "
                     "still-missing post-echo prerequisites.")},
        {"stop_condition",
         json_string("Stop immediately if the next step requires credential values, raw endpoint URLs, "
                     "provider/client instantiation, HTTP/TCP, runtime shell implementation or invocation, "
                     "ledger/schema writes, mini-kv write/admin commands, authority state, or automatic upstream "
                     "start.")},
    });

    const auto source_node_v310_summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_check_count", json_integer(source_check_count)},
        {"source_passed_check_count", json_integer(source_check_count)},
        {"prerequisite_count", json_integer(prerequisite_count)},
        {"missing_prerequisite_count", json_integer(missing_prerequisite_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto source_node_v310_reference = json_object({
        {"source_version", json_string("Node v310")},
        {"profile_version", json_string(node_v310_profile_version)},
        {"decision_gate_state", json_string(node_v310_decision_gate_state)},
        {"runtime_shell_chain_decision", json_string(runtime_shell_chain_decision)},
        {"ready_for_post_echo_decision_gate", json_bool(true)},
        {"read_only_decision_gate", json_bool(true)},
        {"human_approval_artifact_review_post_echo_decision_gate_only", json_bool(true)},
        {"consumes_node_v309_human_approval_artifact_review_upstream_echo_verification", json_bool(true)},
        {"ready_for_parallel_java_v144_mini_kv_v137_echo_request", json_bool(true)},
        {"ready_for_node_v311_before_upstream_echo", json_bool(false)},
        {"decision_gate", decision_gate},
        {"necessity_proof", necessity_proof},
        {"summary", source_node_v310_summary},
    });

    const auto source_node_v309_reference = json_object({
        {"source_version", json_string("Node v309")},
        {"profile_version", json_string(node_v309_profile_version)},
        {"verification_state", json_string(node_v309_verification_state)},
        {"ready_for_human_approval_artifact_review_upstream_echo_verification", json_bool(true)},
        {"read_only_upstream_echo_verification", json_bool(true)},
        {"verification_digest", json_string(node_v309_verification_digest)},
        {"verification_mode", json_string("human-approval-artifact-review-upstream-echo-verification-only")},
        {"source_span", json_string(node_v309_source_span)},
        {"source_node_v308_review_packet_digest", json_string(node_v308_packet_digest)},
        {"source_node_v308_required_field_count", json_integer(required_field_count)},
        {"source_node_v308_prohibited_field_count", json_integer(prohibited_field_count)},
        {"source_node_v308_rejection_reason_count", json_integer(rejection_reason_count)},
        {"source_node_v308_missing_field_check_count", json_integer(missing_field_check_count)},
        {"source_node_v308_no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"source_node_v308_upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"source_check_count", json_integer(source_check_count)},
        {"source_passed_check_count", json_integer(source_check_count)},
        {"source_node_v308_check_count", json_integer(source_node_v308_check_count)},
        {"source_node_v308_passed_check_count", json_integer(source_node_v308_check_count)},
        {"java_v143_echo_ready", json_bool(true)},
        {"mini_kv_v136_receipt_ready", json_bool(true)},
        {"upstream_echo_aligned", json_bool(true)},
        {"review_packet_contract_aligned", json_bool(true)},
        {"side_effect_boundaries_aligned", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto mini_kv_receipt = json_object({
        {"receipt_mode",
         json_string("human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt-only")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(check_count)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_check_count", json_integer(source_check_count)},
        {"source_passed_check_count", json_integer(source_check_count)},
        {"source_node_v308_check_count", json_integer(source_node_v308_check_count)},
        {"source_node_v308_passed_check_count", json_integer(source_node_v308_check_count)},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"missing_field_check_count", json_integer(missing_field_check_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"prerequisite_count", json_integer(prerequisite_count)},
        {"missing_prerequisite_count", json_integer(missing_prerequisite_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"proof_complete", json_bool(true)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({json_object({
        {"code", json_string("POST_ECHO_DECISION_DOES_NOT_AUTHORIZE_RUNTIME")},
        {"severity", json_string("warning")},
    })});

    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_JAVA_V144_AND_MINI_KV_V137_IN_PARALLEL")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("KEEP_RUNTIME_SHELL_BLOCKED")}, {"severity", json_string("recommendation")}}),
    });

    const auto evidence_endpoints = json_object({
        {"human_approval_artifact_review_post_echo_decision_gate_json", json_string(node_v310_json)},
        {"human_approval_artifact_review_post_echo_decision_gate_markdown", json_string(node_v310_markdown)},
        {"source_node_v309_json", json_string(node_v309_json)},
        {"source_node_v309_markdown", json_string(node_v309_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v144",
         json_string("Java v144 human approval artifact review post-echo decision gate echo")},
        {"recommended_parallel_mini_kv_v137",
         json_string("mini-kv v137 human approval artifact review post-echo decision gate non-participation receipt")},
        {"next_node_verification", json_string("Node v311")},
    });

    const auto next_required_evidence_versions = json_array({json_string("Java v144"), json_string("mini-kv v137")});
    const auto next_actions = json_array({
        json_string("Archive mini-kv v137 as a post-echo decision gate non-participation receipt."),
        json_string("Keep Node v311 blocked until Java v144 and mini-kv v137 are both available."),
        json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
        json_string("Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, "
                    "write ledgers or schema state, auto-start services, or execute "
                    "LOAD/COMPACT/RESTORE/SETNXEX."),
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string(receipt_version)},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_decision_gate", json_string(source_decision_gate)},
        {"source_upstream_echo", json_string(source_upstream_echo)},
        {"consumer_hint", json_string(receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(receipt_release_version)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_role", json_string(runtime_role)},
        {"source_node_v310_reference", source_node_v310_reference},
        {"source_node_v309_reference", source_node_v309_reference},
        {"mini_kv_receipt", mini_kv_receipt},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", next_required_evidence_versions},
        {"next_actions", next_actions},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(
             credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_digest(
                 read_commands))},
    };
    append_closed_boundary_flag_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
