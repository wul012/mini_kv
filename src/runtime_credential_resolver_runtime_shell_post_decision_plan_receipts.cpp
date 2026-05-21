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

constexpr std::string_view receipt_consumer =
    "Node v302 runtime shell post-decision plan intake upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v133";
constexpr std::string_view receipt_artifact_path_hint = "d/133/";
constexpr std::string_view source_plan_intake =
    "Node v301 credential resolver runtime shell post-decision continuation plan intake";
constexpr std::string_view source_upstream_echo =
    "Node v300 runtime shell decision record upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v301_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-continuation-plan-intake.v1";
constexpr std::string_view node_v301_state =
    "runtime-shell-post-decision-continuation-plan-intake-ready";
constexpr std::string_view node_v301_intake_digest =
    "ce85fa1c85cb2759abda5289f9bfec0cf162b71f2da0a12dfdf2ffe7cee8d65a";
constexpr std::string_view node_v301_proof_digest =
    "9b7d9215e58ce16ad0c2b67f4560005ae3b1436fbf4087fb0ebdfca5c841d00e";
constexpr std::string_view node_v301_selected_decision = "continue-blocked-planning";
constexpr std::string_view node_v301_intake_mode =
    "runtime-shell-post-decision-continuation-plan-intake-only";
constexpr std::string_view node_v301_source_span = "Node v300";
constexpr std::string_view node_v301_blocker =
    "v300 verified Java v135 and mini-kv v132 agreement with the blocked decision record, but it did not decide the post-decision continuation path.";
constexpr std::string_view node_v301_consumer =
    "Java v136 and mini-kv v133 consume v301 as read-only echoes; Node v302 consumes both echoes to verify post-decision plan alignment.";
constexpr std::string_view node_v301_v300_reuse =
    "v300 is an upstream echo verification for Node v299; it lacks a selected continuation option, v136/v133 handoff target, and explicit stop condition for the post-decision chain.";
constexpr std::string_view node_v301_stop_condition =
    "Stop immediately if the next step requires credential values, raw endpoint URLs, provider/client instantiation, HTTP/TCP, runtime shell implementation or invocation, ledger/schema writes, or automatic upstream start.";

constexpr std::string_view node_v300_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-decision-record-upstream-echo-verification.v1";
constexpr std::string_view node_v300_state =
    "runtime-shell-decision-record-upstream-echo-verification-ready";
constexpr std::string_view node_v300_verification_digest =
    "a7d1cc22ce3f8d6da30d5a91d2d1f8fc13e480babfac8296c4b8ca86519d4f8c";
constexpr std::string_view node_v300_source_span =
    "Node v299 + Java v135 + mini-kv v132";

constexpr std::string_view active_plan =
    "docs/plans2/v300-post-runtime-shell-decision-record-upstream-echo-roadmap.md";
constexpr std::string_view node_v301_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-continuation-plan-intake";
constexpr std::string_view node_v301_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-continuation-plan-intake?format=markdown";
constexpr std::string_view node_v300_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-decision-record-upstream-echo-verification";
constexpr std::string_view node_v300_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-decision-record-upstream-echo-verification?format=markdown";

constexpr int check_count = 25;
constexpr int source_check_count = 19;
constexpr int continuation_option_count = 4;
constexpr int selected_option_count = 1;
constexpr int rejected_runtime_option_count = 1;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver runtime shell post-decision plan intake non-participation receipt only; mini-kv echoes Node v301 blocked continuation plan intake, does not implement, enable, or invoke a runtime shell, does not instantiate providers or resolver clients, does not read credential values, does not parse raw endpoint URLs, does not send HTTP/TCP or external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view node_action =
    "verify mini-kv v133 runtime shell post-decision plan intake non-participation receipt with Java v136 before Node v302 upstream echo verification";

std::string format_continuation_options_json() {
    return "[{\"code\":\"CONTINUE_BLOCKED_PLANNING\",\"status\":\"selected\","
           "\"allowed_actions\":[\"write-v301-intake\",\"request-java-v136-echo\","
           "\"request-mini-kv-v133-non-participation\"],"
           "\"prohibited_actions\":[\"implement-runtime-shell\",\"invoke-runtime-shell\","
           "\"open-managed-audit-connection\"]},"
           "{\"code\":\"PAUSE_RUNTIME_SHELL_CHAIN\",\"status\":\"documented-alternative\","
           "\"allowed_actions\":[\"archive-v301-as-paused\",\"return-to-quality-work\"],"
           "\"prohibited_actions\":[\"treat-pause-as-production-approval\"]},"
           "{\"code\":\"REQUIRE_EXPLICIT_APPROVAL_PREREQUISITES\","
           "\"status\":\"documented-alternative\","
           "\"allowed_actions\":[\"list-approval-prerequisites\",\"keep-prerequisites-read-only\"],"
           "\"prohibited_actions\":[\"read-credential-value\",\"parse-raw-endpoint-url\","
           "\"instantiate-provider-client\"]},"
           "{\"code\":\"IMPLEMENT_RUNTIME_SHELL_NOW\",\"status\":\"rejected\","
           "\"allowed_actions\":[],"
           "\"prohibited_actions\":[\"implement-runtime-shell\",\"invoke-runtime-shell\","
           "\"send-external-request\",\"write-ledger-or-schema\"]}]";
}

std::string format_checks_json() {
    return "{\"source_node_v300_loaded\":true,"
           "\"source_node_v300_ready\":true,"
           "\"source_node_v300_ready_for_post_decision_plan\":true,"
           "\"source_node_v300_keeps_runtime_blocked\":true,"
           "\"source_node_v300_keeps_credential_boundary_closed\":true,"
           "\"source_node_v300_keeps_endpoint_boundary_closed\":true,"
           "\"source_node_v300_keeps_connection_boundary_closed\":true,"
           "\"source_node_v300_keeps_write_boundary_closed\":true,"
           "\"continuation_decision_selected\":true,"
           "\"decision_options_documented\":true,"
           "\"runtime_implementation_option_rejected\":true,"
           "\"necessity_proof_has_blocker\":true,"
           "\"necessity_proof_has_consumer\":true,"
           "\"necessity_proof_explains_v300_reuse_boundary\":true,"
           "\"necessity_proof_defines_stop_condition\":true,"
           "\"necessity_proof_complete\":true,"
           "\"runtime_shell_implementation_still_forbidden\":true,"
           "\"runtime_shell_invocation_still_forbidden\":true,"
           "\"provider_client_instantiation_still_forbidden\":true,"
           "\"external_request_still_forbidden\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"mini_kv_non_participation_recorded\":true}";
}

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"runtime_shell_post_decision_plan_intake_non_participation_receipt_only\":true,"
           "\"runtime_shell_post_decision_continuation_plan_intake_only\":true,"
           "\"post_decision_plan_intake_only\":true,"
           "\"plan_intake_state\":\"runtime-shell-post-decision-continuation-plan-intake-ready\","
           "\"selected_continuation_decision\":\"continue-blocked-planning\","
           "\"consumes_node_v301_runtime_shell_post_decision_plan_intake\":true,"
           "\"consumes_node_v300_runtime_shell_decision_record_upstream_echo_verification\":true,"
           "\"ready_for_node_v302_post_decision_plan_intake_upstream_echo_verification\":true,"
           "\"ready_for_node_v302_runtime_shell_implementation\":false,"
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
           "\"disabled_runtime_shell_participates\":false,"
           "\"test_only_fake_harness_allowed\":false,"
           "\"test_only_fake_harness_execution_allowed\":false,"
           "\"fake_harness_runtime_enabled\":false,"
           "\"fake_harness_runtime_invoked\":false,"
           "\"real_resolver_implementation_allowed\":false,"
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

std::string credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_plan_intake}},
        {std::string{source_upstream_echo}},
        {std::string{node_v301_profile_version}},
        {std::string{node_v301_state}},
        {std::string{node_v301_intake_digest}},
        {std::string{node_v301_proof_digest}},
        {std::string{node_v301_selected_decision}},
        {std::string{node_v301_intake_mode}},
        {std::string{node_v300_profile_version}},
        {std::string{node_v300_state}},
        {std::string{node_v300_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(continuation_option_count)},
        {std::to_string(selected_option_count)},
        {std::to_string(rejected_runtime_option_count)},
        {"runtime-shell-post-decision-plan-intake-non-participation-receipt-only"},
        {"continue-blocked-planning"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-runtime-shell-post-decision-plan-intake", parts);
}

std::string format_credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"source_plan_intake\":" + field_string(source_plan_intake) +
           ",\"source_upstream_echo\":" + field_string(source_upstream_echo) +
           ",\"consumer_hint\":" + field_string(receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(runtime_role) +
           ",\"source_node_v301_reference\":{\"source_version\":\"Node v301\","
           "\"profile_version\":" + field_string(node_v301_profile_version) +
           ",\"plan_intake_state\":" + field_string(node_v301_state) +
           ",\"intake_digest\":" + field_string(node_v301_intake_digest) +
           ",\"proof_digest\":" + field_string(node_v301_proof_digest) +
           ",\"intake_mode\":" + field_string(node_v301_intake_mode) +
           ",\"source_span\":" + field_string(node_v301_source_span) +
           ",\"selected_continuation_decision\":" + field_string(node_v301_selected_decision) +
           ",\"decision_option_count\":" + std::to_string(continuation_option_count) +
           ",\"selected_decision_option_count\":" + std::to_string(selected_option_count) +
           ",\"rejected_runtime_implementation_option_count\":" +
           std::to_string(rejected_runtime_option_count) +
           ",\"next_java_echo_version\":\"Java v136\""
           ",\"next_mini_kv_receipt_version\":\"mini-kv v133\""
           ",\"next_node_verification_version\":\"Node v302\""
           ",\"blocker_resolved\":" + field_string(node_v301_blocker) +
           ",\"consumer\":" + field_string(node_v301_consumer) +
           ",\"why_v300_cannot_be_reused\":" + field_string(node_v301_v300_reuse) +
           ",\"stop_condition\":" + field_string(node_v301_stop_condition) +
           ",\"proof_complete\":true"
           ",\"continuation_options\":" + format_continuation_options_json() +
           ",\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) +
           ",\"read_only_plan_intake\":true"
           ",\"runtime_shell_post_decision_continuation_plan_intake_only\":true"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_enabled\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"runtime_shell_implementation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"raw_endpoint_url_rendered\":false"
           ",\"external_request_sent\":false"
           ",\"secret_provider_instantiated\":false"
           ",\"resolver_client_instantiated\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"source_node_v300_reference\":{\"source_version\":\"Node v300\","
           "\"profile_version\":" + field_string(node_v300_profile_version) +
           ",\"verification_state\":" + field_string(node_v300_state) +
           ",\"verification_digest\":" + field_string(node_v300_verification_digest) +
           ",\"source_span\":" + field_string(node_v300_source_span) +
           ",\"read_only_upstream_echo_verification\":true"
           ",\"runtime_shell_decision_record_upstream_echo_verification_only\":true"
           ",\"consumes_node_v299_runtime_shell_candidate_gate_decision_record\":true"
           ",\"consumes_java_v135_runtime_shell_decision_record_echo\":true"
           ",\"consumes_mini_kv_v132_runtime_shell_decision_record_non_participation_receipt\":true"
           ",\"ready_for_post_runtime_shell_decision_plan\":true"
           ",\"upstream_echo_aligned\":true"
           ",\"blocked_decision_aligned\":true"
           ",\"required_evidence_aligned\":true"
           ",\"no_go_conditions_aligned\":true"
           ",\"side_effect_boundaries_aligned\":true"
           ",\"implementation_still_blocked\":true"
           ",\"check_count\":" + std::to_string(source_check_count) +
           ",\"passed_check_count\":" + std::to_string(source_check_count) +
           ",\"production_blocker_count\":0"
           ",\"warning_count\":1"
           ",\"recommendation_count\":2"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_enabled\":false"
           ",\"runtime_shell_invocation_allowed\":false"
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
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"runtime-shell-post-decision-plan-intake-non-participation-receipt-only\","
           "\"shell_name\":\"credential-resolver-disabled-runtime-shell\""
           ",\"non_participation_scope\":\"post-decision plan intake, runtime shell implementation, invocation, provider/client wiring, credential reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and authority\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":25}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v300_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v300_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"continuation_option_count\":" + std::to_string(continuation_option_count) +
           ",\"selected_continuation_option_count\":" + std::to_string(selected_option_count) +
           ",\"rejected_runtime_implementation_option_count\":" +
           std::to_string(rejected_runtime_option_count) +
           ",\"proof_complete\":true"
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"CONTINUATION_PLAN_DOES_NOT_AUTHORIZE_RUNTIME\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"REQUEST_PARALLEL_JAVA_MINI_KV_ECHO\",\"severity\":\"recommendation\"},"
           "{\"code\":\"STOP_CHAIN_AFTER_V302_WITHOUT_NEW_BLOCKER\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"runtime_shell_post_decision_plan_intake_json\":" +
           field_string(node_v301_json) +
           ",\"runtime_shell_post_decision_plan_intake_markdown\":" +
           field_string(node_v301_markdown) +
           ",\"source_node_v300_json\":" + field_string(node_v300_json) +
           ",\"source_node_v300_markdown\":" + field_string(node_v300_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v136\":\"Java v136 post-decision plan intake echo\""
           ",\"recommended_parallel_mini_kv_v133\":\"mini-kv v133 post-decision plan intake non-participation receipt\""
           ",\"next_node_verification\":\"Node v302\"}"
           ",\"next_required_evidence_versions\":[\"Java v136\",\"mini-kv v133\"]"
           ",\"next_actions\":[\"Archive mini-kv v133 as a runtime shell post-decision plan intake non-participation receipt.\","
           "\"Keep Node v302 blocked until Java v136 and mini-kv v133 are both available.\","
           "\"Do not implement, enable, or invoke a disabled runtime shell from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
