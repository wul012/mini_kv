#include "minikv/shard_preview_candidate_request_package_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_request_package_validation {

std::string format_candidate_document_request_package_validation_json(int planned_item_count,
                                                                     int planned_check_count,
                                                                     int planned_gate_count,
                                                                     int completed_item_count,
                                                                     int completed_check_count,
                                                                     int completed_gate_count,
                                                                     bool source_intake_audit_chain_complete,
                                                                     int published_stage_count) {
    const bool planned_catalogs_aligned =
        planned_item_count == 15 && planned_check_count == 15 && planned_gate_count == 38;
    const bool completed_catalogs_within_bounds = completed_item_count >= 0 && completed_item_count <= planned_item_count &&
                                                  completed_check_count >= 0 && completed_check_count <= planned_check_count &&
                                                  completed_gate_count >= 0 && completed_gate_count <= planned_gate_count;
    const bool item_check_counts_aligned = completed_item_count == completed_check_count;
    const bool stage_count_valid = published_stage_count >= 1 && published_stage_count <= 15;
    const bool closeout_complete = published_stage_count == 15 && completed_item_count == 15 &&
                                   completed_check_count == 15 && completed_gate_count == 38;
    const bool validation_passed = source_intake_audit_chain_complete && planned_catalogs_aligned &&
                                   completed_catalogs_within_bounds && item_check_counts_aligned &&
                                   stage_count_valid;

    return "{\"candidateDocumentRequestPackageValidationPassed\":" +
           runtime_evidence::json_bool(validation_passed) +
           ",\"sourceIntakeAuditChainComplete\":" +
           runtime_evidence::json_bool(source_intake_audit_chain_complete) +
           ",\"plannedCatalogsAligned\":" + runtime_evidence::json_bool(planned_catalogs_aligned) +
           ",\"completedCatalogsWithinBounds\":" +
           runtime_evidence::json_bool(completed_catalogs_within_bounds) +
           ",\"itemCheckCountsAligned\":" + runtime_evidence::json_bool(item_check_counts_aligned) +
           ",\"stageCountValid\":" + runtime_evidence::json_bool(stage_count_valid) +
           ",\"closeoutComplete\":" + runtime_evidence::json_bool(closeout_complete) +
           ",\"plannedItemCount\":" + std::to_string(planned_item_count) +
           ",\"plannedCheckCount\":" + std::to_string(planned_check_count) +
           ",\"plannedGateCount\":" + std::to_string(planned_gate_count) +
           ",\"completedItemCount\":" + std::to_string(completed_item_count) +
           ",\"completedCheckCount\":" + std::to_string(completed_check_count) +
           ",\"completedGateCount\":" + std::to_string(completed_gate_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"realCandidateDocumentAccepted\":false"
           ",\"payloadImported\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_candidate_request_package_validation
