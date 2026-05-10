#include "minikv/wal.hpp"

#include <chrono>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace minikv {
namespace {

constexpr std::string_view wal2_prefix = "WAL2 ";
constexpr std::uint64_t fnv_offset_basis = 14695981039346656037ull;
constexpr std::uint64_t fnv_prime = 1099511628211ull;

struct DecodedWalRecord {
    bool valid = false;
    bool checksum_failed = false;
    std::string record;
};

std::string to_upper(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

std::uint64_t wal_checksum(std::string_view text) {
    std::uint64_t hash = fnv_offset_basis;
    for (const unsigned char ch : text) {
        hash ^= static_cast<std::uint64_t>(ch);
        hash *= fnv_prime;
    }
    return hash;
}

std::string format_checksum(std::uint64_t checksum) {
    std::ostringstream output;
    output << std::hex << std::nouppercase << std::setfill('0') << std::setw(16) << checksum;
    return output.str();
}

std::optional<std::uint64_t> parse_checksum(std::string_view text) {
    if (text.size() != 16) {
        return std::nullopt;
    }

    std::uint64_t value = 0;
    for (const char ch : text) {
        unsigned digit = 0;
        if (ch >= '0' && ch <= '9') {
            digit = static_cast<unsigned>(ch - '0');
        } else if (ch >= 'a' && ch <= 'f') {
            digit = static_cast<unsigned>(ch - 'a' + 10);
        } else if (ch >= 'A' && ch <= 'F') {
            digit = static_cast<unsigned>(ch - 'A' + 10);
        } else {
            return std::nullopt;
        }

        value = (value << 4) | digit;
    }
    return value;
}

std::string encode_wal_record(std::string_view record) {
    return std::string{wal2_prefix} + format_checksum(wal_checksum(record)) + " " + std::string{record};
}

DecodedWalRecord decode_wal_record(std::string_view line) {
    if (line.substr(0, wal2_prefix.size()) != wal2_prefix) {
        return DecodedWalRecord{true, false, std::string{line}};
    }

    const auto payload = line.substr(wal2_prefix.size());
    if (payload.size() < 17 || payload[16] != ' ') {
        return DecodedWalRecord{false, true, {}};
    }

    const auto expected = parse_checksum(payload.substr(0, 16));
    if (!expected.has_value()) {
        return DecodedWalRecord{false, true, {}};
    }

    std::string record{payload.substr(17)};
    if (wal_checksum(record) != *expected) {
        return DecodedWalRecord{false, true, {}};
    }

    return DecodedWalRecord{true, false, std::move(record)};
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

bool is_blank(std::string_view text) {
    for (const char ch : text) {
        if (std::isspace(static_cast<unsigned char>(ch)) == 0) {
            return false;
        }
    }
    return true;
}

bool has_final_newline(const std::filesystem::path& path) {
    std::ifstream input{path, std::ios::binary};
    if (!input) {
        return true;
    }

    input.seekg(0, std::ios::end);
    const auto size = input.tellg();
    if (size <= 0) {
        return true;
    }

    input.seekg(-1, std::ios::end);
    char last = '\0';
    input.get(last);
    return last == '\n';
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

    output << encode_wal_record(record) << '\n';
    output.flush();
    return static_cast<bool>(output);
}

std::size_t WriteAheadLog::replay(Store& store) const {
    return replay_with_report(store).applied_records;
}

WalReplayReport WriteAheadLog::replay_with_report(Store& store) const {
    std::lock_guard lock(mutex_);

    std::ifstream input{path_};
    if (!input) {
        return {};
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(std::move(line));
    }

    WalReplayReport report;
    if (!has_final_newline(path_) && !lines.empty()) {
        lines.pop_back();
        ++report.skipped_records;
        ++report.truncated_records;
    }

    for (const auto& record : lines) {
        if (is_blank(record)) {
            continue;
        }

        const auto decoded = decode_wal_record(record);
        if (!decoded.valid) {
            ++report.skipped_records;
            if (decoded.checksum_failed) {
                ++report.checksum_failed_records;
            }
            continue;
        }

        if (replay_record(store, decoded.record)) {
            ++report.applied_records;
        } else {
            ++report.skipped_records;
        }
    }

    return report;
}

const std::filesystem::path& WriteAheadLog::path() const {
    return path_;
}

} // namespace minikv
