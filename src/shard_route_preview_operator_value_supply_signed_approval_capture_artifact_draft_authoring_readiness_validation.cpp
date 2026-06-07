#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation {

std::string format_signed_approval_capture_artifact_draft_authoring_readiness_validation_json(int requirement_count,
                                                                                              int blocker_count,
                                                                                              int profile_count,
                                                                                              bool source_chain_complete,
                                                                                              int published_stage_count) {
    const bool published_count_valid = published_stage_count >= 0 && published_stage_count <= 25;
    const bool catalogs_aligned = requirement_count == blocker_count && blocker_count == profile_count;
    const bool validation_passed = source_chain_complete && published_count_valid && catalogs_aligned;
    return "{\"authoringReadinessValidationPassed\":" + runtime_evidence::json_bool(validation_passed) +
           ",\"sourceArtifactPreflightChainComplete\":" + runtime_evidence::json_bool(source_chain_complete) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":25"
           ",\"requirementCount\":" + std::to_string(requirement_count) +
           ",\"blockerCount\":" + std::to_string(blocker_count) +
           ",\"profileCount\":" + std::to_string(profile_count) +
           ",\"catalogsAligned\":" + runtime_evidence::json_bool(catalogs_aligned) +
           ",\"readyForDraftAuthoringReview\":true"
           ",\"readyForSignedApprovalArtifactDraft\":false"
           ",\"draftTextGenerationAllowed\":false"
           ",\"artifactMaterializationAllowed\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation
