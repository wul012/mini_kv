#include "minikv/runtime_evidence_receipts.hpp"

#include "runtime_credential_resolver_receipt_utils.hpp"

#include "minikv/version.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts {
namespace {

using credential_resolver_detail::DigestPart;
using credential_resolver_detail::field_string;
using credential_resolver_detail::receipt_digest;

constexpr std::string_view receipt_consumer =
    "Node v296 disabled runtime shell upstream echo verification";
constexpr std::string_view receipt_fixture_path =
    "fixtures/release/credential-resolver-disabled-runtime-shell-non-participation-receipt.json";
constexpr std::string_view receipt_release_version = "v130";
constexpr std::string_view receipt_artifact_path_hint = "d/130/";
constexpr std::string_view source_design_review =
    "Node v295 credential resolver disabled runtime shell design review";
constexpr std::string_view source_pre_plan =
    "Node v294 credential resolver disabled runtime shell pre-plan intake";
constexpr std::string_view runtime_role = "runtime evidence provider only";

constexpr std::string_view node_v295_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-design-review.v1";
constexpr std::string_view node_v295_state = "disabled-runtime-shell-design-review-ready";
constexpr std::string_view node_v295_review_version = "node-v295-disabled-runtime-shell-design-review.v1";
constexpr std::string_view node_v295_review_digest =
    "3bbe96497638d826ab644c7503ab5309c0cc4c4fccdd39a0e82a9b6123ca36c9";
constexpr std::string_view node_v295_decision =
    "request-parallel-java-v132-and-mini-kv-v130-before-runtime-implementation";
constexpr std::string_view node_v295_blocker =
    "runtime-shell-implementation-has-no-upstream-design-echo";

constexpr std::string_view node_v294_profile_version =
    "managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-pre-plan-intake.v1";
constexpr std::string_view node_v294_state = "disabled-runtime-shell-pre-plan-intake-ready";
constexpr std::string_view node_v294_plan_digest =
    "29e1a772ec0f1aca8dd56b12f63dd390f499cb8da9c3ecaa658b4c84c0f97008";
constexpr std::string_view node_v294_intake_digest =
    "c6aae7474f3cfbd05103dc4a57fbf06335724d44a6b7300bb9cdd2cc0c714eb8";

constexpr std::string_view active_plan =
    "docs/plans2/v295-post-disabled-runtime-shell-design-review-roadmap.md";
constexpr std::string_view source_node_v295_json =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-design-review";
constexpr std::string_view source_node_v295_markdown =
    "/api/v1/audit/managed-audit-manual-sandbox-connection-credential-resolver-disabled-runtime-shell-design-review?format=markdown";

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
    "credential resolver disabled runtime shell non-participation receipt only; mini-kv confirms it does not participate in or authorize disabled runtime shell implementation, enablement, invocation, provider or resolver client instantiation, credential value reads, raw endpoint parsing, external HTTP/TCP requests, managed audit connections, storage writes, approval ledger writes, schema migrations, LOAD/COMPACT/RESTORE/SETNXEX execution, automatic upstream starts, or audit/order authority";
constexpr std::string_view node_action =
    "verify mini-kv v130 disabled runtime shell non-participation receipt with Java v132 before Node v296 upstream echo verification";

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
    return "[\"Stop if Java v132 or mini-kv v130 would need to read credential values, parse raw endpoint URLs, write ledgers, run migrations, or open external connections.\","
           "\"Stop if Node v296 would instantiate a provider/client before consuming Java v132 and mini-kv v130.\","
           "\"Stop if a future plan tries to reuse UPSTREAM_ACTIONS_ENABLED as runtime approval instead of a dedicated disabled-by-default gate.\"]";
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

std::string format_closed_boundary_flags_json() {
    return "\"read_only\":true,\"execution_allowed\":false,"
           "\"disabled_runtime_shell_non_participation_receipt_only\":true,"
           "\"disabled_runtime_shell_design_review_only\":true,"
           "\"consumes_node_v295_disabled_runtime_shell_design_review\":true,"
           "\"consumes_node_v294_disabled_runtime_shell_pre_plan_intake\":true,"
           "\"ready_for_node_v296_disabled_runtime_shell_upstream_echo_verification\":true,"
           "\"ready_for_node_v296_runtime_shell_implementation\":false,"
           "\"ready_for_disabled_runtime_shell_implementation\":false,"
           "\"ready_for_disabled_runtime_shell_invocation\":false,"
           "\"ready_for_managed_audit_resolver_implementation\":false,"
           "\"ready_for_managed_audit_sandbox_adapter_connection\":false,"
           "\"ready_for_production_audit\":false,"
           "\"ready_for_production_window\":false,"
           "\"ready_for_production_operations\":false,"
           "\"runtime_shell_implemented\":false,"
           "\"runtime_shell_enabled\":false,"
           "\"runtime_shell_invocation_allowed\":false,"
           "\"runtime_shell_implementation_allowed\":false,"
           "\"disabled_runtime_shell_participates\":false,"
           "\"test_only_fake_harness_allowed\":false,"
           "\"test_only_fake_harness_execution_allowed\":false,"
           "\"fake_harness_runtime_enabled\":false,"
           "\"fake_harness_runtime_invoked\":false,"
           "\"real_resolver_implementation_allowed\":false,"
           "\"credential_resolver_implemented\":false,"
           "\"credential_resolver_invoked\":false,"
           "\"resolver_client_instantiated\":false,"
           "\"secret_provider_instantiated\":false,"
           "\"fake_secret_provider_instantiated\":false,"
           "\"fake_resolver_client_instantiated\":false,"
           "\"provider_client_instantiation_allowed\":false,"
           "\"credential_value_read_allowed\":false,"
           "\"credential_value_read\":false,"
           "\"credential_value_provided\":false,"
           "\"credential_value_loaded\":false,"
           "\"credential_value_stored\":false,"
           "\"credential_value_included\":false,"
           "\"credential_value_rendered\":false,"
           "\"raw_endpoint_url_parse_allowed\":false,"
           "\"raw_endpoint_url_render_allowed\":false,"
           "\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_rendered\":false,"
           "\"raw_endpoint_url_provided\":false,"
           "\"raw_endpoint_url_included\":false,"
           "\"external_request_allowed\":false,"
           "\"external_request_sent\":false,"
           "\"http_tcp_dial_allowed\":false,"
           "\"connects_managed_audit\":false,"
           "\"reads_managed_audit_credential\":false,"
           "\"stores_managed_audit_credential\":false,"
           "\"managed_audit_store\":false,"
           "\"managed_audit_storage_backend\":false,"
           "\"sandbox_audit_storage_backend\":false,"
           "\"storage_write_allowed\":false,"
           "\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,"
           "\"runtime_write_observed\":false,"
           "\"approval_gate_required\":true,"
           "\"approval_gate_satisfied\":false,"
           "\"approval_ledger_write_allowed\":false,"
           "\"approval_ledger_written\":false,"
           "\"approval_ledger_write_executed\":false,"
           "\"managed_audit_write_executed\":false,"
           "\"production_record_written\":false,"
           "\"schema_migration_allowed\":false,"
           "\"schema_migration_executed\":false,"
           "\"schema_rehearsal_execution_allowed\":false,"
           "\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,"
           "\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,"
           "\"node_auto_start_allowed\":false,"
           "\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,"
           "\"automatic_upstream_start_allowed\":false,"
           "\"automatic_upstream_start\":false,"
           "\"audit_authoritative\":false,"
           "\"order_authoritative\":false";
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
    return "{\"receipt_version\":\"mini-kv-credential-resolver-disabled-runtime-shell-non-participation-receipt.v1\","
           "\"receipt_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"source_design_review\":" + field_string(source_design_review) +
           ",\"source_pre_plan_intake\":" + field_string(source_pre_plan) +
           ",\"consumer_hint\":" + field_string(receipt_consumer) +
           ",\"current_project_version\":" + field_string(version) +
           ",\"runtime_project_version\":" + field_string(version) +
           ",\"current_release_version\":" + field_string(receipt_release_version) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"current_runtime_fixture_release_version\":\"v102\""
           ",\"current_runtime_fixture_artifact_path_hint\":\"c/102/\""
           ",\"current_live_read_session_echo\":\"mini-kv-live-read-v102\""
           ",\"runtime_role\":" + field_string(runtime_role) +
           ",\"source_node_v295_reference\":{\"source_version\":\"Node v295\","
           "\"profile_version\":" + field_string(node_v295_profile_version) +
           ",\"design_review_state\":" + field_string(node_v295_state) +
           ",\"review_version\":" + field_string(node_v295_review_version) +
           ",\"review_mode\":\"design-review-only\""
           ",\"review_digest\":" + field_string(node_v295_review_digest) +
           ",\"decision\":" + field_string(node_v295_decision) +
           ",\"blocker\":" + field_string(node_v295_blocker) +
           ",\"consumer\":\"Node v296 disabled runtime shell implementation decision\""
           ",\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_check_count\":" + std::to_string(source_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"source_boundary_count\":" + std::to_string(source_boundary_count) +
           ",\"review_question_count\":" + std::to_string(review_question_count) +
           ",\"yes_review_question_count\":" + std::to_string(review_question_count) +
           ",\"recommended_parallel_work_count\":" +
           std::to_string(recommended_parallel_work_count) +
           ",\"stop_condition_count\":" + std::to_string(stop_condition_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) +
           ",\"disabled_runtime_shell_design_review_only\":true"
           ",\"read_only_design_review\":true"
           ",\"ready_for_parallel_upstream_echo_request\":true"
           ",\"ready_for_node_v296_runtime_shell_implementation\":false"
           ",\"ready_for_disabled_runtime_shell_implementation\":false"
           ",\"ready_for_disabled_runtime_shell_invocation\":false"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_enabled\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"credential_value_read\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"external_request_sent\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false"
           ",\"review_questions\":" + format_review_questions_json() +
           ",\"stop_conditions\":" + format_stop_conditions_json() + "}"
           ",\"source_node_v294_reference\":{\"source_version\":\"Node v294\","
           "\"profile_version\":" + field_string(node_v294_profile_version) +
           ",\"pre_plan_intake_state\":" + field_string(node_v294_state) +
           ",\"ready_for_disabled_runtime_shell_pre_plan_intake\":true"
           ",\"ready_for_disabled_runtime_shell_design_review\":true"
           ",\"plan_digest\":" + field_string(node_v294_plan_digest) +
           ",\"intake_digest\":" + field_string(node_v294_intake_digest) +
           ",\"boundary_count\":" + std::to_string(source_boundary_count) +
           ",\"defined_boundary_count\":" + std::to_string(source_boundary_count) +
           ",\"all_required_boundaries_defined\":true"
           ",\"required_boundary_count\":" + std::to_string(source_boundary_count) +
           ",\"missing_boundary_count\":0"
           ",\"next_node_review_version\":\"Node v295\""
           ",\"next_java_echo_version\":\"Java v132\""
           ",\"next_mini_kv_receipt_version\":\"mini-kv v130\""
           ",\"runtime_shell_implementation_allowed\":false"
           ",\"runtime_shell_invocation_allowed\":false"
           ",\"fake_harness_runtime_allowed\":false"
           ",\"credential_value_read_allowed\":false"
           ",\"raw_endpoint_url_parse_allowed\":false"
           ",\"provider_client_instantiation_allowed\":false"
           ",\"external_request_allowed\":false"
           ",\"schema_migration_allowed\":false"
           ",\"approval_ledger_write_allowed\":false"
           ",\"automatic_upstream_start_allowed\":false"
           ",\"runtime_shell_implemented\":false"
           ",\"runtime_shell_enabled\":false"
           ",\"test_only_fake_harness_allowed\":false"
           ",\"test_only_fake_harness_execution_allowed\":false"
           ",\"execution_allowed\":false"
           ",\"connects_managed_audit\":false"
           ",\"reads_managed_audit_credential\":false"
           ",\"stores_managed_audit_credential\":false"
           ",\"credential_value_read\":false"
           ",\"credential_value_provided\":false"
           ",\"raw_endpoint_url_parsed\":false"
           ",\"raw_endpoint_url_rendered\":false"
           ",\"external_request_sent\":false"
           ",\"secret_provider_instantiated\":false"
           ",\"resolver_client_instantiated\":false"
           ",\"fake_secret_provider_instantiated\":false"
           ",\"fake_resolver_client_instantiated\":false"
           ",\"schema_migration_executed\":false"
           ",\"approval_ledger_written\":false"
           ",\"automatic_upstream_start\":false"
           ",\"source_check_count\":" + std::to_string(source_check_count) +
           ",\"source_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"source_production_blocker_count\":0"
           ",\"source_warning_count\":2"
           ",\"source_recommendation_count\":2}"
           ",\"mini_kv_receipt\":{\"receipt_mode\":\"disabled-runtime-shell-non-participation-receipt-only\","
           "\"shell_name\":\"credential-resolver-disabled-runtime-shell\""
           ",\"non_participation_scope\":\"runtime shell implementation, invocation, provider/client wiring, credential reads, endpoint parsing, network, writes, schema, restore/load/compact/setnxex, and authority\""
           ",\"standalone_fixture_path\":" + field_string(receipt_fixture_path) +
           ",\"current_artifact_path_hint\":" + field_string(receipt_artifact_path_hint) +
           ",\"runtime_smoke_exposes_receipt\":true"
           ",\"release_manifest_exposes_receipt\":true"
           ",\"verification_count\":18}"
           ",\"checks\":" + format_checks_json() +
           ",\"summary\":{\"check_count\":" + std::to_string(check_count) +
           ",\"passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v295_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v295_passed_check_count\":" + std::to_string(check_count) +
           ",\"source_node_v294_check_count\":" + std::to_string(source_check_count) +
           ",\"source_node_v294_passed_check_count\":" + std::to_string(source_check_count) +
           ",\"source_boundary_count\":" + std::to_string(source_boundary_count) +
           ",\"review_question_count\":" + std::to_string(review_question_count) +
           ",\"yes_review_question_count\":" + std::to_string(review_question_count) +
           ",\"recommended_parallel_work_count\":" +
           std::to_string(recommended_parallel_work_count) +
           ",\"stop_condition_count\":" + std::to_string(stop_condition_count) +
           ",\"production_blocker_count\":" + std::to_string(production_blocker_count) +
           ",\"warning_count\":" + std::to_string(warning_count) +
           ",\"recommendation_count\":" + std::to_string(recommendation_count) + "}"
           ",\"production_blockers\":[]"
           ",\"warnings\":[{\"code\":\"DISABLED_RUNTIME_SHELL_RECEIPT_IS_NOT_RUNTIME\",\"severity\":\"warning\"},"
           "{\"code\":\"UPSTREAM_ECHO_REQUIRED_BEFORE_NODE_V296_RUNTIME_DECISION\",\"severity\":\"warning\"}]"
           ",\"recommendations\":[{\"code\":\"RECOMMENDED_PARALLEL_JAVA_V132_MINI_KV_V130\",\"severity\":\"recommendation\"},"
           "{\"code\":\"NODE_V296_MUST_CONSUME_BOTH_UPSTREAM_ECHOES\",\"severity\":\"recommendation\"}]"
           ",\"evidence_endpoints\":{\"source_node_v295_json\":" + field_string(source_node_v295_json) +
           ",\"source_node_v295_markdown\":" + field_string(source_node_v295_markdown) +
           ",\"active_plan\":" + field_string(active_plan) +
           ",\"next_recommended_parallel\":\"Java v132 + mini-kv v130\""
           ",\"next_node_verification\":\"Node v296\"}"
           ",\"next_required_evidence_versions\":[\"Java v132\",\"mini-kv v130\"]"
           ",\"next_actions\":[\"Archive mini-kv v130 as a disabled runtime shell non-participation receipt.\","
           "\"Keep Node v296 blocked until Java v132 and mini-kv v130 are both available.\","
           "\"Do not implement, enable, or invoke a disabled runtime shell from mini-kv.\","
           "\"Do not read credentials, parse raw endpoints, send HTTP/TCP, write ledgers or schema state, auto-start upstream services, or execute LOAD/COMPACT/RESTORE/SETNXEX.\"]"
           ",\"binary_provenance_digest\":" + field_string(binary_provenance_digest()) +
           ",\"retention_check_digest\":" + field_string(retention_provenance_check_digest()) +
           ",\"retention_replay_marker_digest\":" +
           field_string(retention_provenance_replay_marker_digest()) +
           ",\"read_command_list_digest\":" + field_string(read_command_list_digest(read_commands)) +
           ",\"receipt_digest\":" +
           field_string(credential_resolver_disabled_runtime_shell_non_participation_receipt_digest(
               read_commands)) +
           "," + format_closed_boundary_flags_json() +
           ",\"boundary\":" + field_string(boundary_text) +
           ",\"node_action\":" + field_string(node_action) + "}";
}

} // namespace minikv::runtime_evidence_receipts
