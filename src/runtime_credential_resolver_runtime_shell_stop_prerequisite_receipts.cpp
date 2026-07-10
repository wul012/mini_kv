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
    "Node v305 runtime shell chain stop/prerequisite upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v134";
constexpr std::string_view receipt_artifact_path_hint = "d/134/";
constexpr std::string_view source_decision_record =
    "Node v304 runtime shell chain stop-or-prerequisite decision record";
constexpr std::string_view source_upstream_echo = "Node v303 post-decision plan intake upstream echo verification";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v304_profile_version = "managed-audit-manual-sandbox-connection-credential-resolver-"
                                                       "runtime-shell-chain-stop-or-prerequisite-decision-record.v1";
constexpr std::string_view node_v304_state = "runtime-shell-chain-stop-or-prerequisite-decision-record-ready";
constexpr std::string_view node_v304_decision = "require-explicit-approval-prerequisites-before-runtime-shell";
constexpr std::string_view node_v304_decision_digest =
    "9212d0b804fdc1eda9098ac70d2441681730a98ff736776859811df9e288a654";
constexpr std::string_view node_v304_record_mode = "runtime-shell-chain-stop-or-prerequisite-decision-record-only";
constexpr std::string_view node_v304_decision_scope =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell";
constexpr std::string_view node_v304_source_span = "Node v303 + Java v136 + mini-kv v133";
constexpr std::string_view node_v304_decision_reason =
    "Node v303 aligned the post-decision intake echoes, but the runtime shell chain still lacks explicit operator "
    "approval, credential-handle readiness, no-network safety tests, abort semantics, and upstream echo of the "
    "prerequisite decision.";
constexpr std::string_view node_v304_selected_path = "continue-only-as-blocked-prerequisite-review";
constexpr std::string_view node_v304_blocker =
    "v303 aligned the post-decision plan intake echoes, but did not decide whether the runtime shell chain should stop "
    "or require explicit approval prerequisites.";
constexpr std::string_view node_v304_consumer = "Java v141 and mini-kv v134, then Node v305";
constexpr std::string_view node_v304_v303_reuse =
    "v303 verifies upstream echo alignment only; it does not enumerate missing prerequisites or publish a decision "
    "that Java and mini-kv can echo before any later runtime-shell discussion.";
constexpr std::string_view node_v304_stop_condition =
    "Stop immediately if the next step requires credential values, raw endpoint URLs, provider/client instantiation, "
    "HTTP/TCP, runtime shell implementation or invocation, ledger/schema writes, mini-kv write/admin commands, or "
    "automatic upstream start.";

constexpr std::string_view node_v303_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-plan-intake-upstream-echo-"
    "verification.v1";
constexpr std::string_view node_v303_state = "runtime-shell-post-decision-plan-intake-upstream-echo-verification-ready";
constexpr std::string_view node_v303_verification_digest =
    "9a2f807d01e34c632ee02ac40db98f5a217a7f186d6305c6b97309a6a7d257df";
constexpr std::string_view node_v303_source_span = "Node v301 + Node v302 + Java v136 + mini-kv v133";

constexpr std::string_view active_plan = "docs/plans2/v303-post-decision-plan-intake-upstream-echo-roadmap.md";
constexpr std::string_view node_v304_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-"
                                            "chain-stop-or-prerequisite-decision-record";
constexpr std::string_view node_v304_markdown = "/api/v1/audit/"
                                                "managed-audit-manual-sandbox-connection-credential-resolver-runtime-"
                                                "shell-chain-stop-or-prerequisite-decision-record?format=markdown";
constexpr std::string_view node_v303_json = "/api/v1/audit/"
                                            "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-"
                                            "post-decision-plan-intake-upstream-echo-verification";
constexpr std::string_view node_v303_markdown =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-runtime-shell-post-decision-plan-intake-upstream-echo-"
    "verification?format=markdown";

constexpr int check_count = 17;
constexpr int source_check_count = 23;
constexpr int source_production_blocker_count = 0;
constexpr int source_warning_count = 2;
constexpr int source_recommendation_count = 2;
constexpr int prerequisite_count = 6;
constexpr int missing_runtime_prerequisite_count = 6;
constexpr int no_go_condition_count = 8;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 1;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver runtime shell chain stop/prerequisite non-participation receipt only; mini-kv echoes Node "
    "v304 stop-or-prerequisite decision record, does not implement, enable, or invoke a runtime shell, does not "
    "instantiate providers or resolver clients, does not read credential values, does not parse raw endpoint URLs, "
    "does not send HTTP/TCP or external requests, does not connect managed audit, does not write storage, approval "
    "ledger, or schema state, does not execute LOAD/COMPACT/RESTORE/SETNXEX, and does not become audit or order "
    "authority";
constexpr std::string_view node_action = "verify mini-kv v134 runtime shell chain stop/prerequisite non-participation "
                                         "receipt with Java v141 before Node v305 upstream echo verification";

std::string format_required_prerequisites_json() {
    return "[{\"id\":\"operator-approval-artifact\","
           "\"label\":\"Operator approval artifact\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: no signed operator approval artifact has been produced for runtime shell "
           "implementation\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"credential-handle-readiness\","
           "\"label\":\"Credential handle readiness\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: only credential handle/review status can be referenced; credential value "
           "reading remains forbidden\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"raw-endpoint-allowlist-review\","
           "\"label\":\"Raw endpoint allowlist review\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: endpoint handle can be reviewed, but raw endpoint URL parsing/rendering "
           "remains forbidden\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"no-network-test-fixture\","
           "\"label\":\"No-network safety tests\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: no test has proven runtime shell code cannot dial managed audit before "
           "explicit approval\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"manual-abort-and-rollback-semantics\","
           "\"label\":\"Manual abort and rollback semantics\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: abort semantics are documented as required, but no executable runtime "
           "shell abort contract exists\","
           "\"required_before_runtime_shell\":true},"
           "{\"id\":\"java-mini-kv-prerequisite-echo\","
           "\"label\":\"Java/mini-kv prerequisite echo\","
           "\"status\":\"documented-missing\","
           "\"current_evidence\":\"missing: Java v141 and mini-kv v134 have not yet echoed this stop/prerequisite "
           "decision\","
           "\"required_before_runtime_shell\":true}]";
}

std::string format_no_go_conditions_json() {
    return "[{\"code\":\"RUNTIME_SHELL_IMPLEMENTATION_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"RUNTIME_SHELL_INVOCATION_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"CREDENTIAL_VALUE_READ_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"RAW_ENDPOINT_URL_PARSE_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"PROVIDER_CLIENT_INSTANTIATION_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"EXTERNAL_REQUEST_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"LEDGER_OR_SCHEMA_WRITE_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"},"
           "{\"code\":\"MINIKV_WRITE_OR_AUTHORITY_REQUESTED\","
           "\"action\":\"pause-and-do-not-implement-runtime-shell\"}]";
}

std::string format_checks_json() {
    return "{\"source_node_v303_loaded\":true,"
           "\"source_node_v303_ready\":true,"
           "\"source_node_v303_upstream_echo_aligned\":true,"
           "\"source_node_v303_keeps_runtime_blocked\":true,"
           "\"source_node_v303_keeps_side_effects_closed\":true,"
           "\"decision_selects_prerequisite_gate\":true,"
           "\"decision_record_blocks_runtime_shell\":true,"
           "\"decision_record_still_read_only\":true,"
           "\"required_prerequisites_documented\":true,"
           "\"missing_runtime_prerequisites_block_implementation\":true,"
           "\"necessity_proof_complete\":true,"
           "\"parallel_java_v141_mini_kv_v134_echo_recommended\":true,"
           "\"upstream_probes_still_disabled\":true,"
           "\"upstream_actions_still_disabled\":true,"
           "\"production_audit_still_blocked\":true,"
           "\"production_window_still_blocked\":true,"
           "\"mini_kv_non_participation_recorded\":true}";
}

} // namespace

std::string credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_decision_record}},
        {std::string{source_upstream_echo}},
        {std::string{node_v304_profile_version}},
        {std::string{node_v304_state}},
        {std::string{node_v304_decision}},
        {std::string{node_v304_decision_digest}},
        {std::string{node_v304_record_mode}},
        {std::string{node_v304_selected_path}},
        {std::string{node_v303_profile_version}},
        {std::string{node_v303_state}},
        {std::string{node_v303_verification_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(prerequisite_count)},
        {std::to_string(missing_runtime_prerequisite_count)},
        {std::to_string(no_go_condition_count)},
        {"runtime-shell-chain-stop-or-prerequisite-non-participation-receipt-only"},
        {"require-explicit-approval-prerequisites-before-runtime-shell"},
        {"continue-only-as-blocked-prerequisite-review"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-runtime-shell-chain-stop-or-prerequisite", parts);
}

std::string format_credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v304_reference = json_object({
        {"source_version", json_string("Node v304")},
        {"profile_version", json_string(node_v304_profile_version)},
        {"decision_record_state", json_string(node_v304_state)},
        {"runtime_shell_chain_decision", json_string(node_v304_decision)},
        {"decision_digest", json_string(node_v304_decision_digest)},
        {"record_mode", json_string(node_v304_record_mode)},
        {"decision_scope", json_string(node_v304_decision_scope)},
        {"source_span", json_string(node_v304_source_span)},
        {"decision", json_string(node_v304_decision)},
        {"decision_reason", json_string(node_v304_decision_reason)},
        {"selected_path", json_string(node_v304_selected_path)},
        {"stop_runtime_shell_chain_without_prerequisites", json_bool(true)},
        {"allows_parallel_java_v141_mini_kv_v134_echo_request", json_bool(true)},
        {"ready_for_node_v305_stop_prerequisite_upstream_echo_verification", json_bool(false)},
        {"allows_node_v305_before_upstream_echo", json_bool(false)},
        {"allows_disabled_runtime_shell_implementation", json_bool(false)},
        {"allows_disabled_runtime_shell_invocation", json_bool(false)},
        {"allows_real_resolver_implementation", json_bool(false)},
        {"allows_secret_provider_instantiation", json_bool(false)},
        {"allows_resolver_client_instantiation", json_bool(false)},
        {"allows_credential_value_read", json_bool(false)},
        {"allows_raw_endpoint_url_parse", json_bool(false)},
        {"allows_external_request", json_bool(false)},
        {"allows_managed_audit_connection", json_bool(false)},
        {"allows_schema_migration", json_bool(false)},
        {"allows_approval_ledger_write", json_bool(false)},
        {"allows_automatic_upstream_start", json_bool(false)},
        {"prerequisite_count", json_integer(prerequisite_count)},
        {"missing_runtime_prerequisite_count", json_integer(missing_runtime_prerequisite_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"required_prerequisites", format_required_prerequisites_json()},
        {"explicit_no_go_conditions", format_no_go_conditions_json()},
        {"blocker_resolved", json_string(node_v304_blocker)},
        {"consumer", json_string(node_v304_consumer)},
        {"why_v303_cannot_be_reused", json_string(node_v304_v303_reuse)},
        {"existing_report_reuse_decision",
         json_string("Reuse v303 as source evidence, but create v304 as the minimal decision layer that records "
                     "stop/prerequisite requirements.")},
        {"stop_condition", json_string(node_v304_stop_condition)},
        {"proof_complete", json_bool(true)},
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
        {"read_only_decision_record", json_bool(true)},
        {"runtime_shell_chain_stop_or_prerequisite_decision_record_only", json_bool(true)},
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

    const auto source_node_v303_reference = json_object({
        {"source_version", json_string("Node v303")},
        {"profile_version", json_string(node_v303_profile_version)},
        {"verification_state", json_string(node_v303_state)},
        {"verification_digest", json_string(node_v303_verification_digest)},
        {"source_span", json_string(node_v303_source_span)},
        {"active_node_verification_version", json_string("Node v303")},
        {"legacy_node_v302_consumer_marker_accepted", json_bool(true)},
        {"source_node_v301_ready", json_bool(true)},
        {"source_node_v302_quality_pass_ready", json_bool(true)},
        {"java_v136_echo_ready", json_bool(true)},
        {"mini_kv_v133_receipt_ready", json_bool(true)},
        {"upstream_echo_aligned", json_bool(true)},
        {"side_effect_boundaries_aligned", json_bool(true)},
        {"implementation_still_blocked", json_bool(true)},
        {"check_count", json_integer(source_check_count)},
        {"passed_check_count", json_integer(source_check_count)},
        {"production_blocker_count", json_integer(source_production_blocker_count)},
        {"warning_count", json_integer(source_warning_count)},
        {"recommendation_count", json_integer(source_recommendation_count)},
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
        {"receipt_mode", json_string("runtime-shell-chain-stop-or-prerequisite-non-participation-receipt-only")},
        {"shell_name", json_string("credential-resolver-disabled-runtime-shell")},
        {"non_participation_scope",
         json_string("stop/prerequisite decision, runtime shell implementation, invocation, provider/client wiring, "
                     "credential reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and "
                     "authority")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(17)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v303_check_count", json_integer(source_check_count)},
        {"source_node_v303_passed_check_count", json_integer(source_check_count)},
        {"source_node_v303_production_blocker_count", json_integer(source_production_blocker_count)},
        {"source_node_v303_warning_count", json_integer(source_warning_count)},
        {"source_node_v303_recommendation_count", json_integer(source_recommendation_count)},
        {"prerequisite_count", json_integer(prerequisite_count)},
        {"missing_runtime_prerequisite_count", json_integer(missing_runtime_prerequisite_count)},
        {"no_go_condition_count", json_integer(no_go_condition_count)},
        {"proof_complete", json_bool(true)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({
        json_object({{"code", json_string("PREREQUISITE_DECISION_DOES_NOT_AUTHORIZE_RUNTIME")},
                     {"severity", json_string("warning")}}),
    });
    const auto recommendations = json_array({
        json_object({{"code", json_string("RUN_JAVA_V141_AND_MINIKV_V134_IN_PARALLEL")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("KEEP_RUNTIME_SHELL_BLOCKED")}, {"severity", json_string("recommendation")}}),
    });
    const auto evidence_endpoints = json_object({
        {"runtime_shell_chain_stop_or_prerequisite_decision_record_json", json_string(node_v304_json)},
        {"runtime_shell_chain_stop_or_prerequisite_decision_record_markdown", json_string(node_v304_markdown)},
        {"source_node_v303_json", json_string(node_v303_json)},
        {"source_node_v303_markdown", json_string(node_v303_markdown)},
        {"active_plan", json_string(active_plan)},
        {"recommended_parallel_java_v141",
         json_string("Java v141 runtime shell chain stop/prerequisite decision echo")},
        {"recommended_parallel_mini_kv_v134",
         json_string("mini-kv v134 runtime shell chain stop/prerequisite non-participation receipt")},
        {"next_node_verification", json_string("Node v305")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-"
                     "receipt.v1")},
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
        {"source_node_v304_reference", source_node_v304_reference},
        {"source_node_v303_reference", source_node_v303_reference},
        {"mini_kv_receipt", mini_kv_receipt},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", json_array({json_string("Java v141"), json_string("mini-kv v134")})},
        {"next_actions",
         json_array({
             json_string("Archive mini-kv v134 as a runtime shell chain stop/prerequisite non-participation receipt."),
             json_string("Keep Node v305 blocked until Java v141 and mini-kv v134 are both available."),
             json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
             json_string("Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema "
                         "state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX."),
         })},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_digest(
             read_commands))},
        {"read_only", json_bool(true)},
        {"execution_allowed", json_bool(false)},
        {"runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_only", json_bool(true)},
        {"runtime_shell_chain_stop_or_prerequisite_decision_record_only", json_bool(true)},
        {"stop_or_prerequisite_decision_record_only", json_bool(true)},
        {"decision_record_state", json_string("runtime-shell-chain-stop-or-prerequisite-decision-record-ready")},
        {"runtime_shell_chain_decision", json_string("require-explicit-approval-prerequisites-before-runtime-shell")},
        {"selected_path", json_string("continue-only-as-blocked-prerequisite-review")},
        {"stop_runtime_shell_chain_without_prerequisites", json_bool(true)},
        {"consumes_node_v304_runtime_shell_chain_stop_or_prerequisite_decision_record", json_bool(true)},
        {"consumes_node_v303_post_decision_plan_intake_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v305_stop_prerequisite_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v305_runtime_shell_implementation", json_bool(false)},
    };
    append_runtime_shell_readiness_fields(fields);
    append_non_participation_tail_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
