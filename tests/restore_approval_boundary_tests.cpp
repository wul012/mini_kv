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
    const auto boundary_path = std::filesystem::path{"fixtures"} / "release" / "restore-approval-boundary.json";
    const auto boundary = read_fixture_text(boundary_path);

    assert_contains(boundary, "\"boundary_version\":\"mini-kv-restore-approval-boundary.v1\"");
    assert_contains(boundary, "\"project\":\"mini-kv\"");
    assert_contains(boundary, "\"project_version\":\"0.74.0\"");
    assert_contains(boundary, "\"release_version\":\"v74\"");
    assert_contains(boundary, "\"path\":\"fixtures/release/restore-approval-boundary.json\"");
    assert_contains(boundary, "\"read_only\":true");
    assert_contains(boundary, "\"execution_allowed\":false");
    assert_contains(boundary, "\"restore_execution_allowed\":false");
    assert_contains(boundary, "\"order_authoritative\":false");
    assert_contains(boundary, "\"java_transaction_chain_connected\":false");
    assert_contains(boundary, "\"consumer_hint\":\"Node v182 release approval decision rehearsal packet\"");
    assert_contains(boundary, "\"scope\":\"restore approval boundary fixture");

    assert_contains(boundary, "\"target_release_version\":\"v74\"");
    assert_contains(boundary, "\"source_retained_digest_fixture\":\"fixtures/release/retained-restore-artifact-digest.json\"");
    assert_contains(boundary, "\"source_verification_manifest\":\"fixtures/release/verification-manifest.json\"");
    assert_contains(boundary, "\"approval_boundary_id\":\"mini-kv-restore-approval-boundary-v74\"");
    assert_contains(boundary, "\"restore_approver_placeholder\":\"operator:<restore-approval-approver>\"");
    assert_contains(boundary, "\"restore_target_placeholder\":\"restore-target:<operator-approved-restore-target>\"");
    assert_contains(boundary, "\"artifact_digest_placeholder\":\"sha256:<operator-approved-restore-artifact-digest>\"");
    assert_contains(boundary, "\"java_transaction_chain\":\"disconnected\"");
    assert_contains(boundary, "\"boundary\":\"restore approval boundary records approval evidence only and does not execute restore\"");

    assert_contains(boundary, "\"restore_approver\"");
    assert_contains(boundary, "\"restore_target\"");
    assert_contains(boundary, "\"artifact_digest\"");
    assert_contains(boundary, "\"java_transaction_chain_connected\"");
    assert_contains(boundary, "\"approval_boundary_ready_for_node_review\"");
    assert_contains(boundary, "\"restore approver recorded without production secret value\"");
    assert_contains(boundary, "\"restore target placeholder recorded outside mini-kv\"");
    assert_contains(boundary, "\"artifact digest placeholder matches retained restore evidence outside mini-kv\"");
    assert_contains(boundary, "\"Java transaction chain remains disconnected\"");
    assert_contains(boundary, "\"mini-kv order_authoritative remains false\"");
    assert_contains(boundary, "\"restore execution remains disabled\"");
    assert_contains(boundary, "approval ledger and artifact bytes live outside mini-kv");

    assert_contains(boundary, "\"CHECKJSON LOAD data/approval-boundary-restore.snap\"");
    assert_contains(boundary, "\"CHECKJSON COMPACT\"");
    assert_contains(boundary, "\"CHECKJSON SETNXEX restore:approval-token 30 value\"");
    assert_contains(boundary, "\"GET restore:approval-token\"");
    assert_contains(boundary, "\"expected\":[\"INFOJSON version matches 0.74.0\"");
    assert_contains(boundary, "\"write_commands_executed\":false");
    assert_contains(boundary, "\"admin_commands_executed\":false");
    assert_contains(boundary, "\"approval_decision_rehearsal_input\":true");
    assert_contains(boundary, "\"restore approval boundary fixture only\"");
    assert_contains(boundary, "\"restore approver placeholder only\"");
    assert_contains(boundary, "\"approval ledger not written\"");
    assert_contains(boundary, "\"Java transaction chain disconnected\"");
    assert_contains(boundary, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(boundary, "\"mini-kv remains not Java order authority\"");
    assert_contains(boundary, "\"Node v182 may consume this fixture only after Node v181, Java v65, and mini-kv v74 are complete\"");
    assert_contains(boundary, "\"restore approval boundary fixture is not an approval decision and not a restore executor\"");
    assert_contains(boundary, "\"restore approver or restore target is unclear\"");

    const std::vector<std::filesystem::path> required_paths = {
        boundary_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
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
        assert_contains(boundary, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("INFOJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.98.0\"");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");

    result = processor.execute("CHECKJSON LOAD data/approval-boundary-restore.snap");
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

    result = processor.execute("CHECKJSON SETNXEX restore:approval-token 30 value");
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

    result = processor.execute("GET restore:approval-token");
    assert(result.response == "(nil)");

    return 0;
}
