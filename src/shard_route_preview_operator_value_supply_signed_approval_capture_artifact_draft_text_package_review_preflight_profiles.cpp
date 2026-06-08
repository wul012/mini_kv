#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles {
namespace {

constexpr std::array<DraftTextPackageReviewPreflightProfile, 25> review_profiles = {{
    {1, "draft_text_package_review_preflight_package_manifest_criterion_profile", "manifest-only", "review package manifest without package acceptance"},
    {2, "draft_text_package_review_preflight_instruction_preflight_digest_criterion_profile", "digest-only", "review instruction preflight digest without package acceptance"},
    {3, "draft_text_package_review_preflight_package_schema_version_criterion_profile", "schema-metadata-only", "review package schema version without package acceptance"},
    {4, "draft_text_package_review_preflight_request_correlation_criterion_profile", "correlation-only", "review request correlation without package acceptance"},
    {5, "draft_text_package_review_preflight_artifact_digest_binding_criterion_profile", "digest-only", "review artifact digest binding without package acceptance"},
    {6, "draft_text_package_review_preflight_template_digest_binding_criterion_profile", "digest-only", "review template digest binding without package acceptance"},
    {7, "draft_text_package_review_preflight_review_digest_binding_criterion_profile", "digest-only", "review review digest binding without package acceptance"},
    {8, "draft_text_package_review_preflight_detached_signature_envelope_criterion_profile", "signature-envelope-only", "review detached signature envelope without package acceptance"},
    {9, "draft_text_package_review_preflight_signature_algorithm_criterion_profile", "signature-policy-only", "review signature algorithm without package acceptance"},
    {10, "draft_text_package_review_preflight_signature_redaction_criterion_profile", "signature-redaction-only", "review signature redaction without package acceptance"},
    {11, "draft_text_package_review_preflight_source_release_handle_criterion_profile", "source-handle-only", "review source release handle without package acceptance"},
    {12, "draft_text_package_review_preflight_source_file_handle_criterion_profile", "source-handle-only", "review source file handle without package acceptance"},
    {13, "draft_text_package_review_preflight_source_snippet_handle_criterion_profile", "source-handle-only", "review source snippet handle without package acceptance"},
    {14, "draft_text_package_review_preflight_operator_identity_handle_criterion_profile", "identity-handle-only", "review operator identity handle without package acceptance"},
    {15, "draft_text_package_review_preflight_operator_role_handle_criterion_profile", "role-handle-only", "review operator role handle without package acceptance"},
    {16, "draft_text_package_review_preflight_redacted_value_digest_criterion_profile", "redacted-digest-only", "review redacted value digest without package acceptance"},
    {17, "draft_text_package_review_preflight_value_shape_criterion_profile", "shape-metadata-only", "review value shape without package acceptance"},
    {18, "draft_text_package_review_preflight_redaction_policy_criterion_profile", "policy-only", "review redaction policy without package acceptance"},
    {19, "draft_text_package_review_preflight_provenance_policy_criterion_profile", "policy-only", "review provenance policy without package acceptance"},
    {20, "draft_text_package_review_preflight_separate_reviewer_control_profile", "review-control-only", "review separate reviewer without package acceptance"},
    {21, "draft_text_package_review_preflight_unreviewable_package_rejection_control_profile", "rejection-control-only", "review unreviewable package rejection without package acceptance"},
    {22, "draft_text_package_review_preflight_runtime_payload_rejection_control_profile", "rejection-control-only", "review runtime payload rejection without package acceptance"},
    {23, "draft_text_package_review_preflight_write_route_rejection_control_profile", "rejection-control-only", "review write route rejection without package acceptance"},
    {24, "draft_text_package_review_preflight_sibling_non_mutation_control_profile", "non-mutation-only", "review sibling non-mutation without package acceptance"},
    {25, "draft_text_package_review_preflight_closeout_profile", "closeout-only", "review archive closeout without package acceptance"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(review_profiles.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageReviewPreflightProfile> signed_approval_capture_artifact_draft_text_package_review_preflight_profiles() {
    return {review_profiles.data(), review_profiles.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_review_preflight_profile_count() {
    return static_cast<int>(review_profiles.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles_json(int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& profile = review_profiles[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(profile.sequence) +
                  ",\"profileCode\":" + json_string(profile.profile_code) +
                  ",\"boundary\":" + json_string(profile.boundary) +
                  ",\"reviewPrompt\":" + json_string(profile.review_prompt) +
                  ",\"readOnly\":true"
                  ",\"draftTextPackageAcceptanceAllowed\":false"
                  ",\"approvalGrantAllowed\":false"
                  ",\"operatorValueImportAllowed\":false"
                  ",\"executionAllowed\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles