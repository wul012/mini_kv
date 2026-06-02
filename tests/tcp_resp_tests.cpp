#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"

#include "tcp_test_support.hpp"

#include <cassert>
#include <chrono>
#include <exception>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace {

using minikv::test_support::contains_log;
using minikv::test_support::exchange_raw;
using minikv::test_support::make_resp_array;
using minikv::test_support::wait_for_log;

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

    const std::string request = make_resp_array({"PING", "external"}) +
                                make_resp_array({"SET", "name", "resp-client"}) +
                                make_resp_array({"GET", "name"}) +
                                make_resp_array({"SIZE"}) +
                                make_resp_array({"QUIT"});

    const std::string expected = "$8\r\nexternal\r\n"
                                 "+OK inserted\r\n"
                                 "$11\r\nresp-client\r\n"
                                 ":1\r\n"
                                 "+BYE\r\n";
    const auto response = exchange_raw("127.0.0.1", bound_port, request);
    assert(response == expected);

    bool closed = false;
    for (int attempt = 0; attempt < 100; ++attempt) {
        {
            std::lock_guard lock{logs_mutex};
            closed = contains_log(logs, "event=tcp_client_closed");
        }
        const auto stats = server.connection_stats();
        if (closed && stats.active_connections == 0 && stats.total_connections == 1) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(closed);
    auto stats = server.connection_stats();
    assert(stats.total_connections == 1);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    server.request_stop();
    server_thread.join();

    if (failure) {
        std::rethrow_exception(failure);
    }

    assert(server.stop_requested());
    stats = server.connection_stats();
    assert(stats.total_connections == 1);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    std::lock_guard lock{logs_mutex};
    assert(contains_log(logs, "event=tcp_listen"));
    assert(contains_log(logs, "event=tcp_client_accepted"));
    assert(contains_log(logs, "event=tcp_client_closed"));
    assert(contains_log(logs, "event=tcp_stop"));
    assert(contains_log(logs, "total_connections=1"));
    assert(contains_log(logs, "peak_connections=1"));

    return 0;
}
