#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation_json(
    int check_count,
    int blocker_count,
    int profile_count,
    bool source_review_preflight_chain_complete,
    int published_stage_count) {
    const bool catalogs_aligned = check_count == blocker_count && blocker_count == profile_count;
    const bool published_checks_guarded =
        published_stage_count <= check_count && published_stage_count <= blocker_count &&
        published_stage_count <= profile_count;
    const bool validation_passed = catalogs_aligned && published_checks_guarded && source_review_preflight_chain_complete;

    return "{\"draftTextPackageReviewCloseoutAuditValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceReviewPreflightChainComplete\":" + json_bool(source_review_preflight_chain_complete) +
           ",\"auditCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"publishedAuditChecksGuarded\":" + json_bool(published_checks_guarded) +
           ",\"auditCheckCount\":" + std::to_string(check_count) +
           ",\"auditBlockerCount\":" + std::to_string(blocker_count) +
           ",\"auditProfileCount\":" + std::to_string(profile_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"readyForCloseoutAudit\":true"
           ",\"readyForDraftTextPackageReview\":false"
           ",\"draftTextPackageParsingAllowed\":false"
           ",\"draftTextPackageAcceptanceAllowed\":false"
           ",\"operatorValueImportAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_validation
