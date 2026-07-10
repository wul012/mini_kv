#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::field_string;
using credential_resolver_detail::receipt_digest;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

constexpr std::string_view receipt_version =
    "mini-kv-credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.v1";
constexpr std::string_view receipt_consumer = "Node v330 candidate gate upstream alignment / hardening review";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v143";
constexpr std::string_view receipt_artifact_path_hint = "d/143/";
constexpr std::string_view source_candidate_gate_decision =
    "Node v329 implementation candidate gate / input-hardening decision";
constexpr std::string_view source_final_prerequisite_closure = "Node v328 final prerequisite closure review";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v329_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "implementation-candidate-gate-input-hardening-decision.v1";
constexpr std::string_view node_v329_candidate_gate_state =
    "implementation-candidate-gate-input-hardening-decision-ready";
constexpr std::string_view node_v329_candidate_gate_decision =
    "require-input-export-hardening-before-disabled-runtime-design";
constexpr std::string_view node_v329_decision_digest =
    "e92109116e9d106ffdaa5691aa2eaa626fb595f8a9f4020dbf903fbce7874e87";
constexpr std::string_view node_v329_record_mode = "implementation-candidate-gate-input-hardening-decision-only";
constexpr std::string_view node_v329_decision_scope =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell";
constexpr std::string_view node_v329_source_span = "Node v328 final prerequisite closure review";
constexpr std::string_view node_v329_decision_reason =
    "Node v328 closed all prerequisites, but v327 still consumes Java Markdown archive evidence and a mini-kv release "
    "receipt file; v329 therefore enters the implementation candidate gate only to require stable input export "
    "hardening before any disabled runtime design draft.";

constexpr std::string_view node_v328_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-final-prerequisite-closure-review.v1";
constexpr std::string_view node_v328_review_state = "final-prerequisite-closure-review-ready";
constexpr std::string_view node_v328_closure_digest =
    "15d924b9d529f4d60dd43010b82689f5173d4f2583f8ae31f070f877141a6d14";
constexpr std::string_view node_v328_next_step_mode = "implementation-candidate-gate-only";
constexpr std::string_view active_plan = "docs/plans2/v328-post-final-prerequisite-closure-roadmap.md";

constexpr std::string_view node_v329_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-"
                                            "implementation-candidate-gate-input-hardening-decision";
constexpr std::string_view node_v329_markdown =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-implementation-candidate-gate-input-hardening-"
    "decision?format=markdown";
constexpr std::string_view node_v328_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-final-prerequisite-closure-review";
constexpr std::string_view node_v328_markdown =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-final-prerequisite-closure-review?format=markdown";

constexpr int check_count = 16;
constexpr int source_node_v328_check_count = 18;
constexpr int source_node_v328_production_blocker_count = 0;
constexpr int completed_prerequisite_count = 6;
constexpr int remaining_prerequisite_count = 0;
constexpr int original_prerequisite_count = 6;
constexpr int input_hardening_requirement_count = 4;
constexpr int no_go_condition_count = 7;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver implementation candidate gate input-hardening non-participation receipt only; mini-kv echoes "
    "Node v329's input-hardening decision, confirms a stable current receipt export is required and provided, does not "
    "create runtime shell design permission, and does not accept credential values, raw endpoint URLs, provider/client "
    "configuration, HTTP/TCP payloads, Java SQL, rollback, ledger/schema writes, mini-kv write/admin commands, "
    "LOAD/COMPACT/RESTORE/SETNXEX, or automatic upstream start";
constexpr std::string_view node_action = "verify mini-kv v143 input-hardening non-participation receipt with Java v151 "
                                         "before Node v330 candidate gate upstream alignment";

std::string format_input_hardening_requirements_json() {
    return "[{\"id\":\"java-stable-evidence-export\",\"owner\":\"java\","
           "\"label\":\"Stabilize Java evidence export\","
           "\"current_evidence\":\"Node v327 currently reads Java v150 archive Markdown; Java v151 should echo whether "
           "a stable JSON/current evidence export is required before design draft.\","
           "\"required_before_design_draft\":true,\"requested_version\":\"Java v151\",\"status\":\"required\"},"
           "{\"id\":\"mini-kv-stable-current-receipt\",\"owner\":\"mini-kv\","
           "\"label\":\"Stabilize mini-kv current receipt export\","
           "\"current_evidence\":\"Node v327 currently reads a mini-kv v142 release receipt file; mini-kv v143 should "
           "echo whether a stable current receipt path is required before design draft.\","
           "\"required_before_design_draft\":true,\"requested_version\":\"mini-kv v143\",\"status\":\"required\"},"
           "{\"id\":\"node-fail-closed-diagnostics\",\"owner\":\"node\","
           "\"label\":\"Keep fail-closed diagnostics as a candidate gate requirement\","
           "\"current_evidence\":\"Node v329 must preserve missing-input and enabled-upstream-action blockers before "
           "Node v330 evaluates upstream alignment.\","
           "\"required_before_design_draft\":true,\"requested_version\":\"Node v330\",\"status\":\"required\"},"
           "{\"id\":\"route-evidence-consumability\",\"owner\":\"node\","
           "\"label\":\"Keep JSON/Markdown evidence consumable\","
           "\"current_evidence\":\"Node v329 keeps the report route readable and screenshot-friendly so v330 can "
           "consume the archived decision without manual reconstruction.\","
           "\"required_before_design_draft\":true,\"requested_version\":\"Node v330\",\"status\":\"required\"}]";
}

std::string format_explicit_no_go_conditions_json() {
    return "[{\"code\":\"CREDENTIAL_VALUE_REQUIRED\","
           "\"condition\":\"The next step requires reading or rendering credential values.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"RAW_ENDPOINT_URL_REQUIRED\","
           "\"condition\":\"The next step requires parsing or rendering a raw endpoint URL.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"PROVIDER_OR_CLIENT_REQUIRED\","
           "\"condition\":\"The next step requires instantiating a provider, resolver client, or fake client.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"NETWORK_REQUEST_REQUIRED\","
           "\"condition\":\"The next step requires HTTP/TCP or managed audit network access.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"JAVA_WRITE_REQUIRED\","
           "\"condition\":\"The next step requires Java SQL, deployment, rollback, ledger, or schema writes.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"MINI_KV_WRITE_OR_ADMIN_REQUIRED\","
           "\"condition\":\"The next step requires mini-kv LOAD, COMPACT, RESTORE, SETNXEX, or write commands.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"AUTO_START_REQUIRED\","
           "\"condition\":\"The next step requires automatically starting Java, mini-kv, or external services.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"}]";
}

void append_closed_side_effect_fields(std::vector<OrderedJsonField>& fields) {
    fields.push_back({"runtime_shell_implemented", json_bool(false)});
    fields.push_back({"runtime_shell_enabled", json_bool(false)});
    fields.push_back({"runtime_shell_invocation_allowed", json_bool(false)});
    fields.push_back({"runtime_shell_implementation_allowed", json_bool(false)});
    fields.push_back({"runtime_shell_design_draft_allowed", json_bool(false)});
    fields.push_back({"real_resolver_implementation_allowed", json_bool(false)});
    fields.push_back({"credential_resolver_implemented", json_bool(false)});
    fields.push_back({"credential_resolver_invoked", json_bool(false)});
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_instantiated", json_bool(false)});
    fields.push_back({"provider_client_instantiation_allowed", json_bool(false)});
    fields.push_back({"credential_value_accepted", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_read", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_stored", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"credential_value_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_accepted", json_bool(false)});
    fields.push_back({"raw_endpoint_url_stored", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parse_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_render_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_rendered", json_bool(false)});
    fields.push_back({"external_request_payload_accepted", json_bool(false)});
    fields.push_back({"external_request_allowed", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    fields.push_back({"http_request_allowed", json_bool(false)});
    fields.push_back({"http_request_sent", json_bool(false)});
    fields.push_back({"tcp_connection_allowed", json_bool(false)});
    fields.push_back({"tcp_connection_attempted", json_bool(false)});
    fields.push_back({"network_socket_opened", json_bool(false)});
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
    fields.push_back({"schema_migration_execution_allowed", json_bool(false)});
    fields.push_back({"rollback_execution_allowed", json_bool(false)});
    fields.push_back({"rollback_executed", json_bool(false)});
    fields.push_back({"deployment_action_allowed", json_bool(false)});
    fields.push_back({"deployment_action_executed", json_bool(false)});
    fields.push_back({"java_sql_execution_allowed", json_bool(false)});
    fields.push_back({"java_sql_executed", json_bool(false)});
    fields.push_back({"mini_kv_write_command_allowed", json_bool(false)});
    fields.push_back({"mini_kv_load_allowed", json_bool(false)});
    fields.push_back({"mini_kv_compact_allowed", json_bool(false)});
    fields.push_back({"mini_kv_restore_allowed", json_bool(false)});
    fields.push_back({"mini_kv_setnxex_allowed", json_bool(false)});
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

std::string format_source_node_v328_json() {
    std::vector<OrderedJsonField> fields = {
        {"source_version", json_string("Node v328")},
        {"profile_version", json_string(node_v328_profile_version)},
        {"review_state", json_string(node_v328_review_state)},
        {"ready_for_final_prerequisite_closure_review", json_bool(true)},
        {"all_prerequisites_closed", json_bool(true)},
        {"ready_for_implementation_candidate_gate", json_bool(true)},
        {"next_node_version_suggested", json_string("Node v329")},
        {"closure_digest", json_string(node_v328_closure_digest)},
        {"source_node_readiness_version", json_string("Node v327")},
        {"completed_prerequisite_count", json_integer(completed_prerequisite_count)},
        {"remaining_prerequisite_count", json_integer(remaining_prerequisite_count)},
        {"original_prerequisite_count", json_integer(original_prerequisite_count)},
        {"next_step_mode", json_string(node_v328_next_step_mode)},
        {"source_check_count", json_integer(source_node_v328_check_count)},
        {"source_passed_check_count", json_integer(source_node_v328_check_count)},
        {"source_production_blocker_count", json_integer(source_node_v328_production_blocker_count)},
        {"source_warning_count", json_integer(1)},
        {"source_recommendation_count", json_integer(2)},
    };
    append_closed_side_effect_fields(fields);
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

std::string format_decision_record_json() {
    return "{\"decision_digest\":" + field_string(node_v329_decision_digest) +
           ",\"record_mode\":" + field_string(node_v329_record_mode) +
           ",\"decision_scope\":" + field_string(node_v329_decision_scope) +
           ",\"source_span\":" + field_string(node_v329_source_span) +
           ",\"decision\":" + field_string(node_v329_candidate_gate_decision) +
           ",\"decision_reason\":" + field_string(node_v329_decision_reason) +
           ",\"all_prerequisites_closed\":true"
           ",\"candidate_gate_entered\":true"
           ",\"allows_parallel_java_v151_mini_kv_v143_echo_request\":true"
           ",\"allows_node_v330_before_upstream_echo\":false"
           ",\"allows_disabled_runtime_shell_design_draft\":false"
           ",\"allows_disabled_runtime_shell_implementation\":false"
           ",\"allows_disabled_runtime_shell_invocation\":false"
           ",\"allows_real_resolver_implementation\":false"
           ",\"allows_secret_provider_instantiation\":false"
           ",\"allows_resolver_client_instantiation\":false"
           ",\"allows_credential_value_read\":false"
           ",\"allows_raw_endpoint_url_parse\":false"
           ",\"allows_external_request\":false"
           ",\"allows_managed_audit_connection\":false"
           ",\"allows_schema_migration\":false"
           ",\"allows_approval_ledger_write\":false"
           ",\"allows_rollback_execution\":false"
           ",\"allows_mini_kv_write_or_admin_command\":false"
           ",\"allows_automatic_upstream_start\":false"
           ",\"input_hardening_requirement_count\":" +
           std::to_string(input_hardening_requirement_count) +
           ",\"no_go_condition_count\":" + std::to_string(no_go_condition_count) +
           ",\"input_hardening_requirements\":" + format_input_hardening_requirements_json() +
           ",\"explicit_no_go_conditions\":" + format_explicit_no_go_conditions_json() + "}";
}

std::string format_necessity_proof_json() {
    return "{\"proof_complete\":true"
           ",\"blocker_resolved\":\"v328 closed all prerequisites, but candidate-gate input export hardening is still "
           "needed before any disabled runtime design draft can be considered.\""
           ",\"consumer\":\"Java v151 + mini-kv v143, then Node v330\""
           ",\"why_v328_cannot_be_reused\":\"v328 is a final prerequisite closure review; it proves 6/6 prerequisites "
           "are closed but does not answer whether Java and mini-kv expose stable current input artifacts for Node "
           "v330.\""
           ",\"existing_report_reuse_decision\":\"Reuse v328 as source closure state and v329 as the candidate gate "
           "decision; create v143 only to echo mini-kv stable current receipt requirements and non-participation "
           "boundaries.\""
           ",\"stop_condition\":\"Stop if input hardening requires credential values, raw endpoint URLs, "
           "provider/client instantiation, HTTP/TCP, Java SQL, rollback, ledger/schema writes, mini-kv writes/admin "
           "commands, LOAD/COMPACT/RESTORE/SETNXEX, or automatic upstream start.\"}";
}

std::string format_checks_json() {
    return "{\"source_node_v329_ready\":true,"
           "\"source_node_v329_requires_input_hardening\":true,"
           "\"source_node_v328_all_prerequisites_closed\":true,"
           "\"stable_current_receipt_requirement_echoed\":true,"
           "\"stable_current_receipt_export_paths_documented\":true,"
           "\"implementation_candidate_gate_only\":true,"
           "\"node_v330_waits_for_upstream_echo\":true,"
           "\"runtime_design_still_blocked\":true,"
           "\"provider_client_still_disabled\":true,"
           "\"credential_raw_endpoint_still_closed\":true,"
           "\"network_still_closed\":true,"
           "\"java_write_still_closed\":true,"
           "\"mini_kv_admin_write_still_closed\":true,"
           "\"load_compact_restore_setnxex_still_blocked\":true,"
           "\"upstream_auto_start_still_disabled\":true,"
           "\"ready_for_node_v330_candidate_gate_upstream_alignment_after_java_v151\":true}";
}

std::string format_stable_current_receipt_export_json() {
    return "{\"requirement_id\":\"mini-kv-stable-current-receipt\""
           ",\"owner\":\"mini-kv\""
           ",\"required_before_design_draft\":true"
           ",\"requested_version\":\"mini-kv v143\""
           ",\"status\":\"required-and-echoed\""
           ",\"stable_current_receipt_path_required\":true"
           ",\"stable_current_receipt_export_ready\":true"
           ",\"stable_current_receipt_export_paths\":[" +
           field_string(receipt_fixture_path) +
           ",\"fixtures/release/runtime-smoke-evidence.json\","
           "\"fixtures/release/verification-manifest.json\"]"
           ",\"runtime_smoke_field\":\"credential_resolver_implementation_candidate_gate_input_hardening_non_"
           "participation_receipt\""
           ",\"node_v330_consumption\":\"Node v330 can consume SMOKEJSON, runtime-smoke-evidence.json, or the "
           "standalone v143 fixture, but still must wait for Java v151 before deciding any disabled runtime design "
           "draft.\"}";
}

std::string format_source_node_v329_json() {
    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v328_check_count", json_integer(source_node_v328_check_count)},
        {"source_node_v328_passed_check_count", json_integer(source_node_v328_check_count)},
        {"source_production_blocker_count", json_integer(source_node_v328_production_blocker_count)},
        {"input_hardening_requirement_count", json_integer(input_hardening_requirement_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });
    std::vector<OrderedJsonField> fields = {
        {"source_version", json_string("Node v329")},
        {"profile_version", json_string(node_v329_profile_version)},
        {"candidate_gate_state", json_string(node_v329_candidate_gate_state)},
        {"candidate_gate_decision", json_string(node_v329_candidate_gate_decision)},
        {"ready_for_implementation_candidate_gate_input_hardening_decision", json_bool(true)},
        {"read_only_decision_record", json_bool(true)},
        {"implementation_candidate_gate_only", json_bool(true)},
        {"consumes_node_v328_final_prerequisite_closure_review", json_bool(true)},
        {"active_node_version", json_string("Node v329")},
        {"source_node_closure_version", json_string("Node v328")},
        {"ready_for_parallel_java_v151_mini_kv_v143_echo_request", json_bool(true)},
        {"ready_for_node_v330_candidate_gate_upstream_alignment", json_bool(false)},
        {"ready_for_disabled_runtime_shell_design_draft", json_bool(false)},
        {"ready_for_runtime_shell_implementation", json_bool(false)},
        {"ready_for_runtime_shell_invocation", json_bool(false)},
        {"ready_for_managed_audit_resolver_implementation", json_bool(false)},
        {"decision_record", format_decision_record_json()},
        {"necessity_proof", format_necessity_proof_json()},
        {"source_node_v328_reference", format_source_node_v328_json()},
        {"summary", summary},
    };
    append_closed_side_effect_fields(fields);
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace

std::string credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_candidate_gate_decision}},
        {std::string{source_final_prerequisite_closure}},
        {std::string{node_v329_profile_version}},
        {std::string{node_v329_candidate_gate_state}},
        {std::string{node_v329_candidate_gate_decision}},
        {std::string{node_v329_decision_digest}},
        {std::string{node_v328_profile_version}},
        {std::string{node_v328_review_state}},
        {std::string{node_v328_closure_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_node_v328_check_count)},
        {std::to_string(input_hardening_requirement_count)},
        {std::to_string(no_go_condition_count)},
        {"stable-current-receipt-required-and-echoed"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-input-hardening-candidate-gate", parts);
}

std::string format_credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto mini_kv_receipt = json_object({
        {"receipt_mode", json_string("implementation-candidate-gate-input-hardening-non-participation-receipt-only")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(check_count)},
        {"input_hardening_requirement_count", json_integer(input_hardening_requirement_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"stable_current_receipt_export", format_stable_current_receipt_export_json()},
    });
    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v328_check_count", json_integer(source_node_v328_check_count)},
        {"source_node_v328_passed_check_count", json_integer(source_node_v328_check_count)},
        {"source_completed_prerequisite_count", json_integer(completed_prerequisite_count)},
        {"source_remaining_prerequisite_count", json_integer(remaining_prerequisite_count)},
        {"input_hardening_requirement_count", json_integer(input_hardening_requirement_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"proof_complete", json_bool(true)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });
    const auto evidence_endpoints = json_object({
        {"implementation_candidate_gate_input_hardening_decision_json", json_string(node_v329_json)},
        {"implementation_candidate_gate_input_hardening_decision_markdown", json_string(node_v329_markdown)},
        {"source_node_v328_json", json_string(node_v328_json)},
        {"source_node_v328_markdown", json_string(node_v328_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v151", json_string("Java v151 input-hardening echo")},
        {"recommended_parallel_mini_kv_v143", json_string("mini-kv v143 input-hardening non-participation receipt")},
        {"next_node_version", json_string("Node v330")},
    });
    const auto next_actions = json_array({
        json_string("Archive mini-kv v143 as an implementation candidate gate input-hardening non-participation "
                    "receipt."),
        json_string("Keep Node v330 blocked until Java v151 and mini-kv v143 are both available."),
        json_string("Expose the stable current receipt path through SMOKEJSON, runtime-smoke-evidence.json, "
                    "verification-manifest.json, and the standalone v143 fixture."),
        json_string("Do not read credentials, parse raw endpoints, instantiate providers or clients, send HTTP/TCP, "
                    "write Java ledgers or schema state, auto-start services, or execute mini-kv "
                    "LOAD/COMPACT/RESTORE/SETNXEX/write/admin commands."),
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version", json_string(receipt_version)},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_candidate_gate_decision", json_string(source_candidate_gate_decision)},
        {"source_final_prerequisite_closure", json_string(source_final_prerequisite_closure)},
        {"consumer_hint", json_string(receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(receipt_release_version)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_role", json_string(runtime_role)},
        {"source_node_v329_reference", format_source_node_v329_json()},
        {"mini_kv_receipt", mini_kv_receipt},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", json_array({json_object({{"code", json_string("CANDIDATE_GATE_IS_NOT_RUNTIME_DESIGN")},
                                              {"severity", json_string("warning")}})})},
        {"recommendations",
         json_array({json_object({{"code", json_string("PROVIDE_MINI_KV_STABLE_CURRENT_RECEIPT_EXPORT")},
                                  {"severity", json_string("recommendation")}}),
                     json_object({{"code", json_string("LET_NODE_V330_CONSUME_JAVA_V151_AND_MINI_KV_V143")},
                                  {"severity", json_string("recommendation")}})})},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", json_array({json_string("Java v151"), json_string("mini-kv v143")})},
        {"next_actions", next_actions},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_digest(
             read_commands))},
        {"read_only", json_bool(true)},
        {"execution_allowed", json_bool(false)},
        {"implementation_candidate_gate_input_hardening_non_participation_receipt_only", json_bool(true)},
        {"implementation_candidate_gate_only", json_bool(true)},
        {"read_only_input_hardening_decision_echo", json_bool(true)},
        {"consumes_node_v329_implementation_candidate_gate_input_hardening_decision", json_bool(true)},
        {"consumes_node_v328_final_prerequisite_closure_review", json_bool(true)},
        {"stable_current_receipt_export_required", json_bool(true)},
        {"stable_current_receipt_export_ready", json_bool(true)},
        {"mini_kv_v143_ready_for_node_v330_candidate_gate_upstream_alignment", json_bool(true)},
        {"ready_for_node_v330_candidate_gate_upstream_alignment_after_java_v151", json_bool(true)},
        {"ready_for_node_v330_before_java_v151", json_bool(false)},
        {"ready_for_node_v330_before_upstream_echo", json_bool(false)},
        {"ready_for_disabled_runtime_shell_design_draft", json_bool(false)},
    };
    append_closed_side_effect_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
