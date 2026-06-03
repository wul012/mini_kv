#include "minikv/shard_route_preview_verification_report.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_verification.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification_report {
namespace {

constexpr int current_report_stage_count = 8;

struct ReportStage {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

constexpr std::array<ReportStage, 20> report_stages = {{
    {1,
     "v320",
     "route-preview-verification-report-command-contract",
     "v319",
     "fixtures/release/shard-readiness-v319.json",
     "adds read-only SHARDROUTEVERIFYREPORTJSON verification report command"},
    {2,
     "v321",
     "route-preview-verification-report-command-catalog-alignment",
     "v320",
     "fixtures/release/shard-readiness-v320.json",
     "aligns route preview verification report with command catalog and help surfaces"},
    {3,
     "v322",
     "route-preview-verification-report-explain-contract",
     "v321",
     "fixtures/release/shard-readiness-v321.json",
     "freezes EXPLAINJSON route preview verification report semantics as metadata read only"},
    {4,
     "v323",
     "route-preview-verification-report-checkjson-contract",
     "v322",
     "fixtures/release/shard-readiness-v322.json",
     "freezes CHECKJSON route preview verification report no-execution contract"},
    {5,
     "v324",
     "route-preview-verification-report-cli-smoke-contract",
     "v323",
     "fixtures/release/shard-readiness-v323.json",
     "records CLI smoke expectations for read-only route preview verification report"},
    {6,
     "v325",
     "route-preview-verification-report-tcp-smoke-contract",
     "v324",
     "fixtures/release/shard-readiness-v324.json",
     "records TCP smoke expectations for read-only route preview verification report"},
    {7,
     "v326",
     "route-preview-verification-report-fixture-freeze",
     "v325",
     "fixtures/release/shard-readiness-v325.json",
     "freezes route preview verification report rollout fixture baseline"},
    {8,
     "v327",
     "route-preview-verification-report-sample-parity-audit",
     "v326",
     "fixtures/release/shard-readiness-v326.json",
     "audits report samples against route preview verification samples"},
    {9,
     "v328",
     "route-preview-verification-report-rollout-catalog",
     "v327",
     "fixtures/release/shard-readiness-v327.json",
     "catalogs route preview verification report rollout stages"},
    {10,
     "v329",
     "route-preview-verification-report-rollout-catalog-audit",
     "v328",
     "fixtures/release/shard-readiness-v328.json",
     "audits route preview verification report rollout catalog continuity"},
    {11,
     "v330",
     "route-preview-verification-report-history-fallback-freeze",
     "v329",
     "fixtures/release/shard-readiness-v329.json",
     "freezes route preview verification report historical fallback source"},
    {12,
     "v331",
     "route-preview-verification-report-operator-handoff",
     "v330",
     "fixtures/release/shard-readiness-v330.json",
     "documents operator handoff for read-only route preview verification report"},
    {13,
     "v332",
     "route-preview-verification-report-node-consumer-handoff",
     "v331",
     "fixtures/release/shard-readiness-v331.json",
     "documents Node consumer handoff for route preview verification report evidence"},
    {14,
     "v333",
     "route-preview-verification-report-java-echo-handoff",
     "v332",
     "fixtures/release/shard-readiness-v332.json",
     "documents Java echo handoff for route preview verification report evidence"},
    {15,
     "v334",
     "route-preview-verification-report-boundary-field-audit",
     "v333",
     "fixtures/release/shard-readiness-v333.json",
     "audits route preview verification report read-only boundary fields"},
    {16,
     "v335",
     "route-preview-verification-report-no-router-audit",
     "v334",
     "fixtures/release/shard-readiness-v334.json",
     "audits that route preview verification report still installs no active router"},
    {17,
     "v336",
     "route-preview-verification-report-no-write-audit",
     "v335",
     "fixtures/release/shard-readiness-v335.json",
     "audits that route preview verification report still performs no writes"},
    {18,
     "v337",
     "route-preview-verification-report-no-execution-audit",
     "v336",
     "fixtures/release/shard-readiness-v336.json",
     "audits that route preview verification report still permits no execution"},
    {19,
     "v338",
     "route-preview-verification-report-release-package",
     "v337",
     "fixtures/release/shard-readiness-v337.json",
     "packages route preview verification report rollout evidence for release consumption"},
    {20,
     "v339",
     "route-preview-verification-report-closeout-audit",
     "v338",
     "fixtures/release/shard-readiness-v338.json",
     "audits route preview verification report closeout continuity"},
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

const ReportStage& current_stage() {
    return report_stages.at(static_cast<std::size_t>(current_report_stage_count - 1));
}

std::string format_stage_json(const ReportStage& stage) {
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

std::string format_stage_catalog_json() {
    std::string result = "[";
    for (int index = 0; index < current_report_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(report_stages.at(static_cast<std::size_t>(index)));
    }
    result += "]";
    return result;
}

} // namespace

std::string format_report_json(std::string_view key) {
    const auto verification = shard_route_preview_verification::verify_key(key);
    const auto& stage = current_stage();
    const bool passed = verification.passed_count == verification.check_count;

    return "{\"contract\":\"shard-route-preview-verification-report.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTJSON\","
           "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\","
           "\"previewCommand\":\"SHARDROUTEJSON\","
           "\"reportMode\":\"read-only-route-preview-verification-package\","
           "\"key\":" + json_string(verification.preview.key) +
           ",\"slot\":" + std::to_string(verification.preview.slot) +
           ",\"slotCount\":16"
           ",\"shardId\":" + json_string(verification.preview.shard_id) +
           ",\"routeMode\":" + json_string(verification.preview.route_mode) +
           ",\"previewHash\":" + json_string(verification.preview.preview_hash) +
           ",\"pinnedFixtureSample\":" + json_bool(verification.preview.pinned_fixture_sample) +
           ",\"verificationPassed\":" + json_bool(passed) +
           ",\"checkCount\":" + std::to_string(verification.check_count) +
           ",\"passedCount\":" + std::to_string(verification.passed_count) +
           ",\"failedCount\":" + std::to_string(verification.check_count - verification.passed_count) +
           ",\"reportStage\":" + json_string(stage.stage) +
           ",\"reportStageSequence\":" + std::to_string(stage.sequence) +
           ",\"reportReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"readOnly\":true"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"executesRoute\":false"
           ",\"storagePath\":\"not-created\""
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"startsServices\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"liveReadAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string format_rollout_json() {
    const auto& stage = current_stage();
    return "{\"rolloutMode\":\"read-only-shard-route-preview-verification-report-rollout\","
           "\"sourceNodePlan\":\"docs/plans3/v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"rolloutStage\":" + json_string(stage.stage) +
           ",\"rolloutStageSequence\":" + std::to_string(stage.sequence) +
           ",\"rolloutReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_report_stage_count) +
           ",\"latestStageScope\":" + json_string(stage.scope) +
           ",\"reportCommandAvailable\":true"
           ",\"reportedCommand\":\"SHARDROUTEVERIFYJSON\""
           ",\"previewCommand\":\"SHARDROUTEJSON\""
           ",\"commands\":" + json_string_array({"SHARDROUTEVERIFYREPORTJSON"}) +
           ",\"sampleKeys\":" + json_string_array({"orderops:alpha", "audit:receipt:001", "mini-kv:health"}) +
           ",\"checks\":" + json_string_array({"slot_within_range",
                                               "single_shard",
                                               "preview_only",
                                               "pinned_sample_consistent",
                                               "hash_present",
                                               "read_only",
                                               "no_active_router",
                                               "no_write_routing",
                                               "no_execution"}) +
           ",\"stageCatalog\":" + format_stage_catalog_json() +
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

std::string rollout_digest_marker() {
    const auto& stage = current_stage();
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-stage-" +
           std::to_string(stage.sequence);
}

} // namespace minikv::shard_route_preview_verification_report
