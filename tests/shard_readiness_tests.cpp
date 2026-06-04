#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"
#include "test_support.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

namespace {

using minikv::test_support::assert_contains;
using minikv::test_support::read_fixture_text;

std::map<int, std::string> read_shard_readiness_fixtures(int first_release, int last_release) {
    assert(first_release <= last_release);

    std::map<int, std::string> fixtures;
    for (int release_version = first_release; release_version <= last_release; ++release_version) {
        fixtures.emplace(release_version, minikv::test_support::read_shard_readiness_fixture(release_version));
    }
    return fixtures;
}

void assert_fixture_differs_from_each(const std::string& fixture,
                                      const std::map<int, std::string>& historical_fixtures) {
    for (const auto& [release_version, historical_fixture] : historical_fixtures) {
        if (fixture == historical_fixture) {
            std::cerr << "current fixture unexpectedly matched v" << release_version << '\n';
        }
        assert(fixture != historical_fixture);
    }
}

void assert_shard_readiness_contract(const std::string& json) {
    assert_contains(json, "\"contract\":\"shard-readiness.v1\"");
    assert_contains(json, "\"evidenceType\":\"shard_readiness\"");
    assert_contains(json, "\"project\":\"mini-kv\"");
    assert_contains(json, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_contains(json, "\"version\":\"" + std::string{minikv::version} + "\",\"releaseVersion\":\"v363\"");
    assert_contains(json, "\"readOnly\":true");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardEnabled\":false");
    assert_contains(json, "\"shardCount\":1");
    assert_contains(json, "\"slotCount\":16");
    assert_contains(json, "\"routingMode\":\"single-shard-readiness-prototype\"");
    assert_contains(json, "\"evidencePath\":\"fixtures/release/shard-readiness.json\"");
    assert_contains(json, "\"status\":\"node-route-catalog-cleanup-post-closeout-continuity-read-only\"");
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
    assert_contains(json, "\"shardRoutePreview\":{\"rolloutMode\":\"read-only-shard-route-preview-rollout\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v297\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v297.json\"");
    assert_contains(json, "\"rolloutStage\":\"route-preview-release-package-audit\"");
    assert_contains(json, "\"rolloutStageSequence\":20");
    assert_contains(json, "\"rolloutReleaseVersion\":\"v298\"");
    assert_contains(json, "\"publishedStageCount\":30");
    assert_contains(json, "\"commands\":[\"SHARDROUTE\",\"SHARDROUTEJSON\"]");
    assert_contains(json, "\"sampleSlotsPinnedToExistingEvidence\":true");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewVerification\":{\"rolloutMode\":"
                          "\"read-only-shard-route-preview-verification-rollout\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v318\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v318.json\"");
    assert_contains(json, "\"rolloutStage\":\"route-preview-verification-closeout-audit\"");
    assert_contains(json, "\"rolloutStageSequence\":21");
    assert_contains(json, "\"rolloutReleaseVersion\":\"v319\"");
    assert_contains(json, "\"publishedStageCount\":31");
    assert_contains(json, "\"verificationCommandAvailable\":true");
    assert_contains(json, "\"verifiedCommand\":\"SHARDROUTEJSON\"");
    assert_contains(json, "\"commands\":[\"SHARDROUTEVERIFYJSON\"]");
    assert_contains(json, "\"executesRoute\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewVerificationReport\":{\"rolloutMode\":"
                          "\"read-only-shard-route-preview-verification-report-rollout\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v338\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v338.json\"");
    assert_contains(json, "\"rolloutStage\":\"route-preview-verification-report-closeout-audit\"");
    assert_contains(json, "\"rolloutStageSequence\":20");
    assert_contains(json, "\"rolloutReleaseVersion\":\"v339\"");
    assert_contains(json, "\"publishedStageCount\":30");
    assert_contains(json, "\"reportCommandAvailable\":true");
    assert_contains(json, "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(json, "\"previewCommand\":\"SHARDROUTEJSON\"");
    assert_contains(json, "\"commands\":[\"SHARDROUTEVERIFYREPORTJSON\"]");
    assert_contains(json, "\"executesRoute\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewVerificationReportCloseout\":{\"contract\":"
                          "\"shard-route-preview-verification-report-closeout.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_contains(json, "\"closeoutMode\":\"read-only-route-preview-verification-report-closeout-summary\"");
    assert_contains(json, "\"closedReleaseRangeStart\":\"v320\"");
    assert_contains(json, "\"closedReleaseRangeEnd\":\"v339\"");
    assert_contains(json, "\"closedVersionCount\":20");
    assert_contains(json, "\"reportStageCount\":20");
    assert_contains(json, "\"allReportStagesPublished\":true");
    assert_contains(json, "\"archiveManifestPlannedReleaseVersion\":\"v341\"");
    assert_contains(json, "\"archiveVerificationPlannedReleaseVersion\":\"v342\"");
    assert_contains(json, "\"executesRoute\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewVerificationReportArchive\":{\"contract\":"
                          "\"shard-route-preview-verification-report-archive.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(json, "\"archiveMode\":\"read-only-route-preview-verification-report-archive-chain\"");
    assert_contains(json, "\"sourceCloseoutReleaseVersion\":\"v340\"");
    assert_contains(json, "\"archiveStage\":\"route-preview-verification-report-archive-closeout-audit\"");
    assert_contains(json, "\"archiveStageSequence\":20");
    assert_contains(json, "\"archiveReleaseVersion\":\"v360\"");
    assert_contains(json, "\"publishedStageCount\":30");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v359\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v359.json\"");
    assert_contains(json, "\"archiveManifestPublished\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewVerificationReportArchiveCloseout\":{\"contract\":"
                          "\"shard-route-preview-verification-report-archive-closeout.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(json, "\"closeoutMode\":\"read-only-route-preview-verification-report-archive-closeout-chain\"");
    assert_contains(json, "\"sourceArchiveCommand\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(json, "\"sourceArchiveReleaseVersion\":\"v360\"");
    assert_contains(json, "\"sourceArchiveFixturePath\":\"fixtures/release/shard-readiness-v360.json\"");
    assert_contains(json, "\"sourceArchivePublishedStageCount\":20");
    assert_contains(json, "\"sourceArchiveChainComplete\":true");
    assert_contains(json, "\"closeoutStage\":\"route-preview-verification-report-archive-closeout-explain-contract\"");
    assert_contains(json, "\"closeoutStageSequence\":3");
    assert_contains(json, "\"closeoutReleaseVersion\":\"v363\"");
    assert_contains(json, "\"publishedStageCount\":3");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v362\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v362.json\"");
    assert_contains(json, "\"archivedArchiveReleaseRangeStart\":\"v341\"");
    assert_contains(json, "\"archivedArchiveReleaseRangeEnd\":\"v360\"");
    assert_contains(json, "\"archivedArchiveReleaseCount\":20");
    assert_contains(json, "\"archiveCloseoutCommandAvailable\":true");
    assert_contains(json, "\"archiveCloseoutChainComplete\":false");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
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
        "\"nodeConsumer\":\"Node v550+ may consume v363 as read-only shard route preview verification report archive closeout explain-contract evidence only\"");
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
                          "\"fixtures/release/shard-readiness-v200.json\",\"fixtures/release/shard-readiness-v201.json\",\"fixtures/release/shard-readiness-v202.json\",\"fixtures/release/shard-readiness-v203.json\",\"fixtures/release/shard-readiness-v204.json\",\"fixtures/release/shard-readiness-v205.json\",\"fixtures/release/shard-readiness-v206.json\",\"fixtures/release/shard-readiness-v207.json\",\"fixtures/release/shard-readiness-v208.json\",\"fixtures/release/shard-readiness-v209.json\",\"fixtures/release/shard-readiness-v210.json\",\"fixtures/release/shard-readiness-v211.json\",\"fixtures/release/shard-readiness-v212.json\",\"fixtures/release/shard-readiness-v213.json\",\"fixtures/release/shard-readiness-v214.json\",\"fixtures/release/shard-readiness-v215.json\",\"fixtures/release/shard-readiness-v216.json\",\"fixtures/release/shard-readiness-v217.json\",\"fixtures/release/shard-readiness-v218.json\",\"fixtures/release/shard-readiness-v219.json\",\"fixtures/release/shard-readiness-v220.json\",\"fixtures/release/shard-readiness-v221.json\",\"fixtures/release/shard-readiness-v222.json\",\"fixtures/release/shard-readiness-v223.json\",\"fixtures/release/shard-readiness-v224.json\",\"fixtures/release/shard-readiness-v225.json\",\"fixtures/release/shard-readiness-v226.json\",\"fixtures/release/shard-readiness-v227.json\",\"fixtures/release/shard-readiness-v228.json\",\"fixtures/release/shard-readiness-v229.json\",\"fixtures/release/shard-readiness-v230.json\",\"fixtures/release/shard-readiness-v231.json\",\"fixtures/release/shard-readiness-v232.json\",\"fixtures/release/shard-readiness-v233.json\",\"fixtures/release/shard-readiness-v234.json\",\"fixtures/release/shard-readiness-v235.json\",\"fixtures/release/shard-readiness-v236.json\",\"fixtures/release/shard-readiness-v237.json\",\"fixtures/release/shard-readiness-v238.json\",\"fixtures/release/shard-readiness-v239.json\",\"fixtures/release/shard-readiness-v240.json\",\"fixtures/release/shard-readiness-v241.json\",\"fixtures/release/shard-readiness-v242.json\",\"fixtures/release/shard-readiness-v243.json\",\"fixtures/release/shard-readiness-v244.json\",\"fixtures/release/shard-readiness-v245.json\",\"fixtures/release/shard-readiness-v246.json\",\"fixtures/release/shard-readiness-v247.json\",\"fixtures/release/shard-readiness-v248.json\",\"fixtures/release/shard-readiness-v249.json\",\"fixtures/release/shard-readiness-v250.json\",\"fixtures/release/shard-readiness-v251.json\",\"fixtures/release/shard-readiness-v252.json\",\"fixtures/release/shard-readiness-v253.json\",\"fixtures/release/shard-readiness-v254.json\",\"fixtures/release/shard-readiness-v255.json\",\"fixtures/release/shard-readiness-v256.json\",\"fixtures/release/shard-readiness-v257.json\",\"fixtures/release/shard-readiness-v258.json\",\"fixtures/release/shard-readiness-v259.json\",\"fixtures/release/shard-readiness-v260.json\",\"fixtures/release/shard-readiness-v261.json\"]");
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
    assert_contains(json, "\"historicalFallback\":{\"previousConsumedReleaseVersion\":\"v362\"");
    assert_contains(json, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v362.json\"");
    assert_contains(
        json,
        "\"previousConsumptionNodeVersion\":\"Node v550+ may consume v363 as read-only shard route preview verification report archive closeout explain-contract evidence only\"");
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
    assert_contains(json, "\"nodeV479JavaMiniKvLatestRouteCatalogCleanupEvidenceArchiveVerificationRoutePreserved\":true");
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
    assert_contains(json, "\"releaseVersion\":\"v190\",\"status\":\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v191\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v192\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v193\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
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
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
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
    assert_contains(json, "\"releaseVersion\":\"v194\",\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v195\",\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-audit-read-only\"");
    assert_contains(json, "\"releaseVersion\":\"v196\",\"status\":\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
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
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
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
                          "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v362\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v362.json\"");
    assert_contains(json, "\"sourceFrozenDigest\":\"fnv1a64:ac8f76721f29a0f0\"");
    assert_contains(json, "\"continuityStage\":\"feature-read-only-shard-route-preview-verification-report-archive-closeout-explain-contract\"");
    assert_contains(json, "\"stageSequence\":163");
    assert_contains(json, "\"stageReleaseVersion\":\"v363\"");
    assert_contains(json, "\"nodeBatchCloseoutVersion\":\"Node v549\"");
    assert_contains(json, "\"previousCloseoutAuditReleaseVersion\":\"v200\"");
    assert_contains(json, "\"trackedMiniKvCloseoutRangeStart\":\"v194\"");
    assert_contains(json, "\"trackedMiniKvCloseoutRangeEnd\":\"v200\"");
    assert_contains(json, "\"trackedPostCloseoutRangeStart\":\"v201\"");
    assert_contains(json, "\"trackedPostCloseoutRangeEnd\":\"v363\"");
    assert_contains(json, "\"trackedPostCloseoutReleaseCount\":163");
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
    assert_contains(json, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.latestPackagedDigestMatchesPackageSource\"");
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
    assert_contains(json, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.latestCloseoutDigestMatchesCloseoutSource\"");
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

} // namespace

int main() {
    const auto fixture = read_fixture_text(std::filesystem::path{"fixtures"} / "release" / "shard-readiness.json");
    const auto historical_fixtures = read_shard_readiness_fixtures(144, 200);
    const auto& historical_fixture = historical_fixtures.at(144);
    const auto fixture_for = [&historical_fixtures](int release_version) -> const std::string& {
        return historical_fixtures.at(release_version);
    };
    const auto formatted_fixture = minikv::shard_readiness::format_json();
    if (fixture != formatted_fixture) {
        std::cerr << "fixture length=" << fixture.size()
                  << " formatted length=" << formatted_fixture.size() << '\n';
        const auto limit = std::min(fixture.size(), formatted_fixture.size());
        for (std::size_t index = 0; index < limit; ++index) {
            if (fixture[index] != formatted_fixture[index]) {
                std::cerr << "first diff at " << index << " fixture="
                          << static_cast<int>(static_cast<unsigned char>(fixture[index]))
                          << " formatted="
                          << static_cast<int>(static_cast<unsigned char>(formatted_fixture[index]))
                          << '\n';
                break;
            }
        }
    }
    assert(fixture == formatted_fixture);
    assert(minikv::shard_readiness::fixture_path() == "fixtures/release/shard-readiness.json");
    assert_shard_readiness_contract(fixture);
    assert_fixture_differs_from_each(fixture, historical_fixtures);
    assert_contains(historical_fixture, "\"releaseVersion\":\"v144\"");
    assert_contains(historical_fixture, "\"status\":\"prototype-ready-read-only\"");
    assert_contains(historical_fixture, "\"evidenceDigest\":\"fnv1a64:22d3c4815a440804\"");
    assert_contains(fixture_for(145), "\"releaseVersion\":\"v145\"");
    assert_contains(fixture_for(145), "\"status\":\"hardened-read-only\"");
    assert_contains(fixture_for(145), "\"evidenceDigest\":\"fnv1a64:ebe4c7e1a2704482\"");
    assert_contains(fixture_for(145), "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_contains(fixture_for(146), "\"releaseVersion\":\"v146\"");
    assert_contains(fixture_for(146), "\"status\":\"historical-fallback-hardened-read-only\"");
    assert_contains(fixture_for(146), "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v145.json\"");
    assert_contains(fixture_for(146), "\"evidenceDigest\":\"fnv1a64:6847d87decb76fcb\"");
    assert_contains(fixture_for(147), "\"releaseVersion\":\"v147\"");
    assert_contains(fixture_for(147), "\"status\":\"active-prototype-prerequisite-read-only\"");
    assert_contains(fixture_for(147), "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_contains(fixture_for(147), "\"activeShardPrototypeAllowed\":false");
    assert_contains(fixture_for(147), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(147), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(147), "\"evidenceDigest\":\"fnv1a64:e4a386fc9add4eaf\"");
    assert_contains(fixture_for(148), "\"releaseVersion\":\"v148\"");
    assert_contains(fixture_for(148), "\"status\":\"active-prototype-plan-frozen-read-only\"");
    assert_contains(fixture_for(148), "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v147\"");
    assert_contains(fixture_for(148), "\"frozenRouterActivationAllowed\":false");
    assert_contains(fixture_for(148), "\"frozenWriteRoutingAllowed\":false");
    assert_contains(fixture_for(148), "\"evidenceDigest\":\"fnv1a64:8270eeebbf1a7f29\"");
    assert_contains(fixture_for(149), "\"releaseVersion\":\"v149\"");
    assert_contains(fixture_for(149), "\"status\":\"frozen-evidence-handoff-read-only\"");
    assert_contains(fixture_for(149), "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v148\"");
    assert_contains(fixture_for(149), "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_contains(fixture_for(149), "\"startsServices\":false");
    assert_contains(fixture_for(149), "\"executionAllowed\":false");
    assert_contains(fixture_for(149), "\"evidenceDigest\":\"fnv1a64:521fe6dee47f7f2c\"");
    assert_contains(fixture_for(150), "\"releaseVersion\":\"v150\"");
    assert_contains(fixture_for(150), "\"status\":\"live-read-gate-prerequisite-read-only\"");
    assert_contains(fixture_for(150), "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_contains(fixture_for(150), "\"liveReadGateAllowed\":false");
    assert_contains(fixture_for(150), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture_for(150), "\"startsServices\":false");
    assert_contains(fixture_for(150), "\"evidenceDigest\":\"fnv1a64:b8b134f6aa527ca4\"");
    assert_contains(fixture_for(151), "\"releaseVersion\":\"v151\"");
    assert_contains(fixture_for(151), "\"status\":\"operator-service-lifecycle-template-read-only\"");
    assert_contains(fixture_for(151), "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_contains(fixture_for(151), "\"serviceOwnerDeclared\":false");
    assert_contains(fixture_for(151), "\"startupCommandDeclared\":false");
    assert_contains(fixture_for(151), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture_for(151), "\"evidenceDigest\":\"fnv1a64:c9fb568ddff895e3\"");
    assert_contains(fixture_for(152), "\"releaseVersion\":\"v152\"");
    assert_contains(fixture_for(152), "\"status\":\"declared-operator-lifecycle-no-runtime-read-only\"");
    assert_contains(fixture_for(152),
                    "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_contains(fixture_for(152), "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_contains(fixture_for(152), "\"serviceOwnerDeclared\":true");
    assert_contains(fixture_for(152), "\"runtimeGateApproved\":false");
    assert_contains(fixture_for(152), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture_for(152), "\"executionAllowed\":false");
    assert_contains(fixture_for(152), "\"evidenceDigest\":\"fnv1a64:55cd5b8db109c64f\"");
    assert_contains(fixture_for(153), "\"releaseVersion\":\"v153\"");
    assert_contains(fixture_for(153),
                    "\"status\":\"runtime-execution-artifact-intake-preflight-blocked-read-only\"");
    assert_contains(fixture_for(153),
                    "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                    "\"blocked-missing-runtime-execution-artifacts\"");
    assert_contains(fixture_for(153), "\"presentRuntimeExecutionArtifactCount\":0");
    assert_contains(fixture_for(153), "\"missingRuntimeExecutionArtifactCount\":6");
    assert_contains(fixture_for(153), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(153), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(153), "\"executionAllowed\":false");
    assert_contains(fixture_for(153), "\"evidenceDigest\":\"fnv1a64:ca09095f3f3e4b2c\"");
    assert_contains(fixture_for(154), "\"releaseVersion\":\"v154\"");
    assert_contains(fixture_for(154),
                    "\"status\":\"mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only\"");
    assert_contains(fixture_for(154),
                    "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                    "\"mini-kv-side-candidate-no-runtime\"");
    assert_contains(fixture_for(154), "\"candidateArtifactCount\":4");
    assert_contains(fixture_for(154), "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_contains(fixture_for(154), "\"missingAcceptedRuntimeExecutionArtifactCount\":6");
    assert_contains(fixture_for(154), "\"miniKvLoopbackPortCandidate\":6424");
    assert_contains(fixture_for(154), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(154), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(154), "\"executionAllowed\":false");
    assert_contains(fixture_for(154), "\"evidenceDigest\":\"fnv1a64:edbe9c546c0d780a\"");
    assert_contains(fixture_for(155), "\"releaseVersion\":\"v155\"");
    assert_contains(fixture_for(155),
                    "\"status\":\"runtime-execution-approval-gate-input-precheck-blocked-read-only\"");
    assert_contains(fixture_for(155),
                    "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                    "\"blocked-missing-approval-gate-inputs\"");
    assert_contains(fixture_for(155), "\"approvalGateInputCount\":0");
    assert_contains(fixture_for(155), "\"missingApprovalGateInputCount\":3");
    assert_contains(fixture_for(155), "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(fixture_for(155), "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(fixture_for(155), "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(fixture_for(155), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(155), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(155), "\"executionAllowed\":false");
    assert_contains(fixture_for(155), "\"evidenceDigest\":\"fnv1a64:edb1c4d8534461ab\"");
    assert_contains(fixture_for(156), "\"releaseVersion\":\"v156\"");
    assert_contains(fixture_for(156), "\"status\":\"mini-kv-final-approval-gate-input-no-runtime-read-only\"");
    assert_contains(fixture_for(156),
                    "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_contains(fixture_for(156), "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_contains(fixture_for(156), "\"miniKvApprovalGateInputComplete\":true");
    assert_contains(fixture_for(156), "\"miniKvLoopbackPort\":6424");
    assert_contains(fixture_for(156), "\"cleanupProofPresent\":false");
    assert_contains(fixture_for(156), "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_contains(fixture_for(156), "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_contains(fixture_for(156), "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_contains(fixture_for(156), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(156), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(156), "\"executionAllowed\":false");
    assert_contains(fixture_for(156), "\"evidenceDigest\":\"fnv1a64:f240d1fe1b25fab7\"");
    assert_contains(fixture_for(157), "\"releaseVersion\":\"v157\"");
    assert_contains(fixture_for(157),
                    "\"status\":\"runtime-execution-approval-input-template-validator-echo-read-only\"");
    assert_contains(fixture_for(157),
                    "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                    "\"template-validator-echo-no-canonical-inputs\"");
    assert_contains(fixture_for(157), "\"templateOnlyInputCount\":3");
    assert_contains(fixture_for(157), "\"canonicalRuntimeInputPresent\":false");
    assert_contains(fixture_for(157), "\"templateCopiedToCanonicalInput\":false");
    assert_contains(fixture_for(157), "\"sharedApprovalCorrelationIdPresent\":false");
    assert_contains(fixture_for(157), "\"templatesAuthorizeRuntime\":false");
    assert_contains(fixture_for(157), "\"templateDigestAcceptedAsApproval\":false");
    assert_contains(fixture_for(157), "\"writesCanonicalApprovalInputFiles\":false");
    assert_contains(fixture_for(157), "\"changesNodeInputTemplateFiles\":false");
    assert_contains(fixture_for(157), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(157), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(157), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(157), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(157), "\"executionAllowed\":false");
    assert_contains(fixture_for(157), "\"evidenceDigest\":\"fnv1a64:1d8cdf7f597f837e\"");
    assert_contains(fixture_for(158), "\"releaseVersion\":\"v158\"");
    assert_contains(fixture_for(158),
                    "\"status\":\"runtime-execution-canonical-approval-input-precheck-read-only\"");
    assert_contains(fixture_for(158),
                    "\"runtimeExecutionCanonicalApprovalInputPrecheck\":{\"precheckMode\":"
                    "\"blocked-missing-canonical-approval-inputs\"");
    assert_contains(fixture_for(158), "\"requiredCanonicalInputCount\":3");
    assert_contains(fixture_for(158), "\"presentCanonicalInputCount\":0");
    assert_contains(fixture_for(158), "\"missingCanonicalInputCount\":3");
    assert_contains(fixture_for(158), "\"canonicalApprovalInputsComplete\":false");
    assert_contains(fixture_for(158), "\"sharedApprovalCorrelationIdValidated\":false");
    assert_contains(fixture_for(158), "\"templatesAcceptedAsCanonicalInputs\":false");
    assert_contains(fixture_for(158), "\"templateCompatibilityEvidenceAcceptedAsApproval\":false");
    assert_contains(fixture_for(158), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(158), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(158), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(158), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(158), "\"executionAllowed\":false");
    assert_contains(fixture_for(158), "\"evidenceDigest\":\"fnv1a64:bce4629123167bc6\"");
    assert_contains(fixture_for(159), "\"releaseVersion\":\"v159\"");
    assert_contains(fixture_for(159), "\"status\":\"node-route-group-split-compatibility-read-only\"");
    assert_contains(fixture_for(159), "\"nodeRouteGroupSplitCompatibility\":{\"compatibilityMode\":"
                                          "\"node-route-group-split-contract-stable\"");
    assert_contains(fixture_for(159),
                    "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\"");
    assert_contains(fixture_for(159), "\"sourceFrozenReleaseVersion\":\"v158\"");
    assert_contains(fixture_for(159), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v158.json\"");
    assert_contains(fixture_for(159), "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_contains(fixture_for(159), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(159), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(159), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(159), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(159), "\"executionAllowed\":false");
    assert_contains(fixture_for(159), "\"evidenceDigest\":\"fnv1a64:0741c3475ee371e2\"");
    assert_contains(fixture_for(160), "\"releaseVersion\":\"v160\"");
    assert_contains(fixture_for(160), "\"status\":\"boundary-field-catalog-split-read-only\"");
    assert_contains(fixture_for(160), "\"boundaryCatalogMaintenance\":{\"maintenanceMode\":"
                                          "\"boundary-field-catalog-split-contract-preserving\"");
    assert_contains(fixture_for(160), "\"sourceFrozenReleaseVersion\":\"v159\"");
    assert_contains(fixture_for(160), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v159.json\"");
    assert_contains(fixture_for(160), "\"boundaryFieldCatalogExtracted\":true");
    assert_contains(fixture_for(160), "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_contains(fixture_for(160), "\"publicShardJsonContractChanged\":false");
    assert_contains(fixture_for(160), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(160), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(160), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(160), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(160), "\"executionAllowed\":false");
    assert_contains(fixture_for(160), "\"evidenceDigest\":\"fnv1a64:c96ae9497cfd6386\"");
    assert_contains(fixture_for(161), "\"releaseVersion\":\"v161\"");
    assert_contains(fixture_for(161), "\"status\":\"boundary-field-catalog-index-read-only\"");
    assert_contains(fixture_for(161), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v2\"");
    assert_contains(fixture_for(161), "\"sourceFrozenReleaseVersion\":\"v160\"");
    assert_contains(fixture_for(161), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v160.json\"");
    assert_contains(fixture_for(161), "\"fieldCount\":290");
    assert_contains(fixture_for(161), "\"groupCount\":15");
    assert_contains(fixture_for(161), "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_contains(fixture_for(161), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(161), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(161), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(161), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(161), "\"executionAllowed\":false");
    assert_contains(fixture_for(161), "\"evidenceDigest\":\"fnv1a64:2193962195ba633a\"");
    assert_contains(fixture_for(162), "\"releaseVersion\":\"v162\"");
    assert_contains(fixture_for(162), "\"status\":\"slot-table-preview-read-only\"");
    assert_contains(fixture_for(162), "\"slotTablePreview\":{\"previewMode\":\"single-shard-slot-table-read-only\"");
    assert_contains(fixture_for(162), "\"sourceFrozenReleaseVersion\":\"v161\"");
    assert_contains(fixture_for(162), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v161.json\"");
    assert_contains(fixture_for(162), "\"slotTablePreviewOnly\":true");
    assert_contains(fixture_for(162), "\"readOnlyShardMapPreview\":true");
    assert_contains(fixture_for(162), "\"activeRouterInstalled\":false");
    assert_contains(fixture_for(162), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(162), "\"executionAllowed\":false");
    assert_contains(fixture_for(162), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v3\"");
    assert_contains(fixture_for(162), "\"fieldCount\":303");
    assert_contains(fixture_for(162), "\"groupCount\":16");
    assert_contains(fixture_for(162), "\"evidenceDigest\":\"fnv1a64:8c01a3657f07cfd1\"");
    assert_contains(fixture_for(163), "\"releaseVersion\":\"v163\"");
    assert_contains(fixture_for(163), "\"status\":\"slot-table-preview-audit-read-only\"");
    assert_contains(fixture_for(163), "\"slotTablePreviewAudit\":{\"auditMode\":"
                                           "\"slot-table-preview-consistency-read-only\"");
    assert_contains(fixture_for(163), "\"sourceFrozenReleaseVersion\":\"v162\"");
    assert_contains(fixture_for(163), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\"");
    assert_contains(fixture_for(163), "\"observedSlotCount\":16");
    assert_contains(fixture_for(163), "\"contiguousSlotRange\":true");
    assert_contains(fixture_for(163), "\"duplicateSlotsDetected\":false");
    assert_contains(fixture_for(163), "\"unassignedSlotsDetected\":false");
    assert_contains(fixture_for(163), "\"allAssignmentsReadOnly\":true");
    assert_contains(fixture_for(163), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(163), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(163), "\"executionAllowed\":false");
    assert_contains(fixture_for(163), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v4\"");
    assert_contains(fixture_for(163), "\"fieldCount\":319");
    assert_contains(fixture_for(163), "\"groupCount\":17");
    assert_contains(fixture_for(163), "\"evidenceDigest\":\"fnv1a64:f81c5b47cb396df6\"");
    assert_contains(fixture_for(164), "\"releaseVersion\":\"v164\"");
    assert_contains(fixture_for(164), "\"status\":\"slot-table-preview-audit-maintenance-read-only\"");
    assert_contains(fixture_for(164), "\"slotTablePreviewAuditMaintenance\":{\"maintenanceMode\":"
                                           "\"slot-table-preview-audit-formatter-split-contract-preserving\"");
    assert_contains(fixture_for(164), "\"sourceFrozenReleaseVersion\":\"v163\"");
    assert_contains(fixture_for(164), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v163.json\"");
    assert_contains(fixture_for(164), "\"auditFormatterExtracted\":true");
    assert_contains(fixture_for(164), "\"slotPreviewModuleOwnsAssignments\":true");
    assert_contains(fixture_for(164), "\"slotPreviewAuditStillPublished\":true");
    assert_contains(fixture_for(164), "\"slotTablePreviewStillPublished\":true");
    assert_contains(fixture_for(164), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(164), "\"startsJavaService\":false");
    assert_contains(fixture_for(164), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(164), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(164), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(164), "\"executionAllowed\":false");
    assert_contains(fixture_for(164), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v5\"");
    assert_contains(fixture_for(164), "\"fieldCount\":337");
    assert_contains(fixture_for(164), "\"groupCount\":18");
    assert_contains(fixture_for(164), "\"evidenceDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_contains(fixture_for(165), "\"releaseVersion\":\"v165\"");
    assert_contains(fixture_for(165), "\"status\":\"shard-readiness-release-catalog-read-only\"");
    assert_contains(fixture_for(165), "\"shardReadinessReleaseCatalog\":{\"catalogMode\":"
                                           "\"versioned-shard-readiness-release-catalog-read-only\"");
    assert_contains(fixture_for(165), "\"sourceFrozenReleaseVersion\":\"v164\"");
    assert_contains(fixture_for(165), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v164.json\"");
    assert_contains(fixture_for(165), "\"catalogedReleaseCount\":4");
    assert_contains(fixture_for(165), "\"latestCatalogedReleaseVersion\":\"v164\"");
    assert_contains(fixture_for(165), "\"latestCatalogedDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_contains(fixture_for(165), "\"primarySection\":\"slotTablePreviewAuditMaintenance\"");
    assert_contains(fixture_for(165), "\"versionedFixtureCatalogOnly\":true");
    assert_contains(fixture_for(165), "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_contains(fixture_for(165), "\"activeRouterInstalled\":false");
    assert_contains(fixture_for(165), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(165), "\"executionAllowed\":false");
    assert_contains(fixture_for(165), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v6\"");
    assert_contains(fixture_for(165), "\"fieldCount\":357");
    assert_contains(fixture_for(165), "\"groupCount\":19");
    assert_contains(fixture_for(165), "\"evidenceDigest\":\"fnv1a64:5e94e2a9b438d2ad\"");
    assert_contains(fixture_for(166), "\"releaseVersion\":\"v166\"");
    assert_contains(fixture_for(166), "\"status\":\"shard-readiness-release-catalog-audit-read-only\"");
    assert_contains(fixture_for(166), "\"shardReadinessReleaseCatalogAudit\":{\"auditMode\":"
                                           "\"shard-readiness-release-catalog-consistency-read-only\"");
    assert_contains(fixture_for(166), "\"sourceFrozenReleaseVersion\":\"v165\"");
    assert_contains(fixture_for(166), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v165.json\"");
    assert_contains(fixture_for(166), "\"expectedReleaseCount\":4");
    assert_contains(fixture_for(166), "\"observedReleaseCount\":4");
    assert_contains(fixture_for(166), "\"contiguousReleaseRange\":true");
    assert_contains(fixture_for(166), "\"duplicateReleasesDetected\":false");
    assert_contains(fixture_for(166), "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(fixture_for(166), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(166), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(166), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(166), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(166), "\"executionAllowed\":false");
    assert_contains(fixture_for(166), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v7\"");
    assert_contains(fixture_for(166), "\"fieldCount\":381");
    assert_contains(fixture_for(166), "\"groupCount\":20");
    assert_contains(fixture_for(166), "\"evidenceDigest\":\"fnv1a64:4bd18a01790dc5f1\"");
    assert_contains(fixture_for(167), "\"releaseVersion\":\"v167\"");
    assert_contains(fixture_for(167), "\"status\":\"node-route-split-window-compatibility-read-only\"");
    assert_contains(fixture_for(167), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v440-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(167), "\"sourceFrozenReleaseVersion\":\"v166\"");
    assert_contains(fixture_for(167), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v166.json\"");
    assert_contains(fixture_for(167), "\"windowEndNodeVersion\":\"Node v440\"");
    assert_contains(fixture_for(167), "\"splitRouteGroupCount\":8");
    assert_contains(fixture_for(167), "\"nodeAddsEvidenceGate\":false");
    assert_contains(fixture_for(167), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(167), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(167), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(167), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(167), "\"executionAllowed\":false");
    assert_contains(fixture_for(167), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v8\"");
    assert_contains(fixture_for(167), "\"fieldCount\":401");
    assert_contains(fixture_for(167), "\"groupCount\":21");
    assert_contains(fixture_for(167), "\"evidenceDigest\":\"fnv1a64:caf300915d4e988c\"");
    assert_contains(fixture_for(168), "\"releaseVersion\":\"v168\"");
    assert_contains(fixture_for(168), "\"status\":\"node-route-split-window-preparation-plan-read-only\"");
    assert_contains(fixture_for(168), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v441-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(168), "\"sourceFrozenReleaseVersion\":\"v167\"");
    assert_contains(fixture_for(168), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v167.json\"");
    assert_contains(fixture_for(168), "\"windowEndNodeVersion\":\"Node v441\"");
    assert_contains(fixture_for(168), "\"splitRouteGroupCount\":9");
    assert_contains(fixture_for(168), "\"nodeV441DesignDraftBodyPreparationPlanRouteSplitPreserved\":true");
    assert_contains(fixture_for(168), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(168), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(168), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(168), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(168), "\"executionAllowed\":false");
    assert_contains(fixture_for(168), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v8\"");
    assert_contains(fixture_for(168), "\"fieldCount\":401");
    assert_contains(fixture_for(168), "\"groupCount\":21");
    assert_contains(fixture_for(168), "\"evidenceDigest\":\"fnv1a64:d0d0ef2b2a0f5a0a\"");
    assert_contains(fixture_for(169), "\"releaseVersion\":\"v169\"");
    assert_contains(fixture_for(169), "\"status\":\"node-route-split-window-audit-read-only\"");
    assert_contains(fixture_for(169), "\"nodeRouteSplitCompatibilityWindowAudit\":{\"auditMode\":"
                                           "\"node-route-split-compatibility-window-consistency-read-only\"");
    assert_contains(fixture_for(169), "\"sourceFrozenReleaseVersion\":\"v168\"");
    assert_contains(fixture_for(169), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v168.json\"");
    assert_contains(fixture_for(169), "\"expectedWindowVersionCount\":9");
    assert_contains(fixture_for(169), "\"observedWindowVersionCount\":9");
    assert_contains(fixture_for(169), "\"windowRangeEnd\":\"Node v441\"");
    assert_contains(fixture_for(169), "\"sourceFrozenWindowDigest\":\"fnv1a64:d0d0ef2b2a0f5a0a\"");
    assert_contains(fixture_for(169), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(169), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(169), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(169), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(169), "\"executionAllowed\":false");
    assert_contains(fixture_for(169), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(169), "\"fieldCount\":420");
    assert_contains(fixture_for(169), "\"groupCount\":22");
    assert_contains(fixture_for(169), "\"evidenceDigest\":\"fnv1a64:eaa587d1a50d8200\"");
    assert_contains(fixture_for(170), "\"releaseVersion\":\"v170\"");
    assert_contains(fixture_for(170), "\"status\":\"node-route-split-window-draft-candidate-read-only\"");
    assert_contains(fixture_for(170), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v442-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(170), "\"sourceFrozenReleaseVersion\":\"v169\"");
    assert_contains(fixture_for(170), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v169.json\"");
    assert_contains(fixture_for(170), "\"windowEndNodeVersion\":\"Node v442\"");
    assert_contains(fixture_for(170), "\"splitRouteGroupCount\":10");
    assert_contains(fixture_for(170), "\"observedWindowVersionCount\":10");
    assert_contains(fixture_for(170), "\"nodeV442DesignDraftBodyDraftCandidateRouteSplitPreserved\":true");
    assert_contains(fixture_for(170), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(170), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(170), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(170), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(170), "\"executionAllowed\":false");
    assert_contains(fixture_for(170), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(170), "\"fieldCount\":420");
    assert_contains(fixture_for(170), "\"groupCount\":22");
    assert_contains(fixture_for(170), "\"evidenceDigest\":\"fnv1a64:d06aaa033f5c1d86\"");
    assert_contains(fixture_for(171), "\"releaseVersion\":\"v171\"");
    assert_contains(fixture_for(171), "\"status\":\"node-route-split-window-managed-audit-read-only\"");
    assert_contains(fixture_for(171), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v444-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(171), "\"sourceFrozenReleaseVersion\":\"v170\"");
    assert_contains(fixture_for(171), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v170.json\"");
    assert_contains(fixture_for(171), "\"windowEndNodeVersion\":\"Node v444\"");
    assert_contains(fixture_for(171), "\"splitRouteGroupCount\":12");
    assert_contains(fixture_for(171), "\"observedWindowVersionCount\":12");
    assert_contains(fixture_for(171), "\"nodeV443ManagedAuditAdapterRouteSplitPreserved\":true");
    assert_contains(fixture_for(171), "\"nodeV444ManagedAuditPersistenceRouteSplitPreserved\":true");
    assert_contains(fixture_for(171), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(171), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(171), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(171), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(171), "\"executionAllowed\":false");
    assert_contains(fixture_for(171), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(171), "\"fieldCount\":420");
    assert_contains(fixture_for(171), "\"groupCount\":22");
    assert_contains(fixture_for(171), "\"evidenceDigest\":\"fnv1a64:9f6aa583ef521d1a\"");
    assert_contains(fixture_for(172), "\"releaseVersion\":\"v172\"");
    assert_contains(fixture_for(172), "\"status\":\"node-route-split-window-restore-drill-read-only\"");
    assert_contains(fixture_for(172), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v446-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(172), "\"sourceFrozenReleaseVersion\":\"v171\"");
    assert_contains(fixture_for(172), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v171.json\"");
    assert_contains(fixture_for(172), "\"windowEndNodeVersion\":\"Node v446\"");
    assert_contains(fixture_for(172), "\"splitRouteGroupCount\":14");
    assert_contains(fixture_for(172), "\"observedWindowVersionCount\":14");
    assert_contains(fixture_for(172), "\"nodeV445ManagedAuditIdentityApprovalRouteSplitPreserved\":true");
    assert_contains(fixture_for(172), "\"nodeV446ManagedAuditRestoreDrillRouteSplitPreserved\":true");
    assert_contains(fixture_for(172), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(172), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(172), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(172), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(172), "\"executionAllowed\":false");
    assert_contains(fixture_for(172), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(172), "\"fieldCount\":420");
    assert_contains(fixture_for(172), "\"groupCount\":22");
    assert_contains(fixture_for(172), "\"evidenceDigest\":\"fnv1a64:85f0acb5a011256f\"");
    assert_contains(fixture_for(173), "\"releaseVersion\":\"v173\"");
    assert_contains(fixture_for(173), "\"status\":\"node-route-split-window-dry-run-adapter-read-only\"");
    assert_contains(fixture_for(173), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v447-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(173), "\"sourceFrozenReleaseVersion\":\"v172\"");
    assert_contains(fixture_for(173), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v172.json\"");
    assert_contains(fixture_for(173), "\"windowEndNodeVersion\":\"Node v447\"");
    assert_contains(fixture_for(173), "\"splitRouteGroupCount\":15");
    assert_contains(fixture_for(173), "\"observedWindowVersionCount\":15");
    assert_contains(fixture_for(173), "\"nodeV447ManagedAuditDryRunAdapterRouteSplitPreserved\":true");
    assert_contains(fixture_for(173), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(173), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(173), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(173), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(173), "\"executionAllowed\":false");
    assert_contains(fixture_for(173), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(173), "\"fieldCount\":420");
    assert_contains(fixture_for(173), "\"groupCount\":22");
    assert_contains(fixture_for(173), "\"evidenceDigest\":\"fnv1a64:33a60bc1d66fd286\"");
    assert_contains(fixture_for(174), "\"releaseVersion\":\"v174\"");
    assert_contains(fixture_for(174), "\"status\":\"node-route-split-window-route-quality-read-only\"");
    assert_contains(fixture_for(174), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v448-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(174), "\"sourceFrozenReleaseVersion\":\"v173\"");
    assert_contains(fixture_for(174), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v173.json\"");
    assert_contains(fixture_for(174), "\"windowEndNodeVersion\":\"Node v448\"");
    assert_contains(fixture_for(174), "\"splitRouteGroupCount\":16");
    assert_contains(fixture_for(174), "\"observedWindowVersionCount\":16");
    assert_contains(fixture_for(174), "\"nodeV448ManagedAuditRouteQualityRouteSplitPreserved\":true");
    assert_contains(fixture_for(174), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(174), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(174), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(174), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(174), "\"executionAllowed\":false");
    assert_contains(fixture_for(174), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(174), "\"fieldCount\":420");
    assert_contains(fixture_for(174), "\"groupCount\":22");
    assert_contains(fixture_for(174), "\"evidenceDigest\":\"fnv1a64:d582c3d04aed99e1\"");
    assert_contains(fixture_for(175), "\"releaseVersion\":\"v175\"");
    assert_contains(fixture_for(175), "\"status\":\"node-route-split-window-adapter-implementation-read-only\"");
    assert_contains(fixture_for(175), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v449-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(175), "\"sourceFrozenReleaseVersion\":\"v174\"");
    assert_contains(fixture_for(175), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v174.json\"");
    assert_contains(fixture_for(175), "\"windowEndNodeVersion\":\"Node v449\"");
    assert_contains(fixture_for(175), "\"splitRouteGroupCount\":17");
    assert_contains(fixture_for(175), "\"observedWindowVersionCount\":17");
    assert_contains(fixture_for(175), "\"nodeV449ManagedAuditAdapterImplementationRouteSplitPreserved\":true");
    assert_contains(fixture_for(175), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(175), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(175), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(175), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(175), "\"executionAllowed\":false");
    assert_contains(fixture_for(175), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(175), "\"fieldCount\":420");
    assert_contains(fixture_for(175), "\"groupCount\":22");
    assert_contains(fixture_for(175), "\"evidenceDigest\":\"fnv1a64:61e561e1c8ba7d50\"");
    assert_contains(fixture_for(176), "\"releaseVersion\":\"v176\"");
    assert_contains(fixture_for(176),
                    "\"status\":\"node-route-split-window-manual-sandbox-fake-transport-read-only\"");
    assert_contains(fixture_for(176), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v457-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(176), "\"sourceFrozenReleaseVersion\":\"v175\"");
    assert_contains(fixture_for(176), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v175.json\"");
    assert_contains(fixture_for(176), "\"windowEndNodeVersion\":\"Node v457\"");
    assert_contains(fixture_for(176), "\"splitRouteGroupCount\":25");
    assert_contains(fixture_for(176), "\"observedWindowVersionCount\":25");
    assert_contains(fixture_for(176), "\"nodeV457ManualSandboxConnectionFakeTransportRouteSplitPreserved\":true");
    assert_contains(fixture_for(176), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(176), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(176), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(176), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(176), "\"executionAllowed\":false");
    assert_contains(fixture_for(176), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(176), "\"fieldCount\":420");
    assert_contains(fixture_for(176), "\"groupCount\":22");
    assert_contains(fixture_for(176), "\"evidenceDigest\":\"fnv1a64:528073c4315ef89b\"");
    assert_contains(fixture_for(177), "\"releaseVersion\":\"v177\"");
    assert_contains(fixture_for(177), "\"status\":\"node-route-split-window-foundational-audit-read-only\"");
    assert_contains(fixture_for(177), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v458-route-split-window-contract-stable-read-only\"");
    assert_contains(fixture_for(177), "\"sourceFrozenReleaseVersion\":\"v176\"");
    assert_contains(fixture_for(177), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v176.json\"");
    assert_contains(fixture_for(177), "\"windowEndNodeVersion\":\"Node v458\"");
    assert_contains(fixture_for(177), "\"splitRouteGroupCount\":26");
    assert_contains(fixture_for(177), "\"observedWindowVersionCount\":26");
    assert_contains(fixture_for(177), "\"nodeV458FoundationalAuditRouteSplitPreserved\":true");
    assert_contains(fixture_for(177), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(177), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(177), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(177), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(177), "\"executionAllowed\":false");
    assert_contains(fixture_for(177), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(177), "\"fieldCount\":420");
    assert_contains(fixture_for(177), "\"groupCount\":22");
    assert_contains(fixture_for(177), "\"evidenceDigest\":\"fnv1a64:e773708a4decc60e\"");
    assert_contains(fixture_for(178), "\"releaseVersion\":\"v178\"");
    assert_contains(fixture_for(178), "\"status\":\"node-route-split-window-computed-audit-read-only\"");
    assert_contains(fixture_for(178), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v458-route-split-window-computed-audit-read-only\"");
    assert_contains(fixture_for(178), "\"sourceFrozenReleaseVersion\":\"v177\"");
    assert_contains(fixture_for(178), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v177.json\"");
    assert_contains(fixture_for(178), "\"windowEndNodeVersion\":\"Node v458\"");
    assert_contains(fixture_for(178), "\"splitRouteGroupCount\":26");
    assert_contains(fixture_for(178), "\"observedWindowVersionCount\":26");
    assert_contains(fixture_for(178), "\"contiguousNodeVersionWindow\":true");
    assert_contains(fixture_for(178), "\"duplicateWindowVersionsDetected\":false");
    assert_contains(fixture_for(178), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(178), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(178), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(178), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(178), "\"executionAllowed\":false");
    assert_contains(fixture_for(178), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v9\"");
    assert_contains(fixture_for(178), "\"fieldCount\":420");
    assert_contains(fixture_for(178), "\"groupCount\":22");
    assert_contains(fixture_for(178), "\"evidenceDigest\":\"fnv1a64:dd89f7c3bd63d7c3\"");
    assert_contains(fixture_for(179), "\"releaseVersion\":\"v179\"");
    assert_contains(fixture_for(179), "\"status\":\"node-route-split-window-numeric-span-audit-read-only\"");
    assert_contains(fixture_for(179), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v458-route-split-window-numeric-span-audit-read-only\"");
    assert_contains(fixture_for(179), "\"sourceFrozenReleaseVersion\":\"v178\"");
    assert_contains(fixture_for(179), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v178.json\"");
    assert_contains(fixture_for(179), "\"windowRangeStartNumber\":433");
    assert_contains(fixture_for(179), "\"windowRangeEndNumber\":458");
    assert_contains(fixture_for(179), "\"computedWindowVersionSpan\":26");
    assert_contains(fixture_for(179), "\"windowRangeNumbersParseable\":true");
    assert_contains(fixture_for(179), "\"windowCountMatchesRange\":true");
    assert_contains(fixture_for(179), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(179), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(179), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(179), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(179), "\"executionAllowed\":false");
    assert_contains(fixture_for(179), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v10\"");
    assert_contains(fixture_for(179), "\"fieldCount\":422");
    assert_contains(fixture_for(179), "\"groupCount\":22");
    assert_contains(fixture_for(179), "\"evidenceDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_contains(fixture_for(180), "\"releaseVersion\":\"v180\"");
    assert_contains(fixture_for(180), "\"status\":\"shard-readiness-history-formatter-maintenance-read-only\"");
    assert_contains(fixture_for(180), "\"shardReadinessHistoryMaintenance\":{\"maintenanceMode\":"
                                           "\"history-fixture-archive-formatter-split-read-only\"");
    assert_contains(fixture_for(180), "\"sourceFrozenReleaseVersion\":\"v179\"");
    assert_contains(fixture_for(180), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v179.json\"");
    assert_contains(fixture_for(180), "\"sourceFrozenDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_contains(fixture_for(180), "\"preservesFixtureParity\":true");
    assert_contains(fixture_for(180), "\"preservesArchiveCompatibility\":true");
    assert_contains(fixture_for(180), "\"preservesHistoricalFallback\":true");
    assert_contains(fixture_for(180), "\"changesShardJsonCommand\":false");
    assert_contains(fixture_for(180), "\"changesFixturePath\":false");
    assert_contains(fixture_for(180), "\"startsServices\":false");
    assert_contains(fixture_for(180), "\"runtimeProbeAllowed\":false");
    assert_contains(fixture_for(180), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(180), "\"executionAllowed\":false");
    assert_contains(fixture_for(180), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v11\"");
    assert_contains(fixture_for(180), "\"fieldCount\":431");
    assert_contains(fixture_for(180), "\"groupCount\":23");
    assert_contains(fixture_for(180), "\"evidenceDigest\":\"fnv1a64:3403c490a3e623e0\"");
    assert_contains(fixture_for(181), "\"releaseVersion\":\"v181\"");
    assert_contains(fixture_for(181), "\"status\":\"node-route-catalog-integrity-window-read-only\"");
    assert_contains(fixture_for(181), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v460-route-catalog-integrity-window-read-only\"");
    assert_contains(fixture_for(181), "\"sourceFrozenReleaseVersion\":\"v180\"");
    assert_contains(fixture_for(181), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v180.json\"");
    assert_contains(fixture_for(181), "\"windowEndNodeVersion\":\"Node v460\"");
    assert_contains(fixture_for(181), "\"splitRouteGroupCount\":28");
    assert_contains(fixture_for(181), "\"computedWindowVersionSpan\":28");
    assert_contains(fixture_for(181), "\"sourceFrozenWindowDigest\":\"fnv1a64:3403c490a3e623e0\"");
    assert_contains(fixture_for(181), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(181), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(181), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(181), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(181), "\"executionAllowed\":false");
    assert_contains(fixture_for(181), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v11\"");
    assert_contains(fixture_for(181), "\"fieldCount\":431");
    assert_contains(fixture_for(181), "\"groupCount\":23");
    assert_contains(fixture_for(181), "\"evidenceDigest\":\"fnv1a64:58e9b51084ad91e5\"");
    assert_contains(fixture_for(182), "\"releaseVersion\":\"v182\"");
    assert_contains(fixture_for(182), "\"status\":\"node-route-catalog-test-migration-window-read-only\"");
    assert_contains(fixture_for(182), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v462-route-catalog-test-migration-window-read-only\"");
    assert_contains(fixture_for(182), "\"sourceFrozenReleaseVersion\":\"v181\"");
    assert_contains(fixture_for(182), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v181.json\"");
    assert_contains(fixture_for(182), "\"windowEndNodeVersion\":\"Node v462\"");
    assert_contains(fixture_for(182), "\"splitRouteGroupCount\":30");
    assert_contains(fixture_for(182), "\"computedWindowVersionSpan\":30");
    assert_contains(fixture_for(182), "\"sourceFrozenWindowDigest\":\"fnv1a64:58e9b51084ad91e5\"");
    assert_contains(fixture_for(182), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(182), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(182), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(182), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(182), "\"executionAllowed\":false");
    assert_contains(fixture_for(182), "\"evidenceDigest\":\"fnv1a64:e4a40fd2e1d1d741\"");
    assert_contains(fixture_for(183), "\"releaseVersion\":\"v183\"");
    assert_contains(fixture_for(183), "\"status\":\"node-route-quality-catalog-integrity-window-read-only\"");
    assert_contains(fixture_for(183), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v463-route-quality-catalog-integrity-window-read-only\"");
    assert_contains(fixture_for(183), "\"sourceFrozenReleaseVersion\":\"v182\"");
    assert_contains(fixture_for(183), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v182.json\"");
    assert_contains(fixture_for(183), "\"windowEndNodeVersion\":\"Node v463\"");
    assert_contains(fixture_for(183), "\"splitRouteGroupCount\":31");
    assert_contains(fixture_for(183), "\"computedWindowVersionSpan\":31");
    assert_contains(fixture_for(183), "\"sourceFrozenWindowDigest\":\"fnv1a64:e4a40fd2e1d1d741\"");
    assert_contains(fixture_for(183), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(183), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(183), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(183), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(183), "\"executionAllowed\":false");
    assert_contains(fixture_for(183), "\"evidenceDigest\":\"fnv1a64:8b252a90cb150a99\"");
    assert_contains(fixture_for(184), "\"releaseVersion\":\"v184\"");
    assert_contains(fixture_for(184), "\"status\":\"node-route-catalog-final-closeout-window-read-only\"");
    assert_contains(fixture_for(184), "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                           "\"node-v433-v464-route-catalog-final-closeout-window-read-only\"");
    assert_contains(fixture_for(184), "\"sourceFrozenReleaseVersion\":\"v183\"");
    assert_contains(fixture_for(184), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v183.json\"");
    assert_contains(fixture_for(184), "\"windowEndNodeVersion\":\"Node v464\"");
    assert_contains(fixture_for(184), "\"splitRouteGroupCount\":32");
    assert_contains(fixture_for(184), "\"computedWindowVersionSpan\":32");
    assert_contains(fixture_for(184), "\"sourceFrozenWindowDigest\":\"fnv1a64:8b252a90cb150a99\"");
    assert_contains(fixture_for(184), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(184), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(184), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(184), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(184), "\"executionAllowed\":false");
    assert_contains(fixture_for(184), "\"evidenceDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_contains(fixture_for(185), "\"releaseVersion\":\"v185\"");
    assert_contains(fixture_for(185), "\"status\":\"node-route-catalog-closeout-snapshot-read-only\"");
    assert_contains(fixture_for(185), "\"nodeRouteCatalogCloseoutSnapshot\":{\"snapshotMode\":"
                                           "\"node-route-catalog-final-closeout-snapshot-read-only\"");
    assert_contains(fixture_for(185), "\"sourceFrozenReleaseVersion\":\"v184\"");
    assert_contains(fixture_for(185), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v184.json\"");
    assert_contains(fixture_for(185), "\"sourceFrozenDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_contains(fixture_for(185), "\"focusedCloseoutTestFileCount\":7");
    assert_contains(fixture_for(185), "\"focusedCloseoutTestCount\":17");
    assert_contains(fixture_for(185), "\"nodeFullVitestFileCount\":393");
    assert_contains(fixture_for(185), "\"nodeFullVitestTestCount\":1221");
    assert_contains(fixture_for(185), "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_contains(fixture_for(185), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(185), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(185), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(185), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(185), "\"executionAllowed\":false");
    assert_contains(fixture_for(185), "\"evidenceDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_contains(fixture_for(186), "\"releaseVersion\":\"v186\"");
    assert_contains(fixture_for(186), "\"status\":\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(fixture_for(186), "\"nodeRouteCatalogAnchorRemovalAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_contains(fixture_for(186), "\"sourceFrozenReleaseVersion\":\"v185\"");
    assert_contains(fixture_for(186), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v185.json\"");
    assert_contains(fixture_for(186), "\"sourceFrozenDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_contains(fixture_for(186), "\"windowEndNodeVersion\":\"Node v467\"");
    assert_contains(fixture_for(186), "\"splitRouteGroupCount\":35");
    assert_contains(fixture_for(186), "\"computedWindowVersionSpan\":35");
    assert_contains(fixture_for(186), "\"sourceAnchorCompatibilityRemoved\":true");
    assert_contains(fixture_for(186), "\"miniKvRouteAnchorDependencyIntroduced\":false");
    assert_contains(fixture_for(186), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(186), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(186), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(186), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(186), "\"executionAllowed\":false");
    assert_contains(fixture_for(186), "\"evidenceDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_contains(fixture_for(187), "\"releaseVersion\":\"v187\"");
    assert_contains(fixture_for(187),
                    "\"status\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(fixture_for(187), "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_contains(fixture_for(187), "\"sourceFrozenReleaseVersion\":\"v186\"");
    assert_contains(fixture_for(187), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v186.json\"");
    assert_contains(fixture_for(187), "\"sourceFrozenDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_contains(fixture_for(187), "\"windowEndNodeVersion\":\"Node v471\"");
    assert_contains(fixture_for(187), "\"splitRouteGroupCount\":39");
    assert_contains(fixture_for(187), "\"computedWindowVersionSpan\":39");
    assert_contains(fixture_for(187), "\"expectedIntegritySnapshotFactoryMovedByNodeV471\":true");
    assert_contains(fixture_for(187), "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(fixture_for(187), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(187), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(187), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(187), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(187), "\"executionAllowed\":false");
    assert_contains(fixture_for(187), "\"evidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(fixture_for(188), "\"releaseVersion\":\"v188\"");
    assert_contains(fixture_for(188),
                    "\"status\":\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(fixture_for(188), "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":{\"freezeMode\":"
                                           "\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_contains(fixture_for(188), "\"frozenReleaseVersion\":\"v187\"");
    assert_contains(fixture_for(188), "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v187.json\"");
    assert_contains(fixture_for(188), "\"frozenEvidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(fixture_for(188), "\"sourceFrozenWindowDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_contains(fixture_for(188), "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_contains(fixture_for(188), "\"preservesExpectedIntegritySnapshotAudit\":true");
    assert_contains(fixture_for(188), "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_contains(fixture_for(188), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(188), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(188), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(188), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(188), "\"executionAllowed\":false");
    assert_contains(fixture_for(188), "\"evidenceDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(fixture_for(189), "\"releaseVersion\":\"v189\"");
    assert_contains(fixture_for(189), "\"status\":\"node-route-catalog-evidence-chain-read-only\"");
    assert_contains(fixture_for(189), "\"nodeRouteCatalogEvidenceChain\":{\"catalogMode\":"
                                           "\"node-route-catalog-evidence-chain-read-only\"");
    assert_contains(fixture_for(189), "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_contains(fixture_for(189), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_contains(fixture_for(189), "\"sourceFrozenDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(fixture_for(189), "\"catalogedReleaseCount\":4");
    assert_contains(fixture_for(189), "\"releaseRangeEnd\":\"v188\"");
    assert_contains(fixture_for(189), "\"latestCatalogedDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_contains(fixture_for(189), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v17\"");
    assert_contains(fixture_for(189), "\"fieldCount\":547");
    assert_contains(fixture_for(189), "\"groupCount\":28");
    assert_contains(fixture_for(189), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(189), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(189), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(189), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(189), "\"executionAllowed\":false");
    assert_contains(fixture_for(189), "\"evidenceDigest\":\"fnv1a64:a595f2d9cdcbac8d\"");
    assert_contains(fixture_for(190), "\"releaseVersion\":\"v190\"");
    assert_contains(fixture_for(190), "\"status\":\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_contains(fixture_for(190), "\"nodeRouteCatalogEvidenceChainAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_contains(fixture_for(190), "\"sourceFrozenReleaseVersion\":\"v189\"");
    assert_contains(fixture_for(190), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v189.json\"");
    assert_contains(fixture_for(190), "\"nodeCleanupCloseoutVersion\":\"Node v472\"");
    assert_contains(fixture_for(190), "\"nodeFullVitestTestCount\":1222");
    assert_contains(fixture_for(190), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v18\"");
    assert_contains(fixture_for(190), "\"fieldCount\":573");
    assert_contains(fixture_for(190), "\"groupCount\":29");
    assert_contains(fixture_for(190), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(190), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(190), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(190), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(190), "\"executionAllowed\":false");
    assert_contains(fixture_for(190), "\"evidenceDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_contains(fixture_for(191), "\"releaseVersion\":\"v191\"");
    assert_contains(fixture_for(191), "\"status\":\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_contains(fixture_for(191), "\"nodeRouteCatalogCleanupCloseoutHandoff\":{\"handoffMode\":"
                                           "\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_contains(fixture_for(191), "\"sourceFrozenReleaseVersion\":\"v190\"");
    assert_contains(fixture_for(191), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v190.json\"");
    assert_contains(fixture_for(191), "\"readyForDownstreamConsumption\":true");
    assert_contains(fixture_for(191), "\"handoffBoundaryCatalogVersion\":\"read-only-boundary-fields.v18\"");
    assert_contains(fixture_for(191), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v19\"");
    assert_contains(fixture_for(191), "\"fieldCount\":597");
    assert_contains(fixture_for(191), "\"groupCount\":30");
    assert_contains(fixture_for(191), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(191), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(191), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(191), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(191), "\"executionAllowed\":false");
    assert_contains(fixture_for(191), "\"evidenceDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_contains(fixture_for(192), "\"releaseVersion\":\"v192\"");
    assert_contains(fixture_for(192), "\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(fixture_for(192), "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_contains(fixture_for(192), "\"sourceFrozenReleaseVersion\":\"v191\"");
    assert_contains(fixture_for(192), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v191.json\"");
    assert_contains(fixture_for(192), "\"handoffIncludedSectionsComplete\":true");
    assert_contains(fixture_for(192), "\"rollingCurrentRejected\":true");
    assert_contains(fixture_for(192), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v20\"");
    assert_contains(fixture_for(192), "\"fieldCount\":625");
    assert_contains(fixture_for(192), "\"groupCount\":31");
    assert_contains(fixture_for(192), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(192), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(192), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(192), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(192), "\"executionAllowed\":false");
    assert_contains(fixture_for(192), "\"evidenceDigest\":\"fnv1a64:89bf4177b09f1ec0\"");
    assert_contains(fixture_for(193), "\"releaseVersion\":\"v193\"");
    assert_contains(fixture_for(193), "\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_contains(fixture_for(193), "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\":{\"freezeMode\":"
                                           "\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_contains(fixture_for(193), "\"frozenReleaseVersion\":\"v192\"");
    assert_contains(fixture_for(193), "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v192.json\"");
    assert_contains(fixture_for(193), "\"frozenEvidenceDigest\":\"fnv1a64:89bf4177b09f1ec0\"");
    assert_contains(fixture_for(193), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v21\"");
    assert_contains(fixture_for(193), "\"fieldCount\":648");
    assert_contains(fixture_for(193), "\"groupCount\":32");
    assert_contains(fixture_for(193), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(193), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(193), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(193), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(193), "\"executionAllowed\":false");
    assert_contains(fixture_for(193), "\"evidenceDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_contains(fixture_for(194), "\"releaseVersion\":\"v194\"");
    assert_contains(fixture_for(194), "\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_contains(fixture_for(194), "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\":{\"catalogMode\":"
                                           "\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_contains(fixture_for(194), "\"sourceFrozenReleaseVersion\":\"v193\"");
    assert_contains(fixture_for(194), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v193.json\"");
    assert_contains(fixture_for(194), "\"catalogedReleaseCount\":4");
    assert_contains(fixture_for(194), "\"latestCatalogedReleaseVersion\":\"v193\"");
    assert_contains(fixture_for(194), "\"latestCatalogedDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_contains(fixture_for(194), "\"allCatalogedEntriesReadOnly\":true");
    assert_contains(fixture_for(194), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v22\"");
    assert_contains(fixture_for(194), "\"fieldCount\":669");
    assert_contains(fixture_for(194), "\"groupCount\":33");
    assert_contains(fixture_for(194), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(194), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(194), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(194), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(194), "\"executionAllowed\":false");
    assert_contains(fixture_for(194), "\"evidenceDigest\":\"fnv1a64:626f5a2a96a980c2\"");
    assert_contains(fixture_for(195), "\"releaseVersion\":\"v195\"");
    assert_contains(fixture_for(195), "\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-audit-read-only\"");
    assert_contains(fixture_for(195), "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-cleanup-closeout-release-catalog-consistency-read-only\"");
    assert_contains(fixture_for(195), "\"sourceFrozenReleaseVersion\":\"v194\"");
    assert_contains(fixture_for(195), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v194.json\"");
    assert_contains(fixture_for(195), "\"sourceFrozenDigest\":\"fnv1a64:626f5a2a96a980c2\"");
    assert_contains(fixture_for(195), "\"observedCatalogedReleaseCount\":4");
    assert_contains(fixture_for(195), "\"releaseVersionsContiguous\":true");
    assert_contains(fixture_for(195), "\"latestDigestMatchesFrozenSource\":true");
    assert_contains(fixture_for(195), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v23\"");
    assert_contains(fixture_for(195), "\"fieldCount\":693");
    assert_contains(fixture_for(195), "\"groupCount\":34");
    assert_contains(fixture_for(195), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(195), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(195), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(195), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(195), "\"executionAllowed\":false");
    assert_contains(fixture_for(195), "\"evidenceDigest\":\"fnv1a64:36463a895469c4bc\"");
    assert_contains(fixture_for(196), "\"releaseVersion\":\"v196\"");
    assert_contains(fixture_for(196), "\"status\":\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_contains(fixture_for(196), "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\":{\"maintenanceMode\":"
                                           "\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_contains(fixture_for(196), "\"sourceFrozenReleaseVersion\":\"v195\"");
    assert_contains(fixture_for(196), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v195.json\"");
    assert_contains(fixture_for(196), "\"sourceFrozenDigest\":\"fnv1a64:36463a895469c4bc\"");
    assert_contains(fixture_for(196), "\"extractedCatalogFormatter\":true");
    assert_contains(fixture_for(196), "\"preservesReleaseCatalogAudit\":true");
    assert_contains(fixture_for(196), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v24\"");
    assert_contains(fixture_for(196), "\"fieldCount\":714");
    assert_contains(fixture_for(196), "\"groupCount\":35");
    assert_contains(fixture_for(196), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(196), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(196), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(196), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(196), "\"executionAllowed\":false");
    assert_contains(fixture_for(196), "\"evidenceDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(fixture_for(197), "\"releaseVersion\":\"v197\"");
    assert_contains(fixture_for(197), "\"status\":\"node-route-catalog-cleanup-latest-evidence-package-read-only\"");
    assert_contains(fixture_for(197), "\"nodeRouteCatalogCleanupLatestEvidencePackage\":{\"packageMode\":"
                                           "\"node-route-catalog-cleanup-latest-evidence-package-read-only\"");
    assert_contains(fixture_for(197), "\"sourceFrozenReleaseVersion\":\"v196\"");
    assert_contains(fixture_for(197), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v196.json\"");
    assert_contains(fixture_for(197), "\"sourceFrozenDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(fixture_for(197), "\"packageReleaseCount\":3");
    assert_contains(fixture_for(197), "\"latestPackagedReleaseVersion\":\"v196\"");
    assert_contains(fixture_for(197), "\"latestPackagedDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_contains(fixture_for(197), "\"packageReadyForArchiveVerification\":true");
    assert_contains(fixture_for(197), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v25\"");
    assert_contains(fixture_for(197), "\"fieldCount\":735");
    assert_contains(fixture_for(197), "\"groupCount\":36");
    assert_contains(fixture_for(197), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(197), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(197), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(197), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(197), "\"executionAllowed\":false");
    assert_contains(fixture_for(197), "\"evidenceDigest\":\"fnv1a64:836d9f0b3956fe30\"");
    assert_contains(fixture_for(198), "\"releaseVersion\":\"v198\"");
    assert_contains(fixture_for(198), "\"status\":\"node-route-catalog-cleanup-latest-evidence-package-audit-read-only\"");
    assert_contains(fixture_for(198), "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-cleanup-latest-evidence-package-consistency-read-only\"");
    assert_contains(fixture_for(198), "\"sourceFrozenReleaseVersion\":\"v197\"");
    assert_contains(fixture_for(198), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v197.json\"");
    assert_contains(fixture_for(198), "\"sourceFrozenDigest\":\"fnv1a64:836d9f0b3956fe30\"");
    assert_contains(fixture_for(198), "\"observedPackageReleaseCount\":3");
    assert_contains(fixture_for(198), "\"releaseVersionsContiguous\":true");
    assert_contains(fixture_for(198), "\"latestPackagedDigestMatchesPackageSource\":true");
    assert_contains(fixture_for(198), "\"packageReadyForArchiveVerification\":true");
    assert_contains(fixture_for(198), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v26\"");
    assert_contains(fixture_for(198), "\"fieldCount\":756");
    assert_contains(fixture_for(198), "\"groupCount\":37");
    assert_contains(fixture_for(198), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(198), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(198), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(198), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(198), "\"executionAllowed\":false");
    assert_contains(fixture_for(198), "\"evidenceDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(fixture_for(199), "\"releaseVersion\":\"v199\"");
    assert_contains(fixture_for(199), "\"status\":\"node-route-catalog-cleanup-evidence-batch-closeout-read-only\"");
    assert_contains(fixture_for(199), "\"nodeRouteCatalogCleanupEvidenceBatchCloseout\":{\"closeoutMode\":"
                                           "\"node-route-catalog-cleanup-evidence-batch-closeout-read-only\"");
    assert_contains(fixture_for(199), "\"sourceFrozenReleaseVersion\":\"v198\"");
    assert_contains(fixture_for(199), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_contains(fixture_for(199), "\"sourceFrozenDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(fixture_for(199), "\"miniKvCloseoutReleaseCount\":5");
    assert_contains(fixture_for(199), "\"latestCloseoutReleaseVersion\":\"v198\"");
    assert_contains(fixture_for(199), "\"latestCloseoutDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_contains(fixture_for(199), "\"closeoutReadyForNextBatch\":true");
    assert_contains(fixture_for(199), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v27\"");
    assert_contains(fixture_for(199), "\"fieldCount\":778");
    assert_contains(fixture_for(199), "\"groupCount\":38");
    assert_contains(fixture_for(199), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(199), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(199), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(199), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(199), "\"executionAllowed\":false");
    assert_contains(fixture_for(199), "\"evidenceDigest\":\"fnv1a64:3a5716f6f09c2b3b\"");
    assert_contains(fixture_for(200), "\"releaseVersion\":\"v200\"");
    assert_contains(fixture_for(200),
                    "\"status\":\"node-route-catalog-cleanup-evidence-batch-closeout-audit-read-only\"");
    assert_contains(fixture_for(200), "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit\":{\"auditMode\":"
                                           "\"node-route-catalog-cleanup-evidence-batch-closeout-consistency-read-only\"");
    assert_contains(fixture_for(200), "\"sourceFrozenReleaseVersion\":\"v199\"");
    assert_contains(fixture_for(200), "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v199.json\"");
    assert_contains(fixture_for(200), "\"sourceFrozenDigest\":\"fnv1a64:3a5716f6f09c2b3b\"");
    assert_contains(fixture_for(200), "\"observedCloseoutReleaseCount\":5");
    assert_contains(fixture_for(200), "\"releaseVersionsContiguous\":true");
    assert_contains(fixture_for(200), "\"latestCloseoutDigestMatchesCloseoutSource\":true");
    assert_contains(fixture_for(200), "\"closeoutReadyForNextBatch\":true");
    assert_contains(fixture_for(200), "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v28\"");
    assert_contains(fixture_for(200), "\"fieldCount\":800");
    assert_contains(fixture_for(200), "\"groupCount\":39");
    assert_contains(fixture_for(200), "\"runtimeExecutionPacketExecutable\":false");
    assert_contains(fixture_for(200), "\"startsMiniKvService\":false");
    assert_contains(fixture_for(200), "\"routerActivationAllowed\":false");
    assert_contains(fixture_for(200), "\"writeRoutingAllowed\":false");
    assert_contains(fixture_for(200), "\"executionAllowed\":false");
    assert_contains(fixture_for(200), "\"evidenceDigest\":\"fnv1a64:d1e889711b5d8574\"");

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
    assert_contains(result.response, "SHARDROUTE(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVERIFYREPORTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVERIFYREPORTARCHIVEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        result.response,
        "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    assert_contains(result.response, "\"name\":\"SHARDJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTE\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVERIFYJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTJSON\",\"category\":\"read\",\"mutates_store\":false,"
                    "\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                    "\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\",\"category\":\"read\","
                    "\"mutates_store\":false,\"touches_wal\":false");

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

    result = processor.execute("SHARDROUTE orderops:alpha");
    assert(result.response == "slot=5 shard=shard-0 route_mode=preview-only read_only=yes "
                              "router_installed=no write_routing_allowed=no execution_allowed=no");

    result = processor.execute("SHARDROUTEJSON orderops:alpha");
    assert_contains(result.response, "\"contract\":\"shard-route-preview.v1\"");
    assert_contains(result.response, "\"slot\":5");
    assert_contains(result.response, "\"pinnedFixtureSample\":true");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYJSON orderops:alpha");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(result.response, "\"checkedCommand\":\"SHARDROUTEJSON\"");
    assert_contains(result.response, "\"passed\":true");
    assert_contains(result.response, "\"executesRoute\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYREPORTJSON orderops:alpha");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification-report.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(result.response, "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(result.response, "\"verificationPassed\":true");
    assert_contains(result.response, "\"executesRoute\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVEJSON");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification-report-archive.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(result.response, "\"archiveStage\":\"route-preview-verification-report-archive-closeout-audit\"");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification-report-archive-closeout.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(result.response,
                    "\"closeoutStage\":\"route-preview-verification-report-archive-closeout-explain-contract\"");
    assert_contains(result.response, "\"sourceArchiveChainComplete\":true");
    assert_contains(result.response, "\"archiveCloseoutChainComplete\":false");
    assert_contains(result.response, "\"executionAllowed\":false");
}
