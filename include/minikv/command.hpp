#pragma once

#include "minikv/store.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace minikv {

class WriteAheadLog;

struct CommandResult {
    std::string response;
    bool should_close = false;
};

struct CommandConnectionStats {
    bool available = false;
    std::uint64_t total_connections = 0;
    std::size_t active_connections = 0;
    std::size_t peak_connections = 0;
};

struct CommandBreakdownMetrics {
    std::string command;
    std::uint64_t total_commands = 0;
    std::uint64_t successful_commands = 0;
    std::uint64_t error_commands = 0;
    std::uint64_t total_latency_ns = 0;
    std::uint64_t max_latency_ns = 0;
};

struct CommandProcessorMetrics {
    std::uint64_t total_commands = 0;
    std::uint64_t successful_commands = 0;
    std::uint64_t error_commands = 0;
    std::uint64_t total_latency_ns = 0;
    std::uint64_t max_latency_ns = 0;
    std::vector<CommandBreakdownMetrics> command_breakdown;
};

class CommandMetricsTracker {
public:
    void record(const CommandResult& result);
    void record(std::string_view command, const CommandResult& result, std::uint64_t elapsed_ns);
    void reset();
    CommandProcessorMetrics stats() const;

private:
    mutable std::mutex mutex_;
    CommandProcessorMetrics totals_;
    std::map<std::string, CommandBreakdownMetrics> breakdown_;
};

std::string format_command_metrics(const CommandProcessorMetrics& metrics);
std::string format_command_metrics_json(const CommandProcessorMetrics& metrics);

struct CommandProcessorOptions {
    using ConnectionStatsProvider = std::function<CommandConnectionStats()>;

    bool auto_compact_wal = false;
    ConnectionStatsProvider connection_stats;
    std::shared_ptr<CommandMetricsTracker> metrics_tracker;
};

class CommandProcessor {
public:
    explicit CommandProcessor(Store& store,
                              WriteAheadLog* wal = nullptr,
                              CommandProcessorOptions options = {});

    CommandResult execute(std::string_view line);
    CommandProcessorMetrics metrics() const;

    static std::string help_text();

private:
    CommandResult execute_trimmed(std::string_view trimmed);
    void auto_compact_wal_if_needed();

    Store& store_;
    WriteAheadLog* wal_;
    CommandProcessorOptions options_;
    std::shared_ptr<CommandMetricsTracker> metrics_tracker_;
};

} // namespace minikv
