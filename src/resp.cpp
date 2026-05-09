#include "minikv/resp.hpp"

#include <charconv>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

namespace minikv {
namespace {

RespParseResult incomplete_result() {
    return {RespParseStatus::incomplete, {}, {}};
}

RespParseResult error_result(std::string message) {
    return {RespParseStatus::error, {}, std::move(message)};
}

std::optional<std::size_t> find_crlf(std::string_view input, std::size_t start) {
    const auto pos = input.find("\r\n", start);
    if (pos == std::string_view::npos) {
        return std::nullopt;
    }

    return pos;
}

bool parse_integer(std::string_view text, long long& value) {
    if (text.empty()) {
        return false;
    }

    const auto* first = text.data();
    const auto* last = text.data() + text.size();
    long long parsed = 0;
    const auto [ptr, ec] = std::from_chars(first, last, parsed);
    if (ec != std::errc{} || ptr != last) {
        return false;
    }

    value = parsed;
    return true;
}

} // namespace

RespParseResult RespParser::parse_command(std::string_view input) {
    if (input.empty()) {
        return incomplete_result();
    }

    if (input.front() != '*') {
        return error_result("RESP command must start with an array");
    }

    const auto count_end = find_crlf(input, 1);
    if (!count_end.has_value()) {
        return incomplete_result();
    }

    long long count_value = 0;
    if (!parse_integer(input.substr(1, *count_end - 1), count_value)) {
        return error_result("invalid RESP array length");
    }

    if (count_value <= 0) {
        return error_result("RESP command array must contain at least one bulk string");
    }

    const auto count = static_cast<std::size_t>(count_value);
    std::vector<std::string> arguments;

    std::size_t cursor = *count_end + 2;
    for (std::size_t index = 0; index < count; ++index) {
        if (cursor >= input.size()) {
            return incomplete_result();
        }

        if (input[cursor] != '$') {
            return error_result("RESP command arguments must be bulk strings");
        }

        const auto length_end = find_crlf(input, cursor + 1);
        if (!length_end.has_value()) {
            return incomplete_result();
        }

        long long length_value = 0;
        if (!parse_integer(input.substr(cursor + 1, *length_end - (cursor + 1)), length_value)) {
            return error_result("invalid RESP bulk length");
        }

        if (length_value < 0) {
            return error_result("null RESP bulk strings are not supported");
        }

        const auto length = static_cast<std::size_t>(length_value);
        cursor = *length_end + 2;

        if (length > input.size() - cursor) {
            return incomplete_result();
        }

        const auto terminator = cursor + length;
        if (input.size() - terminator < 2) {
            return incomplete_result();
        }

        if (input[terminator] != '\r' || input[terminator + 1] != '\n') {
            return error_result("RESP bulk string must end with CRLF");
        }

        arguments.emplace_back(input.substr(cursor, length));
        cursor = terminator + 2;
    }

    return {RespParseStatus::complete, RespCommand{std::move(arguments), cursor}, {}};
}

std::string RespParser::to_inline_command(const RespCommand& command) {
    if (command.arguments.empty()) {
        return {};
    }

    std::string line = command.arguments.front();
    for (std::size_t index = 1; index < command.arguments.size(); ++index) {
        line.push_back(' ');
        line += command.arguments[index];
    }

    return line;
}

} // namespace minikv
