#include "command_test_suites.hpp"

namespace minikv::command_test {

void run_command_shard_freeze_tests(CommandFixture& fixture) {
    auto& result = fixture.result;

    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\":{\"freezeMode\":"
                                     "\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_response_contains(result, "\"frozenReleaseVersion\":\"v192\"");
    assert_response_contains(result, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v192.json\"");
    assert_response_contains(result, "\"frozenEvidenceDigest\":\"fnv1a64:89bf4177b09f1ec0\"");
    assert_response_contains(result,
                             "\"frozenStatus\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_response_contains(result, "\"frozenHandoffSourceReleaseVersion\":\"v191\"");
    assert_response_contains(result, "\"frozenHandoffSourceDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_response_contains(result, "\"frozenBoundaryCatalogVersion\":\"read-only-boundary-fields.v20\"");
    assert_response_contains(result, "\"frozenBoundaryFieldCount\":625");
    assert_response_contains(result, "\"frozenBoundaryGroupCount\":31");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_response_contains(result, "\"preservesHandoffAudit\":true");
    assert_response_contains(result, "\"preservesHandoffManifest\":true");
    assert_response_contains(result, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_response_contains(result, "\"frozenReadyForDownstreamConsumption\":true");
    assert_response_contains(result, "\"frozenRollingCurrentRejected\":true");
    assert_response_contains(result, "\"freezeOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\":{\"catalogMode\":"
                                     "\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v193\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v193.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_response_contains(result, "\"catalogedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeStart\":\"v190\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v193\"");
    assert_response_contains(result, "\"latestCatalogedReleaseVersion\":\"v193\"");
    assert_response_contains(result, "\"latestCatalogedDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_response_contains(result, "\"catalogedSections\":[\"nodeRouteCatalogEvidenceChainAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutHandoff\","
                                     "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\"]");
    assert_response_contains(
        result, "\"releaseVersion\":\"v190\",\"status\":\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_response_contains(
        result, "\"releaseVersion\":\"v191\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_response_contains(
        result,
        "\"releaseVersion\":\"v192\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v193\",\"status\":\"node-route-catalog-cleanup-closeout-"
                                     "handoff-audit-freeze-read-only\"");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v21\"");
    assert_response_contains(result, "\"fieldCount\":648");
    assert_response_contains(result, "\"groupCount\":32");
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
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-closeout-release-catalog-consistency-read-only\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":\"docs/plans3/"
                             "v474-post-java-mini-kv-route-catalog-cleanup-handoff-evidence-report-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v194\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v194.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:626f5a2a96a980c2\"");
    assert_response_contains(result, "\"auditedCatalogSection\":\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\"");
    assert_response_contains(result, "\"auditedCatalogReleaseVersion\":\"v194\"");
    assert_response_contains(result, "\"catalogSourceReleaseVersion\":\"v193\"");
    assert_response_contains(result, "\"expectedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"observedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"expectedReleaseRangeSpan\":4");
    assert_response_contains(result, "\"observedReleaseRangeSpan\":4");
    assert_response_contains(result, "\"catalogEntryVersions\":[\"v190\",\"v191\",\"v192\",\"v193\"]");
    assert_response_contains(result, "\"catalogIncludesEvidenceChainAudit\":true");
    assert_response_contains(result, "\"catalogIncludesHandoff\":true");
    assert_response_contains(result, "\"catalogIncludesHandoffAudit\":true");
    assert_response_contains(result, "\"catalogIncludesHandoffAuditFreeze\":true");
    assert_response_contains(result, "\"releaseVersionsContiguous\":true");
    assert_response_contains(result, "\"duplicateReleaseVersionsDetected\":false");
    assert_response_contains(result, "\"allCatalogedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCatalogedFixturesVersioned\":true");
    assert_response_contains(result, "\"allCatalogedDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestDigestMatchesFrozenSource\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"catalogOnlyAudit\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\":{\"maintenanceMode\":"
                                     "\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/"
                "v477-post-java-mini-kv-latest-route-catalog-cleanup-evidence-report-archive-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v195\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v195.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:36463a895469c4bc\"");
    assert_response_contains(result, "\"splitFrom\":\"src/shard_readiness_route_catalog_evidence_chain.cpp\"");
    assert_response_contains(result, "\"splitInto\":\"src/shard_readiness_route_catalog_cleanup_catalog.cpp\"");
    assert_response_contains(result, "\"extractedCatalogFormatter\":true");
    assert_response_contains(result, "\"extractedCatalogAuditFormatter\":true");
    assert_response_contains(result, "\"preservesReleaseCatalog\":true");
    assert_response_contains(result, "\"preservesReleaseCatalogAudit\":true");
    assert_response_contains(result, "\"addsMaintenanceSection\":true");
    assert_response_contains(result, "\"publicCatalogSectionsChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage\":{\"packageMode\":"
                                     "\"node-route-catalog-cleanup-latest-evidence-package-read-only\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/"
                "v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v196\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v196.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_response_contains(result, "\"packageReleaseCount\":3");
    assert_response_contains(result, "\"releaseRangeStart\":\"v194\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v196\"");
    assert_response_contains(result, "\"latestPackagedReleaseVersion\":\"v196\"");
    assert_response_contains(result, "\"latestPackagedDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_response_contains(result, "\"packagedSections\":[\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\","
                                     "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\"]");
    assert_response_contains(
        result,
        "\"releaseVersion\":\"v194\",\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v195\",\"status\":\"node-route-catalog-cleanup-closeout-"
                                     "release-catalog-audit-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v196\",\"status\":\"node-route-catalog-cleanup-closeout-"
                                     "catalog-formatter-split-read-only\"");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v24\"");
    assert_response_contains(result, "\"fieldCount\":714");
    assert_response_contains(result, "\"groupCount\":35");
    assert_response_contains(result, "\"versionedFixturePackageOnly\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenPackage\":false");
    assert_response_contains(result, "\"allPackagedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allPackagedFixturesVersioned\":true");
    assert_response_contains(result, "\"allPackagedDigestsPresent\":true");
    assert_response_contains(result, "\"packageReadyForArchiveVerification\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-latest-evidence-package-consistency-read-only\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/"
                "v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v197\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v197.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:836d9f0b3956fe30\"");
    assert_response_contains(result, "\"auditedPackageSection\":\"nodeRouteCatalogCleanupLatestEvidencePackage\"");
    assert_response_contains(result, "\"auditedPackageReleaseVersion\":\"v197\"");
    assert_response_contains(result, "\"packageSourceReleaseVersion\":\"v196\"");
    assert_response_contains(result, "\"packageSourceDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_response_contains(result, "\"expectedPackageReleaseCount\":3");
    assert_response_contains(result, "\"observedPackageReleaseCount\":3");
    assert_response_contains(result, "\"packageRangeStart\":\"v194\"");
    assert_response_contains(result, "\"packageRangeEnd\":\"v196\"");
    assert_response_contains(result, "\"expectedPackageRangeSpan\":3");
    assert_response_contains(result, "\"observedPackageRangeSpan\":3");
    assert_response_contains(result, "\"packageEntryVersions\":[\"v194\",\"v195\",\"v196\"]");
    assert_response_contains(result, "\"packageIncludesReleaseCatalog\":true");
    assert_response_contains(result, "\"packageIncludesReleaseCatalogAudit\":true");
    assert_response_contains(result, "\"packageIncludesCatalogMaintenance\":true");
    assert_response_contains(result, "\"releaseVersionsContiguous\":true");
    assert_response_contains(result, "\"duplicatePackageVersionsDetected\":false");
    assert_response_contains(result, "\"allPackagedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allPackagedFixturesVersioned\":true");
    assert_response_contains(result, "\"allPackagedDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestPackagedDigestMatchesPackageSource\":true");
    assert_response_contains(result, "\"packageReadyForArchiveVerification\":true");
    assert_response_contains(result, "\"auditUsesVersionedFixture\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"packageAuditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout\":{\"closeoutMode\":"
                                     "\"node-route-catalog-cleanup-evidence-batch-closeout-read-only\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":\"docs/plans3/"
                             "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v198\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_response_contains(result, "\"nodeBatchCloseoutVersion\":\"Node v480\"");
    assert_response_contains(result, "\"nodeBatchStartVersion\":\"Node v473\"");
    assert_response_contains(result, "\"nodeBatchEndVersion\":\"Node v479\"");
    assert_response_contains(result, "\"miniKvCloseoutReleaseCount\":5");
    assert_response_contains(result, "\"miniKvCloseoutRangeStart\":\"v194\"");
    assert_response_contains(result, "\"miniKvCloseoutRangeEnd\":\"v198\"");
    assert_response_contains(result, "\"latestCloseoutReleaseVersion\":\"v198\"");
    assert_response_contains(result, "\"latestCloseoutDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_response_contains(result, "\"closeoutSections\":[\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\","
                                     "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\","
                                     "\"nodeRouteCatalogCleanupLatestEvidencePackage\","
                                     "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\"]");
    assert_response_contains(
        result, "\"releaseVersion\":\"v198\",\"fixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v26\"");
    assert_response_contains(result, "\"fieldCount\":756");
    assert_response_contains(result, "\"groupCount\":37");
    assert_response_contains(result, "\"nodeBatchValidationRecorded\":true");
    assert_response_contains(result, "\"nodeFocusedCleanupTestsPassed\":true");
    assert_response_contains(result, "\"nodeFullVitestFileCount\":398");
    assert_response_contains(result, "\"nodeFullVitestTestCount\":1230");
    assert_response_contains(result, "\"nodeHttpStyleSmokePassed\":true");
    assert_response_contains(result, "\"allCloseoutEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCloseoutFixturesVersioned\":true");
    assert_response_contains(result, "\"allCloseoutDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"futureNodeBatchMayConsumeCleanTags\":true");
    assert_response_contains(result, "\"closeoutReadyForNextBatch\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenCloseout\":false");
    assert_response_contains(result, "\"closeoutOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-evidence-batch-closeout-consistency-read-only\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":\"docs/plans3/"
                             "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v199\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v199.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:3a5716f6f09c2b3b\"");
    assert_response_contains(result, "\"auditedCloseoutSection\":\"nodeRouteCatalogCleanupEvidenceBatchCloseout\"");
    assert_response_contains(result, "\"auditedCloseoutReleaseVersion\":\"v199\"");
    assert_response_contains(result, "\"closeoutSourceReleaseVersion\":\"v198\"");
    assert_response_contains(result, "\"closeoutSourceDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_response_contains(result, "\"expectedCloseoutReleaseCount\":5");
}

} // namespace minikv::command_test
