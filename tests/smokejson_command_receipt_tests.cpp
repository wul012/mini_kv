#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

namespace {

void assert_response_contains(const minikv::CommandResult& result, std::string_view expected) {
    if (result.response.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(result.response.find(std::string{expected}) != std::string::npos);
}

} // namespace

int main() {
    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SET smoke:key value");
    assert(result.response == "OK inserted");

    result = processor.execute("SMOKEJSON extra");
    assert(result.response == "ERR usage: SMOKEJSON");

    result = processor.execute("SMOKEJSON");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"execution_allowed\":false");
    assert_response_contains(result, "\"restore_execution_allowed\":false");
    assert_response_contains(result, "\"order_authoritative\":false");
    assert_response_contains(result, "\"evidence_type\":\"runtime_smoke\"");
    assert_response_contains(result, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_response_contains(result, "\"server\":{\"protocol\":[\"inline\"]");
    assert_response_contains(result, "\"metrics_enabled\":false");
    assert_response_contains(result, "\"store\":{\"live_keys\":1,\"order_authoritative\":false}");
    assert_response_contains(result, "\"wal\":{\"enabled\":false,\"status\":\"disabled\","
                                     "\"compact_recommended\":false}");
    assert_response_contains(result, "\"connection_stats\":{\"available\":false}");
    assert_response_contains(result, "\"real_read\":{\"allowed\":true,\"commands\":[\"INFOJSON\","
                                     "\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_response_contains(result, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_response_contains(result, "\"write_commands_executed\":false");
    assert_response_contains(result, "\"admin_commands_executed\":false");
    assert_response_contains(result, "\"runtime_write_observed\":false");
    assert_response_contains(result, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_response_contains(result, "\"session_id_echo\":\"mini-kv-live-read-v102\"");
    assert_response_contains(result, "\"server_uptime_bucket\":\"lt_60s\"");
    assert_response_contains(result, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_response_contains(result, "\"read_command_count\":4");
    assert_response_contains(result, "\"read_commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_response_contains(result, "\"write_commands_allowed\":false");
    assert_response_contains(result, "\"auto_start_allowed\":false");
    assert_response_contains(result, "\"operator_window\":{\"consumer\":\"Node v200 real-read window CI archive artifact manifest\"");
    assert_response_contains(result, "\"identity_neutral_proof\":true");
    assert_response_contains(result, "\"node_action\":\"verify digest before importing manual window results\"}");
    assert_response_contains(result, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/80/\"");
    assert_response_contains(result, "\"no_restore_proof\":true");
    assert_response_contains(result, "\"upload_allowed\":false");
    assert_response_contains(result, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_response_contains(result, "\"artifact_root\":\"c/\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/81/\"");
    assert_response_contains(result, "\"retention_days\":30");
    assert_response_contains(result, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_response_contains(result, "\"github_artifact_upload_attempted\":false");
    assert_response_contains(result, "\"production_window_allowed\":false");
    assert_response_contains(result, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_response_contains(result, "\"source_version\":\"0.102.0\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"runtime_binary_hint\":\"cmake-build-v102/minikv_server and cmake-build-v102/minikv_client from the current CMake build\"");
    assert_response_contains(result, "\"release_manifest_path\":\"fixtures/release/verification-manifest.json\"");
    assert_response_contains(result, "\"runtime_smoke_evidence_path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_response_contains(result, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"load_restore_compact_executed\":false");
    assert_response_contains(result, "\"production_binary_claimed\":false");
    assert_response_contains(result, "\"node_action\":\"verify binary, fixture, and release evidence path alignment before managed audit persistence boundary work\"");
    assert_response_contains(result, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_response_contains(result, "\"retention_source_path_hint\":\"c/81/\"");
    assert_response_contains(result, "\"provenance_source\":\"binary_provenance.provenance_digest\"");
    assert_response_contains(result, "\"expected_binary_provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_response_contains(result, "\"check_digest\":\"fnv1a64:8cf7cc0df218035f\"");
    assert_response_contains(result, "\"managed_audit_write_executed\":false");
    assert_response_contains(result, "\"node_action\":\"verify runtime evidence retention and binary provenance alignment before managed audit dry-run packet work\"");
    assert_response_contains(result, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v84\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_response_contains(result, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_response_contains(result, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"marker_digest\":\"fnv1a64:6b9ef93f40cbd343\"");
    assert_response_contains(result, "\"replay_executed\":false");
    assert_response_contains(result, "\"node_action\":\"verify v84 retention provenance consumption before managed audit packet restore drill planning\"");
    assert_response_contains(result, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v213 managed audit packet restore drill plan\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v85\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_response_contains(result, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_response_contains(result, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:1177efbaceb52c1c\"");
    assert_response_contains(result, "\"adapter_write_allowed\":false");
    assert_response_contains(result, "\"restore_execution_allowed\":false");
    assert_response_contains(result, "\"load_restore_compact_executed\":false");
    assert_response_contains(result, "\"order_authoritative\":false");
    assert_response_contains(result, "\"boundary\":\"mini-kv remains a read-only evidence provider for managed audit adapter preparation\"");
    assert_response_contains(result, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v86\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/86/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:f39d8e3ef98654ea\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:667590b83f510a58\"");
    assert_response_contains(result, "\"managed_audit_store\":false");
    assert_response_contains(result, "\"storage_write_allowed\":false");
    assert_response_contains(result, "\"admin_commands_allowed\":false");
    assert_response_contains(result, "\"boundary\":\"mini-kv is non-authoritative read-only storage evidence, not the managed audit store\"");
    assert_response_contains(result, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v87\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/87/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:111f0daf1283eab6\"");
    assert_response_contains(result, "\"dispatch_family\":\"runtime_evidence_command_family\"");
    assert_response_contains(result, "\"split_commands\":\"STATS,STATSJSON,SMOKEJSON,STORAGEJSON,HEALTH,INFO,INFOJSON,COMMANDS,COMMANDSJSON\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:0f2caa6931b482b8\"");
    assert_response_contains(result, "\"dispatch_split_applied\":true");
    assert_response_contains(result, "\"handler_table_required\":false");
    assert_response_contains(result, "\"write_handler_changed\":false");
    assert_response_contains(result, "\"admin_handler_changed\":false");
    assert_response_contains(result, "\"wal_snapshot_restore_touched\":false");
    assert_response_contains(result, "\"behavior_changed\":false");
    assert_response_contains(result, "\"fixture_contract_preserved\":true");
    assert_response_contains(result, "\"boundary\":\"command dispatch quality receipt only; no write, admin, WAL, snapshot, restore, or managed audit storage behavior change\"");
    assert_response_contains(result, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v220 managed audit adapter interface and disabled shell\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v88\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/88/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:4aa6d12fb067e2a6\"");
    assert_response_contains(result, "\"adapter_shell\":\"ManagedAuditAdapter disabled shell\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:0174fa831d17cea4\"");
    assert_response_contains(result, "\"adapter_shell_storage_backend\":false");
    assert_response_contains(result, "\"storage_backend_allowed\":false");
    assert_response_contains(result, "\"local_dry_run_records_written\":false");
    assert_response_contains(result, "\"boundary\":\"adapter shell non-storage guard receipt only; mini-kv remains runtime evidence and is not a managed audit adapter storage backend\"");
    assert_response_contains(result, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v89\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_response_contains(result, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_response_contains(result, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:9bacde73d6d07097\"");
    assert_response_contains(result, "\"external_adapter_storage_backend\":false");
    assert_response_contains(result, "\"participates_in_external_adapter\":false");
    assert_response_contains(result, "\"credential_required\":false");
    assert_response_contains(result, "\"credential_read_allowed\":false");
    assert_response_contains(result, "\"migration_required\":false");
    assert_response_contains(result, "\"migration_execution_allowed\":false");
    assert_response_contains(result, "\"schema_authoritative\":false");
    assert_response_contains(result, "\"boundary\":\"external adapter non-participation receipt only; mini-kv is not an external managed audit adapter storage backend and keeps runtime evidence provider role\"");
    assert_response_contains(result, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node v225 managed audit sandbox adapter dry-run package\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v224 managed audit sandbox adapter dry-run plan\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v90\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/90/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:0dfb07cd2f8de289\"");
    assert_response_contains(result, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:f0cae7a4ce0674c2\"");
    assert_response_contains(result, "\"sandbox_adapter_storage_backend\":false");
    assert_response_contains(result, "\"participates_in_sandbox_adapter\":false");
    assert_response_contains(result, "\"credential_value_required\":false");
    assert_response_contains(result, "\"credential_value_read_allowed\":false");
    assert_response_contains(result, "\"production_credential_read_allowed\":false");
    assert_response_contains(result, "\"schema_migration_execution_allowed\":false");
    assert_response_contains(result, "\"sandbox_schema_authoritative\":false");
    assert_response_contains(result, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_response_contains(result, "\"sandbox_dry_run_records_written\":false");
    assert_response_contains(result, "\"boundary\":\"sandbox adapter runtime evidence non-participation receipt only; mini-kv is not a sandbox audit storage backend and does not read credentials or write managed audit state\"");
    assert_response_contains(result, "\"managed_audit_sandbox_connection_receipt_echo_marker\":{\"consumer\":\"Node v229 manual sandbox connection packet verification\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v228 managed audit manual sandbox connection operator packet\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v95\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/95/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_response_contains(result, "\"source_operator_packet_version\":\"Node v228\"");
    assert_response_contains(result, "\"source_operator_packet_profile\":\"managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_response_contains(result, "\"packet_mode\":\"manual-sandbox-connection-operator-packet-only\"");
    assert_response_contains(result, "\"owner_approval_artifact_id_field\":\"ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID\"");
    assert_response_contains(result, "\"credential_handle_name_field\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_response_contains(result, "\"schema_rehearsal_id_field\":\"ORDEROPS_MANAGED_AUDIT_SCHEMA_REHEARSAL_ID\"");
    assert_response_contains(result, "\"rollback_path_id_field\":\"ORDEROPS_MANAGED_AUDIT_ROLLBACK_PATH_ID\"");
    assert_response_contains(result, "\"timeout_budget_ms\":15000");
    assert_response_contains(result, "\"manual_abort_marker\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_ABORT\"");
    assert_response_contains(result, "\"required_operator_field_count\":6");
    assert_response_contains(result, "\"marker_digest\":\"fnv1a64:beb8dd6a0b102a11\"");
    assert_response_contains(result, "\"connection_execution_allowed\":false");
    assert_response_contains(result, "\"managed_audit_write_allowed\":false");
    assert_response_contains(result, "\"participates_in_sandbox_connection\":false");
    assert_response_contains(result, "\"node_auto_start_allowed\":false");
    assert_response_contains(result, "\"boundary\":\"sandbox connection receipt echo marker only; mini-kv echoes Node v228 operator packet field handles but remains outside managed audit connection, credential reads, schema migration, storage, and state writes\"");
    assert_response_contains(result, "\"managed_audit_sandbox_connection_no_start_guard_receipt\":{\"consumer\":\"Node v231 manual sandbox connection preflight verification\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v96\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_response_contains(result, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_response_contains(result, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"preflight_gate_version\":\"Node v230\"");
    assert_response_contains(result, "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_response_contains(result, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:69e377d01f15fd57\"");
    assert_response_contains(result, "\"manual_window_open_by_default\":false");
    assert_response_contains(result, "\"java_auto_start_allowed\":false");
    assert_response_contains(result, "\"mini_kv_auto_start_allowed\":false");
    assert_response_contains(result, "\"schema_rehearsal_execution_allowed\":false");
    assert_response_contains(result, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_response_contains(result, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_response_contains(result, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_response_contains(result, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_response_contains(result, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.102.0\"");
    assert_response_contains(result, "\"expected_taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_response_contains(result, "\"node_action\":\"verify digest before importing manual window results\"");
    assert_response_contains(result, "\"id\":\"connection-refused\",\"source\":\"tcp_connect\"");
    assert_response_contains(result, "\"id\":\"invalid-json\",\"source\":\"SMOKEJSON_parse\"");
    assert_response_contains(result, "\"id\":\"read-command-failed\",\"source\":\"runtime_read_command\"");
    assert_response_contains(result, "\"id\":\"unsafe-surface\",\"source\":\"adapter_command_plan\"");
    assert_response_contains(result, "\"id\":\"unexpected-write-signal\",\"source\":\"runtime_smoke_diagnostics\"");
    assert_response_contains(result, "\"safe_to_auto_start\":false");
    assert_response_contains(result, "\"write_risk\":true");
    assert_response_contains(result,
                             "\"node_consumption\":\"Node v293 may verify the mini-kv v129 credential resolver disabled fake harness execution-denied receipt verification/retention check");
    assert_response_contains(result,
                             "Node v291 may verify the mini-kv v128 credential resolver disabled fake harness execution-denied non-participation receipt");
    assert_response_contains(result,
                             "Node v289 may verify the mini-kv v127 credential resolver disabled fake harness non-participation receipt");
    assert_response_contains(result,
                             "Node v284 may verify the mini-kv v126 credential resolver implementation plan non-participation receipt");
    assert_response_contains(result,
                             "Node v282 may verify the mini-kv v122 credential resolver approval-required implementation readiness non-participation receipt");
    assert_response_contains(result,
                             "Node v275 may verify the mini-kv v121 credential resolver approval-required boundary non-participation receipt");
    assert_response_contains(result,
                             "Node v274 may verify the mini-kv v120 credential resolver disabled implementation candidate non-participation receipt");
    assert_response_contains(result,
                             "Node v272 may verify the mini-kv v119 credential resolver pre-implementation plan intake non-participation receipt");
    assert_response_contains(result, "Node v269 may verify the mini-kv v118 credential resolver production-readiness blocked-decision non-participation receipt");
    assert_response_contains(result, "Node v267 may verify the mini-kv v117 credential resolver fake-shell archive non-participation receipt");
    assert_response_contains(result, "Node v265 may verify the mini-kv v116 test-only resolver shell non-participation receipt");
    assert_response_contains(result, "Node v246 may verify the mini-kv v108 manual sandbox connection precheck non-participation receipt");
    assert_response_contains(result, "Node v244 may still verify the mini-kv v107 manual sandbox dry-run command non-participation receipt");
    assert_response_contains(result, "Node v263 may verify the mini-kv v115 disabled credential resolver precheck non-participation receipt");
    assert_response_contains(result, "Node v261 may verify the mini-kv v114 credential resolver non-participation receipt");
    assert_response_contains(result, "Node v259 may verify the mini-kv v113 sandbox endpoint handle non-participation receipt");
    assert_response_contains(result, "Node v239 may still verify the mini-kv operator window no-start/no-write receipt");
    assert_response_contains(result, "\"live_read_session_hint\"");
    assert_response_contains(result, "\"binary_provenance_hint\"");
    assert_response_contains(result, "\"retention_provenance_check\"");
    assert_response_contains(result, "\"retention_provenance_replay_marker\"");
    assert_response_contains(result, "\"managed_audit_adapter_restore_boundary_receipt\"");
    assert_response_contains(result, "\"managed_audit_adapter_non_authoritative_storage_receipt\"");
    assert_response_contains(result, "\"manual_sandbox_dry_run_command_non_participation_receipt\"");
    assert_response_contains(result, "\"manual_sandbox_connection_precheck_non_participation_receipt\"");
    assert_response_contains(result, "\"sandbox_endpoint_handle_non_participation_receipt\"");
    assert_response_contains(result, "\"credential_resolver_non_participation_receipt\"");
    assert_response_contains(result, "\"disabled_credential_resolver_precheck_non_participation_receipt\"");
    assert_response_contains(result, "\"test_only_resolver_shell_non_participation_receipt\"");
    assert_response_contains(result, "\"credential_resolver_fake_shell_archive_non_participation_receipt\"");
    assert_response_contains(result, "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\"");
    assert_response_contains(result, "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\"");
    assert_response_contains(result,
                             "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\"");
    assert_response_contains(result,
                             "\"credential_resolver_approval_required_boundary_non_participation_receipt\"");
    assert_response_contains(
        result,
        "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\"");
    assert_response_contains(result, "\"credential_resolver_implementation_plan_non_participation_receipt\"");
    assert_response_contains(result, "\"credential_resolver_disabled_fake_harness_non_participation_receipt\"");
    assert_response_contains(
        result,
        "\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\"");
    assert_response_contains(
        result,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\"");
    assert_response_contains(
        result,
        "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review.v1\"");
    assert_response_contains(result, "\"required_artifact_count\":18");
    assert_response_contains(result, "\"ready_for_java_v116_mini_kv_v122_echo\":true");
    assert_response_contains(result, "\"current_artifact_path_hint\":\"d/122/\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:b6e1efa8878b64d6\"");
    assert_response_contains(result, "\"read_only_aggregate\"");
    assert_response_contains(result, "\"does_not_execute_load_compact_setnxex_or_restore\"");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    return 0;
}
