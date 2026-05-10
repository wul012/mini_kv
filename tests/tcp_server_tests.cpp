#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <exception>
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
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace {

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

    ~TestNetworkRuntime() {
        WSACleanup();
    }
};

void close_test_socket(TestSocket socket) {
    if (socket != invalid_test_socket) {
        closesocket(socket);
    }
}

std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": WSA error " + std::to_string(WSAGetLastError());
}

#else
using TestSocket = int;
constexpr TestSocket invalid_test_socket = -1;

class TestNetworkRuntime {
public:
    TestNetworkRuntime() = default;
};

void close_test_socket(TestSocket socket) {
    if (socket != invalid_test_socket) {
        close(socket);
    }
}

std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": " + std::strerror(errno);
}
#endif

bool contains_log(const std::vector<std::string>& logs, std::string_view needle) {
    return std::any_of(logs.begin(), logs.end(), [needle](const std::string& line) {
        return line.find(needle) != std::string::npos;
    });
}

std::string exchange_inline(std::uint16_t port, std::string_view payload) {
    TestNetworkRuntime runtime;
    TestSocket socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket == invalid_test_socket) {
        throw std::runtime_error{socket_error_message("socket failed")};
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) != 1) {
        close_test_socket(socket);
        throw std::runtime_error{"inet_pton failed"};
    }

    if (connect(socket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
        close_test_socket(socket);
        throw std::runtime_error{socket_error_message("connect failed")};
    }

    std::string_view remaining = payload;
    while (!remaining.empty()) {
#ifdef _WIN32
        const int sent = send(socket, remaining.data(), static_cast<int>(remaining.size()), 0);
#else
        const auto sent = send(socket, remaining.data(), remaining.size(), 0);
#endif
        if (sent <= 0) {
            close_test_socket(socket);
            throw std::runtime_error{socket_error_message("send failed")};
        }
        remaining.remove_prefix(static_cast<std::size_t>(sent));
    }

#ifdef _WIN32
    shutdown(socket, SD_SEND);
#else
    shutdown(socket, SHUT_WR);
#endif

    std::string response;
    char buffer[256]{};
    while (true) {
#ifdef _WIN32
        const int received = recv(socket, buffer, static_cast<int>(sizeof(buffer)), 0);
#else
        const auto received = recv(socket, buffer, sizeof(buffer), 0);
#endif
        if (received < 0) {
            close_test_socket(socket);
            throw std::runtime_error{socket_error_message("recv failed")};
        }
        if (received == 0) {
            break;
        }
        response.append(buffer, static_cast<std::size_t>(received));
    }

    close_test_socket(socket);
    return response;
}

} // namespace

int main() {
    using namespace std::chrono_literals;

    minikv::Store store;
    minikv::TcpServer::Options options;
    options.host = "127.0.0.1";
    options.port = 0;
    options.accept_poll_interval = 10ms;
    options.max_request_bytes = 8;

    std::mutex logs_mutex;
    std::vector<std::string> logs;
    options.logger = [&](const std::string& message) {
        std::lock_guard lock{logs_mutex};
        logs.push_back(message);
    };

    minikv::TcpServer server{store, options};
    std::exception_ptr failure;
    std::thread server_thread{[&] {
        try {
            server.run();
        } catch (...) {
            failure = std::current_exception();
        }
    }};

    bool started = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard lock{logs_mutex};
            started = contains_log(logs, "event=tcp_listen");
        }
        if (started) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(started);
    const auto bound_port = server.bound_port();
    assert(bound_port > 0);
    assert(!server.stop_requested());
    auto stats = server.connection_stats();
    assert(stats.total_connections == 0);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 0);

    const auto response = exchange_inline(bound_port, "PING\nQUIT\n");
    assert(response.find("PONG\n") != std::string::npos);
    assert(response.find("BYE\n") != std::string::npos);

    bool closed = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard lock{logs_mutex};
            closed = contains_log(logs, "event=tcp_client_closed");
        }
        stats = server.connection_stats();
        if (closed && stats.active_connections == 0 && stats.total_connections == 1) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(closed);
    stats = server.connection_stats();
    assert(stats.total_connections == 1);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    const auto too_long_response = exchange_inline(bound_port, "0123456789");
    assert(too_long_response.find("ERR line too long\n") != std::string::npos);

    bool rejected = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard lock{logs_mutex};
            rejected = contains_log(logs, "event=tcp_request_rejected");
        }
        stats = server.connection_stats();
        if (rejected && stats.active_connections == 0 && stats.total_connections == 2) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(rejected);
    stats = server.connection_stats();
    assert(stats.total_connections == 2);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    server.request_stop();
    server_thread.join();

    if (failure) {
        std::rethrow_exception(failure);
    }

    assert(server.stop_requested());

    std::lock_guard lock{logs_mutex};
    assert(contains_log(logs, "event=tcp_listen"));
    assert(contains_log(logs, "event=tcp_client_accepted"));
    assert(contains_log(logs, "event=tcp_client_closed"));
    assert(contains_log(logs, "event=tcp_request_rejected"));
    assert(contains_log(logs, "connection_id=1"));
    assert(contains_log(logs, "connection_id=2"));
    assert(contains_log(logs, "active_connections=1"));
    assert(contains_log(logs, "active_connections=0"));
    assert(contains_log(logs, "pending_bytes=10"));
    assert(contains_log(logs, "max_request_bytes=8"));
    assert(contains_log(logs, "total_connections=2"));
    assert(contains_log(logs, "peak_connections=1"));
    assert(contains_log(logs, "event=tcp_stop"));

    return 0;
}
