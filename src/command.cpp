#include "minikv/command.hpp"

#include "minikv/snapshot.hpp"
#include "minikv/wal.hpp"

#include <chrono>
#include <cctype>
#include <mutex>
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

CommandResult wal_error() {
    return {"ERR wal append failed"};
}

CommandResult wal_disabled_error() {
    return {"ERR WAL not enabled"};
}

CommandResult wal_compact_error() {
    return {"ERR wal compact failed"};
}

CommandResult snapshot_error(std::string_view action) {
    return {std::string{"ERR snapshot "} + std::string{action} + " failed"};
}

std::mutex& wal_command_mutex() {
    static std::mutex mutex;
    return mutex;
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

std::string format_yes_no(bool value) {
    return value ? "yes" : "no";
}

std::string format_walinfo(const WalMaintenanceReport& report) {
    return "wal_bytes=" + std::to_string(report.bytes) + " wal_records=" + std::to_string(report.records) +
           " live_keys=" + std::to_string(report.live_keys) +
           " compact_recommended=" + format_yes_no(report.compact_recommended) +
           " compact_min_records=" + std::to_string(report.options.compact_min_records) +
           " compact_record_ratio=" + std::to_string(report.options.compact_record_ratio) +
           " compact_min_bytes=" + std::to_string(report.options.compact_min_bytes) +
           " manual_compactions=" + std::to_string(report.compaction_stats.manual_compactions) +
           " auto_compactions=" + std::to_string(report.compaction_stats.auto_compactions) +
           " repair_compactions=" + std::to_string(report.compaction_stats.repair_compactions) +
           " compacted_keys=" + std::to_string(report.compaction_stats.compacted_keys) +
           " records_removed=" + std::to_string(report.compaction_stats.records_removed) +
           " bytes_saved=" + std::to_string(report.compaction_stats.bytes_saved);
}

CommandConnectionStats connection_stats(const CommandProcessorOptions& options) {
    if (options.connection_stats) {
        return options.connection_stats();
    }
    return {};
}

std::string format_connection_stats(const CommandConnectionStats& stats) {
    if (!stats.available) {
        return " connection_stats_available=no";
    }

    return " connection_stats_available=yes active_connections=" + std::to_string(stats.active_connections) +
           " total_connections=" + std::to_string(stats.total_connections) +
           " peak_connections=" + std::to_string(stats.peak_connections);
}

std::string format_stats(std::size_t live_keys,
                         WriteAheadLog* wal,
                         const std::optional<WalMaintenanceReport>& wal_report,
                         const CommandConnectionStats& stats) {
    std::string response = "live_keys=" + std::to_string(live_keys) +
                           " wal_enabled=" + format_yes_no(wal != nullptr);

    if (wal_report.has_value()) {
        response += " wal_bytes=" + std::to_string(wal_report->bytes) +
                    " wal_records=" + std::to_string(wal_report->records) +
                    " wal_live_keys=" + std::to_string(wal_report->live_keys) +
                    " compact_recommended=" + format_yes_no(wal_report->compact_recommended) +
                    " compact_min_records=" + std::to_string(wal_report->options.compact_min_records) +
                    " compact_record_ratio=" + std::to_string(wal_report->options.compact_record_ratio) +
                    " compact_min_bytes=" + std::to_string(wal_report->options.compact_min_bytes) +
                    " manual_compactions=" + std::to_string(wal_report->compaction_stats.manual_compactions) +
                    " auto_compactions=" + std::to_string(wal_report->compaction_stats.auto_compactions) +
                    " repair_compactions=" + std::to_string(wal_report->compaction_stats.repair_compactions) +
                    " compacted_keys=" + std::to_string(wal_report->compaction_stats.compacted_keys) +
                    " records_removed=" + std::to_string(wal_report->compaction_stats.records_removed) +
                    " bytes_saved=" + std::to_string(wal_report->compaction_stats.bytes_saved);
    }

    response += format_connection_stats(stats);
    return response;
}

std::string format_health(std::size_t live_keys,
                          WriteAheadLog* wal,
                          const std::optional<WalMaintenanceReport>& wal_report,
                          const CommandConnectionStats& stats) {
    std::string response = "OK live_keys=" + std::to_string(live_keys) +
                           " wal_enabled=" + format_yes_no(wal != nullptr);

    if (wal_report.has_value()) {
        response += " compact_recommended=" + format_yes_no(wal_report->compact_recommended);
    } else {
        response += " compact_recommended=na";
    }

    response += format_connection_stats(stats);
    return response;
}

} // namespace

CommandProcessor::CommandProcessor(Store& store, WriteAheadLog* wal, CommandProcessorOptions options)
    : store_(store), wal_(wal), options_(options) {}

void CommandProcessor::auto_compact_wal_if_needed() {
    if (wal_ == nullptr || !options_.auto_compact_wal) {
        return;
    }

    (void)wal_->compact_if_recommended(store_);
}

CommandResult CommandProcessor::execute(std::string_view line) {
    const std::string trimmed = trim_copy(line);
    if (trimmed.empty()) {
        return {};
    }

    std::istringstream input{trimmed};

    std::string command;
    input >> command;
    command = to_upper(command);

    if (command == "PING") {
        std::string message;
        std::getline(input >> std::ws, message);

        return {message.empty() ? "PONG" : message};
    }

    if (command == "SET") {
        std::string key;
        input >> key;

        std::string value;
        std::getline(input >> std::ws, value);

        if (key.empty() || value.empty()) {
            return usage("SET key value");
        }

        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            if (!wal_->append(std::string{"SET "} + key + " " + value)) {
                return wal_error();
            }

            const bool inserted = store_.set(key, value);
            auto_compact_wal_if_needed();
            return {inserted ? "OK inserted" : "OK updated"};
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

        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            if (!store_.contains(key)) {
                return {"0"};
            }

            if (!wal_->append(std::string{"DEL "} + key)) {
                return wal_error();
            }

            const bool erased = store_.erase(key);
            auto_compact_wal_if_needed();
            return {erased ? "1" : "0"};
        }

        if (!store_.contains(key)) {
            return {"0"};
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

        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            if (!store_.contains(key)) {
                return {"0"};
            }

            const auto expires_at = Store::Clock::now() + *seconds;
            if (!wal_->append(format_expires_at(key, expires_at))) {
                return wal_error();
            }

            const bool updated = store_.expire_at(key, expires_at);
            auto_compact_wal_if_needed();
            return {updated ? "1" : "0"};
        }

        if (!store_.contains(key)) {
            return {"0"};
        }

        const auto expires_at = Store::Clock::now() + *seconds;
        return {store_.expire_at(key, expires_at) ? "1" : "0"};
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

    if (command == "SAVE") {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("SAVE path");
        }

        std::size_t saved = 0;
        if (!SnapshotFile::save(store_, path, &saved)) {
            return snapshot_error("save");
        }

        return {std::string{"OK saved "} + std::to_string(saved)};
    }

    if (command == "LOAD") {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("LOAD path");
        }

        std::size_t loaded = 0;
        if (!SnapshotFile::load(store_, path, &loaded)) {
            return snapshot_error("load");
        }

        return {std::string{"OK loaded "} + std::to_string(loaded)};
    }

    if (command == "COMPACT") {
        if (has_extra_token(input)) {
            return usage("COMPACT");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        std::size_t compacted = 0;
        {
            std::lock_guard lock(wal_command_mutex());
            if (!wal_->compact(store_, &compacted)) {
                return wal_compact_error();
            }
        }

        return {std::string{"OK compacted "} + std::to_string(compacted)};
    }

    if (command == "WALINFO") {
        if (has_extra_token(input)) {
            return usage("WALINFO");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        std::lock_guard lock(wal_command_mutex());
        return {format_walinfo(wal_->maintenance_report(store_))};
    }

    if (command == "STATS") {
        if (has_extra_token(input)) {
            return usage("STATS");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {format_stats(live_keys, wal_, wal_report, connection_stats(options_))};
    }

    if (command == "HEALTH") {
        if (has_extra_token(input)) {
            return usage("HEALTH");
        }

        std::optional<WalMaintenanceReport> wal_report;
        if (wal_ != nullptr) {
            std::lock_guard lock(wal_command_mutex());
            wal_report = wal_->maintenance_report(store_);
        }

        const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
        return {format_health(live_keys, wal_, wal_report, connection_stats(options_))};
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
           "  PING [message]\n"
           "  SET key value\n"
           "  GET key\n"
           "  DEL key\n"
           "  EXPIRE key seconds\n"
           "  TTL key\n"
           "  SIZE\n"
           "  SAVE path\n"
           "  LOAD path\n"
           "  COMPACT\n"
           "  WALINFO\n"
           "  STATS\n"
           "  HEALTH\n"
           "  HELP\n"
           "  EXIT";
}

} // namespace minikv
