#include "minikv/shard_preview_candidate_request_package.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace package = minikv::shard_preview_candidate_request_package;

    const auto json = package::format_candidate_document_request_package_closeout_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-document-request-package-closeout.v1\"");
    assert_contains(json, "\"command\":"
                          "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUEST"
                          "PACKAGECLOSEOUTJSON\"");
    assert_contains(
        json, "\"sourceNodePlan\":\"docs/plans3/"
              "v1386-controlled-read-only-shard-preview-candidate-document-request-package-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_contains(
        json, "\"sourceComparedPackageEvidenceIntakeAuditFixturePath\":\"fixtures/release/shard-readiness-v880.json\"");
    assert_contains(json, "\"sourceComparedPackageEvidenceIntakeAuditPublishedStageCount\":20");
    assert_contains(json, "\"sourceComparedPackageEvidenceIntakeAuditChainComplete\":true");
    assert_contains(json, "\"candidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v894\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v894.json\"");
    assert_contains(json, "\"publishedStageCount\":15");
    assert_contains(json, "\"plannedStageCount\":15");
    assert_contains(json, "\"plannedRequestItemCount\":15");
    assert_contains(json, "\"plannedAcceptanceCheckCount\":15");
    assert_contains(json, "\"plannedRequestGateCount\":38");
    assert_contains(json, "\"completedRequestItemCount\":15");
    assert_contains(json, "\"completedAcceptanceCheckCount\":15");
    assert_contains(json, "\"completedRequestGateCount\":38");
    assert_contains(json, "\"requestedCandidateFieldCount\":20");
    assert_contains(json, "\"candidateDocumentRequestPackageDeclared\":true");
    assert_contains(json, "\"candidateDocumentRequestPackageCloseoutOnly\":true");
    assert_contains(json, "\"requestPackageReadyForFutureRealDocumentIntake\":true");
    assert_contains(json, "\"realCandidateDocumentPresent\":false");
    assert_contains(json, "\"candidateDocumentCreated\":false");
    assert_contains(json, "\"candidateDocumentImported\":false");
    assert_contains(json, "\"syntheticCandidateDocumentAccepted\":false");
    assert_contains(json, "\"stagedCandidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"candidateDocumentAccepted\":false");
    assert_contains(json, "\"candidateApprovalGrantCaptured\":false");
    assert_contains(json, "\"candidateSignedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"requestItems\":[");
    assert_contains(json, "\"acceptanceChecks\":[");
    assert_contains(json, "\"requestGates\":[");
    assert_contains(json, "\"candidateDocumentRequestPackageValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = package::candidate_document_request_package_digest_marker();
    assert(
        digest.find(
            "v895-route-preview-candidate-document-request-package-archive-closeout-final-summary-15-of-15-stages") !=
        std::string::npos);
    assert(package::candidate_document_request_package_status() ==
           "route-preview-candidate-document-request-package-archive-closeout-final-summary-read-only");
    assert(package::published_stage_count() == 15);
    return 0;
}
