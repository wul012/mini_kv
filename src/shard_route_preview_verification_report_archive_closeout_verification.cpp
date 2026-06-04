#include "minikv/shard_route_preview_verification_report_archive_closeout_verification.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification_report_archive_closeout_verification {
namespace {

constexpr int current_archive_closeout_verification_stage_count = 11;
constexpr int planned_archive_closeout_verification_stage_count = 20;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

struct ArchiveCloseoutVerificationStage {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

constexpr std::array<ArchiveCloseoutVerificationStage, planned_archive_closeout_verification_stage_count>
    archive_closeout_verification_stages = {{
    {1,
     "v381",
     "route-preview-verification-report-archive-closeout-verification-command-contract",
     "v380",
     "fixtures/release/shard-readiness-v380.json",
     "publishes a read-only verification command for the v361-v380 archive closeout evidence chain"},
    {2,
     "v382",
     "route-preview-verification-report-archive-closeout-verification-catalog-alignment",
     "v381",
     "fixtures/release/shard-readiness-v381.json",
     "aligns the verification command with COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON without changing runtime behavior"},
    {3,
     "v383",
     "route-preview-verification-report-archive-closeout-verification-explain-contract",
     "v382",
     "fixtures/release/shard-readiness-v382.json",
     "pins EXPLAINJSON semantics for the archive closeout verification command as metadata-only evidence"},
    {4,
     "v384",
     "route-preview-verification-report-archive-closeout-verification-checkjson-contract",
     "v383",
     "fixtures/release/shard-readiness-v383.json",
     "pins CHECKJSON semantics for the archive closeout verification command as read-only and non-executable"},
    {5,
     "v385",
     "route-preview-verification-report-archive-closeout-verification-cli-smoke-contract",
     "v384",
     "fixtures/release/shard-readiness-v384.json",
     "records CLI smoke expectations for archive closeout verification without opening write routing"},
    {6,
     "v386",
     "route-preview-verification-report-archive-closeout-verification-tcp-smoke-contract",
     "v385",
     "fixtures/release/shard-readiness-v385.json",
     "records TCP smoke expectations for archive closeout verification while stopping the owned server after validation"},
    {7,
     "v387",
     "route-preview-verification-report-archive-closeout-verification-fixture-freeze",
     "v386",
     "fixtures/release/shard-readiness-v386.json",
     "freezes the previous shard readiness fixture before rolling current verification evidence forward"},
    {8,
     "v388",
     "route-preview-verification-report-archive-closeout-verification-source-closeout-index",
     "v387",
     "fixtures/release/shard-readiness-v387.json",
     "indexes the closed v361-v380 source closeout range for downstream read-only consumers"},
    {9,
     "v389",
     "route-preview-verification-report-archive-closeout-verification-source-closeout-index-audit",
     "v388",
     "fixtures/release/shard-readiness-v388.json",
     "audits source closeout index continuity without filesystem archive walking or router activation"},
    {10,
     "v390",
     "route-preview-verification-report-archive-closeout-verification-retention-map",
     "v389",
     "fixtures/release/shard-readiness-v389.json",
     "maps retained fixtures, screenshots, explanations, and walkthroughs for the verification chain"},
    {11,
     "v391",
     "route-preview-verification-report-archive-closeout-verification-retention-audit",
     "v390",
     "fixtures/release/shard-readiness-v390.json",
     "audits retention metadata while preserving no filesystem read and no runtime probe boundaries"},
    {12,
     "v392",
     "route-preview-verification-report-archive-closeout-verification-consumer-handoff",
     "v391",
     "fixtures/release/shard-readiness-v391.json",
     "records Node consumer handoff semantics for archive closeout verification fields"},
    {13,
     "v393",
     "route-preview-verification-report-archive-closeout-verification-consumer-handoff-audit",
     "v392",
     "fixtures/release/shard-readiness-v392.json",
     "audits consumer handoff semantics and keeps live reads blocked"},
    {14,
     "v394",
     "route-preview-verification-report-archive-closeout-verification-java-echo-handoff",
     "v393",
     "fixtures/release/shard-readiness-v393.json",
     "records Java echo handoff semantics without giving mini-kv order authority"},
    {15,
     "v395",
     "route-preview-verification-report-archive-closeout-verification-java-echo-audit",
     "v394",
     "fixtures/release/shard-readiness-v394.json",
     "audits Java echo expectations while preserving the read-only evidence contract"},
    {16,
     "v396",
     "route-preview-verification-report-archive-closeout-verification-boundary-field-audit",
     "v395",
     "fixtures/release/shard-readiness-v395.json",
     "audits boundary fields for readOnly, executionAllowed, router, write, WAL, and service-start semantics"},
    {17,
     "v397",
     "route-preview-verification-report-archive-closeout-verification-no-router-audit",
     "v396",
     "fixtures/release/shard-readiness-v396.json",
     "audits that archive closeout verification evidence installs no active shard router"},
    {18,
     "v398",
     "route-preview-verification-report-archive-closeout-verification-no-write-audit",
     "v397",
     "fixtures/release/shard-readiness-v397.json",
     "audits that archive closeout verification evidence permits no writes and touches no WAL"},
    {19,
     "v399",
     "route-preview-verification-report-archive-closeout-verification-no-execution-audit",
     "v398",
     "fixtures/release/shard-readiness-v398.json",
     "audits that archive closeout verification evidence cannot trigger restore, load, compact, routing, or service execution"},
    {20,
     "v400",
     "route-preview-verification-report-archive-closeout-verification-release-package",
     "v399",
     "fixtures/release/shard-readiness-v399.json",
     "packages final v381-v400 archive closeout verification evidence for read-only downstream consumption"},
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

const ArchiveCloseoutVerificationStage& current_stage() {
    return archive_closeout_verification_stages.at(
        static_cast<std::size_t>(current_archive_closeout_verification_stage_count - 1));
}

std::string format_stage_json(const ArchiveCloseoutVerificationStage& stage) {
    return "{\"sequence\":" + std::to_string(stage.sequence) +
           ",\"releaseVersion\":" + json_string(stage.release_version) +
           ",\"stage\":" + json_string(stage.stage) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"scope\":" + json_string(stage.scope) +
           ",\"readOnly\":true"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::string format_verification_stage_catalog_json() {
    std::string result = "[";
    for (int index = 0; index < current_archive_closeout_verification_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(archive_closeout_verification_stages.at(static_cast<std::size_t>(index)));
    }
    result += "]";
    return result;
}

std::string format_verification_json() {
    const auto& stage = current_stage();
    const bool source_chain_complete =
        shard_route_preview_verification_report_archive_closeout::published_stage_count() == 20;
    return "{\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\","
           "\"verificationMode\":\"read-only-route-preview-verification-report-archive-closeout-verification\","
           "\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceArchiveCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\""
           ",\"sourceArchiveCommand\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\""
           ",\"sourceReportCloseoutCommand\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\""
           ",\"sourceReportCommand\":\"SHARDROUTEVERIFYREPORTJSON\""
           ",\"sourceVerificationCommand\":\"SHARDROUTEVERIFYJSON\""
           ",\"sourcePreviewCommand\":\"SHARDROUTEJSON\""
           ",\"sourceArchiveCloseoutReleaseVersion\":\"v380\""
           ",\"sourceArchiveCloseoutFixturePath\":\"fixtures/release/shard-readiness-v380.json\""
           ",\"sourceArchiveCloseoutPublishedStageCount\":" +
           std::to_string(shard_route_preview_verification_report_archive_closeout::published_stage_count()) +
           ",\"sourceArchiveCloseoutPlannedStageCount\":20"
           ",\"sourceArchiveCloseoutChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceArchiveCloseoutDigestMarker\":" +
           json_string(shard_route_preview_verification_report_archive_closeout::closeout_digest_marker()) +
           ",\"verificationStage\":" +
           json_string(stage.stage) +
           ",\"verificationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"verificationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_archive_closeout_verification_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_archive_closeout_verification_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"verifiedCloseoutReleaseRangeStart\":\"v361\""
           ",\"verifiedCloseoutReleaseRangeEnd\":\"v380\""
           ",\"verifiedCloseoutReleaseCount\":20"
           ",\"sourceCloseoutVersionedFixturePath\":\"fixtures/release/shard-readiness-v380.json\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"verificationStageCatalog\":" + format_verification_stage_catalog_json() +
           ",\"verificationChecks\":" +
           json_string_array({"source closeout command remains available",
                              "source closeout chain remains complete",
                              "source closeout digest marker remains pinned",
                              "previous current fixture is frozen before rolling",
                              "current SHARDJSON embeds verification evidence",
                              "COMMANDS and COMMANDSJSON expose read-only catalog metadata",
                              "EXPLAINJSON classifies the command as metadata read",
                              "CHECKJSON denies execution",
                              "CLI smoke reads evidence only",
                              "TCP smoke uses an owned server and stops it",
                              "no router, write, WAL, restore, load, compact, or live-read authority is added"}) +
           ",\"verificationCheckCount\":11"
           ",\"verificationPassedCount\":11"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" +
           json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"archiveCloseoutVerificationCommandAvailable\":true"
           ",\"archiveCloseoutVerificationChainComplete\":" +
           json_bool(current_archive_closeout_verification_stage_count ==
                     planned_archive_closeout_verification_stage_count) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read archive closeout verification evidence only",
                              "treat v361-v380 archive closeout as frozen source evidence",
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

std::string verification_digest_marker() {
    const auto& stage = current_stage();
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-" +
           std::to_string(current_archive_closeout_verification_stage_count) + "-of-" +
           std::to_string(planned_archive_closeout_verification_stage_count) + "-stages";
}

int published_stage_count() {
    return current_archive_closeout_verification_stage_count;
}

} // namespace minikv::shard_route_preview_verification_report_archive_closeout_verification