#include "atomic_file_writer.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>

namespace {

#ifndef MINIKV_SOURCE_DIR
#error "MINIKV_SOURCE_DIR must be defined"
#endif

std::string read_text(const std::filesystem::path& path) {
    std::ifstream input{path};
    return {std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
}

void assert_contains(std::string_view text, std::string_view fragment) {
    assert(text.find(fragment) != std::string_view::npos);
}

void assert_not_contains(std::string_view text, std::string_view fragment) {
    assert(text.find(fragment) == std::string_view::npos);
}

void write_text(const std::filesystem::path& path, std::string_view text) {
    std::ofstream output{path, std::ios::trunc};
    output << text;
    assert(output.good());
}

void test_successful_replace(const std::filesystem::path& root) {
    const auto target = root / "nested" / "state.dat";
    std::filesystem::create_directories(target.parent_path());
    write_text(target, "old");

    minikv::detail::AtomicFileWriter writer{target};
    assert(writer.ready());
    const auto temp = writer.temp_path();
    assert(temp.parent_path() == target.parent_path());
    assert(temp.filename().string().find(".tmp.") != std::string::npos);
    writer.stream() << "new";
    assert(writer.commit());
    assert(!writer.commit());
    assert(!std::filesystem::exists(temp));
    assert(read_text(target) == "new");
}

void test_abandoned_write(const std::filesystem::path& root) {
    const auto target = root / "abandoned.dat";
    write_text(target, "stable");
    std::filesystem::path temp;
    {
        minikv::detail::AtomicFileWriter writer{target, "draft"};
        assert(writer.ready());
        temp = writer.temp_path();
        writer.stream() << "discarded";
        assert(std::filesystem::exists(temp));
    }
    assert(!std::filesystem::exists(temp));
    assert(read_text(target) == "stable");
}

void test_parent_creation(const std::filesystem::path& root) {
    const auto target = root / "created" / "state.dat";
    assert(!std::filesystem::exists(target.parent_path()));

    minikv::detail::AtomicFileWriter writer{target};
    assert(writer.ready());
    assert(std::filesystem::is_directory(target.parent_path()));
    writer.stream() << "created";
    assert(writer.commit());
    assert(read_text(target) == "created");
}

void test_bad_stream_cleanup(const std::filesystem::path& root) {
    const auto target = root / "bad-stream.dat";
    write_text(target, "stable");
    std::filesystem::path temp;
    {
        minikv::detail::AtomicFileWriter writer{target};
        assert(writer.ready());
        temp = writer.temp_path();
        writer.stream() << "partial";
        writer.stream().setstate(std::ios::badbit);
        assert(!writer.commit());
    }
    assert(!std::filesystem::exists(temp));
    assert(read_text(target) == "stable");
}

void test_replace_failure_cleanup(const std::filesystem::path& root) {
    const auto target = root / "directory-target";
    std::filesystem::create_directory(target);
    std::filesystem::path temp;
    {
        minikv::detail::AtomicFileWriter writer{target};
        assert(writer.ready());
        temp = writer.temp_path();
        writer.stream() << "cannot replace a directory";
        assert(!writer.commit());
    }
    assert(std::filesystem::is_directory(target));
    assert(!std::filesystem::exists(temp));
}

void test_parent_creation_failure(const std::filesystem::path& root) {
    const auto blocker = root / "parent-is-file";
    write_text(blocker, "blocker");

    minikv::detail::AtomicFileWriter writer{blocker / "state.dat"};
    assert(!writer.ready());
    assert(writer.temp_path().empty());
    assert(!writer.commit());
    assert(read_text(blocker) == "blocker");
}

void test_unique_temp_paths(const std::filesystem::path& root) {
    const auto target = root / "shared.dat";
    minikv::detail::AtomicFileWriter first{target, "compact"};
    minikv::detail::AtomicFileWriter second{target, "compact"};
    assert(first.ready());
    assert(second.ready());
    assert(first.temp_path() != second.temp_path());
    assert(first.temp_path().filename().string().find(".compact.tmp.") != std::string::npos);
}

void test_single_owner_contract() {
    const auto source_root = std::filesystem::path{MINIKV_SOURCE_DIR};
    const auto wal_source = read_text(source_root / "src" / "wal.cpp");
    const auto snapshot_source = read_text(source_root / "src" / "snapshot.cpp");

    for (const auto& source : {wal_source, snapshot_source}) {
        assert_contains(source, "#include \"atomic_file_writer.hpp\"");
        assert_contains(source, "detail::AtomicFileWriter output_file");
        assert_not_contains(source, "replace_file_atomically");
        assert_not_contains(source, "MoveFileExW");
    }
    assert_not_contains(wal_source, "TempWalFile");
    assert_not_contains(wal_source, "make_temp_wal_path");
    assert_not_contains(snapshot_source, "TempSnapshotFile");
    assert_not_contains(snapshot_source, "make_temp_snapshot_path");
}

} // namespace

int main() {
    const auto suffix = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto root = std::filesystem::temp_directory_path() / ("minikv-atomic-file-tests-" + std::to_string(suffix));
    std::filesystem::remove_all(root);
    std::filesystem::create_directories(root);

    test_successful_replace(root);
    test_abandoned_write(root);
    test_parent_creation(root);
    test_bad_stream_cleanup(root);
    test_replace_failure_cleanup(root);
    test_parent_creation_failure(root);
    test_unique_temp_paths(root);
    test_single_owner_contract();

    std::filesystem::remove_all(root);
    return 0;
}
