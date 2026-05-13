#include "minikv/command.hpp"
#include "minikv/store.hpp"

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
    assert_contains(index, "\"consumer_hint\":\"Node v152/v153 production pass evidence preparation\"");
    assert_contains(index, "\"fixture_count\":4");
    assert(count_occurrences(index, "\"path\":") == 4);
    assert_contains(index, "\"command\":\"CHECKJSON SET orderops:1 value\"");
    assert_contains(index, "\"path\":\"fixtures/checkjson/set-orderops-write-contract.json\"");
    assert_contains(index, "\"command\":\"CHECKJSON GET orderops:1\"");
    assert_contains(index, "\"path\":\"fixtures/checkjson/get-orderops-read-contract.json\"");
    assert_contains(index, "\"command\":\"INFOJSON\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/infojson-empty-inline.json\"");
    assert_contains(index, "\"dynamic_fields\":[\"server.uptime_seconds\"]");
    assert_contains(index, "\"command\":\"STATSJSON\"");
    assert_contains(index, "\"path\":\"fixtures/readonly/statsjson-empty-inline.json\"");
    assert_contains(index, "\"no write command executed\"");
    assert_contains(index, "external control planes must treat samples as shape evidence, not production pass evidence");

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
    assert_contains(result.response, "\"version\":\"0.58.0\"");
    assert_contains(result.response, "\"uptime_seconds\":0");
    assert_contains(result.response, "\"live_keys\":0");
    assert_contains(result.response, "\"wal\":{\"enabled\":false}");
    assert_contains(result.response, "\"metrics\":{\"enabled\":false}");

    minikv::Store stats_store;
    minikv::CommandProcessor stats_processor{stats_store};
    result = stats_processor.execute("STATSJSON");
    assert(result.response == statsjson_fixture);
    assert_contains(result.response, "\"live_keys\":0");
    assert_contains(result.response, "\"wal_enabled\":false");
    assert_contains(result.response, "\"wal\":null");
    assert_contains(result.response, "\"total_commands\":0");
    assert_contains(result.response, "\"breakdown\":[]");
    assert_contains(result.response, "\"connection_stats\":{\"available\":false}");

    return 0;
}
