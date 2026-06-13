#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

std::string read_file_text(const std::filesystem::path& relative_path) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());

    std::ostringstream output;
    output << input.rdbuf();
    return output.str();
}

void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

void assert_not_contains(const std::string& text, std::string_view unexpected) {
    if (text.find(std::string{unexpected}) != std::string::npos) {
        std::cerr << "unexpected text present: " << unexpected << '\n';
    }
    assert(text.find(std::string{unexpected}) == std::string::npos);
}

} // namespace

int main() {
    const auto dependabot = read_file_text(std::filesystem::path{".github"} / "dependabot.yml");
    const auto ci_workflow = read_file_text(std::filesystem::path{".github"} / "workflows" / "ci.yml");

    assert_contains(dependabot, "version: 2");
    assert_contains(dependabot, "package-ecosystem: \"github-actions\"");
    assert_contains(dependabot, "directory: \"/\"");
    assert_contains(dependabot, "interval: \"weekly\"");
    assert_contains(dependabot, "timezone: \"Asia/Shanghai\"");
    assert_contains(dependabot, "open-pull-requests-limit: 5");
    assert_contains(dependabot, "github-actions-minor-patch:");
    assert_contains(dependabot, "update-types:");
    assert_contains(dependabot, "- \"minor\"");
    assert_contains(dependabot, "- \"patch\"");
    assert_contains(dependabot, "- \"version-update:semver-major\"");
    assert_contains(dependabot, "labels:");
    assert_contains(dependabot, "- \"dependencies\"");
    assert_contains(dependabot, "- \"github-actions\"");

    assert_not_contains(dependabot, "package-ecosystem: \"npm\"");
    assert_not_contains(dependabot, "package-ecosystem: \"maven\"");
    assert_not_contains(dependabot, "package-ecosystem: \"pip\"");
    assert_not_contains(dependabot, "package-ecosystem: \"cargo\"");
    assert_not_contains(dependabot, "package-ecosystem: \"docker\"");

    assert_contains(ci_workflow, "uses: actions/checkout@v6.0.3");
    assert_contains(ci_workflow, "uses: actions/upload-artifact@v7.0.1");
    assert_not_contains(ci_workflow, "uses: actions/checkout@v4");
    assert_not_contains(ci_workflow, "uses: actions/upload-artifact@v4");
    assert_contains(ci_workflow, "cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug");
    assert_contains(ci_workflow, "timeout-minutes: 30");
    assert_contains(ci_workflow, "ctest --test-dir build -C Debug --output-on-failure --timeout 120 --progress");
    assert_contains(ci_workflow, "name: archive inventory");
    assert_contains(ci_workflow, "python3 scripts/archive_inventory.py --budget-mib 8 --github-warning");
    assert_contains(ci_workflow, "name: ubuntu sanitizer");
    assert_not_contains(ci_workflow, "continue-on-error: true");
    assert_contains(ci_workflow, "cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DMINIKV_SANITIZE=ON");
    assert_contains(ci_workflow, "cmake --build build-sanitize --config Debug --parallel 4 --verbose");
    assert_contains(ci_workflow,
                    "ctest --test-dir build-sanitize -C Debug --output-on-failure --timeout 120 --progress");
    assert_contains(ci_workflow, "name: ubuntu coverage");
    assert_contains(ci_workflow, "sudo apt-get install -y gcovr");
    assert_contains(ci_workflow, "cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DMINIKV_COVERAGE=ON");
    assert_contains(ci_workflow, "cmake --build build-coverage --config Debug --parallel 4 --verbose");
    assert_contains(ci_workflow,
                    "ctest --test-dir build-coverage -C Debug --output-on-failure --timeout 120 --progress");
    assert_contains(
        ci_workflow,
        "src/"
        "(store|command|command_catalog|command_contracts|command_dispatch|command_expiry_ops|command_"
        "introspection_ops|command_parse_helpers|command_persistence_ops|command_response_formatters|command_"
        "smoke_formatters|command_string_ops|command_wal_gate|wal|snapshot|resp)\\\\.cpp");
    assert_contains(ci_workflow, "--gcov-ignore-parse-errors");
    assert_contains(ci_workflow, "negative_hits.warn_once_per_file");
    assert_contains(ci_workflow, "--fail-under-line 88");
    assert_contains(ci_workflow, "name: mini-kv-core-coverage");

    return 0;
}
