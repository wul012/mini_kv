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
using minikv::test_support::wait_for_connection_total;
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
