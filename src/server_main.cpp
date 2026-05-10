#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
#include "minikv/wal.hpp"

#include <csignal>
#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

volatile std::sig_atomic_t shutdown_requested = 0;

void handle_shutdown_signal(int) {
    shutdown_requested = 1;
}

bool should_stop() {
    return shutdown_requested != 0;
}

void install_shutdown_handlers() {
    std::signal(SIGINT, handle_shutdown_signal);
    std::signal(SIGTERM, handle_shutdown_signal);
}

std::string quote_value(std::string_view value) {
    std::string quoted = "\"";
    for (const char ch : value) {
        if (ch == '\\' || ch == '"') {
            quoted.push_back('\\');
        }
        quoted.push_back(ch);
    }
    quoted.push_back('"');
    return quoted;
}

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
        std::cout << std::unitbuf;
        std::cerr << std::unitbuf;

        minikv::TcpServer::Options options;
        options.should_stop = should_stop;
        options.logger = [](const std::string& message) {
            std::cout << message << '\n';
        };

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
            std::cout << "event=wal_replay path=" << quote_value(wal->path().string()) << " records=" << replayed
                      << '\n';
        }

        minikv::TcpServer server{store, options, wal.has_value() ? &*wal : nullptr};

        install_shutdown_handlers();

        std::cout << "event=server_start host=" << options.host << " port=" << options.port
                  << " protocol=inline,resp" << '\n';
        std::cout << "event=server_hint command=" << quote_value("SET name mini-kv") << '\n';

        server.run();
        std::cout << "event=server_stopped host=" << options.host << " port=" << options.port << '\n';
    } catch (const std::exception& error) {
        std::cerr << "fatal: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
