#include "minikv/command.hpp"

#include "command_parse_helpers.hpp"
#include "minikv/command_catalog.hpp"
#include "minikv/command_contracts.hpp"
#include "minikv/command_response_formatters.hpp"

#include <optional>
#include <sstream>
#include <string>

namespace minikv {
namespace {

using CommandDispatchVerb = command_catalog::CommandDispatchVerb;
using command_internal::has_extra_token;
using command_internal::usage;

} // namespace

CommandResult CommandProcessor::execute_introspection_command(CommandDispatchVerb verb, std::istringstream& input) {
    switch (verb) {
    case CommandDispatchVerb::Size: {
        if (has_extra_token(input)) {
            return usage("SIZE");
        }

        return {std::to_string(store_.size())};
    }

    case CommandDispatchVerb::Keys: {
        std::string prefix;
        if (input >> prefix) {
            if (has_extra_token(input)) {
                return usage("KEYS [prefix]");
            }

            return {command_response_formatters::format_prefixed_keys(prefix, store_.keys_with_prefix(prefix))};
        }

        return {command_response_formatters::format_keys(store_.keys())};
    }

    case CommandDispatchVerb::KeysJson: {
        std::string prefix;
        if (input >> prefix) {
            if (has_extra_token(input)) {
                return usage("KEYSJSON [prefix]");
            }

            return {command_response_formatters::format_keys_json(prefix, store_.keys_with_prefix(prefix))};
        }

        return {command_response_formatters::format_keys_json(std::nullopt, store_.keys())};
    }

    case CommandDispatchVerb::ResetStats: {
        if (has_extra_token(input)) {
            return usage("RESETSTATS");
        }

        metrics_tracker_->reset();
        return {"OK stats reset"};
    }

    case CommandDispatchVerb::ExplainJson: {
        std::string target_command;
        std::getline(input >> std::ws, target_command);
        if (target_command.empty()) {
            return usage("EXPLAINJSON command");
        }

        return {command_contracts::format_explain_json(target_command)};
    }

    case CommandDispatchVerb::CheckJson: {
        std::string target_command;
        std::getline(input >> std::ws, target_command);
        if (target_command.empty()) {
            return usage("CHECKJSON command");
        }

        return {command_contracts::format_check_json(target_command, wal_ != nullptr)};
    }

    case CommandDispatchVerb::Help:
        if (has_extra_token(input)) {
            return usage("HELP");
        }

        return {help_text()};

    default:
        return {"ERR unknown command"};
    }
}

} // namespace minikv
