#pragma once

#include "minikv/command.hpp"

#include <chrono>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

namespace minikv::command_internal {

bool has_extra_token(std::istringstream& input);
CommandResult usage(std::string_view command);
CommandResult wal_error();
CommandResult wal_disabled_error();
CommandResult wal_compact_error();
CommandResult snapshot_error(std::string_view action);
std::optional<std::chrono::seconds> parse_positive_seconds(std::string_view text);
std::string format_expires_at(std::string_view key, Store::TimePoint expires_at);
std::string format_setex_at(std::string_view key, Store::TimePoint expires_at, std::string_view value);

} // namespace minikv::command_internal
