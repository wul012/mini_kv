#pragma once

#include "minikv/osfs/virtual_disk.hpp"

#include <cstdint>
#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace minikv::osfs {

constexpr std::uint16_t owner_read = 0400;
constexpr std::uint16_t owner_write = 0200;
constexpr std::uint16_t group_read = 0040;
constexpr std::uint16_t group_write = 0020;
constexpr std::uint16_t other_read = 0004;
constexpr std::uint16_t other_write = 0002;

struct FormatOptions {
    std::uint32_t block_size = 512;
    std::uint32_t block_count = 256;
    std::uint32_t inode_count = 64;
};

struct SuperBlock {
    std::uint32_t version = 3;
    std::uint32_t block_size = 0;
    std::uint32_t block_count = 0;
    std::uint32_t inode_count = 0;
    std::uint32_t data_start_block = 0;
    std::uint32_t mfd_inode = 0;
    std::uint32_t free_block_count = 0;
    std::uint32_t free_inode_count = 0;
    std::uint32_t user_count = 0;
};

struct UserInfo {
    std::string username;
    std::uint32_t uid = 0;
    std::uint32_t gid = 0;
    std::uint32_t ufd_inode = 0;
};

struct DirectoryEntry {
    std::string name;
    std::uint32_t inode = 0;
    std::uint32_t first_block = 0;
    std::uint16_t mode = 0;
    std::uint32_t owner_uid = 0;
    std::uint32_t owner_gid = 0;
    std::uint64_t size = 0;
};

struct FileStat {
    std::string name;
    std::uint32_t inode = 0;
    std::uint32_t first_block = 0;
    std::uint16_t mode = 0;
    std::uint32_t owner_uid = 0;
    std::uint32_t owner_gid = 0;
    std::uint64_t size = 0;
    std::uint64_t created_at = 0;
    std::uint64_t accessed_at = 0;
    std::uint64_t modified_at = 0;
};

struct FsckCheck {
    std::string name;
    bool ok = false;
    std::string detail;
};

struct FsckReport {
    bool ok = false;
    std::vector<FsckCheck> checks;
};

class FileSystem {
public:
    static FileSystem format(const std::filesystem::path& path, const FormatOptions& options = {});
    static FileSystem open(const std::filesystem::path& path);

    const std::filesystem::path& path() const;
    SuperBlock super_block() const;
    std::vector<UserInfo> users() const;
    std::optional<UserInfo> authenticate(const std::string& username, const std::string& password) const;
    std::optional<UserInfo> user_by_uid(std::uint32_t uid) const;
    FsckReport check_consistency() const;
    bool add_user(const std::string& username, const std::string& password, std::uint32_t actor_uid,
                  std::string* error = nullptr);
    bool change_password(const std::string& username, const std::string& password, std::uint32_t actor_uid,
                         std::string* error = nullptr);

    bool create_file(const std::string& name, std::uint32_t actor_uid, std::string* error = nullptr);
    bool delete_file(const std::string& name, std::uint32_t uid, std::string* error = nullptr);
    bool write_file(const std::string& name, const std::string& contents, std::uint32_t uid,
                    std::string* error = nullptr);
    std::optional<std::string> read_file(const std::string& name, std::uint32_t uid,
                                         std::string* error = nullptr) const;
    bool truncate_file(const std::string& name, std::uint32_t uid, std::string* error = nullptr);
    bool write_file_range(const std::string& name, std::size_t offset, const std::string& contents, std::uint32_t uid,
                          std::string* error = nullptr);
    std::optional<std::string> read_file_range(const std::string& name, std::size_t offset,
                                               std::optional<std::size_t> length, std::uint32_t uid,
                                               std::string* error = nullptr) const;
    bool chmod(const std::string& name, std::uint16_t mode, std::uint32_t uid, std::string* error = nullptr);

    std::vector<DirectoryEntry> list_directory(std::uint32_t uid = 0) const;
    std::optional<std::vector<DirectoryEntry>> list_directory_for(const std::string& username, std::uint32_t actor_uid,
                                                                  std::string* error = nullptr) const;
    std::optional<FileStat> stat(const std::string& name, std::uint32_t actor_uid = 0) const;
    bool can_read(const std::string& name, std::uint32_t uid) const;
    bool can_write(const std::string& name, std::uint32_t uid) const;

private:
    explicit FileSystem(VirtualDisk disk);

    mutable VirtualDisk disk_;
};

std::string format_mode(std::uint16_t mode);

} // namespace minikv::osfs
