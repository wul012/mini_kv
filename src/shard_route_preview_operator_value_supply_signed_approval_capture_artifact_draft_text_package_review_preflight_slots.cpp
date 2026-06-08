#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots {
namespace {

constexpr std::array<DraftTextPackageReviewPreflightSlot, 25> review_criteria = {{
    {1, "package_manifest_criterion", "package manifest", "starts draft text package review preflight with package manifest criteria and no package parsing"},
    {2, "instruction_preflight_digest_criterion", "instruction preflight digest", "binds the v785 instruction preflight closeout digest before any review package is accepted"},
    {3, "package_schema_version_criterion", "package schema version", "checks the expected package schema version without parsing submitted text"},
    {4, "request_correlation_criterion", "request correlation", "requires request correlation metadata while keeping approval capture closed"},
    {5, "artifact_digest_binding_criterion", "artifact digest binding", "requires artifact digest binding without materializing artifacts"},
    {6, "template_digest_binding_criterion", "template digest binding", "requires template digest binding without generating draft text"},
    {7, "review_digest_binding_criterion", "review digest binding", "requires review digest binding without writing approval statements"},
    {8, "detached_signature_envelope_criterion", "detached signature envelope", "reviews detached signature envelope metadata while payload bytes stay absent"},
    {9, "signature_algorithm_criterion", "signature algorithm", "reviews signature algorithm metadata without accepting signatures"},
    {10, "signature_redaction_criterion", "signature redaction", "requires signature redaction policy and keeps raw signature material blocked"},
    {11, "source_release_handle_criterion", "source release handle", "reviews source release handles from frozen evidence only"},
    {12, "source_file_handle_criterion", "source file handle", "reviews source file handles without walking the filesystem"},
    {13, "source_snippet_handle_criterion", "source snippet handle", "reviews source snippet handles without assembling draft text"},
    {14, "operator_identity_handle_criterion", "operator identity handle", "reviews operator identity handles without credential values"},
    {15, "operator_role_handle_criterion", "operator role handle", "reviews operator role handles without granting authority"},
    {16, "redacted_value_digest_criterion", "redacted value digest", "reviews redacted value digests while raw values remain absent"},
    {17, "value_shape_criterion", "value shape", "reviews value shape metadata without accepting supplied values"},
    {18, "redaction_policy_criterion", "redaction policy", "reviews redaction policy without secret storage"},
    {19, "provenance_policy_criterion", "provenance policy", "reviews provenance policy without mutable provenance"},
    {20, "separate_reviewer_control", "separate reviewer", "requires separate reviewer evidence while capture remains closed"},
    {21, "unreviewable_package_rejection_control", "unreviewable package rejection", "rejects unreviewable packages before parsing or acceptance"},
    {22, "runtime_payload_rejection_control", "runtime payload rejection", "rejects runtime payloads and keeps live reads disabled"},
    {23, "write_route_rejection_control", "write route rejection", "rejects write routes and keeps router activation disabled"},
    {24, "sibling_non_mutation_control", "sibling non-mutation", "proves Node Java and sibling mini-kv state are not mutated"},
    {25, "closeout", "archive closeout", "closes draft text package review preflight without accepting a draft text package"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(review_criteria.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageReviewPreflightSlot> signed_approval_capture_artifact_draft_text_package_review_preflight_slots() {
    return {review_criteria.data(), review_criteria.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_review_preflight_slot_count() {
    return static_cast<int>(review_criteria.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_preflight_slots_json(int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& criterion = review_criteria[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(criterion.sequence) +
                  ",\"criterionCode\":" + json_string(criterion.slot_code) +
                  ",\"reviewSubject\":" + json_string(criterion.source_instruction) +
                  ",\"meaning\":" + json_string(criterion.meaning) +
                  ",\"reviewCriterionPrepared\":true"
                  ",\"draftTextPackageParsed\":false"
                  ",\"draftTextPackageAccepted\":false"
                  ",\"acceptsSignatureMaterial\":false"
                  ",\"importsOperatorValue\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots