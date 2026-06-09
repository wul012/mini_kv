#include "minikv/shard_preview_candidate_submission_precheck.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace precheck = minikv::shard_preview_candidate_submission_precheck;

    const auto json = precheck::format_candidate_submission_precheck_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-document-submission-precheck.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v1411-controlled-read-only-shard-preview-candidate-document-submission-precheck-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceNodeSubmissionPrecheckReleaseVersion\":\"Node v1411\"");
    assert_contains(json, "\"sourceNodeSubmissionCheckpointCount\":25");
    assert_contains(json, "\"sourceNodeSubmissionValidatorCount\":25");
    assert_contains(json, "\"sourceNodeSubmissionGateCount\":40");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(json, "\"sourceCandidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_contains(json, "\"sourceCandidateRequestPackageIntegrityFixturePath\":\"fixtures/release/shard-readiness-v920.json\"");
    assert_contains(json, "\"sourceCandidateRequestPackageIntegrityPublishedStageCount\":25");
    assert_contains(json, "\"sourceCandidateRequestPackageIntegrityComplete\":true");
    assert_contains(json, "\"requestedCandidateFieldCount\":20");
    assert_contains(json, "\"submissionGateCount\":40");
    assert_contains(json, "\"candidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v929\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v929.json\"");
    assert_contains(json, "\"publishedStageCount\":10");
    assert_contains(json, "\"plannedStageCount\":10");
    assert_contains(json, "\"plannedSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"completedSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"plannedSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"completedSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"candidateSubmissionPrecheckOnly\":true");
    assert_contains(json, "\"candidateSubmissionPrecheckChainComplete\":true");
    assert_contains(json, "\"additionalRequestEchoCreated\":false");
    assert_contains(json, "\"additionalSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(json, "\"syntheticCandidateDocumentAccepted\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"candidateSubmissionPrecheckValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = precheck::candidate_submission_precheck_digest_marker();
    assert(digest.find("v930-route-preview-candidate-submission-precheck-closeout-summary-10-of-10-stages") !=
           std::string::npos);
    assert(precheck::candidate_submission_precheck_status() ==
           "route-preview-candidate-submission-precheck-closeout-summary-read-only");
    assert(precheck::published_stage_count() == 10);
}
