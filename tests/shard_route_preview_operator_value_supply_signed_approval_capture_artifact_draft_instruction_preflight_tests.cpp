#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace preflight =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight;

    const auto json = preflight::format_signed_approval_capture_artifact_draft_instruction_preflight_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-instruction-preflight.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\"");
    assert_contains(json,
                    "\"sourceDraftAuthoringReadinessCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\"");
    assert_contains(json, "\"sourceDraftAuthoringReadinessReleaseVersion\":\"v760\"");
    assert_contains(json, "\"sourceDraftAuthoringReadinessFixturePath\":\"fixtures/release/shard-readiness-v760.json\"");
    assert_contains(json, "\"sourceDraftAuthoringReadinessPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftAuthoringReadinessChainComplete\":true");
    assert_contains(json,
                    "\"instructionPreflightStage\":\"route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-template-digest-slot\"");
    assert_contains(json, "\"instructionPreflightReleaseVersion\":\"v763\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v762\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v762.json\"");
    assert_contains(json, "\"publishedStageCount\":3");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"instructionSlotCount\":25");
    assert_contains(json, "\"instructionGuardCount\":25");
    assert_contains(json, "\"instructionProfileCount\":25");
    assert_contains(json, "\"instructionCatalogsAligned\":true");
    assert_contains(json, "\"draftInstructionSlotPreparedCount\":3");
    assert_contains(json, "\"materializedInstructionTextCount\":0");
    assert_contains(json, "\"signedApprovalCaptureArtifactDraftInstructionPreflightDeclared\":true");
    assert_contains(json, "\"signedApprovalCaptureArtifactDraftInstructionPreflightOnly\":true");
    assert_contains(json, "\"draftInstructionPreflightOnly\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftPresent\":false");
    assert_contains(json, "\"signedApprovalArtifactDraftMaterialized\":false");
    assert_contains(json, "\"signedApprovalArtifactDraftTextGenerated\":false");
    assert_contains(json, "\"draftInstructionTextGenerated\":false");
    assert_contains(json, "\"draftInstructionTextMaterialized\":false");
    assert_contains(json, "\"detachedSignaturePayloadPresent\":false");
    assert_contains(json, "\"approvalGrantPresent\":false");
    assert_contains(json, "\"approvalGrantEmitted\":false");
    assert_contains(json, "\"submittedOperatorValueCount\":0");
    assert_contains(json, "\"importedEvidenceValueCount\":0");
    assert_contains(json, "\"readyForDraftInstructionReview\":true");
    assert_contains(json, "\"readyForSignedApprovalArtifactDraft\":false");
    assert_contains(json, "\"runtimePayloadAccepted\":false");
    assert_contains(json, "\"instructionPreflightSlotHelperApplied\":true");
    assert_contains(json, "\"instructionPreflightGuardHelperApplied\":true");
    assert_contains(json, "\"instructionPreflightProfileHelperApplied\":true");
    assert_contains(json, "\"instructionPreflightValidationHelperApplied\":true");
    assert_contains(json, "\"instructionSlots\":[{\"sequence\":1,\"slotCode\":\"request_manifest_slot\"");
    assert_contains(json, "\"instructionGuards\":[{\"sequence\":1,\"guardCode\":\"request_manifest_guard\"");
    assert_contains(json,
                    "\"instructionProfiles\":[{\"sequence\":1,\"profileCode\":\"instruction_preflight_request_manifest_profile\"");
    assert_contains(json, "\"instructionPreflightValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = preflight::signed_approval_capture_artifact_draft_instruction_preflight_digest_marker();
    assert(digest.find("v763-route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-template-digest-slot") !=
           std::string::npos);
    assert(preflight::published_stage_count() == 3);
}