#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation_json(
    int source_precheck_upstream_receipt_stage_count,
    bool source_precheck_upstream_receipt_complete,
    bool node_requires_fresh_minikv_evidence,
    bool node_requires_fresh_java_evidence,
    bool mini_kv_reads_node_archive_files,
    bool mini_kv_replays_node_archive,
    bool mini_kv_starts_node_services,
    bool mini_kv_starts_java_service,
    bool mini_kv_starts_mini_kv_service,
    bool mini_kv_reads_endpoints,
    bool mini_kv_reads_credentials,
    bool mini_kv_parses_raw_endpoint,
    bool mini_kv_executes_runtime_probe,
    bool mini_kv_enables_active_router,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked =
        source_precheck_upstream_receipt_stage_count == 20 && source_precheck_upstream_receipt_complete;
    const bool node_plan_observed_only = !node_requires_fresh_minikv_evidence && !node_requires_fresh_java_evidence;
    const bool mini_kv_boundary_closed =
        !mini_kv_reads_node_archive_files && !mini_kv_replays_node_archive && !mini_kv_starts_node_services &&
        !mini_kv_starts_java_service && !mini_kv_starts_mini_kv_service && !mini_kv_reads_endpoints &&
        !mini_kv_reads_credentials && !mini_kv_parses_raw_endpoint && !mini_kv_executes_runtime_probe &&
        !mini_kv_enables_active_router;
    const bool counts_aligned = planned_check_count == 20 && completed_check_count == 20 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 20;
    const bool approval_gate_blocked = true;
    const bool validation_passed =
        source_locked && node_plan_observed_only && mini_kv_boundary_closed && counts_aligned && approval_gate_blocked;

    return "{\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitPublishedStageCount\":" +
           std::to_string(source_precheck_upstream_receipt_stage_count) +
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitComplete\":" +
           json_bool(source_precheck_upstream_receipt_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"nodeRequiresFreshJavaEvidence\":" + json_bool(node_requires_fresh_java_evidence) +
           ",\"nodePlanObservedOnly\":" + json_bool(node_plan_observed_only) +
           ",\"miniKvReadsNodeArchiveFiles\":" + json_bool(mini_kv_reads_node_archive_files) +
           ",\"miniKvReplaysNodeArchive\":" + json_bool(mini_kv_replays_node_archive) +
           ",\"miniKvStartsNodeServices\":" + json_bool(mini_kv_starts_node_services) +
           ",\"miniKvStartsJavaService\":" + json_bool(mini_kv_starts_java_service) +
           ",\"miniKvStartsMiniKvService\":" + json_bool(mini_kv_starts_mini_kv_service) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvParsesRawEndpoint\":" + json_bool(mini_kv_parses_raw_endpoint) +
           ",\"miniKvExecutesRuntimeProbe\":" + json_bool(mini_kv_executes_runtime_probe) +
           ",\"miniKvEnablesActiveRouter\":" + json_bool(mini_kv_enables_active_router) +
           ",\"miniKvBoundaryClosed\":" + json_bool(mini_kv_boundary_closed) +
           ",\"plannedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"approvalGateBlocked\":" + json_bool(approval_gate_blocked) +
           ",\"approvalInputCount\":0"
           ",\"requiredApprovalInputCount\":3"
           ",\"runtimeArtifactCount\":0"
           ",\"requiredRuntimeArtifactCount\":6"
           ",\"nodeArchiveFilesReadByMiniKv\":false"
           ",\"nodeArchiveReplayExecutedByMiniKv\":false"
           ",\"startsJavaService\":false"
           ",\"startsMiniKvService\":false"
           ",\"credentialValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"runtimeProbeExecuted\":false"
           ",\"activeShardPrototypeEnabled\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation
