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
    "mini-kv-credential-resolver-no-network-safety-fixture-contract-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer = "Node v324 no-network safety fixture upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-no-network-safety-fixture-contract-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v141";
constexpr std::string_view receipt_artifact_path_hint = "d/141/";
constexpr std::string_view source_contract_intake = "Node v323 no-network safety fixture contract intake";
constexpr std::string_view source_closure_review =
    "Node v322 endpoint-handle allowlist approval prerequisite closure review";
constexpr std::string_view source_prerequisite_catalog =
    "Node v313 human approval post-echo prerequisite catalog cleanup";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v323_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-no-network-safety-fixture-contract-intake.v1";
constexpr std::string_view node_v323_contract_state = "no-network-safety-fixture-contract-intake-ready";
constexpr std::string_view node_v323_governance_chain_decision =
    "continue-only-for-no-network-safety-fixture-contract-intake";
constexpr std::string_view node_v323_target_prerequisite_id = "no-network-safety-fixture";
constexpr std::string_view node_v323_contract_digest =
    "73dcd413298048da6283f81bd0c3b759b9c1c2d360eb1382983d7db7349b2b88";
constexpr std::string_view node_v323_contract_name = "managed-audit-no-network-safety-fixture";
constexpr std::string_view node_v323_contract_version = "no-network-safety-fixture.v1";
constexpr std::string_view node_v323_contract_mode = "no-network-safety-fixture-contract-intake-only";
constexpr std::string_view node_v323_source_span = "Node v322 closure review + Node v313 catalog";
constexpr std::string_view node_v323_purpose =
    "Define the no-network safety fixture shape required before any later resolver can discuss a runtime path that "
    "must refuse HTTP/TCP before approval.";

constexpr std::string_view node_v322_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-endpoint-handle-allowlist-approval-prerequisite-"
    "closure-review.v1";
constexpr std::string_view node_v322_review_state =
    "endpoint-handle-allowlist-approval-prerequisite-closure-review-ready";
constexpr std::string_view node_v322_review_digest = "3b774f17053360aa1f3f747db9179298190d5c8daa03aaaf15ff591483eb863e";

constexpr std::string_view active_plan = "docs/plans2/v322-post-endpoint-handle-prerequisite-closure-roadmap.md";
constexpr std::string_view node_v323_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-no-network-safety-fixture-contract-intake";
constexpr std::string_view node_v323_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-no-"
                                                "network-safety-fixture-contract-intake?format=markdown";
constexpr std::string_view node_v322_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-endpoint-"
                                            "handle-allowlist-approval-prerequisite-closure-review";
constexpr std::string_view node_v322_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-endpoint-"
                                                "handle-allowlist-approval-prerequisite-closure-review?format=markdown";

constexpr int check_count = 21;
constexpr int source_node_v322_check_count = 17;
constexpr int source_node_v321_check_count = 24;
constexpr int original_prerequisite_count = 6;
constexpr int completed_prerequisite_count = 4;
constexpr int remaining_prerequisite_count = 2;
constexpr int required_field_count = 10;
constexpr int prohibited_field_count = 12;
constexpr int rejection_reason_count = 6;
constexpr int no_go_boundary_count = 10;
constexpr int upstream_echo_request_count = 2;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 2;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver no-network safety fixture contract non-participation receipt only; mini-kv echoes Node v323's "
    "no-network fixture contract, does not execute the fixture, open sockets, send HTTP/TCP, perform DNS/TLS transport "
    "work, or become network safety authority, and does not accept credential values, raw endpoint URLs, "
    "provider/client configuration, external request payloads, ledger mutations, schema SQL, runtime shell invocation "
    "requests, automatic upstream start, or LOAD/COMPACT/RESTORE/SETNXEX execution";
constexpr std::string_view node_action = "verify mini-kv v141 no-network safety fixture contract non-participation "
                                         "receipt with Java v149 before Node v324 upstream echo verification";

std::string format_required_fields_json() {
    return "[{\"id\":\"fixture_id\",\"accepted_shape\":\"stable non-secret fixture identifier\"},"
           "{\"id\":\"operator_confirmation_handle\",\"accepted_shape\":\"operator confirmation handle, no credential "
           "value\"},"
           "{\"id\":\"approval_correlation_id\",\"accepted_shape\":\"stable non-secret correlation id\"},"
           "{\"id\":\"transport_denial_policy_id\",\"accepted_shape\":\"policy id or semantic version\"},"
           "{\"id\":\"expected_denied_transport_classes\",\"accepted_shape\":\"HTTP, HTTPS, TCP, TLS, DNS, or "
           "equivalent class names\"},"
           "{\"id\":\"required_denial_evidence\",\"accepted_shape\":\"machine-readable denial evidence handle or "
           "digest\"},"
           "{\"id\":\"forbidden_network_actions\",\"accepted_shape\":\"non-empty action id list\"},"
           "{\"id\":\"cleanup_marker\",\"accepted_shape\":\"stable cleanup marker or digest\"},"
           "{\"id\":\"timeout_budget_ms\",\"accepted_shape\":\"positive integer budget\"},"
           "{\"id\":\"audit_digest\",\"accepted_shape\":\"sha256 digest or equivalent stable digest\"}]";
}

std::string format_prohibited_fields_json() {
    return "[{\"id\":\"credential_value\",\"rejection_code\":\"CREDENTIAL_VALUE_PRESENT\"},"
           "{\"id\":\"raw_endpoint_url\",\"rejection_code\":\"RAW_ENDPOINT_URL_PRESENT\"},"
           "{\"id\":\"secret_provider_config\",\"rejection_code\":\"SECRET_PROVIDER_CONFIG_PRESENT\"},"
           "{\"id\":\"resolver_client_config\",\"rejection_code\":\"RESOLVER_CLIENT_CONFIG_PRESENT\"},"
           "{\"id\":\"external_request_payload\",\"rejection_code\":\"EXTERNAL_REQUEST_PAYLOAD_PRESENT\"},"
           "{\"id\":\"network_socket_open\",\"rejection_code\":\"NETWORK_SOCKET_OPEN_PRESENT\"},"
           "{\"id\":\"http_request_execution\",\"rejection_code\":\"HTTP_REQUEST_EXECUTION_PRESENT\"},"
           "{\"id\":\"tcp_connection_attempt\",\"rejection_code\":\"TCP_CONNECTION_ATTEMPT_PRESENT\"},"
           "{\"id\":\"approval_ledger_mutation\",\"rejection_code\":\"APPROVAL_LEDGER_MUTATION_PRESENT\"},"
           "{\"id\":\"schema_migration_sql\",\"rejection_code\":\"SCHEMA_MIGRATION_SQL_PRESENT\"},"
           "{\"id\":\"upstream_process_start\",\"rejection_code\":\"UPSTREAM_PROCESS_START_PRESENT\"},"
           "{\"id\":\"runtime_shell_invocation\",\"rejection_code\":\"RUNTIME_SHELL_INVOCATION_PRESENT\"}]";
}

std::string format_rejection_reasons_json() {
    return "[\"FIXTURE_ID_MISSING\",\"DENIAL_POLICY_MISSING\",\"NETWORK_ACTION_PRESENT\","
           "\"CREDENTIAL_OR_RAW_ENDPOINT_PRESENT\",\"PROVIDER_CLIENT_CONFIG_PRESENT\","
           "\"WRITE_OR_RUNTIME_ACTION_PRESENT\"]";
}

std::string format_no_go_boundaries_json() {
    return "[{\"id\":\"credential_value_read\",\"allowed\":false},"
           "{\"id\":\"raw_endpoint_url_parse\",\"allowed\":false},"
           "{\"id\":\"secret_provider_instantiation\",\"allowed\":false},"
           "{\"id\":\"resolver_client_instantiation\",\"allowed\":false},"
           "{\"id\":\"http_request_send\",\"allowed\":false},"
           "{\"id\":\"tcp_socket_connect\",\"allowed\":false},"
           "{\"id\":\"network_fixture_execution\",\"allowed\":false},"
           "{\"id\":\"ledger_or_schema_write\",\"allowed\":false},"
           "{\"id\":\"automatic_upstream_start\",\"allowed\":false},"
           "{\"id\":\"runtime_shell_invocation\",\"allowed\":false}]";
}

std::string format_upstream_echo_requests_json() {
    return "[{\"project\":\"java\",\"version\":\"Java v149\",\"requested_echo\":\"Read-only echo of the Node v323 "
           "no-network safety fixture contract, confirming Java will not execute SQL, deployment, rollback, ledger "
           "writes, or external network calls.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true},"
           "{\"project\":\"mini-kv\",\"version\":\"mini-kv v141\",\"requested_echo\":\"Non-participation receipt "
           "proving mini-kv does not execute LOAD/COMPACT/RESTORE/SETNXEX, open network sockets, or become network "
           "safety authority.\",\"can_run_in_parallel\":true,\"must_remain_read_only\":true}]";
}

std::string format_completed_prerequisites_json() {
    return "[\"java-mini-kv-decision-echo\",\"signed-human-approval-artifact\","
           "\"credential-handle-approval\",\"endpoint-handle-allowlist-approval\"]";
}

std::string format_remaining_prerequisites_json() {
    return "[\"no-network-safety-fixture\",\"abort-rollback-semantics\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v322_ready\":true,"
           "\"source_node_v322_points_to_no_network_safety_fixture\":true,"
           "\"source_node_v322_keeps_runtime_blocked\":true,"
           "\"source_node_v322_keeps_side_effects_closed\":true,"
           "\"no_network_safety_fixture_still_missing_in_source\":true,"
           "\"catalog_target_matches_no_network_safety_fixture\":true,"
           "\"contract_required_fields_documented\":true,"
           "\"contract_prohibited_fields_documented\":true,"
           "\"rejection_reasons_documented\":true,"
           "\"no_go_boundaries_closed\":true,"
           "\"prerequisite_transition_scoped_to_no_network_safety_fixture\":true,"
           "\"necessity_proof_documented\":true,"
           "\"java_mini_kv_echo_request_explicitly_parallel\":true,"
           "\"contract_stays_no_network_and_non_secret\":true,"
           "\"fixture_execution_still_blocked\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"runtime_shell_implementation_still_blocked\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v324_no_network_safety_fixture_upstream_echo_verification\":true}";
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
                           "no_network_safety_fixture_contract_non_participation_receipt_only",
                           "no_network_safety_fixture_contract_intake_only",
                           "read_only_no_network_safety_fixture_contract",
                           "consumes_node_v323_no_network_safety_fixture_contract_intake",
                           "consumes_node_v322_endpoint_handle_allowlist_approval_prerequisite_closure_review",
                           "consumes_node_v313_prerequisite_catalog",
                           "ready_for_node_v324_no_network_safety_fixture_upstream_echo_verification",
                       },
                       true);
    append_bool_fields(fields,
                       {
                           "ready_for_node_v324_before_upstream_echo",
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
                           "network_fixture_execution_allowed",
                           "network_safety_authority",
                           "transport_denial_policy_authoritative",
                           "cleanup_marker_authoritative",
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

std::string credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_contract_intake}},
        {std::string{source_closure_review}},
        {std::string{source_prerequisite_catalog}},
        {std::string{node_v323_profile_version}},
        {std::string{node_v323_contract_state}},
        {std::string{node_v323_contract_digest}},
        {std::string{node_v323_target_prerequisite_id}},
        {std::string{node_v322_profile_version}},
        {std::string{node_v322_review_state}},
        {std::string{node_v322_review_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_node_v322_check_count)},
        {std::to_string(remaining_prerequisite_count)},
        {std::to_string(required_field_count)},
        {std::to_string(prohibited_field_count)},
        {std::to_string(rejection_reason_count)},
        {std::to_string(no_go_boundary_count)},
        {"no-network-safety-fixture-contract-non-participation-only"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-no-network-safety-fixture-contract", parts);
}

std::string format_credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto no_network_safety_fixture_contract = json_object({
        {"contract_digest", json_string(node_v323_contract_digest)},
        {"contract_name", json_string(node_v323_contract_name)},
        {"contract_version", json_string(node_v323_contract_version)},
        {"contract_mode", json_string(node_v323_contract_mode)},
        {"source_span", json_string(node_v323_source_span)},
        {"target_prerequisite_id", json_string(node_v323_target_prerequisite_id)},
        {"purpose", json_string(node_v323_purpose)},
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
        {"fixture_execution_allowed", json_bool(false)},
    });

    const auto prerequisite_transition = json_object({
        {"prerequisite_id", json_string(node_v323_target_prerequisite_id)},
        {"catalog_label", json_string("No-network safety fixture")},
        {"before_v323", json_string("still-missing")},
        {"after_v323", json_string("contract-intake-defined")},
        {"closure_requires_upstream_echo", json_bool(true)},
        {"completed_prerequisite_count_before_v323", json_integer(4)},
        {"remaining_prerequisite_count_before_v323", json_integer(2)},
        {"preserves_signed_human_approval_artifact_closure", json_bool(true)},
        {"preserves_credential_handle_approval_closure", json_bool(true)},
        {"preserves_endpoint_handle_allowlist_approval_closure", json_bool(true)},
        {"closes_no_network_safety_fixture", json_bool(false)},
        {"closes_abort_rollback_semantics", json_bool(false)},
    });

    const auto necessity_proof = json_object({
        {"proof_complete", json_bool(true)},
        {"blocker_resolved", json_string("v322 completed the endpoint-handle-allowlist-approval prerequisite and named "
                                         "no-network-safety-fixture as the next concrete missing contract.")},
        {"consumer", json_string("Java v149 + mini-kv v141, then Node v324")},
        {"why_v322_cannot_be_reused",
         json_string("v322 is a closure review only; it proves endpoint-handle-allowlist-approval is complete but does "
                     "not define no-network denial evidence for upstream echo.")},
        {"existing_report_reuse_decision",
         json_string("Reuse v322 as source state and v313 as the prerequisite catalog; create v323 only for the "
                     "no-network safety fixture contract intake.")},
        {"stop_condition",
         json_string("Stop if the contract requires credential values, raw endpoint URLs, provider/client "
                     "configuration, network execution, runtime shell implementation or invocation, ledger/schema "
                     "writes, mini-kv authority, or automatic upstream start.")},
    });

    const auto source_node_v323_summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v322_check_count", json_integer(source_node_v322_check_count)},
        {"source_node_v322_passed_check_count", json_integer(source_node_v322_check_count)},
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

    const auto source_node_v323_reference = json_object({
        {"source_version", json_string("Node v323")},
        {"profile_version", json_string(node_v323_profile_version)},
        {"contract_state", json_string(node_v323_contract_state)},
        {"governance_chain_decision", json_string(node_v323_governance_chain_decision)},
        {"ready_for_no_network_safety_fixture_contract_intake", json_bool(true)},
        {"no_network_safety_fixture_contract_intake_only", json_bool(true)},
        {"read_only_no_network_safety_fixture_contract", json_bool(true)},
        {"consumes_node_v322_endpoint_handle_allowlist_approval_prerequisite_closure_review", json_bool(true)},
        {"consumes_node_v313_prerequisite_catalog", json_bool(true)},
        {"target_prerequisite_id", json_string(node_v323_target_prerequisite_id)},
        {"next_java_version", json_string("Java v149")},
        {"next_mini_kv_version", json_string("mini-kv v141")},
        {"next_node_verification_version", json_string("Node v324")},
        {"ready_for_parallel_java_v149_mini_kv_v141_echo", json_bool(true)},
        {"ready_for_node_v324_before_upstream_echo", json_bool(false)},
        {"no_network_safety_fixture_contract", no_network_safety_fixture_contract},
        {"prerequisite_transition", prerequisite_transition},
        {"necessity_proof", necessity_proof},
        {"summary", source_node_v323_summary},
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
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto source_node_v322_reference = json_object({
        {"source_version", json_string("Node v322")},
        {"profile_version", json_string(node_v322_profile_version)},
        {"review_state", json_string(node_v322_review_state)},
        {"ready_for_endpoint_handle_allowlist_approval_prerequisite_closure_review", json_bool(true)},
        {"review_digest", json_string(node_v322_review_digest)},
        {"completed_prerequisite_count", json_integer(completed_prerequisite_count)},
        {"remaining_prerequisite_count", json_integer(remaining_prerequisite_count)},
        {"original_prerequisite_count", json_integer(original_prerequisite_count)},
        {"next_concrete_prerequisite_id", json_string(node_v323_target_prerequisite_id)},
        {"next_concrete_prerequisite_contract_required", json_bool(true)},
        {"next_node_version_suggested", json_string("Node v323")},
        {"chain_continuation_allowed", json_bool(true)},
        {"runtime_shell_still_blocked", json_bool(true)},
        {"completed_prerequisite_ids", format_completed_prerequisites_json()},
        {"remaining_prerequisite_ids", format_remaining_prerequisites_json()},
        {"source_check_count", json_integer(source_node_v322_check_count)},
        {"source_passed_check_count", json_integer(source_node_v322_check_count)},
        {"source_node_v321_check_count", json_integer(source_node_v321_check_count)},
        {"source_node_v321_passed_check_count", json_integer(source_node_v321_check_count)},
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
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto mini_kv_receipt = json_object({
        {"receipt_mode", json_string("no-network-safety-fixture-contract-non-participation-receipt-only")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(check_count)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v322_check_count", json_integer(source_node_v322_check_count)},
        {"source_node_v322_passed_check_count", json_integer(source_node_v322_check_count)},
        {"source_node_v321_check_count", json_integer(source_node_v321_check_count)},
        {"source_node_v321_passed_check_count", json_integer(source_node_v321_check_count)},
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
            {"code", json_string("NO_NETWORK_SAFETY_FIXTURE_CONTRACT_DOES_NOT_EXECUTE_FIXTURE")},
            {"severity", json_string("warning")},
        }),
        json_object({
            {"code", json_string("NO_NETWORK_SAFETY_FIXTURE_DOES_NOT_CLOSE_ABORT_ROLLBACK")},
            {"severity", json_string("warning")},
        }),
    });

    const auto recommendations = json_array({
        json_object({
            {"code", json_string("RUN_JAVA_V149_AND_MINI_KV_V141_AFTER_V323_ARCHIVE")},
            {"severity", json_string("recommendation")},
        }),
        json_object({
            {"code", json_string("KEEP_NO_NETWORK_FIXTURE_CONTRACT_NON_EXECUTING")},
            {"severity", json_string("recommendation")},
        }),
    });

    const auto evidence_endpoints = json_object({
        {"no_network_safety_fixture_contract_intake_json", json_string(node_v323_json)},
        {"no_network_safety_fixture_contract_intake_markdown", json_string(node_v323_markdown)},
        {"source_node_v322_json", json_string(node_v322_json)},
        {"source_node_v322_markdown", json_string(node_v322_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v149", json_string("Java v149 no-network safety fixture contract echo")},
        {"recommended_parallel_mini_kv_v141",
         json_string("mini-kv v141 no-network safety fixture contract non-participation receipt")},
        {"next_node_verification", json_string("Node v324")},
    });

    const auto next_required_evidence_versions = json_array({
        json_string("Java v149"),
        json_string("mini-kv v141"),
    });

    const auto next_actions = json_array({
        json_string("Archive mini-kv v141 as a no-network safety fixture contract non-participation receipt."),
        json_string("Keep Node v324 blocked until Java v149 and mini-kv v141 are both available."),
        json_string("Do not execute fixture behavior, open sockets, send HTTP/TCP, or make mini-kv network safety "
                    "authority."),
        json_string("Do not read credentials, parse raw endpoints, accept provider/client config, write ledgers or "
                    "schema state, auto-start services, or execute LOAD/COMPACT/RESTORE/SETNXEX."),
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
        {"source_node_v323_reference", source_node_v323_reference},
        {"source_node_v322_reference", source_node_v322_reference},
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
             credential_resolver_no_network_safety_fixture_contract_non_participation_receipt_digest(read_commands))},
    };
    append_closed_boundary_flag_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
