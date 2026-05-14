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

std::string read_file_text(const std::filesystem::path& path) {
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

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    return read_file_text(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path);
}

void assert_contains(const std::string& text, std::string_view expected) {
    assert(text.find(std::string{expected}) != std::string::npos);
}

void assert_path_exists(const std::filesystem::path& relative_path) {
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path));
}

} // namespace

int main() {
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json";
    const auto manifest = read_fixture_text(manifest_path);

    assert_contains(manifest, "\"manifest_version\":\"mini-kv-release-verification-manifest.v1\"");
    assert_contains(manifest, "\"project\":\"mini-kv\"");
    assert_contains(manifest, "\"project_version\":\"0.66.0\"");
    assert_contains(manifest, "\"release_version\":\"v66\"");
    assert_contains(manifest, "\"read_only\":true");
    assert_contains(manifest, "\"execution_allowed\":false");
    assert_contains(manifest, "\"order_authoritative\":false");
    assert_contains(manifest, "\"no_runtime_command_added\":true");
    assert_contains(manifest, "\"consumer_hint\":\"Node v166 rollback window readiness checklist\"");

    assert_contains(manifest, "\"command\":\"cmake -S . -B cmake-build-v66");
    assert_contains(manifest, "\"command\":\"cmake --build cmake-build-v66 --parallel 2\"");
    assert_contains(manifest, "\"command\":\"ctest --test-dir cmake-build-v66 --output-on-failure\"");
    assert_contains(manifest, "\"minikv_command_tests\"");
    assert_contains(manifest, "\"minikv_readonly_fixture_tests\"");
    assert_contains(manifest, "\"minikv_recovery_fixture_index_tests\"");
    assert_contains(manifest, "\"minikv_ttl_token_recovery_tests\"");
    assert_contains(manifest, "\"minikv_release_verification_manifest_tests\"");
    assert_contains(manifest, "\"minikv_runtime_artifact_rollback_evidence_tests\"");
    assert_contains(manifest, "\"minikv_runtime_artifact_bundle_manifest_tests\"");
    assert_contains(manifest, "\"minikv_restore_compatibility_handoff_tests\"");

    assert_contains(manifest, "\"INFOJSON\"");
    assert_contains(manifest, "\"CHECKJSON LOAD data/restore.snap\"");
    assert_contains(manifest, "\"CHECKJSON COMPACT\"");
    assert_contains(manifest, "\"CHECKJSON SETNXEX restore:token 30 value\"");
    assert_contains(manifest, "\"STORAGEJSON\"");
    assert_contains(manifest, "\"HEALTH\"");
    assert_contains(manifest, "\"GET restore:token\"");
    assert_contains(manifest, "\"write_commands_executed\":false");
    assert_contains(manifest, "\"admin_commands_executed\":false");

    const std::vector<std::filesystem::path> required_paths = {
        manifest_path,
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-rollback-evidence.json",
        std::filesystem::path{"fixtures"} / "release" / "runtime-artifact-bundle-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-compatibility-handoff.json",
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
        assert_contains(manifest, "\"path\":\"" + path.generic_string() + "\"");
    }

    assert_contains(manifest, "\"cmake_project_version\":\"0.66.0\"");
    assert_contains(manifest, "\"generated_header\":\"include/minikv/version.hpp.in\"");
    assert_contains(manifest, "\"fixtures/readonly/infojson-empty-inline.json\"");
    assert_contains(manifest, "\"fixtures/release/verification-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-artifact-rollback-evidence.json\"");
    assert_contains(manifest, "\"fixtures/release/runtime-artifact-bundle-manifest.json\"");
    assert_contains(manifest, "\"fixtures/release/restore-compatibility-handoff.json\"");
    assert_contains(manifest, "\"manifest only\"");
    assert_contains(manifest, "\"bundle manifest only\"");
    assert_contains(manifest, "\"restore compatibility handoff sample only\"");
    assert_contains(manifest, "\"no runtime command added\"");
    assert_contains(manifest, "\"not connected to Java transaction chain\"");
    assert_contains(manifest, "\"does not perform restore\"");

    const auto cmake_lists = read_file_text(std::filesystem::path{MINIKV_SOURCE_DIR} / "CMakeLists.txt");
    assert_contains(cmake_lists, "project(mini_kv VERSION 0.66.0");
    assert_contains(cmake_lists, "minikv_release_verification_manifest_tests");
    assert_contains(cmake_lists, "minikv_runtime_artifact_rollback_evidence_tests");
    assert_contains(cmake_lists, "minikv_runtime_artifact_bundle_manifest_tests");
    assert_contains(cmake_lists, "minikv_restore_compatibility_handoff_tests");

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

    result = processor.execute("GET restore:token");
    assert(result.response == "(nil)");

    return 0;
}
