#include "minikv/shard_route_preview_verification_report_closeout.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_verification_report.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification_report_closeout {
namespace {

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";
constexpr std::string_view closeout_release_version = "v340";
constexpr std::string_view closed_range_start = "v320";
constexpr std::string_view closed_range_end = "v339";
constexpr std::string_view frozen_fixture_range_start = "v319";
constexpr std::string_view frozen_fixture_range_end = "v338";
constexpr int closed_version_count = 20;
constexpr int report_stage_count = 20;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_closeout_json() {
    return "{\"contract\":\"shard-route-preview-verification-report-closeout.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\","
           "\"closeoutMode\":\"read-only-route-preview-verification-report-closeout-summary\","
           "\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceReportCommand\":\"SHARDROUTEVERIFYREPORTJSON\""
           ",\"sourceVerificationCommand\":\"SHARDROUTEVERIFYJSON\""
           ",\"sourcePreviewCommand\":\"SHARDROUTEJSON\""
           ",\"closedReleaseRangeStart\":" +
           json_string(closed_range_start) +
           ",\"closedReleaseRangeEnd\":" + json_string(closed_range_end) +
           ",\"closedVersionCount\":" + std::to_string(closed_version_count) +
           ",\"reportStageCount\":" + std::to_string(report_stage_count) +
           ",\"latestReportReleaseVersion\":\"v339\""
           ",\"latestReportStage\":\"route-preview-verification-report-closeout-audit\""
           ",\"frozenFixtureRangeStart\":" +
           json_string(frozen_fixture_range_start) +
           ",\"frozenFixtureRangeEnd\":" + json_string(frozen_fixture_range_end) +
           ",\"versionedCurrentFixturePath\":\"fixtures/release/shard-readiness-v339.json\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"rolloutDigestMarker\":" +
           json_string(shard_route_preview_verification_report::rollout_digest_marker()) +
           ",\"closedStages\":" +
           json_string_array({"command-contract",
                              "command-catalog-alignment",
                              "explain-contract",
                              "checkjson-contract",
                              "cli-smoke-contract",
                              "tcp-smoke-contract",
                              "fixture-freeze",
                              "sample-parity-audit",
                              "rollout-catalog",
                              "rollout-catalog-audit",
                              "history-fallback-freeze",
                              "operator-handoff",
                              "node-consumer-handoff",
                              "java-echo-handoff",
                              "boundary-field-audit",
                              "no-router-audit",
                              "no-write-audit",
                              "no-execution-audit",
                              "release-package",
                              "closeout-audit"}) +
           ",\"allReportStagesPublished\":true"
           ",\"stageCatalogClosed\":true"
           ",\"archiveManifestPlannedReleaseVersion\":\"v341\""
           ",\"archiveVerificationPlannedReleaseVersion\":\"v342\""
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
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
    return std::string{closeout_release_version} + "-route-preview-verification-report-closeout-summary-" +
           std::to_string(closed_version_count) + "-versions";
}

} // namespace minikv::shard_route_preview_verification_report_closeout
