#include "command_parse_helpers.hpp"

#include <chrono>
#include <sstream>
#include <string>

namespace minikv::command_internal {

bool has_extra_token(std::istringstream& input) {
    std::string extra;
    return static_cast<bool>(input >> extra);
}

CommandResult usage(std::string_view command) { return {std::string{"ERR usage: "} + std::string{command}}; }

CommandResult wal_error() { return {"ERR wal append failed"}; }

CommandResult wal_disabled_error() { return {"ERR WAL not enabled"}; }

CommandResult wal_compact_error() { return {"ERR wal compact failed"}; }

CommandResult snapshot_error(std::string_view action) {
    return {std::string{"ERR snapshot "} + std::string{action} + " failed"};
}

std::optional<std::chrono::seconds> parse_positive_seconds(std::string_view text) {
    std::istringstream input{std::string{text}};
    long long seconds = 0;
    char extra = '\0';

    if (!(input >> seconds) || seconds <= 0 || (input >> extra)) {
        return std::nullopt;
    }

    return std::chrono::seconds{seconds};
}

std::string format_expires_at(std::string_view key, Store::TimePoint expires_at) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"EXPIREAT "} + std::string{key} + " " + std::to_string(epoch_millis);
}

std::string format_setex_at(std::string_view key, Store::TimePoint expires_at, std::string_view value) {
    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at.time_since_epoch()).count();
    return std::string{"SETEXAT "} + std::string{key} + " " + std::to_string(epoch_millis) + " " + std::string{value};
}

} // namespace minikv::command_internal
