#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks;

    const auto records = checks::signed_approval_artifact_draft_text_package_profile_section_checks();
    assert(checks::planned_signed_approval_artifact_draft_text_package_profile_section_check_count() == 25);
    assert(records.size() == 25);
    assert(records.front().check_code == std::string{"source-current-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-text-package-chain-created"});

    const auto json = checks::format_signed_approval_artifact_draft_text_package_profile_section_checks_json(25);
    assert_contains(json, "\"checkCode\":\"text-package-aggregator-boundary\"");
    assert_contains(json, "\"checkCode\":\"compared-evidence-candidate-intake-section-carried\"");
    assert_contains(json, "\"textPackageProfileSectionOnly\":true");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"textPackageMaterialImported\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_signed_approval_artifact_draft_text_package_profile_section_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
