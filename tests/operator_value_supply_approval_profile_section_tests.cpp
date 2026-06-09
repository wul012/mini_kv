#include "minikv/shard_preview_operator_value_supply_approval_profile_section.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace profile = minikv::shard_preview_operator_value_supply_approval_profile_section;

    const auto json = profile::format_operator_value_supply_approval_profile_section_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-preview-operator-value-supply-approval-profile-section-renderer-"
                    "split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeOperatorValueSupplyApprovalProfileSectionReleaseVersion\":\"Node v1556\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1110\"");
    assert_contains(json, "\"sourceApprovalPacketDraftReleaseVersion\":\"v660\"");
    assert_contains(json, "\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\"");
    assert_contains(json, "\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\"");
    assert_contains(json, "\"sourceSignedApprovalCaptureArtifactPreflightReleaseVersion\":\"v735\"");
    assert_contains(json, "\"operatorValueSupplyApprovalProfileSectionReleaseVersion\":\"v1135\"");
    assert_contains(json, "\"operatorValueSupplyApprovalProfileSectionCount\":5");
    assert_contains(json, "\"nodeMigratedOperatorValueSupplyApprovalProfileSectionCount\":5");
    assert_contains(json, "\"approval-packet-review\"");
    assert_contains(json, "\"signed-approval-capture-artifact-preflight\"");
    assert_contains(json, "\"approvalMaterialImported\":false");
    assert_contains(json, "\"signedApprovalCaptured\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"operatorValueSupplyApprovalProfileSectionValidationPassed\":true");

    const auto digest = profile::operator_value_supply_approval_profile_section_digest_marker();
    assert(digest.find("v1135-route-preview-operator-value-supply-approval-profile-section-closeout-summary") !=
           std::string::npos);
    assert(profile::published_stage_count() == 25);
    assert(profile::operator_value_supply_approval_profile_section_status() ==
           "route-preview-operator-value-supply-approval-profile-section-closeout-summary-read-only");

    return 0;
}
