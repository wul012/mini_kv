#include "minikv/shard_preview_candidate_material_submission_precheck.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace precheck = minikv::shard_preview_candidate_material_submission_precheck;

    const auto json = precheck::format_candidate_material_submission_precheck_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-document-material-submission-precheck.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON\"");
    assert_contains(json, "\"sourceNodeMaterialSubmissionPrecheckReleaseVersion\":\"Node v1456\"");
    assert_contains(json, "\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\"");
    assert_contains(json, "\"sourceNodeMaterialSubmissionCheckpointCount\":10");
    assert_contains(json, "\"sourceNodeMaterialSubmissionValidatorCount\":10");
    assert_contains(json, "\"sourceNodeMaterialRequestItemCount\":25");
    assert_contains(json, "\"sourceNodeMaterialAcceptanceCheckCount\":25");
    assert_contains(json, "\"requiredMaterialFieldCount\":20");
    assert_contains(json, "\"submissionMaterialFieldCount\":20");
    assert_contains(json, "\"materialSubmissionGateCount\":41");
    assert_contains(json, "\"sourceCandidateMaterialRequestReleaseVersion\":\"v965\"");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityReleaseVersion\":\"v975\"");
    assert_contains(
        json, "\"sourceCandidateMaterialRequestIntegrityFixturePath\":\"fixtures/release/shard-readiness-v975.json\"");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityComplete\":true");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckReleaseVersion\":\"v985\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v984\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v984.json\"");
    assert_contains(json, "\"publishedStageCount\":10");
    assert_contains(json, "\"plannedStageCount\":10");
    assert_contains(json, "\"plannedMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"completedMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"plannedMaterialSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"completedMaterialSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckOnly\":true");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckChainComplete\":true");
    assert_contains(json, "\"nodeStopConditionRespected\":true");
    assert_contains(json, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(json, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = precheck::candidate_material_submission_precheck_digest_marker();
    assert(
        digest.find(
            "v985-route-preview-candidate-material-submission-precheck-closeout-archive-checkpoint-10-of-10-stages") !=
        std::string::npos);
    assert(precheck::candidate_material_submission_precheck_status() ==
           "route-preview-candidate-material-submission-precheck-closeout-archive-checkpoint-read-only");
    assert(precheck::published_stage_count() == 10);
    return 0;
}
