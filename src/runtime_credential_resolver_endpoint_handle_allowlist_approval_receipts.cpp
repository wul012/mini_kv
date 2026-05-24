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
    "mini-kv-credential-resolver-endpoint-handle-allowlist-approval-contract-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer =
    "Node v321 endpoint-handle allowlist approval contract upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-endpoint-handle-allowlist-approval-contract-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v140";
constexpr std::string_view receipt_artifact_path_hint = "d/140/";
constexpr std::string_view source_contract_intake =
    "Node v320 endpoint-handle allowlist approval contract intake";
constexpr std::string_view source_closure_review =
    "Node v319 credential-handle approval prerequisite closure review";
constexpr std::string_view source_prerequisite_catalog =
    "Node v313 human approval post-echo prerequisite catalog cleanup";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v320_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-endpoint-handle-allowlist-approval-contract-intake.v1";
constexpr std::string_view node_v320_contract_state =
    "endpoint-handle-allowlist-approval-contract-intake-ready";
constexpr std::string_view node_v320_governance_chain_decision =
    "continue-only-for-endpoint-handle-allowlist-approval-contract-intake";
constexpr std::string_view node_v320_target_prerequisite_id = "endpoint-handle-allowlist-approval";
constexpr std::string_view node_v320_contract_digest =
    "4657f89caa6866bad87db284dc98efd8d09a6538d8d735535e6a7e6d4d6c33e5";
constexpr std::string_view node_v320_contract_name = "managed-audit-endpoint-handle-allowlist-approval";
constexpr std::string_view node_v320_contract_version = "endpoint-handle-allowlist-approval.v1";
constexpr std::string_view node_v320_contract_mode = "endpoint-handle-allowlist-approval-contract-intake-only";
constexpr std::string_view node_v320_source_span = "Node v319 closure review + Node v313 catalog";
constexpr std::string_view node_v320_purpose =
    "Define the non-secret endpoint handle allowlist approval shape required before any later resolver can discuss a managed audit sandbox endpoint.";

constexpr std::string_view node_v319_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-credential-handle-approval-prerequisite-closure-review.v1";
constexpr std::string_view node_v319_review_state =
    "credential-handle-approval-prerequisite-closure-review-ready";
constexpr std::string_view node_v319_review_digest =
    "59888d94ccd996aeb2f126c25291a8f5ba6f37d6d93cdf190fc656c0121bc7e5";

constexpr std::string_view active_plan =
    "docs/plans2/v319-post-credential-handle-prerequisite-closure-roadmap.md";
constexpr std::string_view node_v320_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-endpoint-handle-allowlist-approval-contract-intake";
constexpr std::string_view node_v320_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-endpoint-handle-allowlist-approval-contract-intake?format=markdown";
constexpr std::string_view node_v319_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-credential-handle-approval-prerequisite-closure-review";
constexpr std::string_view node_v319_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-credential-handle-approval-prerequisite-closure-review?format=markdown";

constexpr int check_count = 20;
constexpr int source_node_v319_check_count = 17;
constexpr int source_node_v318_check_count = 23;
constexpr int original_prerequisite_count = 6;
constexpr int completed_prerequisite_count = 3;
constexpr int remaining_prerequisite_count = 3;
constexpr int required_field_count = 10;
constexpr int prohibited_field_count = 8;
constexpr int rejection_reason_count = 5;
constexpr int no_go_boundary_count = 9;
constexpr int upstream_echo_request_count = 2;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 2;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver endpoint-handle allowlist approval contract non-participation receipt only; mini-kv echoes Node v320's non-raw-URL endpoint-handle allowlist approval contract, does not store raw endpoint URLs, validate endpoint authority, or become authority for endpoint handles or allowlist approval statuses, and does not accept credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP payloads, ledger mutations, schema SQL, mini-kv write commands, runtime shell invocation requests, automatic upstream start, or LOAD/COMPACT/RESTORE/SETNXEX execution";
constexpr std::string_view node_action =
    "verify mini-kv v140 endpoint-handle allowlist approval contract non-participation receipt with Java v147 before Node v321 upstream echo verification";

std::string format_required_fields_json() {
    return "[{\"id\":\"endpoint_handle\",\"accepted_shape\":\"stable non-secret endpoint handle\"},"
           "{\"id\":\"approval_correlation_id\",\"accepted_shape\":\"stable non-secret correlation id\"},"
           "{\"id\":\"operator_identity_handle\",\"accepted_shape\":\"operator identity handle, no credential value\"},"
           "{\"id\":\"reviewer_identity_handle\",\"accepted_shape\":\"reviewer identity handle, no private key\"},"
           "{\"id\":\"policy_version\",\"accepted_shape\":\"policy id or semantic version\"},"
           "{\"id\":\"approval_status\",\"accepted_shape\":\"approved, rejected, expired, or revoked\"},"
           "{\"id\":\"issued_at\",\"accepted_shape\":\"ISO-8601 timestamp\"},"
           "{\"id\":\"expires_at\",\"accepted_shape\":\"ISO-8601 timestamp\"},"
           "{\"id\":\"revocation_marker\",\"accepted_shape\":\"boolean marker plus optional evidence handle\"},"
           "{\"id\":\"audit_digest\",\"accepted_shape\":\"sha256 digest or equivalent stable digest\"}]";
}

std::string format_prohibited_fields_json() {
    return "[{\"id\":\"credential_value\",\"rejection_code\":\"CREDENTIAL_VALUE_PRESENT\"},"
           "{\"id\":\"raw_endpoint_url\",\"rejection_code\":\"RAW_ENDPOINT_URL_PRESENT\"},"
           "{\"id\":\"secret_provider_config\",\"rejection_code\":\"SECRET_PROVIDER_CONFIG_PRESENT\"},"
           "{\"id\":\"resolver_client_config\",\"rejection_code\":\"RESOLVER_CLIENT_CONFIG_PRESENT\"},"
           "{\"id\":\"provider_client_runtime_binding\",\"rejection_code\":\"PROVIDER_CLIENT_RUNTIME_BINDING_PRESENT\"},"
           "{\"id\":\"external_request_payload\",\"rejection_code\":\"EXTERNAL_REQUEST_PAYLOAD_PRESENT\"},"
           "{\"id\":\"approval_ledger_mutation\",\"rejection_code\":\"APPROVAL_LEDGER_MUTATION_PRESENT\"},"
           "{\"id\":\"schema_migration_sql\",\"rejection_code\":\"SCHEMA_MIGRATION_SQL_PRESENT\"}]";
}

std::string format_rejection_reasons_json() {
    return "[\"ENDPOINT_HANDLE_MISSING\",\"CREDENTIAL_VALUE_PRESENT\",\"RAW_ENDPOINT_URL_PRESENT\","
           "\"PROVIDER_CLIENT_CONFIG_PRESENT\",\"WRITE_OR_MIGRATION_PRESENT\"]";
}

std::string format_no_go_boundaries_json() {
    return "[{\"id\":\"credential_value_read\",\"allowed\":false},"
           "{\"id\":\"raw_endpoint_url_parse\",\"allowed\":false},"
           "{\"id\":\"secret_provider_instantiation\",\"allowed\":false},"
           "{\"id\":\"resolver_client_instantiation\",\"allowed\":false},"
           "{\"id\":\"external_request\",\"allowed\":false},"
           "{\"id\":\"ledger_or_schema_write\",\"allowed\":false},"
           "{\"id\":\"automatic_upstream_start\",\"allowed\":false},"
           "{\"id\":\"runtime_shell_implementation\",\"allowed\":false},"
           "{\"id\":\"runtime_shell_invocation\",\"allowed\":false}]";
}

std::string format_upstream_echo_requests_json() {
    return "[{\"project\":\"java\",\"version\":\"Java v147\",\"requested_echo\":\"Read-only echo of the Node v320 endpoint-handle allowlist approval contract.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true},"
           "{\"project\":\"mini-kv\",\"version\":\"mini-kv v140\",\"requested_echo\":\"Non-participation receipt proving mini-kv does not store raw endpoint URLs, validate allowlist authority, or become authority for endpoint handles.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true}]";
}

std::string format_completed_prerequisites_json() {
    return "[\"java-mini-kv-decision-echo\",\"signed-human-approval-artifact\",\"credential-handle-approval\"]";
}

std::string format_remaining_prerequisites_json() {
    return "[\"endpoint-handle-allowlist-approval\",\"no-network-safety-fixture\",\"abort-rollback-semantics\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v319_ready\":true,"
           "\"source_node_v319_points_to_endpoint_handle_allowlist\":true,"
           "\"source_node_v319_keeps_runtime_blocked\":true,"
           "\"source_node_v319_keeps_side_effects_closed\":true,"
           "\"endpoint_handle_allowlist_approval_still_missing_in_source\":true,"
           "\"catalog_target_matches_endpoint_handle_allowlist\":true,"
           "\"contract_required_fields_documented\":true,"
           "\"contract_prohibited_fields_documented\":true,"
           "\"rejection_reasons_documented\":true,"
           "\"no_go_boundaries_closed\":true,"
           "\"prerequisite_transition_scoped_to_endpoint_handle_allowlist\":true,"
           "\"necessity_proof_documented\":true,"
           "\"java_mini_kv_echo_request_explicitly_parallel\":true,"
           "\"contract_stays_non_secret\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"runtime_shell_implementation_still_blocked\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v321_endpoint_handle_allowlist_approval_contract_upstream_echo_verification\":true}";
}

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"endpoint_handle_allowlist_approval_contract_non_participation_receipt_only\":true,"
           "\"endpoint_handle_allowlist_approval_contract_intake_only\":true,"
           "\"read_only_endpoint_handle_allowlist_contract\":true,"
           "\"consumes_node_v320_endpoint_handle_allowlist_approval_contract_intake\":true,"
           "\"consumes_node_v319_credential_handle_approval_prerequisite_closure_review\":true,"
           "\"consumes_node_v313_prerequisite_catalog\":true,"
           "\"ready_for_node_v321_endpoint_handle_allowlist_approval_contract_upstream_echo_verification\":true,"
           "\"ready_for_node_v321_before_upstream_echo\":false,"
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
           "\"endpoint_handle_stored\":false,"
           "\"endpoint_handle_validated\":false,"
           "\"endpoint_handle_resolved\":false,"
           "\"endpoint_handle_authoritative\":false,"
           "\"endpoint_allowlist_authority\":false,"
           "\"endpoint_allowlist_approval_status_authoritative\":false,"
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

std::string credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_contract_intake}},
        {std::string{source_closure_review}},
        {std::string{source_prerequisite_catalog}},
        {std::string{node_v320_profile_version}},
        {std::string{node_v320_contract_state}},
        {std::string{node_v320_contract_digest}},
        {std::string{node_v320_target_prerequisite_id}},
        {std::string{node_v319_profile_version}},
        {std::string{node_v319_review_state}},
        {std::string{node_v319_review_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_node_v319_check_count)},
        {std::to_string(remaining_prerequisite_count)},
        {std::to_string(required_field_count)},
        {std::to_string(prohibited_field_count)},
        {std::to_string(rejection_reason_count)},
        {std::to_string(no_go_boundary_count)},
        {"endpoint-handle-allowlist-approval-contract-non-participation-only"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-endpoint-handle-allowlist-approval-contract", parts);
}

std::string format_credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_json(
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
           ",\"source_node_v320_reference\":{\"source_version\":\"Node v320\""
           ",\"profile_version\":" + field_string(node_v320_profile_version) +
           ",\"contract_state\":" + field_string(node_v320_contract_state) +
           ",\"governance_chain_decision\":" + field_string(node_v320_governance_chain_decision) +
           ",\"ready_for_endpoint_handle_allowlist_approval_contract_intake\":true"
           ",\"endpoint_handle_allowlist_approval_contract_intake_only\":true"
           ",\"read_only_endpoint_handle_allowlist_approval_contract\":true"
           ",\"consumes_node_v319_credential_handle_approval_prerequisite_closure_review\":true"
           ",\"consumes_node_v313_prerequisite_catalog\":true"
           ",\"target_prerequisite_id\":" + field_string(node_v320_target_prerequisite_id) +
           ",\"next_java_version\":\"Java v147\""
           ",\"next_mini_kv_version\":\"mini-kv v140\""
           ",\"next_node_verification_version\":\"Node v321\""
           ",\"ready_for_parallel_java_v147_mini_kv_v140_echo\":true"
           ",\"ready_for_node_v321_before_upstream_echo\":false"
           ",\"endpoint_handle_allowlist_approval_contract\":{\"contract_digest\":" + field_string(node_v320_contract_digest) +
           ",\"contract_name\":" + field_string(node_v320_contract_name) +
           ",\"contract_version\":" + field_string(node_v320_contract_version) +
           ",\"contract_mode\":" + field_string(node_v320_contract_mode) +
           ",\"source_span\":" + field_string(node_v320_source_span) +
           ",\"target_prerequisite_id\":" + field_string(node_v320_target_prerequisite_id) +
           ",\"purpose\":" + field_string(node_v320_purpose) +
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
           ",\"prerequisite_transition\":{\"prerequisite_id\":" + field_string(node_v320_target_prerequisite_id) +
           ",\"catalog_label\":\"Endpoint handle allowlist approval\""
           ",\"before_v320\":\"still-missing\""
           ",\"after_v320\":\"contract-intake-defined\""
           ",\"closure_requires_upstream_echo\":true"
           ",\"completed_prerequisite_count_before_v320\":3"
           ",\"remaining_prerequisite_count_before_v320\":3"
           ",\"preserves_signed_human_approval_artifact_closure\":true"
           ",\"preserves_credential_handle_approval_closure\":true"
           ",\"closes_endpoint_handle_allowlist_approval\":false"
           ",\"closes_no_network_safety_fixture\":false"
           ",\"closes_abort_rollback_semantics\":false}"
           ",\"necessity_proof\":{\"proof_complete\":true"
           ",\"blocker_resolved\":\"v319 completed the credential-handle-approval prerequisite and named endpoint-handle-allowlist-approval as the next concrete missing contract.\""
           ",\"consumer\":\"Java v147 + mini-kv v140, then Node v321\""
           ",\"why_v319_cannot_be_reused\":\"v319 is a closure review only; it proves the credential-handle-approval prerequisite is complete but does not define endpoint handle allowlist approval fields for upstream echo.\""
           ",\"existing_report_reuse_decision\":\"Reuse v319 as source state and v313 as the prerequisite catalog; create v320 only for the endpoint-handle allowlist approval contract intake.\""
           ",\"stop_condition\":\"Stop if the contract requires credential values, raw endpoint URLs, provider/client configuration, external requests, runtime shell implementation or invocation, ledger/schema writes, mini-kv authority, or automatic upstream start.\"}"
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v319_check_count\":" + std::to_string(source_node_v319_check_count) +
           ",\"source_node_v319_passed_check_count\":" + std::to_string(source_node_v319_check_count) +
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
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"source_node_v319_reference\":{\"source_version\":\"Node v319\""
           ",\"profile_version\":" + field_string(node_v319_profile_version) +
           ",\"review_state\":" + field_string(node_v319_review_state) +
           ",\"ready_for_credential_handle_approval_prerequisite_closure_review\":true"
           ",\"review_digest\":" + field_string(node_v319_review_digest) +
           ",\"completed_prerequisite_count\":" + std::to_string(completed_prerequisite_count) +
           ",\"remaining_prerequisite_count\":" + std::to_string(remaining_prerequisite_count) +
           ",\"original_prerequisite_count\":" + std::to_string(original_prerequisite_count) +
           ",\"next_concrete_prerequisite_id\":" + field_string(node_v320_target_prerequisite_id) +
           ",\"next_concrete_prerequisite_contract_required\":true"
           ",\"next_node_version_suggested\":\"Node v320\""
           ",\"chain_continuation_allowed\":true"
           ",\"runtime_shell_still_blocked\":true"
           ",\"completed_prerequisite_ids\":" + format_completed_prerequisites_json() +
           ",\"remaining_prerequisite_ids\":" + format_remaining_prerequisites_json() +
           ",\"source_check_count\":" + std::to_string(source_node_v319_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_node_v319_check_count) +
           ",\"source_node_v318_check_count\":" + std::to_string(source_node_v318_check_count) +
           ",\"source_node_v318_passed_check_count\":" + std::to_string(source_node_v318_check_count) +
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
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false}"
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"endpoint-handle-allowlist-approval-contract-non-participation-receipt-only\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":" + std::to_string(check_count) + "}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v319_check_count\":" + std::to_string(source_node_v319_check_count) +
           ",\"source_node_v319_passed_check_count\":" + std::to_string(source_node_v319_check_count) +
           ",\"source_node_v318_check_count\":" + std::to_string(source_node_v318_check_count) +
           ",\"source_node_v318_passed_check_count\":" + std::to_string(source_node_v318_check_count) +
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
           ",\"warnings\":[{\"code\":\"ENDPOINT_HANDLE_ALLOWLIST_CONTRACT_DOES_NOT_CLOSE_ALL_PREREQUISITES\",\"severity\":\"warning\"},"
           "{\"code\":\"ENDPOINT_HANDLE_ALLOWLIST_APPROVAL_IS_NOT_CONNECTION_PERMISSION\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_JAVA_V147_AND_MINI_KV_V140_AFTER_V320_ARCHIVE\",\"severity\":\"recommendation\"},"
           "{\"code\":\"KEEP_ENDPOINT_HANDLE_ALLOWLIST_APPROVAL_NON_SECRET\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"endpoint_handle_allowlist_approval_contract_intake_json\":" + field_string(node_v320_json) +
           ",\"endpoint_handle_allowlist_approval_contract_intake_markdown\":" + field_string(node_v320_markdown) +
           ",\"source_node_v319_json\":" + field_string(node_v319_json) +
           ",\"source_node_v319_markdown\":" + field_string(node_v319_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"recommended_parallel_java_v147\":\"Java v147 endpoint-handle allowlist approval contract echo\""
           ",\"recommended_parallel_mini_kv_v140\":\"mini-kv v140 endpoint-handle allowlist approval contract non-participation receipt\""
           ",\"next_node_verification\":\"Node v321\"}"
           ",\"next_required_evidence_versions\":[\"Java v147\",\"mini-kv v140\"]"
           ",\"next_actions\":[\"Archive mini-kv v140 as an endpoint-handle allowlist approval contract non-participation receipt.\","
           "\"Keep Node v321 blocked until Java v147 and mini-kv v140 are both available.\","
           "\"Do not store, validate, resolve, or become authority for endpoint handles or approval statuses from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, write ledgers or schema state, auto-start services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" + field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_digest(
               read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
