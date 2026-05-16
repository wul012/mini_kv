#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::string read_file_text(const std::filesystem::path& path) {
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

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    return read_file_text(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path);
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
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const auto manifest = read_fixture_text(manifest_path);

    assert_contains(manifest, "\"manifest_version\":\"mini-kv-release-verification-manifest.v1\"");
    assert_contains(manifest, "\"project\":\"mini-kv\"");
    assert_contains(manifest, "\"project_version\":\"0.83.0\"");
    assert_contains(manifest, "\"release_version\":\"v83\"");
    assert_contains(manifest, "\"read_only\":true");
    assert_contains(manifest, "\"execution_allowed\":false");
    assert_contains(manifest, "\"order_authoritative\":false");
    assert_contains(manifest, "\"no_runtime_write_command_added\":true");
    assert_contains(manifest, "\"consumer_hint\":\"Node v208 managed audit persistence boundary candidate\"");

    assert_contains(manifest, "\"command\":\"cmake -S . -B cmake-build-v83");
    assert_contains(manifest, "\"command\":\"cmake --build cmake-build-v83 --parallel 2\"");
    assert_contains(manifest, "\"command\":\"ctest --test-dir cmake-build-v83 --output-on-failure\"");
    assert_contains(manifest, "\"minikv_command_tests\"");
    assert_contains(manifest, "\"minikv_readonly_fixture_tests\"");
    assert_contains(manifest, "\"minikv_recovery_fixture_index_tests\"");
    assert_contains(manifest, "\"minikv_ttl_token_recovery_tests\"");
    assert_contains(manifest, "\"minikv_release_verification_manifest_tests\"");
    assert_contains(manifest, "\"minikv_runtime_artifact_rollback_evidence_tests\"");
    assert_contains(manifest, "\"minikv_runtime_artifact_bundle_manifest_tests\"");
    assert_contains(manifest, "\"minikv_restore_compatibility_handoff_tests\"");
    assert_contains(manifest, "\"minikv_restore_dry_run_operator_package_tests\"");
    assert_contains(manifest, "\"minikv_artifact_digest_compatibility_matrix_tests\"");
    assert_contains(manifest, "\"minikv_release_artifact_digest_package_tests\"");
    assert_contains(manifest, "\"minikv_restore_drill_evidence_tests\"");
    assert_contains(manifest, "\"minikv_restore_handoff_checklist_tests\"");
    assert_contains(manifest, "\"minikv_restore_evidence_retention_tests\"");
    assert_contains(manifest, "\"minikv_retained_restore_artifact_digest_tests\"");
    assert_contains(manifest, "\"minikv_restore_approval_boundary_tests\"");
    assert_contains(manifest, "\"minikv_restore_boundary_smoke_manifest_tests\"");
    assert_contains(manifest, "\"minikv_runtime_smoke_evidence_tests\"");

    assert_contains(manifest, "\"SMOKEJSON\"");
    assert_contains(manifest, "\"INFOJSON\"");
    assert_contains(manifest, "\"STORAGEJSON\"");
    assert_contains(manifest, "\"HEALTH\"");
    assert_contains(manifest, "\"GET restore:real-read-token\"");
    assert_contains(manifest, "\"SMOKEJSON version matches 0.83.0\"");
    assert_contains(manifest, "\"SMOKEJSON returns runtime_smoke evidence\"");
    assert_contains(manifest, "\"SMOKEJSON exposes live_read_session.session_id_echo=mini-kv-live-read-v83\"");
    assert_contains(manifest, "\"SMOKEJSON exposes live_read_session.read_command_list_digest=fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(manifest, "\"SMOKEJSON exposes binary_provenance.provenance_digest=fnv1a64:c1c0896fc6b77fe2\"");
    assert_contains(manifest, "\"SMOKEJSON exposes binary_provenance.production_binary_claimed=false\"");
    assert_contains(manifest, "\"INFOJSON exposes ci_evidence.artifact_path_hint=c/80/\"");
    assert_contains(manifest, "\"INFOJSON exposes artifact_retention.artifact_path_hint=c/81/\"");
    assert_contains(manifest, "\"SMOKEJSON exposes ci_evidence.no_restore_proof=true and upload_allowed=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes artifact_retention.retention_days=30 and github_artifact_upload_attempted=false\"");
    assert_contains(manifest, "\"SMOKEJSON exposes taxonomy_digest fnv1a64:f92fcba55feb26a2");
    assert_contains(manifest, "\"SMOKEJSON exposes operator_window.identity_neutral_proof=true for Node v200 archive handoff\"");
    assert_contains(manifest, "\"write_commands_executed\":false");
    assert_contains(manifest, "\"admin_commands_executed\":false");
    assert_contains(manifest, "\"runtime_write_observed\":false");
    assert_contains(manifest, "\"ci_artifact_upload_executed\":false");
    assert_contains(manifest, "\"production_window_opened\":false");

    const std::vector<std::filesystem::path> required_paths = {
        manifest_path,
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-rollback-evidence.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-bundle-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-compatibility-handoff.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-dry-run-operator-package.json",
        std::filesystem::path{"fixtures"} / "release" / "artifact-digest-compatibility-matrix.json",
        std::filesystem::path{"fixtures"} / "release" / "release-artifact-digest-package.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-drill-evidence.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-handoff-checklist.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-evidence-retention.json",
        std::filesystem::path{"fixtures"} / "release" / "retained-restore-artifact-digest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-approval-boundary.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-boundary-smoke-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json",
        std::filesystem::path{"fixtures"} / "readonly" / "index.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "readonly" / "statsjson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "readonly" / "runtime-read-field-guide.json",
        std::filesystem::path{"fixtures"} / "checkjson" / "set-orderops-write-contract.json",
        std::filesystem::path{"fixtures"} / "checkjson" / "get-orderops-read-contract.json",
        std::filesystem::path{"fixtures"} / "recovery" / "index.json",
        std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "index.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "recovery-evidence.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(manifest, "\"path\":\"" + path.generic_string() + "\"");
    }

    assert_contains(manifest, "\"cmake_project_version\":\"0.83.0\"");
    assert_contains(manifest, "\"generated_header\":\"include/minikv/version.hpp.in\"");
    assert_contains(manifest, "\"fixtures/readonly/infojson-empty-inline.json\"");
    assert_contains(manifest, "\"fixtures/release/verification-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-artifact-rollback-evidence.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-artifact-bundle-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-compatibility-handoff.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-dry-run-operator-package.json\"");
    assert_contains(manifest, "\"fixtures/release/artifact-digest-compatibility-matrix.json\"");
    assert_contains(manifest, "\"fixtures/release/release-artifact-digest-package.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-drill-evidence.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-handoff-checklist.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-evidence-retention.json\"");
    assert_contains(manifest, "\"fixtures/release/retained-restore-artifact-digest.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-approval-boundary.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-boundary-smoke-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(manifest, "\"manifest only\"");
    assert_contains(manifest, "\"bundle manifest only\"");
    assert_contains(manifest, "\"restore compatibility handoff sample only\"");
    assert_contains(manifest, "\"restore dry-run operator package only\"");
    assert_contains(manifest, "\"artifact digest compatibility matrix only\"");
    assert_contains(manifest, "\"release artifact digest package only\"");
    assert_contains(manifest, "\"restore drill evidence only\"");
    assert_contains(manifest, "\"restore handoff checklist only\"");
    assert_contains(manifest, "\"restore evidence retention only\"");
    assert_contains(manifest, "\"retained restore artifact digest only\"");
    assert_contains(manifest, "\"restore approval boundary only\"");
    assert_contains(manifest, "\"restore boundary smoke manifest only\"");
    assert_contains(manifest, "\"runtime smoke evidence only\"");
    assert_contains(manifest, "\"live-read session hint only\"");
    assert_contains(manifest, "\"binary provenance hint only\"");
    assert_contains(manifest, "\"CI evidence hint only\"");
    assert_contains(manifest, "\"artifact path hint only\"");
    assert_contains(manifest, "\"artifact retention evidence only\"");
    assert_contains(manifest, "\"retention dry-run only\"");
    assert_contains(manifest, "\"no production window opening\"");
    assert_contains(manifest, "\"taxonomy digest sample only\"");
    assert_contains(manifest, "\"operator-window proof only\"");
    assert_contains(manifest, "\"identity-neutral proof only\"");
    assert_contains(manifest, "\"no-restore proof only\"");
    assert_contains(manifest, "\"no CI artifact upload\"");
    assert_contains(manifest, "\"not connected to Java transaction chain\"");
    assert_contains(manifest, "\"does not perform restore\"");
    assert_contains(manifest, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(manifest, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(manifest, "\"no_restore_proof\":true");
    assert_contains(manifest, "\"upload_allowed\":false");
    assert_contains(manifest, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(manifest, "\"artifact_root\":\"c/\"");
    assert_contains(manifest, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(manifest, "\"retention_days\":30");
    assert_contains(manifest, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_contains(manifest, "\"release_evidence_ready\":true");
    assert_contains(manifest, "\"github_artifact_upload_attempted\":false");
    assert_contains(manifest, "\"production_window_allowed\":false");
    assert_contains(manifest, "\"node_action\":\"verify retention days and artifact path before cross-project retention gate\"");
    assert_contains(manifest, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(manifest, "\"session_id_echo\":\"mini-kv-live-read-v83\"");
    assert_contains(manifest, "\"server_uptime_bucket\":\"lt_60s\"");
    assert_contains(manifest, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(manifest, "\"read_command_count\":4");
    assert_contains(manifest, "\"read_commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(manifest, "\"write_commands_allowed\":false");
    assert_contains(manifest, "\"auto_start_allowed\":false");
    assert_contains(manifest, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(manifest, "\"source_version\":\"0.83.0\"");
    assert_contains(manifest, "\"artifact_path_hint\":\"c/83/\"");
    assert_contains(manifest, "\"runtime_binary_hint\":\"cmake-build-v83/minikv_server and cmake-build-v83/minikv_client from the current CMake build\"");
    assert_contains(manifest, "\"release_manifest_path\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(manifest, "\"runtime_smoke_evidence_path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(manifest, "\"provenance_digest\":\"fnv1a64:c1c0896fc6b77fe2\"");
    assert_contains(manifest, "\"load_restore_compact_executed\":false");
    assert_contains(manifest, "\"production_binary_claimed\":false");
    assert_contains(manifest, "\"verify binary, fixture, and release evidence path alignment before managed audit persistence boundary work\"");
    assert_contains(manifest, "\"Node v208 may verify SMOKEJSON and INFOJSON binary provenance hints before managed audit persistence boundary work\"");
    assert_contains(manifest, "\"Binary provenance hint is read-only path alignment evidence, not production binary attestation\"");
    assert_contains(manifest, "\"Node v205 may verify SMOKEJSON live-read session hints before the real-read execution packet\"");
    assert_contains(manifest, "\"Live-read session hint is read-only evidence, not permission to auto-start mini-kv\"");
    assert_contains(manifest, "\"Node v203 may verify SMOKEJSON and INFOJSON artifact retention evidence before the cross-project retention gate\"");
    assert_contains(manifest, "\"Artifact retention evidence is read-only rehearsal metadata, not a real GitHub artifact retention configuration\"");
    assert_contains(manifest, "\"Node v201 may still verify SMOKEJSON and INFOJSON CI evidence hints before checking the CI artifact manifest\"");
    assert_contains(manifest, "\"CI evidence hint is read-only path evidence, not a real GitHub artifact upload\"");
    assert_contains(manifest, "\"SMOKEJSON operator-window proof is identity-neutral evidence, not authentication or production authorization\"");

    const auto cmake_lists = read_file_text(std::filesystem::path{MINIKV_SOURCE_DIR} / "CMakeLists.txt");
    assert_contains(cmake_lists, "project(mini_kv VERSION 0.83.0");
    assert_contains(cmake_lists, "minikv_release_verification_manifest_tests");
    assert_contains(cmake_lists, "minikv_runtime_artifact_rollback_evidence_tests");
    assert_contains(cmake_lists, "minikv_runtime_artifact_bundle_manifest_tests");
    assert_contains(cmake_lists, "minikv_restore_compatibility_handoff_tests");
    assert_contains(cmake_lists, "minikv_restore_dry_run_operator_package_tests");
    assert_contains(cmake_lists, "minikv_artifact_digest_compatibility_matrix_tests");
    assert_contains(cmake_lists, "minikv_release_artifact_digest_package_tests");
    assert_contains(cmake_lists, "minikv_restore_drill_evidence_tests");
    assert_contains(cmake_lists, "minikv_restore_handoff_checklist_tests");
    assert_contains(cmake_lists, "minikv_restore_evidence_retention_tests");
    assert_contains(cmake_lists, "minikv_retained_restore_artifact_digest_tests");
    assert_contains(cmake_lists, "minikv_restore_approval_boundary_tests");
    assert_contains(cmake_lists, "minikv_restore_boundary_smoke_manifest_tests");
    assert_contains(cmake_lists, "minikv_runtime_smoke_evidence_tests");

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.83.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(result.response, "\"no_restore_proof\":true");
    assert_contains(result.response, "\"upload_allowed\":false");
    assert_contains(result.response, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(result.response, "\"artifact_root\":\"c/\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(result.response, "\"retention_days\":30");
    assert_contains(result.response, "\"github_artifact_upload_attempted\":false");
    assert_contains(result.response, "\"production_window_allowed\":false");
    assert_contains(result.response, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(result.response, "\"source_version\":\"0.83.0\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/83/\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:c1c0896fc6b77fe2\"");
    assert_contains(result.response, "\"load_restore_compact_executed\":false");
    assert_contains(result.response, "\"production_binary_claimed\":false");

    result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_smoke\"");
    assert_contains(result.response, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(result.response, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_contains(result.response, "\"session_id_echo\":\"mini-kv-live-read-v83\"");
    assert_contains(result.response, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_contains(result.response, "\"write_commands_allowed\":false");
    assert_contains(result.response, "\"auto_start_allowed\":false");
    assert_contains(result.response, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(result.response, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(result.response, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(result.response, "\"operator_window\":{\"consumer\":\"Node v200 real-read window CI archive artifact manifest\"");
    assert_contains(result.response, "\"identity_neutral_proof\":true");
    assert_contains(result.response, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(result.response, "\"no_restore_proof\":true");
    assert_contains(result.response, "\"upload_allowed\":false");
    assert_contains(result.response, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(result.response, "\"artifact_root\":\"c/\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(result.response, "\"retention_days\":30");
    assert_contains(result.response, "\"github_artifact_upload_attempted\":false");
    assert_contains(result.response, "\"production_window_allowed\":false");
    assert_contains(result.response, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_contains(result.response, "\"provenance_digest\":\"fnv1a64:c1c0896fc6b77fe2\"");
    assert_contains(result.response, "\"production_binary_claimed\":false");
    assert_contains(result.response, "\"node_consumption\":\"Node v208 may verify binary provenance, live-read session echo, uptime bucket, read command digest, taxonomy digest, operator-window identity-neutral proof, CI evidence hints, and artifact retention evidence before managed audit persistence boundary work; mini-kv must already be running and the read-only window must be open\"");
    assert_contains(result.response, "\"write_commands_executed\":false");

    result = processor.execute("STORAGEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    return 0;
}
