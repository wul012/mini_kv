#pragma once

#include "minikv/store.hpp"

#include <cstdint>
#include <string>

namespace minikv {

class WriteAheadLog;

class TcpServer {
public:
    struct Options {
        std::string host = "0.0.0.0";
        std::uint16_t port = 6379;
        int backlog = 64;
    };

    explicit TcpServer(Store& store, Options options);
    TcpServer(Store& store, Options options, WriteAheadLog* wal);

    void run();

private:
    Store& store_;
    Options options_;
    WriteAheadLog* wal_ = nullptr;
};

} // namespace minikv
