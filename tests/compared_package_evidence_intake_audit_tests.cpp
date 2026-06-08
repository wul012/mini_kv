#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace audit = minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit;

    const auto json = audit::format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-audit.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\"");
    assert_contains(json, "\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_contains(json, "\"sourceDraftTextPackageComparisonCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v860.json\"");
    assert_contains(json, "\"sourceDraftTextPackageComparisonCloseoutAuditPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftTextPackageComparisonCloseoutAuditChainComplete\":true");
    assert_contains(json, "\"draftTextPackageComparedPackageEvidenceIntakeAuditStage\":\"route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-approval-grant-separation-guard\"");
    assert_contains(json, "\"draftTextPackageComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v878\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v877\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v877.json\"");
    assert_contains(json, "\"publishedStageCount\":18");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(json, "\"plannedEvidenceSlotCount\":10");
    assert_contains(json, "\"plannedIntakeGuardCount\":10");
    assert_contains(json, "\"completedEvidenceSlotCount\":9");
    assert_contains(json, "\"completedIntakeGuardCount\":9");
    assert_contains(json, "\"comparedPackageEvidenceIntakeAuditDeclared\":true");
    assert_contains(json, "\"comparedPackageEvidenceIntakeAuditOnly\":true");
    assert_contains(json, "\"comparedPackageEvidencePresent\":false");
    assert_contains(json, "\"comparedPackageEvidenceFabricated\":false");
    assert_contains(json, "\"syntheticComparedPackageEvidenceAccepted\":false");
    assert_contains(json, "\"manualComparedPackageParsed\":false");
    assert_contains(json, "\"comparedPackageAccepted\":false");
    assert_contains(json, "\"detachedSignaturePayloadParsed\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"slotCode\":\"source_acceptance_precheck_evidence\"");
    assert_contains(json, "\"comparedPackageEvidenceIntakeAuditValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = audit::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_digest_marker();
    assert(digest.find("v878-route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-compared-package-evidence-intake-approval-grant-separation-guard") != std::string::npos);
    assert(audit::published_stage_count() == 18);
}
