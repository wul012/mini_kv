#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks {
namespace {

constexpr std::array<DraftTextPackageComparisonCloseoutAuditCheck, 25> audit_checks = {{
    {1, "source_freeze_audit", "v835 frozen source", "v835 fixture is frozen before comparison closeout audit starts"},
    {2, "release_range_audit", "Node v1287-v1311 comparison range", "comparison preflight closeout range is visible and bounded"},
    {3, "comparison_lane_catalog_sequence_audit", "comparison lane sequence", "comparison lane sequence remains contiguous"},
    {4, "identity_lane_audit", "identity lane", "identity lane is cataloged as handle-only"},
    {5, "request_correlation_lane_audit", "request correlation lane", "request correlation lane is cataloged as metadata-only"},
    {6, "artifact_digest_binding_lane_audit", "artifact digest binding", "artifact digest binding lane is cataloged"},
    {7, "template_digest_binding_lane_audit", "template digest binding", "template digest binding lane is cataloged"},
    {8, "review_digest_recheck_control_audit", "review digest recheck", "review digest recheck control is cataloged"},
    {9, "detached_signature_envelope_lane_audit", "detached signature envelope", "detached signature envelope lane is cataloged"},
    {10, "signature_metadata_lane_audit", "signature metadata", "signature metadata lane is cataloged as metadata-only"},
    {11, "signature_payload_parse_rejection_audit", "signature payload parser", "signature payload parsing remains rejected"},
    {12, "source_evidence_handle_lane_audit", "source evidence handle", "source evidence handle lane is cataloged"},
    {13, "source_snippet_digest_lane_audit", "source snippet digest", "source snippet digest lane is cataloged"},
    {14, "operator_value_handle_lane_audit", "operator value handle", "operator value handle lane is cataloged"},
    {15, "operator_value_zero_count_audit", "operator values", "operator value counts remain zero"},
    {16, "policy_assertion_lane_audit", "policy assertions", "policy assertion lane is cataloged"},
    {17, "review_state_control_lane_audit", "review state controls", "review state control lane is cataloged"},
    {18, "uncompared_package_rejection_audit", "uncompared packages", "uncompared packages remain rejected"},
    {19, "unacceptable_material_rejection_audit", "unacceptable material", "unacceptable material remains rejected"},
    {20, "acceptance_grant_rejection_audit", "acceptance grant", "acceptance grant remains absent"},
    {21, "execution_lock_control_audit", "execution lock", "execution lock remains closed"},
    {22, "runtime_payload_rejection_audit", "runtime payload", "runtime payload acceptance remains rejected"},
    {23, "write_route_rejection_audit", "write routing", "write routing remains rejected"},
    {24, "sibling_mutation_rejection_audit", "sibling mutation", "sibling mutation remains rejected"},
    {25, "comparison_closeout_summary", "comparison closeout summary", "comparison audit closes without accepting package material"},
}};

int clamp_count(int count) {
    if (count < 0) {
        return 0;
    }
    const int total = static_cast<int>(audit_checks.size());
    return count > total ? total : count;
}

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const DraftTextPackageComparisonCloseoutAuditCheck>
signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks() {
    return {audit_checks.data(), audit_checks.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_check_count() {
    return static_cast<int>(audit_checks.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks_json(
    int published_stage_count) {
    std::string output = "[";
    const int count = clamp_count(published_stage_count);
    for (int index = 0; index < count; ++index) {
        const auto& check = audit_checks[static_cast<std::size_t>(index)];
        if (index != 0) {
            output += ",";
        }
        output += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"auditedArea\":" + json_string(check.audited_area) +
                  ",\"expectedEvidence\":" + json_string(check.expected_evidence) +
                  ",\"auditCheckPrepared\":true"
                  ",\"readsRuntimeArchive\":false"
                  ",\"parsesDraftTextPackage\":false"
                  ",\"acceptsDraftTextPackage\":false"
                  ",\"mutatesSiblingState\":false}";
    }
    output += "]";
    return output;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks
