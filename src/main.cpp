#include "minikv/command.hpp"
#include "minikv/wal.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace {

struct CliOptions {
    std::optional<std::string> wal_path;
    bool repair_wal = false;
};

void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [wal_path] [--repair-wal]\n";
}

std::optional<CliOptions> parse_options(int argc, char** argv) {
    CliOptions options;

    for (int index = 1; index < argc; ++index) {
        const std::string_view argument = argv[index];
        if (argument == "--repair-wal") {
            options.repair_wal = true;
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

    if (options.repair_wal && !options.wal_path.has_value()) {
        return std::nullopt;
    }

    return options;
}

void print_wal_replay(const minikv::WriteAheadLog& wal, const minikv::WalReplayReport& replay) {
    std::cout << "WAL: " << wal.path().string() << " (" << replay.applied_records
              << " records replayed, " << replay.skipped_records << " skipped, "
              << replay.truncated_records << " truncated, " << replay.checksum_failed_records
              << " checksum failed)\n";
}

void print_wal_repair(const minikv::WriteAheadLog& wal, const minikv::WalRepairReport& repair) {
    std::cout << "WAL: " << wal.path().string() << " (" << repair.replay.applied_records
              << " records replayed, " << repair.replay.skipped_records << " skipped, "
              << repair.replay.truncated_records << " truncated, " << repair.replay.checksum_failed_records
              << " checksum failed; repaired to " << repair.compacted_keys << " keys)\n";
}

const char* yes_no(bool value) {
    return value ? "yes" : "no";
}

void print_wal_maintenance(const minikv::WalMaintenanceReport& report) {
    std::cout << "WAL stats: bytes=" << report.bytes << " records=" << report.records
              << " live_keys=" << report.live_keys
              << " compact_recommended=" << yes_no(report.compact_recommended) << '\n';
    if (report.compact_recommended) {
        std::cout << "WAL hint: run COMPACT to rewrite long-running log history.\n";
    }
}

} // namespace

int main(int argc, char** argv) {
    try {
        const auto options = parse_options(argc, argv);
        if (!options.has_value()) {
            print_usage(argv[0]);
            return 2;
        }

        minikv::Store store;
        std::optional<minikv::WriteAheadLog> wal;

        if (options->wal_path.has_value()) {
            wal.emplace(*options->wal_path);
            if (options->repair_wal) {
                minikv::WalRepairReport repair;
                if (!wal->repair(store, &repair)) {
                    std::cerr << "fatal: WAL repair failed\n";
                    return 1;
                }
                print_wal_repair(*wal, repair);
            } else {
                print_wal_replay(*wal, wal->replay_with_report(store));
            }
            print_wal_maintenance(wal->maintenance_report(store));
        }

        minikv::CommandProcessor processor{store, wal.has_value() ? &*wal : nullptr};

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
        std::cerr << "fatal: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
