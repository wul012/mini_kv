#include "minikv/shard_preview_candidate_material_request.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace material = minikv::shard_preview_candidate_material_request;

    const auto json = material::format_candidate_material_request_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-document-material-request.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEMATERIALREQUESTJSON\"");
    assert_contains(json, "\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\"");
    assert_contains(json, "\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\"");
    assert_contains(json, "\"sourceNodeMaterialRequestItemCount\":25");
    assert_contains(json, "\"sourceNodeAcceptanceCheckCount\":25");
    assert_contains(json, "\"sourceNodeIntakeSlotCount\":10");
    assert_contains(json, "\"sourceNodeIntakeGuardCount\":10");
    assert_contains(json, "\"sourceNodeRequestedMaterialFieldCount\":20");
    assert_contains(json, "\"sourceNodeMaterialRequestGateCount\":40");
    assert_contains(json, "\"sourceCandidateIntakePacketReleaseVersion\":\"v955\"");
    assert_contains(json, "\"sourceCandidateIntakePacketFixturePath\":\"fixtures/release/shard-readiness-v955.json\"");
    assert_contains(json, "\"sourceCandidateIntakePacketPublishedStageCount\":25");
    assert_contains(json, "\"sourceCandidateIntakePacketComplete\":true");
    assert_contains(json, "\"candidateMaterialRequestReleaseVersion\":\"v965\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v964\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v964.json\"");
    assert_contains(json, "\"publishedStageCount\":10");
    assert_contains(json, "\"plannedStageCount\":10");
    assert_contains(json, "\"plannedMaterialRequestItemCount\":25");
    assert_contains(json, "\"completedMaterialRequestItemCount\":25");
    assert_contains(json, "\"plannedAcceptanceCheckCount\":25");
    assert_contains(json, "\"completedAcceptanceCheckCount\":25");
    assert_contains(json, "\"candidateMaterialRequestOnly\":true");
    assert_contains(json, "\"candidateMaterialRequestChainComplete\":true");
    assert_contains(json, "\"externalReviewedMaterialRequested\":true");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(json, "\"materialRequestSatisfied\":false");
    assert_contains(json, "\"realMaterialAccepted\":false");
    assert_contains(json, "\"syntheticMaterialAccepted\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(json, "\"candidateMaterialRequestValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = material::candidate_material_request_digest_marker();
    assert(digest.find("v965-route-preview-candidate-material-request-closeout-summary-10-of-10-stages") !=
           std::string::npos);
    assert(material::candidate_material_request_status() ==
           "route-preview-candidate-material-request-closeout-summary-read-only");
    assert(material::published_stage_count() == 10);
    return 0;
}
