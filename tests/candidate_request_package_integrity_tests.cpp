#include "minikv/shard_preview_candidate_request_package_integrity.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace integrity = minikv::shard_preview_candidate_request_package_integrity;

    const auto json = integrity::format_candidate_request_package_integrity_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-request-package-fixture-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v1386-controlled-read-only-shard-preview-candidate-document-request-package-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageFixturePath\":\"fixtures/release/shard-readiness-v895.json\"");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackagePublishedStageCount\":15");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageChainComplete\":true");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageRequestItemCount\":15");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageAcceptanceCheckCount\":15");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageGateCount\":38");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageRequestedFieldCount\":20");
    assert_contains(json, "\"candidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v919\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v919.json\"");
    assert_contains(json, "\"publishedStageCount\":25");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"plannedIntegrityCheckCount\":25");
    assert_contains(json, "\"completedIntegrityCheckCount\":25");
    assert_contains(json, "\"candidateRequestPackageIntegrityOnly\":true");
    assert_contains(json, "\"additionalRequestEchoCreated\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"realCandidateDocumentRead\":false");
    assert_contains(json, "\"syntheticCandidateDocumentAccepted\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"candidateRequestPackageIntegrityValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = integrity::candidate_request_package_integrity_digest_marker();
    assert(digest.find("v920-route-preview-candidate-request-package-integrity-closeout-summary-25-of-25-stages") !=
           std::string::npos);
    assert(integrity::candidate_request_package_integrity_status() ==
           "route-preview-candidate-request-package-integrity-closeout-summary-read-only");
    assert(integrity::published_stage_count() == 25);
}
