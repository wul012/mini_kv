#include "minikv/osfs.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <optional>
#include <string>

namespace {

std::filesystem::path unique_disk_path() {
    const auto suffix = std::chrono::system_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() / ("minikv-osfs-test-" + std::to_string(suffix) + ".img");
}

int parse_fd(const std::string& response) {
    const auto marker = response.find("fd=");
    assert(marker != std::string::npos);
    return std::stoi(response.substr(marker + 3));
}

} // namespace

int main() {
    const auto path = unique_disk_path();
    std::filesystem::remove(path);

    minikv::osfs::FormatOptions options;
    options.block_count = 96;
    options.inode_count = 32;

    {
        auto fs = minikv::osfs::FileSystem::format(path, options);
        const auto sb = fs.super_block();
        assert(sb.version == 1);
        assert(sb.block_size == 512);
        assert(sb.block_count == 96);
        assert(sb.inode_count == 32);
        assert(sb.data_start_block > 2);
        assert(fs.list_directory().empty());

        std::string error;
        assert(fs.create_file("notes.txt", 0, &error));
        assert(!fs.create_file("notes.txt", 0, &error));
        assert(error == "file already exists");
        assert(fs.write_file("notes.txt", "hello os course", 0, &error));

        const auto text = fs.read_file("notes.txt", 0, &error);
        assert(text == std::optional<std::string>{"hello os course"});

        const auto entries = fs.list_directory();
        assert(entries.size() == 1);
        assert(entries.front().name == "notes.txt");
        assert(entries.front().first_block >= sb.data_start_block);
        assert(entries.front().size == 15);
        assert(minikv::osfs::format_mode(entries.front().mode) == "0644");

        const auto stat = fs.stat("notes.txt");
        assert(stat.has_value());
        assert(stat->name == "notes.txt");
        assert(stat->size == 15);
        assert(stat->owner_uid == 0);
    }

    {
        auto fs = minikv::osfs::FileSystem::open(path);
        std::string error;
        const auto text = fs.read_file("notes.txt", 0, &error);
        assert(text == std::optional<std::string>{"hello os course"});

        assert(fs.chmod("notes.txt", 0600, 0, &error));
        assert(!fs.read_file("notes.txt", minikv::osfs::uid_for_user("guest"), &error).has_value());
        assert(error == "permission denied");
        assert(!fs.write_file("notes.txt", "guest edit", minikv::osfs::uid_for_user("guest"), &error));
        assert(error == "permission denied");
        assert(fs.delete_file("notes.txt", 0, &error));
        assert(fs.list_directory().empty());
    }

    {
        auto fs = minikv::osfs::FileSystem::open(path);
        minikv::osfs::CommandProcessor processor{fs};
        assert(processor.execute("WHOAMI") == "root uid=0");
        assert(processor.execute("CREATE report") == "OK created report");

        const auto opened = processor.execute("OPEN report rw");
        assert(opened.rfind("OK fd=", 0) == 0);
        const auto fd = parse_fd(opened);
        assert(processor.execute("WRITE " + std::to_string(fd) + " operating system storage") == "OK wrote 24 bytes");
        assert(processor.execute("READ " + std::to_string(fd)) == "operating system storage");
        assert(processor.execute("CLOSE " + std::to_string(fd)) == "OK closed " + std::to_string(fd));
        assert(processor.execute("READ " + std::to_string(fd)) == "ERR bad fd");

        const auto dir = processor.execute("DIR");
        assert(dir.find("report") != std::string::npos);
        assert(dir.find("0644") != std::string::npos);
        assert(processor.execute("LOGIN guest").rfind("OK login guest uid=", 0) == 0);
        assert(processor.execute("OPEN report r").rfind("OK fd=", 0) == 0);
        assert(processor.execute("OPEN report w") == "ERR permission denied");
    }

    std::filesystem::remove(path);

    const auto full_path = unique_disk_path();
    std::filesystem::remove(full_path);
    {
        minikv::osfs::FormatOptions tiny_options;
        tiny_options.block_count = 16;
        tiny_options.inode_count = 8;

        auto fs = minikv::osfs::FileSystem::format(full_path, tiny_options);
        std::string error;
        assert(fs.create_file("keep", 0, &error));
        assert(fs.write_file("keep", "old contents", 0, &error));
        assert(fs.create_file("filler", 0, &error));
        assert(fs.write_file("filler", std::string(8 * tiny_options.block_size, 'x'), 0, &error));
        assert(!fs.write_file("keep", std::string(4 * tiny_options.block_size, 'y'), 0, &error));
        assert(error == "no free data block");
        assert(fs.read_file("keep", 0, &error) == std::optional<std::string>{"old contents"});
    }
    std::filesystem::remove(full_path);

    return 0;
}
