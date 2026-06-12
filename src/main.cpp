#include "minikv/command.hpp"
#include "minikv/log.hpp"
#include "minikv/wal.hpp"

#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

struct CliOptions {
    std::optional<std::string> wal_path;
    bool repair_wal = false;
    bool auto_compact_wal = false;
    bool custom_wal_options = false;
    minikv::LogLevel log_level = minikv::LogLevel::warn;
    minikv::WalMaintenanceOptions wal_options;
};

void print_usage(const char* program) {
    std::cerr << "Usage: " << program
              << " [wal_path] [--repair-wal] [--auto-compact-wal]"
                 " [--wal-compact-min-records count] [--wal-compact-record-ratio ratio]"
                 " [--wal-compact-min-bytes bytes] [--log-level error|warn|info|debug]\n";
}

std::uintmax_t parse_positive_uintmax(const char* text, std::string_view name) {
    if (text[0] == '\0' || text[0] == '-') {
        throw std::out_of_range{std::string{name} + " must be a positive integer"};
    }

    std::size_t parsed_chars = 0;
    const auto value = std::stoull(text, &parsed_chars);
    if (text[parsed_chars] != '\0' || value == 0 ||
        value > static_cast<unsigned long long>(std::numeric_limits<std::uintmax_t>::max())) {
        throw std::out_of_range{std::string{name} + " must be a positive integer"};
    }
    return static_cast<std::uintmax_t>(value);
}

std::size_t parse_positive_size(const char* text, std::string_view name) {
    const auto value = parse_positive_uintmax(text, name);
    if (value > static_cast<std::uintmax_t>(std::numeric_limits<std::size_t>::max())) {
        throw std::out_of_range{std::string{name} + " is too large"};
    }
    return static_cast<std::size_t>(value);
}

std::optional<CliOptions> parse_options(int argc, char** argv) {
    CliOptions options;

    for (int index = 1; index < argc; ++index) {
        const std::string_view argument = argv[index];
        if (argument == "--repair-wal") {
            options.repair_wal = true;
            continue;
        }

        if (argument == "--auto-compact-wal") {
            options.auto_compact_wal = true;
            continue;
        }

        if (argument == "--wal-compact-min-records") {
            if (++index >= argc) {
                return std::nullopt;
            }
            options.wal_options.compact_min_records = parse_positive_size(argv[index], "wal-compact-min-records");
            options.custom_wal_options = true;
            continue;
        }

        if (argument == "--wal-compact-record-ratio") {
            if (++index >= argc) {
                return std::nullopt;
            }
            options.wal_options.compact_record_ratio = parse_positive_size(argv[index], "wal-compact-record-ratio");
            options.custom_wal_options = true;
            continue;
        }

        if (argument == "--wal-compact-min-bytes") {
            if (++index >= argc) {
                return std::nullopt;
            }
            options.wal_options.compact_min_bytes = parse_positive_uintmax(argv[index], "wal-compact-min-bytes");
            options.custom_wal_options = true;
            continue;
        }

        if (argument == "--log-level") {
            if (++index >= argc) {
                return std::nullopt;
            }
            const auto log_level = minikv::parse_log_level(argv[index]);
            if (!log_level.has_value()) {
                return std::nullopt;
            }
            options.log_level = *log_level;
            continue;
        }

        if (!argument.empty() && argument.front() == '-') {
            return std::nullopt;
        }

        if (options.wal_path.has_value()) {
            return std::nullopt;
        }
        options.wal_path = std::string{argument};
    }

    if ((options.repair_wal || options.auto_compact_wal || options.custom_wal_options) &&
        !options.wal_path.has_value()) {
        return std::nullopt;
    }

    return options;
}

void print_wal_replay(const minikv::WriteAheadLog& wal, const minikv::WalReplayReport& replay) {
    std::cout << "WAL: " << wal.path().string() << " (" << replay.applied_records << " records replayed, "
              << replay.skipped_records << " skipped, " << replay.truncated_records << " truncated, "
              << replay.checksum_failed_records << " checksum failed)\n";
}

void print_wal_repair(const minikv::WriteAheadLog& wal, const minikv::WalRepairReport& repair) {
    std::cout << "WAL: " << wal.path().string() << " (" << repair.replay.applied_records << " records replayed, "
              << repair.replay.skipped_records << " skipped, " << repair.replay.truncated_records << " truncated, "
              << repair.replay.checksum_failed_records << " checksum failed; repaired to " << repair.compacted_keys
              << " keys)\n";
}

const char* yes_no(bool value) { return value ? "yes" : "no"; }

void print_wal_maintenance(const minikv::WalMaintenanceReport& report) {
    std::cout << "WAL stats: bytes=" << report.bytes << " records=" << report.records
              << " live_keys=" << report.live_keys << " compact_recommended=" << yes_no(report.compact_recommended)
              << " compact_min_records=" << report.options.compact_min_records
              << " compact_record_ratio=" << report.options.compact_record_ratio
              << " compact_min_bytes=" << report.options.compact_min_bytes
              << " manual_compactions=" << report.compaction_stats.manual_compactions
              << " auto_compactions=" << report.compaction_stats.auto_compactions
              << " repair_compactions=" << report.compaction_stats.repair_compactions
              << " compacted_keys=" << report.compaction_stats.compacted_keys
              << " records_removed=" << report.compaction_stats.records_removed
              << " bytes_saved=" << report.compaction_stats.bytes_saved << '\n';
    if (report.compact_recommended) {
        std::cout << "WAL hint: run COMPACT to rewrite long-running log history.\n";
    }
}

void print_wal_auto_compact(const minikv::WalAutoCompactReport& report) {
    if (!report.compacted) {
        std::cout << "WAL auto compact: not needed\n";
        return;
    }

    std::cout << "WAL auto compact: compacted=" << report.compacted_keys << " records=" << report.before.records << "->"
              << report.after.records << " bytes=" << report.before.bytes << "->" << report.after.bytes << '\n';
}

} // namespace

int main(int argc, char** argv) {
    try {
        const auto options = parse_options(argc, argv);
        if (!options.has_value()) {
            print_usage(argv[0]);
            return 2;
        }
        minikv::Logger logger{options->log_level, std::cerr};

        minikv::Store store;
        std::optional<minikv::WriteAheadLog> wal;

        if (options->wal_path.has_value()) {
            wal.emplace(*options->wal_path, options->wal_options);
            if (options->repair_wal) {
                minikv::WalRepairReport repair;
                if (!wal->repair(store, &repair)) {
                    logger.log(minikv::LogLevel::error, "fatal: WAL repair failed");
                    return 1;
                }
                print_wal_repair(*wal, repair);
            } else {
                print_wal_replay(*wal, wal->replay_with_report(store));
            }
            if (options->auto_compact_wal) {
                minikv::WalAutoCompactReport compact;
                if (!wal->compact_if_recommended(store, &compact)) {
                    logger.log(minikv::LogLevel::error, "fatal: WAL auto compact failed");
                    return 1;
                }
                print_wal_auto_compact(compact);
            }
            print_wal_maintenance(wal->maintenance_report(store));
        }

        minikv::CommandProcessorOptions command_options;
        command_options.auto_compact_wal = options->auto_compact_wal;
        command_options.runtime_info.protocol = "cli";
        minikv::CommandProcessor processor{store, wal.has_value() ? &*wal : nullptr, command_options};

        std::cout << "mini-kv CLI\n";
        std::cout << minikv::CommandProcessor::help_text() << '\n';

        std::string line;
        while (true) {
            std::cout << "mini-kv> ";
            if (!std::getline(std::cin, line)) {
                std::cout << '\n';
                break;
            }

            const auto result = processor.execute(line);
            if (!result.response.empty()) {
                std::cout << result.response << '\n';
            }

            if (result.should_close) {
                break;
            }
        }
    } catch (const std::exception& error) {
        minikv::Logger{}.log(minikv::LogLevel::error, std::string{"fatal: "} + error.what());
        return 1;
    }

    return 0;
}
