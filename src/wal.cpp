#include "minikv/wal.hpp"

#include <chrono>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

namespace minikv {
namespace {

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

bool replay_record(Store& store, std::string_view record) {
    std::istringstream input{std::string{record}};

    std::string command;
    input >> command;
    command = to_upper(command);

    if (command == "SET") {
        std::string key;
        input >> key;

        std::string value;
        std::getline(input >> std::ws, value);

        if (key.empty() || value.empty()) {
            return false;
        }

        store.set(key, value);
        return true;
    }

    if (command == "DEL") {
        std::string key;
        input >> key;

        if (key.empty() || has_extra_token(input)) {
            return false;
        }

        store.erase(key);
        return true;
    }

    if (command == "EXPIREAT") {
        std::string key;
        long long epoch_millis = 0;
        input >> key >> epoch_millis;

        if (key.empty() || !input || has_extra_token(input)) {
            return false;
        }

        const Store::TimePoint expires_at{std::chrono::milliseconds{epoch_millis}};
        if (expires_at <= Store::Clock::now()) {
            store.erase(key);
        } else {
            store.expire_at(key, expires_at);
        }
        return true;
    }

    return false;
}

} // namespace

WriteAheadLog::WriteAheadLog(std::filesystem::path path) : path_(std::move(path)) {}

bool WriteAheadLog::append(std::string_view record) {
    std::lock_guard lock(mutex_);

    if (path_.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(path_.parent_path(), error);
        if (error) {
            return false;
        }
    }

    std::ofstream output{path_, std::ios::app};
    if (!output) {
        return false;
    }

    output << record << '\n';
    output.flush();
    return static_cast<bool>(output);
}

std::size_t WriteAheadLog::replay(Store& store) const {
    std::lock_guard lock(mutex_);

    std::ifstream input{path_};
    if (!input) {
        return 0;
    }

    std::size_t applied = 0;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        if (replay_record(store, line)) {
            ++applied;
        }
    }

    return applied;
}

const std::filesystem::path& WriteAheadLog::path() const {
    return path_;
}

} // namespace minikv
