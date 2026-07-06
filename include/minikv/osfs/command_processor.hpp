#pragma once

#include "minikv/osfs/filesystem.hpp"

#include <cstdint>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace minikv::osfs {

class CommandProcessor {
public:
    explicit CommandProcessor(FileSystem& fs);

    std::string execute(std::string_view line);
    static std::string help_text();

private:
    struct Handle {
        std::string name;
        bool readable = false;
        bool writable = false;
        std::size_t read_offset = 0;
        std::size_t write_offset = 0;
    };

    std::string login(const std::string& user, const std::string& password);
    std::string open_file(const std::string& name, const std::string& mode);
    std::string close_file(int fd);
    std::string read_file(int fd, std::optional<std::size_t> length);
    std::string write_file(int fd, const std::string& contents);
    std::string seek_file(int fd, std::size_t offset);
    std::string tell_file(int fd) const;

    FileSystem& fs_;
    std::string current_user_;
    std::uint32_t current_uid_ = 0;
    bool authenticated_ = false;
    int next_fd_ = 3;
    std::unordered_map<int, Handle> handles_;
};

} // namespace minikv::osfs
