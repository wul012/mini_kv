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
    const auto matrix_path = std::filesystem::path{"fixtures"} / "release" /
                             "artifact-digest-compatibility-matrix.json";
    const auto matrix = read_fixture_text(matrix_path);

    assert_contains(matrix, "\"matrix_version\":\"mini-kv-artifact-digest-compatibility-matrix.v1\"");
    assert_contains(matrix, "\"project\":\"mini-kv\"");
    assert_contains(matrix, "\"project_version\":\"0.68.0\"");
    assert_contains(matrix, "\"release_version\":\"v68\"");
    assert_contains(matrix, "\"read_only\":true");
    assert_contains(matrix, "\"execution_allowed\":false");
    assert_contains(matrix, "\"restore_execution_allowed\":false");
    assert_contains(matrix, "\"order_authoritative\":false");
    assert_contains(matrix, "\"consumer_hint\":\"Node v168 production environment preflight checklist\"");

    assert_contains(matrix, "\"target_release_version\":\"v68\"");
    assert_contains(matrix, "\"previous_release_version\":\"v67\"");
    assert_contains(matrix, "\"id\":\"binary-digest\"");
    assert_contains(matrix, "\"id\":\"wal-checksum-evidence\"");
    assert_contains(matrix, "\"id\":\"snapshot-digest-evidence\"");
    assert_contains(matrix, "\"id\":\"fixture-digest\"");
    assert_contains(matrix, "\"digest_placeholder\":\"sha256:<operator-recorded-binary-digest>\"");
    assert_contains(matrix, "\"digest_placeholder\":\"sha256:<operator-recorded-wal-evidence-digest>\"");
    assert_contains(matrix, "\"digest_placeholder\":\"sha256:<operator-recorded-snapshot-evidence-digest>\"");
    assert_contains(matrix, "\"digest_placeholder\":\"sha256:<operator-recorded-fixture-digest>\"");
    assert_contains(matrix, "\"compatible_with\":[\"CMake project version 0.68.0\",\"INFOJSON runtime version 0.68.0\"]");

    assert_contains(matrix, "\"id\":\"binary-version-match\"");
    assert_contains(matrix, "\"id\":\"wal-matrix-review\"");
    assert_contains(matrix, "\"id\":\"snapshot-matrix-review\"");
    assert_contains(matrix, "\"id\":\"fixture-matrix-review\"");
    assert_contains(matrix, "\"CHECKJSON LOAD data/digest-matrix.snap\"");
    assert_contains(matrix, "\"CHECKJSON COMPACT\"");
    assert_contains(matrix, "\"CHECKJSON SETNXEX digest:token 30 value\"");
    assert_contains(matrix, "\"GET digest:token\"");
    assert_contains(matrix, "\"write_commands_executed\":false");
    assert_contains(matrix, "\"admin_commands_executed\":false");
    assert_contains(matrix, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(matrix, "\"mini-kv remains not Java order authority\"");
    assert_contains(matrix, "\"digest placeholders must be verified outside mini-kv before production use\"");
    assert_contains(matrix, "\"binary/WAL/Snapshot/fixture digest is unclear or mismatched\"");

    const std::vector<std::filesystem::path> required_paths = {
        matrix_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-dry-run-operator-package.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-bundle-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-compatibility-handoff.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "recovery-evidence.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(matrix, path.generic_string());
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

    result = processor.execute("CHECKJSON LOAD data/digest-matrix.snap");
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

    result = processor.execute("CHECKJSON SETNXEX digest:token 30 value");
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

    result = processor.execute("GET digest:token");
    assert(result.response == "(nil)");

    return 0;
}
