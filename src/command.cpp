#include "minikv/command.hpp"

#include <chrono>
#include <cctype>
#include <optional>
#include <sstream>
#include <string>

namespace minikv {
namespace {

std::string trim_copy(std::string_view text) {
    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())) != 0) {
        text.remove_prefix(1);
    }

    while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())) != 0) {
        text.remove_suffix(1);
    }

    return std::string{text};
}

std::string to_upper(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

bool has_extra_token(std::istringstream& input) {
    std::string extra;
    return static_cast<bool>(input >> extra);
}

CommandResult usage(std::string_view command) {
    return {std::string{"ERR usage: "} + std::string{command}};
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

} // namespace

CommandProcessor::CommandProcessor(Store& store) : store_(store) {}

CommandResult CommandProcessor::execute(std::string_view line) {
    const std::string trimmed = trim_copy(line);
    if (trimmed.empty()) {
        return {};
    }

    std::istringstream input{trimmed};

    std::string command;
    input >> command;
    command = to_upper(command);

    if (command == "SET") {
        std::string key;
        input >> key;

        std::string value;
        std::getline(input >> std::ws, value);

        if (key.empty() || value.empty()) {
            return usage("SET key value");
        }

        const bool inserted = store_.set(key, value);
        return {inserted ? "OK inserted" : "OK updated"};
    }

    if (command == "GET") {
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

    if (command == "DEL") {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return usage("DEL key");
        }

        return {store_.erase(key) ? "1" : "0"};
    }

    if (command == "EXPIRE") {
        std::string key;
        std::string seconds_text;
        input >> key >> seconds_text;

        const auto seconds = parse_positive_seconds(seconds_text);
        if (key.empty() || !seconds.has_value() || has_extra_token(input)) {
            return usage("EXPIRE key seconds");
        }

        return {store_.expire(key, *seconds) ? "1" : "0"};
    }

    if (command == "TTL") {
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

    if (command == "SIZE") {
        if (has_extra_token(input)) {
            return usage("SIZE");
        }

        return {std::to_string(store_.size())};
    }

    if (command == "HELP") {
        if (has_extra_token(input)) {
            return usage("HELP");
        }

        return {help_text()};
    }

    if (command == "EXIT" || command == "QUIT") {
        if (has_extra_token(input)) {
            return usage("QUIT");
        }

        return {"BYE", true};
    }

    return {"ERR unknown command"};
}

std::string CommandProcessor::help_text() {
    return "Commands:\n"
           "  SET key value\n"
           "  GET key\n"
           "  DEL key\n"
           "  EXPIRE key seconds\n"
           "  TTL key\n"
           "  SIZE\n"
           "  HELP\n"
           "  EXIT";
}

} // namespace minikv
