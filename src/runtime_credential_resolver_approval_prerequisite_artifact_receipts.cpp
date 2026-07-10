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

constexpr std::string_view receipt_consumer = "Node v307 approval prerequisite artifact upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v135";
constexpr std::string_view receipt_artifact_path_hint = "d/135/";
constexpr std::string_view source_artifact_intake = "Node v306 approval prerequisite artifact intake plan";
constexpr std::string_view source_upstream_echo =
    "Node v305 runtime shell chain stop/prerequisite upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v306_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-approval-prerequisite-artifact-intake-plan.v1";
constexpr std::string_view node_v306_plan_state = "approval-prerequisite-artifact-intake-plan-ready";
constexpr std::string_view node_v306_artifact_name = "managed-audit-runtime-shell-approval-prerequisite-artifact";
constexpr std::string_view node_v306_artifact_version = "approval-prerequisite-artifact.v1";
constexpr std::string_view node_v306_intake_mode = "approval-prerequisite-artifact-intake-plan-only";
constexpr std::string_view node_v306_source_span = "Node v305 + planned Java v142 + planned mini-kv v135";
constexpr std::string_view node_v306_artifact_digest =
    "72f3e90606e40a978611fa4b8596c76c3ebc468124c4ead7bb9c4833130ee9c2";
constexpr std::string_view node_v306_necessity_consumer = "Java v142 + mini-kv v135, then Node v307";
constexpr std::string_view node_v306_blocker =
    "v305 proved the blocked prerequisite decision is echoed upstream, but the six documented-missing prerequisites "
    "still lack a concrete non-secret artifact shape.";
constexpr std::string_view node_v306_reuse =
    "v305 verifies Java v141 and mini-kv v134 echoed Node v304; it does not define required artifact fields, "
    "prohibited fields, rejection reasons, or no-go boundaries for the next intake.";
constexpr std::string_view node_v306_stop_condition =
    "Stop if the next step asks for credential values, raw endpoint URLs, provider/client configuration, HTTP/TCP, "
    "runtime shell implementation or invocation, ledger writes, schema migration, mini-kv writes/admin commands, or "
    "automatic upstream start.";

constexpr std::string_view node_v305_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-chain-stop-prerequisite-upstream-echo-"
    "verification.v1";
constexpr std::string_view node_v305_verification_state =
    "runtime-shell-chain-stop-prerequisite-upstream-echo-verification-ready";
constexpr std::string_view node_v305_verification_digest =
    "75278ffa6fe777a5936b47382e49d4524dbc2bd6eec893f2b002c6323e47b5bb";
constexpr std::string_view node_v305_source_span = "Node v304 + Java v141 + mini-kv v134";
constexpr std::string_view node_v304_decision_digest =
    "9212d0b804fdc1eda9098ac70d2441681730a98ff736776859811df9e288a654";
constexpr std::string_view runtime_shell_chain_decision =
    "require-explicit-approval-prerequisites-before-runtime-shell";

constexpr std::string_view active_plan = "docs/plans2/v305-post-stop-prerequisite-upstream-echo-roadmap.md";
constexpr std::string_view node_v306_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-approval-prerequisite-artifact-intake-plan";
constexpr std::string_view node_v306_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-approval-"
                                                "prerequisite-artifact-intake-plan?format=markdown";
constexpr std::string_view node_v305_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-"
                                            "chain-stop-prerequisite-upstream-echo-verification";
constexpr std::string_view node_v305_markdown =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-chain-stop-prerequisite-upstream-echo-"
    "verification?format=markdown";

constexpr int check_count = 18;
constexpr int source_check_count = 16;
constexpr int prerequisite_count_from_node_v305 = 6;
constexpr int missing_runtime_prerequisite_count_from_node_v305 = 6;
constexpr int no_go_condition_count_from_node_v305 = 8;
constexpr int required_field_count = 12;
constexpr int prohibited_field_count = 8;
constexpr int rejection_reason_count = 9;
constexpr int no_go_boundary_count = 12;
constexpr int upstream_echo_request_count = 2;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver approval prerequisite artifact intake non-participation receipt only; mini-kv echoes Node "
    "v306's non-secret artifact contract, does not accept credential values, raw endpoint URLs, provider/client "
    "configuration, HTTP/TCP payloads, ledger mutations, schema SQL, mini-kv write commands, or authority claims, and "
    "does not execute LOAD/COMPACT/RESTORE/SETNXEX";
constexpr std::string_view node_action = "verify mini-kv v135 approval prerequisite artifact intake non-participation "
                                         "receipt with Java v142 before Node v307 upstream echo verification";

std::string format_required_fields_json() {
    return "[{\"id\":\"artifact_id\",\"label\":\"Artifact id\",\"required\":true,\"source\":\"operator\","
           "\"accepted_shape\":\"stable non-secret id\"},"
           "{\"id\":\"source_node_verification\",\"label\":\"Source Node verification\",\"required\":true,"
           "\"source\":\"node-v305\",\"accepted_shape\":\"Node v305 digest + route reference\"},"
           "{\"id\":\"operator_approval_reference\",\"label\":\"Operator approval reference\",\"required\":true,"
           "\"source\":\"operator\",\"accepted_shape\":\"non-secret approval ticket or review id\"},"
           "{\"id\":\"credential_handle_review_status\",\"label\":\"Credential handle review "
           "status\",\"required\":true,"
           "\"source\":\"audit-process\",\"accepted_shape\":\"credential handle + status, no value\"},"
           "{\"id\":\"endpoint_handle_allowlist_review_status\",\"label\":\"Endpoint handle allowlist review status\","
           "\"required\":true,\"source\":\"audit-process\",\"accepted_shape\":\"endpoint handle + allowlist status, no "
           "raw URL\"},"
           "{\"id\":\"no_network_safety_test_reference\",\"label\":\"No-network safety test "
           "reference\",\"required\":true,"
           "\"source\":\"node-v305\",\"accepted_shape\":\"test or report id\"},"
           "{\"id\":\"manual_abort_semantics_reference\",\"label\":\"Manual abort semantics "
           "reference\",\"required\":true,"
           "\"source\":\"operator\",\"accepted_shape\":\"runbook or review id\"},"
           "{\"id\":\"rollback_semantics_reference\",\"label\":\"Rollback semantics reference\",\"required\":true,"
           "\"source\":\"operator\",\"accepted_shape\":\"runbook or review id\"},"
           "{\"id\":\"java_echo_required_version\",\"label\":\"Java echo required version\",\"required\":true,"
           "\"source\":\"java-v142\",\"accepted_shape\":\"Java v142 receipt reference\"},"
           "{\"id\":\"mini_kv_receipt_required_version\",\"label\":\"mini-kv receipt required "
           "version\",\"required\":true,"
           "\"source\":\"mini-kv-v135\",\"accepted_shape\":\"mini-kv v135 receipt reference\"},"
           "{\"id\":\"created_by_operator_identity\",\"label\":\"Created by operator identity\",\"required\":true,"
           "\"source\":\"operator\",\"accepted_shape\":\"operator id or identity handle\"},"
           "{\"id\":\"audit_correlation_id\",\"label\":\"Audit correlation id\",\"required\":true,"
           "\"source\":\"audit-process\",\"accepted_shape\":\"stable correlation id\"}]";
}

std::string format_prohibited_fields_json() {
    return "[{\"id\":\"credential_value\",\"rejection_code\":\"CREDENTIAL_VALUE_PRESENT\"},"
           "{\"id\":\"raw_endpoint_url\",\"rejection_code\":\"RAW_ENDPOINT_URL_PRESENT\"},"
           "{\"id\":\"secret_provider_config\",\"rejection_code\":\"PROVIDER_OR_CLIENT_CONFIG_PRESENT\"},"
           "{\"id\":\"resolver_client_config\",\"rejection_code\":\"PROVIDER_OR_CLIENT_CONFIG_PRESENT\"},"
           "{\"id\":\"external_request_payload\",\"rejection_code\":\"EXTERNAL_REQUEST_REQUESTED\"},"
           "{\"id\":\"approval_ledger_mutation\",\"rejection_code\":\"WRITE_OR_SCHEMA_MUTATION_REQUESTED\"},"
           "{\"id\":\"schema_migration_sql\",\"rejection_code\":\"WRITE_OR_SCHEMA_MUTATION_REQUESTED\"},"
           "{\"id\":\"mini_kv_write_command\",\"rejection_code\":\"MINI_KV_WRITE_OR_AUTHORITY_REQUESTED\"}]";
}

std::string format_rejection_reasons_json() {
    return "[\"MISSING_OPERATOR_APPROVAL_REFERENCE\",\"CREDENTIAL_VALUE_PRESENT\","
           "\"RAW_ENDPOINT_URL_PRESENT\",\"NO_NETWORK_SAFETY_TEST_MISSING\","
           "\"ABORT_ROLLBACK_SEMANTICS_MISSING\",\"JAVA_OR_MINIKV_ECHO_MISSING\","
           "\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\",\"EXTERNAL_REQUEST_REQUESTED\","
           "\"WRITE_OR_SCHEMA_MUTATION_REQUESTED\"]";
}

std::string format_no_go_boundaries_json() {
    return "[{\"id\":\"runtime_shell_implemented\",\"allowed\":false},"
           "{\"id\":\"runtime_shell_invocation_allowed\",\"allowed\":false},"
           "{\"id\":\"execution_allowed\",\"allowed\":false},"
           "{\"id\":\"connects_managed_audit\",\"allowed\":false},"
           "{\"id\":\"credential_value_read\",\"allowed\":false},"
           "{\"id\":\"raw_endpoint_url_parsed\",\"allowed\":false},"
           "{\"id\":\"external_request_sent\",\"allowed\":false},"
           "{\"id\":\"provider_or_client_instantiated\",\"allowed\":false},"
           "{\"id\":\"schema_migration_executed\",\"allowed\":false},"
           "{\"id\":\"approval_ledger_written\",\"allowed\":false},"
           "{\"id\":\"mini_kv_write_or_authority\",\"allowed\":false},"
           "{\"id\":\"automatic_upstream_start\",\"allowed\":false}]";
}

std::string format_upstream_echo_requests_json() {
    return "[{\"project\":\"java\",\"version\":\"Java v142\",\"can_run_in_parallel\":true,"
           "\"must_remain_read_only\":true},"
           "{\"project\":\"mini-kv\",\"version\":\"mini-kv v135\",\"can_run_in_parallel\":true,"
           "\"must_remain_read_only\":true}]";
}

std::string format_checks_json() {
    return "{\"source_node_v305_ready\":true,"
           "\"source_node_v305_upstream_echo_aligned\":true,"
           "\"source_node_v305_prerequisite_gate_blocked\":true,"
           "\"source_node_v305_side_effects_closed\":true,"
           "\"approval_prerequisite_artifact_contract_echoed\":true,"
           "\"required_artifact_fields_documented\":true,"
           "\"prohibited_artifact_fields_documented\":true,"
           "\"rejection_reasons_documented\":true,"
           "\"no_go_boundaries_closed\":true,"
           "\"necessity_proof_documented\":true,"
           "\"java_mini_kv_echo_request_explicitly_parallel\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"runtime_shell_implementation_still_blocked\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"mini_kv_non_participation_recorded\":true,"
           "\"ready_for_node_v307_approval_prerequisite_artifact_upstream_echo_verification\":true}";
}

void append_closed_boundary_flag_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back({"approval_prerequisite_artifact_intake_non_participation_receipt_only", json_bool(true)});
    fields.push_back({"approval_prerequisite_artifact_intake_plan_only", json_bool(true)});
    fields.push_back({"read_only_artifact_contract", json_bool(true)});
    fields.push_back({"consumes_node_v306_approval_prerequisite_artifact_intake_plan", json_bool(true)});
    fields.push_back({"consumes_node_v305_stop_prerequisite_upstream_echo_verification", json_bool(true)});
    fields.push_back(
        {"ready_for_node_v307_approval_prerequisite_artifact_upstream_echo_verification", json_bool(true)});
    fields.push_back({"ready_for_node_v307_before_upstream_echo", json_bool(false)});
    append_closed_runtime_shell_boundary_fields(fields);
}

} // namespace

std::string credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_artifact_intake}},
        {std::string{source_upstream_echo}},
        {std::string{node_v306_profile_version}},
        {std::string{node_v306_plan_state}},
        {std::string{node_v306_artifact_name}},
        {std::string{node_v306_artifact_version}},
        {std::string{node_v306_intake_mode}},
        {std::string{node_v306_artifact_digest}},
        {std::string{node_v305_profile_version}},
        {std::string{node_v305_verification_state}},
        {std::string{node_v305_verification_digest}},
        {std::string{node_v304_decision_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(required_field_count)},
        {std::to_string(prohibited_field_count)},
        {std::to_string(rejection_reason_count)},
        {std::to_string(no_go_boundary_count)},
        {std::to_string(upstream_echo_request_count)},
        {"approval-prerequisite-artifact-intake-non-participation-receipt-only"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-approval-prerequisite-artifact-intake", parts);
}

std::string format_credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto artifact_intake_plan = json_object({
        {"artifact_name", json_string(node_v306_artifact_name)},
        {"artifact_version", json_string(node_v306_artifact_version)},
        {"intake_mode", json_string(node_v306_intake_mode)},
        {"source_span", json_string(node_v306_source_span)},
        {"artifact_digest", json_string(node_v306_artifact_digest)},
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
        {"java_mini_kv_echo_can_run_in_parallel", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
    });

    const auto necessity_proof = json_object({
        {"proof_complete", json_bool(true)},
        {"blocker_resolved", json_string(node_v306_blocker)},
        {"consumer", json_string(node_v306_necessity_consumer)},
        {"why_v305_cannot_be_reused", json_string(node_v306_reuse)},
        {"existing_report_reuse_decision",
         json_string("Reuse v305 only as source evidence and create v306 as the smallest artifact intake contract "
                     "consumed by Java v142, mini-kv v135, and Node v307.")},
        {"stop_condition", json_string(node_v306_stop_condition)},
    });

    const auto source_node_v306_summary = json_object({
        {"check_count", json_integer(source_check_count)},
        {"passed_check_count", json_integer(source_check_count)},
        {"prerequisite_count_from_node_v305", json_integer(prerequisite_count_from_node_v305)},
        {"missing_runtime_prerequisite_count_from_node_v305",
         json_integer(missing_runtime_prerequisite_count_from_node_v305)},
        {"no_go_condition_count_from_node_v305", json_integer(no_go_condition_count_from_node_v305)},
        {"required_field_count", json_integer(required_field_count)},
        {"prohibited_field_count", json_integer(prohibited_field_count)},
        {"rejection_reason_count", json_integer(rejection_reason_count)},
        {"no_go_boundary_count", json_integer(no_go_boundary_count)},
        {"upstream_echo_request_count", json_integer(upstream_echo_request_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto source_node_v306_reference = json_object({
        {"source_version", json_string("Node v306")},
        {"profile_version", json_string(node_v306_profile_version)},
        {"plan_state", json_string(node_v306_plan_state)},
        {"runtime_shell_chain_decision", json_string(runtime_shell_chain_decision)},
        {"ready_for_approval_prerequisite_artifact_intake_plan", json_bool(true)},
        {"approval_prerequisite_artifact_intake_plan_only", json_bool(true)},
        {"read_only_artifact_contract", json_bool(true)},
        {"consumes_node_v305_stop_prerequisite_upstream_echo_verification", json_bool(true)},
        {"next_java_version", json_string("Java v142")},
        {"next_mini_kv_version", json_string("mini-kv v135")},
        {"next_node_verification_version", json_string("Node v307")},
        {"ready_for_parallel_java_v142_mini_kv_v135_echo", json_bool(true)},
        {"ready_for_node_v307_before_upstream_echo", json_bool(false)},
        {"artifact_intake_plan", artifact_intake_plan},
        {"necessity_proof", necessity_proof},
        {"summary", source_node_v306_summary},
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

    const auto source_node_v305_reference = json_object({
        {"source_version", json_string("Node v305")},
        {"profile_version", json_string(node_v305_profile_version)},
        {"verification_state", json_string(node_v305_verification_state)},
        {"ready_for_upstream_echo_verification", json_bool(true)},
        {"verification_digest", json_string(node_v305_verification_digest)},
        {"source_span", json_string(node_v305_source_span)},
        {"upstream_echo_aligned", json_bool(true)},
        {"prerequisite_gate_still_blocked", json_bool(true)},
        {"side_effect_boundaries_aligned", json_bool(true)},
        {"source_node_v304_decision_digest", json_string(node_v304_decision_digest)},
        {"prerequisite_count", json_integer(prerequisite_count_from_node_v305)},
        {"missing_runtime_prerequisite_count", json_integer(missing_runtime_prerequisite_count_from_node_v305)},
        {"no_go_condition_count", json_integer(no_go_condition_count_from_node_v305)},
        {"production_blocker_count", json_integer(0)},
        {"warning_count", json_integer(1)},
        {"recommendation_count", json_integer(2)},
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
        {"receipt_mode", json_string("approval-prerequisite-artifact-intake-non-participation-receipt-only")},
        {"shell_name", json_string("credential-resolver-disabled-runtime-shell")},
        {"non_participation_scope",
         json_string("v306 artifact intake contract, credential values, raw endpoints, provider/client config, "
                     "HTTP/TCP, ledger/schema writes, mini-kv writes/admin commands, restore/load/compact/setnxex, "
                     "and authority")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(check_count)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v306_check_count", json_integer(source_check_count)},
        {"source_node_v306_passed_check_count", json_integer(source_check_count)},
        {"prerequisite_count_from_node_v305", json_integer(prerequisite_count_from_node_v305)},
        {"missing_runtime_prerequisite_count_from_node_v305",
         json_integer(missing_runtime_prerequisite_count_from_node_v305)},
        {"no_go_condition_count_from_node_v305", json_integer(no_go_condition_count_from_node_v305)},
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

    const auto warnings = json_array({json_object({
        {"code", json_string("ARTIFACT_INTAKE_PLAN_DOES_NOT_AUTHORIZE_RUNTIME")},
        {"severity", json_string("warning")},
    })});

    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_JAVA_V142_AND_MINI_KV_V135_IN_PARALLEL_AFTER_V306")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("VERIFY_ARTIFACT_ECHO_WITH_NODE_V307")},
                     {"severity", json_string("recommendation")}}),
    });

    const auto evidence_endpoints = json_object({
        {"approval_prerequisite_artifact_intake_plan_json", json_string(node_v306_json)},
        {"approval_prerequisite_artifact_intake_plan_markdown", json_string(node_v306_markdown)},
        {"source_node_v305_json", json_string(node_v305_json)},
        {"source_node_v305_markdown", json_string(node_v305_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v142", json_string("Java v142 approval prerequisite artifact intake echo")},
        {"recommended_parallel_mini_kv_v135",
         json_string("mini-kv v135 approval prerequisite artifact intake non-participation receipt")},
        {"next_node_verification", json_string("Node v307")},
    });

    const auto next_required_evidence_versions = json_array({json_string("Java v142"), json_string("mini-kv v135")});
    const auto next_actions = json_array({
        json_string("Archive mini-kv v135 as an approval prerequisite artifact intake non-participation receipt."),
        json_string("Keep Node v307 blocked until Java v142 and mini-kv v135 are both available."),
        json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
        json_string("Do not read credentials, parse raw endpoints, accept provider/client config, send HTTP/TCP, "
                    "write ledgers or schema state, auto-start services, or execute "
                    "LOAD/COMPACT/RESTORE/SETNXEX."),
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_artifact_intake", json_string(source_artifact_intake)},
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
        {"source_node_v306_reference", source_node_v306_reference},
        {"source_node_v305_reference", source_node_v305_reference},
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
         json_string(credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_digest(
             read_commands))},
    };
    append_closed_boundary_flag_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
