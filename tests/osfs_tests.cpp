#include "minikv/osfs.hpp"

#include "osfs_test_support.hpp"

#include <cassert>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace {

using osfs_test::fsck_has_failed_check;
using osfs_test::parse_fd;
using osfs_test::require_user;
using osfs_test::set_block_bitmap_used;
using osfs_test::unique_disk_path;

void test_disk_users_and_two_level_directories() {
    const auto path = unique_disk_path("two-level");
    std::filesystem::remove(path);

    minikv::osfs::FormatOptions options;
    options.block_count = 96;
    options.inode_count = 32;
    auto fs = minikv::osfs::FileSystem::format(path, options);

    const auto sb = fs.super_block();
    assert(sb.version == 3);
    assert(sb.block_size == 512);
    assert(sb.block_count == 96);
    assert(sb.inode_count == 32);
    assert(sb.data_start_block > 3);
    assert(sb.mfd_inode == 0);
    assert(sb.user_count == 3);
    assert(sb.free_block_count > 0);
    assert(sb.free_inode_count == options.inode_count - 4);

    const auto users = fs.users();
    assert(users.size() == 3);
    const auto& root = require_user(users, "root");
    const auto& alice = require_user(users, "alice");
    const auto& bob = require_user(users, "bob");
    assert(root.uid == 0);
    assert(alice.uid == 1000);
    assert(bob.uid == 1001);
    assert(root.ufd_inode != alice.ufd_inode);
    assert(alice.ufd_inode != bob.ufd_inode);
    assert(fs.authenticate("alice", "wrong") == std::nullopt);
    assert(fs.authenticate("alice", "alice123").has_value());

    std::string error;
    assert(fs.create_file("notes.txt", alice.uid, &error));
    assert(fs.write_file("notes.txt", "alice private notes", alice.uid, &error));
    assert(fs.chmod("notes.txt", 0600, alice.uid, &error));
    assert(fs.create_file("report.txt", bob.uid, &error));
    assert(fs.write_file("report.txt", "bob report", bob.uid, &error));

    const auto alice_entries = fs.list_directory(alice.uid);
    const auto bob_entries = fs.list_directory(bob.uid);
    assert(alice_entries.size() == 1);
    assert(alice_entries.front().name == "notes.txt");
    assert(alice_entries.front().owner_uid == alice.uid);
    assert(bob_entries.size() == 1);
    assert(bob_entries.front().name == "report.txt");
    assert(bob_entries.front().owner_uid == bob.uid);

    assert(!fs.read_file("alice/notes.txt", bob.uid, &error).has_value());
    assert(error == "permission denied");
    assert(!fs.write_file("alice/notes.txt", "intrusion", bob.uid, &error));
    assert(error == "permission denied");
    assert(!fs.list_directory_for("alice", bob.uid, &error).has_value());
    assert(error == "permission denied");
    const auto root_view = fs.list_directory_for("alice", root.uid, &error);
    assert(root_view.has_value());
    assert(root_view->size() == 1);

    fs = minikv::osfs::FileSystem::open(path);
    assert(fs.authenticate("alice", "alice123").has_value());
    assert(fs.authenticate("alice", "wrong") == std::nullopt);
    assert(fs.read_file("notes.txt", alice.uid, &error) == std::optional<std::string>{"alice private notes"});
    assert(fs.list_directory(alice.uid).size() == 1);
    assert(fs.list_directory(bob.uid).size() == 1);

    assert(fs.delete_file("notes.txt", alice.uid, &error));
    assert(fs.list_directory(alice.uid).empty());
    assert(fs.list_directory(bob.uid).size() == 1);
    std::filesystem::remove(path);
}

void test_authenticated_command_shell() {
    const auto path = unique_disk_path("shell");
    std::filesystem::remove(path);
    auto fs = minikv::osfs::FileSystem::format(path);
    minikv::osfs::CommandProcessor processor{fs};

    assert(processor.execute("WHOAMI") == "not logged in");
    assert(processor.execute("CREATE report") == "ERR login required");
    assert(processor.execute("LOGIN alice wrong") == "ERR authentication failed");
    assert(processor.execute("LOGIN alice alice123") == "OK login alice uid=1000");
    assert(processor.execute("WHOAMI") == "alice uid=1000");
    assert(processor.execute("CREATE report") == "OK created report");

    const auto opened = processor.execute("OPEN report rw");
    const auto fd = parse_fd(opened);
    assert(processor.execute("WRITE " + std::to_string(fd) + " operating system storage") ==
           "OK wrote 24 bytes offset=24");
    assert(processor.execute("READ " + std::to_string(fd)) == "operating system storage");
    assert(processor.execute("CLOSE " + std::to_string(fd)) == "OK closed " + std::to_string(fd));
    assert(processor.execute("CHMOD report 0600") == "OK chmod report 0600");
    assert(processor.execute("DIR").find("report") != std::string::npos);

    assert(processor.execute("LOGIN bob bob123") == "OK login bob uid=1001");
    assert(processor.execute("DIR") == "(empty)");
    assert(processor.execute("DIR alice") == "ERR permission denied");
    assert(processor.execute("OPEN alice/report r") == "ERR permission denied");
    assert(processor.execute("OPEN alice/report w") == "ERR permission denied");

    assert(processor.execute("LOGIN root root") == "OK login root uid=0");
    assert(processor.execute("DIR alice").find("report") != std::string::npos);
    assert(processor.execute("OPEN alice/report r").rfind("OK fd=", 0) == 0);
    std::filesystem::remove(path);
}

void test_descriptor_offsets_and_range_io() {
    const auto path = unique_disk_path("offsets");
    std::filesystem::remove(path);
    auto fs = minikv::osfs::FileSystem::format(path);
    minikv::osfs::CommandProcessor processor{fs};
    assert(processor.execute("LOGIN alice alice123") == "OK login alice uid=1000");
    assert(processor.execute("CREATE stream") == "OK created stream");

    auto fd = parse_fd(processor.execute("OPEN stream w"));
    assert(processor.execute("WRITE " + std::to_string(fd) + " hello-") == "OK wrote 6 bytes offset=6");
    assert(processor.execute("WRITE " + std::to_string(fd) + " world") == "OK wrote 5 bytes offset=11");
    assert(processor.execute("TELL " + std::to_string(fd)) == "fd=3 read_offset=0 write_offset=11");
    assert(processor.execute("CLOSE " + std::to_string(fd)) == "OK closed 3");

    fd = parse_fd(processor.execute("OPEN stream r"));
    assert(processor.execute("READ " + std::to_string(fd) + " 5") == "hello");
    assert(processor.execute("READ " + std::to_string(fd) + " 1") == "-");
    assert(processor.execute("TELL " + std::to_string(fd)) == "fd=4 read_offset=6 write_offset=0");
    assert(processor.execute("READ " + std::to_string(fd)) == "world");
    assert(processor.execute("READ " + std::to_string(fd)) == "(eof)");
    assert(processor.execute("CLOSE " + std::to_string(fd)) == "OK closed 4");

    fd = parse_fd(processor.execute("OPEN stream rw"));
    assert(processor.execute("SEEK " + std::to_string(fd) + " 6") == "OK seek 5 6");
    assert(processor.execute("WRITE " + std::to_string(fd) + " OSFS!") == "OK wrote 5 bytes offset=11");
    assert(processor.execute("SEEK " + std::to_string(fd) + " 0") == "OK seek 5 0");
    assert(processor.execute("READ " + std::to_string(fd)) == "hello-OSFS!");
    assert(processor.execute("CLOSE " + std::to_string(fd)) == "OK closed 5");

    fd = parse_fd(processor.execute("OPEN stream a"));
    assert(processor.execute("TELL " + std::to_string(fd)) == "fd=6 read_offset=0 write_offset=11");
    assert(processor.execute("WRITE " + std::to_string(fd) + " -append") == "OK wrote 7 bytes offset=18");
    assert(processor.execute("CLOSE " + std::to_string(fd)) == "OK closed 6");

    const auto alice = fs.authenticate("alice", "alice123");
    assert(alice.has_value());
    std::string error;
    assert(fs.read_file("stream", alice->uid, &error) == std::optional<std::string>{"hello-OSFS!-append"});

    assert(fs.create_file("blocks", alice->uid, &error));
    assert(fs.write_file("blocks", std::string(700, 'x'), alice->uid, &error));
    assert(fs.read_file_range("blocks", 500, 40, alice->uid, &error) ==
           std::optional<std::string>{std::string(40, 'x')});
    assert(fs.write_file_range("blocks", 508, "boundary-write", alice->uid, &error));
    assert(fs.read_file_range("blocks", 508, 14, alice->uid, &error) == std::optional<std::string>{"boundary-write"});
    assert(fs.create_file("indirect-range", alice->uid, &error));
    assert(fs.write_file_range("indirect-range", 8 * 512, "indirect", alice->uid, &error));
    assert(fs.read_file_range("indirect-range", 8 * 512, 8, alice->uid, &error) ==
           std::optional<std::string>{"indirect"});
    const auto block_size = fs.super_block().block_size;
    const auto max_file_size = static_cast<std::size_t>(8 + block_size / sizeof(std::uint32_t)) * block_size;
    assert(!fs.write_file_range("stream", max_file_size, "x", alice->uid, &error));
    assert(error == "file exceeds indirect-block limit");
    assert(fs.read_file("stream", alice->uid, &error) == std::optional<std::string>{"hello-OSFS!-append"});

    assert(fs.create_file("truncate", alice->uid, &error));
    assert(fs.write_file("truncate", "old data", alice->uid, &error));
    fd = parse_fd(processor.execute("OPEN truncate w"));
    assert(processor.execute("CLOSE " + std::to_string(fd)) == "OK closed 7");
    assert(fs.read_file("truncate", alice->uid, &error) == std::optional<std::string>{""});

    fs = minikv::osfs::FileSystem::open(path);
    assert(fs.read_file("stream", alice->uid, &error) == std::optional<std::string>{"hello-OSFS!-append"});
    assert(fs.read_file_range("blocks", 508, 14, alice->uid, &error) == std::optional<std::string>{"boundary-write"});
    std::filesystem::remove(path);
}

void test_failed_rewrite_preserves_existing_data() {
    const auto path = unique_disk_path("capacity");
    std::filesystem::remove(path);
    minikv::osfs::FormatOptions options;
    options.block_count = 24;
    options.inode_count = 8;
    auto fs = minikv::osfs::FileSystem::format(path, options);
    const auto alice = fs.authenticate("alice", "alice123");
    assert(alice.has_value());

    std::string error;
    assert(fs.create_file("keep", alice->uid, &error));
    assert(fs.write_file("keep", "old contents", alice->uid, &error));
    assert(fs.create_file("filler", alice->uid, &error));
    assert(fs.write_file("filler", std::string(8 * options.block_size, 'x'), alice->uid, &error));
    assert(!fs.write_file("keep", std::string(8 * options.block_size, 'y'), alice->uid, &error));
    assert(error == "no free data block");
    assert(fs.read_file("keep", alice->uid, &error) == std::optional<std::string>{"old contents"});
    std::filesystem::remove(path);
}

void test_indirect_blocks_persist_and_release() {
    const auto path = unique_disk_path("indirect");
    std::filesystem::remove(path);
    minikv::osfs::FormatOptions options;
    options.block_count = 192;
    options.inode_count = 32;
    auto fs = minikv::osfs::FileSystem::format(path, options);
    const auto alice = fs.authenticate("alice", "alice123");
    assert(alice.has_value());

    std::string error;
    assert(fs.create_file("large", alice->uid, &error));
    const auto before_write = fs.super_block().free_block_count;
    std::string payload(9 * options.block_size + 37, 'a');
    payload[8 * options.block_size - 1] = 'D';
    payload[8 * options.block_size] = 'I';
    payload.back() = 'Z';
    assert(fs.write_file("large", payload, alice->uid, &error));
    assert(fs.super_block().free_block_count == before_write - 11);
    assert(fs.read_file_range("large", 8 * options.block_size - 1, 2, alice->uid, &error) ==
           std::optional<std::string>{"DI"});
    assert(fs.read_file_range("large", payload.size() - 1, 1, alice->uid, &error) == std::optional<std::string>{"Z"});

    fs = minikv::osfs::FileSystem::open(path);
    assert(fs.authenticate("alice", "alice123").has_value());
    assert(fs.read_file_range("large", 8 * options.block_size - 1, 2, alice->uid, &error) ==
           std::optional<std::string>{"DI"});
    assert(fs.delete_file("large", alice->uid, &error));
    assert(fs.super_block().free_block_count == before_write);
    std::filesystem::remove(path);
}

void test_fsck_reports_ok_and_detects_corruption() {
    const auto path = unique_disk_path("fsck");
    std::filesystem::remove(path);
    minikv::osfs::FormatOptions options;
    options.block_count = 192;
    options.inode_count = 32;
    auto fs = minikv::osfs::FileSystem::format(path, options);
    const auto alice = fs.authenticate("alice", "alice123");
    assert(alice.has_value());

    std::string error;
    assert(fs.create_file("large", alice->uid, &error));
    assert(fs.write_file("large", std::string(9 * options.block_size + 11, 'L'), alice->uid, &error));
    const auto stat = fs.stat("large", alice->uid);
    assert(stat.has_value());

    auto report = fs.check_consistency();
    assert(report.ok);
    assert(report.checks.size() >= 6);

    set_block_bitmap_used(path, options.block_size, stat->first_block, false);
    fs = minikv::osfs::FileSystem::open(path);
    report = fs.check_consistency();
    assert(!report.ok);
    assert(fsck_has_failed_check(report, "block bitmap", "block bitmap mismatch at block"));

    minikv::osfs::CommandProcessor processor{fs};
    assert(processor.execute("LOGIN root root") == "OK login root uid=0");
    const auto output = processor.execute("FSCK");
    assert(output.find("FSCK ERROR checks=") != std::string::npos);
    assert(output.find("ERROR block bitmap: block bitmap mismatch at block") != std::string::npos);
    std::filesystem::remove(path);
}

void test_useradd_passwd_and_fsck_command() {
    const auto path = unique_disk_path("useradmin");
    std::filesystem::remove(path);
    auto fs = minikv::osfs::FileSystem::format(path);
    minikv::osfs::CommandProcessor processor{fs};

    assert(processor.execute("FSCK") == "ERR login required");
    assert(processor.execute("LOGIN alice alice123") == "OK login alice uid=1000");
    assert(processor.execute("USERADD carol carol123") == "ERR USERADD requires root");
    assert(processor.execute("LOGIN root root") == "OK login root uid=0");
    assert(processor.execute("USERADD carol carol123") == "OK useradd carol");
    assert(processor.execute("USERADD carol other") == "ERR user already exists");
    assert(processor.execute("PASSWD carol carol456") == "OK password changed carol");
    const auto fsck_output = processor.execute("FSCK");
    assert(fsck_output.find("FSCK OK checks=") != std::string::npos);
    assert(fsck_output.find("OK user table:") != std::string::npos);

    fs = minikv::osfs::FileSystem::open(path);
    assert(fs.authenticate("carol", "carol123") == std::nullopt);
    const auto carol = fs.authenticate("carol", "carol456");
    assert(carol.has_value());

    minikv::osfs::CommandProcessor carol_shell{fs};
    assert(carol_shell.execute("LOGIN carol carol456") == "OK login carol uid=" + std::to_string(carol->uid));
    assert(carol_shell.execute("CREATE diary") == "OK created diary");
    auto fd = parse_fd(carol_shell.execute("OPEN diary w"));
    assert(carol_shell.execute("WRITE " + std::to_string(fd) + " carol-notes") == "OK wrote 11 bytes offset=11");
    assert(carol_shell.execute("CLOSE " + std::to_string(fd)) == "OK closed " + std::to_string(fd));
    assert(carol_shell.execute("PASSWD carol789") == "OK password changed carol");

    fs = minikv::osfs::FileSystem::open(path);
    assert(fs.authenticate("carol", "carol456") == std::nullopt);
    assert(fs.authenticate("carol", "carol789").has_value());
    minikv::osfs::CommandProcessor bob_shell{fs};
    assert(bob_shell.execute("LOGIN bob bob123") == "OK login bob uid=1001");
    assert(bob_shell.execute("PASSWD carol blocked") == "ERR permission denied");
    assert(bob_shell.execute("DIR carol") == "ERR permission denied");

    minikv::osfs::CommandProcessor root_shell{fs};
    assert(root_shell.execute("LOGIN root root") == "OK login root uid=0");
    assert(root_shell.execute("DIR carol").find("diary") != std::string::npos);

    std::string error;
    assert(fs.add_user("u1", "pw", 0, &error));
    assert(fs.add_user("u2", "pw", 0, &error));
    assert(fs.add_user("u3", "pw", 0, &error));
    assert(fs.add_user("u4", "pw", 0, &error));
    assert(!fs.add_user("u5", "pw", 0, &error));
    assert(error == "user table is full");
    assert(fs.check_consistency().ok);
    std::filesystem::remove(path);
}

} // namespace

int main() {
    test_disk_users_and_two_level_directories();
    test_authenticated_command_shell();
    test_descriptor_offsets_and_range_io();
    test_failed_rewrite_preserves_existing_data();
    test_indirect_blocks_persist_and_release();
    test_fsck_reports_ok_and_detects_corruption();
    test_useradd_passwd_and_fsck_command();
    return 0;
}
