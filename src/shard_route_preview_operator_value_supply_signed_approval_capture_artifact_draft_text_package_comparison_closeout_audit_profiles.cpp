#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles {
namespace {

constexpr std::array<DraftTextPackageComparisonCloseoutAuditProfile, 25> audit_profiles = {{
    {1, "draft_text_package_comparison_closeout_audit_source_freeze_profile", "source-fixture-only", "review v835 frozen fixture evidence only"},
    {2, "draft_text_package_comparison_closeout_audit_release_range_profile", "release-range-only", "review comparison range evidence only"},
    {3, "draft_text_package_comparison_closeout_audit_comparison_lane_catalog_sequence_profile", "stage-chain-only", "review contiguous comparison lane evidence only"},
    {4, "draft_text_package_comparison_closeout_audit_identity_lane_profile", "identity-handle-only", "review identity handles without package acceptance"},
    {5, "draft_text_package_comparison_closeout_audit_request_correlation_lane_profile", "correlation-only", "review request correlation metadata only"},
    {6, "draft_text_package_comparison_closeout_audit_artifact_digest_binding_lane_profile", "digest-only", "review artifact digest binding only"},
    {7, "draft_text_package_comparison_closeout_audit_template_digest_binding_lane_profile", "digest-only", "review template digest binding only"},
    {8, "draft_text_package_comparison_closeout_audit_review_digest_recheck_control_profile", "digest-recheck-only", "review digest recheck evidence only"},
    {9, "draft_text_package_comparison_closeout_audit_detached_signature_envelope_lane_profile", "signature-envelope-only", "review detached signature envelope metadata only"},
    {10, "draft_text_package_comparison_closeout_audit_signature_metadata_lane_profile", "signature-metadata-only", "review signature metadata only"},
    {11, "draft_text_package_comparison_closeout_audit_signature_payload_parse_rejection_profile", "parse-rejection-only", "review signature parse lock evidence only"},
    {12, "draft_text_package_comparison_closeout_audit_source_evidence_handle_lane_profile", "source-handle-only", "review source evidence handles only"},
    {13, "draft_text_package_comparison_closeout_audit_source_snippet_digest_lane_profile", "source-digest-only", "review source snippet digest handles only"},
    {14, "draft_text_package_comparison_closeout_audit_operator_value_handle_lane_profile", "value-handle-only", "review operator value handles only"},
    {15, "draft_text_package_comparison_closeout_audit_operator_value_zero_count_profile", "zero-value-only", "review zero operator value counts only"},
    {16, "draft_text_package_comparison_closeout_audit_policy_assertion_lane_profile", "policy-only", "review policy assertion handles only"},
    {17, "draft_text_package_comparison_closeout_audit_review_state_control_lane_profile", "review-state-only", "review state controls without transition"},
    {18, "draft_text_package_comparison_closeout_audit_uncompared_package_rejection_profile", "rejection-control-only", "review uncompared package rejection only"},
    {19, "draft_text_package_comparison_closeout_audit_unacceptable_material_rejection_profile", "rejection-control-only", "review unacceptable material rejection only"},
    {20, "draft_text_package_comparison_closeout_audit_acceptance_grant_rejection_profile", "grant-rejection-only", "review no-grant evidence only"},
    {21, "draft_text_package_comparison_closeout_audit_execution_lock_control_profile", "execution-lock-only", "review execution lock evidence only"},
    {22, "draft_text_package_comparison_closeout_audit_runtime_payload_rejection_profile", "runtime-payload-rejection-only", "review runtime payload lock evidence only"},
    {23, "draft_text_package_comparison_closeout_audit_write_route_rejection_profile", "write-route-rejection-only", "review write route lock evidence only"},
    {24, "draft_text_package_comparison_closeout_audit_sibling_mutation_rejection_profile", "sibling-mutation-rejection-only", "review sibling mutation lock evidence only"},
    {25, "draft_text_package_comparison_closeout_audit_summary_profile", "closeout-only", "review final comparison audit summary only"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(audit_profiles.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageComparisonCloseoutAuditProfile>
signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles() {
    return {audit_profiles.data(), audit_profiles.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profile_count() {
    return static_cast<int>(audit_profiles.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles_json(
    int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& profile = audit_profiles[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(profile.sequence) +
                  ",\"profileCode\":" + json_string(profile.profile_code) +
                  ",\"boundary\":" + json_string(profile.boundary) +
                  ",\"reviewPrompt\":" + json_string(profile.review_prompt) +
                  ",\"readOnly\":true"
                  ",\"draftTextPackageAcceptanceAllowed\":false"
                  ",\"runtimePayloadAllowed\":false"
                  ",\"writeRoutingAllowed\":false"
                  ",\"executionAllowed\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles
