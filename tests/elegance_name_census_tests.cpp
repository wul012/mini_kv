#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#ifndef MINIKV_SOURCE_DIR
#error "MINIKV_SOURCE_DIR must be defined"
#endif

#ifndef MINIKV_NAME_FILE_MAX
#error "MINIKV_NAME_FILE_MAX must be defined"
#endif

#ifndef MINIKV_NAME_ID_MAX
#error "MINIKV_NAME_ID_MAX must be defined"
#endif

namespace {

namespace fs = std::filesystem;

constexpr std::size_t kNameBudget = 40;

struct Census {
    std::set<std::string> files;
    std::set<std::string> ids;
};

std::string read_text(const fs::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("cannot read " + path.string());
    }
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

bool is_header(const fs::path& path) {
    static const std::set<std::string> extensions = {".h", ".hh", ".hpp", ".hxx"};
    return extensions.contains(path.extension().string());
}

std::string without_includes(std::string_view input) {
    std::istringstream lines{std::string(input)};
    std::string output;
    std::string line;
    while (std::getline(lines, line)) {
        const auto first =
            std::find_if_not(line.begin(), line.end(), [](unsigned char ch) { return std::isspace(ch) != 0; });
        const auto offset = static_cast<std::size_t>(std::distance(line.begin(), first));
        if (first == line.end() || !std::string_view(line).substr(offset).starts_with("#include")) {
            output += line;
        }
        output.push_back('\n');
    }
    return output;
}

std::string strip_non_code(std::string_view input) {
    enum class Mode { code, line_comment, block_comment, quoted, character };
    Mode mode = Mode::code;
    bool escaped = false;
    std::string output(input.size(), ' ');

    for (std::size_t i = 0; i < input.size(); ++i) {
        const char ch = input[i];
        const char next = i + 1 < input.size() ? input[i + 1] : '\0';
        if (mode == Mode::line_comment) {
            if (ch == '\n') {
                mode = Mode::code;
                output[i] = ch;
            }
            continue;
        }
        if (mode == Mode::block_comment) {
            if (ch == '*' && next == '/') {
                mode = Mode::code;
                ++i;
            }
            continue;
        }
        if (mode == Mode::quoted || mode == Mode::character) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if ((mode == Mode::quoted && ch == '"') || (mode == Mode::character && ch == '\'')) {
                mode = Mode::code;
            }
            if (ch == '\n') {
                output[i] = ch;
            }
            continue;
        }
        if (ch == '/' && next == '/') {
            mode = Mode::line_comment;
            ++i;
            continue;
        }
        if (ch == '/' && next == '*') {
            mode = Mode::block_comment;
            ++i;
            continue;
        }
        if (ch == 'R' && next == '"') {
            const auto open = input.find('(', i + 2);
            if (open != std::string_view::npos && open - (i + 2) <= 16) {
                const std::string delimiter(input.substr(i + 2, open - (i + 2)));
                const std::string close = ")" + delimiter + "\"";
                const auto end = input.find(close, open + 1);
                if (end != std::string_view::npos) {
                    i = end + close.size() - 1;
                    continue;
                }
            }
        }
        if (ch == '"') {
            mode = Mode::quoted;
            continue;
        }
        if (ch == '\'') {
            mode = Mode::character;
            continue;
        }
        output[i] = ch;
    }
    return output;
}

void scan_names(const fs::path& root, const fs::path& path, Census& census) {
    std::error_code error;
    if (!fs::exists(path, error)) {
        throw std::runtime_error("missing census root " + path.string());
    }
    for (fs::recursive_directory_iterator it(path, error), end; it != end; it.increment(error)) {
        if (error) {
            throw std::runtime_error("cannot scan " + path.string() + ": " + error.message());
        }
        if (!it->is_regular_file()) {
            continue;
        }
        const auto stem = it->path().stem().string();
        if (stem.size() > kNameBudget) {
            census.files.insert(fs::relative(it->path(), root).generic_string());
        }
    }
}

void scan_ids(const fs::path& include_root, Census& census) {
    static const std::regex identifier("[A-Za-z_][A-Za-z0-9_]*");
    std::error_code error;
    for (fs::recursive_directory_iterator it(include_root, error), end; it != end; it.increment(error)) {
        if (error) {
            throw std::runtime_error("cannot scan headers: " + error.message());
        }
        if (!it->is_regular_file() || !is_header(it->path())) {
            continue;
        }
        const auto code = strip_non_code(without_includes(read_text(it->path())));
        for (std::sregex_iterator match(code.begin(), code.end(), identifier), last; match != last; ++match) {
            if (match->str().size() > kNameBudget) {
                census.ids.insert(match->str());
            }
        }
    }
}

Census take_census(const fs::path& root) {
    Census census;
    for (const auto* name : {"src", "include", "tests"}) {
        scan_names(root, root / name, census);
    }
    scan_ids(root / "include", census);
    return census;
}

Census read_baseline(const fs::path& path) {
    Census baseline;
    std::istringstream lines(read_text(path));
    std::string line;
    while (std::getline(lines, line)) {
        if (line.empty() || line.starts_with('#')) {
            continue;
        }
        if (line.starts_with("F|")) {
            baseline.files.insert(line.substr(2));
        } else if (line.starts_with("I|")) {
            baseline.ids.insert(line.substr(2));
        } else {
            throw std::runtime_error("invalid baseline row: " + line);
        }
    }
    return baseline;
}

void write_baseline(const fs::path& path, const Census& census) {
    fs::create_directories(path.parent_path());
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) {
        throw std::runtime_error("cannot write " + path.string());
    }
    output << "# mini-kv elegance name baseline v1\n"
           << "# budget=40; entries are shrink-only\n"
           << "# file_count=" << census.files.size() << "\n"
           << "# public_identifier_count=" << census.ids.size() << "\n";
    for (const auto& file : census.files) {
        output << "F|" << file << '\n';
    }
    for (const auto& id : census.ids) {
        output << "I|" << id << '\n';
    }
}

void print_set(std::ostream& output, std::string_view label, const std::set<std::string>& values) {
    for (const auto& value : values) {
        output << label << value << '\n';
    }
}

std::set<std::string> difference(const std::set<std::string>& left, const std::set<std::string>& right) {
    std::set<std::string> result;
    std::set_difference(left.begin(), left.end(), right.begin(), right.end(), std::inserter(result, result.end()));
    return result;
}

bool compare_group(std::string_view group, const std::set<std::string>& current, const std::set<std::string>& baseline,
                   std::size_t maximum) {
    bool valid = true;
    if (baseline.size() > maximum) {
        std::cerr << group << " baseline " << baseline.size() << " exceeds ratchet " << maximum << '\n';
        valid = false;
    }
    if (current.size() > maximum) {
        std::cerr << group << " census " << current.size() << " exceeds ratchet " << maximum << '\n';
        valid = false;
    }
    const auto added = difference(current, baseline);
    const auto removed = difference(baseline, current);
    if (!added.empty()) {
        std::cerr << group << " has new violations:\n";
        print_set(std::cerr, "  + ", added);
        valid = false;
    }
    if (!removed.empty()) {
        std::cerr << group << " baseline is stale; shrink it in this commit:\n";
        print_set(std::cerr, "  - ", removed);
        valid = false;
    }
    return valid;
}

} // namespace

int main(int argc, char** argv) {
    try {
        const fs::path root = MINIKV_SOURCE_DIR;
        const fs::path baseline_path = root / "config" / "elegance-name-baseline.txt";
        const Census current = take_census(root);
        if (argc == 2 && std::string_view(argv[1]) == "--write-baseline") {
            write_baseline(baseline_path, current);
            std::cout << "wrote files=" << current.files.size() << " public_ids=" << current.ids.size() << '\n';
            return 0;
        }
        if (argc == 2 && std::string_view(argv[1]) == "--list") {
            print_set(std::cout, "F|", current.files);
            print_set(std::cout, "I|", current.ids);
            return 0;
        }
        if (argc != 1) {
            std::cerr << "usage: elegance_name_census_tests [--list|--write-baseline]\n";
            return 2;
        }
        const Census baseline = read_baseline(baseline_path);
        const bool files_ok = compare_group("file names", current.files, baseline.files, MINIKV_NAME_FILE_MAX);
        const bool ids_ok = compare_group("public identifiers", current.ids, baseline.ids, MINIKV_NAME_ID_MAX);
        std::cout << "elegance census: files=" << current.files.size() << " public_ids=" << current.ids.size()
                  << " budget=" << kNameBudget << '\n';
        return files_ok && ids_ok ? 0 : 1;
    } catch (const std::exception& error) {
        std::cerr << "elegance census error: " << error.what() << '\n';
        return 2;
    }
}
