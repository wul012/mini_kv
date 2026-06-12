#include "minikv/log.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>

namespace minikv {

namespace {

std::tm utc_time(std::time_t value) {
    static std::mutex mutex;
    std::lock_guard lock{mutex};
    const std::tm* result = std::gmtime(&value);
    return result == nullptr ? std::tm{} : *result;
}

std::string current_timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto seconds = std::chrono::system_clock::to_time_t(now);
    const auto utc = utc_time(seconds);

    std::ostringstream out;
    out << std::put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
    return out.str();
}

std::string current_thread_id() {
    std::ostringstream out;
    out << std::this_thread::get_id();
    return out.str();
}

} // namespace

std::optional<LogLevel> parse_log_level(std::string_view value) {
    if (value == "error") {
        return LogLevel::error;
    }
    if (value == "warn") {
        return LogLevel::warn;
    }
    if (value == "info") {
        return LogLevel::info;
    }
    if (value == "debug") {
        return LogLevel::debug;
    }
    return std::nullopt;
}

std::string_view log_level_name(LogLevel level) {
    switch (level) {
    case LogLevel::error:
        return "error";
    case LogLevel::warn:
        return "warn";
    case LogLevel::info:
        return "info";
    case LogLevel::debug:
        return "debug";
    }
    return "warn";
}

Logger::Logger(LogLevel minimum_level) : Logger(minimum_level, std::cerr) {}

Logger::Logger(LogLevel minimum_level, std::ostream& sink) : minimum_level_(minimum_level), sink_(&sink) {}

LogLevel Logger::minimum_level() const { return minimum_level_; }

bool Logger::enabled(LogLevel level) const { return static_cast<int>(level) <= static_cast<int>(minimum_level_); }

void Logger::log(LogLevel level, std::string_view message) {
    if (!enabled(level)) {
        return;
    }
    *sink_ << format_log_line(level, message) << '\n';
}

std::string format_log_line(LogLevel level, std::string_view message) {
    std::ostringstream out;
    out << "ts=" << current_timestamp() << " level=" << log_level_name(level) << " thread=" << current_thread_id()
        << ' ' << message;
    return out.str();
}

} // namespace minikv
