#include "minikv/command.hpp"
#include "minikv/resp.hpp"
#include "minikv/store.hpp"

#include <cassert>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::string make_resp_array(std::initializer_list<std::string_view> arguments) {
    std::string encoded = "*" + std::to_string(arguments.size()) + "\r\n";
    for (const auto argument : arguments) {
        encoded += "$" + std::to_string(argument.size()) + "\r\n";
        encoded += argument;
        encoded += "\r\n";
    }
    return encoded;
}

void assert_complete(const minikv::RespParseResult& result) {
    assert(result.status == minikv::RespParseStatus::complete);
    assert(result.error.empty());
}

void assert_error(const minikv::RespParseResult& result) {
    assert(result.status == minikv::RespParseStatus::error);
    assert(!result.error.empty());
}

} // namespace

int main() {
    const auto set_request = make_resp_array({"SET", "name", "mini-kv"});
    const auto set_result = minikv::RespParser::parse_command(set_request);
    assert_complete(set_result);
    const std::vector<std::string> expected_set{"SET", "name", "mini-kv"};
    assert(set_result.command.arguments == expected_set);
    assert(set_result.command.consumed == set_request.size());
    assert(minikv::RespParser::to_inline_command(set_result.command) == "SET name mini-kv");
    assert(minikv::RespParser::to_resp_response("OK inserted") == "+OK inserted\r\n");
    assert(minikv::RespParser::to_resp_response("mini-kv") == "$7\r\nmini-kv\r\n");
    assert(minikv::RespParser::to_resp_response("(nil)") == "$-1\r\n");
    assert(minikv::RespParser::to_resp_response("2") == ":2\r\n");
    assert(minikv::RespParser::to_resp_response("-2") == ":-2\r\n");
    assert(minikv::RespParser::to_resp_response("ERR unknown command") == "-ERR unknown command\r\n");

    minikv::Store store;
    minikv::CommandProcessor processor{store};
    auto response = processor.execute(minikv::RespParser::to_inline_command(set_result.command));
    assert(response.response == "OK inserted");

    const auto spaced_request = make_resp_array({"SET", "phrase", "hello from resp wire"});
    const auto spaced_result = minikv::RespParser::parse_command(spaced_request);
    assert_complete(spaced_result);
    const std::vector<std::string> expected_spaced{"SET", "phrase", "hello from resp wire"};
    assert(spaced_result.command.arguments == expected_spaced);
    response = processor.execute(minikv::RespParser::to_inline_command(spaced_result.command));
    assert(response.response == "OK inserted");
    response = processor.execute("GET phrase");
    assert(response.response == "hello from resp wire");

    const auto pipelined = make_resp_array({"GET", "name"}) + make_resp_array({"SIZE"});
    const auto first = minikv::RespParser::parse_command(pipelined);
    assert_complete(first);
    const std::vector<std::string> expected_get{"GET", "name"};
    assert(first.command.arguments == expected_get);
    assert(first.command.consumed < pipelined.size());

    const auto second = minikv::RespParser::parse_command(pipelined.substr(first.command.consumed));
    assert_complete(second);
    const std::vector<std::string> expected_size{"SIZE"};
    assert(second.command.arguments == expected_size);
    assert(second.command.consumed == make_resp_array({"SIZE"}).size());

    const auto incomplete = set_request.substr(0, set_request.size() - 2);
    const auto incomplete_result = minikv::RespParser::parse_command(incomplete);
    assert(incomplete_result.status == minikv::RespParseStatus::incomplete);

    const auto not_resp = minikv::RespParser::parse_command("GET name\r\n");
    assert_error(not_resp);

    const auto invalid_bulk = minikv::RespParser::parse_command("*1\r\n+PING\r\n");
    assert_error(invalid_bulk);

    const auto null_bulk = minikv::RespParser::parse_command("*1\r\n$-1\r\n");
    assert_error(null_bulk);

    const auto empty_array = minikv::RespParser::parse_command("*0\r\n");
    assert_error(empty_array);

    return 0;
}
