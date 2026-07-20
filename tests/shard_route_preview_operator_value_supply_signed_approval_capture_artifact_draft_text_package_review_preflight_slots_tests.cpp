#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace criteria = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots;

    const auto records = criteria::signed_approval_capture_artifact_draft_text_package_review_preflight_slots();
    assert(records.size() == 25);
    assert(records.front().slot_code == std::string{"package_manifest_criterion"});
    assert(records.back().slot_code == std::string{"closeout"});
    assert(criteria::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_slot_count() == 25);

    const auto first =
        criteria::format_signed_approval_capture_artifact_draft_text_package_review_preflight_slots_json(1);
    assert_contains(first, "\"criterionCode\":\"package_manifest_criterion\"");
    assert_contains(first, "\"draftTextPackageParsed\":false");
    assert_contains(first, "\"draftTextPackageAccepted\":false");
    assert(first.find("\"criterionCode\":\"instruction_preflight_digest_criterion\"") == std::string::npos);

    const auto all =
        criteria::format_signed_approval_capture_artifact_draft_text_package_review_preflight_slots_json(99);
    assert_contains(all, "\"criterionCode\":\"closeout\"");
    assert_contains(all, "\"acceptsSignatureMaterial\":false");
    assert_contains(all, "\"importsOperatorValue\":false");
    return 0;
}