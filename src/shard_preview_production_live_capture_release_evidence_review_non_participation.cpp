#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation.hpp"
#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_checks.hpp"
#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_stages.hpp"
#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_production_live_capture_release_evidence_review_non_participation_checks;
namespace source_integrity = shard_preview_production_live_capture_archive_verification_non_participation;
namespace stages = shard_preview_production_live_capture_release_evidence_review_non_participation_stages;
namespace validation = shard_preview_production_live_capture_release_evidence_review_non_participation_validation;

constexpr std::string_view command_name =
    "SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1721-production-live-probe-read-only-window-capture-release-evidence-review-module-split-roadmap.md";
constexpr int current_stage_count = 35;
constexpr int source_integrity_planned_stage_count = 35;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_starts_release_evidence_review = false;
constexpr bool mini_kv_reads_release_review_endpoint = false;

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
    return stages::production_live_capture_release_evidence_review_non_participation_stages()[static_cast<std::size_t>(
        current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::production_live_capture_release_evidence_review_non_participation_stages(),
        current_stage_count,
        stages::planned_production_live_capture_release_evidence_review_non_participation_stage_count(),
        stages::first_production_live_capture_release_evidence_review_non_participation_release_number());
}

bool source_integrity_complete() {
    return source_integrity::published_stage_count() == source_integrity_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_production_live_capture_release_evidence_review_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::production_live_capture_release_evidence_review_non_participation_stages(),
        current_stage_count);
}

std::string production_live_capture_release_evidence_review_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_production_live_capture_release_evidence_review_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_integrity::published_stage_count();
    const bool source_complete = source_integrity_complete();
    const int planned_check_count =
        checks::planned_production_live_capture_release_evidence_review_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool non_participation_chain_complete =
        current_stage_count ==
            stages::planned_production_live_capture_release_evidence_review_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-preview-production-live-capture-release-evidence-review-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationMode\":\"controlled-read-only-production-live-capture-release-evidence-review-non-participation-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureReleaseEvidenceReviewReleaseRange\":\"Node v1687-v1721\""
           ",\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureReleaseEvidenceReviewCloseoutReleaseVersion\":\"Node v1721\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeReleaseEvidenceReviewModuleSplitOnly\":true"
           ",\"sourceNodeHttpSmokeIsNodeOnly\":true"
           ",\"sourceNodeReleaseEvidenceReviewRequiresSiblingStartup\":false"
           ",\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\""
           ",\"sourceIntegrityContract\":\"shard-route-preview-production-live-capture-archive-verification-non-participation.v1\""
           ",\"sourceIntegrityReleaseVersion\":\"v1260\""
           ",\"sourceIntegrityFixturePath\":\"fixtures/release/shard-readiness-v1260.json\""
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_complete) +
           ",\"sourceIntegrityStatus\":\"route-preview-production-live-capture-archive-verification-non-participation-closeout-summary-read-only\""
           ",\"sourceIntegrityDigestMarker\":" +
           json_string(source_integrity::production_live_capture_archive_verification_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1260\""
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationStage\":" + json_string(stage.stage) +
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseRangeStart\":\"v1261\""
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_production_live_capture_release_evidence_review_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationDeclared\":true"
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationOnly\":true"
           ",\"productionLiveCaptureReleaseEvidenceReviewNonParticipationChainComplete\":" +
           json_bool(non_participation_chain_complete) +
           ",\"sourceArchiveVerificationNonParticipationFixtureFrozen\":true"
           ",\"sourceArchiveVerificationDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"nodeReleaseEvidenceReviewModuleSplitFocus\":" +
           json_string_array({"release-review-profile-field-guide-message-contracts",
                              "stable-entrypoint-type-reexports",
                              "release-review-build-parts",
                              "java-v51-field-guide-reference-constants",
                              "mini-kv-v60-field-guide-reference-constants",
                              "release-evidence-review-endpoint-constants",
                              "release-semantics-selection",
                              "release-review-digest-generation",
                              "release-review-check-assembly",
                              "ready-for-read-only-capture-release-evidence-review",
                              "review-state-resolution",
                              "field-guide-readiness-validation",
                              "non-pass-only-field-guide-policy-checks",
                              "skipped-mixed-capture-non-pass-policy-checks",
                              "upstream-actions-automatic-start-safety",
                              "no-execution-authorization-review-check",
                              "production-blocker-collection",
                              "warning-generation",
                              "recommendation-generation",
                              "summary-counts",
                              "markdown-renderer",
                              "loader-orchestration",
                              "json-markdown-route-coverage",
                              "archive-verification-upstream-consumer-validation",
                              "status-live-probe-route-wiring",
                              "typecheck-focused-release-review-tests",
                              "adjacent-tests-build",
                              "http-smoke-json-markdown",
                              "segmented-full-vitest",
                              "github-actions-closeout"}) +
           ",\"nodeReleaseEvidenceReviewExecuted\":false"
           ",\"nodeReleaseReviewLoaderExecuted\":false"
           ",\"nodeReleaseReviewRendererExecuted\":false"
           ",\"nodeReleaseReviewCatalogImported\":false"
           ",\"nodeReleaseReviewEndpointReadByMiniKv\":false"
           ",\"nodeReleaseReviewFixtureImportedByMiniKv\":false"
           ",\"nodeReleaseReviewPayloadAcceptedByMiniKv\":false"
           ",\"nodeReleaseReviewDigestGeneratedByMiniKv\":false"
           ",\"miniKvStartsReleaseEvidenceReview\":" + json_bool(mini_kv_starts_release_evidence_review) +
           ",\"miniKvConsumesReleaseReviewRecords\":false"
           ",\"miniKvReadsReleaseReviewEndpoint\":" + json_bool(mini_kv_reads_release_review_endpoint) +
           ",\"miniKvImportsReleaseReviewEndpointConstants\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"releaseReviewEndpointRead\":false"
           ",\"archiveVerificationEndpointRead\":false"
           ",\"liveEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"releaseReviewPayloadImported\":false"
           ",\"releaseReviewPayloadAccepted\":false"
           ",\"releaseReviewFixtureImported\":false"
           ",\"releaseReviewStateAccepted\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"moduleSplit\":[\"production_live_capture_release_evidence_review_non_participation_core\",\"production_live_capture_release_evidence_review_non_participation_stages\",\"production_live_capture_release_evidence_review_non_participation_checks\",\"production_live_capture_release_evidence_review_non_participation_validation\"]"
           ",\"stageCatalog\":" +
           format_production_live_capture_release_evidence_review_non_participation_stage_catalog_json() +
           ",\"nonParticipationChecks\":" +
           checks::format_production_live_capture_release_evidence_review_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_production_live_capture_release_evidence_review_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_starts_release_evidence_review,
               mini_kv_reads_release_review_endpoint,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_production_live_capture_release_evidence_review_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1260 remains the frozen source archive verification non-participation fixture before v1261-v1295 release evidence review non-participation evidence",
                              "Node v1687-v1721 is treated as production live probe read-only capture release evidence review module-split maintenance context only",
                              "mini-kv does not start release review services sibling services Node loaders or Markdown renderers",
                              "release review endpoint constants raw endpoint values credentials payload fixtures routers writes WAL and execution remain disabled"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"releaseEvidenceReviewRouterInstalled\":false"
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

std::string production_live_capture_release_evidence_review_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_production_live_capture_release_evidence_review_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation
