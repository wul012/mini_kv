#include "minikv/snapshot.hpp"

#include "atomic_file_writer.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace minikv {
namespace {

constexpr std::string_view snapshot_header = "MINIKV_SNAPSHOT 1";

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

    detail::AtomicFileWriter output_file{path};
    if (!output_file.ready()) {
        return false;
    }
    auto& output = output_file.stream();

    output << snapshot_header << '\n';
    for (const auto& item : items) {
        output << "ENTRY " << format_expires_at(item.expires_at) << ' ' << std::quoted(item.key) << ' '
               << std::quoted(item.value) << '\n';
    }

    if (!output_file.commit()) {
        return false;
    }

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
