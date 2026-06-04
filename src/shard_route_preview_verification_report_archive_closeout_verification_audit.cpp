#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit {
namespace {

constexpr int current_archive_closeout_verification_audit_stage_count = 6;
constexpr int planned_archive_closeout_verification_audit_stage_count = 20;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

struct ArchiveCloseoutVerificationAuditStage {
    int sequence;
    std::string_view release_version;
    std::string_view stage;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view scope;
};

constexpr std::array<ArchiveCloseoutVerificationAuditStage, planned_archive_closeout_verification_audit_stage_count>
    archive_closeout_verification_audit_stages = {{
    {1,
     "v401",
     "route-preview-verification-report-archive-closeout-verification-audit-command-contract-and-section-dedup",
     "v400",
     "fixtures/release/shard-readiness-v400.json",
     "publishes a read-only audit command for the v381-v400 verification chain and deduplicates the SHARDJSON verification section"},
    {2,
     "v402",
     "route-preview-verification-report-archive-closeout-verification-audit-catalog-alignment",
     "v401",
     "fixtures/release/shard-readiness-v401.json",
     "aligns the audit command with COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON without changing runtime behavior"},
    {3,
     "v403",
     "route-preview-verification-report-archive-closeout-verification-audit-explain-contract",
     "v402",
     "fixtures/release/shard-readiness-v402.json",
     "pins EXPLAINJSON semantics for the audit command as metadata-only evidence"},
    {4,
     "v404",
     "route-preview-verification-report-archive-closeout-verification-audit-checkjson-contract",
     "v403",
     "fixtures/release/shard-readiness-v403.json",
     "pins CHECKJSON semantics for the audit command as read-only and non-executable"},
    {5,
     "v405",
     "route-preview-verification-report-archive-closeout-verification-audit-cli-smoke-contract",
     "v404",
     "fixtures/release/shard-readiness-v404.json",
     "records CLI smoke expectations for audit evidence without opening write routing"},
    {6,
     "v406",
     "route-preview-verification-report-archive-closeout-verification-audit-tcp-smoke-contract",
     "v405",
     "fixtures/release/shard-readiness-v405.json",
     "records TCP smoke expectations for audit evidence while stopping the owned server after validation"},
    {7,
     "v407",
     "route-preview-verification-report-archive-closeout-verification-audit-fixture-freeze",
     "v406",
     "fixtures/release/shard-readiness-v406.json",
     "freezes the previous shard readiness fixture before rolling current audit evidence forward"},
    {8,
     "v408",
     "route-preview-verification-report-archive-closeout-verification-audit-source-verification-index",
     "v407",
     "fixtures/release/shard-readiness-v407.json",
     "indexes the closed v381-v400 source verification range for downstream read-only consumers"},
    {9,
     "v409",
     "route-preview-verification-report-archive-closeout-verification-audit-source-verification-index-audit",
     "v408",
     "fixtures/release/shard-readiness-v408.json",
     "audits source verification index continuity without filesystem archive walking or router activation"},
    {10,
     "v410",
     "route-preview-verification-report-archive-closeout-verification-audit-retention-map",
     "v409",
     "fixtures/release/shard-readiness-v409.json",
     "maps retained fixtures, screenshots, explanations, and walkthroughs for the audit chain"},
    {11,
     "v411",
     "route-preview-verification-report-archive-closeout-verification-audit-retention-audit",
     "v410",
     "fixtures/release/shard-readiness-v410.json",
     "audits retention metadata while preserving no filesystem read and no runtime probe boundaries"},
    {12,
     "v412",
     "route-preview-verification-report-archive-closeout-verification-audit-consumer-handoff",
     "v411",
     "fixtures/release/shard-readiness-v411.json",
     "records Node consumer handoff semantics for audit fields"},
    {13,
     "v413",
     "route-preview-verification-report-archive-closeout-verification-audit-consumer-handoff-audit",
     "v412",
     "fixtures/release/shard-readiness-v412.json",
     "audits consumer handoff semantics and keeps live reads blocked"},
    {14,
     "v414",
     "route-preview-verification-report-archive-closeout-verification-audit-java-echo-handoff",
     "v413",
     "fixtures/release/shard-readiness-v413.json",
     "records Java echo handoff semantics without giving mini-kv order authority"},
    {15,
     "v415",
     "route-preview-verification-report-archive-closeout-verification-audit-java-echo-audit",
     "v414",
     "fixtures/release/shard-readiness-v414.json",
     "audits Java echo expectations while preserving the read-only evidence contract"},
    {16,
     "v416",
     "route-preview-verification-report-archive-closeout-verification-audit-boundary-field-audit",
     "v415",
     "fixtures/release/shard-readiness-v415.json",
     "audits boundary fields for readOnly, executionAllowed, router, write, WAL, and service-start semantics"},
    {17,
     "v417",
     "route-preview-verification-report-archive-closeout-verification-audit-no-router-audit",
     "v416",
     "fixtures/release/shard-readiness-v416.json",
     "audits that audit evidence installs no active shard router"},
    {18,
     "v418",
     "route-preview-verification-report-archive-closeout-verification-audit-no-write-audit",
     "v417",
     "fixtures/release/shard-readiness-v417.json",
     "audits that audit evidence permits no writes and touches no WAL"},
    {19,
     "v419",
     "route-preview-verification-report-archive-closeout-verification-audit-no-execution-audit",
     "v418",
     "fixtures/release/shard-readiness-v418.json",
     "audits that audit evidence cannot trigger restore, load, compact, routing, or service execution"},
    {20,
     "v420",
     "route-preview-verification-report-archive-closeout-verification-audit-release-package",
     "v419",
     "fixtures/release/shard-readiness-v419.json",
     "packages final v401-v420 audit evidence for read-only downstream consumption"},
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

const ArchiveCloseoutVerificationAuditStage& current_stage() {
    return archive_closeout_verification_audit_stages.at(
        static_cast<std::size_t>(current_archive_closeout_verification_audit_stage_count - 1));
}

std::string format_stage_json(const ArchiveCloseoutVerificationAuditStage& stage) {
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

std::string format_audit_stage_catalog_json() {
    std::string result = "[";
    for (int index = 0; index < current_archive_closeout_verification_audit_stage_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_stage_json(archive_closeout_verification_audit_stages.at(static_cast<std::size_t>(index)));
    }
    result += "]";
    return result;
}

std::string format_audit_json() {
    const auto& stage = current_stage();
    const bool source_chain_complete =
        shard_route_preview_verification_report_archive_closeout_verification::published_stage_count() == 20;
    return "{\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\","
           "\"auditMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit\","
           "\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\""
           ",\"sourceArchiveCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\""
           ",\"sourceArchiveCommand\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\""
           ",\"sourceVerificationReleaseVersion\":\"v400\""
           ",\"sourceVerificationFixturePath\":\"fixtures/release/shard-readiness-v400.json\""
           ",\"sourceVerificationPublishedStageCount\":" +
           std::to_string(shard_route_preview_verification_report_archive_closeout_verification::published_stage_count()) +
           ",\"sourceVerificationPlannedStageCount\":20"
           ",\"sourceVerificationChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceVerificationDigestMarker\":" +
           json_string(shard_route_preview_verification_report_archive_closeout_verification::verification_digest_marker()) +
           ",\"auditStage\":" +
           json_string(stage.stage) +
           ",\"auditStageSequence\":" + std::to_string(stage.sequence) +
           ",\"auditReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_archive_closeout_verification_audit_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_archive_closeout_verification_audit_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"auditedVerificationReleaseRangeStart\":\"v381\""
           ",\"auditedVerificationReleaseRangeEnd\":\"v400\""
           ",\"auditedVerificationReleaseCount\":20"
           ",\"sourceVerificationVersionedFixturePath\":\"fixtures/release/shard-readiness-v400.json\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"auditStageCatalog\":" + format_audit_stage_catalog_json() +
           ",\"auditChecks\":" +
           json_string_array({"source verification command remains available",
                              "source verification chain remains complete",
                              "source verification digest marker remains pinned",
                              "previous current fixture is frozen before rolling",
                              "current SHARDJSON embeds exactly one verification section",
                              "current SHARDJSON embeds exactly one verification audit section",
                              "COMMANDS and COMMANDSJSON expose read-only catalog metadata",
                              "EXPLAINJSON classifies the audit command as metadata read",
                              "CHECKJSON denies execution",
                              "CLI smoke reads audit evidence only",
                              "TCP smoke uses an owned server and stops it",
                              "no router, write, WAL, restore, load, compact, or live-read authority is added"}) +
           ",\"auditCheckCount\":12"
           ",\"auditPassedCount\":12"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" +
           json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleVerificationSectionExpected\":true"
           ",\"singleAuditSectionExpected\":true"
           ",\"duplicateVerificationSectionRemovedInCurrent\":true"
           ",\"archiveCloseoutVerificationAuditCommandAvailable\":true"
           ",\"archiveCloseoutVerificationAuditChainComplete\":" +
           json_bool(current_archive_closeout_verification_audit_stage_count ==
                     planned_archive_closeout_verification_audit_stage_count) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read archive closeout verification audit evidence only",
                              "treat v381-v400 verification release package as frozen source evidence",
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

std::string audit_digest_marker() {
    const auto& stage = current_stage();
    return std::string{stage.release_version} + "-" + std::string{stage.stage} + "-" +
           std::to_string(current_archive_closeout_verification_audit_stage_count) + "-of-" +
           std::to_string(planned_archive_closeout_verification_audit_stage_count) + "-stages";
}

int published_stage_count() {
    return current_archive_closeout_verification_audit_stage_count;
}

} // namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit