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
    const auto bundle_path = std::filesystem::path{"fixtures"} / "release" /
                             "runtime-artifact-bundle-manifest.json";
    const auto bundle = read_fixture_text(bundle_path);

    assert_contains(bundle, "\"bundle_version\":\"mini-kv-runtime-artifact-bundle.v1\"");
    assert_contains(bundle, "\"project\":\"mini-kv\"");
    assert_contains(bundle, "\"project_version\":\"0.65.0\"");
    assert_contains(bundle, "\"release_version\":\"v65\"");
    assert_contains(bundle, "\"read_only\":true");
    assert_contains(bundle, "\"execution_allowed\":false");
    assert_contains(bundle, "\"order_authoritative\":false");
    assert_contains(bundle, "\"consumer_hint\":\"Node v164 cross-project release bundle gate\"");

    assert_contains(bundle, "\"id\":\"binary-version\"");
    assert_contains(bundle, "\"id\":\"release-verification-manifest\"");
    assert_contains(bundle, "\"id\":\"runtime-artifact-rollback-evidence\"");
    assert_contains(bundle, "\"id\":\"wal-snapshot-compatibility\"");
    assert_contains(bundle, "\"id\":\"fixture-inventory\"");
    assert_contains(bundle, "\"source_release_version\":\"v64\"");
    assert_contains(bundle, "\"expected_release_version\":\"v65\"");
    assert_contains(bundle, "\"CHECKJSON LOAD data/bundle.snap\"");
    assert_contains(bundle, "\"CHECKJSON COMPACT\"");
    assert_contains(bundle, "\"CHECKJSON SETNXEX bundle:token 30 value\"");
    assert_contains(bundle, "\"GET bundle:token\"");
    assert_contains(bundle, "\"write_commands_executed\":false");
    assert_contains(bundle, "\"admin_commands_executed\":false");
    assert_contains(bundle, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(bundle, "\"WAL/Snapshot compatibility cannot create Java order authority\"");
    assert_contains(bundle, "\"bundle version compatibility is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        bundle_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-rollback-evidence.json",
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
        assert_contains(bundle, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.70.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("COMMANDSJSON");
    assert_contains(result.response, "\"name\":\"CHECKJSON\"");
    assert_contains(result.response, "\"name\":\"STORAGEJSON\"");
    assert_contains(result.response, "\"name\":\"SETNXEX\"");

    result = processor.execute("CHECKJSON LOAD data/bundle.snap");
    assert_contains(result.response, "\"command\":\"LOAD\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"side_effects\":[\"store_replace_from_snapshot\"]");

    result = processor.execute("CHECKJSON COMPACT");
    assert_contains(result.response, "\"command\":\"COMPACT\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"side_effects\":[\"wal_rewrite_when_enabled\"]");

    result = processor.execute("CHECKJSON SETNXEX bundle:token 30 value");
    assert_contains(result.response, "\"command\":\"SETNXEX\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"write_command\":true");

    result = processor.execute("STORAGEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("GET bundle:token");
    assert(result.response == "(nil)");

    return 0;
}
