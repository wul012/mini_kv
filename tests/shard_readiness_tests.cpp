#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {

void assert_contains(const std::string& text, std::string_view expected) {
    if (text.find(std::string{expected}) == std::string::npos) {
        std::cerr << "missing expected text: " << expected << '\n';
    }
    assert(text.find(std::string{expected}) != std::string::npos);
}

std::string read_fixture_text(const std::filesystem::path& relative_path) {
    const auto path = std::filesystem::path{MINIKV_SOURCE_DIR} / relative_path;
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());
    std::ostringstream output;
    output << input.rdbuf();
    auto text = output.str();
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
        text.pop_back();
    }
    return text;
}

void assert_shard_readiness_contract(const std::string& json) {
    assert_contains(json, "\"contract\":\"shard-readiness.v1\"");
    assert_contains(json, "\"evidenceType\":\"shard_readiness\"");
    assert_contains(json, "\"project\":\"mini-kv\"");
    assert_contains(json, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(json, "\"releaseVersion\":\"v189\"");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardEnabled\":false");
    assert_contains(json, "\"shardCount\":1");
    assert_contains(json, "\"slotCount\":16");
    assert_contains(json, "\"routingMode\":\"single-shard-readiness-prototype\"");
    assert_contains(json, "\"evidencePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(json, "\"status\":\"node-route-catalog-evidence-chain-read-only\"");
    assert_contains(json, "\"shardId\":\"shard-0\"");
    assert_contains(json, "\"storagePath\":\"not-created\"");
    assert_contains(json, "\"writesAllowed\":false");
    assert_contains(json, "\"key\":\"orderops:alpha\",\"slot\":5,\"shardId\":\"shard-0\"");
    assert_contains(json, "\"key\":\"audit:receipt:001\",\"slot\":9,\"shardId\":\"shard-0\"");
    assert_contains(json, "\"slotTablePreview\":{\"previewMode\":\"single-shard-slot-table-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v425-post-credential-resolver-disabled-runtime-shell-readiness-route-group-split-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v161\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v161.json\"");
    assert_contains(json, "\"slotTableMaterializedForEvidence\":true");
    assert_contains(json, "\"slotTablePreviewOnly\":true");
    assert_contains(json, "\"readOnlyShardMapPreview\":true");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"routerActivationAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"writeCommandsAllowed\":false");
    assert_contains(json, "\"adminCommandsAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"startsServices\":false");
    assert_contains(json, "\"runtimeProbeAllowed\":false");
    assert_contains(json, "\"liveReadAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"slotAssignments\":[{\"slot\":0,\"shardId\":\"shard-0\","
                          "\"routeMode\":\"preview-only\",\"readOnly\":true,\"writesAllowed\":false,"
                          "\"routerInstalled\":false}");
    assert_contains(json, "{\"slot\":15,\"shardId\":\"shard-0\",\"routeMode\":\"preview-only\","
                          "\"readOnly\":true,\"writesAllowed\":false,\"routerInstalled\":false}]");
    assert_contains(json, "\"slotTablePreviewAudit\":{\"auditMode\":\"slot-table-preview-consistency-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v426-post-credential-resolver-runtime-shell-decision-route-group-split-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v162\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\"");
    assert_contains(json, "\"auditedPreviewSection\":\"slotTablePreview\"");
    assert_contains(json, "\"expectedSlotCount\":16");
    assert_contains(json, "\"observedSlotCount\":16");
    assert_contains(json, "\"slotRangeStart\":0");
    assert_contains(json, "\"slotRangeEnd\":15");
    assert_contains(json, "\"contiguousSlotRange\":true");
    assert_contains(json, "\"duplicateSlotsDetected\":false");
    assert_contains(json, "\"unassignedSlotsDetected\":false");
    assert_contains(json, "\"allSlotsMapped\":true");
    assert_contains(json, "\"uniqueShardCount\":1");
    assert_contains(json, "\"allAssignmentsReadOnly\":true");
    assert_contains(json, "\"allAssignmentsPreviewOnly\":true");
    assert_contains(json, "\"derivedFromSlotPreviewModule\":true");
    assert_contains(json, "\"slotTablePreviewAuditMaintenance\":{\"maintenanceMode\":"
                          "\"slot-table-preview-audit-formatter-split-contract-preserving\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v430-post-credential-resolver-signed-human-approval-artifact-route-group-split-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v163\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v163.json\"");
    assert_contains(json, "\"auditFormatterExtracted\":true");
    assert_contains(json, "\"slotPreviewModuleOwnsAssignments\":true");
    assert_contains(json, "\"slotPreviewAuditStillPublished\":true");
    assert_contains(json, "\"slotTablePreviewStillPublished\":true");
    assert_contains(json, "\"publicShardJsonContractChanged\":false");
    assert_contains(json, "\"fixturePathChanged\":false");
    assert_contains(json, "\"shardJsonCommandChanged\":false");
    assert_contains(json, "\"nodeAddsEvidenceGate\":false");
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
    assert_contains(json, "\"shardReadinessReleaseCatalogAudit\":{\"auditMode\":"
                          "\"shard-readiness-release-catalog-consistency-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
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
    assert_contains(
        json,
        "\"nodeConsumer\":\"Node v471+ may consume v189 as a versioned route catalog evidence chain only\"");
    assert_contains(json, "\"nodeArchivedEvidencePreserved\":true");
    assert_contains(json, "\"commandCatalog\":{\"command\":\"SHARDJSON\",\"category\":\"read\"");
    assert_contains(json, "\"mutatesStore\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"extraArgsAllowed\":false");
    assert_contains(json, "\"sideEffects\":[\"metadata_read\"]");
    assert_contains(json, "\"fixtureParity\":{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(json, "\"historicalFixturePaths\":[\"fixtures/release/shard-readiness-v144.json\","
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
                          "\"fixtures/release/shard-readiness-v188.json\"]");
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
                          "\"Node v468\",\"Node v469\",\"Node v470\",\"Node v471\"]");
    assert_contains(json, "\"changesArchivedNodeEvidence\":false");
    assert_contains(json, "\"historicalFallback\":{\"previousConsumedReleaseVersion\":\"v188\"");
    assert_contains(json, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_contains(
        json,
        "\"previousConsumptionNodeVersion\":\"Node v471 route catalog evidence chain may consume v188 frozen audit evidence\"");
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
    assert_contains(json,
                    "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
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
    assert_contains(json,
                    "\"runtimeExecutionArtifactIntakePreflightFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v154.json\"");
    assert_contains(json, "\"frozenStatus\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\"");
    assert_contains(json, "\"preservesBlockedPreflight\":true");
    assert_contains(json, "\"frozenPresentRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"frozenMissingRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json,
                    "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
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
    assert_contains(json,
                    "\"getOnlySmokeCommandCandidate\":\"minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream\"");
    assert_contains(json, "\"getOnlySmokeCommandOperatorApproved\":false");
    assert_contains(json, "\"serviceOwnerCandidateDeclared\":true");
    assert_contains(json, "\"serviceOwnerOperatorConfirmed\":false");
    assert_contains(json, "\"processCleanupRulesCandidateDeclared\":true");
    assert_contains(json, "\"cleanupProofPresent\":false");
    assert_contains(json, "\"requiresCrossProjectPacket\":true");
    assert_contains(json,
                    "\"miniKvRuntimeExecutionArtifactCandidateFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_contains(json, "\"preservesCandidateOnlyContribution\":true");
    assert_contains(json, "\"frozenCandidateArtifactCount\":4");
    assert_contains(json, "\"frozenAcceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"frozenMissingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_contains(json, "\"frozenStartsMiniKvService\":false");
    assert_contains(json, "\"frozenExecutionAllowed\":false");
    assert_contains(json,
                    "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
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
    assert_contains(json,
                    "\"runtimeExecutionApprovalGateInputPrecheckFreeze\":{\"frozenReleaseVersion\":\"v155\"");
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
    assert_contains(json,
                    "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                    "\"template-validator-echo-no-canonical-inputs\"");
    assert_contains(json, "\"sourceNodeValidatorVersion\":\"Node v402 runtime execution approval input template validator\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v402-post-runtime-execution-approval-input-template-validator-roadmap.md\"");
    assert_contains(json, "\"templateArchiveRoot\":\"e/402/input-templates\"");
    assert_contains(json, "\"canonicalInputRoot\":\"e/398/input\"");
    assert_contains(json, "\"templateOnlyInputCount\":3");
    assert_contains(json, "\"templateArchivePaths\":[\"e/402/input-templates/node-approved-runtime-window-v402.template.json\","
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
    assert_contains(json,
                    "\"runtimeExecutionCanonicalApprovalInputPrecheck\":{\"precheckMode\":"
                    "\"blocked-missing-canonical-approval-inputs\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v157\"");
    assert_contains(json, "\"sourceNodeCompatibilityVersion\":\"Node v403 template compatibility intake\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v403-post-java-mini-kv-runtime-execution-approval-input-template-compatibility-intake-roadmap.md\"");
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
    assert_contains(json, "\"nodeRouteGroupSplitCompatibility\":{\"compatibilityMode\":"
                          "\"node-route-group-split-contract-stable\"");
    assert_contains(json, "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v418-post-sandbox-endpoint-credential-resolver-route-group-split-roadmap.md\"");
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
                          "\"node-v433-v471-route-catalog-expected-integrity-snapshot-freeze-window-read-only\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v471-post-route-catalog-expected-integrity-snapshot-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_contains(json, "\"windowStartNodeVersion\":\"Node v433\"");
    assert_contains(json, "\"windowEndNodeVersion\":\"Node v471\"");
    assert_contains(json, "\"splitRouteGroupCount\":39");
    assert_contains(json, "\"coveredNodeVersions\":[\"Node v433\",\"Node v434\",\"Node v435\",\"Node v436\","
                          "\"Node v437\",\"Node v438\",\"Node v439\",\"Node v440\",\"Node v441\","
                          "\"Node v442\",\"Node v443\",\"Node v444\",\"Node v445\",\"Node v446\","
                          "\"Node v447\",\"Node v448\",\"Node v449\",\"Node v450\",\"Node v451\","
                          "\"Node v452\",\"Node v453\",\"Node v454\",\"Node v455\",\"Node v456\","
                          "\"Node v457\",\"Node v458\",\"Node v459\",\"Node v460\","
                          "\"Node v461\",\"Node v462\",\"Node v463\",\"Node v464\","
                          "\"Node v465\",\"Node v466\",\"Node v467\","
                          "\"Node v468\",\"Node v469\",\"Node v470\",\"Node v471\"]");
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
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_contains(json, "\"auditedWindowSection\":\"nodeRouteSplitCompatibilityWindow\"");
    assert_contains(json, "\"expectedWindowVersionCount\":39");
    assert_contains(json, "\"observedWindowVersionCount\":39");
    assert_contains(json, "\"windowRangeStart\":\"Node v433\"");
    assert_contains(json, "\"windowRangeEnd\":\"Node v471\"");
    assert_contains(json, "\"windowRangeStartNumber\":433");
    assert_contains(json, "\"windowRangeEndNumber\":471");
    assert_contains(json, "\"computedWindowVersionSpan\":39");
    assert_contains(json, "\"windowRangeNumbersParseable\":true");
    assert_contains(json, "\"windowCountMatchesRange\":true");
    assert_contains(json, "\"contiguousNodeVersionWindow\":true");
    assert_contains(json, "\"duplicateWindowVersionsDetected\":false");
    assert_contains(json, "\"allWindowVersionsRouteRegistrationOnly\":true");
    assert_contains(json, "\"sourceFrozenWindowDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
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
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v471-post-route-catalog-expected-integrity-snapshot-roadmap.md\"");
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
    assert_contains(json, "\"releaseVersion\":\"v186\",\"status\":\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(json,
                    "\"releaseVersion\":\"v187\",\"status\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(json,
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
    assert_contains(json, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v17\"");
    assert_contains(json, "\"sourceNodePlan\":"
                          "\"docs/plans3/"
                          "v471-post-route-catalog-expected-integrity-snapshot-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_contains(json, "\"fieldCount\":547");
    assert_contains(json, "\"groupCount\":28");
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
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.expectedIntegritySnapshotFactoryMovedByNodeV471\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.routeQualityServiceConsumesCatalogIntegrityFactory\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.miniKvOwnsNodeCatalogIntegritySnapshot\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.miniKvComputesNodeRouteSummary\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.miniKvOwnsNodeRouteQualitySnapshot\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.routerActivationAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.writeRoutingAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit.executionAllowed\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.rollingCurrentUsedForFrozenBaseline\"");
    assert_contains(json, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze.preservesExpectedIntegritySnapshotAudit\"");
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

} // namespace

int main() {
    const auto fixture_path = std::filesystem::path{"fixtures"} / "release" / "shard-readiness.json";
    const auto historical_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v144.json";
    const auto consumed_v145_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v145.json";
    const auto consumed_v146_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v146.json";
    const auto consumed_v147_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v147.json";
    const auto consumed_v148_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v148.json";
    const auto consumed_v149_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v149.json";
    const auto consumed_v150_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v150.json";
    const auto consumed_v151_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v151.json";
    const auto consumed_v152_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v152.json";
    const auto consumed_v153_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v153.json";
    const auto consumed_v154_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v154.json";
    const auto consumed_v155_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v155.json";
    const auto consumed_v156_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v156.json";
    const auto consumed_v157_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v157.json";
    const auto consumed_v158_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v158.json";
    const auto consumed_v159_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v159.json";
    const auto consumed_v160_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v160.json";
    const auto consumed_v161_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v161.json";
    const auto consumed_v162_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v162.json";
    const auto consumed_v163_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v163.json";
    const auto consumed_v164_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v164.json";
    const auto consumed_v165_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v165.json";
    const auto consumed_v166_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v166.json";
    const auto consumed_v167_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v167.json";
    const auto consumed_v168_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v168.json";
    const auto consumed_v169_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v169.json";
    const auto consumed_v170_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v170.json";
    const auto consumed_v171_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v171.json";
    const auto consumed_v172_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v172.json";
    const auto consumed_v173_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v173.json";
    const auto consumed_v174_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v174.json";
    const auto consumed_v175_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v175.json";
    const auto consumed_v176_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v176.json";
    const auto consumed_v177_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v177.json";
    const auto consumed_v178_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v178.json";
    const auto consumed_v179_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v179.json";
    const auto consumed_v180_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v180.json";
    const auto consumed_v181_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v181.json";
    const auto consumed_v182_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v182.json";
    const auto consumed_v183_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v183.json";
    const auto consumed_v184_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v184.json";
    const auto consumed_v185_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v185.json";
    const auto consumed_v186_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v186.json";
    const auto consumed_v187_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v187.json";
    const auto consumed_v188_fixture_path =
        std::filesystem::path{"fixtures"} / "release" / "shard-readiness-v188.json";
    const auto fixture = read_fixture_text(fixture_path);
    const auto historical_fixture = read_fixture_text(historical_fixture_path);
    const auto consumed_v145_fixture = read_fixture_text(consumed_v145_fixture_path);
    const auto consumed_v146_fixture = read_fixture_text(consumed_v146_fixture_path);
    const auto consumed_v147_fixture = read_fixture_text(consumed_v147_fixture_path);
    const auto consumed_v148_fixture = read_fixture_text(consumed_v148_fixture_path);
    const auto consumed_v149_fixture = read_fixture_text(consumed_v149_fixture_path);
    const auto consumed_v150_fixture = read_fixture_text(consumed_v150_fixture_path);
    const auto consumed_v151_fixture = read_fixture_text(consumed_v151_fixture_path);
    const auto consumed_v152_fixture = read_fixture_text(consumed_v152_fixture_path);
    const auto consumed_v153_fixture = read_fixture_text(consumed_v153_fixture_path);
    const auto consumed_v154_fixture = read_fixture_text(consumed_v154_fixture_path);
    const auto consumed_v155_fixture = read_fixture_text(consumed_v155_fixture_path);
    const auto consumed_v156_fixture = read_fixture_text(consumed_v156_fixture_path);
    const auto consumed_v157_fixture = read_fixture_text(consumed_v157_fixture_path);
    const auto consumed_v158_fixture = read_fixture_text(consumed_v158_fixture_path);
    const auto consumed_v159_fixture = read_fixture_text(consumed_v159_fixture_path);
    const auto consumed_v160_fixture = read_fixture_text(consumed_v160_fixture_path);
    const auto consumed_v161_fixture = read_fixture_text(consumed_v161_fixture_path);
    const auto consumed_v162_fixture = read_fixture_text(consumed_v162_fixture_path);
    const auto consumed_v163_fixture = read_fixture_text(consumed_v163_fixture_path);
    const auto consumed_v164_fixture = read_fixture_text(consumed_v164_fixture_path);
    const auto consumed_v165_fixture = read_fixture_text(consumed_v165_fixture_path);
    const auto consumed_v166_fixture = read_fixture_text(consumed_v166_fixture_path);
    const auto consumed_v167_fixture = read_fixture_text(consumed_v167_fixture_path);
    const auto consumed_v168_fixture = read_fixture_text(consumed_v168_fixture_path);
    const auto consumed_v169_fixture = read_fixture_text(consumed_v169_fixture_path);
    const auto consumed_v170_fixture = read_fixture_text(consumed_v170_fixture_path);
    const auto consumed_v171_fixture = read_fixture_text(consumed_v171_fixture_path);
    const auto consumed_v172_fixture = read_fixture_text(consumed_v172_fixture_path);
    const auto consumed_v173_fixture = read_fixture_text(consumed_v173_fixture_path);
    const auto consumed_v174_fixture = read_fixture_text(consumed_v174_fixture_path);
    const auto consumed_v175_fixture = read_fixture_text(consumed_v175_fixture_path);
    const auto consumed_v176_fixture = read_fixture_text(consumed_v176_fixture_path);
    const auto consumed_v177_fixture = read_fixture_text(consumed_v177_fixture_path);
    const auto consumed_v178_fixture = read_fixture_text(consumed_v178_fixture_path);
    const auto consumed_v179_fixture = read_fixture_text(consumed_v179_fixture_path);
    const auto consumed_v180_fixture = read_fixture_text(consumed_v180_fixture_path);
    const auto consumed_v181_fixture = read_fixture_text(consumed_v181_fixture_path);
    const auto consumed_v182_fixture = read_fixture_text(consumed_v182_fixture_path);
    const auto consumed_v183_fixture = read_fixture_text(consumed_v183_fixture_path);
    const auto consumed_v184_fixture = read_fixture_text(consumed_v184_fixture_path);
    const auto consumed_v185_fixture = read_fixture_text(consumed_v185_fixture_path);
    const auto consumed_v186_fixture = read_fixture_text(consumed_v186_fixture_path);
    const auto consumed_v187_fixture = read_fixture_text(consumed_v187_fixture_path);
    const auto consumed_v188_fixture = read_fixture_text(consumed_v188_fixture_path);

    assert(fixture == minikv::shard_readiness::format_json());
    assert(minikv::shard_readiness::fixture_path() == "fixtures/release/shard-readiness.json");
    assert_shard_readiness_contract(fixture);
    assert(fixture != historical_fixture);
    assert(fixture != consumed_v145_fixture);
    assert(fixture != consumed_v146_fixture);
    assert(fixture != consumed_v147_fixture);
    assert(fixture != consumed_v148_fixture);
    assert(fixture != consumed_v149_fixture);
    assert(fixture != consumed_v150_fixture);
    assert(fixture != consumed_v151_fixture);
    assert(fixture != consumed_v152_fixture);
    assert(fixture != consumed_v153_fixture);
    assert(fixture != consumed_v154_fixture);
    assert(fixture != consumed_v155_fixture);
    assert(fixture != consumed_v156_fixture);
    assert(fixture != consumed_v157_fixture);
    assert(fixture != consumed_v158_fixture);
    assert(fixture != consumed_v159_fixture);
    assert(fixture != consumed_v160_fixture);
    assert(fixture != consumed_v161_fixture);
    assert(fixture != consumed_v162_fixture);
    assert(fixture != consumed_v163_fixture);
    assert(fixture != consumed_v164_fixture);
    assert(fixture != consumed_v165_fixture);
    assert(fixture != consumed_v166_fixture);
    assert(fixture != consumed_v167_fixture);
    assert(fixture != consumed_v168_fixture);
    assert(fixture != consumed_v169_fixture);
    assert(fixture != consumed_v170_fixture);
    assert(fixture != consumed_v171_fixture);
    assert(fixture != consumed_v172_fixture);
    assert(fixture != consumed_v173_fixture);
    assert(fixture != consumed_v174_fixture);
    assert(fixture != consumed_v175_fixture);
    assert(fixture != consumed_v176_fixture);
    assert(fixture != consumed_v177_fixture);
    assert(fixture != consumed_v178_fixture);
    assert(fixture != consumed_v179_fixture);
    assert(fixture != consumed_v180_fixture);
    assert(fixture != consumed_v181_fixture);
    assert(fixture != consumed_v182_fixture);
    assert(fixture != consumed_v183_fixture);
    assert(fixture != consumed_v184_fixture);
    assert(fixture != consumed_v185_fixture);
    assert(fixture != consumed_v186_fixture);
    assert(fixture != consumed_v187_fixture);
    assert(fixture != consumed_v188_fixture);
    assert_contains(historical_fixture, "\"releaseVersion\":\"v144\"");
    assert_contains(historical_fixture, "\"status\":\"prototype-ready-read-only\"");
    assert_contains(historical_fixture, "\"evidenceDigest\":\"fnv1a64:22d3c4815a440804\"");
    assert_contains(consumed_v145_fixture, "\"releaseVersion\":\"v145\"");
    assert_contains(consumed_v145_fixture, "\"status\":\"hardened-read-only\"");
    assert_contains(consumed_v145_fixture, "\"evidenceDigest\":\"fnv1a64:ebe4c7e1a2704482\"");
    assert_contains(consumed_v145_fixture, "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_contains(consumed_v146_fixture, "\"releaseVersion\":\"v146\"");
    assert_contains(consumed_v146_fixture, "\"status\":\"historical-fallback-hardened-read-only\"");
    assert_contains(consumed_v146_fixture, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v145.json\"");
    assert_contains(consumed_v146_fixture, "\"evidenceDigest\":\"fnv1a64:6847d87decb76fcb\"");
    assert_contains(consumed_v147_fixture, "\"releaseVersion\":\"v147\"");
    assert_contains(consumed_v147_fixture, "\"status\":\"active-prototype-prerequisite-read-only\"");
    assert_contains(consumed_v147_fixture, "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_contains(consumed_v147_fixture, "\"activeShardPrototypeAllowed\":false");
    assert_contains(consumed_v147_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v147_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v147_fixture, "\"evidenceDigest\":\"fnv1a64:e4a386fc9add4eaf\"");
    assert_contains(consumed_v148_fixture, "\"releaseVersion\":\"v148\"");
    assert_contains(consumed_v148_fixture, "\"status\":\"active-prototype-plan-frozen-read-only\"");
    assert_contains(consumed_v148_fixture, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v147\"");
    assert_contains(consumed_v148_fixture, "\"frozenRouterActivationAllowed\":false");
    assert_contains(consumed_v148_fixture, "\"frozenWriteRoutingAllowed\":false");
    assert_contains(consumed_v148_fixture, "\"evidenceDigest\":\"fnv1a64:8270eeebbf1a7f29\"");
    assert_contains(consumed_v149_fixture, "\"releaseVersion\":\"v149\"");
    assert_contains(consumed_v149_fixture, "\"status\":\"frozen-evidence-handoff-read-only\"");
    assert_contains(consumed_v149_fixture, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v148\"");
    assert_contains(consumed_v149_fixture, "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_contains(consumed_v149_fixture, "\"startsServices\":false");
    assert_contains(consumed_v149_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v149_fixture, "\"evidenceDigest\":\"fnv1a64:521fe6dee47f7f2c\"");
    assert_contains(consumed_v150_fixture, "\"releaseVersion\":\"v150\"");
    assert_contains(consumed_v150_fixture, "\"status\":\"live-read-gate-prerequisite-read-only\"");
    assert_contains(consumed_v150_fixture, "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_contains(consumed_v150_fixture, "\"liveReadGateAllowed\":false");
    assert_contains(consumed_v150_fixture, "\"runtimeProbeAllowed\":false");
    assert_contains(consumed_v150_fixture, "\"startsServices\":false");
    assert_contains(consumed_v150_fixture, "\"evidenceDigest\":\"fnv1a64:b8b134f6aa527ca4\"");
    assert_contains(consumed_v151_fixture, "\"releaseVersion\":\"v151\"");
    assert_contains(consumed_v151_fixture, "\"status\":\"operator-service-lifecycle-template-read-only\"");
    assert_contains(consumed_v151_fixture, "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_contains(consumed_v151_fixture, "\"serviceOwnerDeclared\":false");
    assert_contains(consumed_v151_fixture, "\"startupCommandDeclared\":false");
    assert_contains(consumed_v151_fixture, "\"runtimeProbeAllowed\":false");
    assert_contains(consumed_v151_fixture, "\"evidenceDigest\":\"fnv1a64:c9fb568ddff895e3\"");
    assert_contains(consumed_v152_fixture, "\"releaseVersion\":\"v152\"");
    assert_contains(consumed_v152_fixture, "\"status\":\"declared-operator-lifecycle-no-runtime-read-only\"");
    assert_contains(consumed_v152_fixture,
                    "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_contains(consumed_v152_fixture, "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(consumed_v152_fixture, "\"serviceOwnerDeclared\":true");
    assert_contains(consumed_v152_fixture, "\"runtimeGateApproved\":false");
    assert_contains(consumed_v152_fixture, "\"runtimeProbeAllowed\":false");
    assert_contains(consumed_v152_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v152_fixture, "\"evidenceDigest\":\"fnv1a64:55cd5b8db109c64f\"");
    assert_contains(consumed_v153_fixture, "\"releaseVersion\":\"v153\"");
    assert_contains(consumed_v153_fixture,
                    "\"status\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(consumed_v153_fixture,
                    "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                    "\"blocked-missing-runtime-execution-artifacts\"");
    assert_contains(consumed_v153_fixture, "\"presentRuntimeExecutionArtifactCount\":0");
    assert_contains(consumed_v153_fixture, "\"missingRuntimeExecutionArtifactCount\":6");
    assert_contains(consumed_v153_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v153_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v153_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v153_fixture, "\"evidenceDigest\":\"fnv1a64:ca09095f3f3e4b2c\"");
    assert_contains(consumed_v154_fixture, "\"releaseVersion\":\"v154\"");
    assert_contains(consumed_v154_fixture,
                    "\"status\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\"");
    assert_contains(consumed_v154_fixture,
                    "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                    "\"mini-kv-side-candidate-no-runtime\"");
    assert_contains(consumed_v154_fixture, "\"candidateArtifactCount\":4");
    assert_contains(consumed_v154_fixture, "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(consumed_v154_fixture, "\"missingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(consumed_v154_fixture, "\"miniKvLoopbackPortCandidate\":6424");
    assert_contains(consumed_v154_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v154_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v154_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v154_fixture, "\"evidenceDigest\":\"fnv1a64:edbe9c546c0d780a\"");
    assert_contains(consumed_v155_fixture, "\"releaseVersion\":\"v155\"");
    assert_contains(consumed_v155_fixture,
                    "\"status\":\"runtime-execution-approval-gate-input-precheck-blocked-read-only\"");
    assert_contains(consumed_v155_fixture,
                    "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                    "\"blocked-missing-approval-gate-inputs\"");
    assert_contains(consumed_v155_fixture, "\"approvalGateInputCount\":0");
    assert_contains(consumed_v155_fixture, "\"missingApprovalGateInputCount\":3");
    assert_contains(consumed_v155_fixture, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(consumed_v155_fixture, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(consumed_v155_fixture, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(consumed_v155_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v155_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v155_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v155_fixture, "\"evidenceDigest\":\"fnv1a64:edb1c4d8534461ab\"");
    assert_contains(consumed_v156_fixture, "\"releaseVersion\":\"v156\"");
    assert_contains(consumed_v156_fixture, "\"status\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\"");
    assert_contains(consumed_v156_fixture,
                    "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_contains(consumed_v156_fixture, "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_contains(consumed_v156_fixture, "\"miniKvApprovalGateInputComplete\":true");
    assert_contains(consumed_v156_fixture, "\"miniKvLoopbackPort\":6424");
    assert_contains(consumed_v156_fixture, "\"cleanupProofPresent\":false");
    assert_contains(consumed_v156_fixture, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(consumed_v156_fixture, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(consumed_v156_fixture, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(consumed_v156_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v156_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v156_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v156_fixture, "\"evidenceDigest\":\"fnv1a64:f240d1fe1b25fab7\"");
    assert_contains(consumed_v157_fixture, "\"releaseVersion\":\"v157\"");
    assert_contains(consumed_v157_fixture,
                    "\"status\":\"runtime-execution-approval-input-template-validator-echo-read-only\"");
    assert_contains(consumed_v157_fixture,
                    "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                    "\"template-validator-echo-no-canonical-inputs\"");
    assert_contains(consumed_v157_fixture, "\"templateOnlyInputCount\":3");
    assert_contains(consumed_v157_fixture, "\"canonicalRuntimeInputPresent\":false");
    assert_contains(consumed_v157_fixture, "\"templateCopiedToCanonicalInput\":false");
    assert_contains(consumed_v157_fixture, "\"sharedApprovalCorrelationIdPresent\":false");
    assert_contains(consumed_v157_fixture, "\"templatesAuthorizeRuntime\":false");
    assert_contains(consumed_v157_fixture, "\"templateDigestAcceptedAsApproval\":false");
    assert_contains(consumed_v157_fixture, "\"writesCanonicalApprovalInputFiles\":false");
    assert_contains(consumed_v157_fixture, "\"changesNodeInputTemplateFiles\":false");
    assert_contains(consumed_v157_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v157_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v157_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v157_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v157_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v157_fixture, "\"evidenceDigest\":\"fnv1a64:1d8cdf7f597f837e\"");
    assert_contains(consumed_v158_fixture, "\"releaseVersion\":\"v158\"");
    assert_contains(consumed_v158_fixture,
                    "\"status\":\"runtime-execution-canonical-approval-input-precheck-read-only\"");
    assert_contains(consumed_v158_fixture,
                    "\"runtimeExecutionCanonicalApprovalInputPrecheck\":{\"precheckMode\":"
                    "\"blocked-missing-canonical-approval-inputs\"");
    assert_contains(consumed_v158_fixture, "\"requiredCanonicalInputCount\":3");
    assert_contains(consumed_v158_fixture, "\"presentCanonicalInputCount\":0");
    assert_contains(consumed_v158_fixture, "\"missingCanonicalInputCount\":3");
    assert_contains(consumed_v158_fixture, "\"canonicalApprovalInputsComplete\":false");
    assert_contains(consumed_v158_fixture, "\"sharedApprovalCorrelationIdValidated\":false");
    assert_contains(consumed_v158_fixture, "\"templatesAcceptedAsCanonicalInputs\":false");
    assert_contains(consumed_v158_fixture, "\"templateCompatibilityEvidenceAcceptedAsApproval\":false");
    assert_contains(consumed_v158_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v158_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v158_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v158_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v158_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v158_fixture, "\"evidenceDigest\":\"fnv1a64:bce4629123167bc6\"");
    assert_contains(consumed_v159_fixture, "\"releaseVersion\":\"v159\"");
    assert_contains(consumed_v159_fixture, "\"status\":\"node-route-group-split-compatibility-read-only\"");
    assert_contains(consumed_v159_fixture, "\"nodeRouteGroupSplitCompatibility\":{\"compatibilityMode\":"
                                          "\"node-route-group-split-contract-stable\"");
    assert_contains(consumed_v159_fixture,
                    "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\"");
    assert_contains(consumed_v159_fixture, "\"sourceFrozenReleaseVersion\":\"v158\"");
    assert_contains(consumed_v159_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v158.json\"");
    assert_contains(consumed_v159_fixture, "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_contains(consumed_v159_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v159_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v159_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v159_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v159_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v159_fixture, "\"evidenceDigest\":\"fnv1a64:0741c3475ee371e2\"");
    assert_contains(consumed_v160_fixture, "\"releaseVersion\":\"v160\"");
    assert_contains(consumed_v160_fixture, "\"status\":\"boundary-field-catalog-split-read-only\"");
    assert_contains(consumed_v160_fixture, "\"boundaryCatalogMaintenance\":{\"maintenanceMode\":"
                                          "\"boundary-field-catalog-split-contract-preserving\"");
    assert_contains(consumed_v160_fixture, "\"sourceFrozenReleaseVersion\":\"v159\"");
    assert_contains(consumed_v160_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v159.json\"");
    assert_contains(consumed_v160_fixture, "\"boundaryFieldCatalogExtracted\":true");
    assert_contains(consumed_v160_fixture, "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_contains(consumed_v160_fixture, "\"publicShardJsonContractChanged\":false");
    assert_contains(consumed_v160_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v160_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v160_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v160_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v160_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v160_fixture, "\"evidenceDigest\":\"fnv1a64:c96ae9497cfd6386\"");
    assert_contains(consumed_v161_fixture, "\"releaseVersion\":\"v161\"");
    assert_contains(consumed_v161_fixture, "\"status\":\"boundary-field-catalog-index-read-only\"");
    assert_contains(consumed_v161_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v2\"");
    assert_contains(consumed_v161_fixture, "\"sourceFrozenReleaseVersion\":\"v160\"");
    assert_contains(consumed_v161_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v160.json\"");
    assert_contains(consumed_v161_fixture, "\"fieldCount\":290");
    assert_contains(consumed_v161_fixture, "\"groupCount\":15");
    assert_contains(consumed_v161_fixture, "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_contains(consumed_v161_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v161_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v161_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v161_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v161_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v161_fixture, "\"evidenceDigest\":\"fnv1a64:2193962195ba633a\"");
    assert_contains(consumed_v162_fixture, "\"releaseVersion\":\"v162\"");
    assert_contains(consumed_v162_fixture, "\"status\":\"slot-table-preview-read-only\"");
    assert_contains(consumed_v162_fixture, "\"slotTablePreview\":{\"previewMode\":\"single-shard-slot-table-read-only\"");
    assert_contains(consumed_v162_fixture, "\"sourceFrozenReleaseVersion\":\"v161\"");
    assert_contains(consumed_v162_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v161.json\"");
    assert_contains(consumed_v162_fixture, "\"slotTablePreviewOnly\":true");
    assert_contains(consumed_v162_fixture, "\"readOnlyShardMapPreview\":true");
    assert_contains(consumed_v162_fixture, "\"activeRouterInstalled\":false");
    assert_contains(consumed_v162_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v162_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v162_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v3\"");
    assert_contains(consumed_v162_fixture, "\"fieldCount\":303");
    assert_contains(consumed_v162_fixture, "\"groupCount\":16");
    assert_contains(consumed_v162_fixture, "\"evidenceDigest\":\"fnv1a64:8c01a3657f07cfd1\"");
    assert_contains(consumed_v163_fixture, "\"releaseVersion\":\"v163\"");
    assert_contains(consumed_v163_fixture, "\"status\":\"slot-table-preview-audit-read-only\"");
    assert_contains(consumed_v163_fixture, "\"slotTablePreviewAudit\":{\"auditMode\":"
                                           "\"slot-table-preview-consistency-read-only\"");
    assert_contains(consumed_v163_fixture, "\"sourceFrozenReleaseVersion\":\"v162\"");
    assert_contains(consumed_v163_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\"");
    assert_contains(consumed_v163_fixture, "\"observedSlotCount\":16");
    assert_contains(consumed_v163_fixture, "\"contiguousSlotRange\":true");
    assert_contains(consumed_v163_fixture, "\"duplicateSlotsDetected\":false");
    assert_contains(consumed_v163_fixture, "\"unassignedSlotsDetected\":false");
    assert_contains(consumed_v163_fixture, "\"allAssignmentsReadOnly\":true");
    assert_contains(consumed_v163_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v163_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v163_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v163_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v4\"");
    assert_contains(consumed_v163_fixture, "\"fieldCount\":319");
    assert_contains(consumed_v163_fixture, "\"groupCount\":17");
    assert_contains(consumed_v163_fixture, "\"evidenceDigest\":\"fnv1a64:f81c5b47cb396df6\"");
    assert_contains(consumed_v164_fixture, "\"releaseVersion\":\"v164\"");
    assert_contains(consumed_v164_fixture, "\"status\":\"slot-table-preview-audit-maintenance-read-only\"");
    assert_contains(consumed_v164_fixture, "\"slotTablePreviewAuditMaintenance\":{\"maintenanceMode\":"
                                           "\"slot-table-preview-audit-formatter-split-contract-preserving\"");
    assert_contains(consumed_v164_fixture, "\"sourceFrozenReleaseVersion\":\"v163\"");
    assert_contains(consumed_v164_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v163.json\"");
    assert_contains(consumed_v164_fixture, "\"auditFormatterExtracted\":true");
    assert_contains(consumed_v164_fixture, "\"slotPreviewModuleOwnsAssignments\":true");
    assert_contains(consumed_v164_fixture, "\"slotPreviewAuditStillPublished\":true");
    assert_contains(consumed_v164_fixture, "\"slotTablePreviewStillPublished\":true");
    assert_contains(consumed_v164_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v164_fixture, "\"startsJavaService\":false");
    assert_contains(consumed_v164_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v164_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v164_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v164_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v164_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v5\"");
    assert_contains(consumed_v164_fixture, "\"fieldCount\":337");
    assert_contains(consumed_v164_fixture, "\"groupCount\":18");
    assert_contains(consumed_v164_fixture, "\"evidenceDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_contains(consumed_v165_fixture, "\"releaseVersion\":\"v165\"");
    assert_contains(consumed_v165_fixture, "\"status\":\"shard-readiness-release-catalog-read-only\"");
    assert_contains(consumed_v165_fixture, "\"shardReadinessReleaseCatalog\":{\"catalogMode\":"
                                           "\"versioned-shard-readiness-release-catalog-read-only\"");
    assert_contains(consumed_v165_fixture, "\"sourceFrozenReleaseVersion\":\"v164\"");
    assert_contains(consumed_v165_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v164.json\"");
    assert_contains(consumed_v165_fixture, "\"catalogedReleaseCount\":4");
    assert_contains(consumed_v165_fixture, "\"latestCatalogedReleaseVersion\":\"v164\"");
    assert_contains(consumed_v165_fixture, "\"latestCatalogedDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_contains(consumed_v165_fixture, "\"primarySection\":\"slotTablePreviewAuditMaintenance\"");
    assert_contains(consumed_v165_fixture, "\"versionedFixtureCatalogOnly\":true");
    assert_contains(consumed_v165_fixture, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_contains(consumed_v165_fixture, "\"activeRouterInstalled\":false");
    assert_contains(consumed_v165_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v165_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v165_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v6\"");
    assert_contains(consumed_v165_fixture, "\"fieldCount\":357");
    assert_contains(consumed_v165_fixture, "\"groupCount\":19");
    assert_contains(consumed_v165_fixture, "\"evidenceDigest\":\"fnv1a64:5e94e2a9b438d2ad\"");
    assert_contains(consumed_v166_fixture, "\"releaseVersion\":\"v166\"");
    assert_contains(consumed_v166_fixture, "\"status\":\"shard-readiness-release-catalog-audit-read-only\"");
    assert_contains(consumed_v166_fixture, "\"shardReadinessReleaseCatalogAudit\":{\"auditMode\":"
                                           "\"shard-readiness-release-catalog-consistency-read-only\"");
    assert_contains(consumed_v166_fixture, "\"sourceFrozenReleaseVersion\":\"v165\"");
    assert_contains(consumed_v166_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v165.json\"");
    assert_contains(consumed_v166_fixture, "\"expectedReleaseCount\":4");
    assert_contains(consumed_v166_fixture, "\"observedReleaseCount\":4");
    assert_contains(consumed_v166_fixture, "\"contiguousReleaseRange\":true");
    assert_contains(consumed_v166_fixture, "\"duplicateReleasesDetected\":false");
    assert_contains(consumed_v166_fixture, "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(consumed_v166_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v166_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v166_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v166_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v166_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v166_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v7\"");
    assert_contains(consumed_v166_fixture, "\"fieldCount\":381");
    assert_contains(consumed_v166_fixture, "\"groupCount\":20");
    assert_contains(consumed_v166_fixture, "\"evidenceDigest\":\"fnv1a64:4bd18a01790dc5f1\"");
    assert_contains(consumed_v167_fixture, "\"releaseVersion\":\"v167\"");
    assert_contains(consumed_v167_fixture, "\"status\":\"node-route-split-window-compatibility-read-only\"");
    assert_contains(consumed_v167_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v440-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v167_fixture, "\"sourceFrozenReleaseVersion\":\"v166\"");
    assert_contains(consumed_v167_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v166.json\"");
    assert_contains(consumed_v167_fixture, "\"windowEndNodeVersion\":\"Node v440\"");
    assert_contains(consumed_v167_fixture, "\"splitRouteGroupCount\":8");
    assert_contains(consumed_v167_fixture, "\"nodeAddsEvidenceGate\":false");
    assert_contains(consumed_v167_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v167_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v167_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v167_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v167_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v167_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v8\"");
    assert_contains(consumed_v167_fixture, "\"fieldCount\":401");
    assert_contains(consumed_v167_fixture, "\"groupCount\":21");
    assert_contains(consumed_v167_fixture, "\"evidenceDigest\":\"fnv1a64:caf300915d4e988c\"");
    assert_contains(consumed_v168_fixture, "\"releaseVersion\":\"v168\"");
    assert_contains(consumed_v168_fixture, "\"status\":\"node-route-split-window-preparation-plan-read-only\"");
    assert_contains(consumed_v168_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v441-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v168_fixture, "\"sourceFrozenReleaseVersion\":\"v167\"");
    assert_contains(consumed_v168_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v167.json\"");
    assert_contains(consumed_v168_fixture, "\"windowEndNodeVersion\":\"Node v441\"");
    assert_contains(consumed_v168_fixture, "\"splitRouteGroupCount\":9");
    assert_contains(consumed_v168_fixture, "\"nodeV441DesignDraftBodyPreparationPlanRouteSplitPreserved\":true");
    assert_contains(consumed_v168_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v168_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v168_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v168_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v168_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v168_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v8\"");
    assert_contains(consumed_v168_fixture, "\"fieldCount\":401");
    assert_contains(consumed_v168_fixture, "\"groupCount\":21");
    assert_contains(consumed_v168_fixture, "\"evidenceDigest\":\"fnv1a64:d0d0ef2b2a0f5a0a\"");
    assert_contains(consumed_v169_fixture, "\"releaseVersion\":\"v169\"");
    assert_contains(consumed_v169_fixture, "\"status\":\"node-route-split-window-audit-read-only\"");
    assert_contains(consumed_v169_fixture, "\"nodeRouteSplitCompatibilityWindowAudit\":{\"auditMode\":"
                                           "\"node-route-split-compatibility-window-consistency-read-only\"");
    assert_contains(consumed_v169_fixture, "\"sourceFrozenReleaseVersion\":\"v168\"");
    assert_contains(consumed_v169_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v168.json\"");
    assert_contains(consumed_v169_fixture, "\"expectedWindowVersionCount\":9");
    assert_contains(consumed_v169_fixture, "\"observedWindowVersionCount\":9");
    assert_contains(consumed_v169_fixture, "\"windowRangeEnd\":\"Node v441\"");
    assert_contains(consumed_v169_fixture, "\"sourceFrozenWindowDigest\":\"fnv1a64:d0d0ef2b2a0f5a0a\"");
    assert_contains(consumed_v169_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v169_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v169_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v169_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v169_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v169_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v169_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v169_fixture, "\"groupCount\":22");
    assert_contains(consumed_v169_fixture, "\"evidenceDigest\":\"fnv1a64:eaa587d1a50d8200\"");
    assert_contains(consumed_v170_fixture, "\"releaseVersion\":\"v170\"");
    assert_contains(consumed_v170_fixture, "\"status\":\"node-route-split-window-draft-candidate-read-only\"");
    assert_contains(consumed_v170_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v442-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v170_fixture, "\"sourceFrozenReleaseVersion\":\"v169\"");
    assert_contains(consumed_v170_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v169.json\"");
    assert_contains(consumed_v170_fixture, "\"windowEndNodeVersion\":\"Node v442\"");
    assert_contains(consumed_v170_fixture, "\"splitRouteGroupCount\":10");
    assert_contains(consumed_v170_fixture, "\"observedWindowVersionCount\":10");
    assert_contains(consumed_v170_fixture, "\"nodeV442DesignDraftBodyDraftCandidateRouteSplitPreserved\":true");
    assert_contains(consumed_v170_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v170_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v170_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v170_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v170_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v170_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v170_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v170_fixture, "\"groupCount\":22");
    assert_contains(consumed_v170_fixture, "\"evidenceDigest\":\"fnv1a64:d06aaa033f5c1d86\"");
    assert_contains(consumed_v171_fixture, "\"releaseVersion\":\"v171\"");
    assert_contains(consumed_v171_fixture, "\"status\":\"node-route-split-window-managed-audit-read-only\"");
    assert_contains(consumed_v171_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v444-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v171_fixture, "\"sourceFrozenReleaseVersion\":\"v170\"");
    assert_contains(consumed_v171_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v170.json\"");
    assert_contains(consumed_v171_fixture, "\"windowEndNodeVersion\":\"Node v444\"");
    assert_contains(consumed_v171_fixture, "\"splitRouteGroupCount\":12");
    assert_contains(consumed_v171_fixture, "\"observedWindowVersionCount\":12");
    assert_contains(consumed_v171_fixture, "\"nodeV443ManagedAuditAdapterRouteSplitPreserved\":true");
    assert_contains(consumed_v171_fixture, "\"nodeV444ManagedAuditPersistenceRouteSplitPreserved\":true");
    assert_contains(consumed_v171_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v171_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v171_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v171_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v171_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v171_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v171_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v171_fixture, "\"groupCount\":22");
    assert_contains(consumed_v171_fixture, "\"evidenceDigest\":\"fnv1a64:9f6aa583ef521d1a\"");
    assert_contains(consumed_v172_fixture, "\"releaseVersion\":\"v172\"");
    assert_contains(consumed_v172_fixture, "\"status\":\"node-route-split-window-restore-drill-read-only\"");
    assert_contains(consumed_v172_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v446-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v172_fixture, "\"sourceFrozenReleaseVersion\":\"v171\"");
    assert_contains(consumed_v172_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v171.json\"");
    assert_contains(consumed_v172_fixture, "\"windowEndNodeVersion\":\"Node v446\"");
    assert_contains(consumed_v172_fixture, "\"splitRouteGroupCount\":14");
    assert_contains(consumed_v172_fixture, "\"observedWindowVersionCount\":14");
    assert_contains(consumed_v172_fixture, "\"nodeV445ManagedAuditIdentityApprovalRouteSplitPreserved\":true");
    assert_contains(consumed_v172_fixture, "\"nodeV446ManagedAuditRestoreDrillRouteSplitPreserved\":true");
    assert_contains(consumed_v172_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v172_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v172_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v172_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v172_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v172_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v172_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v172_fixture, "\"groupCount\":22");
    assert_contains(consumed_v172_fixture, "\"evidenceDigest\":\"fnv1a64:85f0acb5a011256f\"");
    assert_contains(consumed_v173_fixture, "\"releaseVersion\":\"v173\"");
    assert_contains(consumed_v173_fixture, "\"status\":\"node-route-split-window-dry-run-adapter-read-only\"");
    assert_contains(consumed_v173_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v447-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v173_fixture, "\"sourceFrozenReleaseVersion\":\"v172\"");
    assert_contains(consumed_v173_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v172.json\"");
    assert_contains(consumed_v173_fixture, "\"windowEndNodeVersion\":\"Node v447\"");
    assert_contains(consumed_v173_fixture, "\"splitRouteGroupCount\":15");
    assert_contains(consumed_v173_fixture, "\"observedWindowVersionCount\":15");
    assert_contains(consumed_v173_fixture, "\"nodeV447ManagedAuditDryRunAdapterRouteSplitPreserved\":true");
    assert_contains(consumed_v173_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v173_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v173_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v173_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v173_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v173_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v173_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v173_fixture, "\"groupCount\":22");
    assert_contains(consumed_v173_fixture, "\"evidenceDigest\":\"fnv1a64:33a60bc1d66fd286\"");
    assert_contains(consumed_v174_fixture, "\"releaseVersion\":\"v174\"");
    assert_contains(consumed_v174_fixture, "\"status\":\"node-route-split-window-route-quality-read-only\"");
    assert_contains(consumed_v174_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v448-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v174_fixture, "\"sourceFrozenReleaseVersion\":\"v173\"");
    assert_contains(consumed_v174_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v173.json\"");
    assert_contains(consumed_v174_fixture, "\"windowEndNodeVersion\":\"Node v448\"");
    assert_contains(consumed_v174_fixture, "\"splitRouteGroupCount\":16");
    assert_contains(consumed_v174_fixture, "\"observedWindowVersionCount\":16");
    assert_contains(consumed_v174_fixture, "\"nodeV448ManagedAuditRouteQualityRouteSplitPreserved\":true");
    assert_contains(consumed_v174_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v174_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v174_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v174_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v174_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v174_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v174_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v174_fixture, "\"groupCount\":22");
    assert_contains(consumed_v174_fixture, "\"evidenceDigest\":\"fnv1a64:d582c3d04aed99e1\"");
    assert_contains(consumed_v175_fixture, "\"releaseVersion\":\"v175\"");
    assert_contains(consumed_v175_fixture, "\"status\":\"node-route-split-window-adapter-implementation-read-only\"");
    assert_contains(consumed_v175_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v449-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v175_fixture, "\"sourceFrozenReleaseVersion\":\"v174\"");
    assert_contains(consumed_v175_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v174.json\"");
    assert_contains(consumed_v175_fixture, "\"windowEndNodeVersion\":\"Node v449\"");
    assert_contains(consumed_v175_fixture, "\"splitRouteGroupCount\":17");
    assert_contains(consumed_v175_fixture, "\"observedWindowVersionCount\":17");
    assert_contains(consumed_v175_fixture, "\"nodeV449ManagedAuditAdapterImplementationRouteSplitPreserved\":true");
    assert_contains(consumed_v175_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v175_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v175_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v175_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v175_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v175_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v175_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v175_fixture, "\"groupCount\":22");
    assert_contains(consumed_v175_fixture, "\"evidenceDigest\":\"fnv1a64:61e561e1c8ba7d50\"");
    assert_contains(consumed_v176_fixture, "\"releaseVersion\":\"v176\"");
    assert_contains(consumed_v176_fixture,
                    "\"status\":\"node-route-split-window-manual-sandbox-fake-transport-read-only\"");
    assert_contains(consumed_v176_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v457-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v176_fixture, "\"sourceFrozenReleaseVersion\":\"v175\"");
    assert_contains(consumed_v176_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v175.json\"");
    assert_contains(consumed_v176_fixture, "\"windowEndNodeVersion\":\"Node v457\"");
    assert_contains(consumed_v176_fixture, "\"splitRouteGroupCount\":25");
    assert_contains(consumed_v176_fixture, "\"observedWindowVersionCount\":25");
    assert_contains(consumed_v176_fixture, "\"nodeV457ManualSandboxConnectionFakeTransportRouteSplitPreserved\":true");
    assert_contains(consumed_v176_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v176_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v176_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v176_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v176_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v176_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v176_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v176_fixture, "\"groupCount\":22");
    assert_contains(consumed_v176_fixture, "\"evidenceDigest\":\"fnv1a64:528073c4315ef89b\"");
    assert_contains(consumed_v177_fixture, "\"releaseVersion\":\"v177\"");
    assert_contains(consumed_v177_fixture, "\"status\":\"node-route-split-window-foundational-audit-read-only\"");
    assert_contains(consumed_v177_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v458-route-split-window-contract-stable-read-only\"");
    assert_contains(consumed_v177_fixture, "\"sourceFrozenReleaseVersion\":\"v176\"");
    assert_contains(consumed_v177_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v176.json\"");
    assert_contains(consumed_v177_fixture, "\"windowEndNodeVersion\":\"Node v458\"");
    assert_contains(consumed_v177_fixture, "\"splitRouteGroupCount\":26");
    assert_contains(consumed_v177_fixture, "\"observedWindowVersionCount\":26");
    assert_contains(consumed_v177_fixture, "\"nodeV458FoundationalAuditRouteSplitPreserved\":true");
    assert_contains(consumed_v177_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v177_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v177_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v177_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v177_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v177_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v177_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v177_fixture, "\"groupCount\":22");
    assert_contains(consumed_v177_fixture, "\"evidenceDigest\":\"fnv1a64:e773708a4decc60e\"");
    assert_contains(consumed_v178_fixture, "\"releaseVersion\":\"v178\"");
    assert_contains(consumed_v178_fixture, "\"status\":\"node-route-split-window-computed-audit-read-only\"");
    assert_contains(consumed_v178_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v458-route-split-window-computed-audit-read-only\"");
    assert_contains(consumed_v178_fixture, "\"sourceFrozenReleaseVersion\":\"v177\"");
    assert_contains(consumed_v178_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v177.json\"");
    assert_contains(consumed_v178_fixture, "\"windowEndNodeVersion\":\"Node v458\"");
    assert_contains(consumed_v178_fixture, "\"splitRouteGroupCount\":26");
    assert_contains(consumed_v178_fixture, "\"observedWindowVersionCount\":26");
    assert_contains(consumed_v178_fixture, "\"contiguousNodeVersionWindow\":true");
    assert_contains(consumed_v178_fixture, "\"duplicateWindowVersionsDetected\":false");
    assert_contains(consumed_v178_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v178_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v178_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v178_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v178_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v178_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(consumed_v178_fixture, "\"fieldCount\":420");
    assert_contains(consumed_v178_fixture, "\"groupCount\":22");
    assert_contains(consumed_v178_fixture, "\"evidenceDigest\":\"fnv1a64:dd89f7c3bd63d7c3\"");
    assert_contains(consumed_v179_fixture, "\"releaseVersion\":\"v179\"");
    assert_contains(consumed_v179_fixture, "\"status\":\"node-route-split-window-numeric-span-audit-read-only\"");
    assert_contains(consumed_v179_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v458-route-split-window-numeric-span-audit-read-only\"");
    assert_contains(consumed_v179_fixture, "\"sourceFrozenReleaseVersion\":\"v178\"");
    assert_contains(consumed_v179_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v178.json\"");
    assert_contains(consumed_v179_fixture, "\"windowRangeStartNumber\":433");
    assert_contains(consumed_v179_fixture, "\"windowRangeEndNumber\":458");
    assert_contains(consumed_v179_fixture, "\"computedWindowVersionSpan\":26");
    assert_contains(consumed_v179_fixture, "\"windowRangeNumbersParseable\":true");
    assert_contains(consumed_v179_fixture, "\"windowCountMatchesRange\":true");
    assert_contains(consumed_v179_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v179_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v179_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v179_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v179_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v179_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v10\"");
    assert_contains(consumed_v179_fixture, "\"fieldCount\":422");
    assert_contains(consumed_v179_fixture, "\"groupCount\":22");
    assert_contains(consumed_v179_fixture, "\"evidenceDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_contains(consumed_v180_fixture, "\"releaseVersion\":\"v180\"");
    assert_contains(consumed_v180_fixture, "\"status\":\"shard-readiness-history-formatter-maintenance-read-only\"");
    assert_contains(consumed_v180_fixture, "\"shardReadinessHistoryMaintenance\":{\"maintenanceMode\":"
                                           "\"history-fixture-archive-formatter-split-read-only\"");
    assert_contains(consumed_v180_fixture, "\"sourceFrozenReleaseVersion\":\"v179\"");
    assert_contains(consumed_v180_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v179.json\"");
    assert_contains(consumed_v180_fixture, "\"sourceFrozenDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_contains(consumed_v180_fixture, "\"preservesFixtureParity\":true");
    assert_contains(consumed_v180_fixture, "\"preservesArchiveCompatibility\":true");
    assert_contains(consumed_v180_fixture, "\"preservesHistoricalFallback\":true");
    assert_contains(consumed_v180_fixture, "\"changesShardJsonCommand\":false");
    assert_contains(consumed_v180_fixture, "\"changesFixturePath\":false");
    assert_contains(consumed_v180_fixture, "\"startsServices\":false");
    assert_contains(consumed_v180_fixture, "\"runtimeProbeAllowed\":false");
    assert_contains(consumed_v180_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v180_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v180_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v11\"");
    assert_contains(consumed_v180_fixture, "\"fieldCount\":431");
    assert_contains(consumed_v180_fixture, "\"groupCount\":23");
    assert_contains(consumed_v180_fixture, "\"evidenceDigest\":\"fnv1a64:3403c490a3e623e0\"");
    assert_contains(consumed_v181_fixture, "\"releaseVersion\":\"v181\"");
    assert_contains(consumed_v181_fixture, "\"status\":\"node-route-catalog-integrity-window-read-only\"");
    assert_contains(consumed_v181_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v460-route-catalog-integrity-window-read-only\"");
    assert_contains(consumed_v181_fixture, "\"sourceFrozenReleaseVersion\":\"v180\"");
    assert_contains(consumed_v181_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v180.json\"");
    assert_contains(consumed_v181_fixture, "\"windowEndNodeVersion\":\"Node v460\"");
    assert_contains(consumed_v181_fixture, "\"splitRouteGroupCount\":28");
    assert_contains(consumed_v181_fixture, "\"computedWindowVersionSpan\":28");
    assert_contains(consumed_v181_fixture, "\"sourceFrozenWindowDigest\":\"fnv1a64:3403c490a3e623e0\"");
    assert_contains(consumed_v181_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v181_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v181_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v181_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v181_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v181_fixture, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v11\"");
    assert_contains(consumed_v181_fixture, "\"fieldCount\":431");
    assert_contains(consumed_v181_fixture, "\"groupCount\":23");
    assert_contains(consumed_v181_fixture, "\"evidenceDigest\":\"fnv1a64:58e9b51084ad91e5\"");
    assert_contains(consumed_v182_fixture, "\"releaseVersion\":\"v182\"");
    assert_contains(consumed_v182_fixture, "\"status\":\"node-route-catalog-test-migration-window-read-only\"");
    assert_contains(consumed_v182_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v462-route-catalog-test-migration-window-read-only\"");
    assert_contains(consumed_v182_fixture, "\"sourceFrozenReleaseVersion\":\"v181\"");
    assert_contains(consumed_v182_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v181.json\"");
    assert_contains(consumed_v182_fixture, "\"windowEndNodeVersion\":\"Node v462\"");
    assert_contains(consumed_v182_fixture, "\"splitRouteGroupCount\":30");
    assert_contains(consumed_v182_fixture, "\"computedWindowVersionSpan\":30");
    assert_contains(consumed_v182_fixture, "\"sourceFrozenWindowDigest\":\"fnv1a64:58e9b51084ad91e5\"");
    assert_contains(consumed_v182_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v182_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v182_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v182_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v182_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v182_fixture, "\"evidenceDigest\":\"fnv1a64:e4a40fd2e1d1d741\"");
    assert_contains(consumed_v183_fixture, "\"releaseVersion\":\"v183\"");
    assert_contains(consumed_v183_fixture, "\"status\":\"node-route-quality-catalog-integrity-window-read-only\"");
    assert_contains(consumed_v183_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v463-route-quality-catalog-integrity-window-read-only\"");
    assert_contains(consumed_v183_fixture, "\"sourceFrozenReleaseVersion\":\"v182\"");
    assert_contains(consumed_v183_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v182.json\"");
    assert_contains(consumed_v183_fixture, "\"windowEndNodeVersion\":\"Node v463\"");
    assert_contains(consumed_v183_fixture, "\"splitRouteGroupCount\":31");
    assert_contains(consumed_v183_fixture, "\"computedWindowVersionSpan\":31");
    assert_contains(consumed_v183_fixture, "\"sourceFrozenWindowDigest\":\"fnv1a64:e4a40fd2e1d1d741\"");
    assert_contains(consumed_v183_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v183_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v183_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v183_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v183_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v183_fixture, "\"evidenceDigest\":\"fnv1a64:8b252a90cb150a99\"");
    assert_contains(consumed_v184_fixture, "\"releaseVersion\":\"v184\"");
    assert_contains(consumed_v184_fixture, "\"status\":\"node-route-catalog-final-closeout-window-read-only\"");
    assert_contains(consumed_v184_fixture, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v464-route-catalog-final-closeout-window-read-only\"");
    assert_contains(consumed_v184_fixture, "\"sourceFrozenReleaseVersion\":\"v183\"");
    assert_contains(consumed_v184_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v183.json\"");
    assert_contains(consumed_v184_fixture, "\"windowEndNodeVersion\":\"Node v464\"");
    assert_contains(consumed_v184_fixture, "\"splitRouteGroupCount\":32");
    assert_contains(consumed_v184_fixture, "\"computedWindowVersionSpan\":32");
    assert_contains(consumed_v184_fixture, "\"sourceFrozenWindowDigest\":\"fnv1a64:8b252a90cb150a99\"");
    assert_contains(consumed_v184_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v184_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v184_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v184_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v184_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v184_fixture, "\"evidenceDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_contains(consumed_v185_fixture, "\"releaseVersion\":\"v185\"");
    assert_contains(consumed_v185_fixture, "\"status\":\"node-route-catalog-closeout-snapshot-read-only\"");
    assert_contains(consumed_v185_fixture, "\"nodeRouteCatalogCloseoutSnapshot\":{\"snapshotMode\":"
                                           "\"node-route-catalog-final-closeout-snapshot-read-only\"");
    assert_contains(consumed_v185_fixture, "\"sourceFrozenReleaseVersion\":\"v184\"");
    assert_contains(consumed_v185_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v184.json\"");
    assert_contains(consumed_v185_fixture, "\"sourceFrozenDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_contains(consumed_v185_fixture, "\"focusedCloseoutTestFileCount\":7");
    assert_contains(consumed_v185_fixture, "\"focusedCloseoutTestCount\":17");
    assert_contains(consumed_v185_fixture, "\"nodeFullVitestFileCount\":393");
    assert_contains(consumed_v185_fixture, "\"nodeFullVitestTestCount\":1221");
    assert_contains(consumed_v185_fixture, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(consumed_v185_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v185_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v185_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v185_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v185_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v185_fixture, "\"evidenceDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_contains(consumed_v186_fixture, "\"releaseVersion\":\"v186\"");
    assert_contains(consumed_v186_fixture, "\"status\":\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(consumed_v186_fixture, "\"nodeRouteCatalogAnchorRemovalAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(consumed_v186_fixture, "\"sourceFrozenReleaseVersion\":\"v185\"");
    assert_contains(consumed_v186_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v185.json\"");
    assert_contains(consumed_v186_fixture, "\"sourceFrozenDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_contains(consumed_v186_fixture, "\"windowEndNodeVersion\":\"Node v467\"");
    assert_contains(consumed_v186_fixture, "\"splitRouteGroupCount\":35");
    assert_contains(consumed_v186_fixture, "\"computedWindowVersionSpan\":35");
    assert_contains(consumed_v186_fixture, "\"sourceAnchorCompatibilityRemoved\":true");
    assert_contains(consumed_v186_fixture, "\"miniKvRouteAnchorDependencyIntroduced\":false");
    assert_contains(consumed_v186_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v186_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v186_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v186_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v186_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v186_fixture, "\"evidenceDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_contains(consumed_v187_fixture, "\"releaseVersion\":\"v187\"");
    assert_contains(consumed_v187_fixture,
                    "\"status\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(consumed_v187_fixture, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(consumed_v187_fixture, "\"sourceFrozenReleaseVersion\":\"v186\"");
    assert_contains(consumed_v187_fixture, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v186.json\"");
    assert_contains(consumed_v187_fixture, "\"sourceFrozenDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_contains(consumed_v187_fixture, "\"windowEndNodeVersion\":\"Node v471\"");
    assert_contains(consumed_v187_fixture, "\"splitRouteGroupCount\":39");
    assert_contains(consumed_v187_fixture, "\"computedWindowVersionSpan\":39");
    assert_contains(consumed_v187_fixture, "\"expectedIntegritySnapshotFactoryMovedByNodeV471\":true");
    assert_contains(consumed_v187_fixture, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(consumed_v187_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v187_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v187_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v187_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v187_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v187_fixture, "\"evidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(consumed_v188_fixture, "\"releaseVersion\":\"v188\"");
    assert_contains(consumed_v188_fixture,
                    "\"status\":\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(consumed_v188_fixture, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":{\"freezeMode\":"
                                           "\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(consumed_v188_fixture, "\"frozenReleaseVersion\":\"v187\"");
    assert_contains(consumed_v188_fixture, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v187.json\"");
    assert_contains(consumed_v188_fixture, "\"frozenEvidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(consumed_v188_fixture, "\"sourceFrozenWindowDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(consumed_v188_fixture, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(consumed_v188_fixture, "\"preservesExpectedIntegritySnapshotAudit\":true");
    assert_contains(consumed_v188_fixture, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(consumed_v188_fixture, "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(consumed_v188_fixture, "\"startsMiniKvService\":false");
    assert_contains(consumed_v188_fixture, "\"routerActivationAllowed\":false");
    assert_contains(consumed_v188_fixture, "\"writeRoutingAllowed\":false");
    assert_contains(consumed_v188_fixture, "\"executionAllowed\":false");
    assert_contains(consumed_v188_fixture, "\"evidenceDigest\":\"fnv1a64:565e5ce1fdb5f308\"");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDJSON extra");
    assert(result.response == "ERR usage: SHARDJSON");

    result = processor.execute("SHARDJSON");
    assert(result.response == fixture);
    assert_shard_readiness_contract(result.response);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    assert_contains(result.response, "SHARDJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    assert_contains(result.response, "\"name\":\"SHARDJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");

    result = processor.execute("EXPLAINJSON SHARDJSON");
    assert_contains(result.response, "\"command\":\"SHARDJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDJSON");
    assert_contains(result.response, "\"command\":\"SHARDJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"warnings\":[\"not a write command\"]");

    result = processor.execute("CHECKJSON SHARDJSON extra");
    assert_contains(result.response, "\"allowed_by_parser\":false");
    assert_contains(result.response, "\"usage: SHARDJSON\"");
}
