#pragma once

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

    std::vector<std::pair<std::string, std::string>> snapshot() const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> data_;
};

} // namespace minikv
