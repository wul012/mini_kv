#include "release_manifest_checks.hpp"

#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <string>

int main() {
    using minikv::release_test::assert_contains;
    minikv::release_test::verify_release_inputs();

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.102.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response,
                    "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(result.response, "\"no_restore_proof\":true");
    assert_contains(result.response, "\"upload_allowed\":false");
    assert_contains(result.response,
                    "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(result.response, "\"artifact_root\":\"c/\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(result.response, "\"retention_days\":30");
    assert_contains(result.response, "\"github_artifact_upload_attempted\":false");
    assert_contains(result.response, "\"production_window_allowed\":false");
    assert_contains(result.response,
                    "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(result.response, "\"source_version\":\"0.102.0\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"production_binary_claimed\":false");

    result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_smoke\"");
    assert_contains(result.response, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(
        result.response,
        "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(result.response, "\"session_id_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(result.response, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(result.response, "\"write_commands_allowed\":false");
    assert_contains(result.response, "\"auto_start_allowed\":false");
    assert_contains(result.response, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(result.response, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(result.response, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response,
                    "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(result.response,
                    "\"operator_window\":{\"consumer\":\"Node v200 real-read window CI archive artifact manifest\"");
    assert_contains(result.response, "\"identity_neutral_proof\":true");
    assert_contains(result.response,
                    "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(result.response, "\"no_restore_proof\":true");
    assert_contains(result.response, "\"upload_allowed\":false");
    assert_contains(result.response,
                    "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(result.response, "\"artifact_root\":\"c/\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(result.response, "\"retention_days\":30");
    assert_contains(result.response, "\"github_artifact_upload_attempted\":false");
    assert_contains(result.response, "\"production_window_allowed\":false");
    assert_contains(result.response,
                    "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(result.response, "\"production_binary_claimed\":false");
    assert_contains(result.response, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity "
                                     "approval provenance dry-run packet\"");
    assert_contains(result.response, "\"check_digest\":\"fnv1a64:8cf7cc0df218035f\"");
    assert_contains(result.response, "\"managed_audit_write_executed\":false");
    assert_contains(
        result.response,
        "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v84\"");
    assert_contains(result.response, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:6b9ef93f40cbd343\"");
    assert_contains(result.response, "\"replay_executed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 "
                                     "managed audit dry-run adapter candidate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v85\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:1177efbaceb52c1c\"");
    assert_contains(result.response, "\"adapter_write_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node "
                                     "v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(result.response, "\"managed_audit_store\":false");
    assert_contains(result.response, "\"storage_write_allowed\":false");
    assert_contains(result.response, "\"admin_commands_allowed\":false");
    assert_contains(result.response, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit "
                                     "adapter implementation precheck packet\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0f2caa6931b482b8\"");
    assert_contains(result.response, "\"dispatch_split_applied\":true");
    assert_contains(result.response, "\"wal_snapshot_restore_touched\":false");
    assert_contains(result.response, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed "
                                     "audit local adapter candidate dry-run\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0174fa831d17cea4\"");
    assert_contains(result.response, "\"adapter_shell_storage_backend\":false");
    assert_contains(result.response, "\"local_dry_run_records_written\":false");
    assert_contains(result.response, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":"
                                     "\"Node v223 managed audit external adapter connection readiness review\"");
    assert_contains(result.response,
                    "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v89\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_contains(result.response, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:9bacde73d6d07097\"");
    assert_contains(result.response, "\"external_adapter_storage_backend\":false");
    assert_contains(result.response, "\"participates_in_external_adapter\":false");
    assert_contains(result.response, "\"credential_read_allowed\":false");
    assert_contains(result.response, "\"migration_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node "
                                     "v225 managed audit sandbox adapter dry-run package\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v224 managed audit sandbox adapter dry-run plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v90\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/90/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:0dfb07cd2f8de289\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:f0cae7a4ce0674c2\"");
    assert_contains(result.response, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(result.response, "\"participates_in_sandbox_adapter\":false");
    assert_contains(result.response, "\"credential_value_required\":false");
    assert_contains(result.response, "\"credential_value_read_allowed\":false");
    assert_contains(result.response, "\"production_credential_read_allowed\":false");
    assert_contains(result.response, "\"schema_migration_execution_allowed\":false");
    assert_contains(result.response, "\"sandbox_schema_authoritative\":false");
    assert_contains(result.response, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(result.response, "\"sandbox_dry_run_records_written\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_connection_receipt_echo_marker\":{\"consumer\":\"Node "
                                     "v229 manual sandbox connection packet verification\"");
    assert_contains(result.response,
                    "\"consumed_by\":\"Node v228 managed audit manual sandbox connection operator packet\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v95\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/95/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(
        result.response,
        "\"source_operator_packet_profile\":\"managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_contains(result.response, "\"packet_mode\":\"manual-sandbox-connection-operator-packet-only\"");
    assert_contains(result.response,
                    "\"credential_handle_name_field\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:beb8dd6a0b102a11\"");
    assert_contains(result.response, "\"connection_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_write_allowed\":false");
    assert_contains(result.response, "\"participates_in_sandbox_connection\":false");
    assert_contains(result.response, "\"node_auto_start_allowed\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_connection_no_start_guard_receipt\":{\"consumer\":\"Node "
                                     "v231 manual sandbox connection preflight verification\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v96\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"preflight_gate_version\":\"Node v230\"");
    assert_contains(result.response,
                    "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_contains(result.response, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:69e377d01f15fd57\"");
    assert_contains(result.response, "\"manual_window_open_by_default\":false");
    assert_contains(result.response, "\"java_auto_start_allowed\":false");
    assert_contains(result.response, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(result.response, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(result.response, "Node v315 may verify the mini-kv v138 credential resolver signed human approval "
                                     "artifact contract non-participation receipt");
    assert_contains(result.response, "Node v305 may verify the mini-kv v134 credential resolver runtime shell chain "
                                     "stop/prerequisite non-participation receipt");
    assert_contains(result.response, "Node v302 may verify the mini-kv v133 credential resolver runtime shell "
                                     "post-decision plan intake non-participation receipt");
    assert_contains(result.response, "Node v300 may verify the mini-kv v132 credential resolver runtime shell decision "
                                     "record non-participation receipt");
    assert_contains(result.response, "Node v298 may verify the mini-kv v131 credential resolver disabled runtime shell "
                                     "candidate gate non-participation receipt");
    assert_contains(
        result.response,
        "Node v296 may verify the mini-kv v130 credential resolver disabled runtime shell non-participation receipt");
    assert_contains(result.response, "Node v293 may verify the mini-kv v129 credential resolver disabled fake harness "
                                     "execution-denied receipt verification/retention check");
    assert_contains(result.response, "Node v291 may verify the mini-kv v128 credential resolver disabled fake harness "
                                     "execution-denied non-participation receipt");
    assert_contains(
        result.response,
        "Node v289 may verify the mini-kv v127 credential resolver disabled fake harness non-participation receipt");
    assert_contains(
        result.response,
        "Node v284 may verify the mini-kv v126 credential resolver implementation plan non-participation receipt");
    assert_contains(result.response, "Node v282 may verify the mini-kv v122 credential resolver approval-required "
                                     "implementation readiness non-participation receipt");
    assert_contains(result.response, "Node v275 may verify the mini-kv v121 credential resolver approval-required "
                                     "boundary non-participation receipt");
    assert_contains(result.response, "Node v274 may verify the mini-kv v120 credential resolver disabled "
                                     "implementation candidate non-participation receipt");
    assert_contains(result.response, "Node v272 may verify the mini-kv v119 credential resolver pre-implementation "
                                     "plan intake non-participation receipt");
    assert_contains(result.response, "Node v269 may verify the mini-kv v118 credential resolver production-readiness "
                                     "blocked-decision non-participation receipt");
    assert_contains(
        result.response,
        "Node v267 may verify the mini-kv v117 credential resolver fake-shell archive non-participation receipt");
    assert_contains(result.response,
                    "Node v265 may verify the mini-kv v116 test-only resolver shell non-participation receipt");
    assert_contains(
        result.response,
        "Node v246 may verify the mini-kv v108 manual sandbox connection precheck non-participation receipt");
    assert_contains(
        result.response,
        "Node v244 may still verify the mini-kv v107 manual sandbox dry-run command non-participation receipt");
    assert_contains(result.response,
                    "Node v259 may verify the mini-kv v113 sandbox endpoint handle non-participation receipt");
    assert_contains(result.response,
                    "Node v239 may still verify the mini-kv operator window no-start/no-write receipt");
    assert_contains(result.response, "\"manual_sandbox_connection_precheck_non_participation_receipt\"");
    assert_contains(result.response, "\"sandbox_endpoint_handle_non_participation_receipt\"");
    assert_contains(result.response, "\"test_only_resolver_shell_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_fake_shell_archive_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_approval_required_boundary_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_implementation_plan_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_disabled_fake_harness_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt\"");
    assert_contains(
        result.response,
        "\"credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check\"");
    assert_contains(result.response, "\"credential_resolver_disabled_runtime_shell_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_runtime_shell_decision_record_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"review_digest\":\"3bbe96497638d826ab644c7503ab5309c0cc4c4fccdd39a0e82a9b6123ca36c9\"");
    assert_contains(result.response, "\"current_release_version\":\"v130\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/130/\"");
    assert_contains(result.response, "\"current_release_version\":\"v131\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/131/\"");
    assert_contains(result.response, "\"current_release_version\":\"v132\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/132/\"");
    assert_contains(result.response,
                    "\"gate_digest\":\"651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22\"");
    assert_contains(result.response, "\"runtime_shell_candidate_gate_non_participation_receipt_only\":true");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0557867ad4f6ed7e\"");
    assert_contains(result.response,
                    "\"decision_digest\":\"4f6f73fa2806a9ba74174d7bbab17b43459bd1d790237276d95a3937c646e9c0\"");
    assert_contains(result.response, "\"runtime_shell_decision_record_non_participation_receipt_only\":true");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:7c144f01161c2f81\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt\"");
    assert_contains(result.response, "\"runtime_shell_implemented\":false");
    assert_contains(result.response, "\"runtime_shell_invocation_allowed\":false");
    assert_contains(result.response, "\"write_commands_executed\":false");

    result = processor.execute("STORAGEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    return 0;
}
