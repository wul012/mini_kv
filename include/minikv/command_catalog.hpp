#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::command_catalog {

enum class CommandDispatchVerb {
    Ping,
    Set,
    SetNxEx,
    Get,
    Del,
    Expire,
    Ttl,
    Size,
    Keys,
    KeysJson,
    Save,
    Load,
    Compact,
    WalInfo,
    ResetStats,
    RuntimeEvidence,
    ExplainJson,
    CheckJson,
    Help,
    Quit,
    Unknown,
};

struct CommandCatalogEntry {
    std::string_view name;
    std::string_view usage;
    std::string_view category;
    bool mutates_store;
    bool touches_wal;
    bool stable;
    std::string_view description;
    CommandDispatchVerb dispatch_verb;
    bool key_completion;
};

const std::vector<CommandCatalogEntry>& entries();
const CommandCatalogEntry* find(std::string_view command);
bool is_known(std::string_view command);
std::size_t count();
CommandDispatchVerb lookup_dispatch_verb(std::string_view command);
bool is_key_completion_command(std::string_view command);
std::vector<std::string> command_names();
std::string help_text();

} // namespace minikv::command_catalog
