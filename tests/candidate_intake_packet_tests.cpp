#include "minikv/shard_preview_candidate_intake_packet.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace intake = minikv::shard_preview_candidate_intake_packet;

    const auto json = intake::format_candidate_intake_packet_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-document-intake-packet.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEINTAKEPACKETJSON\"");
    assert_contains(json, "\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\"");
    assert_contains(json, "\"sourceNodeSubmissionPrecheckReleaseVersion\":\"Node v1411\"");
    assert_contains(json, "\"sourceNodeIntakeSlotCount\":10");
    assert_contains(json, "\"sourceNodeIntakeGuardCount\":10");
    assert_contains(json, "\"sourceNodeCoveredSubmissionCheckpointCount\":25");
    assert_contains(json, "\"sourceNodeCoveredSubmissionValidatorCount\":25");
    assert_contains(json, "\"sourceNodeIntakeGateCount\":35");
    assert_contains(json, "\"sourceCandidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_contains(json, "\"sourceCandidateSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v930.json\"");
    assert_contains(json, "\"sourceCandidateSubmissionPrecheckPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateSubmissionPrecheckComplete\":true");
    assert_contains(json, "\"candidateIntakePacketReleaseVersion\":\"v955\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v954\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v954.json\"");
    assert_contains(json, "\"publishedStageCount\":25");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"plannedIntakeSlotCount\":10");
    assert_contains(json, "\"completedIntakeSlotCount\":10");
    assert_contains(json, "\"plannedIntakeGuardCount\":10");
    assert_contains(json, "\"completedIntakeGuardCount\":10");
    assert_contains(json, "\"candidateIntakePacketOnly\":true");
    assert_contains(json, "\"candidateIntakePacketChainComplete\":true");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"additionalIntakeEchoCreated\":false");
    assert_contains(json, "\"candidateIntakePacketValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = intake::candidate_intake_packet_digest_marker();
    assert(digest.find("v955-route-preview-candidate-intake-packet-closeout-summary-25-of-25-stages") !=
           std::string::npos);
    assert(intake::candidate_intake_packet_status() ==
           "route-preview-candidate-intake-packet-closeout-summary-read-only");
    assert(intake::published_stage_count() == 25);
}
