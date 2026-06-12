#pragma once

#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>

namespace minikv {

enum class LogLevel {
    error = 0,
    warn = 1,
    info = 2,
    debug = 3,
};

std::optional<LogLevel> parse_log_level(std::string_view value);

std::string_view log_level_name(LogLevel level);

class Logger {
public:
    explicit Logger(LogLevel minimum_level = LogLevel::warn);
    Logger(LogLevel minimum_level, std::ostream& sink);

    [[nodiscard]] LogLevel minimum_level() const;
    [[nodiscard]] bool enabled(LogLevel level) const;

    void log(LogLevel level, std::string_view message);

private:
    LogLevel minimum_level_;
    std::ostream* sink_;
};

std::string format_log_line(LogLevel level, std::string_view message);

} // namespace minikv
