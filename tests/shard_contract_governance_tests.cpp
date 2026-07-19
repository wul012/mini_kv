#include "shard_test_suites.hpp"

namespace minikv::shard_test {

void check_shard_governance_contract(ShardFixture& fixture) {
    const auto& json = *fixture.active_json;
    assert_contains(json, "\"shardPreviewTypeBarrelSplitFollowUpNonParticipation\":{\"contract\":"
                          "\"shard-route-preview-type-barrel-split-follow-up-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceTypeBarrelSplitReleaseVersion\":\"v1385\"");
    assert_contains(json, "\"sourceTypeBarrelSplitPublishedStageCount\":25");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationReleaseVersion\":\"v1405\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationReleaseRangeStart\":\"v1386\"");
    assert_contains(json, "\"plannedTypeBarrelSplitFollowUpNonParticipationCheckCount\":20");
    assert_contains(json, "\"completedTypeBarrelSplitFollowUpNonParticipationCheckCount\":20");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"miniKvStartsTypeBarrelFollowUp\":false");
    assert_contains(json, "\"miniKvReadsProfileEndpoint\":false");
    assert_contains(json, "\"shardPreviewTypeBarrelSplitFollowUpFixtureAudit\":{\"contract\":"
                          "\"shard-route-preview-type-barrel-split-follow-up-fixture-audit.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON\"");
    assert_contains(json, "\"sourceNodeTestOnlyShellSplitCloseoutReleaseVersion\":\"Node v1866\"");
    assert_contains(json, "\"sourceFollowUpReleaseVersion\":\"v1405\"");
    assert_contains(json, "\"sourceFollowUpPublishedStageCount\":20");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditReleaseVersion\":\"v1417\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditReleaseRangeStart\":\"v1406\"");
    assert_contains(json, "\"plannedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":12");
    assert_contains(json, "\"completedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":12");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditValidationPassed\":true");
    assert_contains(json, "\"sourceFollowUpNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"nodeTestOnlyShellModulesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeCredentialResolverShellExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvRuntimeReadsSourceFixturePayload\":false");
    assert_contains(json, "\"shardPreviewDisabledPrecheckUpstreamEchoNonParticipation\":{\"contract\":"
                          "\"shard-route-disabled-precheck-upstream-echo-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeDisabledPrecheckUpstreamEchoCloseoutReleaseVersion\":\"Node v1878\"");
    assert_contains(json, "\"sourceFixtureAuditReleaseVersion\":\"v1417\"");
    assert_contains(json, "\"sourceFixtureAuditPublishedStageCount\":12");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationReleaseVersion\":\"v1442\"");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationReleaseRangeStart\":\"v1418\"");
    assert_contains(json, "\"plannedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":25");
    assert_contains(json, "\"completedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":25");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceFixtureAuditFrozen\":true");
    assert_contains(json, "\"nodeDisabledPrecheckStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledPrecheckCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewSandboxEndpointCredentialResolverUpstreamEchoNonParticipation\":{\"contract\":"
                          "\"shard-route-sandbox-endpoint-credential-resolver-upstream-echo-non-participation.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoCloseoutReleaseVersion\":\"Node v1903\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v114\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v105\"");
    assert_contains(json, "\"sourceDisabledPrecheckReleaseVersion\":\"v1442\"");
    assert_contains(json, "\"sourceDisabledPrecheckPublishedStageCount\":25");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseVersion\":\"v1472\"");
    assert_contains(json,
                    "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseRangeStart\":\"v1443\"");
    assert_contains(json, "\"plannedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":30");
    assert_contains(json, "\"completedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":30");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvReadsEndpointHandles\":false");
    assert_contains(json, "\"miniKvParsesRawEndpoint\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"shardPreviewImplementationPlanUpstreamEchoCloseoutNonParticipation\":{\"contract\":"
                          "\"shard-route-implementation-plan-upstream-echo-closeout-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeImplementationPlanUpstreamEchoCloseoutReleaseVersion\":\"Node v1934\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v126\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v121\"");
    assert_contains(json, "\"sourceSandboxEndpointReleaseVersion\":\"v1472\"");
    assert_contains(json, "\"sourceSandboxEndpointPublishedStageCount\":30");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseVersion\":\"v1475\"");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseRangeStart\":\"v1473\"");
    assert_contains(json, "\"plannedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":8");
    assert_contains(json, "\"completedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":8");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true");
    assert_contains(json, "\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"shardPreviewReleaseWindowReadinessPacketSplitNonParticipation\":{\"contract\":"
                          "\"shard-route-release-window-readiness-packet-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeReleaseWindowReadinessPacketSplitCloseoutReleaseVersion\":\"Node v1937\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v70\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v61\"");
    assert_contains(json, "\"sourceImplementationPlanReleaseVersion\":\"v1475\"");
    assert_contains(json, "\"sourceImplementationPlanPublishedStageCount\":3");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationReleaseVersion\":\"v1490\"");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationReleaseRangeStart\":\"v1476\"");
    assert_contains(json, "\"plannedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":15");
    assert_contains(json, "\"completedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":15");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeProductionReleaseDryRunEnvelopeExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeStatusRoutesExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvExecutesReleaseWindowPacket\":false");
    assert_contains(json, "\"miniKvReplaysRestoreDrillEvidence\":false");
    assert_contains(
        json, "\"shardPreviewDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipation\":{\"contract\":"
              "\"shard-route-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation.v1\"");
    assert_contains(
        json, "\"command\":\"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON\"");
    assert_contains(
        json,
        "\"sourceNodeDisabledFakeHarnessContractUpstreamEchoVerificationSplitCloseoutReleaseVersion\":\"Node v1967\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v127\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersionRange\":\"Java v122-v126\"");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitReleaseVersion\":\"v1490\"");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitPublishedStageCount\":15");
    assert_contains(
        json, "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_contains(
        json,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseRangeStart\":\"v1491\"");
    assert_contains(json,
                    "\"plannedDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheckCount\":15");
    assert_contains(
        json, "\"completedDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationCheckCount\":15");
    assert_contains(
        json, "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceReleaseWindowReadinessPacketSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoPublicEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoReferencesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoPolicyExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoCoreExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessContractUpstreamEchoVerificationExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledFakeHarnessExecutionDeniedRoutePreflightExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvParsesHistoricalReceipts\":false");
    assert_contains(json, "\"miniKvReadsHistoricalMiniKvV127ReceiptFile\":false");
    assert_contains(json, "\"miniKvScansJavaEvidence\":false");
    assert_contains(json, "\"shardPreviewBlockedExecutionRehearsalSplitNonParticipation\":{\"contract\":"
                          "\"shard-route-blocked-execution-rehearsal-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeBlockedExecutionRehearsalSplitCloseoutReleaseVersion\":\"Node v1982\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v99\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"wal-regression-evidence\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v90\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaEvidence\":\"context-normalization-evidence\"");
    assert_contains(
        json,
        "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_contains(json, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitPublishedStageCount\":15");
    assert_contains(json, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitComplete\":true");
    assert_contains(json, "\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_contains(json, "\"blockedExecutionRehearsalSplitNonParticipationReleaseRangeStart\":\"v1506\"");
    assert_contains(json, "\"plannedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"completedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvReadsHistoricalMiniKvV99WalRegressionFile\":false");
    assert_contains(json, "\"miniKvScansJavaV90ContextNormalizationEvidence\":false");
    assert_contains(json, "\"miniKvReplaysWalRegression\":false");
    assert_contains(json, "\"shardPreviewPrecheckUpstreamReceiptVerificationSplitNonParticipation\":{\"contract\":"
                          "\"shard-route-precheck-upstream-receipt-verification-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceNodePrecheckUpstreamReceiptVerificationSplitCloseoutReleaseVersion\":\"Node v2002\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v108\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"non-participation-evidence\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v99\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaEvidence\":\"receipt-evidence\"");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationPublishedStageCount\":20");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationComplete\":true");
    assert_contains(json, "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v1545\"");
    assert_contains(json, "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseRangeStart\":\"v1526\"");
    assert_contains(json, "\"plannedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"completedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"precheckUpstreamReceiptVerificationSplitNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationPublicLoaderImportedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationReferencesParsedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationCheckPolicyExecutedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationMessagePolicyExecutedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodePrecheckUpstreamReceiptVerificationEntrypointExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeCodeHealthScanExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeLargeFileInventoryMutatedByMiniKv\":false");
    assert_contains(json, "\"nodeDirectTestsExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDownstreamCodeHealthExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeDownstreamRehearsalGuardExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvReadsHistoricalMiniKvV108NonParticipationFile\":false");
    assert_contains(json, "\"miniKvScansJavaV99Evidence\":false");
    assert_contains(json, "\"miniKvExecutesDownstreamChecks\":false");
    assert_contains(json, "\"miniKvMutatesLargeFileInventory\":false");
    assert_contains(
        json, "\"shardPreviewRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipation\":{\"contract\":"
              "\"shard-route-runtime-execution-packet-approval-gate-archive-verification-non-participation.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeArchiveVerificationReleaseVersion\":\"Node v399\"");
    assert_contains(json, "\"sourceNodeVerifiedArchiveReleaseVersion\":\"Node v398\"");
    assert_contains(json, "\"sourceNodeNextConsumer\":\"Node v400\"");
    assert_contains(json, "\"sourcePrecheckUpstreamReceiptVerificationSplitReleaseVersion\":\"v1545\"");
    assert_contains(json, "\"sourcePrecheckUpstreamReceiptVerificationSplitPublishedStageCount\":20");
    assert_contains(json, "\"sourcePrecheckUpstreamReceiptVerificationSplitComplete\":true");
    assert_contains(
        json, "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseVersion\":\"v1585\"");
    assert_contains(
        json, "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseRangeStart\":\"v1566\"");
    assert_contains(
        json, "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseRangeEnd\":\"v1585\"");
    assert_contains(json,
                    "\"plannedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":20");
    assert_contains(json,
                    "\"completedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":20");
    assert_contains(json,
                    "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationValidationPassed\":true");
    assert_contains(json, "\"archiveVerificationOnly\":true");
    assert_contains(json, "\"approvalInputCount\":0");
    assert_contains(json, "\"requiredApprovalInputCount\":3");
    assert_contains(json, "\"runtimeArtifactCount\":0");
    assert_contains(json, "\"requiredRuntimeArtifactCount\":6");
    assert_contains(json, "\"nodeV400MustNotTreatArchiveVerificationAsApproval\":true");
    assert_contains(json, "\"miniKvReadsNodeArchiveFiles\":false");
    assert_contains(json, "\"miniKvReplaysNodeArchive\":false");
    assert_contains(json, "\"miniKvStartsNodeServices\":false");
    assert_contains(json, "\"miniKvStartsJavaService\":false");
    assert_contains(json, "\"miniKvStartsMiniKvService\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"miniKvParsesRawEndpoint\":false");
    assert_contains(json, "\"miniKvExecutesRuntimeProbe\":false");
    assert_contains(json, "\"miniKvEnablesActiveRouter\":false");
    assert_contains(json, "\"runtimeExecutionPacketPresent\":false");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"shardPreviewCodeWalkthroughQualityGateNonParticipation\":{\"contract\":"
                          "\"shard-route-code-walkthrough-quality-gate-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodeCodeWalkthroughQualityGateReleaseVersion\":\"Node v2077\"");
    assert_contains(json, "\"sourceNodeQualityRunRange\":\"Node v2068-v2077\"");
    assert_contains(json, "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationReleaseVersion\":\"v1585\"");
    assert_contains(json,
                    "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationFixturePath\":\"fixtures/release/"
                    "shard-readiness-v1585.json\"");
    assert_contains(json, "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationPublishedStageCount\":20");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1590\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1590.json\"");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationReleaseRangeStart\":\"v1586\"");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationReleaseRangeEnd\":\"v1591\"");
    assert_contains(json, "\"plannedCodeWalkthroughQualityGateNonParticipationCheckCount\":12");
    assert_contains(json, "\"completedCodeWalkthroughQualityGateNonParticipationCheckCount\":12");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationValidationPassed\":true");
    assert_contains(json, "\"walkthroughQualityGateOnly\":true");
    assert_contains(json, "\"codeWalkthroughMayBeOmittedForTinyMaintenance\":true");
    assert_contains(json, "\"placeholderWalkthroughAllowed\":false");
    assert_contains(json, "\"unsafeProductionClaimAllowed\":false");
    assert_contains(json, "\"bulkHistoricalRelocationAllowed\":false");
    assert_contains(json, "\"nodeWalkthroughFilesReadByMiniKv\":false");
    assert_contains(json, "\"miniKvScansNodeRepository\":false");
    assert_contains(json, "\"miniKvExecutesNodeQualityRoute\":false");
    assert_contains(json, "\"miniKvMutatesNodeDocs\":false");
    assert_contains(json, "\"miniKvMutatesJavaDocs\":false");
    assert_contains(json, "\"shardPreviewProductionShardExecutionOwnerReceiptRequestPacket\":{\"contract\":"
                          "\"shard-production-shard-execution-owner-receipt-request-packet.v1\"");
    assert_contains(json, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON\"");
    assert_contains(json, "\"sourceNodeProductionShardExecutionReleaseVersion\":\"Node v2087\"");
    assert_contains(json, "\"sourceNodeProductionShardExecutionProfile\":"
                          "\"production-shard-execution-owner-receipt-request-packet\"");
    assert_contains(json, "\"sourceNodeWaitsForMiniKv\":false");
    assert_contains(json, "\"sourceNodeRequiresExistingSignedReceipts\":false");
    assert_contains(json, "\"sourceNodeAllowsJavaMiniKvParallel\":true");
    assert_contains(json, "\"sourceCodeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1595\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1595.json\"");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseRangeStart\":\"v1592\"");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseRangeEnd\":\"v1596\"");
    assert_contains(json, "\"publishedStageCount\":5");
    assert_contains(json, "\"plannedStageCount\":5");
    assert_contains(json, "\"requestedReceiptSlotCount\":9");
    assert_contains(json, "\"ownerReceiptRequestPacketOnly\":true");
    assert_contains(json, "\"signedMiniKvReceiptPresent\":false");
    assert_contains(json, "\"signedJavaReceiptPresent\":false");
    assert_contains(json, "\"signedCrossProjectCleanupReceiptPresent\":false");
    assert_contains(json, "\"perVersionArchiveRequired\":true");
    assert_contains(json, "\"combinedArchiveFolderAllowed\":false");
    assert_contains(json,
                    "\"versionedArchiveDirectories\":[\"e/1592/\",\"e/1593/\",\"e/1594/\",\"e/1595/\",\"e/1596/\"]");
    assert_contains(json, "\"completedProductionShardExecutionOwnerReceiptRequestPacketCheckCount\":12");
    assert_contains(json, "\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":true");
    assert_contains(json, "\"productionShardExecutionEnabled\":false");
    assert_contains(json, "\"managedAuditStoreBound\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"startsNodeServices\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewProductionShardExecutionMiniKvOwnerReceiptBundle\":{\"contract\":"
                          "\"shard-production-shard-execution-mini-kv-owner-receipt-bundle.v1\"");
    assert_contains(json, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON\"");
    assert_contains(json, "\"sourceOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
    assert_contains(json, "\"sourceOwnerReceiptRequestPacketComplete\":true");
    assert_contains(json, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1600\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1600.json\"");
    assert_contains(json, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseRangeStart\":\"v1597\"");
    assert_contains(json, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseRangeEnd\":\"v1601\"");
    assert_contains(json, "\"miniKvReceiptSlotCount\":4");
    assert_contains(json, "\"miniKvAbortOwnerReceiptPresent\":true");
    assert_contains(json, "\"miniKvRollbackOwnerReceiptPresent\":true");
    assert_contains(json, "\"miniKvLifecycleOwnerReceiptPresent\":true");
    assert_contains(json, "\"miniKvCleanupNoStartProofPresent\":true");
    assert_contains(json, "\"miniKvReceiptsComplete\":true");
    assert_contains(json, "\"javaOwnerReceiptsPresent\":false");
    assert_contains(json, "\"crossProjectCleanupReconciliationReceiptPresent\":false");
    assert_contains(json, "\"archiveRoot\":\"f\"");
    assert_contains(json, "\"archiveRootIsF\":true");
    assert_contains(json,
                    "\"versionedArchiveDirectories\":[\"f/1597/\",\"f/1598/\",\"f/1599/\",\"f/1600/\",\"f/1601/\"]");
    assert_contains(json, "\"completedProductionShardExecutionMiniKvOwnerReceiptBundleCheckCount\":12");
    assert_contains(json, "\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":true");
    assert_contains(json, "\"abortExecutionPerformed\":false");
    assert_contains(json, "\"rollbackExecutionPerformed\":false");
    assert_contains(json, "\"lifecycleServiceStarted\":false");
    assert_contains(json, "\"cleanupExecutionPerformed\":false");
    assert_contains(json, "\"shardPreviewProductionShardExecutionExternalArtifactDryRunCloseout\":{\"contract\":"
                          "\"shard-production-shard-execution-external-artifact-dry-run-closeout.v1\"");
    assert_contains(json, "\"command\":\"SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON\"");
    assert_contains(json, "\"sourceMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_contains(json, "\"sourceMiniKvOwnerReceiptBundleComplete\":true");
    assert_contains(json, "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseVersion\":\"v1606\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1605\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1605.json\"");
    assert_contains(json, "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseRangeStart\":\"v1602\"");
    assert_contains(json, "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseRangeEnd\":\"v1606\"");
    assert_contains(json, "\"dryRunBatchClosed\":true");
    assert_contains(json, "\"nodeOnlyGrowthStopped\":true");
    assert_contains(json, "\"missingRealArtifactBlockerListed\":true");
    assert_contains(json, "\"realExternalArtifactPresent\":false");
    assert_contains(json, "\"externalArtifactParsed\":false");
    assert_contains(json, "\"externalArtifactAccepted\":false");
    assert_contains(json, "\"externalArtifactStored\":false");
    assert_contains(json, "\"dryRunArtifactExecuted\":false");
    assert_contains(json, "\"realSignedApprovalPresent\":false");
    assert_contains(json, "\"managedAuditOwnerBindingPresent\":false");
    assert_contains(json, "\"realJavaReceiptPresent\":false");
    assert_contains(json, "\"realMiniKvSignedReceiptPresent\":false");
    assert_contains(json, "\"realCleanupReceiptPresent\":false");
    assert_contains(json, "\"archiveRoot\":\"f\"");
    assert_contains(json,
                    "\"versionedArchiveDirectories\":[\"f/1602/\",\"f/1603/\",\"f/1604/\",\"f/1605/\",\"f/1606/\"]");
    assert_contains(json, "\"completedProductionShardExecutionExternalArtifactDryRunCloseoutCheckCount\":12");
    assert_contains(json, "\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":true");
    assert_contains(json, "\"realExternalArtifactAbsent\":true");
    assert_contains(json, "\"realReceiptsMissing\":true");
    assert_contains(json, "\"productionShardExecutionEnabled\":false");
    assert_contains(json, "\"managedAuditStoreBound\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardPreviewFFolderExplanationQualityCloseout\":{\"contract\":"
                          "\"shard-f-folder-explanation-quality-closeout.v1\"");
    assert_contains(json, "\"command\":\"SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/v2103-f-folder-explanation-expansion-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceExternalArtifactCloseoutReleaseVersion\":\"v1606\"");
    assert_contains(json, "\"sourceExternalArtifactCloseoutComplete\":true");
    assert_contains(json, "\"fFolderExplanationQualityCloseoutReleaseVersion\":\"v1607\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1606\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1606.json\"");
    assert_contains(json, "\"nodeV2103CloseoutObserved\":true");
    assert_contains(json, "\"longChineseExplanationsRequired\":true");
    assert_contains(json, "\"chineseWritingRequired\":true");
    assert_contains(json, "\"minimumWalkthroughChineseCharacterCount\":3000");
    assert_contains(json, "\"insufficientWordCountExpandsVersionWorkload\":true");
    assert_contains(json, "\"fillerPaddingAllowed\":false");
    assert_contains(json, "\"shortExplanationsRejected\":true");
    assert_contains(json, "\"plannedFFolderExplanationQualityRuleCount\":13");
    assert_contains(json, "\"publishedFFolderExplanationQualityRuleCount\":13");
    assert_contains(json, "\"plannedFFolderExplanationQualityCloseoutCheckCount\":19");
    assert_contains(json, "\"completedFFolderExplanationQualityCloseoutCheckCount\":19");
    assert_contains(json, "\"singleVersionLargeScope\":true");
    assert_contains(json, "\"multiVersionFragmentationAllowed\":false");
    assert_contains(json, "\"pictureDirectoryCreatedOnlyWithRealScreenshot\":true");
    assert_contains(json, "\"fFolderExplanationQualityCloseoutValidationPassed\":true");
    assert_contains(json, "\"scansNodeFolders\":false");
    assert_contains(json, "\"importsNodeQualityRoute\":false");
    assert_contains(json, "\"createsPictureDirsWithoutEvidence\":false");
    assert_contains(json, "\"rewritesHistoricalWalkthroughs\":false");
    assert_contains(json, "\"startsNodeServices\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardId\":\"shard-0\"");
    assert_contains(json, "\"storagePath\":\"not-created\"");
    assert_contains(json, "\"writesAllowed\":false");
    assert_contains(json, "\"key\":\"orderops:alpha\",\"slot\":5,\"shardId\":\"shard-0\"");
    assert_contains(json, "\"key\":\"audit:receipt:001\",\"slot\":9,\"shardId\":\"shard-0\"");
}

} // namespace minikv::shard_test
