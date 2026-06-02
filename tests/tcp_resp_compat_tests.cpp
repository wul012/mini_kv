#include "tcp_test_support.hpp"

#include <cassert>
#include <string>

namespace {

using minikv::test_support::TestTcpServerHarness;
using minikv::test_support::exchange_raw;
using minikv::test_support::make_resp_array;
using minikv::test_support::wait_for_connection_total;

} // namespace

int main() {
    TestTcpServerHarness harness;
    const auto bound_port = harness.bound_port();

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
    wait_for_connection_total(harness.server(), 1);

    const std::string protocol_error_response = exchange_raw("127.0.0.1", bound_port, "*1\r\n$-1\r\n");
    assert(protocol_error_response == "-ERR null RESP bulk strings are not supported\r\n");
    wait_for_connection_total(harness.server(), 2);

    harness.stop();
    const auto stats = harness.server().connection_stats();
    assert(stats.total_connections == 2);
    assert(stats.active_connections == 0);
    assert(stats.peak_connections == 1);

    assert(harness.contains_log("event=tcp_listen"));
    assert(harness.contains_log("event=tcp_client_accepted"));
    assert(harness.contains_log("event=tcp_client_closed"));
    assert(harness.contains_log("event=tcp_stop"));
    assert(harness.contains_log("total_connections=2"));
    assert(harness.contains_log("peak_connections=1"));

    return 0;
}
