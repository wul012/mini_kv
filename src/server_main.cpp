#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
#include "minikv/wal.hpp"

#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

namespace {

std::uint16_t parse_port(const char* text) {
    const int port = std::stoi(text);
    if (port < 1 || port > 65535) {
        throw std::out_of_range{"port must be between 1 and 65535"};
    }
    return static_cast<std::uint16_t>(port);
}

void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [port] [host] [wal_path]\n";
}

} // namespace

int main(int argc, char** argv) {
    try {
        minikv::TcpServer::Options options;

        if (argc > 4) {
            print_usage(argv[0]);
            return 2;
        }

        if (argc >= 2) {
            options.port = parse_port(argv[1]);
        }

        if (argc >= 3) {
            options.host = argv[2];
        }

        minikv::Store store;
        std::optional<minikv::WriteAheadLog> wal;

        if (argc >= 4) {
            wal.emplace(argv[3]);
            const auto replayed = wal->replay(store);
            std::cout << "WAL: " << wal->path().string() << " (" << replayed << " records replayed)\n";
        }

        minikv::TcpServer server{store, options, wal.has_value() ? &*wal : nullptr};

        std::cout << "mini-kv server listening on " << options.host << ':' << options.port << '\n';
        std::cout << "Protocol: inline text or RESP array requests. Try: SET name mini-kv" << '\n';

        server.run();
    } catch (const std::exception& error) {
        std::cerr << "fatal: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
