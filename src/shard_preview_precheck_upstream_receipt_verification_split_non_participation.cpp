#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation.hpp"
#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks.hpp"
#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages.hpp"
#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks;
namespace source_blocked = shard_preview_blocked_execution_rehearsal_split_non_participation;
namespace stages = shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages;
namespace validation = shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation;

constexpr std::string_view command_name = "SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1983-v2002-precheck-upstream-receipt-verification-split-roadmap.md";
constexpr int current_stage_count = 20;
constexpr int source_blocked_planned_stage_count = 20;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool node_requires_fresh_java_evidence = false;
constexpr bool mini_kv_imports_node_modules = false;
constexpr bool mini_kv_executes_node_checks = false;
constexpr bool mini_kv_starts_services = false;
constexpr bool mini_kv_reads_endpoints = false;
constexpr bool mini_kv_reads_credentials = false;
constexpr bool mini_kv_parses_historical_receipts = false;
constexpr bool mini_kv_scans_java_evidence = false;
constexpr bool mini_kv_executes_downstream_checks = false;
constexpr bool mini_kv_mutates_large_file_inventory = false;

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
    return stages::precheck_upstream_receipt_verification_split_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::precheck_upstream_receipt_verification_split_non_participation_stages(),
        current_stage_count,
        stages::planned_precheck_upstream_receipt_verification_split_non_participation_stage_count(),
        stages::first_precheck_upstream_receipt_verification_split_non_participation_release_number());
}

bool source_blocked_complete() {
    return source_blocked::published_stage_count() == source_blocked_planned_stage_count;
}

int completed_check_count() {
    return checks::planned_precheck_upstream_receipt_verification_split_non_participation_check_count();
}

} // namespace

std::string format_precheck_upstream_receipt_verification_split_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::precheck_upstream_receipt_verification_split_non_participation_stages(),
        current_stage_count);
}

std::string precheck_upstream_receipt_verification_split_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_precheck_upstream_receipt_verification_split_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_blocked::published_stage_count();
    const bool source_complete = source_blocked_complete();
    const int planned_check_count =
        checks::planned_precheck_upstream_receipt_verification_split_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool chain_complete =
        current_stage_count ==
            stages::planned_precheck_upstream_receipt_verification_split_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-precheck-upstream-receipt-verification-split-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationMode\":\"controlled-read-only-precheck-upstream-receipt-verification-split-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodePrecheckUpstreamReceiptVerificationSplitReleaseRange\":\"Node v1983-v2002\""
           ",\"sourceNodePrecheckUpstreamReceiptVerificationSplitCloseoutReleaseVersion\":\"Node v2002\""
           ",\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v108\""
           ",\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"non-participation-evidence\""
           ",\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v99\""
           ",\"sourceNodeConsumesHistoricalJavaEvidence\":\"receipt-evidence\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":" + json_bool(node_requires_fresh_java_evidence) +
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeServiceStartupRequired\":false"
           ",\"sourceNodePortsOpened\":false"
           ",\"sourceNodeCrossProjectWritesAllowed\":false"
           ",\"sourceNodePrecheckUpstreamReceiptVerificationSplitOnly\":true"
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationCommand\":\"SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON\""
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationContract\":\"shard-route-blocked-execution-rehearsal-split-non-participation.v1\""
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\""
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationFixturePath\":\"fixtures/release/shard-readiness-v1525.json\""
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationPublishedStageCount\":" +
           std::to_string(source_stage_count) +
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationComplete\":" + json_bool(source_complete) +
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationStatus\":" +
           json_string(source_blocked::blocked_execution_rehearsal_split_non_participation_status()) +
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationDigestMarker\":" +
           json_string(source_blocked::blocked_execution_rehearsal_split_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1525\""
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationStage\":" + json_string(stage.stage) +
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseRangeStart\":\"v1526\""
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_precheck_upstream_receipt_verification_split_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationDeclared\":true"
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationOnly\":true"
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationChainComplete\":" +
           json_bool(chain_complete) +
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationFixtureFrozen\":true"
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeServiceSplitObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"precheckUpstreamReceiptVerificationSplitFocus\":" +
           json_string_array({"public-loader-render-exports-preserved-no-import",
                              "types-module-no-import",
                              "constants-module-no-import",
                              "node-v245-source-adapter-no-parse",
                              "java-v99-receipt-reference-no-scan",
                              "minikv-v108-non-participation-reference-no-parse",
                              "reference-helpers-private-no-execution",
                              "check-policy-no-execution",
                              "message-policy-no-execution",
                              "core-assembly-no-execution",
                              "renderer-no-execution",
                              "entrypoint-orchestration-no-execution",
                              "code-health-scan-family-observed-only",
                              "large-file-inventory-observed-only",
                              "direct-tests-fallback-observed-only",
                              "downstream-code-health-observed-only",
                              "downstream-rehearsal-guard-observed-only",
                              "typecheck-build-observed-only",
                              "node-closeout-observed-only",
                              "clean-workspace-ci-closeout"}) +
           ",\"nodePrecheckUpstreamReceiptVerificationPublicLoaderImportedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationTypesImportedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationConstantsImportedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationReferencesParsedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationCheckPolicyExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationMessagePolicyExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationRendererExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationEntrypointExecutedByMiniKv\":false"
           ",\"nodeCodeHealthScanExecutedByMiniKv\":false"
           ",\"nodeLargeFileInventoryMutatedByMiniKv\":false"
           ",\"nodeDirectTestsExecutedByMiniKv\":false"
           ",\"nodeDownstreamCodeHealthExecutedByMiniKv\":false"
           ",\"nodeDownstreamRehearsalGuardExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvParsesHistoricalReceipts\":" + json_bool(mini_kv_parses_historical_receipts) +
           ",\"miniKvReadsHistoricalMiniKvV108NonParticipationFile\":false"
           ",\"miniKvScansJavaV99Evidence\":false"
           ",\"miniKvScansJavaEvidence\":" + json_bool(mini_kv_scans_java_evidence) +
           ",\"miniKvExecutesDownstreamChecks\":" + json_bool(mini_kv_executes_downstream_checks) +
           ",\"miniKvMutatesLargeFileInventory\":" + json_bool(mini_kv_mutates_large_file_inventory) +
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"schemaMigrationExecuted\":false"
           ",\"ledgerWriteExecuted\":false"
           ",\"externalRequestSent\":false"
           ",\"moduleSplit\":[\"precheck_upstream_receipt_verification_split_non_participation_core\",\"precheck_upstream_receipt_verification_split_non_participation_stages\",\"precheck_upstream_receipt_verification_split_non_participation_checks\",\"precheck_upstream_receipt_verification_split_non_participation_validation\"]"
           ",\"stageCatalog\":" +
           format_precheck_upstream_receipt_verification_split_non_participation_stage_catalog_json() +
           ",\"precheckUpstreamReceiptVerificationSplitNonParticipationChecks\":" +
           checks::format_precheck_upstream_receipt_verification_split_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_precheck_upstream_receipt_verification_split_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               node_requires_fresh_java_evidence,
               mini_kv_imports_node_modules,
               mini_kv_executes_node_checks,
               mini_kv_starts_services,
               mini_kv_reads_endpoints,
               mini_kv_reads_credentials,
               mini_kv_parses_historical_receipts,
               mini_kv_scans_java_evidence,
               mini_kv_executes_downstream_checks,
               mini_kv_mutates_large_file_inventory,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_precheck_upstream_receipt_verification_split_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1525 remains the frozen blocked execution rehearsal split non-participation source",
                              "Node v1983-v2002 splits precheck upstream receipt verification into public loader types constants references checks messages core renderer and orchestration modules",
                              "Node consumes historical Java v99 receipt evidence and mini-kv v108 non-participation evidence only; no fresh sibling work or service startup is required",
                              "mini-kv records the split without importing Node modules parsing historical receipts scanning Java evidence executing downstream checks mutating inventories reading endpoints credentials managed connections routers writes WAL or execution"}) +
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

std::string precheck_upstream_receipt_verification_split_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_precheck_upstream_receipt_verification_split_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation
