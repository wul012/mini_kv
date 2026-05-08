#pragma once

#include "minikv/store.hpp"

#include <filesystem>
#include <mutex>
#include <string_view>

namespace minikv {

class WriteAheadLog {
public:
    explicit WriteAheadLog(std::filesystem::path path);

    bool append(std::string_view record);
    std::size_t replay(Store& store) const;

    const std::filesystem::path& path() const;

private:
    std::filesystem::path path_;
    mutable std::mutex mutex_;
};

} // namespace minikv
