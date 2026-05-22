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

void assert_not_contains(const std::string& text, std::string_view forbidden) {
    if (text.find(std::string{forbidden}) != std::string::npos) {
        std::cerr << "unexpected text: " << forbidden << '\n';
    }
    assert(text.find(std::string{forbidden}) == std::string::npos);
}

void assert_path_exists(const std::filesystem::path& relative_path) {
    assert(std::filesystem::exists(std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path));
}

void assert_current_digest_set(const std::string& text) {
    assert_contains(text, "\"current_project_version\":\"0.102.0\"");
    assert_contains(text, "\"current_release_version\":\"v102\"");
    assert_contains(text, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_contains(text, "\"current_live_read_session_echo\":\"mini-kv-live-read-v102\"");
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
    assert_contains(text, "fnv1a64:f92fcba55feb26a2");
    assert_contains(text, "fnv1a64:5bef33f2fbe65cc5");
}

void assert_no_start_no_write_flags(const std::string& text) {
    assert_contains(text, "\"read_only\":true");
    assert_contains(text, "\"execution_allowed\":false");
    assert_contains(text, "\"node_auto_start_allowed\":false");
    assert_contains(text, "\"java_auto_start_allowed\":false");
    assert_contains(text, "\"mini_kv_auto_start_allowed\":false");
    assert_contains(text, "\"connection_execution_allowed\":false");
    assert_contains(text, "\"write_commands_executed\":false");
    assert_contains(text, "\"admin_commands_executed\":false");
    assert_contains(text, "\"runtime_write_observed\":false");
    assert_contains(text, "\"managed_audit_store\":false");
    assert_contains(text, "\"storage_write_allowed\":false");
    assert_contains(text, "\"managed_audit_write_executed\":false");
    assert_contains(text, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_contains(text, "\"credential_value_required\":false");
    assert_contains(text, "\"credential_value_read_allowed\":false");
    assert_contains(text, "\"schema_rehearsal_execution_allowed\":false");
    assert_contains(text, "\"schema_migration_execution_allowed\":false");
    assert_contains(text, "\"restore_execution_allowed\":false");
    assert_contains(text, "\"load_restore_compact_executed\":false");
    assert_contains(text, "\"setnxex_execution_allowed\":false");
    assert_contains(text, "\"operator_window_write_allowed\":false");
    assert_contains(text, "\"order_authoritative\":false");
}

} // namespace

int main() {
    const auto receipt_path = std::filesystem::path{"fixtures"} / "release" /
                              "operator-window-no-start-no-write-receipt.json";
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" /
                            "runtime-smoke-evidence.json";
    const auto manifest_path = std::filesystem::path{"fixtures"} / "release" /
                               "verification-manifest.json";

    assert_path_exists(receipt_path);
    assert_path_exists(smoke_path);
    assert_path_exists(manifest_path);

    const auto receipt = read_fixture_text(receipt_path);
    assert_contains(receipt, "\"receipt_version\":\"mini-kv-operator-window-no-start-no-write-receipt.v1\"");
    assert_contains(receipt, "\"path\":\"fixtures/release/operator-window-no-start-no-write-receipt.json\"");
    assert_contains(receipt, "\"consumer_hint\":\"Node v239 manual sandbox connection operator window evidence verification\"");
    assert_contains(receipt, "\"producer\":\"Node v238 manual sandbox connection operator window checklist\"");
    assert_contains(receipt, "\"checklist_state\":\"manual-sandbox-connection-operator-window-checklist-ready\"");
    assert_contains(receipt, "\"approval_item_count\":3");
    assert_contains(receipt, "\"checklist_step_count\":8");
    assert_contains(receipt, "\"pause_condition_count\":8");
    assert_contains(receipt, "\"forbidden_operation_count\":6");
    assert_contains(receipt, "\"ready_for_java_v93_echo_receipt\":true");
    assert_contains(receipt, "\"ready_for_managed_audit_sandbox_adapter_connection\":false");
    assert_contains(receipt, "\"operator_window_materials_read_only\":true");
    assert_not_contains(receipt, "credential_value\":\"");
    assert_current_digest_set(receipt);
    assert_no_start_no_write_flags(receipt);
    assert_contains(receipt, "\"operator window receipt only\"");
    assert_contains(receipt, "\"Node v238 checklist field echo only\"");
    assert_contains(receipt, "\"no mini-kv auto-start permission\"");
    assert_contains(receipt, "\"no storage write or managed audit state write\"");
    assert_contains(receipt, "\"no LOAD/COMPACT/RESTORE/SETNXEX execution\"");

    const auto smoke = read_fixture_text(smoke_path);
    assert_contains(smoke, "\"runtime_smoke_evidence_version\":\"mini-kv-runtime-smoke-evidence.v20\"");
    assert_contains(smoke, "fixtures/release/operator-window-no-start-no-write-receipt.json");
    assert_contains(smoke, "\"operator_window_no_start_no_write_receipt\":");
    assert_contains(smoke, "\"source_checklist\":\"Node v238 manual sandbox connection operator window checklist\"");
    assert_current_digest_set(smoke);
    assert_no_start_no_write_flags(smoke);

    const auto manifest = read_fixture_text(manifest_path);
    assert_contains(manifest, "\"minikv_operator_window_no_start_no_write_receipt_tests\"");
    assert_contains(manifest, "\"path\":\"fixtures/release/operator-window-no-start-no-write-receipt.json\"");
    assert_contains(manifest, "\"operator_window_no_start_no_write_receipt\":");
    assert_contains(manifest, "\"operator window no-start/no-write receipt only\"");
    assert_contains(manifest, "\"Node v238 checklist field echo only\"");
    assert_current_digest_set(manifest);
    assert_no_start_no_write_flags(manifest);

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now();
    minikv::CommandProcessor processor{store, nullptr, options};

    const auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"operator_window_no_start_no_write_receipt\":");
    assert_contains(result.response, "\"receipt_fixture_path\":\"fixtures/release/operator-window-no-start-no-write-receipt.json\"");
    assert_contains(result.response, "\"source_checklist\":\"Node v238 manual sandbox connection operator window checklist\"");
    assert_current_digest_set(result.response);
    assert_no_start_no_write_flags(result.response);

    const auto restore_token = processor.execute("GET restore:real-read-token");
    assert(restore_token.response == "(nil)");
}
