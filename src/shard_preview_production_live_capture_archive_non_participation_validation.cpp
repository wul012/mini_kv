#include "minikv/shard_preview_production_live_capture_archive_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_production_live_capture_archive_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_production_live_capture_archive_non_participation_validation_json(
    int source_integrity_stage_count,
    bool source_integrity_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_capture_archive,
    bool mini_kv_reads_archive_endpoint,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_integrity_stage_count == 25 && source_integrity_complete;
    const bool archive_capture_closed = !node_requires_fresh_minikv_evidence && !mini_kv_starts_capture_archive &&
                                        !mini_kv_reads_archive_endpoint;
    const bool counts_aligned = planned_check_count == 30 && completed_check_count == 30 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 30;
    const bool validation_passed = source_locked && archive_capture_closed && counts_aligned;

    return "{\"productionLiveCaptureArchiveNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_integrity_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvStartsCaptureArchive\":" + json_bool(mini_kv_starts_capture_archive) +
           ",\"miniKvReadsArchiveEndpoint\":" + json_bool(mini_kv_reads_archive_endpoint) +
           ",\"archiveCaptureClosed\":" + json_bool(archive_capture_closed) +
           ",\"plannedProductionLiveCaptureArchiveNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionLiveCaptureArchiveNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeCaptureArchiveExecuted\":false"
           ",\"nodeArchiveLoaderExecuted\":false"
           ",\"nodeArchiveRendererExecuted\":false"
           ",\"nodeArchiveVerificationExecuted\":false"
           ",\"nodeReleaseReviewExecuted\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvCaptureArchiveStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"archiveEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"archivePayloadImported\":false"
           ",\"archivePayloadAccepted\":false"
           ",\"archiveFixtureImported\":false"
           ",\"archiveStateAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_production_live_capture_archive_non_participation_validation
