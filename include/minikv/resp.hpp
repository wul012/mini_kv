#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv {

enum class RespParseStatus {
    complete,
    incomplete,
    error,
};

struct RespCommand {
    std::vector<std::string> arguments;
    std::size_t consumed = 0;
};

struct RespParseResult {
    RespParseStatus status = RespParseStatus::incomplete;
    RespCommand command;
    std::string error;
};

class RespParser {
public:
    static RespParseResult parse_command(std::string_view input);
    static std::string to_inline_command(const RespCommand& command);
    static std::string to_resp_response(std::string_view response);
};

} // namespace minikv
