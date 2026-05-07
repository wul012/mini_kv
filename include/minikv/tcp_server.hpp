#pragma once

#include "minikv/store.hpp"

#include <cstdint>
#include <string>

namespace minikv {

class TcpServer {
public:
    struct Options {
        std::string host = "0.0.0.0";
        std::uint16_t port = 6379;
        int backlog = 64;
    };

    explicit TcpServer(Store& store, Options options);

    void run();

private:
    Store& store_;
    Options options_;
};

} // namespace minikv
