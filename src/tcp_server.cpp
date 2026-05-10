#include "minikv/tcp_server.hpp"

#include "minikv/command.hpp"
#include "minikv/resp.hpp"
#include "minikv/wal.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <cerrno>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace minikv {
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

    ~NetworkRuntime() {
        WSACleanup();
    }

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

std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": " + std::strerror(errno);
}
#endif

void log_event(const TcpServer::Options& options, const std::string& message) {
    if (options.logger) {
        options.logger(message);
    }
}

std::string endpoint_fields(const TcpServer::Options& options) {
    return "host=" + options.host + " port=" + std::to_string(options.port);
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

bool wait_for_listener(SocketHandle listener, std::chrono::milliseconds timeout) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(listener, &read_fds);

    timeval wait_time = to_timeval(timeout);
#ifdef _WIN32
    const int result = select(0, &read_fds, nullptr, nullptr, &wait_time);
#else
    const int result = select(listener + 1, &read_fds, nullptr, nullptr, &wait_time);
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

    return result > 0 && FD_ISSET(listener, &read_fds);
}

class SocketGuard {
public:
    explicit SocketGuard(SocketHandle socket = invalid_socket) : socket_(socket) {}

    ~SocketGuard() {
        close_socket(socket_);
    }

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

    SocketHandle get() const {
        return socket_;
    }

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

    addrinfo* get() const {
        return info_;
    }

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

void serve_client(Store& store, WriteAheadLog* wal, SocketHandle client_socket) {
    SocketGuard client{client_socket};
    CommandProcessor processor{store, wal};
    std::string pending;
    std::array<char, 4096> buffer{};

    while (true) {
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

        if (pending.size() > 64 * 1024) {
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
    : store_(store), options_(std::move(options)), wal_(wal) {}

void TcpServer::request_stop() {
    stop_requested_.store(true);
}

bool TcpServer::stop_requested() const {
    return stop_requested_.load() || (options_.should_stop && options_.should_stop());
}

void TcpServer::run() {
    NetworkRuntime runtime;
    SocketGuard listener = bind_listener(options_);
    log_event(options_, "event=tcp_listen " + endpoint_fields(options_));

    while (!stop_requested()) {
        if (!wait_for_listener(listener.get(), options_.accept_poll_interval)) {
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

        log_event(options_, "event=tcp_client_accepted " + endpoint_fields(options_));
        std::thread{serve_client, std::ref(store_), wal_, client}.detach();
    }

    log_event(options_, "event=tcp_stop " + endpoint_fields(options_));
}

} // namespace minikv
