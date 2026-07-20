#include "minikv/shard_preview_candidate_material_submission_precheck_integrity.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace integrity = minikv::shard_preview_candidate_material_submission_precheck_integrity;

    const auto json = integrity::format_candidate_material_submission_precheck_integrity_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-preview-candidate-material-submission-precheck-fixture-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON\"");
    assert_contains(json, "\"sourceNodeMaterialSubmissionPrecheckReleaseVersion\":\"Node v1456\"");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckReleaseVersion\":\"v985\"");
    assert_contains(
        json,
        "\"sourceCandidateMaterialSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v985.json\"");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckChainComplete\":true");
    assert_contains(json, "\"sourceMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"sourceMaterialSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"sourceMaterialRequestItemCount\":25");
    assert_contains(json, "\"sourceAcceptanceCheckCount\":25");
    assert_contains(json, "\"sourceRequiredMaterialFieldCount\":20");
    assert_contains(json, "\"sourceSubmissionMaterialFieldCount\":20");
    assert_contains(json, "\"sourceMaterialSubmissionGateCount\":41");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1009\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1009.json\"");
    assert_contains(json, "\"publishedStageCount\":25");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"plannedIntegrityCheckCount\":25");
    assert_contains(json, "\"completedIntegrityCheckCount\":25");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckIntegrityOnly\":true");
    assert_contains(json, "\"integrityChainComplete\":true");
    assert_contains(json, "\"nodeStopConditionRespected\":true");
    assert_contains(json, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckIntegrityValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = integrity::candidate_material_submission_precheck_integrity_digest_marker();
    assert(
        digest.find(
            "v1010-route-preview-candidate-material-submission-precheck-integrity-closeout-summary-25-of-25-stages") !=
        std::string::npos);
    assert(integrity::candidate_material_submission_precheck_integrity_status() ==
           "route-preview-candidate-material-submission-precheck-integrity-closeout-summary-read-only");
    assert(integrity::published_stage_count() == 25);
    return 0;
}
