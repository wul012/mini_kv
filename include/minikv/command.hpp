#pragma once

#include "minikv/store.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
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

struct CommandProcessorOptions {
    using ConnectionStatsProvider = std::function<CommandConnectionStats()>;

    bool auto_compact_wal = false;
    ConnectionStatsProvider connection_stats;
};

class CommandProcessor {
public:
    explicit CommandProcessor(Store& store,
                              WriteAheadLog* wal = nullptr,
                              CommandProcessorOptions options = {});

    CommandResult execute(std::string_view line);

    static std::string help_text();

private:
    void auto_compact_wal_if_needed();

    Store& store_;
    WriteAheadLog* wal_;
    CommandProcessorOptions options_;
};

} // namespace minikv
