#include "minikv/shard_readiness_boundary_fields.hpp"

#include "shard_readiness_boundary_catalog_groups.hpp"
#include "shard_readiness_boundary_field_names.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <vector>

namespace minikv::shard_readiness::boundary_fields {
namespace {

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

} // namespace

std::string format_catalog_maintenance_json() {
    return "{\"maintenanceMode\":\"boundary-field-catalog-split-contract-preserving\","
           "\"sourceNodePlan\":\"docs/plans3/v418-post-sandbox-endpoint-credential-resolver-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v159\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v159.json\","
           "\"boundaryFieldCatalogExtracted\":true,"
           "\"readOnlyBoundaryFieldsStillPublished\":true,"
           "\"publicShardJsonContractChanged\":false,"
           "\"fixturePathChanged\":false,"
           "\"shardJsonCommandChanged\":false,"
           "\"nodeRouteGroupSplitCompatibilityPreserved\":true,"
           "\"nodeAddsEvidenceGate\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"runtimeExecutionPacketPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"startsJavaService\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"writeCommandsAllowed\":false,"
           "\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"readOnly\":true}";
}

std::string format_catalog_index_json() {
    const auto& fields = read_only_boundary_field_names();
    const auto& groups = boundary_catalog_groups();
    return std::string{"{\"catalogVersion\":\"read-only-boundary-fields.v29\","} +
           "\"sourceNodePlan\":\"docs/plans3/v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v200\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\","
           "\"fieldCount\":" + std::to_string(fields.size()) +
           ",\"groupCount\":" + std::to_string(groups.size()) +
           ",\"fieldListGeneratedFromSharedVector\":true,"
           "\"catalogGroups\":" + json_string_array(groups) +
           ",\"slotTablePreviewCataloged\":true"
           ",\"slotTablePreviewAuditCataloged\":true"
           ",\"slotTablePreviewAuditMaintenanceCataloged\":true"
           ",\"shardReadinessReleaseCatalogCataloged\":true"
           ",\"shardReadinessReleaseCatalogAuditCataloged\":true"
           ",\"readOnlyBoundaryFieldsStillPublished\":true,"
           "\"nodeRouteSplitCompatibilityWindowCataloged\":true,"
           "\"nodeRouteSplitCompatibilityWindowAuditCataloged\":true,"
           "\"nodeRouteCatalogCloseoutSnapshotCataloged\":true,"
           "\"nodeRouteCatalogAnchorRemovalAuditCataloged\":true,"
           "\"nodeRouteCatalogExpectedIntegritySnapshotAuditCataloged\":true,"
           "\"nodeRouteCatalogExpectedIntegritySnapshotFreezeCataloged\":true,"
           "\"nodeRouteCatalogEvidenceChainCataloged\":true,"
           "\"nodeRouteCatalogEvidenceChainAuditCataloged\":true,"
           "\"nodeRouteCatalogCleanupCloseoutHandoffCataloged\":true,"
           "\"nodeRouteCatalogCleanupCloseoutHandoffAuditCataloged\":true,"
           "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreezeCataloged\":true,"
           "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogCataloged\":true,"
           "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAuditCataloged\":true,"
           "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenanceCataloged\":true,"
           "\"nodeRouteCatalogCleanupLatestEvidencePackageCataloged\":true,"
           "\"nodeRouteCatalogCleanupLatestEvidencePackageAuditCataloged\":true,"
           "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutCataloged\":true,"
           "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAuditCataloged\":true,"
           "\"nodeRouteCatalogCleanupPostCloseoutContinuityCataloged\":true,"
           "\"shardReadinessHistoryMaintenanceCataloged\":true,"
           "\"boundaryCatalogMaintenancePreserved\":true,"
           "\"publicShardJsonContractChanged\":false,"
           "\"fixturePathChanged\":false,"
           "\"shardJsonCommandChanged\":false,"
           "\"nodeAddsEvidenceGate\":false,"
           "\"runtimeGateApprovalPresent\":false,"
           "\"runtimeExecutionPacketPresent\":false,"
           "\"runtimeExecutionPacketExecutable\":false,"
           "\"startsJavaService\":false,"
           "\"startsMiniKvService\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"activeShardPrototypeEnabled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"writeCommandsAllowed\":false,"
           "\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"readOnly\":true}";
}

std::string format_read_only_boundary_fields_json() {
    return json_string_array(read_only_boundary_field_names());
}

} // namespace minikv::shard_readiness::boundary_fields
