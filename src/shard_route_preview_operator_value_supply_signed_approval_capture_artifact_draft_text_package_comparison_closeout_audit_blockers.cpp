#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_blockers.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_blockers {
namespace {

constexpr std::array<DraftTextPackageComparisonCloseoutAuditBlocker, 25> audit_blockers = {{
    {1, "source_freeze_audit_blocker", "rolling_current_as_source", "requires v835 frozen fixture before comparison evidence"},
    {2, "release_range_audit_blocker", "unbounded_comparison_range", "blocks unbounded comparison preflight ranges"},
    {3, "comparison_lane_catalog_sequence_audit_blocker", "non_contiguous_comparison_lane_sequence", "blocks non-contiguous comparison lane claims"},
    {4, "identity_lane_audit_blocker", "identity_payload_parse", "blocks identity payload parsing"},
    {5, "request_correlation_lane_audit_blocker", "raw_request_payload_import", "blocks raw request payload import"},
    {6, "artifact_digest_binding_lane_audit_blocker", "missing_artifact_digest_binding", "blocks package acceptance without artifact digest binding"},
    {7, "template_digest_binding_lane_audit_blocker", "missing_template_digest_binding", "blocks package acceptance without template digest binding"},
    {8, "review_digest_recheck_control_audit_blocker", "unrechecked_review_digest", "blocks accepting unrechecked review digests"},
    {9, "detached_signature_envelope_lane_audit_blocker", "signature_payload_parse", "blocks detached signature payload parsing"},
    {10, "signature_metadata_lane_audit_blocker", "raw_signature_material", "blocks raw signature material storage"},
    {11, "signature_payload_parse_rejection_audit_blocker", "detached_signature_payload_parse", "blocks detached signature payload parsing"},
    {12, "source_evidence_handle_lane_audit_blocker", "source_file_read", "blocks source file reads during comparison audit"},
    {13, "source_snippet_digest_lane_audit_blocker", "source_snippet_materialization", "blocks source snippet materialization"},
    {14, "operator_value_handle_lane_audit_blocker", "operator_value_import", "blocks operator value import"},
    {15, "operator_value_zero_count_audit_blocker", "operator_value_persistence", "blocks submitted accepted imported or persisted values"},
    {16, "policy_assertion_lane_audit_blocker", "policy_assertion_acceptance", "blocks policy assertion acceptance without manual review"},
    {17, "review_state_control_lane_audit_blocker", "review_state_transition", "blocks review state transitions"},
    {18, "uncompared_package_rejection_audit_blocker", "uncompared_package_acceptance", "blocks acceptance of uncompared package material"},
    {19, "unacceptable_material_rejection_audit_blocker", "unacceptable_package_material", "blocks unacceptable package material acceptance"},
    {20, "acceptance_grant_rejection_audit_blocker", "approval_grant_emission", "blocks approval grant emission"},
    {21, "execution_lock_control_audit_blocker", "runtime_execution", "blocks runtime execution"},
    {22, "runtime_payload_rejection_audit_blocker", "runtime_payload_acceptance", "blocks runtime payload acceptance"},
    {23, "write_route_rejection_audit_blocker", "write_route_activation", "blocks write route activation"},
    {24, "sibling_mutation_rejection_audit_blocker", "sibling_mutation", "blocks Java or Node sibling mutation"},
    {25, "comparison_closeout_summary_blocker", "post_closeout_execution", "blocks any execution after comparison closeout"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(audit_blockers.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageComparisonCloseoutAuditBlocker>
signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_blockers() {
    return {audit_blockers.data(), audit_blockers.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_blocker_count() {
    return static_cast<int>(audit_blockers.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_blockers_json(
    int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& blocker = audit_blockers[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(blocker.sequence) +
                  ",\"blockerCode\":" + json_string(blocker.blocker_code) +
                  ",\"blocks\":" + json_string(blocker.blocks) +
                  ",\"reason\":" + json_string(blocker.reason) +
                  ",\"failClosed\":true"
                  ",\"draftTextPackageAcceptanceBlocked\":true"
                  ",\"runtimeExecutionBlocked\":true"
                  ",\"writeRouteBlocked\":true}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_blockers
