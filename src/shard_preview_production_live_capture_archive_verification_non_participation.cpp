#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_production_live_capture_archive_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_checks.hpp"
#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_stages.hpp"
#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_production_live_capture_archive_verification_non_participation_checks;
namespace source_integrity = shard_preview_production_live_capture_archive_non_participation;
namespace stages = shard_preview_production_live_capture_archive_verification_non_participation_stages;
namespace validation = shard_preview_production_live_capture_archive_verification_non_participation_validation;

constexpr std::string_view command_name =
    "SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1686-production-live-probe-read-only-window-capture-archive-verification-module-split-roadmap.md";
constexpr int current_stage_count = 35;
constexpr int source_integrity_planned_stage_count = 30;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_starts_capture_archive_verification = false;
constexpr bool mini_kv_reads_verification_endpoint = false;

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
    return stages::production_live_capture_archive_verification_non_participation_stages()[static_cast<std::size_t>(
        current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::production_live_capture_archive_verification_non_participation_stages(),
        current_stage_count,
        stages::planned_production_live_capture_archive_verification_non_participation_stage_count(),
        stages::first_production_live_capture_archive_verification_non_participation_release_number());
}

bool source_integrity_complete() {
    return source_integrity::published_stage_count() == source_integrity_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_production_live_capture_archive_verification_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::production_live_capture_archive_verification_non_participation_stages(),
        current_stage_count);
}

std::string production_live_capture_archive_verification_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_production_live_capture_archive_verification_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_integrity::published_stage_count();
    const bool source_complete = source_integrity_complete();
    const int planned_check_count =
        checks::planned_production_live_capture_archive_verification_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool non_participation_chain_complete =
        current_stage_count ==
            stages::planned_production_live_capture_archive_verification_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-preview-production-live-capture-archive-verification-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"productionLiveCaptureArchiveVerificationNonParticipationMode\":\"controlled-read-only-production-live-capture-archive-verification-non-participation-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveVerificationReleaseRange\":\"Node v1657-v1686\""
           ",\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveVerificationCloseoutReleaseVersion\":\"Node v1686\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeArchiveVerificationModuleSplitOnly\":true"
           ",\"sourceNodeHttpSmokeIsNodeOnly\":true"
           ",\"sourceNodeCaptureArchiveVerificationRequiresSiblingStartup\":false"
           ",\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\""
           ",\"sourceIntegrityContract\":\"shard-route-preview-production-live-capture-archive-non-participation.v1\""
           ",\"sourceIntegrityReleaseVersion\":\"v1225\""
           ",\"sourceIntegrityFixturePath\":\"fixtures/release/shard-readiness-v1225.json\""
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_complete) +
           ",\"sourceIntegrityStatus\":\"route-preview-production-live-capture-archive-non-participation-closeout-summary-read-only\""
           ",\"sourceIntegrityDigestMarker\":" +
           json_string(source_integrity::production_live_capture_archive_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1225\""
           ",\"productionLiveCaptureArchiveVerificationNonParticipationStage\":" + json_string(stage.stage) +
           ",\"productionLiveCaptureArchiveVerificationNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"productionLiveCaptureArchiveVerificationNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"productionLiveCaptureArchiveVerificationNonParticipationReleaseRangeStart\":\"v1226\""
           ",\"productionLiveCaptureArchiveVerificationNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_production_live_capture_archive_verification_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"productionLiveCaptureArchiveVerificationNonParticipationDeclared\":true"
           ",\"productionLiveCaptureArchiveVerificationNonParticipationOnly\":true"
           ",\"productionLiveCaptureArchiveVerificationNonParticipationChainComplete\":" +
           json_bool(non_participation_chain_complete) +
           ",\"sourceArchiveNonParticipationFixtureFrozen\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"nodeCaptureArchiveVerificationModuleSplitFocus\":" +
           json_string_array({"verification-profile-message-contracts",
                              "stable-entrypoint-type-reexports",
                              "verification-build-parts",
                              "expected-archive-digest-recompute-builder",
                              "verification-digest-generation-builder",
                              "java-v50-mini-kv-v59-reference-constants",
                              "verification-evidence-endpoint-constants",
                              "verification-check-assembly",
                              "ready-for-read-only-capture-archive-verification",
                              "verification-state-resolution",
                              "archive-digest-match-validation",
                              "archive-profile-readiness-checks",
                              "upstream-reference-readiness-checks",
                              "skipped-mixed-capture-policy-checks",
                              "upstream-actions-automatic-start-safety",
                              "blockers-warnings-recommendations",
                              "summary-counts",
                              "markdown-renderer",
                              "loader-orchestration",
                              "json-markdown-route-coverage",
                              "release-evidence-review-consumer-validation",
                              "typecheck-focused-adjacent-build",
                              "http-smoke-segmented-full-cleanup",
                              "github-actions-closeout"}) +
           ",\"nodeCaptureArchiveVerificationExecuted\":false"
           ",\"nodeArchiveVerificationLoaderExecuted\":false"
           ",\"nodeArchiveVerificationRendererExecuted\":false"
           ",\"nodeReleaseEvidenceReviewExecuted\":false"
           ",\"nodeVerificationCatalogImported\":false"
           ",\"nodeVerificationEndpointReadByMiniKv\":false"
           ",\"nodeVerificationFixtureImportedByMiniKv\":false"
           ",\"nodeVerificationPayloadAcceptedByMiniKv\":false"
           ",\"archiveDigestRecomputedByMiniKv\":false"
           ",\"miniKvStartsCaptureArchiveVerification\":" + json_bool(mini_kv_starts_capture_archive_verification) +
           ",\"miniKvConsumesCaptureArchiveVerificationRecords\":false"
           ",\"miniKvReadsVerificationEndpoint\":" + json_bool(mini_kv_reads_verification_endpoint) +
           ",\"miniKvImportsVerificationEndpointConstants\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"verificationEndpointRead\":false"
           ",\"archiveEndpointRead\":false"
           ",\"liveEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"verificationPayloadImported\":false"
           ",\"verificationPayloadAccepted\":false"
           ",\"verificationFixtureImported\":false"
           ",\"verificationStateAccepted\":false"
           ",\"archivePayloadImported\":false"
           ",\"archivePayloadAccepted\":false"
           ",\"archiveFixtureImported\":false"
           ",\"archiveStateAccepted\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"moduleSplit\":[\"production_live_capture_archive_verification_non_participation_core\",\"production_live_capture_archive_verification_non_participation_stages\",\"production_live_capture_archive_verification_non_participation_checks\",\"production_live_capture_archive_verification_non_participation_validation\"]"
           ",\"stageCatalog\":" +
           format_production_live_capture_archive_verification_non_participation_stage_catalog_json() +
           ",\"nonParticipationChecks\":" +
           checks::format_production_live_capture_archive_verification_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_production_live_capture_archive_verification_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_starts_capture_archive_verification,
               mini_kv_reads_verification_endpoint,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_production_live_capture_archive_verification_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1225 remains the frozen source archive non-participation fixture before v1226-v1260 archive verification non-participation evidence",
                              "Node v1657-v1686 is treated as production live probe read-only capture archive verification module-split maintenance context only",
                              "mini-kv does not start archive verification services sibling services Node loaders or release evidence review consumers",
                              "verification endpoint constants raw endpoint values credentials verification payload fixtures routers writes WAL and execution remain disabled"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"archiveVerificationRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsSiblingServices\":false"
           ",\"liveReadAllowed\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string production_live_capture_archive_verification_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_production_live_capture_archive_verification_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation
