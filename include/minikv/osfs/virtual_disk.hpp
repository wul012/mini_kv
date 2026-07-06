#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace minikv::osfs {

class VirtualDisk {
public:
    static VirtualDisk create(const std::filesystem::path& path, std::uint32_t block_count, std::uint32_t block_size);
    static VirtualDisk open(const std::filesystem::path& path, std::uint32_t block_size);

    const std::filesystem::path& path() const;
    std::uint32_t block_size() const;
    std::uint32_t block_count() const;

    std::vector<char> read_block(std::uint32_t block_index) const;
    void write_block(std::uint32_t block_index, const std::vector<char>& bytes);

private:
    VirtualDisk(std::filesystem::path path, std::uint32_t block_count, std::uint32_t block_size);

    std::filesystem::path path_;
    std::uint32_t block_count_;
    std::uint32_t block_size_;
};

} // namespace minikv::osfs
