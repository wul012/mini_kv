#include "minikv/store.hpp"

#include <algorithm>

namespace minikv {

bool Store::set(std::string_view key, std::string_view value) {
    if (key.empty()) {
        return false;
    }

    std::lock_guard lock(mutex_);
    const auto [it, inserted] = data_.insert_or_assign(std::string{key}, Entry{std::string{value}, std::nullopt});
    (void)it;
    return inserted;
}

std::optional<std::string> Store::get(std::string_view key) const {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return std::nullopt;
    }

    if (is_expired(it->second, Clock::now())) {
        data_.erase(it);
        return std::nullopt;
    }

    return it->second.value;
}

bool Store::erase(std::string_view key) {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return false;
    }

    if (is_expired(it->second, Clock::now())) {
        data_.erase(it);
        return false;
    }

    return data_.erase(std::string{key}) > 0;
}

bool Store::contains(std::string_view key) const {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return false;
    }

    if (is_expired(it->second, Clock::now())) {
        data_.erase(it);
        return false;
    }

    return true;
}

std::size_t Store::size() const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());
    return data_.size();
}

void Store::clear() {
    std::lock_guard lock(mutex_);
    data_.clear();
}

bool Store::expire(std::string_view key, std::chrono::seconds ttl) {
    if (key.empty() || ttl.count() <= 0) {
        return false;
    }

    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return false;
    }

    const auto now = Clock::now();
    if (is_expired(it->second, now)) {
        data_.erase(it);
        return false;
    }

    it->second.expires_at = now + ttl;
    return true;
}

std::optional<std::chrono::seconds> Store::ttl(std::string_view key) const {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return std::nullopt;
    }

    const auto now = Clock::now();
    if (is_expired(it->second, now)) {
        data_.erase(it);
        return std::nullopt;
    }

    if (!it->second.expires_at.has_value()) {
        return std::chrono::seconds{-1};
    }

    return std::chrono::ceil<std::chrono::seconds>(*it->second.expires_at - now);
}

std::vector<std::pair<std::string, std::string>> Store::snapshot() const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());

    std::vector<std::pair<std::string, std::string>> items;
    items.reserve(data_.size());

    for (const auto& [key, entry] : data_) {
        items.emplace_back(key, entry.value);
    }

    std::ranges::sort(items, {}, &std::pair<std::string, std::string>::first);
    return items;
}

bool Store::is_expired(const Entry& entry, Clock::time_point now) {
    return entry.expires_at.has_value() && *entry.expires_at <= now;
}

void Store::prune_expired_locked(Clock::time_point now) const {
    for (auto it = data_.begin(); it != data_.end();) {
        if (is_expired(it->second, now)) {
            it = data_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace minikv
