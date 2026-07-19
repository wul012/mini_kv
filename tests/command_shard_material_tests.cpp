#include "command_test_suites.hpp"

namespace minikv::command_test {

void run_command_shard_material_tests(CommandFixture& fixture) {
    auto& processor = fixture.processor;
    auto& result = fixture.result;

    result = processor.execute("SHARDROUTECANDIDATEINTAKEPACKETJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-preview-candidate-document-intake-packet.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATEINTAKEPACKETJSON\"");
    assert_response_contains(result, "\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\"");
    assert_response_contains(result, "\"sourceCandidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_response_contains(
        result, "\"sourceCandidateSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v930.json\"");
    assert_response_contains(result, "\"candidateIntakePacketReleaseVersion\":\"v955\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v954\"");
    assert_response_contains(result, "\"publishedStageCount\":25");
    assert_response_contains(result, "\"plannedIntakeSlotCount\":10");
    assert_response_contains(result, "\"plannedIntakeGuardCount\":10");
    assert_response_contains(result, "\"candidateIntakePacketOnly\":true");
    assert_response_contains(result, "\"additionalIntakeEchoCreated\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEINTAKEPACKETJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATEINTAKEPACKETJSON");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALREQUESTJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-preview-candidate-document-material-request.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATEMATERIALREQUESTJSON\"");
    assert_response_contains(result, "\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\"");
    assert_response_contains(result, "\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\"");
    assert_response_contains(result, "\"sourceCandidateIntakePacketReleaseVersion\":\"v955\"");
    assert_response_contains(
        result, "\"sourceCandidateIntakePacketFixturePath\":\"fixtures/release/shard-readiness-v955.json\"");
    assert_response_contains(result, "\"candidateMaterialRequestReleaseVersion\":\"v965\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v964\"");
    assert_response_contains(result, "\"publishedStageCount\":10");
    assert_response_contains(result, "\"plannedMaterialRequestItemCount\":25");
    assert_response_contains(result, "\"plannedAcceptanceCheckCount\":25");
    assert_response_contains(result, "\"candidateMaterialRequestOnly\":true");
    assert_response_contains(result, "\"additionalMaterialRequestEchoCreated\":false");
    assert_response_contains(result, "\"reviewedMaterialPresent\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALREQUESTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATEMATERIALREQUESTJSON");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-candidate-material-request-fixture-integrity.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON\"");
    assert_response_contains(result, "\"sourceCandidateMaterialRequestReleaseVersion\":\"v965\"");
    assert_response_contains(
        result, "\"sourceCandidateMaterialRequestFixturePath\":\"fixtures/release/shard-readiness-v965.json\"");
    assert_response_contains(result, "\"candidateMaterialRequestIntegrityReleaseVersion\":\"v975\"");
    assert_response_contains(result, "\"plannedIntegrityCheckCount\":15");
    assert_response_contains(result, "\"candidateMaterialRequestIntegrityOnly\":true");
    assert_response_contains(result, "\"nodeStopConditionRespected\":true");
    assert_response_contains(result, "\"additionalMaterialRequestEchoCreated\":false");
    assert_response_contains(result, "\"externalReviewedMaterialConsumed\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-candidate-document-material-submission-precheck.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON\"");
    assert_response_contains(result, "\"sourceNodeMaterialSubmissionPrecheckReleaseVersion\":\"Node v1456\"");
    assert_response_contains(result, "\"sourceCandidateMaterialRequestIntegrityReleaseVersion\":\"v975\"");
    assert_response_contains(
        result,
        "\"sourceCandidateMaterialRequestIntegrityFixturePath\":\"fixtures/release/shard-readiness-v975.json\"");
    assert_response_contains(result, "\"candidateMaterialSubmissionPrecheckReleaseVersion\":\"v985\"");
    assert_response_contains(result, "\"plannedMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_response_contains(result, "\"plannedMaterialSubmissionPrecheckValidatorCount\":10");
    assert_response_contains(result, "\"candidateMaterialSubmissionPrecheckOnly\":true");
    assert_response_contains(result, "\"nodeStopConditionRespected\":true");
    assert_response_contains(result, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_response_contains(result, "\"externalReviewedMaterialConsumed\":false");
    assert_response_contains(result, "\"materialPayloadImported\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-preview-candidate-material-submission-precheck-fixture-integrity.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON\"");
    assert_response_contains(result, "\"sourceCandidateMaterialSubmissionPrecheckReleaseVersion\":\"v985\"");
    assert_response_contains(
        result,
        "\"sourceCandidateMaterialSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v985.json\"");
    assert_response_contains(result, "\"sourceCandidateMaterialSubmissionPrecheckPublishedStageCount\":10");
    assert_response_contains(result, "\"sourceMaterialSubmissionPrecheckCheckpointCount\":10");
    assert_response_contains(result, "\"sourceMaterialSubmissionPrecheckValidatorCount\":10");
    assert_response_contains(result, "\"sourceMaterialRequestItemCount\":25");
    assert_response_contains(result, "\"sourceAcceptanceCheckCount\":25");
    assert_response_contains(result, "\"sourceRequiredMaterialFieldCount\":20");
    assert_response_contains(result, "\"sourceSubmissionMaterialFieldCount\":20");
    assert_response_contains(result, "\"sourceMaterialSubmissionGateCount\":41");
    assert_response_contains(result, "\"candidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\"");
    assert_response_contains(result, "\"plannedIntegrityCheckCount\":25");
    assert_response_contains(result, "\"candidateMaterialSubmissionPrecheckIntegrityOnly\":true");
    assert_response_contains(result, "\"nodeStopConditionRespected\":true");
    assert_response_contains(result, "\"additionalMaterialSubmissionPrecheckEchoCreated\":false");
    assert_response_contains(result, "\"externalReviewedMaterialConsumed\":false");
    assert_response_contains(result, "\"materialPayloadImported\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON");

    result = processor.execute("SHARDROUTECANDIDATEPROFILESECTIONJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-preview-candidate-document-profile-section-renderer-split.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATEPROFILESECTIONJSON\"");
    assert_response_contains(result, "\"sourceNodeProfileSectionReleaseVersion\":\"Node v1481\"");
    assert_response_contains(result, "\"sourceNodeProfileSectionGroupCount\":5");
    assert_response_contains(result, "\"sourceNodeMigratedCandidateProfileSectionCount\":5");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRouteMarkdownChanged\":false");
    assert_response_contains(result, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\"");
    assert_response_contains(result, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityFixturePath\":\"fixtures/"
                                     "release/shard-readiness-v1010.json\"");
    assert_response_contains(result, "\"candidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_response_contains(result, "\"plannedProfileSectionCheckCount\":25");
    assert_response_contains(result, "\"candidateProfileSectionOnly\":true");
    assert_response_contains(result, "\"routeFacingMarkdownStable\":true");
    assert_response_contains(result, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_response_contains(result, "\"nodeRendererExecuted\":false");
    assert_response_contains(result, "\"miniKvConsumesProfileMaterial\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"materialSubmissionOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEPROFILESECTIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATEPROFILESECTIONJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON");
    assert_response_contains(
        result,
        "\"contract\":\"shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\"");
    assert_response_contains(result,
                             "\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\"");
    assert_response_contains(result, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionGroupCount\":5");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceCandidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_response_contains(result, "\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\"");
    assert_response_contains(result, "\"signedApprovalArtifactDraftProfileSectionReleaseVersion\":\"v1060\"");
    assert_response_contains(result, "\"plannedSignedApprovalArtifactDraftProfileSectionCheckCount\":25");
    assert_response_contains(result, "\"signedApprovalArtifactDraftProfileSectionOnly\":true");
    assert_response_contains(result, "\"routeFacingMarkdownStable\":true");
    assert_response_contains(result, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_response_contains(result, "\"nodeRendererExecuted\":false");
    assert_response_contains(result, "\"miniKvConsumesSignedApprovalMaterial\":false");
    assert_response_contains(result, "\"draftArtifactCreated\":false");
    assert_response_contains(result, "\"signedApprovalCreated\":false");
    assert_response_contains(result, "\"runtimePayloadImported\":false");
    assert_response_contains(result, "\"materialIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-preview-signed-approval-artifact-draft-profile-section-integrity.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON\"");
    assert_response_contains(result,
                             "\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\"");
    assert_response_contains(result, "\"sourceProfileSectionCommand\":"
                                     "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\"");
    assert_response_contains(result, "\"sourceProfileSectionReleaseVersion\":\"v1060\"");
    assert_response_contains(result, "\"sourceProfileSectionPublishedStageCount\":25");
    assert_response_contains(result, "\"signedApprovalArtifactDraftProfileSectionIntegrityReleaseVersion\":\"v1085\"");
    assert_response_contains(result, "\"plannedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":25");
    assert_response_contains(result, "\"signedApprovalArtifactDraftProfileSectionIntegrityOnly\":true");
    assert_response_contains(result, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_response_contains(result, "\"nodeRendererExecuted\":false");
    assert_response_contains(result, "\"nodeCatalogImported\":false");
    assert_response_contains(result, "\"draftArtifactCreated\":false");
    assert_response_contains(result, "\"signedApprovalCreated\":false");
    assert_response_contains(result, "\"materialIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result =
        processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON extra");
    assert(result.response ==
           "ERR usage: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-preview-signed-approval-artifact-draft-text-package-"
                                     "profile-section-renderer-split.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON\"");
    assert_response_contains(
        result, "\"sourceNodeSignedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"Node v1531\"");
    assert_response_contains(result, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1085\"");
    assert_response_contains(result, "\"sourceSignedApprovalDraftProfileSectionIntegrityReleaseVersion\":\"v1085\"");
    assert_response_contains(result, "\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_response_contains(result, "\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_response_contains(result, "\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_response_contains(result, "\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_response_contains(result,
                             "\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"v1110\"");
    assert_response_contains(result, "\"plannedSignedApprovalArtifactDraftTextPackageProfileSectionCheckCount\":25");
    assert_response_contains(result, "\"signedApprovalArtifactDraftTextPackageProfileSectionOnly\":true");
    assert_response_contains(result, "\"signedApprovalArtifactDraftTextPackageProfileSectionCount\":9");
    assert_response_contains(result, "\"nodeRendererExecuted\":false");
    assert_response_contains(result, "\"nodeCatalogImported\":false");
    assert_response_contains(result, "\"textPackageMaterialImported\":false");
    assert_response_contains(result, "\"secretValueRead\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result =
        processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON extra");
    assert(result.response ==
           "ERR usage: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON");
    assert_response_contains(
        result,
        "\"contract\":\"shard-route-preview-operator-value-supply-approval-profile-section-renderer-split.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON\"");
    assert_response_contains(result,
                             "\"sourceNodeOperatorValueSupplyApprovalProfileSectionReleaseVersion\":\"Node v1556\"");
    assert_response_contains(result, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1110\"");
    assert_response_contains(result, "\"sourceApprovalPacketDraftReleaseVersion\":\"v660\"");
    assert_response_contains(result, "\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\"");
    assert_response_contains(result, "\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\"");
    assert_response_contains(result, "\"sourceSignedApprovalCaptureArtifactPreflightReleaseVersion\":\"v735\"");
    assert_response_contains(result, "\"operatorValueSupplyApprovalProfileSectionReleaseVersion\":\"v1135\"");
    assert_response_contains(result, "\"plannedOperatorValueSupplyApprovalProfileSectionCheckCount\":25");
    assert_response_contains(result, "\"operatorValueSupplyApprovalProfileSectionOnly\":true");
    assert_response_contains(result, "\"operatorValueSupplyApprovalProfileSectionCount\":5");
    assert_response_contains(result, "\"approval-packet-review\"");
    assert_response_contains(result, "\"signed-approval-capture-artifact-preflight\"");
    assert_response_contains(result, "\"nodeRendererExecuted\":false");
    assert_response_contains(result, "\"nodeCatalogImported\":false");
    assert_response_contains(result, "\"approvalMaterialImported\":false");
    assert_response_contains(result, "\"signedApprovalCaptured\":false");
    assert_response_contains(result, "\"secretValueRead\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYPROFILESECTIONJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-preview-operator-value-supply-profile-section-renderer-split.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\"");
    assert_response_contains(result, "\"sourceNodeOperatorValueSupplyProfileSectionReleaseVersion\":\"Node v1581\"");
    assert_response_contains(result, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1135\"");
    assert_response_contains(result, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
    assert_response_contains(result, "\"sourceValueDraftReleaseVersion\":\"v585\"");
    assert_response_contains(result, "\"sourceFreshSiblingIntakeReleaseVersion\":\"Node v936\"");
    assert_response_contains(result, "\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\"");
    assert_response_contains(result, "\"operatorValueSupplyProfileSectionReleaseVersion\":\"v1160\"");
    assert_response_contains(result, "\"plannedOperatorValueSupplyProfileSectionCheckCount\":25");
    assert_response_contains(result, "\"operatorValueSupplyProfileSectionOnly\":true");
    assert_response_contains(result, "\"operatorValueSupplyProfileSectionCount\":4");
    assert_response_contains(result, "\"operator-evidence-fresh-sibling-intake\"");
    assert_response_contains(result, "\"operator-evidence-value-supply-envelope\"");
    assert_response_contains(result, "\"nodeRendererExecuted\":false");
    assert_response_contains(result, "\"nodeCatalogImported\":false");
    assert_response_contains(result, "\"operatorValueImported\":false");
    assert_response_contains(result, "\"freshSiblingEvidenceImported\":false");
    assert_response_contains(result, "\"syntheticEvidenceAccepted\":false");
    assert_response_contains(result, "\"secretValueRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVALUESUPPLYPROFILESECTIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYPROFILESECTIONJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-operator-value-supply-profile-section-integrity.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\"");
    assert_response_contains(
        result, "\"sourceNodeProductionLiveProbeReadOnlyWindowReadinessPacketReleaseVersion\":\"Node v1606\"");
    assert_response_contains(result, "\"sourceProfileSectionCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\"");
    assert_response_contains(result, "\"sourceProfileSectionReleaseVersion\":\"v1160\"");
    assert_response_contains(result, "\"sourceProfileSectionPublishedStageCount\":25");
    assert_response_contains(result, "\"operatorValueSupplyProfileSectionIntegrityReleaseVersion\":\"v1170\"");
    assert_response_contains(result, "\"plannedOperatorValueSupplyProfileSectionIntegrityCheckCount\":10");
    assert_response_contains(result, "\"operatorValueSupplyProfileSectionIntegrityOnly\":true");
    assert_response_contains(result, "\"operatorValueSupplyProfileSectionCount\":4");
    assert_response_contains(result, "\"nodeRendererExecuted\":false");
    assert_response_contains(result, "\"nodeCatalogImported\":false");
    assert_response_contains(result, "\"operatorValueImported\":false");
    assert_response_contains(result, "\"freshSiblingEvidenceImported\":false");
    assert_response_contains(result, "\"syntheticEvidenceAccepted\":false");
    assert_response_contains(result, "\"secretValueRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-production-live-capture-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON\"");
    assert_response_contains(
        result, "\"sourceNodeProductionLiveProbeReadOnlyWindowLiveCaptureCloseoutReleaseVersion\":\"Node v1631\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result, "\"sourceNodeModuleSplitOnly\":true");
    assert_response_contains(result, "\"sourceIntegrityCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\"");
    assert_response_contains(result, "\"sourceIntegrityReleaseVersion\":\"v1170\"");
    assert_response_contains(result, "\"sourceIntegrityPublishedStageCount\":10");
    assert_response_contains(result, "\"productionLiveCaptureNonParticipationReleaseVersion\":\"v1195\"");
    assert_response_contains(result, "\"plannedProductionLiveCaptureNonParticipationCheckCount\":25");
    assert_response_contains(result, "\"productionLiveCaptureNonParticipationOnly\":true");
    assert_response_contains(result, "\"nodeLiveCaptureExecuted\":false");
    assert_response_contains(result, "\"miniKvStartsLiveCapture\":false");
    assert_response_contains(result, "\"miniKvReadsRawEndpoint\":false");
    assert_response_contains(result, "\"liveEndpointRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"rawEndpointStored\":false");
    assert_response_contains(result, "\"credentialValueRead\":false");
    assert_response_contains(result, "\"productionLiveReadAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON");
    assert_response_contains(
        result, "\"contract\":\"shard-route-preview-production-live-capture-archive-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\"");
    assert_response_contains(
        result, "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveCloseoutReleaseVersion\":\"Node v1656\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result, "\"sourceNodeArchiveModuleSplitOnly\":true");
    assert_response_contains(result,
                             "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceIntegrityReleaseVersion\":\"v1195\"");
    assert_response_contains(result, "\"sourceIntegrityPublishedStageCount\":25");
    assert_response_contains(result, "\"productionLiveCaptureArchiveNonParticipationReleaseVersion\":\"v1225\"");
    assert_response_contains(result, "\"plannedProductionLiveCaptureArchiveNonParticipationCheckCount\":30");
    assert_response_contains(result, "\"productionLiveCaptureArchiveNonParticipationOnly\":true");
    assert_response_contains(result, "\"nodeCaptureArchiveExecuted\":false");
    assert_response_contains(result, "\"nodeArchiveLoaderExecuted\":false");
    assert_response_contains(result, "\"miniKvStartsCaptureArchive\":false");
    assert_response_contains(result, "\"miniKvReadsArchiveEndpoint\":false");
    assert_response_contains(result, "\"archiveEndpointRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"archivePayloadImported\":false");
    assert_response_contains(result, "\"archiveFixtureImported\":false");
    assert_response_contains(result, "\"archiveRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON");
    assert_response_contains(
        result,
        "\"contract\":\"shard-route-preview-production-live-capture-archive-verification-non-participation.v1\"");
    assert_response_contains(result,
                             "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveVerificationCloseoutRe"
                                     "leaseVersion\":\"Node v1686\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result, "\"sourceNodeArchiveVerificationModuleSplitOnly\":true");
    assert_response_contains(
        result, "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceIntegrityReleaseVersion\":\"v1225\"");
    assert_response_contains(result, "\"sourceIntegrityPublishedStageCount\":30");
    assert_response_contains(result,
                             "\"productionLiveCaptureArchiveVerificationNonParticipationReleaseVersion\":\"v1260\"");
    assert_response_contains(result,
                             "\"plannedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":35");
    assert_response_contains(result, "\"productionLiveCaptureArchiveVerificationNonParticipationOnly\":true");
    assert_response_contains(result, "\"nodeCaptureArchiveVerificationExecuted\":false");
    assert_response_contains(result, "\"nodeArchiveVerificationLoaderExecuted\":false");
    assert_response_contains(result, "\"nodeArchiveVerificationRendererExecuted\":false");
    assert_response_contains(result, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_response_contains(result, "\"miniKvStartsCaptureArchiveVerification\":false");
    assert_response_contains(result, "\"miniKvReadsVerificationEndpoint\":false");
    assert_response_contains(result, "\"verificationEndpointRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"verificationPayloadImported\":false");
    assert_response_contains(result, "\"verificationFixtureImported\":false");
    assert_response_contains(result, "\"archiveVerificationRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON");
    assert_response_contains(
        result,
        "\"contract\":\"shard-route-preview-production-live-capture-release-evidence-review-non-participation.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureReleaseEvidenceReviewCloseout"
                                     "ReleaseVersion\":\"Node v1721\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeReleaseEvidenceReviewModuleSplitOnly\":true");
    assert_response_contains(
        result, "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceIntegrityReleaseVersion\":\"v1260\"");
    assert_response_contains(result, "\"sourceIntegrityPublishedStageCount\":35");
    assert_response_contains(result,
                             "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":\"v1295\"");
    assert_response_contains(result,
                             "\"plannedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":35");
    assert_response_contains(result, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationOnly\":true");
    assert_response_contains(result, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_response_contains(result, "\"nodeReleaseReviewLoaderExecuted\":false");
    assert_response_contains(result, "\"nodeReleaseReviewRendererExecuted\":false");
    assert_response_contains(result, "\"nodeReleaseReviewEndpointReadByMiniKv\":false");
    assert_response_contains(result, "\"nodeReleaseReviewDigestGeneratedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvStartsReleaseEvidenceReview\":false");
    assert_response_contains(result, "\"miniKvReadsReleaseReviewEndpoint\":false");
    assert_response_contains(result, "\"releaseReviewEndpointRead\":false");
    assert_response_contains(result, "\"rawEndpointParsed\":false");
    assert_response_contains(result, "\"releaseReviewPayloadImported\":false");
    assert_response_contains(result, "\"releaseReviewFixtureImported\":false");
    assert_response_contains(result, "\"releaseEvidenceReviewRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-catalog-entry-group-split-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeCatalogEntryGroupSplitCloseoutReleaseVersion\":\"Node v1756\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result, "\"sourceNodeCatalogEntryGroupSplitOnly\":true");
    assert_response_contains(result, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_response_contains(
        result,
        "\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceIntegrityReleaseVersion\":\"v1295\"");
    assert_response_contains(result, "\"sourceIntegrityPublishedStageCount\":35");
    assert_response_contains(result, "\"catalogEntryGroupSplitNonParticipationReleaseVersion\":\"v1360\"");
    assert_response_contains(result, "\"catalogEntryGroupSplitNonParticipationReleaseRangeStart\":\"v1331\"");
    assert_response_contains(result, "\"plannedCatalogEntryGroupSplitNonParticipationCheckCount\":30");
    assert_response_contains(result, "\"completedCatalogEntryGroupSplitNonParticipationCheckCount\":30");
    assert_response_contains(result, "\"catalogEntryGroupSplitNonParticipationOnly\":true");
    assert_response_contains(result, "\"nodeCatalogEntryRecordsImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeCatalogRegistryExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeCatalogMarkdownRenderedByMiniKv\":false");
    assert_response_contains(result, "\"nodeCatalogEndpointReadByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvStartsCatalogSplit\":false");
    assert_response_contains(result, "\"miniKvConsumesCatalogRecords\":false");
    assert_response_contains(result, "\"miniKvReadsCatalogEndpoint\":false");
    assert_response_contains(result, "\"catalogEndpointRead\":false");
    assert_response_contains(result, "\"catalogRecordsImported\":false");
    assert_response_contains(result, "\"catalogRegistryExecuted\":false");
    assert_response_contains(result, "\"catalogRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-preview-type-barrel-split-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeTypeBarrelSplitCloseoutReleaseVersion\":\"Node v1846\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result, "\"sourceNodeTypeBarrelSplitOnly\":true");
    assert_response_contains(result, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_response_contains(result,
                             "\"sourceIntegrityCommand\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceIntegrityReleaseVersion\":\"v1360\"");
    assert_response_contains(result, "\"sourceIntegrityPublishedStageCount\":30");
    assert_response_contains(result, "\"typeBarrelSplitNonParticipationReleaseVersion\":\"v1385\"");
    assert_response_contains(result, "\"typeBarrelSplitNonParticipationReleaseRangeStart\":\"v1361\"");
    assert_response_contains(result, "\"plannedTypeBarrelSplitNonParticipationCheckCount\":25");
    assert_response_contains(result, "\"completedTypeBarrelSplitNonParticipationCheckCount\":25");
    assert_response_contains(result, "\"typeBarrelSplitNonParticipationOnly\":true");
    assert_response_contains(result, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeAggregateReexportsImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeProfileBoundaryEndpointReadByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvStartsTypeBarrelSplit\":false");
    assert_response_contains(result, "\"miniKvImportsNodeTypes\":false");
    assert_response_contains(result, "\"miniKvConsumesProfileBoundaries\":false");
    assert_response_contains(result, "\"profileBoundaryEndpointRead\":false");
    assert_response_contains(result, "\"evidenceEndpointRead\":false");
    assert_response_contains(result, "\"typeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON");

    result = processor.execute("SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-type-barrel-split-follow-up-non-participation.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceNodeTypeBarrelSplitCloseoutReleaseVersion\":\"Node v1846\"");
    assert_response_contains(result, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_response_contains(result,
                             "\"sourceTypeBarrelSplitCommand\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\"");
    assert_response_contains(result, "\"sourceTypeBarrelSplitReleaseVersion\":\"v1385\"");
    assert_response_contains(result, "\"sourceTypeBarrelSplitPublishedStageCount\":25");
    assert_response_contains(result, "\"typeBarrelSplitFollowUpNonParticipationReleaseVersion\":\"v1405\"");
    assert_response_contains(result, "\"typeBarrelSplitFollowUpNonParticipationReleaseRangeStart\":\"v1386\"");
    assert_response_contains(result, "\"plannedTypeBarrelSplitFollowUpNonParticipationCheckCount\":20");
    assert_response_contains(result, "\"completedTypeBarrelSplitFollowUpNonParticipationCheckCount\":20");
    assert_response_contains(result, "\"typeBarrelSplitFollowUpNonParticipationOnly\":true");
    assert_response_contains(result, "\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true");
    assert_response_contains(result, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeAggregateReexportsImportedByMiniKv\":false");
    assert_response_contains(result, "\"nodeProfileBoundaryEndpointReadByMiniKv\":false");
    assert_response_contains(result, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_response_contains(result, "\"nodeVitestExecutedByMiniKv\":false");
    assert_response_contains(result, "\"miniKvStartsTypeBarrelFollowUp\":false");
    assert_response_contains(result, "\"miniKvImportsNodeTypes\":false");
    assert_response_contains(result, "\"miniKvReadsProfileEndpoint\":false");
    assert_response_contains(result, "\"profileBoundaryEndpointRead\":false");
    assert_response_contains(result, "\"evidenceEndpointRead\":false");
    assert_response_contains(result, "\"typeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON extra");
    assert(result.response == "ERR usage: SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON");
}

} // namespace minikv::command_test
