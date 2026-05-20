#include "minikv/runtime_evidence_receipts.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;

std::string field_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string receipt_digest(std::string_view prefix, const std::vector<DigestPart>& parts) {
    return runtime_evidence::digest(prefix, parts);
}

constexpr std::string_view disabled_candidate_receipt_consumer =
    "Node v274 credential resolver disabled candidate upstream echo verification";
constexpr std::string_view disabled_candidate_receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-implementation-candidate-non-participation-receipt.json";
constexpr std::string_view disabled_candidate_receipt_release_version = "v120";
constexpr std::string_view disabled_candidate_receipt_artifact_path_hint = "c/120/";
constexpr std::string_view disabled_candidate_source_review =
    "Node v273 credential resolver disabled implementation candidate review";
constexpr std::string_view disabled_candidate_source_v272 =
    "Node v272 credential resolver pre-implementation plan intake upstream echo verification";
constexpr std::string_view disabled_candidate_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-implementation-candidate-review.v1";
constexpr std::string_view disabled_candidate_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-implementation-candidate-review";
constexpr std::string_view disabled_candidate_review_state =
    "credential-resolver-disabled-implementation-candidate-review-ready";
constexpr std::string_view disabled_candidate_source_v272_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake-upstream-echo-verification.v1";
constexpr std::string_view disabled_candidate_source_v272_state =
    "credential-resolver-pre-implementation-plan-intake-upstream-echo-verification-ready";
constexpr std::string_view disabled_candidate_source_v272_span =
    "Node v270 + Java v112 + mini-kv v119";
constexpr std::string_view disabled_candidate_source_v272_digest =
    "af7613ac0245fbc1b1ed709b5ea73d7907ab3b84918939c54f57e194446df6f1";
constexpr std::string_view disabled_candidate_plan_version =
    "node-v270-credential-resolver-pre-implementation-plan-intake.v1";
constexpr std::string_view disabled_candidate_plan_mode = "plan-intake-only";
constexpr std::string_view disabled_candidate_plan_digest =
    "3aef2d1d10fa5a0063f9be43af49122de0b38999b0d065a8addb5c3ed1f95ad8";
constexpr std::string_view disabled_candidate_intake_digest =
    "43c40f8aee830a93fa36845366c948abca1353555756154b8dcbb8053a17e603";
constexpr std::string_view disabled_candidate_candidate_version =
    "node-v273-credential-resolver-disabled-implementation-candidate-review.v1";
constexpr std::string_view disabled_candidate_candidate_mode =
    "disabled-interface-and-fake-wiring-review-only";
constexpr std::string_view disabled_candidate_candidate_digest =
    "12862fa65cad3a8db92f88fba86b74c25e584f35614c7a47367d441358b8b7d7";
constexpr std::string_view disabled_candidate_interface_version =
    "disabled-credential-resolver-interface-candidate.v1";
constexpr std::string_view disabled_candidate_fake_wiring_review_version =
    "disabled-credential-resolver-fake-wiring-review.v1";
constexpr std::string_view disabled_candidate_active_plan =
    "docs/plans/v272-post-plan-intake-echo-roadmap.md";
constexpr std::string_view disabled_candidate_runtime_role = "runtime evidence provider only";
constexpr std::string_view disabled_candidate_boundary =
    "credential resolver disabled implementation candidate non-participation receipt only; mini-kv echoes Node v273 disabled interface and fake wiring review boundaries, does not implement or invoke a resolver, does not instantiate a resolver client or secret provider, does not read, load, store, or include credential values, does not parse or include raw endpoint URLs, does not send external requests, does not connect managed audit, does not write storage or approval ledger records, does not execute schema migration or LOAD/COMPACT/RESTORE/SETNXEX, and does not become a managed audit storage backend or order authority";
constexpr std::string_view disabled_candidate_node_action =
    "verify mini-kv non-participation with Node v273 disabled implementation candidate review before Node v274 upstream echo verification";
constexpr int disabled_candidate_source_v272_check_count = 22;
constexpr int disabled_candidate_source_v272_source_check_count = 26;
constexpr int disabled_candidate_check_count = 21;
constexpr int disabled_candidate_candidate_decision_count = 10;
constexpr int disabled_candidate_candidate_ready_decision_count = 4;
constexpr int disabled_candidate_approval_required_decision_count = 6;
constexpr int disabled_candidate_request_field_count = 6;
constexpr int disabled_candidate_response_field_count = 7;
constexpr int disabled_candidate_failure_class_count = 6;
constexpr int disabled_candidate_production_blocker_count = 0;
constexpr int disabled_candidate_warning_count = 2;
constexpr int disabled_candidate_recommendation_count = 2;

std::string format_boundary_codes_json() {
    return "[\"PLAN_DOCUMENT\",\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\","
           "\"DISABLED_SECRET_PROVIDER_STUB\",\"OPERATOR_APPROVAL\",\"ROLLBACK_BOUNDARY\","
           "\"REDACTION_POLICY\",\"EXTERNAL_REQUEST_SIMULATION\",\"SCHEMA_MIGRATION_POLICY\","
           "\"AUDIT_LEDGER_WRITE_POLICY\"]";
}

std::string format_requirement_codes_json() {
    return "[\"REAL_RESOLVER_PRE_IMPLEMENTATION_PLAN_MISSING\","
           "\"CREDENTIAL_HANDLE_BOUNDARY_MISSING\",\"ENDPOINT_HANDLE_BOUNDARY_MISSING\","
           "\"SECRET_PROVIDER_STUB_MISSING\",\"OPERATOR_APPROVAL_BOUNDARY_MISSING\","
           "\"ROLLBACK_BOUNDARY_MISSING\",\"REDACTION_POLICY_MISSING\","
           "\"EXTERNAL_REQUEST_SIMULATION_PLAN_MISSING\",\"SCHEMA_MIGRATION_POLICY_MISSING\","
           "\"AUDIT_LEDGER_WRITE_POLICY_MISSING\"]";
}

std::string format_candidate_ready_boundaries_json() {
    return "[\"PLAN_DOCUMENT\",\"DISABLED_SECRET_PROVIDER_STUB\","
           "\"REDACTION_POLICY\",\"EXTERNAL_REQUEST_SIMULATION\"]";
}

std::string format_approval_required_boundaries_json() {
    return "[\"CREDENTIAL_HANDLE\",\"ENDPOINT_HANDLE\",\"OPERATOR_APPROVAL\","
           "\"ROLLBACK_BOUNDARY\",\"SCHEMA_MIGRATION_POLICY\",\"AUDIT_LEDGER_WRITE_POLICY\"]";
}

std::string format_request_fields_json() {
    return "[\"credentialHandle\",\"endpointHandle\",\"resolverPolicyHandle\","
           "\"operatorIdentity\",\"approvalCorrelationId\",\"manualWindowMarker\"]";
}

std::string format_response_fields_json() {
    return "[\"resolverState\",\"resolvedCredentialValue\",\"rawEndpointUrl\","
           "\"redactionApplied\",\"externalRequestSent\",\"failureClass\",\"auditDigest\"]";
}

std::string format_failure_classes_json() {
    return "[\"disabled-by-config\",\"missing-credential-handle\","
           "\"missing-endpoint-handle\",\"operator-approval-required\","
           "\"manual-window-required\",\"real-runtime-forbidden\"]";
}

std::string format_candidate_decisions_json() {
    return "[{\"code\":\"PLAN_DOCUMENT\",\"requirement_from_v268\":\"REAL_RESOLVER_PRE_IMPLEMENTATION_PLAN_MISSING\","
           "\"disposition\":\"disabled-candidate-ready\",\"owner\":\"release-manager\","
           "\"prohibited_runtime_actions\":[\"implement-real-resolver\",\"open-managed-audit-connection\"]},"
           "{\"code\":\"CREDENTIAL_HANDLE\",\"requirement_from_v268\":\"CREDENTIAL_HANDLE_BOUNDARY_MISSING\","
           "\"disposition\":\"approval-required\",\"owner\":\"security\","
           "\"prohibited_runtime_actions\":[\"read-credential-value\",\"store-credential-value\"]},"
           "{\"code\":\"ENDPOINT_HANDLE\",\"requirement_from_v268\":\"ENDPOINT_HANDLE_BOUNDARY_MISSING\","
           "\"disposition\":\"approval-required\",\"owner\":\"security\","
           "\"prohibited_runtime_actions\":[\"parse-raw-endpoint-url\",\"render-raw-endpoint-url\"]},"
           "{\"code\":\"DISABLED_SECRET_PROVIDER_STUB\",\"requirement_from_v268\":\"SECRET_PROVIDER_STUB_MISSING\","
           "\"disposition\":\"disabled-candidate-ready\",\"owner\":\"node\","
           "\"prohibited_runtime_actions\":[\"instantiate-secret-provider-runtime\",\"load-secret-value\"]},"
           "{\"code\":\"OPERATOR_APPROVAL\",\"requirement_from_v268\":\"OPERATOR_APPROVAL_BOUNDARY_MISSING\","
           "\"disposition\":\"approval-required\",\"owner\":\"operator\","
           "\"prohibited_runtime_actions\":[\"execute-without-operator-marker\",\"auto-approve-operation\"]},"
           "{\"code\":\"ROLLBACK_BOUNDARY\",\"requirement_from_v268\":\"ROLLBACK_BOUNDARY_MISSING\","
           "\"disposition\":\"approval-required\",\"owner\":\"release-manager\","
           "\"prohibited_runtime_actions\":[\"execute-rollback\",\"deploy-resolver-without-abort-marker\"]},"
           "{\"code\":\"REDACTION_POLICY\",\"requirement_from_v268\":\"REDACTION_POLICY_MISSING\","
           "\"disposition\":\"disabled-candidate-ready\",\"owner\":\"security\","
           "\"prohibited_runtime_actions\":[\"log-secret-material\",\"log-raw-endpoint\"]},"
           "{\"code\":\"EXTERNAL_REQUEST_SIMULATION\",\"requirement_from_v268\":\"EXTERNAL_REQUEST_SIMULATION_PLAN_MISSING\","
           "\"disposition\":\"disabled-candidate-ready\",\"owner\":\"node\","
           "\"prohibited_runtime_actions\":[\"send-external-request\",\"connect-managed-audit\"]},"
           "{\"code\":\"SCHEMA_MIGRATION_POLICY\",\"requirement_from_v268\":\"SCHEMA_MIGRATION_POLICY_MISSING\","
           "\"disposition\":\"approval-required\",\"owner\":\"release-manager\","
           "\"prohibited_runtime_actions\":[\"execute-schema-migration\",\"execute-sql\"]},"
           "{\"code\":\"AUDIT_LEDGER_WRITE_POLICY\",\"requirement_from_v268\":\"AUDIT_LEDGER_WRITE_POLICY_MISSING\","
           "\"disposition\":\"approval-required\",\"owner\":\"release-manager\","
           "\"prohibited_runtime_actions\":[\"write-approval-ledger\",\"write-managed-audit-state\"]}]";
}

std::string format_disabled_candidate_checks_json() {
    return "{\"source_node_v272_ready\":true,\"source_node_v272_keeps_read_only_echo_only\":true,"
           "\"source_node_v272_keeps_real_resolver_blocked\":true,"
           "\"source_node_v272_keeps_boundary_alignment\":true,"
           "\"all_candidate_decisions_covered\":true,"
           "\"candidate_ready_boundaries_limited\":true,"
           "\"approval_required_boundaries_preserved\":true,"
           "\"interface_shape_handle_only\":true,\"fake_wiring_review_only\":true,"
           "\"credential_value_still_forbidden\":true,\"raw_endpoint_still_forbidden\":true,"
           "\"secret_provider_runtime_still_disabled\":true,\"resolver_client_still_disabled\":true,"
           "\"external_request_still_blocked\":true,\"schema_migration_still_blocked\":true,"
           "\"ledger_write_still_blocked\":true,\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_disabled_implementation_candidate_review\":true}";
}

std::string format_disabled_candidate_non_participation_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"disabled_implementation_candidate_review_only\":true,\"read_only_candidate_review\":true,"
           "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt_only\":true,"
           "\"ready_for_disabled_resolver_interface_candidate\":true,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"ready_for_production_audit\":false,\"ready_for_production_window\":false,"
           "\"ready_for_production_operations\":false,\"real_resolver_implementation_allowed\":false,"
           "\"credential_resolver_implemented\":false,\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,\"secret_provider_instantiated\":false,"
           "\"secret_provider_runtime_allowed\":false,\"credential_value_read_allowed\":false,"
           "\"credential_value_loaded\":false,\"credential_value_stored\":false,"
           "\"credential_value_included\":false,\"raw_endpoint_url_parse_allowed\":false,"
           "\"raw_endpoint_url_parsed\":false,\"raw_endpoint_url_included\":false,"
           "\"external_request_allowed\":false,\"external_request_sent\":false,"
           "\"connects_managed_audit\":false,\"reads_managed_audit_credential\":false,"
           "\"stores_managed_audit_credential\":false,\"managed_audit_store\":false,"
           "\"managed_audit_storage_backend\":false,\"sandbox_audit_storage_backend\":false,"
           "\"storage_write_allowed\":false,\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,\"runtime_write_observed\":false,"
           "\"approval_ledger_write_allowed\":false,\"approval_ledger_written\":false,"
           "\"approval_ledger_write_executed\":false,\"managed_audit_write_executed\":false,"
           "\"production_record_written\":false,\"schema_migration_allowed\":false,"
           "\"schema_migration_executed\":false,\"schema_rehearsal_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"node_auto_start_allowed\":false,"
           "\"java_auto_start_allowed\":false,\"mini_kv_auto_start_allowed\":false,"
           "\"automatic_upstream_start_allowed\":false,\"automatic_upstream_start\":false,"
           "\"audit_authoritative\":false,\"order_authoritative\":false";
}

} // namespace

std::string credential_resolver_disabled_implementation_candidate_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{disabled_candidate_receipt_release_version}},
        {std::string{disabled_candidate_receipt_artifact_path_hint}},
        {std::string{disabled_candidate_source_review}},
        {std::string{disabled_candidate_profile_version}},
        {std::string{disabled_candidate_route_path}},
        {std::string{disabled_candidate_review_state}},
        {std::string{disabled_candidate_source_v272_digest}},
        {std::string{disabled_candidate_candidate_version}},
        {std::string{disabled_candidate_candidate_mode}},
        {std::string{disabled_candidate_candidate_digest}},
        {std::to_string(disabled_candidate_check_count)},
        {std::to_string(disabled_candidate_candidate_decision_count)},
        {std::to_string(disabled_candidate_candidate_ready_decision_count)},
        {std::to_string(disabled_candidate_approval_required_decision_count)},
        {std::to_string(disabled_candidate_request_field_count)},
        {std::to_string(disabled_candidate_response_field_count)},
        {std::to_string(disabled_candidate_failure_class_count)},
        {std::to_string(disabled_candidate_production_blocker_count)},
        {"disabled-interface-and-fake-wiring-review-only"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest(
        "mini-kv-credential-resolver-disabled-implementation-candidate-non-participation",
        parts);
}

std::string format_credential_resolver_disabled_implementation_candidate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    return "{\"receipt_version\":\"mini-kv-credential-resolver-disabled-implementation-candidate-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(disabled_candidate_receipt_fixture_path) +
           ",\"source_review\":" + field_string(disabled_candidate_source_review) +
           ",\"source_node_v272\":" + field_string(disabled_candidate_source_v272) +
           ",\"consumer_hint\":" + field_string(disabled_candidate_receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(disabled_candidate_receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(disabled_candidate_receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(disabled_candidate_runtime_role) +
           ",\"source_profile_version\":" + field_string(disabled_candidate_profile_version) +
           ",\"source_route_path\":" + field_string(disabled_candidate_route_path) +
           ",\"source_review_state\":" + field_string(disabled_candidate_review_state) +
           ",\"source_ready_for_disabled_implementation_candidate_review\":true"
           ",\"source_disabled_implementation_candidate_review_only\":true"
           ",\"source_read_only_candidate_review\":true"
           ",\"source_ready_for_disabled_resolver_interface_candidate\":true"
           ",\"source_ready_for_managed_audit_sandbox_adapter_connection\":false"
           ",\"source_ready_for_production_audit\":false"
           ",\"source_ready_for_production_window\":false"
           ",\"source_ready_for_production_operations\":false"
           ",\"source_real_resolver_implementation_allowed\":false"
           ",\"source_execution_allowed\":false"
           ",\"source_connects_managed_audit\":false"
           ",\"source_reads_managed_audit_credential\":false"
           ",\"source_stores_managed_audit_credential\":false"
           ",\"source_credential_value_read\":false"
           ",\"source_raw_endpoint_url_parsed\":false"
           ",\"source_external_request_sent\":false"
           ",\"source_secret_provider_instantiated\":false"
           ",\"source_resolver_client_instantiated\":false"
           ",\"source_schema_migration_executed\":false"
           ",\"source_approval_ledger_written\":false"
           ",\"source_automatic_upstream_start\":false"
           ",\"source_node_v272_reference\":{\"source_version\":\"Node v272\","
           "\"profile_version\":" + field_string(disabled_candidate_source_v272_profile_version) +
           ",\"verification_state\":" + field_string(disabled_candidate_source_v272_state) +
           ",\"ready_for_plan_intake_upstream_echo_verification\":true"
           ",\"verification_digest\":" + field_string(disabled_candidate_source_v272_digest) +
           ",\"read_only_upstream_echo_verification\":true"
           ",\"plan_intake_echo_verification_only\":true"
           ",\"source_span\":" + field_string(disabled_candidate_source_v272_span) +
           ",\"source_node_v270_ready\":true,\"java_v112_echo_ready\":true,"
           "\"mini_kv_v119_non_participation_ready\":true,"
           "\"plan_intake_state_aligned\":true,\"plan_counts_aligned\":true,"
           "\"boundary_codes_aligned\":true,\"requirement_codes_aligned\":true,"
           "\"plan_intake_versions_aligned\":true,\"credential_boundary_aligned\":true,"
           "\"raw_endpoint_boundary_aligned\":true,\"resolver_boundary_aligned\":true,"
           "\"connection_boundary_aligned\":true,\"write_boundary_aligned\":true,"
           "\"auto_start_boundary_aligned\":true,\"check_count\":" +
           std::to_string(disabled_candidate_source_v272_check_count) +
           ",\"passed_check_count\":" + std::to_string(disabled_candidate_source_v272_check_count) +
           ",\"source_check_count\":" + std::to_string(disabled_candidate_source_v272_source_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(disabled_candidate_source_v272_source_check_count) +
           ",\"production_blocker_count\":0,\"warning_count\":2,\"recommendation_count\":2,"
           "\"ready_for_credential_resolver_pre_implementation_plan\":true,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"real_resolver_implementation_allowed\":false,\"execution_allowed\":false,"
           "\"connects_managed_audit\":false,\"credential_value_read\":false,"
           "\"raw_endpoint_url_parsed\":false,\"external_request_sent\":false,"
           "\"secret_provider_instantiated\":false,\"resolver_client_instantiated\":false,"
           "\"schema_migration_executed\":false,\"approval_ledger_written\":false,"
           "\"automatic_upstream_start\":false,"
           "\"plan_version\":" + field_string(disabled_candidate_plan_version) +
           ",\"plan_mode\":" + field_string(disabled_candidate_plan_mode) +
           ",\"plan_digest\":" + field_string(disabled_candidate_plan_digest) +
           ",\"intake_digest\":" + field_string(disabled_candidate_intake_digest) +
           ",\"boundary_count\":10,\"defined_boundary_count\":10,\"missing_boundary_count\":0,"
           "\"boundary_codes\":" + format_boundary_codes_json() +
           ",\"requirement_codes\":" + format_requirement_codes_json() + "}"
           ",\"disabled_implementation_candidate\":{\"candidate_version\":" +
           field_string(disabled_candidate_candidate_version) +
           ",\"candidate_mode\":" + field_string(disabled_candidate_candidate_mode) +
           ",\"source_span\":\"Node v272\",\"candidate_digest\":" +
           field_string(disabled_candidate_candidate_digest) +
           ",\"candidate_decision_count\":" +
           std::to_string(disabled_candidate_candidate_decision_count) +
           ",\"candidate_ready_decision_count\":" +
           std::to_string(disabled_candidate_candidate_ready_decision_count) +
           ",\"approval_required_decision_count\":" +
           std::to_string(disabled_candidate_approval_required_decision_count) +
           ",\"candidate_ready_boundary_codes\":" + format_candidate_ready_boundaries_json() +
           ",\"approval_required_boundary_codes\":" + format_approval_required_boundaries_json() +
           ",\"decisions\":" + format_candidate_decisions_json() +
           ",\"interface_shape\":{\"interface_version\":" +
           field_string(disabled_candidate_interface_version) +
           ",\"request_fields\":" + format_request_fields_json() +
           ",\"response_fields\":" + format_response_fields_json() +
           ",\"failure_classes\":" + format_failure_classes_json() +
           ",\"request_field_count\":" + std::to_string(disabled_candidate_request_field_count) +
           ",\"response_field_count\":" + std::to_string(disabled_candidate_response_field_count) +
           ",\"failure_class_count\":" + std::to_string(disabled_candidate_failure_class_count) +
           ",\"handle_only_request\":true,\"includes_credential_value\":false,"
           "\"includes_raw_endpoint_url\":false,\"sends_external_request\":false,"
           "\"instantiates_secret_provider\":false,\"instantiates_resolver_client\":false}"
           ",\"fake_wiring_review\":{\"review_version\":" +
           field_string(disabled_candidate_fake_wiring_review_version) +
           ",\"fake_wiring_review_only\":true,\"fake_runtime_instantiated\":false,"
           "\"real_secret_provider_allowed\":false,\"real_managed_audit_transport_allowed\":false,"
           "\"external_request_allowed\":false,\"cleanup_artifact_count\":0}"
           ",\"disabled_interface_candidate_allowed\":true,\"fake_wiring_review_allowed\":true,"
           "\"real_resolver_implementation_allowed\":false,"
           "\"secret_provider_runtime_allowed\":false,\"credential_value_read_allowed\":false,"
           "\"raw_endpoint_url_parse_allowed\":false,\"external_request_allowed\":false,"
           "\"schema_migration_allowed\":false,\"approval_ledger_write_allowed\":false,"
           "\"automatic_upstream_start_allowed\":false}"
           ",\"checks\":" + format_disabled_candidate_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(disabled_candidate_check_count) +
           ",\"passed_check_count\":" + std::to_string(disabled_candidate_check_count) +
           ",\"source_check_count\":" + std::to_string(disabled_candidate_source_v272_check_count) +
           ",\"source_passed_check_count\":" +
           std::to_string(disabled_candidate_source_v272_check_count) +
           ",\"candidate_decision_count\":" +
           std::to_string(disabled_candidate_candidate_decision_count) +
           ",\"candidate_ready_decision_count\":" +
           std::to_string(disabled_candidate_candidate_ready_decision_count) +
           ",\"approval_required_decision_count\":" +
           std::to_string(disabled_candidate_approval_required_decision_count) +
           ",\"request_field_count\":" + std::to_string(disabled_candidate_request_field_count) +
           ",\"response_field_count\":" + std::to_string(disabled_candidate_response_field_count) +
           ",\"failure_class_count\":" + std::to_string(disabled_candidate_failure_class_count) +
           ",\"production_blocker_count\":" +
           std::to_string(disabled_candidate_production_blocker_count) +
           ",\"warning_count\":" + std::to_string(disabled_candidate_warning_count) +
           ",\"recommendation_count\":" + std::to_string(disabled_candidate_recommendation_count) +
           "}"
           ",\"production_blocker_codes\":[]"
           ",\"warnings\":[{\"code\":\"DISABLED_CANDIDATE_REVIEW_ONLY\",\"severity\":\"warning\"},"
           "{\"code\":\"APPROVAL_BOUNDARIES_REMAIN\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RUN_PARALLEL_JAVA_V113_MINI_KV_V120\","
           "\"severity\":\"recommendation\"},{\"code\":\"VERIFY_WITH_NODE_V274_AFTER_UPSTREAM_ECHO\","
           "\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"disabled_implementation_candidate_review_json\":" +
           field_string(disabled_candidate_route_path) +
           ",\"disabled_implementation_candidate_review_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-implementation-candidate-review?format=markdown\","
           "\"source_node_v272_json\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake-upstream-echo-verification\","
           "\"source_node_v272_markdown\":\"/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-pre-implementation-plan-intake-upstream-echo-verification?format=markdown\","
           "\"active_plan\":" + field_string(disabled_candidate_active_plan) +
           ",\"recommended_parallel_java_v113\":\"Java v113 read-only echo for Node v273 disabled implementation candidate review\","
           "\"recommended_parallel_mini_kv_v120\":\"mini-kv v120 non-participation receipt for Node v273 disabled implementation candidate review\"}"
           ",\"next_required_echo_versions\":[\"Java v113\",\"mini-kv v120\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(
               credential_resolver_disabled_implementation_candidate_non_participation_receipt_digest(
                   read_commands)) +
           "," + format_disabled_candidate_non_participation_flags_json() +
           ",\"boundary\":" + field_string(disabled_candidate_boundary) +
           ",\"node_action\":" + field_string(disabled_candidate_node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
