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
    const auto smoke_path = std::filesystem::path{"fixtures"} / "release" / "runtime-smoke-evidence.json";
    const auto smoke = read_fixture_text(smoke_path);

    assert_contains(smoke, "\"runtime_smoke_evidence_version\":\"mini-kv-runtime-smoke-evidence.v3\"");
    assert_contains(smoke, "\"project\":\"mini-kv\"");
    assert_contains(smoke, "\"project_version\":\"0.78.0\"");
    assert_contains(smoke, "\"release_version\":\"v78\"");
    assert_contains(smoke, "\"path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_contains(smoke, "\"read_only\":true");
    assert_contains(smoke, "\"execution_allowed\":false");
    assert_contains(smoke, "\"restore_execution_allowed\":false");
    assert_contains(smoke, "\"order_authoritative\":false");
    assert_contains(smoke, "\"java_transaction_chain_connected\":false");
    assert_contains(smoke, "\"consumer_hint\":\"Node v196 imported window result packet\"");

    assert_contains(smoke, "\"primary\":\"SMOKEJSON\"");
    assert_contains(smoke, "\"supporting\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(smoke, "\"forbidden\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(smoke, "\"SMOKEJSON version matches 0.78.0\"");
    assert_contains(smoke, "\"SMOKEJSON reports write_commands_executed=false and runtime_write_observed=false\"");
    assert_contains(smoke, "SMOKEJSON exposes failure taxonomy categories and taxonomy_digest");

    assert_contains(smoke, "\"server\":\"minikv_server 6416 127.0.0.1\"");
    assert_contains(smoke, "\"client\":\"minikv_client 127.0.0.1 6416 5000 --connect-retries 10 --retry-delay-ms 100\"");
    assert_contains(smoke, "\"commands\":[\"SMOKEJSON\",\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"GET restore:real-read-token\",\"QUIT\"]");

    assert_contains(smoke, "\"target_version\":\"Node v196\"");
    assert_contains(smoke, "\"mode\":\"imported real-read window result verification\"");
    assert_contains(smoke, "\"default_enabled\":false");
    assert_contains(smoke, "\"mini-kv is already running and user enabled a safe real-read window\"");
    assert_contains(smoke, "\"node_must_verify\":\"failure_taxonomy.taxonomy_digest equals fnv1a64:f92fcba55feb26a2");
    assert_contains(smoke, "execute LOAD/COMPACT/SETNXEX/restore");

    assert_contains(smoke, "\"runtime smoke evidence only\"");
    assert_contains(smoke, "\"read-only aggregate only\"");
    assert_contains(smoke, "\"taxonomy digest sample only\"");
    assert_contains(smoke, "\"does not execute LOAD/COMPACT/SETNXEX\"");
    assert_contains(smoke, "\"mini-kv remains not Java order authority\"");
    assert_contains(smoke, "\"SMOKEJSON is a read adapter target, not an execution gate\"");
    assert_contains(smoke, "\"real-read smoke scope is unclear\"");
    assert_contains(smoke, "\"taxonomy digest is missing or mismatched\"");
    assert_contains(smoke, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(smoke, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(smoke, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(smoke, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(smoke, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.78.0\"");
    assert_contains(smoke, "\"expected_taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(smoke, "\"node_action\":\"verify digest before importing manual window results\"");
    assert_contains(smoke, "\"id\":\"closed-window\",\"source\":\"operator_window\"");
    assert_contains(smoke, "\"id\":\"connection-refused\",\"source\":\"tcp_connect\"");
    assert_contains(smoke, "\"id\":\"timeout\",\"source\":\"tcp_read\"");
    assert_contains(smoke, "\"id\":\"invalid-json\",\"source\":\"SMOKEJSON_parse\"");
    assert_contains(smoke, "\"id\":\"read-command-failed\",\"source\":\"runtime_read_command\"");
    assert_contains(smoke, "\"id\":\"unsafe-surface\",\"source\":\"adapter_command_plan\"");
    assert_contains(smoke, "\"id\":\"unexpected-write-signal\",\"source\":\"runtime_smoke_diagnostics\"");
    assert_contains(smoke, "\"node_action\":\"verify digest and stop before import when unsafe-surface or unexpected-write-signal appears\"");

    const std::vector<std::filesystem::path> required_paths = {
        smoke_path,
        std::filesystem::path{"fixtures"} / "release" / "verification-manifest.json",
        std::filesystem::path{"fixtures"} / "release" / "restore-boundary-smoke-manifest.json",
        std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json",
        std::filesystem::path{"fixtures"} / "readonly" / "runtime-read-field-guide.json",
    };

    for (const auto& path : required_paths) {
        assert_path_exists(path);
        assert_contains(smoke, path.generic_string());
    }

    minikv::Store store;
    minikv::CommandProcessorOptions options;
    options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::CommandProcessor processor{store, nullptr, options};

    auto result = processor.execute("SMOKEJSON");
    assert_contains(result.response, "\"schema_version\":1");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"restore_execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_smoke\"");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"version\":\"0.78.0\"");
    assert_contains(result.response, "\"server\":{\"protocol\":[\"inline\"]");
    assert_contains(result.response, "\"uptime_seconds\":0");
    assert_contains(result.response, "\"store\":{\"live_keys\":0,\"order_authoritative\":false}");
    assert_contains(result.response, "\"wal\":{\"enabled\":false,\"status\":\"disabled\",\"compact_recommended\":false}");
    assert_contains(result.response, "\"connection_stats\":{\"available\":false}");
    assert_contains(result.response, "\"real_read\":{\"allowed\":true,\"commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_contains(result.response, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_contains(result.response, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_contains(result.response, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_contains(result.response, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_contains(result.response, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.78.0\"");
    assert_contains(result.response, "\"expected_taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_contains(result.response, "\"node_action\":\"verify digest before importing manual window results\"");
    assert_contains(result.response, "\"id\":\"closed-window\",\"source\":\"operator_window\"");
    assert_contains(result.response, "\"id\":\"connection-refused\",\"source\":\"tcp_connect\"");
    assert_contains(result.response, "\"id\":\"timeout\",\"source\":\"tcp_read\"");
    assert_contains(result.response, "\"id\":\"invalid-json\",\"source\":\"SMOKEJSON_parse\"");
    assert_contains(result.response, "\"id\":\"read-command-failed\",\"source\":\"runtime_read_command\"");
    assert_contains(result.response, "\"id\":\"unsafe-surface\",\"source\":\"adapter_command_plan\"");
    assert_contains(result.response, "\"id\":\"unexpected-write-signal\",\"source\":\"runtime_smoke_diagnostics\"");
    assert_contains(result.response, "\"safe_to_auto_start\":false");
    assert_contains(result.response, "\"write_risk\":true");
    assert_contains(result.response, "\"write_commands_executed\":false");
    assert_contains(result.response, "\"admin_commands_executed\":false");
    assert_contains(result.response, "\"runtime_write_observed\":false");
    assert_contains(result.response, "\"node_consumption\":\"Node v196 may verify this command before importing a manual real-read window result; mini-kv must already be running and the read-only window must be open\"");
    assert_contains(result.response, "\"notes\":[\"runtime_smoke_evidence\",\"read_only_aggregate\",\"not_order_authoritative\","
                                     "\"does_not_execute_load_compact_setnxex_or_restore\"]");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON SMOKEJSON");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"command\":\"SMOKEJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\",\"store_read\",\"wal_metadata_read_when_enabled\"]");
    assert_contains(result.response, "\"warnings\":[\"not a write command\"]");

    return 0;
}
