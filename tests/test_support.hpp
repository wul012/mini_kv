#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace minikv::test_support {

inline void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

inline void assert_not_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) != std::string::npos) {
        std::cerr << "unexpected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) == std::string::npos);
}

template <typename Result>
inline void assert_response_contains(const Result& result, std::string_view expected) {
    assert_contains(result.response, expected);
}

inline void assert_response_contains(const std::string& response, std::string_view expected) {
    assert_contains(response, expected);
}

inline void assert_current_shard_readiness_status(const std::string& shard_json) {
    assert_contains(
        shard_json,
        "\"status\":\"f-folder-explanation-quality-closeout-single-large-version-read-only\"");
}

inline void assert_precheck_upstream_receipt_source_status(const std::string& shard_json) {
    assert_contains(
        shard_json,
        "\"sourcePrecheckUpstreamReceiptVerificationSplitStatus\":\"route-preview-precheck-upstream-receipt-verification-split-non-participation-clean-workspace-ci-closeout-read-only\"");
}

inline std::filesystem::path source_root() {
#ifdef MINIKV_SOURCE_DIR
    return std::filesystem::path{MINIKV_SOURCE_DIR};
#else
    return std::filesystem::current_path();
#endif
}

inline std::string read_fixture_text(const std::filesystem::path& relative_path) {
    const auto path = source_root() / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());

    std::ostringstream output;
    output << input.rdbuf();

    auto text = output.str();
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

inline std::filesystem::path release_fixture_path(std::string_view file_name) {
    return std::filesystem::path{"fixtures"} / "release" / std::string{file_name};
}

inline std::filesystem::path shard_readiness_fixture_path(int release_version) {
    return release_fixture_path("shard-readiness-v" + std::to_string(release_version) + ".json");
}

inline std::string read_shard_readiness_fixture(int release_version) {
    return read_fixture_text(shard_readiness_fixture_path(release_version));
}

} // namespace minikv::test_support
