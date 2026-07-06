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
    if (needed_blocks > detail::max_direct_blocks) {
        detail::set_error(error, "file exceeds direct-block limit");
        return false;
    }

    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    const auto reclaimable = static_cast<std::uint32_t>(std::count_if(std::begin(inode.direct), std::end(inode.direct),
                                                                      [](std::uint32_t block) { return block != 0; }));
    if (detail::count_free(block_bitmap, sb.data_start_block, sb.block_count) + reclaimable < needed_blocks) {
        detail::set_error(error, "no free data block");
        return false;
    }

    for (auto& direct : inode.direct) {
        if (direct != 0) {
            detail::release_block(block_bitmap, direct);
            detail::clear_block(disk_, direct);
            direct = 0;
        }
    }

    std::size_t offset = 0;
    for (std::uint32_t index = 0; index < needed_blocks; ++index) {
        const auto block = detail::allocate_block(sb, block_bitmap);
        if (!block.has_value()) {
            detail::set_error(error, "no free data block");
            return false;
        }
        inode.direct[index] = *block;
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

    std::string contents;
    contents.reserve(static_cast<std::size_t>(inode.size));
    auto remaining = static_cast<std::size_t>(inode.size);
    for (const auto block_index : inode.direct) {
        if (block_index == 0 || remaining == 0) {
            break;
        }
        const auto block = disk_.read_block(block_index);
        const auto count = std::min<std::size_t>(remaining, block.size());
        contents.append(block.data(), count);
        remaining -= count;
    }
    inode.accessed_at = detail::now_seconds();
    detail::write_inodes(disk_, sb, inodes);
    return contents;
}

} // namespace minikv::osfs
