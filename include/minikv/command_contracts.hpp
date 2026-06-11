#pragma once

#include "minikv/command_catalog.hpp"

#include <string>
#include <string_view>

namespace minikv::command_contracts {

using CommandCatalogEntry = command_catalog::CommandCatalogEntry;

const CommandCatalogEntry* find_command_catalog_entry(std::string_view command);
bool is_known_command(std::string_view command);

std::string format_commands();
std::string format_commands_json();
std::string format_explain_json(std::string_view command_line);
std::string format_check_json(std::string_view command_line, bool wal_enabled);

} // namespace minikv::command_contracts
