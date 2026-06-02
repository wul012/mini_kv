#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"

#include "tcp_test_support.hpp"

#include <cassert>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace {

constexpr std::size_t client_count = 4;

using minikv::test_support::TestNetworkRuntime;
using minikv::test_support::TestSocketGuard;
using minikv::test_support::bulk_string;
using minikv::test_support::connect_test_socket;
using minikv::test_support::contains_log;
using minikv::test_support::make_resp_array;
using minikv::test_support::receive_all;
using minikv::test_support::send_all;
using minikv::test_support::shutdown_write;
using minikv::test_support::wait_for_log;

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
