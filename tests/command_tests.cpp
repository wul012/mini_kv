#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"
#include "minikv/wal.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

namespace {

const minikv::CommandBreakdownMetrics* find_command_metrics(const minikv::CommandProcessorMetrics& metrics,
                                                            std::string_view command) {
    for (const auto& command_metrics : metrics.command_breakdown) {
        if (command_metrics.command == command) {
            return &command_metrics;
        }
    }
    return nullptr;
}

void assert_response_contains(const minikv::CommandResult& result, std::string_view expected) {
    if (result.response.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(result.response.find(std::string{expected}) != std::string::npos);
}

std::string extract_json_string_field(const std::string& json, std::string_view field) {
    const std::string marker = "\"" + std::string{field} + "\":\"";
    const auto start = json.find(marker);
    assert(start != std::string::npos);
    const auto value_start = start + marker.size();
    const auto value_end = json.find('"', value_start);
    assert(value_end != std::string::npos);
    return json.substr(value_start, value_end - value_start);
}

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

} // namespace

int main() {
    using namespace std::chrono_literals;

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("");
    assert(result.response.empty());
    assert(!result.should_close);

    result = processor.execute("PING");
    assert(result.response == "PONG");
    assert(!result.should_close);

    result = processor.execute("PING hello from ping");
    assert(result.response == "hello from ping");
    assert(!result.should_close);

    result = processor.execute("SET name mini-kv");
    assert(result.response == "OK inserted");
    assert(!result.should_close);

    result = processor.execute("GET name");
    assert(result.response == "mini-kv");

    result = processor.execute("set name redis-ish");
    assert(result.response == "OK updated");

    result = processor.execute("GET name");
    assert(result.response == "redis-ish");

    result = processor.execute("SIZE");
    assert(result.response == "1");

    result = processor.execute("SET alpha first");
    assert(result.response == "OK inserted");

    result = processor.execute("KEYS");
    assert(result.response == "key_count=2 keys=alpha name");

    result = processor.execute("SET alpine mountain");
    assert(result.response == "OK inserted");

    result = processor.execute("KEYS alp");
    assert(result.response == "key_count=2 prefix=alp keys=alpha alpine");

    result = processor.execute("KEYS z");
    assert(result.response == "key_count=0 prefix=z keys=");

    result = processor.execute("KEYS alp extra");
    assert(result.response == "ERR usage: KEYS [prefix]");

    result = processor.execute("KEYSJSON extra more");
    assert(result.response == "ERR usage: KEYSJSON [prefix]");

    result = processor.execute("KEYSJSON");
    assert(result.response == "{\"prefix\":null,\"key_count\":3,\"keys\":[\"alpha\",\"alpine\",\"name\"],\"truncated\":false,\"limit\":1000}");

    result = processor.execute("KEYSJSON alp");
    assert(result.response == "{\"prefix\":\"alp\",\"key_count\":2,\"keys\":[\"alpha\",\"alpine\"],\"truncated\":false,\"limit\":1000}");

    result = processor.execute("KEYSJSON z");
    assert(result.response == "{\"prefix\":\"z\",\"key_count\":0,\"keys\":[],\"truncated\":false,\"limit\":1000}");

    result = processor.execute("DEL alpha");
    assert(result.response == "1");

    result = processor.execute("DEL alpine");
    assert(result.response == "1");

    result = processor.execute("DEL name");
    assert(result.response == "1");

    result = processor.execute("GET name");
    assert(result.response == "(nil)");

    result = processor.execute("SET temp keep");
    assert(result.response == "OK inserted");

    result = processor.execute("TTL temp");
    assert(result.response == "-1");

    result = processor.execute("EXPIRE temp 1");
    assert(result.response == "1");

    result = processor.execute("TTL temp");
    const long long remaining = std::stoll(result.response);
    assert(remaining >= 0);
    assert(remaining <= 1);

    std::this_thread::sleep_for(1100ms);

    result = processor.execute("GET temp");
    assert(result.response == "(nil)");

    result = processor.execute("TTL temp");
    assert(result.response == "-2");

    result = processor.execute("EXPIRE temp 1");
    assert(result.response == "0");

    result = processor.execute("EXPIRE temp 0");
    assert(result.response == "ERR usage: EXPIRE key seconds");

    result = processor.execute("TTL temp extra");
    assert(result.response == "ERR usage: TTL key");

    result = processor.execute("SETNXEX token 1 first");
    assert(result.response == "1");

    result = processor.execute("SETNXEX token 1 duplicate");
    assert(result.response == "0");

    result = processor.execute("GET token");
    assert(result.response == "first");

    result = processor.execute("TTL token");
    const long long token_remaining = std::stoll(result.response);
    assert(token_remaining >= 0);
    assert(token_remaining <= 1);

    std::this_thread::sleep_for(1100ms);

    result = processor.execute("GET token");
    assert(result.response == "(nil)");

    result = processor.execute("SETNXEX token 1 second");
    assert(result.response == "1");

    result = processor.execute("GET token");
    assert(result.response == "second");

    result = processor.execute("SETNXEX token 0 bad");
    assert(result.response == "ERR usage: SETNXEX key seconds value");

    result = processor.execute("SETNXEX token 1");
    assert(result.response == "ERR usage: SETNXEX key seconds value");

    const auto snapshot_path = std::filesystem::path{"minikv-command-snapshot-test.snap"};
    std::filesystem::remove(snapshot_path);

    store.clear();

    result = processor.execute("SET snap saved value");
    assert(result.response == "OK inserted");

    result = processor.execute(std::string{"SAVE "} + snapshot_path.string());
    assert(result.response == "OK saved 1");

    result = processor.execute("DEL snap");
    assert(result.response == "1");

    result = processor.execute(std::string{"LOAD "} + snapshot_path.string());
    assert(result.response == "OK loaded 1");

    result = processor.execute("GET snap");
    assert(result.response == "saved value");

    std::filesystem::remove(snapshot_path);

    result = processor.execute("COMPACT now");
    assert(result.response == "ERR usage: COMPACT");

    result = processor.execute("COMPACT");
    assert(result.response == "ERR WAL not enabled");

    result = processor.execute("WALINFO extra");
    assert(result.response == "ERR usage: WALINFO");

    result = processor.execute("WALINFO");
    assert(result.response == "ERR WAL not enabled");

    result = processor.execute("STATS extra");
    assert(result.response == "ERR usage: STATS");

    result = processor.execute("STATS");
    assert(result.response.find("live_keys=1") != std::string::npos);
    assert(result.response.find("wal_enabled=no") != std::string::npos);
    assert(result.response.find("connection_stats_available=no") != std::string::npos);

    result = processor.execute("HEALTH extra");
    assert(result.response == "ERR usage: HEALTH");

    result = processor.execute("HEALTH");
    assert(result.response.find("OK live_keys=1") != std::string::npos);
    assert(result.response.find("wal_enabled=no") != std::string::npos);
    assert(result.response.find("compact_recommended=na") != std::string::npos);
    assert(result.response.find("connection_stats_available=no") != std::string::npos);

    result = processor.execute("INFO extra");
    assert(result.response == "ERR usage: INFO");

    result = processor.execute("INFO");
    assert(result.response.find("version=" + std::string{minikv::version}) != std::string::npos);
    assert(result.response.find("protocol=inline") != std::string::npos);
    assert(result.response.find("uptime_seconds=") != std::string::npos);
    assert(result.response.find("live_keys=1") != std::string::npos);
    assert(result.response.find("wal_enabled=no") != std::string::npos);
    assert(result.response.find("metrics_enabled=no") != std::string::npos);
    assert(result.response.find("max_request_bytes=0") != std::string::npos);

    result = processor.execute("INFOJSON extra");
    assert(result.response == "ERR usage: INFOJSON");

    result = processor.execute("INFOJSON");
    assert(result.response.find("\"schema_version\":1") != std::string::npos);
    assert(result.response.find("\"read_only\":true") != std::string::npos);
    assert(result.response.find("\"execution_allowed\":false") != std::string::npos);
    assert(result.response.find("\"order_authoritative\":false") != std::string::npos);
    assert(result.response.find("\"evidence_type\":\"runtime_identity\"") != std::string::npos);
    assert(result.response.find("\"version\":\"" + std::string{minikv::version} + "\"") != std::string::npos);
    assert(result.response.find("\"server\":{\"protocol\":[\"inline\"]") != std::string::npos);
    assert(result.response.find("\"uptime_seconds\":") != std::string::npos);
    assert(result.response.find("\"max_request_bytes\":0") != std::string::npos);
    assert(result.response.find("\"store\":{\"live_keys\":1}") != std::string::npos);
    assert(result.response.find("\"wal\":{\"enabled\":false}") != std::string::npos);
    assert(result.response.find("\"metrics\":{\"enabled\":false}") != std::string::npos);
    assert_response_contains(result, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/80/\"");
    assert_response_contains(result, "\"no_restore_proof\":true");
    assert_response_contains(result, "\"upload_allowed\":false");
    assert_response_contains(result, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_response_contains(result, "\"artifact_root\":\"c/\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/81/\"");
    assert_response_contains(result, "\"retention_days\":30");
    assert_response_contains(result, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_response_contains(result, "\"github_artifact_upload_attempted\":false");
    assert_response_contains(result, "\"production_window_allowed\":false");
    assert(result.response.find("\"diagnostics\":{\"write_commands_executed\":false") != std::string::npos);

    result = processor.execute("COMMANDS extra");
    assert(result.response == "ERR usage: COMMANDS");

    result = processor.execute("COMMANDS");
    assert(result.response.find("command_count=29") != std::string::npos);
    assert(result.response.find("PING(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SET(category=write,mutates_store=yes,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("SETNXEX(category=write,mutates_store=yes,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("GET(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("KEYSJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("COMPACT(category=admin,mutates_store=no,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("COMMANDSJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("EXPLAINJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("CHECKJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SMOKEJSON(category=meta,mutates_store=no,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("STORAGEJSON(category=read,mutates_store=no,touches_wal=yes,stable=yes)") != std::string::npos);

    result = processor.execute("COMMANDSJSON extra");
    assert(result.response == "ERR usage: COMMANDSJSON");

    result = processor.execute("COMMANDSJSON");
    assert(result.response.find("\"commands\":[") != std::string::npos);
    assert(result.response.find("\"name\":\"PING\",\"category\":\"meta\",\"mutates_store\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SET\",\"category\":\"write\",\"mutates_store\":true,\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"SETNXEX\",\"category\":\"write\",\"mutates_store\":true,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"GET\",\"category\":\"read\",\"mutates_store\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"KEYSJSON\",\"category\":\"read\",\"mutates_store\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"LOAD\",\"category\":\"admin\",\"mutates_store\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"COMMANDSJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"EXPLAINJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"CHECKJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"SMOKEJSON\",\"category\":\"meta\",\"mutates_store\":false,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"STORAGEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"description\":\"Read command catalog as JSON\"") != std::string::npos);

    result = processor.execute("EXPLAINJSON");
    assert(result.response == "ERR usage: EXPLAINJSON command");

    result = processor.execute("EXPLAINJSON SET orderops:1 value");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"command_digest\":\"fnv1a64:");
    const auto set_digest = extract_json_string_field(result.response, "command_digest");
    assert(set_digest.size() == 24);
    assert_response_contains(result, "\"command\":\"SET\"");
    assert_response_contains(result, "\"category\":\"write\"");
    assert_response_contains(result, "\"mutates_store\":true");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"key\":\"orderops:1\"");
    assert_response_contains(result, "\"requires_value\":true");
    assert_response_contains(result, "\"ttl_sensitive\":false");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");
    assert_response_contains(result, "\"warnings\":[]");

    const auto repeated_set = processor.execute("EXPLAINJSON SET orderops:1 value");
    assert(extract_json_string_field(repeated_set.response, "command_digest") == set_digest);

    result = processor.execute("GET orderops:1");
    assert(result.response == "(nil)");

    result = processor.execute("EXPLAINJSON GET orderops:1");
    assert(extract_json_string_field(result.response, "command_digest") != set_digest);
    const auto get_digest = extract_json_string_field(result.response, "command_digest");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"command\":\"GET\"");
    assert_response_contains(result, "\"category\":\"read\"");
    assert_response_contains(result, "\"mutates_store\":false");
    assert_response_contains(result, "\"touches_wal\":false");
    assert_response_contains(result, "\"key\":\"orderops:1\"");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON EXPIRE orderops:1 60");
    assert_response_contains(result, "\"command\":\"EXPIRE\"");
    assert_response_contains(result, "\"ttl_sensitive\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_ttl_update\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");

    result = processor.execute("EXPLAINJSON SETNXEX orderops:token 30 value");
    assert_response_contains(result, "\"command\":\"SETNXEX\"");
    assert_response_contains(result, "\"category\":\"write\"");
    assert_response_contains(result, "\"mutates_store\":true");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"key\":\"orderops:token\"");
    assert_response_contains(result, "\"requires_value\":true");
    assert_response_contains(result, "\"ttl_sensitive\":true");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"store_ttl_update\","
                                     "\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");

    result = processor.execute("EXPLAINJSON GET orderops:1 extra");
    assert_response_contains(result, "\"command\":\"GET\"");
    assert_response_contains(result, "\"allowed_by_parser\":false");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");
    assert_response_contains(result, "\"warnings\":[\"usage: GET key\"]");

    result = processor.execute("EXPLAINJSON NOPE orderops:1");
    assert_response_contains(result, "\"command\":\"NOPE\"");
    assert_response_contains(result, "\"category\":\"unknown\"");
    assert_response_contains(result, "\"allowed_by_parser\":false");
    assert_response_contains(result, "\"side_effects\":[]");
    assert_response_contains(result, "\"side_effect_count\":0");
    assert_response_contains(result, "\"warnings\":[\"unknown command\"]");

    result = processor.execute("EXPLAINJSON TTL orderops:1");
    assert_response_contains(result, "\"command\":\"TTL\"");
    assert_response_contains(result, "\"ttl_sensitive\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON DEL orderops:1");
    assert_response_contains(result, "\"command\":\"DEL\"");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");

    result = processor.execute("EXPLAINJSON SAVE data/snap.txt");
    assert_response_contains(result, "\"command\":\"SAVE\"");
    assert_response_contains(result, "\"category\":\"admin\"");
    assert_response_contains(result, "\"side_effects\":[\"snapshot_file_write\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON LOAD data/snap.txt");
    assert_response_contains(result, "\"command\":\"LOAD\"");
    assert_response_contains(result, "\"mutates_store\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_replace_from_snapshot\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON COMPACT");
    assert_response_contains(result, "\"command\":\"COMPACT\"");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"side_effects\":[\"wal_rewrite_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON RESETSTATS");
    assert_response_contains(result, "\"command\":\"RESETSTATS\"");
    assert_response_contains(result, "\"side_effects\":[\"metrics_reset\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON INFO");
    assert_response_contains(result, "\"command\":\"INFO\"");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON STORAGEJSON");
    assert_response_contains(result, "\"command\":\"STORAGEJSON\"");
    assert_response_contains(result, "\"category\":\"read\"");
    assert_response_contains(result, "\"mutates_store\":false");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\",\"store_read\","
                                     "\"wal_metadata_read_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");

    result = processor.execute("EXPLAINJSON SMOKEJSON");
    assert_response_contains(result, "\"command\":\"SMOKEJSON\"");
    assert_response_contains(result, "\"category\":\"meta\"");
    assert_response_contains(result, "\"mutates_store\":false");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\",\"store_read\","
                                     "\"wal_metadata_read_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");

    result = processor.execute("EXPLAINJSON QUIT");
    assert_response_contains(result, "\"command\":\"QUIT\"");
    assert_response_contains(result, "\"side_effects\":[\"connection_close\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("CHECKJSON");
    assert(result.response == "ERR usage: CHECKJSON command");

    result = processor.execute("CHECKJSON SET orderops:1 value");
    const auto checkjson_set_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "set-orderops-write-contract.json");
    assert(result.response == checkjson_set_fixture);
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"execution_allowed\":false");
    assert_response_contains(result, "\"command_digest\":\"" + set_digest + "\"");
    assert_response_contains(result, "\"command\":\"SET\"");
    assert_response_contains(result, "\"write_command\":true");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");
    assert_response_contains(result, "\"checks\":{\"parser_allowed\":true,\"write_command\":true,"
                                     "\"wal_append_when_enabled\":true,\"wal_enabled\":false}");
    assert_response_contains(result, "\"wal\":{\"enabled\":false,\"touches_wal\":true,"
                                     "\"append_when_enabled\":true,\"durability\":\"memory_only\"}");
    assert_response_contains(result, "\"warnings\":[\"wal disabled; write would be in-memory only\"]");

    result = processor.execute("CHECKJSON SETNXEX orderops:token 30 value");
    assert_response_contains(result, "\"command\":\"SETNXEX\"");
    assert_response_contains(result, "\"write_command\":true");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"store_ttl_update\","
                                     "\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");
    assert_response_contains(result, "\"checks\":{\"parser_allowed\":true,\"write_command\":true,"
                                     "\"wal_append_when_enabled\":true,\"wal_enabled\":false}");
    assert_response_contains(result, "\"durability\":\"memory_only\"");
    assert_response_contains(result, "\"warnings\":[\"wal disabled; write would be in-memory only\"]");

    result = processor.execute("GET orderops:1");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON GET orderops:1");
    const auto checkjson_get_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "get-orderops-read-contract.json");
    const auto ttl_token_index =
        read_fixture_text(std::filesystem::path{"fixtures"} / "ttl-token" / "index.json");

    assert_response_contains({ttl_token_index}, "\"index_version\":\"mini-kv-ttl-token-fixtures.v1\"");
    assert_response_contains({ttl_token_index}, "\"command\":\"SETNXEX key seconds value\"");
    assert_response_contains({ttl_token_index}, "\"order_authoritative\":false");
    assert_response_contains({ttl_token_index}, "\"consumer_hint\":\"Node v160 idempotency vertical readiness review\"");
    assert_response_contains({ttl_token_index}, "\"atomic_absent_claim\":true");
    assert_response_contains({ttl_token_index}, "\"enabled_record\":\"SETEXAT key epoch_millis value\"");
    assert_response_contains({ttl_token_index}, "\"single_record_claim\":true");
    assert_response_contains({ttl_token_index}, "\"replay_drops_expired_token\":true");
    assert_response_contains({ttl_token_index}, "not connected to Java transaction chain");
    assert_response_contains({ttl_token_index}, "CHECKJSON SETNXEX can inspect the contract without executing the write");
    assert(result.response == checkjson_get_fixture);
    assert_response_contains(result, "\"command_digest\":\"" + get_digest + "\"");
    assert_response_contains(result, "\"command\":\"GET\"");
    assert_response_contains(result, "\"write_command\":false");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");
    assert_response_contains(result, "\"checks\":{\"parser_allowed\":true,\"write_command\":false,"
                                     "\"wal_append_when_enabled\":false,\"wal_enabled\":false}");
    assert_response_contains(result, "\"durability\":\"not_applicable\"");
    assert_response_contains(result, "\"warnings\":[\"not a write command\"]");

    result = processor.execute("STORAGEJSON extra");
    assert(result.response == "ERR usage: STORAGEJSON");

    result = processor.execute("STORAGEJSON");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"execution_allowed\":false");
    assert_response_contains(result, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_response_contains(result, "\"store\":{\"live_keys\":1,\"order_authoritative\":false}");
    assert_response_contains(result, "\"wal\":{\"enabled\":false,\"status\":\"disabled\"}");
    assert_response_contains(result, "\"snapshot\":{\"supported\":true,\"mode\":\"manual\",\"atomic_save\":true,"
                                     "\"load_replaces_store\":true}");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\",\"store_read\","
                                     "\"wal_metadata_read_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");
    assert_response_contains(result, "\"diagnostics\":{\"read_only_command\":true,\"write_commands_executed\":false,"
                                     "\"order_authoritative\":false");
    assert_response_contains(result, "\"notes\":[\"read_only_storage_evidence\",\"not_order_authoritative\","
                                     "\"manual_snapshot_only\",\"wal_disabled\"]");

    result = processor.execute("SMOKEJSON extra");
    assert(result.response == "ERR usage: SMOKEJSON");

    result = processor.execute("SMOKEJSON");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"execution_allowed\":false");
    assert_response_contains(result, "\"restore_execution_allowed\":false");
    assert_response_contains(result, "\"order_authoritative\":false");
    assert_response_contains(result, "\"evidence_type\":\"runtime_smoke\"");
    assert_response_contains(result, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_response_contains(result, "\"server\":{\"protocol\":[\"inline\"]");
    assert_response_contains(result, "\"metrics_enabled\":false");
    assert_response_contains(result, "\"store\":{\"live_keys\":1,\"order_authoritative\":false}");
    assert_response_contains(result, "\"wal\":{\"enabled\":false,\"status\":\"disabled\","
                                     "\"compact_recommended\":false}");
    assert_response_contains(result, "\"connection_stats\":{\"available\":false}");
    assert_response_contains(result, "\"real_read\":{\"allowed\":true,\"commands\":[\"INFOJSON\","
                                     "\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_response_contains(result, "\"forbidden_commands\":[\"LOAD\",\"COMPACT\",\"SETNXEX\",\"RESTORE\"]");
    assert_response_contains(result, "\"write_commands_executed\":false");
    assert_response_contains(result, "\"admin_commands_executed\":false");
    assert_response_contains(result, "\"runtime_write_observed\":false");
    assert_response_contains(result, "\"live_read_session\":{\"consumer\":\"Node v205 three-project real-read runtime smoke execution packet\"");
    assert_response_contains(result, "\"session_id_echo\":\"mini-kv-live-read-v102\"");
    assert_response_contains(result, "\"server_uptime_bucket\":\"lt_60s\"");
    assert_response_contains(result, "\"read_command_list_digest\":\"fnv1a64:5bef33f2fbe65cc5\"");
    assert_response_contains(result, "\"read_command_count\":4");
    assert_response_contains(result, "\"read_commands\":[\"INFOJSON\",\"STORAGEJSON\",\"HEALTH\",\"STATSJSON\"]");
    assert_response_contains(result, "\"write_commands_allowed\":false");
    assert_response_contains(result, "\"auto_start_allowed\":false");
    assert_response_contains(result, "\"operator_window\":{\"consumer\":\"Node v200 real-read window CI archive artifact manifest\"");
    assert_response_contains(result, "\"identity_neutral_proof\":true");
    assert_response_contains(result, "\"node_action\":\"verify digest before importing manual window results\"}");
    assert_response_contains(result, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/80/\"");
    assert_response_contains(result, "\"no_restore_proof\":true");
    assert_response_contains(result, "\"upload_allowed\":false");
    assert_response_contains(result, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_response_contains(result, "\"artifact_root\":\"c/\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/81/\"");
    assert_response_contains(result, "\"retention_days\":30");
    assert_response_contains(result, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_response_contains(result, "\"github_artifact_upload_attempted\":false");
    assert_response_contains(result, "\"production_window_allowed\":false");
    assert_response_contains(result, "\"binary_provenance\":{\"consumer\":\"Node v208 managed audit persistence boundary candidate\"");
    assert_response_contains(result, "\"source_version\":\"0.102.0\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"runtime_binary_hint\":\"cmake-build-v102/minikv_server and cmake-build-v102/minikv_client from the current CMake build\"");
    assert_response_contains(result, "\"release_manifest_path\":\"fixtures/release/verification-manifest.json\"");
    assert_response_contains(result, "\"runtime_smoke_evidence_path\":\"fixtures/release/runtime-smoke-evidence.json\"");
    assert_response_contains(result, "\"provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"load_restore_compact_executed\":false");
    assert_response_contains(result, "\"production_binary_claimed\":false");
    assert_response_contains(result, "\"node_action\":\"verify binary, fixture, and release evidence path alignment before managed audit persistence boundary work\"");
    assert_response_contains(result, "\"retention_provenance_check\":{\"consumer\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_response_contains(result, "\"retention_source_path_hint\":\"c/81/\"");
    assert_response_contains(result, "\"provenance_source\":\"binary_provenance.provenance_digest\"");
    assert_response_contains(result, "\"expected_binary_provenance_digest\":\"fnv1a64:d9776044d393ecbc\"");
    assert_response_contains(result, "\"check_digest\":\"fnv1a64:8cf7cc0df218035f\"");
    assert_response_contains(result, "\"managed_audit_write_executed\":false");
    assert_response_contains(result, "\"node_action\":\"verify runtime evidence retention and binary provenance alignment before managed audit dry-run packet work\"");
    assert_response_contains(result, "\"retention_provenance_replay_marker\":{\"consumer\":\"Node v213 managed audit packet restore drill plan\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v211 managed audit identity approval provenance dry-run packet\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v84\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/84/\"");
    assert_response_contains(result, "\"consumed_check_digest\":\"fnv1a64:357cc7e9eec3f223\"");
    assert_response_contains(result, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"marker_digest\":\"fnv1a64:6b9ef93f40cbd343\"");
    assert_response_contains(result, "\"replay_executed\":false");
    assert_response_contains(result, "\"node_action\":\"verify v84 retention provenance consumption before managed audit packet restore drill planning\"");
    assert_response_contains(result, "\"managed_audit_adapter_restore_boundary_receipt\":{\"consumer\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v213 managed audit packet restore drill plan\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v85\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/85/\"");
    assert_response_contains(result, "\"consumed_marker_digest\":\"fnv1a64:1ea4570c967cfdb1\"");
    assert_response_contains(result, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:1177efbaceb52c1c\"");
    assert_response_contains(result, "\"adapter_write_allowed\":false");
    assert_response_contains(result, "\"restore_execution_allowed\":false");
    assert_response_contains(result, "\"load_restore_compact_executed\":false");
    assert_response_contains(result, "\"order_authoritative\":false");
    assert_response_contains(result, "\"boundary\":\"mini-kv remains a read-only evidence provider for managed audit adapter preparation\"");
    assert_response_contains(result, "\"managed_audit_adapter_non_authoritative_storage_receipt\":{\"consumer\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v215 managed audit dry-run adapter candidate\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v86\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/86/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:f39d8e3ef98654ea\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:667590b83f510a58\"");
    assert_response_contains(result, "\"managed_audit_store\":false");
    assert_response_contains(result, "\"storage_write_allowed\":false");
    assert_response_contains(result, "\"admin_commands_allowed\":false");
    assert_response_contains(result, "\"boundary\":\"mini-kv is non-authoritative read-only storage evidence, not the managed audit store\"");
    assert_response_contains(result, "\"command_dispatch_quality_receipt\":{\"consumer\":\"Node v219 managed audit adapter implementation precheck packet\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v217 managed audit adapter production-hardening readiness gate\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v87\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/87/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:111f0daf1283eab6\"");
    assert_response_contains(result, "\"dispatch_family\":\"runtime_evidence_command_family\"");
    assert_response_contains(result, "\"split_commands\":\"STATS,STATSJSON,SMOKEJSON,STORAGEJSON,HEALTH,INFO,INFOJSON,COMMANDS,COMMANDSJSON\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:0f2caa6931b482b8\"");
    assert_response_contains(result, "\"dispatch_split_applied\":true");
    assert_response_contains(result, "\"handler_table_required\":false");
    assert_response_contains(result, "\"write_handler_changed\":false");
    assert_response_contains(result, "\"admin_handler_changed\":false");
    assert_response_contains(result, "\"wal_snapshot_restore_touched\":false");
    assert_response_contains(result, "\"behavior_changed\":false");
    assert_response_contains(result, "\"fixture_contract_preserved\":true");
    assert_response_contains(result, "\"boundary\":\"command dispatch quality receipt only; no write, admin, WAL, snapshot, restore, or managed audit storage behavior change\"");
    assert_response_contains(result, "\"adapter_shell_non_storage_guard_receipt\":{\"consumer\":\"Node v221 managed audit local adapter candidate dry-run\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v220 managed audit adapter interface and disabled shell\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v88\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/88/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:4aa6d12fb067e2a6\"");
    assert_response_contains(result, "\"adapter_shell\":\"ManagedAuditAdapter disabled shell\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:0174fa831d17cea4\"");
    assert_response_contains(result, "\"adapter_shell_storage_backend\":false");
    assert_response_contains(result, "\"storage_backend_allowed\":false");
    assert_response_contains(result, "\"local_dry_run_records_written\":false");
    assert_response_contains(result, "\"boundary\":\"adapter shell non-storage guard receipt only; mini-kv remains runtime evidence and is not a managed audit adapter storage backend\"");
    assert_response_contains(result, "\"managed_audit_external_adapter_non_participation_receipt\":{\"consumer\":\"Node v223 managed audit external adapter connection readiness review\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v222 managed audit local adapter candidate verification report\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v89\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/89/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:76411286a0913dc8\"");
    assert_response_contains(result, "\"adapter_target\":\"real external managed audit adapter\"");
    assert_response_contains(result, "\"runtime_role\":\"runtime evidence provider only\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:9bacde73d6d07097\"");
    assert_response_contains(result, "\"external_adapter_storage_backend\":false");
    assert_response_contains(result, "\"participates_in_external_adapter\":false");
    assert_response_contains(result, "\"credential_required\":false");
    assert_response_contains(result, "\"credential_read_allowed\":false");
    assert_response_contains(result, "\"migration_required\":false");
    assert_response_contains(result, "\"migration_execution_allowed\":false");
    assert_response_contains(result, "\"schema_authoritative\":false");
    assert_response_contains(result, "\"boundary\":\"external adapter non-participation receipt only; mini-kv is not an external managed audit adapter storage backend and keeps runtime evidence provider role\"");
    assert_response_contains(result, "\"managed_audit_sandbox_adapter_non_participation_receipt\":{\"consumer\":\"Node v225 managed audit sandbox adapter dry-run package\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v224 managed audit sandbox adapter dry-run plan\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v90\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/90/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:0dfb07cd2f8de289\"");
    assert_response_contains(result, "\"adapter_target\":\"sandbox managed audit adapter dry-run\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:f0cae7a4ce0674c2\"");
    assert_response_contains(result, "\"sandbox_adapter_storage_backend\":false");
    assert_response_contains(result, "\"participates_in_sandbox_adapter\":false");
    assert_response_contains(result, "\"credential_value_required\":false");
    assert_response_contains(result, "\"credential_value_read_allowed\":false");
    assert_response_contains(result, "\"production_credential_read_allowed\":false");
    assert_response_contains(result, "\"schema_migration_execution_allowed\":false");
    assert_response_contains(result, "\"sandbox_schema_authoritative\":false");
    assert_response_contains(result, "\"sandbox_managed_audit_state_write_allowed\":false");
    assert_response_contains(result, "\"sandbox_dry_run_records_written\":false");
    assert_response_contains(result, "\"boundary\":\"sandbox adapter runtime evidence non-participation receipt only; mini-kv is not a sandbox audit storage backend and does not read credentials or write managed audit state\"");
    assert_response_contains(result, "\"managed_audit_sandbox_connection_receipt_echo_marker\":{\"consumer\":\"Node v229 manual sandbox connection packet verification\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v228 managed audit manual sandbox connection operator packet\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v95\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/95/\"");
    assert_response_contains(result, "\"consumed_receipt_digest\":\"fnv1a64:ceaed265f7f9560c\"");
    assert_response_contains(result, "\"source_operator_packet_version\":\"Node v228\"");
    assert_response_contains(result, "\"source_operator_packet_profile\":\"managed-audit-manual-sandbox-connection-operator-packet.v1\"");
    assert_response_contains(result, "\"packet_mode\":\"manual-sandbox-connection-operator-packet-only\"");
    assert_response_contains(result, "\"owner_approval_artifact_id_field\":\"ORDEROPS_MANAGED_AUDIT_OWNER_APPROVAL_ARTIFACT_ID\"");
    assert_response_contains(result, "\"credential_handle_name_field\":\"ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE\"");
    assert_response_contains(result, "\"schema_rehearsal_id_field\":\"ORDEROPS_MANAGED_AUDIT_SCHEMA_REHEARSAL_ID\"");
    assert_response_contains(result, "\"rollback_path_id_field\":\"ORDEROPS_MANAGED_AUDIT_ROLLBACK_PATH_ID\"");
    assert_response_contains(result, "\"timeout_budget_ms\":15000");
    assert_response_contains(result, "\"manual_abort_marker\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_ABORT\"");
    assert_response_contains(result, "\"required_operator_field_count\":6");
    assert_response_contains(result, "\"marker_digest\":\"fnv1a64:beb8dd6a0b102a11\"");
    assert_response_contains(result, "\"connection_execution_allowed\":false");
    assert_response_contains(result, "\"managed_audit_write_allowed\":false");
    assert_response_contains(result, "\"participates_in_sandbox_connection\":false");
    assert_response_contains(result, "\"node_auto_start_allowed\":false");
    assert_response_contains(result, "\"boundary\":\"sandbox connection receipt echo marker only; mini-kv echoes Node v228 operator packet field handles but remains outside managed audit connection, credential reads, schema migration, storage, and state writes\"");
    assert_response_contains(result, "\"managed_audit_sandbox_connection_no_start_guard_receipt\":{\"consumer\":\"Node v231 manual sandbox connection preflight verification\"");
    assert_response_contains(result, "\"consumed_by\":\"Node v230 manual sandbox connection preflight gate\"");
    assert_response_contains(result, "\"consumed_release_version\":\"v96\"");
    assert_response_contains(result, "\"consumed_artifact_path_hint\":\"c/96/\"");
    assert_response_contains(result, "\"consumed_marker_digest\":\"fnv1a64:b9fc556875ea625b\"");
    assert_response_contains(result, "\"current_artifact_path_hint\":\"c/102/\"");
    assert_response_contains(result, "\"preflight_gate_version\":\"Node v230\"");
    assert_response_contains(result, "\"manual_window_flag_name\":\"ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED\"");
    assert_response_contains(result, "\"manual_window_mode\":\"manual-window-required-no-auto-start\"");
    assert_response_contains(result, "\"receipt_digest\":\"fnv1a64:69e377d01f15fd57\"");
    assert_response_contains(result, "\"manual_window_open_by_default\":false");
    assert_response_contains(result, "\"java_auto_start_allowed\":false");
    assert_response_contains(result, "\"mini_kv_auto_start_allowed\":false");
    assert_response_contains(result, "\"schema_rehearsal_execution_allowed\":false");
    assert_response_contains(result, "\"failure_taxonomy\":{\"schema_version\":1");
    assert_response_contains(result, "\"consumer\":\"Node v196 imported window result packet\"");
    assert_response_contains(result, "\"taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_response_contains(result, "\"verification_sample\":{\"sample_version\":\"mini-kv-smoke-taxonomy-verification.v1\"");
    assert_response_contains(result, "\"source_command\":\"SMOKEJSON\",\"source_version\":\"0.102.0\"");
    assert_response_contains(result, "\"expected_taxonomy_digest\":\"fnv1a64:f92fcba55feb26a2\"");
    assert_response_contains(result, "\"node_action\":\"verify digest before importing manual window results\"");
    assert_response_contains(result, "\"id\":\"connection-refused\",\"source\":\"tcp_connect\"");
    assert_response_contains(result, "\"id\":\"invalid-json\",\"source\":\"SMOKEJSON_parse\"");
    assert_response_contains(result, "\"id\":\"read-command-failed\",\"source\":\"runtime_read_command\"");
    assert_response_contains(result, "\"id\":\"unsafe-surface\",\"source\":\"adapter_command_plan\"");
    assert_response_contains(result, "\"id\":\"unexpected-write-signal\",\"source\":\"runtime_smoke_diagnostics\"");
    assert_response_contains(result, "\"safe_to_auto_start\":false");
    assert_response_contains(result, "\"write_risk\":true");
    assert_response_contains(result, "\"node_consumption\":\"Node v246 may verify the mini-kv v108 manual sandbox connection precheck non-participation receipt");
    assert_response_contains(result, "Node v244 may still verify the mini-kv v107 manual sandbox dry-run command non-participation receipt");
    assert_response_contains(result, "Node v261 may verify the mini-kv v114 credential resolver non-participation receipt");
    assert_response_contains(result, "Node v259 may verify the mini-kv v113 sandbox endpoint handle non-participation receipt");
    assert_response_contains(result, "Node v239 may still verify the mini-kv operator window no-start/no-write receipt");
    assert_response_contains(result, "\"live_read_session_hint\"");
    assert_response_contains(result, "\"binary_provenance_hint\"");
    assert_response_contains(result, "\"retention_provenance_check\"");
    assert_response_contains(result, "\"retention_provenance_replay_marker\"");
    assert_response_contains(result, "\"managed_audit_adapter_restore_boundary_receipt\"");
    assert_response_contains(result, "\"managed_audit_adapter_non_authoritative_storage_receipt\"");
    assert_response_contains(result, "\"manual_sandbox_dry_run_command_non_participation_receipt\"");
    assert_response_contains(result, "\"manual_sandbox_connection_precheck_non_participation_receipt\"");
    assert_response_contains(result, "\"sandbox_endpoint_handle_non_participation_receipt\"");
    assert_response_contains(result, "\"credential_resolver_non_participation_receipt\"");
    assert_response_contains(result, "\"read_only_aggregate\"");
    assert_response_contains(result, "\"does_not_execute_load_compact_setnxex_or_restore\"");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON SET orderops:1");
    assert_response_contains(result, "\"command\":\"SET\"");
    assert_response_contains(result, "\"write_command\":true");
    assert_response_contains(result, "\"allowed_by_parser\":false");
    assert_response_contains(result, "\"warnings\":[\"usage: SET key value\","
                                     "\"wal disabled; write would be in-memory only\"]");

    const auto wal_contract_path = std::filesystem::path{"minikv-command-checkjson-test.wal"};
    std::filesystem::remove(wal_contract_path);
    minikv::Store wal_contract_store;
    minikv::WriteAheadLog wal_contract{wal_contract_path};
    minikv::CommandProcessor wal_contract_processor{wal_contract_store, &wal_contract};
    result = wal_contract_processor.execute("CHECKJSON SET orderops:1 value");
    assert_response_contains(result, "\"wal_enabled\":true");
    assert_response_contains(result, "\"wal\":{\"enabled\":true,\"touches_wal\":true,"
                                     "\"append_when_enabled\":true,\"durability\":\"wal_backed\"}");
    assert_response_contains(result, "\"warnings\":[]");

    result = wal_contract_processor.execute("STORAGEJSON");
    assert_response_contains(result, "\"wal\":{\"enabled\":true,\"status\":\"enabled\"");
    assert_response_contains(result, "\"records\":0");
    assert_response_contains(result, "\"live_keys\":0");
    assert_response_contains(result, "\"notes\":[\"read_only_storage_evidence\",\"not_order_authoritative\","
                                     "\"manual_snapshot_only\",\"wal_enabled\"]");
    assert(!std::filesystem::exists(wal_contract_path));
    std::filesystem::remove(wal_contract_path);

    minikv::Store inventory_store;
    minikv::CommandProcessor inventory_processor{inventory_store};
    for (int index = 0; index < 1002; ++index) {
        result = inventory_processor.execute("SET bulk:" + std::to_string(index) + " value");
        assert(result.response == "OK inserted");
    }
    result = inventory_processor.execute("KEYSJSON bulk:");
    assert(result.response.find("\"prefix\":\"bulk:\"") != std::string::npos);
    assert(result.response.find("\"key_count\":1000") != std::string::npos);
    assert(result.response.find("\"truncated\":true") != std::string::npos);
    assert(result.response.find("\"limit\":1000") != std::string::npos);

    minikv::CommandProcessorOptions stats_options;
    stats_options.connection_stats = [] {
        minikv::CommandConnectionStats stats;
        stats.available = true;
        stats.total_connections = 7;
        stats.active_connections = 2;
        stats.peak_connections = 3;
        return stats;
    };
    minikv::CommandProcessor stats_processor{store, nullptr, stats_options};

    result = stats_processor.execute("STATS");
    assert(result.response.find("connection_stats_available=yes") != std::string::npos);
    assert(result.response.find("active_connections=2") != std::string::npos);
    assert(result.response.find("total_connections=7") != std::string::npos);
    assert(result.response.find("peak_connections=3") != std::string::npos);

    result = stats_processor.execute("HEALTH");
    assert(result.response.find("connection_stats_available=yes") != std::string::npos);
    assert(result.response.find("active_connections=2") != std::string::npos);

    minikv::Store metrics_store;
    minikv::CommandProcessor metrics_processor{metrics_store};
    assert(metrics_processor.metrics().total_commands == 0);
    assert(metrics_processor.metrics().successful_commands == 0);
    assert(metrics_processor.metrics().error_commands == 0);
    assert(metrics_processor.metrics().total_latency_ns == 0);
    assert(metrics_processor.metrics().max_latency_ns == 0);
    assert(metrics_processor.metrics().command_breakdown.empty());

    result = metrics_processor.execute("");
    assert(result.response.empty());
    assert(metrics_processor.metrics().total_commands == 0);

    result = metrics_processor.execute("PING");
    assert(result.response == "PONG");
    assert(metrics_processor.metrics().total_commands == 1);
    assert(metrics_processor.metrics().successful_commands == 1);
    assert(metrics_processor.metrics().error_commands == 0);
    auto metrics = metrics_processor.metrics();
    assert(metrics.total_latency_ns > 0);
    assert(metrics.max_latency_ns > 0);
    const auto* ping_metrics = find_command_metrics(metrics, "PING");
    assert(ping_metrics != nullptr);
    assert(ping_metrics->total_commands == 1);
    assert(ping_metrics->successful_commands == 1);
    assert(ping_metrics->error_commands == 0);
    assert(ping_metrics->total_latency_ns > 0);
    assert(ping_metrics->max_latency_ns > 0);

    result = metrics_processor.execute("GET name extra");
    assert(result.response == "ERR usage: GET key");
    assert(metrics_processor.metrics().total_commands == 2);
    assert(metrics_processor.metrics().successful_commands == 1);
    assert(metrics_processor.metrics().error_commands == 1);
    metrics = metrics_processor.metrics();
    const auto* get_metrics = find_command_metrics(metrics, "GET");
    assert(get_metrics != nullptr);
    assert(get_metrics->total_commands == 1);
    assert(get_metrics->successful_commands == 0);
    assert(get_metrics->error_commands == 1);

    result = metrics_processor.execute("BADCOMMAND");
    assert(result.response == "ERR unknown command");
    assert(metrics_processor.metrics().total_commands == 3);
    assert(metrics_processor.metrics().successful_commands == 1);
    assert(metrics_processor.metrics().error_commands == 2);
    metrics = metrics_processor.metrics();
    const auto* unknown_metrics = find_command_metrics(metrics, "UNKNOWN");
    assert(unknown_metrics != nullptr);
    assert(unknown_metrics->total_commands == 1);
    assert(unknown_metrics->successful_commands == 0);
    assert(unknown_metrics->error_commands == 1);

    result = metrics_processor.execute("STATS");
    assert(result.response.find("total_commands=3") != std::string::npos);
    assert(result.response.find("successful_commands=1") != std::string::npos);
    assert(result.response.find("error_commands=2") != std::string::npos);
    assert(result.response.find("total_latency_ns=") != std::string::npos);
    assert(result.response.find("avg_latency_ns=") != std::string::npos);
    assert(result.response.find("max_latency_ns=") != std::string::npos);
    assert(result.response.find("command_breakdown=GET:1/0/1/") != std::string::npos);
    assert(result.response.find("PING:1/1/0/") != std::string::npos);
    assert(result.response.find("UNKNOWN:1/0/1/") != std::string::npos);
    assert(metrics_processor.metrics().total_commands == 4);
    assert(metrics_processor.metrics().successful_commands == 2);
    assert(metrics_processor.metrics().error_commands == 2);
    metrics = metrics_processor.metrics();
    const auto* stats_metrics = find_command_metrics(metrics, "STATS");
    assert(stats_metrics != nullptr);
    assert(stats_metrics->total_commands == 1);
    assert(stats_metrics->successful_commands == 1);
    assert(stats_metrics->error_commands == 0);

    result = metrics_processor.execute("HEALTH");
    assert(result.response.find("total_commands=4") != std::string::npos);
    assert(result.response.find("successful_commands=2") != std::string::npos);
    assert(result.response.find("error_commands=2") != std::string::npos);
    assert(result.response.find("STATS:1/1/0/") != std::string::npos);

    result = metrics_processor.execute("STATSJSON");
    assert(result.response.find("\"schema_version\":1") != std::string::npos);
    assert(result.response.find("\"read_only\":true") != std::string::npos);
    assert(result.response.find("\"execution_allowed\":false") != std::string::npos);
    assert(result.response.find("\"order_authoritative\":false") != std::string::npos);
    assert(result.response.find("\"evidence_type\":\"runtime_metrics\"") != std::string::npos);
    assert(result.response.find("\"live_keys\":0") != std::string::npos);
    assert(result.response.find("\"wal_enabled\":false") != std::string::npos);
    assert(result.response.find("\"wal\":null") != std::string::npos);
    assert(result.response.find("\"commands\":{\"total_commands\":5") != std::string::npos);
    assert(result.response.find("\"successful_commands\":3") != std::string::npos);
    assert(result.response.find("\"error_commands\":2") != std::string::npos);
    assert(result.response.find("\"command\":\"PING\"") != std::string::npos);
    assert(result.response.find("\"command\":\"UNKNOWN\"") != std::string::npos);
    assert(result.response.find("\"connection_stats\":{\"available\":false}") != std::string::npos);
    assert(result.response.find("\"diagnostics\":{\"write_commands_executed\":false") != std::string::npos);
    assert(metrics_processor.metrics().total_commands == 6);
    metrics = metrics_processor.metrics();
    const auto* statsjson_metrics = find_command_metrics(metrics, "STATSJSON");
    assert(statsjson_metrics != nullptr);
    assert(statsjson_metrics->total_commands == 1);
    assert(statsjson_metrics->successful_commands == 1);

    minikv::CommandProcessorOptions info_options;
    info_options.runtime_info.protocol = "inline,resp";
    info_options.runtime_info.started_at = std::chrono::steady_clock::now() - 3s;
    info_options.runtime_info.max_request_bytes = 4096;
    info_options.runtime_info.metrics_enabled = true;
    minikv::CommandProcessor info_processor{metrics_store, nullptr, info_options};
    result = info_processor.execute("INFO");
    assert(result.response.find("protocol=inline,resp") != std::string::npos);
    assert(result.response.find("metrics_enabled=yes") != std::string::npos);
    assert(result.response.find("max_request_bytes=4096") != std::string::npos);

    result = info_processor.execute("INFOJSON");
    assert(result.response.find("\"protocol\":[\"inline\",\"resp\"]") != std::string::npos);
    assert(result.response.find("\"metrics\":{\"enabled\":true}") != std::string::npos);
    assert(result.response.find("\"max_request_bytes\":4096") != std::string::npos);
    assert(result.response.find("\"evidence_type\":\"runtime_identity\"") != std::string::npos);

    result = metrics_processor.execute("RESETSTATS extra");
    assert(result.response == "ERR usage: RESETSTATS");
    assert(metrics_processor.metrics().total_commands == 7);
    assert(metrics_processor.metrics().error_commands == 3);

    result = metrics_processor.execute("RESETSTATS");
    assert(result.response == "OK stats reset");
    assert(metrics_processor.metrics().total_commands == 0);
    assert(metrics_processor.metrics().successful_commands == 0);
    assert(metrics_processor.metrics().error_commands == 0);
    assert(metrics_processor.metrics().command_breakdown.empty());

    result = metrics_processor.execute("STATS");
    assert(result.response.find("total_commands=0") != std::string::npos);
    assert(result.response.find("successful_commands=0") != std::string::npos);
    assert(result.response.find("error_commands=0") != std::string::npos);
    assert(result.response.find("command_breakdown=none") != std::string::npos);
    assert(metrics_processor.metrics().total_commands == 1);

    result = processor.execute("HELP");
    assert(result.response.find("COMPACT") != std::string::npos);
    assert(result.response.find("KEYS") != std::string::npos);
    assert(result.response.find("KEYSJSON") != std::string::npos);
    assert(result.response.find("WALINFO") != std::string::npos);
    assert(result.response.find("STATS") != std::string::npos);
    assert(result.response.find("STATSJSON") != std::string::npos);
    assert(result.response.find("RESETSTATS") != std::string::npos);
    assert(result.response.find("HEALTH") != std::string::npos);
    assert(result.response.find("INFO") != std::string::npos);
    assert(result.response.find("INFOJSON") != std::string::npos);
    assert(result.response.find("COMMANDS") != std::string::npos);
    assert(result.response.find("COMMANDSJSON") != std::string::npos);
    assert(result.response.find("EXPLAINJSON") != std::string::npos);
    assert(result.response.find("CHECKJSON") != std::string::npos);
    assert(result.response.find("STORAGEJSON") != std::string::npos);
    assert(result.response.find("SETNXEX") != std::string::npos);

    result = processor.execute("GET name extra");
    assert(result.response == "ERR usage: GET key");

    result = processor.execute("UNKNOWN");
    assert(result.response == "ERR unknown command");

    result = processor.execute("QUIT");
    assert(result.response == "BYE");
    assert(result.should_close);

    return 0;
}
