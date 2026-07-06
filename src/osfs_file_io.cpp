#include "minikv/osfs/filesystem.hpp"

#include "osfs_disk_layout.hpp"

#include <algorithm>
#include <cstring>

namespace minikv::osfs {

namespace {

struct LocatedFile {
    const detail::UserRecordDisk* actor = nullptr;
    std::uint32_t inode = 0;
};

std::optional<LocatedFile> locate_file(const VirtualDisk& disk, const std::vector<detail::InodeDisk>& inodes,
                                       const std::vector<detail::UserRecordDisk>& users, const std::string& reference,
                                       std::uint32_t actor_uid, std::string* error) {
    const auto* actor = detail::find_user_by_uid(users, actor_uid);
    const auto resolved = detail::resolve_file_reference(users, reference, actor_uid, error);
    if (actor == nullptr) {
        detail::set_error(error, "user not found");
        return std::nullopt;
    }
    if (!resolved.has_value()) {
        return std::nullopt;
    }
    const auto& ufd = inodes[resolved->directory_owner->ufd_inode];
    const auto entries = detail::read_directory_entries(disk, ufd);
    const auto entry = detail::find_entry_index(entries, resolved->filename);
    if (!entry.has_value()) {
        detail::set_error(error, "file not found");
        return std::nullopt;
    }
    return LocatedFile{actor, entries[*entry].inode};
}

} // namespace

bool FileSystem::write_file(const std::string& name, const std::string& contents, std::uint32_t uid,
                            std::string* error) {
    auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    auto inodes = detail::read_inodes(disk_, sb);
    const auto located = locate_file(disk_, inodes, users, name, uid, error);
    if (!located.has_value()) {
        return false;
    }

    auto& inode = inodes[located->inode];
    if (!detail::has_permission(inode, uid, located->actor->gid, owner_write, group_write, other_write)) {
        detail::set_error(error, "permission denied");
        return false;
    }

    const auto needed_blocks =
        contents.empty() ? 0U
                         : static_cast<std::uint32_t>((contents.size() + disk_.block_size() - 1) / disk_.block_size());
    if (needed_blocks > detail::max_file_blocks(disk_.block_size())) {
        detail::set_error(error, "file exceeds indirect-block limit");
        return false;
    }

    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    const auto reclaimable = detail::inode_data_block_count(disk_, inode) + (inode.indirect_block != 0 ? 1U : 0U);
    const auto required = needed_blocks + (needed_blocks > detail::max_direct_blocks ? 1U : 0U);
    if (detail::count_free(block_bitmap, sb.data_start_block, sb.block_count) + reclaimable < required) {
        detail::set_error(error, "no free data block");
        return false;
    }

    detail::release_inode_storage(disk_, inode, block_bitmap);
    if (!detail::ensure_inode_data_blocks(disk_, sb, inode, block_bitmap, needed_blocks, error)) {
        return false;
    }

    std::size_t offset = 0;
    for (std::uint32_t index = 0; index < needed_blocks; ++index) {
        const auto block = detail::inode_data_block_at(disk_, inode, index);
        if (!block.has_value()) {
            detail::set_error(error, "internal missing data block");
            return false;
        }
        std::vector<char> bytes(disk_.block_size(), '\0');
        const auto count = std::min<std::size_t>(bytes.size(), contents.size() - offset);
        std::memcpy(bytes.data(), contents.data() + offset, count);
        disk_.write_block(*block, bytes);
        offset += count;
    }

    inode.size = contents.size();
    inode.modified_at = detail::now_seconds();
    sb.free_block_count = detail::count_free(block_bitmap, sb.data_start_block, sb.block_count);
    detail::write_inodes(disk_, sb, inodes);
    detail::write_block_bitmap(disk_, sb, block_bitmap);
    detail::write_super_block(disk_, sb);
    return true;
}

std::optional<std::string> FileSystem::read_file(const std::string& name, std::uint32_t uid, std::string* error) const {
    return read_file_range(name, 0, std::nullopt, uid, error);
}

bool FileSystem::truncate_file(const std::string& name, std::uint32_t uid, std::string* error) {
    auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    auto inodes = detail::read_inodes(disk_, sb);
    const auto located = locate_file(disk_, inodes, users, name, uid, error);
    if (!located.has_value()) {
        return false;
    }

    auto& inode = inodes[located->inode];
    if (!detail::has_permission(inode, uid, located->actor->gid, owner_write, group_write, other_write)) {
        detail::set_error(error, "permission denied");
        return false;
    }

    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    detail::release_inode_storage(disk_, inode, block_bitmap);
    inode.size = 0;
    inode.modified_at = detail::now_seconds();
    sb.free_block_count = detail::count_free(block_bitmap, sb.data_start_block, sb.block_count);
    detail::write_inodes(disk_, sb, inodes);
    detail::write_block_bitmap(disk_, sb, block_bitmap);
    detail::write_super_block(disk_, sb);
    return true;
}

bool FileSystem::write_file_range(const std::string& name, std::size_t offset, const std::string& contents,
                                  std::uint32_t uid, std::string* error) {
    auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    auto inodes = detail::read_inodes(disk_, sb);
    const auto located = locate_file(disk_, inodes, users, name, uid, error);
    if (!located.has_value()) {
        return false;
    }

    auto& inode = inodes[located->inode];
    if (!detail::has_permission(inode, uid, located->actor->gid, owner_write, group_write, other_write)) {
        detail::set_error(error, "permission denied");
        return false;
    }
    if (contents.empty()) {
        return true;
    }

    const auto max_size = static_cast<std::size_t>(detail::max_file_blocks(disk_.block_size())) * disk_.block_size();
    if (offset > max_size || contents.size() > max_size - offset) {
        detail::set_error(error, "file exceeds indirect-block limit");
        return false;
    }
    const auto target_size = std::max<std::size_t>(static_cast<std::size_t>(inode.size), offset + contents.size());
    const auto needed_blocks = static_cast<std::uint32_t>((target_size + disk_.block_size() - 1) / disk_.block_size());

    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    if (!detail::ensure_inode_data_blocks(disk_, sb, inode, block_bitmap, needed_blocks, error)) {
        return false;
    }

    std::size_t source_offset = 0;
    std::size_t file_offset = offset;
    while (source_offset < contents.size()) {
        const auto logical_index = static_cast<std::uint32_t>(file_offset / disk_.block_size());
        const auto within_block = file_offset % disk_.block_size();
        const auto block_index = detail::inode_data_block_at(disk_, inode, logical_index);
        if (!block_index.has_value()) {
            detail::set_error(error, "internal missing data block");
            return false;
        }
        auto block = disk_.read_block(*block_index);
        const auto count = std::min(contents.size() - source_offset, block.size() - within_block);
        std::memcpy(block.data() + within_block, contents.data() + source_offset, count);
        disk_.write_block(*block_index, block);
        source_offset += count;
        file_offset += count;
    }

    inode.size = target_size;
    inode.modified_at = detail::now_seconds();
    sb.free_block_count = detail::count_free(block_bitmap, sb.data_start_block, sb.block_count);
    detail::write_inodes(disk_, sb, inodes);
    detail::write_block_bitmap(disk_, sb, block_bitmap);
    detail::write_super_block(disk_, sb);
    return true;
}

std::optional<std::string> FileSystem::read_file_range(const std::string& name, std::size_t offset,
                                                       std::optional<std::size_t> length, std::uint32_t uid,
                                                       std::string* error) const {
    const auto sb = detail::read_super_block(disk_);
    const auto users = detail::read_users(disk_, sb);
    auto inodes = detail::read_inodes(disk_, sb);
    const auto located = locate_file(disk_, inodes, users, name, uid, error);
    if (!located.has_value()) {
        return std::nullopt;
    }

    auto& inode = inodes[located->inode];
    if (!detail::has_permission(inode, uid, located->actor->gid, owner_read, group_read, other_read)) {
        detail::set_error(error, "permission denied");
        return std::nullopt;
    }

    const auto file_size = static_cast<std::size_t>(inode.size);
    if (offset >= file_size) {
        inode.accessed_at = detail::now_seconds();
        detail::write_inodes(disk_, sb, inodes);
        return std::string{};
    }

    auto remaining = std::min(length.value_or(file_size - offset), file_size - offset);
    std::string contents;
    contents.reserve(remaining);
    auto file_offset = offset;
    while (remaining > 0) {
        const auto logical_index = static_cast<std::uint32_t>(file_offset / disk_.block_size());
        const auto within_block = file_offset % disk_.block_size();
        const auto block_index = detail::inode_data_block_at(disk_, inode, logical_index);
        if (!block_index.has_value()) {
            detail::set_error(error, "internal missing data block");
            return std::nullopt;
        }
        const auto block = disk_.read_block(*block_index);
        const auto count = std::min(remaining, block.size() - within_block);
        contents.append(block.data() + within_block, count);
        file_offset += count;
        remaining -= count;
    }
    inode.accessed_at = detail::now_seconds();
    detail::write_inodes(disk_, sb, inodes);
    return contents;
}

} // namespace minikv::osfs
