#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/version.hpp"

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::append_closed_runtime_shell_boundary_fields;
using credential_resolver_detail::DigestPart;
using credential_resolver_detail::receipt_digest;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

constexpr std::string_view receipt_version =
    "mini-kv-credential-resolver-human-approval-artifact-review-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer = "Node v309 human approval artifact review upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-human-approval-artifact-review-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v136";
constexpr std::string_view receipt_artifact_path_hint = "d/136/";
constexpr std::string_view source_review_packet = "Node v308 human approval artifact review packet";
constexpr std::string_view source_upstream_echo = "Node v307 approval prerequisite artifact upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v308_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-packet.v1";
constexpr std::string_view node_v308_review_packet_state = "human-approval-artifact-review-packet-ready";
constexpr std::string_view node_v308_packet_name = "managed-audit-runtime-shell-human-approval-artifact-review-packet";
constexpr std::string_view node_v308_packet_version = "human-approval-artifact-review-packet.v1";
constexpr std::string_view node_v308_review_mode = "human-approval-artifact-review-packet-contract-only";
constexpr std::string_view node_v308_source_span = "Node v307";
constexpr std::string_view node_v308_packet_digest = "b0dda954c509337c96a645c177be521d0a200d8f8e6d52081ed8c0df9a43ccf3";
constexpr std::string_view node_v308_necessity_consumer = "Java v143 + mini-kv v136, then Node v309";
constexpr std::string_view node_v308_blocker =
    "v307 proves the approval prerequisite artifact contract is aligned across Node, Java, and mini-kv, but it does "
    "not define the human review packet shape for an operator-submitted artifact.";
constexpr std::string_view node_v308_reuse =
    "v307 verifies the upstream echo chain for Node v306; it cannot replace the smaller human review packet contract, "
    "missing-field checks, or runtime-shell invocation rejection.";
constexpr std::string_view node_v308_stop_condition =
    "Stop if the next step asks for credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP, "
    "runtime shell implementation or invocation, ledger writes, schema migration, mini-kv writes/admin commands, or "
    "automatic upstream start.";

constexpr std::string_view node_v307_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "approval-prerequisite-artifact-upstream-echo-verification.v1";
constexpr std::string_view node_v307_verification_state =
    "approval-prerequisite-artifact-upstream-echo-verification-ready";
constexpr std::string_view node_v307_verification_digest =
    "6c9e5563358402eb2a8d0ac9dc8235db83aab4d0f5634b53c7ff4da9a6b3c362";
constexpr std::string_view node_v307_source_span = "Node v306 + Java v142 + mini-kv v135";
constexpr std::string_view node_v306_artifact_digest =
    "72f3e90606e40a978611fa4b8596c76c3ebc468124c4ead7bb9c4833130ee9c2";
constexpr std::string_view node_v306_plan_state = "approval-prerequisite-artifact-intake-plan-ready";
constexpr std::string_view runtime_shell_chain_decision =
    "require-explicit-approval-prerequisites-before-runtime-shell";

constexpr std::string_view active_plan =
    "docs/plans2/v307-post-approval-prerequisite-artifact-upstream-echo-roadmap.md";
constexpr std::string_view node_v308_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-packet";
constexpr std::string_view node_v308_markdown =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-packet?format=markdown";
constexpr std::string_view node_v307_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-approval-"
                                            "prerequisite-artifact-upstream-echo-verification";
constexpr std::string_view node_v307_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-approval-"
                                                "prerequisite-artifact-upstream-echo-verification?format=markdown";

constexpr int check_count = 18;
constexpr int source_node_v307_check_count = 23;
constexpr int source_node_v306_check_count = 16;
constexpr int required_field_count = 9;
constexpr int prohibited_field_count = 9;
constexpr int rejection_reason_count = 13;
constexpr int missing_field_check_count = 9;
constexpr int no_go_boundary_count = 12;
constexpr int upstream_echo_request_count = 2;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver human approval artifact review non-participation receipt only; mini-kv echoes Node v308's "
    "contract-only human approval artifact review packet, does not accept credential values, raw endpoint URLs, "
    "provider/client configuration, HTTP/TCP payloads, ledger mutations, schema SQL, mini-kv write commands, runtime "
    "shell invocation requests, or authority claims, and does not execute LOAD/COMPACT/RESTORE/SETNXEX";
constexpr std::string_view node_action = "verify mini-kv v136 human approval artifact review non-participation receipt "
                                         "with Java v143 before Node v309 upstream echo verification";

std::string format_required_fields_json() {
    return "[{\"id\":\"artifact_id\",\"missing_field_code\":\"MISSING_ARTIFACT_ID\"},"
           "{\"id\":\"operator_approval_reference\",\"missing_field_code\":\"MISSING_OPERATOR_APPROVAL_REFERENCE\"},"
           "{\"id\":\"credential_handle_review_status\",\"missing_field_code\":\"MISSING_CREDENTIAL_HANDLE_REVIEW_"
           "STATUS\"},"
           "{\"id\":\"endpoint_handle_allowlist_review_status\",\"missing_field_code\":\"MISSING_ENDPOINT_HANDLE_"
           "ALLOWLIST_REVIEW_STATUS\"},"
           "{\"id\":\"no_network_safety_test_reference\",\"missing_field_code\":\"MISSING_NO_NETWORK_SAFETY_TEST_"
           "REFERENCE\"},"
           "{\"id\":\"manual_abort_semantics_reference\",\"missing_field_code\":\"MISSING_MANUAL_ABORT_SEMANTICS_"
           "REFERENCE\"},"
           "{\"id\":\"rollback_semantics_reference\",\"missing_field_code\":\"MISSING_ROLLBACK_SEMANTICS_REFERENCE\"},"
           "{\"id\":\"created_by_operator_identity\",\"missing_field_code\":\"MISSING_CREATED_BY_OPERATOR_IDENTITY\"},"
           "{\"id\":\"audit_correlation_id\",\"missing_field_code\":\"MISSING_AUDIT_CORRELATION_ID\"}]";
}

std::string format_prohibited_fields_json() {
    return "[{\"id\":\"credential_value\",\"rejection_code\":\"CREDENTIAL_VALUE_PRESENT\"},"
           "{\"id\":\"raw_endpoint_url\",\"rejection_code\":\"RAW_ENDPOINT_URL_PRESENT\"},"
           "{\"id\":\"secret_provider_config\",\"rejection_code\":\"PROVIDER_CONFIG_PRESENT\"},"
           "{\"id\":\"resolver_client_config\",\"rejection_code\":\"RESOLVER_CLIENT_CONFIG_PRESENT\"},"
           "{\"id\":\"external_request_payload\",\"rejection_code\":\"EXTERNAL_REQUEST_PAYLOAD_PRESENT\"},"
           "{\"id\":\"approval_ledger_mutation\",\"rejection_code\":\"APPROVAL_LEDGER_MUTATION_PRESENT\"},"
           "{\"id\":\"schema_migration_sql\",\"rejection_code\":\"SCHEMA_MIGRATION_SQL_PRESENT\"},"
           "{\"id\":\"mini_kv_write_command\",\"rejection_code\":\"MINI_KV_WRITE_COMMAND_PRESENT\"},"
           "{\"id\":\"runtime_shell_invocation_request\",\"rejection_code\":\"RUNTIME_SHELL_INVOCATION_REQUEST_"
           "PRESENT\"}]";
}

std::string format_rejection_reasons_json() {
    return "[\"MISSING_ARTIFACT_ID\",\"MISSING_OPERATOR_APPROVAL_REFERENCE\","
           "\"MISSING_CREDENTIAL_HANDLE_REVIEW_STATUS\","
           "\"MISSING_ENDPOINT_HANDLE_ALLOWLIST_REVIEW_STATUS\","
           "\"MISSING_NO_NETWORK_SAFETY_TEST_REFERENCE\","
           "\"MISSING_ABORT_OR_ROLLBACK_SEMANTICS\",\"CREDENTIAL_VALUE_PRESENT\","
           "\"RAW_ENDPOINT_URL_PRESENT\",\"PROVIDER_OR_CLIENT_CONFIG_PRESENT\","
           "\"EXTERNAL_REQUEST_REQUESTED\",\"WRITE_OR_SCHEMA_MUTATION_REQUESTED\","
           "\"MINI_KV_WRITE_OR_AUTHORITY_REQUESTED\","
           "\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\"]";
}

std::string format_missing_field_checks_json() {
    return "[{\"field_id\":\"artifact_id\",\"rejection_code\":\"MISSING_ARTIFACT_ID\"},"
           "{\"field_id\":\"operator_approval_reference\",\"rejection_code\":\"MISSING_OPERATOR_APPROVAL_REFERENCE\"},"
           "{\"field_id\":\"credential_handle_review_status\",\"rejection_code\":\"MISSING_CREDENTIAL_HANDLE_REVIEW_"
           "STATUS\"},"
           "{\"field_id\":\"endpoint_handle_allowlist_review_status\",\"rejection_code\":\"MISSING_ENDPOINT_HANDLE_"
           "ALLOWLIST_REVIEW_STATUS\"},"
           "{\"field_id\":\"no_network_safety_test_reference\",\"rejection_code\":\"MISSING_NO_NETWORK_SAFETY_TEST_"
           "REFERENCE\"},"
           "{\"field_id\":\"manual_abort_semantics_reference\",\"rejection_code\":\"MISSING_MANUAL_ABORT_SEMANTICS_"
           "REFERENCE\"},"
           "{\"field_id\":\"rollback_semantics_reference\",\"rejection_code\":\"MISSING_ROLLBACK_SEMANTICS_REFERENCE\"}"
           ","
           "{\"field_id\":\"created_by_operator_identity\",\"rejection_code\":\"MISSING_CREATED_BY_OPERATOR_IDENTITY\"}"
           ","
           "{\"field_id\":\"audit_correlation_id\",\"rejection_code\":\"MISSING_AUDIT_CORRELATION_ID\"}]";
}

std::string format_no_go_boundaries_json() {
    return "[{\"id\":\"credential_value_read\",\"closed\":true},"
           "{\"id\":\"raw_endpoint_url_parse\",\"closed\":true},"
           "{\"id\":\"secret_provider_instantiation\",\"closed\":true},"
           "{\"id\":\"resolver_client_instantiation\",\"closed\":true},"
           "{\"id\":\"fake_provider_or_client\",\"closed\":true},"
           "{\"id\":\"external_http_or_tcp_request\",\"closed\":true},"
           "{\"id\":\"runtime_shell_implementation\",\"closed\":true},"
           "{\"id\":\"runtime_shell_invocation\",\"closed\":true},"
           "{\"id\":\"approval_ledger_write\",\"closed\":true},"
           "{\"id\":\"schema_migration\",\"closed\":true},"
           "{\"id\":\"mini_kv_write_or_authority\",\"closed\":true},"
           "{\"id\":\"automatic_upstream_start\",\"closed\":true}]";
}

std::string format_upstream_echo_requests_json() {
    return "[{\"project\":\"java\",\"version\":\"Java v143\","
           "\"mode\":\"read-only-human-approval-artifact-review-packet-echo\","
           "\"can_run_in_parallel\":true,\"required_before_node_v309\":true},"
           "{\"project\":\"mini-kv\",\"version\":\"mini-kv v136\","
           "\"mode\":\"read-only-human-approval-artifact-review-non-participation-receipt\","
           "\"can_run_in_parallel\":true,\"required_before_node_v309\":true}]";
}

std::string format_checks_json() {
    return "{\"source_node_v307_ready\":true,"
           "\"source_node_v307_upstream_echo_aligned\":true,"
           "\"source_node_v307_artifact_contract_aligned\":true,"
           "\"source_node_v307_side_effects_closed\":true,"
           "\"required_review_fields_documented\":true,"
           "\"prohibited_review_fields_documented\":true,"
           "\"rejection_reasons_documented\":true,"
           "\"missing_field_checks_documented\":true,"
           "\"no_go_boundaries_closed\":true,"
           "\"necessity_proof_documented\":true,"
           "\"java_mini_kv_echo_request_explicitly_parallel\":true,"
           "\"review_packet_stays_contract_only\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"runtime_shell_implementation_still_blocked\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v309_human_approval_artifact_review_upstream_echo_verification\":true}";
}

void append_closed_boundary_flag_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back({"human_approval_artifact_review_non_participation_receipt_only", json_bool(true)});
    fields.push_back({"human_approval_artifact_review_packet_only", json_bool(true)});
    fields.push_back({"read_only_review_packet_contract", json_bool(true)});
    fields.push_back({"consumes_node_v308_human_approval_artifact_review_packet", json_bool(true)});
    fields.push_back({"consumes_node_v307_approval_prerequisite_artifact_upstream_echo_verification", json_bool(true)});
    fields.push_back(
        {"ready_for_node_v309_human_approval_artifact_review_upstream_echo_verification", json_bool(true)});
    fields.push_back({"ready_for_node_v309_before_upstream_echo", json_bool(false)});
    append_closed_runtime_shell_boundary_fields(fields);
}

} // namespace

std::string credential_resolver_human_approval_artifact_review_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_review_packet}},
        {std::string{source_upstream_echo}},
        {std::string{node_v308_profile_version}},
        {std::string{node_v308_review_packet_state}},
        {std::string{node_v308_packet_name}},
        {std::string{node_v308_packet_version}},
        {std::string{node_v308_review_mode}},
        {std::string{node_v308_packet_digest}},
        {std::string{node_v307_profile_version}},
        {std::string{node_v307_verification_state}},
        {std::string{node_v307_verification_digest}},
        {std::string{node_v306_artifact_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_node_v307_check_count)},
        {std::to_string(required_field_count)},
        {std::to_string(prohibited_field_count)},
        {std::to_string(rejection_reason_count)},
        {std::to_string(missing_field_check_count)},
        {std::to_string(no_go_boundary_count)},
        {std::to_string(upstream_echo_request_count)},
        {"human-approval-artifact-review-non-participation-receipt-only"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-human-approval-artifact-review", parts);
}

std::string format_credential_resolver_human_approval_artifact_review_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto review_packet = json_object({
        {"packet_name", json_string(node_v308_packet_name)},
        {"packet_version", json_string(node_v308_packet_version)},
        {"review_mode", json_string(node_v308_review_mode)},
        {"source_span", json_string(node_v308_source_span)},
        {"packet_digest", json_string(node_v308_packet_digest)},
        {"required_fields", format_required_fields_json()},
        {"prohibited_fields", format_prohibited_fields_json()},
        {"rejection_reasons", format_rejection_reasons_json()},
        {"missing_field_checks", format_missing_field_checks_json()},
        {"no_go_boundaries", format_no_go_boundaries_json()},
        {"upstream_echo_requests", format_upstream_echo_requests_json()},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"missing_field_check_count", json_integer(missing_field_check_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"java_mini_kv_echo_can_run_in_parallel", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
    });

    const auto necessity_proof = json_object({
        {"proof_complete", json_bool(true)},
        {"blocker_resolved", json_string(node_v308_blocker)},
        {"consumer", json_string(node_v308_necessity_consumer)},
        {"why_v307_cannot_be_reused", json_string(node_v308_reuse)},
        {"existing_report_reuse_decision",
         json_string("Reuse v307 only as source alignment evidence and create v308 as the smallest human review "
                     "packet consumed by Java v143, mini-kv v136, and Node v309.")},
        {"stop_condition", json_string(node_v308_stop_condition)},
    });

    const auto source_node_v308_summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v307_check_count", json_integer(source_node_v307_check_count)},
        {"source_node_v307_passed_check_count", json_integer(source_node_v307_check_count)},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"missing_field_check_count", json_integer(missing_field_check_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto source_node_v308_reference = json_object({
        {"source_version", json_string("Node v308")},
        {"profile_version", json_string(node_v308_profile_version)},
        {"review_packet_state", json_string(node_v308_review_packet_state)},
        {"runtime_shell_chain_decision", json_string(runtime_shell_chain_decision)},
        {"ready_for_human_approval_artifact_review_packet", json_bool(true)},
        {"human_approval_artifact_review_packet_only", json_bool(true)},
        {"read_only_review_packet_contract", json_bool(true)},
        {"consumes_node_v307_approval_prerequisite_artifact_upstream_echo_verification", json_bool(true)},
        {"next_java_version", json_string("Java v143")},
        {"next_mini_kv_version", json_string("mini-kv v136")},
        {"next_node_verification_version", json_string("Node v309")},
        {"ready_for_parallel_java_v143_mini_kv_v136_echo", json_bool(true)},
        {"ready_for_node_v309_before_upstream_echo", json_bool(false)},
        {"review_packet", review_packet},
        {"necessity_proof", necessity_proof},
        {"summary", source_node_v308_summary},
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

    const auto source_node_v307_reference = json_object({
        {"source_version", json_string("Node v307")},
        {"profile_version", json_string(node_v307_profile_version)},
        {"verification_state", json_string(node_v307_verification_state)},
        {"ready_for_upstream_echo_verification", json_bool(true)},
        {"verification_digest", json_string(node_v307_verification_digest)},
        {"verification_mode", json_string("approval-prerequisite-artifact-upstream-echo-verification-only")},
        {"source_span", json_string(node_v307_source_span)},
        {"upstream_echo_aligned", json_bool(true)},
        {"artifact_contract_aligned", json_bool(true)},
        {"side_effect_boundaries_aligned", json_bool(true)},
        {"source_node_v306_artifact_digest", json_string(node_v306_artifact_digest)},
        {"source_node_v306_plan_state", json_string(node_v306_plan_state)},
        {"source_node_v306_required_field_count", json_integer(12)},
        {"source_node_v306_prohibited_field_count", json_integer(8)},
        {"source_node_v306_rejection_reason_count", json_integer(9)},
        {"source_node_v306_no_go_boundary_count", json_integer(12)},
        {"source_node_v306_upstream_echo_request_count", json_integer(2)},
        {"source_node_v307_check_count", json_integer(source_node_v307_check_count)},
        {"source_node_v307_passed_check_count", json_integer(source_node_v307_check_count)},
        {"source_node_v306_check_count", json_integer(source_node_v306_check_count)},
        {"source_node_v306_passed_check_count", json_integer(source_node_v306_check_count)},
        {"java_v142_evidence_present", json_bool(true)},
        {"mini_kv_v135_evidence_present", json_bool(true)},
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
        {"receipt_mode", json_string("human-approval-artifact-review-non-participation-receipt-only")},
        {"shell_name", json_string("credential-resolver-disabled-runtime-shell")},
        {"non_participation_scope",
         json_string("v308 human approval artifact review packet, credential values, raw endpoints, provider/client "
                     "config, HTTP/TCP, ledger/schema writes, mini-kv writes/admin commands, "
                     "restore/load/compact/setnxex, runtime shell invocation, and authority")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(check_count)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v307_check_count", json_integer(source_node_v307_check_count)},
        {"source_node_v307_passed_check_count", json_integer(source_node_v307_check_count)},
        {"source_node_v306_check_count", json_integer(source_node_v306_check_count)},
        {"source_node_v306_passed_check_count", json_integer(source_node_v306_check_count)},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"missing_field_check_count", json_integer(missing_field_check_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"proof_complete", json_bool(true)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({json_object({
        {"code", json_string("HUMAN_APPROVAL_REVIEW_PACKET_DOES_NOT_AUTHORIZE_RUNTIME")},
        {"severity", json_string("warning")},
    })});

    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_JAVA_V143_AND_MINI_KV_V136_IN_PARALLEL_AFTER_V308")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("VERIFY_HUMAN_APPROVAL_ARTIFACT_REVIEW_ECHO_WITH_NODE_V309")},
                     {"severity", json_string("recommendation")}}),
    });

    const auto evidence_endpoints = json_object({
        {"human_approval_artifact_review_packet_json", json_string(node_v308_json)},
        {"human_approval_artifact_review_packet_markdown", json_string(node_v308_markdown)},
        {"source_node_v307_json", json_string(node_v307_json)},
        {"source_node_v307_markdown", json_string(node_v307_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v143", json_string("Java v143 human approval artifact review packet echo")},
        {"recommended_parallel_mini_kv_v136",
         json_string("mini-kv v136 human approval artifact review non-participation receipt")},
        {"next_node_verification", json_string("Node v309")},
    });

    const auto next_required_evidence_versions = json_array({json_string("Java v143"), json_string("mini-kv v136")});
    const auto next_actions = json_array({
        json_string("Archive mini-kv v136 as a human approval artifact review non-participation receipt."),
        json_string("Keep Node v309 blocked until Java v143 and mini-kv v136 are both available."),
        json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
        json_string("Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, "
                    "write ledgers or schema state, auto-start services, or execute "
                    "LOAD/COMPACT/RESTORE/SETNXEX."),
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string(receipt_version)},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_review_packet", json_string(source_review_packet)},
        {"source_upstream_echo", json_string(source_upstream_echo)},
        {"consumer_hint", json_string(receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(receipt_release_version)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(runtime_role)},
        {"source_node_v308_reference", source_node_v308_reference},
        {"source_node_v307_reference", source_node_v307_reference},
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
             credential_resolver_human_approval_artifact_review_non_participation_receipt_digest(read_commands))},
    };
    append_closed_boundary_flag_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
