#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/wal.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <thread>
#include <vector>

int main() {
    using namespace std::chrono_literals;

    const auto suffix = std::chrono::system_clock::now().time_since_epoch().count();
    const auto path = std::filesystem::temp_directory_path() / ("minikv-wal-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(path);

    {
        minikv::Store store;
        minikv::WriteAheadLog wal{path};
        minikv::CommandProcessor processor{store, &wal};

        auto result = processor.execute("SET name mini-kv");
        assert(result.response == "OK inserted");

        result = processor.execute("SET phrase hello from wal");
        assert(result.response == "OK inserted");

        result = processor.execute("EXPIRE name 60");
        assert(result.response == "1");

        result = processor.execute("DEL phrase");
        assert(result.response == "1");

        result = processor.execute("SET stale old");
        assert(result.response == "OK inserted");

        result = processor.execute("EXPIRE stale 1");
        assert(result.response == "1");

        std::this_thread::sleep_for(1100ms);

        result = processor.execute("GET stale");
        assert(result.response == "(nil)");
    }

    {
        std::ifstream input{path};
        std::string first_line;
        assert(std::getline(input, first_line));
        assert(first_line.rfind("WAL2 ", 0) == 0);
    }

    {
        minikv::Store restored;
        minikv::WriteAheadLog wal{path};
        const auto applied = wal.replay(restored);

        assert(applied == 6);
        assert(restored.get("name") == std::optional<std::string>{"mini-kv"});
        assert(restored.ttl("name").has_value());
        assert(!restored.get("phrase").has_value());
        assert(!restored.get("stale").has_value());
    }

    std::filesystem::remove(path);

    const auto corrupt_path =
        std::filesystem::temp_directory_path() / ("minikv-wal-corrupt-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(corrupt_path);

    {
        std::ofstream output{corrupt_path, std::ios::binary};
        output << "SET kept value\n";
        output << "GARBAGE record\n";
        output << "EXPIREAT kept not-a-number\n";
        output << "SET partial value";
    }

    {
        minikv::Store restored;
        minikv::WriteAheadLog wal{corrupt_path};
        const auto report = wal.replay_with_report(restored);

        assert(report.applied_records == 1);
        assert(report.skipped_records == 3);
        assert(report.truncated_records == 1);
        assert(report.checksum_failed_records == 0);
        assert(restored.get("kept") == std::optional<std::string>{"value"});
        assert(!restored.get("partial").has_value());
        assert(wal.replay(restored) == 1);
    }

    std::filesystem::remove(corrupt_path);

    const auto checksum_path =
        std::filesystem::temp_directory_path() / ("minikv-wal-checksum-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(checksum_path);

    {
        minikv::WriteAheadLog wal{checksum_path};
        assert(wal.append("SET checked ok"));
        assert(wal.append("SET another value"));
    }

    {
        std::ifstream input{checksum_path, std::ios::binary};
        std::string contents{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
        const auto position = contents.find("SET checked ok");
        assert(position != std::string::npos);
        contents.replace(position, std::string{"SET checked ok"}.size(), "SET checked tampered");

        std::ofstream output{checksum_path, std::ios::binary | std::ios::trunc};
        output << contents;
    }

    {
        minikv::Store restored;
        minikv::WriteAheadLog wal{checksum_path};
        const auto report = wal.replay_with_report(restored);

        assert(report.applied_records == 1);
        assert(report.skipped_records == 1);
        assert(report.truncated_records == 0);
        assert(report.checksum_failed_records == 1);
        assert(!restored.get("checked").has_value());
        assert(restored.get("another") == std::optional<std::string>{"value"});
        assert(wal.replay(restored) == 1);
    }

    std::filesystem::remove(checksum_path);

    const auto compact_path =
        std::filesystem::temp_directory_path() / ("minikv-wal-compact-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(compact_path);

    {
        minikv::Store store;
        minikv::WriteAheadLog wal{compact_path};
        minikv::CommandProcessor processor{store, &wal};

        auto result = processor.execute("SET keep value");
        assert(result.response == "OK inserted");

        result = processor.execute("EXPIRE keep 60");
        assert(result.response == "1");

        result = processor.execute("SET phrase hello from compact");
        assert(result.response == "OK inserted");

        result = processor.execute("SET gone remove");
        assert(result.response == "OK inserted");

        result = processor.execute("DEL gone");
        assert(result.response == "1");

        result = processor.execute("COMPACT");
        assert(result.response == "OK compacted 2");
    }

    {
        std::ifstream input{compact_path};
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(input, line)) {
            lines.push_back(line);
        }

        assert(lines.size() == 3);
        std::string contents;
        for (const auto& record : lines) {
            assert(record.rfind("WAL2 ", 0) == 0);
            contents += record;
            contents += '\n';
        }

        assert(contents.find("SET keep value") != std::string::npos);
        assert(contents.find("EXPIREAT keep ") != std::string::npos);
        assert(contents.find("SET phrase hello from compact") != std::string::npos);
        assert(contents.find("DEL gone") == std::string::npos);
        assert(contents.find("SET gone remove") == std::string::npos);
    }

    {
        minikv::Store restored;
        minikv::WriteAheadLog wal{compact_path};
        const auto report = wal.replay_with_report(restored);

        assert(report.applied_records == 3);
        assert(report.skipped_records == 0);
        assert(report.truncated_records == 0);
        assert(report.checksum_failed_records == 0);
        assert(restored.get("keep") == std::optional<std::string>{"value"});
        assert(restored.ttl("keep").has_value());
        assert(restored.get("phrase") == std::optional<std::string>{"hello from compact"});
        assert(!restored.get("gone").has_value());
    }

    std::filesystem::remove(compact_path);

    return 0;
}
