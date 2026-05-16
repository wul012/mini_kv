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

std::size_t count_occurrences(const std::string& text, std::string_view needle) {
    std::size_t count = 0;
    std::size_t offset = 0;
    const std::string value{needle};
    while (true) {
        const auto found = text.find(value, offset);
        if (found == std::string::npos) {
            return count;
        }
        ++count;
        offset = found + value.size();
    }
}

} // namespace

int main() {
    const auto index = read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "index.json");
    const auto infojson_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "infojson-empty-inline.json");
    const auto statsjson_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "statsjson-empty-inline.json");
    const auto field_guide =
        read_fixture_text(std::filesystem::path{"fixtures"} / "readonly" / "runtime-read-field-guide.json");
    const auto checkjson_set_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "set-orderops-write-contract.json");
    const auto checkjson_get_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "get-orderops-read-contract.json");

    assert_contains(index, "\"index_version\":\"mini-kv-readonly-fixtures.v1\"");
    assert_contains(index, "\"read_only\":true");
    assert_contains(index, "\"execution_allowed\":false");
    assert_contains(index, "\"runtime_read_commands_allowed\":true");
    assert_contains(index, "\"write_execution_allowed\":false");
    assert_contains(index, "\"order_authoritative\":false");
    assert_contains(index,
                    "\"consumer_hint\":\"Node v159 read-only capture release evidence review preparation\"");
    assert_contains(index, "\"fixture_count\":5");
    assert(count_occurrences(index, "\"path\":") == 5);
    assert_contains(index, "\"command\":\"CHECKJSON SET orderops:1 value\"");
    assert_contains(index, "\"path\":\"fixtures/checkjson/set-orderops-write-contract.json\"");
    assert_contains(index, "\"command\":\"CHECKJSON GET orderops:1\"");
    assert_contains(index, "\"path\":\"fixtures/checkjson/get-orderops-read-contract.json\"");
    assert_contains(index, "\"command\":\"INFOJSON\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/infojson-empty-inline.json\"");
    assert_contains(index, "\"sample_type\":\"runtime_identity\",\"schema_version\":1,"
                           "\"read_only\":true,\"execution_allowed\":false,"
                           "\"order_authoritative\":false");
    assert_contains(index, "\"dynamic_fields\":[\"server.uptime_seconds\"]");
    assert_contains(index, "\"command\":\"STATSJSON\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/statsjson-empty-inline.json\"");
    assert_contains(index, "\"sample_type\":\"runtime_metrics\",\"schema_version\":1,"
                           "\"read_only\":true,\"execution_allowed\":false,"
                           "\"order_authoritative\":false");
    assert_contains(index, "\"id\":\"runtime-read-field-guide\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/runtime-read-field-guide.json\"");
    assert_contains(index, "\"sample_type\":\"field_guide\"");
    assert_contains(index, "\"explains\":[\"INFOJSON fields\",\"STATSJSON fields\","
                           "\"read-only smoke semantics\"]");
    assert_contains(index, "\"no write command executed\"");
    assert_contains(index, "INFOJSON and STATSJSON carry their own read-only diagnostics");
    assert_contains(index, "field guide is explanatory evidence, not a runtime endpoint");
    assert_contains(index, "external control planes must treat samples as shape evidence, not production pass evidence");
    assert_contains(index,
                    "runtime-read-field-guide.json explains dynamic fields and read-only smoke interpretation for Node v159");

    assert_contains(field_guide, "\"guide_version\": \"mini-kv-runtime-read-field-guide.v1\"");
    assert_contains(field_guide, "\"consumer_hint\": \"Node v159 read-only capture release evidence review\"");
    assert_contains(field_guide, "\"command\": \"INFOJSON\"");
    assert_contains(field_guide, "\"evidence_type\": \"runtime_identity\"");
    assert_contains(field_guide, "\"path\": \"server.uptime_seconds\"");
    assert_contains(field_guide, "\"comparison_rule\": \"Check type and presence; do not compare exact fixture value in live capture.\"");
    assert_contains(field_guide, "\"command\": \"STATSJSON\"");
    assert_contains(field_guide, "\"evidence_type\": \"runtime_metrics\"");
    assert_contains(field_guide, "\"path\": \"commands.total_latency_ns\"");
    assert_contains(field_guide, "\"allowed_commands\": [");
    assert_contains(field_guide, "\"CHECKJSON GET orderops:1\"");
    assert_contains(field_guide, "\"write_commands_not_used\": [");
    assert_contains(field_guide, "\"SET\"");
    assert_contains(field_guide, "\"DEL\"");
    assert_contains(field_guide, "\"EXPIRE\"");
    assert_contains(field_guide, "Skipped or mixed capture evidence is not a production pass.");
    assert_contains(field_guide, "not a new server endpoint");

    minikv::Store check_store;
    minikv::CommandProcessor check_processor{check_store};
    auto result = check_processor.execute("CHECKJSON SET orderops:1 value");
    assert(result.response == checkjson_set_fixture);
    result = check_processor.execute("CHECKJSON GET orderops:1");
    assert(result.response == checkjson_get_fixture);

    minikv::CommandProcessorOptions info_options;
    info_options.runtime_info.started_at = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    minikv::Store info_store;
    minikv::CommandProcessor info_processor{info_store, nullptr, info_options};
    result = info_processor.execute("INFOJSON");
    assert(result.response == infojson_fixture);
    assert_contains(result.response, "\"schema_version\":1");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_identity\"");
    assert_contains(result.response, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(result.response, "\"uptime_seconds\":0");
    assert_contains(result.response, "\"live_keys\":0");
    assert_contains(result.response, "\"wal\":{\"enabled\":false}");
    assert_contains(result.response, "\"metrics\":{\"enabled\":false}");
    assert_contains(result.response, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/80/\"");
    assert_contains(result.response, "\"no_restore_proof\":true");
    assert_contains(result.response, "\"upload_allowed\":false");
    assert_contains(result.response, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_contains(result.response, "\"artifact_root\":\"c/\"");
    assert_contains(result.response, "\"artifact_path_hint\":\"c/81/\"");
    assert_contains(result.response, "\"retention_days\":30");
    assert_contains(result.response, "\"github_artifact_upload_attempted\":false");
    assert_contains(result.response, "\"production_window_allowed\":false");
    assert_contains(result.response, "\"diagnostics\":{\"write_commands_executed\":false,"
                                     "\"dynamic_fields\":[\"server.uptime_seconds\"]}");

    minikv::Store stats_store;
    minikv::CommandProcessor stats_processor{stats_store};
    result = stats_processor.execute("STATSJSON");
    assert(result.response == statsjson_fixture);
    assert_contains(result.response, "\"schema_version\":1");
    assert_contains(result.response, "\"read_only\":true");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"order_authoritative\":false");
    assert_contains(result.response, "\"evidence_type\":\"runtime_metrics\"");
    assert_contains(result.response, "\"live_keys\":0");
    assert_contains(result.response, "\"wal_enabled\":false");
    assert_contains(result.response, "\"wal\":null");
    assert_contains(result.response, "\"total_commands\":0");
    assert_contains(result.response, "\"breakdown\":[]");
    assert_contains(result.response, "\"connection_stats\":{\"available\":false}");
    assert_contains(result.response, "\"diagnostics\":{\"write_commands_executed\":false,"
                                     "\"dynamic_fields\":[\"commands.total_latency_ns\","
                                     "\"commands.avg_latency_ns\",\"commands.max_latency_ns\","
                                     "\"commands.breakdown[*].*_latency_ns\"]}");

    return 0;
}
