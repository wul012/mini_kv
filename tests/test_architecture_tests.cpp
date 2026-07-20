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
constexpr std::size_t linked_test_count = 342;
constexpr std::size_t test_shard_count = 8;
constexpr std::size_t evidence_command_count = 62;
constexpr std::size_t command_core_limit = 260;
constexpr std::size_t command_branch_limit = 12;
constexpr std::size_t noarg_main_count = 344;
constexpr std::size_t product_source_count = 316;
constexpr std::size_t root_cmake_line_limit = 150;
constexpr std::size_t cmake_module_line_limit = 500;

struct SourceSize {
    std::string path;
    std::size_t nonblank_lines = 0;
    bool has_main = false;
    bool explicit_main_return = false;
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

struct ManifestCase {
    std::string phase;
    std::string kind;
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
using SourcePaths = std::set<std::string>;
using TestRegistrations = std::vector<CMakeTestRegistration>;
using ManifestCases = std::vector<ManifestCase>;
using ProductSources = std::vector<std::string>;

bool is_blank(std::string_view line) {
    return std::all_of(line.begin(), line.end(), [](unsigned char character) { return std::isspace(character) != 0; });
}

std::size_t count_nonblank_text(std::string_view text) {
    std::size_t count = 0;
    std::istringstream input{std::string{text}};
    std::string line;
    while (std::getline(input, line)) {
        if (!is_blank(line)) {
            ++count;
        }
    }
    return count;
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

std::size_t count_physical_lines(const std::filesystem::path& path) {
    std::ifstream input{path};
    if (!input.is_open()) {
        throw std::runtime_error{"cannot read architecture source: " + path.generic_string()};
    }

    std::size_t count = 0;
    std::string line;
    while (std::getline(input, line)) {
        ++count;
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

std::vector<std::string> split_row(std::string_view row) {
    std::vector<std::string> columns;
    std::size_t begin = 0;
    while (begin <= row.size()) {
        const auto separator = row.find('|', begin);
        columns.emplace_back(
            row.substr(begin, separator == std::string_view::npos ? row.size() - begin : separator - begin));
        if (separator == std::string_view::npos) {
            break;
        }
        begin = separator + 1;
    }
    return columns;
}

bool is_safe_name(std::string_view name) {
    return !name.empty() && std::all_of(name.begin(), name.end(), [](unsigned char character) {
        return std::isalnum(character) != 0 || character == '_';
    });
}

ManifestCases parse_case_manifest(const SourcePaths& test_sources, std::string_view text) {
    if (text.find('\r') != std::string_view::npos) {
        throw std::runtime_error{"test case manifest must use LF line endings"};
    }

    ManifestCases cases;
    std::set<std::string> targets;
    std::set<std::string> test_names;
    std::istringstream input{std::string{text}};
    std::string row;
    while (std::getline(input, row)) {
        row = trim_copy(row);
        if (row.empty() || row.front() == '#') {
            continue;
        }

        auto columns = split_row(row);
        if (columns.size() != 5 ||
            std::any_of(columns.begin(), columns.end(), [](const auto& value) { return value.empty(); })) {
            throw std::runtime_error{"test case manifest row must have five nonempty columns: " + row};
        }

        ManifestCase test_case{std::move(columns[0]), std::move(columns[1]), std::move(columns[2]),
                               std::move(columns[3]), std::move(columns[4])};
        if (test_case.phase != "pre_smoke" && test_case.phase != "main") {
            throw std::runtime_error{"unknown test case manifest phase: " + test_case.phase};
        }
        if (test_case.kind != "linked" && test_case.kind != "source_dir") {
            throw std::runtime_error{"unknown test case manifest kind: " + test_case.kind};
        }
        if (!is_safe_name(test_case.target) || !is_safe_name(test_case.test_name)) {
            throw std::runtime_error{"test case manifest names must be safe identifiers"};
        }
        const auto source_path = std::filesystem::path{test_case.source};
        if (!test_case.source.starts_with("tests/") || source_path.extension() != ".cpp" ||
            test_case.source.find("../") != std::string::npos) {
            throw std::runtime_error{"invalid test case source path: " + test_case.source};
        }
        if (!test_sources.contains(test_case.source)) {
            throw std::runtime_error{"test case source is missing: " + test_case.source};
        }
        if (!targets.emplace(test_case.target).second) {
            throw std::runtime_error{"duplicate test case target: " + test_case.target};
        }
        if (!test_names.emplace(test_case.test_name).second) {
            throw std::runtime_error{"duplicate public CTest name: " + test_case.test_name};
        }
        cases.push_back(std::move(test_case));
    }
    return cases;
}

const std::set<std::string>& product_main_sources() {
    static const std::set<std::string> sources{
        "src/main.cpp", "src/server_main.cpp", "src/client_main.cpp", "src/benchmark_main.cpp", "src/osfs_main.cpp",
    };
    return sources;
}

ProductSources parse_product_manifest(const std::filesystem::path& source_root, std::string_view text) {
    if (text.find('\r') != std::string_view::npos) {
        throw std::runtime_error{"product source manifest must use LF line endings"};
    }

    ProductSources sources;
    std::set<std::string> seen;
    std::istringstream input{std::string{text}};
    std::string row;
    while (std::getline(input, row)) {
        const auto source = trim_copy(row);
        const auto path = std::filesystem::path{source};
        if (source.empty() || source != row || source.find('\\') != std::string::npos ||
            path.parent_path().generic_string() != "src" || path.extension() != ".cpp" ||
            !is_safe_name(path.stem().string())) {
            throw std::runtime_error{"invalid product source path: " + source};
        }
        if (product_main_sources().contains(source)) {
            throw std::runtime_error{"executable main entered product source manifest: " + source};
        }
        if (!std::filesystem::is_regular_file(source_root / path)) {
            throw std::runtime_error{"product source is missing: " + source};
        }
        if (!seen.emplace(source).second) {
            throw std::runtime_error{"duplicate product source: " + source};
        }
        sources.push_back(source);
    }
    return sources;
}

ProductSources discover_product_sources(const std::filesystem::path& source_root) {
    ProductSources sources;
    for (const auto& entry : std::filesystem::directory_iterator{source_root / "src"}) {
        if (!entry.is_regular_file() || entry.path().extension() != ".cpp") {
            continue;
        }
        const auto source = std::filesystem::relative(entry.path(), source_root).generic_string();
        if (!product_main_sources().contains(source)) {
            sources.push_back(source);
        }
    }
    std::sort(sources.begin(), sources.end());
    return sources;
}

void verify_product_manifest(const std::filesystem::path& source_root, const ProductSources& sources) {
    if (sources.size() != product_source_count) {
        throw std::runtime_error{"product source manifest census changed"};
    }
    if (sources.front() != "src/atomic_file_writer.cpp" || sources.back() != "src/wal.cpp") {
        throw std::runtime_error{"product source manifest boundary order changed"};
    }

    auto sorted_manifest = sources;
    std::sort(sorted_manifest.begin(), sorted_manifest.end());
    if (sorted_manifest != discover_product_sources(source_root)) {
        throw std::runtime_error{"product source manifest has an unregistered or stale source"};
    }
}

TestRegistrations manifest_registrations(const ManifestCases& cases) {
    TestRegistrations registrations;
    registrations.reserve(cases.size());
    for (const auto& test_case : cases) {
        registrations.push_back({test_case.target, test_case.test_name, test_case.source});
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

TargetMetrics verify_test_targets(const SourcePaths& test_sources, const TestRegistrations& registrations,
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
        if (!test_sources.contains(registration.source)) {
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

void verify_manifest_census(const ManifestCases& cases) {
    std::size_t pre_smoke = 0;
    std::size_t main = 0;
    std::size_t linked = 0;
    std::size_t source_dir = 0;
    bool main_started = false;
    for (const auto& test_case : cases) {
        if (test_case.phase == "pre_smoke") {
            if (main_started) {
                throw std::runtime_error{"pre_smoke test case appears after main phase"};
            }
            ++pre_smoke;
        } else {
            main_started = true;
            ++main;
        }
        if (test_case.kind == "linked") {
            ++linked;
        } else {
            ++source_dir;
        }
    }

    if (cases.size() != linked_test_count || pre_smoke != 8 || main != 334 || linked != 245 || source_dir != 97) {
        throw std::runtime_error{"test case manifest census changed"};
    }
    if (cases.front().test_name != "project_orientation_examples_tests" ||
        cases.back().test_name != "input_hardening_candidate_gate_receipt_tests") {
        throw std::runtime_error{"test case manifest boundary order changed"};
    }
}

std::vector<std::string> expected_ctest_order(const ManifestCases& cases) {
    std::vector<std::string> names{
        "benchmark_evidence_guard",
        "dependabot_config_tests",
        "project_orientation_doc_contract",
        "project_docs_honesty_contract",
        "minikv_track_final_evidence_contract",
        "test_architecture_contract",
        "receipts_consolidation_note_contract",
        "receipt_builder_census_contract",
        "elegance_name_census_contract",
    };
    for (const auto& test_case : cases) {
        if (test_case.phase == "pre_smoke") {
            names.push_back(test_case.test_name);
        }
    }
    names.insert(names.end(), {"osfs_cli_smoke", "cli_log_level_flag_smoke", "server_log_level_invalid_smoke"});
    for (const auto& test_case : cases) {
        if (test_case.phase == "main") {
            names.push_back(test_case.test_name);
        }
    }
    return names;
}

std::vector<std::string> read_ctest_order(const std::filesystem::path& path) {
    const auto text = read_text(path);
    std::vector<std::string> names;
    std::istringstream input{text};
    std::string line;
    while (std::getline(input, line)) {
        const auto trimmed = trim_copy(line);
        if (trimmed.empty() || !is_safe_name(trimmed)) {
            throw std::runtime_error{"malformed generated CTest name: " + trimmed};
        }
        names.push_back(trimmed);
    }
    return names;
}

void verify_ctest_order(const ManifestCases& cases) {
    const auto expected = expected_ctest_order(cases);
    const auto actual = read_ctest_order(std::filesystem::path{MINIKV_BINARY_DIR} / "minikv-ctest-names.txt");
    if (actual != expected) {
        throw std::runtime_error{"generated CTest name/order drift"};
    }
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

bool has_explicit_main_return(std::string_view source_text);

SourceSizes scan_test_sources(const std::filesystem::path& source_root) {
    SourceSizes sources;
    const auto test_root = source_root / "tests";
    for (const auto& entry : std::filesystem::recursive_directory_iterator{test_root}) {
        if (!entry.is_regular_file() || entry.path().extension() != ".cpp") {
            continue;
        }
        const auto source_text = read_text(entry.path());
        const bool has_main = count_occurrences(source_text, "int main()") != 0;
        sources.push_back(SourceSize{std::filesystem::relative(entry.path(), source_root).generic_string(),
                                     count_nonblank_text(source_text), has_main,
                                     !has_main || has_explicit_main_return(source_text)});
    }
    std::sort(sources.begin(), sources.end(),
              [](const SourceSize& left, const SourceSize& right) { return left.path < right.path; });
    return sources;
}

SourcePaths collect_source_paths(const SourceSizes& sources) {
    SourcePaths paths;
    for (const auto& source : sources) {
        paths.emplace(source.path);
    }
    return paths;
}

bool has_explicit_main_return(std::string_view source_text) {
    std::istringstream input{std::string{source_text}};
    std::string previous;
    std::string current;
    std::string line;
    while (std::getline(input, line)) {
        auto trimmed = trim_copy(line);
        if (!trimmed.empty()) {
            previous = std::move(current);
            current = std::move(trimmed);
        }
    }
    return previous == "return 0;" && current == "}";
}

void verify_explicit_main_returns(const SourceSizes& sources) {
    std::size_t observed = 0;
    for (const auto& source : sources) {
        if (!source.has_main) {
            continue;
        }
        ++observed;
        if (!source.explicit_main_return) {
            throw std::runtime_error{"no-argument test main must end with explicit return 0: " + source.path};
        }
    }
    if (observed != noarg_main_count) {
        throw std::runtime_error{"no-argument test main census changed without updating the link-shard contract"};
    }
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

void assert_target_failure(const SourcePaths& test_sources, const TestRegistrations& registrations,
                           const TargetBaseline& baseline, std::string_view expected) {
    try {
        (void)verify_test_targets(test_sources, registrations, baseline);
    } catch (const std::runtime_error& error) {
        assert(std::string_view{error.what()}.find(expected) != std::string_view::npos);
        return;
    }
    assert(false && "expected CMake test target contract failure");
}

void assert_manifest_failure(const SourcePaths& test_sources, std::string_view text, std::string_view expected) {
    try {
        (void)parse_case_manifest(test_sources, text);
    } catch (const std::runtime_error& error) {
        assert(std::string_view{error.what()}.find(expected) != std::string_view::npos);
        return;
    }
    assert(false && "expected test case manifest contract failure");
}

void assert_product_manifest_failure(const std::filesystem::path& source_root, std::string_view text,
                                     bool require_complete, std::string_view expected) {
    try {
        const auto sources = parse_product_manifest(source_root, text);
        if (require_complete) {
            verify_product_manifest(source_root, sources);
        }
    } catch (const std::runtime_error& error) {
        assert(std::string_view{error.what()}.find(expected) != std::string_view::npos);
        return;
    }
    assert(false && "expected product source manifest contract failure");
}

void verify_red_paths(const std::filesystem::path& source_root, const SourcePaths& test_sources) {
    assert_failure({{"tests/new_oversized.cpp", 1001}}, {}, "without baseline");
    assert_failure({{"tests/growing.cpp", 1002}}, {{"tests/growing.cpp", 1001}}, "must shrink");
    assert_failure({{"tests/repaid.cpp", 999}}, {{"tests/repaid.cpp", 1001}}, "stale");

    const std::string long_target(test_target_name_limit + 1, 't');
    assert_target_failure(test_sources, {{long_target, "synthetic_test", "tests/store_tests.cpp"}}, {},
                          "new long internal");
    assert_target_failure(test_sources, {}, {long_target}, "stale internal");
    const std::string risky_source(object_path_score_limit + 1 - test_target_name_limit, 's');
    assert_target_failure(test_sources, {{std::string(test_target_name_limit, 't'), "synthetic_test", risky_source}},
                          {}, "object path score");

    constexpr std::string_view valid = "pre_smoke|linked|target_a|test_a|tests/store_tests.cpp\n";
    assert_manifest_failure(test_sources, std::string{valid} + "\r", "LF line endings");
    assert_manifest_failure(test_sources, "late|linked|target_a|test_a|tests/store_tests.cpp\n", "unknown test case");
    assert_manifest_failure(test_sources, "main|dynamic|target_a|test_a|tests/store_tests.cpp\n", "unknown test case");
    assert_manifest_failure(test_sources, "main|linked|target_a|test_a\n", "five nonempty columns");
    assert_manifest_failure(test_sources, "main|linked|target_a|test_a|tests/missing_case.cpp\n", "source is missing");
    assert_manifest_failure(test_sources, std::string{valid} + "main|linked|target_a|test_b|tests/store_tests.cpp\n",
                            "duplicate test case target");
    assert_manifest_failure(test_sources, std::string{valid} + "main|linked|target_b|test_a|tests/store_tests.cpp\n",
                            "duplicate public CTest name");

    constexpr std::string_view product_source = "src/store.cpp\n";
    assert_product_manifest_failure(source_root, std::string{product_source} + "\r", false, "LF line endings");
    assert_product_manifest_failure(source_root, "src/store.c\n", false, "invalid product source path");
    assert_product_manifest_failure(source_root, "src/main.cpp\n", false, "executable main");
    assert_product_manifest_failure(source_root, "src/not_real.cpp\n", false, "source is missing");
    assert_product_manifest_failure(source_root, std::string{product_source} + std::string{product_source}, false,
                                    "duplicate product source");
    assert_product_manifest_failure(source_root, product_source, true, "census changed");
}

void require_fragment(std::string_view text, std::string_view fragment, std::string_view source) {
    if (text.find(fragment) == std::string_view::npos) {
        throw std::runtime_error{std::string{source} + " is missing test-link fragment: " + std::string{fragment}};
    }
}

void verify_link_shards(const std::filesystem::path& source_root, std::string_view cmake_text) {
    require_fragment(cmake_text, "option(MINIKV_BUNDLE_TESTS", "CMakeLists.txt");
    require_fragment(cmake_text, "set(MINIKV_TEST_SHARD_COUNT 8)", "CMakeLists.txt");
    require_fragment(cmake_text, "instrumented lanes require MINIKV_BUNDLE_TESTS=OFF", "CMakeLists.txt");
    require_fragment(cmake_text, "include(cmake/MinikvTestSuite.cmake)", "CMakeLists.txt");

    const auto helper_text = read_text(source_root / "cmake" / "MinikvTesting.cmake");
    require_fragment(helper_text, "function(minikv_read_test_manifest", "MinikvTesting.cmake");
    require_fragment(helper_text, "minikv_require_lf_file(\"${manifest_path}\" \"test case manifest\")",
                     "MinikvTesting.cmake");
    require_fragment(helper_text, "test case manifest census changed", "MinikvTesting.cmake");
    require_fragment(helper_text, "function(minikv_register_test_group", "MinikvTesting.cmake");
    require_fragment(helper_text, "string(SHA256 case_digest", "MinikvTesting.cmake");
    require_fragment(helper_text, "add_library(${target_name} OBJECT", "MinikvTesting.cmake");
    require_fragment(helper_text, "main=${entry_symbol}", "MinikvTesting.cmake");
    require_fragment(helper_text, "add_executable(${target_name}", "MinikvTesting.cmake legacy path");
    require_fragment(helper_text, "minikv_enable_test_assertions(${target_name})", "MinikvTesting.cmake");
    require_fragment(helper_text, "target_compile_options(${target_name} PRIVATE -UNDEBUG)", "MinikvTesting.cmake");
    require_fragment(helper_text, "minikv_test_shard_${shard_index}", "MinikvTesting.cmake");

    const auto suite_text = read_text(source_root / "cmake" / "MinikvTestSuite.cmake");
    require_fragment(suite_text, "minikv_register_test_group(\"${minikv_test_manifest}\" pre_smoke)",
                     "MinikvTestSuite.cmake");
    require_fragment(suite_text, "minikv_register_test_group(\"${minikv_test_manifest}\" main)",
                     "MinikvTestSuite.cmake");
    require_fragment(suite_text, "minikv_finalize_test_bundle()", "MinikvTestSuite.cmake");
    require_fragment(suite_text, "get_property(minikv_ctest_names GLOBAL PROPERTY MINIKV_CTEST_NAMES)",
                     "MinikvTestSuite.cmake");
    require_fragment(suite_text, "minikv-ctest-names.txt", "MinikvTestSuite.cmake");
    require_fragment(suite_text, "test_architecture_contract PROPERTIES TIMEOUT 300", "MinikvTestSuite.cmake");

    const auto runner_text = read_text(source_root / "cmake" / "minikv_test_runner.cpp.in");
    require_fragment(runner_text, "if (argc != 2)", "minikv_test_runner.cpp.in");
    require_fragment(runner_text, "return test.entry();", "minikv_test_runner.cpp.in");
    require_fragment(runner_text, "unknown bundled test case", "minikv_test_runner.cpp.in");
}

void verify_product_wiring(const std::filesystem::path& source_root, std::string_view cmake_text) {
    require_fragment(cmake_text, "include(cmake/MinikvSources.cmake)", "CMakeLists.txt");
    require_fragment(cmake_text, "minikv_read_product_sources(", "CMakeLists.txt");
    require_fragment(cmake_text, "add_library(minikv ${MINIKV_PRODUCT_SOURCES})", "CMakeLists.txt");

    const auto module_path = source_root / "cmake" / "MinikvSources.cmake";
    const auto module_text = read_text(module_path);
    require_fragment(module_text, "function(minikv_read_product_sources", "MinikvSources.cmake");
    require_fragment(module_text, "minikv_require_lf_file(\"${manifest_path}\" \"product source manifest\")",
                     "MinikvSources.cmake");
    require_fragment(module_text, "duplicate product source", "MinikvSources.cmake");
    require_fragment(module_text, "product source manifest has an unregistered or stale source", "MinikvSources.cmake");

    if (count_physical_lines(source_root / "CMakeLists.txt") > root_cmake_line_limit) {
        throw std::runtime_error{"root CMakeLists.txt exceeded its line ratchet"};
    }
    const auto manifest_module = read_text(source_root / "cmake" / "MinikvManifest.cmake");
    require_fragment(manifest_module, "string(REGEX MATCHALL \"..\" file_bytes", "MinikvManifest.cmake");
    require_fragment(manifest_module, "if(\"0d\" IN_LIST file_bytes)", "MinikvManifest.cmake");

    for (const auto* relative : {"cmake/MinikvManifest.cmake", "cmake/MinikvSources.cmake", "cmake/MinikvTesting.cmake",
                                 "cmake/MinikvTestSuite.cmake"}) {
        if (count_physical_lines(source_root / relative) > cmake_module_line_limit) {
            throw std::runtime_error{"CMake module exceeded its line budget: " + std::string{relative}};
        }
    }
}

void verify_evidence_dispatch(const std::filesystem::path& source_root) {
    const auto command_text = read_text(source_root / "src" / "command.cpp");
    const auto dispatch_text = read_text(source_root / "src" / "command_evidence_dispatch.cpp");
    const auto command_lines = count_nonblank_lines(source_root / "src" / "command.cpp");
    const auto branches = count_occurrences(command_text, "if (command ==");
    const auto entries = count_occurrences(dispatch_text, "EvidenceCommand{");

    if (command_lines > command_core_limit) {
        throw std::runtime_error{"command.cpp grew past the orchestration budget"};
    }
    if (branches > command_branch_limit) {
        throw std::runtime_error{"command.cpp evidence if-chain regrew"};
    }
    if (entries != evidence_command_count) {
        throw std::runtime_error{"evidence command table census changed"};
    }
    require_fragment(dispatch_text, "static_assert(commands.size() == 62)", "command evidence dispatch");
    require_fragment(dispatch_text, "static_assert(unique_names())", "command evidence dispatch");
}

} // namespace

int main() {
    const std::filesystem::path source_root{MINIKV_SOURCE_DIR};
    const auto baseline = read_baseline(source_root / "config" / "test-source-size-baseline.txt");
    const auto sources = scan_test_sources(source_root);
    const auto test_sources = collect_source_paths(sources);
    verify_source_sizes(sources, baseline);
    verify_explicit_main_returns(sources);
    const auto target_baseline = read_target_baseline(source_root / "config" / "test-target-name-baseline.txt");
    const auto cmake_text = read_text(source_root / "CMakeLists.txt");
    const auto suite_text = read_text(source_root / "cmake" / "MinikvTestSuite.cmake");
    const auto manifest_text = read_text(source_root / "config" / "test-cases.txt");
    const auto manifest_cases = parse_case_manifest(test_sources, manifest_text);
    verify_manifest_census(manifest_cases);
    const auto product_text = read_text(source_root / "config" / "product-sources.txt");
    const auto product_sources = parse_product_manifest(source_root, product_text);
    verify_product_manifest(source_root, product_sources);
    auto registrations = manifest_registrations(manifest_cases);
    const auto standalone = parse_test_registrations(suite_text);
    registrations.insert(registrations.end(), standalone.begin(), standalone.end());
    const auto target_metrics = verify_test_targets(test_sources, registrations, target_baseline);
    verify_link_shards(source_root, cmake_text);
    verify_product_wiring(source_root, cmake_text);
    verify_ctest_order(manifest_cases);
    verify_evidence_dispatch(source_root);
    verify_suite(source_root, "config/command-test-parts.txt", "tests/command_tests.cpp", "CommandFixture",
                 {{"assert(", 463}, {"assert_response_contains(", 2447}});
    verify_suite(source_root, "config/shard-test-parts.txt", "tests/shard_readiness_tests.cpp", "ShardFixture",
                 {{"assert(", 8}, {"assert_contains(", 3710}, {"assert_fixture_differs_from_each(", 2}},
                 {{"assert_shard_readiness_contract(fixture, fixture.current);", 1},
                  {"assert_shard_readiness_contract(fixture, result.response);", 1}});
    verify_red_paths(source_root, test_sources);

    std::cout << "test architecture: sources=" << sources.size() << " limit=" << test_source_limit
              << " oversized=" << baseline.size() << " cmake_tests=" << target_metrics.registrations
              << " long_targets=" << target_metrics.long_targets << " max_path_score=" << target_metrics.max_path_score
              << " manifest_cases=" << manifest_cases.size() << " linked_cases=" << linked_test_count
              << " shards=" << test_shard_count << " evidence_commands=" << evidence_command_count
              << " product_sources=" << product_sources.size()
              << " root_cmake_lines=" << count_physical_lines(source_root / "CMakeLists.txt")
              << " explicit_mains=" << noarg_main_count << '\n';
    return 0;
}
