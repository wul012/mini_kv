#include "command_test_suites.hpp"

namespace minikv::command_test {

void run_command_shard_closeout_tests(CommandFixture& fixture) {
    auto& result = fixture.result;

    assert_response_contains(result, "\"observedCloseoutReleaseCount\":5");
    assert_response_contains(result, "\"closeoutRangeStart\":\"v194\"");
    assert_response_contains(result, "\"closeoutRangeEnd\":\"v198\"");
    assert_response_contains(result, "\"expectedCloseoutRangeSpan\":5");
    assert_response_contains(result, "\"observedCloseoutRangeSpan\":5");
    assert_response_contains(result, "\"closeoutEntryVersions\":[\"v194\",\"v195\",\"v196\",\"v197\",\"v198\"]");
    assert_response_contains(result, "\"closeoutIncludesReleaseCatalog\":true");
    assert_response_contains(result, "\"closeoutIncludesReleaseCatalogAudit\":true");
    assert_response_contains(result, "\"closeoutIncludesCatalogMaintenance\":true");
    assert_response_contains(result, "\"closeoutIncludesLatestEvidencePackage\":true");
    assert_response_contains(result, "\"closeoutIncludesLatestEvidencePackageAudit\":true");
    assert_response_contains(result, "\"releaseVersionsContiguous\":true");
    assert_response_contains(result, "\"duplicateCloseoutVersionsDetected\":false");
    assert_response_contains(result, "\"allCloseoutEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCloseoutFixturesVersioned\":true");
    assert_response_contains(result, "\"allCloseoutDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestCloseoutDigestMatchesCloseoutSource\":true");
    assert_response_contains(result, "\"nodeBatchValidationRecorded\":true");
    assert_response_contains(result, "\"nodeFullVitestCountMatchesPlan\":true");
    assert_response_contains(result, "\"nodeArchiveVerifierCheckCount\":16");
    assert_response_contains(result, "\"nodeArchiveVerifierChecksPassed\":16");
    assert_response_contains(result, "\"closeoutReadyForNextBatch\":true");
    assert_response_contains(result, "\"auditUsesVersionedFixture\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"closeoutAuditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity\":{\"continuityMode\":"
                                     "\"node-route-catalog-cleanup-post-closeout-continuity-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v379\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v379.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:01e736bc2963baae\"");
    assert_response_contains(result, "\"continuityStage\":\"feature-read-only-shard-route-preview-verification-report-"
                                     "archive-closeout-release-package\"");
    assert_response_contains(result, "\"stageSequence\":180");
    assert_response_contains(result, "\"stageReleaseVersion\":\"v380\"");
    assert_response_contains(result, "\"nodeBatchCloseoutVersion\":\"Node v549\"");
    assert_response_contains(result, "\"trackedMiniKvCloseoutRangeEnd\":\"v200\"");
    assert_response_contains(result, "\"trackedPostCloseoutRangeStart\":\"v201\"");
    assert_response_contains(result, "\"trackedPostCloseoutRangeEnd\":\"v380\"");
    assert_response_contains(result, "\"trackedPostCloseoutReleaseCount\":180");
    assert_response_contains(result, "\"nodePlanStillLatestForMiniKv\":true");
    assert_response_contains(result, "\"sourceFixtureVersioned\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"preservesBatchCloseoutAudit\":true");
    assert_response_contains(result, "\"preservesNodeV480Validation\":true");
    assert_response_contains(result, "\"archiveVerifierChecksPassed\":3");
    assert_response_contains(result, "\"postCloseoutStageOnly\":true");
    assert_response_contains(result, "\"readyForNextNodeBatch\":true");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"nodeRouteCatalogAnchorRemovalAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":\"docs/plans3/v467-post-route-catalog-anchor-removal-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v185\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v185.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_response_contains(result, "\"auditedNodeVersion\":\"Node v467\"");
    assert_response_contains(result, "\"sourceAnchorCompatibilityRemoved\":true");
    assert_response_contains(result, "\"centralRouteTablePureFlatMapConsumer\":true");
    assert_response_contains(result, "\"routeGroupTestsUseTypedCatalogIdentity\":true");
    assert_response_contains(result, "\"affectedRouteCatalogTestFileCount\":53");
    assert_response_contains(result, "\"affectedRouteCatalogTestCount\":59");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRouteAnchorDependencyIntroduced\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCloseoutSnapshot\":{\"snapshotMode\":"
                                     "\"node-route-catalog-final-closeout-snapshot-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v184\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v184.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_response_contains(result, "\"nodeFinalCloseoutVersion\":\"Node v464\"");
    assert_response_contains(result, "\"routeCatalogBatchStart\":\"Node v433\"");
    assert_response_contains(result, "\"routeCatalogBatchEnd\":\"Node v464\"");
    assert_response_contains(result, "\"coveredNodeVersionCount\":32");
    assert_response_contains(result, "\"focusedCloseoutTestFileCount\":7");
    assert_response_contains(result, "\"focusedCloseoutTestCount\":17");
    assert_response_contains(result, "\"nodeTypecheckPassed\":true");
    assert_response_contains(result, "\"nodeBuildPassed\":true");
    assert_response_contains(result, "\"nodeFullVitestFileCount\":393");
    assert_response_contains(result, "\"nodeFullVitestTestCount\":1221");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"snapshotOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"boundaryCatalogMaintenance\":{\"maintenanceMode\":"
                                     "\"boundary-field-catalog-split-contract-preserving\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v159\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v159.json\"");
    assert_response_contains(result, "\"boundaryFieldCatalogExtracted\":true");
    assert_response_contains(result, "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_response_contains(result, "\"publicShardJsonContractChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"nodeRouteGroupSplitCompatibilityPreserved\":true");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsJavaService\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"liveReadAllowed\":false");
    assert_response_contains(result, "\"activeShardPrototypeEnabled\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"writeCommandsAllowed\":false");
    assert_response_contains(result, "\"adminCommandsAllowed\":false");
    assert_response_contains(result, "\"loadRestoreCompactAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.routerActivationAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v29\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":"
                             "\"docs/plans3/"
                             "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_response_contains(result, "\"fieldCount\":821");
    assert_response_contains(result, "\"groupCount\":40");
    assert_response_contains(result, "\"fieldListGeneratedFromSharedVector\":true");
    assert_response_contains(result, "\"catalogGroups\":[\"top-level-read-only\",\"slot-table-preview\","
                                     "\"slot-table-preview-audit\",\"slot-table-preview-audit-maintenance\","
                                     "\"shard-readiness-release-catalog\",\"shard-readiness-release-catalog-audit\","
                                     "\"core-boundaries\"");
    assert_response_contains(result, "\"slotTablePreviewCataloged\":true");
    assert_response_contains(result, "\"slotTablePreviewAuditCataloged\":true");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenanceCataloged\":true");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogCataloged\":true");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowCataloged\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCloseoutSnapshotCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogAnchorRemovalAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotFreezeCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChainCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChainAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreezeCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenanceCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuityCataloged\":true");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenanceCataloged\":true");
    assert_response_contains(result, "\"boundaryCatalogMaintenancePreserved\":true");
    assert_response_contains(result, "\"slotTablePreview.routerActivationAllowed\"");
    assert_response_contains(result, "\"slotTablePreview.writeRoutingAllowed\"");
    assert_response_contains(result, "\"slotTablePreview.executionAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.contiguousSlotRange\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.executionAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.routerActivationAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.versionedFixtureCatalogOnly\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.rollingCurrentUsedForFrozenCatalog\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.routerActivationAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.writeRoutingAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.executionAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.contiguousReleaseRange\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.duplicateReleasesDetected\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.latestDigestMatchesFrozenSource\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.nodeAddsEvidenceGate\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.windowRangeNumbersParseable\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.windowCountMatchesRange\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.contiguousNodeVersionWindow\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.duplicateWindowVersionsDetected\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.latestWindowMatchesFrozenSource\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.readyForDownstreamConsumption\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.requiresVersionedFixture\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutHandoff.rollingCurrentAllowedAsHistoricalBaseline\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutHandoff.preservesNodeCleanupCloseoutValidation\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.handoffIncludedSectionsComplete\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.handoffReadyForDownstreamConsumption\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.rollingCurrentRejected\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.consumerGuidanceCountMatches\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.executionAllowed\"");
    assert_response_contains(
        result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.rollingCurrentUsedForFrozenBaseline\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.preservesHandoffAudit\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.frozenRollingCurrentRejected\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.catalogedReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.versionedFixtureCatalogOnly\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.allCatalogedEntriesReadOnly\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.executionAllowed\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.expectedCatalogedReleaseCount\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.releaseVersionsContiguous\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.latestDigestMatchesFrozenSource\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.extractedCatalogFormatter\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.preservesReleaseCatalogAudit\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.publicCatalogSectionsChanged\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.packageReleaseCount\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupLatestEvidencePackage.packageReadyForArchiveVerification\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.allPackagedEntriesReadOnly\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.executionAllowed\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.expectedPackageReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.releaseVersionsContiguous\"");
    assert_response_contains(
        result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.latestPackagedDigestMatchesPackageSource\"");
    assert_response_contains(
        result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.packageReadyForArchiveVerification\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.miniKvCloseoutReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.nodeBatchCloseoutVersion\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.futureNodeBatchMayConsumeCleanTags\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.closeoutReadyForNextBatch\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.executionAllowed\"");
    assert_response_contains(result,
                             "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.expectedCloseoutReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.releaseVersionsContiguous\"");
    assert_response_contains(
        result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.latestCloseoutDigestMatchesCloseoutSource\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.closeoutReadyForNextBatch\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.stageSequence\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.stageReleaseVersion\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.sourceFixtureVersioned\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.preservesBatchCloseoutAudit\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.readyForNextNodeBatch\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.executionAllowed\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.preservesFixtureParity\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.preservesArchiveCompatibility\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.preservesHistoricalFallback\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.changesShardJsonCommand\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.changesFixturePath\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.startsServices\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.runtimeProbeAllowed\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.routerActivationAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.writeRoutingAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.executionAllowed\"");
}

} // namespace minikv::command_test
