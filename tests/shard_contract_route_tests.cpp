#include "shard_test_suites.hpp"

namespace minikv::shard_test {

void check_shard_route_contract(ShardFixture& fixture) {
    const auto& json = *fixture.active_json;
    assert_contains(json, "\"slotTablePreview\":{\"previewMode\":\"single-shard-slot-table-read-only\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
                    "v425-post-credential-resolver-disabled-runtime-shell-readiness-route-group-split-roadmap.md\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v161\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v879.json\"");
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
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
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
    assert_contains(json, "\"publishedStageCount\":20");
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
    assert_contains(json, "\"publishedStageCount\":21");
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
    assert_contains(json, "\"publishedStageCount\":20");
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
    assert_contains(json, "\"publishedStageCount\":20");
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
    assert_contains(json, "\"closeoutStage\":\"route-preview-verification-report-archive-closeout-release-package\"");
    assert_contains(json, "\"closeoutStageSequence\":20");
    assert_contains(json, "\"closeoutReleaseVersion\":\"v380\"");
    assert_contains(json, "\"publishedStageCount\":20");
    assert_contains(json, "\"plannedStageCount\":20");
    // Archive closeout verification rollout assertions updated per mini-kv release.
    assert_contains(json, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerification\":{\"contract\":"
                          "\"shard-route-preview-verification-report-archive-closeout-verification.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_contains(
        json, "\"verificationMode\":\"read-only-route-preview-verification-report-archive-closeout-verification\"");
    assert_contains(json, "\"sourceArchiveCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(json, "\"sourceArchiveCloseoutReleaseVersion\":\"v380\"");
    assert_contains(json, "\"sourceArchiveCloseoutFixturePath\":\"fixtures/release/shard-readiness-v380.json\"");
    assert_contains(json, "\"sourceArchiveCloseoutPublishedStageCount\":20");
    assert_contains(json, "\"sourceArchiveCloseoutChainComplete\":true");
    assert_contains(
        json,
        "\"verificationStage\":\"route-preview-verification-report-archive-closeout-verification-release-package\"");
    assert_contains(json, "\"verificationStageSequence\":20");
    assert_contains(json, "\"verificationReleaseVersion\":\"v400\"");
    assert_contains(json, "\"publishedStageCount\":20");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v399\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v399.json\"");
    assert_contains(json, "\"verifiedCloseoutReleaseRangeStart\":\"v361\"");
    assert_contains(json, "\"verifiedCloseoutReleaseRangeEnd\":\"v380\"");
    assert_contains(json, "\"sourceCloseoutVersionedFixturePath\":\"fixtures/release/shard-readiness-v380.json\"");
    assert_contains(json, "\"archiveCloseoutVerificationCommandAvailable\":true");
    assert_contains(json, "\"archiveCloseoutVerificationChainComplete\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    // End archive closeout verification rollout assertions.
    // Archive closeout verification audit rollout assertions updated per mini-kv release.
    assert_contains(json, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAudit\":{\"contract\":"
                          "\"shard-route-preview-verification-report-archive-closeout-verification-audit.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(
        json, "\"auditMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit\"");
    assert_contains(json, "\"sourceVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_contains(json, "\"sourceVerificationReleaseVersion\":\"v400\"");
    assert_contains(json, "\"sourceVerificationFixturePath\":\"fixtures/release/shard-readiness-v400.json\"");
    assert_contains(json, "\"sourceVerificationPublishedStageCount\":20");
    assert_contains(json, "\"sourceVerificationChainComplete\":true");
    assert_contains(
        json,
        "\"auditStage\":\"route-preview-verification-report-archive-closeout-verification-audit-release-package\"");
    assert_contains(json, "\"auditStageSequence\":20");
    assert_contains(json, "\"auditReleaseVersion\":\"v420\"");
    assert_contains(json, "\"publishedStageCount\":20");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v419\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v419.json\"");
    assert_contains(json, "\"auditedVerificationReleaseRangeStart\":\"v381\"");
    assert_contains(json, "\"auditedVerificationReleaseRangeEnd\":\"v400\"");
    assert_contains(json, "\"sourceVerificationVersionedFixturePath\":\"fixtures/release/shard-readiness-v400.json\"");
    assert_contains(json, "\"singleVerificationSectionExpected\":true");
    assert_contains(json, "\"singleAuditSectionExpected\":true");
    assert_contains(json, "\"duplicateVerificationSectionRemovedInCurrent\":true");
    assert_contains(json, "\"archiveCloseoutVerificationAuditCommandAvailable\":true");
    assert_contains(json, "\"archiveCloseoutVerificationAuditChainComplete\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    // End archive closeout verification audit rollout assertions.
    // Archive closeout verification audit closeout assertions stay stable across the v421-v440 rollout.
    assert_contains(json,
                    "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseout\":{\"contract\":"
                    "\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(json, "\"closeoutMode\":\"read-only-route-preview-verification-report-archive-closeout-"
                          "verification-audit-closeout\"");
    assert_contains(json, "\"sourceAuditCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(json, "\"sourceAuditReleaseVersion\":\"v420\"");
    assert_contains(json, "\"sourceAuditPublishedStageCount\":20");
    assert_contains(json, "\"sourceAuditChainComplete\":true");
    assert_contains(json, "\"sharedStageCatalogFormatterApplied\":true");
    assert_contains(json, "\"archiveCloseoutVerificationAuditCloseoutCommandAvailable\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    // End archive closeout verification audit closeout assertions.
    // Archive closeout verification audit closeout archive verification assertions updated per mini-kv release.
    assert_contains(json, "\"shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseoutArchiveVerifica"
                          "tion\":{\"contract\":"
                          "\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout-"
                          "archive-verification.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\"");
    assert_contains(json,
                    "\"sourceAuditCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(json, "\"sourceAuditCloseoutReleaseVersion\":\"v440\"");
    assert_contains(json, "\"sourceAuditCloseoutPublishedStageCount\":20");
    assert_contains(json, "\"sourceAuditCloseoutChainComplete\":true");
    assert_contains(json, "\"sharedStageCatalogFormatterApplied\":true");
    assert_contains(json, "\"declaredArchiveVerificationOnly\":true");
    assert_contains(json, "\"archiveRootHintUsedForHumanReviewOnly\":true");
    assert_contains(json, "\"archiveCloseoutVerificationAuditCloseoutArchiveVerificationCommandAvailable\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    // End archive closeout verification audit closeout archive verification assertions.
    assert_contains(json, "\"shardRoutePreviewArchiveMaintenance\":{\"contract\":"
                          "\"shard-route-preview-archive-maintenance.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEARCHIVEMAINTJSON\"");
    assert_contains(json, "\"maintenanceMode\":\"read-only-route-preview-archive-maintenance-ledger\"");
    assert_contains(json, "\"sourceArchiveVerificationReleaseVersion\":\"v460\"");
    assert_contains(json, "\"sourceArchiveVerificationPublishedStageCount\":20");
    assert_contains(json, "\"sourceArchiveVerificationChainComplete\":true");
    assert_contains(json, "\"maintenanceStage\":\"route-preview-archive-maintenance-release-package\"");
    assert_contains(json, "\"maintenanceStageSequence\":25");
    assert_contains(json, "\"maintenanceReleaseVersion\":\"v485\"");
    assert_contains(json, "\"sharedStageCatalogFormatterApplied\":true");
    assert_contains(json, "\"focusedTestHelperApplied\":true");
    assert_contains(json, "\"archiveMaintenanceCommandAvailable\":true");
    assert_contains(json, "\"archiveMaintenanceChainComplete\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewArchiveMaintenanceVerification\":{\"contract\":"
                          "\"shard-route-preview-archive-maintenance-verification.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\"");
    assert_contains(json, "\"verificationMode\":\"read-only-route-preview-archive-maintenance-chain-verification\"");
    assert_contains(json, "\"sourceMaintenanceReleaseVersion\":\"v485\"");
    assert_contains(json, "\"sourceMaintenancePublishedStageCount\":25");
    assert_contains(json, "\"sourceMaintenanceChainComplete\":true");
    assert_contains(json, "\"stageChainHelperApplied\":true");
    assert_contains(json, "\"archiveMaintenanceVerificationCommandAvailable\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewWorksheetVerification\":{\"contract\":"
                          "\"shard-route-preview-worksheet-verification.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEWORKSHEETVERIFYJSON\"");
    assert_contains(json, "\"verificationMode\":\"read-only-shard-preview-worksheet-boundary-verification\"");
    assert_contains(json, "\"sourceVerificationCommand\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\"");
    assert_contains(json, "\"sourceVerificationReleaseVersion\":\"v510\"");
    assert_contains(json, "\"sourceVerificationPublishedStageCount\":25");
    assert_contains(json, "\"sourceVerificationChainComplete\":true");
    assert_contains(json, "\"verifiedSourceReleaseRangeStart\":\"v486\"");
    assert_contains(json, "\"verifiedSourceReleaseRangeEnd\":\"v510\"");
    assert_contains(json, "\"operatorEntrySlotCount\":25");
    assert_contains(json, "\"readyForOperatorEntryWorksheet\":true");
    assert_contains(json, "\"readyForManualEvidenceEntry\":false");
    assert_contains(json, "\"readyForLiveExecution\":false");
    assert_contains(json, "\"readyForProductionExecution\":false");
    assert_contains(json, "\"operatorEntryValuesAccepted\":false");
    assert_contains(json, "\"manualEvidenceImportAllowed\":false");
    assert_contains(json, "\"runtimePayloadAccepted\":false");
    assert_contains(json, "\"syntheticEvidenceAccepted\":false");
    assert_contains(json, "\"singleWorksheetVerifierSectionExpected\":true");
    assert_contains(json, "\"declaredWorksheetVerificationOnly\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewOperatorImportPreflight\":{\"contract\":"
                          "\"shard-route-preview-operator-import-preflight.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEIMPORTPREFLIGHTJSON\"");
    assert_contains(json, "\"preflightMode\":\"controlled-read-only-operator-evidence-import-preflight\"");
    assert_contains(json, "\"sourceWorksheetCommand\":\"SHARDROUTEWORKSHEETVERIFYJSON\"");
    assert_contains(json, "\"sourceWorksheetReleaseVersion\":\"v535\"");
    assert_contains(json, "\"sourceWorksheetPublishedStageCount\":25");
    assert_contains(json, "\"sourceWorksheetChainComplete\":true");
    assert_contains(json, "\"sourceWorksheetReleaseRangeStart\":\"v511\"");
    assert_contains(json, "\"sourceWorksheetReleaseRangeEnd\":\"v535\"");
    assert_contains(json, "\"operatorPreflightSlotCount\":25");
    assert_contains(json, "\"acceptedOperatorEvidenceSlotCount\":0");
    assert_contains(json, "\"readyForOperatorEvidenceImportPreflight\":true");
    assert_contains(json, "\"readyForEvidenceImport\":false");
    assert_contains(json, "\"readyForManualEvidenceEntry\":false");
    assert_contains(json, "\"readyForLiveExecution\":false");
    assert_contains(json, "\"readyForProductionExecution\":false");
    assert_contains(json, "\"operatorEvidenceValuesAccepted\":false");
    assert_contains(json, "\"evidenceImportAllowed\":false");
    assert_contains(json, "\"manualEvidenceImportAllowed\":false");
    assert_contains(json, "\"runtimePayloadAccepted\":false");
    assert_contains(json, "\"syntheticEvidenceAccepted\":false");
    assert_contains(json, "\"secretValueStored\":false");
    assert_contains(json, "\"credentialValueStored\":false");
    assert_contains(json, "\"rawEndpointStored\":false");
    assert_contains(json, "\"normalizerExecutes\":false");
    assert_contains(json, "\"normalizationExecutionAllowed\":false");
    assert_contains(json, "\"missingValuePolicyBlocksImport\":true");
    assert_contains(json, "\"importBlockersEnforcedAsMetadata\":true");
    assert_contains(json, "\"redactionPreserved\":true");
    assert_contains(json, "\"declaredImportPreflightOnly\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"shardRoutePreviewOperatorValueDraft\":{\"contract\":"
                          "\"shard-route-preview-operator-value-draft.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUEDRAFTJSON\"");
    assert_contains(json, "\"draftMode\":\"controlled-read-only-operator-evidence-value-draft\"");
    assert_contains(json, "\"sourceImportPreflightCommand\":\"SHARDROUTEIMPORTPREFLIGHTJSON\"");
    assert_contains(json, "\"sourceImportPreflightReleaseVersion\":\"v560\"");
    assert_contains(json, "\"sourceImportPreflightFixturePath\":\"fixtures/release/shard-readiness-v560.json\"");
    assert_contains(json, "\"sourceImportPreflightPublishedStageCount\":25");
    assert_contains(json, "\"sourceImportPreflightChainComplete\":true");
    assert_contains(json, "\"sourceImportPreflightReleaseRangeStart\":\"v536\"");
    assert_contains(json, "\"sourceImportPreflightReleaseRangeEnd\":\"v560\"");
    assert_contains(json, "\"operatorValueDraftSlotCount\":25");
    assert_contains(json, "\"expectedOperatorEvidenceSlotCount\":25");
    assert_contains(json, "\"draftSlotCount\":25");
    assert_contains(json, "\"actualValueState\":\"not-supplied\"");
    assert_contains(json, "\"draftValueState\":\"awaiting-operator-value\"");
    assert_contains(json, "\"readyForOperatorEvidenceValueDraft\":true");
    assert_contains(json, "\"operatorValueSupplied\":false");
    assert_contains(json, "\"operatorValueAccepted\":false");
    assert_contains(json, "\"operatorValueImported\":false");
    assert_contains(json, "\"valueSupplyAdapterInstalled\":false");
    assert_contains(json, "\"valueSupplyAdapterEnabled\":false");
    assert_contains(json, "\"draftValuesPersisted\":false");
    assert_contains(json, "\"valueDraftPackageOnly\":true");
    assert_contains(json, "\"valueDraftSlotHelperApplied\":true");
    assert_contains(json, "\"singleValueDraftSectionExpected\":true");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplyEnvelope\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-envelope.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYJSON\"");
    assert_contains(json, "\"envelopeMode\":\"controlled-read-only-disabled-operator-value-supply-envelope\"");
    assert_contains(json, "\"sourceValueDraftCommand\":\"SHARDROUTEVALUEDRAFTJSON\"");
    assert_contains(json, "\"sourceValueDraftReleaseVersion\":\"v585\"");
    assert_contains(json, "\"sourceValueDraftFixturePath\":\"fixtures/release/shard-readiness-v585.json\"");
    assert_contains(json, "\"sourceValueDraftChainComplete\":true");
    assert_contains(json, "\"valueSupplyReleaseRangeStart\":\"v586\"");
    assert_contains(json, "\"valueSupplyReleaseRangeEnd\":\"v610\"");
    assert_contains(json, "\"operatorValueEnvelopeState\":\"disabled-by-default\"");
    assert_contains(json, "\"missingValuePolicy\":\"fail-closed\"");
    assert_contains(json, "\"redactionPolicy\":\"redact-before-persist\"");
    assert_contains(json, "\"readyForDisabledValueSupplyEnvelope\":true");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplyPrecheck\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-precheck.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\"");
    assert_contains(json, "\"precheckMode\":\"controlled-read-only-disabled-operator-value-supply-precheck\"");
    assert_contains(json, "\"sourceValueSupplyEnvelopeCommand\":\"SHARDROUTEVALUESUPPLYJSON\"");
    assert_contains(json, "\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\"");
    assert_contains(json, "\"sourceValueSupplyEnvelopeFixturePath\":\"fixtures/release/shard-readiness-v610.json\"");
    assert_contains(json, "\"sourceValueSupplyEnvelopeChainComplete\":true");
    assert_contains(json, "\"valueSupplyPrecheckReleaseRangeStart\":\"v611\"");
    assert_contains(json, "\"operatorValueSupplyApprovalPacketState\":\"required-not-present\"");
    assert_contains(json, "\"approvalPacketRequired\":true");
    assert_contains(json, "\"approvalPacketPresent\":false");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplyApprovalTemplate\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-approval-template.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\"");
    assert_contains(
        json, "\"approvalTemplateMode\":\"controlled-read-only-disabled-operator-value-supply-approval-template\"");
    assert_contains(json, "\"sourceValueSupplyPrecheckCommand\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\"");
    assert_contains(json, "\"sourceValueSupplyPrecheckReleaseVersion\":\"v635\"");
    assert_contains(json, "\"sourceValueSupplyPrecheckFixturePath\":\"fixtures/release/shard-readiness-v635.json\"");
    assert_contains(json, "\"valueSupplyApprovalTemplateReleaseRangeStart\":\"v636\"");
    assert_contains(json, "\"operatorApprovalPacketTemplateDeclared\":true");
    assert_contains(json, "\"approvalTemplateOnly\":true");
    assert_contains(json, "\"readyForOperatorValueSupplyApproval\":false");
    assert_contains(json, "\"operatorIdentityRequired\":true");
    assert_contains(json, "\"operatorIdentityPresent\":false");
    assert_contains(json, "\"approvalTimestampRequired\":true");
    assert_contains(json, "\"approvalTimestampPresent\":false");
    assert_contains(json, "\"perValueProvenanceRequired\":true");
    assert_contains(json, "\"perValueProvenanceSupplied\":false");
    assert_contains(json, "\"missingValueRejected\":true");
    assert_contains(json, "\"malformedValueRejected\":true");
    assert_contains(json, "\"shardRoutePreviewOperatorValueSupplySignedApprovalTemplate\":{\"contract\":"
                          "\"shard-route-preview-operator-value-supply-signed-approval-template.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\"");
    assert_contains(json, "\"signedApprovalTemplateMode\":\"controlled-read-only-disabled-operator-value-supply-signed-"
                          "approval-template\"");
    assert_contains(json, "\"sourceApprovalTemplateCommand\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\"");
    assert_contains(json, "\"sourceApprovalTemplateReleaseVersion\":\"v660\"");
    assert_contains(json, "\"sourceApprovalTemplateFixturePath\":\"fixtures/release/shard-readiness-v660.json\"");
    assert_contains(json, "\"signedApprovalTemplateReleaseRangeStart\":\"v661\"");
    assert_contains(json, "\"signedApprovalTemplateDeclared\":true");
    assert_contains(json, "\"signedApprovalTemplateOnly\":true");
    assert_contains(json, "\"signedApprovalRequired\":true");
    assert_contains(json, "\"signedApprovalPresent\":false");
    assert_contains(json, "\"signatureCaptured\":false");
    assert_contains(json, "\"readyForSignedApprovalCapturePreflight\":false");
    assert_contains(json, "\"readyForSignedApprovalCapture\":false");
    assert_contains(json, "\"readyForDisabledValueSupplyPrecheck\":true");
    assert_contains(json, "\"readyForOperatorValueSupplyApproval\":false");
    assert_contains(json, "\"readyForOperatorValueSupply\":false");
    assert_contains(json, "\"automaticSiblingImportAllowed\":false");
    assert_contains(json, "\"sourceEvidenceProvenanceSupplied\":false");
    assert_contains(json, "\"valueSupplySlotHelperApplied\":true");
    assert_contains(json, "\"singleValueSupplyEnvelopeSectionExpected\":true");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v379\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v379.json\"");
    assert_contains(json, "\"archivedArchiveReleaseRangeStart\":\"v341\"");
    assert_contains(json, "\"archivedArchiveReleaseRangeEnd\":\"v360\"");
    assert_contains(json, "\"archivedArchiveReleaseCount\":20");
    assert_contains(json, "\"archiveCloseoutCommandAvailable\":true");
    assert_contains(json, "\"archiveCloseoutChainComplete\":true");
    assert_contains(json, "\"filesystemReadPerformed\":false");
    assert_contains(json, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"executionAllowed\":false");
}

} // namespace minikv::shard_test
