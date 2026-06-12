#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string_view>

namespace minikv {

struct MetricsFileOptions {
    std::filesystem::path path;
    std::optional<std::uintmax_t> max_bytes;
    std::size_t keep_files = 3;
};

class MetricsFileWriter {
public:
    explicit MetricsFileWriter(MetricsFileOptions options);

    void write_line(std::string_view line);

    const std::filesystem::path& path() const noexcept;
    std::optional<std::uintmax_t> max_bytes() const noexcept;
    std::size_t keep_files() const noexcept;

private:
    std::filesystem::path rotated_path(std::size_t index) const;
    void open_truncated();
    void rotate();

    static bool exists(const std::filesystem::path& path);
    static void remove_if_exists(const std::filesystem::path& path);
    static void rename_existing(const std::filesystem::path& from, const std::filesystem::path& to);

    MetricsFileOptions options_;
    std::ofstream output_;
    std::uintmax_t current_bytes_ = 0;
};

} // namespace minikv
