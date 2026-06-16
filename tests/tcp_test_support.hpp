#pragma once

#include "minikv/tcp_server.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <exception>
#include <initializer_list>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

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
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace minikv::test_support {

#ifdef _WIN32
using TestSocket = SOCKET;
constexpr TestSocket invalid_test_socket = INVALID_SOCKET;

class TestNetworkRuntime {
public:
    TestNetworkRuntime() {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            throw std::runtime_error{"WSAStartup failed"};
        }
    }

    ~TestNetworkRuntime() { WSACleanup(); }
};

inline void close_test_socket(TestSocket socket) {
    if (socket != invalid_test_socket) {
        closesocket(socket);
    }
}

inline std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": WSA error " + std::to_string(WSAGetLastError());
}

inline std::string address_error_message(int result) { return "getaddrinfo failed: " + std::to_string(result); }

#else
using TestSocket = int;
constexpr TestSocket invalid_test_socket = -1;

class TestNetworkRuntime {
public:
    TestNetworkRuntime() = default;
};

inline void close_test_socket(TestSocket socket) {
    if (socket != invalid_test_socket) {
        close(socket);
    }
}

inline std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": " + std::strerror(errno);
}

inline std::string address_error_message(int result) {
    return std::string{"getaddrinfo failed: "} + gai_strerror(result);
}
#endif

class TestSocketGuard {
public:
    explicit TestSocketGuard(TestSocket socket = invalid_test_socket) : socket_(socket) {}

    ~TestSocketGuard() { close_test_socket(socket_); }

    TestSocketGuard(const TestSocketGuard&) = delete;
    TestSocketGuard& operator=(const TestSocketGuard&) = delete;

    TestSocketGuard(TestSocketGuard&& other) noexcept : socket_(other.release()) {}

    TestSocketGuard& operator=(TestSocketGuard&& other) noexcept {
        if (this != &other) {
            close_test_socket(socket_);
            socket_ = other.release();
        }
        return *this;
    }

    TestSocket get() const { return socket_; }

private:
    TestSocket release() {
        const TestSocket socket = socket_;
        socket_ = invalid_test_socket;
        return socket;
    }

    TestSocket socket_;
};

class TestAddrInfoGuard {
public:
    explicit TestAddrInfoGuard(addrinfo* info) : info_(info) {}

    ~TestAddrInfoGuard() {
        if (info_ != nullptr) {
            freeaddrinfo(info_);
        }
    }

    TestAddrInfoGuard(const TestAddrInfoGuard&) = delete;
    TestAddrInfoGuard& operator=(const TestAddrInfoGuard&) = delete;

    addrinfo* get() const { return info_; }

private:
    addrinfo* info_;
};

inline bool contains_log(const std::vector<std::string>& logs, std::string_view needle) {
    return std::any_of(logs.begin(), logs.end(),
                       [needle](const std::string& line) { return line.find(needle) != std::string::npos; });
}

inline std::string make_resp_array(std::initializer_list<std::string_view> arguments) {
    std::string encoded = "*" + std::to_string(arguments.size()) + "\r\n";
    for (const auto argument : arguments) {
        encoded += "$" + std::to_string(argument.size()) + "\r\n";
        encoded += argument;
        encoded += "\r\n";
    }
    return encoded;
}

inline std::string bulk_string(std::string_view value) {
    std::string response = "$" + std::to_string(value.size()) + "\r\n";
    response += value;
    response += "\r\n";
    return response;
}

inline TestSocketGuard connect_test_socket(std::string_view host, std::uint16_t port) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* raw_info = nullptr;
    const std::string port_text = std::to_string(port);
    const int result = getaddrinfo(std::string{host}.c_str(), port_text.c_str(), &hints, &raw_info);
    if (result != 0) {
        throw std::runtime_error{address_error_message(result)};
    }

    TestAddrInfoGuard addresses{raw_info};
    for (addrinfo* address = addresses.get(); address != nullptr; address = address->ai_next) {
        TestSocketGuard socket{::socket(address->ai_family, address->ai_socktype, address->ai_protocol)};
        if (socket.get() == invalid_test_socket) {
            continue;
        }

        if (connect(socket.get(), address->ai_addr, static_cast<int>(address->ai_addrlen)) == 0) {
            return socket;
        }
    }

    throw std::runtime_error{socket_error_message("connect failed")};
}

inline void send_all(TestSocket socket, std::string_view payload) {
    std::string_view remaining = payload;
    while (!remaining.empty()) {
#ifdef _WIN32
        const int sent = send(socket, remaining.data(), static_cast<int>(remaining.size()), 0);
#else
        const auto sent = send(socket, remaining.data(), remaining.size(), 0);
#endif
        if (sent <= 0) {
            throw std::runtime_error{socket_error_message("send failed")};
        }
        remaining.remove_prefix(static_cast<std::size_t>(sent));
    }
}

inline void shutdown_write(TestSocket socket) {
#ifdef _WIN32
    shutdown(socket, SD_SEND);
#else
    shutdown(socket, SHUT_WR);
#endif
}

inline std::string receive_all(TestSocket socket) {
    std::string response;
    char buffer[256]{};
    while (true) {
#ifdef _WIN32
        const int received = recv(socket, buffer, static_cast<int>(sizeof(buffer)), 0);
#else
        const auto received = recv(socket, buffer, sizeof(buffer), 0);
#endif
        if (received < 0) {
            throw std::runtime_error{socket_error_message("recv failed")};
        }
        if (received == 0) {
            break;
        }
        response.append(buffer, static_cast<std::size_t>(received));
    }

    return response;
}

inline std::string exchange_raw(std::string_view host, std::uint16_t port, std::string_view payload) {
    TestNetworkRuntime runtime;
    TestSocketGuard socket = connect_test_socket(host, port);

    send_all(socket.get(), payload);
    shutdown_write(socket.get());
    return receive_all(socket.get());
}

inline void wait_for_log(const std::vector<std::string>& logs, std::mutex& logs_mutex, std::string_view needle) {
    using namespace std::chrono_literals;

    bool found = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard lock{logs_mutex};
            found = contains_log(logs, needle);
        }
        if (found) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(found);
}

inline void wait_for_connection_total(const TcpServer& server, std::uint64_t total_connections) {
    using namespace std::chrono_literals;

    for (int attempt = 0; attempt < 100; ++attempt) {
        const auto stats = server.connection_stats();
        if (stats.total_connections == total_connections && stats.active_connections == 0) {
            return;
        }
        std::this_thread::sleep_for(10ms);
    }

    const auto stats = server.connection_stats();
    assert(stats.total_connections == total_connections);
    assert(stats.active_connections == 0);
}

class TestTcpServerHarness {
public:
    explicit TestTcpServerHarness(std::chrono::milliseconds client_idle_timeout = std::chrono::milliseconds{0})
        : options_(make_options(logs_, logs_mutex_, client_idle_timeout)), server_(store_, options_) {
        server_thread_ = std::thread{[this] {
            try {
                server_.run();
            } catch (...) {
                failure_ = std::current_exception();
            }
        }};

        wait_for_log(logs_, logs_mutex_, "event=tcp_listen");
        assert(bound_port() > 0);
    }

    ~TestTcpServerHarness() {
        if (!stopped_) {
            server_.request_stop();
            if (server_thread_.joinable()) {
                server_thread_.join();
            }
        }
    }

    TestTcpServerHarness(const TestTcpServerHarness&) = delete;
    TestTcpServerHarness& operator=(const TestTcpServerHarness&) = delete;

    Store& store() { return store_; }

    TcpServer& server() { return server_; }

    const TcpServer& server() const { return server_; }

    std::uint16_t bound_port() const { return server_.bound_port(); }

    bool contains_log(std::string_view needle) const {
        std::lock_guard lock{logs_mutex_};
        return minikv::test_support::contains_log(logs_, needle);
    }

    void stop() {
        server_.request_stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
        stopped_ = true;

        if (failure_) {
            std::rethrow_exception(failure_);
        }

        assert(server_.stop_requested());
    }

private:
    static TcpServer::Options make_options(std::vector<std::string>& logs, std::mutex& logs_mutex,
                                           std::chrono::milliseconds client_idle_timeout) {
        using namespace std::chrono_literals;

        TcpServer::Options options;
        options.host = "127.0.0.1";
        options.port = 0;
        options.accept_poll_interval = 10ms;
        options.client_idle_timeout = client_idle_timeout;
        options.logger = [&](const std::string& message) {
            std::lock_guard lock{logs_mutex};
            logs.push_back(message);
        };
        return options;
    }

    Store store_;
    mutable std::mutex logs_mutex_;
    std::vector<std::string> logs_;
    TcpServer::Options options_;
    TcpServer server_;
    std::exception_ptr failure_;
    std::thread server_thread_;
    bool stopped_ = false;
};

} // namespace minikv::test_support
