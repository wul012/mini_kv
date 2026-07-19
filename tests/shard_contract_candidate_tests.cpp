#include "shard_test_suites.hpp"

namespace minikv::shard_test {

void check_shard_candidate_contract(ShardFixture& fixture) {
    const auto& json = *fixture.active_json;
    assert_contains(json, "\"contract\":\"shard-readiness.v1\"");
    assert_contains(json, "\"evidenceType\":\"shard_readiness\"");
    assert_contains(json, "\"project\":\"mini-kv\"");
    assert_contains(json, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(json, "\"version\":\"" + std::string{minikv::version} + "\",\"releaseVersion\":\"v");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardEnabled\":false");
    assert_contains(json, "\"shardCount\":1");
    assert_contains(json, "\"slotCount\":16");
    assert_contains(json, "\"routingMode\":\"single-shard-readiness-prototype\"");
    assert_contains(json, "\"evidencePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(json, "\"status\":\"f-folder-explanation-quality-closeout-single-large-version-read-only\"");
    assert_contains(
        json,
        "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftInstructionPreflight\":{\"contract\":"
        "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-instruction-preflight."
        "v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\"");
    assert_contains(json, "\"sourceDraftAuthoringReadinessReleaseVersion\":\"v760\"");
    assert_contains(json, "\"sourceDraftAuthoringReadinessPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftAuthoringReadinessChainComplete\":true");
    assert_contains(json, "\"instructionPreflightReleaseVersion\":\"v785\"");
    assert_contains(json, "\"instructionSlotCount\":25");
    assert_contains(json, "\"instructionGuardCount\":25");
    assert_contains(json, "\"instructionProfileCount\":25");
    assert_contains(json, "\"signedApprovalCaptureArtifactDraftInstructionPreflightDeclared\":true");
    assert_contains(json, "\"draftInstructionPreflightOnly\":true");
    assert_contains(json, "\"draftInstructionTextGenerated\":false");
    assert_contains(json, "\"draftInstructionTextMaterialized\":false");
    assert_contains(json, "\"materializedInstructionTextCount\":0");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageReviewPre"
                          "flight\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-"
                          "package-review-preflight.v1\"");
    assert_contains(
        json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\"");
    assert_contains(json, "\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\"");
    assert_contains(json, "\"sourceDraftInstructionPreflightPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftInstructionPreflightChainComplete\":true");
    assert_contains(json, "\"draftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_contains(json, "\"draftTextPackageReviewPreflightOnly\":true");
    assert_contains(json, "\"draftTextPackageParsed\":false");
    assert_contains(json, "\"draftTextPackageAccepted\":false");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageReviewClo"
                          "seoutAudit\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-"
                          "package-review-closeout-audit.v1\"");
    assert_contains(
        json,
        "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightChainComplete\":true");
    assert_contains(json, "\"draftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_contains(json, "\"draftTextPackageReviewCloseoutAuditOnly\":true");
    assert_contains(json, "\"preflightPackageParsedByAudit\":false");
    assert_contains(json, "\"auditReadsRuntimeArchive\":false");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageCompariso"
                          "nCloseoutAudit\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-"
                          "package-comparison-closeout-audit.v1\"");
    assert_contains(
        json, "\"command\":"
              "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_contains(
        json,
        "\"sourceDraftTextPackageReviewCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v835.json\"");
    assert_contains(json, "\"draftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v859\"");
    assert_contains(json, "\"publishedStageCount\":25");
    assert_contains(json, "\"draftTextPackageComparisonCloseoutAuditOnly\":true");
    assert_contains(json, "\"submittedDraftTextPackageParsed\":false");
    assert_contains(json, "\"detachedSignaturePayloadParsed\":false");
    assert_contains(json, "\"uncomparedPackageAccepted\":false");
    assert_contains(json, "\"unacceptablePackageAccepted\":false");
    assert_contains(json, "\"readyForDraftTextPackageComparison\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageComparedP"
                          "ackageEvidenceIntakeAudit\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-"
                          "package-compared-package-evidence-intake-audit.v1\"");
    assert_contains(
        json,
        "\"command\":"
        "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\"");
    assert_contains(json, "\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_contains(
        json,
        "\"sourceDraftTextPackageComparisonCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v860.json\"");
    assert_contains(json, "\"draftTextPackageComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v879\"");
    assert_contains(json, "\"publishedStageCount\":20");
    assert_contains(json, "\"comparedPackageEvidenceIntakeAuditOnly\":true");
    assert_contains(json, "\"comparedPackageEvidencePresent\":false");
    assert_contains(json, "\"syntheticComparedPackageEvidenceAccepted\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateDocumentRequestPackageCloseout\":{\"contract\":"
                          "\"shard-route-preview-candidate-document-request-package-closeout.v1\"");
    assert_contains(json, "\"command\":"
                          "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUEST"
                          "PACKAGECLOSEOUTJSON\"");
    assert_contains(json, "\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_contains(
        json, "\"sourceComparedPackageEvidenceIntakeAuditFixturePath\":\"fixtures/release/shard-readiness-v880.json\"");
    assert_contains(json, "\"candidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v894\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v894.json\"");
    assert_contains(json, "\"publishedStageCount\":15");
    assert_contains(json, "\"plannedRequestItemCount\":15");
    assert_contains(json, "\"plannedAcceptanceCheckCount\":15");
    assert_contains(json, "\"plannedRequestGateCount\":38");
    assert_contains(json, "\"completedRequestGateCount\":38");
    assert_contains(json, "\"requestedCandidateFieldCount\":20");
    assert_contains(json, "\"candidateDocumentRequestPackageCloseoutOnly\":true");
    assert_contains(json, "\"realCandidateDocumentPresent\":false");
    assert_contains(json, "\"candidateDocumentImported\":false");
    assert_contains(json, "\"syntheticCandidateDocumentAccepted\":false");
    assert_contains(json, "\"stagedCandidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"candidateDocumentAccepted\":false");
    assert_contains(json, "\"candidateApprovalGrantCaptured\":false");
    assert_contains(json, "\"candidateSignedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentRequestPackageValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateRequestPackageIntegrity\":{\"contract\":"
                          "\"shard-route-preview-candidate-request-package-fixture-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON\"");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(
        json, "\"sourceCandidateDocumentRequestPackageFixturePath\":\"fixtures/release/shard-readiness-v895.json\"");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackagePublishedStageCount\":15");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageChainComplete\":true");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageRequestItemCount\":15");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageAcceptanceCheckCount\":15");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageGateCount\":38");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageRequestedFieldCount\":20");
    assert_contains(json, "\"candidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v919\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v919.json\"");
    assert_contains(json, "\"candidateRequestPackageIntegrityOnly\":true");
    assert_contains(json, "\"plannedIntegrityCheckCount\":25");
    assert_contains(json, "\"completedIntegrityCheckCount\":25");
    assert_contains(json, "\"candidateRequestPackageIntegrityValidationPassed\":true");
    assert_contains(json, "\"additionalRequestEchoCreated\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"realCandidateDocumentRead\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateSubmissionPrecheck\":{\"contract\":"
                          "\"shard-route-preview-candidate-document-submission-precheck.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON\"");
    assert_contains(
        json, "\"sourceNodePlan\":\"docs/plans3/"
              "v1411-controlled-read-only-shard-preview-candidate-document-submission-precheck-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceNodeSubmissionPrecheckReleaseVersion\":\"Node v1411\"");
    assert_contains(json, "\"sourceNodeSubmissionCheckpointCount\":25");
    assert_contains(json, "\"sourceNodeSubmissionValidatorCount\":25");
    assert_contains(json, "\"sourceNodeSubmissionGateCount\":40");
    assert_contains(json, "\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_contains(json, "\"sourceCandidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_contains(
        json, "\"sourceCandidateRequestPackageIntegrityFixturePath\":\"fixtures/release/shard-readiness-v920.json\"");
    assert_contains(json, "\"sourceCandidateRequestPackageIntegrityPublishedStageCount\":25");
    assert_contains(json, "\"sourceCandidateRequestPackageIntegrityComplete\":true");
    assert_contains(json, "\"requestedCandidateFieldCount\":20");
    assert_contains(json, "\"submissionGateCount\":40");
    assert_contains(json, "\"candidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v929\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v929.json\"");
    assert_contains(json, "\"candidateSubmissionPrecheckOnly\":true");
    assert_contains(json, "\"plannedSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"completedSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"plannedSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"completedSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"candidateSubmissionPrecheckValidationPassed\":true");
    assert_contains(json, "\"additionalSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(json, "\"candidateSubmissionAccepted\":false");
    assert_contains(json, "\"candidateSubmissionImported\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateIntakePacket\":{\"contract\":"
                          "\"shard-route-preview-candidate-document-intake-packet.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEINTAKEPACKETJSON\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
                    "v1421-controlled-read-only-shard-preview-candidate-document-intake-packet-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\"");
    assert_contains(json, "\"sourceNodeSubmissionPrecheckReleaseVersion\":\"Node v1411\"");
    assert_contains(json, "\"sourceNodeIntakeSlotCount\":10");
    assert_contains(json, "\"sourceNodeIntakeGuardCount\":10");
    assert_contains(json, "\"sourceNodeCoveredSubmissionCheckpointCount\":25");
    assert_contains(json, "\"sourceNodeCoveredSubmissionValidatorCount\":25");
    assert_contains(json, "\"sourceNodeIntakeGateCount\":35");
    assert_contains(json, "\"sourceCandidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_contains(json,
                    "\"sourceCandidateSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v930.json\"");
    assert_contains(json, "\"sourceCandidateSubmissionPrecheckPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateSubmissionPrecheckComplete\":true");
    assert_contains(json, "\"candidateIntakePacketReleaseVersion\":\"v955\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v954\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v954.json\"");
    assert_contains(json, "\"candidateIntakePacketOnly\":true");
    assert_contains(json, "\"plannedIntakeSlotCount\":10");
    assert_contains(json, "\"completedIntakeSlotCount\":10");
    assert_contains(json, "\"plannedIntakeGuardCount\":10");
    assert_contains(json, "\"completedIntakeGuardCount\":10");
    assert_contains(json, "\"candidateIntakePacketValidationPassed\":true");
    assert_contains(json, "\"additionalIntakeEchoCreated\":false");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"shardPreviewCandidateMaterialRequest\":{\"contract\":"
                          "\"shard-route-preview-candidate-document-material-request.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEMATERIALREQUESTJSON\"");
    assert_contains(
        json, "\"sourceNodePlan\":\"docs/plans3/"
              "v1446-controlled-read-only-shard-preview-candidate-document-material-request-closeout-roadmap.md\"");
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
    assert_contains(json, "\"candidateMaterialRequestOnly\":true");
    assert_contains(json, "\"plannedMaterialRequestItemCount\":25");
    assert_contains(json, "\"completedMaterialRequestItemCount\":25");
    assert_contains(json, "\"plannedAcceptanceCheckCount\":25");
    assert_contains(json, "\"completedAcceptanceCheckCount\":25");
    assert_contains(json, "\"candidateMaterialRequestValidationPassed\":true");
    assert_contains(json, "\"additionalMaterialRequestEchoCreated\":false");
    assert_contains(json, "\"externalReviewedMaterialRequested\":true");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"reviewedRealCandidateDocumentRead\":false");
    assert_contains(json, "\"materialRequestSatisfied\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateMaterialRequestIntegrity\":{\"contract\":"
                          "\"shard-route-preview-candidate-material-request-fixture-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON\"");
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
    assert_contains(json, "\"candidateMaterialRequestIntegrityOnly\":true");
    assert_contains(json, "\"plannedIntegrityCheckCount\":15");
    assert_contains(json, "\"completedIntegrityCheckCount\":15");
    assert_contains(json, "\"candidateMaterialRequestIntegrityValidationPassed\":true");
    assert_contains(json, "\"nodeStopConditionRespected\":true");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"materialRequestReopened\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateMaterialSubmissionPrecheck\":{\"contract\":"
                          "\"shard-route-preview-candidate-document-material-submission-precheck.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON\"");
    assert_contains(json, "\"sourceNodeMaterialSubmissionPrecheckReleaseVersion\":\"Node v1456\"");
    assert_contains(json, "\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\"");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityReleaseVersion\":\"v975\"");
    assert_contains(
        json, "\"sourceCandidateMaterialRequestIntegrityFixturePath\":\"fixtures/release/shard-readiness-v975.json\"");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityPublishedStageCount\":10");
    assert_contains(json, "\"sourceCandidateMaterialRequestIntegrityComplete\":true");
    assert_contains(json, "\"sourceNodeMaterialRequestItemCount\":25");
    assert_contains(json, "\"sourceNodeMaterialAcceptanceCheckCount\":25");
    assert_contains(json, "\"requiredMaterialFieldCount\":20");
    assert_contains(json, "\"submissionMaterialFieldCount\":20");
    assert_contains(json, "\"materialSubmissionGateCount\":41");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckReleaseVersion\":\"v985\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v984\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v984.json\"");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckOnly\":true");
    assert_contains(json, "\"plannedMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"completedMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_contains(json, "\"plannedMaterialSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"completedMaterialSubmissionPrecheckValidatorCount\":10");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckValidationPassed\":true");
    assert_contains(json, "\"nodeStopConditionRespected\":true");
    assert_contains(json, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateMaterialSubmissionPrecheckIntegrity\":{\"contract\":"
                          "\"shard-route-preview-candidate-material-submission-precheck-fixture-integrity.v1\"");
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
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckIntegrityOnly\":true");
    assert_contains(json, "\"plannedIntegrityCheckCount\":25");
    assert_contains(json, "\"completedIntegrityCheckCount\":25");
    assert_contains(json, "\"candidateMaterialSubmissionPrecheckIntegrityValidationPassed\":true");
    assert_contains(json, "\"nodeStopConditionRespected\":true");
    assert_contains(json, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"externalReviewedMaterialConsumed\":false");
    assert_contains(json, "\"reviewedMaterialPresent\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"candidatePayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"approvalGrantCaptured\":false");
    assert_contains(json, "\"signedApprovalEmitted\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewCandidateProfileSection\":{\"contract\":"
                          "\"shard-route-preview-candidate-document-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeProfileSectionReleaseVersion\":\"Node v1481\"");
    assert_contains(json, "\"sourceNodeCandidateProfileSectionRendererModule\":"
                          "\"controlledReadOnlyShardPreviewCandidateDocumentProfileSectionRenderer.ts\"");
    assert_contains(json, "\"sourceNodeProfileSectionGroupCount\":5");
    assert_contains(json, "\"sourceNodeMigratedCandidateProfileSectionCount\":5");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRouteMarkdownChanged\":false");
    assert_contains(json, "\"sourceNodeRuntimeBehaviorAdded\":false");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\"");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityFixturePath\":\"fixtures/release/"
                          "shard-readiness-v1010.json\"");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityPublishedStageCount\":25");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityComplete\":true");
    assert_contains(json, "\"candidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1034\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1034.json\"");
    assert_contains(json, "\"candidateProfileSectionOnly\":true");
    assert_contains(json, "\"plannedProfileSectionCheckCount\":25");
    assert_contains(json, "\"completedProfileSectionCheckCount\":25");
    assert_contains(json, "\"candidateProfileSectionValidationPassed\":true");
    assert_contains(json, "\"routeFacingMarkdownStable\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"miniKvConsumesProfileMaterial\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewSignedApprovalArtifactDraftProfileSection\":{\"contract\":"
                          "\"shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionRendererModule\":"
                          "\"controlledReadOnlyShardPreviewSignedApprovalArtifactDraftProfileSectionRenderer.ts\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionGroupCount\":5");
    assert_contains(json, "\"sourceNodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":5");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRouteMarkdownChanged\":false");
    assert_contains(json, "\"sourceNodeRuntimeBehaviorAdded\":false");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1035\"");
    assert_contains(json, "\"sourceCandidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_contains(json, "\"sourceCandidateProfileSectionPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\"");
    assert_contains(json, "\"sourceDraftInstructionPreflightPublishedStageCount\":25");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionReleaseVersion\":\"v1060\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1059\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1059.json\"");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionOnly\":true");
    assert_contains(json, "\"plannedSignedApprovalArtifactDraftProfileSectionCheckCount\":25");
    assert_contains(json, "\"completedSignedApprovalArtifactDraftProfileSectionCheckCount\":25");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionValidationPassed\":true");
    assert_contains(json, "\"routeFacingMarkdownStable\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"miniKvConsumesSignedApprovalMaterial\":false");
    assert_contains(json, "\"draftArtifactCreated\":false");
    assert_contains(json, "\"signedApprovalCreated\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"materialIntakeOpened\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewSignedApprovalArtifactDraftProfileSectionIntegrity\":{\"contract\":"
                          "\"shard-route-preview-signed-approval-artifact-draft-profile-section-integrity.v1\"");
    assert_contains(
        json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON\"");
    assert_contains(json, "\"sourceProfileSectionCommand\":"
                          "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceProfileSectionReleaseVersion\":\"v1060\"");
    assert_contains(json, "\"sourceProfileSectionPublishedStageCount\":25");
    assert_contains(json, "\"sourceProfileSectionComplete\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionIntegrityReleaseVersion\":\"v1085\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1084\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1084.json\"");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionIntegrityOnly\":true");
    assert_contains(json, "\"plannedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":25");
    assert_contains(json, "\"completedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":25");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionIntegrityValidationPassed\":true");
    assert_contains(json, "\"sourceProfileSectionFrozen\":true");
    assert_contains(json, "\"profileScopeAligned\":true");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"draftArtifactCreated\":false");
    assert_contains(json, "\"signedApprovalCreated\":false");
    assert_contains(json, "\"materialIntakeOpened\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(
        json, "\"shardPreviewSignedApprovalArtifactDraftTextPackageProfileSection\":{\"contract\":"
              "\"shard-route-preview-signed-approval-artifact-draft-text-package-profile-section-renderer-split.v1\"");
    assert_contains(
        json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON\"");
    assert_contains(json,
                    "\"sourceNodeSignedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"Node v1531\"");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1085\"");
    assert_contains(json, "\"sourceSignedApprovalDraftProfileSectionIntegrityReleaseVersion\":\"v1085\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_contains(json, "\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_contains(json, "\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_contains(json, "\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"v1110\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1109\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1109.json\"");
    assert_contains(json, "\"signedApprovalArtifactDraftTextPackageProfileSectionOnly\":true");
    assert_contains(json, "\"plannedSignedApprovalArtifactDraftTextPackageProfileSectionCheckCount\":25");
    assert_contains(json, "\"completedSignedApprovalArtifactDraftTextPackageProfileSectionCheckCount\":25");
    assert_contains(json, "\"signedApprovalArtifactDraftTextPackageProfileSectionValidationPassed\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftTextPackageProfileSectionCount\":9");
    assert_contains(json, "\"nodeMigratedSignedApprovalArtifactDraftTextPackageProfileSectionCount\":9");
    assert_contains(json, "\"textPackageMaterialImported\":false");
    assert_contains(json, "\"textPackageMaterialAccepted\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewOperatorValueSupplyApprovalProfileSection\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-approval-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeOperatorValueSupplyApprovalProfileSectionReleaseVersion\":\"Node v1556\"");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1110\"");
    assert_contains(json, "\"sourceApprovalPacketDraftReleaseVersion\":\"v660\"");
    assert_contains(json, "\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\"");
    assert_contains(json, "\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\"");
    assert_contains(json, "\"sourceSignedApprovalCaptureArtifactPreflightReleaseVersion\":\"v735\"");
    assert_contains(json, "\"operatorValueSupplyApprovalProfileSectionReleaseVersion\":\"v1135\"");
    assert_contains(json, "\"operatorValueSupplyApprovalProfileSectionOnly\":true");
    assert_contains(json, "\"plannedOperatorValueSupplyApprovalProfileSectionCheckCount\":25");
    assert_contains(json, "\"completedOperatorValueSupplyApprovalProfileSectionCheckCount\":25");
    assert_contains(json, "\"operatorValueSupplyApprovalProfileSectionValidationPassed\":true");
    assert_contains(json, "\"operatorValueSupplyApprovalProfileSectionCount\":5");
    assert_contains(json, "\"nodeMigratedOperatorValueSupplyApprovalProfileSectionCount\":5");
    assert_contains(json, "\"approval-packet-review\"");
    assert_contains(json, "\"signed-approval-capture-artifact-preflight\"");
    assert_contains(json, "\"approvalMaterialImported\":false");
    assert_contains(json, "\"signedApprovalCaptured\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewOperatorValueSupplyProfileSection\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeOperatorValueSupplyProfileSectionReleaseVersion\":\"Node v1581\"");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1135\"");
    assert_contains(json, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
    assert_contains(json, "\"sourceValueDraftReleaseVersion\":\"v585\"");
    assert_contains(json, "\"sourceFreshSiblingIntakeReleaseVersion\":\"Node v936\"");
    assert_contains(json, "\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\"");
    assert_contains(json, "\"operatorValueSupplyProfileSectionReleaseVersion\":\"v1160\"");
    assert_contains(json, "\"operatorValueSupplyProfileSectionOnly\":true");
    assert_contains(json, "\"plannedOperatorValueSupplyProfileSectionCheckCount\":25");
    assert_contains(json, "\"completedOperatorValueSupplyProfileSectionCheckCount\":25");
    assert_contains(json, "\"operatorValueSupplyProfileSectionValidationPassed\":true");
    assert_contains(json, "\"operatorValueSupplyProfileSectionCount\":4");
    assert_contains(json, "\"nodeMigratedOperatorValueSupplyProfileSectionCount\":4");
    assert_contains(json, "\"operator-evidence-fresh-sibling-intake\"");
    assert_contains(json, "\"operator-evidence-value-supply-envelope\"");
    assert_contains(json, "\"operatorValueImported\":false");
    assert_contains(json, "\"freshSiblingEvidenceImported\":false");
    assert_contains(json, "\"syntheticEvidenceAccepted\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewOperatorValueSupplyProfileSectionIntegrity\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-profile-section-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\"");
    assert_contains(json,
                    "\"sourceNodeProductionLiveProbeReadOnlyWindowReadinessPacketReleaseVersion\":\"Node v1606\"");
    assert_contains(json, "\"sourceProfileSectionCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceProfileSectionReleaseVersion\":\"v1160\"");
    assert_contains(json, "\"sourceProfileSectionPublishedStageCount\":25");
    assert_contains(json, "\"operatorValueSupplyProfileSectionIntegrityReleaseVersion\":\"v1170\"");
    assert_contains(json, "\"operatorValueSupplyProfileSectionIntegrityOnly\":true");
    assert_contains(json, "\"plannedOperatorValueSupplyProfileSectionIntegrityCheckCount\":10");
    assert_contains(json, "\"completedOperatorValueSupplyProfileSectionIntegrityCheckCount\":10");
    assert_contains(json, "\"operatorValueSupplyProfileSectionIntegrityValidationPassed\":true");
    assert_contains(json, "\"operatorValueSupplyProfileSectionCount\":4");
    assert_contains(json, "\"Node-v936-fresh-sibling-intake-plan\"");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"rawEndpointStored\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewProductionLiveCaptureNonParticipation\":{\"contract\":"
                          "\"shard-route-preview-production-live-capture-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceNodeProductionLiveProbeReadOnlyWindowLiveCaptureCloseoutReleaseVersion\":\"Node v1631\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1170\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":10");
    assert_contains(json, "\"productionLiveCaptureNonParticipationReleaseVersion\":\"v1195\"");
    assert_contains(json, "\"plannedProductionLiveCaptureNonParticipationCheckCount\":25");
    assert_contains(json, "\"completedProductionLiveCaptureNonParticipationCheckCount\":25");
    assert_contains(json, "\"productionLiveCaptureNonParticipationValidationPassed\":true");
    assert_contains(json, "\"nodeLiveCaptureExecuted\":false");
    assert_contains(json, "\"miniKvStartsLiveCapture\":false");
    assert_contains(json, "\"miniKvReadsRawEndpoint\":false");
    assert_contains(json, "\"liveEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"rawEndpointStored\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"productionLiveReadAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewProductionLiveCaptureArchiveNonParticipation\":{\"contract\":"
                          "\"shard-route-preview-production-live-capture-archive-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\"");
    assert_contains(
        json, "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveCloseoutReleaseVersion\":\"Node v1656\"");
    assert_contains(json, "\"sourceNodeArchiveModuleSplitOnly\":true");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1195\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":25");
    assert_contains(json, "\"productionLiveCaptureArchiveNonParticipationReleaseVersion\":\"v1225\"");
    assert_contains(json, "\"plannedProductionLiveCaptureArchiveNonParticipationCheckCount\":30");
    assert_contains(json, "\"completedProductionLiveCaptureArchiveNonParticipationCheckCount\":30");
    assert_contains(json, "\"productionLiveCaptureArchiveNonParticipationValidationPassed\":true");
    assert_contains(json, "\"nodeCaptureArchiveExecuted\":false");
    assert_contains(json, "\"nodeArchiveLoaderExecuted\":false");
    assert_contains(json, "\"miniKvStartsCaptureArchive\":false");
    assert_contains(json, "\"shardPreviewProductionLiveCaptureArchiveVerificationNonParticipation\":{\"contract\":"
                          "\"shard-route-preview-production-live-capture-archive-verification-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveVerificationCloseoutReleaseVersio"
                          "n\":\"Node v1686\"");
    assert_contains(json, "\"sourceNodeArchiveVerificationModuleSplitOnly\":true");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1225\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":30");
    assert_contains(json, "\"productionLiveCaptureArchiveVerificationNonParticipationReleaseVersion\":\"v1260\"");
    assert_contains(json, "\"plannedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":35");
    assert_contains(json, "\"completedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":35");
    assert_contains(json, "\"productionLiveCaptureArchiveVerificationNonParticipationValidationPassed\":true");
    assert_contains(json, "\"nodeCaptureArchiveVerificationExecuted\":false");
    assert_contains(json, "\"nodeArchiveVerificationLoaderExecuted\":false");
    assert_contains(json, "\"nodeArchiveVerificationRendererExecuted\":false");
    assert_contains(json, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_contains(json, "\"miniKvStartsCaptureArchiveVerification\":false");
    assert_contains(json, "\"miniKvReadsVerificationEndpoint\":false");
    assert_contains(json, "\"miniKvReadsArchiveEndpoint\":false");
    assert_contains(json, "\"archiveEndpointRead\":false");
    assert_contains(json, "\"archivePayloadImported\":false");
    assert_contains(json, "\"archiveFixtureImported\":false");
    assert_contains(json, "\"archiveRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json,
                    "\"shardPreviewProductionLiveCaptureReleaseEvidenceReviewNonParticipation\":{\"contract\":"
                    "\"shard-route-preview-production-live-capture-release-evidence-review-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureReleaseEvidenceReviewCloseoutReleaseVers"
                          "ion\":\"Node v1721\"");
    assert_contains(json, "\"sourceNodeReleaseEvidenceReviewModuleSplitOnly\":true");
    assert_contains(json,
                    "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1260\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":35");
    assert_contains(json, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":\"v1295\"");
    assert_contains(json, "\"plannedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":35");
    assert_contains(json, "\"completedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":35");
    assert_contains(json, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationValidationPassed\":true");
    assert_contains(json, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewLoaderExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewRendererExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseReviewDigestGeneratedByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsReleaseEvidenceReview\":false");
    assert_contains(json, "\"miniKvReadsReleaseReviewEndpoint\":false");
    assert_contains(json, "\"releaseReviewEndpointRead\":false");
    assert_contains(json, "\"releaseReviewPayloadImported\":false");
    assert_contains(json, "\"releaseReviewFixtureImported\":false");
    assert_contains(json, "\"releaseEvidenceReviewRouterInstalled\":false");
    assert_contains(json, "\"shardPreviewCatalogEntryGroupSplitNonParticipation\":{\"contract\":"
                          "\"shard-route-preview-catalog-entry-group-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeCatalogEntryGroupSplitCloseoutReleaseVersion\":\"Node v1756\"");
    assert_contains(json, "\"sourceNodeCatalogEntryGroupSplitOnly\":true");
    assert_contains(
        json,
        "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1295\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":35");
    assert_contains(json, "\"catalogEntryGroupSplitNonParticipationReleaseVersion\":\"v1360\"");
    assert_contains(json, "\"catalogEntryGroupSplitNonParticipationReleaseRangeStart\":\"v1331\"");
    assert_contains(json, "\"plannedCatalogEntryGroupSplitNonParticipationCheckCount\":30");
    assert_contains(json, "\"completedCatalogEntryGroupSplitNonParticipationCheckCount\":30");
    assert_contains(json, "\"catalogEntryGroupSplitNonParticipationValidationPassed\":true");
    assert_contains(json, "\"nodeCatalogEntryRecordsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogRegistryExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogMarkdownRenderedByMiniKv\":false");
    assert_contains(json, "\"nodeCatalogEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsCatalogSplit\":false");
    assert_contains(json, "\"miniKvConsumesCatalogRecords\":false");
    assert_contains(json, "\"miniKvReadsCatalogEndpoint\":false");
    assert_contains(json, "\"catalogEndpointRead\":false");
    assert_contains(json, "\"catalogRecordsImported\":false");
    assert_contains(json, "\"catalogRegistryExecuted\":false");
    assert_contains(json, "\"catalogMarkdownRendered\":false");
    assert_contains(json, "\"catalogRouterInstalled\":false");
    assert_contains(json, "\"shardPreviewTypeBarrelSplitNonParticipation\":{\"contract\":"
                          "\"shard-route-preview-type-barrel-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeTypeBarrelSplitCloseoutReleaseVersion\":\"Node v1846\"");
    assert_contains(json, "\"sourceNodeTypeBarrelSplitOnly\":true");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1360\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":30");
    assert_contains(json, "\"typeBarrelSplitNonParticipationReleaseVersion\":\"v1385\"");
    assert_contains(json, "\"typeBarrelSplitNonParticipationReleaseRangeStart\":\"v1361\"");
    assert_contains(json, "\"plannedTypeBarrelSplitNonParticipationCheckCount\":25");
    assert_contains(json, "\"completedTypeBarrelSplitNonParticipationCheckCount\":25");
    assert_contains(json, "\"typeBarrelSplitNonParticipationValidationPassed\":true");
    assert_contains(json, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeAggregateReexportsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileBoundaryEndpointReadByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsTypeBarrelSplit\":false");
    assert_contains(json, "\"miniKvImportsNodeTypes\":false");
    assert_contains(json, "\"miniKvConsumesProfileBoundaries\":false");
    assert_contains(json, "\"profileBoundaryEndpointRead\":false");
    assert_contains(json, "\"evidenceEndpointRead\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
}

void assert_shard_readiness_contract(ShardFixture& fixture, const std::string& json) {
    fixture.active_json = &json;
    check_shard_candidate_contract(fixture);
    check_shard_governance_contract(fixture);
    check_shard_route_contract(fixture);
    check_shard_history_contract(fixture);
    check_shard_catalog_contract(fixture);
    fixture.active_json = nullptr;
}

} // namespace minikv::shard_test
