#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_type_barrel_split_follow_up_fixture_audit_validation_json(
    int source_follow_up_stage_count,
    bool source_follow_up_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_runtime_reads_source_fixture_payload,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_follow_up_stage_count == 20 && source_follow_up_complete;
    const bool audit_closed = !node_requires_fresh_minikv_evidence &&
                              !mini_kv_runtime_reads_source_fixture_payload && !mini_kv_imports_node_modules &&
                              !mini_kv_executes_node_checks && !mini_kv_starts_services;
    const bool counts_aligned = planned_check_count == 12 && completed_check_count == 12 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 12;
    const bool validation_passed = source_locked && audit_closed && counts_aligned;

    return "{\"typeBarrelSplitFollowUpFixtureAuditValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceFollowUpNonParticipationPublishedStageCount\":" + std::to_string(source_follow_up_stage_count) +
           ",\"sourceFollowUpNonParticipationComplete\":" + json_bool(source_follow_up_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvRuntimeReadsSourceFixturePayload\":" + json_bool(mini_kv_runtime_reads_source_fixture_payload) +
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"auditClosed\":" + json_bool(audit_closed) +
           ",\"plannedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"sourceFollowUpNonParticipationFixtureFrozen\":true"
           ",\"currentFixtureParityAudited\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeStableBarrelImportedByMiniKv\":false"
           ",\"nodeProfileTypesImportedByMiniKv\":false"
           ",\"nodeTestOnlyShellModulesImportedByMiniKv\":false"
           ",\"nodeCredentialResolverShellExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"activeRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_validation
