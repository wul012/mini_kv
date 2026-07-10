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

constexpr std::string_view receipt_consumer = "Node v296 disabled runtime shell upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-runtime-shell-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v130";
constexpr std::string_view receipt_artifact_path_hint = "d/130/";
constexpr std::string_view source_design_review = "Node v295 credential resolver disabled runtime shell design review";
constexpr std::string_view source_pre_plan = "Node v294 credential resolver disabled runtime shell pre-plan intake";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v295_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-design-review.v1";
constexpr std::string_view node_v295_state = "disabled-runtime-shell-design-review-ready";
constexpr std::string_view node_v295_review_version = "node-v295-disabled-runtime-shell-design-review.v1";
constexpr std::string_view node_v295_review_digest = "3bbe96497638d826ab644c7503ab5309c0cc4c4fccdd39a0e82a9b6123ca36c9";
constexpr std::string_view node_v295_decision =
    "request-parallel-java-v132-and-mini-kv-v130-before-runtime-implementation";
constexpr std::string_view node_v295_blocker = "runtime-shell-implementation-has-no-upstream-design-echo";

constexpr std::string_view node_v294_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-pre-plan-intake.v1";
constexpr std::string_view node_v294_state = "disabled-runtime-shell-pre-plan-intake-ready";
constexpr std::string_view node_v294_plan_digest = "29e1a772ec0f1aca8dd56b12f63dd390f499cb8da9c3ecaa658b4c84c0f97008";
constexpr std::string_view node_v294_intake_digest = "c6aae7474f3cfbd05103dc4a57fbf06335724d44a6b7300bb9cdd2cc0c714eb8";

constexpr std::string_view active_plan = "docs/plans2/v295-post-disabled-runtime-shell-design-review-roadmap.md";
constexpr std::string_view source_node_v295_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-design-review";
constexpr std::string_view source_node_v295_markdown =
    "/api/v1/audit/"
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-design-review?format=markdown";

constexpr int check_count = 27;
constexpr int source_check_count = 28;
constexpr int source_boundary_count = 10;
constexpr int review_question_count = 11;
constexpr int recommended_parallel_work_count = 2;
constexpr int stop_condition_count = 3;
constexpr int production_blocker_count = 0;
constexpr int warning_count = 2;
constexpr int recommendation_count = 2;

constexpr std::string_view boundary_text =
    "credential resolver disabled runtime shell non-participation receipt only; mini-kv confirms it does not "
    "participate in or authorize disabled runtime shell implementation, enablement, invocation, provider or resolver "
    "client instantiation, credential value reads, raw endpoint parsing, external HTTP/TCP requests, managed audit "
    "connections, storage writes, approval ledger writes, schema migrations, LOAD/COMPACT/RESTORE/SETNXEX execution, "
    "automatic upstream starts, or audit/order authority";
constexpr std::string_view node_action = "verify mini-kv v130 disabled runtime shell non-participation receipt with "
                                         "Java v132 before Node v296 upstream echo verification";

std::string format_review_questions_json() {
    return "[{\"code\":\"SOURCE_PRE_PLAN_READY\",\"answer\":\"yes\"},"
           "{\"code\":\"SOURCE_BOUNDARIES_COMPLETE\",\"answer\":\"yes\"},"
           "{\"code\":\"IMPLEMENTATION_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"INVOCATION_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"CREDENTIAL_VALUE_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"RAW_ENDPOINT_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"PROVIDER_CLIENT_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"NETWORK_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"WRITE_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"AUTO_START_STILL_FORBIDDEN\",\"answer\":\"yes\"},"
           "{\"code\":\"UPSTREAM_ECHO_NEEDED_BEFORE_IMPLEMENTATION\",\"answer\":\"yes\"}]";
}

std::string format_stop_conditions_json() {
    return "[\"Stop if Java v132 or mini-kv v130 would need to read credential values, parse raw endpoint URLs, write "
           "ledgers, run migrations, or open external connections.\","
           "\"Stop if Node v296 would instantiate a provider/client before consuming Java v132 and mini-kv v130.\","
           "\"Stop if a future plan tries to reuse UPSTREAM_ACTIONS_ENABLED as runtime approval instead of a dedicated "
           "disabled-by-default gate.\"]";
}

std::string format_checks_json() {
    return "{\"source_node_v295_ready\":true,"
           "\"source_node_v294_ready\":true,"
           "\"source_boundaries_complete\":true,"
           "\"design_review_only\":true,"
           "\"necessity_documented\":true,"
           "\"parallel_upstream_echo_recommended\":true,"
           "\"mini_kv_non_participation_recorded\":true,"
           "\"runtime_shell_implementation_forbidden\":true,"
           "\"runtime_shell_invocation_forbidden\":true,"
           "\"credential_value_read_forbidden\":true,"
           "\"raw_endpoint_parse_forbidden\":true,"
           "\"provider_client_instantiation_forbidden\":true,"
           "\"external_request_forbidden\":true,"
           "\"writes_and_migrations_forbidden\":true,"
           "\"load_compact_restore_setnxex_forbidden\":true,"
           "\"auto_start_forbidden\":true,"
           "\"audit_and_order_authority_forbidden\":true,"
           "\"ready_for_node_v296_upstream_echo\":true}";
}

} // namespace

std::string credential_resolver_disabled_runtime_shell_non_participation_receipt_digest(
    const std::vector<std::string>& read_commands) {
    const std::vector<DigestPart> parts = {
        {std::string{version}},
        {std::string{receipt_release_version}},
        {std::string{receipt_artifact_path_hint}},
        {std::string{source_design_review}},
        {std::string{source_pre_plan}},
        {std::string{node_v295_profile_version}},
        {std::string{node_v295_state}},
        {std::string{node_v295_review_version}},
        {std::string{node_v295_review_digest}},
        {std::string{node_v295_decision}},
        {std::string{node_v294_profile_version}},
        {std::string{node_v294_state}},
        {std::string{node_v294_plan_digest}},
        {std::string{node_v294_intake_digest}},
        {std::to_string(check_count)},
        {std::to_string(source_check_count)},
        {std::to_string(source_boundary_count)},
        {std::to_string(review_question_count)},
        {std::to_string(recommended_parallel_work_count)},
        {std::to_string(stop_condition_count)},
        {"disabled-runtime-shell-non-participation-receipt-only"},
        {"runtime-shell-implementation-forbidden"},
        {"false"},
        {"false"},
        {"false"},
        {read_command_list_digest(read_commands)},
    };
    return receipt_digest("mini-kv-credential-resolver-disabled-runtime-shell-non-participation", parts);
}

std::string format_credential_resolver_disabled_runtime_shell_non_participation_receipt_json(
    const std::vector<std::string>& read_commands) {
    const auto source_node_v295_reference = json_object({
        {"source_version", json_string("Node v295")},
        {"profile_version", json_string(node_v295_profile_version)},
        {"design_review_state", json_string(node_v295_state)},
        {"review_version", json_string(node_v295_review_version)},
        {"review_mode", json_string("design-review-only")},
        {"review_digest", json_string(node_v295_review_digest)},
        {"decision", json_string(node_v295_decision)},
        {"blocker", json_string(node_v295_blocker)},
        {"consumer", json_string("Node v296 disabled runtime shell implementation decision")},
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_check_count", json_integer(source_check_count)},
        {"source_passed_check_count", json_integer(source_check_count)},
        {"source_boundary_count", json_integer(source_boundary_count)},
        {"review_question_count", json_integer(review_question_count)},
        {"yes_review_question_count", json_integer(review_question_count)},
        {"recommended_parallel_work_count", json_integer(recommended_parallel_work_count)},
        {"stop_condition_count", json_integer(stop_condition_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
        {"disabled_runtime_shell_design_review_only", json_bool(true)},
        {"read_only_design_review", json_bool(true)},
        {"ready_for_parallel_upstream_echo_request", json_bool(true)},
        {"ready_for_node_v296_runtime_shell_implementation", json_bool(false)},
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
        {"review_questions", format_review_questions_json()},
        {"stop_conditions", format_stop_conditions_json()},
    });

    const auto source_node_v294_reference = json_object({
        {"source_version", json_string("Node v294")},
        {"profile_version", json_string(node_v294_profile_version)},
        {"pre_plan_intake_state", json_string(node_v294_state)},
        {"ready_for_disabled_runtime_shell_pre_plan_intake", json_bool(true)},
        {"ready_for_disabled_runtime_shell_design_review", json_bool(true)},
        {"plan_digest", json_string(node_v294_plan_digest)},
        {"intake_digest", json_string(node_v294_intake_digest)},
        {"boundary_count", json_integer(source_boundary_count)},
        {"defined_boundary_count", json_integer(source_boundary_count)},
        {"all_required_boundaries_defined", json_bool(true)},
        {"required_boundary_count", json_integer(source_boundary_count)},
        {"missing_boundary_count", json_integer(0)},
        {"next_node_review_version", json_string("Node v295")},
        {"next_java_echo_version", json_string("Java v132")},
        {"next_mini_kv_receipt_version", json_string("mini-kv v130")},
        {"runtime_shell_implementation_allowed", json_bool(false)},
        {"runtime_shell_invocation_allowed", json_bool(false)},
        {"fake_harness_runtime_allowed", json_bool(false)},
        {"credential_value_read_allowed", json_bool(false)},
        {"raw_endpoint_url_parse_allowed", json_bool(false)},
        {"provider_client_instantiation_allowed", json_bool(false)},
        {"external_request_allowed", json_bool(false)},
        {"schema_migration_allowed", json_bool(false)},
        {"approval_ledger_write_allowed", json_bool(false)},
        {"automatic_upstream_start_allowed", json_bool(false)},
        {"runtime_shell_implemented", json_bool(false)},
        {"runtime_shell_enabled", json_bool(false)},
        {"test_only_fake_harness_allowed", json_bool(false)},
        {"test_only_fake_harness_execution_allowed", json_bool(false)},
        {"execution_allowed", json_bool(false)},
        {"connects_managed_audit", json_bool(false)},
        {"reads_managed_audit_credential", json_bool(false)},
        {"stores_managed_audit_credential", json_bool(false)},
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
        {"source_check_count", json_integer(source_check_count)},
        {"source_passed_check_count", json_integer(source_check_count)},
        {"source_production_blocker_count", json_integer(0)},
        {"source_warning_count", json_integer(2)},
        {"source_recommendation_count", json_integer(2)},
    });

    const auto mini_kv_receipt = json_object({
        {"receipt_mode", json_string("disabled-runtime-shell-non-participation-receipt-only")},
        {"shell_name", json_string("credential-resolver-disabled-runtime-shell")},
        {"non_participation_scope",
         json_string("runtime shell implementation, invocation, provider/client wiring, credential reads, endpoint "
                     "parsing, network, writes, schema, restore/load/compact/setnxex, and authority")},
        {"standalone_fixture_path", json_string(receipt_fixture_path)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"runtime_smoke_exposes_receipt", json_bool(true)},
        {"release_manifest_exposes_receipt", json_bool(true)},
        {"verification_count", json_integer(18)},
    });

    const auto summary = json_object({
        {"check_count", json_integer(check_count)},
        {"passed_check_count", json_integer(check_count)},
        {"source_node_v295_check_count", json_integer(check_count)},
        {"source_node_v295_passed_check_count", json_integer(check_count)},
        {"source_node_v294_check_count", json_integer(source_check_count)},
        {"source_node_v294_passed_check_count", json_integer(source_check_count)},
        {"source_boundary_count", json_integer(source_boundary_count)},
        {"review_question_count", json_integer(review_question_count)},
        {"yes_review_question_count", json_integer(review_question_count)},
        {"recommended_parallel_work_count", json_integer(recommended_parallel_work_count)},
        {"stop_condition_count", json_integer(stop_condition_count)},
        {"production_blocker_count", json_integer(production_blocker_count)},
        {"warning_count", json_integer(warning_count)},
        {"recommendation_count", json_integer(recommendation_count)},
    });

    const auto warnings = json_array({
        json_object({{"code", json_string("DISABLED_RUNTIME_SHELL_RECEIPT_IS_NOT_RUNTIME")},
                     {"severity", json_string("warning")}}),
        json_object({{"code", json_string("UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V296_RUNTIME_DECISION")},
                     {"severity", json_string("warning")}}),
    });
    const auto recommendations = json_array({
        json_object({{"code", json_string("RECOMMENDED_PARALLEL_JAVA_V132_MINI_KV_V130")},
                     {"severity", json_string("recommendation")}}),
        json_object({{"code", json_string("NODE_V296_MUST_CONSUME_BOTH_UPSTREAM_ECHOES")},
                     {"severity", json_string("recommendation")}}),
    });
    const auto evidence_endpoints = json_object({
        {"source_node_v295_json", json_string(source_node_v295_json)},
        {"source_node_v295_markdown", json_string(source_node_v295_markdown)},
        {"active_plan", json_string(active_plan)},
        {"next_recommended_parallel", json_string("Java v132 + mini-kv v130")},
        {"next_node_verification", json_string("Node v296")},
    });

    std::vector<OrderedJsonField> fields = {
        {"receipt_version",
         json_string("mini-kv-credential-resolver-disabled-runtime-shell-non-participation-receipt.v1")},
        {"receipt_fixture_path", json_string(receipt_fixture_path)},
        {"source_design_review", json_string(source_design_review)},
        {"source_pre_plan_intake", json_string(source_pre_plan)},
        {"consumer_hint", json_string(receipt_consumer)},
        {"current_project_version", json_string(version)},
        {"runtime_project_version", json_string(version)},
        {"current_release_version", json_string(receipt_release_version)},
        {"current_artifact_path_hint", json_string(receipt_artifact_path_hint)},
        {"current_runtime_fixture_release_version", json_string("v102")},
        {"current_runtime_fixture_artifact_path_hint", json_string("c/102/")},
        {"current_live_read_session_echo", json_string("mini-kv-live-read-v102")},
        {"runtime_role", json_string(runtime_role)},
        {"source_node_v295_reference", source_node_v295_reference},
        {"source_node_v294_reference", source_node_v294_reference},
        {"mini_kv_receipt", mini_kv_receipt},
        {"checks", format_checks_json()},
        {"summary", summary},
        {"production_blockers", json_array({})},
        {"warnings", warnings},
        {"recommendations", recommendations},
        {"evidence_endpoints", evidence_endpoints},
        {"next_required_evidence_versions", json_array({json_string("Java v132"), json_string("mini-kv v130")})},
        {"next_actions",
         json_array({
             json_string("Archive mini-kv v130 as a disabled runtime shell non-participation receipt."),
             json_string("Keep Node v296 blocked until Java v132 and mini-kv v130 are both available."),
             json_string("Do not implement, enable, or invoke a disabled runtime shell from mini-kv."),
             json_string("Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema "
                         "state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX."),
         })},
        {"binary_provenance_digest", json_string(binary_provenance_digest())},
        {"retention_check_digest", json_string(retention_provenance_check_digest())},
        {"retention_replay_marker_digest", json_string(retention_provenance_replay_marker_digest())},
        {"read_command_list_digest", json_string(read_command_list_digest(read_commands))},
        {"receipt_digest",
         json_string(credential_resolver_disabled_runtime_shell_non_participation_receipt_digest(read_commands))},
        {"read_only", json_bool(true)},
        {"execution_allowed", json_bool(false)},
        {"disabled_runtime_shell_non_participation_receipt_only", json_bool(true)},
        {"disabled_runtime_shell_design_review_only", json_bool(true)},
        {"consumes_node_v295_disabled_runtime_shell_design_review", json_bool(true)},
        {"consumes_node_v294_disabled_runtime_shell_pre_plan_intake", json_bool(true)},
        {"ready_for_node_v296_disabled_runtime_shell_upstream_echo_verification", json_bool(true)},
        {"ready_for_node_v296_runtime_shell_implementation", json_bool(false)},
    };
    append_runtime_shell_readiness_fields(fields);
    append_non_participation_tail_fields(fields);
    fields.push_back({"boundary", json_string(boundary_text)});
    fields.push_back({"node_action", json_string(node_action)});
    return json_object(std::span<const OrderedJsonField>{fields.data(), fields.size()});
}

} // namespace minikv::runtime_evidence_receipts
