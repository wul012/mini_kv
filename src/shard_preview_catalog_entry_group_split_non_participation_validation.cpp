#include "minikv/shard_preview_catalog_entry_group_split_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_catalog_entry_group_split_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_catalog_entry_group_split_non_participation_validation_json(
    int source_integrity_stage_count,
    bool source_integrity_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_catalog_split,
    bool mini_kv_reads_catalog_endpoint,
    bool mini_kv_consumes_catalog_records,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_integrity_stage_count == 35 && source_integrity_complete;
    const bool catalog_split_closed = !node_requires_fresh_minikv_evidence &&
                                      !mini_kv_starts_catalog_split &&
                                      !mini_kv_reads_catalog_endpoint &&
                                      !mini_kv_consumes_catalog_records;
    const bool counts_aligned = planned_check_count == 30 && completed_check_count == 30 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 30;
    const bool validation_passed = source_locked && catalog_split_closed && counts_aligned;

    return "{\"catalogEntryGroupSplitNonParticipationValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_integrity_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvStartsCatalogSplit\":" + json_bool(mini_kv_starts_catalog_split) +
           ",\"miniKvReadsCatalogEndpoint\":" + json_bool(mini_kv_reads_catalog_endpoint) +
           ",\"miniKvConsumesCatalogRecords\":" + json_bool(mini_kv_consumes_catalog_records) +
           ",\"catalogSplitClosed\":" + json_bool(catalog_split_closed) +
           ",\"plannedCatalogEntryGroupSplitNonParticipationCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedCatalogEntryGroupSplitNonParticipationCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeCatalogEntryRecordsImportedByMiniKv\":false"
           ",\"nodeCatalogRegistryExecutedByMiniKv\":false"
           ",\"nodeCatalogMarkdownRenderedByMiniKv\":false"
           ",\"nodeCatalogEndpointReadByMiniKv\":false"
           ",\"nodeRouteTestsExecutedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvCatalogSplitStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"catalogEndpointRead\":false"
           ",\"catalogRecordsImported\":false"
           ",\"catalogRegistryExecuted\":false"
           ",\"catalogMarkdownRendered\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"activeRouterInstalled\":false"
           ",\"catalogRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_catalog_entry_group_split_non_participation_validation
