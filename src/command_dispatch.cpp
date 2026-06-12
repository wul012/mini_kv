#include "minikv/command.hpp"

#include "command_parse_helpers.hpp"
#include "minikv/command_catalog.hpp"
#include "minikv/string_utils.hpp"

#include <sstream>
#include <string>

namespace minikv {
namespace {

using CommandDispatchVerb = command_catalog::CommandDispatchVerb;
using command_internal::has_extra_token;
using command_internal::usage;

} // namespace

CommandResult CommandProcessor::execute_trimmed(std::string_view trimmed) {
    std::istringstream input{std::string{trimmed}};

    std::string command;
    input >> command;
    command = to_upper(command);

    switch (command_catalog::lookup_dispatch_verb(command)) {
    case CommandDispatchVerb::Ping: {
        std::string message;
        std::getline(input >> std::ws, message);

        return {message.empty() ? "PONG" : message};
    }

    case CommandDispatchVerb::Set: {
        return execute_string_command(CommandDispatchVerb::Set, input);
    }
    case CommandDispatchVerb::Del: {
        return execute_string_command(CommandDispatchVerb::Del, input);
    }
    case CommandDispatchVerb::Get: {
        return execute_string_command(CommandDispatchVerb::Get, input);
    }

    case CommandDispatchVerb::SetNxEx: {
        return execute_expiry_command(CommandDispatchVerb::SetNxEx, input);
    }
    case CommandDispatchVerb::Expire: {
        return execute_expiry_command(CommandDispatchVerb::Expire, input);
    }
    case CommandDispatchVerb::Ttl: {
        return execute_expiry_command(CommandDispatchVerb::Ttl, input);
    }

    case CommandDispatchVerb::Size: {
        return execute_introspection_command(CommandDispatchVerb::Size, input);
    }
    case CommandDispatchVerb::Keys: {
        return execute_introspection_command(CommandDispatchVerb::Keys, input);
    }
    case CommandDispatchVerb::KeysJson: {
        return execute_introspection_command(CommandDispatchVerb::KeysJson, input);
    }

    case CommandDispatchVerb::Save: {
        return execute_persistence_command(CommandDispatchVerb::Save, input);
    }
    case CommandDispatchVerb::Load: {
        return execute_persistence_command(CommandDispatchVerb::Load, input);
    }
    case CommandDispatchVerb::Compact: {
        return execute_persistence_command(CommandDispatchVerb::Compact, input);
    }
    case CommandDispatchVerb::WalInfo: {
        return execute_persistence_command(CommandDispatchVerb::WalInfo, input);
    }

    case CommandDispatchVerb::ResetStats: {
        return execute_introspection_command(CommandDispatchVerb::ResetStats, input);
    }

    case CommandDispatchVerb::RuntimeEvidence:
        return execute_runtime_evidence_command(command, input);

    case CommandDispatchVerb::ExplainJson: {
        return execute_introspection_command(CommandDispatchVerb::ExplainJson, input);
    }
    case CommandDispatchVerb::CheckJson: {
        return execute_introspection_command(CommandDispatchVerb::CheckJson, input);
    }

    case CommandDispatchVerb::Help:
        return execute_introspection_command(CommandDispatchVerb::Help, input);

    case CommandDispatchVerb::Quit:
        if (has_extra_token(input)) {
            return usage("QUIT");
        }

        return {"BYE", true};

    case CommandDispatchVerb::Unknown:
        return {"ERR unknown command"};
    }
    return {"ERR unknown command"};
}

std::string CommandProcessor::help_text() { return command_catalog::help_text(); }

} // namespace minikv
