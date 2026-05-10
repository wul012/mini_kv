#pragma once

#include "minikv/store.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <mutex>
#include <string_view>

namespace minikv {

struct WalReplayReport {
    std::size_t applied_records = 0;
    std::size_t skipped_records = 0;
    std::size_t truncated_records = 0;
    std::size_t checksum_failed_records = 0;
};

struct WalRepairReport {
    WalReplayReport replay;
    std::size_t compacted_keys = 0;
};

struct WalMaintenanceOptions {
    std::size_t compact_min_records = 8;
    std::size_t compact_record_ratio = 4;
    std::uintmax_t compact_min_bytes = 64 * 1024;
};

struct WalCompactionStats {
    std::size_t manual_compactions = 0;
    std::size_t auto_compactions = 0;
    std::size_t repair_compactions = 0;
    std::size_t compacted_keys = 0;
    std::size_t records_removed = 0;
    std::uintmax_t bytes_saved = 0;
};

struct WalMaintenanceReport {
    std::uintmax_t bytes = 0;
    std::size_t records = 0;
    std::size_t live_keys = 0;
    bool compact_recommended = false;
    WalMaintenanceOptions options;
    WalCompactionStats compaction_stats;
};

struct WalAutoCompactReport {
    WalMaintenanceReport before;
    WalMaintenanceReport after;
    std::size_t compacted_keys = 0;
    bool compacted = false;
};

class WriteAheadLog {
public:
    explicit WriteAheadLog(std::filesystem::path path, WalMaintenanceOptions options = {});

    bool append(std::string_view record);
    bool compact(const Store& store, std::size_t* compacted = nullptr);
    bool compact_if_recommended(const Store& store, WalAutoCompactReport* report = nullptr);
    bool repair(Store& store, WalRepairReport* repair = nullptr);
    WalMaintenanceReport maintenance_report(const Store& store) const;
    std::size_t replay(Store& store) const;
    WalReplayReport replay_with_report(Store& store) const;

    const std::filesystem::path& path() const;

private:
    std::filesystem::path path_;
    WalMaintenanceOptions options_;
    WalCompactionStats compaction_stats_;
    mutable std::mutex mutex_;
};

} // namespace minikv
