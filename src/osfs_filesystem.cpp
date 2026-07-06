#include "minikv/osfs/filesystem.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <sstream>
#include <stdexcept>

namespace minikv::osfs {

namespace {

constexpr std::array<char, 8> magic = {'M', 'K', 'V', 'O', 'S', 'F', 'S', '\0'};
constexpr std::uint32_t disk_version = 1;
constexpr std::uint32_t bitmap_block = 1;
constexpr std::uint32_t inode_table_start = 2;
constexpr std::uint32_t root_inode_index = 0;
constexpr std::uint32_t max_direct_blocks = 8;
constexpr std::uint16_t default_file_mode = 0644;
constexpr std::uint16_t default_root_mode = 0755;

#pragma pack(push, 1)
struct SuperBlockDisk {
    char magic[8];
    std::uint32_t version;
    std::uint32_t block_size;
    std::uint32_t block_count;
    std::uint32_t inode_count;
    std::uint32_t bitmap_block;
    std::uint32_t inode_table_start;
    std::uint32_t inode_table_blocks;
    std::uint32_t data_start_block;
    std::uint32_t root_inode;
    char reserved[468];
};

struct InodeDisk {
    std::uint8_t used;
    std::uint8_t type;
    std::uint16_t mode;
    std::uint32_t owner_uid;
    std::uint64_t size;
    std::uint64_t created_at;
    std::uint64_t modified_at;
    std::uint32_t direct[max_direct_blocks];
    char reserved[32];
};

struct DirectoryEntryDisk {
    std::uint8_t used;
    std::uint32_t inode;
    char name[55];
    char reserved[4];
};
#pragma pack(pop)

static_assert(sizeof(InodeDisk) == 96);
static_assert(sizeof(DirectoryEntryDisk) == 64);

std::uint64_t now_seconds() {
    return static_cast<std::uint64_t>(
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

void set_error(std::string* error, std::string message) {
    if (error != nullptr) {
        *error = std::move(message);
    }
}

bool valid_name(const std::string& name) {
    return !name.empty() && name.size() < 55 && name != "." && name != ".." && name.find('/') == std::string::npos &&
           name.find('\\') == std::string::npos;
}

std::uint32_t inode_table_blocks_for(std::uint32_t inode_count, std::uint32_t block_size) {
    return (inode_count * static_cast<std::uint32_t>(sizeof(InodeDisk)) + block_size - 1) / block_size;
}

SuperBlockDisk read_super_block(const VirtualDisk& disk) {
    auto block = disk.read_block(0);
    SuperBlockDisk sb{};
    std::memcpy(&sb, block.data(), sizeof(sb));
    if (!std::equal(magic.begin(), magic.end(), sb.magic) || sb.version != disk_version) {
        throw std::runtime_error{"invalid osfs superblock"};
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

std::vector<char> read_bitmap(const VirtualDisk& disk) { return disk.read_block(bitmap_block); }

void write_bitmap(VirtualDisk& disk, const std::vector<char>& bitmap) { disk.write_block(bitmap_block, bitmap); }

bool bitmap_used(const std::vector<char>& bitmap, std::uint32_t block) {
    return (static_cast<unsigned char>(bitmap[block / 8]) & (1U << (block % 8))) != 0;
}

void bitmap_set(std::vector<char>& bitmap, std::uint32_t block, bool used) {
    auto& byte = bitmap[block / 8];
    const auto mask = static_cast<char>(1U << (block % 8));
    if (used) {
        byte = static_cast<char>(byte | mask);
    } else {
        byte = static_cast<char>(byte & ~mask);
    }
}

std::vector<InodeDisk> read_inodes(const VirtualDisk& disk, const SuperBlockDisk& sb) {
    std::vector<InodeDisk> inodes(sb.inode_count);
    auto* output = reinterpret_cast<char*>(inodes.data());
    std::uint32_t copied = 0;
    for (std::uint32_t block = 0; block < sb.inode_table_blocks; ++block) {
        auto bytes = disk.read_block(sb.inode_table_start + block);
        const auto remaining = sizeof(InodeDisk) * inodes.size() - copied;
        const auto count = std::min<std::size_t>(remaining, bytes.size());
        std::memcpy(output + copied, bytes.data(), count);
        copied += static_cast<std::uint32_t>(count);
    }
    return inodes;
}

void write_inodes(VirtualDisk& disk, const SuperBlockDisk& sb, const std::vector<InodeDisk>& inodes) {
    const auto* input = reinterpret_cast<const char*>(inodes.data());
    std::uint32_t copied = 0;
    for (std::uint32_t block = 0; block < sb.inode_table_blocks; ++block) {
        std::vector<char> bytes(disk.block_size(), '\0');
        const auto remaining = sizeof(InodeDisk) * inodes.size() - copied;
        const auto count = std::min<std::size_t>(remaining, bytes.size());
        std::memcpy(bytes.data(), input + copied, count);
        disk.write_block(sb.inode_table_start + block, bytes);
        copied += static_cast<std::uint32_t>(count);
    }
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

std::uint32_t count_free_data_blocks(const SuperBlockDisk& sb, const std::vector<char>& bitmap) {
    std::uint32_t free_blocks = 0;
    for (std::uint32_t block = sb.data_start_block; block < sb.block_count; ++block) {
        if (!bitmap_used(bitmap, block)) {
            ++free_blocks;
        }
    }
    return free_blocks;
}

void clear_block(VirtualDisk& disk, std::uint32_t block_index) {
    disk.write_block(block_index, std::vector<char>(disk.block_size(), '\0'));
}

std::vector<DirectoryEntryDisk> read_directory_entries(const VirtualDisk& disk, const InodeDisk& root) {
    std::vector<DirectoryEntryDisk> entries;
    const auto entries_per_block = disk.block_size() / sizeof(DirectoryEntryDisk);
    for (const auto block_index : root.direct) {
        if (block_index == 0) {
            continue;
        }
        auto block = disk.read_block(block_index);
        for (std::size_t index = 0; index < entries_per_block; ++index) {
            DirectoryEntryDisk entry{};
            std::memcpy(&entry, block.data() + index * sizeof(DirectoryEntryDisk), sizeof(entry));
            entries.push_back(entry);
        }
    }
    return entries;
}

void write_directory_entries(VirtualDisk& disk, const InodeDisk& root, const std::vector<DirectoryEntryDisk>& entries) {
    const auto entries_per_block = disk.block_size() / sizeof(DirectoryEntryDisk);
    std::size_t offset = 0;
    for (const auto block_index : root.direct) {
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

std::optional<std::size_t> find_entry_index(const std::vector<DirectoryEntryDisk>& entries, const std::string& name) {
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (entries[index].used != 0 && name == entries[index].name) {
            return index;
        }
    }
    return std::nullopt;
}

bool has_permission(const InodeDisk& inode, std::uint32_t uid, std::uint16_t owner_bit, std::uint16_t other_bit) {
    if (uid == 0) {
        return true;
    }
    if (uid == inode.owner_uid) {
        return (inode.mode & owner_bit) != 0;
    }
    return (inode.mode & other_bit) != 0;
}

std::uint16_t normalize_mode(std::uint16_t mode) { return static_cast<std::uint16_t>(mode & 0777); }

} // namespace

FileSystem FileSystem::format(const std::filesystem::path& path, const FormatOptions& options) {
    const auto inode_table_blocks = inode_table_blocks_for(options.inode_count, options.block_size);
    const auto data_start_block = inode_table_start + inode_table_blocks;
    if (data_start_block + 1 >= options.block_count) {
        throw std::invalid_argument{"osfs geometry leaves no room for data blocks"};
    }

    auto disk = VirtualDisk::create(path, options.block_count, options.block_size);

    SuperBlockDisk sb{};
    std::copy(magic.begin(), magic.end(), sb.magic);
    sb.version = disk_version;
    sb.block_size = options.block_size;
    sb.block_count = options.block_count;
    sb.inode_count = options.inode_count;
    sb.bitmap_block = bitmap_block;
    sb.inode_table_start = inode_table_start;
    sb.inode_table_blocks = inode_table_blocks;
    sb.data_start_block = data_start_block;
    sb.root_inode = root_inode_index;
    write_super_block(disk, sb);

    std::vector<char> bitmap(options.block_size, '\0');
    for (std::uint32_t block = 0; block < data_start_block; ++block) {
        bitmap_set(bitmap, block, true);
    }
    bitmap_set(bitmap, data_start_block, true);
    write_bitmap(disk, bitmap);

    std::vector<InodeDisk> inodes(options.inode_count);
    auto& root = inodes[root_inode_index];
    root.used = 1;
    root.type = 2;
    root.mode = default_root_mode;
    root.owner_uid = 0;
    root.created_at = now_seconds();
    root.modified_at = root.created_at;
    root.direct[0] = data_start_block;
    write_inodes(disk, sb, inodes);
    clear_block(disk, data_start_block);

    return FileSystem{std::move(disk)};
}

FileSystem FileSystem::open(const std::filesystem::path& path) {
    auto raw = VirtualDisk::open(path, 512);
    const auto first_block = raw.read_block(0);
    SuperBlockDisk sb{};
    std::memcpy(&sb, first_block.data(), sizeof(sb));
    if (!std::equal(magic.begin(), magic.end(), sb.magic)) {
        throw std::runtime_error{"invalid osfs superblock"};
    }
    return FileSystem{VirtualDisk::open(path, sb.block_size)};
}

FileSystem::FileSystem(VirtualDisk disk) : disk_{std::move(disk)} { (void)read_super_block(disk_); }

const std::filesystem::path& FileSystem::path() const { return disk_.path(); }

SuperBlock FileSystem::super_block() const {
    const auto sb = read_super_block(disk_);
    return SuperBlock{sb.version, sb.block_size, sb.block_count, sb.inode_count, sb.data_start_block, sb.root_inode};
}

bool FileSystem::create_file(const std::string& name, std::uint32_t owner_uid, std::string* error) {
    if (!valid_name(name)) {
        set_error(error, "invalid file name");
        return false;
    }

    const auto sb = read_super_block(disk_);
    auto inodes = read_inodes(disk_, sb);
    auto bitmap = read_bitmap(disk_);
    auto& root = inodes[sb.root_inode];
    auto entries = read_directory_entries(disk_, root);

    if (find_entry_index(entries, name).has_value()) {
        set_error(error, "file already exists");
        return false;
    }

    auto inode_index = std::optional<std::uint32_t>{};
    for (std::uint32_t index = 1; index < inodes.size(); ++index) {
        if (inodes[index].used == 0) {
            inode_index = index;
            break;
        }
    }
    if (!inode_index.has_value()) {
        set_error(error, "inode table is full");
        return false;
    }

    auto slot = std::optional<std::size_t>{};
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (entries[index].used == 0) {
            slot = index;
            break;
        }
    }

    if (!slot.has_value()) {
        for (auto& direct : root.direct) {
            if (direct == 0) {
                const auto block = allocate_block(sb, bitmap);
                if (!block.has_value()) {
                    set_error(error, "no free block for directory growth");
                    return false;
                }
                direct = *block;
                clear_block(disk_, *block);
                const auto entries_per_block = disk_.block_size() / sizeof(DirectoryEntryDisk);
                entries.resize(entries.size() + entries_per_block);
                slot = entries.size() - entries_per_block;
                break;
            }
        }
    }

    if (!slot.has_value()) {
        set_error(error, "root directory is full");
        return false;
    }

    auto& inode = inodes[*inode_index];
    inode = {};
    inode.used = 1;
    inode.type = 1;
    inode.mode = default_file_mode;
    inode.owner_uid = owner_uid;
    inode.created_at = now_seconds();
    inode.modified_at = inode.created_at;

    DirectoryEntryDisk entry{};
    entry.used = 1;
    entry.inode = *inode_index;
    std::strncpy(entry.name, name.c_str(), sizeof(entry.name) - 1);
    entries[*slot] = entry;

    root.modified_at = now_seconds();
    write_directory_entries(disk_, root, entries);
    write_inodes(disk_, sb, inodes);
    write_bitmap(disk_, bitmap);
    return true;
}

bool FileSystem::delete_file(const std::string& name, std::uint32_t uid, std::string* error) {
    const auto sb = read_super_block(disk_);
    auto inodes = read_inodes(disk_, sb);
    auto bitmap = read_bitmap(disk_);
    auto& root = inodes[sb.root_inode];
    auto entries = read_directory_entries(disk_, root);
    const auto entry_index = find_entry_index(entries, name);
    if (!entry_index.has_value()) {
        set_error(error, "file not found");
        return false;
    }

    auto& inode = inodes[entries[*entry_index].inode];
    if (uid != 0 && uid != inode.owner_uid) {
        set_error(error, "delete requires owner or root");
        return false;
    }

    for (auto& direct : inode.direct) {
        if (direct != 0) {
            bitmap_set(bitmap, direct, false);
            clear_block(disk_, direct);
            direct = 0;
        }
    }
    inode = {};
    entries[*entry_index] = {};
    root.modified_at = now_seconds();

    write_directory_entries(disk_, root, entries);
    write_inodes(disk_, sb, inodes);
    write_bitmap(disk_, bitmap);
    return true;
}

bool FileSystem::write_file(const std::string& name, const std::string& contents, std::uint32_t uid,
                            std::string* error) {
    const auto sb = read_super_block(disk_);
    auto inodes = read_inodes(disk_, sb);
    auto bitmap = read_bitmap(disk_);
    auto& root = inodes[sb.root_inode];
    const auto entries = read_directory_entries(disk_, root);
    const auto entry_index = find_entry_index(entries, name);
    if (!entry_index.has_value()) {
        set_error(error, "file not found");
        return false;
    }

    auto& inode = inodes[entries[*entry_index].inode];
    if (!has_permission(inode, uid, owner_write, other_write)) {
        set_error(error, "permission denied");
        return false;
    }

    const auto needed_blocks =
        contents.empty() ? 0
                         : static_cast<std::uint32_t>((contents.size() + disk_.block_size() - 1) / disk_.block_size());
    if (needed_blocks > max_direct_blocks) {
        set_error(error, "file exceeds direct-block limit");
        return false;
    }

    std::uint32_t reclaimable_blocks = 0;
    for (const auto direct : inode.direct) {
        if (direct != 0) {
            ++reclaimable_blocks;
        }
    }
    if (count_free_data_blocks(sb, bitmap) + reclaimable_blocks < needed_blocks) {
        set_error(error, "no free data block");
        return false;
    }

    for (auto& direct : inode.direct) {
        if (direct != 0) {
            bitmap_set(bitmap, direct, false);
            clear_block(disk_, direct);
            direct = 0;
        }
    }

    std::size_t offset = 0;
    for (std::uint32_t index = 0; index < needed_blocks; ++index) {
        const auto block = allocate_block(sb, bitmap);
        if (!block.has_value()) {
            set_error(error, "no free data block");
            return false;
        }
        inode.direct[index] = *block;
        std::vector<char> bytes(disk_.block_size(), '\0');
        const auto count = std::min<std::size_t>(disk_.block_size(), contents.size() - offset);
        std::memcpy(bytes.data(), contents.data() + offset, count);
        disk_.write_block(*block, bytes);
        offset += count;
    }

    inode.size = contents.size();
    inode.modified_at = now_seconds();
    write_inodes(disk_, sb, inodes);
    write_bitmap(disk_, bitmap);
    return true;
}

std::optional<std::string> FileSystem::read_file(const std::string& name, std::uint32_t uid, std::string* error) const {
    const auto sb = read_super_block(disk_);
    const auto inodes = read_inodes(disk_, sb);
    const auto root = inodes[sb.root_inode];
    const auto entries = read_directory_entries(disk_, root);
    const auto entry_index = find_entry_index(entries, name);
    if (!entry_index.has_value()) {
        set_error(error, "file not found");
        return std::nullopt;
    }

    const auto& inode = inodes[entries[*entry_index].inode];
    if (!has_permission(inode, uid, owner_read, other_read)) {
        set_error(error, "permission denied");
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
    return contents;
}

bool FileSystem::chmod(const std::string& name, std::uint16_t mode, std::uint32_t uid, std::string* error) {
    const auto sb = read_super_block(disk_);
    auto inodes = read_inodes(disk_, sb);
    const auto root = inodes[sb.root_inode];
    const auto entries = read_directory_entries(disk_, root);
    const auto entry_index = find_entry_index(entries, name);
    if (!entry_index.has_value()) {
        set_error(error, "file not found");
        return false;
    }
    auto& inode = inodes[entries[*entry_index].inode];
    if (uid != 0 && uid != inode.owner_uid) {
        set_error(error, "chmod requires owner or root");
        return false;
    }
    inode.mode = normalize_mode(mode);
    inode.modified_at = now_seconds();
    write_inodes(disk_, sb, inodes);
    return true;
}

std::vector<DirectoryEntry> FileSystem::list_directory() const {
    const auto sb = read_super_block(disk_);
    const auto inodes = read_inodes(disk_, sb);
    const auto root = inodes[sb.root_inode];
    const auto entries = read_directory_entries(disk_, root);
    std::vector<DirectoryEntry> result;
    for (const auto& entry : entries) {
        if (entry.used == 0) {
            continue;
        }
        const auto& inode = inodes[entry.inode];
        result.push_back(
            DirectoryEntry{entry.name, entry.inode, inode.direct[0], inode.mode, inode.owner_uid, inode.size});
    }
    std::sort(result.begin(), result.end(),
              [](const DirectoryEntry& left, const DirectoryEntry& right) { return left.name < right.name; });
    return result;
}

std::optional<FileStat> FileSystem::stat(const std::string& name) const {
    const auto sb = read_super_block(disk_);
    const auto inodes = read_inodes(disk_, sb);
    const auto root = inodes[sb.root_inode];
    const auto entries = read_directory_entries(disk_, root);
    const auto entry_index = find_entry_index(entries, name);
    if (!entry_index.has_value()) {
        return std::nullopt;
    }
    const auto& entry = entries[*entry_index];
    const auto& inode = inodes[entry.inode];
    return FileStat{entry.name,      entry.inode, inode.direct[0],  inode.mode,
                    inode.owner_uid, inode.size,  inode.created_at, inode.modified_at};
}

bool FileSystem::can_read(const std::string& name, std::uint32_t uid) const {
    const auto stat_result = stat(name);
    if (!stat_result.has_value()) {
        return false;
    }
    const auto sb = read_super_block(disk_);
    const auto inodes = read_inodes(disk_, sb);
    return has_permission(inodes[stat_result->inode], uid, owner_read, other_read);
}

bool FileSystem::can_write(const std::string& name, std::uint32_t uid) const {
    const auto stat_result = stat(name);
    if (!stat_result.has_value()) {
        return false;
    }
    const auto sb = read_super_block(disk_);
    const auto inodes = read_inodes(disk_, sb);
    return has_permission(inodes[stat_result->inode], uid, owner_write, other_write);
}

std::uint32_t uid_for_user(const std::string& user) {
    if (user == "root") {
        return 0;
    }
    std::uint32_t hash = 2166136261U;
    for (const auto ch : user) {
        hash ^= static_cast<unsigned char>(ch);
        hash *= 16777619U;
    }
    return hash == 0 ? 1 : hash;
}

std::string format_mode(std::uint16_t mode) {
    std::ostringstream out;
    out << '0' << ((mode >> 6) & 7) << ((mode >> 3) & 7) << (mode & 7);
    return out.str();
}

} // namespace minikv::osfs
