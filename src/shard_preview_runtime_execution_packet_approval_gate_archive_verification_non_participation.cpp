#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation.hpp"
#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks.hpp"
#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_stages.hpp"
#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks =
    shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks;
namespace source_precheck = shard_preview_precheck_upstream_receipt_verification_split_non_participation;
namespace stages =
    shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_stages;
namespace validation =
    shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation;

constexpr std::string_view command_name =
    "SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v399-post-java-mini-kv-runtime-execution-packet-approval-gate-review-archive-verification-roadmap.md";
constexpr int current_stage_count = 20;
constexpr int source_precheck_planned_stage_count = 20;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool node_requires_fresh_java_evidence = false;
constexpr bool mini_kv_reads_node_archive_files = false;
constexpr bool mini_kv_replays_node_archive = false;
constexpr bool mini_kv_starts_node_services = false;
constexpr bool mini_kv_starts_java_service = false;
constexpr bool mini_kv_starts_mini_kv_service = false;
constexpr bool mini_kv_reads_endpoints = false;
constexpr bool mini_kv_reads_credentials = false;
constexpr bool mini_kv_parses_raw_endpoint = false;
constexpr bool mini_kv_executes_runtime_probe = false;
constexpr bool mini_kv_enables_active_router = false;

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
    return stages::runtime_execution_packet_approval_gate_archive_verification_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::runtime_execution_packet_approval_gate_archive_verification_non_participation_stages(),
        current_stage_count,
        stages::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_count(),
        stages::first_runtime_execution_packet_approval_gate_archive_verification_non_participation_release_number());
}

bool source_precheck_complete() {
    return source_precheck::published_stage_count() == source_precheck_planned_stage_count;
}

int completed_check_count() {
    return checks::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_check_count();
}

} // namespace

std::string format_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::runtime_execution_packet_approval_gate_archive_verification_non_participation_stages(),
        current_stage_count);
}

std::string runtime_execution_packet_approval_gate_archive_verification_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_runtime_execution_packet_approval_gate_archive_verification_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_precheck::published_stage_count();
    const bool source_complete = source_precheck_complete();
    const int planned_check_count =
        checks::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool chain_complete =
        current_stage_count ==
            stages::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-runtime-execution-packet-approval-gate-archive-verification-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationMode\":\"controlled-read-only-runtime-execution-packet-approval-gate-archive-verification-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeArchiveVerificationReleaseVersion\":\"Node v399\""
           ",\"sourceNodeVerifiedArchiveReleaseVersion\":\"Node v398\""
           ",\"sourceNodeNextConsumer\":\"Node v400\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":" + json_bool(node_requires_fresh_java_evidence) +
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeServiceStartupRequired\":false"
           ",\"sourceNodePortsOpened\":false"
           ",\"sourceNodeCrossProjectWritesAllowed\":false"
           ",\"sourceNodeArchiveVerificationOnly\":true"
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitCommand\":\"SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON\""
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitContract\":\"shard-route-precheck-upstream-receipt-verification-split-non-participation.v1\""
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitReleaseVersion\":\"v1545\""
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitFixturePath\":\"fixtures/release/shard-readiness-v1545.json\""
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitPublishedStageCount\":" +
           std::to_string(source_stage_count) +
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitComplete\":" + json_bool(source_complete) +
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitStatus\":" +
           json_string(source_precheck::precheck_upstream_receipt_verification_split_non_participation_status()) +
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitDigestMarker\":" +
           json_string(source_precheck::precheck_upstream_receipt_verification_split_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1565\""
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationStage\":" +
           json_string(stage.stage) +
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseRangeStart\":\"v1566\""
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationDeclared\":true"
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationOnly\":true"
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationChainComplete\":" +
           json_bool(chain_complete) +
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitFixtureFrozen\":true"
           ",\"sourcePrecheckUpstreamReceiptVerificationSplitDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"archiveVerificationOnly\":true"
           ",\"nodeV398ArchiveReferences\":" +
           json_string_array({"java-mini-kv-runtime-execution-packet-approval-gate-review-v398-http.json",
                              "java-mini-kv-runtime-execution-packet-approval-gate-review-v398-http.md",
                              "java-mini-kv-runtime-execution-packet-approval-gate-review-v398-summary.json",
                              "java-mini-kv-runtime-execution-packet-approval-gate-review-v398-browser-snapshot.md",
                              "java-mini-kv-runtime-execution-packet-approval-gate-review-v398.png",
                              "java-mini-kv-runtime-execution-packet-approval-gate-review-v398.md",
                              "403-java-mini-kv-runtime-execution-packet-approval-gate-review-v398.md"}) +
           ",\"nodeV398ArchiveFilesReadByMiniKv\":false"
           ",\"nodeV398ArchiveMarkdownParsedByMiniKv\":false"
           ",\"nodeV398ArchiveBrowserEvidenceRenderedByMiniKv\":false"
           ",\"nodeV398ArchiveReplayExecutedByMiniKv\":false"
           ",\"nodeV398ArchiveTreatedAsRuntimeApproval\":false"
           ",\"nodeV400MayConsumeRealApprovalGateInputsLater\":true"
           ",\"nodeV400MustNotTreatArchiveVerificationAsApproval\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
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
           ",\"approvalInputCount\":0"
           ",\"requiredApprovalInputCount\":3"
           ",\"runtimeArtifactCount\":0"
           ",\"requiredRuntimeArtifactCount\":6"
           ",\"presentRuntimeExecutionArtifactCount\":0"
           ",\"missingRuntimeExecutionArtifactCount\":6"
           ",\"runtimeExecutionPacketPresent\":false"
           ",\"runtimeExecutionPacketExecutable\":false"
           ",\"runtimeGateApprovalPresent\":false"
           ",\"readyForRuntimeExecutionPacket\":false"
           ",\"readyForRuntimeLiveReadGate\":false"
           ",\"approvalGateBlocked\":true"
           ",\"warnings\":" +
           json_string_array({"archive verification is not runtime approval",
                              "Node v400 must consume real approval-gate inputs before runtime",
                              "mini-kv records handles only and does not inspect sibling archive files"}) +
           ",\"blockers\":" +
           json_string_array({"missing three approval-gate inputs",
                              "missing six runtime execution artifacts",
                              "runtime execution packet and runtime gate approval are absent"}) +
           ",\"moduleSplit\":[\"runtime_execution_packet_approval_gate_archive_verification_non_participation_core\",\"runtime_execution_packet_approval_gate_archive_verification_non_participation_stages\",\"runtime_execution_packet_approval_gate_archive_verification_non_participation_checks\",\"runtime_execution_packet_approval_gate_archive_verification_non_participation_validation\"]"
           ",\"stageCatalog\":" +
           format_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_catalog_json() +
           ",\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationChecks\":" +
           checks::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_json(
               check_count) +
           ",\"validation\":" +
           validation::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               node_requires_fresh_java_evidence,
               mini_kv_reads_node_archive_files,
               mini_kv_replays_node_archive,
               mini_kv_starts_node_services,
               mini_kv_starts_java_service,
               mini_kv_starts_mini_kv_service,
               mini_kv_reads_endpoints,
               mini_kv_reads_credentials,
               mini_kv_parses_raw_endpoint,
               mini_kv_executes_runtime_probe,
               mini_kv_enables_active_router,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1565 remains the frozen command catalog quality source",
                              "Node v399 verifies the Node v398 archive but does not unlock runtime execution",
                              "mini-kv exposes only non-participation evidence and does not start Java or mini-kv services",
                              "approval inputs remain 0 of 3 and runtime artifacts remain 0 of 6"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
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
           ",\"runtimeProbeExecuted\":false"
           ",\"executionAttempted\":false"
           ",\"executionAllowed\":false"
           ",\"readOnly\":true}";
}

std::string runtime_execution_packet_approval_gate_archive_verification_non_participation_digest_marker() {
    const auto& stage = current_stage();
    return shard_route_preview_stage_catalog::format_digest_marker(
        stage,
        current_stage_count,
        stages::planned_runtime_execution_packet_approval_gate_archive_verification_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation
