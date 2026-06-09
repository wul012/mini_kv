#include "minikv/shard_preview_candidate_profile_section_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace checks = minikv::shard_preview_candidate_profile_section_checks;

    const auto catalog = checks::candidate_profile_section_checks();
    assert(catalog.size() == 25);
    assert(checks::planned_candidate_profile_section_check_count() == 25);
    assert(catalog.front().check_code == std::string_view{"source-integrity-fixture-frozen"});
    assert(catalog.back().check_code == std::string_view{"no-document-intake-opened"});

    const auto json = checks::format_candidate_profile_section_checks_json(25);
    assert_contains(json, "\"checkCode\":\"source-integrity-fixture-frozen\"");
    assert_contains(json, "\"checkCode\":\"node-renderer-split-plan-pinned\"");
    assert_contains(json, "\"checkCode\":\"candidate-section-group-boundary\"");
    assert_contains(json, "\"checkCode\":\"route-markdown-parity\"");
    assert_contains(json, "\"checkCode\":\"write-wal-boundary\"");
    assert_contains(json, "\"profileSectionOnly\":true");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_candidate_profile_section_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}
