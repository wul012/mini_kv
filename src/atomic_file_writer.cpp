#include "atomic_file_writer.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <string>
#include <system_error>
#include <utility>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace minikv::detail {
namespace {

std::atomic<std::uint64_t> next_temp_id{0};

std::uint64_t process_id() {
#ifdef _WIN32
    return static_cast<std::uint64_t>(GetCurrentProcessId());
#else
    return static_cast<std::uint64_t>(getpid());
#endif
}

std::filesystem::path make_temp_path(const std::filesystem::path& target, std::string_view temp_tag) {
    const auto parent = target.parent_path();
    const std::string filename = target.filename().empty() ? "atomic" : target.filename().string();
    const std::string tag = temp_tag.empty() ? ".tmp." : "." + std::string{temp_tag} + ".tmp.";
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto sequence = next_temp_id.fetch_add(1, std::memory_order_relaxed);
    return parent / (filename + tag + std::to_string(process_id()) + "." + std::to_string(stamp) + "." +
                     std::to_string(sequence));
}

bool replace_file(const std::filesystem::path& source, const std::filesystem::path& target) {
#ifdef _WIN32
    return MoveFileExW(source.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    std::error_code error;
    std::filesystem::rename(source, target, error);
    return !error;
#endif
}

} // namespace

AtomicFileWriter::AtomicFileWriter(std::filesystem::path target, std::string_view temp_tag)
    : target_path_(std::move(target)) {
    if (target_path_.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(target_path_.parent_path(), error);
        if (error) {
            return;
        }
        const bool parent_is_directory = std::filesystem::is_directory(target_path_.parent_path(), error);
        if (error || !parent_is_directory) {
            return;
        }
    }

    temp_path_ = make_temp_path(target_path_, temp_tag);
    output_.open(temp_path_, std::ios::trunc);
}

AtomicFileWriter::~AtomicFileWriter() {
    if (output_.is_open()) {
        output_.close();
    }
    if (committed_ || temp_path_.empty()) {
        return;
    }

    std::error_code error;
    std::filesystem::remove(temp_path_, error);
}

bool AtomicFileWriter::ready() const { return output_.is_open() && output_.good(); }

std::ostream& AtomicFileWriter::stream() { return output_; }

bool AtomicFileWriter::commit() {
    if (committed_ || !output_.is_open()) {
        return false;
    }

    output_.flush();
    if (!output_) {
        return false;
    }

    output_.close();
    if (!output_) {
        return false;
    }

    if (!replace_file(temp_path_, target_path_)) {
        return false;
    }
    committed_ = true;
    return true;
}

const std::filesystem::path& AtomicFileWriter::temp_path() const { return temp_path_; }

} // namespace minikv::detail
