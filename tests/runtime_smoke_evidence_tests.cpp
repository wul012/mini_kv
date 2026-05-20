#include "minikv/command.hpp"
#include "minikv/client_limits.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());
    std::ostringstream output;
    output << input.rdbuf();
    auto text = output.str();
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

void assert_path_exists(const std::filesystem::path& relative_path) {
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path));
}

} // namespace

int main() {
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto smoke = read_fixture_text(smoke_path);

    assert(smoke.size() > 64 * 1024);
    assert(smoke.size() < minikv::client_max_response_line_bytes);

    assert_contains(smoke, "\"runtime_smoke_evidence_version\":\"mini-kv-runtime-smoke-evidence.v17\"");
    assert_contains(smoke, "\"project\":\"mini-kv\"");
    assert_contains(smoke, "\"project_version\":\"0.102.0\"");
    assert_contains(smoke, "\"release_version\":\"v102\"");
    assert_contains(smoke, "\"path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(smoke, "\"read_only\":true");
    assert_contains(smoke, "\"execution_allowed\":false");
    assert_contains(smoke, "\"restore_execution_allowed\":false");
    assert_contains(smoke, "\"order_authoritative\":false");
    assert_contains(smoke, "\"java_transaction_chain_connected\":false");
    assert_contains(smoke, "\"consumer_hint\":\"Node v246 manual sandbox connection precheck upstream receipt verification\"");
    assert_contains(smoke, "command contract formatter module split");
    assert_contains(smoke, "command response formatter module split");
    assert_contains(smoke, "sandbox connection receipt echo marker");

    assert_contains(smoke, "\"primary\":\"SMOKEJSON\"");
    assert_contains(smoke, "\"supporting\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(smoke, "\"forbidden\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(smoke, "\"SMOKEJSON version matches 0.102.0\"");
    assert_contains(smoke, "\"SMOKEJSON reports write_commands_executed=false and runtime_write_observed=false\"");
    assert_contains(smoke, "SMOKEJSON exposes failure taxonomy categories and taxonomy_digest");
    assert_contains(smoke, "SMOKEJSON exposes operator_window.identity_neutral_proof=true");
    assert_contains(smoke, "SMOKEJSON and INFOJSON expose ci_evidence.artifact_path_hint=c/80/ and no_restore_proof=true");
    assert_contains(smoke, "SMOKEJSON and INFOJSON expose artifact_retention.retention_days=30 and github_artifact_upload_attempted=false");
    assert_contains(smoke, "SMOKEJSON exposes live_read_session.session_id_echo=mini-kv-live-read-v102 and read_command_list_digest=fnv1a64:5bef33f2fbe65cc5");
    assert_contains(smoke, "SMOKEJSON exposes binary_provenance.provenance_digest=fnv1a64:d9776044d393ecbc and production_binary_claimed=false");
    assert_contains(smoke, "SMOKEJSON exposes retention_provenance_check.check_digest=fnv1a64:8cf7cc0df218035f and managed_audit_write_executed=false");
    assert_contains(smoke, "SMOKEJSON exposes retention_provenance_replay_marker.marker_digest=fnv1a64:6b9ef93f40cbd343 and replay_executed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_adapter_restore_boundary_receipt.receipt_digest=fnv1a64:1177efbaceb52c1c and adapter_write_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_adapter_non_authoritative_storage_receipt.receipt_digest=fnv1a64:667590b83f510a58 and managed_audit_store=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.consumed_release_version=v89");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.external_adapter_storage_backend=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.receipt_digest=fnv1a64:9bacde73d6d07097");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.credential_read_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.consumed_release_version=v90");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.receipt_digest=fnv1a64:f0cae7a4ce0674c2");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.sandbox_adapter_storage_backend=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.credential_value_read_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.sandbox_managed_audit_state_write_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.consumed_release_version=v95");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.consumed_artifact_path_hint=c/95/");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.consumed_receipt_digest=fnv1a64:ceaed265f7f9560c");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.source_operator_packet_profile=managed-audit-manual-sandbox-connection-operator-packet.v1");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.packet_mode=manual-sandbox-connection-operator-packet-only");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.marker_digest=fnv1a64:beb8dd6a0b102a11");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.connection_execution_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.credential_value_read_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.managed_audit_write_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_receipt_echo_marker.node_auto_start_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.consumed_release_version=v96");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.consumed_marker_digest=fnv1a64:b9fc556875ea625b");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.receipt_digest=fnv1a64:69e377d01f15fd57");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.manual_window_open_by_default=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.node_auto_start_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.mini_kv_auto_start_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_connection_no_start_guard_receipt.schema_rehearsal_execution_allowed=false");

    assert_contains(smoke, "\"server\":\"minikv_server 6497 127.0.0.1\"");
    assert_contains(smoke, "\"client\":\"minikv_client 127.0.0.1 6497 5000 --connect-retries 10 --retry-delay-ms 100\"");
    assert_contains(smoke, "\"commands\":[\"SMOKEJSON\",\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"GET restore:real-read-token\",\"QUIT\"]");

    assert_contains(smoke, "\"target_version\":\"Node v231\"");
    assert_contains(smoke, "\"mode\":\"manual sandbox connection preflight verification\"");
    assert_contains(smoke, "\"default_enabled\":false");
    assert_contains(smoke, "\"mini-kv is already running and user enabled a safe manual read-only window\"");
    assert_contains(smoke, "\"node_must_verify\":\"managed_audit_sandbox_connection_no_start_guard_receipt.consumed_release_version equals v96");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.consumed_artifact_path_hint equals c/96/");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.consumed_marker_digest equals fnv1a64:b9fc556875ea625b");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.current_artifact_path_hint equals c/102/");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.preflight_gate_version equals Node v230");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.manual_window_flag_name equals ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.manual_window_open_by_default is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.receipt_digest equals fnv1a64:69e377d01f15fd57");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.node_auto_start_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.java_auto_start_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.mini_kv_auto_start_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.connection_execution_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.credential_value_read_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.schema_rehearsal_execution_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_no_start_guard_receipt.managed_audit_write_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.consumed_release_version equals v95");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.consumed_artifact_path_hint equals c/95/");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.consumed_receipt_digest equals fnv1a64:ceaed265f7f9560c");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.source_operator_packet_version equals Node v228");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.source_operator_packet_profile equals managed-audit-manual-sandbox-connection-operator-packet.v1");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.packet_mode equals manual-sandbox-connection-operator-packet-only");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.owner_approval_artifact_id_field equals ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.credential_handle_name_field equals ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.schema_rehearsal_id_field equals ORDEROPS_MANAGED_AUDIT_SCHEMA_REHEARSAL_ID");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.rollback_path_id_field equals ORDEROPS_MANAGED_AUDIT_ROLLBACK_PATH_ID");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.timeout_budget_ms equals 15000");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.manual_abort_marker equals ORDEROPS_MANAGED_AUDIT_MANUAL_ABORT");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.required_operator_field_count equals 6");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.marker_digest equals fnv1a64:beb8dd6a0b102a11");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.connection_execution_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.credential_value_read_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.managed_audit_write_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.participates_in_sandbox_connection is false");
    assert_contains(smoke, "managed_audit_sandbox_connection_receipt_echo_marker.node_auto_start_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.consumed_artifact_path_hint equals c/90/");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.consumed_receipt_digest equals fnv1a64:0dfb07cd2f8de289");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.current_artifact_path_hint equals c/102/");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.adapter_target equals sandbox managed audit adapter dry-run");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.receipt_digest equals fnv1a64:f0cae7a4ce0674c2");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.sandbox_adapter_storage_backend is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.credential_value_read_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.sandbox_managed_audit_state_write_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.sandbox_dry_run_records_written is false");
    assert_contains(smoke, "managed_audit_external_adapter_non_participation_receipt.receipt_digest equals fnv1a64:9bacde73d6d07097");
    assert_contains(smoke, "adapter_shell_non_storage_guard_receipt.receipt_digest equals fnv1a64:0174fa831d17cea4");
    assert_contains(smoke, "command_dispatch_quality_receipt.receipt_digest equals fnv1a64:0f2caa6931b482b8");
    assert_contains(smoke, "binary_provenance.provenance_digest equals fnv1a64:d9776044d393ecbc");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.runtime_role equals runtime evidence provider only");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.read_only is true");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.execution_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.participates_in_sandbox_adapter is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.credential_value_required is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.production_credential_read_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.schema_migration_execution_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.sandbox_schema_authoritative is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.write_handler_changed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.admin_handler_changed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.restore_execution_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.load_restore_compact_executed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.managed_audit_write_executed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.order_authoritative is false");
    assert_contains(smoke, "retention_provenance_check.check_digest equals fnv1a64:8cf7cc0df218035f");
    assert_contains(smoke, "retention_provenance_replay_marker.current_artifact_path_hint equals c/102/");
    assert_contains(smoke, "retention_provenance_replay_marker.marker_digest equals fnv1a64:6b9ef93f40cbd343");
    assert_contains(smoke, "managed_audit_adapter_restore_boundary_receipt.current_artifact_path_hint equals c/102/");
    assert_contains(smoke, "managed_audit_adapter_restore_boundary_receipt.receipt_digest equals fnv1a64:1177efbaceb52c1c");
    assert_contains(smoke, "managed_audit_adapter_non_authoritative_storage_receipt.current_artifact_path_hint equals c/102/");
    assert_contains(smoke, "managed_audit_adapter_non_authoritative_storage_receipt.receipt_digest equals fnv1a64:667590b83f510a58");
    assert_contains(smoke, "live_read_session.session_id_echo equals mini-kv-live-read-v102");
    assert_contains(smoke, "live_read_session.read_command_list_digest equals fnv1a64:5bef33f2fbe65cc5");
    assert_contains(smoke, "ci_evidence.artifact_path_hint equals c/80/");
    assert_contains(smoke, "artifact_retention.artifact_path_hint equals c/81/");
    assert_contains(smoke, "artifact_retention.production_window_allowed is false");
    assert_contains(smoke, "execute LOAD/COMPACT/SETNXEX/restore");
    assert_contains(smoke, "upload CI artifacts");
    assert_contains(smoke, "open a production window");
    assert_contains(smoke, "infer operator identity from mini-kv");
    assert_contains(smoke, "read production managed audit credentials");
    assert_contains(smoke, "execute external adapter schema migration");
    assert_contains(smoke, "make mini-kv an external managed audit adapter participant");

    assert_contains(smoke, "\"operator_window\":{\"consumer\":\"Node v200 real-read window CI archive artifact manifest\"");
    assert_contains(smoke, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(smoke, "\"session_id_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(smoke, "\"server_uptime_bucket\":\"lt_60s\"");
    assert_contains(smoke, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(smoke, "\"read_command_count\":4");
    assert_contains(smoke, "\"read_commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(smoke, "\"write_commands_allowed\":false");
    assert_contains(smoke, "\"auto_start_allowed\":false");
    assert_contains(smoke, "\"identity_neutral_proof\":true");
    assert_contains(smoke, "\"node_action\":\"verify digest before importing manual window results\"}");
    assert_contains(smoke, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(smoke, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(smoke, "\"no_restore_proof\":true");
    assert_contains(smoke, "\"upload_allowed\":false");
    assert_contains(smoke, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(smoke, "\"artifact_root\":\"c/\"");
    assert_contains(smoke, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(smoke, "\"retention_days\":30");
    assert_contains(smoke, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_contains(smoke, "\"release_evidence_ready\":true");
    assert_contains(smoke, "\"github_artifact_upload_attempted\":false");
    assert_contains(smoke, "\"production_window_allowed\":false");
    assert_contains(smoke, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(smoke, "\"source_version\":\"0.102.0\"");
    assert_contains(smoke, "\"artifact_path_hint\":\"c/102/\"");
    assert_contains(smoke, "\"runtime_binary_hint\":\"cmake-build-v102/minikv_server and cmake-build-v102/minikv_client from the current CMake build\"");
    assert_contains(smoke, "\"release_manifest_path\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(smoke, "\"runtime_smoke_evidence_path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(smoke, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(smoke, "\"load_restore_compact_executed\":false");
    assert_contains(smoke, "\"production_binary_claimed\":false");
    assert_contains(smoke, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(smoke, "\"expected_binary_provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(smoke, "\"check_digest\":\"fnv1a64:8cf7cc0df218035f\"");
    assert_contains(smoke, "\"managed_audit_write_executed\":false");
    assert_contains(smoke, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v84\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_contains(smoke, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(smoke, "\"marker_digest\":\"fnv1a64:6b9ef93f40cbd343\"");
    assert_contains(smoke, "\"replay_executed\":false");
    assert_contains(smoke, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(smoke, "\"consumed_by\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v85\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_contains(smoke, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:1177efbaceb52c1c\"");
    assert_contains(smoke, "\"adapter_write_allowed\":false");
    assert_contains(smoke, "\"restore_execution_allowed\":false");
    assert_contains(smoke, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:667590b83f510a58\"");
    assert_contains(smoke, "\"managed_audit_store\":false");
    assert_contains(smoke, "\"storage_write_allowed\":false");
    assert_contains(smoke, "\"admin_commands_allowed\":false");
    assert_contains(smoke, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v87\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/87/\"");
    assert_contains(smoke, "\"consumed_receipt_digest\":\"fnv1a64:111f0daf1283eab6\"");
    assert_contains(smoke, "\"dispatch_family\":\"runtime_evidence_command_family\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:0f2caa6931b482b8\"");
    assert_contains(smoke, "\"dispatch_split_applied\":true");
    assert_contains(smoke, "\"write_handler_changed\":false");
    assert_contains(smoke, "\"admin_handler_changed\":false");
    assert_contains(smoke, "\"wal_snapshot_restore_touched\":false");
    assert_contains(smoke, "\"behavior_changed\":false");
    assert_contains(smoke, "\"fixture_contract_preserved\":true");
    assert_contains(smoke, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_contains(smoke, "\"consumed_by\":\"Node v220 managed audit adapter interface and disabled shell\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v88\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/88/\"");
    assert_contains(smoke, "\"consumed_receipt_digest\":\"fnv1a64:4aa6d12fb067e2a6\"");
    assert_contains(smoke, "\"adapter_shell\":\"ManagedAuditAdapter disabled shell\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:0174fa831d17cea4\"");
    assert_contains(smoke, "\"adapter_shell_storage_backend\":false");
    assert_contains(smoke, "\"storage_backend_allowed\":false");
    assert_contains(smoke, "\"local_dry_run_records_written\":false");
    assert_contains(smoke, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_contains(smoke, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v89\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_contains(smoke, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_contains(smoke, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_contains(smoke, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:9bacde73d6d07097\"");
    assert_contains(smoke, "\"external_adapter_storage_backend\":false");
    assert_contains(smoke, "\"participates_in_external_adapter\":false");
    assert_contains(smoke, "\"credential_required\":false");
    assert_contains(smoke, "\"credential_read_allowed\":false");
    assert_contains(smoke, "\"migration_required\":false");
    assert_contains(smoke, "\"migration_execution_allowed\":false");
    assert_contains(smoke, "\"schema_authoritative\":false");
    assert_contains(smoke, "\"managed_audit_write_executed\":false");
    assert_contains(smoke, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node v225 managed audit sandbox adapter dry-run package\"");
    assert_contains(smoke, "\"consumed_by\":\"Node v224 managed audit sandbox adapter dry-run plan\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v90\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/90/\"");
    assert_contains(smoke, "\"consumed_receipt_digest\":\"fnv1a64:0dfb07cd2f8de289\"");
    assert_contains(smoke, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(smoke, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_contains(smoke, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:f0cae7a4ce0674c2\"");
    assert_contains(smoke, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(smoke, "\"participates_in_sandbox_adapter\":false");
    assert_contains(smoke, "\"credential_value_required\":false");
    assert_contains(smoke, "\"credential_value_read_allowed\":false");
    assert_contains(smoke, "\"production_credential_read_allowed\":false");
    assert_contains(smoke, "\"schema_migration_execution_allowed\":false");
    assert_contains(smoke, "\"sandbox_schema_authoritative\":false");
    assert_contains(smoke, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(smoke, "\"sandbox_dry_run_records_written\":false");
    assert_contains(smoke, "\"managed_audit_sandbox_connection_receipt_echo_marker\":{\"consumer\":\"Node v229 manual sandbox connection packet verification\"");
    assert_contains(smoke, "\"consumed_by\":\"Node v228 managed audit manual sandbox connection operator packet\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v95\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/95/\"");
    assert_contains(smoke, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(smoke, "\"source_operator_packet_version\":\"Node v228\"");
    assert_contains(smoke, "\"source_operator_packet_profile\":\"managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_contains(smoke, "\"packet_mode\":\"manual-sandbox-connection-operator-packet-only\"");
    assert_contains(smoke, "\"owner_approval_artifact_id_field\":\"ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID\"");
    assert_contains(smoke, "\"credential_handle_name_field\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(smoke, "\"schema_rehearsal_id_field\":\"ORDEROPS_MANAGED_AUDIT_SCHEMA_REHEARSAL_ID\"");
    assert_contains(smoke, "\"rollback_path_id_field\":\"ORDEROPS_MANAGED_AUDIT_ROLLBACK_PATH_ID\"");
    assert_contains(smoke, "\"timeout_budget_ms\":15000");
    assert_contains(smoke, "\"manual_abort_marker\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_ABORT\"");
    assert_contains(smoke, "\"required_operator_field_count\":6");
    assert_contains(smoke, "\"marker_digest\":\"fnv1a64:beb8dd6a0b102a11\"");
    assert_contains(smoke, "\"connection_execution_allowed\":false");
    assert_contains(smoke, "\"managed_audit_write_allowed\":false");
    assert_contains(smoke, "\"participates_in_sandbox_connection\":false");
    assert_contains(smoke, "\"node_auto_start_allowed\":false");
    assert_contains(smoke, "\"managed_audit_sandbox_connection_no_start_guard_receipt\":{\"consumer\":\"Node v231 manual sandbox connection preflight verification\"");
    assert_contains(smoke, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v96\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_contains(smoke, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_contains(smoke, "\"preflight_gate_version\":\"Node v230\"");
    assert_contains(smoke, "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_contains(smoke, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:69e377d01f15fd57\"");
    assert_contains(smoke, "\"manual_window_open_by_default\":false");
    assert_contains(smoke, "\"java_auto_start_allowed\":false");
    assert_contains(smoke, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(smoke, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(smoke,
                    "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\":");
    assert_contains(smoke,
                    "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review.v1\"");
    assert_contains(smoke,
                    "\"source_review_state\":\"credential-resolver-approval-required-implementation-readiness-review-ready\"");
    assert_contains(smoke, "\"readiness_review\":{\"review_mode\":\"node-v281-approval-required-implementation-readiness-review-only\"");
    assert_contains(smoke, "\"required_artifact_count\":18");
    assert_contains(smoke, "\"approval_required_implementation_readiness_non_participation_receipt_only\":true");
    assert_contains(smoke, "\"ready_for_java_v116_mini_kv_v122_echo\":true");
    assert_contains(smoke, "\"current_artifact_path_hint\":\"d/122/\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:b6e1efa8878b64d6\"");
    assert_contains(
        smoke,
        "SMOKEJSON exposes credential_resolver_approval_required_implementation_readiness_non_participation_receipt for Node v282");
    assert_contains(
        smoke,
        "SMOKEJSON credential resolver approval-required implementation readiness receipt keeps source Node v281 check_count=21, passed_check_count=21, boundary_count=6, echo_ready_boundary_count=6, blocked_for_implementation_boundary_count=6, required_artifact_count=18, java_v116_echo_hint_count=6, mini_kv_v122_receipt_hint_count=6, and production_blocker_count=0");
    assert_contains(
        smoke,
        "SMOKEJSON credential resolver approval-required implementation readiness receipt keeps all six approval-required boundary codes and all eighteen required artifact ids for Java v116 and mini-kv v122 echo");
    assert_contains(
        smoke,
        "SMOKEJSON credential resolver approval-required implementation readiness receipt keeps real_resolver_implementation_allowed=false, resolver_client_instantiated=false, secret_provider_instantiated=false, credential_value_read_allowed=false, raw_endpoint_url_parsed=false, external_request_sent=false, storage_write_allowed=false, approval_ledger_write_allowed=false, schema_migration_executed=false, load_restore_compact_executed=false, setnxex_execution_allowed=false, and managed_audit_storage_backend=false");

    assert_contains(smoke, "\"runtime smoke evidence only\"");
    assert_contains(smoke, "\"read-only aggregate only\"");
    assert_contains(smoke, "\"live-read session hint only\"");
    assert_contains(smoke, "\"binary provenance hint only\"");
    assert_contains(smoke, "\"retention provenance check only\"");
    assert_contains(smoke, "\"retention provenance replay marker only\"");
    assert_contains(smoke, "\"managed audit adapter restore boundary receipt only\"");
    assert_contains(smoke, "\"managed audit adapter non-authoritative storage receipt only\"");
    assert_contains(smoke, "\"adapter shell non-storage guard receipt only\"");
    assert_contains(smoke, "\"not adapter shell storage backend\"");
    assert_contains(smoke, "\"no local dry-run records written\"");
    assert_contains(smoke, "\"managed audit external adapter non-participation receipt only\"");
    assert_contains(smoke, "\"not external adapter storage backend\"");
    assert_contains(smoke, "\"no external adapter participation\"");
    assert_contains(smoke, "\"no credential read\"");
    assert_contains(smoke, "\"no migration execution\"");
    assert_contains(smoke, "\"managed audit sandbox adapter non-participation receipt only\"");
    assert_contains(smoke, "\"managed audit receipt formatter split only\"");
    assert_contains(smoke, "\"runtime evidence receipt formatter split only\"");
    assert_contains(smoke, "\"command contract formatter split only\"");
    assert_contains(smoke, "\"command response formatter split only\"");
    assert_contains(smoke, "\"no INFOJSON/SMOKEJSON field contract expansion from formatter split\"");
    assert_contains(smoke, "\"no INFOJSON/SMOKEJSON field contract expansion from runtime evidence receipt split\"");
    assert_contains(smoke, "\"no CHECKJSON/EXPLAINJSON field contract expansion from command contract split\"");
    assert_contains(smoke, "\"no runtime response field contract expansion from command response split\"");
    assert_contains(smoke, "\"not sandbox audit storage backend\"");
    assert_contains(smoke, "\"no sandbox adapter participation\"");
    assert_contains(smoke, "\"no sandbox credential value read\"");
    assert_contains(smoke, "\"no production credential read\"");
    assert_contains(smoke, "\"no sandbox managed audit state write\"");
    assert_contains(smoke, "\"no sandbox schema migration execution\"");
    assert_contains(smoke, "\"managed audit sandbox connection receipt echo marker only\"");
    assert_contains(smoke, "\"operator packet field echo only\"");
    assert_contains(smoke, "\"not sandbox connection participant\"");
    assert_contains(smoke, "\"no sandbox connection execution\"");
    assert_contains(smoke, "\"no credential handle value read\"");
    assert_contains(smoke, "\"no schema rehearsal execution\"");
    assert_contains(smoke, "\"no managed audit state write from connection marker\"");
    assert_contains(smoke, "\"no node auto-start from connection marker\"");
    assert_contains(smoke, "\"CI evidence hint only\"");
    assert_contains(smoke, "\"artifact retention evidence only\"");
    assert_contains(smoke, "\"artifact path hint only\"");
    assert_contains(smoke, "\"retention dry-run only\"");
    assert_contains(smoke, "\"taxonomy digest sample only\"");
    assert_contains(smoke, "\"operator-window proof only\"");
    assert_contains(smoke, "\"identity-neutral proof only\"");
    assert_contains(smoke, "\"no-restore proof only\"");
    assert_contains(smoke, "\"no CI artifact upload\"");
    assert_contains(smoke, "\"no production window opening\"");
    assert_contains(smoke, "\"no operator identity accepted or persisted\"");
    assert_contains(smoke, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(smoke, "\"mini-kv remains not Java order authority\"");
    assert_contains(smoke, "\"SMOKEJSON is a read adapter target, not an execution gate\"");
    assert_contains(smoke, "\"Binary provenance hint aligns runtime binary and fixture paths, but does not claim production binary attestation\"");
    assert_contains(smoke, "\"Retention provenance check links retained evidence and binary provenance without writing managed audit state\"");
    assert_contains(smoke, "\"Retention provenance replay marker is a read-only consumption receipt, not a restore execution\"");
    assert_contains(smoke, "\"Managed audit adapter restore boundary receipt blocks mini-kv writes and restore execution\"");
    assert_contains(smoke, "\"Managed audit external adapter non-participation receipt blocks external adapter storage backend use, credential reads, migration execution, and adapter participation\"");
    assert_contains(smoke, "\"Managed audit sandbox adapter non-participation receipt blocks sandbox audit storage backend use, credential value reads, schema migration execution, and managed audit state writes\"");
    assert_contains(smoke, "\"Managed audit sandbox connection receipt echo marker echoes field names only; mini-kv does not connect, read credential values, run schema rehearsal, or write managed audit state\"");
    assert_contains(smoke, "\"Managed audit receipt formatter split is code organization evidence, not a new managed audit backend\"");
    assert_contains(smoke, "\"Runtime evidence receipt formatter split is code organization evidence, not a new runtime execution path\"");
    assert_contains(smoke, "\"Command contract formatter split is code organization evidence, not a new command execution path\"");
    assert_contains(smoke, "\"Command response formatter split is code organization evidence, not a new runtime execution path\"");
    assert_contains(smoke, "\"Live-read session hint is read-only evidence, not permission to auto-start mini-kv\"");
    assert_contains(smoke, "\"Artifact retention evidence is a read-only rehearsal hint, not real GitHub artifact retention configuration\"");
    assert_contains(smoke, "\"CI evidence hint is a path and boundary hint, not a real GitHub artifact upload\"");
    assert_contains(smoke, "\"Operator-window proof is identity-neutral evidence, not authentication or production authorization\"");
    assert_contains(smoke, "\"real-read smoke scope is unclear\"");
    assert_contains(smoke, "\"taxonomy digest is missing or mismatched\"");
    assert_contains(smoke, "\"operator window proof is missing or unclear\"");
    assert_contains(smoke, "\"ci evidence hint is missing or unclear\"");
    assert_contains(smoke, "\"artifact retention evidence is missing or unclear\"");
    assert_contains(smoke, "\"artifact retention days are mismatched\"");
    assert_contains(smoke, "\"live-read session echo is missing or mismatched\"");
    assert_contains(smoke, "\"read command list digest is missing or mismatched\"");
    assert_contains(smoke, "\"binary provenance digest is missing or mismatched\"");
    assert_contains(smoke, "\"runtime binary path and fixture path alignment is unclear\"");
    assert_contains(smoke, "\"retention provenance check digest is missing or mismatched\"");
    assert_contains(smoke, "\"retention provenance replay marker is missing or mismatched\"");
    assert_contains(smoke, "\"managed audit adapter restore boundary receipt is missing or mismatched\"");
    assert_contains(smoke, "\"managed audit external adapter non-participation receipt is missing or mismatched\"");
    assert_contains(smoke, "\"needs mini-kv to become external adapter storage backend\"");
    assert_contains(smoke, "\"needs mini-kv to read managed audit credentials\"");
    assert_contains(smoke, "\"needs mini-kv to execute external adapter schema migration\"");
    assert_contains(smoke, "\"managed audit sandbox adapter non-participation receipt is missing or mismatched\"");
    assert_contains(smoke, "\"managed audit sandbox connection receipt echo marker is missing or mismatched\"");
    assert_contains(smoke, "\"needs mini-kv to become sandbox audit storage backend\"");
    assert_contains(smoke, "\"needs mini-kv to read sandbox credential value\"");
    assert_contains(smoke, "\"needs mini-kv to write sandbox managed audit state\"");
    assert_contains(smoke, "\"needs mini-kv to execute sandbox adapter schema migration\"");
    assert_contains(smoke, "\"needs mini-kv to execute sandbox connection\"");
    assert_contains(smoke, "\"needs mini-kv to read sandbox credential handle value\"");
    assert_contains(smoke, "\"needs mini-kv to run schema rehearsal\"");
    assert_contains(smoke, "\"needs mini-kv to write managed audit state from connection marker\"");
    assert_contains(smoke, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(smoke, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(smoke, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(smoke, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(smoke, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.102.0\"");
    assert_contains(smoke, "\"expected_taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(smoke, "\"node_action\":\"verify digest before importing manual window results\"");
    assert_contains(smoke, "\"id\":\"closed-window\",\"source\":\"operator_window\"");
    assert_contains(smoke, "\"id\":\"connection-refused\",\"source\":\"tcp_connect\"");
    assert_contains(smoke, "\"id\":\"timeout\",\"source\":\"tcp_read\"");
    assert_contains(smoke, "\"id\":\"invalid-json\",\"source\":\"SMOKEJSON_parse\"");
    assert_contains(smoke, "\"id\":\"read-command-failed\",\"source\":\"runtime_read_command\"");
    assert_contains(smoke, "\"id\":\"unsafe-surface\",\"source\":\"adapter_command_plan\"");
    assert_contains(smoke, "\"id\":\"unexpected-write-signal\",\"source\":\"runtime_smoke_diagnostics\"");
    assert_contains(smoke, "\"node_action\":\"verify digest and stop before import when unsafe-surface or unexpected-write-signal appears\"");

    const std::vector<std::filesystem::path> required_paths = {
        smoke_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" /
            "credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-boundary-smoke-manifest.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "readonly" / "runtime-read-field-guide.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(smoke, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"schema_version\":1");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_smoke\"");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.102.0\"");
    assert_contains(result.response, "\"server\":{\"protocol\":[\"inline\"]");
    assert_contains(result.response, "\"uptime_seconds\":0");
    assert_contains(result.response, "\"store\":{\"live_keys\":0,\"order_authoritative\":false}");
    assert_contains(result.response, "\"wal\":{\"enabled\":false,\"status\":\"disabled\",\"compact_recommended\":false}");
    assert_contains(result.response, "\"connection_stats\":{\"available\":false}");
    assert_contains(result.response, "\"real_read\":{\"allowed\":true,\"commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(result.response, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(result.response, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(result.response, "\"session_id_echo\":\"mini-kv-live-read-v102\"");
    assert_contains(result.response, "\"server_uptime_bucket\":\"lt_60s\"");
    assert_contains(result.response, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(result.response, "\"read_command_count\":4");
    assert_contains(result.response, "\"read_commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(result.response, "\"write_commands_allowed\":false");
    assert_contains(result.response, "\"auto_start_allowed\":false");
    assert_contains(result.response, "\"operator_window\":{\"consumer\":\"Node v200 real-read window CI archive artifact manifest\"");
    assert_contains(result.response, "\"identity_neutral_proof\":true");
    assert_contains(result.response, "\"node_action\":\"verify digest before importing manual window results\"}");
    assert_contains(result.response, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(result.response, "\"no_restore_proof\":true");
    assert_contains(result.response, "\"upload_allowed\":false");
    assert_contains(result.response, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(result.response, "\"artifact_root\":\"c/\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(result.response, "\"retention_days\":30");
    assert_contains(result.response, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_contains(result.response, "\"release_evidence_ready\":true");
    assert_contains(result.response, "\"github_artifact_upload_attempted\":false");
    assert_contains(result.response, "\"production_window_allowed\":false");
    assert_contains(result.response, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(result.response, "\"source_version\":\"0.102.0\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v86\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:f39d8e3ef98654ea\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:667590b83f510a58\"");
    assert_contains(result.response, "\"managed_audit_store\":false");
    assert_contains(result.response, "\"storage_write_allowed\":false");
    assert_contains(result.response, "\"admin_commands_allowed\":false");
    assert_contains(result.response, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0f2caa6931b482b8\"");
    assert_contains(result.response, "\"dispatch_split_applied\":true");
    assert_contains(result.response, "\"wal_snapshot_restore_touched\":false");
    assert_contains(result.response, "\"fixture_contract_preserved\":true");
    assert_contains(result.response, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v220 managed audit adapter interface and disabled shell\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v88\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:4aa6d12fb067e2a6\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:0174fa831d17cea4\"");
    assert_contains(result.response, "\"adapter_shell_storage_backend\":false");
    assert_contains(result.response, "\"local_dry_run_records_written\":false");
    assert_contains(result.response, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v89\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_contains(result.response, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:9bacde73d6d07097\"");
    assert_contains(result.response, "\"external_adapter_storage_backend\":false");
    assert_contains(result.response, "\"participates_in_external_adapter\":false");
    assert_contains(result.response, "\"credential_required\":false");
    assert_contains(result.response, "\"credential_read_allowed\":false");
    assert_contains(result.response, "\"migration_required\":false");
    assert_contains(result.response, "\"migration_execution_allowed\":false");
    assert_contains(result.response, "\"schema_authoritative\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node v225 managed audit sandbox adapter dry-run package\"");
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
    assert_contains(result.response, "\"managed_audit_sandbox_connection_receipt_echo_marker\":{\"consumer\":\"Node v229 manual sandbox connection packet verification\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v228 managed audit manual sandbox connection operator packet\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v95\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/95/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(result.response, "\"source_operator_packet_profile\":\"managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_contains(result.response, "\"packet_mode\":\"manual-sandbox-connection-operator-packet-only\"");
    assert_contains(result.response, "\"credential_handle_name_field\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:beb8dd6a0b102a11\"");
    assert_contains(result.response, "\"connection_execution_allowed\":false");
    assert_contains(result.response, "\"managed_audit_write_allowed\":false");
    assert_contains(result.response, "\"participates_in_sandbox_connection\":false");
    assert_contains(result.response, "\"node_auto_start_allowed\":false");
    assert_contains(result.response, "\"managed_audit_sandbox_connection_no_start_guard_receipt\":{\"consumer\":\"Node v231 manual sandbox connection preflight verification\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v96\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(result.response, "\"preflight_gate_version\":\"Node v230\"");
    assert_contains(result.response, "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_contains(result.response, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:69e377d01f15fd57\"");
    assert_contains(result.response, "\"manual_window_open_by_default\":false");
    assert_contains(result.response, "\"java_auto_start_allowed\":false");
    assert_contains(result.response, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(result.response, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(result.response, "\"production_binary_claimed\":false");
    assert_contains(result.response, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(result.response, "\"expected_binary_provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_contains(result.response, "\"check_digest\":\"fnv1a64:8cf7cc0df218035f\"");
    assert_contains(result.response, "\"managed_audit_write_executed\":false");
    assert_contains(result.response, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v84\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_contains(result.response, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:6b9ef93f40cbd343\"");
    assert_contains(result.response, "\"replay_executed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v85\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:1177efbaceb52c1c\"");
    assert_contains(result.response, "\"adapter_write_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(result.response, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(result.response, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(result.response, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.102.0\"");
    assert_contains(result.response, "\"expected_taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response, "\"node_action\":\"verify digest before importing manual window results\"");
    assert_contains(result.response, "\"id\":\"closed-window\",\"source\":\"operator_window\"");
    assert_contains(result.response, "\"id\":\"connection-refused\",\"source\":\"tcp_connect\"");
    assert_contains(result.response, "\"id\":\"timeout\",\"source\":\"tcp_read\"");
    assert_contains(result.response, "\"id\":\"invalid-json\",\"source\":\"SMOKEJSON_parse\"");
    assert_contains(result.response, "\"id\":\"read-command-failed\",\"source\":\"runtime_read_command\"");
    assert_contains(result.response, "\"id\":\"unsafe-surface\",\"source\":\"adapter_command_plan\"");
    assert_contains(result.response, "\"id\":\"unexpected-write-signal\",\"source\":\"runtime_smoke_diagnostics\"");
    assert_contains(result.response, "\"safe_to_auto_start\":false");
    assert_contains(result.response, "\"write_risk\":true");
    assert_contains(result.response, "\"write_commands_executed\":false");
    assert_contains(result.response, "\"admin_commands_executed\":false");
    assert_contains(result.response, "\"runtime_write_observed\":false");
    assert_contains(result.response,
                    "\"node_consumption\":\"Node v284 may verify the mini-kv v126 credential resolver implementation plan non-participation receipt");
    assert_contains(result.response,
                    "Node v282 may verify the mini-kv v122 credential resolver approval-required implementation readiness non-participation receipt");
    assert_contains(result.response,
                    "Node v275 may verify the mini-kv v121 credential resolver approval-required boundary non-participation receipt");
    assert_contains(result.response,
                    "Node v274 may verify the mini-kv v120 credential resolver disabled implementation candidate non-participation receipt");
    assert_contains(result.response,
                    "Node v272 may verify the mini-kv v119 credential resolver pre-implementation plan intake non-participation receipt");
    assert_contains(result.response, "Node v269 may verify the mini-kv v118 credential resolver production-readiness blocked-decision non-participation receipt");
    assert_contains(result.response, "Node v267 may verify the mini-kv v117 credential resolver fake-shell archive non-participation receipt");
    assert_contains(result.response, "Node v265 may verify the mini-kv v116 test-only resolver shell non-participation receipt");
    assert_contains(result.response, "Node v246 may verify the mini-kv v108 manual sandbox connection precheck non-participation receipt");
    assert_contains(result.response, "Node v244 may still verify the mini-kv v107 manual sandbox dry-run command non-participation receipt");
    assert_contains(result.response, "Node v263 may verify the mini-kv v115 disabled credential resolver precheck non-participation receipt");
    assert_contains(result.response, "Node v261 may verify the mini-kv v114 credential resolver non-participation receipt");
    assert_contains(result.response, "Node v259 may verify the mini-kv v113 sandbox endpoint handle non-participation receipt");
    assert_contains(result.response, "Node v239 may still verify the mini-kv operator window no-start/no-write receipt");
    assert_contains(result.response, "\"manual_sandbox_dry_run_command_non_participation_receipt\"");
    assert_contains(result.response, "\"manual_sandbox_connection_precheck_non_participation_receipt\"");
    assert_contains(result.response, "\"sandbox_endpoint_handle_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_non_participation_receipt\"");
    assert_contains(result.response, "\"disabled_credential_resolver_precheck_non_participation_receipt\"");
    assert_contains(result.response, "\"test_only_resolver_shell_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_fake_shell_archive_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_production_readiness_blocked_decision_non_participation_receipt\"");
    assert_contains(result.response, "\"credential_resolver_pre_implementation_plan_intake_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_disabled_implementation_candidate_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_approval_required_boundary_non_participation_receipt\"");
    assert_contains(result.response,
                    "\"credential_resolver_approval_required_implementation_readiness_non_participation_receipt\"");
    assert_contains(
        result.response,
        "\"source_profile_version\":\"managed-audit-manual-sandbox-connection-credential-resolver-approval-required-implementation-readiness-review.v1\"");
    assert_contains(result.response,
                    "\"source_review_state\":\"credential-resolver-approval-required-implementation-readiness-review-ready\"");
    assert_contains(result.response,
                    "\"readiness_review\":{\"review_mode\":\"node-v281-approval-required-implementation-readiness-review-only\"");
    assert_contains(result.response, "\"required_artifact_count\":18");
    assert_contains(result.response,
                    "\"approval_required_implementation_readiness_non_participation_receipt_only\":true");
    assert_contains(result.response, "\"ready_for_java_v116_mini_kv_v122_echo\":true");
    assert_contains(result.response, "\"current_artifact_path_hint\":\"d/122/\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:b6e1efa8878b64d6\"");
    assert_contains(result.response, "\"read_only_aggregate\"");
    assert_contains(result.response, "\"does_not_execute_load_compact_setnxex_or_restore\"");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SMOKEJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\",\"store_read\",\"wal_metadata_read_when_enabled\"]");
    assert_contains(result.response, "\"warnings\":[\"not a write command\"]");

    return 0;
}
