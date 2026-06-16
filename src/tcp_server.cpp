#include "minikv/tcp_server.hpp"

#include "minikv/command.hpp"
#include "minikv/resp.hpp"
#include "minikv/wal.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <cerrno>
#include <cstdint>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace minikv {

struct ConnectionSnapshot {
    std::uint64_t connection_id = 0;
    std::uint64_t total_connections = 0;
    std::size_t active_connections = 0;
    std::size_t peak_connections = 0;
};

struct TcpServerConnectionTracker {
    ConnectionSnapshot connected() {
        ConnectionSnapshot snapshot;
        snapshot.connection_id = next_connection_id.fetch_add(1);
        snapshot.total_connections = total_connections.fetch_add(1) + 1;
        snapshot.active_connections = active_connections.fetch_add(1) + 1;
        snapshot.peak_connections = update_peak(snapshot.active_connections);
        return snapshot;
    }

    ConnectionSnapshot closed(std::uint64_t connection_id) {
        ConnectionSnapshot snapshot;
        snapshot.connection_id = connection_id;
        snapshot.active_connections = active_connections.fetch_sub(1) - 1;
        snapshot.total_connections = total_connections.load();
        snapshot.peak_connections = peak_connections.load();
        return snapshot;
    }

    TcpServerConnectionStats stats() const {
        return {total_connections.load(), active_connections.load(), peak_connections.load()};
    }

    std::atomic<std::uint64_t> next_connection_id{1};
    std::atomic<std::uint64_t> total_connections{0};
    std::atomic_size_t active_connections{0};
    std::atomic_size_t peak_connections{0};

private:
    std::size_t update_peak(std::size_t active) {
        std::size_t current_peak = peak_connections.load();
        while (active > current_peak && !peak_connections.compare_exchange_weak(current_peak, active)) {
        }
        return peak_connections.load();
    }
};

namespace {

#ifdef _WIN32
using SocketHandle = SOCKET;
constexpr SocketHandle invalid_socket = INVALID_SOCKET;

void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        closesocket(socket);
    }
}

class NetworkRuntime {
public:
    NetworkRuntime() {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            throw std::runtime_error{"WSAStartup failed"};
        }
    }

    ~NetworkRuntime() { WSACleanup(); }

    NetworkRuntime(const NetworkRuntime&) = delete;
    NetworkRuntime& operator=(const NetworkRuntime&) = delete;
};

std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": WSA error " + std::to_string(WSAGetLastError());
}

#else
using SocketHandle = int;
constexpr SocketHandle invalid_socket = -1;

void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        close(socket);
    }
}

class NetworkRuntime {
public:
    NetworkRuntime() = default;
};

std::string socket_error_message(std::string_view prefix) { return std::string{prefix} + ": " + std::strerror(errno); }
#endif

void log_event(const TcpServer::Options& options, const std::string& message) {
    if (options.logger) {
        options.logger(message);
    }
}

std::string json_string(std::string_view value) {
    constexpr char hex[] = "0123456789ABCDEF";
    std::string result = "\"";
    for (const unsigned char ch : value) {
        switch (ch) {
        case '"':
            result += "\\\"";
            break;
        case '\\':
            result += "\\\\";
            break;
        case '\b':
            result += "\\b";
            break;
        case '\f':
            result += "\\f";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\r':
            result += "\\r";
            break;
        case '\t':
            result += "\\t";
            break;
        default:
            if (ch < 0x20) {
                result += "\\u00";
                result.push_back(hex[(ch >> 4) & 0x0F]);
                result.push_back(hex[ch & 0x0F]);
            } else {
                result.push_back(static_cast<char>(ch));
            }
            break;
        }
    }
    result.push_back('"');
    return result;
}

std::string metrics_json_line(std::string_view event, const TcpServer::Options& options, std::uint16_t bound_port,
                              const TcpServerConnectionStats& stats, const CommandProcessorMetrics& command_metrics,
                              std::optional<std::chrono::milliseconds> interval) {
    std::string result = "{\"event\":" + json_string(event) + ",\"host\":" + json_string(options.host) +
                         ",\"port\":" + std::to_string(bound_port) +
                         ",\"connection_stats\":{\"active_connections\":" + std::to_string(stats.active_connections) +
                         ",\"total_connections\":" + std::to_string(stats.total_connections) +
                         ",\"peak_connections\":" + std::to_string(stats.peak_connections) + "}," +
                         format_command_metrics_json(command_metrics);
    if (interval.has_value()) {
        result += ",\"metrics_interval_ms\":" + std::to_string(interval->count());
    }
    result += "}";
    return result;
}

void export_metrics_event(const TcpServer::Options& options, const std::string& log_message,
                          const std::string& jsonl_message) {
    log_event(options, log_message);
    if (options.metrics_exporter) {
        options.metrics_exporter(options.metrics_export_format == MetricsExportFormat::jsonl ? jsonl_message
                                                                                             : log_message);
    }
}

std::string endpoint_fields(const TcpServer::Options& options, std::uint16_t bound_port) {
    return "host=" + options.host + " port=" + std::to_string(bound_port);
}

std::string connection_fields(const ConnectionSnapshot& snapshot) {
    return "connection_id=" + std::to_string(snapshot.connection_id) +
           " active_connections=" + std::to_string(snapshot.active_connections) +
           " total_connections=" + std::to_string(snapshot.total_connections) +
           " peak_connections=" + std::to_string(snapshot.peak_connections);
}

std::string stats_fields(const TcpServerConnectionStats& stats) {
    return "active_connections=" + std::to_string(stats.active_connections) +
           " total_connections=" + std::to_string(stats.total_connections) +
           " peak_connections=" + std::to_string(stats.peak_connections);
}

std::string command_metrics_fields(const CommandProcessorMetrics& metrics) { return format_command_metrics(metrics); }

bool metrics_logging_enabled(const TcpServer::Options& options) {
    return options.metrics_log_interval > std::chrono::milliseconds::zero();
}

std::chrono::milliseconds listener_wait_interval(const TcpServer::Options& options) {
    if (metrics_logging_enabled(options) && options.metrics_log_interval < options.accept_poll_interval) {
        return options.metrics_log_interval;
    }
    return options.accept_poll_interval;
}

std::string metrics_fields(const TcpServerConnectionStats& stats, const CommandProcessorMetrics& command_metrics,
                           std::chrono::milliseconds interval) {
    return stats_fields(stats) + " " + command_metrics_fields(command_metrics) +
           " metrics_interval_ms=" + std::to_string(interval.count());
}

std::string request_limit_fields(std::size_t pending_bytes, std::size_t max_request_bytes) {
    return "pending_bytes=" + std::to_string(pending_bytes) + " max_request_bytes=" + std::to_string(max_request_bytes);
}

std::string client_timeout_fields(std::size_t pending_bytes, std::chrono::milliseconds client_idle_timeout) {
    return "pending_bytes=" + std::to_string(pending_bytes) +
           " client_idle_timeout_ms=" + std::to_string(client_idle_timeout.count());
}

std::uint16_t socket_bound_port(SocketHandle socket) {
    sockaddr_storage address{};
#ifdef _WIN32
    int length = sizeof(address);
#else
    socklen_t length = sizeof(address);
#endif
    if (getsockname(socket, reinterpret_cast<sockaddr*>(&address), &length) != 0) {
        throw std::runtime_error{socket_error_message("getsockname failed")};
    }

    if (address.ss_family == AF_INET) {
        const auto* ipv4 = reinterpret_cast<const sockaddr_in*>(&address);
        return ntohs(ipv4->sin_port);
    }

    if (address.ss_family == AF_INET6) {
        const auto* ipv6 = reinterpret_cast<const sockaddr_in6*>(&address);
        return ntohs(ipv6->sin6_port);
    }

    throw std::runtime_error{"getsockname returned an unsupported address family"};
}

timeval to_timeval(std::chrono::milliseconds timeout) {
    if (timeout < std::chrono::milliseconds::zero()) {
        timeout = std::chrono::milliseconds::zero();
    }

    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeout - seconds);

    timeval value{};
    value.tv_sec = static_cast<long>(seconds.count());
    value.tv_usec = static_cast<long>(microseconds.count());
    return value;
}

bool wait_for_readable(SocketHandle socket, std::chrono::milliseconds timeout) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket, &read_fds);

    timeval wait_time = to_timeval(timeout);
#ifdef _WIN32
    const int result = select(0, &read_fds, nullptr, nullptr, &wait_time);
#else
    const int result = select(socket + 1, &read_fds, nullptr, nullptr, &wait_time);
#endif
    if (result < 0) {
#ifdef _WIN32
        if (WSAGetLastError() == WSAEINTR) {
            return false;
        }
#else
        if (errno == EINTR) {
            return false;
        }
#endif
        throw std::runtime_error{socket_error_message("select failed")};
    }

    return result > 0 && FD_ISSET(socket, &read_fds);
}

class SocketGuard {
public:
    explicit SocketGuard(SocketHandle socket = invalid_socket) : socket_(socket) {}

    ~SocketGuard() { close_socket(socket_); }

    SocketGuard(const SocketGuard&) = delete;
    SocketGuard& operator=(const SocketGuard&) = delete;

    SocketGuard(SocketGuard&& other) noexcept : socket_(other.release()) {}

    SocketGuard& operator=(SocketGuard&& other) noexcept {
        if (this != &other) {
            close_socket(socket_);
            socket_ = other.release();
        }
        return *this;
    }

    SocketHandle get() const { return socket_; }

    SocketHandle release() {
        const SocketHandle socket = socket_;
        socket_ = invalid_socket;
        return socket;
    }

private:
    SocketHandle socket_;
};

class AddrInfoGuard {
public:
    explicit AddrInfoGuard(addrinfo* info) : info_(info) {}

    ~AddrInfoGuard() {
        if (info_ != nullptr) {
            freeaddrinfo(info_);
        }
    }

    AddrInfoGuard(const AddrInfoGuard&) = delete;
    AddrInfoGuard& operator=(const AddrInfoGuard&) = delete;

    addrinfo* get() const { return info_; }

private:
    addrinfo* info_;
};

bool send_all(SocketHandle socket, std::string_view data) {
    while (!data.empty()) {
        const std::size_t chunk_size = std::min<std::size_t>(data.size(), 4096);
#ifdef _WIN32
        const int sent = send(socket, data.data(), static_cast<int>(chunk_size), 0);
#else
        const auto sent = send(socket, data.data(), chunk_size, 0);
#endif
        if (sent <= 0) {
            return false;
        }

        data.remove_prefix(static_cast<std::size_t>(sent));
    }

    return true;
}

class ConnectionCloseLogger {
public:
    ConnectionCloseLogger(TcpServer::Options::LogHandler logger, std::string endpoint,
                          std::shared_ptr<TcpServerConnectionTracker> tracker, std::uint64_t connection_id)
        : logger_(std::move(logger)), endpoint_(std::move(endpoint)), tracker_(std::move(tracker)),
          connection_id_(connection_id) {}

    ~ConnectionCloseLogger() {
        if (!tracker_) {
            return;
        }

        const auto snapshot = tracker_->closed(connection_id_);
        if (logger_) {
            logger_("event=tcp_client_closed " + endpoint_ + " " + connection_fields(snapshot));
        }
    }

    ConnectionCloseLogger(const ConnectionCloseLogger&) = delete;
    ConnectionCloseLogger& operator=(const ConnectionCloseLogger&) = delete;

private:
    TcpServer::Options::LogHandler logger_;
    std::string endpoint_;
    std::shared_ptr<TcpServerConnectionTracker> tracker_;
    std::uint64_t connection_id_ = 0;
};

bool process_inline_commands(SocketHandle socket, CommandProcessor& processor, std::string& pending, bool& needs_more) {
    needs_more = false;

    std::size_t newline = std::string::npos;
    while ((newline = pending.find('\n')) != std::string::npos) {
        std::string line = pending.substr(0, newline);
        pending.erase(0, newline + 1);

        const auto result = processor.execute(line);
        if (!result.response.empty() && !send_all(socket, result.response + "\n")) {
            return false;
        }

        if (result.should_close) {
            return false;
        }

        if (!pending.empty() && pending.front() == '*') {
            return true;
        }
    }

    needs_more = !pending.empty();
    return true;
}

bool process_resp_commands(SocketHandle socket, CommandProcessor& processor, std::string& pending, bool& needs_more) {
    needs_more = false;

    while (!pending.empty() && pending.front() == '*') {
        const auto parsed = RespParser::parse_command(pending);
        if (parsed.status == RespParseStatus::incomplete) {
            needs_more = true;
            return true;
        }

        if (parsed.status == RespParseStatus::error) {
            send_all(socket, "-ERR " + parsed.error + "\r\n");
            return false;
        }

        const auto result = processor.execute(RespParser::to_inline_command(parsed.command));
        if (!result.response.empty() && !send_all(socket, RespParser::to_resp_response(result.response))) {
            return false;
        }

        pending.erase(0, parsed.command.consumed);

        if (result.should_close) {
            return false;
        }
    }

    return true;
}

void serve_client(Store& store, WriteAheadLog* wal, SocketHandle client_socket, TcpServer::Options::LogHandler logger,
                  std::string endpoint, std::shared_ptr<TcpServerConnectionTracker> tracker,
                  std::shared_ptr<CommandMetricsTracker> command_metrics_tracker, std::uint64_t connection_id,
                  std::size_t max_request_bytes, std::chrono::milliseconds client_idle_timeout,
                  CommandRuntimeInfo runtime_info, bool auto_compact_wal) {
    SocketGuard client{client_socket};
    ConnectionCloseLogger close_logger{logger, endpoint, tracker, connection_id};
    CommandProcessorOptions command_options;
    command_options.auto_compact_wal = auto_compact_wal;
    command_options.connection_stats = [tracker] {
        const auto stats = tracker->stats();
        return CommandConnectionStats{true, stats.total_connections, stats.active_connections, stats.peak_connections};
    };
    command_options.metrics_tracker = std::move(command_metrics_tracker);
    command_options.runtime_info = std::move(runtime_info);
    CommandProcessor processor{store, wal, command_options};
    std::string pending;
    std::array<char, 4096> buffer{};

    while (true) {
        if (client_idle_timeout > std::chrono::milliseconds::zero() &&
            !wait_for_readable(client.get(), client_idle_timeout)) {
            const bool has_partial_command = !pending.empty();
            if (logger) {
                logger("event=tcp_request_rejected " + endpoint + " connection_id=" + std::to_string(connection_id) +
                       " reason=" + (has_partial_command ? "command_timeout " : "client_idle_timeout ") +
                       client_timeout_fields(pending.size(), client_idle_timeout));
            }

            if (has_partial_command) {
                if (pending.front() == '*') {
                    send_all(client.get(), "-ERR command timeout\r\n");
                } else {
                    send_all(client.get(), "ERR command timeout\n");
                }
            }
            return;
        }

#ifdef _WIN32
        const int received = recv(client.get(), buffer.data(), static_cast<int>(buffer.size()), 0);
#else
        const auto received = recv(client.get(), buffer.data(), buffer.size(), 0);
#endif
        if (received <= 0) {
            return;
        }

        pending.append(buffer.data(), static_cast<std::size_t>(received));

        while (!pending.empty()) {
            bool needs_more = false;
            const bool ok = pending.front() == '*'
                                ? process_resp_commands(client.get(), processor, pending, needs_more)
                                : process_inline_commands(client.get(), processor, pending, needs_more);
            if (!ok) {
                return;
            }

            if (pending.empty() || needs_more) {
                break;
            }
        }

        if (pending.size() > max_request_bytes) {
            if (logger) {
                logger("event=tcp_request_rejected " + endpoint + " connection_id=" + std::to_string(connection_id) +
                       " reason=request_too_long " + request_limit_fields(pending.size(), max_request_bytes));
            }

            if (!pending.empty() && pending.front() == '*') {
                send_all(client.get(), "-ERR request too long\r\n");
            } else {
                send_all(client.get(), "ERR line too long\n");
            }
            return;
        }
    }
}

SocketGuard bind_listener(const TcpServer::Options& options) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    addrinfo* raw_info = nullptr;
    const std::string port = std::to_string(options.port);
    const int result = getaddrinfo(options.host.c_str(), port.c_str(), &hints, &raw_info);
    if (result != 0) {
#ifdef _WIN32
        throw std::runtime_error{"getaddrinfo failed: " + std::to_string(result)};
#else
        throw std::runtime_error{std::string{"getaddrinfo failed: "} + gai_strerror(result)};
#endif
    }

    AddrInfoGuard addresses{raw_info};

    for (addrinfo* addr = addresses.get(); addr != nullptr; addr = addr->ai_next) {
        SocketGuard listener{socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)};
        if (listener.get() == invalid_socket) {
            continue;
        }

        int reuse = 1;
        setsockopt(listener.get(), SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));

        if (bind(listener.get(), addr->ai_addr, static_cast<int>(addr->ai_addrlen)) != 0) {
            continue;
        }

        if (listen(listener.get(), options.backlog) != 0) {
            continue;
        }

        return listener;
    }

    throw std::runtime_error{socket_error_message("bind/listen failed")};
}

} // namespace

TcpServer::TcpServer(Store& store, Options options) : TcpServer(store, std::move(options), nullptr) {}

TcpServer::TcpServer(Store& store, Options options, WriteAheadLog* wal)
    : store_(store), options_(std::move(options)), wal_(wal),
      connection_tracker_(std::make_shared<TcpServerConnectionTracker>()),
      command_metrics_tracker_(std::make_shared<CommandMetricsTracker>()) {}

void TcpServer::request_stop() { stop_requested_.store(true); }

bool TcpServer::stop_requested() const {
    return stop_requested_.load() || (options_.should_stop && options_.should_stop());
}

std::uint16_t TcpServer::bound_port() const { return bound_port_.load(); }

TcpServerConnectionStats TcpServer::connection_stats() const { return connection_tracker_->stats(); }

CommandProcessorMetrics TcpServer::command_metrics() const { return command_metrics_tracker_->stats(); }

void TcpServer::run() {
    NetworkRuntime runtime;
    (void)runtime;
    SocketGuard listener = bind_listener(options_);
    const auto actual_port = socket_bound_port(listener.get());
    const auto server_started_at = std::chrono::steady_clock::now();
    bound_port_.store(actual_port);
    const std::string endpoint = endpoint_fields(options_, actual_port);
    log_event(options_,
              "event=tcp_listen " + endpoint + " max_request_bytes=" + std::to_string(options_.max_request_bytes));
    const bool metrics_enabled = metrics_logging_enabled(options_);
    auto next_metrics_log = std::chrono::steady_clock::now() + options_.metrics_log_interval;

    while (!stop_requested()) {
        if (metrics_enabled) {
            const auto now = std::chrono::steady_clock::now();
            if (now >= next_metrics_log) {
                const auto connection_stats = connection_tracker_->stats();
                const auto command_metrics = command_metrics_tracker_->stats();
                export_metrics_event(
                    options_,
                    "event=server_metrics " + endpoint + " " +
                        metrics_fields(connection_stats, command_metrics, options_.metrics_log_interval),
                    metrics_json_line("server_metrics", options_, actual_port, connection_stats, command_metrics,
                                      options_.metrics_log_interval));
                do {
                    next_metrics_log += options_.metrics_log_interval;
                } while (next_metrics_log <= now);
            }
        }

        if (!wait_for_readable(listener.get(), listener_wait_interval(options_))) {
            continue;
        }

        if (stop_requested()) {
            break;
        }

        SocketHandle client = accept(listener.get(), nullptr, nullptr);
        if (client == invalid_socket) {
            if (stop_requested()) {
                break;
            }
            throw std::runtime_error{socket_error_message("accept failed")};
        }

        const auto snapshot = connection_tracker_->connected();
        log_event(options_, "event=tcp_client_accepted " + endpoint + " " + connection_fields(snapshot));
        CommandRuntimeInfo runtime_info;
        runtime_info.protocol = "inline,resp";
        runtime_info.started_at = server_started_at;
        runtime_info.max_request_bytes = options_.max_request_bytes;
        runtime_info.metrics_enabled = metrics_enabled;
        std::thread{serve_client,
                    std::ref(store_),
                    wal_,
                    client,
                    options_.logger,
                    endpoint,
                    connection_tracker_,
                    command_metrics_tracker_,
                    snapshot.connection_id,
                    options_.max_request_bytes,
                    options_.client_idle_timeout,
                    std::move(runtime_info),
                    options_.auto_compact_wal}
            .detach();
    }

    const auto connection_stats = connection_tracker_->stats();
    const auto command_metrics = command_metrics_tracker_->stats();
    export_metrics_event(
        options_,
        "event=tcp_stop " + endpoint + " " + stats_fields(connection_stats) + " " +
            command_metrics_fields(command_metrics),
        metrics_json_line("tcp_stop", options_, actual_port, connection_stats, command_metrics, std::nullopt));
}

} // namespace minikv
