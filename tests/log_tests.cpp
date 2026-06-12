#include "minikv/log.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& expected) {
    assert(text.find(expected) != std::string::npos);
}

void assert_not_contains(const std::string& text, const std::string& unexpected) {
    assert(text.find(unexpected) == std::string::npos);
}

void parse_levels_by_name() {
    assert(minikv::parse_log_level("error") == minikv::LogLevel::error);
    assert(minikv::parse_log_level("warn") == minikv::LogLevel::warn);
    assert(minikv::parse_log_level("info") == minikv::LogLevel::info);
    assert(minikv::parse_log_level("debug") == minikv::LogLevel::debug);
    assert(!minikv::parse_log_level("trace").has_value());
    assert(!minikv::parse_log_level("").has_value());
    assert(minikv::log_level_name(minikv::LogLevel::warn) == "warn");
}

void default_warn_filters_info_and_debug() {
    std::ostringstream sink;
    minikv::Logger logger{minikv::LogLevel::warn, sink};

    logger.log(minikv::LogLevel::debug, "debug-message");
    logger.log(minikv::LogLevel::info, "info-message");
    logger.log(minikv::LogLevel::warn, "warn-message");
    logger.log(minikv::LogLevel::error, "error-message");

    const std::string output = sink.str();
    assert_not_contains(output, "debug-message");
    assert_not_contains(output, "info-message");
    assert_contains(output, "warn-message");
    assert_contains(output, "error-message");
    assert_contains(output, "level=warn");
    assert_contains(output, "level=error");
    assert_contains(output, "ts=");
    assert_contains(output, "thread=");
}

void debug_level_emits_everything() {
    std::ostringstream sink;
    minikv::Logger logger{minikv::LogLevel::debug, sink};

    logger.log(minikv::LogLevel::debug, "debug-visible");
    logger.log(minikv::LogLevel::info, "info-visible");

    const std::string output = sink.str();
    assert_contains(output, "level=debug");
    assert_contains(output, "debug-visible");
    assert_contains(output, "level=info");
    assert_contains(output, "info-visible");
}

void formatted_line_has_single_record_shape() {
    const std::string line = minikv::format_log_line(minikv::LogLevel::warn, "boundary-check");
    assert_contains(line, "ts=");
    assert_contains(line, " level=warn ");
    assert_contains(line, " thread=");
    assert_contains(line, " boundary-check");
}

} // namespace

int main() {
    parse_levels_by_name();
    default_warn_filters_info_and_debug();
    debug_level_emits_everything();
    formatted_line_has_single_record_shape();
    return 0;
}
