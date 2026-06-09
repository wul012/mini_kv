#include "minikv/shard_preview_production_live_capture_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_production_live_capture_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_production_live_capture_non_participation_validation_json(
    int source_integrity_stage_count,
    bool source_integrity_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_live_capture,
    bool mini_kv_reads_raw_endpoint,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_integrity_stage_count == 10 && source_integrity_complete;
    const bool live_capture_closed =
        !node_requires_fresh_minikv_evidence && !mini_kv_starts_live_capture && !mini_kv_reads_raw_endpoint;
    const bool counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool validation_passed = source_locked && live_capture_closed && counts_aligned;

    return "{\"productionLiveCaptureNonParticipationValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_integrity_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvStartsLiveCapture\":" + json_bool(mini_kv_starts_live_capture) +
           ",\"miniKvReadsRawEndpoint\":" + json_bool(mini_kv_reads_raw_endpoint) +
           ",\"liveCaptureClosed\":" + json_bool(live_capture_closed) +
           ",\"plannedProductionLiveCaptureNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionLiveCaptureNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeLiveCaptureExecuted\":false"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvLiveCaptureStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"liveEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_production_live_capture_non_participation_validation
