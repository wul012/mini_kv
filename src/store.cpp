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

bool Store::set_if_absent(std::string_view key, std::string_view value, std::optional<TimePoint> expires_at) {
    if (key.empty()) {
        return false;
    }

    std::lock_guard lock(mutex_);
    const auto now = Clock::now();
    const auto existing = data_.find(std::string{key});
    if (existing != data_.end()) {
        if (!is_expired(existing->second, now)) {
            return false;
        }
        data_.erase(existing);
    }

    data_.emplace(std::string{key}, Entry{std::string{value}, expires_at});
    return true;
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

std::vector<std::string> Store::keys() const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());

    std::vector<std::string> result;
    result.reserve(data_.size());
    for (const auto& item : data_) {
        result.push_back(item.first);
    }

    std::ranges::sort(result);
    return result;
}

std::vector<std::string> Store::keys_with_prefix(std::string_view prefix) const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());

    std::vector<std::string> result;
    for (const auto& item : data_) {
        if (item.first.rfind(prefix, 0) == 0) {
            result.push_back(item.first);
        }
    }

    std::ranges::sort(result);
    return result;
}

void Store::clear() {
    std::lock_guard lock(mutex_);
    data_.clear();
}

bool Store::expire(std::string_view key, std::chrono::seconds ttl) {
    if (key.empty() || ttl.count() <= 0) {
        return false;
    }

    return expire_at(key, Clock::now() + ttl);
}

bool Store::expire_at(std::string_view key, TimePoint expires_at) {
    if (key.empty()) {
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

    it->second.expires_at = expires_at;
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
    const auto records = snapshot_items();

    std::vector<std::pair<std::string, std::string>> items;
    items.reserve(records.size());

    for (const auto& item : records) {
        items.emplace_back(item.key, item.value);
    }

    return items;
}

std::vector<Store::SnapshotItem> Store::snapshot_items() const {
    std::lock_guard lock(mutex_);
    prune_expired_locked(Clock::now());

    std::vector<SnapshotItem> items;
    items.reserve(data_.size());

    for (const auto& [key, entry] : data_) {
        items.push_back(SnapshotItem{key, entry.value, entry.expires_at});
    }

    std::ranges::sort(items, {}, &SnapshotItem::key);
    return items;
}

void Store::restore_snapshot(std::vector<SnapshotItem> items) {
    std::lock_guard lock(mutex_);
    data_.clear();

    const auto now = Clock::now();
    for (auto& item : items) {
        if (item.key.empty()) {
            continue;
        }

        Entry entry{std::move(item.value), item.expires_at};
        if (is_expired(entry, now)) {
            continue;
        }

        data_.insert_or_assign(std::move(item.key), std::move(entry));
    }
}

bool Store::is_expired(const Entry& entry, TimePoint now) {
    return entry.expires_at.has_value() && *entry.expires_at <= now;
}

void Store::prune_expired_locked(TimePoint now) const {
    for (auto it = data_.begin(); it != data_.end();) {
        if (is_expired(it->second, now)) {
            it = data_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace minikv
