#include "minikv/store.hpp"
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

std::string make_resp_array(std::initializer_list<std::string_view> arguments) {
    std::string encoded = "*" + std::to_string(arguments.size()) + "\r\n";
    for (const auto argument : arguments) {
        encoded += "$" + std::to_string(argument.size()) + "\r\n";
        encoded += argument;
        encoded += "\r\n";
    }
    return encoded;
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

std::string exchange_raw(std::string_view host, std::uint16_t port, std::string_view payload) {
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

void wait_for_log(const std::vector<std::string>& logs,
                  std::mutex& logs_mutex,
                  std::string_view needle) {
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

void wait_for_connection_total(const minikv::TcpServer& server, std::uint64_t total_connections) {
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

} // namespace

int main() {
    using namespace std::chrono_literals;

    minikv::Store store;
    minikv::TcpServer::Options options;
    options.host = "127.0.0.1";
    options.port = 0;
    options.accept_poll_interval = 10ms;

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

    wait_for_log(logs, logs_mutex, "event=tcp_listen");
    const auto bound_port = server.bound_port();
    assert(bound_port > 0);

    const std::string compatibility_request =
        make_resp_array({"GET", "missing"}) +
        make_resp_array({"SET", "durable", "value"}) +
        make_resp_array({"TTL", "durable"}) +
        make_resp_array({"EXPIRE", "durable", "120"}) +
        make_resp_array({"DEL", "durable"}) +
        make_resp_array({"TTL", "durable"}) +
        make_resp_array({"NOPE"}) +
        make_resp_array({"GET", "durable", "extra"}) +
        make_resp_array({"QUIT"});

    const std::string compatibility_expected = "$-1\r\n"
                                               "+OK inserted\r\n"
                                               ":-1\r\n"
                                               ":1\r\n"
                                               ":1\r\n"
                                               ":-2\r\n"
                                               "-ERR unknown command\r\n"
                                               "-ERR usage: GET key\r\n"
                                               "+BYE\r\n";
    const auto compatibility_response = exchange_raw("127.0.0.1", bound_port, compatibility_request);
    assert(compatibility_response == compatibility_expected);
    wait_for_connection_total(server, 1);

    const std::string protocol_error_response = exchange_raw("127.0.0.1", bound_port, "*1\r\n$-1\r\n");
    assert(protocol_error_response == "-ERR null RESP bulk strings are not supported\r\n");
    wait_for_connection_total(server, 2);

    server.request_stop();
    server_thread.join();

    if (failure) {
        std::rethrow_exception(failure);
    }

    assert(server.stop_requested());
    const auto stats = server.connection_stats();
    assert(stats.total_connections == 2);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    std::lock_guard lock{logs_mutex};
    assert(contains_log(logs, "event=tcp_listen"));
    assert(contains_log(logs, "event=tcp_client_accepted"));
    assert(contains_log(logs, "event=tcp_client_closed"));
    assert(contains_log(logs, "event=tcp_stop"));
    assert(contains_log(logs, "total_connections=2"));
    assert(contains_log(logs, "peak_connections=1"));

    return 0;
}
