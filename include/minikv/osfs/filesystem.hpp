#pragma once

#include "minikv/osfs/virtual_disk.hpp"

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace minikv::osfs {

constexpr std::uint16_t owner_read = 0400;
constexpr std::uint16_t owner_write = 0200;
constexpr std::uint16_t other_read = 0004;
constexpr std::uint16_t other_write = 0002;

struct FormatOptions {
    std::uint32_t block_size = 512;
    std::uint32_t block_count = 256;
    std::uint32_t inode_count = 64;
};

struct SuperBlock {
    std::uint32_t version = 1;
    std::uint32_t block_size = 0;
    std::uint32_t block_count = 0;
    std::uint32_t inode_count = 0;
    std::uint32_t data_start_block = 0;
    std::uint32_t root_inode = 0;
};

struct DirectoryEntry {
    std::string name;
    std::uint32_t inode = 0;
    std::uint32_t first_block = 0;
    std::uint16_t mode = 0;
    std::uint32_t owner_uid = 0;
    std::uint64_t size = 0;
};

struct FileStat {
    std::string name;
    std::uint32_t inode = 0;
    std::uint32_t first_block = 0;
    std::uint16_t mode = 0;
    std::uint32_t owner_uid = 0;
    std::uint64_t size = 0;
    std::uint64_t created_at = 0;
    std::uint64_t modified_at = 0;
};

class FileSystem {
public:
    static FileSystem format(const std::filesystem::path& path, const FormatOptions& options = {});
    static FileSystem open(const std::filesystem::path& path);

    const std::filesystem::path& path() const;
    SuperBlock super_block() const;

    bool create_file(const std::string& name, std::uint32_t owner_uid, std::string* error = nullptr);
    bool delete_file(const std::string& name, std::uint32_t uid, std::string* error = nullptr);
    bool write_file(const std::string& name, const std::string& contents, std::uint32_t uid,
                    std::string* error = nullptr);
    std::optional<std::string> read_file(const std::string& name, std::uint32_t uid,
                                         std::string* error = nullptr) const;
    bool chmod(const std::string& name, std::uint16_t mode, std::uint32_t uid, std::string* error = nullptr);

    std::vector<DirectoryEntry> list_directory() const;
    std::optional<FileStat> stat(const std::string& name) const;
    bool can_read(const std::string& name, std::uint32_t uid) const;
    bool can_write(const std::string& name, std::uint32_t uid) const;

private:
    explicit FileSystem(VirtualDisk disk);

    VirtualDisk disk_;
};

std::uint32_t uid_for_user(const std::string& user);
std::string format_mode(std::uint16_t mode);

} // namespace minikv::osfs
