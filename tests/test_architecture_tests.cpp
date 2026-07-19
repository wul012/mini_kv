#include <algorithm>
#include <cassert>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

constexpr std::size_t test_source_limit = 1000;
constexpr std::size_t test_target_name_limit = 40;
constexpr std::size_t object_path_score_limit = 197;

struct SourceSize {
    std::string path;
    std::size_t nonblank_lines = 0;
};

struct SuitePart {
    std::string path;
    std::string entry;
};

struct CMakeTestRegistration {
    std::string target;
    std::string test_name;
    std::string source;
};

struct TargetMetrics {
    std::size_t registrations = 0;
    std::size_t long_targets = 0;
    std::size_t max_path_score = 0;
};

using SourceSizes = std::vector<SourceSize>;
using SizeBaseline = std::map<std::string, std::size_t>;
using SuiteParts = std::vector<SuitePart>;
using TokenCounts = std::map<std::string, std::size_t>;
using TargetBaseline = std::set<std::string>;
using TestRegistrations = std::vector<CMakeTestRegistration>;

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

std::string trim_copy(std::string_view value) {
    const auto first = std::find_if_not(value.begin(), value.end(),
                                        [](unsigned char character) { return std::isspace(character) != 0; });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char character) {
                          return std::isspace(character) != 0;
                      }).base();
    return first < last ? std::string{first, last} : std::string{};
}

bool is_test_helper(std::string_view line) {
    return line == "minikv_add_linked_test(" || line == "minikv_add_source_dir_test(" ||
           line == "minikv_add_standalone_source_dir_test(";
}

TestRegistrations parse_test_registrations(std::string_view cmake_text) {
    std::istringstream input{std::string{cmake_text}};
    TestRegistrations registrations;
    std::string line;
    while (std::getline(input, line)) {
        if (!is_test_helper(trim_copy(line))) {
            continue;
        }

        std::vector<std::string> arguments;
        bool closed = false;
        while (std::getline(input, line)) {
            auto argument = trim_copy(line);
            if (argument == ")") {
                closed = true;
                break;
            }
            if (!argument.empty() && argument.front() != '#') {
                if (argument.find_first_of(" \t") != std::string::npos) {
                    throw std::runtime_error{"CMake test helper argument must occupy one line: " + argument};
                }
                arguments.push_back(std::move(argument));
            }
        }
        if (!closed || arguments.size() != 3) {
            throw std::runtime_error{"CMake test helper must contain target, test name, and source"};
        }
        registrations.push_back({std::move(arguments[0]), std::move(arguments[1]), std::move(arguments[2])});
    }
    return registrations;
}

TargetBaseline read_target_baseline(const std::filesystem::path& path) {
    std::ifstream input{path};
    if (!input.is_open()) {
        throw std::runtime_error{"cannot read CMake target baseline: " + path.generic_string()};
    }

    TargetBaseline baseline;
    std::string row;
    while (std::getline(input, row)) {
        row = trim_copy(row);
        if (row.empty() || row.front() == '#') {
            continue;
        }
        if (row.size() <= test_target_name_limit) {
            throw std::runtime_error{"CMake target baseline contains a compliant name: " + row};
        }
        if (!baseline.emplace(row).second) {
            throw std::runtime_error{"duplicate CMake target baseline name: " + row};
        }
    }
    return baseline;
}

TargetMetrics verify_test_targets(const std::filesystem::path& source_root, const TestRegistrations& registrations,
                                  const TargetBaseline& baseline) {
    std::set<std::string> targets;
    std::set<std::string> test_names;
    std::set<std::string> observed_long_targets;
    TargetMetrics metrics{registrations.size(), 0, 0};

    for (const auto& registration : registrations) {
        if (!targets.emplace(registration.target).second) {
            throw std::runtime_error{"duplicate internal CMake test target: " + registration.target};
        }
        if (!test_names.emplace(registration.test_name).second) {
            throw std::runtime_error{"duplicate public CTest name: " + registration.test_name};
        }

        const auto path_score = registration.target.size() + registration.source.size();
        metrics.max_path_score = std::max(metrics.max_path_score, path_score);
        if (path_score > object_path_score_limit) {
            throw std::runtime_error{"CMake test object path score exceeds budget: " + registration.target};
        }
        if (!std::filesystem::is_regular_file(source_root / registration.source)) {
            throw std::runtime_error{"CMake test source is missing: " + registration.source};
        }
        if (registration.target.size() > test_target_name_limit) {
            if (!baseline.contains(registration.target)) {
                throw std::runtime_error{"new long internal CMake test target: " + registration.target};
            }
            observed_long_targets.emplace(registration.target);
        }
    }

    metrics.long_targets = observed_long_targets.size();
    if (observed_long_targets != baseline) {
        throw std::runtime_error{"stale internal CMake test target baseline entry"};
    }
    if (metrics.max_path_score < object_path_score_limit) {
        throw std::runtime_error{"CMake test object path score budget must shrink in the same commit"};
    }
    return metrics;
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
                  const std::filesystem::path& main_path, std::string_view fixture_type,
                  const TokenCounts& expected_tokens, const TokenCounts& required_calls = {}) {
    const auto parts = read_suite_parts(source_root / manifest_path);
    const auto main_text = read_text(source_root / main_path);
    std::map<std::string, bool> paths;
    std::map<std::string, bool> entries;
    std::map<std::string, std::string> part_texts;
    TokenCounts observed_tokens;
    std::string call_graph_text = main_text;

    for (const auto& part : parts) {
        if (part.path.empty() || part.entry.empty() || !paths.emplace(part.path, true).second ||
            !entries.emplace(part.entry, true).second) {
            throw std::runtime_error{"duplicate or empty test suite part: " + part.path};
        }
        const auto part_text = read_text(source_root / part.path);
        part_texts.emplace(part.path, part_text);
        call_graph_text += part_text;
        for (const auto& [token, expected] : expected_tokens) {
            (void)expected;
            observed_tokens[token] += count_occurrences(part_text, token);
        }
    }

    for (const auto& part : parts) {
        const auto& part_text = part_texts.at(part.path);
        const auto definition = "void " + part.entry + "(" + std::string{fixture_type} + "& fixture)";
        const auto call = part.entry + "(fixture);";
        if (count_occurrences(part_text, definition) != 1) {
            throw std::runtime_error{"test suite part must define its entry exactly once: " + part.entry};
        }
        if (count_occurrences(call_graph_text, call) != 1) {
            throw std::runtime_error{"test suite graph must call its entry exactly once: " + part.entry};
        }
    }

    if (observed_tokens != expected_tokens) {
        throw std::runtime_error{"test suite assertion census changed: " + manifest_path.generic_string()};
    }
    for (const auto& [call, expected] : required_calls) {
        if (count_occurrences(call_graph_text, call) != expected) {
            throw std::runtime_error{"test suite required call count changed: " + call};
        }
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

void assert_target_failure(const std::filesystem::path& source_root, const TestRegistrations& registrations,
                           const TargetBaseline& baseline, std::string_view expected) {
    try {
        (void)verify_test_targets(source_root, registrations, baseline);
    } catch (const std::runtime_error& error) {
        assert(std::string_view{error.what()}.find(expected) != std::string_view::npos);
        return;
    }
    assert(false && "expected CMake test target contract failure");
}

void verify_red_paths(const std::filesystem::path& source_root) {
    assert_failure({{"tests/new_oversized.cpp", 1001}}, {}, "without baseline");
    assert_failure({{"tests/growing.cpp", 1002}}, {{"tests/growing.cpp", 1001}}, "must shrink");
    assert_failure({{"tests/repaid.cpp", 999}}, {{"tests/repaid.cpp", 1001}}, "stale");

    const std::string long_target(test_target_name_limit + 1, 't');
    assert_target_failure(source_root, {{long_target, "synthetic_test", "tests/store_tests.cpp"}}, {},
                          "new long internal");
    assert_target_failure(source_root, {}, {long_target}, "stale internal");
    const std::string risky_source(object_path_score_limit + 1 - test_target_name_limit, 's');
    assert_target_failure(source_root, {{std::string(test_target_name_limit, 't'), "synthetic_test", risky_source}}, {},
                          "object path score");
}

} // namespace

int main() {
    const std::filesystem::path source_root{MINIKV_SOURCE_DIR};
    const auto baseline = read_baseline(source_root / "config" / "test-source-size-baseline.txt");
    const auto sources = scan_test_sources(source_root);
    verify_source_sizes(sources, baseline);
    const auto target_baseline = read_target_baseline(source_root / "config" / "test-target-name-baseline.txt");
    const auto registrations = parse_test_registrations(read_text(source_root / "CMakeLists.txt"));
    const auto target_metrics = verify_test_targets(source_root, registrations, target_baseline);
    verify_suite(source_root, "config/command-test-parts.txt", "tests/command_tests.cpp", "CommandFixture",
                 {{"assert(", 457}, {"assert_response_contains(", 2447}});
    verify_suite(source_root, "config/shard-test-parts.txt", "tests/shard_readiness_tests.cpp", "ShardFixture",
                 {{"assert(", 8}, {"assert_contains(", 3710}, {"assert_fixture_differs_from_each(", 2}},
                 {{"assert_shard_readiness_contract(fixture, fixture.current);", 1},
                  {"assert_shard_readiness_contract(fixture, result.response);", 1}});
    verify_red_paths(source_root);

    std::cout << "test architecture: sources=" << sources.size() << " limit=" << test_source_limit
              << " oversized=" << baseline.size() << " cmake_tests=" << target_metrics.registrations
              << " long_targets=" << target_metrics.long_targets << " max_path_score=" << target_metrics.max_path_score
              << '\n';
    return 0;
}
