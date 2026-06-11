#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_checks.hpp"
#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_stages.hpp"
#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_validation.hpp"
#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_code_walkthrough_quality_gate_non_participation_checks;
namespace source_runtime_gate =
    shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation;
namespace stages = shard_preview_code_walkthrough_quality_gate_non_participation_stages;
namespace validation = shard_preview_code_walkthrough_quality_gate_non_participation_validation;

constexpr std::string_view command_name = "SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v2077-post-code-walkthrough-quality-gate-closeout-roadmap.md";
constexpr int current_stage_count = 6;
constexpr int source_runtime_gate_planned_stage_count = 20;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool node_requires_fresh_java_evidence = false;
constexpr bool mini_kv_reads_node_walkthrough_files = false;
constexpr bool mini_kv_scans_node_repository = false;
constexpr bool mini_kv_executes_node_quality_route = false;
constexpr bool mini_kv_starts_node_services = false;
constexpr bool mini_kv_starts_java_service = false;
constexpr bool mini_kv_starts_mini_kv_service = false;
constexpr bool mini_kv_mutates_node_docs = false;
constexpr bool mini_kv_mutates_java_docs = false;
constexpr bool placeholder_walkthrough_allowed = false;
constexpr bool bulk_historical_relocation_allowed = false;

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
    return stages::code_walkthrough_quality_gate_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::code_walkthrough_quality_gate_non_participation_stages(),
        current_stage_count,
        stages::planned_code_walkthrough_quality_gate_non_participation_stage_count(),
        stages::first_code_walkthrough_quality_gate_non_participation_release_number());
}

bool source_runtime_gate_complete() {
    return source_runtime_gate::published_stage_count() == source_runtime_gate_planned_stage_count;
}

int completed_check_count() {
    return checks::planned_code_walkthrough_quality_gate_non_participation_check_count();
}

} // namespace

std::string format_code_walkthrough_quality_gate_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::code_walkthrough_quality_gate_non_participation_stages(),
        current_stage_count);
}

std::string code_walkthrough_quality_gate_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_code_walkthrough_quality_gate_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_runtime_gate::published_stage_count();
    const bool source_complete = source_runtime_gate_complete();
    const int planned_check_count = checks::planned_code_walkthrough_quality_gate_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool chain_complete =
        current_stage_count == stages::planned_code_walkthrough_quality_gate_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-code-walkthrough-quality-gate-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"codeWalkthroughQualityGateNonParticipationMode\":\"controlled-read-only-code-walkthrough-quality-gate-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeCodeWalkthroughQualityGateReleaseVersion\":\"Node v2077\""
           ",\"sourceNodeQualityRunRange\":\"Node v2068-v2077\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":" + json_bool(node_requires_fresh_java_evidence) +
           ",\"sourceNodeRouteConsumedByMiniKv\":false"
           ",\"sourceNodeTestsExecutedByMiniKv\":false"
           ",\"sourceNodeWalkthroughFilesReadByMiniKv\":false"
           ",\"sourceNodeRepositoryScannedByMiniKv\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeArchiveVerificationOnly\":true"
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationCommand\":\"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON\""
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationContract\":\"shard-route-runtime-execution-packet-approval-gate-archive-verification-non-participation.v1\""
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationReleaseVersion\":\"v1585\""
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationFixturePath\":\"fixtures/release/shard-readiness-v1585.json\""
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationPublishedStageCount\":" +
           std::to_string(source_stage_count) +
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationComplete\":" + json_bool(source_complete) +
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationStatus\":" +
           json_string(source_runtime_gate::runtime_execution_packet_approval_gate_archive_verification_non_participation_status()) +
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationDigestMarker\":" +
           json_string(source_runtime_gate::runtime_execution_packet_approval_gate_archive_verification_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1585\""
           ",\"codeWalkthroughQualityGateNonParticipationStage\":" + json_string(stage.stage) +
           ",\"codeWalkthroughQualityGateNonParticipationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"codeWalkthroughQualityGateNonParticipationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"codeWalkthroughQualityGateNonParticipationReleaseRangeStart\":\"v1586\""
           ",\"codeWalkthroughQualityGateNonParticipationReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_code_walkthrough_quality_gate_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedCodeWalkthroughQualityGateNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedCodeWalkthroughQualityGateNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"codeWalkthroughQualityGateNonParticipationDeclared\":true"
           ",\"codeWalkthroughQualityGateNonParticipationOnly\":true"
           ",\"codeWalkthroughQualityGateNonParticipationChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationFixtureFrozen\":true"
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"walkthroughQualityGateOnly\":true"
           ",\"walkthroughQualityPolicy\":" +
           json_string_array({"walkthroughs may be omitted for tiny maintenance",
                              "written walkthroughs must explain goal, role, non-goals, entry, evidence, boundaries, tests, and integration value",
                              "placeholder or unsafe production claims are blocked",
                              "historical walkthroughs are indexed rather than bulk-moved"}) +
           ",\"requiredWalkthroughSections\":" +
           json_string_array({"goal-role-non-goal", "entry-command-or-module", "response-evidence-structure",
                              "core-flow-and-responsibility", "boundary-field-interpretation",
                              "test-behavior-protection", "loose-integration-value"}) +
           ",\"nodeWalkthroughFilesReadByMiniKv\":" + json_bool(mini_kv_reads_node_walkthrough_files) +
           ",\"miniKvScansNodeRepository\":" + json_bool(mini_kv_scans_node_repository) +
           ",\"miniKvExecutesNodeQualityRoute\":" + json_bool(mini_kv_executes_node_quality_route) +
           ",\"miniKvStartsNodeServices\":" + json_bool(mini_kv_starts_node_services) +
           ",\"miniKvStartsJavaService\":" + json_bool(mini_kv_starts_java_service) +
           ",\"miniKvStartsMiniKvService\":" + json_bool(mini_kv_starts_mini_kv_service) +
           ",\"miniKvMutatesNodeDocs\":" + json_bool(mini_kv_mutates_node_docs) +
           ",\"miniKvMutatesJavaDocs\":" + json_bool(mini_kv_mutates_java_docs) +
           ",\"codeWalkthroughWrittenWhenSubstantive\":true"
           ",\"codeWalkthroughMayBeOmittedForTinyMaintenance\":true"
           ",\"placeholderWalkthroughAllowed\":" + json_bool(placeholder_walkthrough_allowed) +
           ",\"unsafeProductionClaimAllowed\":false"
           ",\"bulkHistoricalRelocationAllowed\":" + json_bool(bulk_historical_relocation_allowed) +
           ",\"newWalkthroughVolume\":\"code-walkthrough-production-prototype-fourth-volume\""
           ",\"newWalkthroughVolumeActive\":true"
           ",\"historicalWalkthroughFilesMoved\":false"
           ",\"archiveScreenshotsMustMatchCommands\":true"
           ",\"fixturesMustParseAsJson\":true"
           ",\"trailingWhitespaceAllowed\":false"
           ",\"testSectionsExplainProtectedBehavior\":true"
           ",\"warnings\":" +
           json_string_array({"documentation quality gate is not runtime approval",
                              "mini-kv records its own walkthrough standard without auditing Node files",
                              "future tiny maintenance should skip walkthroughs instead of adding filler"}) +
           ",\"blockers\":" +
           json_string_array({"placeholder walkthrough", "unsafe production-ready claim without evidence",
                              "walkthrough that lists tests without explaining protected behavior"}) +
           ",\"moduleSplit\":[\"code_walkthrough_quality_gate_non_participation_core\",\"code_walkthrough_quality_gate_non_participation_stages\",\"code_walkthrough_quality_gate_non_participation_checks\",\"code_walkthrough_quality_gate_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_code_walkthrough_quality_gate_non_participation_stage_catalog_json() +
           ",\"codeWalkthroughQualityGateNonParticipationChecks\":" +
           checks::format_code_walkthrough_quality_gate_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_code_walkthrough_quality_gate_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               node_requires_fresh_java_evidence,
               mini_kv_reads_node_walkthrough_files,
               mini_kv_scans_node_repository,
               mini_kv_executes_node_quality_route,
               mini_kv_starts_node_services,
               mini_kv_starts_java_service,
               mini_kv_starts_mini_kv_service,
               mini_kv_mutates_node_docs,
               mini_kv_mutates_java_docs,
               placeholder_walkthrough_allowed,
               bulk_historical_relocation_allowed,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_code_walkthrough_quality_gate_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1585 remains the frozen runtime approval gate archive evidence source",
                              "Node v2077 closes a documentation-quality run without requiring fresh sibling evidence",
                              "mini-kv now records a local quality gate for future walkthroughs",
                              "six-version batch uses one evidence module, not one tiny module per release"}) +
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

std::string code_walkthrough_quality_gate_non_participation_digest_marker() {
    const auto& stage = current_stage();
    return shard_route_preview_stage_catalog::format_digest_marker(
        stage,
        current_stage_count,
        stages::planned_code_walkthrough_quality_gate_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation
