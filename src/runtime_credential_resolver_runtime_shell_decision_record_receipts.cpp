#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_disabled_runtime_shell_receipt_fields.hpp"
#include "runtime_credential_resolver_receipt_utils.hpp"
#include "runtime_receipt_json_builder.hpp"

#include "minikv/version.hpp"

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::receipt_digest;
using disabled_runtime_shell_detail::append_non_participation_tail_fields;
using disabled_runtime_shell_detail::append_runtime_shell_readiness_fields;
using runtime_receipt_json::json_array;
using runtime_receipt_json::json_bool;
using runtime_receipt_json::json_integer;
using runtime_receipt_json::json_object;
using runtime_receipt_json::json_string;
using runtime_receipt_json::OrderedJsonField;

constexpr std::string_view receipt_consumer = "Node v300 runtime shell decision record upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-runtime-shell-decision-record-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v132";
constexpr std::string_view receipt_artifact_path_hint = "d/132/";
constexpr std::string_view source_decision_record =
    "Node v299 credential resolver runtime shell candidate gate decision record";
constexpr std::string_view source_upstream_echo = "Node v298 runtime shell candidate gate upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v299_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-decision-record.v1";
constexpr std::string_view node_v299_state = "runtime-shell-candidate-gate-decision-record-ready";
constexpr std::string_view node_v299_decision = "blocked";
constexpr std::string_view node_v299_decision_digest =
    "4f6f73fa2806a9ba74174d7bbab17b43459bd1d790237276d95a3937c646e9c0";
constexpr std::string_view node_v299_record_mode = "runtime-shell-candidate-gate-decision-record-only";
constexpr std::string_view node_v299_decision_scope =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell";
constexpr std::string_view node_v299_source_span = "Node v297-v298 + Java v134 + mini-kv v131";
constexpr std::string_view node_v299_decision_reason =
    "Node v298 verified the runtime shell candidate gate echoes, but runtime shell implementation remains blocked "
    "until a separate successor plan with explicit approval is produced.";

constexpr std::string_view node_v298_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "runtime-shell-candidate-gate-upstream-echo-verification.v1";
constexpr std::string_view node_v298_state = "runtime-shell-candidate-gate-upstream-echo-verification-ready";
constexpr std::string_view node_v298_verification_digest =
    "fdf53be9d847694a7301731be659f694854a95d9b5a7a62b73dd069c6f08aa76";
constexpr std::string_view node_v298_source_span = "Node v297 + Java v134 + mini-kv v131";

constexpr std::string_view active_plan = "docs/plans2/v299-post-runtime-shell-candidate-gate-decision-roadmap.md";
constexpr std::string_view node_v299_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-candidate-gate-decision-record";
constexpr std::string_view node_v299_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-runtime-"
                                                "shell-candidate-gate-decision-record?format=markdown";
constexpr std::string_view node_v298_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-"
                                            "candidate-gate-upstream-echo-verification";
constexpr std::string_view node_v298_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-runtime-"
                                                "shell-candidate-gate-upstream-echo-verification?format=markdown";

constexpr int check_count = 14;
constexpr int source_check_count = 26;
constexpr int required_evidence_count = 4;
constexpr int missing_required_evidence_count = 0;
constexpr int no_go_condition_count = 6;
constexpr int source_required_gate_count = 5;
constexpr int source_documented_gate_count = 5;
constexpr int source_review_evidence_satisfied_count = 5;
constexpr int source_runtime_prerequisite_satisfied_count = 0;
constexpr int source_implementation_allowed_gate_count = 0;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;
constexpr int source_warning_count = 2;
constexpr int source_recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver runtime shell decision record non-participation receipt only; mini-kv echoes Node v299 "
    "blocked decision record, does not implement, enable, or invoke a runtime shell, does not instantiate providers or "
    "resolver clients, does not read credential values, does not parse raw endpoint URLs, does not send HTTP/TCP or "
    "external requests, does not connect managed audit, does not write storage, approval ledger, or schema state, does "
    "not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order authority";
constexpr std::string_view node_action = "verify mini-kv v132 runtime shell decision record non-participation receipt "
                                         "with Java v135 before Node v300 upstream echo verification";

std::string format_required_evidence_json() {
    return "[{\"id\":\"node-v298-upstream-echo-ready\","
           "\"label\":\"Node v298 upstream echo verification\","
           "\"current_evidence\":\"Node v298 readyForUpstreamEchoVerification\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"java-v134-echo-ready\","
           "\"label\":\"Java v134 runtime shell candidate gate echo\","
           "\"current_evidence\":\"present evidence and documented\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"mini-kv-v131-receipt-ready\","
           "\"label\":\"mini-kv v131 runtime shell candidate gate non-participation receipt\","
           "\"current_evidence\":\"present evidence and documented\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true},"
           "{\"id\":\"runtime-shell-still-blocked\","
           "\"label\":\"Runtime shell remains blocked\","
           "\"current_evidence\":\"Node v298 still keeps runtime shell blocked\","
           "\"status\":\"present\",\"required_before_runtime_shell\":true}]";
}

std::string format_no_go_conditions_json() {
    return "[{\"code\":\"RUNTIME_SHELL_IMPLEMENTATION_REQUIRED\","
           "\"condition\":\"The next step would have to implement or invoke a runtime shell.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"CREDENTIAL_VALUE_REQUIRED\","
           "\"condition\":\"The next step would have to read, store, render, or test credential values.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"RAW_ENDPOINT_URL_REQUIRED\","
           "\"condition\":\"The next step would have to parse or render a raw endpoint URL.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"MANAGED_AUDIT_CONNECTION_REQUIRED\","
           "\"condition\":\"The next step would have to open managed audit connectivity.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"LEDGER_SCHEMA_WRITE_REQUIRED\","
           "\"condition\":\"The next step would have to write ledger state or execute schema migration SQL.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"AUTOSTART_REQUIRED\","
           "\"condition\":\"The next step would have to auto-start Java, mini-kv, or managed audit services.\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"}]";
}

std::string format_checks_json() {
    return "{\"source_node_v298_ready\":true,"
           "\"source_node_v298_keeps_implementation_blocked\":true,"
           "\"source_node_v298_keeps_side_effects_closed\":true,"
           "\"decision_record_state_ready\":true,"
           "\"decision_record_is_blocked\":true,"
           "\"decision_digest_recorded\":true,"
           "\"required_evidence_count_stable\":true,"
           "\"required_evidence_present\":true,"
           "\"no_go_condition_count_stable\":true,"
           "\"runtime_implementation_not_authorized\":true,"
           "\"parallel_java_v135_mini_kv_v132_echo_allowed\":true,"
           "\"node_v300_stays_blocked_until_parallel_echo\":true,"
           "\"mini_kv_non_participation_recorded\":true,"
           "\"side_effect_boundaries_closed\":true}";
}

} // namespace

std::string credential_resolver_runtime_shell_decision_record_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_decision_record}},
        {std::string{source_upstream_echo}},
        {std::string{node_v299_profile_version}},
        {std::string{node_v299_state}},
        {std::string{node_v299_decision}},
        {std::string{node_v299_decision_digest}},
        {std::string{node_v299_record_mode}},
        {std::string{node_v299_decision_scope}},
        {std::string{node_v298_profile_version}},
        {std::string{node_v298_state}},
        {std::string{node_v298_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(required_evidence_count)},
        {std::to_string(no_go_condition_count)},
        {"runtime-shell-decision-record-non-participation-receipt-only"},
        {"blocked"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-runtime-shell-decision-record", parts);
}

std::string format_credential_resolver_runtime_shell_decision_record_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v299_reference = json_object({
        {"source_version", json_string("Node v299")},
        {"profile_version", json_string(node_v299_profile_version)},
        {"decision_record_state", json_string(node_v299_state)},
        {"runtime_shell_decision", json_string(node_v299_decision)},
        {"decision_digest", json_string(node_v299_decision_digest)},
        {"record_mode", json_string(node_v299_record_mode)},
        {"decision_scope", json_string(node_v299_decision_scope)},
        {"source_span", json_string(node_v299_source_span)},
        {"decision_reason", json_string(node_v299_decision_reason)},
        {"upstream_echo_verified", json_bool(true)},
        {"ready_for_parallel_java_v135_mini_kv_v132_echo_request", json_bool(true)},
        {"ready_for_node_v300_runtime_shell_decision_record_upstream_echo_verification", json_bool(false)},
        {"allows_node_v300_before_upstream_echo", json_bool(false)},
        {"allows_disabled_runtime_shell_implementation", json_bool(false)},
        {"allows_disabled_runtime_shell_invocation", json_bool(false)},
        {"allows_real_resolver_implementation", json_bool(false)},
        {"allows_fake_harness_runtime_implementation", json_bool(false)},
        {"allows_secret_provider_instantiation", json_bool(false)},
        {"allows_resolver_client_instantiation", json_bool(false)},
        {"allows_credential_value_read", json_bool(false)},
        {"allows_raw_endpoint_url_parse", json_bool(false)},
        {"allows_external_request", json_bool(false)},
        {"allows_managed_audit_connection", json_bool(false)},
        {"allows_schema_migration", json_bool(false)},
        {"allows_approval_ledger_write", json_bool(false)},
        {"allows_automatic_upstream_start", json_bool(false)},
        {"required_evidence_count", json_integer(required_evidence_count)},
        {"missing_required_evidence_count", json_integer(missing_required_evidence_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"required_evidence", format_required_evidence_json()},
        {"explicit_no_go_conditions", format_no_go_conditions_json()},
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
        {"read_only_decision_record", json_bool(true)},
        {"runtime_shell_candidate_gate_decision_record_only", json_bool(true)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_enabled", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto source_node_v298_reference = json_object({
        {"source_version", json_string("Node v298")},
        {"profile_version", json_string(node_v298_profile_version)},
        {"verification_state", json_string(node_v298_state)},
        {"verification_digest", json_string(node_v298_verification_digest)},
        {"source_span", json_string(node_v298_source_span)},
        {"read_only_upstream_echo_verification", json_bool(true)},
        {"runtime_shell_candidate_gate_upstream_echo_verification_only", json_bool(true)},
        {"consumes_java_v134_runtime_shell_candidate_gate_echo", json_bool(true)},
        {"consumes_mini_kv_v131_runtime_shell_candidate_gate_non_participation_receipt", json_bool(true)},
        {"ready_for_node_v299_runtime_shell_candidate_gate_decision_record", json_bool(true)},
        {"ready_for_node_v299_runtime_shell_implementation", json_bool(false)},
        {"check_count", json_integer(source_check_count)},
        {"passed_check_count", json_integer(source_check_count)},
        {"required_gate_count", json_integer(source_required_gate_count)},
        {"documented_gate_count", json_integer(source_documented_gate_count)},
        {"review_evidence_satisfied_count", json_integer(source_review_evidence_satisfied_count)},
        {"runtime_prerequisite_satisfied_count", json_integer(source_runtime_prerequisite_satisfied_count)},
        {"implementation_allowed_gate_count", json_integer(source_implementation_allowed_gate_count)},
        {"production_blocker_count", json_integer(0)},
        {"warning_count", json_integer(source_warning_count)},
        {"recommendation_count", json_integer(source_recommendation_count)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_enabled", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto mini_kv_receipt = json_object({
        {"receipt_mode", json_string("runtime-shell-decision-record-non-participation-receipt-only")},
        {"shell_name", json_string("credential-resolver-disabled-runtime-shell")},
        {"non_participation_scope",
         json_string("runtime shell decision record, implementation, invocation, provider/client wiring, credential "
                     "reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and authority")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(18)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"required_evidence_count", json_integer(required_evidence_count)},
        {"missing_required_evidence_count", json_integer(missing_required_evidence_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"source_node_v298_check_count", json_integer(source_check_count)},
        {"source_node_v298_passed_check_count", json_integer(source_check_count)},
        {"source_node_v298_required_gate_count", json_integer(source_required_gate_count)},
        {"source_node_v298_implementation_allowed_gate_count", json_integer(source_implementation_allowed_gate_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({
        json_object({{"code", json_string("DECISION_RECORD_ONLY_DOES_NOT_AUTHORIZE_RUNTIME")},
                     {"severity", json_string("warning")}}),
    });
    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_JAVA_V135_AND_MINIKV_V132_IN_PARALLEL")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("KEEP_NODE_V300_BEHIND_PARALLEL_EVIDENCE")},
                     {"severity", json_string("recommendation")}}),
    });
    const auto evidence_endpoints = json_object({
        {"runtime_shell_candidate_gate_decision_record_json", json_string(node_v299_json)},
        {"runtime_shell_candidate_gate_decision_record_markdown", json_string(node_v299_markdown)},
        {"source_node_v298_json", json_string(node_v298_json)},
        {"source_node_v298_markdown", json_string(node_v298_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v135", json_string("Java v135 runtime shell decision record echo")},
        {"recommended_parallel_mini_kv_v132",
         json_string("mini-kv v132 runtime shell decision record non-participation receipt")},
        {"next_node_verification", json_string("Node v300")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-runtime-shell-decision-record-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_decision_record", json_string(source_decision_record)},
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
        {"source_node_v299_reference", source_node_v299_reference},
        {"source_node_v298_reference", source_node_v298_reference},
        {"mini_kv_receipt", mini_kv_receipt},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", json_array({json_string("Java v135"), json_string("mini-kv v132")})},
        {"next_actions",
         json_array({
             json_string("Archive mini-kv v132 as a runtime shell decision record non-participation receipt."),
             json_string("Keep Node v300 blocked until Java v135 and mini-kv v132 are both available."),
             json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
             json_string("Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema "
                         "state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX."),
         })},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(
             credential_resolver_runtime_shell_decision_record_non_participation_receipt_digest(read_commands))},
        {"read_only", json_bool(true)},
        {"execution_allowed", json_bool(false)},
        {"runtime_shell_decision_record_non_participation_receipt_only", json_bool(true)},
        {"runtime_shell_candidate_gate_decision_record_only", json_bool(true)},
        {"decision_record_only", json_bool(true)},
        {"decision_record_state", json_string("runtime-shell-candidate-gate-decision-record-ready")},
        {"runtime_shell_decision", json_string("blocked")},
        {"consumes_node_v299_runtime_shell_candidate_gate_decision_record", json_bool(true)},
        {"consumes_node_v298_runtime_shell_candidate_gate_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v300_runtime_shell_decision_record_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v300_runtime_shell_implementation", json_bool(false)},
    };
    append_runtime_shell_readiness_fields(fields);
    append_non_participation_tail_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
