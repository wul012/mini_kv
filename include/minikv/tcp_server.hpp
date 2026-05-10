#pragma once

#include "minikv/store.hpp"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace minikv {

class WriteAheadLog;
struct TcpServerConnectionTracker;

struct TcpServerConnectionStats {
    std::uint64_t total_connections = 0;
    std::size_t active_connections = 0;
    std::size_t peak_connections = 0;
};

class TcpServer {
public:
    struct Options {
        using LogHandler = std::function<void(const std::string&)>;
        using StopPredicate = std::function<bool()>;

        std::string host = "0.0.0.0";
        std::uint16_t port = 6379;
        int backlog = 64;
        std::chrono::milliseconds accept_poll_interval{200};
        std::size_t max_request_bytes = 64 * 1024;
        bool auto_compact_wal = false;
        StopPredicate should_stop;
        LogHandler logger;
    };

    explicit TcpServer(Store& store, Options options);
    TcpServer(Store& store, Options options, WriteAheadLog* wal);

    void run();
    void request_stop();
    bool stop_requested() const;
    std::uint16_t bound_port() const;
    TcpServerConnectionStats connection_stats() const;

private:
    Store& store_;
    Options options_;
    WriteAheadLog* wal_ = nullptr;
    std::atomic_bool stop_requested_{false};
    std::atomic<std::uint16_t> bound_port_{0};
    std::shared_ptr<TcpServerConnectionTracker> connection_tracker_;
};

} // namespace minikv
