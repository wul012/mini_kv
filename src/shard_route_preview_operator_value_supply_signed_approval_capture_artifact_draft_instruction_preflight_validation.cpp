#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <string>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_instruction_preflight_validation_json(
    int slot_count,
    int guard_count,
    int profile_count,
    bool source_authoring_readiness_chain_complete,
    int published_stage_count) {
    const bool catalogs_aligned = slot_count == guard_count && guard_count == profile_count;
    const bool published_slots_guarded =
        published_stage_count <= slot_count && published_stage_count <= guard_count &&
        published_stage_count <= profile_count;
    const bool validation_passed =
        catalogs_aligned && published_slots_guarded && source_authoring_readiness_chain_complete;

    return "{\"instructionPreflightValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceAuthoringReadinessChainComplete\":" +
           json_bool(source_authoring_readiness_chain_complete) +
           ",\"instructionCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"publishedInstructionSlotsGuarded\":" + json_bool(published_slots_guarded) +
           ",\"slotCount\":" + std::to_string(slot_count) +
           ",\"guardCount\":" + std::to_string(guard_count) +
           ",\"profileCount\":" + std::to_string(profile_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"readyForDraftInstructionReview\":true"
           ",\"readyForSignedApprovalArtifactDraft\":false"
           ",\"draftInstructionTextGenerationAllowed\":false"
           ",\"instructionMaterializationAllowed\":false"
           ",\"detachedSignaturePayloadAllowed\":false"
           ",\"operatorValueImportAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_validation