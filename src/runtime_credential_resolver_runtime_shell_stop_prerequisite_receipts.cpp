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
    "Node v305 runtime shell chain stop/prerequisite upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v134";
constexpr std::string_view receipt_artifact_path_hint = "d/134/";
constexpr std::string_view source_decision_record =
    "Node v304 runtime shell chain stop-or-prerequisite decision record";
constexpr std::string_view source_upstream_echo =
    "Node v303 post-decision plan intake upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v304_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-chain-stop-or-prerequisite-decision-record.v1";
constexpr std::string_view node_v304_state =
    "runtime-shell-chain-stop-or-prerequisite-decision-record-ready";
constexpr std::string_view node_v304_decision =
    "require-explicit-approval-prerequisites-before-runtime-shell";
constexpr std::string_view node_v304_decision_digest =
    "9212d0b804fdc1eda9098ac70d2441681730a98ff736776859811df9e288a654";
constexpr std::string_view node_v304_record_mode =
    "runtime-shell-chain-stop-or-prerequisite-decision-record-only";
constexpr std::string_view node_v304_decision_scope =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell";
constexpr std::string_view node_v304_source_span =
    "Node v303 + Java v136 + mini-kv v133";
constexpr std::string_view node_v304_decision_reason =
    "Node v303 aligned the post-decision intake echoes, but the runtime shell chain still lacks explicit operator approval, credential-handle readiness, no-network safety tests, abort semantics, and upstream echo of the prerequisite decision.";
constexpr std::string_view node_v304_selected_path =
    "continue-only-as-blocked-prerequisite-review";
constexpr std::string_view node_v304_blocker =
    "v303 aligned the post-decision plan intake echoes, but did not decide whether the runtime shell chain should stop or require explicit approval prerequisites.";
constexpr std::string_view node_v304_consumer =
    "Java v141 and mini-kv v134, then Node v305";
constexpr std::string_view node_v304_v303_reuse =
    "v303 verifies upstream echo alignment only; it does not enumerate missing prerequisites or publish a decision that Java and mini-kv can echo before any later runtime-shell discussion.";
constexpr std::string_view node_v304_stop_condition =
    "Stop immediately if the next step requires credential values, raw endpoint URLs, provider/client instantiation, HTTP/TCP, runtime shell implementation or invocation, ledger/schema writes, mini-kv write/admin commands, or automatic upstream start.";

constexpr std::string_view node_v303_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-plan-intake-upstream-echo-verification.v1";
constexpr std::string_view node_v303_state =
    "runtime-shell-post-decision-plan-intake-upstream-echo-verification-ready";
constexpr std::string_view node_v303_verification_digest =
    "9a2f807d01e34c632ee02ac40db98f5a217a7f186d6305c6b97309a6a7d257df";
constexpr std::string_view node_v303_source_span =
    "Node v301 + Node v302 + Java v136 + mini-kv v133";

constexpr std::string_view active_plan =
    "docs/plans2/v303-post-decision-plan-intake-upstream-echo-roadmap.md";
constexpr std::string_view node_v304_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-chain-stop-or-prerequisite-decision-record";
constexpr std::string_view node_v304_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-chain-stop-or-prerequisite-decision-record?format=markdown";
constexpr std::string_view node_v303_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-plan-intake-upstream-echo-verification";
constexpr std::string_view node_v303_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-plan-intake-upstream-echo-verification?format=markdown";

constexpr int check_count = 17;
constexpr int source_check_count = 23;
constexpr int source_production_blocker_count = 0;
constexpr int source_warning_count = 2;
constexpr int source_recommendation_count = 2;
constexpr int prerequisite_count = 6;
constexpr int missing_runtime_prerequisite_count = 6;
constexpr int no_go_condition_count = 8;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver runtime shell chain stop/prerequisite non-participation receipt only; mini-kv echoes Node v304 stop-or-prerequisite decision record, does not implement, enable, or invoke a runtime shell, does not instantiate providers or resolver clients, does not read credential values, does not parse raw endpoint URLs, does not send HTTP/TCP or external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view node_action =
    "verify mini-kv v134 runtime shell chain stop/prerequisite non-participation receipt with Java v141 before Node v305 upstream echo verification";

std::string format_required_prerequisites_json() {
    return "[{\"id\":\"operator-approval-artifact\","
           "\"label\":\"Operator approval artifact\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: no signed operator approval artifact has been produced for runtime shell implementation\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"credential-handle-readiness\","
           "\"label\":\"Credential handle readiness\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: only credential handle/review status can be referenced; credential value reading remains forbidden\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"raw-endpoint-allowlist-review\","
           "\"label\":\"Raw endpoint allowlist review\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: endpoint handle can be reviewed, but raw endpoint URL parsing/rendering remains forbidden\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"no-network-test-fixture\","
           "\"label\":\"No-network safety tests\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: no test has proven runtime shell code cannot dial managed audit before explicit approval\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"manual-abort-and-rollback-semantics\","
           "\"label\":\"Manual abort and rollback semantics\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: abort semantics are documented as required, but no executable runtime shell abort contract exists\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"java-mini-kv-prerequisite-echo\","
           "\"label\":\"Java/mini-kv prerequisite echo\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: Java v141 and mini-kv v134 have not yet echoed this stop/prerequisite decision\","
           "\"required_before_runtime_shell\":true}]";
}

std::string format_no_go_conditions_json() {
    return "[{\"code\":\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"RUNTIME_SHELL_INVOCATION_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"CREDENTIAL_VALUE_READ_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"RAW_ENDPOINT_URL_PARSE_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"PROVIDER_CLIENT_INSTANTIATION_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"EXTERNAL_REQUEST_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"LEDGER_OR_SCHEMA_WRITE_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"MINIKV_WRITE_OR_AUTHORITY_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"}]";
}

std::string format_checks_json() {
    return "{\"source_node_v303_loaded\":true,"
           "\"source_node_v303_ready\":true,"
           "\"source_node_v303_upstream_echo_aligned\":true,"
           "\"source_node_v303_keeps_runtime_blocked\":true,"
           "\"source_node_v303_keeps_side_effects_closed\":true,"
           "\"decision_selects_prerequisite_gate\":true,"
           "\"decision_record_blocks_runtime_shell\":true,"
           "\"decision_record_still_read_only\":true,"
           "\"required_prerequisites_documented\":true,"
           "\"missing_runtime_prerequisites_block_implementation\":true,"
           "\"necessity_proof_complete\":true,"
           "\"parallel_java_v141_mini_kv_v134_echo_recommended\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"mini_kv_non_participation_recorded\":true}";
}

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_only\":true,"
           "\"runtime_shell_chain_stop_or_prerequisite_decision_record_only\":true,"
           "\"stop_or_prerequisite_decision_record_only\":true,"
           "\"decision_record_state\":\"runtime-shell-chain-stop-or-prerequisite-decision-record-ready\","
           "\"runtime_shell_chain_decision\":\"require-explicit-approval-prerequisites-before-runtime-shell\","
           "\"selected_path\":\"continue-only-as-blocked-prerequisite-review\","
           "\"stop_runtime_shell_chain_without_prerequisites\":true,"
           "\"consumes_node_v304_runtime_shell_chain_stop_or_prerequisite_decision_record\":true,"
           "\"consumes_node_v303_post_decision_plan_intake_upstream_echo_verification\":true,"
           "\"ready_for_node_v305_stop_prerequisite_upstream_echo_verification\":true,"
           "\"ready_for_node_v305_runtime_shell_implementation\":false,"
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

std::string credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_decision_record}},
        {std::string{source_upstream_echo}},
        {std::string{node_v304_profile_version}},
        {std::string{node_v304_state}},
        {std::string{node_v304_decision}},
        {std::string{node_v304_decision_digest}},
        {std::string{node_v304_record_mode}},
        {std::string{node_v304_selected_path}},
        {std::string{node_v303_profile_version}},
        {std::string{node_v303_state}},
        {std::string{node_v303_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(prerequisite_count)},
        {std::to_string(missing_runtime_prerequisite_count)},
        {std::to_string(no_go_condition_count)},
        {"runtime-shell-chain-stop-or-prerequisite-non-participation-receipt-only"},
        {"require-explicit-approval-prerequisites-before-runtime-shell"},
        {"continue-only-as-blocked-prerequisite-review"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-runtime-shell-chain-stop-or-prerequisite", parts);
}

std::string format_credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.v1\","
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
           ",\"source_node_v304_reference\":{\"source_version\":\"Node v304\","
           "\"profile_version\":" + field_string(node_v304_profile_version) +
           ",\"decision_record_state\":" + field_string(node_v304_state) +
           ",\"runtime_shell_chain_decision\":" + field_string(node_v304_decision) +
           ",\"decision_digest\":" + field_string(node_v304_decision_digest) +
           ",\"record_mode\":" + field_string(node_v304_record_mode) +
           ",\"decision_scope\":" + field_string(node_v304_decision_scope) +
           ",\"source_span\":" + field_string(node_v304_source_span) +
           ",\"decision\":" + field_string(node_v304_decision) +
           ",\"decision_reason\":" + field_string(node_v304_decision_reason) +
           ",\"selected_path\":" + field_string(node_v304_selected_path) +
           ",\"stop_runtime_shell_chain_without_prerequisites\":true"
           ",\"allows_parallel_java_v141_mini_kv_v134_echo_request\":true"
           ",\"ready_for_node_v305_stop_prerequisite_upstream_echo_verification\":false"
           ",\"allows_node_v305_before_upstream_echo\":false"
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
           ",\"prerequisite_count\":" + std::to_string(prerequisite_count) +
           ",\"missing_runtime_prerequisite_count\":" +
           std::to_string(missing_runtime_prerequisite_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"required_prerequisites\":" + format_required_prerequisites_json() +
           ",\"explicit_no_go_conditions\":" + format_no_go_conditions_json() +
           ",\"blocker_resolved\":" + field_string(node_v304_blocker) +
           ",\"consumer\":" + field_string(node_v304_consumer) +
           ",\"why_v303_cannot_be_reused\":" + field_string(node_v304_v303_reuse) +
           ",\"existing_report_reuse_decision\":\"Reuse v303 as source evidence, but create v304 as the minimal decision layer that records stop/prerequisite requirements.\""
           ",\"stop_condition\":" + field_string(node_v304_stop_condition) +
           ",\"proof_complete\":true"
           ",\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) +
           ",\"read_only_decision_record\":true"
           ",\"runtime_shell_chain_stop_or_prerequisite_decision_record_only\":true"
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
           ",\"source_node_v303_reference\":{\"source_version\":\"Node v303\","
           "\"profile_version\":" + field_string(node_v303_profile_version) +
           ",\"verification_state\":" + field_string(node_v303_state) +
           ",\"verification_digest\":" + field_string(node_v303_verification_digest) +
           ",\"source_span\":" + field_string(node_v303_source_span) +
           ",\"active_node_verification_version\":\"Node v303\""
           ",\"legacy_node_v302_consumer_marker_accepted\":true"
           ",\"source_node_v301_ready\":true"
           ",\"source_node_v302_quality_pass_ready\":true"
           ",\"java_v136_echo_ready\":true"
           ",\"mini_kv_v133_receipt_ready\":true"
           ",\"upstream_echo_aligned\":true"
           ",\"side_effect_boundaries_aligned\":true"
           ",\"implementation_still_blocked\":true"
           ",\"check_count\":" + std::to_string(source_check_count) +
           ",\"passed_check_count\":" + std::to_string(source_check_count) +
           ",\"production_blocker_count\":" + std::to_string(source_production_blocker_count) +
           ",\"warning_count\":" + std::to_string(source_warning_count) +
           ",\"recommendation_count\":" + std::to_string(source_recommendation_count) +
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
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"runtime-shell-chain-stop-or-prerequisite-non-participation-receipt-only\","
           "\"shell_name\":\"credential-resolver-disabled-runtime-shell\""
           ",\"non_participation_scope\":\"stop/prerequisite decision, runtime shell implementation, invocation, provider/client wiring, credential reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and authority\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":17}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v303_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v303_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v303_production_blocker_count\":" +
           std::to_string(source_production_blocker_count) +
           ",\"source_node_v303_warning_count\":" + std::to_string(source_warning_count) +
           ",\"source_node_v303_recommendation_count\":" +
           std::to_string(source_recommendation_count) +
           ",\"prerequisite_count\":" + std::to_string(prerequisite_count) +
           ",\"missing_runtime_prerequisite_count\":" +
           std::to_string(missing_runtime_prerequisite_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"proof_complete\":true"
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"PREREQUISITE_DECISION_DOES_NOT_AUTHORIZE_RUNTIME\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V141_AND_MINIKV_V134_IN_PARALLEL\",\"severity\":\"recommendation\"},"
           "{\"code\":\"KEEP_RUNTIME_SHELL_BLOCKED\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"runtime_shell_chain_stop_or_prerequisite_decision_record_json\":" +
           field_string(node_v304_json) +
           ",\"runtime_shell_chain_stop_or_prerequisite_decision_record_markdown\":" +
           field_string(node_v304_markdown) +
           ",\"source_node_v303_json\":" + field_string(node_v303_json) +
           ",\"source_node_v303_markdown\":" + field_string(node_v303_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v141\":\"Java v141 runtime shell chain stop/prerequisite decision echo\""
           ",\"recommended_parallel_mini_kv_v134\":\"mini-kv v134 runtime shell chain stop/prerequisite non-participation receipt\""
           ",\"next_node_verification\":\"Node v305\"}"
           ",\"next_required_evidence_versions\":[\"Java v141\",\"mini-kv v134\"]"
           ",\"next_actions\":[\"Archive mini-kv v134 as a runtime shell chain stop/prerequisite non-participation receipt.\","
           "\"Keep Node v305 blocked until Java v141 and mini-kv v134 are both available.\","
           "\"Do not implement, enable, or invoke a disabled runtime shell from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
