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

constexpr std::string_view receipt_consumer =
    "Node v302 runtime shell post-decision plan intake upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v133";
constexpr std::string_view receipt_artifact_path_hint = "d/133/";
constexpr std::string_view source_plan_intake =
    "Node v301 credential resolver runtime shell post-decision continuation plan intake";
constexpr std::string_view source_upstream_echo = "Node v300 runtime shell decision record upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v301_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "runtime-shell-post-decision-continuation-plan-intake.v1";
constexpr std::string_view node_v301_state = "runtime-shell-post-decision-continuation-plan-intake-ready";
constexpr std::string_view node_v301_intake_digest = "ce85fa1c85cb2759abda5289f9bfec0cf162b71f2da0a12dfdf2ffe7cee8d65a";
constexpr std::string_view node_v301_proof_digest = "9b7d9215e58ce16ad0c2b67f4560005ae3b1436fbf4087fb0ebdfca5c841d00e";
constexpr std::string_view node_v301_selected_decision = "continue-blocked-planning";
constexpr std::string_view node_v301_intake_mode = "runtime-shell-post-decision-continuation-plan-intake-only";
constexpr std::string_view node_v301_source_span = "Node v300";
constexpr std::string_view node_v301_blocker =
    "v300 verified Java v135 and mini-kv v132 agreement with the blocked decision record, but it did not decide the "
    "post-decision continuation path.";
constexpr std::string_view node_v301_consumer = "Java v136 and mini-kv v133 consume v301 as read-only echoes; Node "
                                                "v302 consumes both echoes to verify post-decision plan alignment.";
constexpr std::string_view node_v301_v300_reuse =
    "v300 is an upstream echo verification for Node v299; it lacks a selected continuation option, v136/v133 handoff "
    "target, and explicit stop condition for the post-decision chain.";
constexpr std::string_view node_v301_stop_condition =
    "Stop immediately if the next step requires credential values, raw endpoint URLs, provider/client instantiation, "
    "HTTP/TCP, runtime shell implementation or invocation, ledger/schema writes, or automatic upstream start.";

constexpr std::string_view node_v300_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "runtime-shell-decision-record-upstream-echo-verification.v1";
constexpr std::string_view node_v300_state = "runtime-shell-decision-record-upstream-echo-verification-ready";
constexpr std::string_view node_v300_verification_digest =
    "a7d1cc22ce3f8d6da30d5a91d2d1f8fc13e480babfac8296c4b8ca86519d4f8c";
constexpr std::string_view node_v300_source_span = "Node v299 + Java v135 + mini-kv v132";

constexpr std::string_view active_plan = "docs/plans2/v300-post-runtime-shell-decision-record-upstream-echo-roadmap.md";
constexpr std::string_view node_v301_json =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-continuation-plan-intake";
constexpr std::string_view node_v301_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-runtime-"
                                                "shell-post-decision-continuation-plan-intake?format=markdown";
constexpr std::string_view node_v300_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-"
                                            "decision-record-upstream-echo-verification";
constexpr std::string_view node_v300_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-runtime-"
                                                "shell-decision-record-upstream-echo-verification?format=markdown";

constexpr int check_count = 25;
constexpr int source_check_count = 19;
constexpr int continuation_option_count = 4;
constexpr int selected_option_count = 1;
constexpr int rejected_runtime_option_count = 1;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver runtime shell post-decision plan intake non-participation receipt only; mini-kv echoes Node "
    "v301 blocked continuation plan intake, does not implement, enable, or invoke a runtime shell, does not "
    "instantiate providers or resolver clients, does not read credential values, does not parse raw endpoint URLs, "
    "does not send HTTP/TCP or external requests, does not connect managed audit, does not write storage, approval "
    "ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order "
    "authority";
constexpr std::string_view node_action =
    "verify mini-kv v133 runtime shell post-decision plan intake non-participation receipt with Java v136 before Node "
    "v302 upstream echo verification";

std::string format_continuation_options_json() {
    return "[{\"code\":\"CONTINUE_BLOCKED_PLANNING\",\"status\":\"selected\","
           "\"allowed_actions\":[\"write-v301-intake\",\"request-java-v136-echo\","
           "\"request-mini-kv-v133-non-participation\"],"
           "\"prohibited_actions\":[\"implement-runtime-shell\",\"invoke-runtime-shell\","
           "\"open-managed-audit-connection\"]},"
           "{\"code\":\"PAUSE_RUNTIME_SHELL_CHAIN\",\"status\":\"documented-alternative\","
           "\"allowed_actions\":[\"archive-v301-as-paused\",\"return-to-quality-work\"],"
           "\"prohibited_actions\":[\"treat-pause-as-production-approval\"]},"
           "{\"code\":\"REQUIRE_EXPLICIT_APPROVAL_PREREQUISITES\","
           "\"status\":\"documented-alternative\","
           "\"allowed_actions\":[\"list-approval-prerequisites\",\"keep-prerequisites-read-only\"],"
           "\"prohibited_actions\":[\"read-credential-value\",\"parse-raw-endpoint-url\","
           "\"instantiate-provider-client\"]},"
           "{\"code\":\"IMPLEMENT_RUNTIME_SHELL_NOW\",\"status\":\"rejected\","
           "\"allowed_actions\":[],"
           "\"prohibited_actions\":[\"implement-runtime-shell\",\"invoke-runtime-shell\","
           "\"send-external-request\",\"write-ledger-or-schema\"]}]";
}

std::string format_checks_json() {
    return "{\"source_node_v300_loaded\":true,"
           "\"source_node_v300_ready\":true,"
           "\"source_node_v300_ready_for_post_decision_plan\":true,"
           "\"source_node_v300_keeps_runtime_blocked\":true,"
           "\"source_node_v300_keeps_credential_boundary_closed\":true,"
           "\"source_node_v300_keeps_endpoint_boundary_closed\":true,"
           "\"source_node_v300_keeps_connection_boundary_closed\":true,"
           "\"source_node_v300_keeps_write_boundary_closed\":true,"
           "\"continuation_decision_selected\":true,"
           "\"decision_options_documented\":true,"
           "\"runtime_implementation_option_rejected\":true,"
           "\"necessity_proof_has_blocker\":true,"
           "\"necessity_proof_has_consumer\":true,"
           "\"necessity_proof_explains_v300_reuse_boundary\":true,"
           "\"necessity_proof_defines_stop_condition\":true,"
           "\"necessity_proof_complete\":true,"
           "\"runtime_shell_implementation_still_forbidden\":true,"
           "\"runtime_shell_invocation_still_forbidden\":true,"
           "\"provider_client_instantiation_still_forbidden\":true,"
           "\"external_request_still_forbidden\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"mini_kv_non_participation_recorded\":true}";
}

} // namespace

std::string credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_plan_intake}},
        {std::string{source_upstream_echo}},
        {std::string{node_v301_profile_version}},
        {std::string{node_v301_state}},
        {std::string{node_v301_intake_digest}},
        {std::string{node_v301_proof_digest}},
        {std::string{node_v301_selected_decision}},
        {std::string{node_v301_intake_mode}},
        {std::string{node_v300_profile_version}},
        {std::string{node_v300_state}},
        {std::string{node_v300_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(continuation_option_count)},
        {std::to_string(selected_option_count)},
        {std::to_string(rejected_runtime_option_count)},
        {"runtime-shell-post-decision-plan-intake-non-participation-receipt-only"},
        {"continue-blocked-planning"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-runtime-shell-post-decision-plan-intake", parts);
}

std::string format_credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v301_reference = json_object({
        {"source_version", json_string("Node v301")},
        {"profile_version", json_string(node_v301_profile_version)},
        {"plan_intake_state", json_string(node_v301_state)},
        {"intake_digest", json_string(node_v301_intake_digest)},
        {"proof_digest", json_string(node_v301_proof_digest)},
        {"intake_mode", json_string(node_v301_intake_mode)},
        {"source_span", json_string(node_v301_source_span)},
        {"selected_continuation_decision", json_string(node_v301_selected_decision)},
        {"decision_option_count", json_integer(continuation_option_count)},
        {"selected_decision_option_count", json_integer(selected_option_count)},
        {"rejected_runtime_implementation_option_count", json_integer(rejected_runtime_option_count)},
        {"next_java_echo_version", json_string("Java v136")},
        {"next_mini_kv_receipt_version", json_string("mini-kv v133")},
        {"next_node_verification_version", json_string("Node v302")},
        {"blocker_resolved", json_string(node_v301_blocker)},
        {"consumer", json_string(node_v301_consumer)},
        {"why_v300_cannot_be_reused", json_string(node_v301_v300_reuse)},
        {"stop_condition", json_string(node_v301_stop_condition)},
        {"proof_complete", json_bool(true)},
        {"continuation_options", format_continuation_options_json()},
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
        {"read_only_plan_intake", json_bool(true)},
        {"runtime_shell_post_decision_continuation_plan_intake_only", json_bool(true)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_enabled", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"runtime_shell_implementation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"raw_endpoint_url_rendered", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto source_node_v300_reference = json_object({
        {"source_version", json_string("Node v300")},
        {"profile_version", json_string(node_v300_profile_version)},
        {"verification_state", json_string(node_v300_state)},
        {"verification_digest", json_string(node_v300_verification_digest)},
        {"source_span", json_string(node_v300_source_span)},
        {"read_only_upstream_echo_verification", json_bool(true)},
        {"runtime_shell_decision_record_upstream_echo_verification_only", json_bool(true)},
        {"consumes_node_v299_runtime_shell_candidate_gate_decision_record", json_bool(true)},
        {"consumes_java_v135_runtime_shell_decision_record_echo", json_bool(true)},
        {"consumes_mini_kv_v132_runtime_shell_decision_record_non_participation_receipt", json_bool(true)},
        {"ready_for_post_runtime_shell_decision_plan", json_bool(true)},
        {"upstream_echo_aligned", json_bool(true)},
        {"blocked_decision_aligned", json_bool(true)},
        {"required_evidence_aligned", json_bool(true)},
        {"no_go_conditions_aligned", json_bool(true)},
        {"side_effect_boundaries_aligned", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
        {"check_count", json_integer(source_check_count)},
        {"passed_check_count", json_integer(source_check_count)},
        {"production_blocker_count", json_integer(0)},
        {"warning_count", json_integer(1)},
        {"recommendation_count", json_integer(2)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_enabled", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"credential_value_read", json_bool(false)},
        {"credential_value_provided", json_bool(false)},
        {"raw_endpoint_url_parsed", json_bool(false)},
        {"raw_endpoint_url_rendered", json_bool(false)},
        {"external_request_sent", json_bool(false)},
        {"secret_provider_instantiated", json_bool(false)},
        {"resolver_client_instantiated", json_bool(false)},
        {"fake_secret_provider_instantiated", json_bool(false)},
        {"fake_resolver_client_instantiated", json_bool(false)},
        {"schema_migration_executed", json_bool(false)},
        {"approval_ledger_written", json_bool(false)},
        {"automatic_upstream_start", json_bool(false)},
    });

    const auto mini_kv_receipt = json_object({
        {"receipt_mode", json_string("runtime-shell-post-decision-plan-intake-non-participation-receipt-only")},
        {"shell_name", json_string("credential-resolver-disabled-runtime-shell")},
        {"non_participation_scope",
         json_string("post-decision plan intake, runtime shell implementation, invocation, provider/client wiring, "
                     "credential reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and "
                     "authority")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(25)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v300_check_count", json_integer(source_check_count)},
        {"source_node_v300_passed_check_count", json_integer(source_check_count)},
        {"continuation_option_count", json_integer(continuation_option_count)},
        {"selected_continuation_option_count", json_integer(selected_option_count)},
        {"rejected_runtime_implementation_option_count", json_integer(rejected_runtime_option_count)},
        {"proof_complete", json_bool(true)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({
        json_object({{"code", json_string("CONTINUATION_PLAN_DOES_NOT_AUTHORIZE_RUNTIME")},
                     {"severity", json_string("warning")}}),
    });
    const auto recommendations = json_array({
        json_object(
            {{"code", json_string("REQUEST_PARALLEL_JAVA_MINI_KV_ECHO")}, {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("STOP_CHAIN_AFTER_V302_WITHOUT_NEW_BLOCKER")},
                     {"severity", json_string("recommendation")}}),
    });
    const auto evidence_endpoints = json_object({
        {"runtime_shell_post_decision_plan_intake_json", json_string(node_v301_json)},
        {"runtime_shell_post_decision_plan_intake_markdown", json_string(node_v301_markdown)},
        {"source_node_v300_json", json_string(node_v300_json)},
        {"source_node_v300_markdown", json_string(node_v300_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v136", json_string("Java v136 post-decision plan intake echo")},
        {"recommended_parallel_mini_kv_v133",
         json_string("mini-kv v133 post-decision plan intake non-participation receipt")},
        {"next_node_verification", json_string("Node v302")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string(
             "mini-kv-credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_plan_intake", json_string(source_plan_intake)},
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
        {"source_node_v301_reference", source_node_v301_reference},
        {"source_node_v300_reference", source_node_v300_reference},
        {"mini_kv_receipt", mini_kv_receipt},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", json_array({json_string("Java v136"), json_string("mini-kv v133")})},
        {"next_actions",
         json_array({
             json_string(
                 "Archive mini-kv v133 as a runtime shell post-decision plan intake non-participation receipt."),
             json_string("Keep Node v302 blocked until Java v136 and mini-kv v133 are both available."),
             json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
             json_string("Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema "
                         "state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX."),
         })},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_digest(
             read_commands))},
        {"read_only", json_bool(true)},
        {"execution_allowed", json_bool(false)},
        {"runtime_shell_post_decision_plan_intake_non_participation_receipt_only", json_bool(true)},
        {"runtime_shell_post_decision_continuation_plan_intake_only", json_bool(true)},
        {"post_decision_plan_intake_only", json_bool(true)},
        {"plan_intake_state", json_string("runtime-shell-post-decision-continuation-plan-intake-ready")},
        {"selected_continuation_decision", json_string("continue-blocked-planning")},
        {"consumes_node_v301_runtime_shell_post_decision_plan_intake", json_bool(true)},
        {"consumes_node_v300_runtime_shell_decision_record_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v302_post_decision_plan_intake_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v302_runtime_shell_implementation", json_bool(false)},
    };
    append_runtime_shell_readiness_fields(fields);
    append_non_participation_tail_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
