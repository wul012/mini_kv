#include "minikv/shard_preview_candidate_material_request_integrity.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace integrity = minikv::shard_preview_candidate_material_request_integrity;

    const auto json = integrity::format_candidate_material_request_integrity_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-material-request-fixture-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON\"");
    assert_contains(json, "\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\"");
    assert_contains(json, "\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\"");
    assert_contains(json, "\"sourceCandidateMaterialRequestCommand\":\"SHARDROUTECANDIDATEMATERIALREQUESTJSON\"");
    assert_contains(json, "\"sourceCandidateMaterialRequestReleaseVersion\":\"v965\"");
    assert_contains(json,
                    "\"sourceCandidateMaterialRequestFixturePath\":\"fixtures/release/shard-readiness-v965.json\"");
    assert_contains(json, "\"sourceCandidateMaterialRequestPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateMaterialRequestComplete\":true");
    assert_contains(json, "\"sourceMaterialRequestItemCount\":25");
    assert_contains(json, "\"sourceAcceptanceCheckCount\":25");
    assert_contains(json, "\"sourceRequestedMaterialFieldCount\":20");
    assert_contains(json, "\"sourceMaterialRequestGateCount\":40");
    assert_contains(json, "\"candidateMaterialRequestIntegrityReleaseVersion\":\"v975\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v974\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v974.json\"");
    assert_contains(json, "\"publishedStageCount\":10");
    assert_contains(json, "\"plannedStageCount\":10");
    assert_contains(json, "\"plannedIntegrityCheckCount\":15");
    assert_contains(json, "\"completedIntegrityCheckCount\":15");
    assert_contains(json, "\"candidateMaterialRequestIntegrityOnly\":true");
    assert_contains(json, "\"candidateMaterialRequestIntegrityChainComplete\":true");
    assert_contains(json, "\"nodeStopConditionRespected\":true");
    assert_contains(json, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(json, "\"materialRequestReopened\":false");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"candidateMaterialRequestIntegrityValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = integrity::candidate_material_request_integrity_digest_marker();
    assert(digest.find("v975-route-preview-candidate-material-request-integrity-closeout-summary-10-of-10-stages") !=
           std::string::npos);
    assert(integrity::candidate_material_request_integrity_status() ==
           "route-preview-candidate-material-request-integrity-closeout-summary-read-only");
    assert(integrity::published_stage_count() == 10);
    return 0;
}
