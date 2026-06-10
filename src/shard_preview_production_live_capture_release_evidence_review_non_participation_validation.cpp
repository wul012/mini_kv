#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_production_live_capture_release_evidence_review_non_participation_validation_json(
    int source_integrity_stage_count,
    bool source_integrity_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_release_evidence_review,
    bool mini_kv_reads_release_review_endpoint,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_integrity_stage_count == 35 && source_integrity_complete;
    const bool verification_closed = !node_requires_fresh_minikv_evidence &&
                                     !mini_kv_starts_release_evidence_review &&
                                     !mini_kv_reads_release_review_endpoint;
    const bool counts_aligned = planned_check_count == 35 && completed_check_count == 35 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 35;
    const bool validation_passed = source_locked && verification_closed && counts_aligned;

    return "{\"productionLiveCaptureReleaseEvidenceReviewNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_integrity_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvStartsReleaseEvidenceReview\":" +
           json_bool(mini_kv_starts_release_evidence_review) +
           ",\"miniKvReadsReleaseReviewEndpoint\":" + json_bool(mini_kv_reads_release_review_endpoint) +
           ",\"releaseEvidenceReviewClosed\":" + json_bool(verification_closed) +
           ",\"plannedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeReleaseEvidenceReviewExecuted\":false"
           ",\"nodeReleaseReviewLoaderExecuted\":false"
           ",\"nodeReleaseReviewRendererExecuted\":false"
           ",\"nodeReleaseReviewEndpointReadByMiniKv\":false"
           ",\"nodeReleaseReviewFixtureImportedByMiniKv\":false"
           ",\"nodeReleaseReviewPayloadAcceptedByMiniKv\":false"
           ",\"nodeReleaseReviewDigestGeneratedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvReleaseEvidenceReviewStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"releaseReviewEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"releaseReviewPayloadImported\":false"
           ",\"releaseReviewPayloadAccepted\":false"
           ",\"releaseReviewFixtureImported\":false"
           ",\"releaseReviewStateAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"releaseEvidenceReviewRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_validation
