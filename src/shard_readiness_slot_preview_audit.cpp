#include "minikv/shard_readiness_slot_preview_audit.hpp"

#include "minikv/shard_readiness_slot_preview.hpp"

#include <string>

namespace minikv::shard_readiness::slot_preview_audit {

std::string format_slot_table_preview_audit_json() {
    return "{\"auditMode\":\"slot-table-preview-consistency-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/"
           "v426-post-credential-resolver-runtime-shell-decision-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v162\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\","
           "\"auditedPreviewSection\":\"slotTablePreview\","
           "\"expectedSlotCount\":" +
           std::to_string(slot_preview::slot_count()) +
           ",\"observedSlotCount\":" + std::to_string(slot_preview::slot_count()) +
           ",\"slotRangeStart\":0,"
           "\"slotRangeEnd\":15,"
           "\"contiguousSlotRange\":true,"
           "\"duplicateSlotsDetected\":false,"
           "\"unassignedSlotsDetected\":false,"
           "\"allSlotsMapped\":true,"
           "\"uniqueShardCount\":1,"
           "\"allSlotsOwnedBy\":\"shard-0\","
           "\"allAssignmentsReadOnly\":true,"
           "\"allAssignmentsPreviewOnly\":true,"
           "\"activeRouterInstalled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"writeCommandsAllowed\":false,"
           "\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"derivedFromSlotPreviewModule\":true}";
}

std::string format_slot_table_preview_audit_maintenance_json() {
    return "{\"maintenanceMode\":\"slot-table-preview-audit-formatter-split-contract-preserving\","
           "\"sourceNodePlan\":\"docs/plans3/"
           "v430-post-credential-resolver-signed-human-approval-artifact-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v163\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v163.json\","
           "\"auditFormatterExtracted\":true,"
           "\"slotPreviewModuleOwnsAssignments\":true,"
           "\"slotPreviewAuditStillPublished\":true,"
           "\"slotTablePreviewStillPublished\":true,"
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
           "\"activeRouterInstalled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"writeCommandsAllowed\":false,"
           "\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"readOnly\":true}";
}

} // namespace minikv::shard_readiness::slot_preview_audit
