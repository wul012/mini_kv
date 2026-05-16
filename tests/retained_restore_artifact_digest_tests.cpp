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
    const auto digest_path = std::filesystem::path{"fixtures"} / "release" /
                             "retained-restore-artifact-digest.json";
    const auto digest = read_fixture_text(digest_path);

    assert_contains(digest, "\"digest_version\":\"mini-kv-retained-restore-artifact-digest.v1\"");
    assert_contains(digest, "\"project\":\"mini-kv\"");
    assert_contains(digest, "\"project_version\":\"0.73.0\"");
    assert_contains(digest, "\"release_version\":\"v73\"");
    assert_contains(digest, "\"path\":\"fixtures/release/retained-restore-artifact-digest.json\"");
    assert_contains(digest, "\"read_only\":true");
    assert_contains(digest, "\"execution_allowed\":false");
    assert_contains(digest, "\"restore_execution_allowed\":false");
    assert_contains(digest, "\"order_authoritative\":false");
    assert_contains(digest, "\"consumer_hint\":\"Node v180 approval decision prerequisite gate\"");
    assert_contains(digest, "\"scope\":\"retained restore artifact digest fixture");

    assert_contains(digest, "\"target_release_version\":\"v73\"");
    assert_contains(digest, "\"source_retention_fixture\":\"fixtures/release/restore-evidence-retention.json\"");
    assert_contains(digest, "\"source_verification_manifest\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(digest, "\"retention_id\":\"mini-kv-retained-restore-artifact-digest-v73\"");
    assert_contains(digest, "\"restore_target_placeholder\":\"restore-target:<operator-recorded-restore-target>\"");
    assert_contains(digest, "\"restore_artifact_digest_placeholder\":\"sha256:<operator-retained-restore-artifact-digest>\"");
    assert_contains(digest, "\"snapshot_review_digest_placeholder\":\"sha256:<operator-retained-snapshot-review-digest>\"");
    assert_contains(digest, "\"wal_review_digest_placeholder\":\"sha256:<operator-retained-wal-review-digest>\"");
    assert_contains(digest, "\"retention_owner\":\"operator:<retained-restore-artifact-owner>\"");
    assert_contains(digest, "\"boundary\":\"retained digest fixture records approval input evidence only and does not execute restore\"");

    assert_contains(digest, "\"restore_artifact_digest_retained_at\"");
    assert_contains(digest, "\"snapshot_review_digest_retained_at\"");
    assert_contains(digest, "\"wal_review_digest_retained_at\"");
    assert_contains(digest, "\"approval_prerequisite_ready_for_node_review\"");
    assert_contains(digest, "\"restore artifact digest retained outside mini-kv\"");
    assert_contains(digest, "\"Snapshot review digest retained outside mini-kv\"");
    assert_contains(digest, "\"WAL review digest retained outside mini-kv\"");
    assert_contains(digest, "\"mini-kv order_authoritative remains false\"");
    assert_contains(digest, "artifact bytes and operator records live outside mini-kv");

    assert_contains(digest, "\"CHECKJSON LOAD data/retained-digest-restore.snap\"");
    assert_contains(digest, "\"CHECKJSON COMPACT\"");
    assert_contains(digest, "\"CHECKJSON SETNXEX restore:digest-token 30 value\"");
    assert_contains(digest, "\"GET restore:digest-token\"");
    assert_contains(digest, "\"expected\":[\"INFOJSON version matches 0.73.0\"");
    assert_contains(digest, "\"write_commands_executed\":false");
    assert_contains(digest, "\"admin_commands_executed\":false");
    assert_contains(digest, "\"approval_prerequisite_input\":true");
    assert_contains(digest, "\"retained restore artifact digest fixture only\"");
    assert_contains(digest, "\"restore target placeholder only\"");
    assert_contains(digest, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(digest, "\"mini-kv remains not Java order authority\"");
    assert_contains(digest, "\"Node v180 may consume this fixture only after Java v64, mini-kv v73, and Node v179 are complete\"");
    assert_contains(digest, "\"retained digest fixture is not an approval decision and not a restore executor\"");
    assert_contains(digest, "\"restore target or retained digest owner is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        digest_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-evidence-retention.json",
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
        assert_contains(digest, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.82.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/retained-digest-restore.snap");
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

    result = processor.execute("CHECKJSON SETNXEX restore:digest-token 30 value");
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

    result = processor.execute("GET restore:digest-token");
    assert(result.response == "(nil)");

    return 0;
}
