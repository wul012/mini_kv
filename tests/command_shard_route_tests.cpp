#include "command_test_suites.hpp"

namespace minikv::command_test {

void run_command_shard_route_tests(CommandFixture& fixture) {
    auto& processor = fixture.processor;
    auto& result = fixture.result;

    result = processor.execute("SHARDJSON");
    assert_response_contains(result, "\"contract\":\"shard-readiness.v1\"");
    assert_response_contains(result, "\"project\":\"mini-kv\"");
    assert_response_contains(result, "\"version\":\"" + std::string{minikv::version} + "\",\"releaseVersion\":\"v");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardEnabled\":false");
    assert_response_contains(result, "\"shardCount\":1");
    assert_response_contains(result, "\"slotCount\":16");
    assert_response_contains(result, "\"routingMode\":\"single-shard-readiness-prototype\"");
    assert_response_contains(result, "\"evidencePath\":\"fixtures/release/shard-readiness.json\"");
    assert_response_contains(result,
                             "\"status\":\"f-folder-explanation-quality-closeout-single-large-version-read-only\"");
    assert_response_contains(result, "\"slotTablePreview\":{\"previewMode\":\"single-shard-slot-table-read-only\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/"
                "v425-post-credential-resolver-disabled-runtime-shell-readiness-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v161\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v161.json\"");
    assert_response_contains(result, "\"slotTableMaterializedForEvidence\":true");
    assert_response_contains(result, "\"slotTablePreviewOnly\":true");
    assert_response_contains(result, "\"readOnlyShardMapPreview\":true");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"writeCommandsAllowed\":false");
    assert_response_contains(result, "\"adminCommandsAllowed\":false");
    assert_response_contains(result, "\"loadRestoreCompactAllowed\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"liveReadAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"slotAssignments\":[{\"slot\":0,\"shardId\":\"shard-0\","
                                     "\"routeMode\":\"preview-only\",\"readOnly\":true,\"writesAllowed\":false,"
                                     "\"routerInstalled\":false}");
    assert_response_contains(result,
                             "\"slotTablePreviewAudit\":{\"auditMode\":\"slot-table-preview-consistency-read-only\"");
    assert_response_contains(result,
                             "\"sourceNodePlan\":\"docs/plans3/"
                             "v426-post-credential-resolver-runtime-shell-decision-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v162\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\"");
    assert_response_contains(result, "\"auditedPreviewSection\":\"slotTablePreview\"");
    assert_response_contains(result, "\"expectedSlotCount\":16");
    assert_response_contains(result, "\"observedSlotCount\":16");
    assert_response_contains(result, "\"contiguousSlotRange\":true");
    assert_response_contains(result, "\"duplicateSlotsDetected\":false");
    assert_response_contains(result, "\"unassignedSlotsDetected\":false");
    assert_response_contains(result, "\"allSlotsMapped\":true");
    assert_response_contains(result, "\"allAssignmentsReadOnly\":true");
    assert_response_contains(result, "\"allAssignmentsPreviewOnly\":true");
    assert_response_contains(result, "\"derivedFromSlotPreviewModule\":true");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance\":{\"maintenanceMode\":"
                                     "\"slot-table-preview-audit-formatter-split-contract-preserving\"");
    assert_response_contains(
        result, "\"sourceNodePlan\":\"docs/plans3/"
                "v430-post-credential-resolver-signed-human-approval-artifact-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v163\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v163.json\"");
    assert_response_contains(result, "\"auditFormatterExtracted\":true");
    assert_response_contains(result, "\"slotPreviewModuleOwnsAssignments\":true");
    assert_response_contains(result, "\"slotPreviewAuditStillPublished\":true");
    assert_response_contains(result, "\"slotTablePreviewStillPublished\":true");
    assert_response_contains(result, "\"publicShardJsonContractChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result,
                             "\"shardRoutePreview\":{\"rolloutMode\":\"read-only-shard-route-preview-rollout\"");
    assert_response_contains(result, "\"rolloutStage\":\"route-preview-release-package-audit\"");
    assert_response_contains(result, "\"rolloutStageSequence\":20");
    assert_response_contains(result, "\"rolloutReleaseVersion\":\"v298\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"commands\":[\"SHARDROUTE\",\"SHARDROUTEJSON\"]");
    assert_response_contains(result, "\"sampleSlotsPinnedToExistingEvidence\":true");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerification\":{\"rolloutMode\":"
                                     "\"read-only-shard-route-preview-verification-rollout\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v318\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v318.json\"");
    assert_response_contains(result, "\"rolloutStage\":\"route-preview-verification-closeout-audit\"");
    assert_response_contains(result, "\"rolloutStageSequence\":21");
    assert_response_contains(result, "\"rolloutReleaseVersion\":\"v319\"");
    assert_response_contains(result, "\"publishedStageCount\":21");
    assert_response_contains(result, "\"verificationCommandAvailable\":true");
    assert_response_contains(result, "\"verifiedCommand\":\"SHARDROUTEJSON\"");
    assert_response_contains(result, "\"commands\":[\"SHARDROUTEVERIFYJSON\"]");
    assert_response_contains(result, "\"executesRoute\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerificationReport\":{\"rolloutMode\":"
                                     "\"read-only-shard-route-preview-verification-report-rollout\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v338\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v338.json\"");
    assert_response_contains(result, "\"rolloutStage\":\"route-preview-verification-report-closeout-audit\"");
    assert_response_contains(result, "\"rolloutStageSequence\":20");
    assert_response_contains(result, "\"rolloutReleaseVersion\":\"v339\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"reportCommandAvailable\":true");
    assert_response_contains(result, "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_response_contains(result, "\"previewCommand\":\"SHARDROUTEJSON\"");
    assert_response_contains(result, "\"commands\":[\"SHARDROUTEVERIFYREPORTJSON\"]");
    assert_response_contains(result, "\"executesRoute\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerificationReportCloseout\":{\"contract\":"
                                     "\"shard-route-preview-verification-report-closeout.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_response_contains(result,
                             "\"closeoutMode\":\"read-only-route-preview-verification-report-closeout-summary\"");
    assert_response_contains(result, "\"closedReleaseRangeStart\":\"v320\"");
    assert_response_contains(result, "\"closedReleaseRangeEnd\":\"v339\"");
    assert_response_contains(result, "\"closedVersionCount\":20");
    assert_response_contains(result, "\"reportStageCount\":20");
    assert_response_contains(result, "\"allReportStagesPublished\":true");
    assert_response_contains(result, "\"archiveManifestPlannedReleaseVersion\":\"v341\"");
    assert_response_contains(result, "\"archiveVerificationPlannedReleaseVersion\":\"v342\"");
    assert_response_contains(result, "\"executesRoute\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerificationReportArchive\":{\"contract\":"
                                     "\"shard-route-preview-verification-report-archive.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_response_contains(result, "\"archiveMode\":\"read-only-route-preview-verification-report-archive-chain\"");
    assert_response_contains(result, "\"sourceCloseoutReleaseVersion\":\"v340\"");
    assert_response_contains(result, "\"archiveStage\":\"route-preview-verification-report-archive-closeout-audit\"");
    assert_response_contains(result, "\"archiveStageSequence\":20");
    assert_response_contains(result, "\"archiveReleaseVersion\":\"v360\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"plannedStageCount\":20");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v359\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v359.json\"");
    assert_response_contains(result, "\"archiveManifestPublished\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerificationReportArchiveCloseout\":{\"contract\":"
                                     "\"shard-route-preview-verification-report-archive-closeout.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_response_contains(result, "\"closeoutMode\":"
                                     "\"read-only-route-preview-verification-report-archive-closeout-chain\"");
    assert_response_contains(result, "\"sourceArchiveCommand\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_response_contains(result, "\"sourceArchiveReleaseVersion\":\"v360\"");
    assert_response_contains(result, "\"sourceArchiveFixturePath\":\"fixtures/release/shard-readiness-v360.json\"");
    assert_response_contains(result, "\"sourceArchivePublishedStageCount\":20");
    assert_response_contains(result, "\"sourceArchiveChainComplete\":true");
    assert_response_contains(
        result, "\"closeoutStage\":\"route-preview-verification-report-archive-closeout-release-package\"");
    assert_response_contains(result, "\"closeoutStageSequence\":20");
    assert_response_contains(result, "\"closeoutReleaseVersion\":\"v380\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"plannedStageCount\":20");
    // Archive closeout verification rollout assertions updated per mini-kv release.
    assert_response_contains(result, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerification\":{\"contract\":"
                                     "\"shard-route-preview-verification-report-archive-closeout-verification.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_response_contains(
        result, "\"verificationMode\":\"read-only-route-preview-verification-report-archive-closeout-verification\"");
    assert_response_contains(result, "\"sourceArchiveCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_response_contains(result, "\"sourceArchiveCloseoutReleaseVersion\":\"v380\"");
    assert_response_contains(result,
                             "\"sourceArchiveCloseoutFixturePath\":\"fixtures/release/shard-readiness-v380.json\"");
    assert_response_contains(result, "\"sourceArchiveCloseoutPublishedStageCount\":20");
    assert_response_contains(result, "\"sourceArchiveCloseoutChainComplete\":true");
    assert_response_contains(
        result,
        "\"verificationStage\":\"route-preview-verification-report-archive-closeout-verification-release-package\"");
    assert_response_contains(result, "\"verificationStageSequence\":20");
    assert_response_contains(result, "\"verificationReleaseVersion\":\"v400\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"plannedStageCount\":20");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v399\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v399.json\"");
    assert_response_contains(result, "\"verifiedCloseoutReleaseRangeStart\":\"v361\"");
    assert_response_contains(result, "\"verifiedCloseoutReleaseRangeEnd\":\"v380\"");
    assert_response_contains(result,
                             "\"sourceCloseoutVersionedFixturePath\":\"fixtures/release/shard-readiness-v380.json\"");
    assert_response_contains(result, "\"archiveCloseoutVerificationCommandAvailable\":true");
    assert_response_contains(result, "\"archiveCloseoutVerificationChainComplete\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    // End archive closeout verification rollout assertions.
    // Archive closeout verification audit rollout assertions updated per mini-kv release.
    assert_response_contains(result,
                             "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAudit\":{\"contract\":"
                             "\"shard-route-preview-verification-report-archive-closeout-verification-audit.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_response_contains(
        result, "\"auditMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit\"");
    assert_response_contains(result,
                             "\"sourceVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_response_contains(result, "\"sourceVerificationReleaseVersion\":\"v400\"");
    assert_response_contains(result,
                             "\"sourceVerificationFixturePath\":\"fixtures/release/shard-readiness-v400.json\"");
    assert_response_contains(result, "\"sourceVerificationPublishedStageCount\":20");
    assert_response_contains(result, "\"sourceVerificationChainComplete\":true");
    assert_response_contains(
        result,
        "\"auditStage\":\"route-preview-verification-report-archive-closeout-verification-audit-release-package\"");
    assert_response_contains(result, "\"auditStageSequence\":20");
    assert_response_contains(result, "\"auditReleaseVersion\":\"v420\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"plannedStageCount\":20");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v419\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v419.json\"");
    assert_response_contains(result, "\"auditedVerificationReleaseRangeStart\":\"v381\"");
    assert_response_contains(result, "\"auditedVerificationReleaseRangeEnd\":\"v400\"");
    assert_response_contains(
        result, "\"sourceVerificationVersionedFixturePath\":\"fixtures/release/shard-readiness-v400.json\"");
    assert_response_contains(result, "\"singleVerificationSectionExpected\":true");
    assert_response_contains(result, "\"singleAuditSectionExpected\":true");
    assert_response_contains(result, "\"duplicateVerificationSectionRemovedInCurrent\":true");
    assert_response_contains(result, "\"archiveCloseoutVerificationAuditCommandAvailable\":true");
    assert_response_contains(result, "\"archiveCloseoutVerificationAuditChainComplete\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    // End archive closeout verification audit rollout assertions.
    // Archive closeout verification audit closeout assertions stay stable across the v421-v440 rollout.
    assert_response_contains(
        result, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseout\":{\"contract\":"
                "\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_response_contains(result, "\"closeoutMode\":\"read-only-route-preview-verification-report-archive-closeout-"
                                     "verification-audit-closeout\"");
    assert_response_contains(result, "\"sourceAuditCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_response_contains(result, "\"sourceAuditReleaseVersion\":\"v420\"");
    assert_response_contains(result, "\"sourceAuditPublishedStageCount\":20");
    assert_response_contains(result, "\"sourceAuditChainComplete\":true");
    assert_response_contains(result, "\"sharedStageCatalogFormatterApplied\":true");
    assert_response_contains(result, "\"archiveCloseoutVerificationAuditCloseoutCommandAvailable\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    // End archive closeout verification audit closeout assertions.
    // Archive closeout verification audit closeout archive verification assertions updated per mini-kv release.
    assert_response_contains(result, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseoutArch"
                                     "iveVerification\":{\"contract\":"
                                     "\"shard-route-preview-verification-report-archive-closeout-verification-audit-"
                                     "closeout-archive-verification.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\"");
    assert_response_contains(
        result, "\"sourceAuditCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_response_contains(result, "\"sourceAuditCloseoutReleaseVersion\":\"v440\"");
    assert_response_contains(result, "\"sourceAuditCloseoutPublishedStageCount\":20");
    assert_response_contains(result, "\"sourceAuditCloseoutChainComplete\":true");
    assert_response_contains(result, "\"sharedStageCatalogFormatterApplied\":true");
    assert_response_contains(result, "\"declaredArchiveVerificationOnly\":true");
    assert_response_contains(result, "\"archiveRootHintUsedForHumanReviewOnly\":true");
    assert_response_contains(result,
                             "\"archiveCloseoutVerificationAuditCloseoutArchiveVerificationCommandAvailable\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    // End archive closeout verification audit closeout archive verification assertions.
    assert_response_contains(result, "\"shardRoutePreviewArchiveMaintenance\":{\"contract\":"
                                     "\"shard-route-preview-archive-maintenance.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEARCHIVEMAINTJSON\"");
    assert_response_contains(result, "\"maintenanceMode\":\"read-only-route-preview-archive-maintenance-ledger\"");
    assert_response_contains(result, "\"sourceArchiveVerificationReleaseVersion\":\"v460\"");
    assert_response_contains(result, "\"sourceArchiveVerificationPublishedStageCount\":20");
    assert_response_contains(result, "\"sourceArchiveVerificationChainComplete\":true");
    assert_response_contains(result, "\"maintenanceStage\":\"route-preview-archive-maintenance-release-package\"");
    assert_response_contains(result, "\"maintenanceStageSequence\":25");
    assert_response_contains(result, "\"maintenanceReleaseVersion\":\"v485\"");
    assert_response_contains(result, "\"sharedStageCatalogFormatterApplied\":true");
    assert_response_contains(result, "\"focusedTestHelperApplied\":true");
    assert_response_contains(result, "\"archiveMaintenanceCommandAvailable\":true");
    assert_response_contains(result, "\"archiveMaintenanceChainComplete\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewArchiveMaintenanceVerification\":{\"contract\":"
                                     "\"shard-route-preview-archive-maintenance-verification.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\"");
    assert_response_contains(result,
                             "\"verificationMode\":\"read-only-route-preview-archive-maintenance-chain-verification\"");
    assert_response_contains(result, "\"sourceMaintenanceReleaseVersion\":\"v485\"");
    assert_response_contains(result, "\"sourceMaintenancePublishedStageCount\":25");
    assert_response_contains(result, "\"sourceMaintenanceChainComplete\":true");
    assert_response_contains(result, "\"stageChainHelperApplied\":true");
    assert_response_contains(result, "\"archiveMaintenanceVerificationCommandAvailable\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewWorksheetVerification\":{\"contract\":"
                                     "\"shard-route-preview-worksheet-verification.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEWORKSHEETVERIFYJSON\"");
    assert_response_contains(result,
                             "\"verificationMode\":\"read-only-shard-preview-worksheet-boundary-verification\"");
    assert_response_contains(result, "\"sourceVerificationCommand\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\"");
    assert_response_contains(result, "\"sourceVerificationReleaseVersion\":\"v510\"");
    assert_response_contains(result, "\"sourceVerificationChainComplete\":true");
    assert_response_contains(result, "\"readyForOperatorEntryWorksheet\":true");
    assert_response_contains(result, "\"readyForManualEvidenceEntry\":false");
    assert_response_contains(result, "\"operatorEntryValuesAccepted\":false");
    assert_response_contains(result, "\"manualEvidenceImportAllowed\":false");
    assert_response_contains(result, "\"runtimePayloadAccepted\":false");
    assert_response_contains(result, "\"syntheticEvidenceAccepted\":false");
    assert_response_contains(result, "\"singleWorksheetVerifierSectionExpected\":true");
    assert_response_contains(result, "\"worksheetVerificationChainComplete\":");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewOperatorImportPreflight\":{\"contract\":"
                                     "\"shard-route-preview-operator-import-preflight.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEIMPORTPREFLIGHTJSON\"");
    assert_response_contains(result, "\"preflightMode\":\"controlled-read-only-operator-evidence-import-preflight\"");
    assert_response_contains(result, "\"sourceWorksheetCommand\":\"SHARDROUTEWORKSHEETVERIFYJSON\"");
    assert_response_contains(result, "\"sourceWorksheetReleaseVersion\":\"v535\"");
    assert_response_contains(result, "\"sourceWorksheetChainComplete\":true");
    assert_response_contains(result, "\"readyForOperatorEvidenceImportPreflight\":true");
    assert_response_contains(result, "\"readyForEvidenceImport\":false");
    assert_response_contains(result, "\"operatorEvidenceValuesAccepted\":false");
    assert_response_contains(result, "\"evidenceImportAllowed\":false");
    assert_response_contains(result, "\"runtimePayloadAccepted\":false");
    assert_response_contains(result, "\"syntheticEvidenceAccepted\":false");
    assert_response_contains(result, "\"normalizerExecutes\":false");
    assert_response_contains(result, "\"normalizationExecutionAllowed\":false");
    assert_response_contains(result, "\"missingValuePolicyBlocksImport\":true");
    assert_response_contains(result, "\"importBlockersEnforcedAsMetadata\":true");
    assert_response_contains(result, "\"singleImportPreflightSectionExpected\":true");
    assert_response_contains(result, "\"declaredImportPreflightOnly\":true");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewOperatorValueDraft\":{\"contract\":"
                                     "\"shard-route-preview-operator-value-draft.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUEDRAFTJSON\"");
    assert_response_contains(result, "\"draftMode\":\"controlled-read-only-operator-evidence-value-draft\"");
    assert_response_contains(result, "\"sourceImportPreflightCommand\":\"SHARDROUTEIMPORTPREFLIGHTJSON\"");
    assert_response_contains(result, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
    assert_response_contains(result, "\"sourceImportPreflightPublishedStageCount\":25");
    assert_response_contains(result, "\"sourceImportPreflightChainComplete\":true");
    assert_response_contains(result, "\"sourceImportPreflightReleaseRangeStart\":\"v536\"");
    assert_response_contains(result, "\"sourceImportPreflightReleaseRangeEnd\":\"v560\"");
    assert_response_contains(result, "\"operatorValueDraftSlotCount\":25");
    assert_response_contains(result, "\"actualValueState\":\"not-supplied\"");
    assert_response_contains(result, "\"draftValueState\":\"awaiting-operator-value\"");
    assert_response_contains(result, "\"readyForOperatorEvidenceValueDraft\":true");
    assert_response_contains(result, "\"operatorValueSupplied\":false");
    assert_response_contains(result, "\"operatorValueAccepted\":false");
    assert_response_contains(result, "\"operatorValueImported\":false");
    assert_response_contains(result, "\"valueSupplyAdapterInstalled\":false");
    assert_response_contains(result, "\"valueSupplyAdapterEnabled\":false");
    assert_response_contains(result, "\"draftValuesPersisted\":false");
    assert_response_contains(result, "\"valueDraftPackageOnly\":true");
    assert_response_contains(result, "\"valueDraftSlotHelperApplied\":true");
    assert_response_contains(result, "\"singleValueDraftSectionExpected\":true");
    assert_response_contains(result, "\"shardRoutePreviewOperatorValueSupplyEnvelope\":{\"contract\":"
                                     "\"shard-route-preview-operator-value-supply-envelope.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYJSON\"");
    assert_response_contains(result, "\"shardRoutePreviewOperatorValueSupplyPrecheck\":{\"contract\":"
                                     "\"shard-route-preview-operator-value-supply-precheck.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\"");
    assert_response_contains(result, "\"shardRoutePreviewOperatorValueSupplyApprovalTemplate\":{\"contract\":"
                                     "\"shard-route-preview-operator-value-supply-approval-template.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\"");
    assert_response_contains(result, "\"shardRoutePreviewOperatorValueSupplySignedApprovalTemplate\":{\"contract\":"
                                     "\"shard-route-preview-operator-value-supply-signed-approval-template.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\"");
    assert_response_contains(result, "\"signedApprovalTemplateDeclared\":true");
    assert_response_contains(result, "\"signedApprovalTemplateOnly\":true");
    assert_response_contains(result, "\"signedApprovalPresent\":false");
    assert_response_contains(result, "\"signatureCaptured\":false");
    assert_response_contains(result,
                             "\"shardRoutePreviewOperatorValueSupplySignedApprovalCapturePreflight\":{\"contract\":"
                             "\"shard-route-preview-operator-value-supply-signed-approval-capture-preflight.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\"");
    assert_response_contains(result, "\"signedApprovalCapturePreflightDeclared\":true");
    assert_response_contains(result, "\"signedApprovalCapturePreflightOnly\":true");
    assert_response_contains(result, "\"capturePreflightOnly\":true");
    assert_response_contains(result, "\"approvalGrantEmitted\":false");
    assert_response_contains(
        result, "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactPreflight\":{\"contract\":"
                "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-preflight.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\"");
    assert_response_contains(result, "\"signedApprovalCaptureArtifactPreflightDeclared\":true");
    assert_response_contains(result, "\"signedApprovalCaptureArtifactPreflightOnly\":true");
    assert_response_contains(result, "\"captureArtifactPreflightOnly\":true");
    assert_response_contains(result, "\"signedApprovalArtifactDraftPresent\":false");
    assert_response_contains(result, "\"signedApprovalArtifactMaterialized\":false");
    assert_response_contains(
        result,
        "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftAuthoringReadiness\":{\"contract\":"
        "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-authoring-readiness.v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\"");
    assert_response_contains(result, "\"signedApprovalCaptureArtifactDraftAuthoringReadinessDeclared\":true");
    assert_response_contains(result, "\"signedApprovalCaptureArtifactDraftAuthoringReadinessOnly\":true");
    assert_response_contains(result, "\"authoringReadinessOnly\":true");
    assert_response_contains(result, "\"signedApprovalArtifactDraftTextGenerated\":false");
    assert_response_contains(result, "\"approvalStatementTextGenerated\":false");
    assert_response_contains(
        result,
        "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftInstructionPreflight\":{\"contract\":"
        "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-instruction-preflight."
        "v1\"");
    assert_response_contains(
        result, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\"");
    assert_response_contains(result, "\"sourceDraftAuthoringReadinessReleaseVersion\":\"v760\"");
    assert_response_contains(result, "\"sourceDraftAuthoringReadinessPublishedStageCount\":25");
    assert_response_contains(result, "\"sourceDraftAuthoringReadinessChainComplete\":true");
    assert_response_contains(result, "\"instructionPreflightReleaseVersion\":\"v785\"");
    assert_response_contains(result, "\"instructionSlotCount\":25");
    assert_response_contains(result, "\"instructionGuardCount\":25");
    assert_response_contains(result, "\"instructionProfileCount\":25");
    assert_response_contains(result, "\"signedApprovalCaptureArtifactDraftInstructionPreflightDeclared\":true");
    assert_response_contains(result, "\"draftInstructionPreflightOnly\":true");
    assert_response_contains(result, "\"draftInstructionTextGenerated\":false");
    assert_response_contains(result, "\"draftInstructionTextMaterialized\":false");
    assert_response_contains(result, "\"materializedInstructionTextCount\":0");
    assert_response_contains(result, "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPacka"
                                     "geReviewPreflight\":{\"contract\":"
                                     "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-"
                                     "draft-text-package-review-preflight.v1\"");
    assert_response_contains(
        result,
        "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\"");
    assert_response_contains(result, "\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\"");
    assert_response_contains(result, "\"draftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_response_contains(result, "\"draftTextPackageReviewPreflightOnly\":true");
    assert_response_contains(result, "\"draftTextPackageParsed\":false");
    assert_response_contains(result, "\"draftTextPackageAccepted\":false");
    assert_response_contains(result, "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPacka"
                                     "geReviewCloseoutAudit\":{\"contract\":"
                                     "\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-"
                                     "draft-text-package-review-closeout-audit.v1\"");
    assert_response_contains(
        result,
        "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\"");
    assert_response_contains(result, "\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_response_contains(result, "\"sourceDraftTextPackageReviewPreflightPublishedStageCount\":25");
    assert_response_contains(result, "\"sourceDraftTextPackageReviewPreflightChainComplete\":true");
    assert_response_contains(result, "\"draftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_response_contains(result, "\"draftTextPackageReviewCloseoutAuditOnly\":true");

    result = processor.execute(
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-"
                                     "artifact-draft-text-package-comparison-closeout-audit.v1\"");
    assert_response_contains(
        result, "\"command\":"
                "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON\"");
    assert_response_contains(result, "\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_response_contains(
        result,
        "\"sourceDraftTextPackageReviewCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v835.json\"");
    assert_response_contains(result, "\"draftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v859\"");
    assert_response_contains(result, "\"publishedStageCount\":25");
    assert_response_contains(result, "\"draftTextPackageComparisonCloseoutAuditOnly\":true");
    assert_response_contains(result, "\"submittedDraftTextPackageParsed\":false");
    assert_response_contains(result, "\"detachedSignaturePayloadParsed\":false");
    assert_response_contains(result, "\"uncomparedPackageAccepted\":false");
    assert_response_contains(result, "\"unacceptablePackageAccepted\":false");
    assert_response_contains(result, "\"readyForDraftTextPackageComparison\":false");
    assert_response_contains(result, "\"auditReadsRuntimeArchive\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute(
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-"
                                     "artifact-draft-text-package-compared-package-evidence-intake-audit.v1\"");
    assert_response_contains(
        result,
        "\"command\":"
        "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\"");
    assert_response_contains(result, "\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_response_contains(result, "\"draftTextPackageComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v879\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"comparedPackageEvidenceIntakeAuditOnly\":true");
    assert_response_contains(result, "\"comparedPackageEvidencePresent\":false");
    assert_response_contains(result, "\"syntheticComparedPackageEvidenceAccepted\":false");
    assert_response_contains(result, "\"approvalGrantCaptured\":false");
    assert_response_contains(result, "\"runtimePayloadImported\":false");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQU"
                               "ESTPACKAGECLOSEOUTJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-candidate-document-request-package-closeout.v1\"");
    assert_response_contains(result, "\"command\":"
                                     "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCU"
                                     "MENTREQUESTPACKAGECLOSEOUTJSON\"");
    assert_response_contains(result, "\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_response_contains(
        result,
        "\"sourceComparedPackageEvidenceIntakeAuditFixturePath\":\"fixtures/release/shard-readiness-v880.json\"");
    assert_response_contains(result, "\"candidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v894\"");
    assert_response_contains(result, "\"publishedStageCount\":15");
    assert_response_contains(result, "\"plannedRequestItemCount\":15");
    assert_response_contains(result, "\"plannedAcceptanceCheckCount\":15");
    assert_response_contains(result, "\"plannedRequestGateCount\":38");
    assert_response_contains(result, "\"requestedCandidateFieldCount\":20");
    assert_response_contains(result, "\"candidateDocumentRequestPackageCloseoutOnly\":true");
    assert_response_contains(result, "\"realCandidateDocumentPresent\":false");
    assert_response_contains(result, "\"syntheticCandidateDocumentAccepted\":false");
    assert_response_contains(result, "\"stagedCandidatePayloadImported\":false");
    assert_response_contains(result, "\"candidateDocumentEvaluated\":false");
    assert_response_contains(result, "\"candidateDocumentAccepted\":false");
    assert_response_contains(result, "\"candidateApprovalGrantCaptured\":false");
    assert_response_contains(result, "\"candidateSignedApprovalEmitted\":false");
    assert_response_contains(result, "\"runtimePayloadImported\":false");
    assert_response_contains(result, "\"candidateDocumentRequestPackageValidationPassed\":true");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON");
    assert_response_contains(result,
                             "\"contract\":\"shard-route-preview-candidate-request-package-fixture-integrity.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON\"");
    assert_response_contains(result, "\"sourceCandidateDocumentRequestPackageReleaseVersion\":\"v895\"");
    assert_response_contains(
        result, "\"sourceCandidateDocumentRequestPackageFixturePath\":\"fixtures/release/shard-readiness-v895.json\"");
    assert_response_contains(result, "\"candidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v919\"");
    assert_response_contains(result, "\"publishedStageCount\":25");
    assert_response_contains(result, "\"plannedIntegrityCheckCount\":25");
    assert_response_contains(result, "\"candidateRequestPackageIntegrityOnly\":true");
    assert_response_contains(result, "\"additionalRequestEchoCreated\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON");

    result = processor.execute("SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON");
    assert_response_contains(result, "\"contract\":\"shard-route-preview-candidate-document-submission-precheck.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON\"");
    assert_response_contains(result, "\"sourceNodeSubmissionPrecheckReleaseVersion\":\"Node v1411\"");
    assert_response_contains(result, "\"sourceCandidateRequestPackageIntegrityReleaseVersion\":\"v920\"");
    assert_response_contains(
        result, "\"sourceCandidateRequestPackageIntegrityFixturePath\":\"fixtures/release/shard-readiness-v920.json\"");
    assert_response_contains(result, "\"candidateSubmissionPrecheckReleaseVersion\":\"v930\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v929\"");
    assert_response_contains(result, "\"publishedStageCount\":10");
    assert_response_contains(result, "\"plannedSubmissionPrecheckCheckpointCount\":10");
    assert_response_contains(result, "\"plannedSubmissionPrecheckValidatorCount\":10");
    assert_response_contains(result, "\"candidateSubmissionPrecheckOnly\":true");
    assert_response_contains(result, "\"additionalSubmissionPrecheckEchoCreated\":false");
    assert_response_contains(result, "\"documentIntakeOpened\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"touchesWal\":false");
    assert_response_contains(result, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON extra");
    assert(result.response == "ERR usage: SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON");
}

} // namespace minikv::command_test
