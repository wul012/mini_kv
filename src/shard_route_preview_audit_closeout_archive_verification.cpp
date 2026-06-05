#include "minikv/shard_route_preview_audit_closeout_archive_verification.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_verification_report_archive_closeout_verification_audit_closeout.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_audit_closeout_archive_verification {
namespace {

constexpr int current_audit_closeout_archive_verification_stage_count = 17;
constexpr int planned_audit_closeout_archive_verification_stage_count = 20;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v549-post-java-mini-kv-route-catalog-cleanup-latest-sibling-live-smoke-archive-verification-route-archive-verification-roadmap.md";

using shard_route_preview_stage_catalog::StageRecord;

constexpr std::array<StageRecord, planned_audit_closeout_archive_verification_stage_count>
    audit_closeout_archive_verification_stages = {{
        {1,
         "v441",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-intake",
         "v440",
         "fixtures/release/shard-readiness-v440.json",
         "starts a read-only archive verification layer over the closed v421-v440 audit closeout package"},
        {2,
         "v442",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-catalog-alignment",
         "v441",
         "fixtures/release/shard-readiness-v441.json",
         "aligns archive verification command metadata with COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON"},
        {3,
         "v443",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-explain-contract",
         "v442",
         "fixtures/release/shard-readiness-v442.json",
         "pins EXPLAINJSON semantics for archive verification as metadata-only evidence"},
        {4,
         "v444",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-checkjson-contract",
         "v443",
         "fixtures/release/shard-readiness-v443.json",
         "pins CHECKJSON semantics so archive verification remains non-executable"},
        {5,
         "v445",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-cli-smoke-contract",
         "v444",
         "fixtures/release/shard-readiness-v444.json",
         "records CLI smoke expectations for archive verification without opening routing or writes"},
        {6,
         "v446",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-tcp-smoke-contract",
         "v445",
         "fixtures/release/shard-readiness-v445.json",
         "records TCP smoke expectations and requires the owned mini-kv server to stop after validation"},
        {7,
         "v447",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-fixture-freeze",
         "v446",
         "fixtures/release/shard-readiness-v446.json",
         "freezes the prior shard readiness fixture before rolling archive verification evidence forward"},
        {8,
         "v448",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-source-closeout-index",
         "v447",
         "fixtures/release/shard-readiness-v447.json",
         "indexes the closed v421-v440 audit closeout range for read-only downstream archive checks"},
        {9,
         "v449",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-source-closeout-index-audit",
         "v448",
         "fixtures/release/shard-readiness-v448.json",
         "audits source closeout index continuity without walking archive directories at runtime"},
        {10,
         "v450",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-retention-map",
         "v449",
         "fixtures/release/shard-readiness-v449.json",
         "maps retained screenshots, explanations, walkthroughs, fixtures, commits, tags, and CI evidence hints"},
        {11,
         "v451",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-retention-audit",
         "v450",
         "fixtures/release/shard-readiness-v450.json",
         "audits retention metadata while preserving no filesystem archive walk and no runtime probe boundaries"},
        {12,
         "v452",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-consumer-handoff",
         "v451",
         "fixtures/release/shard-readiness-v451.json",
         "records downstream consumer handoff semantics for archive verification fields"},
        {13,
         "v453",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-consumer-handoff-audit",
         "v452",
         "fixtures/release/shard-readiness-v452.json",
         "audits consumer handoff semantics while keeping live reads and service starts blocked"},
        {14,
         "v454",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-java-echo-handoff",
         "v453",
         "fixtures/release/shard-readiness-v453.json",
         "records Java echo handoff semantics without giving mini-kv order or audit authority"},
        {15,
         "v455",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-java-echo-audit",
         "v454",
         "fixtures/release/shard-readiness-v454.json",
         "audits Java echo expectations while preserving the read-only archive verification contract"},
        {16,
         "v456",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-boundary-field-audit",
         "v455",
         "fixtures/release/shard-readiness-v455.json",
         "audits boundary fields for readOnly, executionAllowed, router, write, WAL, and service-start semantics"},
        {17,
         "v457",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-no-router-audit",
         "v456",
         "fixtures/release/shard-readiness-v456.json",
         "audits that archive verification evidence installs no active shard router"},
        {18,
         "v458",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-no-write-audit",
         "v457",
         "fixtures/release/shard-readiness-v457.json",
         "audits that archive verification evidence permits no writes and touches no WAL"},
        {19,
         "v459",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-no-execution-audit",
         "v458",
         "fixtures/release/shard-readiness-v458.json",
         "audits that archive verification cannot trigger restore, load, compact, routing, or service execution"},
        {20,
         "v460",
         "route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-release-package",
         "v459",
         "fixtures/release/shard-readiness-v459.json",
         "packages final v441-v460 archive verification evidence for read-only downstream consumption"},
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
    return audit_closeout_archive_verification_stages.at(
        static_cast<std::size_t>(current_audit_closeout_archive_verification_stage_count - 1));
}

} // namespace

std::string format_archive_verification_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        audit_closeout_archive_verification_stages,
        current_audit_closeout_archive_verification_stage_count);
}

std::string format_archive_verification_json() {
    const auto& stage = current_stage();
    const bool source_chain_complete =
        shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::
            published_stage_count() == 20;
    return "{\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification.v1\","
           "\"project\":\"mini-kv\","
           "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\","
           "\"archiveVerificationMode\":\"read-only-route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification\","
           "\"sourceNodePlan\":" +
           json_string(source_node_plan) +
           ",\"sourceAuditCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\""
           ",\"sourceAuditCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\""
           ",\"sourceVerificationCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\""
           ",\"sourceArchiveCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\""
           ",\"sourceAuditCloseoutReleaseVersion\":\"v440\""
           ",\"sourceAuditCloseoutFixturePath\":\"fixtures/release/shard-readiness-v440.json\""
           ",\"sourceAuditCloseoutArchiveRootHint\":\"e/440/\""
           ",\"sourceAuditCloseoutPublishedStageCount\":" +
           std::to_string(
               shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::
                   published_stage_count()) +
           ",\"sourceAuditCloseoutPlannedStageCount\":20"
           ",\"sourceAuditCloseoutChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourceAuditCloseoutDigestMarker\":" +
           json_string(
               shard_route_preview_verification_report_archive_closeout_verification_audit_closeout::
                   closeout_digest_marker()) +
           ",\"archiveVerificationStage\":" +
           json_string(stage.stage) +
           ",\"archiveVerificationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"archiveVerificationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" +
           std::to_string(current_audit_closeout_archive_verification_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(planned_audit_closeout_archive_verification_stage_count) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"verifiedAuditCloseoutReleaseRangeStart\":\"v421\""
           ",\"verifiedAuditCloseoutReleaseRangeEnd\":\"v440\""
           ",\"verifiedAuditCloseoutReleaseCount\":20"
           ",\"sourceAuditCloseoutVersionedFixturePath\":\"fixtures/release/shard-readiness-v440.json\""
           ",\"currentFixturePath\":\"fixtures/release/shard-readiness.json\""
           ",\"expectedCommandScreenshotFilesPerVersion\":6"
           ",\"expectedExplanationFilesPerVersion\":1"
           ",\"walkthroughVolume\":\"production-prototype-stage-volume-3\""
           ",\"sharedStageCatalogFormatter\":\"shard_route_preview_stage_catalog\""
           ",\"archiveVerificationStageCatalog\":" +
           format_archive_verification_stage_catalog_json() +
           ",\"archiveVerificationChecks\":" +
           json_string_array({"source audit closeout command remains available",
                              "source audit closeout chain remains complete",
                              "source audit closeout digest marker remains pinned",
                              "source v421-v440 release range remains declared and versioned",
                              "archive root is a retained evidence hint, not a runtime filesystem walk",
                              "previous current fixture is frozen before rolling",
                              "current SHARDJSON embeds exactly one archive verification section",
                              "COMMANDS and COMMANDSJSON expose read-only catalog metadata",
                              "EXPLAINJSON classifies the archive verification command as metadata read",
                              "CHECKJSON denies execution",
                              "CLI smoke reads archive verification evidence only",
                              "TCP smoke uses an owned server and stops it",
                              "no router, write, WAL, restore, load, compact, or live-read authority is added"}) +
           ",\"archiveVerificationCheckCount\":13"
           ",\"archiveVerificationPassedCount\":13"
           ",\"sourceCommandAvailable\":true"
           ",\"sourceChainComplete\":" +
           json_bool(source_chain_complete) +
           ",\"sourcePublishedStageCountMatchesPlan\":" +
           json_bool(source_chain_complete) +
           ",\"sourceDigestMarkerPinned\":true"
           ",\"sharedStageCatalogFormatterApplied\":true"
           ",\"versionedFixtureExpected\":true"
           ",\"singleArchiveVerificationSectionExpected\":true"
           ",\"declaredArchiveVerificationOnly\":true"
           ",\"archiveRootHintUsedForHumanReviewOnly\":true"
           ",\"archiveCloseoutVerificationAuditCloseoutArchiveVerificationCommandAvailable\":true"
           ",\"archiveCloseoutVerificationAuditCloseoutArchiveVerificationChainComplete\":" +
           json_bool(current_audit_closeout_archive_verification_stage_count ==
                     planned_audit_closeout_archive_verification_stage_count) +
           ",\"controlPlaneInterpretation\":" +
           json_string_array({"read archive verification evidence only",
                              "treat v421-v440 audit closeout release package as frozen source evidence",
                              "use sourceFrozenFixturePath for historical comparison",
                              "treat sourceAuditCloseoutArchiveRootHint as a documentation pointer only",
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

std::string archive_verification_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_audit_closeout_archive_verification_stage_count,
        planned_audit_closeout_archive_verification_stage_count);
}

int published_stage_count() {
    return current_audit_closeout_archive_verification_stage_count;
}

} // namespace minikv::shard_route_preview_audit_closeout_archive_verification
