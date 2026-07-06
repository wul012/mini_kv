#include "osfs_disk_layout.hpp"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <stdexcept>

namespace minikv::osfs::detail {

std::uint64_t now_seconds() {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

void set_error(std::string* error, std::string message) {
    if (error != nullptr) {
        *error = std::move(message);
    }
}

bool valid_name(std::string_view name) {
    return !name.empty() && name.size() < 55 && name != "." && name != ".." &&
           name.find('/') == std::string_view::npos && name.find('\\') == std::string_view::npos;
}

std::uint16_t normalize_mode(std::uint16_t mode) { return static_cast<std::uint16_t>(mode & 0777); }

std::uint32_t inode_table_blocks_for(std::uint32_t inode_count, std::uint32_t block_size) {
    return (inode_count * static_cast<std::uint32_t>(sizeof(InodeDisk)) + block_size - 1) / block_size;
}

SuperBlockDisk read_super_block(const VirtualDisk& disk) {
    const auto block = disk.read_block(0);
    SuperBlockDisk sb{};
    std::memcpy(&sb, block.data(), sizeof(sb));
    if (!std::equal(magic.begin(), magic.end(), sb.magic)) {
        throw std::runtime_error{"invalid osfs superblock"};
    }
    if (sb.version != disk_version) {
        throw std::runtime_error{"unsupported osfs disk version; reformat required"};
    }
    if (sb.block_size != disk.block_size() || sb.block_count != disk.block_count()) {
        throw std::runtime_error{"osfs superblock geometry does not match disk image"};
    }
    return sb;
}

void write_super_block(VirtualDisk& disk, const SuperBlockDisk& sb) {
    std::vector<char> block(disk.block_size(), '\0');
    std::memcpy(block.data(), &sb, sizeof(sb));
    disk.write_block(0, block);
}

std::vector<char> read_block_bitmap(const VirtualDisk& disk, const SuperBlockDisk& sb) {
    return disk.read_block(sb.block_bitmap_block);
}

std::vector<char> read_inode_bitmap(const VirtualDisk& disk, const SuperBlockDisk& sb) {
    return disk.read_block(sb.inode_bitmap_block);
}

void write_block_bitmap(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<char>& bitmap) {
    disk.write_block(sb.block_bitmap_block, bitmap);
}

void write_inode_bitmap(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<char>& bitmap) {
    disk.write_block(sb.inode_bitmap_block, bitmap);
}

bool bitmap_used(const std::vector<char>& bitmap, std::uint32_t index) {
    return (static_cast<unsigned char>(bitmap[index / 8]) & (1U << (index % 8))) != 0;
}

void bitmap_set(std::vector<char>& bitmap, std::uint32_t index, bool used) {
    auto& byte = bitmap[index / 8];
    const auto mask = static_cast<char>(1U << (index % 8));
    byte = used ? static_cast<char>(byte | mask) : static_cast<char>(byte & ~mask);
}

std::vector<InodeDisk> read_inodes(const VirtualDisk& disk, const SuperBlockDisk& sb) {
    std::vector<InodeDisk> inodes(sb.inode_count);
    auto* output = reinterpret_cast<char*>(inodes.data());
    std::size_t copied = 0;
    for (std::uint32_t block = 0; block < sb.inode_table_blocks; ++block) {
        const auto bytes = disk.read_block(sb.inode_table_start + block);
        const auto remaining = sizeof(InodeDisk) * inodes.size() - copied;
        const auto count = std::min(remaining, bytes.size());
        std::memcpy(output + copied, bytes.data(), count);
        copied += count;
    }
    return inodes;
}

void write_inodes(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<InodeDisk>& inodes) {
    const auto* input = reinterpret_cast<const char*>(inodes.data());
    std::size_t copied = 0;
    for (std::uint32_t block = 0; block < sb.inode_table_blocks; ++block) {
        std::vector<char> bytes(disk.block_size(), '\0');
        const auto remaining = sizeof(InodeDisk) * inodes.size() - copied;
        const auto count = std::min(remaining, bytes.size());
        std::memcpy(bytes.data(), input + copied, count);
        disk.write_block(sb.inode_table_start + block, bytes);
        copied += count;
    }
}

std::vector<UserRecordDisk> read_users(const VirtualDisk& disk, const SuperBlockDisk& sb) {
    const auto capacity = static_cast<std::size_t>(sb.user_table_blocks) * disk.block_size() / sizeof(UserRecordDisk);
    std::vector<UserRecordDisk> users(capacity);
    auto* output = reinterpret_cast<char*>(users.data());
    std::size_t copied = 0;
    for (std::uint32_t block = 0; block < sb.user_table_blocks; ++block) {
        const auto bytes = disk.read_block(sb.user_table_start + block);
        std::memcpy(output + copied, bytes.data(), bytes.size());
        copied += bytes.size();
    }
    return users;
}

void write_users(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<UserRecordDisk>& users) {
    const auto* input = reinterpret_cast<const char*>(users.data());
    std::size_t copied = 0;
    for (std::uint32_t block = 0; block < sb.user_table_blocks; ++block) {
        std::vector<char> bytes(disk.block_size(), '\0');
        const auto remaining = sizeof(UserRecordDisk) * users.size() - copied;
        const auto count = std::min(remaining, bytes.size());
        std::memcpy(bytes.data(), input + copied, count);
        disk.write_block(sb.user_table_start + block, bytes);
        copied += count;
    }
}

std::vector<DirectoryEntryDisk> read_directory_entries(const VirtualDisk& disk, const InodeDisk& directory) {
    std::vector<DirectoryEntryDisk> entries;
    const auto entries_per_block = disk.block_size() / sizeof(DirectoryEntryDisk);
    for (const auto block_index : directory.direct) {
        if (block_index == 0) {
            continue;
        }
        const auto block = disk.read_block(block_index);
        for (std::size_t index = 0; index < entries_per_block; ++index) {
            DirectoryEntryDisk entry{};
            std::memcpy(&entry, block.data() + index * sizeof(DirectoryEntryDisk), sizeof(entry));
            entries.push_back(entry);
        }
    }
    return entries;
}

void write_directory_entries(VirtualDisk& disk, const InodeDisk& directory,
                             const std::vector<DirectoryEntryDisk>& entries) {
    const auto entries_per_block = disk.block_size() / sizeof(DirectoryEntryDisk);
    std::size_t offset = 0;
    for (const auto block_index : directory.direct) {
        if (block_index == 0) {
            continue;
        }
        std::vector<char> block(disk.block_size(), '\0');
        for (std::size_t index = 0; index < entries_per_block && offset < entries.size(); ++index, ++offset) {
            std::memcpy(block.data() + index * sizeof(DirectoryEntryDisk), &entries[offset],
                        sizeof(DirectoryEntryDisk));
        }
        disk.write_block(block_index, block);
    }
}

std::optional<std::size_t> find_entry_index(const std::vector<DirectoryEntryDisk>& entries, std::string_view name) {
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (entries[index].used != 0 && name == entries[index].name) {
            return index;
        }
    }
    return std::nullopt;
}

std::optional<std::uint32_t> allocate_block(const SuperBlockDisk& sb, std::vector<char>& bitmap) {
    for (std::uint32_t block = sb.data_start_block; block < sb.block_count; ++block) {
        if (!bitmap_used(bitmap, block)) {
            bitmap_set(bitmap, block, true);
            return block;
        }
    }
    return std::nullopt;
}

std::optional<std::uint32_t> allocate_inode(const SuperBlockDisk& sb, std::vector<char>& bitmap) {
    for (std::uint32_t inode = 0; inode < sb.inode_count; ++inode) {
        if (!bitmap_used(bitmap, inode)) {
            bitmap_set(bitmap, inode, true);
            return inode;
        }
    }
    return std::nullopt;
}

void release_block(std::vector<char>& bitmap, std::uint32_t block) { bitmap_set(bitmap, block, false); }

void release_inode(std::vector<char>& bitmap, std::uint32_t inode) { bitmap_set(bitmap, inode, false); }

std::uint32_t count_free(const std::vector<char>& bitmap, std::uint32_t begin, std::uint32_t end) {
    std::uint32_t free_count = 0;
    for (auto index = begin; index < end; ++index) {
        if (!bitmap_used(bitmap, index)) {
            ++free_count;
        }
    }
    return free_count;
}

void clear_block(VirtualDisk& disk, std::uint32_t block_index) {
    disk.write_block(block_index, std::vector<char>(disk.block_size(), '\0'));
}

std::uint32_t indirect_entries_per_block(std::uint32_t block_size) {
    return block_size / static_cast<std::uint32_t>(sizeof(std::uint32_t));
}

std::uint32_t max_file_blocks(std::uint32_t block_size) {
    return max_direct_blocks + indirect_entries_per_block(block_size);
}

std::vector<std::uint32_t> read_indirect_block(const VirtualDisk& disk, std::uint32_t block_index) {
    std::vector<std::uint32_t> entries(indirect_entries_per_block(disk.block_size()));
    if (block_index == 0) {
        return entries;
    }
    const auto block = disk.read_block(block_index);
    std::memcpy(entries.data(), block.data(), entries.size() * sizeof(std::uint32_t));
    return entries;
}

void write_indirect_block(VirtualDisk& disk, std::uint32_t block_index, const std::vector<std::uint32_t>& entries) {
    std::vector<char> block(disk.block_size(), '\0');
    const auto count = std::min(block.size(), entries.size() * sizeof(std::uint32_t));
    std::memcpy(block.data(), entries.data(), count);
    disk.write_block(block_index, block);
}

std::uint32_t inode_data_block_count(const VirtualDisk& disk, const InodeDisk& inode) {
    std::uint32_t count = 0;
    for (const auto block : inode.direct) {
        if (block != 0) {
            ++count;
        }
    }
    if (inode.indirect_block != 0) {
        const auto indirect = read_indirect_block(disk, inode.indirect_block);
        count += static_cast<std::uint32_t>(
            std::count_if(indirect.begin(), indirect.end(), [](const auto block) { return block != 0; }));
    }
    return count;
}

std::vector<std::uint32_t> inode_data_blocks(const VirtualDisk& disk, const InodeDisk& inode) {
    std::vector<std::uint32_t> blocks;
    for (const auto block : inode.direct) {
        if (block != 0) {
            blocks.push_back(block);
        }
    }
    if (inode.indirect_block != 0) {
        const auto indirect = read_indirect_block(disk, inode.indirect_block);
        for (const auto block : indirect) {
            if (block != 0) {
                blocks.push_back(block);
            }
        }
    }
    return blocks;
}

std::optional<std::uint32_t> inode_data_block_at(const VirtualDisk& disk, const InodeDisk& inode,
                                                 std::uint32_t logical_index) {
    if (logical_index < max_direct_blocks) {
        return inode.direct[logical_index] == 0 ? std::nullopt
                                                : std::optional<std::uint32_t>{inode.direct[logical_index]};
    }
    if (inode.indirect_block == 0) {
        return std::nullopt;
    }
    const auto indirect = read_indirect_block(disk, inode.indirect_block);
    const auto indirect_index = logical_index - max_direct_blocks;
    if (indirect_index >= indirect.size() || indirect[indirect_index] == 0) {
        return std::nullopt;
    }
    return indirect[indirect_index];
}

void release_inode_storage(VirtualDisk& disk, InodeDisk& inode, std::vector<char>& block_bitmap) {
    for (auto& direct : inode.direct) {
        if (direct != 0) {
            release_block(block_bitmap, direct);
            clear_block(disk, direct);
            direct = 0;
        }
    }
    if (inode.indirect_block != 0) {
        const auto indirect = read_indirect_block(disk, inode.indirect_block);
        for (const auto block : indirect) {
            if (block != 0) {
                release_block(block_bitmap, block);
                clear_block(disk, block);
            }
        }
        release_block(block_bitmap, inode.indirect_block);
        clear_block(disk, inode.indirect_block);
        inode.indirect_block = 0;
    }
}

bool ensure_inode_data_blocks(VirtualDisk& disk, const SuperBlockDisk& sb, InodeDisk& inode,
                              std::vector<char>& block_bitmap, std::uint32_t needed_blocks, std::string* error) {
    if (needed_blocks > max_file_blocks(disk.block_size())) {
        set_error(error, "file exceeds indirect-block limit");
        return false;
    }
    const auto current_blocks = inode_data_block_count(disk, inode);
    if (needed_blocks <= current_blocks) {
        return true;
    }

    const auto needs_indirect = needed_blocks > max_direct_blocks && inode.indirect_block == 0;
    const auto additional_blocks = needed_blocks - current_blocks + (needs_indirect ? 1U : 0U);
    if (count_free(block_bitmap, sb.data_start_block, sb.block_count) < additional_blocks) {
        set_error(error, "no free data block");
        return false;
    }

    std::vector<std::uint32_t> indirect;
    if (needs_indirect) {
        const auto block = allocate_block(sb, block_bitmap);
        if (!block.has_value()) {
            set_error(error, "no free data block");
            return false;
        }
        inode.indirect_block = *block;
        indirect.assign(indirect_entries_per_block(disk.block_size()), 0);
        write_indirect_block(disk, inode.indirect_block, indirect);
    } else if (inode.indirect_block != 0) {
        indirect = read_indirect_block(disk, inode.indirect_block);
    }

    bool indirect_changed = false;
    for (std::uint32_t index = current_blocks; index < needed_blocks; ++index) {
        const auto block = allocate_block(sb, block_bitmap);
        if (!block.has_value()) {
            set_error(error, "no free data block");
            return false;
        }
        clear_block(disk, *block);
        if (index < max_direct_blocks) {
            inode.direct[index] = *block;
        } else {
            if (indirect.empty()) {
                indirect = read_indirect_block(disk, inode.indirect_block);
            }
            indirect[index - max_direct_blocks] = *block;
            indirect_changed = true;
        }
    }
    if (indirect_changed) {
        write_indirect_block(disk, inode.indirect_block, indirect);
    }
    return true;
}

const UserRecordDisk* find_user_by_name(const std::vector<UserRecordDisk>& users, std::string_view username) {
    const auto found = std::find_if(users.begin(), users.end(), [username](const auto& user) {
        return user.used != 0 && username == user.username;
    });
    return found == users.end() ? nullptr : &*found;
}

const UserRecordDisk* find_user_by_uid(const std::vector<UserRecordDisk>& users, std::uint32_t uid) {
    const auto found =
        std::find_if(users.begin(), users.end(), [uid](const auto& user) { return user.used != 0 && uid == user.uid; });
    return found == users.end() ? nullptr : &*found;
}

std::optional<FileReference> resolve_file_reference(const std::vector<UserRecordDisk>& users,
                                                    std::string_view reference, std::uint32_t actor_uid,
                                                    std::string* error) {
    const auto separator = reference.find('/');
    const UserRecordDisk* owner = nullptr;
    std::string_view filename = reference;
    if (separator == std::string_view::npos) {
        owner = find_user_by_uid(users, actor_uid);
    } else {
        owner = find_user_by_name(users, reference.substr(0, separator));
        filename = reference.substr(separator + 1);
    }
    if (owner == nullptr) {
        set_error(error, "user not found");
        return std::nullopt;
    }
    if (!valid_name(filename)) {
        set_error(error, "invalid file name");
        return std::nullopt;
    }
    return FileReference{owner, std::string{filename}};
}

std::uint64_t password_hash(std::string_view username, std::string_view password) {
    std::uint64_t value = 1469598103934665603ULL;
    const auto absorb = [&value](std::string_view input) {
        for (const auto ch : input) {
            value ^= static_cast<unsigned char>(ch);
            value *= 1099511628211ULL;
        }
    };
    absorb("minikv-osfs-v2:");
    absorb(username);
    absorb(":");
    absorb(password);
    return value;
}

bool has_permission(const InodeDisk& inode, std::uint32_t uid, std::uint32_t gid, std::uint16_t owner_bit,
                    std::uint16_t group_bit, std::uint16_t other_bit) {
    if (uid == 0) {
        return true;
    }
    if (uid == inode.owner_uid) {
        return (inode.mode & owner_bit) != 0;
    }
    if (gid == inode.owner_gid) {
        return (inode.mode & group_bit) != 0;
    }
    return (inode.mode & other_bit) != 0;
}

} // namespace minikv::osfs::detail
