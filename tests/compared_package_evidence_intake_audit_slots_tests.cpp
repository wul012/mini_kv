#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace slots = minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots;
    const auto records = slots::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots();
    assert(records.size() == 10);
    assert(records.front().slot_code == std::string{"source_acceptance_precheck_evidence"});
    assert(records.back().slot_code == std::string{"archive_closeout_evidence"});
    assert(slots::planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slot_count() == 10);
    const auto first = slots::format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots_json(1);
    assert_contains(first, "\"slotCode\":\"source_acceptance_precheck_evidence\"");
    assert(first.find("\"slotCode\":\"manual_submission_reference_evidence\"") == std::string::npos);
    const auto all = slots::format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_slots_json(99);
    assert_contains(all, "\"slotCode\":\"archive_closeout_evidence\"");
    assert_contains(all, "\"payloadAccepted\":false");
}