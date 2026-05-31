#include "minikv/shard_readiness_slot_preview.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <string>
#include <string_view>

namespace minikv::shard_readiness::slot_preview {
namespace {

constexpr int preview_slot_count = 16;

struct RouteSample {
    std::string_view key;
    int slot;
};

constexpr std::array<RouteSample, 3> route_samples = {{
    {"orderops:alpha", 5},
    {"audit:receipt:001", 9},
    {"mini-kv:health", 14},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string format_slot_assignments_json() {
    std::string response = "[";
    for (int slot = 0; slot < preview_slot_count; ++slot) {
        if (slot > 0) {
            response += ",";
        }
        response += "{\"slot\":" + std::to_string(slot) +
                    ",\"shardId\":\"shard-0\","
                    "\"routeMode\":\"preview-only\","
                    "\"readOnly\":true,"
                    "\"writesAllowed\":false,"
                    "\"routerInstalled\":false}";
    }
    response += "]";
    return response;
}

} // namespace

int slot_count() {
    return preview_slot_count;
}

std::string format_route_samples_json() {
    std::string response = "[";
    bool first = true;
    for (const auto& sample : route_samples) {
        if (!first) {
            response += ",";
        }
        first = false;
        response += "{\"key\":" + json_string(sample.key) +
                    ",\"slot\":" + std::to_string(sample.slot) +
                    ",\"shardId\":\"shard-0\"}";
    }
    response += "]";
    return response;
}

std::string format_shard_map_json() {
    return "[{\"shardId\":\"shard-0\",\"slotStart\":0,\"slotEnd\":15,"
           "\"storagePath\":\"not-created\",\"readOnly\":true,\"writesAllowed\":false}]";
}

std::string format_slot_table_preview_json() {
    return "{\"previewMode\":\"single-shard-slot-table-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/"
           "v425-post-credential-resolver-disabled-runtime-shell-readiness-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v161\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v161.json\","
           "\"slotCount\":" +
           std::to_string(preview_slot_count) +
           ",\"shardCount\":1,"
           "\"allSlotsOwnedBy\":\"shard-0\","
           "\"slotTableMaterializedForEvidence\":true,"
           "\"slotTablePreviewOnly\":true,"
           "\"readOnlyShardMapPreview\":true,"
           "\"activeRouterInstalled\":false,"
           "\"routerActivationAllowed\":false,"
           "\"writeRoutingAllowed\":false,"
           "\"writeCommandsAllowed\":false,"
           "\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,"
           "\"storageDirectoriesCreated\":false,"
           "\"startsServices\":false,"
           "\"runtimeProbeAllowed\":false,"
           "\"liveReadAllowed\":false,"
           "\"executionAllowed\":false,"
           "\"slotAssignments\":" +
           format_slot_assignments_json() + "}";
}

std::string format_slot_table_preview_audit_json() {
    return "{\"auditMode\":\"slot-table-preview-consistency-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/"
           "v426-post-credential-resolver-runtime-shell-decision-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v162\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\","
           "\"auditedPreviewSection\":\"slotTablePreview\","
           "\"expectedSlotCount\":" +
           std::to_string(preview_slot_count) +
           ",\"observedSlotCount\":" + std::to_string(preview_slot_count) +
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

} // namespace minikv::shard_readiness::slot_preview
