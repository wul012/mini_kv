#include "minikv/command.hpp"
#include "minikv/store.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
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

    result = processor.execute("KEYS extra");
    assert(result.response == "ERR usage: KEYS");

    result = processor.execute("DEL alpha");
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
    assert(result.response.find("\"live_keys\":0") != std::string::npos);
    assert(result.response.find("\"wal_enabled\":false") != std::string::npos);
    assert(result.response.find("\"wal\":null") != std::string::npos);
    assert(result.response.find("\"commands\":{\"total_commands\":5") != std::string::npos);
    assert(result.response.find("\"successful_commands\":3") != std::string::npos);
    assert(result.response.find("\"error_commands\":2") != std::string::npos);
    assert(result.response.find("\"command\":\"PING\"") != std::string::npos);
    assert(result.response.find("\"command\":\"UNKNOWN\"") != std::string::npos);
    assert(result.response.find("\"connection_stats\":{\"available\":false}") != std::string::npos);
    assert(metrics_processor.metrics().total_commands == 6);
    metrics = metrics_processor.metrics();
    const auto* statsjson_metrics = find_command_metrics(metrics, "STATSJSON");
    assert(statsjson_metrics != nullptr);
    assert(statsjson_metrics->total_commands == 1);
    assert(statsjson_metrics->successful_commands == 1);

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
    assert(result.response.find("WALINFO") != std::string::npos);
    assert(result.response.find("STATS") != std::string::npos);
    assert(result.response.find("STATSJSON") != std::string::npos);
    assert(result.response.find("RESETSTATS") != std::string::npos);
    assert(result.response.find("HEALTH") != std::string::npos);

    result = processor.execute("GET name extra");
    assert(result.response == "ERR usage: GET key");

    result = processor.execute("UNKNOWN");
    assert(result.response == "ERR unknown command");

    result = processor.execute("QUIT");
    assert(result.response == "BYE");
    assert(result.should_close);

    return 0;
}
