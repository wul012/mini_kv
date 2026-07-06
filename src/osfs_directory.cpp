#include "minikv/osfs/filesystem.hpp"

#include "osfs_disk_layout.hpp"

#include <algorithm>
#include <cstring>

namespace minikv::osfs {

namespace {

struct DirectoryContext {
    const detail::UserRecordDisk* owner = nullptr;
    std::vector<detail::DirectoryEntryDisk> entries;
};

std::optional<DirectoryContext> load_directory(const VirtualDisk& disk, const detail::SuperBlockDisk& sb,
                                               const std::vector<detail::InodeDisk>& inodes,
                                               const std::vector<detail::UserRecordDisk>& users,
                                               const std::string& reference, std::uint32_t actor_uid,
                                               std::string* error) {
    const auto resolved = detail::resolve_file_reference(users, reference, actor_uid, error);
    if (!resolved.has_value()) {
        return std::nullopt;
    }
    if (resolved->directory_owner->ufd_inode >= sb.inode_count ||
        inodes[resolved->directory_owner->ufd_inode].type != 2) {
        detail::set_error(error, "invalid user directory");
        return std::nullopt;
    }
    return DirectoryContext{resolved->directory_owner,
                            detail::read_directory_entries(disk, inodes[resolved->directory_owner->ufd_inode])};
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

} // namespace

bool FileSystem::create_file(const std::string& name, std::uint32_t actor_uid, std::string* error) {
    if (!detail::valid_name(name)) {
        detail::set_error(error, "invalid file name");
        return false;
    }

    auto sb = detail::read_super_block(disk_);
    auto users = detail::read_users(disk_, sb);
    const auto* actor = detail::find_user_by_uid(users, actor_uid);
    if (actor == nullptr) {
        detail::set_error(error, "user not found");
        return false;
    }

    auto inodes = detail::read_inodes(disk_, sb);
    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    auto inode_bitmap = detail::read_inode_bitmap(disk_, sb);
    auto& ufd = inodes[actor->ufd_inode];
    auto entries = detail::read_directory_entries(disk_, ufd);
    if (detail::find_entry_index(entries, name).has_value()) {
        detail::set_error(error, "file already exists");
        return false;
    }

    const auto inode_index = detail::allocate_inode(sb, inode_bitmap);
    if (!inode_index.has_value()) {
        detail::set_error(error, "inode table is full");
        return false;
    }

    auto slot = find_free_slot(entries);
    if (!slot.has_value()) {
        auto direct = std::find(std::begin(ufd.direct), std::end(ufd.direct), 0U);
        if (direct == std::end(ufd.direct)) {
            detail::release_inode(inode_bitmap, *inode_index);
            detail::set_error(error, "user directory is full");
            return false;
        }
        const auto block = detail::allocate_block(sb, block_bitmap);
        if (!block.has_value()) {
            detail::release_inode(inode_bitmap, *inode_index);
            detail::set_error(error, "no free block for directory growth");
            return false;
        }
        *direct = *block;
        detail::clear_block(disk_, *block);
        const auto old_size = entries.size();
        entries.resize(old_size + disk_.block_size() / sizeof(detail::DirectoryEntryDisk));
        slot = old_size;
    }

    auto& inode = inodes[*inode_index];
    inode = {};
    inode.used = 1;
    inode.type = 1;
    inode.mode = detail::default_file_mode;
    inode.owner_uid = actor->uid;
    inode.owner_gid = actor->gid;
    inode.created_at = detail::now_seconds();
    inode.accessed_at = inode.created_at;
    inode.modified_at = inode.created_at;

    auto& entry = entries[*slot];
    entry = {};
    entry.used = 1;
    entry.inode = *inode_index;
    std::strncpy(entry.name, name.c_str(), sizeof(entry.name) - 1);

    ufd.size = directory_size(entries);
    ufd.modified_at = detail::now_seconds();
    sb.free_block_count = detail::count_free(block_bitmap, sb.data_start_block, sb.block_count);
    sb.free_inode_count = detail::count_free(inode_bitmap, 0, sb.inode_count);
    detail::write_directory_entries(disk_, ufd, entries);
    detail::write_inodes(disk_, sb, inodes);
    detail::write_block_bitmap(disk_, sb, block_bitmap);
    detail::write_inode_bitmap(disk_, sb, inode_bitmap);
    detail::write_super_block(disk_, sb);
    return true;
}

bool FileSystem::delete_file(const std::string& name, std::uint32_t uid, std::string* error) {
    auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    const auto* actor = detail::find_user_by_uid(users, uid);
    if (actor == nullptr) {
        detail::set_error(error, "user not found");
        return false;
    }
    auto inodes = detail::read_inodes(disk_, sb);
    const auto context = load_directory(disk_, sb, inodes, users, name, uid, error);
    if (!context.has_value()) {
        return false;
    }
    const auto reference = detail::resolve_file_reference(users, name, uid, error);
    const auto entry_index = detail::find_entry_index(context->entries, reference->filename);
    if (!entry_index.has_value()) {
        detail::set_error(error, "file not found");
        return false;
    }

    auto& inode = inodes[context->entries[*entry_index].inode];
    if (uid != 0 && uid != inode.owner_uid) {
        detail::set_error(error, "delete requires owner or root");
        return false;
    }

    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    auto inode_bitmap = detail::read_inode_bitmap(disk_, sb);
    detail::release_inode_storage(disk_, inode, block_bitmap);
    detail::release_inode(inode_bitmap, context->entries[*entry_index].inode);
    inode = {};

    auto entries = context->entries;
    entries[*entry_index] = {};
    auto& ufd = inodes[context->owner->ufd_inode];
    ufd.size = directory_size(entries);
    ufd.modified_at = detail::now_seconds();
    sb.free_block_count = detail::count_free(block_bitmap, sb.data_start_block, sb.block_count);
    sb.free_inode_count = detail::count_free(inode_bitmap, 0, sb.inode_count);
    detail::write_directory_entries(disk_, ufd, entries);
    detail::write_inodes(disk_, sb, inodes);
    detail::write_block_bitmap(disk_, sb, block_bitmap);
    detail::write_inode_bitmap(disk_, sb, inode_bitmap);
    detail::write_super_block(disk_, sb);
    return true;
}

bool FileSystem::chmod(const std::string& name, std::uint16_t mode, std::uint32_t uid, std::string* error) {
    const auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    auto inodes = detail::read_inodes(disk_, sb);
    const auto context = load_directory(disk_, sb, inodes, users, name, uid, error);
    const auto reference = detail::resolve_file_reference(users, name, uid, error);
    if (!context.has_value() || !reference.has_value()) {
        return false;
    }
    const auto entry_index = detail::find_entry_index(context->entries, reference->filename);
    if (!entry_index.has_value()) {
        detail::set_error(error, "file not found");
        return false;
    }
    auto& inode = inodes[context->entries[*entry_index].inode];
    if (uid != 0 && uid != inode.owner_uid) {
        detail::set_error(error, "chmod requires owner or root");
        return false;
    }
    inode.mode = detail::normalize_mode(mode);
    inode.modified_at = detail::now_seconds();
    detail::write_inodes(disk_, sb, inodes);
    return true;
}

std::vector<DirectoryEntry> FileSystem::list_directory(std::uint32_t uid) const {
    const auto user = user_by_uid(uid);
    if (!user.has_value()) {
        return {};
    }
    std::string error;
    const auto entries = list_directory_for(user->username, uid, &error);
    return entries.value_or(std::vector<DirectoryEntry>{});
}

std::optional<std::vector<DirectoryEntry>>
FileSystem::list_directory_for(const std::string& username, std::uint32_t actor_uid, std::string* error) const {
    const auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    const auto* actor = detail::find_user_by_uid(users, actor_uid);
    const auto* owner = detail::find_user_by_name(users, username);
    if (actor == nullptr || owner == nullptr) {
        detail::set_error(error, "user not found");
        return std::nullopt;
    }
    if (actor_uid != 0 && actor_uid != owner->uid) {
        detail::set_error(error, "permission denied");
        return std::nullopt;
    }

    auto inodes = detail::read_inodes(disk_, sb);
    auto& ufd = inodes[owner->ufd_inode];
    const auto raw_entries = detail::read_directory_entries(disk_, ufd);
    std::vector<DirectoryEntry> result;
    for (const auto& entry : raw_entries) {
        if (entry.used == 0 || entry.inode >= inodes.size() || inodes[entry.inode].used == 0) {
            continue;
        }
        const auto& inode = inodes[entry.inode];
        result.push_back(DirectoryEntry{entry.name, entry.inode, inode.direct[0], inode.mode, inode.owner_uid,
                                        inode.owner_gid, inode.size});
    }
    std::sort(result.begin(), result.end(), [](const auto& left, const auto& right) { return left.name < right.name; });
    ufd.accessed_at = detail::now_seconds();
    detail::write_inodes(disk_, sb, inodes);
    return result;
}

std::optional<FileStat> FileSystem::stat(const std::string& name, std::uint32_t actor_uid) const {
    const auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    const auto inodes = detail::read_inodes(disk_, sb);
    std::string error;
    const auto context = load_directory(disk_, sb, inodes, users, name, actor_uid, &error);
    const auto reference = detail::resolve_file_reference(users, name, actor_uid, &error);
    if (!context.has_value() || !reference.has_value()) {
        return std::nullopt;
    }
    const auto entry_index = detail::find_entry_index(context->entries, reference->filename);
    if (!entry_index.has_value()) {
        return std::nullopt;
    }
    const auto& entry = context->entries[*entry_index];
    const auto& inode = inodes[entry.inode];
    return FileStat{entry.name,      entry.inode, inode.direct[0],  inode.mode,        inode.owner_uid,
                    inode.owner_gid, inode.size,  inode.created_at, inode.accessed_at, inode.modified_at};
}

bool FileSystem::can_read(const std::string& name, std::uint32_t uid) const {
    const auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    const auto* actor = detail::find_user_by_uid(users, uid);
    const auto value = stat(name, uid);
    if (actor == nullptr || !value.has_value()) {
        return false;
    }
    const auto inodes = detail::read_inodes(disk_, sb);
    return detail::has_permission(inodes[value->inode], uid, actor->gid, owner_read, group_read, other_read);
}

bool FileSystem::can_write(const std::string& name, std::uint32_t uid) const {
    const auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    const auto* actor = detail::find_user_by_uid(users, uid);
    const auto value = stat(name, uid);
    if (actor == nullptr || !value.has_value()) {
        return false;
    }
    const auto inodes = detail::read_inodes(disk_, sb);
    return detail::has_permission(inodes[value->inode], uid, actor->gid, owner_write, group_write, other_write);
}

} // namespace minikv::osfs
