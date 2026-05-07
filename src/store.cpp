#include "minikv/store.hpp"

#include <algorithm>

namespace minikv {

bool Store::set(std::string_view key, std::string_view value) {
    if (key.empty()) {
        return false;
    }

    std::lock_guard lock(mutex_);
    const auto [it, inserted] = data_.insert_or_assign(std::string{key}, std::string{value});
    (void)it;
    return inserted;
}

std::optional<std::string> Store::get(std::string_view key) const {
    std::lock_guard lock(mutex_);
    const auto it = data_.find(std::string{key});
    if (it == data_.end()) {
        return std::nullopt;
    }
    return it->second;
}

bool Store::erase(std::string_view key) {
    std::lock_guard lock(mutex_);
    return data_.erase(std::string{key}) > 0;
}

bool Store::contains(std::string_view key) const {
    std::lock_guard lock(mutex_);
    return data_.find(std::string{key}) != data_.end();
}

std::size_t Store::size() const {
    std::lock_guard lock(mutex_);
    return data_.size();
}

void Store::clear() {
    std::lock_guard lock(mutex_);
    data_.clear();
}

std::vector<std::pair<std::string, std::string>> Store::snapshot() const {
    std::lock_guard lock(mutex_);

    std::vector<std::pair<std::string, std::string>> items;
    items.reserve(data_.size());

    for (const auto& [key, value] : data_) {
        items.emplace_back(key, value);
    }

    std::ranges::sort(items, {}, &std::pair<std::string, std::string>::first);
    return items;
}

} // namespace minikv
