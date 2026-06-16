#include "tcp_test_support.hpp"

#include <cassert>
#include <chrono>
#include <cstddef>
#include <string>

namespace {

using minikv::test_support::connect_test_socket;
using minikv::test_support::exchange_raw;
using minikv::test_support::receive_all;
using minikv::test_support::send_all;
using minikv::test_support::TestNetworkRuntime;
using minikv::test_support::TestSocketGuard;
using minikv::test_support::TestTcpServerHarness;
using minikv::test_support::wait_for_connection_total;

constexpr std::size_t sequential_connection_count = 100;

} // namespace

int main() {
    using namespace std::chrono_literals;

    TestTcpServerHarness harness{50ms};
    const auto bound_port = harness.bound_port();

    {
        TestNetworkRuntime runtime;
        TestSocketGuard idle_socket = connect_test_socket("127.0.0.1", bound_port);
        const auto idle_response = receive_all(idle_socket.get());
        assert(idle_response.empty());
    }

    wait_for_connection_total(harness.server(), 1);
    assert(harness.contains_log("reason=client_idle_timeout"));
    assert(harness.contains_log("client_idle_timeout_ms=50"));

    {
        TestNetworkRuntime runtime;
        TestSocketGuard partial_socket = connect_test_socket("127.0.0.1", bound_port);
        send_all(partial_socket.get(), "PING without-newline");
        const auto timeout_response = receive_all(partial_socket.get());
        assert(timeout_response.find("ERR command timeout\n") != std::string::npos);
    }

    wait_for_connection_total(harness.server(), 2);
    assert(harness.contains_log("reason=command_timeout"));
    assert(harness.contains_log("pending_bytes=20"));

    for (std::size_t index = 0; index < sequential_connection_count; ++index) {
        const std::string message = "seq-" + std::to_string(index);
        const auto response = exchange_raw("127.0.0.1", bound_port, "PING " + message + "\nQUIT\n");
        assert(response.find(message + "\n") != std::string::npos);
        assert(response.find("BYE\n") != std::string::npos);
    }

    wait_for_connection_total(harness.server(), 2 + sequential_connection_count);

    harness.stop();

    const auto stats = harness.server().connection_stats();
    assert(stats.total_connections == 2 + sequential_connection_count);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections >= 1);
    assert(harness.contains_log("event=tcp_client_closed"));
    assert(harness.contains_log("total_connections=" + std::to_string(2 + sequential_connection_count)));

    return 0;
}
