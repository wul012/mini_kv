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
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <cerrno>
#include <netdb.h>
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

std::string address_error_message(int result) {
    return "getaddrinfo failed: " + std::to_string(result);
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

std::string address_error_message(int result) {
    return std::string{"getaddrinfo failed: "} + gai_strerror(result);
}
#endif

class TestSocketGuard {
public:
    explicit TestSocketGuard(TestSocket socket = invalid_test_socket) : socket_(socket) {}

    ~TestSocketGuard() {
        close_test_socket(socket_);
    }

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

    TestSocket get() const {
        return socket_;
    }

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

    addrinfo* get() const {
        return info_;
    }

private:
    addrinfo* info_;
};

bool contains_log(const std::vector<std::string>& logs, std::string_view needle) {
    return std::any_of(logs.begin(), logs.end(), [needle](const std::string& line) {
        return line.find(needle) != std::string::npos;
    });
}

const minikv::CommandBreakdownMetrics* find_command_metrics(const minikv::CommandProcessorMetrics& metrics,
                                                            std::string_view command) {
    for (const auto& command_metrics : metrics.command_breakdown) {
        if (command_metrics.command == command) {
            return &command_metrics;
        }
    }
    return nullptr;
}

TestSocketGuard connect_test_socket(std::string_view host, std::uint16_t port) {
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

std::string exchange_inline(std::string_view host, std::uint16_t port, std::string_view payload) {
    TestNetworkRuntime runtime;
    TestSocketGuard socket = connect_test_socket(host, port);

    std::string_view remaining = payload;
    while (!remaining.empty()) {
#ifdef _WIN32
        const int sent = send(socket.get(), remaining.data(), static_cast<int>(remaining.size()), 0);
#else
        const auto sent = send(socket.get(), remaining.data(), remaining.size(), 0);
#endif
        if (sent <= 0) {
            throw std::runtime_error{socket_error_message("send failed")};
        }
        remaining.remove_prefix(static_cast<std::size_t>(sent));
    }

#ifdef _WIN32
    shutdown(socket.get(), SD_SEND);
#else
    shutdown(socket.get(), SHUT_WR);
#endif

    std::string response;
    char buffer[256]{};
    while (true) {
#ifdef _WIN32
        const int received = recv(socket.get(), buffer, static_cast<int>(sizeof(buffer)), 0);
#else
        const auto received = recv(socket.get(), buffer, sizeof(buffer), 0);
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

} // namespace

int main() {
    using namespace std::chrono_literals;

    minikv::Store store;
    minikv::TcpServer::Options options;
    options.host = "127.0.0.1";
    options.port = 0;
    options.accept_poll_interval = 10ms;
    options.metrics_log_interval = 20ms;
    options.max_request_bytes = 8;

    std::mutex logs_mutex;
    std::vector<std::string> logs;
    std::vector<std::string> metrics_exports;
    options.logger = [&](const std::string& message) {
        std::lock_guard lock{logs_mutex};
        logs.push_back(message);
    };
    options.metrics_exporter = [&](const std::string& message) {
        std::lock_guard lock{logs_mutex};
        metrics_exports.push_back(message);
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

    bool metrics_logged = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard lock{logs_mutex};
            metrics_logged = contains_log(logs, "event=server_metrics");
        }
        if (metrics_logged) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(metrics_logged);

    const auto response = exchange_inline("127.0.0.1", bound_port, "PING\nSTATS\nHEALTH\nQUIT\n");
    assert(response.find("PONG\n") != std::string::npos);
    assert(response.find("connection_stats_available=yes") != std::string::npos);
    assert(response.find("active_connections=1") != std::string::npos);
    assert(response.find("total_connections=1") != std::string::npos);
    assert(response.find("peak_connections=1") != std::string::npos);
    assert(response.find("total_commands=1") != std::string::npos);
    assert(response.find("successful_commands=1") != std::string::npos);
    assert(response.find("error_commands=0") != std::string::npos);
    assert(response.find("total_latency_ns=") != std::string::npos);
    assert(response.find("avg_latency_ns=") != std::string::npos);
    assert(response.find("max_latency_ns=") != std::string::npos);
    assert(response.find("command_breakdown=PING:1/1/0/") != std::string::npos);
    assert(response.find("OK live_keys=0") != std::string::npos);
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
    auto command_metrics = server.command_metrics();
    assert(command_metrics.total_commands == 4);
    assert(command_metrics.successful_commands == 4);
    assert(command_metrics.error_commands == 0);

    const auto too_long_response = exchange_inline("127.0.0.1", bound_port, "0123456789");
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
    command_metrics = server.command_metrics();
    assert(command_metrics.total_commands == 4);
    assert(command_metrics.successful_commands == 4);
    assert(command_metrics.error_commands == 0);
    assert(command_metrics.total_latency_ns > 0);
    assert(command_metrics.max_latency_ns > 0);
    assert(find_command_metrics(command_metrics, "PING") != nullptr);
    assert(find_command_metrics(command_metrics, "STATS") != nullptr);
    assert(find_command_metrics(command_metrics, "HEALTH") != nullptr);
    assert(find_command_metrics(command_metrics, "QUIT") != nullptr);

    const auto reset_response = exchange_inline("127.0.0.1",
                                                bound_port,
                                                "STATSJSON\nRESETSTATS\nSTATS\nQUIT\n");
    assert(reset_response.find("\"connection_stats\":{\"available\":true") != std::string::npos);
    assert(reset_response.find("\"active_connections\":1") != std::string::npos);
    assert(reset_response.find("\"total_connections\":3") != std::string::npos);
    assert(reset_response.find("\"commands\":{\"total_commands\":4") != std::string::npos);
    assert(reset_response.find("OK stats reset\n") != std::string::npos);
    assert(reset_response.find("total_commands=0") != std::string::npos);
    assert(reset_response.find("successful_commands=0") != std::string::npos);
    assert(reset_response.find("error_commands=0") != std::string::npos);

    bool reset_closed = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard lock{logs_mutex};
            reset_closed = contains_log(logs, "connection_id=3") &&
                           contains_log(logs, "event=tcp_client_closed");
        }
        stats = server.connection_stats();
        if (reset_closed && stats.active_connections == 0 && stats.total_connections == 3) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(reset_closed);
    stats = server.connection_stats();
    assert(stats.total_connections == 3);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);
    command_metrics = server.command_metrics();
    assert(command_metrics.total_commands == 2);
    assert(command_metrics.successful_commands == 2);
    assert(command_metrics.error_commands == 0);
    assert(find_command_metrics(command_metrics, "STATS") != nullptr);
    assert(find_command_metrics(command_metrics, "QUIT") != nullptr);
    assert(find_command_metrics(command_metrics, "RESETSTATS") == nullptr);
    assert(find_command_metrics(command_metrics, "PING") == nullptr);

    server.request_stop();
    server_thread.join();

    if (failure) {
        std::rethrow_exception(failure);
    }

    assert(server.stop_requested());

    minikv::Store jsonl_store;
    minikv::TcpServer::Options jsonl_options;
    jsonl_options.host = "127.0.0.1";
    jsonl_options.port = 0;
    jsonl_options.accept_poll_interval = 10ms;
    jsonl_options.metrics_log_interval = 20ms;
    jsonl_options.metrics_export_format = minikv::MetricsExportFormat::jsonl;

    std::mutex jsonl_logs_mutex;
    std::vector<std::string> jsonl_logs;
    std::vector<std::string> jsonl_exports;
    jsonl_options.logger = [&](const std::string& message) {
        std::lock_guard jsonl_lock{jsonl_logs_mutex};
        jsonl_logs.push_back(message);
    };
    jsonl_options.metrics_exporter = [&](const std::string& message) {
        std::lock_guard jsonl_lock{jsonl_logs_mutex};
        jsonl_exports.push_back(message);
    };

    minikv::TcpServer jsonl_server{jsonl_store, jsonl_options};
    std::exception_ptr jsonl_failure;
    std::thread jsonl_thread{[&] {
        try {
            jsonl_server.run();
        } catch (...) {
            jsonl_failure = std::current_exception();
        }
    }};

    bool jsonl_started = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard jsonl_lock{jsonl_logs_mutex};
            jsonl_started = contains_log(jsonl_logs, "event=tcp_listen");
        }
        if (jsonl_started) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(jsonl_started);
    const auto jsonl_port = jsonl_server.bound_port();
    assert(jsonl_port > 0);

    bool jsonl_metrics_logged = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard jsonl_lock{jsonl_logs_mutex};
            jsonl_metrics_logged = contains_log(jsonl_logs, "event=server_metrics") &&
                                   contains_log(jsonl_exports, "\"event\":\"server_metrics\"");
        }
        if (jsonl_metrics_logged) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }
    assert(jsonl_metrics_logged);

    const auto jsonl_response = exchange_inline("127.0.0.1", jsonl_port, "PING jsonl\nQUIT\n");
    assert(jsonl_response.find("jsonl\n") != std::string::npos);
    assert(jsonl_response.find("BYE\n") != std::string::npos);

    jsonl_server.request_stop();
    jsonl_thread.join();

    if (jsonl_failure) {
        std::rethrow_exception(jsonl_failure);
    }

    {
        std::lock_guard jsonl_lock{jsonl_logs_mutex};
        assert(contains_log(jsonl_logs, "event=server_metrics"));
        assert(contains_log(jsonl_logs, "metrics_interval_ms=20"));
        assert(contains_log(jsonl_logs, "event=tcp_stop"));
        assert(contains_log(jsonl_exports, "\"event\":\"server_metrics\""));
        assert(contains_log(jsonl_exports, "\"event\":\"tcp_stop\""));
        assert(contains_log(jsonl_exports, "\"host\":\"127.0.0.1\""));
        assert(contains_log(jsonl_exports, "\"port\":"));
        assert(contains_log(jsonl_exports, "\"connection_stats\":{\"active_connections\""));
        assert(contains_log(jsonl_exports, "\"commands\":{\"total_commands\""));
        assert(contains_log(jsonl_exports, "\"metrics_interval_ms\":20"));
        assert(contains_log(jsonl_exports, "\"command\":\"PING\""));
        assert(!contains_log(jsonl_exports, "event=server_metrics"));
    }

    std::lock_guard lock{logs_mutex};
    assert(contains_log(logs, "event=tcp_listen"));
    assert(contains_log(logs, "event=tcp_client_accepted"));
    assert(contains_log(logs, "event=tcp_client_closed"));
    assert(contains_log(logs, "event=tcp_request_rejected"));
    assert(contains_log(logs, "connection_id=1"));
    assert(contains_log(logs, "connection_id=2"));
    assert(contains_log(logs, "connection_id=3"));
    assert(contains_log(logs, "active_connections=1"));
    assert(contains_log(logs, "active_connections=0"));
    assert(contains_log(logs, "pending_bytes=10"));
    assert(contains_log(logs, "max_request_bytes=8"));
    assert(contains_log(logs, "total_connections=3"));
    assert(contains_log(logs, "peak_connections=1"));
    assert(contains_log(logs, "event=server_metrics"));
    assert(contains_log(logs, "metrics_interval_ms=20"));
    assert(contains_log(logs, "total_commands="));
    assert(contains_log(logs, "error_commands="));
    assert(contains_log(logs, "avg_latency_ns="));
    assert(contains_log(logs, "command_breakdown="));
    assert(contains_log(logs, "event=tcp_stop"));
    assert(contains_log(metrics_exports, "event=server_metrics"));
    assert(contains_log(metrics_exports, "metrics_interval_ms=20"));
    assert(contains_log(metrics_exports, "event=tcp_stop"));
    assert(!contains_log(metrics_exports, "event=tcp_client_accepted"));

    minikv::Store localhost_store;
    minikv::TcpServer::Options localhost_options;
    localhost_options.host = "localhost";
    localhost_options.port = 0;
    localhost_options.accept_poll_interval = 10ms;

    std::mutex localhost_logs_mutex;
    std::vector<std::string> localhost_logs;
    localhost_options.logger = [&](const std::string& message) {
        std::lock_guard localhost_lock{localhost_logs_mutex};
        localhost_logs.push_back(message);
    };

    minikv::TcpServer localhost_server{localhost_store, localhost_options};
    std::exception_ptr localhost_failure;
    std::thread localhost_thread{[&] {
        try {
            localhost_server.run();
        } catch (...) {
            localhost_failure = std::current_exception();
        }
    }};

    bool localhost_started = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard localhost_lock{localhost_logs_mutex};
            localhost_started = contains_log(localhost_logs, "event=tcp_listen");
        }
        if (localhost_started) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(localhost_started);
    const auto localhost_port = localhost_server.bound_port();
    assert(localhost_port > 0);
    const auto localhost_response = exchange_inline("localhost", localhost_port, "PING localhost\nQUIT\n");
    assert(localhost_response.find("localhost\n") != std::string::npos);
    assert(localhost_response.find("BYE\n") != std::string::npos);

    localhost_server.request_stop();
    localhost_thread.join();

    if (localhost_failure) {
        std::rethrow_exception(localhost_failure);
    }

    const auto localhost_stats = localhost_server.connection_stats();
    assert(localhost_stats.total_connections == 1);
    assert(localhost_stats.active_connections == 0);
    assert(localhost_stats.peak_connections == 1);

    std::lock_guard localhost_lock{localhost_logs_mutex};
    assert(contains_log(localhost_logs, "event=tcp_listen"));
    assert(contains_log(localhost_logs, "host=localhost"));
    assert(contains_log(localhost_logs, "event=tcp_client_accepted"));
    assert(contains_log(localhost_logs, "event=tcp_client_closed"));
    assert(contains_log(localhost_logs, "event=tcp_stop"));

    return 0;
}
