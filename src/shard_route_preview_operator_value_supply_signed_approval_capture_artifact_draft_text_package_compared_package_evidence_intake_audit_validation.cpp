#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation {

std::string format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation_json(
    int planned_slot_count,
    int planned_guard_count,
    int completed_slot_count,
    int completed_guard_count,
    bool source_comparison_closeout_audit_chain_complete,
    int published_stage_count) {
    const bool planned_catalogs_aligned = planned_slot_count == 10 && planned_guard_count == 10;
    const bool completed_catalogs_aligned =
        completed_slot_count >= completed_guard_count && completed_slot_count - completed_guard_count <= 1;
    const bool stage_count_valid = published_stage_count >= 1 && published_stage_count <= 20;
    const bool validation_passed =
        source_comparison_closeout_audit_chain_complete && planned_catalogs_aligned && completed_catalogs_aligned &&
        stage_count_valid;

    return "{\"comparedPackageEvidenceIntakeAuditValidationPassed\":" +
           runtime_evidence::json_bool(validation_passed) +
           ",\"sourceComparisonCloseoutAuditChainComplete\":" +
           runtime_evidence::json_bool(source_comparison_closeout_audit_chain_complete) +
           ",\"plannedCatalogsAligned\":" + runtime_evidence::json_bool(planned_catalogs_aligned) +
           ",\"completedCatalogsAligned\":" + runtime_evidence::json_bool(completed_catalogs_aligned) +
           ",\"stageCountValid\":" + runtime_evidence::json_bool(stage_count_valid) +
           ",\"plannedSlotCount\":" + std::to_string(planned_slot_count) +
           ",\"plannedGuardCount\":" + std::to_string(planned_guard_count) +
           ",\"completedSlotCount\":" + std::to_string(completed_slot_count) +
           ",\"completedGuardCount\":" + std::to_string(completed_guard_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"payloadAccepted\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation