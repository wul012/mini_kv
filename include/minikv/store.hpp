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
    bool set(std::string_view key, std::string_view value);
    std::optional<std::string> get(std::string_view key) const;
    bool erase(std::string_view key);
    bool contains(std::string_view key) const;
    std::size_t size() const;
    void clear();

    bool expire(std::string_view key, std::chrono::seconds ttl);
    std::optional<std::chrono::seconds> ttl(std::string_view key) const;

    std::vector<std::pair<std::string, std::string>> snapshot() const;

private:
    using Clock = std::chrono::steady_clock;

    struct Entry {
        std::string value;
        std::optional<Clock::time_point> expires_at;
    };

    static bool is_expired(const Entry& entry, Clock::time_point now);
    void prune_expired_locked(Clock::time_point now) const;

    mutable std::mutex mutex_;
    mutable std::unordered_map<std::string, Entry> data_;
};

} // namespace minikv
