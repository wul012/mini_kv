#include "minikv/snapshot.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace minikv {
namespace {

constexpr std::string_view snapshot_header = "MINIKV_SNAPSHOT 1";

class TempSnapshotFile {
public:
    explicit TempSnapshotFile(std::filesystem::path path) : path_(std::move(path)) {}

    ~TempSnapshotFile() {
        if (!active_) {
            return;
        }

        std::error_code error;
        std::filesystem::remove(path_, error);
    }

    TempSnapshotFile(const TempSnapshotFile&) = delete;
    TempSnapshotFile& operator=(const TempSnapshotFile&) = delete;

    const std::filesystem::path& path() const {
        return path_;
    }

    void release() {
        active_ = false;
    }

private:
    std::filesystem::path path_;
    bool active_ = true;
};

std::filesystem::path make_temp_snapshot_path(const std::filesystem::path& path) {
    const auto parent = path.parent_path();
    const std::string filename = path.filename().empty() ? "snapshot" : path.filename().string();
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();

    for (int attempt = 0; attempt < 100; ++attempt) {
        auto candidate = parent / (filename + ".tmp." + std::to_string(stamp) + "." + std::to_string(attempt));
        std::error_code error;
        if (!std::filesystem::exists(candidate, error)) {
            return candidate;
        }
    }

    return parent / (filename + ".tmp." + std::to_string(stamp) + ".fallback");
}

bool replace_file_atomically(const std::filesystem::path& source, const std::filesystem::path& target) {
#ifdef _WIN32
    return MoveFileExW(source.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
    std::error_code error;
    std::filesystem::rename(source, target, error);
    return !error;
#endif
}

std::string format_expires_at(std::optional<Store::TimePoint> expires_at) {
    if (!expires_at.has_value()) {
        return "-";
    }

    const auto epoch_millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(expires_at->time_since_epoch()).count();
    return std::to_string(epoch_millis);
}

bool parse_expires_at(const std::string& text, std::optional<Store::TimePoint>& expires_at) {
    if (text == "-") {
        expires_at = std::nullopt;
        return true;
    }

    try {
        std::size_t parsed = 0;
        const long long epoch_millis = std::stoll(text, &parsed);
        if (parsed != text.size()) {
            return false;
        }

        expires_at = Store::TimePoint{std::chrono::milliseconds{epoch_millis}};
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace

bool SnapshotFile::save(const Store& store, const std::filesystem::path& path, std::size_t* saved) {
    const auto items = store.snapshot_items();

    if (path.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(path.parent_path(), error);
        if (error) {
            return false;
        }
    }

    TempSnapshotFile temp_file{make_temp_snapshot_path(path)};
    std::ofstream output{temp_file.path(), std::ios::trunc};
    if (!output) {
        return false;
    }

    output << snapshot_header << '\n';
    for (const auto& item : items) {
        output << "ENTRY " << format_expires_at(item.expires_at) << ' ' << std::quoted(item.key) << ' '
               << std::quoted(item.value) << '\n';
    }

    output.flush();
    if (!output) {
        return false;
    }

    output.close();
    if (!output) {
        return false;
    }

    if (!replace_file_atomically(temp_file.path(), path)) {
        return false;
    }
    temp_file.release();

    if (saved != nullptr) {
        *saved = items.size();
    }
    return true;
}

bool SnapshotFile::load(Store& store, const std::filesystem::path& path, std::size_t* loaded) {
    std::ifstream input{path};
    if (!input) {
        return false;
    }

    std::string header;
    if (!std::getline(input, header) || header != snapshot_header) {
        return false;
    }

    std::vector<Store::SnapshotItem> items;
    std::string tag;
    while (input >> tag) {
        if (tag != "ENTRY") {
            return false;
        }

        std::string expires_text;
        std::string key;
        std::string value;
        if (!(input >> expires_text >> std::quoted(key) >> std::quoted(value))) {
            return false;
        }

        std::optional<Store::TimePoint> expires_at;
        if (!parse_expires_at(expires_text, expires_at)) {
            return false;
        }

        items.push_back(Store::SnapshotItem{std::move(key), std::move(value), expires_at});
    }

    if (!input.eof()) {
        return false;
    }

    const auto count = items.size();
    store.restore_snapshot(std::move(items));

    if (loaded != nullptr) {
        *loaded = count;
    }
    return true;
}

} // namespace minikv
