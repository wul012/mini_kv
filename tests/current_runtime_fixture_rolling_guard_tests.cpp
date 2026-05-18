#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
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
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

void assert_path_exists(const std::filesystem::path& relative_path) {
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path));
}

void assert_current_rolling_digest_set(const std::string& text) {
    assert_contains(text, "0.102.0");
    assert_contains(text, "v102");
    assert_contains(text, "c/102/");
    assert_contains(text, "mini-kv-live-read-v102");
    assert_contains(text, "fnv1a64:d9776044d393ecbc");
    assert_contains(text, "fnv1a64:8cf7cc0df218035f");
    assert_contains(text, "fnv1a64:6b9ef93f40cbd343");
    assert_contains(text, "fnv1a64:1177efbaceb52c1c");
    assert_contains(text, "fnv1a64:667590b83f510a58");
    assert_contains(text, "fnv1a64:0f2caa6931b482b8");
    assert_contains(text, "fnv1a64:0174fa831d17cea4");
    assert_contains(text, "fnv1a64:9bacde73d6d07097");
    assert_contains(text, "fnv1a64:f0cae7a4ce0674c2");
    assert_contains(text, "fnv1a64:beb8dd6a0b102a11");
    assert_contains(text, "fnv1a64:69e377d01f15fd57");
    assert_contains(text, "fnv1a64:5bef33f2fbe65cc5");
}

void assert_historical_consumed_digest_anchors(const std::string& text) {
    assert_contains(text, "fnv1a64:357cc7e9eec3f223");
    assert_contains(text, "fnv1a64:1ea4570c967cfdb1");
    assert_contains(text, "fnv1a64:f39d8e3ef98654ea");
    assert_contains(text, "fnv1a64:111f0daf1283eab6");
    assert_contains(text, "fnv1a64:4aa6d12fb067e2a6");
    assert_contains(text, "fnv1a64:76411286a0913dc8");
    assert_contains(text, "fnv1a64:0dfb07cd2f8de289");
    assert_contains(text, "fnv1a64:ceaed265f7f9560c");
    assert_contains(text, "fnv1a64:b9fc556875ea625b");
}

} // namespace

int main() {
    const auto guard_path = std::filesystem::path{"fixtures"} / "release" /
                            "current-runtime-fixture-rolling-guard.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" /
                            "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" /
                               "verification-manifest.json";

    assert_path_exists(guard_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto guard = read_fixture_text(guard_path);
    assert_contains(guard, "\"guard_version\":\"mini-kv-current-runtime-fixture-rolling-guard.v1\"");
    assert_contains(guard, "\"project\":\"mini-kv\"");
    assert_contains(guard, "\"path\":\"fixtures/release/current-runtime-fixture-rolling-guard.json\"");
    assert_contains(guard, "\"read_only\":true");
    assert_contains(guard, "\"execution_allowed\":false");
    assert_contains(guard, "\"restore_execution_allowed\":false");
    assert_contains(guard, "\"order_authoritative\":false");
    assert_contains(guard, "\"consumer_hint\":\"Node v239 manual sandbox connection operator window evidence verification\"");
    assert_contains(guard, "current runtime fixture rolling evidence guard");
    assert_contains(guard, "\"source_fixture\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(guard, "\"release_manifest\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(guard, "\"historical_receipt_retention\":[");
    assert_contains(guard, "\"must_remain_stable\":true");
    assert_contains(guard, "historical consumed release versions, artifact path hints, and consumed digests remain fixed");
    assert_contains(guard, "\"guard fixture only\"");
    assert_contains(guard, "\"read-only rolling evidence check only\"");
    assert_contains(guard, "\"current runtime fixture may roll by release\"");
    assert_contains(guard, "\"historical consumed receipt digests must remain stable\"");
    assert_contains(guard, "\"no LOAD/COMPACT/RESTORE/SETNXEX execution\"");
    assert_contains(guard, "\"no WAL, snapshot, restore, or command dispatch behavior change\"");
    assert_contains(guard, "\"no managed audit storage backend\"");
    assert_contains(guard, "\"no credential value read\"");
    assert_contains(guard, "\"no schema rehearsal execution\"");
    assert_contains(guard, "\"no Node, Java, or mini-kv auto-start permission\"");
    assert_contains(guard, "\"not Java order authority\"");
    assert_current_rolling_digest_set(guard);
    assert_contains(guard, "fnv1a64:f92fcba55feb26a2");
    assert_historical_consumed_digest_anchors(guard);

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"project_version\":\"0.102.0\"");
    assert_contains(smoke, "\"release_version\":\"v102\"");
    assert_contains(smoke, "\"consumer_hint\":\"Node v239 manual sandbox connection operator window evidence verification\"");
    assert_contains(smoke, "\"current_runtime_fixture_rolling_guard\":");
    assert_contains(smoke, "\"guard_fixture_path\":\"fixtures/release/current-runtime-fixture-rolling-guard.json\"");
    assert_contains(smoke, "\"historical_consumed_digest_count\":9");
    assert_contains(smoke, "Current runtime fixture rolling evidence guard lets Node v239 verify the v102 current fixture while preserving historical consumed digest anchors");
    assert_current_rolling_digest_set(smoke);
    assert_contains(smoke, "fnv1a64:f92fcba55feb26a2");
    assert_historical_consumed_digest_anchors(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_current_runtime_fixture_rolling_guard_tests\"");
    assert_contains(manifest, "\"path\":\"fixtures/release/current-runtime-fixture-rolling-guard.json\"");
    assert_contains(manifest, "\"current_runtime_fixture_rolling_guard\":");
    assert_contains(manifest, "\"historical_consumed_digest_count\":9");
    assert_contains(manifest, "\"current runtime fixture rolling guard only\"");
    assert_contains(manifest, "\"current fixture evidence may roll while historical consumed digest anchors remain fixed\"");
    assert_current_rolling_digest_set(manifest);
    assert_historical_consumed_digest_anchors(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.102.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_current_rolling_digest_set(result.response);
    assert_contains(result.response, "fnv1a64:f92fcba55feb26a2");
    assert_historical_consumed_digest_anchors(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
