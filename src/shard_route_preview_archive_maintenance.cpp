#include "minikv/shard_route_preview_archive_maintenance.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_audit_closeout_archive_verification.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_archive_maintenance {
namespace {

constexpr int current_archive_maintenance_stage_count = 8;
constexpr int planned_archive_maintenance_stage_count = 25;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_archive_maintenance_stage_count> archive_maintenance_stages = {{
    {1,
     "v461",
     "route-preview-archive-maintenance-intake",
     "v460",
     "fixtures/release/shard-readiness-v460.json",
     "starts a read-only maintenance ledger over the completed v441-v460 archive verification package"},
    {2,
     "v462",
     "route-preview-archive-maintenance-source-release-freeze",
     "v461",
     "fixtures/release/shard-readiness-v461.json",
     "freezes the first maintenance ledger fixture before rolling source package checks forward"},
    {3,
     "v463",
     "route-preview-archive-maintenance-stage-catalog-refactor",
     "v462",
     "fixtures/release/shard-readiness-v462.json",
     "keeps stage catalog formatting centralized instead of duplicating JSON record formatting"},
    {4,
     "v464",
     "route-preview-archive-maintenance-boundary-helper-split",
     "v463",
     "fixtures/release/shard-readiness-v463.json",
     "records boundary assertion helper extraction for read-only evidence tests"},
    {5,
     "v465",
     "route-preview-archive-maintenance-focused-test-helper",
     "v464",
     "fixtures/release/shard-readiness-v464.json",
     "keeps the maintenance ledger test isolated from giant command and shard readiness suites"},
    {6,
     "v466",
     "route-preview-archive-maintenance-command-catalog-contract",
     "v465",
     "fixtures/release/shard-readiness-v465.json",
     "pins COMMANDS and COMMANDSJSON metadata for the maintenance ledger command"},
    {7,
     "v467",
     "route-preview-archive-maintenance-explain-contract",
     "v466",
     "fixtures/release/shard-readiness-v466.json",
     "pins EXPLAINJSON metadata-read semantics for the maintenance ledger command"},
    {8,
     "v468",
     "route-preview-archive-maintenance-checkjson-contract",
     "v467",
     "fixtures/release/shard-readiness-v467.json",
     "pins CHECKJSON no-execution semantics for the maintenance ledger command"},
    {9,
     "v469",
     "route-preview-archive-maintenance-cli-smoke-contract",
     "v468",
     "fixtures/release/shard-readiness-v468.json",
     "records CLI smoke expectations for reading maintenance evidence without writes"},
    {10,
     "v470",
     "route-preview-archive-maintenance-tcp-smoke-contract",
     "v469",
     "fixtures/release/shard-readiness-v469.json",
     "records owned TCP smoke expectations and cleanup requirements for the maintenance command"},
    {11,
     "v471",
     "route-preview-archive-maintenance-fixture-freeze",
     "v470",
     "fixtures/release/shard-readiness-v470.json",
     "requires previous rolling fixtures to be frozen before each maintenance ledger advance"},
    {12,
     "v472",
     "route-preview-archive-maintenance-source-fixture-index",
     "v471",
     "fixtures/release/shard-readiness-v471.json",
     "indexes source v441-v460 fixtures as retained human-review evidence"},
    {13,
     "v473",
     "route-preview-archive-maintenance-source-fixture-index-audit",
     "v472",
     "fixtures/release/shard-readiness-v472.json",
     "audits source fixture index continuity without filesystem reads at runtime"},
    {14,
     "v474",
     "route-preview-archive-maintenance-retention-map",
     "v473",
     "fixtures/release/shard-readiness-v473.json",
     "maps retained screenshots, explanations, walkthroughs, fixtures, commits, tags, and CI evidence"},
    {15,
     "v475",
     "route-preview-archive-maintenance-retention-audit",
     "v474",
     "fixtures/release/shard-readiness-v474.json",
     "audits retention metadata while keeping archive roots as documentation hints only"},
    {16,
     "v476",
     "route-preview-archive-maintenance-walkthrough-index",
     "v475",
     "fixtures/release/shard-readiness-v475.json",
     "indexes walkthrough coverage for the maintenance chain"},
    {17,
     "v477",
     "route-preview-archive-maintenance-walkthrough-index-audit",
     "v476",
     "fixtures/release/shard-readiness-v476.json",
     "audits walkthrough index coverage and Markdown hygiene expectations"},
    {18,
     "v478",
     "route-preview-archive-maintenance-command-dispatch-audit",
     "v477",
     "fixtures/release/shard-readiness-v477.json",
     "audits command dispatch as a metadata-read path with no store mutation"},
    {19,
     "v479",
     "route-preview-archive-maintenance-shardjson-embedding-audit",
     "v478",
     "fixtures/release/shard-readiness-v478.json",
     "audits that SHARDJSON embeds exactly one maintenance ledger section"},
    {20,
     "v480",
     "route-preview-archive-maintenance-consumer-handoff",
     "v479",
     "fixtures/release/shard-readiness-v479.json",
     "records downstream consumer handoff semantics for maintenance evidence"},
    {21,
     "v481",
     "route-preview-archive-maintenance-java-echo-boundary-handoff",
     "v480",
     "fixtures/release/shard-readiness-v480.json",
     "records Java echo boundary semantics without granting mini-kv order or audit authority"},
    {22,
     "v482",
     "route-preview-archive-maintenance-no-router-audit",
     "v481",
     "fixtures/release/shard-readiness-v481.json",
     "audits that maintenance evidence installs no active shard router"},
    {23,
     "v483",
     "route-preview-archive-maintenance-no-write-audit",
     "v482",
     "fixtures/release/shard-readiness-v482.json",
     "audits that maintenance evidence permits no writes and touches no WAL"},
    {24,
     "v484",
     "route-preview-archive-maintenance-no-execution-audit",
     "v483",
     "fixtures/release/shard-readiness-v483.json",
     "audits that maintenance evidence cannot trigger restore, load, compact, routing, or service execution"},
    {25,
     "v485",
     "route-preview-archive-maintenance-release-package",
     "v484",
     "fixtures/release/shard-readiness-v484.json",
     "packages final v461-v485 maintenance evidence for read-only downstream consumption"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

const StageRecord& current_stage() {
    return archive_maintenance_stages.at(static_cast<std::size_t>(current_archive_maintenance_stage_count - 1));
}

} // namespace

std::string format_maintenance_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(archive_maintenance_stages,
                                                                        current_archive_maintenance_stage_count);
}

std::string format_maintenance_json() {
    const auto& stage = current_stage();
    const bool source_chain_complete =
        shard_route_preview_audit_closeout_archive_verification::published_stage_count() == 20;

    return "{\"contract\":\"shard-route-preview-archive-maintenance.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEARCHIVEMAINTJSON\""
           ",\"maintenanceMode\":\"read-only-route-preview-archive-maintenance-ledger\""
           ",\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceArchiveVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\""
           ",\"sourceArchiveVerificationReleaseVersion\":\"v460\""
           ",\"sourceArchiveVerificationFixturePath\":\"fixtures/release/shard-readiness-v460.json\""
           ",\"sourceArchiveVerificationArchiveRootHint\":\"e/460/\""
           ",\"sourceArchiveVerificationPublishedStageCount\":" +
           std::to_string(shard_route_preview_audit_closeout_archive_verification::published_stage_count()) +
           ",\"sourceArchiveVerificationPlannedStageCount\":20"
           ",\"sourceArchiveVerificationChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceArchiveVerificationDigestMarker\":" +
           json_string(shard_route_preview_audit_closeout_archive_verification::archive_verification_digest_marker()) +
           ",\"maintenanceStage\":" +
           json_string(stage.stage) +
           ",\"maintenanceStageSequence\":" + std::to_string(stage.sequence) +
           ",\"maintenanceReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_archive_maintenance_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_archive_maintenance_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"maintainedReleaseRangeStart\":\"v441\""
           ",\"maintainedReleaseRangeEnd\":\"v460\""
           ",\"maintainedReleaseCount\":20"
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"focusedTestHelper\":\"route_preview_evidence_assertions\""
           ",\"archiveMaintenanceStageCatalog\":" +
           format_maintenance_stage_catalog_json() +
           ",\"maintenanceChecks\":" +
           json_string_array({"source archive verification command remains available",
                              "source archive verification chain remains complete",
                              "source archive verification digest marker remains pinned",
                              "v441-v460 source range remains declared and versioned",
                              "archive roots are retained evidence hints, not runtime filesystem walks",
                              "previous current fixture is frozen before rolling",
                              "current SHARDJSON embeds exactly one maintenance ledger section",
                              "COMMANDS and COMMANDSJSON expose read-only catalog metadata",
                              "EXPLAINJSON classifies the maintenance command as metadata read",
                              "CHECKJSON denies execution",
                              "CLI smoke reads maintenance evidence only",
                              "TCP smoke uses an owned server and stops it",
                              "focused tests use a shared read-only evidence assertion helper",
                              "no router, write, WAL, restore, load, compact, or live-read authority is added"}) +
           ",\"maintenanceCheckCount\":14"
           ",\"maintenancePassedCount\":14"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" + json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"focusedTestHelperApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleMaintenanceSectionExpected\":true"
           ",\"declaredMaintenanceOnly\":true"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"archiveMaintenanceCommandAvailable\":true"
           ",\"archiveMaintenanceChainComplete\":" +
           json_bool(current_archive_maintenance_stage_count == planned_archive_maintenance_stage_count) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read maintenance evidence only",
                              "treat v441-v460 archive verification release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat sourceArchiveVerificationArchiveRootHint as a documentation pointer only",
                              "do not install an active shard router",
                              "do not route writes",
                              "do not run restore, load, compact, or service startup",
                              "do not treat mini-kv as Java order authority"}) +
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"executesRoute\":false"
           ",\"storageDirectoriesCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsServices\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"liveReadAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string maintenance_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_archive_maintenance_stage_count,
                                                                   planned_archive_maintenance_stage_count);
}

int published_stage_count() {
    return current_archive_maintenance_stage_count;
}

} // namespace minikv::shard_route_preview_archive_maintenance
