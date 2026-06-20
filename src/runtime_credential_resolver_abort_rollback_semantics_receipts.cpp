#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_receipt_json_builder.hpp"
#include "runtime_credential_resolver_receipt_utils.hpp"

#include "minikv/version.hpp"

#include <initializer_list>
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
    "mini-kv-credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer = "Node v327 abort/rollback semantics upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v142";
constexpr std::string_view receipt_artifact_path_hint = "d/142/";
constexpr std::string_view source_contract_intake = "Node v326 abort/rollback semantics contract intake";
constexpr std::string_view source_closure_review = "Node v325 no-network safety fixture prerequisite closure review";
constexpr std::string_view source_prerequisite_catalog =
    "Node v313 human approval post-echo prerequisite catalog cleanup";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v326_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-abort-rollback-semantics-contract-intake.v1";
constexpr std::string_view node_v326_contract_state = "abort-rollback-semantics-contract-intake-ready";
constexpr std::string_view node_v326_governance_chain_decision =
    "continue-only-for-abort-rollback-semantics-contract-intake";
constexpr std::string_view node_v326_target_prerequisite_id = "abort-rollback-semantics";
constexpr std::string_view node_v326_contract_digest =
    "fe05bcfd65aabf56ef170bf458837053a11edf0ae44ad203a88d4ecd284299f9";
constexpr std::string_view node_v326_contract_name = "managed-audit-abort-rollback-semantics";
constexpr std::string_view node_v326_contract_version = "abort-rollback-semantics.v1";
constexpr std::string_view node_v326_contract_mode = "abort-rollback-semantics-contract-intake-only";
constexpr std::string_view node_v326_source_span = "Node v325 closure review + Node v313 catalog";
constexpr std::string_view node_v326_purpose = "Define the final manual abort and rollback semantics prerequisite "
                                               "before any later resolver can discuss implementation candidate gates.";

constexpr std::string_view node_v325_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-no-"
                                                       "network-safety-fixture-prerequisite-closure-review.v1";
constexpr std::string_view node_v325_review_state = "no-network-safety-fixture-prerequisite-closure-review-ready";
constexpr std::string_view node_v325_review_digest = "5781245b6dd5b67d6e2985e7e6f70e942defcd4ea95a09dc516743abf7abf0ca";
constexpr std::string_view node_v325_prerequisite_closure_decision = "advance-no-network-safety-fixture-only";

constexpr std::string_view active_plan =
    "docs/plans2/v325-post-no-network-safety-fixture-prerequisite-closure-roadmap.md";
constexpr std::string_view node_v326_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-abort-rollback-semantics-contract-intake";
constexpr std::string_view node_v326_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-abort-"
                                                "rollback-semantics-contract-intake?format=markdown";
constexpr std::string_view node_v325_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-no-network-safety-fixture-prerequisite-closure-review";
constexpr std::string_view node_v325_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-no-"
                                                "network-safety-fixture-prerequisite-closure-review?format=markdown";

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
    "credential resolver abort/rollback semantics contract non-participation receipt only; mini-kv echoes Node v326's "
    "non-executing abort/rollback semantics contract, does not execute rollback, deployment, Java SQL, mini-kv write "
    "commands, runtime shell commands, LOAD/COMPACT/RESTORE/SETNXEX, or become abort/rollback authority, and does not "
    "accept credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP payloads, ledger mutations, "
    "schema SQL, shell script bodies, runtime shell commands, automatic upstream start, or rollback execution requests";
constexpr std::string_view node_action = "verify mini-kv v142 abort/rollback semantics contract non-participation "
                                         "receipt with Java v150 before Node v327 upstream echo verification";

std::string format_required_fields_json() {
    return "[{\"id\":\"manual_abort_marker\",\"accepted_shape\":\"stable non-secret manual abort marker\"},"
           "{\"id\":\"rollback_runbook_reference\",\"accepted_shape\":\"runbook id or immutable document reference\"},"
           "{\"id\":\"operator_confirmation_handle\",\"accepted_shape\":\"operator confirmation handle, no credential "
           "value\"},"
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
    return "[{\"project\":\"java\",\"version\":\"Java v150\",\"requested_echo\":\"Read-only echo of the Node v326 "
           "abort/rollback semantics contract, confirming Java will not execute SQL, deployment, rollback, ledger "
           "writes, or external network calls.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true},"
           "{\"project\":\"mini-kv\",\"version\":\"mini-kv v142\",\"requested_echo\":\"Non-participation receipt "
           "proving mini-kv does not execute LOAD/COMPACT/RESTORE/SETNXEX, write commands, or become abort/rollback "
           "authority.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true}]";
}

std::string format_completed_prerequisites_json() {
    return "[\"java-mini-kv-decision-echo\",\"signed-human-approval-artifact\","
           "\"credential-handle-approval\",\"endpoint-handle-allowlist-approval\","
           "\"no-network-safety-fixture\"]";
}

std::string format_remaining_prerequisites_json() { return "[\"abort-rollback-semantics\"]"; }

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

void append_bool_fields(std::vector<OrderedJsonField>& fields, std::initializer_list<std::string_view> names,
                        bool value) {
    for (const auto name : names) {
        fields.push_back({name, json_bool(value)});
    }
}

void append_closed_boundary_flag_fields(std::vector<OrderedJsonField>& fields) {
    append_bool_fields(fields,
                       {
                           "read_only",
                       },
                       true);
    append_bool_fields(fields,
                       {
                           "execution_allowed",
                       },
                       false);
    append_bool_fields(fields,
                       {
                           "abort_rollback_semantics_contract_non_participation_receipt_only",
                           "abort_rollback_semantics_contract_intake_only",
                           "read_only_abort_rollback_semantics_contract",
                           "consumes_node_v326_abort_rollback_semantics_contract_intake",
                           "consumes_node_v325_no_network_safety_fixture_prerequisite_closure_review",
                           "consumes_node_v313_prerequisite_catalog",
                           "ready_for_node_v327_abort_rollback_semantics_upstream_echo_verification",
                       },
                       true);
    append_bool_fields(fields,
                       {
                           "ready_for_node_v327_before_upstream_echo",
                           "ready_for_final_prerequisite_closure_review",
                           "ready_for_disabled_runtime_shell_implementation",
                           "ready_for_disabled_runtime_shell_invocation",
                           "ready_for_managed_audit_resolver_implementation",
                           "ready_for_managed_audit_sandbox_adapter_connection",
                           "ready_for_production_audit",
                           "ready_for_production_window",
                           "ready_for_production_operations",
                           "runtime_shell_implemented",
                           "runtime_shell_enabled",
                           "runtime_shell_invocation_allowed",
                           "runtime_shell_implementation_allowed",
                           "runtime_shell_command_rendered",
                           "real_resolver_implementation_allowed",
                           "credential_resolver_implemented",
                           "credential_resolver_invoked",
                           "resolver_client_instantiated",
                           "secret_provider_instantiated",
                           "fake_secret_provider_instantiated",
                           "fake_resolver_client_instantiated",
                           "provider_client_instantiation_allowed",
                           "credential_value_accepted",
                           "credential_value_read_allowed",
                           "credential_value_read",
                           "credential_value_provided",
                           "credential_value_loaded",
                           "credential_value_stored",
                           "credential_value_included",
                           "credential_value_rendered",
                           "raw_endpoint_url_accepted",
                           "raw_endpoint_url_stored",
                           "raw_endpoint_url_parse_allowed",
                           "raw_endpoint_url_render_allowed",
                           "raw_endpoint_url_parsed",
                           "raw_endpoint_url_rendered",
                           "endpoint_handle_allowlist_approved",
                           "network_safety_fixture_executed",
                           "abort_rollback_semantics_executed",
                           "abort_rollback_execution_allowed",
                           "abort_rollback_authority",
                           "rollback_execution_allowed",
                           "rollback_executed",
                           "rollback_authority_boundary_authoritative",
                           "rollback_runbook_authoritative",
                           "manual_abort_marker_authoritative",
                           "cleanup_evidence_marker_authoritative",
                           "recovery_checkpoint_authoritative",
                           "deployment_action_allowed",
                           "deployment_action_executed",
                           "java_sql_execution_allowed",
                           "java_sql_executed",
                           "mini_kv_write_command_allowed",
                           "network_fixture_execution_allowed",
                           "network_safety_authority",
                           "transport_denial_policy_authoritative",
                           "external_request_payload_accepted",
                           "external_request_allowed",
                           "external_request_sent",
                           "http_request_allowed",
                           "http_request_sent",
                           "tcp_connection_allowed",
                           "tcp_connection_attempted",
                           "network_socket_opened",
                           "http_tcp_dial_allowed",
                           "dns_lookup_allowed",
                           "tls_handshake_allowed",
                           "connects_managed_audit",
                           "reads_managed_audit_credential",
                           "stores_managed_audit_credential",
                           "managed_audit_store",
                           "managed_audit_storage_backend",
                           "sandbox_audit_storage_backend",
                           "storage_write_allowed",
                           "write_commands_executed",
                           "admin_commands_executed",
                           "runtime_write_observed",
                           "approval_ledger_write_allowed",
                           "approval_ledger_written",
                           "approval_ledger_write_executed",
                           "managed_audit_write_executed",
                           "production_record_written",
                           "schema_migration_allowed",
                           "schema_migration_executed",
                           "schema_rehearsal_execution_allowed",
                           "schema_migration_execution_allowed",
                           "restore_execution_allowed",
                           "load_restore_compact_executed",
                           "setnxex_execution_allowed",
                           "node_auto_start_allowed",
                           "java_auto_start_allowed",
                           "mini_kv_auto_start_allowed",
                           "automatic_upstream_start_allowed",
                           "automatic_upstream_start",
                           "audit_authoritative",
                           "order_authoritative",
                       },
                       false);
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
    const auto abort_rollback_contract = json_object({
        {"contract_digest", json_string(node_v326_contract_digest)},
        {"contract_name", json_string(node_v326_contract_name)},
        {"contract_version", json_string(node_v326_contract_version)},
        {"contract_mode", json_string(node_v326_contract_mode)},
        {"source_span", json_string(node_v326_source_span)},
        {"target_prerequisite_id", json_string(node_v326_target_prerequisite_id)},
        {"purpose", json_string(node_v326_purpose)},
        {"required_fields", format_required_fields_json()},
        {"prohibited_fields", format_prohibited_fields_json()},
        {"rejection_reasons", format_rejection_reasons_json()},
        {"no_go_boundaries", format_no_go_boundaries_json()},
        {"upstream_echo_requests", format_upstream_echo_requests_json()},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"implementation_still_blocked", json_bool(true)},
        {"abort_rollback_execution_allowed", json_bool(false)},
    });

    const auto prerequisite_transition = json_object({
        {"prerequisite_id", json_string(node_v326_target_prerequisite_id)},
        {"catalog_label", json_string("Abort and rollback semantics")},
        {"before_v326", json_string("still-missing")},
        {"after_v326", json_string("contract-intake-defined")},
        {"closure_requires_upstream_echo", json_bool(true)},
        {"completed_prerequisite_count_before_v326", json_integer(5)},
        {"remaining_prerequisite_count_before_v326", json_integer(1)},
        {"preserves_signed_human_approval_artifact_closure", json_bool(true)},
        {"preserves_credential_handle_approval_closure", json_bool(true)},
        {"preserves_endpoint_handle_allowlist_approval_closure", json_bool(true)},
        {"preserves_no_network_safety_fixture_closure", json_bool(true)},
        {"closes_abort_rollback_semantics", json_bool(false)},
    });

    const auto necessity_proof = json_object({
        {"proof_complete", json_bool(true)},
        {"blocker_resolved",
         json_string("v325 completed the no-network-safety-fixture prerequisite and named abort-rollback-semantics "
                     "as the final concrete missing contract.")},
        {"consumer", json_string("Java v150 + mini-kv v142, then Node v327")},
        {"why_v325_cannot_be_reused",
         json_string("v325 is a closure review for no-network-safety-fixture only; it proves 5/6 prerequisites but "
                     "does not define manual abort markers, rollback runbook references, cleanup evidence, authority "
                     "boundaries, or idempotent no-op failure handling.")},
        {"existing_report_reuse_decision",
         json_string("Reuse v325 as source state and v313 as the prerequisite catalog; create v326 only for the "
                     "final abort/rollback semantics contract intake.")},
        {"stop_condition",
         json_string("Stop if the contract requires credential values, raw endpoint URLs, runtime shell commands, "
                     "provider/client configuration, HTTP/TCP, deployment, rollback execution, Java SQL, mini-kv "
                     "writes, ledger/schema writes, or automatic upstream start.")},
    });

    const auto source_node_v326_summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v325_check_count", json_integer(source_node_v325_check_count)},
        {"source_node_v325_passed_check_count", json_integer(source_node_v325_check_count)},
        {"source_completed_prerequisite_count", json_integer(completed_prerequisite_count)},
        {"source_remaining_prerequisite_count", json_integer(remaining_prerequisite_count)},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto source_node_v326_reference = json_object({
        {"source_version", json_string("Node v326")},
        {"profile_version", json_string(node_v326_profile_version)},
        {"contract_state", json_string(node_v326_contract_state)},
        {"governance_chain_decision", json_string(node_v326_governance_chain_decision)},
        {"ready_for_abort_rollback_semantics_contract_intake", json_bool(true)},
        {"abort_rollback_semantics_contract_intake_only", json_bool(true)},
        {"read_only_abort_rollback_semantics_contract", json_bool(true)},
        {"consumes_node_v325_no_network_safety_fixture_prerequisite_closure_review", json_bool(true)},
        {"consumes_node_v313_prerequisite_catalog", json_bool(true)},
        {"target_prerequisite_id", json_string(node_v326_target_prerequisite_id)},
        {"next_java_version", json_string("Java v150")},
        {"next_mini_kv_version", json_string("mini-kv v142")},
        {"next_node_verification_version", json_string("Node v327")},
        {"ready_for_parallel_java_v150_mini_kv_v142_echo", json_bool(true)},
        {"ready_for_node_v327_before_upstream_echo", json_bool(false)},
        {"ready_for_final_prerequisite_closure_review", json_bool(false)},
        {"abort_rollback_semantics_contract", abort_rollback_contract},
        {"prerequisite_transition", prerequisite_transition},
        {"necessity_proof", necessity_proof},
        {"summary", source_node_v326_summary},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"runtime_shell_command_rendered", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"network_safety_fixture_executed", json_bool(false)},
        {"abort_rollback_semantics_executed", json_bool(false)},
        {"rollback_execution_allowed", json_bool(false)},
        {"deployment_action_allowed", json_bool(false)},
        {"java_sql_execution_allowed", json_bool(false)},
        {"mini_kv_write_command_allowed", json_bool(false)},
        {"http_request_sent", json_bool(false)},
        {"tcp_connection_attempted", json_bool(false)},
        {"network_socket_opened", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto source_node_v325_reference = json_object({
        {"source_version", json_string("Node v325")},
        {"profile_version", json_string(node_v325_profile_version)},
        {"review_state", json_string(node_v325_review_state)},
        {"ready_for_no_network_safety_fixture_prerequisite_closure_review", json_bool(true)},
        {"review_digest", json_string(node_v325_review_digest)},
        {"prerequisite_closure_decision", json_string(node_v325_prerequisite_closure_decision)},
        {"completed_prerequisite_count", json_integer(completed_prerequisite_count)},
        {"remaining_prerequisite_count", json_integer(remaining_prerequisite_count)},
        {"original_prerequisite_count", json_integer(original_prerequisite_count)},
        {"next_concrete_prerequisite_id", json_string(node_v326_target_prerequisite_id)},
        {"next_concrete_prerequisite_contract_required", json_bool(true)},
        {"next_node_version_suggested", json_string("Node v326")},
        {"chain_continuation_allowed", json_bool(true)},
        {"runtime_shell_still_blocked", json_bool(true)},
        {"completed_prerequisite_ids", format_completed_prerequisites_json()},
        {"remaining_prerequisite_ids", format_remaining_prerequisites_json()},
        {"source_check_count", json_integer(source_node_v325_check_count)},
        {"source_passed_check_count", json_integer(source_node_v325_check_count)},
        {"source_node_v324_check_count", json_integer(source_node_v324_check_count)},
        {"source_node_v324_passed_check_count", json_integer(source_node_v324_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"source_warning_count", json_integer(1)},
        {"source_recommendation_count", json_integer(2)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"network_safety_fixture_executed", json_bool(false)},
        {"http_request_sent", json_bool(false)},
        {"tcp_connection_attempted", json_bool(false)},
        {"network_socket_opened", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto mini_kv_receipt = json_object({
        {"receipt_mode", json_string("abort-rollback-semantics-contract-non-participation-receipt-only")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(check_count)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v325_check_count", json_integer(source_node_v325_check_count)},
        {"source_node_v325_passed_check_count", json_integer(source_node_v325_check_count)},
        {"source_node_v324_check_count", json_integer(source_node_v324_check_count)},
        {"source_node_v324_passed_check_count", json_integer(source_node_v324_check_count)},
        {"source_completed_prerequisite_count", json_integer(completed_prerequisite_count)},
        {"source_remaining_prerequisite_count", json_integer(remaining_prerequisite_count)},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"proof_complete", json_bool(true)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({
        json_object({
            {"code", json_string("ABORT_ROLLBACK_SEMANTICS_CONTRACT_DOES_NOT_EXECUTE_ROLLBACK")},
            {"severity", json_string("warning")},
        }),
        json_object({
            {"code", json_string("FINAL_PREREQUISITE_CONTRACT_DOES_NOT_APPROVE_RUNTIME")},
            {"severity", json_string("warning")},
        }),
    });

    const auto recommendations = json_array({
        json_object({
            {"code", json_string("RUN_JAVA_V150_AND_MINI_KV_V142_AFTER_V326_ARCHIVE")},
            {"severity", json_string("recommendation")},
        }),
        json_object({
            {"code", json_string("KEEP_ABORT_ROLLBACK_SEMANTICS_CONTRACT_NON_EXECUTING")},
            {"severity", json_string("recommendation")},
        }),
    });

    const auto evidence_endpoints = json_object({
        {"abort_rollback_semantics_contract_intake_json", json_string(node_v326_json)},
        {"abort_rollback_semantics_contract_intake_markdown", json_string(node_v326_markdown)},
        {"source_node_v325_json", json_string(node_v325_json)},
        {"source_node_v325_markdown", json_string(node_v325_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v150", json_string("Java v150 abort/rollback semantics contract echo")},
        {"recommended_parallel_mini_kv_v142",
         json_string("mini-kv v142 abort/rollback semantics contract non-participation receipt")},
        {"next_node_verification", json_string("Node v327")},
    });

    const auto next_required_evidence_versions = json_array({
        json_string("Java v150"),
        json_string("mini-kv v142"),
    });

    const auto next_actions = json_array({
        json_string("Archive mini-kv v142 as an abort/rollback semantics contract non-participation receipt."),
        json_string("Keep Node v327 blocked until Java v150 and mini-kv v142 are both available."),
        json_string("Do not execute rollback, deployment, Java SQL, runtime shell commands, mini-kv write commands, or "
                    "become abort/rollback authority."),
        json_string("Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, write "
                    "ledgers or schema state, auto-start services, or execute LOAD/COMPACT/RESTORE/SETNXEX."),
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string(receipt_version)},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_contract_intake", json_string(source_contract_intake)},
        {"source_closure_review", json_string(source_closure_review)},
        {"source_prerequisite_catalog", json_string(source_prerequisite_catalog)},
        {"consumer_hint", json_string(receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(receipt_release_version)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_role", json_string(runtime_role)},
        {"source_node_v326_reference", source_node_v326_reference},
        {"source_node_v325_reference", source_node_v325_reference},
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
             credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_digest(read_commands))},
    };
    append_closed_boundary_flag_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
