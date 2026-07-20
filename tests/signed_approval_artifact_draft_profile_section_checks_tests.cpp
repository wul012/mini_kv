#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_checks.hpp"

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
    namespace checks = minikv::shard_preview_signed_approval_artifact_draft_profile_section_checks;

    const auto catalog = checks::signed_approval_artifact_draft_profile_section_checks();
    assert(catalog.size() == 25);
    assert(checks::planned_signed_approval_artifact_draft_profile_section_check_count() == 25);
    assert(catalog.front().check_code == std::string_view{"source-current-fixture-frozen"});
    assert(catalog.back().check_code == std::string_view{"no-approval-chain-created"});

    const auto json = checks::format_signed_approval_artifact_draft_profile_section_checks_json(25);
    assert_contains(json, "\"checkCode\":\"source-current-fixture-frozen\"");
    assert_contains(json, "\"checkCode\":\"node-renderer-split-plan-pinned\"");
    assert_contains(json, "\"checkCode\":\"signed-approval-draft-section-group-boundary\"");
    assert_contains(json, "\"checkCode\":\"false-flag-lock\"");
    assert_contains(json, "\"checkCode\":\"write-wal-boundary\"");
    assert_contains(json, "\"profileSectionOnly\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"signedApprovalCreated\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"materialIntakeOpened\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_signed_approval_artifact_draft_profile_section_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    return 0;
}
