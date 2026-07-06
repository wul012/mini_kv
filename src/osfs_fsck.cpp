#include "minikv/osfs/filesystem.hpp"

#include "osfs_disk_layout.hpp"

#include <algorithm>
#include <exception>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace minikv::osfs {

namespace {

void add_check(FsckReport& report, std::string name, std::vector<std::string> issues, std::string ok_detail) {
    FsckCheck check;
    check.name = std::move(name);
    check.ok = issues.empty();
    if (check.ok) {
        check.detail = std::move(ok_detail);
    } else {
        std::ostringstream out;
        for (std::size_t index = 0; index < issues.size(); ++index) {
            if (index != 0) {
                out << "; ";
            }
            out << issues[index];
        }
        check.detail = out.str();
    }
    report.ok = report.ok && check.ok;
    report.checks.push_back(std::move(check));
}

std::uint32_t required_blocks_for_size(std::uint64_t size, std::uint32_t block_size) {
    if (size == 0) {
        return 0;
    }
    return static_cast<std::uint32_t>((size + block_size - 1) / block_size);
}

std::string block_name(std::uint32_t block) { return "block " + std::to_string(block); }

bool valid_block(const detail::SuperBlockDisk& sb, std::uint32_t block) {
    return block >= sb.data_start_block && block < sb.block_count;
}

void mark_expected_block(const detail::SuperBlockDisk& sb, std::vector<char>& expected,
                         std::vector<std::string>& owners, std::uint32_t block, const std::string& owner,
                         std::vector<std::string>& issues) {
    if (!valid_block(sb, block)) {
        issues.push_back(owner + " references out-of-range " + block_name(block));
        return;
    }
    if (detail::bitmap_used(expected, block)) {
        issues.push_back(owner + " duplicates " + block_name(block) + " already owned by " + owners[block]);
        return;
    }
    detail::bitmap_set(expected, block, true);
    owners[block] = owner;
}

std::vector<std::uint32_t> collect_file_blocks(const VirtualDisk& disk, const detail::SuperBlockDisk& sb,
                                               const detail::InodeDisk& inode, std::uint32_t inode_index,
                                               std::vector<char>& expected_blocks,
                                               std::vector<std::string>& block_owners,
                                               std::vector<std::string>& block_issues,
                                               std::vector<std::string>& size_issues) {
    std::vector<std::uint32_t> data_blocks;
    for (std::uint32_t index = 0; index < detail::max_direct_blocks; ++index) {
        const auto block = inode.direct[index];
        if (block == 0) {
            continue;
        }
        data_blocks.push_back(block);
        mark_expected_block(sb, expected_blocks, block_owners, block,
                            "inode " + std::to_string(inode_index) + " direct[" + std::to_string(index) + "]",
                            block_issues);
    }

    if (inode.indirect_block != 0) {
        mark_expected_block(sb, expected_blocks, block_owners, inode.indirect_block,
                            "inode " + std::to_string(inode_index) + " indirect block", block_issues);
        if (valid_block(sb, inode.indirect_block)) {
            const auto indirect = detail::read_indirect_block(disk, inode.indirect_block);
            for (std::uint32_t index = 0; index < indirect.size(); ++index) {
                const auto block = indirect[index];
                if (block == 0) {
                    continue;
                }
                data_blocks.push_back(block);
                mark_expected_block(sb, expected_blocks, block_owners, block,
                                    "inode " + std::to_string(inode_index) + " indirect[" + std::to_string(index) + "]",
                                    block_issues);
            }
        }
    }

    const auto required = required_blocks_for_size(inode.size, disk.block_size());
    if (data_blocks.size() != required) {
        size_issues.push_back("inode " + std::to_string(inode_index) + " size requires " + std::to_string(required) +
                              " blocks but references " + std::to_string(data_blocks.size()));
    }
    if (required <= detail::max_direct_blocks && inode.indirect_block != 0) {
        size_issues.push_back("inode " + std::to_string(inode_index) +
                              " keeps an indirect block below the direct-block capacity");
    }
    return data_blocks;
}

bool directory_inode_valid(const std::vector<detail::InodeDisk>& inodes, std::uint32_t inode_index) {
    return inode_index < inodes.size() && inodes[inode_index].used != 0 && inodes[inode_index].type == 2;
}

} // namespace

FsckReport FileSystem::check_consistency() const {
    FsckReport report;
    report.ok = true;

    detail::SuperBlockDisk sb{};
    try {
        sb = detail::read_super_block(disk_);
    } catch (const std::exception& error) {
        add_check(report, "superblock", {error.what()}, "magic/version/geometry ok");
        return report;
    }

    std::vector<std::string> superblock_issues;
    if (sb.block_bitmap_block != detail::block_bitmap_block || sb.inode_bitmap_block != detail::inode_bitmap_block ||
        sb.user_table_start != detail::user_table_start || sb.user_table_blocks != detail::user_table_blocks) {
        superblock_issues.push_back("metadata block locations drift from the OSFS layout constants");
    }
    if (sb.inode_table_start + sb.inode_table_blocks > sb.data_start_block || sb.data_start_block >= sb.block_count) {
        superblock_issues.push_back("metadata ranges overlap the data region");
    }
    if (sb.mfd_inode >= sb.inode_count) {
        superblock_issues.push_back("MFD inode is out of range");
    }
    add_check(report, "superblock", std::move(superblock_issues), "magic/version/geometry ok");
    if (!report.checks.back().ok) {
        return report;
    }

    auto block_bitmap = detail::read_block_bitmap(disk_, sb);
    auto inode_bitmap = detail::read_inode_bitmap(disk_, sb);
    const auto inodes = detail::read_inodes(disk_, sb);
    const auto users = detail::read_users(disk_, sb);

    std::vector<char> expected_block_bitmap(disk_.block_size(), '\0');
    std::vector<char> expected_inode_bitmap(disk_.block_size(), '\0');
    std::vector<std::string> block_owners(sb.block_count);
    for (std::uint32_t block = 0; block < sb.data_start_block; ++block) {
        detail::bitmap_set(expected_block_bitmap, block, true);
        block_owners[block] = "metadata";
    }

    std::vector<std::string> inode_bitmap_issues;
    std::vector<std::string> block_issues;
    std::vector<std::string> size_issues;
    std::vector<std::string> directory_issues;

    for (std::uint32_t index = 0; index < inodes.size(); ++index) {
        const auto& inode = inodes[index];
        if (inode.used == 0) {
            if (inode.indirect_block != 0 || std::any_of(std::begin(inode.direct), std::end(inode.direct),
                                                         [](const auto block) { return block != 0; })) {
                inode_bitmap_issues.push_back("unused inode " + std::to_string(index) + " still references storage");
            }
            continue;
        }
        detail::bitmap_set(expected_inode_bitmap, index, true);
        if (inode.type != 1 && inode.type != 2) {
            inode_bitmap_issues.push_back("inode " + std::to_string(index) + " has invalid type " +
                                          std::to_string(inode.type));
        }
        if (inode.type == 2) {
            if (inode.indirect_block != 0) {
                directory_issues.push_back("directory inode " + std::to_string(index) +
                                           " unexpectedly uses an indirect block");
            }
            for (std::uint32_t direct_index = 0; direct_index < detail::max_direct_blocks; ++direct_index) {
                const auto block = inode.direct[direct_index];
                if (block != 0) {
                    mark_expected_block(sb, expected_block_bitmap, block_owners, block,
                                        "directory inode " + std::to_string(index) + " direct[" +
                                            std::to_string(direct_index) + "]",
                                        block_issues);
                }
            }
        } else if (inode.type == 1) {
            (void)collect_file_blocks(disk_, sb, inode, index, expected_block_bitmap, block_owners, block_issues,
                                      size_issues);
        }
    }

    for (std::uint32_t index = 0; index < sb.inode_count; ++index) {
        if (detail::bitmap_used(inode_bitmap, index) != detail::bitmap_used(expected_inode_bitmap, index)) {
            inode_bitmap_issues.push_back("inode bitmap mismatch at inode " + std::to_string(index));
        }
    }
    const auto expected_free_inodes = detail::count_free(expected_inode_bitmap, 0, sb.inode_count);
    if (sb.free_inode_count != expected_free_inodes) {
        inode_bitmap_issues.push_back("superblock free_inode_count=" + std::to_string(sb.free_inode_count) +
                                      " expected " + std::to_string(expected_free_inodes));
    }

    for (std::uint32_t block = 0; block < sb.block_count; ++block) {
        if (detail::bitmap_used(block_bitmap, block) != detail::bitmap_used(expected_block_bitmap, block)) {
            block_issues.push_back("block bitmap mismatch at " + block_name(block));
        }
    }
    const auto expected_free_blocks = detail::count_free(expected_block_bitmap, sb.data_start_block, sb.block_count);
    if (sb.free_block_count != expected_free_blocks) {
        block_issues.push_back("superblock free_block_count=" + std::to_string(sb.free_block_count) + " expected " +
                               std::to_string(expected_free_blocks));
    }

    std::vector<std::string> user_issues;
    std::map<std::string, std::uint32_t> mfd_entries;
    if (!directory_inode_valid(inodes, sb.mfd_inode)) {
        directory_issues.push_back("MFD inode is not a used directory");
    } else {
        const auto entries = detail::read_directory_entries(disk_, inodes[sb.mfd_inode]);
        for (const auto& entry : entries) {
            if (entry.used == 0) {
                continue;
            }
            if (!detail::valid_name(entry.name)) {
                directory_issues.push_back("MFD entry has invalid name");
                continue;
            }
            if (!directory_inode_valid(inodes, entry.inode)) {
                directory_issues.push_back(std::string{"MFD entry "} + entry.name + " points at invalid UFD inode");
                continue;
            }
            mfd_entries[entry.name] = entry.inode;
        }
    }

    std::set<std::string> usernames;
    std::set<std::uint32_t> uids;
    std::uint32_t used_user_count = 0;
    for (const auto& user : users) {
        if (user.used == 0) {
            continue;
        }
        ++used_user_count;
        const std::string username = user.username;
        if (!detail::valid_name(username)) {
            user_issues.push_back("user table contains an invalid username");
        }
        if (!usernames.insert(username).second) {
            user_issues.push_back("duplicate username " + username);
        }
        if (!uids.insert(user.uid).second) {
            user_issues.push_back("duplicate uid " + std::to_string(user.uid));
        }
        if (!directory_inode_valid(inodes, user.ufd_inode)) {
            user_issues.push_back("user " + username + " has invalid UFD inode");
            continue;
        }
        if (inodes[user.ufd_inode].owner_uid != user.uid) {
            user_issues.push_back("user " + username + " UFD owner uid does not match user table");
        }
        const auto mfd = mfd_entries.find(username);
        if (mfd == mfd_entries.end() || mfd->second != user.ufd_inode) {
            user_issues.push_back("user " + username + " is not mirrored by the MFD entry");
        }

        const auto entries = detail::read_directory_entries(disk_, inodes[user.ufd_inode]);
        for (const auto& entry : entries) {
            if (entry.used == 0) {
                continue;
            }
            if (!detail::valid_name(entry.name)) {
                directory_issues.push_back("UFD entry for user " + username + " has invalid name");
                continue;
            }
            if (entry.inode >= inodes.size() || inodes[entry.inode].used == 0 || inodes[entry.inode].type != 1) {
                directory_issues.push_back("UFD entry " + username + "/" + entry.name + " points at invalid file");
                continue;
            }
            if (inodes[entry.inode].owner_uid != user.uid) {
                directory_issues.push_back("UFD entry " + username + "/" + entry.name +
                                           " points at a file owned by another uid");
            }
        }
    }
    if (used_user_count != sb.user_count) {
        user_issues.push_back("superblock user_count=" + std::to_string(sb.user_count) + " expected " +
                              std::to_string(used_user_count));
    }

    add_check(report, "inode bitmap", std::move(inode_bitmap_issues), "inode bitmap matches used inode records");
    add_check(report, "block bitmap", std::move(block_issues), "block bitmap matches metadata and reachable blocks");
    add_check(report, "file sizes", std::move(size_issues), "file sizes match referenced data-block counts");
    add_check(report, "user table", std::move(user_issues), "user table, UFD pointers, and MFD entries match");
    add_check(report, "directories", std::move(directory_issues), "MFD/UFD entries point at valid inodes");
    return report;
}

} // namespace minikv::osfs
