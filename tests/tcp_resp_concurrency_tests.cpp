#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"

#include <algorithm>
#include <cassert>
#include <atomic>
#include <chrono>
#include <cstddef>
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

constexpr std::size_t client_count = 4;

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

std::string bulk_string(std::string_view value) {
    std::string response = "$" + std::to_string(value.size()) + "\r\n";
    response += value;
    response += "\r\n";
    return response;
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

void send_all(TestSocket socket, std::string_view payload) {
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

void shutdown_write(TestSocket socket) {
#ifdef _WIN32
    shutdown(socket, SD_SEND);
#else
    shutdown(socket, SHUT_WR);
#endif
}

std::string receive_all(TestSocket socket) {
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

bool has_failures(const std::vector<std::string>& failures, std::mutex& failures_mutex) {
    std::lock_guard lock{failures_mutex};
    return !failures.empty();
}

void add_failure(std::vector<std::string>& failures, std::mutex& failures_mutex, std::string message) {
    std::lock_guard lock{failures_mutex};
    failures.push_back(std::move(message));
}

bool wait_for_concurrent_clients(const minikv::TcpServer& server,
                                 const std::atomic_size_t& ready_clients,
                                 const std::vector<std::string>& failures,
                                 std::mutex& failures_mutex) {
    using namespace std::chrono_literals;

    for (int attempt = 0; attempt < 200; ++attempt) {
        if (has_failures(failures, failures_mutex)) {
            return false;
        }

        const auto stats = server.connection_stats();
        if (ready_clients.load() == client_count &&
            stats.total_connections == client_count &&
            stats.active_connections == client_count) {
            return true;
        }

        std::this_thread::sleep_for(10ms);
    }

    return false;
}

void wait_for_closed_clients(const minikv::TcpServer& server) {
    using namespace std::chrono_literals;

    for (int attempt = 0; attempt < 200; ++attempt) {
        const auto stats = server.connection_stats();
        if (stats.total_connections == client_count && stats.active_connections == 0) {
            return;
        }
        std::this_thread::sleep_for(10ms);
    }

    const auto stats = server.connection_stats();
    assert(stats.total_connections == client_count);
    assert(stats.active_connections == 0);
}

std::string client_request(std::size_t index) {
    const std::string key = "worker:" + std::to_string(index);
    const std::string value = "value-" + std::to_string(index);
    const std::string ping_message = "client-" + std::to_string(index);

    return make_resp_array({"SET", key, value}) +
           make_resp_array({"GET", key}) +
           make_resp_array({"PING", ping_message}) +
           make_resp_array({"TTL", key}) +
           make_resp_array({"DEL", key}) +
           make_resp_array({"GET", key}) +
           make_resp_array({"QUIT"});
}

std::string client_expected_response(std::size_t index) {
    const std::string value = "value-" + std::to_string(index);
    const std::string ping_message = "client-" + std::to_string(index);

    return "+OK inserted\r\n" +
           bulk_string(value) +
           bulk_string(ping_message) +
           ":-1\r\n"
           ":1\r\n"
           "$-1\r\n"
           "+BYE\r\n";
}

void run_client(std::uint16_t port,
                std::size_t index,
                const std::atomic_bool& start,
                std::atomic_size_t& ready_clients,
                std::vector<std::string>& failures,
                std::mutex& failures_mutex) {
    using namespace std::chrono_literals;

    try {
        TestNetworkRuntime runtime;
        TestSocketGuard socket = connect_test_socket("127.0.0.1", port);
        ready_clients.fetch_add(1);

        while (!start.load()) {
            std::this_thread::sleep_for(1ms);
        }

        send_all(socket.get(), client_request(index));
        shutdown_write(socket.get());

        const auto response = receive_all(socket.get());
        if (response != client_expected_response(index)) {
            add_failure(failures, failures_mutex, "client " + std::to_string(index) + " response mismatch");
        }
    } catch (const std::exception& error) {
        add_failure(failures, failures_mutex, "client " + std::to_string(index) + " failed: " + error.what());
    }
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

    std::atomic_bool start{false};
    std::atomic_size_t ready_clients{0};
    std::mutex failures_mutex;
    std::vector<std::string> failures;
    std::vector<std::thread> clients;
    clients.reserve(client_count);

    for (std::size_t index = 0; index < client_count; ++index) {
        clients.emplace_back(run_client,
                             bound_port,
                             index,
                             std::cref(start),
                             std::ref(ready_clients),
                             std::ref(failures),
                             std::ref(failures_mutex));
    }

    const bool concurrent_clients_ready =
        wait_for_concurrent_clients(server, ready_clients, failures, failures_mutex);
    start.store(true);

    for (auto& client : clients) {
        client.join();
    }

    wait_for_closed_clients(server);

    server.request_stop();
    server_thread.join();

    if (failure) {
        std::rethrow_exception(failure);
    }

    assert(concurrent_clients_ready);
    assert(!has_failures(failures, failures_mutex));
    assert(server.stop_requested());

    const auto stats = server.connection_stats();
    assert(stats.total_connections == client_count);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == client_count);
    assert(store.size() == 0);

    std::lock_guard lock{logs_mutex};
    assert(contains_log(logs, "event=tcp_listen"));
    assert(contains_log(logs, "event=tcp_client_accepted"));
    assert(contains_log(logs, "event=tcp_client_closed"));
    assert(contains_log(logs, "event=tcp_stop"));
    assert(contains_log(logs, "total_connections=" + std::to_string(client_count)));
    assert(contains_log(logs, "peak_connections=" + std::to_string(client_count)));

    return 0;
}
