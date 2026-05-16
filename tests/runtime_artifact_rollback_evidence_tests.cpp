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
    const auto evidence_path = std::filesystem::path{"fixtures"} / "release" /
                               "runtime-artifact-rollback-evidence.json";
    const auto evidence = read_fixture_text(evidence_path);

    assert_contains(evidence, "\"evidence_version\":\"mini-kv-runtime-artifact-rollback.v1\"");
    assert_contains(evidence, "\"project\":\"mini-kv\"");
    assert_contains(evidence, "\"project_version\":\"0.64.0\"");
    assert_contains(evidence, "\"release_version\":\"v64\"");
    assert_contains(evidence, "\"read_only\":true");
    assert_contains(evidence, "\"execution_allowed\":false");
    assert_contains(evidence, "\"order_authoritative\":false");
    assert_contains(evidence, "\"consumer_hint\":\"Node v163 release rollback readiness runbook\"");

    assert_contains(evidence, "\"id\":\"binary-version\"");
    assert_contains(evidence, "\"id\":\"wal\"");
    assert_contains(evidence, "\"id\":\"snapshot\"");
    assert_contains(evidence, "\"id\":\"fixtures\"");
    assert_contains(evidence, "WAL replay can restore mini-kv keys but cannot create Java order authority");
    assert_contains(evidence, "snapshot LOAD is a store replacement operation");
    assert_contains(evidence, "\"CHECKJSON LOAD data/rollback.snap\"");
    assert_contains(evidence, "\"CHECKJSON COMPACT\"");
    assert_contains(evidence, "\"CHECKJSON SETNXEX rollback:token 30 value\"");
    assert_contains(evidence, "\"GET rollback:token\"");
    assert_contains(evidence, "\"write_commands_executed\":false");
    assert_contains(evidence, "\"admin_commands_executed\":false");
    assert_contains(evidence, "\"cannot forge Java order authoritative state\"");
    assert_contains(evidence, "\"rollback target or version compatibility is unclear\"");

    assert_path_exists(evidence_path);
    assert_path_exists(std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json");
    assert_path_exists(std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json");

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.87.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/rollback.snap");
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

    result = processor.execute("CHECKJSON SETNXEX rollback:token 30 value");
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

    result = processor.execute("GET rollback:token");
    assert(result.response == "(nil)");

    return 0;
}
