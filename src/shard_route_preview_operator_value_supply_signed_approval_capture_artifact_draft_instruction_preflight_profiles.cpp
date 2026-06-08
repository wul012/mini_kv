#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles {
namespace {

constexpr std::array<InstructionPreflightProfile, 25> instruction_profiles = {{
    {1, "instruction_preflight_request_manifest_profile", "manifest-only", "review the request manifest slot only"},
    {2, "instruction_preflight_artifact_preflight_digest_profile", "digest-only", "review the artifact preflight digest slot only"},
    {3, "instruction_preflight_template_digest_profile", "digest-only", "review the template digest slot only"},
    {4, "instruction_preflight_review_digest_profile", "digest-only", "review the review digest slot only"},
    {5, "instruction_preflight_operator_identity_profile", "identity-metadata-only", "review the operator identity slot without credentials"},
    {6, "instruction_preflight_operator_role_profile", "role-metadata-only", "review the operator role slot without grants"},
    {7, "instruction_preflight_window_id_profile", "window-metadata-only", "review the window id slot without capture"},
    {8, "instruction_preflight_channel_policy_profile", "channel-policy-only", "review the channel policy slot without endpoints"},
    {9, "instruction_preflight_signature_algorithm_profile", "signature-policy-only", "review the signature algorithm slot without signatures"},
    {10, "instruction_preflight_detached_signature_profile", "detached-signature-policy-only", "review detached signature handling without payloads"},
    {11, "instruction_preflight_signature_redaction_profile", "signature-redaction-only", "review signature redaction without raw bytes"},
    {12, "instruction_preflight_approval_statement_digest_profile", "statement-digest-only", "review approval statement digest without text"},
    {13, "instruction_preflight_source_version_profile", "source-citation-only", "review source version citation"},
    {14, "instruction_preflight_source_file_profile", "source-citation-only", "review source file citation"},
    {15, "instruction_preflight_source_snippet_profile", "source-citation-only", "review source snippet citation"},
    {16, "instruction_preflight_redacted_value_digest_profile", "redacted-digest-only", "review redacted value digest without raw material"},
    {17, "instruction_preflight_value_shape_profile", "shape-metadata-only", "review value shape without accepting values"},
    {18, "instruction_preflight_redaction_policy_profile", "policy-only", "review redaction policy"},
    {19, "instruction_preflight_provenance_policy_profile", "policy-only", "review provenance policy"},
    {20, "instruction_preflight_raw_secret_embargo_profile", "embargo-only", "review raw secret embargo"},
    {21, "instruction_preflight_approval_grant_embargo_profile", "embargo-only", "review approval grant embargo"},
    {22, "instruction_preflight_zero_value_import_embargo_profile", "embargo-only", "review zero value import embargo"},
    {23, "instruction_preflight_write_route_embargo_profile", "embargo-only", "review write route embargo"},
    {24, "instruction_preflight_sibling_non_mutation_profile", "non-mutation-only", "review sibling non-mutation"},
    {25, "instruction_preflight_closeout_profile", "closeout-only", "review closeout without execution"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(instruction_profiles.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const InstructionPreflightProfile> signed_approval_capture_artifact_draft_instruction_preflight_profiles() {
    return {instruction_profiles.data(), instruction_profiles.size()};
}

int planned_signed_approval_capture_artifact_draft_instruction_preflight_profile_count() {
    return static_cast<int>(instruction_profiles.size());
}

std::string format_signed_approval_capture_artifact_draft_instruction_preflight_profiles_json(int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& profile = instruction_profiles[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(profile.sequence) +
                  ",\"profileCode\":" + json_string(profile.profile_code) +
                  ",\"boundary\":" + json_string(profile.boundary) +
                  ",\"reviewPrompt\":" + json_string(profile.review_prompt) +
                  ",\"readOnly\":true"
                  ",\"instructionTextAllowed\":false"
                  ",\"approvalGrantAllowed\":false"
                  ",\"operatorValueImportAllowed\":false"
                  ",\"executionAllowed\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles
