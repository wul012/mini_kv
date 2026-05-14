#pragma once

#include <chrono>
#include <cstddef>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace minikv {

class Store {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;

    struct SnapshotItem {
        std::string key;
        std::string value;
        std::optional<TimePoint> expires_at;
    };

    bool set(std::string_view key, std::string_view value);
    bool set_if_absent(std::string_view key,
                       std::string_view value,
                       std::optional<TimePoint> expires_at = std::nullopt);
    std::optional<std::string> get(std::string_view key) const;
    bool erase(std::string_view key);
    bool contains(std::string_view key) const;
    std::size_t size() const;
    std::vector<std::string> keys() const;
    std::vector<std::string> keys_with_prefix(std::string_view prefix) const;
    void clear();

    bool expire(std::string_view key, std::chrono::seconds ttl);
    bool expire_at(std::string_view key, TimePoint expires_at);
    std::optional<std::chrono::seconds> ttl(std::string_view key) const;

    std::vector<std::pair<std::string, std::string>> snapshot() const;
    std::vector<SnapshotItem> snapshot_items() const;
    void restore_snapshot(std::vector<SnapshotItem> items);

private:
    struct Entry {
        std::string value;
        std::optional<TimePoint> expires_at;
    };

    static bool is_expired(const Entry& entry, TimePoint now);
    void prune_expired_locked(TimePoint now) const;

    mutable std::mutex mutex_;
    mutable std::unordered_map<std::string, Entry> data_;
};

} // namespace minikv
