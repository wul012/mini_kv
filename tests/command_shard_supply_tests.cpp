#include "command_test_suites.hpp"

namespace minikv::command_test {

void run_command_shard_supply_tests(CommandFixture& fixture) {
    auto& processor = fixture.processor;
    auto& result = fixture.result;

    result = processor.execute("SHARDJSON");
    assert_response_contains(result, "\"approvalPacketRequired\":true");
    assert_response_contains(result, "\"approvalPacketPresent\":false");
    assert_response_contains(result, "\"readyForOperatorValueSupplyApproval\":false");
    assert_response_contains(result,
                             "\"envelopeMode\":\"controlled-read-only-disabled-operator-value-supply-envelope\"");
    assert_response_contains(result, "\"sourceValueDraftCommand\":\"SHARDROUTEVALUEDRAFTJSON\"");
    assert_response_contains(result, "\"sourceValueDraftReleaseVersion\":\"v585\"");
    assert_response_contains(result, "\"operatorValueEnvelopeState\":\"disabled-by-default\"");
    assert_response_contains(result, "\"missingValuePolicy\":\"fail-closed\"");
    assert_response_contains(result, "\"redactionPolicy\":\"redact-before-persist\"");
    assert_response_contains(result, "\"readyForDisabledValueSupplyEnvelope\":true");
    assert_response_contains(result, "\"readyForOperatorValueSupply\":false");
    assert_response_contains(result, "\"automaticSiblingImportAllowed\":false");
    assert_response_contains(result, "\"valueSupplySlotHelperApplied\":true");
    assert_response_contains(result, "\"singleValueSupplyEnvelopeSectionExpected\":true");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v379\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v879.json\"");
    assert_response_contains(result, "\"archivedArchiveReleaseRangeStart\":\"v341\"");
    assert_response_contains(result, "\"archivedArchiveReleaseRangeEnd\":\"v360\"");
    assert_response_contains(result, "\"archivedArchiveReleaseCount\":20");
    assert_response_contains(result, "\"archiveCloseoutCommandAvailable\":true");
    assert_response_contains(result, "\"archiveCloseoutChainComplete\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog\":{\"catalogMode\":"
                                     "\"versioned-shard-readiness-release-catalog-read-only\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":\"docs/plans3/"
                             "v431-post-credential-resolver-credential-handle-approval-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v164\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v164.json\"");
    assert_response_contains(result, "\"catalogedReleaseCount\":4");
    assert_response_contains(result, "\"latestCatalogedReleaseVersion\":\"v164\"");
    assert_response_contains(result, "\"latestCatalogedDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_response_contains(result, "\"catalogedSections\":[\"boundaryCatalogIndex\",\"slotTablePreview\","
                                     "\"slotTablePreviewAudit\",\"slotTablePreviewAuditMaintenance\"]");
    assert_response_contains(result, "\"primarySection\":\"slotTablePreviewAuditMaintenance\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v5\"");
    assert_response_contains(result, "\"versionedFixtureCatalogOnly\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_response_contains(result, "\"publicShardJsonContractChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit\":{\"auditMode\":"
                                     "\"shard-readiness-release-catalog-consistency-read-only\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/"
                "v432-post-credential-resolver-endpoint-handle-allowlist-approval-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v165\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v165.json\"");
    assert_response_contains(result, "\"auditedCatalogSection\":\"shardReadinessReleaseCatalog\"");
    assert_response_contains(result, "\"expectedReleaseCount\":4");
    assert_response_contains(result, "\"observedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeStart\":\"v161\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v164\"");
    assert_response_contains(result, "\"contiguousReleaseRange\":true");
    assert_response_contains(result, "\"duplicateReleasesDetected\":false");
    assert_response_contains(result, "\"missingCatalogEntriesDetected\":false");
    assert_response_contains(result, "\"allCatalogedDigestsPresent\":true");
    assert_response_contains(result, "\"allCatalogedFixturesVersioned\":true");
    assert_response_contains(result, "\"allCatalogedEntriesReadOnly\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestDigestMatchesFrozenSource\":true");
    assert_response_contains(result, "\"catalogOnlyAudit\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"archivedNodeEvidenceMutated\":false");
    assert_response_contains(result, "\"commandCatalog\":{\"command\":\"SHARDJSON\",\"category\":\"read\"");
    assert_response_contains(result, "\"commandCatalogQuality\":{\"releaseRangeStart\":\"v1546\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v1607\"");
    assert_response_contains(result,
                             "\"scope\":\"command-catalog-single-source-refactor-plus-code-walkthrough-owner-request-"
                             "mini-kv-receipt-external-artifact-and-f-folder-explanation-quality-gates\"");
    assert_response_contains(result, "\"commandCount\":94");
    assert_response_contains(result, "\"dispatchCatalogSplit\":true");
    assert_response_contains(result, "\"contractCatalogDeduplicated\":true");
    assert_response_contains(result, "\"helpTextGeneratedFromCatalog\":true");
    assert_response_contains(result, "\"completionCandidatesGeneratedFromCatalog\":true");
    assert_response_contains(result, "\"runtimeCommandBehaviorChanged\":false");
    assert_response_contains(result, "\"storageBehaviorChanged\":false");
    assert_response_contains(result, "\"shardPreviewFFolderExplanationQualityCloseout\":{\"contract\":\"shard-f-folder-"
                                     "explanation-quality-closeout.v1\"");
    assert_response_contains(result, "\"fFolderExplanationQualityCloseoutReleaseVersion\":\"v1607\"");
    assert_response_contains(result, "\"sourceExternalArtifactCloseoutReleaseVersion\":\"v1606\"");
    assert_response_contains(result, "\"minimumWalkthroughChineseCharacterCount\":3000");
    assert_response_contains(result, "\"fillerPaddingAllowed\":false");
    assert_response_contains(result, "\"fFolderExplanationQualityCloseoutValidationPassed\":true");
    assert_response_contains(result,
                             "\"fixtureParity\":{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\"");
    assert_response_contains(result, "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_response_contains(result, "\"historicalFallback\":{\"previousConsumedReleaseVersion\":\"v419\"");
    assert_response_contains(result, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v419.json\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance\":{\"maintenanceMode\":"
                                     "\"history-fixture-archive-formatter-split-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v179\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v179.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_response_contains(result, "\"splitFrom\":\"src/shard_readiness.cpp\"");
    assert_response_contains(result, "\"splitInto\":\"src/shard_readiness_history.cpp\"");
    assert_response_contains(result, "\"preservesFixtureParity\":true");
    assert_response_contains(result, "\"preservesArchiveCompatibility\":true");
    assert_response_contains(result, "\"preservesHistoricalFallback\":true");
    assert_response_contains(result, "\"changesShardJsonCommand\":false");
    assert_response_contains(result, "\"changesFixturePath\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"rollingCurrentUsedForHistoricalBaseline\":false");
    assert_response_contains(result, "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_response_contains(result, "\"activeShardPrototypeAllowed\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenRouterActivationAllowed\":false");
    assert_response_contains(result, "\"frozenWriteRoutingAllowed\":false");
    assert_response_contains(result, "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_response_contains(result, "\"readyForNodeConsumption\":true");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_response_contains(result, "\"liveReadGateAllowed\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"requiresCleanup\":true");
    assert_response_contains(result, "\"liveReadGatePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenRuntimeProbeAllowed\":false");
    assert_response_contains(result, "\"frozenExecutionAllowed\":false");
    assert_response_contains(result,
                             "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_response_contains(result, "\"operatorOwnedServiceLifecycleRequired\":true");
    assert_response_contains(result, "\"serviceOwnerDeclared\":false");
    assert_response_contains(result, "\"getOnlySmokeTargetDeclared\":false");
    assert_response_contains(result, "\"operatorServiceLifecycleTemplateFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenServiceOwnerDeclared\":false");
    assert_response_contains(
        result, "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_response_contains(result, "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_response_contains(result, "\"serviceOwnerDeclared\":true");
    assert_response_contains(result, "\"startupCommandDeclared\":true");
    assert_response_contains(result, "\"portListDeclared\":true");
    assert_response_contains(result, "\"getOnlySmokeTargetDeclared\":true");
    assert_response_contains(result, "\"runtimeGateApproved\":false");
    assert_response_contains(result, "\"requiresSeparateRuntimeGate\":true");
    assert_response_contains(result, "\"operatorServiceLifecycleEvidenceFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenRuntimeGateApproved\":false");
    assert_response_contains(result, "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                                     "\"blocked-missing-runtime-execution-artifacts\"");
    assert_response_contains(result, "\"readyForRuntimeExecutionPacket\":false");
    assert_response_contains(result, "\"runtimeExecutionArtifactsComplete\":false");
    assert_response_contains(result, "\"presentRuntimeExecutionArtifactCount\":0");
    assert_response_contains(result, "\"missingRuntimeExecutionArtifactCount\":6");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"executionAttempted\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"failClosedOnMissingArtifacts\":true");
    assert_response_contains(result,
                             "\"runtimeExecutionArtifactIntakePreflightFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                                     "\"mini-kv-side-candidate-no-runtime\"");
    assert_response_contains(result, "\"candidateArtifactCount\":4");
    assert_response_contains(result, "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_response_contains(result, "\"operatorApprovalRecordPresent\":false");
    assert_response_contains(result, "\"nodeRuntimeWindowApproved\":false");
    assert_response_contains(result, "\"miniKvLoopbackPortCandidate\":6424");
    assert_response_contains(result, "\"miniKvLoopbackPortOperatorApproved\":false");
    assert_response_contains(result, "\"getOnlySmokeCommandOperatorApproved\":false");
    assert_response_contains(result, "\"serviceOwnerCandidateDeclared\":true");
    assert_response_contains(result, "\"cleanupProofPresent\":false");
    assert_response_contains(result, "\"requiresCrossProjectPacket\":true");
    assert_response_contains(result,
                             "\"miniKvRuntimeExecutionArtifactCandidateFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_response_contains(result, "\"preservesCandidateOnlyContribution\":true");
    assert_response_contains(result, "\"frozenAcceptedRuntimeExecutionArtifactCount\":0");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                                     "\"blocked-missing-approval-gate-inputs\"");
    assert_response_contains(result, "\"approvalGateInputCount\":0");
    assert_response_contains(result, "\"missingApprovalGateInputCount\":3");
    assert_response_contains(result, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_response_contains(result, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_response_contains(result, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_response_contains(result, "\"approvalGateInputsComplete\":false");
    assert_response_contains(result, "\"runtimeGateApprovalPresent\":false");
    assert_response_contains(result, "\"concreteLoopbackPortsAssigned\":false");
    assert_response_contains(result, "\"startsJavaService\":false");
    assert_response_contains(result, "\"failClosedOnMissingApprovalInputs\":true");
    assert_response_contains(result,
                             "\"runtimeExecutionApprovalGateInputPrecheckFreeze\":{\"frozenReleaseVersion\":\"v155\"");
    assert_response_contains(result, "\"preservesApprovalGateInputPrecheck\":true");
    assert_response_contains(result, "\"frozenApprovalGateInputCount\":0");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(
        result, "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_response_contains(result, "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_response_contains(result, "\"miniKvApprovalGateInputComplete\":true");
    assert_response_contains(result, "\"miniKvApprovalGateInputItemCount\":4");
    assert_response_contains(result, "\"miniKvLoopbackPort\":6424");
    assert_response_contains(result, "\"serviceOwnerConfirmed\":true");
    assert_response_contains(result, "\"processCleanupRulesComplete\":true");
    assert_response_contains(result, "\"stopOnlyOwnedProcessCleanupRequired\":true");
    assert_response_contains(result, "\"cleanupProofPresent\":false");
    assert_response_contains(result, "\"cleanupProofRequiresApprovedRuntimeStart\":true");
    assert_response_contains(result, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_response_contains(result, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_response_contains(result, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"requiresNodeRuntimeWindow\":true");
    assert_response_contains(result, "\"requiresCorrelatedOperatorApproval\":true");
    assert_response_contains(result, "\"requiresCompleteCrossProjectPacket\":true");
    assert_response_contains(result, "\"miniKvFinalApprovalGateInputFreeze\":{\"frozenReleaseVersion\":\"v156\"");
    assert_response_contains(result, "\"preservesMiniKvFinalApprovalGateInput\":true");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"frozenRouterActivationAllowed\":false");
    assert_response_contains(result, "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                                     "\"template-validator-echo-no-canonical-inputs\"");
    assert_response_contains(
        result, "\"sourceNodeValidatorVersion\":\"Node v402 runtime execution approval input template validator\"");
    assert_response_contains(result, "\"templateArchiveRoot\":\"e/402/input-templates\"");
    assert_response_contains(result, "\"canonicalInputRoot\":\"e/398/input\"");
    assert_response_contains(result, "\"templateOnlyInputCount\":3");
    assert_response_contains(result, "\"canonicalRuntimeInputPresent\":false");
    assert_response_contains(result, "\"templateCopiedToCanonicalInput\":false");
    assert_response_contains(result, "\"nodeApprovedRuntimeWindowCanonicalPresent\":false");
    assert_response_contains(result, "\"correlatedOperatorApprovalRecordCanonicalPresent\":false");
    assert_response_contains(result, "\"completeCrossProjectRuntimeExecutionPacketCanonicalPresent\":false");
    assert_response_contains(result, "\"templatesAuthorizeRuntime\":false");
    assert_response_contains(result, "\"writesCanonicalApprovalInputFiles\":false");
    assert_response_contains(result, "\"changesNodeInputTemplateFiles\":false");
    assert_response_contains(result, "\"failClosedOnTemplateOnlyInputs\":true");
    assert_response_contains(
        result, "\"runtimeExecutionApprovalInputTemplateValidatorEchoFreeze\":{\"frozenReleaseVersion\":\"v157\"");
    assert_response_contains(result, "\"preservesTemplateValidatorEcho\":true");
    assert_response_contains(result, "\"frozenCanonicalRuntimeInputPresent\":false");
    assert_response_contains(result, "\"frozenTemplateCopiedToCanonicalInput\":false");
    assert_response_contains(result, "\"frozenTemplatesAuthorizeRuntime\":false");
    assert_response_contains(result, "\"frozenExecutionAllowed\":false");
    assert_response_contains(result, "\"runtimeExecutionCanonicalApprovalInputPrecheck\":{\"precheckMode\":"
                                     "\"blocked-missing-canonical-approval-inputs\"");
    assert_response_contains(result, "\"sourceNodeCompatibilityVersion\":\"Node v403 template compatibility intake\"");
    assert_response_contains(result, "\"requiredCanonicalInputCount\":3");
    assert_response_contains(result, "\"presentCanonicalInputCount\":0");
    assert_response_contains(result, "\"missingCanonicalInputCount\":3");
    assert_response_contains(result, "\"canonicalApprovalInputsComplete\":false");
    assert_response_contains(result, "\"sharedApprovalCorrelationIdValidated\":false");
    assert_response_contains(result, "\"templatesAcceptedAsCanonicalInputs\":false");
    assert_response_contains(result, "\"templateCompatibilityEvidenceAcceptedAsApproval\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketPresent\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"requiresRealCanonicalInputs\":true");
}

} // namespace minikv::command_test
