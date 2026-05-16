#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
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
    assert(text.find(std::string{expected}) != std::string::npos);
}

void assert_path_exists(const std::filesystem::path& relative_path) {
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path));
}

} // namespace

int main() {
    const auto retention_path = std::filesystem::path{"fixtures"} / "release" / "restore-evidence-retention.json";
    const auto retention = read_fixture_text(retention_path);

    assert_contains(retention, "\"retention_version\":\"mini-kv-restore-evidence-retention.v1\"");
    assert_contains(retention, "\"project\":\"mini-kv\"");
    assert_contains(retention, "\"project_version\":\"0.72.0\"");
    assert_contains(retention, "\"release_version\":\"v72\"");
    assert_contains(retention, "\"path\":\"fixtures/release/restore-evidence-retention.json\"");
    assert_contains(retention, "\"read_only\":true");
    assert_contains(retention, "\"execution_allowed\":false");
    assert_contains(retention, "\"restore_execution_allowed\":false");
    assert_contains(retention, "\"order_authoritative\":false");
    assert_contains(retention, "\"consumer_hint\":\"Node v178 cross-project evidence retention gate\"");
    assert_contains(retention, "\"scope\":\"restore evidence retention fixture for retaining restore checklist, artifact digest, WAL/Snapshot review, and CHECKJSON risk evidence\"");

    assert_contains(retention, "\"target_release_version\":\"v72\"");
    assert_contains(retention, "\"source_checklist\":\"fixtures/release/restore-handoff-checklist.json\"");
    assert_contains(retention, "\"retention_id\":\"mini-kv-restore-retention-v72\"");
    assert_contains(retention, "\"artifact_digest_placeholder\":\"sha256:<operator-retained-restore-artifact-digest>\"");
    assert_contains(retention, "\"snapshot_review_retention\":\"sha256:<operator-retained-snapshot-review-digest>\"");
    assert_contains(retention, "\"wal_review_retention\":\"sha256:<operator-retained-wal-review-digest>\"");
    assert_contains(retention, "\"retention_days\":90");
    assert_contains(retention, "\"operator_identity_placeholder\":\"operator:<retained-restore-reviewer>\"");
    assert_contains(retention, "\"boundary\":\"restore evidence retention records review evidence only and does not execute restore\"");

    assert_contains(retention, "\"restore_checklist_retention_id\"");
    assert_contains(retention, "\"artifact_digest_retained_at\"");
    assert_contains(retention, "\"snapshot_review_retained_at\"");
    assert_contains(retention, "\"wal_review_retained_at\"");
    assert_contains(retention, "\"checkjson_risk_evidence_retained_at\"");
    assert_contains(retention, "\"retention_owner\"");
    assert_contains(retention, "\"retention_gate_ready_for_node_review\"");
    assert_contains(retention, "\"retention owner recorded without production secret value\"");
    assert_contains(retention, "\"storage_policy\":\"fixture records retention field names and placeholders only; retained artifacts live outside mini-kv\"");

    assert_contains(retention, "\"CHECKJSON LOAD data/retention-restore.snap\"");
    assert_contains(retention, "\"CHECKJSON COMPACT\"");
    assert_contains(retention, "\"CHECKJSON SETNXEX restore:retention-token 30 value\"");
    assert_contains(retention, "\"GET restore:retention-token\"");
    assert_contains(retention, "\"expected\":[\"INFOJSON version matches 0.72.0\"");
    assert_contains(retention, "\"write_commands_executed\":false");
    assert_contains(retention, "\"admin_commands_executed\":false");
    assert_contains(retention, "\"retention_gate_input\":true");
    assert_contains(retention, "\"retention metadata only\"");
    assert_contains(retention, "\"snapshot/WAL review retention placeholder only\"");
    assert_contains(retention, "\"CHECKJSON risk evidence retention only\"");
    assert_contains(retention, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(retention, "\"mini-kv remains not Java order authority\"");
    assert_contains(retention, "\"Node v178 may consume this fixture only after Java v63, mini-kv v72, and Node v177 are complete\"");
    assert_contains(retention, "\"retention fixture is not a restore executor\"");
    assert_contains(retention, "\"retention owner or retained artifact digest is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        retention_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-handoff-checklist.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-drill-evidence.json",
        std::filesystem::path{"fixtures"} / "release" / "release-artifact-digest-package.json",
        std::filesystem::path{"fixtures"} / "release" / "artifact-digest-compatibility-matrix.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-dry-run-operator-package.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-bundle-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-compatibility-handoff.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "recovery-evidence.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(retention, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.90.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/retention-restore.snap");
    assert_contains(result.response, "\"command\":\"LOAD\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"write_command\":true");
    assert_contains(result.response, "\"side_effects\":[\"store_replace_from_snapshot\"]");

    result = processor.execute("CHECKJSON COMPACT");
    assert_contains(result.response, "\"command\":\"COMPACT\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"side_effects\":[\"wal_rewrite_when_enabled\"]");

    result = processor.execute("CHECKJSON SETNXEX restore:retention-token 30 value");
    assert_contains(result.response, "\"command\":\"SETNXEX\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"write_command\":true");
    assert_contains(result.response, "\"side_effects\":[\"store_write\",\"store_ttl_update\","
                                     "\"wal_append_when_enabled\"]");

    result = processor.execute("STORAGEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"load_replaces_store\":true");

    result = processor.execute("GET restore:retention-token");
    assert(result.response == "(nil)");

    return 0;
}
