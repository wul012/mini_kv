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
    "Node v298 runtime shell candidate gate upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v131";
constexpr std::string_view receipt_artifact_path_hint = "d/131/";
constexpr std::string_view source_candidate_gate =
    "Node v297 credential resolver disabled runtime shell implementation candidate gate";
constexpr std::string_view source_upstream_echo =
    "Node v296 disabled runtime shell upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v297_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-implementation-candidate-gate.v1";
constexpr std::string_view node_v297_state =
    "disabled-runtime-shell-implementation-candidate-gate-reviewed";
constexpr std::string_view node_v297_gate_version =
    "node-v297-disabled-runtime-shell-implementation-candidate-gate.v1";
constexpr std::string_view node_v297_gate_mode = "candidate-gate-only-default-blocked";
constexpr std::string_view node_v297_gate_decision =
    "blocked-request-parallel-java-v134-and-mini-kv-v131-before-implementation";
constexpr std::string_view node_v297_gate_digest =
    "651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22";
constexpr std::string_view node_v297_decision_rationale =
    "Node v296 proves the three-project upstream echo is aligned, but a runtime shell still needs a dedicated disabled-by-default flag, operator approval, abort semantics, no-network tests, and fallback evidence echoed by Java and mini-kv before any later implementation decision.";
constexpr std::string_view node_v297_blocker =
    "candidate-gate-lacks-upstream-echo-and-runtime-prerequisite-proof";
constexpr std::string_view node_v297_cannot_reuse_reason =
    "Node v296 verified upstream echo evidence only; it did not enumerate the runtime-shell candidate prerequisites or publish a blocked decision that Java v134 and mini-kv v131 can echo.";
constexpr std::string_view node_v297_stop_condition =
    "Stop extending the candidate gate once Java v134 and mini-kv v131 echo the blocked decision and Node v298 verifies both echoes.";

constexpr std::string_view node_v296_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-upstream-echo-verification.v1";
constexpr std::string_view node_v296_state =
    "disabled-runtime-shell-upstream-echo-verification-ready";
constexpr std::string_view node_v296_verification_digest =
    "9bbba02d888b5c986026f3e80e0c07df27d08699405d9989cc42e9bd1c8c6f7b";

constexpr std::string_view active_plan =
    "docs/plans2/v297-post-disabled-runtime-shell-implementation-candidate-gate-roadmap.md";
constexpr std::string_view node_v297_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-implementation-candidate-gate";
constexpr std::string_view node_v297_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-implementation-candidate-gate?format=markdown";
constexpr std::string_view node_v296_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-upstream-echo-verification";
constexpr std::string_view node_v296_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-upstream-echo-verification?format=markdown";

constexpr int check_count = 27;
constexpr int source_check_count = 24;
constexpr int required_gate_count = 5;
constexpr int documented_gate_count = 5;
constexpr int review_evidence_satisfied_count = 5;
constexpr int runtime_prerequisite_satisfied_count = 0;
constexpr int implementation_allowed_gate_count = 0;
constexpr int stop_condition_count = 6;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 2;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver disabled runtime shell candidate gate non-participation receipt only; mini-kv echoes Node v297 blocked implementation candidate gate, does not implement, enable, or invoke a runtime shell, does not instantiate providers or resolver clients, does not read credential values, does not parse raw endpoint URLs, does not send HTTP/TCP or external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view node_action =
    "verify mini-kv v131 runtime shell candidate gate non-participation receipt with Java v134 before Node v298 upstream echo verification";

std::string format_required_gates_json() {
    return "[{\"code\":\"DEDICATED_DISABLED_BY_DEFAULT_FLAG\",\"title\":\"Dedicated disabled-by-default flag\","
           "\"owner\":\"node\",\"failure_class\":\"missing-disabled-runtime-shell-flag\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"OPERATOR_APPROVAL\",\"title\":\"Operator approval marker\","
           "\"owner\":\"operator\",\"failure_class\":\"operator-approval-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"ABORT_SEMANTICS\",\"title\":\"Abort semantics\","
           "\"owner\":\"release\",\"failure_class\":\"abort-semantics-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"NO_NETWORK_TESTS\",\"title\":\"No-network tests\","
           "\"owner\":\"test\",\"failure_class\":\"no-network-tests-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"HISTORICAL_FALLBACK_EVIDENCE\",\"title\":\"Historical fallback evidence\","
           "\"owner\":\"release\",\"failure_class\":\"historical-fallback-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false}]";
}

std::string format_stop_conditions_json() {
    return "[\"Any candidate asks Node to implement, enable, or invoke a runtime shell.\","
           "\"Any candidate asks Node to instantiate a provider or resolver client.\","
           "\"Any candidate asks Node, Java, or mini-kv to read credential values or parse raw endpoint URLs.\","
           "\"Any candidate asks Node to send HTTP/TCP to managed audit.\","
           "\"Any candidate asks Java to write approval ledger records, execute SQL, deploy, or roll back.\","
           "\"Any candidate asks mini-kv to execute LOAD/COMPACT/RESTORE/SETNXEX or become audit/order authority.\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v296_ready\":true,"
           "\"source_node_v296_keeps_implementation_blocked\":true,"
           "\"source_node_v296_keeps_side_effects_closed\":true,"
           "\"candidate_gate_count_stable\":true,"
           "\"all_candidate_gates_documented\":true,"
           "\"all_candidate_gates_review_evidence_satisfied\":true,"
           "\"candidate_gate_keeps_runtime_blocked\":true,"
           "\"dedicated_disabled_by_default_flag_required\":true,"
           "\"operator_approval_required\":true,"
           "\"abort_semantics_required\":true,"
           "\"no_network_tests_required\":true,"
           "\"historical_fallback_evidence_required\":true,"
           "\"necessity_documented\":true,"
           "\"parallel_upstream_echo_recommended\":true,"
           "\"mini_kv_non_participation_recorded\":true,"
           "\"no_runtime_implementation_created\":true,"
           "\"no_runtime_invocation_allowed\":true,"
           "\"credential_boundary_closed\":true,"
           "\"raw_endpoint_boundary_closed\":true,"
           "\"provider_client_boundary_closed\":true,"
           "\"connection_boundary_closed\":true,"
           "\"write_boundary_closed\":true,"
           "\"auto_start_boundary_closed\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v298_upstream_echo\":true}";
}

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"candidate_gate_non_participation_receipt_only\":true,"
           "\"runtime_shell_candidate_gate_non_participation_receipt_only\":true,"
           "\"implementation_candidate_gate_only\":true,"
           "\"consumes_node_v297_disabled_runtime_shell_implementation_candidate_gate\":true,"
           "\"consumes_node_v296_disabled_runtime_shell_upstream_echo_verification\":true,"
           "\"ready_for_node_v298_runtime_shell_candidate_gate_upstream_echo_verification\":true,"
           "\"ready_for_node_v298_runtime_shell_implementation\":false,"
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
           "\"runtime_prerequisite_satisfied\":false,"
           "\"implementation_allowed\":false,"
           "\"implementation_candidate_allowed\":false,"
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

std::string credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_candidate_gate}},
        {std::string{source_upstream_echo}},
        {std::string{node_v297_profile_version}},
        {std::string{node_v297_state}},
        {std::string{node_v297_gate_version}},
        {std::string{node_v297_gate_mode}},
        {std::string{node_v297_gate_decision}},
        {std::string{node_v297_gate_digest}},
        {std::string{node_v296_profile_version}},
        {std::string{node_v296_state}},
        {std::string{node_v296_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(required_gate_count)},
        {std::to_string(review_evidence_satisfied_count)},
        {std::to_string(runtime_prerequisite_satisfied_count)},
        {std::to_string(implementation_allowed_gate_count)},
        {"runtime-shell-candidate-gate-non-participation-receipt-only"},
        {"blocked"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-disabled-runtime-shell-candidate-gate", parts);
}

std::string format_credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"source_candidate_gate\":" + field_string(source_candidate_gate) +
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
           ",\"source_node_v297_reference\":{\"source_version\":\"Node v297\","
           "\"profile_version\":" + field_string(node_v297_profile_version) +
           ",\"candidate_gate_state\":" + field_string(node_v297_state) +
           ",\"gate_version\":" + field_string(node_v297_gate_version) +
           ",\"gate_mode\":" + field_string(node_v297_gate_mode) +
           ",\"gate_decision\":" + field_string(node_v297_gate_decision) +
           ",\"gate_digest\":" + field_string(node_v297_gate_digest) +
           ",\"decision_rationale\":" + field_string(node_v297_decision_rationale) +
           ",\"necessity\":{\"blocker\":" + field_string(node_v297_blocker) +
           ",\"consumer\":\"Java v134 and mini-kv v131, then Node v298\""
           ",\"cannot_reuse_existing_report_reason\":" + field_string(node_v297_cannot_reuse_reason) +
           ",\"stop_condition\":" + field_string(node_v297_stop_condition) + "}"
           ",\"required_gates\":" + format_required_gates_json() +
           ",\"stop_conditions\":" + format_stop_conditions_json() +
           ",\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_check_count\":" + std::to_string(source_check_count) +
           ",\"required_gate_count\":" + std::to_string(required_gate_count) +
           ",\"documented_gate_count\":" + std::to_string(documented_gate_count) +
           ",\"review_evidence_satisfied_count\":" +
           std::to_string(review_evidence_satisfied_count) +
           ",\"runtime_prerequisite_satisfied_count\":" +
           std::to_string(runtime_prerequisite_satisfied_count) +
           ",\"implementation_allowed_gate_count\":" +
           std::to_string(implementation_allowed_gate_count) +
           ",\"stop_condition_count\":" + std::to_string(stop_condition_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) +
           ",\"read_only_candidate_gate\":true"
           ",\"implementation_candidate_gate_only\":true"
           ",\"ready_for_parallel_java_v134_mini_kv_v131_echo_request\":true"
           ",\"ready_for_node_v298_runtime_shell_candidate_gate_upstream_echo_verification\":false"
           ",\"ready_for_node_v298_runtime_shell_implementation\":false"
           ",\"ready_for_disabled_runtime_shell_implementation\":false"
           ",\"ready_for_disabled_runtime_shell_invocation\":false"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_enabled\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"source_node_v296_reference\":{\"source_version\":\"Node v296\","
           "\"profile_version\":" + field_string(node_v296_profile_version) +
           ",\"verification_state\":" + field_string(node_v296_state) +
           ",\"verification_digest\":" + field_string(node_v296_verification_digest) +
           ",\"source_span\":\"Node v295 + Java v133 + mini-kv v130\""
           ",\"ready_for_upstream_echo_verification\":true"
           ",\"read_only_upstream_echo_verification\":true"
           ",\"disabled_runtime_shell_upstream_echo_verification_only\":true"
           ",\"plan_version_correction_applied\":true"
           ",\"planned_java_version\":\"Java v132\""
           ",\"actual_java_echo_version\":\"Java v133\""
           ",\"consumes_java_v133\":true"
           ",\"consumes_mini_kv_v130\":true"
           ",\"ready_for_node_v297_candidate_gate\":true"
           ",\"ready_for_node_v297_runtime_shell_implementation\":false"
           ",\"check_count\":" + std::to_string(source_check_count) +
           ",\"passed_check_count\":" + std::to_string(source_check_count) +
           ",\"production_blocker_count\":0"
           ",\"warning_count\":2"
           ",\"recommendation_count\":2"
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
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"runtime-shell-candidate-gate-non-participation-receipt-only\","
           "\"shell_name\":\"credential-resolver-disabled-runtime-shell\""
           ",\"non_participation_scope\":\"runtime shell candidate gate, implementation, invocation, provider/client wiring, credential reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and authority\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":20}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v297_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v297_passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v296_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v296_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"required_gate_count\":" + std::to_string(required_gate_count) +
           ",\"documented_gate_count\":" + std::to_string(documented_gate_count) +
           ",\"review_evidence_satisfied_count\":" +
           std::to_string(review_evidence_satisfied_count) +
           ",\"runtime_prerequisite_satisfied_count\":" +
           std::to_string(runtime_prerequisite_satisfied_count) +
           ",\"implementation_allowed_gate_count\":" +
           std::to_string(implementation_allowed_gate_count) +
           ",\"stop_condition_count\":" + std::to_string(stop_condition_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"CANDIDATE_GATE_ONLY_DEFAULT_BLOCKED\",\"severity\":\"warning\"},"
           "{\"code\":\"UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V298\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_PARALLEL_JAVA_V134_MINI_KV_V131\",\"severity\":\"recommendation\"},"
           "{\"code\":\"VERIFY_WITH_NODE_V298_AFTER_UPSTREAM_ECHO\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"disabled_runtime_shell_implementation_candidate_gate_json\":" +
           field_string(node_v297_json) +
           ",\"disabled_runtime_shell_implementation_candidate_gate_markdown\":" +
           field_string(node_v297_markdown) +
           ",\"source_node_v296_json\":" + field_string(node_v296_json) +
           ",\"source_node_v296_markdown\":" + field_string(node_v296_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v134\":\"Java v134 runtime shell candidate gate echo; read-only blocked decision only\""
           ",\"recommended_parallel_mini_kv_v131\":\"mini-kv v131 runtime shell candidate gate non-participation receipt\""
           ",\"next_node_verification\":\"Node v298\"}"
           ",\"next_required_evidence_versions\":[\"Java v134\",\"mini-kv v131\"]"
           ",\"next_actions\":[\"Archive mini-kv v131 as a runtime shell candidate gate non-participation receipt.\","
           "\"Keep Node v298 blocked until Java v134 and mini-kv v131 are both available.\","
           "\"Do not implement, enable, or invoke a disabled runtime shell from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
