#include "minikv/shard_preview_operator_value_supply_profile_section_integrity.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace integrity = minikv::shard_preview_operator_value_supply_profile_section_integrity;

    const auto json = integrity::format_operator_value_supply_profile_section_integrity_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-operator-value-supply-profile-section-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\"");
    assert_contains(json, "\"sourceNodeProductionLiveProbeReadOnlyWindowReadinessPacketReleaseVersion\":\"Node v1606\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceProfileSectionCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceProfileSectionReleaseVersion\":\"v1160\"");
    assert_contains(json, "\"sourceProfileSectionPublishedStageCount\":25");
    assert_contains(json, "\"sourceProfileSectionComplete\":true");
    assert_contains(json, "\"sourceProfileSectionDigestPinned\":true");
    assert_contains(json, "\"operatorValueSupplyProfileSectionIntegrityReleaseVersion\":\"v1170\"");
    assert_contains(json, "\"plannedOperatorValueSupplyProfileSectionIntegrityCheckCount\":10");
    assert_contains(json, "\"operatorValueSupplyProfileSectionIntegrityOnly\":true");
    assert_contains(json, "\"operatorValueSupplyProfileSectionCount\":4");
    assert_contains(json, "\"nodeMigratedOperatorValueSupplyProfileSectionCount\":4");
    assert_contains(json, "\"Node-v936-fresh-sibling-intake-plan\"");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"operatorValueImported\":false");
    assert_contains(json, "\"freshSiblingEvidenceImported\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"syntheticEvidenceAccepted\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"rawEndpointStored\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"operatorValueSupplyProfileSectionIntegrityValidationPassed\":true");

    const auto digest = integrity::operator_value_supply_profile_section_integrity_digest_marker();
    assert(digest.find("v1170-route-preview-operator-value-supply-profile-section-integrity-closeout-summary") !=
           std::string::npos);
    assert(integrity::published_stage_count() == 10);
    assert(integrity::operator_value_supply_profile_section_integrity_status() ==
           "route-preview-operator-value-supply-profile-section-integrity-closeout-summary-read-only");

    return 0;
}
