#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots.hpp"

#include "minikv/runtime_evidence.hpp"

#include <algorithm>
#include <array>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots {
namespace {

constexpr std::array<ComparedPackageEvidenceIntakeSlot, 10> compared_package_evidence_intake_slots = {{
    {1, "source_acceptance_precheck_evidence", "Node v1321 acceptance precheck checkpoint and guard summary", "submitted compared package payload"},
    {2, "manual_submission_reference_evidence", "manual submission reference handle and operator-owned artifact pointer", "raw endpoint credential or package body"},
    {3, "offline_comparison_result_evidence", "offline comparison result handle and comparison outcome marker", "computed comparison execution"},
    {4, "identity_binding_evidence", "operator identity binding handle and request correlation marker", "identity secret or privilege grant"},
    {5, "digest_match_summary_evidence", "digest match summary handle and recheck marker", "payload bytes or signed approval"},
    {6, "detached_signature_envelope_observation_evidence", "detached signature envelope observation metadata", "signature payload parse or verification result"},
    {7, "source_and_operator_value_handle_evidence", "source evidence handle and zero operator value import marker", "operator value content"},
    {8, "policy_and_execution_lock_evidence", "policy review handle and execution lock marker", "runtime enablement"},
    {9, "approval_grant_separation_evidence", "approval grant review separation handle", "approval grant capture"},
    {10, "archive_closeout_evidence", "archive closeout handle and final guard marker", "runtime archive walk or sibling mutation"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const ComparedPackageEvidenceIntakeSlot> signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots() {
    return {compared_package_evidence_intake_slots.data(), compared_package_evidence_intake_slots.size()};
}

int planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slot_count() {
    return static_cast<int>(compared_package_evidence_intake_slots.size());
}

std::string format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots_json(
    int completed_slot_count) {
    const int visible_count = std::clamp(completed_slot_count, 0, planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slot_count());
    std::string json = "[";
    for (int index = 0; index < visible_count; ++index) {
        const auto& slot = compared_package_evidence_intake_slots[static_cast<std::size_t>(index)];
        if (index > 0) {
            json += ",";
        }
        json += "{\"sequence\":" + std::to_string(slot.sequence) +
                ",\"slotCode\":" + json_string(slot.slot_code) +
                ",\"expectedEvidence\":" + json_string(slot.expected_evidence) +
                ",\"absentMaterial\":" + json_string(slot.absent_material) +
                ",\"readOnly\":true"
                ",\"payloadAccepted\":false}";
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots