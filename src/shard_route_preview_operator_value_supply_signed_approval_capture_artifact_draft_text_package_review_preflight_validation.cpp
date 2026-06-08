#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_text_package_review_preflight_validation_json(
    int criterion_count,
    int rejection_control_count,
    int profile_count,
    bool source_instruction_preflight_chain_complete,
    int published_stage_count) {
    const bool catalogs_aligned =
        criterion_count == rejection_control_count && rejection_control_count == profile_count;
    const bool published_criteria_guarded =
        published_stage_count <= criterion_count && published_stage_count <= rejection_control_count &&
        published_stage_count <= profile_count;
    const bool validation_passed =
        catalogs_aligned && published_criteria_guarded && source_instruction_preflight_chain_complete;

    return "{\"draftTextPackageReviewPreflightValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceInstructionPreflightChainComplete\":" +
           json_bool(source_instruction_preflight_chain_complete) +
           ",\"reviewCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"publishedReviewCriteriaGuarded\":" + json_bool(published_criteria_guarded) +
           ",\"reviewCriterionCount\":" + std::to_string(criterion_count) +
           ",\"rejectionControlCount\":" + std::to_string(rejection_control_count) +
           ",\"reviewProfileCount\":" + std::to_string(profile_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"readyForDraftTextPackageReview\":true"
           ",\"readyForSignedApprovalCapture\":false"
           ",\"draftTextPackageParsingAllowed\":false"
           ",\"draftTextPackageAcceptanceAllowed\":false"
           ",\"detachedSignaturePayloadAllowed\":false"
           ",\"operatorValueImportAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation