#include "minikv/command.hpp"

#include "command_parse_helpers.hpp"
#include "minikv/command_catalog.hpp"

#include <optional>
#include <sstream>
#include <string>

namespace minikv {
namespace {

using CommandDispatchVerb = command_catalog::CommandDispatchVerb;
using command_internal::format_expires_at;
using command_internal::format_setex_at;
using command_internal::has_extra_token;
using command_internal::parse_positive_seconds;
using command_internal::usage;

} // namespace

CommandResult CommandProcessor::execute_expiry_command(CommandDispatchVerb verb, std::istringstream& input) {
    switch (verb) {
    case CommandDispatchVerb::SetNxEx: {
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;

        std::string value;
        std::getline(input >> std::ws, value);

        const auto seconds = parse_positive_seconds(seconds_text);
        if (key.empty() || !seconds.has_value() || value.empty()) {
            return usage("SETNXEX key seconds value");
        }

        const auto expires_at = Store::Clock::now() + *seconds;
        return execute_with_wal(
            [this, key, expires_at, value]() -> std::optional<std::string> {
                if (store_.contains(key)) {
                    return std::nullopt;
                }
                return format_setex_at(key, expires_at, value);
            },
            [this, key, value, expires_at] {
                const bool claimed = store_.set_if_absent(key, value, expires_at);
                return CommandResult{claimed ? "1" : "0"};
            });
    }

    case CommandDispatchVerb::Expire: {
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;

        const auto seconds = parse_positive_seconds(seconds_text);
        if (key.empty() || !seconds.has_value() || has_extra_token(input)) {
            return usage("EXPIRE key seconds");
        }

        const auto expires_at = Store::Clock::now() + *seconds;
        return execute_with_wal(
            [this, key, expires_at]() -> std::optional<std::string> {
                if (!store_.contains(key)) {
                    return std::nullopt;
                }
                return format_expires_at(key, expires_at);
            },
            [this, key, expires_at] {
                const bool updated = store_.expire_at(key, expires_at);
                return CommandResult{updated ? "1" : "0"};
            });
    }

    case CommandDispatchVerb::Ttl: {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("TTL key");
        }

        const auto ttl = store_.ttl(key);
        if (!ttl.has_value()) {
            return {"-2"};
        }

        return {std::to_string(ttl->count())};
    }

    default:
        return {"ERR unknown command"};
    }
}

} // namespace minikv
