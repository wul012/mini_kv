#include "shard_test_suites.hpp"

namespace minikv::shard_test {

void check_shard_catalog_contract(ShardFixture& fixture) {
    const auto& json = *fixture.active_json;
    assert_contains(json, "\"nodeRouteGroupSplitCompatibility\":{\"compatibilityMode\":"
                          "\"node-route-group-split-contract-stable\"");
    assert_contains(json, "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v418-post-sandbox-endpoint-credential-resolver-route-group-split-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v158\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v158.json\"");
    assert_contains(json, "\"nodeChangeType\":\"route-registration-refactor-only\"");
    assert_contains(json, "\"nodeApiPathChanged\":false");
    assert_contains(json, "\"nodeResponseShapeChanged\":false");
    assert_contains(json, "\"nodeAddsEvidenceGate\":false");
    assert_contains(json, "\"nodeStartsJavaService\":false");
    assert_contains(json, "\"nodeStartsMiniKvService\":false");
    assert_contains(json, "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_contains(json, "\"miniKvFixturePathChanged\":false");
    assert_contains(json, "\"miniKvShardJsonCommandChanged\":false");
    assert_contains(json, "\"miniKvRequiresNodeRouteTableChange\":false");
    assert_contains(json, "\"archiveCompatibilityPreserved\":true");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                          "\"node-v433-v472-route-catalog-cleanup-closeout-window-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v472-post-route-catalog-cleanup-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_contains(json, "\"windowStartNodeVersion\":\"Node v433\"");
    assert_contains(json, "\"windowEndNodeVersion\":\"Node v472\"");
    assert_contains(json, "\"splitRouteGroupCount\":40");
    assert_contains(json, "\"coveredNodeVersions\":[\"Node v433\",\"Node v434\",\"Node v435\",\"Node v436\","
                          "\"Node v437\",\"Node v438\",\"Node v439\",\"Node v440\",\"Node v441\","
                          "\"Node v442\",\"Node v443\",\"Node v444\",\"Node v445\",\"Node v446\","
                          "\"Node v447\",\"Node v448\",\"Node v449\",\"Node v450\",\"Node v451\","
                          "\"Node v452\",\"Node v453\",\"Node v454\",\"Node v455\",\"Node v456\","
                          "\"Node v457\",\"Node v458\",\"Node v459\",\"Node v460\","
                          "\"Node v461\",\"Node v462\",\"Node v463\",\"Node v464\","
                          "\"Node v465\",\"Node v466\",\"Node v467\","
                          "\"Node v468\",\"Node v469\",\"Node v470\",\"Node v471\",\"Node v472\"]");
    assert_contains(json, "\"allChangesRouteRegistrationOnly\":true");
    assert_contains(json, "\"allChangesRouteCatalogOnly\":true");
    assert_contains(json, "\"nodeApiPathChanged\":false");
    assert_contains(json, "\"nodeResponseShapeChanged\":false");
    assert_contains(json, "\"nodeAddsEvidenceGate\":false");
    assert_contains(json, "\"nodeStartsJavaService\":false");
    assert_contains(json, "\"nodeStartsMiniKvService\":false");
    assert_contains(json, "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_contains(json, "\"miniKvFixturePathChanged\":false");
    assert_contains(json, "\"miniKvShardJsonCommandChanged\":false");
    assert_contains(json, "\"miniKvRequiresNodeRouteTableChange\":false");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"activeShardPrototypeEnabled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit\":{\"auditMode\":"
                          "\"node-route-split-compatibility-window-consistency-read-only\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_contains(json, "\"auditedWindowSection\":\"nodeRouteSplitCompatibilityWindow\"");
    assert_contains(json, "\"expectedWindowVersionCount\":40");
    assert_contains(json, "\"observedWindowVersionCount\":40");
    assert_contains(json, "\"windowRangeStart\":\"Node v433\"");
    assert_contains(json, "\"windowRangeEnd\":\"Node v472\"");
    assert_contains(json, "\"windowRangeStartNumber\":433");
    assert_contains(json, "\"windowRangeEndNumber\":472");
    assert_contains(json, "\"computedWindowVersionSpan\":40");
    assert_contains(json, "\"windowRangeNumbersParseable\":true");
    assert_contains(json, "\"windowCountMatchesRange\":true");
    assert_contains(json, "\"contiguousNodeVersionWindow\":true");
    assert_contains(json, "\"duplicateWindowVersionsDetected\":false");
    assert_contains(json, "\"allWindowVersionsRouteRegistrationOnly\":true");
    assert_contains(json, "\"sourceFrozenWindowDigest\":\"fnv1a64:d1e889711b5d8574\"");
    assert_contains(json, "\"latestWindowMatchesFrozenSource\":true");
    assert_contains(json, "\"windowAuditOnly\":true");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"activeShardPrototypeEnabled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"writeCommandsAllowed\":false");
    assert_contains(json, "\"adminCommandsAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":{\"auditMode\":"
                          "\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(
        json, "\"sourceNodePlan\":\"docs/plans3/v471-post-route-catalog-expected-integrity-snapshot-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v186\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v186.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_contains(json, "\"auditedNodeVersion\":\"Node v471\"");
    assert_contains(json, "\"anchorFieldsRemovedByNodeV468\":true");
    assert_contains(json, "\"summaryBuilderIntroducedByNodeV469\":true");
    assert_contains(json, "\"expectedSummaryCentralizedByNodeV470\":true");
    assert_contains(json, "\"expectedIntegritySnapshotFactoryMovedByNodeV471\":true");
    assert_contains(json, "\"routeQualityDefaultSnapshotReusesExpectedSummary\":true");
    assert_contains(json, "\"routeQualityServiceConsumesCatalogIntegrityFactory\":true");
    assert_contains(json, "\"serviceLocalIntegrityFallbackRemoved\":true");
    assert_contains(json, "\"catalogEvaluatorParityTested\":true");
    assert_contains(json, "\"duplicateDomainCountLiteralsRemoved\":true");
    assert_contains(json, "\"miniKvComputesNodeRouteSummary\":false");
    assert_contains(json, "\"miniKvOwnsNodeRouteQualitySnapshot\":false");
    assert_contains(json, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(json, "\"focusedExpectedSummaryTestFileCount\":3");
    assert_contains(json, "\"focusedExpectedSummaryTestCount\":7");
    assert_contains(json, "\"focusedExpectedIntegritySnapshotTestFileCount\":3");
    assert_contains(json, "\"focusedExpectedIntegritySnapshotTestCount\":7");
    assert_contains(json, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(json, "\"miniKvRuntimeContractChanged\":false");
    assert_contains(json, "\"auditOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":{\"freezeMode\":"
                          "\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(json, "\"frozenReleaseVersion\":\"v187\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v187.json\"");
    assert_contains(json, "\"frozenEvidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(json, "\"frozenStatus\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(json, "\"frozenWindowEndNodeVersion\":\"Node v471\"");
    assert_contains(json, "\"frozenSplitRouteGroupCount\":39");
    assert_contains(json, "\"frozenBoundaryCatalogVersion\":\"read-only-boundary-fields.v15\"");
    assert_contains(json, "\"frozenBoundaryFieldCount\":502");
    assert_contains(json, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(json, "\"preservesExpectedIntegritySnapshotAudit\":true");
    assert_contains(json, "\"preservesNodeV471CatalogOwnershipCleanup\":true");
    assert_contains(json, "\"preservesRouteCatalogWindow\":true");
    assert_contains(json, "\"preservesBoundaryCatalog\":true");
    assert_contains(json, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(json, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(json, "\"miniKvRuntimeContractChanged\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChain\":{\"catalogMode\":"
                          "\"node-route-catalog-evidence-chain-read-only\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(json, "\"catalogedReleaseCount\":4");
    assert_contains(json, "\"releaseRangeStart\":\"v185\"");
    assert_contains(json, "\"releaseRangeEnd\":\"v188\"");
    assert_contains(json, "\"latestCatalogedReleaseVersion\":\"v188\"");
    assert_contains(json, "\"latestCatalogedDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(json, "\"catalogedSections\":[\"nodeRouteCatalogCloseoutSnapshot\","
                          "\"nodeRouteCatalogAnchorRemovalAudit\","
                          "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\","
                          "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\"]");
    assert_contains(json, "\"releaseVersion\":\"v185\",\"status\":\"node-route-catalog-closeout-snapshot-read-only\"");
    assert_contains(json,
                    "\"releaseVersion\":\"v186\",\"status\":\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(
        json,
        "\"releaseVersion\":\"v187\",\"status\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(
        json,
        "\"releaseVersion\":\"v188\",\"status\":\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(json, "\"primarySection\":\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\"");
    assert_contains(json, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v16\"");
    assert_contains(json, "\"fieldCount\":525");
    assert_contains(json, "\"groupCount\":27");
    assert_contains(json, "\"versionedFixtureCatalogOnly\":true");
    assert_contains(json, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_contains(json, "\"allCatalogedEntriesReadOnly\":true");
    assert_contains(json, "\"allCatalogedFixturesVersioned\":true");
    assert_contains(json, "\"allCatalogedDigestsPresent\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAudit\":{\"auditMode\":"
                          "\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v189\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v189.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:a595f2d9cdcbac8d\"");
    assert_contains(json, "\"auditedChainSection\":\"nodeRouteCatalogEvidenceChain\"");
    assert_contains(json, "\"nodeCleanupBatchStart\":\"Node v465\"");
    assert_contains(json, "\"nodeCleanupBatchEnd\":\"Node v472\"");
    assert_contains(json, "\"nodeCleanupCloseoutVersion\":\"Node v472\"");
    assert_contains(json, "\"expectedCatalogedReleaseCount\":4");
    assert_contains(json, "\"observedCatalogedReleaseCount\":4");
    assert_contains(json, "\"releaseRangeContiguous\":true");
    assert_contains(json, "\"duplicateReleasesDetected\":false");
    assert_contains(json, "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(json, "\"nodeCleanupCloseoutValidated\":true");
    assert_contains(json, "\"nodeFullVitestShardsPassed\":true");
    assert_contains(json, "\"nodeFullVitestFileCount\":394");
    assert_contains(json, "\"nodeFullVitestTestCount\":1222");
    assert_contains(json, "\"nodeTypecheckPassed\":true");
    assert_contains(json, "\"nodeBuildPassed\":true");
    assert_contains(json, "\"auditOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff\":{\"handoffMode\":"
                          "\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v190\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v190.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_contains(json, "\"upstreamNodeCloseoutVersion\":\"Node v472\"");
    assert_contains(json, "\"latestAuditedMiniKvRelease\":\"v190\"");
    assert_contains(json, "\"handoffBoundaryCatalogVersion\":\"read-only-boundary-fields.v18\"");
    assert_contains(json, "\"handoffBoundaryFieldCount\":573");
    assert_contains(json, "\"handoffBoundaryGroupCount\":29");
    assert_contains(json, "\"includedSections\":[\"nodeRouteCatalogEvidenceChain\","
                          "\"nodeRouteCatalogEvidenceChainAudit\","
                          "\"nodeRouteSplitCompatibilityWindow\","
                          "\"boundaryCatalogIndex\"]");
    assert_contains(json, "\"readyForDownstreamConsumption\":true");
    assert_contains(json, "\"requiresVersionedFixture\":true");
    assert_contains(json, "\"rollingCurrentAllowedAsHistoricalBaseline\":false");
    assert_contains(json, "\"readOnlyFixtureRequired\":true");
    assert_contains(json, "\"preservesRouteCatalogEvidenceChainAudit\":true");
    assert_contains(json, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_contains(json, "\"handoffOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\":{\"auditMode\":"
                          "\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v191\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v191.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_contains(json, "\"auditedHandoffSection\":\"nodeRouteCatalogCleanupCloseoutHandoff\"");
    assert_contains(json, "\"auditedHandoffReleaseVersion\":\"v191\"");
    assert_contains(json, "\"handoffSourceReleaseVersion\":\"v190\"");
    assert_contains(json, "\"handoffSourceDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_contains(json, "\"expectedIncludedSectionCount\":4");
    assert_contains(json, "\"observedIncludedSectionCount\":4");
    assert_contains(json, "\"handoffIncludedSectionsComplete\":true");
    assert_contains(json, "\"handoffReadyForDownstreamConsumption\":true");
    assert_contains(json, "\"versionedFixtureRequired\":true");
    assert_contains(json, "\"rollingCurrentRejected\":true");
    assert_contains(json, "\"consumerGuidanceCountMatches\":true");
    assert_contains(json, "\"preservesRouteCatalogEvidenceChainAudit\":true");
    assert_contains(json, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_contains(json, "\"auditOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\":{\"freezeMode\":"
                          "\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_contains(json, "\"frozenReleaseVersion\":\"v192\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v192.json\"");
    assert_contains(json, "\"frozenEvidenceDigest\":\"fnv1a64:89bf4177b09f1ec0\"");
    assert_contains(json, "\"frozenStatus\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(json, "\"frozenHandoffSourceReleaseVersion\":\"v191\"");
    assert_contains(json, "\"frozenHandoffSourceDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_contains(json, "\"frozenBoundaryCatalogVersion\":\"read-only-boundary-fields.v20\"");
    assert_contains(json, "\"frozenBoundaryFieldCount\":625");
    assert_contains(json, "\"frozenBoundaryGroupCount\":31");
    assert_contains(json, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(json, "\"preservesHandoffAudit\":true");
    assert_contains(json, "\"preservesHandoffManifest\":true");
    assert_contains(json, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_contains(json, "\"frozenReadyForDownstreamConsumption\":true");
    assert_contains(json, "\"frozenRollingCurrentRejected\":true");
    assert_contains(json, "\"freezeOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\":{\"catalogMode\":"
                          "\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v193\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v193.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_contains(json, "\"catalogedReleaseCount\":4");
    assert_contains(json, "\"releaseRangeStart\":\"v190\"");
    assert_contains(json, "\"releaseRangeEnd\":\"v193\"");
    assert_contains(json, "\"latestCatalogedReleaseVersion\":\"v193\"");
    assert_contains(json, "\"latestCatalogedDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_contains(json, "\"catalogedSections\":[\"nodeRouteCatalogEvidenceChainAudit\","
                          "\"nodeRouteCatalogCleanupCloseoutHandoff\","
                          "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\","
                          "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\"]");
    assert_contains(json,
                    "\"releaseVersion\":\"v190\",\"status\":\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_contains(json,
                    "\"releaseVersion\":\"v191\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_contains(
        json, "\"releaseVersion\":\"v192\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v193\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-"
                          "freeze-read-only\"");
    assert_contains(json, "\"primarySection\":\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\"");
    assert_contains(json, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v21\"");
    assert_contains(json, "\"fieldCount\":648");
    assert_contains(json, "\"groupCount\":32");
    assert_contains(json, "\"versionedFixtureCatalogOnly\":true");
    assert_contains(json, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_contains(json, "\"allCatalogedEntriesReadOnly\":true");
    assert_contains(json, "\"allCatalogedFixturesVersioned\":true");
    assert_contains(json, "\"allCatalogedDigestsPresent\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\":{\"auditMode\":"
                          "\"node-route-catalog-cleanup-closeout-release-catalog-consistency-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v474-post-java-mini-kv-route-catalog-cleanup-handoff-evidence-report-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v194\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v194.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:626f5a2a96a980c2\"");
    assert_contains(json, "\"auditedCatalogSection\":\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\"");
    assert_contains(json, "\"auditedCatalogReleaseVersion\":\"v194\"");
    assert_contains(json, "\"catalogSourceReleaseVersion\":\"v193\"");
    assert_contains(json, "\"expectedCatalogedReleaseCount\":4");
    assert_contains(json, "\"observedCatalogedReleaseCount\":4");
    assert_contains(json, "\"expectedReleaseRangeSpan\":4");
    assert_contains(json, "\"observedReleaseRangeSpan\":4");
    assert_contains(json, "\"catalogEntryVersions\":[\"v190\",\"v191\",\"v192\",\"v193\"]");
    assert_contains(json, "\"catalogIncludesEvidenceChainAudit\":true");
    assert_contains(json, "\"catalogIncludesHandoff\":true");
    assert_contains(json, "\"catalogIncludesHandoffAudit\":true");
    assert_contains(json, "\"catalogIncludesHandoffAuditFreeze\":true");
    assert_contains(json, "\"releaseVersionsContiguous\":true");
    assert_contains(json, "\"duplicateReleaseVersionsDetected\":false");
    assert_contains(json, "\"allCatalogedEntriesReadOnly\":true");
    assert_contains(json, "\"allCatalogedFixturesVersioned\":true");
    assert_contains(json, "\"allCatalogedDigestsPresent\":true");
    assert_contains(json, "\"fieldCountsMonotonic\":true");
    assert_contains(json, "\"groupCountsMonotonic\":true");
    assert_contains(json, "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(json, "\"rollingCurrentRejected\":true");
    assert_contains(json, "\"catalogOnlyAudit\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\":{\"maintenanceMode\":"
                          "\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v477-post-java-mini-kv-latest-route-catalog-cleanup-evidence-report-archive-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v195\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v195.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:36463a895469c4bc\"");
    assert_contains(json, "\"splitFrom\":\"src/shard_readiness_route_catalog_evidence_chain.cpp\"");
    assert_contains(json, "\"splitInto\":\"src/shard_readiness_route_catalog_cleanup_catalog.cpp\"");
    assert_contains(json, "\"extractedCatalogFormatter\":true");
    assert_contains(json, "\"extractedCatalogAuditFormatter\":true");
    assert_contains(json, "\"preservesReleaseCatalog\":true");
    assert_contains(json, "\"preservesReleaseCatalogAudit\":true");
    assert_contains(json, "\"addsMaintenanceSection\":true");
    assert_contains(json, "\"publicCatalogSectionsChanged\":false");
    assert_contains(json, "\"fixturePathChanged\":false");
    assert_contains(json, "\"shardJsonCommandChanged\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackage\":{\"packageMode\":"
                          "\"node-route-catalog-cleanup-latest-evidence-package-read-only\"");
    assert_contains(
        json, "\"sourceNodePlan\":\"docs/plans3/"
              "v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v196\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v196.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(json, "\"packageReleaseCount\":3");
    assert_contains(json, "\"releaseRangeStart\":\"v194\"");
    assert_contains(json, "\"releaseRangeEnd\":\"v196\"");
    assert_contains(json, "\"latestPackagedReleaseVersion\":\"v196\"");
    assert_contains(json, "\"latestPackagedDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(json, "\"packagedSections\":[\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\","
                          "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\","
                          "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\"]");
    assert_contains(
        json,
        "\"releaseVersion\":\"v194\",\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v195\",\"status\":\"node-route-catalog-cleanup-closeout-release-"
                          "catalog-audit-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v196\",\"status\":\"node-route-catalog-cleanup-closeout-catalog-"
                          "formatter-split-read-only\"");
    assert_contains(json, "\"primarySection\":\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\"");
    assert_contains(json, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v24\"");
    assert_contains(json, "\"fieldCount\":714");
    assert_contains(json, "\"groupCount\":35");
    assert_contains(json, "\"versionedFixturePackageOnly\":true");
    assert_contains(json, "\"rollingCurrentUsedForFrozenPackage\":false");
    assert_contains(json, "\"allPackagedEntriesReadOnly\":true");
    assert_contains(json, "\"allPackagedFixturesVersioned\":true");
    assert_contains(json, "\"allPackagedDigestsPresent\":true");
    assert_contains(json, "\"packageReadyForArchiveVerification\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\":{\"auditMode\":"
                          "\"node-route-catalog-cleanup-latest-evidence-package-consistency-read-only\"");
    assert_contains(
        json, "\"sourceNodePlan\":\"docs/plans3/"
              "v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v197\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v197.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:836d9f0b3956fe30\"");
    assert_contains(json, "\"auditedPackageSection\":\"nodeRouteCatalogCleanupLatestEvidencePackage\"");
    assert_contains(json, "\"auditedPackageReleaseVersion\":\"v197\"");
    assert_contains(json, "\"packageSourceReleaseVersion\":\"v196\"");
    assert_contains(json, "\"packageSourceDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(json, "\"expectedPackageReleaseCount\":3");
    assert_contains(json, "\"observedPackageReleaseCount\":3");
    assert_contains(json, "\"packageRangeStart\":\"v194\"");
    assert_contains(json, "\"packageRangeEnd\":\"v196\"");
    assert_contains(json, "\"expectedPackageRangeSpan\":3");
    assert_contains(json, "\"observedPackageRangeSpan\":3");
    assert_contains(json, "\"packageEntryVersions\":[\"v194\",\"v195\",\"v196\"]");
    assert_contains(json, "\"packageIncludesReleaseCatalog\":true");
    assert_contains(json, "\"packageIncludesReleaseCatalogAudit\":true");
    assert_contains(json, "\"packageIncludesCatalogMaintenance\":true");
    assert_contains(json, "\"releaseVersionsContiguous\":true");
    assert_contains(json, "\"duplicatePackageVersionsDetected\":false");
    assert_contains(json, "\"allPackagedEntriesReadOnly\":true");
    assert_contains(json, "\"allPackagedFixturesVersioned\":true");
    assert_contains(json, "\"allPackagedDigestsPresent\":true");
    assert_contains(json, "\"fieldCountsMonotonic\":true");
    assert_contains(json, "\"groupCountsMonotonic\":true");
    assert_contains(json, "\"latestPackagedDigestMatchesPackageSource\":true");
    assert_contains(json, "\"packageReadyForArchiveVerification\":true");
    assert_contains(json, "\"auditUsesVersionedFixture\":true");
    assert_contains(json, "\"rollingCurrentRejected\":true");
    assert_contains(json, "\"packageAuditOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout\":{\"closeoutMode\":"
                          "\"node-route-catalog-cleanup-evidence-batch-closeout-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v198\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(json, "\"nodeBatchCloseoutVersion\":\"Node v480\"");
    assert_contains(json, "\"nodeBatchStartVersion\":\"Node v473\"");
    assert_contains(json, "\"nodeBatchEndVersion\":\"Node v479\"");
    assert_contains(json, "\"miniKvCloseoutReleaseCount\":5");
    assert_contains(json, "\"miniKvCloseoutRangeStart\":\"v194\"");
    assert_contains(json, "\"miniKvCloseoutRangeEnd\":\"v198\"");
    assert_contains(json, "\"latestCloseoutReleaseVersion\":\"v198\"");
    assert_contains(json, "\"latestCloseoutDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(json, "\"closeoutSections\":[\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\","
                          "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\","
                          "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\","
                          "\"nodeRouteCatalogCleanupLatestEvidencePackage\","
                          "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\"]");
    assert_contains(json, "\"releaseVersion\":\"v198\",\"fixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_contains(json, "\"primarySection\":\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\"");
    assert_contains(json, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v26\"");
    assert_contains(json, "\"fieldCount\":756");
    assert_contains(json, "\"groupCount\":37");
    assert_contains(json, "\"nodeBatchValidationRecorded\":true");
    assert_contains(json, "\"nodeFocusedCleanupTestsPassed\":true");
    assert_contains(json, "\"nodeFullVitestFileCount\":398");
    assert_contains(json, "\"nodeFullVitestTestCount\":1230");
    assert_contains(json, "\"nodeHttpStyleSmokePassed\":true");
    assert_contains(json, "\"allCloseoutEntriesReadOnly\":true");
    assert_contains(json, "\"allCloseoutFixturesVersioned\":true");
    assert_contains(json, "\"allCloseoutDigestsPresent\":true");
    assert_contains(json, "\"fieldCountsMonotonic\":true");
    assert_contains(json, "\"groupCountsMonotonic\":true");
    assert_contains(json, "\"futureNodeBatchMayConsumeCleanTags\":true");
    assert_contains(json, "\"closeoutReadyForNextBatch\":true");
    assert_contains(json, "\"rollingCurrentUsedForFrozenCloseout\":false");
    assert_contains(json, "\"closeoutOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit\":{\"auditMode\":"
                          "\"node-route-catalog-cleanup-evidence-batch-closeout-consistency-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v199\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v199.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:3a5716f6f09c2b3b\"");
    assert_contains(json, "\"auditedCloseoutSection\":\"nodeRouteCatalogCleanupEvidenceBatchCloseout\"");
    assert_contains(json, "\"auditedCloseoutReleaseVersion\":\"v199\"");
    assert_contains(json, "\"closeoutSourceReleaseVersion\":\"v198\"");
    assert_contains(json, "\"closeoutSourceDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(json, "\"expectedCloseoutReleaseCount\":5");
    assert_contains(json, "\"observedCloseoutReleaseCount\":5");
    assert_contains(json, "\"closeoutRangeStart\":\"v194\"");
    assert_contains(json, "\"closeoutRangeEnd\":\"v198\"");
    assert_contains(json, "\"expectedCloseoutRangeSpan\":5");
    assert_contains(json, "\"observedCloseoutRangeSpan\":5");
    assert_contains(json, "\"closeoutEntryVersions\":[\"v194\",\"v195\",\"v196\",\"v197\",\"v198\"]");
    assert_contains(json, "\"closeoutIncludesReleaseCatalog\":true");
    assert_contains(json, "\"closeoutIncludesReleaseCatalogAudit\":true");
    assert_contains(json, "\"closeoutIncludesCatalogMaintenance\":true");
    assert_contains(json, "\"closeoutIncludesLatestEvidencePackage\":true");
    assert_contains(json, "\"closeoutIncludesLatestEvidencePackageAudit\":true");
    assert_contains(json, "\"releaseVersionsContiguous\":true");
    assert_contains(json, "\"duplicateCloseoutVersionsDetected\":false");
    assert_contains(json, "\"allCloseoutEntriesReadOnly\":true");
    assert_contains(json, "\"allCloseoutFixturesVersioned\":true");
    assert_contains(json, "\"allCloseoutDigestsPresent\":true");
    assert_contains(json, "\"fieldCountsMonotonic\":true");
    assert_contains(json, "\"groupCountsMonotonic\":true");
    assert_contains(json, "\"latestCloseoutDigestMatchesCloseoutSource\":true");
    assert_contains(json, "\"nodeBatchValidationRecorded\":true");
    assert_contains(json, "\"nodeFullVitestCountMatchesPlan\":true");
    assert_contains(json, "\"nodeArchiveVerifierCheckCount\":16");
    assert_contains(json, "\"nodeArchiveVerifierChecksPassed\":16");
    assert_contains(json, "\"closeoutReadyForNextBatch\":true");
    assert_contains(json, "\"auditUsesVersionedFixture\":true");
    assert_contains(json, "\"rollingCurrentRejected\":true");
    assert_contains(json, "\"closeoutAuditOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity\":{\"continuityMode\":"
                          "\"node-route-catalog-cleanup-post-closeout-continuity-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-"
                          "route-archive-verification-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v379\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v379.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:01e736bc2963baae\"");
    assert_contains(json, "\"continuityStage\":\"feature-read-only-shard-route-preview-verification-report-archive-"
                          "closeout-release-package\"");
    assert_contains(json, "\"stageSequence\":180");
    assert_contains(json, "\"stageReleaseVersion\":\"v380\"");
    assert_contains(json, "\"nodeBatchCloseoutVersion\":\"Node v549\"");
    assert_contains(json, "\"previousCloseoutAuditReleaseVersion\":\"v200\"");
    assert_contains(json, "\"trackedMiniKvCloseoutRangeStart\":\"v194\"");
    assert_contains(json, "\"trackedMiniKvCloseoutRangeEnd\":\"v200\"");
    assert_contains(json, "\"trackedPostCloseoutRangeStart\":\"v201\"");
    assert_contains(json, "\"trackedPostCloseoutRangeEnd\":\"v380\"");
    assert_contains(json, "\"trackedPostCloseoutReleaseCount\":180");
    assert_contains(json, "\"nodePlanStillLatestForMiniKv\":true");
    assert_contains(json, "\"sourceFixtureVersioned\":true");
    assert_contains(json, "\"rollingCurrentRejected\":true");
    assert_contains(json, "\"preservesBatchCloseout\":true");
    assert_contains(json, "\"preservesBatchCloseoutAudit\":true");
    assert_contains(json, "\"preservesNodeV480Validation\":true");
    assert_contains(json, "\"preservesArchiveVerifierChecks\":true");
    assert_contains(json, "\"archiveVerifierCheckCount\":3");
    assert_contains(json, "\"archiveVerifierChecksPassed\":3");
    assert_contains(json, "\"postCloseoutStageOnly\":true");
    assert_contains(json, "\"readyForNextNodeBatch\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit\":{\"auditMode\":"
                          "\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v467-post-route-catalog-anchor-removal-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v185\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v185.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_contains(json, "\"auditedNodeVersion\":\"Node v467\"");
    assert_contains(json, "\"sourceAnchorCompatibilityRemoved\":true");
    assert_contains(json, "\"centralRouteTablePureFlatMapConsumer\":true");
    assert_contains(json, "\"routeGroupTestsUseTypedCatalogIdentity\":true");
    assert_contains(json, "\"routeOrderAlignmentRequired\":true");
    assert_contains(json, "\"flatMapAlignmentRequired\":true");
    assert_contains(json, "\"affectedRouteCatalogTestFileCount\":53");
    assert_contains(json, "\"affectedRouteCatalogTestCount\":59");
    assert_contains(json, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(json, "\"miniKvRouteAnchorDependencyIntroduced\":false");
    assert_contains(json, "\"miniKvRuntimeContractChanged\":false");
    assert_contains(json, "\"auditOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshot\":{\"snapshotMode\":"
                          "\"node-route-catalog-final-closeout-snapshot-read-only\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v184\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v184.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_contains(json, "\"nodeFinalCloseoutVersion\":\"Node v464\"");
    assert_contains(json, "\"routeCatalogBatchStart\":\"Node v433\"");
    assert_contains(json, "\"routeCatalogBatchEnd\":\"Node v464\"");
    assert_contains(json, "\"coveredNodeVersionCount\":32");
    assert_contains(json, "\"focusedCloseoutTestFileCount\":7");
    assert_contains(json, "\"focusedCloseoutTestCount\":17");
    assert_contains(json, "\"nodeTypecheckPassed\":true");
    assert_contains(json, "\"nodeBuildPassed\":true");
    assert_contains(json, "\"nodeFullVitestFileCount\":393");
    assert_contains(json, "\"nodeFullVitestTestCount\":1221");
    assert_contains(json, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(json, "\"miniKvRuntimeContractChanged\":false");
    assert_contains(json, "\"snapshotOnly\":true");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"boundaryCatalogMaintenance\":{\"maintenanceMode\":"
                          "\"boundary-field-catalog-split-contract-preserving\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v159\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v159.json\"");
    assert_contains(json, "\"boundaryFieldCatalogExtracted\":true");
    assert_contains(json, "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_contains(json, "\"publicShardJsonContractChanged\":false");
    assert_contains(json, "\"fixturePathChanged\":false");
    assert_contains(json, "\"shardJsonCommandChanged\":false");
    assert_contains(json, "\"nodeRouteGroupSplitCompatibilityPreserved\":true");
    assert_contains(json, "\"nodeAddsEvidenceGate\":false");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"activeShardPrototypeEnabled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"writeCommandsAllowed\":false");
    assert_contains(json, "\"adminCommandsAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"readOnlyBoundaryFields\":[\"readOnly\",\"executionAllowed\"");
    assert_contains(json, "\"boundaryCatalogMaintenance.publicShardJsonContractChanged\"");
    assert_contains(json, "\"boundaryCatalogMaintenance.routerActivationAllowed\"");
    assert_contains(json, "\"boundaryCatalogMaintenance.writeRoutingAllowed\"");
    assert_contains(json, "\"boundaryCatalogMaintenance.executionAllowed\"");
    assert_contains(json, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v29\"");
    assert_contains(json, "\"sourceNodePlan\":"
                          "\"docs/plans3/"
                          "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_contains(json, "\"fieldCount\":821");
    assert_contains(json, "\"groupCount\":40");
    assert_contains(json, "\"fieldListGeneratedFromSharedVector\":true");
    assert_contains(json, "\"catalogGroups\":[\"top-level-read-only\",\"slot-table-preview\","
                          "\"slot-table-preview-audit\",\"slot-table-preview-audit-maintenance\","
                          "\"shard-readiness-release-catalog\",\"shard-readiness-release-catalog-audit\","
                          "\"core-boundaries\",\"historical-fallback\","
                          "\"shard-readiness-history-maintenance\"");
    assert_contains(json, "\"node-route-group-split-compatibility\","
                          "\"node-route-split-compatibility-window\","
                          "\"node-route-split-compatibility-window-audit\","
                          "\"node-route-catalog-closeout-snapshot\","
                          "\"node-route-catalog-anchor-removal-audit\","
                          "\"node-route-catalog-expected-integrity-snapshot-audit\","
                          "\"node-route-catalog-expected-integrity-snapshot-freeze\","
                          "\"node-route-catalog-evidence-chain\","
                          "\"node-route-catalog-evidence-chain-audit\","
                          "\"node-route-catalog-cleanup-closeout-handoff\","
                          "\"node-route-catalog-cleanup-closeout-handoff-audit\","
                          "\"node-route-catalog-cleanup-closeout-handoff-audit-freeze\","
                          "\"node-route-catalog-cleanup-closeout-release-catalog\","
                          "\"node-route-catalog-cleanup-closeout-release-catalog-audit\","
                          "\"node-route-catalog-cleanup-closeout-catalog-maintenance\","
                          "\"node-route-catalog-cleanup-latest-evidence-package\","
                          "\"node-route-catalog-cleanup-latest-evidence-package-audit\","
                          "\"node-route-catalog-cleanup-evidence-batch-closeout\","
                          "\"node-route-catalog-cleanup-evidence-batch-closeout-audit\","
                          "\"node-route-catalog-cleanup-post-closeout-continuity\","
                          "\"boundary-catalog-maintenance\","
                          "\"boundary-catalog-index\"]");
    assert_contains(json, "\"slotTablePreviewCataloged\":true");
    assert_contains(json, "\"slotTablePreviewAuditCataloged\":true");
    assert_contains(json, "\"slotTablePreviewAuditMaintenanceCataloged\":true");
    assert_contains(json, "\"shardReadinessReleaseCatalogCataloged\":true");
    assert_contains(json, "\"shardReadinessReleaseCatalogAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowCataloged\":true");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshotCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreezeCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreezeCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenanceCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAuditCataloged\":true");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuityCataloged\":true");
    assert_contains(json, "\"shardReadinessHistoryMaintenanceCataloged\":true");
    assert_contains(json, "\"boundaryCatalogMaintenancePreserved\":true");
    assert_contains(json, "\"slotTablePreview.routerActivationAllowed\"");
    assert_contains(json, "\"slotTablePreview.writeRoutingAllowed\"");
    assert_contains(json, "\"slotTablePreview.executionAllowed\"");
    assert_contains(json, "\"slotTablePreviewAudit.contiguousSlotRange\"");
    assert_contains(json, "\"slotTablePreviewAudit.routerActivationAllowed\"");
    assert_contains(json, "\"slotTablePreviewAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"slotTablePreviewAudit.executionAllowed\"");
    assert_contains(json, "\"slotTablePreviewAuditMaintenance.publicShardJsonContractChanged\"");
    assert_contains(json, "\"slotTablePreviewAuditMaintenance.routerActivationAllowed\"");
    assert_contains(json, "\"slotTablePreviewAuditMaintenance.writeRoutingAllowed\"");
    assert_contains(json, "\"slotTablePreviewAuditMaintenance.executionAllowed\"");
    assert_contains(json, "\"shardReadinessReleaseCatalog.publicShardJsonContractChanged\"");
    assert_contains(json, "\"shardReadinessReleaseCatalog.versionedFixtureCatalogOnly\"");
    assert_contains(json, "\"shardReadinessReleaseCatalog.rollingCurrentUsedForFrozenCatalog\"");
    assert_contains(json, "\"shardReadinessReleaseCatalog.routerActivationAllowed\"");
    assert_contains(json, "\"shardReadinessReleaseCatalog.writeRoutingAllowed\"");
    assert_contains(json, "\"shardReadinessReleaseCatalog.executionAllowed\"");
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit.contiguousReleaseRange\"");
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit.duplicateReleasesDetected\"");
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit.latestDigestMatchesFrozenSource\"");
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit.routerActivationAllowed\"");
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindow.nodeAddsEvidenceGate\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindow.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindow.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindow.executionAllowed\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.windowRangeNumbersParseable\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.windowCountMatchesRange\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.contiguousNodeVersionWindow\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.duplicateWindowVersionsDetected\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.latestWindowMatchesFrozenSource\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteSplitCompatibilityWindowAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshot.nodeConsumesFreshMiniKvEvidence\"");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshot.miniKvRuntimeContractChanged\"");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshot.snapshotOnly\"");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshot.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshot.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCloseoutSnapshot.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit.sourceAnchorCompatibilityRemoved\"");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit.centralRouteTablePureFlatMapConsumer\"");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit.routeGroupTestsUseTypedCatalogIdentity\"");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit.miniKvRouteAnchorDependencyIntroduced\"");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogAnchorRemovalAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.expectedSummaryCentralizedByNodeV470\"");
    assert_contains(
        json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.expectedIntegritySnapshotFactoryMovedByNodeV471\"");
    assert_contains(
        json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.routeQualityServiceConsumesCatalogIntegrityFactory\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.miniKvOwnsNodeCatalogIntegritySnapshot\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.miniKvComputesNodeRouteSummary\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.miniKvOwnsNodeRouteQualitySnapshot\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.rollingCurrentUsedForFrozenBaseline\"");
    assert_contains(json,
                    "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.preservesExpectedIntegritySnapshotAudit\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.miniKvOwnsNodeCatalogIntegritySnapshot\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChain.versionedFixtureCatalogOnly\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChain.rollingCurrentUsedForFrozenCatalog\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChain.allCatalogedEntriesReadOnly\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChain.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChain.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChain.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAudit.releaseRangeContiguous\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAudit.nodeCleanupCloseoutValidated\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAudit.nodeFullVitestShardsPassed\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogEvidenceChainAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff.readyForDownstreamConsumption\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff.requiresVersionedFixture\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff.rollingCurrentAllowedAsHistoricalBaseline\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff.preservesNodeCleanupCloseoutValidation\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoff.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.handoffIncludedSectionsComplete\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.handoffReadyForDownstreamConsumption\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.rollingCurrentRejected\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.consumerGuidanceCountMatches\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.rollingCurrentUsedForFrozenBaseline\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.preservesHandoffAudit\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.frozenRollingCurrentRejected\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.catalogedReleaseCount\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.versionedFixtureCatalogOnly\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.allCatalogedEntriesReadOnly\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.expectedCatalogedReleaseCount\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.releaseVersionsContiguous\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.latestDigestMatchesFrozenSource\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.extractedCatalogFormatter\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.preservesReleaseCatalogAudit\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.publicCatalogSectionsChanged\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackage.packageReleaseCount\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackage.packageReadyForArchiveVerification\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackage.allPackagedEntriesReadOnly\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackage.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackage.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackage.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.expectedPackageReleaseCount\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.releaseVersionsContiguous\"");
    assert_contains(json,
                    "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.latestPackagedDigestMatchesPackageSource\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.packageReadyForArchiveVerification\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.miniKvCloseoutReleaseCount\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.nodeBatchCloseoutVersion\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.futureNodeBatchMayConsumeCleanTags\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.closeoutReadyForNextBatch\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.expectedCloseoutReleaseCount\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.releaseVersionsContiguous\"");
    assert_contains(json,
                    "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.latestCloseoutDigestMatchesCloseoutSource\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.closeoutReadyForNextBatch\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.stageSequence\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.stageReleaseVersion\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.sourceFixtureVersioned\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.preservesBatchCloseoutAudit\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.readyForNextNodeBatch\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.executionAllowed\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.preservesFixtureParity\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.preservesArchiveCompatibility\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.preservesHistoricalFallback\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.changesShardJsonCommand\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.changesFixturePath\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.startsServices\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.runtimeProbeAllowed\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.writeRoutingAllowed\"");
    assert_contains(json, "\"shardReadinessHistoryMaintenance.executionAllowed\"");
    assert_contains(json, "\"boundaryCatalogIndex.publicShardJsonContractChanged\"");
    assert_contains(json, "\"boundaryCatalogIndex.routerActivationAllowed\"");
    assert_contains(json, "\"boundaryCatalogIndex.writeRoutingAllowed\"");
    assert_contains(json, "\"boundaryCatalogIndex.executionAllowed\"");
    assert_contains(json, "\"evidenceDigest\":\"fnv1a64:");
}

} // namespace minikv::shard_test
