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
    const auto package_path = std::filesystem::path{"fixtures"} / "release" /
                              "release-artifact-digest-package.json";
    const auto package = read_fixture_text(package_path);

    assert_contains(package, "\"package_version\":\"mini-kv-release-artifact-digest-package.v1\"");
    assert_contains(package, "\"project\":\"mini-kv\"");
    assert_contains(package, "\"project_version\":\"0.69.0\"");
    assert_contains(package, "\"release_version\":\"v69\"");
    assert_contains(package, "\"read_only\":true");
    assert_contains(package, "\"execution_allowed\":false");
    assert_contains(package, "\"restore_execution_allowed\":false");
    assert_contains(package, "\"order_authoritative\":false");
    assert_contains(package, "\"consumer_hint\":\"Node v170 deployment evidence intake gate\"");

    assert_contains(package, "\"target_release_version\":\"v69\"");
    assert_contains(package, "\"previous_release_version\":\"v68\"");
    assert_contains(package, "\"id\":\"binary-digest\"");
    assert_contains(package, "\"id\":\"wal-checksum-evidence\"");
    assert_contains(package, "\"id\":\"snapshot-digest-evidence\"");
    assert_contains(package, "\"id\":\"fixture-digest\"");
    assert_contains(package, "\"digest_placeholder\":\"sha256:<operator-recorded-binary-digest>\"");
    assert_contains(package, "\"digest_placeholder\":\"sha256:<operator-recorded-wal-evidence-digest>\"");
    assert_contains(package, "\"digest_placeholder\":\"sha256:<operator-recorded-snapshot-evidence-digest>\"");
    assert_contains(package, "\"digest_placeholder\":\"sha256:<operator-recorded-fixture-digest>\"");
    assert_contains(package, "\"compatible_with\":[\"CMake project version 0.69.0\",\"INFOJSON runtime version 0.69.0\"]");

    assert_contains(package, "\"commands\":[\"INFOJSON\",\"CHECKJSON LOAD data/release-artifact-drill.snap\"");
    assert_contains(package, "\"CHECKJSON COMPACT\"");
    assert_contains(package, "\"CHECKJSON SETNXEX release:token 30 value\"");
    assert_contains(package, "\"GET release:token\"");
    assert_contains(package, "\"release_operator_id\"");
    assert_contains(package, "\"binary_digest_recorded_at\"");
    assert_contains(package, "\"wal_snapshot_digest_recorded_at\"");
    assert_contains(package, "\"fixture_digest_recorded_at\"");
    assert_contains(package, "\"artifact_matrix_cross_checked\"");
    assert_contains(package, "\"write_commands_executed\":false");
    assert_contains(package, "\"admin_commands_executed\":false");
    assert_contains(package, "\"release artifact package is review evidence only");
    assert_contains(package, "\"digest placeholders must be verified outside mini-kv before production use\"");
    assert_contains(package, "\"binary/WAL/Snapshot/fixture digest is unclear or mismatched\"");

    const std::vector<std::filesystem::path> required_paths = {
        package_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "artifact-digest-compatibility-matrix.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-dry-run-operator-package.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-bundle-manifest.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "recovery-evidence.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(package, path.generic_string());
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

    result = processor.execute("CHECKJSON LOAD data/release-artifact-drill.snap");
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

    result = processor.execute("CHECKJSON SETNXEX release:token 30 value");
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

    result = processor.execute("GET release:token");
    assert(result.response == "(nil)");

    return 0;
}
