#include "minikv/metrics_file.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace {

std::filesystem::path unique_test_dir() {
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() /
           ("minikv_metrics_file_tests_" + std::to_string(now));
}

std::string read_text(const std::filesystem::path& path) {
    std::ifstream input{path};
    assert(input);
    return {std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
}

bool contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

} // namespace

int main() {
    const auto root = unique_test_dir();
    std::filesystem::create_directories(root);

    try {
        const auto rotating_path = root / "server.metrics.log";
        minikv::MetricsFileOptions rotating_options;
        rotating_options.path = rotating_path;
        rotating_options.max_bytes = 90;
        rotating_options.keep_files = 2;

        {
            minikv::MetricsFileWriter rotating{rotating_options};
            rotating.write_line("event=server_metrics sample=one payload=aaaaaaaaaaaaaaaaaaaaaaaaaaaa");
            rotating.write_line("event=server_metrics sample=two payload=bbbbbbbbbbbbbbbbbbbbbbbbbbbb");
            rotating.write_line("event=server_metrics sample=three payload=cccccccccccccccccccccccccc");
            rotating.write_line("event=server_metrics sample=four payload=ddddddddddddddddddddddddddd");
        }

        assert(std::filesystem::exists(rotating_path));
        assert(std::filesystem::exists(rotating_path.string() + ".1"));
        assert(std::filesystem::exists(rotating_path.string() + ".2"));
        assert(!std::filesystem::exists(rotating_path.string() + ".3"));
        assert(contains(read_text(rotating_path), "sample=four"));
        assert(contains(read_text(rotating_path.string() + ".1"), "sample=three"));
        assert(contains(read_text(rotating_path.string() + ".2"), "sample=two"));
        assert(!contains(read_text(rotating_path.string() + ".2"), "sample=one"));

        const auto no_keep_path = root / "discard-old.metrics.log";
        minikv::MetricsFileOptions no_keep_options;
        no_keep_options.path = no_keep_path;
        no_keep_options.max_bytes = 60;
        no_keep_options.keep_files = 0;

        {
            minikv::MetricsFileWriter no_keep{no_keep_options};
            no_keep.write_line("event=server_metrics sample=first payload=xxxxxxxxxxxxxxxxxxxxxxxxx");
            no_keep.write_line("event=server_metrics sample=second payload=yyyyyyyyyyyyyyyyyyyyyyyy");
        }

        assert(std::filesystem::exists(no_keep_path));
        assert(!std::filesystem::exists(no_keep_path.string() + ".1"));
        assert(contains(read_text(no_keep_path), "sample=second"));
        assert(!contains(read_text(no_keep_path), "sample=first"));

        const auto plain_path = root / "plain.metrics.log";
        minikv::MetricsFileOptions plain_options;
        plain_options.path = plain_path;
        plain_options.keep_files = 1;

        {
            minikv::MetricsFileWriter plain{plain_options};
            plain.write_line("event=server_metrics sample=alpha");
            plain.write_line("event=server_metrics sample=beta");
        }

        const auto plain_text = read_text(plain_path);
        assert(contains(plain_text, "sample=alpha"));
        assert(contains(plain_text, "sample=beta"));
        assert(!std::filesystem::exists(plain_path.string() + ".1"));

        bool rejected_zero_limit = false;
        try {
            minikv::MetricsFileOptions invalid_options;
            invalid_options.path = root / "invalid.metrics.log";
            invalid_options.max_bytes = 0;
            minikv::MetricsFileWriter invalid{invalid_options};
        } catch (const std::invalid_argument&) {
            rejected_zero_limit = true;
        }
        assert(rejected_zero_limit);

        bool rejected_empty_path = false;
        try {
            minikv::MetricsFileOptions invalid_options;
            minikv::MetricsFileWriter invalid{invalid_options};
        } catch (const std::invalid_argument&) {
            rejected_empty_path = true;
        }
        assert(rejected_empty_path);
    } catch (...) {
        std::filesystem::remove_all(root);
        throw;
    }

    std::filesystem::remove_all(root);
    return 0;
}
