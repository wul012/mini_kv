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
    const auto handoff_path = std::filesystem::path{"fixtures"} / "release" /
                              "restore-compatibility-handoff.json";
    const auto handoff = read_fixture_text(handoff_path);

    assert_contains(handoff, "\"handoff_version\":\"mini-kv-restore-compatibility-handoff.v1\"");
    assert_contains(handoff, "\"project\":\"mini-kv\"");
    assert_contains(handoff, "\"project_version\":\"0.66.0\"");
    assert_contains(handoff, "\"release_version\":\"v66\"");
    assert_contains(handoff, "\"read_only\":true");
    assert_contains(handoff, "\"execution_allowed\":false");
    assert_contains(handoff, "\"restore_execution_allowed\":false");
    assert_contains(handoff, "\"order_authoritative\":false");
    assert_contains(handoff, "\"consumer_hint\":\"Node v166 rollback window readiness checklist\"");

    assert_contains(handoff, "\"id\":\"binary-compatibility\"");
    assert_contains(handoff, "\"id\":\"wal-compatibility\"");
    assert_contains(handoff, "\"id\":\"snapshot-compatibility\"");
    assert_contains(handoff, "\"id\":\"fixture-compatibility\"");
    assert_contains(handoff, "\"operator_confirmation_required\":true");
    assert_contains(handoff, "\"load_replaces_store_acknowledged\"");
    assert_contains(handoff, "\"CHECKJSON LOAD data/restore.snap\"");
    assert_contains(handoff, "\"CHECKJSON COMPACT\"");
    assert_contains(handoff, "\"CHECKJSON SETNXEX restore:token 30 value\"");
    assert_contains(handoff, "\"GET restore:token\"");
    assert_contains(handoff, "\"write_commands_executed\":false");
    assert_contains(handoff, "\"admin_commands_executed\":false");
    assert_contains(handoff, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(handoff, "\"restore compatibility cannot create Java order authority\"");
    assert_contains(handoff, "\"restore target or version compatibility is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        handoff_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-bundle-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-rollback-evidence.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "recovery" / "restart-recovery-evidence.json",
        std::filesystem::path{"fixtures"} / "ttl-token" / "recovery-evidence.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(handoff, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.66.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/restore.snap");
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

    result = processor.execute("CHECKJSON SETNXEX restore:token 30 value");
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

    result = processor.execute("GET restore:token");
    assert(result.response == "(nil)");

    return 0;
}
