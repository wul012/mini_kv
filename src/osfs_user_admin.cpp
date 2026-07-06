#include "minikv/osfs/filesystem.hpp"

#include "osfs_disk_layout.hpp"

#include <algorithm>
#include <cstring>

namespace minikv::osfs {

namespace {

void initialize_directory_inode(detail::InodeDisk& inode, std::uint32_t uid, std::uint32_t gid, std::uint16_t mode,
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

std::optional<std::size_t> find_free_slot(const std::vector<detail::DirectoryEntryDisk>& entries) {
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (entries[index].used == 0) {
            return index;
        }
    }
    return std::nullopt;
}

std::uint64_t directory_size(const std::vector<detail::DirectoryEntryDisk>& entries) {
    return static_cast<std::uint64_t>(
               std::count_if(entries.begin(), entries.end(), [](const auto& entry) { return entry.used != 0; })) *
           sizeof(detail::DirectoryEntryDisk);
}

std::uint32_t next_uid(const std::vector<detail::UserRecordDisk>& users) {
    std::uint32_t uid = 1000;
    for (const auto& user : users) {
        if (user.used != 0 && user.uid >= uid) {
            uid = user.uid + 1;
        }
    }
    return uid;
}

} // namespace

bool FileSystem::add_user(const std::string& username, const std::string& password, std::uint32_t actor_uid,
                          std::string* error) {
    if (actor_uid != 0) {
        detail::set_error(error, "USERADD requires root");
        return false;
    }
    if (!detail::valid_name(username)) {
        detail::set_error(error, "invalid username");
        return false;
    }
    if (password.empty()) {
        detail::set_error(error, "password required");
        return false;
    }

    auto sb = detail::read_super_block(disk_);
    auto users = detail::read_users(disk_, sb);
    if (detail::find_user_by_name(users, username) != nullptr) {
        detail::set_error(error, "user already exists");
        return false;
    }

    std::optional<std::size_t> free_user_slot;
    for (std::size_t index = 0; index < users.size(); ++index) {
        if (users[index].used == 0) {
            free_user_slot = index;
            break;
        }
    }
    if (!free_user_slot.has_value()) {
        detail::set_error(error, "user table is full");
        return false;
    }

    auto inodes = detail::read_inodes(disk_, sb);
    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    auto inode_bitmap = detail::read_inode_bitmap(disk_, sb);
    auto& mfd = inodes[sb.mfd_inode];
    auto mfd_entries = detail::read_directory_entries(disk_, mfd);

    auto mfd_slot = find_free_slot(mfd_entries);
    const auto needs_mfd_block = !mfd_slot.has_value();
    const auto needed_blocks = 1U + (needs_mfd_block ? 1U : 0U);
    if (detail::count_free(block_bitmap, sb.data_start_block, sb.block_count) < needed_blocks) {
        detail::set_error(error, "no free block for user directory");
        return false;
    }

    const auto ufd_inode = detail::allocate_inode(sb, inode_bitmap);
    if (!ufd_inode.has_value()) {
        detail::set_error(error, "inode table is full");
        return false;
    }
    const auto ufd_block = detail::allocate_block(sb, block_bitmap);
    if (!ufd_block.has_value()) {
        detail::release_inode(inode_bitmap, *ufd_inode);
        detail::set_error(error, "no free block for user directory");
        return false;
    }

    if (needs_mfd_block) {
        auto direct = std::find(std::begin(mfd.direct), std::end(mfd.direct), 0U);
        if (direct == std::end(mfd.direct)) {
            detail::release_inode(inode_bitmap, *ufd_inode);
            detail::release_block(block_bitmap, *ufd_block);
            detail::set_error(error, "MFD directory is full");
            return false;
        }
        const auto mfd_block = detail::allocate_block(sb, block_bitmap);
        if (!mfd_block.has_value()) {
            detail::release_inode(inode_bitmap, *ufd_inode);
            detail::release_block(block_bitmap, *ufd_block);
            detail::set_error(error, "no free block for MFD growth");
            return false;
        }
        *direct = *mfd_block;
        detail::clear_block(disk_, *mfd_block);
        const auto old_size = mfd_entries.size();
        mfd_entries.resize(old_size + disk_.block_size() / sizeof(detail::DirectoryEntryDisk));
        mfd_slot = old_size;
    }

    const auto uid = next_uid(users);
    initialize_directory_inode(inodes[*ufd_inode], uid, 100, detail::default_directory_mode, *ufd_block);
    detail::clear_block(disk_, *ufd_block);

    auto& user = users[*free_user_slot];
    user = {};
    user.used = 1;
    user.uid = uid;
    user.gid = 100;
    user.ufd_inode = *ufd_inode;
    user.password_hash = detail::password_hash(username, password);
    std::strncpy(user.username, username.c_str(), sizeof(user.username) - 1);

    auto& entry = mfd_entries[*mfd_slot];
    entry = {};
    entry.used = 1;
    entry.inode = *ufd_inode;
    std::strncpy(entry.name, username.c_str(), sizeof(entry.name) - 1);

    mfd.size = directory_size(mfd_entries);
    mfd.modified_at = detail::now_seconds();
    sb.user_count = static_cast<std::uint32_t>(
        std::count_if(users.begin(), users.end(), [](const auto& record) { return record.used != 0; }));
    sb.free_block_count = detail::count_free(block_bitmap, sb.data_start_block, sb.block_count);
    sb.free_inode_count = detail::count_free(inode_bitmap, 0, sb.inode_count);

    detail::write_directory_entries(disk_, mfd, mfd_entries);
    detail::write_users(disk_, sb, users);
    detail::write_inodes(disk_, sb, inodes);
    detail::write_block_bitmap(disk_, sb, block_bitmap);
    detail::write_inode_bitmap(disk_, sb, inode_bitmap);
    detail::write_super_block(disk_, sb);
    return true;
}

bool FileSystem::change_password(const std::string& username, const std::string& password, std::uint32_t actor_uid,
                                 std::string* error) {
    if (password.empty()) {
        detail::set_error(error, "password required");
        return false;
    }
    auto sb = detail::read_super_block(disk_);
    auto users = detail::read_users(disk_, sb);
    const auto* actor = detail::find_user_by_uid(users, actor_uid);
    if (actor == nullptr) {
        detail::set_error(error, "user not found");
        return false;
    }

    auto target = std::find_if(users.begin(), users.end(),
                               [&username](const auto& user) { return user.used != 0 && username == user.username; });
    if (target == users.end()) {
        detail::set_error(error, "user not found");
        return false;
    }
    if (actor_uid != 0 && username != actor->username) {
        detail::set_error(error, "permission denied");
        return false;
    }
    target->password_hash = detail::password_hash(username, password);
    detail::write_users(disk_, sb, users);
    return true;
}

} // namespace minikv::osfs
