#include "minikv/command.hpp"
#include "minikv/wal.hpp"

#include <exception>
#include <iostream>
#include <optional>
#include <string>

namespace {

void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [wal_path]\n";
}

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc > 2) {
            print_usage(argv[0]);
            return 2;
        }

        minikv::Store store;
        std::optional<minikv::WriteAheadLog> wal;

        if (argc == 2) {
            wal.emplace(argv[1]);
            const auto replay = wal->replay_with_report(store);
            std::cout << "WAL: " << wal->path().string() << " (" << replay.applied_records
                      << " records replayed, " << replay.skipped_records << " skipped, "
                      << replay.truncated_records << " truncated, " << replay.checksum_failed_records
                      << " checksum failed)\n";
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
