#pragma once

#include "minikv/osfs/virtual_disk.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::osfs::detail {

constexpr std::array<char, 8> magic = {'M', 'K', 'V', 'O', 'S', 'F', 'S', '\0'};
constexpr std::uint32_t disk_version = 2;
constexpr std::uint32_t block_bitmap_block = 1;
constexpr std::uint32_t inode_bitmap_block = 2;
constexpr std::uint32_t user_table_start = 3;
constexpr std::uint32_t user_table_blocks = 1;
constexpr std::uint32_t max_direct_blocks = 8;
constexpr std::uint32_t mfd_inode_index = 0;
constexpr std::uint16_t default_file_mode = 0644;
constexpr std::uint16_t default_directory_mode = 0700;
constexpr std::uint16_t default_mfd_mode = 0700;

#pragma pack(push, 1)
struct SuperBlockDisk {
    char magic[8];
    std::uint32_t version;
    std::uint32_t block_size;
    std::uint32_t block_count;
    std::uint32_t inode_count;
    std::uint32_t block_bitmap_block;
    std::uint32_t inode_bitmap_block;
    std::uint32_t user_table_start;
    std::uint32_t user_table_blocks;
    std::uint32_t inode_table_start;
    std::uint32_t inode_table_blocks;
    std::uint32_t data_start_block;
    std::uint32_t mfd_inode;
    std::uint32_t free_block_count;
    std::uint32_t free_inode_count;
    std::uint32_t user_count;
    char reserved[444];
};

struct InodeDisk {
    std::uint8_t used;
    std::uint8_t type;
    std::uint16_t mode;
    std::uint32_t owner_uid;
    std::uint32_t owner_gid;
    std::uint64_t size;
    std::uint64_t created_at;
    std::uint64_t accessed_at;
    std::uint64_t modified_at;
    std::uint32_t direct[max_direct_blocks];
    char reserved[20];
};

struct DirectoryEntryDisk {
    std::uint8_t used;
    std::uint32_t inode;
    char name[55];
    char reserved[4];
};

struct UserRecordDisk {
    std::uint8_t used;
    char padding[3];
    std::uint32_t uid;
    std::uint32_t gid;
    std::uint32_t ufd_inode;
    std::uint64_t password_hash;
    char username[32];
    char reserved[8];
};
#pragma pack(pop)

struct FileReference {
    const UserRecordDisk* directory_owner = nullptr;
    std::string filename;
};

static_assert(sizeof(SuperBlockDisk) == 512);
static_assert(sizeof(InodeDisk) == 96);
static_assert(sizeof(DirectoryEntryDisk) == 64);
static_assert(sizeof(UserRecordDisk) == 64);

std::uint64_t now_seconds();
void set_error(std::string* error, std::string message);
bool valid_name(std::string_view name);
std::uint16_t normalize_mode(std::uint16_t mode);
std::uint32_t inode_table_blocks_for(std::uint32_t inode_count, std::uint32_t block_size);

SuperBlockDisk read_super_block(const VirtualDisk& disk);
void write_super_block(VirtualDisk& disk, const SuperBlockDisk& sb);

std::vector<char> read_block_bitmap(const VirtualDisk& disk, const SuperBlockDisk& sb);
std::vector<char> read_inode_bitmap(const VirtualDisk& disk, const SuperBlockDisk& sb);
void write_block_bitmap(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<char>& bitmap);
void write_inode_bitmap(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<char>& bitmap);
bool bitmap_used(const std::vector<char>& bitmap, std::uint32_t index);
void bitmap_set(std::vector<char>& bitmap, std::uint32_t index, bool used);

std::vector<InodeDisk> read_inodes(const VirtualDisk& disk, const SuperBlockDisk& sb);
void write_inodes(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<InodeDisk>& inodes);
std::vector<UserRecordDisk> read_users(const VirtualDisk& disk, const SuperBlockDisk& sb);
void write_users(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<UserRecordDisk>& users);

std::vector<DirectoryEntryDisk> read_directory_entries(const VirtualDisk& disk, const InodeDisk& directory);
void write_directory_entries(VirtualDisk& disk, const InodeDisk& directory,
                             const std::vector<DirectoryEntryDisk>& entries);
std::optional<std::size_t> find_entry_index(const std::vector<DirectoryEntryDisk>& entries, std::string_view name);

std::optional<std::uint32_t> allocate_block(const SuperBlockDisk& sb, std::vector<char>& bitmap);
std::optional<std::uint32_t> allocate_inode(const SuperBlockDisk& sb, std::vector<char>& bitmap);
void release_block(std::vector<char>& bitmap, std::uint32_t block);
void release_inode(std::vector<char>& bitmap, std::uint32_t inode);
std::uint32_t count_free(const std::vector<char>& bitmap, std::uint32_t begin, std::uint32_t end);
void clear_block(VirtualDisk& disk, std::uint32_t block_index);

const UserRecordDisk* find_user_by_name(const std::vector<UserRecordDisk>& users, std::string_view username);
const UserRecordDisk* find_user_by_uid(const std::vector<UserRecordDisk>& users, std::uint32_t uid);
std::optional<FileReference> resolve_file_reference(const std::vector<UserRecordDisk>& users,
                                                    std::string_view reference, std::uint32_t actor_uid,
                                                    std::string* error);
std::uint64_t password_hash(std::string_view username, std::string_view password);
bool has_permission(const InodeDisk& inode, std::uint32_t uid, std::uint32_t gid, std::uint16_t owner_bit,
                    std::uint16_t group_bit, std::uint16_t other_bit);

} // namespace minikv::osfs::detail
