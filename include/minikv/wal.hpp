#pragma once

#include "minikv/store.hpp"

#include <cstddef>
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

class WriteAheadLog {
public:
    explicit WriteAheadLog(std::filesystem::path path);

    bool append(std::string_view record);
    bool compact(const Store& store, std::size_t* compacted = nullptr);
    bool repair(Store& store, WalRepairReport* repair = nullptr);
    std::size_t replay(Store& store) const;
    WalReplayReport replay_with_report(Store& store) const;

    const std::filesystem::path& path() const;

private:
    std::filesystem::path path_;
    mutable std::mutex mutex_;
};

} // namespace minikv
