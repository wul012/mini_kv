#pragma once

#include <string>
#include <string_view>

namespace minikv::command_contracts {

struct CommandCatalogEntry {
    std::string_view name;
    std::string_view category;
    bool mutates_store;
    bool touches_wal;
    bool stable;
    std::string_view description;
};

const CommandCatalogEntry* find_command_catalog_entry(std::string_view command);
bool is_known_command(std::string_view command);

std::string format_commands();
std::string format_commands_json();
std::string format_explain_json(std::string_view command_line);
std::string format_check_json(std::string_view command_line, bool wal_enabled);

} // namespace minikv::command_contracts
