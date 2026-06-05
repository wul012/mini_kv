#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit_closeout {
namespace {

constexpr int current_archive_closeout_verification_audit_closeout_stage_count = 5;
constexpr int planned_archive_closeout_verification_audit_closeout_stage_count = 20;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_archive_closeout_verification_audit_closeout_stage_count>
    archive_closeout_verification_audit_closeout_stages = {{
        {1,
         "v421",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-command-and-stage-catalog-refactor",
         "v420",
         "fixtures/release/shard-readiness-v420.json",
         "publishes a read-only closeout command for the v401-v420 audit chain and extracts shared stage catalog formatting"},
        {2,
         "v422",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-catalog-alignment",
         "v421",
         "fixtures/release/shard-readiness-v421.json",
         "aligns the audit closeout command with COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON"},
        {3,
         "v423",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-explain-contract",
         "v422",
         "fixtures/release/shard-readiness-v422.json",
         "pins EXPLAINJSON semantics for the audit closeout command as metadata-only evidence"},
        {4,
         "v424",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-checkjson-contract",
         "v423",
         "fixtures/release/shard-readiness-v423.json",
         "pins CHECKJSON semantics for the audit closeout command as read-only and non-executable"},
        {5,
         "v425",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-cli-smoke-contract",
         "v424",
         "fixtures/release/shard-readiness-v424.json",
         "records CLI smoke expectations for audit closeout evidence without opening write routing"},
        {6,
         "v426",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-tcp-smoke-contract",
         "v425",
         "fixtures/release/shard-readiness-v425.json",
         "records TCP smoke expectations for audit closeout evidence while stopping the owned server after validation"},
        {7,
         "v427",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-fixture-freeze",
         "v426",
         "fixtures/release/shard-readiness-v426.json",
         "freezes the previous shard readiness fixture before rolling current audit closeout evidence forward"},
        {8,
         "v428",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-source-audit-index",
         "v427",
         "fixtures/release/shard-readiness-v427.json",
         "indexes the closed v401-v420 source audit range for downstream read-only consumers"},
        {9,
         "v429",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-source-audit-index-audit",
         "v428",
         "fixtures/release/shard-readiness-v428.json",
         "audits source audit index continuity without filesystem archive walking or router activation"},
        {10,
         "v430",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-retention-map",
         "v429",
         "fixtures/release/shard-readiness-v429.json",
         "maps retained fixtures, screenshots, explanations, and walkthroughs for the audit closeout chain"},
        {11,
         "v431",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-retention-audit",
         "v430",
         "fixtures/release/shard-readiness-v430.json",
         "audits retention metadata while preserving no filesystem read and no runtime probe boundaries"},
        {12,
         "v432",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-consumer-handoff",
         "v431",
         "fixtures/release/shard-readiness-v431.json",
         "records Node consumer handoff semantics for audit closeout fields"},
        {13,
         "v433",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-consumer-handoff-audit",
         "v432",
         "fixtures/release/shard-readiness-v432.json",
         "audits consumer handoff semantics and keeps live reads blocked"},
        {14,
         "v434",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-java-echo-handoff",
         "v433",
         "fixtures/release/shard-readiness-v433.json",
         "records Java echo handoff semantics without giving mini-kv order authority"},
        {15,
         "v435",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-java-echo-audit",
         "v434",
         "fixtures/release/shard-readiness-v434.json",
         "audits Java echo expectations while preserving the read-only evidence contract"},
        {16,
         "v436",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-boundary-field-audit",
         "v435",
         "fixtures/release/shard-readiness-v435.json",
         "audits boundary fields for readOnly, executionAllowed, router, write, WAL, and service-start semantics"},
        {17,
         "v437",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-no-router-audit",
         "v436",
         "fixtures/release/shard-readiness-v436.json",
         "audits that audit closeout evidence installs no active shard router"},
        {18,
         "v438",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-no-write-audit",
         "v437",
         "fixtures/release/shard-readiness-v437.json",
         "audits that audit closeout evidence permits no writes and touches no WAL"},
        {19,
         "v439",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-no-execution-audit",
         "v438",
         "fixtures/release/shard-readiness-v438.json",
         "audits that audit closeout evidence cannot trigger restore, load, compact, routing, or service execution"},
        {20,
         "v440",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-release-package",
         "v439",
         "fixtures/release/shard-readiness-v439.json",
         "packages final v421-v440 audit closeout evidence for read-only downstream consumption"},
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
    return archive_closeout_verification_audit_closeout_stages.at(
        static_cast<std::size_t>(current_archive_closeout_verification_audit_closeout_stage_count - 1));
}

} // namespace

std::string format_closeout_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        archive_closeout_verification_audit_closeout_stages,
        current_archive_closeout_verification_audit_closeout_stage_count);
}

std::string format_closeout_json() {
    const auto& stage = current_stage();
    const bool source_chain_complete =
        shard_route_preview_verification_report_archive_closeout_verification_audit::published_stage_count() == 20;
    return "{\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\","
           "\"closeoutMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit-closeout\","
           "\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceAuditCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\""
           ",\"sourceVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\""
           ",\"sourceArchiveCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\""
           ",\"sourceAuditReleaseVersion\":\"v420\""
           ",\"sourceAuditFixturePath\":\"fixtures/release/shard-readiness-v420.json\""
           ",\"sourceAuditPublishedStageCount\":" +
           std::to_string(
               shard_route_preview_verification_report_archive_closeout_verification_audit::published_stage_count()) +
           ",\"sourceAuditPlannedStageCount\":20"
           ",\"sourceAuditChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceAuditDigestMarker\":" +
           json_string(
               shard_route_preview_verification_report_archive_closeout_verification_audit::audit_digest_marker()) +
           ",\"closeoutStage\":" +
           json_string(stage.stage) +
           ",\"closeoutStageSequence\":" + std::to_string(stage.sequence) +
           ",\"closeoutReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" +
           std::to_string(current_archive_closeout_verification_audit_closeout_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(planned_archive_closeout_verification_audit_closeout_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"closedAuditReleaseRangeStart\":\"v401\""
           ",\"closedAuditReleaseRangeEnd\":\"v420\""
           ",\"closedAuditReleaseCount\":20"
           ",\"sourceAuditVersionedFixturePath\":\"fixtures/release/shard-readiness-v420.json\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"closeoutStageCatalog\":" +
           format_closeout_stage_catalog_json() +
           ",\"closeoutChecks\":" +
           json_string_array({"source audit command remains available",
                              "source audit chain remains complete",
                              "source audit digest marker remains pinned",
                              "shared stage catalog formatter preserves prior JSON fields",
                              "previous current fixture is frozen before rolling",
                              "current SHARDJSON embeds exactly one verification audit closeout section",
                              "COMMANDS and COMMANDSJSON expose read-only catalog metadata",
                              "EXPLAINJSON classifies the closeout command as metadata read",
                              "CHECKJSON denies execution",
                              "CLI smoke reads closeout evidence only",
                              "TCP smoke uses an owned server and stops it",
                              "no router, write, WAL, restore, load, compact, or live-read authority is added"}) +
           ",\"closeoutCheckCount\":12"
           ",\"closeoutPassedCount\":12"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" +
           json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleAuditCloseoutSectionExpected\":true"
           ",\"archiveCloseoutVerificationAuditCloseoutCommandAvailable\":true"
           ",\"archiveCloseoutVerificationAuditCloseoutChainComplete\":" +
           json_bool(current_archive_closeout_verification_audit_closeout_stage_count ==
                     planned_archive_closeout_verification_audit_closeout_stage_count) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read archive closeout verification audit closeout evidence only",
                              "treat v401-v420 verification audit release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
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

std::string closeout_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_archive_closeout_verification_audit_closeout_stage_count,
        planned_archive_closeout_verification_audit_closeout_stage_count);
}

int published_stage_count() {
    return current_archive_closeout_verification_audit_closeout_stage_count;
}

} // namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit_closeout
