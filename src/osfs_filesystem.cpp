#include "minikv/osfs/filesystem.hpp"

#include "osfs_disk_layout.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <tuple>

namespace minikv::osfs {

namespace {

using detail::DirectoryEntryDisk;
using detail::InodeDisk;
using detail::SuperBlockDisk;
using detail::UserRecordDisk;

struct SeedUser {
    const char* username;
    const char* password;
    std::uint32_t uid;
    std::uint32_t gid;
};

constexpr std::array seed_users = {
    SeedUser{"root", "root", 0, 0},
    SeedUser{"alice", "alice123", 1000, 100},
    SeedUser{"bob", "bob123", 1001, 100},
};

void validate_format_options(const FormatOptions& options) {
    if (options.block_size < sizeof(SuperBlockDisk) || options.block_size % sizeof(DirectoryEntryDisk) != 0) {
        throw std::invalid_argument{"osfs block size must be at least 512 and align directory entries"};
    }
    if (options.block_count > options.block_size * 8 || options.inode_count > options.block_size * 8) {
        throw std::invalid_argument{"osfs bitmap capacity exceeded"};
    }
    if (options.inode_count <= seed_users.size()) {
        throw std::invalid_argument{"osfs needs one MFD inode and one UFD inode per seeded user"};
    }
}

void initialize_directory_inode(InodeDisk& inode, std::uint32_t uid, std::uint32_t gid, std::uint16_t mode,
                                std::uint32_t block) {
    inode = {};
    inode.used = 1;
    inode.type = 2;
    inode.mode = mode;
    inode.owner_uid = uid;
    inode.owner_gid = gid;
    inode.created_at = detail::now_seconds();
    inode.accessed_at = inode.created_at;
    inode.modified_at = inode.created_at;
    inode.direct[0] = block;
}

} // namespace

FileSystem FileSystem::format(const std::filesystem::path& path, const FormatOptions& options) {
    validate_format_options(options);
    const auto inode_table_start = detail::user_table_start + detail::user_table_blocks;
    const auto inode_table_blocks = detail::inode_table_blocks_for(options.inode_count, options.block_size);
    const auto data_start_block = inode_table_start + inode_table_blocks;
    const auto required_directory_blocks = static_cast<std::uint32_t>(seed_users.size() + 1);
    if (data_start_block + required_directory_blocks >= options.block_count) {
        throw std::invalid_argument{"osfs geometry leaves no room for MFD/UFD data blocks"};
    }

    auto disk = VirtualDisk::create(path, options.block_count, options.block_size);
    SuperBlockDisk sb{};
    std::copy(detail::magic.begin(), detail::magic.end(), sb.magic);
    sb.version = detail::disk_version;
    sb.block_size = options.block_size;
    sb.block_count = options.block_count;
    sb.inode_count = options.inode_count;
    sb.block_bitmap_block = detail::block_bitmap_block;
    sb.inode_bitmap_block = detail::inode_bitmap_block;
    sb.user_table_start = detail::user_table_start;
    sb.user_table_blocks = detail::user_table_blocks;
    sb.inode_table_start = inode_table_start;
    sb.inode_table_blocks = inode_table_blocks;
    sb.data_start_block = data_start_block;
    sb.mfd_inode = detail::mfd_inode_index;
    sb.user_count = static_cast<std::uint32_t>(seed_users.size());

    std::vector<char> block_bitmap(options.block_size, '\0');
    std::vector<char> inode_bitmap(options.block_size, '\0');
    for (std::uint32_t block = 0; block < data_start_block; ++block) {
        detail::bitmap_set(block_bitmap, block, true);
    }

    std::vector<InodeDisk> inodes(options.inode_count);
    const auto mfd_inode = detail::allocate_inode(sb, inode_bitmap);
    const auto mfd_block = detail::allocate_block(sb, block_bitmap);
    if (!mfd_inode.has_value() || !mfd_block.has_value()) {
        throw std::runtime_error{"failed to allocate MFD"};
    }
    initialize_directory_inode(inodes[*mfd_inode], 0, 0, detail::default_mfd_mode, *mfd_block);

    const auto user_capacity =
        static_cast<std::size_t>(sb.user_table_blocks) * options.block_size / sizeof(UserRecordDisk);
    std::vector<UserRecordDisk> users(user_capacity);
    std::vector<DirectoryEntryDisk> mfd_entries(options.block_size / sizeof(DirectoryEntryDisk));

    for (std::size_t index = 0; index < seed_users.size(); ++index) {
        const auto ufd_inode = detail::allocate_inode(sb, inode_bitmap);
        const auto ufd_block = detail::allocate_block(sb, block_bitmap);
        if (!ufd_inode.has_value() || !ufd_block.has_value()) {
            throw std::runtime_error{"failed to allocate seeded user directory"};
        }

        const auto& seed = seed_users[index];
        initialize_directory_inode(inodes[*ufd_inode], seed.uid, seed.gid, detail::default_directory_mode, *ufd_block);

        auto& user = users[index];
        user.used = 1;
        user.uid = seed.uid;
        user.gid = seed.gid;
        user.ufd_inode = *ufd_inode;
        user.password_hash = detail::password_hash(seed.username, seed.password);
        std::strncpy(user.username, seed.username, sizeof(user.username) - 1);

        auto& mfd_entry = mfd_entries[index];
        mfd_entry.used = 1;
        mfd_entry.inode = *ufd_inode;
        std::strncpy(mfd_entry.name, seed.username, sizeof(mfd_entry.name) - 1);
        detail::clear_block(disk, *ufd_block);
    }

    inodes[*mfd_inode].size = seed_users.size() * sizeof(DirectoryEntryDisk);
    sb.free_block_count = detail::count_free(block_bitmap, sb.data_start_block, sb.block_count);
    sb.free_inode_count = detail::count_free(inode_bitmap, 0, sb.inode_count);

    detail::write_super_block(disk, sb);
    detail::write_block_bitmap(disk, sb, block_bitmap);
    detail::write_inode_bitmap(disk, sb, inode_bitmap);
    detail::write_users(disk, sb, users);
    detail::write_inodes(disk, sb, inodes);
    detail::write_directory_entries(disk, inodes[*mfd_inode], mfd_entries);

    return FileSystem{std::move(disk)};
}

FileSystem FileSystem::open(const std::filesystem::path& path) {
    auto raw = VirtualDisk::open(path, 512);
    const auto first_block = raw.read_block(0);
    SuperBlockDisk sb{};
    std::memcpy(&sb, first_block.data(), sizeof(sb));
    if (!std::equal(detail::magic.begin(), detail::magic.end(), sb.magic)) {
        throw std::runtime_error{"invalid osfs superblock"};
    }
    return FileSystem{VirtualDisk::open(path, sb.block_size)};
}

FileSystem::FileSystem(VirtualDisk disk) : disk_{std::move(disk)} { (void)detail::read_super_block(disk_); }

const std::filesystem::path& FileSystem::path() const { return disk_.path(); }

SuperBlock FileSystem::super_block() const {
    const auto sb = detail::read_super_block(disk_);
    return SuperBlock{sb.version,   sb.block_size,       sb.block_count,      sb.inode_count, sb.data_start_block,
                      sb.mfd_inode, sb.free_block_count, sb.free_inode_count, sb.user_count};
}

std::vector<UserInfo> FileSystem::users() const {
    const auto sb = detail::read_super_block(disk_);
    const auto records = detail::read_users(disk_, sb);
    std::vector<UserInfo> result;
    result.reserve(sb.user_count);
    for (const auto& record : records) {
        if (record.used != 0) {
            result.push_back(UserInfo{record.username, record.uid, record.gid, record.ufd_inode});
        }
    }
    std::sort(result.begin(), result.end(), [](const auto& left, const auto& right) { return left.uid < right.uid; });
    return result;
}

std::optional<UserInfo> FileSystem::authenticate(const std::string& username, const std::string& password) const {
    const auto sb = detail::read_super_block(disk_);
    const auto records = detail::read_users(disk_, sb);
    const auto* user = detail::find_user_by_name(records, username);
    if (user == nullptr || user->password_hash != detail::password_hash(username, password)) {
        return std::nullopt;
    }
    return UserInfo{user->username, user->uid, user->gid, user->ufd_inode};
}

std::optional<UserInfo> FileSystem::user_by_uid(std::uint32_t uid) const {
    const auto sb = detail::read_super_block(disk_);
    const auto records = detail::read_users(disk_, sb);
    const auto* user = detail::find_user_by_uid(records, uid);
    if (user == nullptr) {
        return std::nullopt;
    }
    return UserInfo{user->username, user->uid, user->gid, user->ufd_inode};
}

std::string format_mode(std::uint16_t mode) {
    return "0" + std::to_string((mode >> 6) & 7) + std::to_string((mode >> 3) & 7) + std::to_string(mode & 7);
}

} // namespace minikv::osfs
