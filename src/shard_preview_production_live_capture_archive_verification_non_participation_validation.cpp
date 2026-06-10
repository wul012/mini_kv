#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_production_live_capture_archive_verification_non_participation_validation_json(
    int source_integrity_stage_count,
    bool source_integrity_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_capture_archive_verification,
    bool mini_kv_reads_verification_endpoint,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_integrity_stage_count == 30 && source_integrity_complete;
    const bool verification_closed = !node_requires_fresh_minikv_evidence &&
                                     !mini_kv_starts_capture_archive_verification &&
                                     !mini_kv_reads_verification_endpoint;
    const bool counts_aligned = planned_check_count == 35 && completed_check_count == 35 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 35;
    const bool validation_passed = source_locked && verification_closed && counts_aligned;

    return "{\"productionLiveCaptureArchiveVerificationNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_integrity_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvStartsCaptureArchiveVerification\":" +
           json_bool(mini_kv_starts_capture_archive_verification) +
           ",\"miniKvReadsVerificationEndpoint\":" + json_bool(mini_kv_reads_verification_endpoint) +
           ",\"archiveVerificationClosed\":" + json_bool(verification_closed) +
           ",\"plannedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionLiveCaptureArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeCaptureArchiveVerificationExecuted\":false"
           ",\"nodeArchiveVerificationLoaderExecuted\":false"
           ",\"nodeArchiveVerificationRendererExecuted\":false"
           ",\"nodeReleaseEvidenceReviewExecuted\":false"
           ",\"nodeVerificationEndpointReadByMiniKv\":false"
           ",\"nodeVerificationFixtureImportedByMiniKv\":false"
           ",\"nodeVerificationPayloadAcceptedByMiniKv\":false"
           ",\"archiveDigestRecomputedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvCaptureArchiveVerificationStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"verificationEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"verificationPayloadImported\":false"
           ",\"verificationPayloadAccepted\":false"
           ",\"verificationFixtureImported\":false"
           ",\"verificationStateAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"archiveVerificationRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_validation
