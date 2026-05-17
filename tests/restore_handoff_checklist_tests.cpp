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
    const auto checklist_path = std::filesystem::path{"fixtures"} / "release" / "restore-handoff-checklist.json";
    const auto checklist = read_fixture_text(checklist_path);

    assert_contains(checklist, "\"checklist_version\":\"mini-kv-restore-handoff-checklist.v1\"");
    assert_contains(checklist, "\"project\":\"mini-kv\"");
    assert_contains(checklist, "\"project_version\":\"0.71.0\"");
    assert_contains(checklist, "\"release_version\":\"v71\"");
    assert_contains(checklist, "\"path\":\"fixtures/release/restore-handoff-checklist.json\"");
    assert_contains(checklist, "\"read_only\":true");
    assert_contains(checklist, "\"execution_allowed\":false");
    assert_contains(checklist, "\"restore_execution_allowed\":false");
    assert_contains(checklist, "\"order_authoritative\":false");
    assert_contains(checklist, "\"consumer_hint\":\"Node v175 release handoff readiness review\"");
    assert_contains(checklist, "\"scope\":\"restore handoff checklist fixture for manual pre-restore review and CHECKJSON risk confirmation\"");

    assert_contains(checklist, "\"target_release_version\":\"v71\"");
    assert_contains(checklist, "\"current_release_version\":\"v71\"");
    assert_contains(checklist, "\"artifact_digest_target\":\"sha256:<operator-recorded-restore-artifact-digest>\"");
    assert_contains(checklist, "\"snapshot_review_placeholder\":\"sha256:<operator-recorded-snapshot-review-digest>\"");
    assert_contains(checklist, "\"wal_review_placeholder\":\"sha256:<operator-recorded-wal-review-digest>\"");
    assert_contains(checklist, "\"boundary\":\"restore handoff checklist is manual review evidence only and does not execute restore\"");

    assert_contains(checklist, "\"restore_operator_id\"");
    assert_contains(checklist, "\"artifact_digest_target_reviewed_at\"");
    assert_contains(checklist, "\"snapshot_review_recorded_at\"");
    assert_contains(checklist, "\"wal_review_recorded_at\"");
    assert_contains(checklist, "\"checkjson_risk_confirmed\"");
    assert_contains(checklist, "\"handoff_ready_for_node_review\"");
    assert_contains(checklist, "\"CHECKJSON LOAD/COMPACT/SETNXEX risk profile reviewed\"");
    assert_contains(checklist, "\"placeholder_policy\":\"operator records review values outside mini-kv; fixture keeps placeholders only\"");

    assert_contains(checklist, "\"CHECKJSON LOAD data/handoff-restore.snap\"");
    assert_contains(checklist, "\"CHECKJSON COMPACT\"");
    assert_contains(checklist, "\"CHECKJSON SETNXEX restore:handoff-token 30 value\"");
    assert_contains(checklist, "\"GET restore:handoff-token\"");
    assert_contains(checklist, "\"expected\":[\"INFOJSON version matches 0.71.0\"");
    assert_contains(checklist, "\"write_commands_executed\":false");
    assert_contains(checklist, "\"admin_commands_executed\":false");
    assert_contains(checklist, "\"manual checklist only\"");
    assert_contains(checklist, "\"artifact digest target placeholder only\"");
    assert_contains(checklist, "\"snapshot/WAL review placeholder only\"");
    assert_contains(checklist, "\"CHECKJSON risk confirmation only\"");
    assert_contains(checklist, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(checklist, "\"mini-kv remains not Java order authority\"");
    assert_contains(checklist, "\"Node v175 may consume this fixture only after Java v62 and mini-kv v71 are complete\"");
    assert_contains(checklist, "\"handoff checklist is not a restore executor\"");
    assert_contains(checklist, "\"restore operator or artifact digest target is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        checklist_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
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
        assert_contains(checklist, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.94.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/handoff-restore.snap");
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

    result = processor.execute("CHECKJSON SETNXEX restore:handoff-token 30 value");
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

    result = processor.execute("GET restore:handoff-token");
    assert(result.response == "(nil)");

    return 0;
}
