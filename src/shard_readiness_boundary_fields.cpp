#include "minikv/shard_readiness_boundary_fields.hpp"

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

namespace {

const std::vector<std::string>& boundary_catalog_groups() {
    static const std::vector<std::string> groups = {
        "top-level-read-only",
        "slot-table-preview",
        "slot-table-preview-audit",
        "slot-table-preview-audit-maintenance",
        "shard-readiness-release-catalog",
        "shard-readiness-release-catalog-audit",
        "core-boundaries",
        "historical-fallback",
        "shard-readiness-history-maintenance",
        "active-prototype-plan",
        "consumer-handoff",
        "live-read-gate",
        "operator-service-lifecycle",
        "runtime-execution-artifacts",
        "approval-gate-inputs",
        "final-mini-kv-approval-input",
        "template-validator-echo",
        "canonical-approval-input-precheck",
        "node-route-group-split-compatibility",
        "node-route-split-compatibility-window",
        "node-route-split-compatibility-window-audit",
        "node-route-catalog-closeout-snapshot",
        "node-route-catalog-anchor-removal-audit",
        "node-route-catalog-expected-integrity-snapshot-audit",
        "node-route-catalog-expected-integrity-snapshot-freeze",
        "node-route-catalog-evidence-chain",
        "node-route-catalog-evidence-chain-audit",
        "node-route-catalog-cleanup-closeout-handoff",
        "node-route-catalog-cleanup-closeout-handoff-audit",
        "node-route-catalog-cleanup-closeout-handoff-audit-freeze",
        "node-route-catalog-cleanup-closeout-release-catalog",
        "node-route-catalog-cleanup-closeout-release-catalog-audit",
        "node-route-catalog-cleanup-closeout-catalog-maintenance",
        "node-route-catalog-cleanup-latest-evidence-package",
        "node-route-catalog-cleanup-latest-evidence-package-audit",
        "node-route-catalog-cleanup-evidence-batch-closeout",
        "node-route-catalog-cleanup-evidence-batch-closeout-audit",
        "node-route-catalog-cleanup-post-closeout-continuity",
        "boundary-catalog-maintenance",
        "boundary-catalog-index",
    };
    return groups;
}

} // namespace

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
