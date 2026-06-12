#include "minikv/command.hpp"

#include "command_parse_helpers.hpp"
#include "minikv/command_catalog.hpp"

#include <optional>
#include <sstream>
#include <string>

namespace minikv {
namespace {

using CommandDispatchVerb = command_catalog::CommandDispatchVerb;
using command_internal::has_extra_token;
using command_internal::usage;

} // namespace

CommandResult CommandProcessor::execute_string_command(CommandDispatchVerb verb, std::istringstream& input) {
    switch (verb) {
    case CommandDispatchVerb::Set: {
        std::string key;
        input >> key;

        std::string value;
        std::getline(input >> std::ws, value);

        if (key.empty() || value.empty()) {
            return usage("SET key value");
        }

        return execute_with_wal(
            [key, value] { return std::optional<std::string>{std::string{"SET "} + key + " " + value}; },
            [this, key, value] {
                const bool inserted = store_.set(key, value);
                return CommandResult{inserted ? "OK inserted" : "OK updated"};
            });
    }

    case CommandDispatchVerb::Get: {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("GET key");
        }

        const auto value = store_.get(key);
        if (!value.has_value()) {
            return {"(nil)"};
        }

        return {*value};
    }

    case CommandDispatchVerb::Del: {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("DEL key");
        }

        return execute_with_wal(
            [this, key]() -> std::optional<std::string> {
                if (!store_.contains(key)) {
                    return std::nullopt;
                }
                return std::string{"DEL "} + key;
            },
            [this, key] {
                const bool erased = store_.erase(key);
                return CommandResult{erased ? "1" : "0"};
            });
    }

    default:
        return {"ERR unknown command"};
    }
}

} // namespace minikv
