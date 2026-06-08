#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers {
namespace {

constexpr std::array<DraftTextPackageReviewCloseoutAuditBlocker, 25> audit_blockers = {{
    {1, "source_freeze_audit_blocker", "rolling_current_as_source", "requires v810 frozen fixture before audit evidence"},
    {2, "release_range_audit_blocker", "unbounded_release_range", "blocks unbounded review preflight ranges"},
    {3, "stage_sequence_audit_blocker", "non_contiguous_stage_sequence", "blocks non-contiguous stage sequence claims"},
    {4, "criterion_slot_count_audit_blocker", "missing_review_criterion", "blocks incomplete criterion catalogs"},
    {5, "rejection_control_count_audit_blocker", "missing_rejection_control", "blocks incomplete rejection controls"},
    {6, "profile_count_audit_blocker", "missing_review_profile", "blocks incomplete review profiles"},
    {7, "validation_pass_audit_blocker", "failed_review_preflight_validation", "blocks failed source validation"},
    {8, "digest_marker_audit_blocker", "missing_digest_marker", "blocks missing source digest markers"},
    {9, "source_fixture_path_audit_blocker", "unversioned_source_fixture", "blocks unversioned source fixture paths"},
    {10, "draft_text_absence_audit_blocker", "draft_text_package_material", "blocks draft text material in audit evidence"},
    {11, "parse_rejection_audit_blocker", "package_parse_attempt", "blocks audit-time package parsing"},
    {12, "acceptance_rejection_audit_blocker", "package_acceptance_attempt", "blocks audit-time package acceptance"},
    {13, "approval_grant_rejection_audit_blocker", "approval_grant_emission", "blocks approval grant emission"},
    {14, "operator_value_zero_count_audit_blocker", "operator_value_import", "blocks submitted accepted imported or persisted values"},
    {15, "sibling_import_rejection_audit_blocker", "automatic_sibling_import", "blocks automatic sibling imports"},
    {16, "runtime_payload_rejection_audit_blocker", "runtime_payload_acceptance", "blocks runtime payload acceptance"},
    {17, "active_router_rejection_audit_blocker", "active_router_installation", "blocks active router installation"},
    {18, "write_route_rejection_audit_blocker", "write_route_activation", "blocks write route activation"},
    {19, "wal_touch_rejection_audit_blocker", "wal_touch", "blocks WAL touch"},
    {20, "load_restore_compact_rejection_audit_blocker", "storage_admin_execution", "blocks LOAD RESTORE and COMPACT"},
    {21, "command_catalog_read_only_audit_blocker", "mutable_command_contract", "blocks mutable command metadata"},
    {22, "shardjson_embedding_audit_blocker", "missing_shardjson_embedding", "blocks missing SHARDJSON audit embedding"},
    {23, "versioned_fixture_catalog_audit_blocker", "missing_versioned_fixture", "blocks missing versioned fixture chain"},
    {24, "archive_explanation_presence_audit_blocker", "unexplained_command_archive", "blocks unexplained command evidence archives"},
    {25, "closeout_audit_summary_blocker", "post_closeout_execution", "blocks any execution after audit closeout"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(audit_blockers.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageReviewCloseoutAuditBlocker>
signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers() {
    return {audit_blockers.data(), audit_blockers.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blocker_count() {
    return static_cast<int>(audit_blockers.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers_json(
    int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& blocker = audit_blockers[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(blocker.sequence) +
                  ",\"blockerCode\":" + json_string(blocker.blocker_code) +
                  ",\"blocks\":" + json_string(blocker.blocks) +
                  ",\"reason\":" + json_string(blocker.reason) +
                  ",\"failClosed\":true"
                  ",\"draftTextPackageAcceptanceBlocked\":true"
                  ",\"runtimeExecutionBlocked\":true"
                  ",\"writeRouteBlocked\":true}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers
