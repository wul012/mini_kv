#include "minikv/shard_route_preview_verification_report_archive_closeout.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_verification_report_archive.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification_report_archive_closeout {
namespace {

constexpr int current_archive_closeout_stage_count = 9;
constexpr int planned_archive_closeout_stage_count = 20;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

struct ArchiveCloseoutStage {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

constexpr std::array<ArchiveCloseoutStage, planned_archive_closeout_stage_count> archive_closeout_stages = {{
    {1,
     "v361",
     "route-preview-verification-report-archive-closeout-command-contract",
     "v360",
     "fixtures/release/shard-readiness-v360.json",
     "publishes a read-only closeout command for the v341-v360 route preview verification report archive chain"},
    {2,
     "v362",
     "route-preview-verification-report-archive-closeout-catalog-alignment",
     "v361",
     "fixtures/release/shard-readiness-v361.json",
     "aligns the closeout command with COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON"},
    {3,
     "v363",
     "route-preview-verification-report-archive-closeout-explain-contract",
     "v362",
     "fixtures/release/shard-readiness-v362.json",
     "pins EXPLAINJSON semantics for the archive closeout command"},
    {4,
     "v364",
     "route-preview-verification-report-archive-closeout-checkjson-contract",
     "v363",
     "fixtures/release/shard-readiness-v363.json",
     "pins CHECKJSON semantics for the archive closeout command"},
    {5,
     "v365",
     "route-preview-verification-report-archive-closeout-cli-smoke-contract",
     "v364",
     "fixtures/release/shard-readiness-v364.json",
     "records CLI smoke expectations for the archive closeout command"},
    {6,
     "v366",
     "route-preview-verification-report-archive-closeout-tcp-smoke-contract",
     "v365",
     "fixtures/release/shard-readiness-v365.json",
     "records TCP smoke expectations for the archive closeout command"},
    {7,
     "v367",
     "route-preview-verification-report-archive-closeout-fixture-freeze",
     "v366",
     "fixtures/release/shard-readiness-v366.json",
     "freezes archive closeout fixture semantics for historical replay"},
    {8,
     "v368",
     "route-preview-verification-report-archive-closeout-index",
     "v367",
     "fixtures/release/shard-readiness-v367.json",
     "indexes the archive closeout stages for downstream read-only consumers"},
    {9,
     "v369",
     "route-preview-verification-report-archive-closeout-index-audit",
     "v368",
     "fixtures/release/shard-readiness-v368.json",
     "audits archive closeout index continuity while preserving no-router boundaries"},
    {10,
     "v370",
     "route-preview-verification-report-archive-closeout-retention-map",
     "v369",
     "fixtures/release/shard-readiness-v369.json",
     "maps retained fixtures, screenshots, explanations, and walkthroughs as closeout evidence metadata"},
    {11,
     "v371",
     "route-preview-verification-report-archive-closeout-retention-audit",
     "v370",
     "fixtures/release/shard-readiness-v370.json",
     "audits closeout retention metadata without scanning runtime directories"},
    {12,
     "v372",
     "route-preview-verification-report-archive-closeout-consumer-handoff",
     "v371",
     "fixtures/release/shard-readiness-v371.json",
     "records Node consumer handoff semantics for archive closeout fields"},
    {13,
     "v373",
     "route-preview-verification-report-archive-closeout-consumer-handoff-audit",
     "v372",
     "fixtures/release/shard-readiness-v372.json",
     "audits consumer handoff boundaries and no live-read requirements"},
    {14,
     "v374",
     "route-preview-verification-report-archive-closeout-java-echo-handoff",
     "v373",
     "fixtures/release/shard-readiness-v373.json",
     "records Java echo handoff semantics for archive closeout fields"},
    {15,
     "v375",
     "route-preview-verification-report-archive-closeout-java-echo-audit",
     "v374",
     "fixtures/release/shard-readiness-v374.json",
     "audits Java echo expectations without granting mini-kv authority"},
    {16,
     "v376",
     "route-preview-verification-report-archive-closeout-boundary-field-audit",
     "v375",
     "fixtures/release/shard-readiness-v375.json",
     "audits boundary fields for read-only archive closeout consumption"},
    {17,
     "v377",
     "route-preview-verification-report-archive-closeout-no-router-audit",
     "v376",
     "fixtures/release/shard-readiness-v376.json",
     "audits that archive closeout evidence installs no active router"},
    {18,
     "v378",
     "route-preview-verification-report-archive-closeout-no-write-audit",
     "v377",
     "fixtures/release/shard-readiness-v377.json",
     "audits that archive closeout evidence permits no writes"},
    {19,
     "v379",
     "route-preview-verification-report-archive-closeout-no-execution-audit",
     "v378",
     "fixtures/release/shard-readiness-v378.json",
     "audits that archive closeout evidence permits no execution"},
    {20,
     "v380",
     "route-preview-verification-report-archive-closeout-release-package",
     "v379",
     "fixtures/release/shard-readiness-v379.json",
     "packages final archive closeout evidence for release consumption"},
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

const ArchiveCloseoutStage& current_stage() {
    return archive_closeout_stages.at(static_cast<std::size_t>(current_archive_closeout_stage_count - 1));
}

std::string format_stage_json(const ArchiveCloseoutStage& stage) {
    return "{\"sequence\":" + std::to_string(stage.sequence) +
           ",\"releaseVersion\":" + json_string(stage.release_version) +
           ",\"stage\":" + json_string(stage.stage) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"scope\":" + json_string(stage.scope) +
           ",\"readOnly\":true"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::string format_stage_catalog_json() {
    std::string result = "[";
    for (int index = 0; index < current_archive_closeout_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(archive_closeout_stages.at(static_cast<std::size_t>(index)));
    }
    result += "]";
    return result;
}

std::string format_closeout_json() {
    const auto& stage = current_stage();
    return "{\"contract\":\"shard-route-preview-verification-report-archive-closeout.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\","
           "\"closeoutMode\":\"read-only-route-preview-verification-report-archive-closeout-chain\","
           "\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceArchiveCommand\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\""
           ",\"sourceCloseoutCommand\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\""
           ",\"sourceReportCommand\":\"SHARDROUTEVERIFYREPORTJSON\""
           ",\"sourceVerificationCommand\":\"SHARDROUTEVERIFYJSON\""
           ",\"sourcePreviewCommand\":\"SHARDROUTEJSON\""
           ",\"sourceArchiveReleaseVersion\":\"v360\""
           ",\"sourceArchiveFixturePath\":\"fixtures/release/shard-readiness-v360.json\""
           ",\"sourceArchivePublishedStageCount\":" +
           std::to_string(shard_route_preview_verification_report_archive::published_stage_count()) +
           ",\"sourceArchivePlannedStageCount\":20"
           ",\"sourceArchiveChainComplete\":" +
           json_bool(shard_route_preview_verification_report_archive::published_stage_count() == 20) +
           ",\"sourceArchiveDigestMarker\":" +
           json_string(shard_route_preview_verification_report_archive::archive_digest_marker()) +
           ",\"closeoutStage\":" +
           json_string(stage.stage) +
           ",\"closeoutStageSequence\":" + std::to_string(stage.sequence) +
           ",\"closeoutReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_archive_closeout_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_archive_closeout_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"archivedArchiveReleaseRangeStart\":\"v341\""
           ",\"archivedArchiveReleaseRangeEnd\":\"v360\""
           ",\"archivedArchiveReleaseCount\":20"
           ",\"archivedReportReleaseRangeStart\":\"v320\""
           ",\"archivedReportReleaseRangeEnd\":\"v339\""
           ",\"archivedReportReleaseCount\":20"
           ",\"archivedReportCloseoutReleaseVersion\":\"v340\""
           ",\"expectedCommandScreenshotFilesPerVersion\":5"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"versionedCurrentFixturePath\":\"fixtures/release/shard-readiness-v360.json\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"closeoutStageCatalog\":" + format_stage_catalog_json() +
           ",\"archiveCloseoutCommandAvailable\":true"
           ",\"archiveCloseoutChainComplete\":" +
           json_bool(current_archive_closeout_stage_count == planned_archive_closeout_stage_count) +
           ",\"archiveCloseoutReleasePackagePlannedReleaseVersion\":\"v380\""
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read archive closeout evidence only",
                              "treat the v341-v360 archive chain as closed evidence",
                              "do not install route writers",
                              "do not run restore, load, or compact",
                              "do not start mini-kv service from evidence review",
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
    const auto& stage = current_stage();
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-" +
           std::to_string(current_archive_closeout_stage_count) + "-of-" +
           std::to_string(planned_archive_closeout_stage_count) + "-stages";
}

int published_stage_count() {
    return current_archive_closeout_stage_count;
}

} // namespace minikv::shard_route_preview_verification_report_archive_closeout
