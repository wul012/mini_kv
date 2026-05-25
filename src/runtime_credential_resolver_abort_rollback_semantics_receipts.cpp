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
    "mini-kv-credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer =
    "Node v327 abort/rollback semantics upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v142";
constexpr std::string_view receipt_artifact_path_hint = "d/142/";
constexpr std::string_view source_contract_intake =
    "Node v326 abort/rollback semantics contract intake";
constexpr std::string_view source_closure_review =
    "Node v325 no-network safety fixture prerequisite closure review";
constexpr std::string_view source_prerequisite_catalog =
    "Node v313 human approval post-echo prerequisite catalog cleanup";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v326_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-abort-rollback-semantics-contract-intake.v1";
constexpr std::string_view node_v326_contract_state =
    "abort-rollback-semantics-contract-intake-ready";
constexpr std::string_view node_v326_governance_chain_decision =
    "continue-only-for-abort-rollback-semantics-contract-intake";
constexpr std::string_view node_v326_target_prerequisite_id = "abort-rollback-semantics";
constexpr std::string_view node_v326_contract_digest =
    "fe05bcfd65aabf56ef170bf458837053a11edf0ae44ad203a88d4ecd284299f9";
constexpr std::string_view node_v326_contract_name = "managed-audit-abort-rollback-semantics";
constexpr std::string_view node_v326_contract_version = "abort-rollback-semantics.v1";
constexpr std::string_view node_v326_contract_mode = "abort-rollback-semantics-contract-intake-only";
constexpr std::string_view node_v326_source_span = "Node v325 closure review + Node v313 catalog";
constexpr std::string_view node_v326_purpose =
    "Define the final manual abort and rollback semantics prerequisite before any later resolver can discuss implementation candidate gates.";

constexpr std::string_view node_v325_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-no-network-safety-fixture-prerequisite-closure-review.v1";
constexpr std::string_view node_v325_review_state =
    "no-network-safety-fixture-prerequisite-closure-review-ready";
constexpr std::string_view node_v325_review_digest =
    "5781245b6dd5b67d6e2985e7e6f70e942defcd4ea95a09dc516743abf7abf0ca";
constexpr std::string_view node_v325_prerequisite_closure_decision =
    "advance-no-network-safety-fixture-only";

constexpr std::string_view active_plan =
    "docs/plans2/v325-post-no-network-safety-fixture-prerequisite-closure-roadmap.md";
constexpr std::string_view node_v326_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-abort-rollback-semantics-contract-intake";
constexpr std::string_view node_v326_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-abort-rollback-semantics-contract-intake?format=markdown";
constexpr std::string_view node_v325_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-no-network-safety-fixture-prerequisite-closure-review";
constexpr std::string_view node_v325_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-no-network-safety-fixture-prerequisite-closure-review?format=markdown";

constexpr int check_count = 21;
constexpr int source_node_v325_check_count = 17;
constexpr int source_node_v324_check_count = 23;
constexpr int original_prerequisite_count = 6;
constexpr int completed_prerequisite_count = 5;
constexpr int remaining_prerequisite_count = 1;
constexpr int required_field_count = 10;
constexpr int prohibited_field_count = 14;
constexpr int rejection_reason_count = 6;
constexpr int no_go_boundary_count = 11;
constexpr int upstream_echo_request_count = 2;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 2;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver abort/rollback semantics contract non-participation receipt only; mini-kv echoes Node v326's non-executing abort/rollback semantics contract, does not execute rollback, deployment, Java SQL, mini-kv write commands, runtime shell commands, LOAD/COMPACT/RESTORE/SETNXEX, or become abort/rollback authority, and does not accept credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP payloads, ledger mutations, schema SQL, shell script bodies, runtime shell commands, automatic upstream start, or rollback execution requests";
constexpr std::string_view node_action =
    "verify mini-kv v142 abort/rollback semantics contract non-participation receipt with Java v150 before Node v327 upstream echo verification";

std::string format_required_fields_json() {
    return "[{\"id\":\"manual_abort_marker\",\"accepted_shape\":\"stable non-secret manual abort marker\"},"
           "{\"id\":\"rollback_runbook_reference\",\"accepted_shape\":\"runbook id or immutable document reference\"},"
           "{\"id\":\"operator_confirmation_handle\",\"accepted_shape\":\"operator confirmation handle, no credential value\"},"
           "{\"id\":\"approval_correlation_id\",\"accepted_shape\":\"stable non-secret correlation id\"},"
           "{\"id\":\"cleanup_evidence_marker\",\"accepted_shape\":\"cleanup marker or evidence digest\"},"
           "{\"id\":\"idempotent_noop_failure_policy\",\"accepted_shape\":\"policy id or semantic version\"},"
           "{\"id\":\"rollback_authority_boundary\",\"accepted_shape\":\"authority handle, no executable permission\"},"
           "{\"id\":\"abort_reason_code\",\"accepted_shape\":\"stable reason code list\"},"
           "{\"id\":\"recovery_checkpoint_reference\",\"accepted_shape\":\"checkpoint handle or digest\"},"
           "{\"id\":\"audit_digest\",\"accepted_shape\":\"sha256 digest or equivalent stable digest\"}]";
}

std::string format_prohibited_fields_json() {
    return "[{\"id\":\"credential_value\",\"rejection_code\":\"CREDENTIAL_VALUE_PRESENT\"},"
           "{\"id\":\"raw_endpoint_url\",\"rejection_code\":\"RAW_ENDPOINT_URL_PRESENT\"},"
           "{\"id\":\"runtime_shell_command\",\"rejection_code\":\"RUNTIME_SHELL_COMMAND_PRESENT\"},"
           "{\"id\":\"shell_script_body\",\"rejection_code\":\"SHELL_SCRIPT_BODY_PRESENT\"},"
           "{\"id\":\"secret_provider_config\",\"rejection_code\":\"SECRET_PROVIDER_CONFIG_PRESENT\"},"
           "{\"id\":\"resolver_client_config\",\"rejection_code\":\"RESOLVER_CLIENT_CONFIG_PRESENT\"},"
           "{\"id\":\"external_request_payload\",\"rejection_code\":\"EXTERNAL_REQUEST_PAYLOAD_PRESENT\"},"
           "{\"id\":\"approval_ledger_mutation\",\"rejection_code\":\"APPROVAL_LEDGER_MUTATION_PRESENT\"},"
           "{\"id\":\"schema_migration_sql\",\"rejection_code\":\"SCHEMA_MIGRATION_SQL_PRESENT\"},"
           "{\"id\":\"deployment_action\",\"rejection_code\":\"DEPLOYMENT_ACTION_PRESENT\"},"
           "{\"id\":\"rollback_execution_action\",\"rejection_code\":\"ROLLBACK_EXECUTION_ACTION_PRESENT\"},"
           "{\"id\":\"upstream_process_start\",\"rejection_code\":\"UPSTREAM_PROCESS_START_PRESENT\"},"
           "{\"id\":\"mini_kv_write_command\",\"rejection_code\":\"MINI_KV_WRITE_COMMAND_PRESENT\"},"
           "{\"id\":\"java_sql_execution\",\"rejection_code\":\"JAVA_SQL_EXECUTION_PRESENT\"}]";
}

std::string format_rejection_reasons_json() {
    return "[\"MANUAL_ABORT_MARKER_MISSING\",\"ROLLBACK_RUNBOOK_REFERENCE_MISSING\","
           "\"CREDENTIAL_OR_RAW_ENDPOINT_PRESENT\",\"RUNTIME_SHELL_COMMAND_PRESENT\","
           "\"NETWORK_OR_PROVIDER_ACTION_PRESENT\",\"WRITE_OR_ROLLBACK_ACTION_PRESENT\"]";
}

std::string format_no_go_boundaries_json() {
    return "[{\"id\":\"credential_value_read\",\"allowed\":false},"
           "{\"id\":\"raw_endpoint_url_parse\",\"allowed\":false},"
           "{\"id\":\"runtime_shell_command_render\",\"allowed\":false},"
           "{\"id\":\"secret_provider_instantiation\",\"allowed\":false},"
           "{\"id\":\"resolver_client_instantiation\",\"allowed\":false},"
           "{\"id\":\"http_or_tcp_execution\",\"allowed\":false},"
           "{\"id\":\"rollback_execution\",\"allowed\":false},"
           "{\"id\":\"java_sql_execution\",\"allowed\":false},"
           "{\"id\":\"mini_kv_write_command\",\"allowed\":false},"
           "{\"id\":\"ledger_or_schema_write\",\"allowed\":false},"
           "{\"id\":\"automatic_upstream_start\",\"allowed\":false}]";
}

std::string format_upstream_echo_requests_json() {
    return "[{\"project\":\"java\",\"version\":\"Java v150\",\"requested_echo\":\"Read-only echo of the Node v326 abort/rollback semantics contract, confirming Java will not execute SQL, deployment, rollback, ledger writes, or external network calls.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true},"
           "{\"project\":\"mini-kv\",\"version\":\"mini-kv v142\",\"requested_echo\":\"Non-participation receipt proving mini-kv does not execute LOAD/COMPACT/RESTORE/SETNXEX, write commands, or become abort/rollback authority.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true}]";
}

std::string format_completed_prerequisites_json() {
    return "[\"java-mini-kv-decision-echo\",\"signed-human-approval-artifact\","
           "\"credential-handle-approval\",\"endpoint-handle-allowlist-approval\","
           "\"no-network-safety-fixture\"]";
}

std::string format_remaining_prerequisites_json() {
    return "[\"abort-rollback-semantics\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v325_ready\":true,"
           "\"source_node_v325_points_to_abort_rollback_semantics\":true,"
           "\"source_node_v325_keeps_runtime_blocked\":true,"
           "\"source_node_v325_keeps_side_effects_closed\":true,"
           "\"abort_rollback_semantics_still_missing_in_source\":true,"
           "\"catalog_target_matches_abort_rollback_semantics\":true,"
           "\"contract_required_fields_documented\":true,"
           "\"contract_prohibited_fields_documented\":true,"
           "\"rejection_reasons_documented\":true,"
           "\"no_go_boundaries_closed\":true,"
           "\"prerequisite_transition_scoped_to_abort_rollback_semantics\":true,"
           "\"necessity_proof_documented\":true,"
           "\"java_mini_kv_echo_request_explicitly_parallel\":true,"
           "\"contract_stays_non_secret_and_non_executing\":true,"
           "\"abort_rollback_execution_still_blocked\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"runtime_shell_implementation_still_blocked\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v327_abort_rollback_semantics_upstream_echo_verification\":true}";
}

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"abort_rollback_semantics_contract_non_participation_receipt_only\":true,"
           "\"abort_rollback_semantics_contract_intake_only\":true,"
           "\"read_only_abort_rollback_semantics_contract\":true,"
           "\"consumes_node_v326_abort_rollback_semantics_contract_intake\":true,"
           "\"consumes_node_v325_no_network_safety_fixture_prerequisite_closure_review\":true,"
           "\"consumes_node_v313_prerequisite_catalog\":true,"
           "\"ready_for_node_v327_abort_rollback_semantics_upstream_echo_verification\":true,"
           "\"ready_for_node_v327_before_upstream_echo\":false,"
           "\"ready_for_final_prerequisite_closure_review\":false,"
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
           "\"runtime_shell_command_rendered\":false,"
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
           "\"raw_endpoint_url_stored\":false,"
           "\"raw_endpoint_url_parse_allowed\":false,"
           "\"raw_endpoint_url_render_allowed\":false,"
           "\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_rendered\":false,"
           "\"endpoint_handle_allowlist_approved\":false,"
           "\"network_safety_fixture_executed\":false,"
           "\"abort_rollback_semantics_executed\":false,"
           "\"abort_rollback_execution_allowed\":false,"
           "\"abort_rollback_authority\":false,"
           "\"rollback_execution_allowed\":false,"
           "\"rollback_executed\":false,"
           "\"rollback_authority_boundary_authoritative\":false,"
           "\"rollback_runbook_authoritative\":false,"
           "\"manual_abort_marker_authoritative\":false,"
           "\"cleanup_evidence_marker_authoritative\":false,"
           "\"recovery_checkpoint_authoritative\":false,"
           "\"deployment_action_allowed\":false,"
           "\"deployment_action_executed\":false,"
           "\"java_sql_execution_allowed\":false,"
           "\"java_sql_executed\":false,"
           "\"mini_kv_write_command_allowed\":false,"
           "\"network_fixture_execution_allowed\":false,"
           "\"network_safety_authority\":false,"
           "\"transport_denial_policy_authoritative\":false,"
           "\"external_request_payload_accepted\":false,"
           "\"external_request_allowed\":false,"
           "\"external_request_sent\":false,"
           "\"http_request_allowed\":false,"
           "\"http_request_sent\":false,"
           "\"tcp_connection_allowed\":false,"
           "\"tcp_connection_attempted\":false,"
           "\"network_socket_opened\":false,"
           "\"http_tcp_dial_allowed\":false,"
           "\"dns_lookup_allowed\":false,"
           "\"tls_handshake_allowed\":false,"
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

std::string credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_contract_intake}},
        {std::string{source_closure_review}},
        {std::string{source_prerequisite_catalog}},
        {std::string{node_v326_profile_version}},
        {std::string{node_v326_contract_state}},
        {std::string{node_v326_contract_digest}},
        {std::string{node_v326_target_prerequisite_id}},
        {std::string{node_v325_profile_version}},
        {std::string{node_v325_review_state}},
        {std::string{node_v325_review_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_node_v325_check_count)},
        {std::to_string(remaining_prerequisite_count)},
        {std::to_string(required_field_count)},
        {std::to_string(prohibited_field_count)},
        {std::to_string(rejection_reason_count)},
        {std::to_string(no_go_boundary_count)},
        {"abort-rollback-semantics-contract-non-participation-only"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-abort-rollback-semantics-contract", parts);
}

std::string format_credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":" + field_string(receipt_version) +
           ",\"receipt_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"source_contract_intake\":" + field_string(source_contract_intake) +
           ",\"source_closure_review\":" + field_string(source_closure_review) +
           ",\"source_prerequisite_catalog\":" + field_string(source_prerequisite_catalog) +
           ",\"consumer_hint\":" + field_string(receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_role\":" + field_string(runtime_role) +
           ",\"source_node_v326_reference\":{\"source_version\":\"Node v326\""
           ",\"profile_version\":" + field_string(node_v326_profile_version) +
           ",\"contract_state\":" + field_string(node_v326_contract_state) +
           ",\"governance_chain_decision\":" + field_string(node_v326_governance_chain_decision) +
           ",\"ready_for_abort_rollback_semantics_contract_intake\":true"
           ",\"abort_rollback_semantics_contract_intake_only\":true"
           ",\"read_only_abort_rollback_semantics_contract\":true"
           ",\"consumes_node_v325_no_network_safety_fixture_prerequisite_closure_review\":true"
           ",\"consumes_node_v313_prerequisite_catalog\":true"
           ",\"target_prerequisite_id\":" + field_string(node_v326_target_prerequisite_id) +
           ",\"next_java_version\":\"Java v150\""
           ",\"next_mini_kv_version\":\"mini-kv v142\""
           ",\"next_node_verification_version\":\"Node v327\""
           ",\"ready_for_parallel_java_v150_mini_kv_v142_echo\":true"
           ",\"ready_for_node_v327_before_upstream_echo\":false"
           ",\"ready_for_final_prerequisite_closure_review\":false"
           ",\"abort_rollback_semantics_contract\":{\"contract_digest\":" + field_string(node_v326_contract_digest) +
           ",\"contract_name\":" + field_string(node_v326_contract_name) +
           ",\"contract_version\":" + field_string(node_v326_contract_version) +
           ",\"contract_mode\":" + field_string(node_v326_contract_mode) +
           ",\"source_span\":" + field_string(node_v326_source_span) +
           ",\"target_prerequisite_id\":" + field_string(node_v326_target_prerequisite_id) +
           ",\"purpose\":" + field_string(node_v326_purpose) +
           ",\"required_fields\":" + format_required_fields_json() +
           ",\"prohibited_fields\":" + format_prohibited_fields_json() +
           ",\"rejection_reasons\":" + format_rejection_reasons_json() +
           ",\"no_go_boundaries\":" + format_no_go_boundaries_json() +
           ",\"upstream_echo_requests\":" + format_upstream_echo_requests_json() +
           ",\"required_field_count\":" + std::to_string(required_field_count) +
           ",\"prohibited_field_count\":" + std::to_string(prohibited_field_count) +
           ",\"rejection_reason_count\":" + std::to_string(rejection_reason_count) +
           ",\"no_go_boundary_count\":" + std::to_string(no_go_boundary_count) +
           ",\"upstream_echo_request_count\":" + std::to_string(upstream_echo_request_count) +
           ",\"implementation_still_blocked\":true"
           ",\"abort_rollback_execution_allowed\":false}"
           ",\"prerequisite_transition\":{\"prerequisite_id\":" + field_string(node_v326_target_prerequisite_id) +
           ",\"catalog_label\":\"Abort and rollback semantics\""
           ",\"before_v326\":\"still-missing\""
           ",\"after_v326\":\"contract-intake-defined\""
           ",\"closure_requires_upstream_echo\":true"
           ",\"completed_prerequisite_count_before_v326\":5"
           ",\"remaining_prerequisite_count_before_v326\":1"
           ",\"preserves_signed_human_approval_artifact_closure\":true"
           ",\"preserves_credential_handle_approval_closure\":true"
           ",\"preserves_endpoint_handle_allowlist_approval_closure\":true"
           ",\"preserves_no_network_safety_fixture_closure\":true"
           ",\"closes_abort_rollback_semantics\":false}"
           ",\"necessity_proof\":{\"proof_complete\":true"
           ",\"blocker_resolved\":\"v325 completed the no-network-safety-fixture prerequisite and named abort-rollback-semantics as the final concrete missing contract.\""
           ",\"consumer\":\"Java v150 + mini-kv v142, then Node v327\""
           ",\"why_v325_cannot_be_reused\":\"v325 is a closure review for no-network-safety-fixture only; it proves 5/6 prerequisites but does not define manual abort markers, rollback runbook references, cleanup evidence, authority boundaries, or idempotent no-op failure handling.\""
           ",\"existing_report_reuse_decision\":\"Reuse v325 as source state and v313 as the prerequisite catalog; create v326 only for the final abort/rollback semantics contract intake.\""
           ",\"stop_condition\":\"Stop if the contract requires credential values, raw endpoint URLs, runtime shell commands, provider/client configuration, HTTP/TCP, deployment, rollback execution, Java SQL, mini-kv writes, ledger/schema writes, or automatic upstream start.\"}"
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v325_check_count\":" + std::to_string(source_node_v325_check_count) +
           ",\"source_node_v325_passed_check_count\":" + std::to_string(source_node_v325_check_count) +
           ",\"source_completed_prerequisite_count\":" + std::to_string(completed_prerequisite_count) +
           ",\"source_remaining_prerequisite_count\":" + std::to_string(remaining_prerequisite_count) +
           ",\"required_field_count\":" + std::to_string(required_field_count) +
           ",\"prohibited_field_count\":" + std::to_string(prohibited_field_count) +
           ",\"rejection_reason_count\":" + std::to_string(rejection_reason_count) +
           ",\"no_go_boundary_count\":" + std::to_string(no_go_boundary_count) +
           ",\"upstream_echo_request_count\":" + std::to_string(upstream_echo_request_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"runtime_shell_command_rendered\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"network_safety_fixture_executed\":false"
           ",\"abort_rollback_semantics_executed\":false"
           ",\"rollback_execution_allowed\":false"
           ",\"deployment_action_allowed\":false"
           ",\"java_sql_execution_allowed\":false"
           ",\"mini_kv_write_command_allowed\":false"
           ",\"http_request_sent\":false"
           ",\"tcp_connection_attempted\":false"
           ",\"network_socket_opened\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"source_node_v325_reference\":{\"source_version\":\"Node v325\""
           ",\"profile_version\":" + field_string(node_v325_profile_version) +
           ",\"review_state\":" + field_string(node_v325_review_state) +
           ",\"ready_for_no_network_safety_fixture_prerequisite_closure_review\":true"
           ",\"review_digest\":" + field_string(node_v325_review_digest) +
           ",\"prerequisite_closure_decision\":" + field_string(node_v325_prerequisite_closure_decision) +
           ",\"completed_prerequisite_count\":" + std::to_string(completed_prerequisite_count) +
           ",\"remaining_prerequisite_count\":" + std::to_string(remaining_prerequisite_count) +
           ",\"original_prerequisite_count\":" + std::to_string(original_prerequisite_count) +
           ",\"next_concrete_prerequisite_id\":" + field_string(node_v326_target_prerequisite_id) +
           ",\"next_concrete_prerequisite_contract_required\":true"
           ",\"next_node_version_suggested\":\"Node v326\""
           ",\"chain_continuation_allowed\":true"
           ",\"runtime_shell_still_blocked\":true"
           ",\"completed_prerequisite_ids\":" + format_completed_prerequisites_json() +
           ",\"remaining_prerequisite_ids\":" + format_remaining_prerequisites_json() +
           ",\"source_check_count\":" + std::to_string(source_node_v325_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_node_v325_check_count) +
           ",\"source_node_v324_check_count\":" + std::to_string(source_node_v324_check_count) +
           ",\"source_node_v324_passed_check_count\":" + std::to_string(source_node_v324_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"source_warning_count\":1"
           ",\"source_recommendation_count\":2"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"network_safety_fixture_executed\":false"
           ",\"http_request_sent\":false"
           ",\"tcp_connection_attempted\":false"
           ",\"network_socket_opened\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"abort-rollback-semantics-contract-non-participation-receipt-only\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":" + std::to_string(check_count) + "}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v325_check_count\":" + std::to_string(source_node_v325_check_count) +
           ",\"source_node_v325_passed_check_count\":" + std::to_string(source_node_v325_check_count) +
           ",\"source_node_v324_check_count\":" + std::to_string(source_node_v324_check_count) +
           ",\"source_node_v324_passed_check_count\":" + std::to_string(source_node_v324_check_count) +
           ",\"source_completed_prerequisite_count\":" + std::to_string(completed_prerequisite_count) +
           ",\"source_remaining_prerequisite_count\":" + std::to_string(remaining_prerequisite_count) +
           ",\"required_field_count\":" + std::to_string(required_field_count) +
           ",\"prohibited_field_count\":" + std::to_string(prohibited_field_count) +
           ",\"rejection_reason_count\":" + std::to_string(rejection_reason_count) +
           ",\"no_go_boundary_count\":" + std::to_string(no_go_boundary_count) +
           ",\"upstream_echo_request_count\":" + std::to_string(upstream_echo_request_count) +
           ",\"proof_complete\":true"
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"ABORT_ROLLBACK_SEMANTICS_CONTRACT_DOES_NOT_EXECUTE_ROLLBACK\",\"severity\":\"warning\"},"
           "{\"code\":\"FINAL_PREREQUISITE_CONTRACT_DOES_NOT_APPROVE_RUNTIME\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V150_AND_MINI_KV_V142_AFTER_V326_ARCHIVE\",\"severity\":\"recommendation\"},"
           "{\"code\":\"KEEP_ABORT_ROLLBACK_SEMANTICS_CONTRACT_NON_EXECUTING\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"abort_rollback_semantics_contract_intake_json\":" + field_string(node_v326_json) +
           ",\"abort_rollback_semantics_contract_intake_markdown\":" + field_string(node_v326_markdown) +
           ",\"source_node_v325_json\":" + field_string(node_v325_json) +
           ",\"source_node_v325_markdown\":" + field_string(node_v325_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v150\":\"Java v150 abort/rollback semantics contract echo\""
           ",\"recommended_parallel_mini_kv_v142\":\"mini-kv v142 abort/rollback semantics contract non-participation receipt\""
           ",\"next_node_verification\":\"Node v327\"}"
           ",\"next_required_evidence_versions\":[\"Java v150\",\"mini-kv v142\"]"
           ",\"next_actions\":[\"Archive mini-kv v142 as an abort/rollback semantics contract non-participation receipt.\","
           "\"Keep Node v327 blocked until Java v150 and mini-kv v142 are both available.\","
           "\"Do not execute rollback, deployment, Java SQL, runtime shell commands, mini-kv write commands, or become abort/rollback authority.\","
           "\"Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, write ledgers or schema state, auto-start services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_digest(
               read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
