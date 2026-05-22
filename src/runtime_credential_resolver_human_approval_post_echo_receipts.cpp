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

constexpr std::string_view receipt_version =
    "mini-kv-credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer =
    "Node v311 human approval artifact review post-echo decision upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v137";
constexpr std::string_view receipt_artifact_path_hint = "d/137/";
constexpr std::string_view source_decision_gate =
    "Node v310 human approval artifact review post-echo decision gate";
constexpr std::string_view source_upstream_echo =
    "Node v309 human approval artifact review upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v310_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-post-echo-decision-gate.v1";
constexpr std::string_view node_v310_decision_gate_state =
    "human-approval-artifact-review-post-echo-decision-gate-ready";
constexpr std::string_view node_v310_decision_digest =
    "91c0927b63fee6dd0fb44e27cd82c7f700ceeeb4cef750606777bd4d3436102c";
constexpr std::string_view node_v310_gate_mode =
    "human-approval-artifact-review-post-echo-decision-gate-only";
constexpr std::string_view node_v310_decision =
    "continue-only-as-blocked-post-echo-prerequisite-review";
constexpr std::string_view node_v310_selected_path =
    "request-read-only-upstream-decision-echo-before-any-runtime-shell";
constexpr std::string_view node_v310_source_span =
    "Node v308 + Java v143 + mini-kv v136 + Node v309";
constexpr std::string_view node_v310_reason =
    "Node v309 aligned Java v143 and mini-kv v136 with the Node v308 review packet, but it did not provide a signed artifact instance, credential-handle approval, endpoint allowlist approval, no-network safety evidence, abort semantics, or upstream echo of this decision gate.";

constexpr std::string_view node_v309_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-upstream-echo-verification.v1";
constexpr std::string_view node_v309_verification_state =
    "human-approval-artifact-review-upstream-echo-verification-ready";
constexpr std::string_view node_v309_verification_digest =
    "b306972aeab587fff6905759ad0fa2968af235cb13f09063c1c6c3af14e7913d";
constexpr std::string_view node_v309_source_span = "Node v308 + Java v143 + mini-kv v136";
constexpr std::string_view node_v308_packet_digest =
    "b0dda954c509337c96a645c177be521d0a200d8f8e6d52081ed8c0df9a43ccf3";
constexpr std::string_view runtime_shell_chain_decision =
    "require-explicit-approval-prerequisites-before-runtime-shell";

constexpr std::string_view active_plan =
    "docs/plans2/v309-post-human-approval-artifact-review-upstream-echo-roadmap.md";
constexpr std::string_view node_v310_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-post-echo-decision-gate";
constexpr std::string_view node_v310_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-post-echo-decision-gate?format=markdown";
constexpr std::string_view node_v309_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-upstream-echo-verification";
constexpr std::string_view node_v309_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-upstream-echo-verification?format=markdown";

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
    "credential resolver human approval artifact review post-echo decision gate non-participation receipt only; mini-kv echoes Node v310's blocked post-echo decision gate, records six documented-missing prerequisites and nine no-go conditions, and does not accept credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP payloads, ledger mutations, schema SQL, mini-kv write commands, runtime shell invocation requests, authority claims, or LOAD/COMPACT/RESTORE/SETNXEX execution";
constexpr std::string_view node_action =
    "verify mini-kv v137 human approval artifact review post-echo decision gate non-participation receipt with Java v144 before Node v311 upstream echo verification";

std::string format_required_prerequisites_json() {
    return "[{\"id\":\"signed-human-approval-artifact\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"credential-handle-approval\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"endpoint-handle-allowlist-approval\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"no-network-safety-fixture\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"abort-rollback-semantics\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"java-mini-kv-decision-echo\",\"status\":\"documented-missing\",\"required_before_runtime_shell\":true}]";
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

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_only\":true,"
           "\"read_only_decision_gate\":true,"
           "\"consumes_node_v310_human_approval_artifact_review_post_echo_decision_gate\":true,"
           "\"consumes_node_v309_human_approval_artifact_review_upstream_echo_verification\":true,"
           "\"ready_for_node_v311_human_approval_artifact_review_post_echo_decision_upstream_echo_verification\":true,"
           "\"ready_for_node_v311_before_upstream_echo\":false,"
           "\"ready_for_disabled_runtime_shell_implementation\":false,"
           "\"ready_for_disabled_runtime_shell_invocation\":false,"
           "\"ready_for_managed_audit_resolver_implementation\":false,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"ready_for_production_audit\":false,"
           "\"ready_for_production_window\":false,"
           "\"ready_for_production_operations\":false,"
           "\"runtime_shell_implemented\":false,"
           "\"runtime_shell_enabled\":false,"
           "\"runtime_shell_invocation_allowed\":false,"
           "\"runtime_shell_implementation_allowed\":false,"
           "\"real_resolver_implementation_allowed\":false,"
           "\"credential_resolver_implemented\":false,"
           "\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,"
           "\"secret_provider_instantiated\":false,"
           "\"fake_secret_provider_instantiated\":false,"
           "\"fake_resolver_client_instantiated\":false,"
           "\"provider_client_instantiation_allowed\":false,"
           "\"credential_value_accepted\":false,"
           "\"credential_value_read_allowed\":false,"
           "\"credential_value_read\":false,"
           "\"credential_value_provided\":false,"
           "\"credential_value_loaded\":false,"
           "\"credential_value_stored\":false,"
           "\"credential_value_included\":false,"
           "\"credential_value_rendered\":false,"
           "\"raw_endpoint_url_accepted\":false,"
           "\"raw_endpoint_url_parse_allowed\":false,"
           "\"raw_endpoint_url_render_allowed\":false,"
           "\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_rendered\":false,"
           "\"external_request_payload_accepted\":false,"
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
           "\"approval_ledger_write_allowed\":false,"
           "\"approval_ledger_written\":false,"
           "\"approval_ledger_write_executed\":false,"
           "\"managed_audit_write_executed\":false,"
           "\"production_record_written\":false,"
           "\"schema_migration_allowed\":false,"
           "\"schema_migration_executed\":false,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"schema_migration_execution_allowed\":false,"
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

std::string format_credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":" + field_string(receipt_version) +
           ",\"receipt_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"source_decision_gate\":" + field_string(source_decision_gate) +
           ",\"source_upstream_echo\":" + field_string(source_upstream_echo) +
           ",\"consumer_hint\":" + field_string(receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_role\":" + field_string(runtime_role) +
           ",\"source_node_v310_reference\":{\"source_version\":\"Node v310\","
           "\"profile_version\":" + field_string(node_v310_profile_version) +
           ",\"decision_gate_state\":" + field_string(node_v310_decision_gate_state) +
           ",\"runtime_shell_chain_decision\":" + field_string(runtime_shell_chain_decision) +
           ",\"ready_for_post_echo_decision_gate\":true"
           ",\"read_only_decision_gate\":true"
           ",\"human_approval_artifact_review_post_echo_decision_gate_only\":true"
           ",\"consumes_node_v309_human_approval_artifact_review_upstream_echo_verification\":true"
           ",\"ready_for_parallel_java_v144_mini_kv_v137_echo_request\":true"
           ",\"ready_for_node_v311_before_upstream_echo\":false"
           ",\"decision_gate\":{\"decision_digest\":" + field_string(node_v310_decision_digest) +
           ",\"gate_mode\":" + field_string(node_v310_gate_mode) +
           ",\"source_span\":" + field_string(node_v310_source_span) +
           ",\"decision\":" + field_string(node_v310_decision) +
           ",\"selected_path\":" + field_string(node_v310_selected_path) +
           ",\"decision_reason\":" + field_string(node_v310_reason) +
           ",\"allows_parallel_java_v144_mini_kv_v137_echo_request\":true"
           ",\"allows_node_v311_before_upstream_echo\":false"
           ",\"allows_disabled_runtime_shell_implementation\":false"
           ",\"allows_disabled_runtime_shell_invocation\":false"
           ",\"allows_real_resolver_implementation\":false"
           ",\"allows_secret_provider_instantiation\":false"
           ",\"allows_resolver_client_instantiation\":false"
           ",\"allows_credential_value_read\":false"
           ",\"allows_raw_endpoint_url_parse\":false"
           ",\"allows_external_request\":false"
           ",\"allows_managed_audit_connection\":false"
           ",\"allows_schema_migration\":false"
           ",\"allows_approval_ledger_write\":false"
           ",\"allows_automatic_upstream_start\":false"
           ",\"required_prerequisites\":" + format_required_prerequisites_json() +
           ",\"explicit_no_go_conditions\":" + format_no_go_conditions_json() +
           ",\"prerequisite_count\":" + std::to_string(prerequisite_count) +
           ",\"missing_prerequisite_count\":" + std::to_string(missing_prerequisite_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) + "}"
           ",\"necessity_proof\":{\"proof_complete\":true"
           ",\"blocker_resolved\":\"v309 aligned Java v143 and mini-kv v136 echoes, but it did not decide whether post-echo work may continue or which approval prerequisites remain missing.\""
           ",\"consumer\":\"Java v144 and mini-kv v137, then Node v311\""
           ",\"why_v309_cannot_be_reused\":\"v309 is an upstream echo verification only; it proves alignment but does not publish a decision gate that upstreams can echo before any later runtime-shell discussion.\""
           ",\"existing_report_reuse_decision\":\"Reuse v309 as source evidence, but create v310 as the minimal decision layer that names the still-missing post-echo prerequisites.\""
           ",\"stop_condition\":\"Stop immediately if the next step requires credential values, raw endpoint URLs, provider/client instantiation, HTTP/TCP, runtime shell implementation or invocation, ledger/schema writes, mini-kv write/admin commands, authority state, or automatic upstream start.\"}"
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_check_count\":" + std::to_string(source_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"prerequisite_count\":" + std::to_string(prerequisite_count) +
           ",\"missing_prerequisite_count\":" + std::to_string(missing_prerequisite_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "} }"
           ",\"source_node_v309_reference\":{\"source_version\":\"Node v309\""
           ",\"profile_version\":" + field_string(node_v309_profile_version) +
           ",\"verification_state\":" + field_string(node_v309_verification_state) +
           ",\"ready_for_human_approval_artifact_review_upstream_echo_verification\":true"
           ",\"read_only_upstream_echo_verification\":true"
           ",\"verification_digest\":" + field_string(node_v309_verification_digest) +
           ",\"verification_mode\":\"human-approval-artifact-review-upstream-echo-verification-only\""
           ",\"source_span\":" + field_string(node_v309_source_span) +
           ",\"source_node_v308_review_packet_digest\":" + field_string(node_v308_packet_digest) +
           ",\"source_node_v308_required_field_count\":" + std::to_string(required_field_count) +
           ",\"source_node_v308_prohibited_field_count\":" + std::to_string(prohibited_field_count) +
           ",\"source_node_v308_rejection_reason_count\":" + std::to_string(rejection_reason_count) +
           ",\"source_node_v308_missing_field_check_count\":" + std::to_string(missing_field_check_count) +
           ",\"source_node_v308_no_go_boundary_count\":" + std::to_string(no_go_boundary_count) +
           ",\"source_node_v308_upstream_echo_request_count\":" + std::to_string(upstream_echo_request_count) +
           ",\"source_check_count\":" + std::to_string(source_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v308_check_count\":" + std::to_string(source_node_v308_check_count) +
           ",\"source_node_v308_passed_check_count\":" + std::to_string(source_node_v308_check_count) +
           ",\"java_v143_echo_ready\":true"
           ",\"mini_kv_v136_receipt_ready\":true"
           ",\"upstream_echo_aligned\":true"
           ",\"review_packet_contract_aligned\":true"
           ",\"side_effect_boundaries_aligned\":true"
           ",\"implementation_still_blocked\":true"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt-only\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":" + std::to_string(check_count) + "}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_check_count\":" + std::to_string(source_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v308_check_count\":" + std::to_string(source_node_v308_check_count) +
           ",\"source_node_v308_passed_check_count\":" + std::to_string(source_node_v308_check_count) +
           ",\"required_field_count\":" + std::to_string(required_field_count) +
           ",\"prohibited_field_count\":" + std::to_string(prohibited_field_count) +
           ",\"rejection_reason_count\":" + std::to_string(rejection_reason_count) +
           ",\"missing_field_check_count\":" + std::to_string(missing_field_check_count) +
           ",\"no_go_boundary_count\":" + std::to_string(no_go_boundary_count) +
           ",\"upstream_echo_request_count\":" + std::to_string(upstream_echo_request_count) +
           ",\"prerequisite_count\":" + std::to_string(prerequisite_count) +
           ",\"missing_prerequisite_count\":" + std::to_string(missing_prerequisite_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"proof_complete\":true"
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"POST_ECHO_DECISION_DOES_NOT_AUTHORIZE_RUNTIME\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V144_AND_MINI_KV_V137_IN_PARALLEL\",\"severity\":\"recommendation\"},"
           "{\"code\":\"KEEP_RUNTIME_SHELL_BLOCKED\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"human_approval_artifact_review_post_echo_decision_gate_json\":" +
           field_string(node_v310_json) +
           ",\"human_approval_artifact_review_post_echo_decision_gate_markdown\":" +
           field_string(node_v310_markdown) +
           ",\"source_node_v309_json\":" + field_string(node_v309_json) +
           ",\"source_node_v309_markdown\":" + field_string(node_v309_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v144\":\"Java v144 human approval artifact review post-echo decision gate echo\""
           ",\"recommended_parallel_mini_kv_v137\":\"mini-kv v137 human approval artifact review post-echo decision gate non-participation receipt\""
           ",\"next_node_verification\":\"Node v311\"}"
           ",\"next_required_evidence_versions\":[\"Java v144\",\"mini-kv v137\"]"
           ",\"next_actions\":[\"Archive mini-kv v137 as a post-echo decision gate non-participation receipt.\","
           "\"Keep Node v311 blocked until Java v144 and mini-kv v137 are both available.\","
           "\"Do not implement, enable, or invoke a disabled runtime shell from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, write ledgers or schema state, auto-start services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
