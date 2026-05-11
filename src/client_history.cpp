#include "minikv/client_history.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <limits>
#include <system_error>
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

std::optional<std::size_t> parse_nonnegative_size(std::string_view text) {
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

std::size_t ClientHistory::load_from_file(const std::filesystem::path& path) {
    std::error_code error;
    if (!std::filesystem::exists(path, error)) {
        if (error) {
            throw std::runtime_error{"failed to check history file: " + error.message()};
        }
        return 0;
    }

    std::ifstream input{path};
    if (!input) {
        throw std::runtime_error{"failed to open history file for reading: " + path.string()};
    }

    std::size_t loaded = 0;
    std::string line;
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        add(std::move(line));
        ++loaded;
        line.clear();
    }

    if (input.bad()) {
        throw std::runtime_error{"failed while reading history file: " + path.string()};
    }

    return loaded;
}

void ClientHistory::save_to_file(const std::filesystem::path& path) const {
    if (const auto parent = path.parent_path(); !parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        if (error) {
            throw std::runtime_error{"failed to create history directory: " + error.message()};
        }
    }

    std::ofstream output{path, std::ios::trunc};
    if (!output) {
        throw std::runtime_error{"failed to open history file for writing: " + path.string()};
    }

    for (const auto& entry : entries_) {
        output << entry << '\n';
    }

    if (!output) {
        throw std::runtime_error{"failed while writing history file: " + path.string()};
    }
}

ClientKeyCache::ClientKeyCache(std::size_t max_entries) : max_entries_(max_entries) {
    if (max_entries_ == 0) {
        throw std::invalid_argument{"client key cache max_entries must be greater than zero"};
    }
}

bool ClientKeyCache::add(std::string key) {
    key = trim_copy(key);
    if (key.empty() || std::ranges::find(entries_, key) != entries_.end()) {
        return false;
    }

    entries_.push_back(std::move(key));
    if (entries_.size() > max_entries_) {
        entries_.erase(entries_.begin());
    }
    return true;
}

bool ClientKeyCache::remove(std::string_view key) {
    const auto it = std::ranges::find(entries_, key);
    if (it == entries_.end()) {
        return false;
    }

    entries_.erase(it);
    return true;
}

bool ClientKeyCache::clear() {
    if (entries_.empty()) {
        return false;
    }

    entries_.clear();
    return true;
}

bool ClientKeyCache::replace(std::vector<std::string> keys) {
    std::vector<std::string> next_entries;
    next_entries.reserve(std::min(keys.size(), max_entries_));

    for (auto& key : keys) {
        key = trim_copy(key);
        if (key.empty() || std::ranges::find(next_entries, key) != next_entries.end()) {
            continue;
        }

        next_entries.push_back(std::move(key));
        if (next_entries.size() > max_entries_) {
            next_entries.erase(next_entries.begin());
        }
    }

    if (next_entries == entries_) {
        return false;
    }

    entries_ = std::move(next_entries);
    return true;
}

bool ClientKeyCache::empty() const {
    return entries_.empty();
}

std::size_t ClientKeyCache::size() const {
    return entries_.size();
}

const std::vector<std::string>& ClientKeyCache::entries() const {
    return entries_;
}

std::size_t ClientKeyCache::load_from_file(const std::filesystem::path& path) {
    std::error_code error;
    if (!std::filesystem::exists(path, error)) {
        if (error) {
            throw std::runtime_error{"failed to check key cache file: " + error.message()};
        }
        return 0;
    }

    std::ifstream input{path};
    if (!input) {
        throw std::runtime_error{"failed to open key cache file for reading: " + path.string()};
    }

    std::size_t loaded = 0;
    std::string line;
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (add(std::move(line))) {
            ++loaded;
        }
        line.clear();
    }

    if (input.bad()) {
        throw std::runtime_error{"failed while reading key cache file: " + path.string()};
    }

    return loaded;
}

void ClientKeyCache::save_to_file(const std::filesystem::path& path) const {
    if (const auto parent = path.parent_path(); !parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        if (error) {
            throw std::runtime_error{"failed to create key cache directory: " + error.message()};
        }
    }

    std::ofstream output{path, std::ios::trunc};
    if (!output) {
        throw std::runtime_error{"failed to open key cache file for writing: " + path.string()};
    }

    for (const auto& entry : entries_) {
        output << entry << '\n';
    }

    if (!output) {
        throw std::runtime_error{"failed while writing key cache file: " + path.string()};
    }
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

std::optional<std::vector<std::string>> parse_key_list_response(std::string_view response) {
    constexpr std::string_view count_prefix = "key_count=";
    constexpr std::string_view keys_separator = " keys=";
    if (response.substr(0, count_prefix.size()) != count_prefix) {
        return std::nullopt;
    }

    const auto separator = response.find(keys_separator, count_prefix.size());
    if (separator == std::string_view::npos) {
        return std::nullopt;
    }

    const auto count = parse_nonnegative_size(response.substr(count_prefix.size(), separator - count_prefix.size()));
    if (!count.has_value()) {
        return std::nullopt;
    }

    const std::string keys_text{response.substr(separator + keys_separator.size())};
    std::istringstream input{keys_text};
    std::vector<std::string> keys;
    std::string key;
    while (input >> key) {
        keys.push_back(std::move(key));
        key.clear();
    }

    if (keys.size() != *count) {
        return std::nullopt;
    }
    return keys;
}

} // namespace minikv
