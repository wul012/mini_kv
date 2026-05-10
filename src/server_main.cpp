#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
#include "minikv/wal.hpp"

#include <csignal>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <mutex>
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

std::size_t parse_positive_size(const char* text, std::string_view name) {
    if (text[0] == '\0' || text[0] == '-') {
        throw std::out_of_range{std::string{name} + " must be a positive integer"};
    }

    std::size_t parsed_chars = 0;
    const unsigned long long value = std::stoull(text, &parsed_chars);
    if (text[parsed_chars] != '\0' || value == 0 ||
        value > static_cast<unsigned long long>(std::numeric_limits<std::size_t>::max())) {
        throw std::out_of_range{std::string{name} + " must be a positive integer"};
    }
    return static_cast<std::size_t>(value);
}

std::chrono::milliseconds parse_positive_milliseconds(const char* text, std::string_view name) {
    const auto value = parse_positive_size(text, name);
    return std::chrono::milliseconds{static_cast<long long>(value)};
}

void print_usage(const char* program) {
    std::cerr << "Usage: " << program
              << " [port] [host] [wal_path] [--repair-wal] [--max-request-bytes bytes] [--accept-poll-ms ms]\n";
}

void log_wal_replay(const minikv::WriteAheadLog& wal, const minikv::WalReplayReport& replay) {
    std::cout << "event=wal_replay path=" << quote_value(wal.path().string())
              << " records=" << replay.applied_records << " skipped=" << replay.skipped_records
              << " truncated=" << replay.truncated_records
              << " checksum_failed=" << replay.checksum_failed_records << '\n';
}

void log_wal_repair(const minikv::WriteAheadLog& wal, const minikv::WalRepairReport& repair) {
    std::cout << "event=wal_repair path=" << quote_value(wal.path().string())
              << " records=" << repair.replay.applied_records << " skipped=" << repair.replay.skipped_records
              << " truncated=" << repair.replay.truncated_records
              << " checksum_failed=" << repair.replay.checksum_failed_records
              << " compacted_keys=" << repair.compacted_keys << '\n';
}

const char* true_false(bool value) {
    return value ? "true" : "false";
}

void log_wal_maintenance(const minikv::WriteAheadLog& wal, const minikv::WalMaintenanceReport& report) {
    std::cout << "event=wal_stats path=" << quote_value(wal.path().string())
              << " wal_bytes=" << report.bytes << " wal_records=" << report.records
              << " live_keys=" << report.live_keys
              << " compact_recommended=" << true_false(report.compact_recommended) << '\n';

    if (report.compact_recommended) {
        std::cout << "event=wal_compact_hint path=" << quote_value(wal.path().string())
                  << " wal_bytes=" << report.bytes << " wal_records=" << report.records
                  << " live_keys=" << report.live_keys << '\n';
    }
}

} // namespace

int main(int argc, char** argv) {
    try {
        std::cout << std::unitbuf;
        std::cerr << std::unitbuf;

        minikv::TcpServer::Options options;
        options.should_stop = should_stop;
        std::mutex log_mutex;
        options.logger = [&log_mutex](const std::string& message) {
            std::lock_guard lock{log_mutex};
            std::cout << message << '\n';
        };

        int positional = 0;
        std::optional<std::string> wal_path;
        bool repair_wal = false;
        for (int index = 1; index < argc; ++index) {
            const std::string_view argument = argv[index];
            if (argument == "--max-request-bytes") {
                if (++index >= argc) {
                    print_usage(argv[0]);
                    return 2;
                }
                options.max_request_bytes = parse_positive_size(argv[index], "max-request-bytes");
                continue;
            }

            if (argument == "--accept-poll-ms") {
                if (++index >= argc) {
                    print_usage(argv[0]);
                    return 2;
                }
                options.accept_poll_interval = parse_positive_milliseconds(argv[index], "accept-poll-ms");
                continue;
            }

            if (argument == "--repair-wal") {
                repair_wal = true;
                continue;
            }

            if (!argument.empty() && argument.front() == '-') {
                print_usage(argv[0]);
                return 2;
            }

            ++positional;
            if (positional == 1) {
                options.port = parse_port(argv[index]);
            } else if (positional == 2) {
                options.host = argv[index];
            } else if (positional == 3) {
                wal_path = argv[index];
            } else {
                print_usage(argv[0]);
                return 2;
            }
        }

        if (repair_wal && !wal_path.has_value()) {
            print_usage(argv[0]);
            return 2;
        }

        minikv::Store store;
        std::optional<minikv::WriteAheadLog> wal;

        if (wal_path.has_value()) {
            wal.emplace(*wal_path);
            if (repair_wal) {
                minikv::WalRepairReport repair;
                if (!wal->repair(store, &repair)) {
                    std::cerr << "fatal: WAL repair failed\n";
                    return 1;
                }
                log_wal_repair(*wal, repair);
            } else {
                log_wal_replay(*wal, wal->replay_with_report(store));
            }
            log_wal_maintenance(*wal, wal->maintenance_report(store));
        }

        minikv::TcpServer server{store, options, wal.has_value() ? &*wal : nullptr};

        install_shutdown_handlers();

        std::cout << "event=server_start host=" << options.host << " port=" << options.port
                  << " protocol=inline,resp max_request_bytes=" << options.max_request_bytes
                  << " accept_poll_ms=" << options.accept_poll_interval.count() << '\n';
        std::cout << "event=server_hint command=" << quote_value("SET name mini-kv") << '\n';

        server.run();
        std::cout << "event=server_stopped host=" << options.host << " port=" << options.port << '\n';
    } catch (const std::exception& error) {
        std::cerr << "fatal: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
