#include "command_test_suites.hpp"

namespace minikv::command_test {

void run_command_shard_compat_tests(CommandFixture& fixture) {
    auto& result = fixture.result;

    assert_response_contains(result, "\"failClosedOnMissingCanonicalInputs\":true");
    assert_response_contains(result, "\"nodeRouteGroupSplitCompatibility\":{\"compatibilityMode\":"
                                     "\"node-route-group-split-contract-stable\"");
    assert_response_contains(
        result, "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\"");
    assert_response_contains(result, "\"nodeChangeType\":\"route-registration-refactor-only\"");
    assert_response_contains(result, "\"nodeApiPathChanged\":false");
    assert_response_contains(result, "\"nodeResponseShapeChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"nodeStartsMiniKvService\":false");
    assert_response_contains(result, "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_response_contains(result, "\"miniKvFixturePathChanged\":false");
    assert_response_contains(result, "\"miniKvShardJsonCommandChanged\":false");
    assert_response_contains(result, "\"miniKvRequiresNodeRouteTableChange\":false");
    assert_response_contains(result, "\"archiveCompatibilityPreserved\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                     "\"node-v433-v472-route-catalog-cleanup-closeout-window-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v472-post-route-catalog-cleanup-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_response_contains(result, "\"windowStartNodeVersion\":\"Node v433\"");
    assert_response_contains(result, "\"windowEndNodeVersion\":\"Node v472\"");
    assert_response_contains(result, "\"splitRouteGroupCount\":40");
    assert_response_contains(result, "\"coveredNodeVersions\":[\"Node v433\",\"Node v434\",\"Node v435\","
                                     "\"Node v436\",\"Node v437\",\"Node v438\",\"Node v439\",\"Node v440\","
                                     "\"Node v441\",\"Node v442\",\"Node v443\",\"Node v444\",\"Node v445\","
                                     "\"Node v446\",\"Node v447\",\"Node v448\",\"Node v449\",\"Node v450\","
                                     "\"Node v451\",\"Node v452\",\"Node v453\",\"Node v454\",\"Node v455\","
                                     "\"Node v456\",\"Node v457\",\"Node v458\",\"Node v459\",\"Node v460\","
                                     "\"Node v461\",\"Node v462\",\"Node v463\",\"Node v464\","
                                     "\"Node v465\",\"Node v466\",\"Node v467\","
                                     "\"Node v468\",\"Node v469\",\"Node v470\",\"Node v471\",\"Node v472\"]");
    assert_response_contains(result, "\"allChangesRouteRegistrationOnly\":true");
    assert_response_contains(result, "\"allChangesRouteCatalogOnly\":true");
    assert_response_contains(result, "\"nodeApiPathChanged\":false");
    assert_response_contains(result, "\"nodeResponseShapeChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"nodeStartsJavaService\":false");
    assert_response_contains(result, "\"nodeStartsMiniKvService\":false");
    assert_response_contains(result, "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_response_contains(result, "\"miniKvFixturePathChanged\":false");
    assert_response_contains(result, "\"miniKvShardJsonCommandChanged\":false");
    assert_response_contains(result, "\"miniKvRequiresNodeRouteTableChange\":false");
    assert_response_contains(result, "\"runtimeGateApprovalPresent\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsJavaService\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"liveReadAllowed\":false");
    assert_response_contains(result, "\"activeShardPrototypeEnabled\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit\":{\"auditMode\":"
                                     "\"node-route-split-compatibility-window-consistency-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_response_contains(result, "\"auditedWindowSection\":\"nodeRouteSplitCompatibilityWindow\"");
    assert_response_contains(result, "\"expectedWindowVersionCount\":40");
    assert_response_contains(result, "\"observedWindowVersionCount\":40");
    assert_response_contains(result, "\"windowRangeStart\":\"Node v433\"");
    assert_response_contains(result, "\"windowRangeEnd\":\"Node v472\"");
    assert_response_contains(result, "\"windowRangeStartNumber\":433");
    assert_response_contains(result, "\"windowRangeEndNumber\":472");
    assert_response_contains(result, "\"computedWindowVersionSpan\":40");
    assert_response_contains(result, "\"windowRangeNumbersParseable\":true");
    assert_response_contains(result, "\"windowCountMatchesRange\":true");
    assert_response_contains(result, "\"contiguousNodeVersionWindow\":true");
    assert_response_contains(result, "\"duplicateWindowVersionsDetected\":false");
    assert_response_contains(result, "\"allWindowVersionsRouteRegistrationOnly\":true");
    assert_response_contains(result, "\"sourceFrozenWindowDigest\":\"fnv1a64:d1e889711b5d8574\"");
    assert_response_contains(result, "\"latestWindowMatchesFrozenSource\":true");
    assert_response_contains(result, "\"windowAuditOnly\":true");
    assert_response_contains(result, "\"runtimeGateApprovalPresent\":false");
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
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/v471-post-route-catalog-expected-integrity-snapshot-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v186\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v186.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_response_contains(result, "\"auditedNodeVersion\":\"Node v471\"");
    assert_response_contains(result, "\"anchorFieldsRemovedByNodeV468\":true");
    assert_response_contains(result, "\"summaryBuilderIntroducedByNodeV469\":true");
    assert_response_contains(result, "\"expectedSummaryCentralizedByNodeV470\":true");
    assert_response_contains(result, "\"expectedIntegritySnapshotFactoryMovedByNodeV471\":true");
    assert_response_contains(result, "\"routeQualityDefaultSnapshotReusesExpectedSummary\":true");
    assert_response_contains(result, "\"routeQualityServiceConsumesCatalogIntegrityFactory\":true");
    assert_response_contains(result, "\"serviceLocalIntegrityFallbackRemoved\":true");
    assert_response_contains(result, "\"catalogEvaluatorParityTested\":true");
    assert_response_contains(result, "\"miniKvComputesNodeRouteSummary\":false");
    assert_response_contains(result, "\"miniKvOwnsNodeRouteQualitySnapshot\":false");
    assert_response_contains(result, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_response_contains(result, "\"focusedExpectedSummaryTestFileCount\":3");
    assert_response_contains(result, "\"focusedExpectedSummaryTestCount\":7");
    assert_response_contains(result, "\"focusedExpectedIntegritySnapshotTestFileCount\":3");
    assert_response_contains(result, "\"focusedExpectedIntegritySnapshotTestCount\":7");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":{\"freezeMode\":"
                                     "\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_response_contains(result, "\"frozenReleaseVersion\":\"v187\"");
    assert_response_contains(result, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v187.json\"");
    assert_response_contains(result, "\"frozenEvidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_response_contains(result,
                             "\"frozenStatus\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_response_contains(result, "\"frozenWindowEndNodeVersion\":\"Node v471\"");
    assert_response_contains(result, "\"frozenSplitRouteGroupCount\":39");
    assert_response_contains(result, "\"frozenBoundaryCatalogVersion\":\"read-only-boundary-fields.v15\"");
    assert_response_contains(result, "\"frozenBoundaryFieldCount\":502");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_response_contains(result, "\"preservesExpectedIntegritySnapshotAudit\":true");
    assert_response_contains(result, "\"preservesNodeV471CatalogOwnershipCleanup\":true");
    assert_response_contains(result, "\"preservesRouteCatalogWindow\":true");
    assert_response_contains(result, "\"preservesBoundaryCatalog\":true");
    assert_response_contains(result, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChain\":{\"catalogMode\":"
                                     "\"node-route-catalog-evidence-chain-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_response_contains(result, "\"catalogedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeStart\":\"v185\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v188\"");
    assert_response_contains(result, "\"latestCatalogedReleaseVersion\":\"v188\"");
    assert_response_contains(result, "\"latestCatalogedDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_response_contains(result, "\"catalogedSections\":[\"nodeRouteCatalogCloseoutSnapshot\","
                                     "\"nodeRouteCatalogAnchorRemovalAudit\","
                                     "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\","
                                     "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\"]");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v16\"");
    assert_response_contains(result, "\"fieldCount\":525");
    assert_response_contains(result, "\"groupCount\":27");
    assert_response_contains(result, "\"versionedFixtureCatalogOnly\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_response_contains(result, "\"allCatalogedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCatalogedFixturesVersioned\":true");
    assert_response_contains(result, "\"allCatalogedDigestsPresent\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChainAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":\"docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v189\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v189.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:a595f2d9cdcbac8d\"");
    assert_response_contains(result, "\"auditedChainSection\":\"nodeRouteCatalogEvidenceChain\"");
    assert_response_contains(result, "\"nodeCleanupBatchStart\":\"Node v465\"");
    assert_response_contains(result, "\"nodeCleanupBatchEnd\":\"Node v472\"");
    assert_response_contains(result, "\"nodeCleanupCloseoutVersion\":\"Node v472\"");
    assert_response_contains(result, "\"expectedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"observedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeContiguous\":true");
    assert_response_contains(result, "\"duplicateReleasesDetected\":false");
    assert_response_contains(result, "\"latestDigestMatchesFrozenSource\":true");
    assert_response_contains(result, "\"nodeCleanupCloseoutValidated\":true");
    assert_response_contains(result, "\"nodeFullVitestShardsPassed\":true");
    assert_response_contains(result, "\"nodeFullVitestFileCount\":394");
    assert_response_contains(result, "\"nodeFullVitestTestCount\":1222");
    assert_response_contains(result, "\"nodeTypecheckPassed\":true");
    assert_response_contains(result, "\"nodeBuildPassed\":true");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff\":{\"handoffMode\":"
                                     "\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v190\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v190.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_response_contains(result, "\"upstreamNodeCloseoutVersion\":\"Node v472\"");
    assert_response_contains(result, "\"latestAuditedMiniKvRelease\":\"v190\"");
    assert_response_contains(result, "\"handoffBoundaryCatalogVersion\":\"read-only-boundary-fields.v18\"");
    assert_response_contains(result, "\"handoffBoundaryFieldCount\":573");
    assert_response_contains(result, "\"handoffBoundaryGroupCount\":29");
    assert_response_contains(result, "\"includedSections\":[\"nodeRouteCatalogEvidenceChain\","
                                     "\"nodeRouteCatalogEvidenceChainAudit\","
                                     "\"nodeRouteSplitCompatibilityWindow\","
                                     "\"boundaryCatalogIndex\"]");
    assert_response_contains(result, "\"readyForDownstreamConsumption\":true");
    assert_response_contains(result, "\"requiresVersionedFixture\":true");
    assert_response_contains(result, "\"rollingCurrentAllowedAsHistoricalBaseline\":false");
    assert_response_contains(result, "\"readOnlyFixtureRequired\":true");
    assert_response_contains(result, "\"preservesRouteCatalogEvidenceChainAudit\":true");
    assert_response_contains(result, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_response_contains(result, "\"handoffOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v191\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v191.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_response_contains(result, "\"auditedHandoffSection\":\"nodeRouteCatalogCleanupCloseoutHandoff\"");
    assert_response_contains(result, "\"auditedHandoffReleaseVersion\":\"v191\"");
    assert_response_contains(result, "\"handoffSourceReleaseVersion\":\"v190\"");
    assert_response_contains(result, "\"handoffSourceDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_response_contains(result, "\"expectedIncludedSectionCount\":4");
    assert_response_contains(result, "\"observedIncludedSectionCount\":4");
    assert_response_contains(result, "\"handoffIncludedSectionsComplete\":true");
    assert_response_contains(result, "\"handoffReadyForDownstreamConsumption\":true");
    assert_response_contains(result, "\"versionedFixtureRequired\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"consumerGuidanceCountMatches\":true");
    assert_response_contains(result, "\"preservesRouteCatalogEvidenceChainAudit\":true");
    assert_response_contains(result, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
}

} // namespace minikv::command_test
