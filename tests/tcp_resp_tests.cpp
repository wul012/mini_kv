#include "tcp_test_support.hpp"

#include <cassert>
#include <chrono>
#include <string>
#include <thread>

namespace {

using minikv::test_support::TestTcpServerHarness;
using minikv::test_support::exchange_raw;
using minikv::test_support::make_resp_array;

} // namespace

int main() {
    using namespace std::chrono_literals;

    TestTcpServerHarness harness;
    const auto bound_port = harness.bound_port();

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
            closed = harness.contains_log("event=tcp_client_closed");
        }
        const auto stats = harness.server().connection_stats();
        if (closed && stats.active_connections == 0 && stats.total_connections == 1) {
            break;
        }
        std::this_thread::sleep_for(10ms);
    }

    assert(closed);
    auto stats = harness.server().connection_stats();
    assert(stats.total_connections == 1);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    harness.stop();
    stats = harness.server().connection_stats();
    assert(stats.total_connections == 1);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    assert(harness.contains_log("event=tcp_listen"));
    assert(harness.contains_log("event=tcp_client_accepted"));
    assert(harness.contains_log("event=tcp_client_closed"));
    assert(harness.contains_log("event=tcp_stop"));
    assert(harness.contains_log("total_connections=1"));
    assert(harness.contains_log("peak_connections=1"));

    return 0;
}
