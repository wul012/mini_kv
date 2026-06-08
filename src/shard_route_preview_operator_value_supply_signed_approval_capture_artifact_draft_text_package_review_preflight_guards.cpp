#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards {
namespace {

constexpr std::array<DraftTextPackageReviewPreflightGuard, 25> rejection_controls = {{
    {1, "package_manifest_criterion_control", "draft_text_package_parse", "rejects package manifest material before side effects"},
    {2, "instruction_preflight_digest_criterion_control", "stale_instruction_preflight_digest", "rejects instruction preflight digest material before side effects"},
    {3, "package_schema_version_criterion_control", "unsupported_package_schema", "rejects package schema version material before side effects"},
    {4, "request_correlation_criterion_control", "missing_request_correlation", "rejects request correlation material before side effects"},
    {5, "artifact_digest_binding_criterion_control", "artifact_materialization", "rejects artifact digest binding material before side effects"},
    {6, "template_digest_binding_criterion_control", "draft_text_generation", "rejects template digest binding material before side effects"},
    {7, "review_digest_binding_criterion_control", "approval_statement_text", "rejects review digest binding material before side effects"},
    {8, "detached_signature_envelope_criterion_control", "detached_signature_payload", "rejects detached signature envelope material before side effects"},
    {9, "signature_algorithm_criterion_control", "signature_payload", "rejects signature algorithm material before side effects"},
    {10, "signature_redaction_criterion_control", "raw_signature_material", "rejects signature redaction material before side effects"},
    {11, "source_release_handle_criterion_control", "live_source_lookup", "rejects source release handle material before side effects"},
    {12, "source_file_handle_criterion_control", "runtime_file_walk", "rejects source file handle material before side effects"},
    {13, "source_snippet_handle_criterion_control", "draft_text_assembly", "rejects source snippet handle material before side effects"},
    {14, "operator_identity_handle_criterion_control", "credential_value", "rejects operator identity handle material before side effects"},
    {15, "operator_role_handle_criterion_control", "approval_grant", "rejects operator role handle material before side effects"},
    {16, "redacted_value_digest_criterion_control", "raw_operator_value", "rejects redacted value digest material before side effects"},
    {17, "value_shape_criterion_control", "operator_value_acceptance", "rejects value shape material before side effects"},
    {18, "redaction_policy_criterion_control", "secret_storage", "rejects redaction policy material before side effects"},
    {19, "provenance_policy_criterion_control", "mutable_provenance", "rejects provenance policy material before side effects"},
    {20, "separate_reviewer_control_control", "self_approval", "rejects separate reviewer material before side effects"},
    {21, "unreviewable_package_rejection_control_control", "unreviewable_package_acceptance", "rejects unreviewable package rejection material before side effects"},
    {22, "runtime_payload_rejection_control_control", "runtime_payload_acceptance", "rejects runtime payload rejection material before side effects"},
    {23, "write_route_rejection_control_control", "write_routing", "rejects write route rejection material before side effects"},
    {24, "sibling_non_mutation_control_control", "sibling_project_mutation", "rejects sibling non-mutation material before side effects"},
    {25, "closeout_control", "execution", "rejects archive closeout material before side effects"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(rejection_controls.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageReviewPreflightGuard> signed_approval_capture_artifact_draft_text_package_review_preflight_guards() {
    return {rejection_controls.data(), rejection_controls.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_review_preflight_guard_count() {
    return static_cast<int>(rejection_controls.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_preflight_guards_json(int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& control = rejection_controls[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(control.sequence) +
                  ",\"controlCode\":" + json_string(control.guard_code) +
                  ",\"blocks\":" + json_string(control.blocks) +
                  ",\"reason\":" + json_string(control.reason) +
                  ",\"draftTextPackageAcceptanceBlocked\":true"
                  ",\"runtimeExecutionBlocked\":true"
                  ",\"writeRouteBlocked\":true"
                  ",\"siblingMutationBlocked\":true}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards