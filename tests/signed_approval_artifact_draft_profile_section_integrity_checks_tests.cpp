#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks.hpp"

#include <cassert>
#include <stdexcept>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace checks = minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks;

    const auto catalog = checks::signed_approval_artifact_draft_profile_section_integrity_checks();
    assert(catalog.size() == 25);
    assert(checks::planned_signed_approval_artifact_draft_profile_section_integrity_check_count() == 25);
    assert(catalog.front().sequence == 1);
    assert(catalog.back().sequence == 25);

    const auto json = checks::format_signed_approval_artifact_draft_profile_section_integrity_checks_json(25);
    assert_contains(json, "\"checkCode\":\"source-profile-section-fixture-frozen\"");
    assert_contains(json, "\"checkCode\":\"signed-approval-false-lock\"");
    assert_contains(json, "\"checkCode\":\"execution-lock\"");
    assert_contains(json, "\"checkCode\":\"closeout-no-chain-created\"");
    assert_contains(json, "\"sourceFixtureFrozen\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"signedApprovalCreated\":false");
    assert_contains(json, "\"materialIntakeOpened\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_signed_approval_artifact_draft_profile_section_integrity_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}
