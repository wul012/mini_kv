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

        result = processor.execute("SETNXEX token 60 claimed");
        assert(result.response == "1");

        result = processor.execute("SETNXEX token 60 duplicate");
        assert(result.response == "0");

        result = processor.execute("DEL phrase");
        assert(result.response == "1");

        result = processor.execute("DEL missing");
        assert(result.response == "0");

        result = processor.execute("SET stale old");
        assert(result.response == "OK inserted");

        result = processor.execute("EXPIRE stale 1");
        assert(result.response == "1");

        result = processor.execute("EXPIRE missing 60");
        assert(result.response == "0");

        const auto records_after_noop_misses = wal.maintenance_report(store).records;

        result = processor.execute("SET only-key");
        assert(result.response == "ERR usage: SET key value");

        result = processor.execute("SETNXEX token 0 invalid");
        assert(result.response == "ERR usage: SETNXEX key seconds value");

        result = processor.execute("DEL");
        assert(result.response == "ERR usage: DEL key");

        result = processor.execute("EXPIRE name not-a-number");
        assert(result.response == "ERR usage: EXPIRE key seconds");

        assert(wal.maintenance_report(store).records == records_after_noop_misses);

        std::this_thread::sleep_for(1100ms);

        result = processor.execute("GET stale");
        assert(result.response == "(nil)");

        const auto records_after_expired_prune = wal.maintenance_report(store).records;

        result = processor.execute("DEL stale");
        assert(result.response == "0");

        result = processor.execute("EXPIRE stale 60");
        assert(result.response == "0");

        assert(wal.maintenance_report(store).records == records_after_expired_prune);
        assert(wal.maintenance_report(store).records == 7);
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

        assert(applied == 7);
        assert(restored.get("name") == std::optional<std::string>{"mini-kv"});
        assert(restored.ttl("name").has_value());
        assert(restored.get("token") == std::optional<std::string>{"claimed"});
        assert(restored.ttl("token").has_value());
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
        const bool appended_checked = wal.append("SET checked ok");
        const bool appended_another = wal.append("SET another value");
        assert(appended_checked);
        assert(appended_another);
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

    const auto repair_path =
        std::filesystem::temp_directory_path() / ("minikv-wal-repair-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(repair_path);

    const auto future_expiration =
        std::chrono::duration_cast<std::chrono::milliseconds>((minikv::Store::Clock::now() + 60s).time_since_epoch())
            .count();

    {
        minikv::WriteAheadLog wal{repair_path};
        const bool appended_good = wal.append("SET good value");
        const bool appended_bad = wal.append("SET bad original");
        const bool appended_gone = wal.append("SET gone old");
        const bool appended_del_gone = wal.append("DEL gone");
        const bool appended_ttl = wal.append("SET ttl keep");
        const bool appended_expiration = wal.append("EXPIREAT ttl " + std::to_string(future_expiration));
        assert(appended_good);
        assert(appended_bad);
        assert(appended_gone);
        assert(appended_del_gone);
        assert(appended_ttl);
        assert(appended_expiration);
    }

    {
        std::ifstream input{repair_path, std::ios::binary};
        std::string contents{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
        const auto position = contents.find("SET bad original");
        assert(position != std::string::npos);
        contents.replace(position, std::string{"SET bad original"}.size(), "SET bad tampered");
        contents += "SET legacy old\n";
        contents += "GARBAGE record\n";
        contents += "SET partial value";

        std::ofstream output{repair_path, std::ios::binary | std::ios::trunc};
        output << contents;
    }

    {
        minikv::Store repaired_store;
        minikv::WriteAheadLog wal{repair_path};
        minikv::WalRepairReport repair;
        const bool repaired = wal.repair(repaired_store, &repair);
        assert(repaired);

        assert(repair.replay.applied_records == 6);
        assert(repair.replay.skipped_records == 3);
        assert(repair.replay.truncated_records == 1);
        assert(repair.replay.checksum_failed_records == 1);
        assert(repair.compacted_keys == 3);
        assert(repaired_store.get("good") == std::optional<std::string>{"value"});
        assert(repaired_store.get("legacy") == std::optional<std::string>{"old"});
        assert(repaired_store.get("ttl") == std::optional<std::string>{"keep"});
        assert(repaired_store.ttl("ttl").has_value());
        assert(!repaired_store.get("bad").has_value());
        assert(!repaired_store.get("gone").has_value());
        assert(!repaired_store.get("partial").has_value());

        const auto maintenance = wal.maintenance_report(repaired_store);
        assert(maintenance.compaction_stats.manual_compactions == 0);
        assert(maintenance.compaction_stats.auto_compactions == 0);
        assert(maintenance.compaction_stats.repair_compactions == 1);
        assert(maintenance.compaction_stats.compacted_keys == 3);
        assert(maintenance.compaction_stats.records_removed > 0);
        assert(maintenance.compaction_stats.bytes_saved > 0);
    }

    {
        std::ifstream input{repair_path};
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(input, line)) {
            lines.push_back(line);
        }

        assert(lines.size() == 4);
        std::string contents;
        for (const auto& record : lines) {
            assert(record.rfind("WAL2 ", 0) == 0);
            contents += record;
            contents += '\n';
        }

        assert(contents.find("SET good value") != std::string::npos);
        assert(contents.find("SET legacy old") != std::string::npos);
        assert(contents.find("SET ttl keep") != std::string::npos);
        assert(contents.find("EXPIREAT ttl ") != std::string::npos);
        assert(contents.find("SET bad") == std::string::npos);
        assert(contents.find("gone") == std::string::npos);
        assert(contents.find("GARBAGE") == std::string::npos);
        assert(contents.find("partial") == std::string::npos);
    }

    {
        minikv::Store restored;
        minikv::WriteAheadLog wal{repair_path};
        const auto report = wal.replay_with_report(restored);

        assert(report.applied_records == 4);
        assert(report.skipped_records == 0);
        assert(report.truncated_records == 0);
        assert(report.checksum_failed_records == 0);
        assert(restored.get("good") == std::optional<std::string>{"value"});
        assert(restored.get("legacy") == std::optional<std::string>{"old"});
        assert(restored.get("ttl") == std::optional<std::string>{"keep"});
        assert(!restored.get("bad").has_value());
        assert(!restored.get("gone").has_value());
    }

    std::filesystem::remove(repair_path);

    const auto info_path =
        std::filesystem::temp_directory_path() / ("minikv-wal-info-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(info_path);

    {
        minikv::Store store;
        minikv::WriteAheadLog wal{info_path};
        minikv::CommandProcessor processor{store, &wal};

        for (int index = 0; index < 10; ++index) {
            auto result = processor.execute("SET hot value" + std::to_string(index));
            assert(result.response.rfind("OK ", 0) == 0);
        }

        auto result = processor.execute("WALINFO");
        assert(result.response.find("wal_records=10") != std::string::npos);
        assert(result.response.find("live_keys=1") != std::string::npos);
        assert(result.response.find("compact_recommended=yes") != std::string::npos);

        auto report = wal.maintenance_report(store);
        assert(report.records == 10);
        assert(report.live_keys == 1);
        assert(report.bytes > 0);
        assert(report.compact_recommended);
        assert(report.options.compact_min_records == 8);
        assert(report.options.compact_record_ratio == 4);
        assert(report.options.compact_min_bytes == 64 * 1024);

        result = processor.execute("COMPACT");
        assert(result.response == "OK compacted 1");

        result = processor.execute("WALINFO");
        assert(result.response.find("wal_records=1") != std::string::npos);
        assert(result.response.find("live_keys=1") != std::string::npos);
        assert(result.response.find("compact_recommended=no") != std::string::npos);

        report = wal.maintenance_report(store);
        assert(report.records == 1);
        assert(report.live_keys == 1);
        assert(report.bytes > 0);
        assert(!report.compact_recommended);
        assert(report.compaction_stats.manual_compactions == 1);
        assert(report.compaction_stats.auto_compactions == 0);
        assert(report.compaction_stats.repair_compactions == 0);
        assert(report.compaction_stats.compacted_keys == 1);
        assert(report.compaction_stats.records_removed == 9);
        assert(report.compaction_stats.bytes_saved > 0);

        result = processor.execute("STATS");
        assert(result.response.find("live_keys=1") != std::string::npos);
        assert(result.response.find("wal_enabled=yes") != std::string::npos);
        assert(result.response.find("wal_records=1") != std::string::npos);
        assert(result.response.find("compact_recommended=no") != std::string::npos);
        assert(result.response.find("manual_compactions=1") != std::string::npos);
        assert(result.response.find("connection_stats_available=no") != std::string::npos);

        result = processor.execute("HEALTH");
        assert(result.response.find("OK live_keys=1") != std::string::npos);
        assert(result.response.find("wal_enabled=yes") != std::string::npos);
        assert(result.response.find("compact_recommended=no") != std::string::npos);

        minikv::WalAutoCompactReport skipped_compact;
        const bool compact_checked = wal.compact_if_recommended(store, &skipped_compact);
        assert(compact_checked);
        assert(!skipped_compact.compacted);
        assert(skipped_compact.before.records == 1);
        assert(skipped_compact.after.records == 1);
        assert(skipped_compact.after.compaction_stats.manual_compactions == 1);
        assert(skipped_compact.after.compaction_stats.auto_compactions == 0);
    }

    std::filesystem::remove(info_path);

    const auto auto_compact_path =
        std::filesystem::temp_directory_path() / ("minikv-wal-auto-compact-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(auto_compact_path);

    {
        minikv::Store store;
        minikv::WriteAheadLog wal{auto_compact_path};
        minikv::CommandProcessorOptions options;
        options.auto_compact_wal = true;
        minikv::CommandProcessor processor{store, &wal, options};

        for (int index = 0; index < 8; ++index) {
            auto result = processor.execute("SET hot value" + std::to_string(index));
            assert(result.response.rfind("OK ", 0) == 0);
        }

        auto result = processor.execute("GET hot");
        assert(result.response == "value7");

        result = processor.execute("WALINFO");
        assert(result.response.find("wal_records=1") != std::string::npos);
        assert(result.response.find("live_keys=1") != std::string::npos);
        assert(result.response.find("compact_recommended=no") != std::string::npos);

        const auto report = wal.maintenance_report(store);
        assert(report.records == 1);
        assert(report.live_keys == 1);
        assert(report.bytes > 0);
        assert(!report.compact_recommended);
        assert(report.compaction_stats.manual_compactions == 0);
        assert(report.compaction_stats.auto_compactions == 1);
        assert(report.compaction_stats.repair_compactions == 0);
        assert(report.compaction_stats.compacted_keys == 1);
        assert(report.compaction_stats.records_removed == 7);
        assert(report.compaction_stats.bytes_saved > 0);
    }

    std::filesystem::remove(auto_compact_path);

    const auto custom_compact_path =
        std::filesystem::temp_directory_path() / ("minikv-wal-custom-compact-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(custom_compact_path);

    {
        minikv::Store store;
        minikv::WalMaintenanceOptions maintenance_options;
        maintenance_options.compact_min_records = 4;
        maintenance_options.compact_record_ratio = 2;
        maintenance_options.compact_min_bytes = 1024 * 1024;

        minikv::WriteAheadLog wal{custom_compact_path, maintenance_options};
        minikv::CommandProcessorOptions command_options;
        command_options.auto_compact_wal = true;
        minikv::CommandProcessor processor{store, &wal, command_options};

        for (int index = 0; index < 4; ++index) {
            auto result = processor.execute("SET tuned value" + std::to_string(index));
            assert(result.response.rfind("OK ", 0) == 0);
        }

        auto result = processor.execute("WALINFO");
        assert(result.response.find("wal_records=1") != std::string::npos);
        assert(result.response.find("compact_min_records=4") != std::string::npos);
        assert(result.response.find("compact_record_ratio=2") != std::string::npos);
        assert(result.response.find("compact_min_bytes=1048576") != std::string::npos);
        assert(result.response.find("auto_compactions=1") != std::string::npos);
        assert(result.response.find("records_removed=3") != std::string::npos);

        const auto report = wal.maintenance_report(store);
        assert(report.records == 1);
        assert(report.live_keys == 1);
        assert(report.options.compact_min_records == 4);
        assert(report.options.compact_record_ratio == 2);
        assert(report.options.compact_min_bytes == 1024 * 1024);
        assert(report.compaction_stats.auto_compactions == 1);
        assert(report.compaction_stats.compacted_keys == 1);
        assert(report.compaction_stats.records_removed == 3);
        assert(report.compaction_stats.bytes_saved > 0);
    }

    std::filesystem::remove(custom_compact_path);

    const auto concurrent_compact_path =
        std::filesystem::temp_directory_path() /
        ("minikv-wal-concurrent-auto-compact-test-" + std::to_string(suffix) + ".wal");
    std::filesystem::remove(concurrent_compact_path);

    {
        minikv::Store store;
        minikv::WalMaintenanceOptions maintenance_options;
        maintenance_options.compact_min_records = 2;
        maintenance_options.compact_record_ratio = 1;
        maintenance_options.compact_min_bytes = 1024 * 1024;

        minikv::WriteAheadLog wal{concurrent_compact_path, maintenance_options};
        minikv::CommandProcessorOptions command_options;
        command_options.auto_compact_wal = true;
        minikv::CommandProcessor processor{store, &wal, command_options};

        std::vector<std::thread> writers;
        for (int thread_index = 0; thread_index < 4; ++thread_index) {
            writers.emplace_back([&processor, thread_index] {
                for (int write_index = 0; write_index < 5; ++write_index) {
                    const auto value = "value-" + std::to_string(thread_index) + "-" + std::to_string(write_index);
                    const auto result = processor.execute("SET shared " + value);
                    assert(result.response.rfind("OK ", 0) == 0);
                }
            });
        }

        for (auto& writer : writers) {
            writer.join();
        }

        const auto final_value = store.get("shared");
        assert(final_value.has_value());
        assert(final_value->rfind("value-", 0) == 0);

        const auto report = wal.maintenance_report(store);
        assert(report.records == 1);
        assert(report.live_keys == 1);
        assert(report.compaction_stats.auto_compactions > 0);

        minikv::Store restored;
        minikv::WriteAheadLog replay_wal{concurrent_compact_path, maintenance_options};
        const auto replay_report = replay_wal.replay_with_report(restored);
        assert(replay_report.applied_records == 1);
        assert(replay_report.skipped_records == 0);
        assert(replay_report.checksum_failed_records == 0);
        assert(restored.get("shared") == final_value);
    }

    std::filesystem::remove(concurrent_compact_path);

    return 0;
}
