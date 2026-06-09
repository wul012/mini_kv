#include "minikv/shard_preview_operator_value_supply_profile_section.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace profile = minikv::shard_preview_operator_value_supply_profile_section;

    const auto json = profile::format_operator_value_supply_profile_section_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-operator-value-supply-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeOperatorValueSupplyProfileSectionReleaseVersion\":\"Node v1581\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1135\"");
    assert_contains(json, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
    assert_contains(json, "\"sourceValueDraftReleaseVersion\":\"v585\"");
    assert_contains(json, "\"sourceFreshSiblingIntakeReleaseVersion\":\"Node v936\"");
    assert_contains(json, "\"sourceFreshSiblingIntakeCommandPresent\":false");
    assert_contains(json, "\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\"");
    assert_contains(json, "\"operatorValueSupplyProfileSectionReleaseVersion\":\"v1160\"");
    assert_contains(json, "\"operatorValueSupplyProfileSectionCount\":4");
    assert_contains(json, "\"nodeMigratedOperatorValueSupplyProfileSectionCount\":4");
    assert_contains(json, "\"operator-evidence-fresh-sibling-intake\"");
    assert_contains(json, "\"operator-evidence-value-supply-envelope\"");
    assert_contains(json, "\"operatorValueImported\":false");
    assert_contains(json, "\"freshSiblingEvidenceImported\":false");
    assert_contains(json, "\"syntheticEvidenceAccepted\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"operatorValueSupplyProfileSectionValidationPassed\":true");

    const auto digest = profile::operator_value_supply_profile_section_digest_marker();
    assert(digest.find("v1160-route-preview-operator-value-supply-profile-section-closeout-summary") !=
           std::string::npos);
    assert(profile::published_stage_count() == 25);
    assert(profile::operator_value_supply_profile_section_status() ==
           "route-preview-operator-value-supply-profile-section-closeout-summary-read-only");

    return 0;
}
