#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation.hpp"
#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_checks.hpp"
#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_stages.hpp"
#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_release_window_readiness_packet_split_non_participation_checks;
namespace source_implementation_plan =
    shard_preview_implementation_plan_upstream_echo_closeout_non_participation;
namespace stages = shard_preview_release_window_readiness_packet_split_non_participation_stages;
namespace validation = shard_preview_release_window_readiness_packet_split_non_participation_validation;

constexpr std::string_view command_name =
    "SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1935-v1937-release-window-readiness-packet-split-roadmap.md";
constexpr int current_stage_count = 15;
constexpr int source_implementation_plan_planned_stage_count = 3;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_imports_node_modules = false;
constexpr bool mini_kv_executes_node_checks = false;
constexpr bool mini_kv_starts_services = false;
constexpr bool mini_kv_reads_endpoints = false;
constexpr bool mini_kv_reads_credentials = false;
constexpr bool mini_kv_executes_release_window_packet = false;

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
    return stages::release_window_readiness_packet_split_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::release_window_readiness_packet_split_non_participation_stages(),
        current_stage_count,
        stages::planned_release_window_readiness_packet_split_non_participation_stage_count(),
        stages::first_release_window_readiness_packet_split_non_participation_release_number());
}

bool source_implementation_plan_complete() {
    return source_implementation_plan::published_stage_count() == source_implementation_plan_planned_stage_count;
}

int completed_check_count() {
    return checks::planned_release_window_readiness_packet_split_non_participation_check_count();
}

} // namespace

std::string format_release_window_readiness_packet_split_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::release_window_readiness_packet_split_non_participation_stages(), current_stage_count);
}

std::string release_window_readiness_packet_split_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_release_window_readiness_packet_split_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_implementation_plan::published_stage_count();
    const bool source_complete = source_implementation_plan_complete();
    const int planned_check_count = checks::planned_release_window_readiness_packet_split_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool chain_complete =
        current_stage_count == stages::planned_release_window_readiness_packet_split_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-release-window-readiness-packet-split-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"releaseWindowReadinessPacketSplitNonParticipationMode\":\"controlled-read-only-release-window-readiness-packet-split-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeReleaseWindowReadinessPacketSplitReleaseRange\":\"Node v1935-v1937\""
           ",\"sourceNodeReleaseWindowReadinessPacketSplitCloseoutReleaseVersion\":\"Node v1937\""
           ",\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v70\""
           ",\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"restore-drill-evidence\""
           ",\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v61\""
           ",\"sourceNodeConsumesHistoricalJavaEvidence\":\"rollback-approval-fixture\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeReleaseWindowReadinessPacketSplitOnly\":true"
           ",\"sourceNodeRequiresSiblingStartup\":false"
           ",\"sourceNodeNoServiceStartupConstraint\":true"
           ",\"sourceImplementationPlanCommand\":\"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON\""
           ",\"sourceImplementationPlanContract\":\"shard-route-implementation-plan-upstream-echo-closeout-non-participation.v1\""
           ",\"sourceImplementationPlanReleaseVersion\":\"v1475\""
           ",\"sourceImplementationPlanFixturePath\":\"fixtures/release/shard-readiness-v1475.json\""
           ",\"sourceImplementationPlanPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceImplementationPlanComplete\":" + json_bool(source_complete) +
           ",\"sourceImplementationPlanStatus\":" +
           json_string(source_implementation_plan::implementation_plan_upstream_echo_closeout_non_participation_status()) +
           ",\"sourceImplementationPlanDigestMarker\":" +
           json_string(source_implementation_plan::implementation_plan_upstream_echo_closeout_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1475\""
           ",\"releaseWindowReadinessPacketSplitNonParticipationStage\":" + json_string(stage.stage) +
           ",\"releaseWindowReadinessPacketSplitNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"releaseWindowReadinessPacketSplitNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"releaseWindowReadinessPacketSplitNonParticipationReleaseRangeStart\":\"v1476\""
           ",\"releaseWindowReadinessPacketSplitNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_release_window_readiness_packet_split_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"releaseWindowReadinessPacketSplitNonParticipationDeclared\":true"
           ",\"releaseWindowReadinessPacketSplitNonParticipationOnly\":true"
           ",\"releaseWindowReadinessPacketSplitNonParticipationChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true"
           ",\"sourceImplementationPlanUpstreamEchoCloseoutDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeCloseoutObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"releaseWindowReadinessPacketSplitFocus\":" +
           json_string_array({"source-implementation-plan-closeout-freeze",
                              "node-release-window-packet-plan-intake",
                              "packet-types-no-import",
                              "packet-evidence-no-import",
                              "packet-policy-no-execution",
                              "stable-entrypoint-no-import",
                              "loader-renderer-no-execution",
                              "frozen-java-v61-reference-only",
                              "frozen-minikv-v70-reference-only",
                              "release-dry-run-envelope-no-execution",
                              "status-routes-no-execution",
                              "endpoint-credential-boundary-closed",
                              "router-write-wal-execution-disabled",
                              "command-shardjson-fixture-parity",
                              "clean-workspace-ci-closeout"}) +
           ",\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketChecksExecutedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketMessagesExecutedByMiniKv\":false"
           ",\"nodeProductionReleaseDryRunEnvelopeExecutedByMiniKv\":false"
           ",\"nodeStatusRoutesExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvExecutesReleaseWindowPacket\":" + json_bool(mini_kv_executes_release_window_packet) +
           ",\"miniKvReplaysRestoreDrillEvidence\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"schemaMigrationExecuted\":false"
           ",\"ledgerWriteExecuted\":false"
           ",\"externalRequestSent\":false"
           ",\"moduleSplit\":[\"release_window_readiness_packet_split_non_participation_core\",\"release_window_readiness_packet_split_non_participation_stages\",\"release_window_readiness_packet_split_non_participation_checks\",\"release_window_readiness_packet_split_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_release_window_readiness_packet_split_non_participation_stage_catalog_json() +
           ",\"releaseWindowReadinessPacketSplitNonParticipationChecks\":" +
           checks::format_release_window_readiness_packet_split_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_release_window_readiness_packet_split_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_imports_node_modules,
               mini_kv_executes_node_checks,
               mini_kv_starts_services,
               mini_kv_reads_endpoints,
               mini_kv_reads_credentials,
               mini_kv_executes_release_window_packet,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_release_window_readiness_packet_split_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1475 remains the frozen implementation-plan upstream echo closeout non-participation source",
                              "Node v1935-v1937 split release-window readiness packet internals into Types Evidence and Policy modules",
                              "Node consumes historical Java v61 rollback approval fixture and mini-kv v70 restore drill evidence; no fresh sibling evidence or service startup is required",
                              "mini-kv records release-window packet split non-participation without importing Node modules reading endpoints credentials managed connections routers writes WAL restore replay or execution"}) +
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
           ",\"runtimeProbeAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string release_window_readiness_packet_split_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_release_window_readiness_packet_split_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation
