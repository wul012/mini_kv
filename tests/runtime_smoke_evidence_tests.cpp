#include "minikv/command.hpp"
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

    assert_contains(smoke, "\"runtime_smoke_evidence_version\":\"mini-kv-runtime-smoke-evidence.v13\"");
    assert_contains(smoke, "\"project\":\"mini-kv\"");
    assert_contains(smoke, "\"project_version\":\"0.95.0\"");
    assert_contains(smoke, "\"release_version\":\"v95\"");
    assert_contains(smoke, "\"path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(smoke, "\"read_only\":true");
    assert_contains(smoke, "\"execution_allowed\":false");
    assert_contains(smoke, "\"restore_execution_allowed\":false");
    assert_contains(smoke, "\"order_authoritative\":false");
    assert_contains(smoke, "\"java_transaction_chain_connected\":false");
    assert_contains(smoke, "\"consumer_hint\":\"Node v225 managed audit sandbox adapter dry-run package\"");
    assert_contains(smoke, "command contract formatter module split");
    assert_contains(smoke, "command response formatter module split");

    assert_contains(smoke, "\"primary\":\"SMOKEJSON\"");
    assert_contains(smoke, "\"supporting\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(smoke, "\"forbidden\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(smoke, "\"SMOKEJSON version matches 0.95.0\"");
    assert_contains(smoke, "\"SMOKEJSON reports write_commands_executed=false and runtime_write_observed=false\"");
    assert_contains(smoke, "SMOKEJSON exposes failure taxonomy categories and taxonomy_digest");
    assert_contains(smoke, "SMOKEJSON exposes operator_window.identity_neutral_proof=true");
    assert_contains(smoke, "SMOKEJSON and INFOJSON expose ci_evidence.artifact_path_hint=c/80/ and no_restore_proof=true");
    assert_contains(smoke, "SMOKEJSON and INFOJSON expose artifact_retention.retention_days=30 and github_artifact_upload_attempted=false");
    assert_contains(smoke, "SMOKEJSON exposes live_read_session.session_id_echo=mini-kv-live-read-v95 and read_command_list_digest=fnv1a64:5bef33f2fbe65cc5");
    assert_contains(smoke, "SMOKEJSON exposes binary_provenance.provenance_digest=fnv1a64:5a5b87dd3e51d634 and production_binary_claimed=false");
    assert_contains(smoke, "SMOKEJSON exposes retention_provenance_check.check_digest=fnv1a64:2156d871db50e2e7 and managed_audit_write_executed=false");
    assert_contains(smoke, "SMOKEJSON exposes retention_provenance_replay_marker.marker_digest=fnv1a64:9ff8ed9e7a8e01a5 and replay_executed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_adapter_restore_boundary_receipt.receipt_digest=fnv1a64:2d50cfbf427d36a6 and adapter_write_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_adapter_non_authoritative_storage_receipt.receipt_digest=fnv1a64:ee75d3848495f666 and managed_audit_store=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.consumed_release_version=v89");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.external_adapter_storage_backend=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.receipt_digest=fnv1a64:3e58b413afa45391");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_external_adapter_non_participation_receipt.credential_read_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.consumed_release_version=v90");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.receipt_digest=fnv1a64:ceaed265f7f9560c");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.sandbox_adapter_storage_backend=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.credential_value_read_allowed=false");
    assert_contains(smoke, "SMOKEJSON exposes managed_audit_sandbox_adapter_non_participation_receipt.sandbox_managed_audit_state_write_allowed=false");

    assert_contains(smoke, "\"server\":\"minikv_server 6429 127.0.0.1\"");
    assert_contains(smoke, "\"client\":\"minikv_client 127.0.0.1 6429 5000 --connect-retries 10 --retry-delay-ms 100\"");
    assert_contains(smoke, "\"commands\":[\"SMOKEJSON\",\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"GET restore:real-read-token\",\"QUIT\"]");

    assert_contains(smoke, "\"target_version\":\"Node v225\"");
    assert_contains(smoke, "\"mode\":\"managed audit sandbox adapter dry-run package\"");
    assert_contains(smoke, "\"default_enabled\":false");
    assert_contains(smoke, "\"mini-kv is already running and user enabled a safe real-read window\"");
    assert_contains(smoke, "\"node_must_verify\":\"managed_audit_sandbox_adapter_non_participation_receipt.consumed_release_version equals v90");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.consumed_artifact_path_hint equals c/90/");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.consumed_receipt_digest equals fnv1a64:0dfb07cd2f8de289");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.current_artifact_path_hint equals c/95/");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.adapter_target equals sandbox managed audit adapter dry-run");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.receipt_digest equals fnv1a64:ceaed265f7f9560c");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.sandbox_adapter_storage_backend is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.credential_value_read_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.sandbox_managed_audit_state_write_allowed is false");
    assert_contains(smoke, "managed_audit_sandbox_adapter_non_participation_receipt.sandbox_dry_run_records_written is false");
    assert_contains(smoke, "managed_audit_external_adapter_non_participation_receipt.receipt_digest equals fnv1a64:3e58b413afa45391");
    assert_contains(smoke, "adapter_shell_non_storage_guard_receipt.receipt_digest equals fnv1a64:f95b3ab13e322be2");
    assert_contains(smoke, "command_dispatch_quality_receipt.receipt_digest equals fnv1a64:bbc791bbf13bbfe2");
    assert_contains(smoke, "binary_provenance.provenance_digest equals fnv1a64:5a5b87dd3e51d634");
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
    assert_contains(smoke, "retention_provenance_check.check_digest equals fnv1a64:2156d871db50e2e7");
    assert_contains(smoke, "retention_provenance_replay_marker.current_artifact_path_hint equals c/95/");
    assert_contains(smoke, "retention_provenance_replay_marker.marker_digest equals fnv1a64:9ff8ed9e7a8e01a5");
    assert_contains(smoke, "managed_audit_adapter_restore_boundary_receipt.current_artifact_path_hint equals c/95/");
    assert_contains(smoke, "managed_audit_adapter_restore_boundary_receipt.receipt_digest equals fnv1a64:2d50cfbf427d36a6");
    assert_contains(smoke, "managed_audit_adapter_non_authoritative_storage_receipt.current_artifact_path_hint equals c/95/");
    assert_contains(smoke, "managed_audit_adapter_non_authoritative_storage_receipt.receipt_digest equals fnv1a64:ee75d3848495f666");
    assert_contains(smoke, "live_read_session.session_id_echo equals mini-kv-live-read-v95");
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
    assert_contains(smoke, "\"session_id_echo\":\"mini-kv-live-read-v95\"");
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
    assert_contains(smoke, "\"source_version\":\"0.95.0\"");
    assert_contains(smoke, "\"artifact_path_hint\":\"c/95/\"");
    assert_contains(smoke, "\"runtime_binary_hint\":\"cmake-build-v95/minikv_server and cmake-build-v95/minikv_client from the current CMake build\"");
    assert_contains(smoke, "\"release_manifest_path\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(smoke, "\"runtime_smoke_evidence_path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(smoke, "\"provenance_digest\":\"fnv1a64:5a5b87dd3e51d634\"");
    assert_contains(smoke, "\"load_restore_compact_executed\":false");
    assert_contains(smoke, "\"production_binary_claimed\":false");
    assert_contains(smoke, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(smoke, "\"expected_binary_provenance_digest\":\"fnv1a64:5a5b87dd3e51d634\"");
    assert_contains(smoke, "\"check_digest\":\"fnv1a64:2156d871db50e2e7\"");
    assert_contains(smoke, "\"managed_audit_write_executed\":false");
    assert_contains(smoke, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v84\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_contains(smoke, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(smoke, "\"marker_digest\":\"fnv1a64:9ff8ed9e7a8e01a5\"");
    assert_contains(smoke, "\"replay_executed\":false");
    assert_contains(smoke, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(smoke, "\"consumed_by\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v85\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_contains(smoke, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:2d50cfbf427d36a6\"");
    assert_contains(smoke, "\"adapter_write_allowed\":false");
    assert_contains(smoke, "\"restore_execution_allowed\":false");
    assert_contains(smoke, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:ee75d3848495f666\"");
    assert_contains(smoke, "\"managed_audit_store\":false");
    assert_contains(smoke, "\"storage_write_allowed\":false");
    assert_contains(smoke, "\"admin_commands_allowed\":false");
    assert_contains(smoke, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_contains(smoke, "\"consumed_release_version\":\"v87\"");
    assert_contains(smoke, "\"consumed_artifact_path_hint\":\"c/87/\"");
    assert_contains(smoke, "\"consumed_receipt_digest\":\"fnv1a64:111f0daf1283eab6\"");
    assert_contains(smoke, "\"dispatch_family\":\"runtime_evidence_command_family\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:bbc791bbf13bbfe2\"");
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
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:f95b3ab13e322be2\"");
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
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:3e58b413afa45391\"");
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
    assert_contains(smoke, "\"current_artifact_path_hint\":\"c/95/\"");
    assert_contains(smoke, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_contains(smoke, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(smoke, "\"receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(smoke, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(smoke, "\"participates_in_sandbox_adapter\":false");
    assert_contains(smoke, "\"credential_value_required\":false");
    assert_contains(smoke, "\"credential_value_read_allowed\":false");
    assert_contains(smoke, "\"production_credential_read_allowed\":false");
    assert_contains(smoke, "\"schema_migration_execution_allowed\":false");
    assert_contains(smoke, "\"sandbox_schema_authoritative\":false");
    assert_contains(smoke, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(smoke, "\"sandbox_dry_run_records_written\":false");

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
    assert_contains(smoke, "\"needs mini-kv to become sandbox audit storage backend\"");
    assert_contains(smoke, "\"needs mini-kv to read sandbox credential value\"");
    assert_contains(smoke, "\"needs mini-kv to write sandbox managed audit state\"");
    assert_contains(smoke, "\"needs mini-kv to execute sandbox adapter schema migration\"");
    assert_contains(smoke, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(smoke, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(smoke, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(smoke, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(smoke, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.95.0\"");
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
    assert_contains(result.response, "\"version\":\"0.95.0\"");
    assert_contains(result.response, "\"server\":{\"protocol\":[\"inline\"]");
    assert_contains(result.response, "\"uptime_seconds\":0");
    assert_contains(result.response, "\"store\":{\"live_keys\":0,\"order_authoritative\":false}");
    assert_contains(result.response, "\"wal\":{\"enabled\":false,\"status\":\"disabled\",\"compact_recommended\":false}");
    assert_contains(result.response, "\"connection_stats\":{\"available\":false}");
    assert_contains(result.response, "\"real_read\":{\"allowed\":true,\"commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(result.response, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(result.response, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(result.response, "\"session_id_echo\":\"mini-kv-live-read-v95\"");
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
    assert_contains(result.response, "\"source_version\":\"0.95.0\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/95/\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:5a5b87dd3e51d634\"");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v86\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:f39d8e3ef98654ea\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:ee75d3848495f666\"");
    assert_contains(result.response, "\"managed_audit_store\":false");
    assert_contains(result.response, "\"storage_write_allowed\":false");
    assert_contains(result.response, "\"admin_commands_allowed\":false");
    assert_contains(result.response, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:bbc791bbf13bbfe2\"");
    assert_contains(result.response, "\"dispatch_split_applied\":true");
    assert_contains(result.response, "\"wal_snapshot_restore_touched\":false");
    assert_contains(result.response, "\"fixture_contract_preserved\":true");
    assert_contains(result.response, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v220 managed audit adapter interface and disabled shell\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v88\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:4aa6d12fb067e2a6\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:f95b3ab13e322be2\"");
    assert_contains(result.response, "\"adapter_shell_storage_backend\":false");
    assert_contains(result.response, "\"local_dry_run_records_written\":false");
    assert_contains(result.response, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_contains(result.response, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v89\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_contains(result.response, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_contains(result.response, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:3e58b413afa45391\"");
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
    assert_contains(result.response, "\"current_artifact_path_hint\":\"c/95/\"");
    assert_contains(result.response, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_contains(result.response, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_contains(result.response, "\"sandbox_adapter_storage_backend\":false");
    assert_contains(result.response, "\"participates_in_sandbox_adapter\":false");
    assert_contains(result.response, "\"credential_value_required\":false");
    assert_contains(result.response, "\"credential_value_read_allowed\":false");
    assert_contains(result.response, "\"production_credential_read_allowed\":false");
    assert_contains(result.response, "\"schema_migration_execution_allowed\":false");
    assert_contains(result.response, "\"sandbox_schema_authoritative\":false");
    assert_contains(result.response, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(result.response, "\"sandbox_dry_run_records_written\":false");
    assert_contains(result.response, "\"production_binary_claimed\":false");
    assert_contains(result.response, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_contains(result.response, "\"expected_binary_provenance_digest\":\"fnv1a64:5a5b87dd3e51d634\"");
    assert_contains(result.response, "\"check_digest\":\"fnv1a64:2156d871db50e2e7\"");
    assert_contains(result.response, "\"managed_audit_write_executed\":false");
    assert_contains(result.response, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v84\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_contains(result.response, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_contains(result.response, "\"marker_digest\":\"fnv1a64:9ff8ed9e7a8e01a5\"");
    assert_contains(result.response, "\"replay_executed\":false");
    assert_contains(result.response, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_contains(result.response, "\"consumed_release_version\":\"v85\"");
    assert_contains(result.response, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_contains(result.response, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_contains(result.response, "\"receipt_digest\":\"fnv1a64:2d50cfbf427d36a6\"");
    assert_contains(result.response, "\"adapter_write_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(result.response, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(result.response, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(result.response, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.95.0\"");
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
    assert_contains(result.response, "\"node_consumption\":\"Node v225 may verify the mini-kv sandbox adapter non-participation receipt, the v90 external adapter non-participation receipt, the v89 adapter shell non-storage guard receipt, the v88 command dispatch quality receipt, the v87 managed audit adapter non-authoritative storage receipt, the v86 managed audit adapter restore boundary receipt, runtime evidence retention, binary provenance digest alignment, live-read session echo, uptime bucket, read command digest, taxonomy digest, operator-window identity-neutral proof, CI evidence hints, and artifact retention evidence before managed audit sandbox adapter dry-run package; mini-kv must already be running and the read-only window must be open\"");
    assert_contains(result.response, "\"notes\":[\"runtime_smoke_evidence\",\"live_read_session_hint\",\"binary_provenance_hint\",\"retention_provenance_check\",\"retention_provenance_replay_marker\",\"managed_audit_adapter_restore_boundary_receipt\",\"managed_audit_adapter_non_authoritative_storage_receipt\",\"command_dispatch_quality_receipt\",\"adapter_shell_non_storage_guard_receipt\",\"managed_audit_external_adapter_non_participation_receipt\",\"managed_audit_sandbox_adapter_non_participation_receipt\",\"read_only_aggregate\",\"not_order_authoritative\","
                                     "\"does_not_execute_load_compact_setnxex_or_restore\"]");

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
