#pragma once

#include "minikv/store.hpp"

#include <cstddef>
#include <filesystem>

namespace minikv {

class SnapshotFile {
public:
    static bool save(const Store& store, const std::filesystem::path& path, std::size_t* saved = nullptr);
    static bool load(Store& store, const std::filesystem::path& path, std::size_t* loaded = nullptr);
};

} // namespace minikv
