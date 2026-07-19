#include <algorithm>
#include <cassert>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::size_t test_source_limit = 1000;

struct SourceSize {
    std::string path;
    std::size_t nonblank_lines = 0;
};

struct SuitePart {
    std::string path;
    std::string entry;
};

using SourceSizes = std::vector<SourceSize>;
using SizeBaseline = std::map<std::string, std::size_t>;
using SuiteParts = std::vector<SuitePart>;
using TokenCounts = std::map<std::string, std::size_t>;

bool is_blank(std::string_view line) {
    return std::all_of(line.begin(), line.end(), [](unsigned char character) { return std::isspace(character) != 0; });
}

std::size_t count_nonblank_lines(const std::filesystem::path& path) {
    std::ifstream input{path};
    if (!input.is_open()) {
        throw std::runtime_error{"cannot read test source: " + path.generic_string()};
    }

    std::size_t count = 0;
    std::string line;
    while (std::getline(input, line)) {
        if (!is_blank(line)) {
            ++count;
        }
    }
    return count;
}

std::string read_text(const std::filesystem::path& path) {
    std::ifstream input{path, std::ios::binary};
    if (!input.is_open()) {
        throw std::runtime_error{"cannot read test architecture file: " + path.generic_string()};
    }
    return {std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
}

std::size_t count_occurrences(std::string_view text, std::string_view token) {
    std::size_t count = 0;
    std::size_t offset = 0;
    while ((offset = text.find(token, offset)) != std::string_view::npos) {
        ++count;
        offset += token.size();
    }
    return count;
}

SizeBaseline read_baseline(const std::filesystem::path& path) {
    std::ifstream input{path};
    if (!input.is_open()) {
        throw std::runtime_error{"cannot read test source baseline: " + path.generic_string()};
    }

    SizeBaseline baseline;
    std::string row;
    while (std::getline(input, row)) {
        if (row.empty() || row.front() == '#') {
            continue;
        }
        const auto separator = row.find('|');
        if (separator == std::string::npos || row.find('|', separator + 1) != std::string::npos) {
            throw std::runtime_error{"invalid test source baseline row: " + row};
        }
        const auto path_text = row.substr(0, separator);
        const auto count_text = row.substr(separator + 1);
        if (path_text.empty() || count_text.empty()) {
            throw std::runtime_error{"invalid test source baseline row: " + row};
        }
        const auto [position, inserted] = baseline.emplace(path_text, std::stoull(count_text));
        if (!inserted) {
            throw std::runtime_error{"duplicate test source baseline path: " + path_text};
        }
        (void)position;
    }
    return baseline;
}

SuiteParts read_suite_parts(const std::filesystem::path& path) {
    std::ifstream input{path};
    if (!input.is_open()) {
        throw std::runtime_error{"cannot read test suite manifest: " + path.generic_string()};
    }

    SuiteParts parts;
    std::string row;
    while (std::getline(input, row)) {
        if (row.empty() || row.front() == '#') {
            continue;
        }
        const auto separator = row.find('|');
        if (separator == std::string::npos || row.find('|', separator + 1) != std::string::npos) {
            throw std::runtime_error{"invalid test suite manifest row: " + row};
        }
        parts.push_back({row.substr(0, separator), row.substr(separator + 1)});
    }
    return parts;
}

SourceSizes scan_test_sources(const std::filesystem::path& source_root) {
    SourceSizes sources;
    const auto test_root = source_root / "tests";
    for (const auto& entry : std::filesystem::recursive_directory_iterator{test_root}) {
        if (!entry.is_regular_file() || entry.path().extension() != ".cpp") {
            continue;
        }
        sources.push_back(SourceSize{
            std::filesystem::relative(entry.path(), source_root).generic_string(),
            count_nonblank_lines(entry.path()),
        });
    }
    std::sort(sources.begin(), sources.end(),
              [](const SourceSize& left, const SourceSize& right) { return left.path < right.path; });
    return sources;
}

void verify_source_sizes(const SourceSizes& sources, const SizeBaseline& baseline) {
    std::map<std::string, std::size_t> oversized;
    for (const auto& source : sources) {
        const auto expected = baseline.find(source.path);
        if (source.nonblank_lines > test_source_limit) {
            if (expected == baseline.end()) {
                throw std::runtime_error{"test source size limit exceeded without baseline: " + source.path};
            }
            if (source.nonblank_lines != expected->second) {
                throw std::runtime_error{"test source size baseline must shrink in the same commit: " + source.path};
            }
            oversized.emplace(source.path, source.nonblank_lines);
        } else if (expected != baseline.end()) {
            throw std::runtime_error{"stale test source size baseline entry: " + source.path};
        }
    }
    if (oversized.size() != baseline.size()) {
        throw std::runtime_error{"baseline contains a missing test source path"};
    }
}

void verify_suite(const std::filesystem::path& source_root, const std::filesystem::path& manifest_path,
                  const std::filesystem::path& main_path, const TokenCounts& expected_tokens) {
    const auto parts = read_suite_parts(source_root / manifest_path);
    const auto main_text = read_text(source_root / main_path);
    std::map<std::string, bool> paths;
    std::map<std::string, bool> entries;
    TokenCounts observed_tokens;

    for (const auto& part : parts) {
        if (part.path.empty() || part.entry.empty() || !paths.emplace(part.path, true).second ||
            !entries.emplace(part.entry, true).second) {
            throw std::runtime_error{"duplicate or empty test suite part: " + part.path};
        }
        const auto part_text = read_text(source_root / part.path);
        const auto definition = "void " + part.entry + "(CommandFixture& fixture)";
        const auto call = part.entry + "(fixture);";
        if (count_occurrences(part_text, definition) != 1) {
            throw std::runtime_error{"test suite part must define its entry exactly once: " + part.entry};
        }
        if (count_occurrences(main_text, call) != 1) {
            throw std::runtime_error{"test suite main must call its entry exactly once: " + part.entry};
        }
        for (const auto& [token, expected] : expected_tokens) {
            (void)expected;
            observed_tokens[token] += count_occurrences(part_text, token);
        }
    }

    if (observed_tokens != expected_tokens) {
        throw std::runtime_error{"test suite assertion census changed: " + manifest_path.generic_string()};
    }
}

void assert_failure(const SourceSizes& sources, const SizeBaseline& baseline, std::string_view expected) {
    try {
        verify_source_sizes(sources, baseline);
    } catch (const std::runtime_error& error) {
        assert(std::string_view{error.what()}.find(expected) != std::string_view::npos);
        return;
    }
    assert(false && "expected test architecture contract failure");
}

void verify_red_paths() {
    assert_failure({{"tests/new_oversized.cpp", 1001}}, {}, "without baseline");
    assert_failure({{"tests/growing.cpp", 1002}}, {{"tests/growing.cpp", 1001}}, "must shrink");
    assert_failure({{"tests/repaid.cpp", 999}}, {{"tests/repaid.cpp", 1001}}, "stale");
}

} // namespace

int main() {
    const std::filesystem::path source_root{MINIKV_SOURCE_DIR};
    const auto baseline = read_baseline(source_root / "config" / "test-source-size-baseline.txt");
    const auto sources = scan_test_sources(source_root);
    verify_source_sizes(sources, baseline);
    verify_suite(source_root, "config/command-test-parts.txt", "tests/command_tests.cpp",
                 {{"assert(", 457}, {"assert_response_contains(", 2447}});
    verify_red_paths();

    std::cout << "test architecture: sources=" << sources.size() << " limit=" << test_source_limit
              << " oversized=" << baseline.size() << '\n';
    return 0;
}
