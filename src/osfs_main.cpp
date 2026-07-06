#include "minikv/osfs.hpp"

#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

namespace {

std::string display_path(const std::filesystem::path& path) {
    const auto bytes = path.u8string();
    return {reinterpret_cast<const char*>(bytes.data()), bytes.size()};
}

struct Options {
    std::filesystem::path disk_path;
    std::filesystem::path script_path;
    minikv::osfs::FormatOptions format_options;
    bool format = false;
};

void print_usage() {
    std::cerr << "Usage: minikv_osfs --disk path [--format] [--blocks count] [--block-size bytes] [--script path]\n";
}

template <typename Char> bool equals_arg(const Char* value, const char* expected) {
    std::size_t index = 0;
    for (; expected[index] != '\0'; ++index) {
        if (value[index] == 0 || value[index] != static_cast<Char>(expected[index])) {
            return false;
        }
    }
    return value[index] == 0;
}

template <typename Char> std::optional<std::uint32_t> parse_u32(const Char* text) {
    if (text == nullptr || text[0] == 0) {
        return std::nullopt;
    }
    std::uint64_t value = 0;
    for (auto* cursor = text; *cursor != 0; ++cursor) {
        if (*cursor < static_cast<Char>('0') || *cursor > static_cast<Char>('9')) {
            return std::nullopt;
        }
        value = value * 10 + static_cast<std::uint64_t>(*cursor - static_cast<Char>('0'));
        if (value > UINT32_MAX) {
            return std::nullopt;
        }
    }
    return static_cast<std::uint32_t>(value);
}

template <typename Char> std::optional<Options> parse_options(int argc, Char** argv) {
    Options options;
    for (int index = 1; index < argc; ++index) {
        if (equals_arg(argv[index], "--disk")) {
            if (++index >= argc || argv[index][0] == '\0') {
                return std::nullopt;
            }
            options.disk_path = std::filesystem::path{argv[index]};
            continue;
        }
        if (equals_arg(argv[index], "--format")) {
            options.format = true;
            continue;
        }
        if (equals_arg(argv[index], "--blocks")) {
            if (++index >= argc) {
                return std::nullopt;
            }
            const auto value = parse_u32(argv[index]);
            if (!value.has_value()) {
                return std::nullopt;
            }
            options.format_options.block_count = *value;
            continue;
        }
        if (equals_arg(argv[index], "--block-size")) {
            if (++index >= argc) {
                return std::nullopt;
            }
            const auto value = parse_u32(argv[index]);
            if (!value.has_value()) {
                return std::nullopt;
            }
            options.format_options.block_size = *value;
            continue;
        }
        if (equals_arg(argv[index], "--script")) {
            if (++index >= argc || argv[index][0] == '\0') {
                return std::nullopt;
            }
            options.script_path = std::filesystem::path{argv[index]};
            continue;
        }
        return std::nullopt;
    }
    if (options.disk_path.empty()) {
        return std::nullopt;
    }
    return options;
}

int run_script(minikv::osfs::CommandProcessor& processor, const std::filesystem::path& path) {
    std::ifstream input{path};
    if (!input) {
        std::cerr << "fatal: failed to open script: " << display_path(path) << '\n';
        return 1;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }
        std::cout << "osfs> " << line << '\n';
        const auto result = processor.execute(line);
        if (!result.empty()) {
            std::cout << result << '\n';
        }
        if (result == "BYE") {
            break;
        }
    }
    return 0;
}

} // namespace

template <typename Char> int run_main(int argc, Char** argv) {
    try {
        const auto options = parse_options(argc, argv);
        if (!options.has_value()) {
            print_usage();
            return 2;
        }

        auto fs = options->format ? minikv::osfs::FileSystem::format(options->disk_path, options->format_options)
                                  : minikv::osfs::FileSystem::open(options->disk_path);

        const auto sb = fs.super_block();
        std::cout << "mini-kv osfs course shell\n";
        std::cout << "disk=" << display_path(fs.path()) << " blocks=" << sb.block_count
                  << " block_size=" << sb.block_size << " inodes=" << sb.inode_count << '\n';
        std::cout << minikv::osfs::CommandProcessor::help_text() << '\n';

        minikv::osfs::CommandProcessor processor{fs};
        if (!options->script_path.empty()) {
            return run_script(processor, options->script_path);
        }

        std::string line;
        while (true) {
            std::cout << "osfs> ";
            if (!std::getline(std::cin, line)) {
                std::cout << '\n';
                break;
            }
            const auto result = processor.execute(line);
            if (!result.empty()) {
                std::cout << result << '\n';
            }
            if (result == "BYE") {
                break;
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "fatal: " << error.what() << '\n';
        return 1;
    }

    return 0;
}

#ifdef _WIN32
int wmain(int argc, wchar_t** argv) { return run_main(argc, argv); }
#else
int main(int argc, char** argv) { return run_main(argc, argv); }
#endif
