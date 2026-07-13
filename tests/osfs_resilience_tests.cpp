#include "minikv/osfs.hpp"

#include "osfs_test_support.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <optional>
#include <string>

namespace {

using osfs_test::fsck_has_failed_check;
using osfs_test::parse_fd;
using osfs_test::require_user;
using osfs_test::set_inode_bitmap_used;
using osfs_test::unique_disk_path;

void test_login_switch_closes_descriptors() {
    const auto path = unique_disk_path("session-reset");
    std::filesystem::remove(path);
    auto fs = minikv::osfs::FileSystem::format(path);
    minikv::osfs::CommandProcessor processor{fs};

    assert(processor.execute("LOGIN alice alice123") == "OK login alice uid=1000");
    assert(processor.execute("CREATE session-note") == "OK created session-note");
    const auto old_fd = parse_fd(processor.execute("OPEN session-note rw"));
    assert(processor.execute("WRITE " + std::to_string(old_fd) + " alice-data") == "OK wrote 10 bytes offset=10");

    assert(processor.execute("LOGIN bob bob123") == "OK login bob uid=1001");
    assert(processor.execute("READ " + std::to_string(old_fd)) == "ERR bad fd");
    assert(processor.execute("TELL " + std::to_string(old_fd)) == "ERR bad fd");

    assert(processor.execute("LOGIN alice alice123") == "OK login alice uid=1000");
    const auto read_fd = parse_fd(processor.execute("OPEN session-note r"));
    assert(read_fd == 3);
    assert(processor.execute("WRITE 3 blocked") == "ERR fd not writable");
    assert(processor.execute("CLOSE 3") == "OK closed 3");

    const auto append_fd = parse_fd(processor.execute("OPEN session-note a"));
    assert(processor.execute("READ " + std::to_string(append_fd)) == "ERR fd not readable");
    assert(processor.execute("CLOSE " + std::to_string(append_fd)) == "OK closed 4");
    assert(processor.execute("CLOSE " + std::to_string(append_fd)) == "ERR bad fd");
    std::filesystem::remove(path);
}

void test_group_permissions_and_root_override() {
    const auto path = unique_disk_path("group-permissions");
    std::filesystem::remove(path);
    auto fs = minikv::osfs::FileSystem::format(path);
    const auto users = fs.users();
    const auto& root = require_user(users, "root");
    const auto& alice = require_user(users, "alice");
    const auto& bob = require_user(users, "bob");

    std::string error;
    assert(alice.gid == bob.gid);
    assert(fs.create_file("shared", alice.uid, &error));
    assert(fs.write_file("shared", "owner-data", alice.uid, &error));
    assert(fs.chmod("shared", 0640, alice.uid, &error));
    assert(fs.read_file("alice/shared", bob.uid, &error) == std::optional<std::string>{"owner-data"});
    assert(!fs.write_file("alice/shared", "group-write", bob.uid, &error));
    assert(error == "permission denied");
    assert(!fs.delete_file("alice/shared", bob.uid, &error));
    assert(error == "delete requires owner or root");

    assert(fs.chmod("shared", 0660, alice.uid, &error));
    assert(fs.write_file("alice/shared", "group-write", bob.uid, &error));
    assert(fs.chmod("alice/shared", 0600, root.uid, &error));
    assert(fs.read_file("alice/shared", root.uid, &error) == std::optional<std::string>{"group-write"});
    assert(fs.delete_file("alice/shared", root.uid, &error));
    assert(fs.list_directory(alice.uid).empty());
    std::filesystem::remove(path);
}

void test_directory_growth_and_slot_reuse() {
    const auto path = unique_disk_path("directory-growth");
    std::filesystem::remove(path);
    minikv::osfs::FormatOptions options;
    options.block_count = 96;
    options.inode_count = 32;
    auto fs = minikv::osfs::FileSystem::format(path, options);
    const auto alice = fs.authenticate("alice", "alice123");
    assert(alice.has_value());

    std::string error;
    const auto before = fs.super_block().free_block_count;
    for (int index = 0; index < 8; ++index) {
        assert(fs.create_file("f" + std::to_string(index), alice->uid, &error));
    }
    assert(fs.super_block().free_block_count == before);
    assert(fs.create_file("f8", alice->uid, &error));
    assert(fs.super_block().free_block_count == before - 1);
    assert(fs.list_directory(alice->uid).size() == 9);

    assert(fs.delete_file("f3", alice->uid, &error));
    const auto after_delete = fs.super_block().free_block_count;
    assert(after_delete == before - 1);
    assert(fs.create_file("replacement", alice->uid, &error));
    assert(fs.super_block().free_block_count == after_delete);

    fs = minikv::osfs::FileSystem::open(path);
    const auto entries = fs.list_directory(alice->uid);
    assert(entries.size() == 9);
    assert(std::none_of(entries.begin(), entries.end(), [](const auto& entry) { return entry.name == "f3"; }));
    assert(std::any_of(entries.begin(), entries.end(), [](const auto& entry) { return entry.name == "replacement"; }));
    assert(fs.check_consistency().ok);
    std::filesystem::remove(path);
}

void test_sparse_range_write_zero_fills_gap() {
    const auto path = unique_disk_path("sparse-range");
    std::filesystem::remove(path);
    auto fs = minikv::osfs::FileSystem::format(path);
    const auto alice = fs.authenticate("alice", "alice123");
    assert(alice.has_value());

    std::string error;
    assert(fs.create_file("sparse", alice->uid, &error));
    assert(fs.write_file_range("sparse", 600, "tail", alice->uid, &error));
    const auto contents = fs.read_file("sparse", alice->uid, &error);
    assert(contents.has_value());
    assert(contents->size() == 604);
    assert(std::all_of(contents->begin(), contents->begin() + 600, [](char value) { return value == '\0'; }));
    assert(contents->substr(600) == "tail");
    assert(fs.read_file_range("sparse", 508, 100, alice->uid, &error) ==
           std::optional<std::string>{std::string(92, '\0') + "tail"});

    fs = minikv::osfs::FileSystem::open(path);
    assert(fs.read_file_range("sparse", 600, 4, alice->uid, &error) == std::optional<std::string>{"tail"});
    assert(fs.check_consistency().ok);
    std::filesystem::remove(path);
}

void test_fsck_finds_inode_bitmap_corruption() {
    const auto path = unique_disk_path("inode-bitmap");
    std::filesystem::remove(path);
    auto fs = minikv::osfs::FileSystem::format(path);
    const auto alice = fs.authenticate("alice", "alice123");
    assert(alice.has_value());

    std::string error;
    assert(fs.create_file("inode-check", alice->uid, &error));
    const auto stat = fs.stat("inode-check", alice->uid);
    assert(stat.has_value());
    assert(fs.check_consistency().ok);

    const auto block_size = fs.super_block().block_size;
    set_inode_bitmap_used(path, block_size, stat->inode, false);
    fs = minikv::osfs::FileSystem::open(path);
    const auto report = fs.check_consistency();
    assert(!report.ok);
    assert(fsck_has_failed_check(report, "inode bitmap", "inode bitmap mismatch at inode"));

    minikv::osfs::CommandProcessor processor{fs};
    assert(processor.execute("LOGIN root root") == "OK login root uid=0");
    const auto output = processor.execute("FSCK");
    assert(output.find("FSCK ERROR checks=") != std::string::npos);
    assert(output.find("ERROR inode bitmap: inode bitmap mismatch at inode") != std::string::npos);
    std::filesystem::remove(path);
}

} // namespace

int main() {
    test_login_switch_closes_descriptors();
    test_group_permissions_and_root_override();
    test_directory_growth_and_slot_reuse();
    test_sparse_range_write_zero_fills_gap();
    test_fsck_finds_inode_bitmap_corruption();
    return 0;
}
