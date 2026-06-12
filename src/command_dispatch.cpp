#include "minikv/command.hpp"

#include "command_parse_helpers.hpp"
#include "command_wal_gate.hpp"
#include "minikv/command_catalog.hpp"
#include "minikv/command_contracts.hpp"
#include "minikv/command_response_formatters.hpp"
#include "minikv/snapshot.hpp"
#include "minikv/string_utils.hpp"
#include "minikv/wal.hpp"

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
using command_internal::snapshot_error;
using command_internal::usage;
using command_internal::wal_command_gate;
using command_internal::wal_compact_error;
using command_internal::wal_disabled_error;

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

    case CommandDispatchVerb::Save: {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("SAVE path");
        }

        std::size_t saved = 0;
        if (!SnapshotFile::save(store_, path, &saved)) {
            return snapshot_error("save");
        }

        return {std::string{"OK saved "} + std::to_string(saved)};
    }

    case CommandDispatchVerb::Load: {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("LOAD path");
        }

        std::size_t loaded = 0;
        if (!SnapshotFile::load(store_, path, &loaded)) {
            return snapshot_error("load");
        }

        return {std::string{"OK loaded "} + std::to_string(loaded)};
    }

    case CommandDispatchVerb::Compact: {
        if (has_extra_token(input)) {
            return usage("COMPACT");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        std::size_t compacted = 0;
        {
            auto scope = wal_command_gate().enter_compaction();
            if (!wal_->compact(store_, &compacted)) {
                return wal_compact_error();
            }
        }

        return {std::string{"OK compacted "} + std::to_string(compacted)};
    }

    case CommandDispatchVerb::WalInfo: {
        if (has_extra_token(input)) {
            return usage("WALINFO");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        auto scope = wal_command_gate().enter_command();
        return {command_response_formatters::format_walinfo(wal_->maintenance_report(store_))};
    }

    case CommandDispatchVerb::ResetStats: {
        if (has_extra_token(input)) {
            return usage("RESETSTATS");
        }

        metrics_tracker_->reset();
        return {"OK stats reset"};
    }

    case CommandDispatchVerb::RuntimeEvidence:
        return execute_runtime_evidence_command(command, input);

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
