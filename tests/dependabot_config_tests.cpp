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

    assert_contains(ci_workflow, "uses: actions/checkout@v4");
    assert_contains(ci_workflow, "cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug");
    assert_contains(ci_workflow, "timeout-minutes: 30");
    assert_contains(ci_workflow, "ctest --test-dir build -C Debug --output-on-failure --timeout 120 --progress");
    assert_contains(ci_workflow, "name: ubuntu sanitizer");
    assert_not_contains(ci_workflow, "continue-on-error: true");
    assert_contains(ci_workflow, "cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DMINIKV_SANITIZE=ON");
    assert_contains(ci_workflow, "cmake --build build-sanitize --config Debug --parallel 4 --verbose");
    assert_contains(ci_workflow,
                    "ctest --test-dir build-sanitize -C Debug --output-on-failure --timeout 120 --progress");

    return 0;
}
