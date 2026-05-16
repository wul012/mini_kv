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
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" /
                            "restore-boundary-smoke-manifest.json";
    const auto smoke = read_fixture_text(smoke_path);

    assert_contains(smoke, "\"smoke_manifest_version\":\"mini-kv-restore-boundary-smoke-manifest.v1\"");
    assert_contains(smoke, "\"project\":\"mini-kv\"");
    assert_contains(smoke, "\"project_version\":\"0.75.0\"");
    assert_contains(smoke, "\"release_version\":\"v75\"");
    assert_contains(smoke, "\"path\":\"fixtures/release/restore-boundary-smoke-manifest.json\"");
    assert_contains(smoke, "\"read_only\":true");
    assert_contains(smoke, "\"execution_allowed\":false");
    assert_contains(smoke, "\"restore_execution_allowed\":false");
    assert_contains(smoke, "\"order_authoritative\":false");
    assert_contains(smoke, "\"java_transaction_chain_connected\":false");
    assert_contains(smoke, "\"consumer_hint\":\"Node v185 real-read rehearsal intake\"");
    assert_contains(smoke, "\"scope\":\"restore approval boundary read-only smoke manifest");

    assert_contains(smoke, "\"target_release_version\":\"v75\"");
    assert_contains(smoke, "\"source_restore_approval_boundary\":\"fixtures/release/restore-approval-boundary.json\"");
    assert_contains(smoke, "\"source_verification_manifest\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(smoke, "\"smoke_manifest_id\":\"mini-kv-restore-boundary-smoke-v75\"");
    assert_contains(smoke, "\"runtime_target\":\"minikv_server 6415 127.0.0.1\"");
    assert_contains(smoke, "\"client_target\":\"minikv_client 127.0.0.1 6415 5000 --connect-retries 10 --retry-delay-ms 100\"");
    assert_contains(smoke, "\"restore_target_placeholder\":\"restore-target:<operator-approved-restore-target>\"");
    assert_contains(smoke, "\"artifact_digest_placeholder\":\"sha256:<operator-approved-restore-artifact-digest>\"");
    assert_contains(smoke, "\"boundary\":\"restore boundary smoke manifest records read-only runtime evidence only and does not execute restore\"");

    assert_contains(smoke, "\"CHECKJSON LOAD data/real-read-boundary-restore.snap\"");
    assert_contains(smoke, "\"CHECKJSON COMPACT\"");
    assert_contains(smoke, "\"CHECKJSON SETNXEX restore:real-read-token 30 value\"");
    assert_contains(smoke, "\"GET restore:real-read-token\"");
    assert_contains(smoke, "\"expected\":[\"INFOJSON version matches 0.75.0\"");
    assert_contains(smoke, "\"write_commands_executed\":false");
    assert_contains(smoke, "\"admin_commands_executed\":false");
    assert_contains(smoke, "\"runtime_write_observed\":false");
    assert_contains(smoke, "\"real_read_rehearsal_input\":true");
    assert_contains(smoke, "\"CHECKJSON used for risky commands\"");
    assert_contains(smoke, "\"GET token used to prove SETNXEX was not executed\"");
    assert_contains(smoke, "\"Node v185 may consume this manifest after Java v66 and mini-kv v75 complete\"");

    assert_contains(smoke, "\"restore boundary smoke manifest only\"");
    assert_contains(smoke, "\"real-read smoke manifest only\"");
    assert_contains(smoke, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(smoke, "\"mini-kv remains not Java order authority\"");
    assert_contains(smoke, "\"Node v185 may consume this fixture only after Java v66 and mini-kv v75 are complete\"");
    assert_contains(smoke, "\"smoke manifest is not a restore executor and not an approval decision\"");
    assert_contains(smoke, "\"restore target or real-read smoke scope is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        smoke_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-approval-boundary.json",
        std::filesystem::path{"fixtures"} / "release" / "retained-restore-artifact-digest.json",
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
        assert_contains(smoke, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.84.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/real-read-boundary-restore.snap");
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

    result = processor.execute("CHECKJSON SETNXEX restore:real-read-token 30 value");
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

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    return 0;
}
