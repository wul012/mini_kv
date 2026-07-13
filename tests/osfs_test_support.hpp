#pragma once

#include "minikv/osfs.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace osfs_test {

inline std::filesystem::path unique_disk_path(std::string_view scenario) {
    const auto suffix = std::chrono::system_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() /
           ("minikv-osfs-" + std::string{scenario} + "-" + std::to_string(suffix) + ".img");
}

inline int parse_fd(const std::string& response) {
    const auto marker = response.find("fd=");
    assert(marker != std::string::npos);
    return std::stoi(response.substr(marker + 3));
}

inline const minikv::osfs::UserInfo& require_user(const std::vector<minikv::osfs::UserInfo>& users,
                                                  const std::string& username) {
    for (const auto& user : users) {
        if (user.username == username) {
            return user;
        }
    }
    assert(false && "required seeded user missing");
    return users.front();
}

inline void set_bitmap_used(const std::filesystem::path& path, std::uint32_t block_size, std::uint32_t bitmap_block,
                            std::uint32_t index, bool used) {
    std::fstream image{path, std::ios::binary | std::ios::in | std::ios::out};
    assert(image);
    const auto byte_offset = static_cast<std::streamoff>(bitmap_block * block_size + (index / 8));
    image.seekg(byte_offset);
    char byte = 0;
    image.read(&byte, 1);
    assert(image.gcount() == 1);
    const auto mask = static_cast<unsigned char>(1U << (index % 8));
    auto value = static_cast<unsigned char>(byte);
    value = used ? static_cast<unsigned char>(value | mask) : static_cast<unsigned char>(value & ~mask);
    image.clear();
    image.seekp(byte_offset);
    byte = static_cast<char>(value);
    image.write(&byte, 1);
    assert(image);
}

inline void set_block_bitmap_used(const std::filesystem::path& path, std::uint32_t block_size, std::uint32_t block,
                                  bool used) {
    constexpr std::uint32_t block_bitmap_block = 1;
    set_bitmap_used(path, block_size, block_bitmap_block, block, used);
}

inline void set_inode_bitmap_used(const std::filesystem::path& path, std::uint32_t block_size, std::uint32_t inode,
                                  bool used) {
    constexpr std::uint32_t inode_bitmap_block = 2;
    set_bitmap_used(path, block_size, inode_bitmap_block, inode, used);
}

inline bool fsck_has_failed_check(const minikv::osfs::FsckReport& report, std::string_view name,
                                  std::string_view detail_fragment) {
    for (const auto& check : report.checks) {
        if (check.name == name && !check.ok && check.detail.find(detail_fragment) != std::string::npos) {
            return true;
        }
    }
    return false;
}

} // namespace osfs_test
