#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_blocked_execution_rehearsal_split_non_participation_validation_json(
    int source_disabled_fake_harness_stage_count,
    bool source_disabled_fake_harness_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    bool mini_kv_reads_endpoints,
    bool mini_kv_reads_credentials,
    bool mini_kv_parses_historical_receipts,
    bool mini_kv_scans_java_evidence,
    bool mini_kv_replays_wal_regression,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_disabled_fake_harness_stage_count == 15 &&
                               source_disabled_fake_harness_complete;
    const bool node_plan_observed_only = !node_requires_fresh_minikv_evidence;
    const bool mini_kv_boundary_closed =
        !mini_kv_imports_node_modules && !mini_kv_executes_node_checks && !mini_kv_starts_services &&
        !mini_kv_reads_endpoints && !mini_kv_reads_credentials && !mini_kv_parses_historical_receipts &&
        !mini_kv_scans_java_evidence && !mini_kv_replays_wal_regression;
    const bool counts_aligned = planned_check_count == 20 && completed_check_count == 20 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 20;
    const bool validation_passed =
        source_locked && node_plan_observed_only && mini_kv_boundary_closed && counts_aligned;

    return "{\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitPublishedStageCount\":" +
           std::to_string(source_disabled_fake_harness_stage_count) +
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitComplete\":" +
           json_bool(source_disabled_fake_harness_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"nodePlanObservedOnly\":" + json_bool(node_plan_observed_only) +
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvParsesHistoricalReceipts\":" + json_bool(mini_kv_parses_historical_receipts) +
           ",\"miniKvScansJavaEvidence\":" + json_bool(mini_kv_scans_java_evidence) +
           ",\"miniKvReplaysWalRegression\":" + json_bool(mini_kv_replays_wal_regression) +
           ",\"miniKvBoundaryClosed\":" + json_bool(mini_kv_boundary_closed) +
           ",\"plannedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true"
           ",\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false"
           ",\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false"
           ",\"nodePreconditionIntakeExecutedByMiniKv\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_validation
