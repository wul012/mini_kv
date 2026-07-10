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

constexpr std::string_view receipt_consumer = "Node v298 runtime shell candidate gate upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v131";
constexpr std::string_view receipt_artifact_path_hint = "d/131/";
constexpr std::string_view source_candidate_gate =
    "Node v297 credential resolver disabled runtime shell implementation candidate gate";
constexpr std::string_view source_upstream_echo = "Node v296 disabled runtime shell upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v297_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "disabled-runtime-shell-implementation-candidate-gate.v1";
constexpr std::string_view node_v297_state = "disabled-runtime-shell-implementation-candidate-gate-reviewed";
constexpr std::string_view node_v297_gate_version = "node-v297-disabled-runtime-shell-implementation-candidate-gate.v1";
constexpr std::string_view node_v297_gate_mode = "candidate-gate-only-default-blocked";
constexpr std::string_view node_v297_gate_decision =
    "blocked-request-parallel-java-v134-and-mini-kv-v131-before-implementation";
constexpr std::string_view node_v297_gate_digest = "651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22";
constexpr std::string_view node_v297_decision_rationale =
    "Node v296 proves the three-project upstream echo is aligned, but a runtime shell still needs a dedicated "
    "disabled-by-default flag, operator approval, abort semantics, no-network tests, and fallback evidence echoed by "
    "Java and mini-kv before any later implementation decision.";
constexpr std::string_view node_v297_blocker = "candidate-gate-lacks-upstream-echo-and-runtime-prerequisite-proof";
constexpr std::string_view node_v297_cannot_reuse_reason =
    "Node v296 verified upstream echo evidence only; it did not enumerate the runtime-shell candidate prerequisites or "
    "publish a blocked decision that Java v134 and mini-kv v131 can echo.";
constexpr std::string_view node_v297_stop_condition =
    "Stop extending the candidate gate once Java v134 and mini-kv v131 echo the blocked decision and Node v298 "
    "verifies both echoes.";

constexpr std::string_view node_v296_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-upstream-echo-verification.v1";
constexpr std::string_view node_v296_state = "disabled-runtime-shell-upstream-echo-verification-ready";
constexpr std::string_view node_v296_verification_digest =
    "9bbba02d888b5c986026f3e80e0c07df27d08699405d9989cc42e9bd1c8c6f7b";

constexpr std::string_view active_plan =
    "docs/plans2/v297-post-disabled-runtime-shell-implementation-candidate-gate-roadmap.md";
constexpr std::string_view node_v297_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-implementation-candidate-gate";
constexpr std::string_view node_v297_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-disabled-"
                                                "runtime-shell-implementation-candidate-gate?format=markdown";
constexpr std::string_view node_v296_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-upstream-echo-verification";
constexpr std::string_view node_v296_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-disabled-"
                                                "runtime-shell-upstream-echo-verification?format=markdown";

constexpr int check_count = 27;
constexpr int source_check_count = 24;
constexpr int required_gate_count = 5;
constexpr int documented_gate_count = 5;
constexpr int review_evidence_satisfied_count = 5;
constexpr int runtime_prerequisite_satisfied_count = 0;
constexpr int implementation_allowed_gate_count = 0;
constexpr int stop_condition_count = 6;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 2;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver disabled runtime shell candidate gate non-participation receipt only; mini-kv echoes Node "
    "v297 blocked implementation candidate gate, does not implement, enable, or invoke a runtime shell, does not "
    "instantiate providers or resolver clients, does not read credential values, does not parse raw endpoint URLs, "
    "does not send HTTP/TCP or external requests, does not connect managed audit, does not write storage, approval "
    "ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order "
    "authority";
constexpr std::string_view node_action = "verify mini-kv v131 runtime shell candidate gate non-participation receipt "
                                         "with Java v134 before Node v298 upstream echo verification";

std::string format_required_gates_json() {
    return "[{\"code\":\"DEDICATED_DISABLED_BY_DEFAULT_FLAG\",\"title\":\"Dedicated disabled-by-default flag\","
           "\"owner\":\"node\",\"failure_class\":\"missing-disabled-runtime-shell-flag\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"OPERATOR_APPROVAL\",\"title\":\"Operator approval marker\","
           "\"owner\":\"operator\",\"failure_class\":\"operator-approval-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"ABORT_SEMANTICS\",\"title\":\"Abort semantics\","
           "\"owner\":\"release\",\"failure_class\":\"abort-semantics-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"NO_NETWORK_TESTS\",\"title\":\"No-network tests\","
           "\"owner\":\"test\",\"failure_class\":\"no-network-tests-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false},"
           "{\"code\":\"HISTORICAL_FALLBACK_EVIDENCE\",\"title\":\"Historical fallback evidence\","
           "\"owner\":\"release\",\"failure_class\":\"historical-fallback-required\","
           "\"documented_for_gate_review\":true,\"review_evidence_satisfied\":true,"
           "\"runtime_prerequisite_satisfied\":false,\"implementation_allowed\":false}]";
}

std::string format_stop_conditions_json() {
    return "[\"Any candidate asks Node to implement, enable, or invoke a runtime shell.\","
           "\"Any candidate asks Node to instantiate a provider or resolver client.\","
           "\"Any candidate asks Node, Java, or mini-kv to read credential values or parse raw endpoint URLs.\","
           "\"Any candidate asks Node to send HTTP/TCP to managed audit.\","
           "\"Any candidate asks Java to write approval ledger records, execute SQL, deploy, or roll back.\","
           "\"Any candidate asks mini-kv to execute LOAD/COMPACT/RESTORE/SETNXEX or become audit/order authority.\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v296_ready\":true,"
           "\"source_node_v296_keeps_implementation_blocked\":true,"
           "\"source_node_v296_keeps_side_effects_closed\":true,"
           "\"candidate_gate_count_stable\":true,"
           "\"all_candidate_gates_documented\":true,"
           "\"all_candidate_gates_review_evidence_satisfied\":true,"
           "\"candidate_gate_keeps_runtime_blocked\":true,"
           "\"dedicated_disabled_by_default_flag_required\":true,"
           "\"operator_approval_required\":true,"
           "\"abort_semantics_required\":true,"
           "\"no_network_tests_required\":true,"
           "\"historical_fallback_evidence_required\":true,"
           "\"necessity_documented\":true,"
           "\"parallel_upstream_echo_recommended\":true,"
           "\"mini_kv_non_participation_recorded\":true,"
           "\"no_runtime_implementation_created\":true,"
           "\"no_runtime_invocation_allowed\":true,"
           "\"credential_boundary_closed\":true,"
           "\"raw_endpoint_boundary_closed\":true,"
           "\"provider_client_boundary_closed\":true,"
           "\"connection_boundary_closed\":true,"
           "\"write_boundary_closed\":true,"
           "\"auto_start_boundary_closed\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"ready_for_node_v298_upstream_echo\":true}";
}

} // namespace

std::string credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_candidate_gate}},
        {std::string{source_upstream_echo}},
        {std::string{node_v297_profile_version}},
        {std::string{node_v297_state}},
        {std::string{node_v297_gate_version}},
        {std::string{node_v297_gate_mode}},
        {std::string{node_v297_gate_decision}},
        {std::string{node_v297_gate_digest}},
        {std::string{node_v296_profile_version}},
        {std::string{node_v296_state}},
        {std::string{node_v296_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(required_gate_count)},
        {std::to_string(review_evidence_satisfied_count)},
        {std::to_string(runtime_prerequisite_satisfied_count)},
        {std::to_string(implementation_allowed_gate_count)},
        {"runtime-shell-candidate-gate-non-participation-receipt-only"},
        {"blocked"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-disabled-runtime-shell-candidate-gate", parts);
}

std::string format_credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto necessity = json_object({
        {"blocker", json_string(node_v297_blocker)},
        {"consumer", json_string("Java v134 and mini-kv v131, then Node v298")},
        {"cannot_reuse_existing_report_reason", json_string(node_v297_cannot_reuse_reason)},
        {"stop_condition", json_string(node_v297_stop_condition)},
    });

    const auto source_node_v297_reference = json_object({
        {"source_version", json_string("Node v297")},
        {"profile_version", json_string(node_v297_profile_version)},
        {"candidate_gate_state", json_string(node_v297_state)},
        {"gate_version", json_string(node_v297_gate_version)},
        {"gate_mode", json_string(node_v297_gate_mode)},
        {"gate_decision", json_string(node_v297_gate_decision)},
        {"gate_digest", json_string(node_v297_gate_digest)},
        {"decision_rationale", json_string(node_v297_decision_rationale)},
        {"necessity", necessity},
        {"required_gates", format_required_gates_json()},
        {"stop_conditions", format_stop_conditions_json()},
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_check_count", json_integer(source_check_count)},
        {"required_gate_count", json_integer(required_gate_count)},
        {"documented_gate_count", json_integer(documented_gate_count)},
        {"review_evidence_satisfied_count", json_integer(review_evidence_satisfied_count)},
        {"runtime_prerequisite_satisfied_count", json_integer(runtime_prerequisite_satisfied_count)},
        {"implementation_allowed_gate_count", json_integer(implementation_allowed_gate_count)},
        {"stop_condition_count", json_integer(stop_condition_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
        {"read_only_candidate_gate", json_bool(true)},
        {"implementation_candidate_gate_only", json_bool(true)},
        {"ready_for_parallel_java_v134_mini_kv_v131_echo_request", json_bool(true)},
        {"ready_for_node_v298_runtime_shell_candidate_gate_upstream_echo_verification", json_bool(false)},
        {"ready_for_node_v298_runtime_shell_implementation", json_bool(false)},
        {"ready_for_disabled_runtime_shell_implementation", json_bool(false)},
        {"ready_for_disabled_runtime_shell_invocation", json_bool(false)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_enabled", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto source_node_v296_reference = json_object({
        {"source_version", json_string("Node v296")},
        {"profile_version", json_string(node_v296_profile_version)},
        {"verification_state", json_string(node_v296_state)},
        {"verification_digest", json_string(node_v296_verification_digest)},
        {"source_span", json_string("Node v295 + Java v133 + mini-kv v130")},
        {"ready_for_upstream_echo_verification", json_bool(true)},
        {"read_only_upstream_echo_verification", json_bool(true)},
        {"disabled_runtime_shell_upstream_echo_verification_only", json_bool(true)},
        {"plan_version_correction_applied", json_bool(true)},
        {"planned_java_version", json_string("Java v132")},
        {"actual_java_echo_version", json_string("Java v133")},
        {"consumes_java_v133", json_bool(true)},
        {"consumes_mini_kv_v130", json_bool(true)},
        {"ready_for_node_v297_candidate_gate", json_bool(true)},
        {"ready_for_node_v297_runtime_shell_implementation", json_bool(false)},
        {"check_count", json_integer(source_check_count)},
        {"passed_check_count", json_integer(source_check_count)},
        {"production_blocker_count", json_integer(0)},
        {"warning_count", json_integer(2)},
        {"recommendation_count", json_integer(2)},
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
        {"receipt_mode", json_string("runtime-shell-candidate-gate-non-participation-receipt-only")},
        {"shell_name", json_string("credential-resolver-disabled-runtime-shell")},
        {"non_participation_scope",
         json_string("runtime shell candidate gate, implementation, invocation, provider/client wiring, credential "
                     "reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and authority")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(20)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v297_check_count", json_integer(check_count)},
        {"source_node_v297_passed_check_count", json_integer(check_count)},
        {"source_node_v296_check_count", json_integer(source_check_count)},
        {"source_node_v296_passed_check_count", json_integer(source_check_count)},
        {"required_gate_count", json_integer(required_gate_count)},
        {"documented_gate_count", json_integer(documented_gate_count)},
        {"review_evidence_satisfied_count", json_integer(review_evidence_satisfied_count)},
        {"runtime_prerequisite_satisfied_count", json_integer(runtime_prerequisite_satisfied_count)},
        {"implementation_allowed_gate_count", json_integer(implementation_allowed_gate_count)},
        {"stop_condition_count", json_integer(stop_condition_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({
        json_object(
            {{"code", json_string("CANDIDATE_GATE_ONLY_DEFAULT_BLOCKED")}, {"severity", json_string("warning")}}),
        json_object(
            {{"code", json_string("UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V298")}, {"severity", json_string("warning")}}),
    });
    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_PARALLEL_JAVA_V134_MINI_KV_V131")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("VERIFY_WITH_NODE_V298_AFTER_UPSTREAM_ECHO")},
                     {"severity", json_string("recommendation")}}),
    });
    const auto evidence_endpoints = json_object({
        {"disabled_runtime_shell_implementation_candidate_gate_json", json_string(node_v297_json)},
        {"disabled_runtime_shell_implementation_candidate_gate_markdown", json_string(node_v297_markdown)},
        {"source_node_v296_json", json_string(node_v296_json)},
        {"source_node_v296_markdown", json_string(node_v296_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v134",
         json_string("Java v134 runtime shell candidate gate echo; read-only blocked decision only")},
        {"recommended_parallel_mini_kv_v131",
         json_string("mini-kv v131 runtime shell candidate gate non-participation receipt")},
        {"next_node_verification", json_string("Node v298")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_candidate_gate", json_string(source_candidate_gate)},
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
        {"source_node_v297_reference", source_node_v297_reference},
        {"source_node_v296_reference", source_node_v296_reference},
        {"mini_kv_receipt", mini_kv_receipt},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", json_array({json_string("Java v134"), json_string("mini-kv v131")})},
        {"next_actions",
         json_array({
             json_string("Archive mini-kv v131 as a runtime shell candidate gate non-participation receipt."),
             json_string("Keep Node v298 blocked until Java v134 and mini-kv v131 are both available."),
             json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
             json_string("Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema "
                         "state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX."),
         })},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_digest(
             read_commands))},
        {"read_only", json_bool(true)},
        {"execution_allowed", json_bool(false)},
        {"candidate_gate_non_participation_receipt_only", json_bool(true)},
        {"runtime_shell_candidate_gate_non_participation_receipt_only", json_bool(true)},
        {"implementation_candidate_gate_only", json_bool(true)},
        {"consumes_node_v297_disabled_runtime_shell_implementation_candidate_gate", json_bool(true)},
        {"consumes_node_v296_disabled_runtime_shell_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v298_runtime_shell_candidate_gate_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v298_runtime_shell_implementation", json_bool(false)},
    };
    append_runtime_shell_readiness_fields(fields);
    fields.push_back({"runtime_prerequisite_satisfied", json_bool(false)});
    fields.push_back({"implementation_allowed", json_bool(false)});
    fields.push_back({"implementation_candidate_allowed", json_bool(false)});
    append_non_participation_tail_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
