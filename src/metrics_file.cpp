#include "minikv/metrics_file.hpp"

#include <ios>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>

namespace minikv {

namespace {

std::runtime_error filesystem_error(const std::string& action,
                                    const std::filesystem::path& path,
                                    const std::error_code& error) {
    return std::runtime_error{action + " " + path.string() + ": " + error.message()};
}

} // namespace

MetricsFileWriter::MetricsFileWriter(MetricsFileOptions options) : options_(std::move(options)) {
    if (options_.path.empty()) {
        throw std::invalid_argument{"metrics file path must not be empty"};
    }
    if (options_.max_bytes.has_value() && *options_.max_bytes == 0) {
        throw std::invalid_argument{"metrics file max bytes must be positive"};
    }
    open_truncated();
}

void MetricsFileWriter::write_line(std::string_view line) {
    const auto byte_count = static_cast<std::uintmax_t>(line.size()) + 1;
    if (options_.max_bytes.has_value() && current_bytes_ > 0 &&
        current_bytes_ + byte_count > *options_.max_bytes) {
        rotate();
    }

    output_ << line << '\n';
    output_.flush();
    if (!output_) {
        throw std::runtime_error{"cannot write metrics file " + options_.path.string()};
    }
    current_bytes_ += byte_count;
}

const std::filesystem::path& MetricsFileWriter::path() const noexcept {
    return options_.path;
}

std::optional<std::uintmax_t> MetricsFileWriter::max_bytes() const noexcept {
    return options_.max_bytes;
}

std::size_t MetricsFileWriter::keep_files() const noexcept {
    return options_.keep_files;
}

std::filesystem::path MetricsFileWriter::rotated_path(std::size_t index) const {
    auto path = options_.path;
    path += "." + std::to_string(index);
    return path;
}

void MetricsFileWriter::open_truncated() {
    const auto parent = options_.path.parent_path();
    if (!parent.empty()) {
        std::error_code error;
        std::filesystem::create_directories(parent, error);
        if (error) {
            throw filesystem_error("cannot create metrics directory", parent, error);
        }
    }

    output_.open(options_.path, std::ios::out | std::ios::trunc);
    if (!output_) {
        throw std::runtime_error{"cannot open metrics file " + options_.path.string()};
    }
    current_bytes_ = 0;
}

void MetricsFileWriter::rotate() {
    output_.close();

    if (options_.keep_files == 0) {
        remove_if_exists(options_.path);
        open_truncated();
        return;
    }

    remove_if_exists(rotated_path(options_.keep_files));
    for (std::size_t index = options_.keep_files; index > 1; --index) {
        rename_existing(rotated_path(index - 1), rotated_path(index));
    }
    rename_existing(options_.path, rotated_path(1));
    open_truncated();
}

bool MetricsFileWriter::exists(const std::filesystem::path& path) {
    std::error_code error;
    const bool present = std::filesystem::exists(path, error);
    if (error) {
        throw filesystem_error("cannot inspect metrics file", path, error);
    }
    return present;
}

void MetricsFileWriter::remove_if_exists(const std::filesystem::path& path) {
    std::error_code error;
    std::filesystem::remove(path, error);
    if (error) {
        throw filesystem_error("cannot remove metrics file", path, error);
    }
}

void MetricsFileWriter::rename_existing(const std::filesystem::path& from, const std::filesystem::path& to) {
    if (!exists(from)) {
        return;
    }

    remove_if_exists(to);
    std::error_code error;
    std::filesystem::rename(from, to, error);
    if (error) {
        throw filesystem_error("cannot rotate metrics file", from, error);
    }
}

} // namespace minikv
