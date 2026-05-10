#pragma once

#include "minikv/store.hpp"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

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

struct CommandProcessorMetrics {
    std::uint64_t total_commands = 0;
    std::uint64_t successful_commands = 0;
    std::uint64_t error_commands = 0;
};

class CommandMetricsTracker {
public:
    void record(const CommandResult& result);
    CommandProcessorMetrics stats() const;

private:
    std::atomic<std::uint64_t> total_commands_{0};
    std::atomic<std::uint64_t> successful_commands_{0};
    std::atomic<std::uint64_t> error_commands_{0};
};

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
