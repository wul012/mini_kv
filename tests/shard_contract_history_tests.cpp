#include "shard_test_suites.hpp"

namespace minikv::shard_test {

void check_shard_history_contract(ShardFixture& fixture) {
    const auto& json = *fixture.active_json;
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit\":{\"auditMode\":"
                          "\"shard-readiness-release-catalog-consistency-read-only\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
                    "v432-post-credential-resolver-endpoint-handle-allowlist-approval-route-group-split-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v165\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v165.json\"");
    assert_contains(json, "\"auditedCatalogSection\":\"shardReadinessReleaseCatalog\"");
    assert_contains(json, "\"expectedReleaseCount\":4");
    assert_contains(json, "\"observedReleaseCount\":4");
    assert_contains(json, "\"releaseRangeStart\":\"v161\"");
    assert_contains(json, "\"releaseRangeEnd\":\"v164\"");
    assert_contains(json, "\"contiguousReleaseRange\":true");
    assert_contains(json, "\"duplicateReleasesDetected\":false");
    assert_contains(json, "\"missingCatalogEntriesDetected\":false");
    assert_contains(json, "\"allCatalogedDigestsPresent\":true");
    assert_contains(json, "\"allCatalogedFixturesVersioned\":true");
    assert_contains(json, "\"allCatalogedEntriesReadOnly\":true");
    assert_contains(json, "\"fieldCountsMonotonic\":true");
    assert_contains(json, "\"groupCountsMonotonic\":true");
    assert_contains(json, "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(json, "\"catalogOnlyAudit\":true");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"writeCommandsAllowed\":false");
    assert_contains(json, "\"adminCommandsAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardReadinessReleaseCatalog\":{\"catalogMode\":"
                          "\"versioned-shard-readiness-release-catalog-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v431-post-credential-resolver-credential-handle-approval-route-group-split-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v164\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v164.json\"");
    assert_contains(json, "\"catalogedReleaseCount\":4");
    assert_contains(json, "\"latestCatalogedReleaseVersion\":\"v164\"");
    assert_contains(json, "\"latestCatalogedDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_contains(json, "\"catalogedSections\":[\"boundaryCatalogIndex\",\"slotTablePreview\","
                          "\"slotTablePreviewAudit\",\"slotTablePreviewAuditMaintenance\"]");
    assert_contains(json, "\"releaseVersion\":\"v161\",\"status\":\"boundary-field-catalog-index-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v162\",\"status\":\"slot-table-preview-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v163\",\"status\":\"slot-table-preview-audit-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v164\",\"status\":\"slot-table-preview-audit-maintenance-read-only\"");
    assert_contains(json, "\"primarySection\":\"slotTablePreviewAuditMaintenance\"");
    assert_contains(json, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v5\"");
    assert_contains(json, "\"versionedFixtureCatalogOnly\":true");
    assert_contains(json, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_contains(json, "\"publicShardJsonContractChanged\":false");
    assert_contains(json, "\"fixturePathChanged\":false");
    assert_contains(json, "\"shardJsonCommandChanged\":false");
    assert_contains(json, "\"nodeAddsEvidenceGate\":false");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"writeCommandsAllowed\":false");
    assert_contains(json, "\"adminCommandsAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"orderAuthoritative\":false");
    assert_contains(json, "\"auditAuthoritative\":false");
    assert_contains(json, "\"writeCommandsAllowed\":false");
    assert_contains(json, "\"adminCommandsAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"setnxexExecutionAllowed\":false");
    assert_contains(json, "\"multiProcessStarted\":false");
    assert_contains(json, "\"storageDirectoriesCreated\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"archivedNodeEvidenceMutated\":false");
    assert_contains(json, "\"writeCommandsExecuted\":false");
    assert_contains(json, "\"adminCommandsExecuted\":false");
    assert_contains(json, "\"loadRestoreCompactExecuted\":false");
    assert_contains(json, "\"nodeConsumer\":\"Node v550+ may consume v420 as read-only shard route preview "
                          "verification report archive closeout verification audit release-package evidence only\"");
    assert_contains(json, "\"nodeArchivedEvidencePreserved\":true");
    assert_contains(json, "\"commandCatalog\":{\"command\":\"SHARDJSON\",\"category\":\"read\"");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"extraArgsAllowed\":false");
    assert_contains(json, "\"sideEffects\":[\"metadata_read\"]");
    assert_contains(json, "\"commandCatalogQuality\":{\"releaseRangeStart\":\"v1546\"");
    assert_contains(json, "\"releaseRangeEnd\":\"v1607\"");
    assert_contains(json, "\"scope\":\"command-catalog-single-source-refactor-plus-code-walkthrough-owner-request-mini-"
                          "kv-receipt-external-artifact-and-f-folder-explanation-quality-gates\"");
    assert_contains(json, "\"commandCount\":94");
    assert_contains(json, "\"dispatchCatalogSplit\":true");
    assert_contains(json, "\"contractCatalogDeduplicated\":true");
    assert_contains(json, "\"helpTextGeneratedFromCatalog\":true");
    assert_contains(json, "\"completionCandidatesGeneratedFromCatalog\":true");
    assert_contains(json, "\"runtimeCommandBehaviorChanged\":false");
    assert_contains(json, "\"storageBehaviorChanged\":false");
    assert_contains(json, "\"fixtureParity\":{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(
        json,
        "\"historicalFixturePaths\":[\"fixtures/release/shard-readiness-v144.json\","
        "\"fixtures/release/shard-readiness-v145.json\","
        "\"fixtures/release/shard-readiness-v146.json\","
        "\"fixtures/release/shard-readiness-v147.json\","
        "\"fixtures/release/shard-readiness-v148.json\","
        "\"fixtures/release/shard-readiness-v149.json\","
        "\"fixtures/release/shard-readiness-v150.json\","
        "\"fixtures/release/shard-readiness-v151.json\","
        "\"fixtures/release/shard-readiness-v152.json\","
        "\"fixtures/release/shard-readiness-v153.json\","
        "\"fixtures/release/shard-readiness-v154.json\","
        "\"fixtures/release/shard-readiness-v155.json\","
        "\"fixtures/release/shard-readiness-v156.json\","
        "\"fixtures/release/shard-readiness-v157.json\","
        "\"fixtures/release/shard-readiness-v158.json\","
        "\"fixtures/release/shard-readiness-v159.json\","
        "\"fixtures/release/shard-readiness-v160.json\","
        "\"fixtures/release/shard-readiness-v161.json\","
        "\"fixtures/release/shard-readiness-v162.json\","
        "\"fixtures/release/shard-readiness-v163.json\","
        "\"fixtures/release/shard-readiness-v164.json\","
        "\"fixtures/release/shard-readiness-v165.json\","
        "\"fixtures/release/shard-readiness-v166.json\","
        "\"fixtures/release/shard-readiness-v167.json\","
        "\"fixtures/release/shard-readiness-v168.json\","
        "\"fixtures/release/shard-readiness-v169.json\","
        "\"fixtures/release/shard-readiness-v170.json\","
        "\"fixtures/release/shard-readiness-v171.json\","
        "\"fixtures/release/shard-readiness-v172.json\","
        "\"fixtures/release/shard-readiness-v173.json\","
        "\"fixtures/release/shard-readiness-v174.json\","
        "\"fixtures/release/shard-readiness-v175.json\","
        "\"fixtures/release/shard-readiness-v176.json\","
        "\"fixtures/release/shard-readiness-v177.json\","
        "\"fixtures/release/shard-readiness-v178.json\","
        "\"fixtures/release/shard-readiness-v179.json\","
        "\"fixtures/release/shard-readiness-v180.json\","
        "\"fixtures/release/shard-readiness-v181.json\","
        "\"fixtures/release/shard-readiness-v182.json\","
        "\"fixtures/release/shard-readiness-v183.json\","
        "\"fixtures/release/shard-readiness-v184.json\","
        "\"fixtures/release/shard-readiness-v185.json\","
        "\"fixtures/release/shard-readiness-v186.json\","
        "\"fixtures/release/shard-readiness-v187.json\","
        "\"fixtures/release/shard-readiness-v188.json\","
        "\"fixtures/release/shard-readiness-v189.json\","
        "\"fixtures/release/shard-readiness-v190.json\","
        "\"fixtures/release/shard-readiness-v191.json\","
        "\"fixtures/release/shard-readiness-v192.json\","
        "\"fixtures/release/shard-readiness-v193.json\","
        "\"fixtures/release/shard-readiness-v194.json\","
        "\"fixtures/release/shard-readiness-v195.json\","
        "\"fixtures/release/shard-readiness-v196.json\","
        "\"fixtures/release/shard-readiness-v197.json\","
        "\"fixtures/release/shard-readiness-v198.json\","
        "\"fixtures/release/shard-readiness-v199.json\","
        "\"fixtures/release/shard-readiness-v200.json\",\"fixtures/release/shard-readiness-v201.json\",\"fixtures/"
        "release/shard-readiness-v202.json\",\"fixtures/release/shard-readiness-v203.json\",\"fixtures/release/"
        "shard-readiness-v204.json\",\"fixtures/release/shard-readiness-v205.json\",\"fixtures/release/"
        "shard-readiness-v206.json\",\"fixtures/release/shard-readiness-v207.json\",\"fixtures/release/"
        "shard-readiness-v208.json\",\"fixtures/release/shard-readiness-v209.json\",\"fixtures/release/"
        "shard-readiness-v210.json\",\"fixtures/release/shard-readiness-v211.json\",\"fixtures/release/"
        "shard-readiness-v212.json\",\"fixtures/release/shard-readiness-v213.json\",\"fixtures/release/"
        "shard-readiness-v214.json\",\"fixtures/release/shard-readiness-v215.json\",\"fixtures/release/"
        "shard-readiness-v216.json\",\"fixtures/release/shard-readiness-v217.json\",\"fixtures/release/"
        "shard-readiness-v218.json\",\"fixtures/release/shard-readiness-v219.json\",\"fixtures/release/"
        "shard-readiness-v220.json\",\"fixtures/release/shard-readiness-v221.json\",\"fixtures/release/"
        "shard-readiness-v222.json\",\"fixtures/release/shard-readiness-v223.json\",\"fixtures/release/"
        "shard-readiness-v224.json\",\"fixtures/release/shard-readiness-v225.json\",\"fixtures/release/"
        "shard-readiness-v226.json\",\"fixtures/release/shard-readiness-v227.json\",\"fixtures/release/"
        "shard-readiness-v228.json\",\"fixtures/release/shard-readiness-v229.json\",\"fixtures/release/"
        "shard-readiness-v230.json\",\"fixtures/release/shard-readiness-v231.json\",\"fixtures/release/"
        "shard-readiness-v232.json\",\"fixtures/release/shard-readiness-v233.json\",\"fixtures/release/"
        "shard-readiness-v234.json\",\"fixtures/release/shard-readiness-v235.json\",\"fixtures/release/"
        "shard-readiness-v236.json\",\"fixtures/release/shard-readiness-v237.json\",\"fixtures/release/"
        "shard-readiness-v238.json\",\"fixtures/release/shard-readiness-v239.json\",\"fixtures/release/"
        "shard-readiness-v240.json\",\"fixtures/release/shard-readiness-v241.json\",\"fixtures/release/"
        "shard-readiness-v242.json\",\"fixtures/release/shard-readiness-v243.json\",\"fixtures/release/"
        "shard-readiness-v244.json\",\"fixtures/release/shard-readiness-v245.json\",\"fixtures/release/"
        "shard-readiness-v246.json\",\"fixtures/release/shard-readiness-v247.json\",\"fixtures/release/"
        "shard-readiness-v248.json\",\"fixtures/release/shard-readiness-v249.json\",\"fixtures/release/"
        "shard-readiness-v250.json\",\"fixtures/release/shard-readiness-v251.json\",\"fixtures/release/"
        "shard-readiness-v252.json\",\"fixtures/release/shard-readiness-v253.json\",\"fixtures/release/"
        "shard-readiness-v254.json\",\"fixtures/release/shard-readiness-v255.json\",\"fixtures/release/"
        "shard-readiness-v256.json\",\"fixtures/release/shard-readiness-v257.json\",\"fixtures/release/"
        "shard-readiness-v258.json\",\"fixtures/release/shard-readiness-v259.json\",\"fixtures/release/"
        "shard-readiness-v260.json\",\"fixtures/release/shard-readiness-v261.json\"]");
    assert_contains(json, "\"runtimeMatchesCurrentFixture\":true");
    assert_contains(json, "\"historicalFixturesPreserved\":true");
    assert_contains(json, "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_contains(json, "\"archivedNodeVersions\":[\"Node v370\",\"Node v371\",\"Node v372\",\"Node v373\","
                          "\"Node v374\",\"Node v375\",\"Node v376\",\"Node v377\",\"Node v378\",\"Node v379\","
                          "\"Node v380\",\"Node v381\",\"Node v382\",\"Node v383\",\"Node v384\",\"Node v385\","
                          "\"Node v386\",\"Node v387\",\"Node v388\",\"Node v389\",\"Node v390\",\"Node v391\","
                          "\"Node v392\",\"Node v393\",\"Node v394\",\"Node v395\",\"Node v396\","
                          "\"Node v397\",\"Node v398\",\"Node v399\",\"Node v400\",\"Node v401\",\"Node v402\","
                          "\"Node v403\",\"Node v418\",\"Node v419\",\"Node v420\",\"Node v421\","
                          "\"Node v422\",\"Node v423\",\"Node v424\",\"Node v425\",\"Node v426\","
                          "\"Node v427\",\"Node v428\",\"Node v429\",\"Node v430\",\"Node v431\",\"Node v432\","
                          "\"Node v433\",\"Node v434\",\"Node v435\",\"Node v436\",\"Node v437\",\"Node v438\","
                          "\"Node v439\",\"Node v440\",\"Node v441\",\"Node v442\",\"Node v443\",\"Node v444\","
                          "\"Node v445\",\"Node v446\",\"Node v447\",\"Node v448\",\"Node v449\",\"Node v450\","
                          "\"Node v451\",\"Node v452\",\"Node v453\",\"Node v454\",\"Node v455\",\"Node v456\","
                          "\"Node v457\",\"Node v458\",\"Node v459\",\"Node v460\","
                          "\"Node v461\",\"Node v462\",\"Node v463\",\"Node v464\","
                          "\"Node v465\",\"Node v466\",\"Node v467\","
                          "\"Node v468\",\"Node v469\",\"Node v470\",\"Node v471\",\"Node v472\","
                          "\"Node v473\",\"Node v474\",\"Node v475\",\"Node v476\",\"Node v477\","
                          "\"Node v478\",\"Node v479\",\"Node v480\"]");
    assert_contains(json, "\"changesArchivedNodeEvidence\":false");
    assert_contains(json, "\"historicalFallback\":{\"previousConsumedReleaseVersion\":\"v419\"");
    assert_contains(json, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v419.json\"");
    assert_contains(json,
                    "\"previousConsumptionNodeVersion\":\"Node v550+ may consume v420 as read-only shard route preview "
                    "verification report archive closeout verification audit release-package evidence only\"");
    assert_contains(json, "\"rollingCurrentUsedForHistoricalBaseline\":false");
    assert_contains(json, "\"nodeV396ProgressIntakePreserved\":true");
    assert_contains(json, "\"nodeV397ContributionReviewPreserved\":true");
    assert_contains(json, "\"nodeV398ApprovalGateReviewPreserved\":true");
    assert_contains(json, "\"nodeV399ArchiveVerificationPreserved\":true");
    assert_contains(json, "\"nodeV400InputContractPreserved\":true");
    assert_contains(json, "\"nodeV401CompletionIntakePreserved\":true");
    assert_contains(json, "\"nodeV402TemplateValidatorPreserved\":true");
    assert_contains(json, "\"nodeV403TemplateCompatibilityIntakePreserved\":true");
    assert_contains(json, "\"nodeV404RequiresRealCanonicalInputs\":true");
    assert_contains(json, "\"nodeV418RouteGroupSplitCompatibilityPreserved\":true");
    assert_contains(json, "\"nodeV419BoundaryCatalogMaintenanceReady\":true");
    assert_contains(json, "\"nodeV459AuditRouteGroupCatalogPreserved\":true");
    assert_contains(json, "\"nodeV460AuditRouteCatalogIntegrityPreserved\":true");
    assert_contains(json, "\"nodeV461AuditRouteCatalogTestSupportPreserved\":true");
    assert_contains(json, "\"nodeV462AuditRouteCatalogTestMigrationPreserved\":true");
    assert_contains(json, "\"nodeV463AuditRouteQualityCatalogIntegrityPreserved\":true");
    assert_contains(json, "\"nodeV464AuditRouteCatalogFinalCloseoutPreserved\":true");
    assert_contains(json, "\"nodeV464AuditRouteCatalogFinalCloseoutSnapshotPreserved\":true");
    assert_contains(json, "\"nodeV465CodeHealthRouteGroupEvidencePreserved\":true");
    assert_contains(json, "\"nodeV466CentralRouteAnchorExtractionPreserved\":true");
    assert_contains(json, "\"nodeV467RouteCatalogAnchorRemovalPreserved\":true");
    assert_contains(json, "\"nodeV468RouteCatalogIntegrityAnchorFieldRemovalPreserved\":true");
    assert_contains(json, "\"nodeV469RouteCatalogSummaryBuilderPreserved\":true");
    assert_contains(json, "\"nodeV470RouteCatalogExpectedSummaryPreserved\":true");
    assert_contains(json, "\"nodeV471RouteCatalogExpectedIntegritySnapshotPreserved\":true");
    assert_contains(json, "\"nodeV472RouteCatalogCleanupCloseoutPreserved\":true");
    assert_contains(json, "\"nodeV473JavaMiniKvRouteCatalogCleanupHandoffEvidencePreserved\":true");
    assert_contains(json, "\"nodeV474JavaMiniKvRouteCatalogCleanupHandoffEvidenceReportPreserved\":true");
    assert_contains(json, "\"nodeV475JavaMiniKvLatestRouteCatalogCleanupEvidenceIntakePreserved\":true");
    assert_contains(json, "\"nodeV476JavaMiniKvLatestRouteCatalogCleanupEvidenceReportPreserved\":true");
    assert_contains(json, "\"nodeV477JavaMiniKvLatestRouteCatalogCleanupEvidenceReportArchivePreserved\":true");
    assert_contains(json, "\"nodeV478JavaMiniKvLatestRouteCatalogCleanupEvidenceArchiveVerificationPreserved\":true");
    assert_contains(json,
                    "\"nodeV479JavaMiniKvLatestRouteCatalogCleanupEvidenceArchiveVerificationRoutePreserved\":true");
    assert_contains(json, "\"nodeV480JavaMiniKvRouteCatalogCleanupEvidenceBatchCloseoutPreserved\":true");
    assert_contains(json, "\"shardReadinessHistoryMaintenance\":{\"maintenanceMode\":"
                          "\"history-fixture-archive-formatter-split-read-only\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v179\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v179.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_contains(json, "\"splitFrom\":\"src/shard_readiness.cpp\"");
    assert_contains(json, "\"splitInto\":\"src/shard_readiness_history.cpp\"");
    assert_contains(json, "\"preservesFixtureParity\":true");
    assert_contains(json, "\"preservesArchiveCompatibility\":true");
    assert_contains(json, "\"preservesHistoricalFallback\":true");
    assert_contains(json, "\"changesShardJsonCommand\":false");
    assert_contains(json, "\"changesFixturePath\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"nodeV420PreImplementationReadinessRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV421ApprovalRequiredReadinessRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV422ImplementationPlanRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV423FakeHarnessContractRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV424FakeHarnessExecutionReadinessRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV425DisabledRuntimeShellReadinessRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV426RuntimeShellDecisionRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV427RuntimeShellPostDecisionRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV428RuntimeShellPrerequisiteRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV429HumanApprovalArtifactReviewRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV430SignedHumanApprovalArtifactRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV431CredentialHandleApprovalRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV432EndpointHandleAllowlistApprovalRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV433NoNetworkSafetyFixtureRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV434FinalPrerequisiteRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV435ImplementationCandidateGateRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV436DesignDraftCandidateRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV437DesignDraftOutlineRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV438DesignDraftBodyIntakeRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV439DesignDraftBodyCandidateRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV440DesignDraftBodyPreDraftDecisionRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV441DesignDraftBodyPreparationPlanRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV442DesignDraftBodyDraftCandidateRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV443ManagedAuditAdapterRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV444ManagedAuditPersistenceRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV445ManagedAuditIdentityApprovalRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV446ManagedAuditRestoreDrillRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV447ManagedAuditDryRunAdapterRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV448ManagedAuditRouteQualityRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV449ManagedAuditAdapterImplementationRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV450ManagedAuditLocalAdapterRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV451ManagedAuditSandboxAdapterRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV452ManualSandboxConnectionPacketRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV453ManualSandboxConnectionReadinessRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV454ManualSandboxConnectionCommandRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV455ManualSandboxConnectionPrecheckRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV456ManualSandboxConnectionAdapterClientRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV457ManualSandboxConnectionFakeTransportRouteSplitPreserved\":true");
    assert_contains(json, "\"nodeV458FoundationalAuditRouteSplitPreserved\":true");
    assert_contains(json, "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_contains(json, "\"activeShardPrototypeAllowed\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"shardDirectoryCreationAllowed\":false");
    assert_contains(json, "\"multiProcessStartAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"requiredBeforeActivation\":[\"separate active shard prototype plan\"");
    assert_contains(json, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(json, "\"preservesActivePrototypePlan\":true");
    assert_contains(json, "\"frozenActiveShardPrototypeAllowed\":false");
    assert_contains(json, "\"frozenRouterActivationAllowed\":false");
    assert_contains(json, "\"frozenWriteRoutingAllowed\":false");
    assert_contains(json, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(json, "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_contains(json, "\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"readyForNodeConsumption\":true");
    assert_contains(json, "\"liveReadGateRequiredBeforeRuntimeProbe\":true");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"activeShardPrototypeEnabled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiredConsumerChecks\":[\"read frozen fixture instead of rolling current\"");
    assert_contains(json, "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_contains(json, "\"liveReadGateAllowed\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"requiresServiceOwner\":true");
    assert_contains(json, "\"requiresPortList\":true");
    assert_contains(json, "\"requiresSmokeTarget\":true");
    assert_contains(json, "\"requiresFailClosedBehavior\":true");
    assert_contains(json, "\"requiresCleanup\":true");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiredBeforeLiveRead\":[\"explicit live-read gate plan\"");
    assert_contains(json, "\"liveReadGatePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(json, "\"preservesLiveReadGatePlan\":true");
    assert_contains(json, "\"frozenLiveReadGateAllowed\":false");
    assert_contains(json, "\"frozenRuntimeProbeAllowed\":false");
    assert_contains(json, "\"frozenStartsServices\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(json, "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"operatorOwnedServiceLifecycleRequired\":true");
    assert_contains(json, "\"serviceOwnerDeclared\":false");
    assert_contains(json, "\"startupCommandDeclared\":false");
    assert_contains(json, "\"portListDeclared\":false");
    assert_contains(json, "\"getOnlySmokeTargetDeclared\":false");
    assert_contains(json, "\"failClosedBehaviorRequired\":true");
    assert_contains(json, "\"cleanupResponsibilityDeclared\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiredOperatorEvidence\":[\"service owner\"");
    assert_contains(json, "\"operatorServiceLifecycleTemplateFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(json, "\"preservesOperatorLifecycleTemplate\":true");
    assert_contains(json, "\"frozenServiceOwnerDeclared\":false");
    assert_contains(json, "\"frozenStartupCommandDeclared\":false");
    assert_contains(json, "\"frozenPortListDeclared\":false");
    assert_contains(json, "\"frozenGetOnlySmokeTargetDeclared\":false");
    assert_contains(json, "\"frozenCleanupResponsibilityDeclared\":false");
    assert_contains(json, "\"frozenRuntimeProbeAllowed\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v153.json\"");
    assert_contains(json, "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(json, "\"serviceOwnerDeclared\":true");
    assert_contains(json, "\"serviceOwnerHandle\":\"mini-kv-local-operator\"");
    assert_contains(json, "\"startupCommandDeclared\":true");
    assert_contains(json, "\"startupCommand\":\"minikv_server.exe <operator-approved-loopback-port> 127.0.0.1\"");
    assert_contains(json, "\"portListDeclared\":true");
    assert_contains(json, "\"declaredPortHandles\":[\"operator-approved-loopback-port\"]");
    assert_contains(json, "\"getOnlySmokeTargetDeclared\":true");
    assert_contains(json, "\"getOnlySmokeTarget\":\"GET-only SHARDJSON evidence read\"");
    assert_contains(json, "\"failClosedBehaviorDeclared\":true");
    assert_contains(json, "\"failClosedBehavior\":\"missing evidence or port conflict blocks runtime gate\"");
    assert_contains(json, "\"cleanupResponsibilityDeclared\":true");
    assert_contains(json, "\"cleanupResponsibility\":\"operator stops service and verifies no LISTENING port\"");
    assert_contains(json, "\"runtimeGateApproved\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"activeShardPrototypeEnabled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiresSeparateRuntimeGate\":true");
    assert_contains(json, "\"requiredBeforeRuntimeGate\":[\"operator approval record\"");
    assert_contains(json, "\"operatorServiceLifecycleEvidenceFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_contains(json, "\"preservesDeclaredLifecycleEvidence\":true");
    assert_contains(json, "\"frozenOperatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(json, "\"frozenRuntimeGateApproved\":false");
    assert_contains(json, "\"frozenRuntimeProbeAllowed\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                          "\"blocked-missing-runtime-execution-artifacts\"");
    assert_contains(json, "\"readyForRuntimeExecutionPacket\":false");
    assert_contains(json, "\"readyForRuntimeLiveReadGate\":false");
    assert_contains(json, "\"runtimeExecutionArtifactsComplete\":false");
    assert_contains(json, "\"presentRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"missingRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"requiredRuntimeExecutionArtifacts\":[\"operator approval record\","
                          "\"concrete loopback port assignment\",\"GET-only smoke command\","
                          "\"cleanup proof\",\"service owner confirmation\",\"process cleanup rules\"]");
    assert_contains(json, "\"runtimeExecutionPacketPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"executionAttempted\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"failClosedOnMissingArtifacts\":true");
    assert_contains(json, "\"runtimeExecutionArtifactIntakePreflightFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\"");
    assert_contains(json, "\"frozenStatus\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\"");
    assert_contains(json, "\"preservesBlockedPreflight\":true");
    assert_contains(json, "\"frozenPresentRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"frozenMissingRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                          "\"mini-kv-side-candidate-no-runtime\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\"");
    assert_contains(json, "\"candidateArtifactCount\":4");
    assert_contains(json, "\"candidateArtifactRows\":[\"mini-kv loopback port candidate\","
                          "\"mini-kv GET-only smoke command candidate\","
                          "\"mini-kv service owner candidate\",\"mini-kv process cleanup rules candidate\"]");
    assert_contains(json, "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"missingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"acceptedRuntimeExecutionArtifactsComplete\":false");
    assert_contains(json, "\"operatorApprovalRecordPresent\":false");
    assert_contains(json, "\"nodeRuntimeWindowApproved\":false");
    assert_contains(json, "\"crossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(json, "\"javaPortBindingPresent\":false");
    assert_contains(json, "\"miniKvLoopbackPortCandidateDeclared\":true");
    assert_contains(json, "\"miniKvLoopbackHost\":\"127.0.0.1\"");
    assert_contains(json, "\"miniKvLoopbackPortCandidate\":6424");
    assert_contains(json, "\"miniKvLoopbackPortOperatorApproved\":false");
    assert_contains(json, "\"startupCommandCandidate\":\"minikv_server.exe 6424 127.0.0.1\"");
    assert_contains(
        json,
        "\"getOnlySmokeCommandCandidate\":\"minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream\"");
    assert_contains(json, "\"getOnlySmokeCommandOperatorApproved\":false");
    assert_contains(json, "\"serviceOwnerCandidateDeclared\":true");
    assert_contains(json, "\"serviceOwnerOperatorConfirmed\":false");
    assert_contains(json, "\"processCleanupRulesCandidateDeclared\":true");
    assert_contains(json, "\"cleanupProofPresent\":false");
    assert_contains(json, "\"requiresCrossProjectPacket\":true");
    assert_contains(json, "\"miniKvRuntimeExecutionArtifactCandidateFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"preservesCandidateOnlyContribution\":true");
    assert_contains(json, "\"frozenCandidateArtifactCount\":4");
    assert_contains(json, "\"frozenAcceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"frozenMissingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                          "\"blocked-missing-approval-gate-inputs\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v155\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v155.json\"");
    assert_contains(json, "\"sourceNodeReviewVersions\":[\"Node v397 contribution review\","
                          "\"Node v398 approval gate review\",\"Node v399 approval gate archive verification\","
                          "\"Node v400 approval input intake contract\"]");
    assert_contains(json, "\"approvalGateInputCount\":0");
    assert_contains(json, "\"missingApprovalGateInputCount\":3");
    assert_contains(json, "\"requiredApprovalGateInputs\":[\"Node-approved runtime window\","
                          "\"correlated operator approval record\","
                          "\"complete cross-project runtime execution packet\"]");
    assert_contains(json, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(json, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(json, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(json, "\"approvalGateInputsComplete\":false");
    assert_contains(json, "\"runtimeGateApprovalPresent\":false");
    assert_contains(json, "\"concreteLoopbackPortsAssigned\":false");
    assert_contains(json, "\"startsJavaService\":false");
    assert_contains(json, "\"failClosedOnMissingApprovalInputs\":true");
    assert_contains(json, "\"runtimeExecutionApprovalGateInputPrecheckFreeze\":{\"frozenReleaseVersion\":\"v155\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v155.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-approval-gate-input-precheck-blocked-read-only\"");
    assert_contains(json, "\"preservesApprovalGateInputPrecheck\":true");
    assert_contains(json, "\"frozenApprovalGateInputCount\":0");
    assert_contains(json, "\"frozenMissingApprovalGateInputCount\":3");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsJavaService\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json,
                    "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_contains(json, "\"sourceNodeContractVersion\":\"Node v400 approval input intake contract\"");
    assert_contains(json, "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_contains(json, "\"miniKvApprovalGateInputComplete\":true");
    assert_contains(json, "\"miniKvApprovalGateInputItemCount\":4");
    assert_contains(json, "\"miniKvApprovalGateInputItems\":[\"concrete mini-kv loopback port assignment\","
                          "\"GET-only SHARDJSON smoke command\",\"mini-kv service owner confirmation\","
                          "\"stop-only-owned-process cleanup rules\"]");
    assert_contains(json, "\"miniKvLoopbackHost\":\"127.0.0.1\"");
    assert_contains(json, "\"miniKvLoopbackPort\":6424");
    assert_contains(json, "\"startupCommand\":\"minikv_server.exe 6424 127.0.0.1\"");
    assert_contains(json,
                    "\"getOnlySmokeCommand\":\"minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream\"");
    assert_contains(json, "\"serviceOwnerConfirmed\":true");
    assert_contains(json, "\"processCleanupRulesComplete\":true");
    assert_contains(json, "\"stopOnlyOwnedProcessCleanupRequired\":true");
    assert_contains(json, "\"cleanupProofPresent\":false");
    assert_contains(json, "\"cleanupProofRequiresApprovedRuntimeStart\":true");
    assert_contains(json, "\"requiresNodeRuntimeWindow\":true");
    assert_contains(json, "\"requiresCorrelatedOperatorApproval\":true");
    assert_contains(json, "\"requiresCompleteCrossProjectPacket\":true");
    assert_contains(json, "\"miniKvFinalApprovalGateInputFreeze\":{\"frozenReleaseVersion\":\"v156\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v156.json\"");
    assert_contains(json, "\"frozenStatus\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\"");
    assert_contains(json, "\"preservesMiniKvFinalApprovalGateInput\":true");
    assert_contains(json, "\"frozenMiniKvApprovalGateInputComplete\":true");
    assert_contains(json, "\"frozenMiniKvLoopbackPort\":6424");
    assert_contains(json, "\"frozenCleanupProofPresent\":false");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenRouterActivationAllowed\":false");
    assert_contains(json, "\"frozenWriteRoutingAllowed\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                          "\"template-validator-echo-no-canonical-inputs\"");
    assert_contains(json,
                    "\"sourceNodeValidatorVersion\":\"Node v402 runtime execution approval input template validator\"");
    assert_contains(
        json,
        "\"sourceNodePlan\":\"docs/plans3/v402-post-runtime-execution-approval-input-template-validator-roadmap.md\"");
    assert_contains(json, "\"templateArchiveRoot\":\"e/402/input-templates\"");
    assert_contains(json, "\"canonicalInputRoot\":\"e/398/input\"");
    assert_contains(json, "\"templateOnlyInputCount\":3");
    assert_contains(
        json, "\"templateArchivePaths\":[\"e/402/input-templates/node-approved-runtime-window-v402.template.json\","
              "\"e/402/input-templates/correlated-operator-approval-record-v402.template.json\","
              "\"e/402/input-templates/cross-project-runtime-execution-packet-v402.template.json\"]");
    assert_contains(json, "\"canonicalTargetPaths\":[\"e/398/input/node-approved-runtime-window-v398.json\","
                          "\"e/398/input/correlated-operator-approval-record-v398.json\","
                          "\"e/398/input/cross-project-runtime-execution-packet-v398.json\"]");
    assert_contains(json, "\"canonicalRuntimeInputPresent\":false");
    assert_contains(json, "\"templateCopiedToCanonicalInput\":false");
    assert_contains(json, "\"nodeApprovedRuntimeWindowCanonicalPresent\":false");
    assert_contains(json, "\"correlatedOperatorApprovalRecordCanonicalPresent\":false");
    assert_contains(json, "\"completeCrossProjectRuntimeExecutionPacketCanonicalPresent\":false");
    assert_contains(json, "\"sharedApprovalCorrelationIdPresent\":false");
    assert_contains(json, "\"templatesAuthorizeRuntime\":false");
    assert_contains(json, "\"templateDigestAcceptedAsApproval\":false");
    assert_contains(json, "\"writesCanonicalApprovalInputFiles\":false");
    assert_contains(json, "\"changesNodeInputTemplateFiles\":false");
    assert_contains(json, "\"requiresCanonicalInputs\":true");
    assert_contains(json, "\"requiresSharedApprovalCorrelationId\":true");
    assert_contains(json, "\"requiresGetOnlySmokeCommands\":true");
    assert_contains(json, "\"requiresCleanupProofAfterApprovedStart\":true");
    assert_contains(json, "\"failClosedOnTemplateOnlyInputs\":true");
    assert_contains(json,
                    "\"runtimeExecutionApprovalInputTemplateValidatorEchoFreeze\":{\"frozenReleaseVersion\":\"v157\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v157.json\"");
    assert_contains(json, "\"frozenStatus\":\"runtime-execution-approval-input-template-validator-echo-read-only\"");
    assert_contains(json, "\"preservesTemplateValidatorEcho\":true");
    assert_contains(json, "\"frozenTemplateOnlyInputCount\":3");
    assert_contains(json, "\"frozenCanonicalRuntimeInputPresent\":false");
    assert_contains(json, "\"frozenTemplateCopiedToCanonicalInput\":false");
    assert_contains(json, "\"frozenTemplatesAuthorizeRuntime\":false");
    assert_contains(json, "\"frozenWritesCanonicalApprovalInputFiles\":false");
    assert_contains(json, "\"frozenChangesNodeInputTemplateFiles\":false");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenRouterActivationAllowed\":false");
    assert_contains(json, "\"frozenWriteRoutingAllowed\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json, "\"runtimeExecutionCanonicalApprovalInputPrecheck\":{\"precheckMode\":"
                          "\"blocked-missing-canonical-approval-inputs\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v157\"");
    assert_contains(json, "\"sourceNodeCompatibilityVersion\":\"Node v403 template compatibility intake\"");
    assert_contains(
        json, "\"sourceNodePlan\":\"docs/plans3/"
              "v403-post-java-mini-kv-runtime-execution-approval-input-template-compatibility-intake-roadmap.md\"");
    assert_contains(json, "\"requiredCanonicalInputCount\":3");
    assert_contains(json, "\"presentCanonicalInputCount\":0");
    assert_contains(json, "\"missingCanonicalInputCount\":3");
    assert_contains(json, "\"requiredCanonicalInputPaths\":[\"e/398/input/node-approved-runtime-window-v398.json\","
                          "\"e/398/input/correlated-operator-approval-record-v398.json\","
                          "\"e/398/input/cross-project-runtime-execution-packet-v398.json\"]");
    assert_contains(json, "\"canonicalApprovalInputsComplete\":false");
    assert_contains(json, "\"sharedApprovalCorrelationIdPresent\":false");
    assert_contains(json, "\"sharedApprovalCorrelationIdValidated\":false");
    assert_contains(json, "\"templatesAcceptedAsCanonicalInputs\":false");
    assert_contains(json, "\"templateCompatibilityEvidenceAcceptedAsApproval\":false");
    assert_contains(json, "\"runtimeExecutionPacketPresent\":false");
    assert_contains(json, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"startsMiniKvService\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"requiresRealCanonicalInputs\":true");
    assert_contains(json, "\"requiresConcreteApprovalValues\":true");
    assert_contains(json, "\"failClosedOnMissingCanonicalInputs\":true");
}

} // namespace minikv::shard_test
