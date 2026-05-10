#pragma once

#include "minikv/store.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>
#include <string>

namespace minikv {

class WriteAheadLog;

class TcpServer {
public:
    struct Options {
        using LogHandler = std::function<void(const std::string&)>;
        using StopPredicate = std::function<bool()>;

        std::string host = "0.0.0.0";
        std::uint16_t port = 6379;
        int backlog = 64;
        std::chrono::milliseconds accept_poll_interval{200};
        StopPredicate should_stop;
        LogHandler logger;
    };

    explicit TcpServer(Store& store, Options options);
    TcpServer(Store& store, Options options, WriteAheadLog* wal);

    void run();
    void request_stop();
    bool stop_requested() const;

private:
    Store& store_;
    Options options_;
    WriteAheadLog* wal_ = nullptr;
    std::atomic_bool stop_requested_{false};
};

} // namespace minikv
