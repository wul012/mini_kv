#include "minikv/client_history.hpp"

#include <algorithm>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <utility>

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

bool all_digits(std::string_view text) {
    return !text.empty() && std::ranges::all_of(text, [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
}

std::optional<std::size_t> parse_history_index(std::string_view text) {
    if (!all_digits(text)) {
        return std::nullopt;
    }

    std::size_t value = 0;
    for (const char ch : text) {
        const auto digit = static_cast<std::size_t>(ch - '0');
        if (value > (std::numeric_limits<std::size_t>::max() - digit) / 10) {
            return std::nullopt;
        }
        value = value * 10 + digit;
    }

    if (value == 0) {
        return std::nullopt;
    }
    return value;
}

ClientInputResult local_message(std::string message) {
    ClientInputResult result;
    result.action = ClientInputAction::local;
    result.messages.push_back(std::move(message));
    return result;
}

ClientInputResult send_command(ClientHistory& history, std::string command, std::vector<std::string> messages = {}) {
    history.add(command);

    ClientInputResult result;
    result.action = ClientInputAction::send;
    result.command = std::move(command);
    result.messages = std::move(messages);
    return result;
}

} // namespace

ClientHistory::ClientHistory(std::size_t max_entries) : max_entries_(max_entries) {
    if (max_entries_ == 0) {
        throw std::invalid_argument{"client history max_entries must be greater than zero"};
    }
}

void ClientHistory::add(std::string command) {
    if (command.empty()) {
        return;
    }

    entries_.push_back(std::move(command));
    if (entries_.size() > max_entries_) {
        entries_.erase(entries_.begin());
    }
}

bool ClientHistory::empty() const {
    return entries_.empty();
}

std::size_t ClientHistory::size() const {
    return entries_.size();
}

const std::vector<std::string>& ClientHistory::entries() const {
    return entries_;
}

std::optional<std::string> ClientHistory::last() const {
    if (entries_.empty()) {
        return std::nullopt;
    }
    return entries_.back();
}

std::optional<std::string> ClientHistory::get(std::size_t one_based_index) const {
    if (one_based_index == 0 || one_based_index > entries_.size()) {
        return std::nullopt;
    }
    return entries_[one_based_index - 1];
}

ClientInputResult resolve_client_input(ClientHistory& history, std::string_view input) {
    const std::string trimmed = trim_copy(input);
    if (trimmed.empty()) {
        return {};
    }

    if (trimmed == ":history") {
        if (history.empty()) {
            return local_message("history is empty");
        }

        ClientInputResult result;
        result.action = ClientInputAction::local;
        const auto& entries = history.entries();
        for (std::size_t index = 0; index < entries.size(); ++index) {
            result.messages.push_back(std::to_string(index + 1) + "  " + entries[index]);
        }
        return result;
    }

    if (trimmed == "!!") {
        const auto command = history.last();
        if (!command.has_value()) {
            return local_message("history is empty");
        }
        return send_command(history, *command, {"repeating: " + *command});
    }

    if (trimmed.size() > 1 && trimmed.front() == '!' && all_digits(std::string_view{trimmed}.substr(1))) {
        const std::string_view index_text = std::string_view{trimmed}.substr(1);
        const auto index = parse_history_index(index_text);
        if (!index.has_value()) {
            return local_message("history entry not found: " + std::string{index_text});
        }

        const auto command = history.get(*index);
        if (!command.has_value()) {
            return local_message("history entry not found: " + std::to_string(*index));
        }
        return send_command(history, *command, {"repeating: " + *command});
    }

    return send_command(history, std::string{input});
}

} // namespace minikv
