#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_receipt_json_builder.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using runtime_evidence::DigestPart;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

constexpr std::string_view plan_receipt_consumer =
    "Node v284 credential resolver implementation plan upstream echo verification";
constexpr std::string_view plan_receipt_fixture_path =
    "fixtures/release/credential-resolver-implementation-plan-non-participation-receipt.json";
constexpr std::string_view plan_receipt_release_version = "v126";
constexpr std::string_view plan_receipt_artifact_path_hint = "d/126/";
constexpr std::string_view plan_source = "Node v283 managed audit resolver implementation plan draft";
constexpr std::string_view plan_source_verification =
    "Node v282 approval-required implementation readiness upstream echo verification";
constexpr std::string_view plan_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-implementation-plan-draft.v1";
constexpr std::string_view plan_route_path =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-implementation-plan-draft";
constexpr std::string_view plan_source_verification_route_path =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-upstream-"
    "echo-verification";
constexpr std::string_view plan_state = "credential-resolver-implementation-plan-draft-ready";
constexpr std::string_view plan_version = "node-v283-credential-resolver-implementation-plan-draft.v1";
constexpr std::string_view plan_mode = "implementation-plan-draft-only";
constexpr std::string_view plan_review_mode = "node-v283-implementation-plan-draft-only";
constexpr std::string_view plan_digest = "152d4261b3c020159da2aebc2a5ef484dcb8f5381f90bf5f29cc21deb9f80edb";
constexpr std::string_view plan_review_digest = "f96ffbad4574e400216b0c6615f4c37fe979f9ede9797a039a5e55888d097a55";
constexpr std::string_view plan_source_v282_digest = "eab97ee07d89be2a915f42cd6f340aea36b79d0ad332e777826f91e760f7a511";
constexpr std::string_view plan_active_plan = "docs/plans2/v282-post-upstream-echo-verification-roadmap.md";
constexpr std::string_view plan_runtime_role = "runtime evidence provider only";
constexpr std::string_view plan_boundary =
    "credential resolver implementation plan non-participation receipt only; mini-kv echoes Node v283 implementation "
    "plan draft and planDigest, does not implement or invoke a resolver, does not instantiate a resolver client or "
    "secret provider, does not read, load, store, or include credential values, does not parse or render raw endpoint "
    "URLs, does not send external requests, does not connect managed audit, does not act as a managed audit storage "
    "backend, does not write storage or approval ledger records, does not execute schema migration or "
    "LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view plan_node_action = "verify mini-kv v126 resolver implementation plan non-participation with "
                                              "Java v121 before Node v284 upstream echo verification";

constexpr int plan_check_count = 28;
constexpr int plan_source_check_count = 23;
constexpr int plan_interface_boundary_count = 7;
constexpr int plan_required_artifact_count = 21;
constexpr int plan_prohibited_action_count = 21;
constexpr int plan_java_echo_requirement_count = 4;
constexpr int plan_mini_kv_receipt_requirement_count = 4;
constexpr int plan_production_blocker_count = 0;
constexpr int plan_warning_count = 2;
constexpr int plan_recommendation_count = 2;

std::string format_interface_boundary_codes_json() {
    return "[\"CONFIG_HANDLE_CONTRACT\",\"CREDENTIAL_HANDLE_CONTRACT\","
           "\"ENDPOINT_HANDLE_CONTRACT\",\"APPROVAL_ARTIFACT_CONTRACT\","
           "\"FAILURE_TAXONOMY_CONTRACT\",\"ROLLBACK_GUARD_CONTRACT\","
           "\"TEST_ONLY_FAKE_HARNESS_CONTRACT\"]";
}

std::string format_required_artifacts_json() {
    return "[\"config-handle-review-id\",\"resolver-policy-handle-review-id\","
           "\"config-redaction-contract\",\"credential-handle-review-id\","
           "\"credential-value-redaction-contract\",\"operator-visible-secret-value-prohibition\","
           "\"endpoint-handle-review-id\",\"allowlist-review-status\","
           "\"raw-endpoint-redaction-contract\",\"operator-identity-binding\","
           "\"approval-correlation-marker\",\"manual-window-open-marker\","
           "\"failure-taxonomy-id\",\"operator-visible-failure-map\","
           "\"retry-policy-review-id\",\"rollback-abort-marker\","
           "\"restore-point-review-id\",\"manual-rollback-runbook-reference\","
           "\"test-only-fake-harness-plan-id\",\"fake-harness-disabled-toggle\","
           "\"fake-harness-side-effect-contract\"]";
}

std::string format_prohibited_actions_json() {
    return "[\"read-secret-env-value\",\"render-secret-env-value\","
           "\"instantiate-runtime-client\",\"read-credential-value\","
           "\"store-credential-value\",\"render-credential-value\","
           "\"parse-raw-endpoint-url\",\"render-raw-endpoint-url\","
           "\"dial-managed-audit-endpoint\",\"auto-approve-operation\","
           "\"execute-without-operator-marker\",\"write-approval-ledger\","
           "\"send-external-request\",\"connect-managed-audit\","
           "\"mask-unclassified-error\",\"execute-rollback\","
           "\"deploy-resolver-without-abort-marker\",\"auto-start-upstream\","
           "\"instantiate-real-secret-provider\",\"resolve-real-credential\","
           "\"send-real-http-request\"]";
}

std::string format_interface_boundaries_json() {
    return "[{\"code\":\"CONFIG_HANDLE_CONTRACT\",\"source_boundary\":\"PLAN_DOCUMENT\","
           "\"owner\":\"node\",\"status\":\"drafted-for-upstream-echo\","
           "\"allowed_inputs\":[\"ORDEROPS_MANAGED_AUDIT_RESOLVER_CONFIG_HANDLE\","
           "\"ORDEROPS_MANAGED_AUDIT_RESOLVER_POLICY_HANDLE\"],"
           "\"allowed_outputs\":[\"configHandle\",\"policyHandle\",\"reviewStatus\"],"
           "\"prohibited_actions\":[\"read-secret-env-value\",\"render-secret-env-value\","
           "\"instantiate-runtime-client\"],"
           "\"required_artifacts\":[\"config-handle-review-id\","
           "\"resolver-policy-handle-review-id\",\"config-redaction-contract\"],"
           "\"mini_kv_position\":\"handle names may be echoed as evidence only; mini-kv creates no runtime client\"},"
           "{\"code\":\"CREDENTIAL_HANDLE_CONTRACT\",\"source_boundary\":\"CREDENTIAL_HANDLE\","
           "\"owner\":\"security\",\"status\":\"drafted-for-upstream-echo\","
           "\"allowed_inputs\":[\"credentialHandle\",\"credentialReviewStatus\"],"
           "\"allowed_outputs\":[\"credentialHandle\",\"credentialReviewStatus\","
           "\"credentialValuePresent=false\"],"
           "\"prohibited_actions\":[\"read-credential-value\",\"store-credential-value\","
           "\"render-credential-value\"],"
           "\"required_artifacts\":[\"credential-handle-review-id\","
           "\"credential-value-redaction-contract\",\"operator-visible-secret-value-prohibition\"],"
           "\"mini_kv_position\":\"credential handles may be referenced; credential values are never read or stored\"},"
           "{\"code\":\"ENDPOINT_HANDLE_CONTRACT\",\"source_boundary\":\"ENDPOINT_HANDLE\","
           "\"owner\":\"security\",\"status\":\"drafted-for-upstream-echo\","
           "\"allowed_inputs\":[\"endpointHandle\",\"allowlistReviewStatus\"],"
           "\"allowed_outputs\":[\"endpointHandle\",\"allowlistReviewStatus\","
           "\"rawEndpointUrlPresent=false\"],"
           "\"prohibited_actions\":[\"parse-raw-endpoint-url\",\"render-raw-endpoint-url\","
           "\"dial-managed-audit-endpoint\"],"
           "\"required_artifacts\":[\"endpoint-handle-review-id\","
           "\"allowlist-review-status\",\"raw-endpoint-redaction-contract\"],"
           "\"mini_kv_position\":\"endpoint handles may be echoed; raw endpoint URLs are never parsed or rendered\"},"
           "{\"code\":\"APPROVAL_ARTIFACT_CONTRACT\",\"source_boundary\":\"OPERATOR_APPROVAL\","
           "\"owner\":\"operator\",\"status\":\"drafted-for-upstream-echo\","
           "\"allowed_inputs\":[\"operatorIdentityBinding\",\"approvalCorrelationId\","
           "\"manualWindowMarker\"],"
           "\"allowed_outputs\":[\"approvalArtifactDigest\",\"approvalState\","
           "\"manualWindowStatus\"],"
           "\"prohibited_actions\":[\"auto-approve-operation\","
           "\"execute-without-operator-marker\",\"write-approval-ledger\"],"
           "\"required_artifacts\":[\"operator-identity-binding\","
           "\"approval-correlation-marker\",\"manual-window-open-marker\"],"
           "\"mini_kv_position\":\"approval artifacts may be echoed; mini-kv writes no ledger state\"},"
           "{\"code\":\"FAILURE_TAXONOMY_CONTRACT\","
           "\"source_boundary\":\"EXTERNAL_REQUEST_SIMULATION\","
           "\"owner\":\"node\",\"status\":\"drafted-for-upstream-echo\","
           "\"allowed_inputs\":[\"simulatedFailureClass\",\"dryRunAdapterResult\","
           "\"blockedReason\"],"
           "\"allowed_outputs\":[\"failureClass\",\"operatorVisibleReason\","
           "\"retryDisposition\"],"
           "\"prohibited_actions\":[\"send-external-request\",\"connect-managed-audit\","
           "\"mask-unclassified-error\"],"
           "\"required_artifacts\":[\"failure-taxonomy-id\","
           "\"operator-visible-failure-map\",\"retry-policy-review-id\"],"
           "\"mini_kv_position\":\"failure classes remain simulated evidence; no network request is sent\"},"
           "{\"code\":\"ROLLBACK_GUARD_CONTRACT\",\"source_boundary\":\"ROLLBACK_BOUNDARY\","
           "\"owner\":\"release-manager\",\"status\":\"drafted-for-upstream-echo\","
           "\"allowed_inputs\":[\"rollbackAbortMarker\",\"restorePointReviewId\","
           "\"manualRollbackRunbookReference\"],"
           "\"allowed_outputs\":[\"rollbackGuardState\",\"abortRequired=true\","
           "\"executionAllowed=false\"],"
           "\"prohibited_actions\":[\"execute-rollback\","
           "\"deploy-resolver-without-abort-marker\",\"auto-start-upstream\"],"
           "\"required_artifacts\":[\"rollback-abort-marker\","
           "\"restore-point-review-id\",\"manual-rollback-runbook-reference\"],"
           "\"mini_kv_position\":\"rollback guard data is evidence only; no LOAD/RESTORE/COMPACT is executed\"},"
           "{\"code\":\"TEST_ONLY_FAKE_HARNESS_CONTRACT\","
           "\"source_boundary\":\"DISABLED_SECRET_PROVIDER_STUB\","
           "\"owner\":\"node\",\"status\":\"drafted-for-upstream-echo\","
           "\"allowed_inputs\":[\"fakeCredentialHandle\",\"fakeEndpointHandle\","
           "\"testOnlyHarnessToggle=false\"],"
           "\"allowed_outputs\":[\"fakeHarnessPlan\",\"sideEffectBoundary\","
           "\"runtimeToggleState\"],"
           "\"prohibited_actions\":[\"instantiate-real-secret-provider\","
           "\"resolve-real-credential\",\"send-real-http-request\"],"
           "\"required_artifacts\":[\"test-only-fake-harness-plan-id\","
           "\"fake-harness-disabled-toggle\",\"fake-harness-side-effect-contract\"],"
           "\"mini_kv_position\":\"fake harness planning is deferred; mini-kv starts no resolver or provider\"}]";
}

std::string format_mini_kv_requirements_json() {
    return "[{\"id\":\"mini-kv-v126-consumes-node-v283-plan\","
           "\"requirement\":\"mini-kv v126 must identify Node v283 planDigest and remain non-participating.\","
           "\"must_remain_read_only\":true,\"must_not_connect_managed_audit\":true,"
           "\"must_not_read_credential_value\":true,\"must_not_parse_raw_endpoint_url\":true,"
           "\"must_not_write_ledger_or_state\":true},"
           "{\"id\":\"mini-kv-v126-no-storage-backend\","
           "\"requirement\":\"mini-kv v126 must prove it is not a managed audit storage backend and not authoritative "
           "for audit/order state.\","
           "\"must_remain_read_only\":true,\"must_not_connect_managed_audit\":true,"
           "\"must_not_read_credential_value\":true,\"must_not_parse_raw_endpoint_url\":true,"
           "\"must_not_write_ledger_or_state\":true},"
           "{\"id\":\"mini-kv-v126-no-secret-or-endpoint\","
           "\"requirement\":\"mini-kv v126 must prove no credential resolver, no secret provider, and no raw endpoint "
           "parser.\","
           "\"must_remain_read_only\":true,\"must_not_connect_managed_audit\":true,"
           "\"must_not_read_credential_value\":true,\"must_not_parse_raw_endpoint_url\":true,"
           "\"must_not_write_ledger_or_state\":true},"
           "{\"id\":\"mini-kv-v126-no-write-command\","
           "\"requirement\":\"mini-kv v126 must keep write/admin commands out of this plan echo receipt.\","
           "\"must_remain_read_only\":true,\"must_not_connect_managed_audit\":true,"
           "\"must_not_read_credential_value\":true,\"must_not_parse_raw_endpoint_url\":true,"
           "\"must_not_write_ledger_or_state\":true}]";
}

std::string format_java_requirements_json() {
    return "[{\"id\":\"java-v121-consumes-node-v283-plan\","
           "\"expected_version\":\"Java v121\",\"must_remain_read_only\":true},"
           "{\"id\":\"java-v121-approval-artifact-boundary\","
           "\"expected_version\":\"Java v121\",\"must_remain_read_only\":true},"
           "{\"id\":\"java-v121-schema-migration-boundary\","
           "\"expected_version\":\"Java v121\",\"must_remain_read_only\":true},"
           "{\"id\":\"java-v121-failure-taxonomy-echo\","
           "\"expected_version\":\"Java v121\",\"must_remain_read_only\":true}]";
}

std::string format_plan_checks_json() {
    return "{\"source_node_v282_ready\":true,\"source_node_v282_allows_plan_draft\":true,"
           "\"source_node_v282_keeps_implementation_blocked\":true,"
           "\"config_handle_contract_defined\":true,"
           "\"credential_handle_contract_defined\":true,"
           "\"endpoint_handle_contract_defined\":true,"
           "\"approval_artifact_contract_defined\":true,"
           "\"failure_taxonomy_contract_defined\":true,"
           "\"rollback_guard_contract_defined\":true,"
           "\"test_only_fake_harness_contract_defined\":true,"
           "\"all_interface_boundaries_defined\":true,"
           "\"all_required_artifacts_named\":true,"
           "\"java_v121_echo_requirements_defined\":true,"
           "\"mini_kv_v126_receipt_requirements_defined\":true,"
           "\"real_resolver_implementation_still_blocked\":true,"
           "\"test_only_fake_harness_still_deferred\":true,"
           "\"credential_value_still_forbidden\":true,"
           "\"raw_endpoint_still_forbidden\":true,"
           "\"external_request_still_forbidden\":true,"
           "\"secret_provider_runtime_still_disabled\":true,"
           "\"resolver_client_still_disabled\":true,"
           "\"schema_migration_still_review_only\":true,"
           "\"approval_ledger_write_still_review_only\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_managed_audit_manual_sandbox_connection_credential_resolver_implementation_plan_draft\":true}";
}

void append_non_participation_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back({"implementation_plan_draft_only", json_bool(true)});
    fields.push_back({"read_only_plan_draft", json_bool(true)});
    fields.push_back({"credential_resolver_implementation_plan_non_participation_receipt_only", json_bool(true)});
    fields.push_back({"consumes_node_v283_plan", json_bool(true)});
    fields.push_back({"ready_for_node_v284_upstream_echo_verification", json_bool(true)});
    fields.push_back({"ready_for_java_v121_mini_kv_v126_echo", json_bool(true)});
    fields.push_back({"ready_for_managed_audit_resolver_implementation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)});
    fields.push_back({"ready_for_production_audit", json_bool(false)});
    fields.push_back({"ready_for_production_window", json_bool(false)});
    fields.push_back({"ready_for_production_operations", json_bool(false)});
    fields.push_back({"real_resolver_implementation_allowed", json_bool(false)});
    fields.push_back({"test_only_fake_harness_allowed", json_bool(false)});
    fields.push_back({"credential_resolver_implemented", json_bool(false)});
    fields.push_back({"credential_resolver_invoked", json_bool(false)});
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_runtime_allowed", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_read", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_stored", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parse_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_render_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_included", json_bool(false)});
    fields.push_back({"external_request_allowed", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    fields.push_back({"connects_managed_audit", json_bool(false)});
    fields.push_back({"reads_managed_audit_credential", json_bool(false)});
    fields.push_back({"stores_managed_audit_credential", json_bool(false)});
    fields.push_back({"managed_audit_store", json_bool(false)});
    fields.push_back({"managed_audit_storage_backend", json_bool(false)});
    fields.push_back({"sandbox_audit_storage_backend", json_bool(false)});
    fields.push_back({"storage_write_allowed", json_bool(false)});
    fields.push_back({"write_commands_executed", json_bool(false)});
    fields.push_back({"admin_commands_executed", json_bool(false)});
    fields.push_back({"runtime_write_observed", json_bool(false)});
    fields.push_back({"approval_ledger_write_allowed", json_bool(false)});
    fields.push_back({"approval_ledger_written", json_bool(false)});
    fields.push_back({"approval_ledger_write_executed", json_bool(false)});
    fields.push_back({"managed_audit_write_executed", json_bool(false)});
    fields.push_back({"production_record_written", json_bool(false)});
    fields.push_back({"schema_migration_allowed", json_bool(false)});
    fields.push_back({"schema_migration_executed", json_bool(false)});
    fields.push_back({"schema_rehearsal_execution_allowed", json_bool(false)});
    fields.push_back({"restore_execution_allowed", json_bool(false)});
    fields.push_back({"load_restore_compact_executed", json_bool(false)});
    fields.push_back({"setnxex_execution_allowed", json_bool(false)});
    fields.push_back({"node_auto_start_allowed", json_bool(false)});
    fields.push_back({"java_auto_start_allowed", json_bool(false)});
    fields.push_back({"mini_kv_auto_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start", json_bool(false)});
    fields.push_back({"audit_authoritative", json_bool(false)});
    fields.push_back({"order_authoritative", json_bool(false)});
}

} // namespace

std::string credential_resolver_implementation_plan_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{plan_receipt_release_version}},
        {std::string{plan_receipt_artifact_path_hint}},
        {std::string{plan_source}},
        {std::string{plan_profile_version}},
        {std::string{plan_state}},
        {std::string{plan_version}},
        {std::string{plan_mode}},
        {std::string{plan_digest}},
        {std::string{plan_review_digest}},
        {std::string{plan_source_v282_digest}},
        {std::to_string(plan_check_count)},
        {std::to_string(plan_source_check_count)},
        {std::to_string(plan_interface_boundary_count)},
        {std::to_string(plan_required_artifact_count)},
        {std::to_string(plan_prohibited_action_count)},
        {std::to_string(plan_java_echo_requirement_count)},
        {std::to_string(plan_mini_kv_receipt_requirement_count)},
        {"CONFIG_HANDLE_CONTRACT"},
        {"CREDENTIAL_HANDLE_CONTRACT"},
        {"ENDPOINT_HANDLE_CONTRACT"},
        {"APPROVAL_ARTIFACT_CONTRACT"},
        {"FAILURE_TAXONOMY_CONTRACT"},
        {"ROLLBACK_GUARD_CONTRACT"},
        {"TEST_ONLY_FAKE_HARNESS_CONTRACT"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return runtime_evidence::digest("mini-kv-credential-resolver-implementation-plan-non-participation", parts);
}

std::string format_credential_resolver_implementation_plan_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v282_reference = json_object({
        {"source_version", json_string("Node v282")},
        {"verification_state",
         json_string("credential-resolver-approval-required-implementation-readiness-upstream-echo-verification-"
                     "ready")},
        {"ready_for_approval_required_implementation_readiness_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v283_implementation_plan_draft", json_bool(true)},
        {"verification_digest", json_string(plan_source_v282_digest)},
        {"source_span", json_string("Node v281 + Java v116 + mini-kv v122")},
        {"source_node_v281_ready", json_bool(true)},
        {"java_v116_echo_ready", json_bool(true)},
        {"mini_kv_v122_non_participation_ready", json_bool(true)},
        {"boundary_readiness_aligned", json_bool(true)},
        {"required_artifacts_aligned", json_bool(true)},
        {"readiness_counts_aligned", json_bool(true)},
        {"side_effect_boundaries_aligned", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
        {"check_count", json_integer(plan_source_check_count)},
        {"passed_check_count", json_integer(plan_source_check_count)},
        {"boundary_count", json_integer(6)},
        {"required_artifact_count", json_integer(18)},
        {"ready_for_managed_audit_resolver_implementation", json_bool(false)},
        {"real_resolver_implementation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"reads_managed_audit_credential", json_bool(false)},
        {"stores_managed_audit_credential", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });
    const auto implementation_plan = json_object({
        {"plan_version", json_string(plan_version)},
        {"plan_mode", json_string(plan_mode)},
        {"source_span", json_string("Node v282")},
        {"plan_digest", json_string(plan_digest)},
        {"interface_boundary_count", json_integer(plan_interface_boundary_count)},
        {"required_artifact_count", json_integer(plan_required_artifact_count)},
        {"prohibited_action_count", json_integer(plan_prohibited_action_count)},
        {"interface_boundary_codes", format_interface_boundary_codes_json()},
        {"required_artifacts", format_required_artifacts_json()},
        {"prohibited_actions", format_prohibited_actions_json()},
        {"interface_boundaries", format_interface_boundaries_json()},
        {"java_v121_echo_requirements", format_java_requirements_json()},
        {"mini_kv_v126_receipt_requirements", format_mini_kv_requirements_json()},
        {"all_interface_boundaries_defined", json_bool(true)},
        {"all_required_artifacts_named", json_bool(true)},
        {"real_resolver_implementation_allowed", json_bool(false)},
        {"test_only_fake_harness_allowed", json_bool(false)},
        {"secret_provider_runtime_allowed", json_bool(false)},
        {"credential_value_read_allowed", json_bool(false)},
        {"raw_endpoint_url_parse_allowed", json_bool(false)},
        {"raw_endpoint_url_render_allowed", json_bool(false)},
        {"external_request_allowed", json_bool(false)},
        {"schema_migration_allowed", json_bool(false)},
        {"approval_ledger_write_allowed", json_bool(false)},
        {"automatic_upstream_start_allowed", json_bool(false)},
    });
    const auto implementation_plan_review = json_object({
        {"review_mode", json_string(plan_review_mode)},
        {"consumed_node_version", json_string("Node v282")},
        {"next_java_echo_version", json_string("Java v121")},
        {"next_mini_kv_receipt_version", json_string("mini-kv v126")},
        {"next_node_verification_version", json_string("Node v284")},
        {"fake_harness_deferred_until", json_string("Node v285")},
        {"interface_boundary_count", json_integer(plan_interface_boundary_count)},
        {"required_artifact_count", json_integer(plan_required_artifact_count)},
        {"prohibited_action_count", json_integer(plan_prohibited_action_count)},
        {"java_echo_requirement_count", json_integer(plan_java_echo_requirement_count)},
        {"mini_kv_receipt_requirement_count", json_integer(plan_mini_kv_receipt_requirement_count)},
        {"source_node_v282_ready", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
        {"ready_for_java_v121_mini_kv_v126_echo", json_bool(true)},
        {"review_digest", json_string(plan_review_digest)},
    });
    const auto summary = json_object({
        {"check_count", json_integer(plan_check_count)},
        {"passed_check_count", json_integer(plan_check_count)},
        {"source_check_count", json_integer(plan_source_check_count)},
        {"source_passed_check_count", json_integer(plan_source_check_count)},
        {"interface_boundary_count", json_integer(plan_interface_boundary_count)},
        {"required_artifact_count", json_integer(plan_required_artifact_count)},
        {"prohibited_action_count", json_integer(plan_prohibited_action_count)},
        {"java_echo_requirement_count", json_integer(plan_java_echo_requirement_count)},
        {"mini_kv_receipt_requirement_count", json_integer(plan_mini_kv_receipt_requirement_count)},
        {"production_blocker_count", json_integer(plan_production_blocker_count)},
        {"warning_count", json_integer(plan_warning_count)},
        {"recommendation_count", json_integer(plan_recommendation_count)},
    });
    const auto warnings = json_array({
        json_object({{"code", json_string("IMPLEMENTATION_STILL_BLOCKED")}, {"severity", json_string("warning")}}),
        json_object({{"code", json_string("UPSTREAM_ECHO_REQUIRED")}, {"severity", json_string("warning")}}),
    });
    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_PARALLEL_JAVA_V121_MINI_KV_V126")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("VERIFY_WITH_NODE_V284_BEFORE_FAKE_HARNESS")},
                     {"severity", json_string("recommendation")}}),
    });
    const auto evidence_endpoints = json_object({
        {"implementation_plan_draft_json", json_string(plan_route_path)},
        {"implementation_plan_draft_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-implementation-"
                     "plan-draft?format=markdown")},
        {"source_node_v282_json", json_string(plan_source_verification_route_path)},
        {"source_node_v282_markdown",
         json_string("/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-approval-required-"
                     "implementation-readiness-upstream-echo-verification?format=markdown")},
        {"active_plan", json_string(plan_active_plan)},
        {"next_recommended_parallel_java_v121", json_string("Java v121 resolver implementation plan echo")},
        {"next_recommended_parallel_mini_kv_v126",
         json_string("mini-kv v126 resolver implementation plan non-participation receipt")},
        {"next_node_verification_version", json_string("Node v284")},
        {"future_fake_harness_precheck_version", json_string("Node v285")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string("mini-kv-credential-resolver-implementation-plan-non-participation-"
                                        "receipt.v1")},
        {"receipt_fixture_path", json_string(plan_receipt_fixture_path)},
        {"source_plan", json_string(plan_source)},
        {"source_verification", json_string(plan_source_verification)},
        {"consumer_hint", json_string(plan_receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(plan_receipt_release_version)},
        {"current_artifact_path_hint", json_string(plan_receipt_artifact_path_hint)},
        {"previous_mini_kv_release_version", json_string("v122")},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(plan_runtime_role)},
        {"source_profile_version", json_string(plan_profile_version)},
        {"source_route_path", json_string(plan_route_path)},
        {"source_verification_route_path", json_string(plan_source_verification_route_path)},
        {"source_plan_state", json_string(plan_state)},
        {"source_ready_for_implementation_plan_draft", json_bool(true)},
        {"source_plan_draft_only", json_bool(true)},
        {"source_read_only_plan_draft", json_bool(true)},
        {"source_implementation_plan_draft_only", json_bool(true)},
        {"source_consumes_node_v282_approval_required_implementation_readiness_echo_verification", json_bool(true)},
        {"source_ready_for_java_v121_mini_kv_v126_echo", json_bool(true)},
        {"source_ready_for_managed_audit_resolver_implementation", json_bool(false)},
        {"source_ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)},
        {"source_ready_for_production_audit", json_bool(false)},
        {"source_ready_for_production_window", json_bool(false)},
        {"source_ready_for_production_operations", json_bool(false)},
        {"source_real_resolver_implementation_allowed", json_bool(false)},
        {"source_test_only_fake_harness_allowed", json_bool(false)},
        {"source_execution_allowed", json_bool(false)},
        {"source_connects_managed_audit", json_bool(false)},
        {"source_reads_managed_audit_credential", json_bool(false)},
        {"source_stores_managed_audit_credential", json_bool(false)},
        {"source_credential_value_read", json_bool(false)},
        {"source_raw_endpoint_url_parsed", json_bool(false)},
        {"source_raw_endpoint_url_rendered", json_bool(false)},
        {"source_external_request_sent", json_bool(false)},
        {"source_secret_provider_instantiated", json_bool(false)},
        {"source_resolver_client_instantiated", json_bool(false)},
        {"source_schema_migration_executed", json_bool(false)},
        {"source_approval_ledger_written", json_bool(false)},
        {"source_automatic_upstream_start", json_bool(false)},
        {"source_node_v282_reference", source_node_v282_reference},
        {"implementation_plan", implementation_plan},
        {"implementation_plan_review", implementation_plan_review},
        {"checks", format_plan_checks_json()},
        {"summary", summary},
        {"production_blocker_codes", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_echo_versions", json_array({json_string("Java v121"), json_string("mini-kv v126")})},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_implementation_plan_non_participation_receipt_digest(read_commands))},
    };
    append_non_participation_fields(fields);
    fields.push_back({"boundary", json_string(plan_boundary)});
    fields.push_back({"node_action", json_string(plan_node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
