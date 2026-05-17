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
    const auto drill_path = std::filesystem::path{"fixtures"} / "release" / "restore-drill-evidence.json";
    const auto drill = read_fixture_text(drill_path);

    assert_contains(drill, "\"drill_version\":\"mini-kv-restore-drill-evidence.v1\"");
    assert_contains(drill, "\"project\":\"mini-kv\"");
    assert_contains(drill, "\"project_version\":\"0.70.0\"");
    assert_contains(drill, "\"release_version\":\"v70\"");
    assert_contains(drill, "\"read_only\":true");
    assert_contains(drill, "\"execution_allowed\":false");
    assert_contains(drill, "\"restore_execution_allowed\":false");
    assert_contains(drill, "\"order_authoritative\":false");
    assert_contains(drill, "\"consumer_hint\":\"Node v173 release window readiness packet\"");
    assert_contains(drill, "\"scope\":\"restore drill evidence fixture for target release, CHECKJSON risk explanation, and digest comparison placeholder\"");

    assert_contains(drill, "\"target_release_version\":\"v70\"");
    assert_contains(drill, "\"current_release_version\":\"v70\"");
    assert_contains(drill, "\"digest_comparison_placeholder\":\"sha256:<operator-recorded-restore-drill-digest>\"");
    assert_contains(drill, "\"comparison_basis\":[\"release artifact digest package v69\",\"artifact digest compatibility matrix v68\"]");
    assert_contains(drill, "\"operator_confirmation_required\":true");
    assert_contains(drill, "\"boundary\":\"restore drill target review evidence only and does not switch traffic or execute restore\"");

    assert_contains(drill, "\"CHECKJSON LOAD data/restore-drill.snap\"");
    assert_contains(drill, "\"CHECKJSON COMPACT\"");
    assert_contains(drill, "\"CHECKJSON SETNXEX restore:drill-token 30 value\"");
    assert_contains(drill, "\"GET restore:drill-token\"");
    assert_contains(drill, "\"expected\":[\"INFOJSON version matches 0.70.0\"");
    assert_contains(drill, "\"write_commands_executed\":false");
    assert_contains(drill, "\"admin_commands_executed\":false");
    assert_contains(drill, "\"restore drill evidence is not a restore executor\"");
    assert_contains(drill, "\"digest comparison placeholder must be verified outside mini-kv before release window review\"");
    assert_contains(drill, "\"restore target or digest comparison is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        drill_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
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
        assert_contains(drill, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.92.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/restore-drill.snap");
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

    result = processor.execute("CHECKJSON SETNXEX restore:drill-token 30 value");
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

    result = processor.execute("GET restore:drill-token");
    assert(result.response == "(nil)");

    return 0;
}
