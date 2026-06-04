#include "minikv/shard_route_preview_verification_report_archive.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_verification_report_closeout.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification_report_archive {
namespace {

constexpr int current_archive_stage_count = 14;
constexpr int planned_archive_stage_count = 20;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

struct ArchiveStage {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

constexpr std::array<ArchiveStage, planned_archive_stage_count> archive_stages = {{
    {1,
     "v341",
     "route-preview-verification-report-archive-manifest",
     "v340",
     "fixtures/release/shard-readiness-v340.json",
     "publishes a read-only manifest for the v320-v340 route preview verification report evidence chain"},
    {2,
     "v342",
     "route-preview-verification-report-archive-verification",
     "v341",
     "fixtures/release/shard-readiness-v341.json",
     "verifies the manifest fields without reading archive files or enabling execution"},
    {3,
     "v343",
     "route-preview-verification-report-archive-index",
     "v342",
     "fixtures/release/shard-readiness-v342.json",
     "indexes the archive manifest stages for downstream read-only consumers"},
    {4,
     "v344",
     "route-preview-verification-report-archive-index-audit",
     "v343",
     "fixtures/release/shard-readiness-v343.json",
     "audits archive index continuity while preserving no-router boundaries"},
    {5,
     "v345",
     "route-preview-verification-report-archive-retention-map",
     "v344",
     "fixtures/release/shard-readiness-v344.json",
     "maps retained fixtures, screenshots, explanations, and walkthroughs as evidence metadata"},
    {6,
     "v346",
     "route-preview-verification-report-archive-retention-audit",
     "v345",
     "fixtures/release/shard-readiness-v345.json",
     "audits retention metadata counts without scanning runtime directories"},
    {7,
     "v347",
     "route-preview-verification-report-archive-continuity-map",
     "v346",
     "fixtures/release/shard-readiness-v346.json",
     "maps v320-v340 continuity from report rollout through closeout and archive"},
    {8,
     "v348",
     "route-preview-verification-report-archive-continuity-audit",
     "v347",
     "fixtures/release/shard-readiness-v347.json",
     "audits archive continuity and keeps the evidence contract read-only"},
    {9,
     "v349",
     "route-preview-verification-report-archive-consumer-handoff",
     "v348",
     "fixtures/release/shard-readiness-v348.json",
     "records Node consumer handoff semantics for the archive manifest"},
    {10,
     "v350",
     "route-preview-verification-report-archive-consumer-handoff-audit",
     "v349",
     "fixtures/release/shard-readiness-v349.json",
     "audits consumer handoff boundaries and no live-read requirements"},
    {11,
     "v351",
     "route-preview-verification-report-archive-java-echo-handoff",
     "v350",
     "fixtures/release/shard-readiness-v350.json",
     "records Java echo handoff semantics for archive fields"},
    {12,
     "v352",
     "route-preview-verification-report-archive-java-echo-audit",
     "v351",
     "fixtures/release/shard-readiness-v351.json",
     "audits Java echo expectations without granting mini-kv authority"},
    {13,
     "v353",
     "route-preview-verification-report-archive-fixture-freeze",
     "v352",
     "fixtures/release/shard-readiness-v352.json",
     "freezes archive manifest fixture semantics for historical replay"},
    {14,
     "v354",
     "route-preview-verification-report-archive-cli-smoke-contract",
     "v353",
     "fixtures/release/shard-readiness-v353.json",
     "records CLI smoke expectations for the archive command"},
    {15,
     "v355",
     "route-preview-verification-report-archive-tcp-smoke-contract",
     "v354",
     "fixtures/release/shard-readiness-v354.json",
     "records TCP smoke expectations for the archive command"},
    {16,
     "v356",
     "route-preview-verification-report-archive-no-router-audit",
     "v355",
     "fixtures/release/shard-readiness-v355.json",
     "audits that archive evidence installs no active router"},
    {17,
     "v357",
     "route-preview-verification-report-archive-no-write-audit",
     "v356",
     "fixtures/release/shard-readiness-v356.json",
     "audits that archive evidence permits no writes"},
    {18,
     "v358",
     "route-preview-verification-report-archive-no-execution-audit",
     "v357",
     "fixtures/release/shard-readiness-v357.json",
     "audits that archive evidence permits no execution"},
    {19,
     "v359",
     "route-preview-verification-report-archive-release-package",
     "v358",
     "fixtures/release/shard-readiness-v358.json",
     "packages archive manifest evidence for release consumption"},
    {20,
     "v360",
     "route-preview-verification-report-archive-closeout-audit",
     "v359",
     "fixtures/release/shard-readiness-v359.json",
     "audits final archive closeout continuity for the twenty-version archive chain"},
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

const ArchiveStage& current_stage() {
    return archive_stages.at(static_cast<std::size_t>(current_archive_stage_count - 1));
}

std::string format_stage_json(const ArchiveStage& stage) {
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
    for (int index = 0; index < current_archive_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(archive_stages.at(static_cast<std::size_t>(index)));
    }
    result += "]";
    return result;
}

std::string format_archive_json() {
    const auto& stage = current_stage();
    return "{\"contract\":\"shard-route-preview-verification-report-archive.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\","
           "\"archiveMode\":\"read-only-route-preview-verification-report-archive-chain\","
           "\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceCloseoutCommand\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\""
           ",\"sourceReportCommand\":\"SHARDROUTEVERIFYREPORTJSON\""
           ",\"sourceVerificationCommand\":\"SHARDROUTEVERIFYJSON\""
           ",\"sourcePreviewCommand\":\"SHARDROUTEJSON\""
           ",\"sourceCloseoutReleaseVersion\":\"v340\""
           ",\"sourceCloseoutFixturePath\":\"fixtures/release/shard-readiness-v340.json\""
           ",\"archiveStage\":" +
           json_string(stage.stage) +
           ",\"archiveStageSequence\":" + std::to_string(stage.sequence) +
           ",\"archiveReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_archive_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_archive_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"archivedReportReleaseRangeStart\":\"v320\""
           ",\"archivedReportReleaseRangeEnd\":\"v339\""
           ",\"archivedCloseoutReleaseVersion\":\"v340\""
           ",\"archivedReportReleaseCount\":20"
           ",\"archivedCloseoutReleaseCount\":1"
           ",\"expectedCommandScreenshotFilesPerVersion\":5"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"versionedCurrentFixturePath\":\"fixtures/release/shard-readiness-v340.json\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"closeoutDigestMarker\":" +
           json_string(shard_route_preview_verification_report_closeout::closeout_digest_marker()) +
           ",\"archiveStageCatalog\":" + format_stage_catalog_json() +
           ",\"archiveManifestPublished\":true"
           ",\"archiveVerificationPlannedReleaseVersion\":\"v342\""
           ",\"archiveVerificationPublished\":" + json_bool(current_archive_stage_count >= 2) +
           ",\"archiveChainComplete\":" + json_bool(current_archive_stage_count == planned_archive_stage_count) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read archive evidence manifest only",
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

std::string archive_digest_marker() {
    const auto& stage = current_stage();
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-" +
           std::to_string(current_archive_stage_count) + "-of-" + std::to_string(planned_archive_stage_count) +
           "-stages";
}

int published_stage_count() {
    return current_archive_stage_count;
}

} // namespace minikv::shard_route_preview_verification_report_archive
