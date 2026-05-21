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
    "Node v300 runtime shell decision record upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-runtime-shell-decision-record-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v132";
constexpr std::string_view receipt_artifact_path_hint = "d/132/";
constexpr std::string_view source_decision_record =
    "Node v299 credential resolver runtime shell candidate gate decision record";
constexpr std::string_view source_upstream_echo =
    "Node v298 runtime shell candidate gate upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v299_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-decision-record.v1";
constexpr std::string_view node_v299_state =
    "runtime-shell-candidate-gate-decision-record-ready";
constexpr std::string_view node_v299_decision = "blocked";
constexpr std::string_view node_v299_decision_digest =
    "4f6f73fa2806a9ba74174d7bbab17b43459bd1d790237276d95a3937c646e9c0";
constexpr std::string_view node_v299_record_mode =
    "runtime-shell-candidate-gate-decision-record-only";
constexpr std::string_view node_v299_decision_scope =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell";
constexpr std::string_view node_v299_source_span =
    "Node v297-v298 + Java v134 + mini-kv v131";
constexpr std::string_view node_v299_decision_reason =
    "Node v298 verified the runtime shell candidate gate echoes, but runtime shell implementation remains blocked until a separate successor plan with explicit approval is produced.";

constexpr std::string_view node_v298_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-upstream-echo-verification.v1";
constexpr std::string_view node_v298_state =
    "runtime-shell-candidate-gate-upstream-echo-verification-ready";
constexpr std::string_view node_v298_verification_digest =
    "fdf53be9d847694a7301731be659f694854a95d9b5a7a62b73dd069c6f08aa76";
constexpr std::string_view node_v298_source_span =
    "Node v297 + Java v134 + mini-kv v131";

constexpr std::string_view active_plan =
    "docs/plans2/v299-post-runtime-shell-candidate-gate-decision-roadmap.md";
constexpr std::string_view node_v299_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-decision-record";
constexpr std::string_view node_v299_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-decision-record?format=markdown";
constexpr std::string_view node_v298_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-upstream-echo-verification";
constexpr std::string_view node_v298_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-upstream-echo-verification?format=markdown";

constexpr int check_count = 14;
constexpr int source_check_count = 26;
constexpr int required_evidence_count = 4;
constexpr int missing_required_evidence_count = 0;
constexpr int no_go_condition_count = 6;
constexpr int source_required_gate_count = 5;
constexpr int source_documented_gate_count = 5;
constexpr int source_review_evidence_satisfied_count = 5;
constexpr int source_runtime_prerequisite_satisfied_count = 0;
constexpr int source_implementation_allowed_gate_count = 0;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;
constexpr int source_warning_count = 2;
constexpr int source_recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver runtime shell decision record non-participation receipt only; mini-kv echoes Node v299 blocked decision record, does not implement, enable, or invoke a runtime shell, does not instantiate providers or resolver clients, does not read credential values, does not parse raw endpoint URLs, does not send HTTP/TCP or external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view node_action =
    "verify mini-kv v132 runtime shell decision record non-participation receipt with Java v135 before Node v300 upstream echo verification";

std::string format_required_evidence_json() {
    return "[{\"id\":\"node-v298-upstream-echo-ready\","
           "\"label\":\"Node v298 upstream echo verification\","
           "\"current_evidence\":\"Node v298 readyForUpstreamEchoVerification\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"java-v134-echo-ready\","
           "\"label\":\"Java v134 runtime shell candidate gate echo\","
           "\"current_evidence\":\"present evidence and documented\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"mini-kv-v131-receipt-ready\","
           "\"label\":\"mini-kv v131 runtime shell candidate gate non-participation receipt\","
           "\"current_evidence\":\"present evidence and documented\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"runtime-shell-still-blocked\","
           "\"label\":\"Runtime shell remains blocked\","
           "\"current_evidence\":\"Node v298 still keeps runtime shell blocked\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true}]";
}

std::string format_no_go_conditions_json() {
    return "[{\"code\":\"RUNTIME_SHELL_IMPLEMENTATION_REQUIRED\","
           "\"condition\":\"The next step would have to implement or invoke a runtime shell.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"CREDENTIAL_VALUE_REQUIRED\","
           "\"condition\":\"The next step would have to read, store, render, or test credential values.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"RAW_ENDPOINT_URL_REQUIRED\","
           "\"condition\":\"The next step would have to parse or render a raw endpoint URL.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"MANAGED_AUDIT_CONNECTION_REQUIRED\","
           "\"condition\":\"The next step would have to open managed audit connectivity.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"LEDGER_SCHEMA_WRITE_REQUIRED\","
           "\"condition\":\"The next step would have to write ledger state or execute schema migration SQL.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"AUTOSTART_REQUIRED\","
           "\"condition\":\"The next step would have to auto-start Java, mini-kv, or managed audit services.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"}]";
}

std::string format_checks_json() {
    return "{\"source_node_v298_ready\":true,"
           "\"source_node_v298_keeps_implementation_blocked\":true,"
           "\"source_node_v298_keeps_side_effects_closed\":true,"
           "\"decision_record_state_ready\":true,"
           "\"decision_record_is_blocked\":true,"
           "\"decision_digest_recorded\":true,"
           "\"required_evidence_count_stable\":true,"
           "\"required_evidence_present\":true,"
           "\"no_go_condition_count_stable\":true,"
           "\"runtime_implementation_not_authorized\":true,"
           "\"parallel_java_v135_mini_kv_v132_echo_allowed\":true,"
           "\"node_v300_stays_blocked_until_parallel_echo\":true,"
           "\"mini_kv_non_participation_recorded\":true,"
           "\"side_effect_boundaries_closed\":true}";
}

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"runtime_shell_decision_record_non_participation_receipt_only\":true,"
           "\"runtime_shell_candidate_gate_decision_record_only\":true,"
           "\"decision_record_only\":true,"
           "\"decision_record_state\":\"runtime-shell-candidate-gate-decision-record-ready\","
           "\"runtime_shell_decision\":\"blocked\","
           "\"consumes_node_v299_runtime_shell_candidate_gate_decision_record\":true,"
           "\"consumes_node_v298_runtime_shell_candidate_gate_upstream_echo_verification\":true,"
           "\"ready_for_node_v300_runtime_shell_decision_record_upstream_echo_verification\":true,"
           "\"ready_for_node_v300_runtime_shell_implementation\":false,"
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

std::string credential_resolver_runtime_shell_decision_record_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_decision_record}},
        {std::string{source_upstream_echo}},
        {std::string{node_v299_profile_version}},
        {std::string{node_v299_state}},
        {std::string{node_v299_decision}},
        {std::string{node_v299_decision_digest}},
        {std::string{node_v299_record_mode}},
        {std::string{node_v299_decision_scope}},
        {std::string{node_v298_profile_version}},
        {std::string{node_v298_state}},
        {std::string{node_v298_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(required_evidence_count)},
        {std::to_string(no_go_condition_count)},
        {"runtime-shell-decision-record-non-participation-receipt-only"},
        {"blocked"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-runtime-shell-decision-record", parts);
}

std::string format_credential_resolver_runtime_shell_decision_record_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-runtime-shell-decision-record-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"source_decision_record\":" + field_string(source_decision_record) +
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
           ",\"source_node_v299_reference\":{\"source_version\":\"Node v299\","
           "\"profile_version\":" + field_string(node_v299_profile_version) +
           ",\"decision_record_state\":" + field_string(node_v299_state) +
           ",\"runtime_shell_decision\":" + field_string(node_v299_decision) +
           ",\"decision_digest\":" + field_string(node_v299_decision_digest) +
           ",\"record_mode\":" + field_string(node_v299_record_mode) +
           ",\"decision_scope\":" + field_string(node_v299_decision_scope) +
           ",\"source_span\":" + field_string(node_v299_source_span) +
           ",\"decision_reason\":" + field_string(node_v299_decision_reason) +
           ",\"upstream_echo_verified\":true"
           ",\"ready_for_parallel_java_v135_mini_kv_v132_echo_request\":true"
           ",\"ready_for_node_v300_runtime_shell_decision_record_upstream_echo_verification\":false"
           ",\"allows_node_v300_before_upstream_echo\":false"
           ",\"allows_disabled_runtime_shell_implementation\":false"
           ",\"allows_disabled_runtime_shell_invocation\":false"
           ",\"allows_real_resolver_implementation\":false"
           ",\"allows_fake_harness_runtime_implementation\":false"
           ",\"allows_secret_provider_instantiation\":false"
           ",\"allows_resolver_client_instantiation\":false"
           ",\"allows_credential_value_read\":false"
           ",\"allows_raw_endpoint_url_parse\":false"
           ",\"allows_external_request\":false"
           ",\"allows_managed_audit_connection\":false"
           ",\"allows_schema_migration\":false"
           ",\"allows_approval_ledger_write\":false"
           ",\"allows_automatic_upstream_start\":false"
           ",\"required_evidence_count\":" + std::to_string(required_evidence_count) +
           ",\"missing_required_evidence_count\":" +
           std::to_string(missing_required_evidence_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"required_evidence\":" + format_required_evidence_json() +
           ",\"explicit_no_go_conditions\":" + format_no_go_conditions_json() +
           ",\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) +
           ",\"read_only_decision_record\":true"
           ",\"runtime_shell_candidate_gate_decision_record_only\":true"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_enabled\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"secret_provider_instantiated\":false"
           ",\"resolver_client_instantiated\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"source_node_v298_reference\":{\"source_version\":\"Node v298\","
           "\"profile_version\":" + field_string(node_v298_profile_version) +
           ",\"verification_state\":" + field_string(node_v298_state) +
           ",\"verification_digest\":" + field_string(node_v298_verification_digest) +
           ",\"source_span\":" + field_string(node_v298_source_span) +
           ",\"read_only_upstream_echo_verification\":true"
           ",\"runtime_shell_candidate_gate_upstream_echo_verification_only\":true"
           ",\"consumes_java_v134_runtime_shell_candidate_gate_echo\":true"
           ",\"consumes_mini_kv_v131_runtime_shell_candidate_gate_non_participation_receipt\":true"
           ",\"ready_for_node_v299_runtime_shell_candidate_gate_decision_record\":true"
           ",\"ready_for_node_v299_runtime_shell_implementation\":false"
           ",\"check_count\":" + std::to_string(source_check_count) +
           ",\"passed_check_count\":" + std::to_string(source_check_count) +
           ",\"required_gate_count\":" + std::to_string(source_required_gate_count) +
           ",\"documented_gate_count\":" + std::to_string(source_documented_gate_count) +
           ",\"review_evidence_satisfied_count\":" +
           std::to_string(source_review_evidence_satisfied_count) +
           ",\"runtime_prerequisite_satisfied_count\":" +
           std::to_string(source_runtime_prerequisite_satisfied_count) +
           ",\"implementation_allowed_gate_count\":" +
           std::to_string(source_implementation_allowed_gate_count) +
           ",\"production_blocker_count\":0"
           ",\"warning_count\":" + std::to_string(source_warning_count) +
           ",\"recommendation_count\":" + std::to_string(source_recommendation_count) +
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_enabled\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"secret_provider_instantiated\":false"
           ",\"resolver_client_instantiated\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"runtime-shell-decision-record-non-participation-receipt-only\","
           "\"shell_name\":\"credential-resolver-disabled-runtime-shell\""
           ",\"non_participation_scope\":\"runtime shell decision record, implementation, invocation, provider/client wiring, credential reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and authority\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":18}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"required_evidence_count\":" + std::to_string(required_evidence_count) +
           ",\"missing_required_evidence_count\":" +
           std::to_string(missing_required_evidence_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"source_node_v298_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v298_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v298_required_gate_count\":" +
           std::to_string(source_required_gate_count) +
           ",\"source_node_v298_implementation_allowed_gate_count\":" +
           std::to_string(source_implementation_allowed_gate_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"DECISION_RECORD_ONLY_DOES_NOT_AUTHORIZE_RUNTIME\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V135_AND_MINIKV_V132_IN_PARALLEL\",\"severity\":\"recommendation\"},"
           "{\"code\":\"KEEP_NODE_V300_BEHIND_PARALLEL_EVIDENCE\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"runtime_shell_candidate_gate_decision_record_json\":" +
           field_string(node_v299_json) +
           ",\"runtime_shell_candidate_gate_decision_record_markdown\":" +
           field_string(node_v299_markdown) +
           ",\"source_node_v298_json\":" + field_string(node_v298_json) +
           ",\"source_node_v298_markdown\":" + field_string(node_v298_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v135\":\"Java v135 runtime shell decision record echo\""
           ",\"recommended_parallel_mini_kv_v132\":\"mini-kv v132 runtime shell decision record non-participation receipt\""
           ",\"next_node_verification\":\"Node v300\"}"
           ",\"next_required_evidence_versions\":[\"Java v135\",\"mini-kv v132\"]"
           ",\"next_actions\":[\"Archive mini-kv v132 as a runtime shell decision record non-participation receipt.\","
           "\"Keep Node v300 blocked until Java v135 and mini-kv v132 are both available.\","
           "\"Do not implement, enable, or invoke a disabled runtime shell from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_runtime_shell_decision_record_non_participation_receipt_digest(
               read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
