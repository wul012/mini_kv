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
    "mini-kv-credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer =
    "Node v315 signed human approval artifact contract upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v138";
constexpr std::string_view receipt_artifact_path_hint = "d/138/";
constexpr std::string_view source_contract_intake =
    "Node v314 signed human approval artifact contract intake";
constexpr std::string_view source_governance_decision =
    "Node v312 human approval artifact review governance stop/prerequisite closure decision";
constexpr std::string_view source_prerequisite_catalog =
    "Node v313 human approval post-echo prerequisite catalog cleanup";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v314_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-signed-human-approval-artifact-contract-intake.v1";
constexpr std::string_view node_v314_contract_state =
    "signed-human-approval-artifact-contract-intake-ready";
constexpr std::string_view node_v314_governance_chain_decision =
    "continue-only-for-signed-human-approval-artifact-contract-intake";
constexpr std::string_view node_v314_target_prerequisite_id = "signed-human-approval-artifact";
constexpr std::string_view node_v314_contract_digest =
    "72498e59c086eadd4d44e80789120de195af1a0b70dd49346b837e2bc8ed4666";
constexpr std::string_view node_v314_artifact_name = "managed-audit-signed-human-approval-artifact";
constexpr std::string_view node_v314_artifact_version = "signed-human-approval-artifact.v1";
constexpr std::string_view node_v314_contract_mode = "signed-human-approval-artifact-contract-intake-only";
constexpr std::string_view node_v314_source_span = "Node v312 + Node v313 catalog";
constexpr std::string_view node_v314_purpose =
    "Define the non-secret signed human approval artifact shape required before any later managed audit credential resolver runtime shell discussion.";

constexpr std::string_view node_v312_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-governance-stop-prerequisite-closure-decision.v1";
constexpr std::string_view node_v312_decision_state =
    "human-approval-artifact-review-governance-stop-prerequisite-closure-decision-ready";
constexpr std::string_view node_v312_decision_digest =
    "152d7517c07119df360446a29c508f5d3d9a78a28adfc6137ea0b0254508b0c6";
constexpr std::string_view node_v312_source_verification_digest =
    "8292327cdb44e1d37ead67ff5a0444c08625860c62a3648846801a84f5a6f194";
constexpr std::string_view active_plan =
    "docs/plans2/v313-post-prerequisite-catalog-cleanup-roadmap.md";
constexpr std::string_view node_v314_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-signed-human-approval-artifact-contract-intake";
constexpr std::string_view node_v314_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-signed-human-approval-artifact-contract-intake?format=markdown";
constexpr std::string_view node_v312_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-governance-stop-prerequisite-closure-decision";
constexpr std::string_view node_v312_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-governance-stop-prerequisite-closure-decision?format=markdown";

constexpr int check_count = 18;
constexpr int source_node_v312_check_count = 16;
constexpr int source_node_v311_check_count = 23;
constexpr int original_prerequisite_count = 6;
constexpr int completed_prerequisite_count = 1;
constexpr int remaining_prerequisite_count = 5;
constexpr int source_no_go_condition_count = 9;
constexpr int required_field_count = 11;
constexpr int prohibited_field_count = 8;
constexpr int rejection_reason_count = 5;
constexpr int no_go_boundary_count = 8;
constexpr int upstream_echo_request_count = 2;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver signed human approval artifact contract non-participation receipt only; mini-kv echoes Node v314's non-secret contract, does not store, validate, sign, verify, or become authority for signed approval artifacts, and does not accept credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP payloads, ledger mutations, schema SQL, mini-kv write commands, runtime shell invocation requests, automatic upstream start, or LOAD/COMPACT/RESTORE/SETNXEX execution";
constexpr std::string_view node_action =
    "verify mini-kv v138 signed human approval artifact contract non-participation receipt with Java v145 before Node v315 upstream echo verification";

std::string format_required_fields_json() {
    return "[{\"id\":\"artifact_id\",\"accepted_shape\":\"stable non-secret artifact id\"},"
           "{\"id\":\"approval_correlation_id\",\"accepted_shape\":\"stable non-secret correlation id\"},"
           "{\"id\":\"operator_identity_handle\",\"accepted_shape\":\"operator identity handle, no credential value\"},"
           "{\"id\":\"signer_identity_handle\",\"accepted_shape\":\"signer identity handle, no private key\"},"
           "{\"id\":\"policy_version\",\"accepted_shape\":\"policy id or semantic version\"},"
           "{\"id\":\"artifact_digest\",\"accepted_shape\":\"sha256 digest or equivalent stable digest\"},"
           "{\"id\":\"issued_at\",\"accepted_shape\":\"ISO-8601 timestamp\"},"
           "{\"id\":\"expires_at\",\"accepted_shape\":\"ISO-8601 timestamp\"},"
           "{\"id\":\"review_status\",\"accepted_shape\":\"approved, rejected, expired, or revoked\"},"
           "{\"id\":\"no_network_assertion\",\"accepted_shape\":\"boolean assertion plus evidence handle\"},"
           "{\"id\":\"rollback_abort_reference\",\"accepted_shape\":\"runbook or evidence handle\"}]";
}

std::string format_prohibited_fields_json() {
    return "[{\"id\":\"credential_value\",\"rejection_code\":\"CREDENTIAL_VALUE_PRESENT\"},"
           "{\"id\":\"raw_endpoint_url\",\"rejection_code\":\"RAW_ENDPOINT_URL_PRESENT\"},"
           "{\"id\":\"signing_private_key\",\"rejection_code\":\"SIGNING_PRIVATE_KEY_PRESENT\"},"
           "{\"id\":\"secret_provider_config\",\"rejection_code\":\"PROVIDER_CONFIG_PRESENT\"},"
           "{\"id\":\"resolver_client_config\",\"rejection_code\":\"RESOLVER_CLIENT_CONFIG_PRESENT\"},"
           "{\"id\":\"external_request_payload\",\"rejection_code\":\"EXTERNAL_REQUEST_PAYLOAD_PRESENT\"},"
           "{\"id\":\"approval_ledger_mutation\",\"rejection_code\":\"APPROVAL_LEDGER_MUTATION_PRESENT\"},"
           "{\"id\":\"schema_migration_sql\",\"rejection_code\":\"SCHEMA_MIGRATION_SQL_PRESENT\"}]";
}

std::string format_rejection_reasons_json() {
    return "[\"SIGNED_ARTIFACT_MISSING\",\"CREDENTIAL_VALUE_PRESENT\",\"RAW_ENDPOINT_URL_PRESENT\","
           "\"RUNTIME_IMPLEMENTATION_PRESENT\",\"WRITE_OR_MIGRATION_PRESENT\"]";
}

std::string format_no_go_boundaries_json() {
    return "[{\"id\":\"runtime_shell_implementation\",\"allowed\":false},"
           "{\"id\":\"runtime_shell_invocation\",\"allowed\":false},"
           "{\"id\":\"credential_value_read\",\"allowed\":false},"
           "{\"id\":\"raw_endpoint_url_parse\",\"allowed\":false},"
           "{\"id\":\"provider_client_instantiation\",\"allowed\":false},"
           "{\"id\":\"external_request\",\"allowed\":false},"
           "{\"id\":\"ledger_or_schema_write\",\"allowed\":false},"
           "{\"id\":\"automatic_upstream_start\",\"allowed\":false}]";
}

std::string format_upstream_echo_requests_json() {
    return "[{\"project\":\"java\",\"version\":\"Java v145\",\"requested_echo\":\"Read-only echo of the Node v314 signed human approval artifact contract.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true},"
           "{\"project\":\"mini-kv\",\"version\":\"mini-kv v138\",\"requested_echo\":\"Non-participation receipt proving mini-kv does not store, validate, or become authority for signed approval artifacts.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true}]";
}

std::string format_remaining_prerequisites_json() {
    return "[\"signed-human-approval-artifact\",\"credential-handle-approval\","
           "\"endpoint-handle-allowlist-approval\",\"no-network-safety-fixture\","
           "\"abort-rollback-semantics\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v312_ready\":true,"
           "\"source_node_v312_keeps_governance_paused\":true,"
           "\"signed_human_approval_artifact_still_missing_in_source\":true,"
           "\"catalog_target_matches_signed_artifact\":true,"
           "\"contract_required_fields_documented\":true,"
           "\"contract_prohibited_fields_documented\":true,"
           "\"rejection_reasons_documented\":true,"
           "\"no_go_boundaries_closed\":true,"
           "\"prerequisite_transition_scoped_to_signed_artifact\":true,"
           "\"necessity_proof_documented\":true,"
           "\"java_mini_kv_echo_request_explicitly_parallel\":true,"
           "\"contract_stays_non_secret\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"runtime_shell_implementation_still_blocked\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v315_signed_human_approval_artifact_contract_upstream_echo_verification\":true}";
}

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"signed_human_approval_artifact_contract_non_participation_receipt_only\":true,"
           "\"signed_human_approval_artifact_contract_intake_only\":true,"
           "\"read_only_artifact_contract\":true,"
           "\"consumes_node_v314_signed_human_approval_artifact_contract_intake\":true,"
           "\"consumes_node_v312_governance_stop_prerequisite_closure_decision\":true,"
           "\"consumes_node_v313_prerequisite_catalog\":true,"
           "\"ready_for_node_v315_signed_human_approval_artifact_contract_upstream_echo_verification\":true,"
           "\"ready_for_node_v315_before_upstream_echo\":false,"
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
           "\"signing_private_key_accepted\":false,"
           "\"signed_human_approval_artifact_stored\":false,"
           "\"signed_human_approval_artifact_validated\":false,"
           "\"signed_human_approval_artifact_authoritative\":false,"
           "\"signed_artifact_signature_verified\":false,"
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

std::string credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_contract_intake}},
        {std::string{source_governance_decision}},
        {std::string{source_prerequisite_catalog}},
        {std::string{node_v314_profile_version}},
        {std::string{node_v314_contract_state}},
        {std::string{node_v314_contract_digest}},
        {std::string{node_v314_target_prerequisite_id}},
        {std::string{node_v312_profile_version}},
        {std::string{node_v312_decision_state}},
        {std::string{node_v312_decision_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_node_v312_check_count)},
        {std::to_string(remaining_prerequisite_count)},
        {std::to_string(required_field_count)},
        {std::to_string(prohibited_field_count)},
        {std::to_string(rejection_reason_count)},
        {std::to_string(no_go_boundary_count)},
        {"signed-human-approval-artifact-contract-non-participation-only"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-signed-human-approval-artifact-contract", parts);
}

std::string format_credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":" + field_string(receipt_version) +
           ",\"receipt_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"source_contract_intake\":" + field_string(source_contract_intake) +
           ",\"source_governance_decision\":" + field_string(source_governance_decision) +
           ",\"source_prerequisite_catalog\":" + field_string(source_prerequisite_catalog) +
           ",\"consumer_hint\":" + field_string(receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_role\":" + field_string(runtime_role) +
           ",\"source_node_v314_reference\":{\"source_version\":\"Node v314\""
           ",\"profile_version\":" + field_string(node_v314_profile_version) +
           ",\"contract_state\":" + field_string(node_v314_contract_state) +
           ",\"governance_chain_decision\":" + field_string(node_v314_governance_chain_decision) +
           ",\"ready_for_signed_human_approval_artifact_contract_intake\":true"
           ",\"signed_human_approval_artifact_contract_intake_only\":true"
           ",\"read_only_artifact_contract\":true"
           ",\"consumes_node_v312_governance_stop_prerequisite_closure_decision\":true"
           ",\"consumes_node_v313_prerequisite_catalog\":true"
           ",\"target_prerequisite_id\":" + field_string(node_v314_target_prerequisite_id) +
           ",\"next_java_version\":\"Java v145\""
           ",\"next_mini_kv_version\":\"mini-kv v138\""
           ",\"next_node_verification_version\":\"Node v315\""
           ",\"ready_for_parallel_java_v145_mini_kv_v138_echo\":true"
           ",\"ready_for_node_v315_before_upstream_echo\":false"
           ",\"signed_artifact_contract\":{\"contract_digest\":" + field_string(node_v314_contract_digest) +
           ",\"artifact_name\":" + field_string(node_v314_artifact_name) +
           ",\"artifact_version\":" + field_string(node_v314_artifact_version) +
           ",\"contract_mode\":" + field_string(node_v314_contract_mode) +
           ",\"source_span\":" + field_string(node_v314_source_span) +
           ",\"target_prerequisite_id\":" + field_string(node_v314_target_prerequisite_id) +
           ",\"purpose\":" + field_string(node_v314_purpose) +
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
           ",\"implementation_still_blocked\":true}"
           ",\"prerequisite_transition\":{\"prerequisite_id\":" + field_string(node_v314_target_prerequisite_id) +
           ",\"catalog_label\":\"Signed human approval artifact\""
           ",\"before_v314\":\"still-missing\""
           ",\"after_v314\":\"contract-intake-defined\""
           ",\"closure_requires_upstream_echo\":true"
           ",\"closes_credential_handle_approval\":false"
           ",\"closes_endpoint_handle_allowlist_approval\":false"
           ",\"closes_no_network_safety_fixture\":false"
           ",\"closes_abort_rollback_semantics\":false}"
           ",\"necessity_proof\":{\"proof_complete\":true"
           ",\"blocker_resolved\":\"v312 paused the governance chain with five remaining prerequisites; v314 names the first concrete missing prerequisite and defines its non-secret contract shape.\""
           ",\"consumer\":\"Java v145 + mini-kv v138, then Node v315\""
           ",\"why_v312_cannot_be_reused\":\"v312 is a closure decision only; it lists signed-human-approval-artifact as missing but does not define the signed artifact contract fields that upstreams can echo.\""
           ",\"existing_report_reuse_decision\":\"Reuse v312 as source state and v313 as the prerequisite catalog; create v314 only for the signed-human-approval-artifact contract intake.\""
           ",\"stop_condition\":\"Stop if the contract requires credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP, runtime shell implementation or invocation, ledger/schema writes, mini-kv authority, or automatic upstream start.\"}"
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v312_check_count\":" + std::to_string(source_node_v312_check_count) +
           ",\"source_node_v312_passed_check_count\":" + std::to_string(source_node_v312_check_count) +
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
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"source_node_v312_reference\":{\"source_version\":\"Node v312\""
           ",\"profile_version\":" + field_string(node_v312_profile_version) +
           ",\"decision_state\":" + field_string(node_v312_decision_state) +
           ",\"ready_for_closure_decision\":true"
           ",\"decision_digest\":" + field_string(node_v312_decision_digest) +
           ",\"source_verification_digest\":" + field_string(node_v312_source_verification_digest) +
           ",\"completed_prerequisite_count\":" + std::to_string(completed_prerequisite_count) +
           ",\"remaining_prerequisite_count\":" + std::to_string(remaining_prerequisite_count) +
           ",\"original_prerequisite_count\":" + std::to_string(original_prerequisite_count) +
           ",\"no_go_condition_count\":" + std::to_string(source_no_go_condition_count) +
           ",\"chain_continuation_allowed\":false"
           ",\"next_concrete_prerequisite_contract_required\":true"
           ",\"completed_prerequisite_ids\":[\"java-mini-kv-decision-echo\"]"
           ",\"remaining_prerequisite_ids\":" + format_remaining_prerequisites_json() +
           ",\"source_check_count\":" + std::to_string(source_node_v312_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_node_v312_check_count) +
           ",\"source_node_v311_check_count\":" + std::to_string(source_node_v311_check_count) +
           ",\"source_node_v311_passed_check_count\":" + std::to_string(source_node_v311_check_count) +
           ",\"java_mini_kv_decision_echo_closed\":true"
           ",\"exactly_one_prerequisite_closed\":true"
           ",\"five_prerequisites_remain_missing\":true"
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
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"signed-human-approval-artifact-contract-non-participation-receipt-only\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":" + std::to_string(check_count) + "}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v312_check_count\":" + std::to_string(source_node_v312_check_count) +
           ",\"source_node_v312_passed_check_count\":" + std::to_string(source_node_v312_check_count) +
           ",\"source_node_v311_check_count\":" + std::to_string(source_node_v311_check_count) +
           ",\"source_node_v311_passed_check_count\":" + std::to_string(source_node_v311_check_count) +
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
           ",\"warnings\":[{\"code\":\"SIGNED_ARTIFACT_CONTRACT_DOES_NOT_CLOSE_ALL_PREREQUISITES\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V145_AND_MINI_KV_V138_AFTER_V314_ARCHIVE\",\"severity\":\"recommendation\"},"
           "{\"code\":\"KEEP_SIGNED_ARTIFACT_CONTRACT_NON_SECRET\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"signed_human_approval_artifact_contract_intake_json\":" +
           field_string(node_v314_json) +
           ",\"signed_human_approval_artifact_contract_intake_markdown\":" + field_string(node_v314_markdown) +
           ",\"source_node_v312_json\":" + field_string(node_v312_json) +
           ",\"source_node_v312_markdown\":" + field_string(node_v312_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v145\":\"Java v145 signed human approval artifact contract echo\""
           ",\"recommended_parallel_mini_kv_v138\":\"mini-kv v138 signed human approval artifact contract non-participation receipt\""
           ",\"next_node_verification\":\"Node v315\"}"
           ",\"next_required_evidence_versions\":[\"Java v145\",\"mini-kv v138\"]"
           ",\"next_actions\":[\"Archive mini-kv v138 as a signed human approval artifact contract non-participation receipt.\","
           "\"Keep Node v315 blocked until Java v145 and mini-kv v138 are both available.\","
           "\"Do not store, validate, sign, verify, or become authority for signed approval artifacts from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, write ledgers or schema state, auto-start services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
